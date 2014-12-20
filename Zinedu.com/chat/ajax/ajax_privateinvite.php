<?php
/**
* PHPSimpleChat - AJAX called private chat update script
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
*
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.1
*/

error_reporting(E_ERROR | E_PARSE);

if (isset($_SERVER['HTTP_REFERER'])) {	
	
	include_once('../config.php');

	$users = array();
	$username = getData('username');
	$ignore = getData('ignore');
	$base = urldecode(getData('base'));

	if (is_numeric($ignore)) {
		ignorePrivateChat($ignore);
	}
	
	//Find any/all private chat invites
	$privateResult = getPrivateInvites($username);

	if ($privateResult) {

		$acceptImage = "<img class=\"private_chat_buttons\" src=\"" . getBase('/images/accept_button.png', $base) . "\" alt=\"Private chat with this user\">";
		$ignoreImage = "<img class=\"private_chat_buttons\" src=\"" . getBase('/images/ignore_button.png', $base) . "\" alt=\"Ignore the private chat request from this user\">";

		foreach ($privateResult as $invite) {
			$user = $invite['user'];
			$roomID = $invite['roomID'];
			$ID = $invite['ID'];
			echo "<div class=\"private_chat phpsimplechat\">$user <a href=\"?private=true&room=$roomID&user=$user&ID=$ID\" target=\"_blank\">$acceptImage</a> <a href=\"#\" onClick=\"private_invite_ignore($ID)\">$ignoreImage</a></div>";		
		}
	}
	else{
		echo '*';
	}
}