<?php

error_reporting(E_ERROR | E_PARSE);

//connect to DB and include general chat functions
include_once('config.php');

class PHPSimpleChat{

	var $username;
	var $smallestID;
	var $now;
    var $private;
    var $privateUser;
    var $privateChatID;
	var $chatRoom;
	var $chatTitle;
	var $recentConvosCount;
	var $roomIdentifier;
	var $expiredUserInterval;
	var $saveRecentConvos;
	var $joinMessageExpiration;
	var $privateInviteExpiration;
	var $phpsimplechatVersion;

	function PHPSimpleChat($startingUser = NULL, $roomIdentifier = NULL) {
		session_start();

		$this->roomIdentifier = isset($roomIdentifier) ? substr($roomIdentifier, 0, 31) : NULL;
		$this->initVariables();
		$this->setUser($startingUser);
		$this->dropExpiredUsers();
		$this->setRoom();
		
		if ($this->saveRecentConvos) {
			$this->setStartingChats();
		}
		else {
			$this->setStartingChatID();
		}

		$this->activateUser();

		//Get list of active users
		$this->users = getActiveUsers($this->chatRoom);

		$privateChatIcon = getBase('/images/icon_private_chat.png');
		$this->users = addPrivateChatLinks($this->users, $this->username, $privateChatIcon);
		//remove you from list
		array_shift($this->users);
	}

    function initVariables() {

		$this->phpsimplechatVersion = '0.2';

		$this->saveRecentConvos = PHPSC_SAVE_CONVOS;
		$this->recentConvosCount = PHPSC_RECENT_CONVO_COUNT_LOAD;
		$this->expiredUserInterval = PHPSC_USER_EXPIRE_INTERVAL;
		$this->joinMessageExpiration = PHPSC_JOIN_MESSAGE_EXPIRE;
		$this->privateInviteExpiration = PHPSC_PRIVATE_INVITE_SEND_EXPIRE;

		$this->username = NULL;
		$this->smallestID = 0;
		$this->now = time();
		$this->chatText = NULL;

		$this->chatRoom = getData('room');
		$this->privateUser = getData('user');
		$this->privateChatID = getData('ID');

		$priv = getData('private');
		$this->private = empty($priv) ? false : true;
	}

	function setUser($startingUser = '') {
		if (isset($_SESSION['user']) && !empty($_SESSION['user'])) {
			$this->username = $_SESSION['username'];
		}
		else {
			$this->username = !empty($startingUser) ? substr(htmlentities($startingUser, ENT_QUOTES), 0, 31) : 'Guest_' . rand(1, 99999);
			$_SESSION['user'] = $this->username;
		}
	}

	function setStartingChats() {
		$chats = array();
		$smallestID = NULL;
		//Customized chats call because we want to alter the limit and pull the smallestID
		$sql = "SELECT id, msg, stamp, usr FROM chats WHERE roomid = " . $this->chatRoom . 
			" ORDER BY id DESC LIMIT " . $this->recentConvosCount;
		if( $result = dbQuery($sql)) {
			while ($row = dbFetchArray($result)) {
				$smallestID = $row["id"];
				$msg = $row["msg"];
				$user = $row["usr"];
				$timestamp = $row["stamp"];
				$chats[] = array($user, $msg, $timestamp);
			}
		}

		$this->smallestID = (is_numeric($smallestID)) ? $smallestID : 0;

		if (count($chats) > 0) {
			$chats = formatChats($chats, $this->username);
			$chats = array_reverse($chats);
			$this->chatText = implode("<br/>", $chats);
		}
	}

	function setStartingChatID() {
		$sql = "SELECT MAX(id) as id FROM chats WHERE roomid = " . $this->chatRoom;
		if ($result = dbQuery($sql)) {
			while ($row = dbFetchArray($result)) {
				$this->smallestID = $row['id'];
			}
		}
	}

	function dropExpiredUsers() {
		$interval = time() - $this->expiredUserInterval;
		$sql = "DELETE FROM chat_users WHERE lastaction < $interval";
		$result = dbQuery($sql);
	}

	function activateUser() {
		//Now activate this user by inserting them into the chat_users table or updating them if they already exist
		//inserts will fail because there is a unique composite key over the roomID and user fields
		$sql = "INSERT INTO chat_users (roomid, usr, lastaction) 
			VALUES (" . $this->chatRoom.", '" . $this->username . "', " . $this->now . ")";
		if (!$result = dbQuery($sql)) {
			$sql="UPDATE chat_users SET lastaction = " . $this->now . 
				" WHERE roomid = " . $this->chatRoom . 
				" AND usr = '" . $this->username . "'";
			$result = dbQuery($sql);
		}
	}

