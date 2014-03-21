/**
* PHPSimpleChat - Javascript functions relating to AJAX functionality
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
*
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.1
*/

var idleCount = 0;

//Timer initialization
var timer;
var userUpdateTimer;
var userActiveTimer;
var privateInviteTimer;

var idleMode = ''; //becomes a boolean after first call
var lastMessages = '';

var ignoreID;

//Start timers
startNormalTimer();
startUserActiveTimer();
startUserUpdateTimer();
startPrivateInviteTimer();

//do an initial check on active users
checkUserActive();

/*********************************************
	USER IS PRESENT RELATED UPDATE
	[A user stays in a room time and then 
	has their last action time updated]
*********************************************/
function startUserUpdateTimer() {
	var interval = userUpdateReloadTime * 1000;
	userUpdateTimer = setInterval("checkUserUpdate()", interval); 
}

function checkUserUpdate() {
	ajaxHelper('user_update', 'ajax');
}

function user_update_init() {
	var script = users_update_script;
	var queryString = "?room=" + roomID + "&usr=" + username + "&arg=set" + '&base=' + requestBase;
	return script + queryString;
}

//No Input to page, only DB changes, so nothing needed here
function user_update_ajax(myvar) {}

/*********************************************
	ACTIVE USERS LIST UPDATE
	[A list of active users is retrieved]
*********************************************/
function startUserActiveTimer() {
	var interval = userActiveReloadTime * 1000;
	userActiveTimer=setInterval("checkUserActive()", interval); 
}

function checkUserActive() {
	ajaxHelper('user_active', 'ajax');
}

function user_active_init() {
	var script = users_update_script;
	var queryString = "?room=" + roomID + "&usr=" + username + "&arg=get" + '&base=' + requestBase;
	return script + queryString;
}

function user_active_ajax(myvar) {
	var chatusers = document.getElementById('div_chat_users');
	var currentuser = document.getElementById('div_current_user');
	//JS Logic
	var messageArray = myvar.split(',');
	var resultHTML='';

	for (var i=0; i < messageArray.length; i++) {
		resultHTML = resultHTML + (messageArray[i]+'<br/>')
	}
	chatusers.innerHTML = resultHTML;
}

/*********************************************
	CHAT MESSAGE SENT UPDATE
	[A user sends a chat message to the chat
	room and it is inserted into the DB]
*********************************************/
function onEnter(evt) {
	//On Enter, call 
	var keyCode = null;
	
	if( evt.keyCode ) {
		keyCode = evt.keyCode;
	}
	/* KEY CODE FOR ENTER = 13*/
	if( 13 == keyCode ) {
		checkChatInsertUpdates();
		clearMessage();
		return false;
	}
	return true;
}

function checkChatInsertUpdates() {
	ajaxHelper('chat_insert_update', 'ajax');
}

function chat_insert_update_init() {
	var message = document.getElementById('message');
	var script = chat_update_script;

	if (message.value) {
		var dateObject = new Date();
		var date = padDigits(dateObject.getMonth(),2) +'/'+ padDigits(dateObject.getDay(),2) +'/'+dateObject.getFullYear();
		var time = padDigits(dateObject.getHours(),2) +':'+ padDigits(dateObject.getMinutes(),2) +':'+ padDigits(dateObject.getSeconds(),2);  
		var userDateString = username + ' (' + date + ' ' + time + '): ';
		var queryString = "?msg=" + message.value + "&startID=" + startID + "&usr=" + username + "&room=" + roomID + "&iden=" + identifier + '&base=' + requestBase;
		return script + queryString;
	}
	else {
		return null;
	}
	
}

function chat_insert_update_ajax(myvar) {
	var chatwindow = document.getElementById('div_chat_window');
	//JS Logic
	var messageArray = myvar.split(',');
	var resultHTML='';
	for (var i=0; i < messageArray.length; i++) {
		resultHTML = resultHTML + (messageArray[i]+'<br/>')
	}

	//RESET IDLE COUNT
	startNormalTimer();
	checkIdle();
	
	chatwindow.innerHTML = resultHTML;
	scrollChatWindow();
}

