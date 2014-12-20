<?php
/** 
* PHPSimpleChat - PostgreSQL database connection functions
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
* 
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.2
*/

function databaseConnect($dbServer, $dbName, $dbUser, $dbPassword, $dbPort = 5432) {
	global $activeDB;

	$activeDB =  pg_connect("host=$dbServer port=$dbPort dbname=$dbName user=$dbUser password=$dbPassword")
	    or die('ERROR: Could not connect:');
	$stat = pg_connection_status($activeDB);
	if ($stat !== PGSQL_CONNECTION_OK) {
		echo ' - connection status bad';
	}
}

function dbFetchArray($result) {
	if ($result) {
		return pg_fetch_assoc($result);
	}
}

function dbQuery($query) {
	global $activeDB, $lastResult;

	$lastResult = pg_query($activeDB, $query);

	if ($lastResult !== FALSE) {
		return $lastResult;
	}
	return FALSE;
}

function dbLastID($table) {
	if ($result = dbQuery("SELECT MAX(id) as id FROM $table")) {
		while ($row = dbFetchArray($result)) {
			$ID = $row["id"];
		}
		return $ID;
	}
	return FALSE;
}