	function getAvailableRoom() {
		$maxID = NULL;
		if (empty($this->roomIdentifier)) {
			//if no args, use the default room 1
			$this->roomIdentifier = 'phpsimplechat_main';
			$maxID = 1;
		}
		else {
			//search for existing chat ID with this identifier
			$sql = "SELECT MAX(roomid) as id FROM chats WHERE identifier LIKE '" . $this->roomIdentifier . "'";
			$result = dbQuery($sql);
			while ($row = dbFetchArray($result)) {
				$max = $row['id'];
			}

			if (!is_numeric($maxID)) {
				//if not numeric, we had no matches, so instead use the largest ID available
				$maxID = $this->getMaxRoomID();

				if (!is_numeric($maxID)) {
					//we still haven't found a valid id, use id=1
					$maxID = 1;
				}
				else{
					//we want to use next available after max
					$maxID++;
				}
			}
		}
		return $maxID;
	}
	
	function getMaxRoomID() {
		$max = NULL;
		$sql = "SELECT MAX(roomid) as id FROM chats";
		if($result = dbQuery($sql)){
			while ($row = dbFetchArray($result)) {
				$max = $row['id'];
			}
		}
		return $max;
	}

	function setRoom() {
		
		if (!$this->private) {
			$this->chatRoom = $this->getAvailableRoom();
			
			if (!$this->hasJoinedRoom($this->chatRoom)) {
				//check to see if has joined room, to avoid extra join msgs
				$this->initChatroom($this->username, $this->chatRoom);
				$this->joinRoom($this->chatRoom);
			}
			$groupChatIcon = '<img id="room_icon" src="' . getBase('/images/group.png') . '">';
			$this->chatTitle = $groupChatIcon . ' Group Chat';
		}
		else if ($this->private) {
			$private_icon = '<img id="room_icon" src="' . getBase('/images/lock.png') . '">';
			$this->roomIdentifier = NULL;

			if(is_numeric($this->chatRoom))  {
				//we are joining a room we have been invited to
				if (!$this->hasJoinedRoom($this->privateChatID)) {
					$this->initChatroom($this->username, $this->chatRoom);
					$this->joinRoom($this->privateChatID);
				}
			}
			else if (!$this->privateInviteSent($this->privateUser)) {
				//we are joining a new room because we invited someone to a private chat
				//no need to reuse a particular room, we just need a new one
				$maxID = $this->getMaxRoomID();
				//If we cannot find a valid room greater that our max, set ID = 2
				$this->chatRoom = is_numeric($maxID) ? $maxID + 1 : 2;
				$this->initChatroom($this->username, $this->chatRoom);
				$this->sendPrivateInvite($this->privateUser, $this->username, $this->now, $this->chatRoom);	
			}
			$this->chatTitle = $private_icon . ' Private chat between ' . $this->privateUser . ' and ' . $this->username;
		}
	}
	
	function initChatroom($user, $roomID) {
		//initialize this chat room with a system message
		$join_message = $user . " has joined this chatroom";
		$msg = "<span class=\"chat_sys\">$join_message</span>";
		$now = time();
		//dont insert identifier, this is a private chat
		$sql = "INSERT INTO chats (msg, roomid, identifier, stamp, usr) VALUES ('$msg', $roomID, '" . $this->roomIdentifier . "', $now, '')";
		$result = dbQuery($sql);
	}

	function joinRoom($id = NULL) {
		//when called in private chat context, id refers to the id of the chat invite
		//when called in group chat context, id refers to roomID
		if ($this->private) {
			if (is_numeric($id)) { 
				//need to also update db to prevent another request for this private chat to be sent
				$sql = "UPDATE chat_invites SET joined = 1 WHERE id = $id";
				$result = dbQuery($sql);
				$_SESSION['joined_pvt_' . $id] = time();
			}
		}
		else {
			$_SESSION['joined_' . $id] = time();
		}
	}

	function hasJoinedRoom($room_ID) {
		//see if we have joined this chat within the last 1 min
		//prevents excessive join messages
		if ($this->private) {
			if (isset($_SESSION['joined_pvt_' . $room_ID])) {
				$timeInvited = $_SESSION['joined_pvt_' . $room_ID];
				if (time() <= $timeInvited + $this->joinMessageExpiration) {
					return true;
				}
			}	
		}
		else {
			if (isset($_SESSION['joined_' . $room_ID])) {
				$timeInvited = $_SESSION['joined_' . $room_ID];
				if (time() <= $timeInvited + $this->joinMessageExpiration) {
					return true;
				}
			}
		}
		return false;
	}

