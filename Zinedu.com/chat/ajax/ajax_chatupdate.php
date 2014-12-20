<?php
/**
* PHPSimpleChat - AJAX called chat update script
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
	
	$chats = array();
	$message = getData('msg');
	$roomID = getData('room');
	$startID = getData('startID');
	$username = getData('usr');
	$identifier = getData('iden');

	insertChat($username, $roomID, $message, $identifier);

	$chats = getRecentChats($roomID, $startID);

	echo implode(', ', str_replace(',', '&#44;', $chats));
}