/*********************************************
	CHAT MESSAGES UPDATE
	[A list of recent chats is retrieved 
	from the DB and displayed]
*********************************************/
function checkChatUpdates() {
	ajaxHelper('chat_update', 'ajax');
}
function startNormalTimer() {
	if (idleMode==true || idleMode=='') {
		idleCount = 0;
		idleMode = false;
		clearInterval(timer);
		var interval = chatReloadTime * 1000;
		timer=setInterval("checkChatUpdates()", interval); 
	}
}
function checkIdle() {
	if (idleCount>idleThreshold & idleMode==false) {
		idleMode = true;
		clearInterval(timer);
		var interval = idleModeReloadTime * 1000;
		timer=setInterval("checkChatUpdates()", interval);
	}
}
function chat_update_init() {
	var script = chat_update_script;
	var queryString = "?startID=" + startID + "&room=" + roomID + '&base=' + requestBase;
	return script + queryString;
}
function chat_update_ajax(myvar) {
	var chatwindow = document.getElementById('div_chat_window');
	//JS Logic
	var messageArray = myvar.split(',');
	var resultHTML = '';
	for (var i=0; i < messageArray.length; i++) {
		resultHTML = resultHTML + (messageArray[i]+'<br/>')
	}

	//IDLE CHECK
	if (lastMessages == resultHTML) {
		idleCount++;
	}
	else {
		startNormalTimer();
	}
	checkIdle();

	lastMessages = resultHTML;
	chatwindow.innerHTML=resultHTML;
	scrollChatWindow();
}

/**********************************************
        PRIVATE CHAT AJAX CALLS
**********************************************/
function startPrivateInviteTimer() {
	var interval = privateInviteReloadTime * 1000;
	privateInviteTimer=setInterval("checkPrivateChat()", interval); 
}

function checkPrivateChat() {
	ajaxHelper('private_invite', 'ajax');
}

function private_invite_init() {
	var script = private_invite_script;
	var queryString = "?username=" + username + '&base=' + requestBase;
	return script + queryString;
}

function private_invite_ajax(myvar) {
	var privatechat = document.getElementById('div_chat_private');
	var privatechatinvites = document.getElementById('div_chat_private_invites');

	if (myvar && myvar!='*') {
		privatechat.style.display = 'block';
		privatechatinvites.innerHTML = myvar;
	}
	else {
		privatechat.style.display = 'none';
	}
}

function private_invite_ignore(id) {
	ignoreID = id;
	ajaxHelper('private_invite_ignore', 'ajax');
}

function private_invite_ignore_ajax(myvar) {
	private_invite_ajax(myvar);
}

function private_invite_ignore_init() {
	var script = private_invite_script;
	var queryString = "?ignore=" + ignoreID + '&username=' + username + '&base=' + requestBase;
	return script + queryString;
}

/*********************************************
	FORM RELATED FUNCTIONS
*********************************************/
function clearMessage() {
	var message = document.getElementById('message');
	message.value = '';
	focusMessage();	
}
function focusMessage() {
	var message = document.getElementById('message');
	message.focus();
}
function scrollChatWindow() {
	var chatwindow = document.getElementById('div_chat_window');
	chatwindow.scrollTop = chatwindow.scrollHeight;
}

/*********************************************
	GENERAL FUNCTIONS
*********************************************/
function padDigits(n, totalDigits) { 
	n = n.toString(); 
	var pd = ''; 
	if (totalDigits > n.length) { 
		for (i=0; i < (totalDigits-n.length); i++) { 
			pd += '0'; 
		} 
	}
	return pd + n.toString(); 
}

function init() {
	focusMessage(); 
	scrollChatWindow();
}

//focus the input box and scroll the chat window on load
window.onload = init; 