	function privateInviteSent($privateUser) {
		//see if we have sent a private chat request within the last 3 mins
		if (isset($_SESSION['invite_' . $privateUser])) {
			$session_vars = unserialize($_SESSION['invite_' . $privateUser]);
			$timeInvited = $session_vars['time'];

			if (time() <= $timeInvited + $this->privateInviteExpiration) {
				$this->chatRoom = $session_vars['room_ID'];
				return true;
			}
		}
		return false;
	}

	function sendPrivateInvite($to, $from, $timestamp, $roomID) {
		//insert chat invite so other user will see
		$sql = "INSERT INTO chat_invites (targetuser, usr, requesttime, roomid) VALUES ('$to', '$from', $timestamp, $roomID)";
		if ($result = dbQuery($sql)) {
			$_SESSION['invite_' . $to] = serialize(array('time' => $timestamp, 'room_ID' => $roomID));
		}
	}
	
	function displayChat() {
		$output = $this->setupJS();
		$output .= $this->setupHTML();
		echo $output;
	}

	function setupJS(){
		$output = '<script type="text/javascript" src="'.  getBase('/ajax/global.js') . '"></script>';
		$output .= '<script type="text/javascript">' .
			"var requestBase = '" . urlencode($_SERVER['SCRIPT_FILENAME']) . "'; " .
			"var username = '" . $this->username . "'; " .
			"var startID = " . $this->smallestID . "; " .
			"var roomID = " . $this->chatRoom . "; " .
			"var identifier = '" . $this->roomIdentifier . "'; " .
			"var private_invite_script = '" . getBase('/ajax/ajax_privateinvite.php') . "'; " .
			"var chat_update_script = '" . getBase('/ajax/ajax_chatupdate.php') . "'; " .
			"var users_update_script = '" . getBase('/ajax/ajax_usersupdate.php') . "'; " .
			"var chatReloadTime = '" . PHPSC_CHAT_INTERVAL. "'; " .
			"var idleModeReloadTime = '" . PHPSC_CHAT_IDLE_INTERVAL . "'; " .
			"var userActiveReloadTime = '" . PHPSC_USER_ACTIVE_INTERVAL . "'; " .
			"var userUpdateReloadTime = '" . PHPSC_USER_UPDATE_INTERVAL . "'; " .
			"var privateInviteReloadTime = '" . PHPSC_PRIVATE_CHAT_INTERVAL . "'; " .
			"var idleThreshold = '" . PHPSC_IDLE_THRESHOLD . "'; " ."</script>";
		$output .= '<script type="text/javascript" src="'.  getBase('/ajax/functions.js') . '"></script>';
		return $output;
	}

	function setupHTML() {
		$output = '<div id="div_chat_outer" class="phpsimplechat"><table id="table_chat_outer" class="phpsimplechat"><tr><td colspan="2">' .
			'<div id="div_chat_header" class="phpsimplechat">' . $this->chatTitle . '</div></td></tr>' .
			'<tr><td><div id="div_chat_window" class="phpsimplechat">' . $this->chatText . '</div>' . 
			'<div id="div_chat_input" class="phpsimplechat"><input type="text" size="70" maxlength="100" id="message" onkeyup="return onEnter(event);" /> ' .
			'<input type="button" value="Send" id="message" onClick="checkChatInsertUpdates(); clearMessage(); return false;"/></div>' . 
			'</td><td><div id="div_chat_right_container" class="phpsimplechat"><div id="div_chat_private" class="phpsimplechat">' .
			'<img id="private_chat_icon" src="' . getBase('/images/icon_private_chat_green.png') . '" alt="Private chat with this user">' . 
			'<b> Private Chat Invites</b><div id="div_chat_private_invites" class="phpsimplechat"></div></div><strong>You:</strong><br>' .
			'<div id="div_current_user" class="phpsimplechat">' . $this->username . '</div>' . 
			'<strong>All Users:</strong><div id="div_chat_users" class="phpsimplechat">' . implode($this->users, '<br>') . '</div></div>' .
			'<div id="div_attribution" class="phpsimplechat">' . $this->getAttribution() . 
			'<span id="span_version">' . $this->phpsimplechatVersion . '</span></div>' .
			'</td></tr></table></div>';
		return $output;
	}

	function displayHeader() {
		$output = $this->setupCSS();
		echo $output;
	}

	function setupCSS() {
		//need dynamic css to for user style handling
		$output = '<link href="' . getBase('/css/styles.css') . '" rel="stylesheet" type="text/css" media="all" />';
		$output .= '<style type="text/css"> .' . $this->username . ' { color: #FF0000; }';
		$output .= '</style>';
		return $output;
	}

	function getAttribution() {
		return '<a href="http://www.ebrueggeman.com" target="_blank">' . '
			<img id="phpfreechat_icon" src="' . getBase('/images/phpsimplechat_logo.png') . 
			'" width="112" height="27" alt="This chat is powered by PHPSimpleChat"></a>';
	}
}