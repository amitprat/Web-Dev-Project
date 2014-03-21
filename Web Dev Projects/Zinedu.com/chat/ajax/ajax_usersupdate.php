<?php
/**
* PHPSimpleChat - AJAX called user update script
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
	$chatRoom = getData('room');
	$username = getData('usr');
	$base = urldecode(getData('base'));
	$arg = getData('arg');

	if ($arg == 'get') {
		//Find all users active in the last xx seconds
		$users = getActiveUsers($chatRoom);
		//add private chat icons
		$privateChatIcon = getBase('/images/icon_private_chat.png', $base);
		$users = addPrivateChatLinks($users, $username, $privateChatIcon);

		echo implode(', ', str_replace(',', '&#44;', $users));
	}
	else if ($arg == 'set') {
		//Set user as active
		setUserActive($chatRoom, $username);
		echo '*';
	}
}