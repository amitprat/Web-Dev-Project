<?php
/** 
* PHPSimpleChat - Common functions
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
* 
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.1
*/

function getActiveUsers($roomID) {
	//Find all users active within interval
	$interval = time() - PHPSC_USER_EXPIRE_INTERVAL;
	$users = array();
	$sql = "SELECT DISTINCT usr FROM chat_users WHERE roomid = $roomID AND lastaction > $interval";
	if ($result = dbQuery($sql)) {
		while ($row = dbFetchArray($result)) {
			$users[]=$row["usr"];
		}
	}
	return $users;
}

function getRecentChats($roomID, $startID) {
	$chats = array();
	$sql = "SELECT msg, usr, stamp FROM chats WHERE roomid = $roomID AND id >= $startID ORDER BY id DESC LIMIT " . PHPSC_RECENT_CONVO_COUNT;
	if ($result = dbQuery($sql)) {
		while ($row = dbFetchArray($result)) {
			$msg = $row["msg"];
			$user = $row["usr"];
			$timestamp = $row["stamp"];
			$chats[] = array($user, $msg, $timestamp);
		}
	}
	if (count($chats)>0) {
		$chats = formatChats($chats);
		$chats = array_reverse($chats);
	}
	return $chats;
}

function formatChats($input) {
	foreach ($input as $value) {
		$formattedDate = (PHPSC_DISPLAY_TIMESTAMP == true) ? ' (' . date('h:i:s A', $value[2]) . '):' : ':';
		$username = $value[0];
		$message = $value[1];
		if(!empty($username)) {
			//regular user chat entry
			$chats[] = "<span class=\"chat_usr $username\">$username</span><span class=\"chat_dt $username\">" . $formattedDate . "</span> " . $message;
		}
		else {
			//this is a system message
			$chats[] = $message;
		}
	}
	return $chats;
}


function garbageCollection() {
	//clean up old chats
	$interval = time() - PHPSC_GARBAGE_EXPIRATION;
	$sql = "DELETE FROM chat_invites WHERE requesttime < $interval";
	$result = dbQuery($sql);

	//clean up private invites chats
	$sql = "DELETE FROM chats WHERE stamp < $interval";
	$result = dbQuery($sql);
}

//Takes an array of usernames and then readies them for private chats by adding in html links
function addPrivateChatLinks($users, $yourUsername = '', $image) {
	$resultArray = array();
	$privateChatIcon = $image;
	foreach ($users as $username ) {
		if ($username != $yourUsername) {
			$resultArray[] = $username . " <span class=\"private_chat\"><a href=\"?private=true&user=$username\" target=\"_blank\"><img id=\"private_chat_icon\" src=\"$privateChatIcon\" alt=\"Private chat with this user\"></a></span>";
		}
	}
	return $resultArray;
}

function getPrivateInvites($username) {
	//By default we look for private chat invites within interval
	$interval = time() - PHPSC_PRIVATE_INVITE_EXPIRE;
	$resultArray = array();
	$sql = "SELECT * FROM chat_invites WHERE targetuser = '$username' AND requesttime > $interval AND joined = 0";
	if ($result = dbQuery($sql)) {
		while ($row = dbFetchArray($result)) {
			$user = $row["usr"];
			$roomID = $row["roomid"];
			$ID = $row["id"];
			$resultArray[] = array('user'=>$user, 'roomID'=>$roomID, 'ID'=>$ID);
		}
		return $resultArray;
	}
	return false;
}

function ignorePrivateChat($id) {
	$sql = "UPDATE chat_invites SET joined = 1 WHERE id = $id";
	$result = dbQuery($sql);
}

function getData($var, $maxLength = 99999) {
  $value = NULL;
  if (isset($_GET[$var])) {
    $value = htmlentities(trim($_GET[$var]), ENT_QUOTES);
    if (strlen($value) > $maxLength) {
      $value = substr($value, 0, $maxLength);
    }
  }
  return $value;
}
 
function postData($var, $maxLength = 99999) {
  $value = NULL;
  if (isset($_POST[$var])) {
    $value = htmlentities(trim($_POST[$var]), ENT_QUOTES);
    if (strlen($value) > $maxLength) {
      $value = substr($value, 0, $maxLength);
    }
  }
  return $value;
}

function setUserActive($chatRoom, $username) {
	$sql="UPDATE chat_users SET lastaction = " . time() . " WHERE roomid = $chatRoom AND usr = '$username'";
	$result = dbQuery($sql);
}

function insertChat($username, $roomID, $message, $identifier) {

	if (!empty($message) && is_numeric($roomID)) {
		$now = time();
		$sql = "INSERT INTO chats (msg, roomid, identifier, usr, stamp) VALUES ('$message', $roomID, '$identifier', '$username', $now)";
		$result = dbQuery($sql);
		if (dbLastID('chats') % PHPSC_GARBAGE_INTERVAL == 0) {
			//we are due for garbage collection
			garbageCollection();
		}
	}
}

function getBase($inputFile, $fromBase = '') {
	//returns relative path from the script to our input file
	$fromFile = !empty($fromBase) ? $fromBase : $_SERVER['SCRIPT_FILENAME'];
	$fromFile = str_replace('\\', '/', $fromFile);

	//actually use path from our config file which = path from PHPSimpleChat.php
	$startingBase = PHPSC_GLOBAL_BASE;
	$startingBase = str_replace('\\', '/', $startingBase);
	$toFile = dirname($startingBase) . $inputFile;

	//standardize everything to forward slashes
	$separator = '/';
	$fromFile = str_replace('\\', $separator, $fromFile);
	$toFile = str_replace('\\', $separator, $toFile);

	$fromParts = explode($separator, $fromFile);
	$toParts   = explode($separator, $toFile);

	$sharedParts = 0;
	while (strcmp($fromParts[$sharedParts], $toParts[$sharedParts]) == 0) { 
		$sharedParts++; 
	}

	//last item is filename so subtract 1
	$foldersDown = count($fromParts) - $sharedParts - 1;
	$foldersUp = count($toParts) - $sharedParts -1;

	$relativePathDown = '';
	$relativePathUp = '';

	if ( $foldersDown == 0 && $foldersUp == 0 ) {}
	else if ( $foldersDown == 0 && $foldersUp != 0 ) {
		$relativePathDown = $separator;
	}
	else {
		for ($i = 0; $i < $foldersDown; $i++) {
			$relativePathDown .= '..' . $separator;
		}
	}

	$toFinalDir = $sharedParts + $foldersUp;
	for ($i = $sharedParts; $i < $toFinalDir; $i++) {
		$relativePathUp .= $toParts[$i] . $separator;
	}

	$relativePathUp .= $toParts[$toFinalDir];
	$path = $relativePathDown . $relativePathUp;

	//remove first / char from path
	$first = substr($path, 0, 1);
	if ($first == '/') {
		return substr($path, 1);
	}
	else {
		return $path;
	}
}