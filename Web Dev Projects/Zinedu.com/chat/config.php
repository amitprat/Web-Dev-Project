<?php
/** 
* PHPSimpleChat - User configuratble settings
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
* 
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.1
*/

/***********************************************************************/
/* Part 1: Database Connection 
/* Change these to the correct variables for your MSYQL instance
/***********************************************************************/

$dbServer = 'localhost';
$dbName = 'chat_database';
$dbUser = 'root';
$dbPassword = '';

//Uncomment $dbPort and add port number if using a non-default port for databases other than MYSQL.
//MYSQL port, if non-default, should be included in the $dbServer variable. See http://www.php.net/mysql-connect for more info.
//The postgreSQL default port number (5432) is already specified by default in common/postgre_database.php
//$dbPort = 12345;

//Uncomment below line or specify correct connection file if using postgreSQL or other supported db. 
//Format must be database name followed by _database.php for the installer to work properly.
$databaseConnectionFile = 'common/mysql_database.php';
//$databaseConnectionFile = 'common/postgre_database.php';

//Make database connection available and connect
include_once($databaseConnectionFile);

//Uncomment second line if using $dbPort argument
databaseConnect($dbServer, $dbName, $dbUser, $dbPassword);
//databaseConnect($dbServer, $dbName, $dbUser, $dbPassword, $dbPort);

//unset password so no cross site scripting vulnerability - other db vars may be needed later
unset($dbPassword);

/*************************************************************************/
/* Part 2: AJAX Tuning
/* You can change these settings for perfomance reasons
/* Lower times mean quicker response from chatroom, but heavier database load.
/*************************************************************************/

//Chat message update ajax interval, in seconds. Default: 4.
define('PHPSC_CHAT_INTERVAL', 4);

//Chat message update ajax interval when a user is idle, in seconds. Default: 26.
define('PHPSC_CHAT_IDLE_INTERVAL', 26);

//Number of chat intervals without activity for a user to be considered idle. Default: 60.
define('PHPSC_IDLE_THRESHOLD', 60);

//Users list ajax update interval, in seconds. Default: 14.
define('PHPSC_USER_ACTIVE_INTERVAL', 14);

//Active user update chat_users table ajax interval, in seconds.  allows new users to room to see
//current users. This must be less than PHPSC_USER_EXPIRE_INTERVAL to work properly. Default: 90.
define('PHPSC_USER_UPDATE_INTERVAL', 90);

//Check for private chat requests ajax interval, in seconds. Default: 15.
define('PHPSC_PRIVATE_CHAT_INTERVAL', 15);


/*************************************************************************/
/* Part 3: Other System Config Variables
/* Changing these is not recommended, unless you are seriously
/* tinkering with the core of PHPSimpleChat
/*************************************************************************/

//Sets whether new users can see recent convos when first opening chat window. Default: TRUE.
define('PHPSC_SAVE_CONVOS', TRUE);

//Turns off/on timestamp display on chat conversations. Default: TRUE.
define('PHPSC_DISPLAY_TIMESTAMP', TRUE);

//Number of chats shown when chatroom is loaded. Default: 20.
define('PHPSC_RECENT_CONVO_COUNT_LOAD', 20);

//Number of chats retrieved on each ajax call. More chats = slower response. Default: 40.
define('PHPSC_RECENT_CONVO_COUNT', 40);

//Number of seconds after activity for user to removed from active user list. 
//This must be greater than PHPSC_USER_UPDATE_INTERVAL to work properly. Default: 120.
define('PHPSC_USER_EXPIRE_INTERVAL', 120);

//Number of seconds after which a user's join message will appear again, if they
//refresh the page or navigate back to chatroom. Prevents excessive join messages. Default: 60.
define('PHPSC_JOIN_MESSAGE_EXPIRE', 60);

//Number of seconds after which a user can send another private chat invite to the same user. Default: 30.
define('PHPSC_PRIVATE_INVITE_SEND_EXPIRE', 30);

//Number of seconds after a private chat invite will be ignored. Default: 600.
define('PHPSC_PRIVATE_INVITE_EXPIRE', 600);

//Interval of number of chat messages at which garbage collection is performed. Default: 500.
define('PHPSC_GARBAGE_INTERVAL', 500);

//Chats and invites older than this # seconds will be deleted in garbage collection. Default: 43200 (12 hours)
define('PHPSC_GARBAGE_EXPIRATION', 43200);

//Specifies global base directory. Do not change.
define('PHPSC_GLOBAL_BASE', __FILE__);

//Common functions & ajax related functions
include_once('common/functions.php');

