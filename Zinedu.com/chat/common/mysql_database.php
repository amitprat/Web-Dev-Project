<?php
/** 
* PHPSimpleChat - MYSQL database connection functions. This file replaces database.php from v0.1.
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
* 
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.2
*/

//$dbPort arg for connection to other types of databases - MYSQL port should be specified in $dbServer string
function databaseConnect($dbServer, $dbName, $dbUser, $dbPassword, $dbPort = NULL) {
	global $activeDB;

	$activeDB = mysql_connect($dbServer, $dbUser, $dbPassword)
	    or die('ERROR: Could not connect: ' . mysql_error() . '. Make sure you have modified your ' . __FILE__ . ' file with the proper database parameters.');
	mysql_select_db($dbName);
}

function dbFetchArray($result) {
  if ($result) {
    return mysql_fetch_array($result, MYSQL_ASSOC);
  }
}

function dbQuery($query) {
  global $activeDB;

  $result = mysql_query($query, $activeDB);

  if (!mysql_errno($activeDB)) {
    return $result;
  }
  return FALSE;
}

//$table arg only for compatibility with other types of databases and their xxxx_database.php connection file
function dbLastID($table = NULL) {
  global $activeDB;
  return mysql_insert_id($activeDB);
}