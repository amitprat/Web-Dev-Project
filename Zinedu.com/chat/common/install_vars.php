<?php
/** 
* PHPSimpleChat - Database table installer SQL statements
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
* 
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.2

*/

//container for storing database creation vars
$mysqlInstall = array();
$postgreInstall = array();

/*Drop table vars  - for both MYSQL and POSTGRESQL*/
$chatsDrop = 'DROP TABLE IF EXISTS chats';
$usersDrop = 'DROP TABLE IF EXISTS chat_users';
$invitesDrop = 'DROP TABLE IF EXISTS chat_invites';

$mysqlInstall['drop'][] = array('query' => $chatsDrop, 
	'success' => 'chats table dropped', 
	'error' => 'chats table could not be dropped');
$mysqlInstall['drop'][] = array('query' => $usersDrop, 
	'success' => 'chat_users table dropped', 
	'error' => 'chat_users table could not be dropped');
$mysqlInstall['drop'][] = array('query' => $invitesDrop, 
	'success' => 'chat_invites table dropped', 
	'error' => 'chat_invites table could not be dropped');

$postgreInstall['drop'][] = array('query' => $chatsDrop, 
	'success' => 'chats table dropped', 
	'error' => 'chats table could not be dropped');
$postgreInstall['drop'][] = array('query' => $usersDrop, 
	'success' => 'chat_users table dropped', 
	'error' => 'chat_users table could not be dropped');
$postgreInstall['drop'][] = array('query' => $invitesDrop, 
	'success' => 'chat_invites table dropped', 
	'error' => 'chat_invites table could not be dropped');

/**
* MYSQL install vars
*/

//chats
$chatsCreateMysql = "CREATE TABLE chats (
	id int(10) unsigned NOT NULL AUTO_INCREMENT,
	msg varchar(255) NOT NULL,
	roomid int(10) unsigned NOT NULL,
	identifier varchar(32) NOT NULL,
	stamp int(10) unsigned NOT NULL DEFAULT '0',
	usr varchar(32) NOT NULL,
	PRIMARY KEY (id),
	KEY roomid (roomid)
	)";
$chatsCreateMysqlMemory = $chatsCreateMysql . " ENGINE=MEMORY";

$mysqlInstall['create']['default'][] = array('query' => $chatsCreateMysql, 
	'success' => 'chats table with the default type engine created', 
	'error' => 'could not create chats table with the default type engine. MYSQL error text:');
$mysqlInstall['create']['memory'][] = array('query' => $chatsCreateMysqlMemory, 
	'success' => 'chats table with the memory type engine created', 
	'error' => ' could not create chats table with the memory type engine. MYSQL error text:');

//chat_users
$chat_usersCreateMysql = "CREATE TABLE chat_users (
	id int(10) unsigned NOT NULL AUTO_INCREMENT,
	roomid int(10) unsigned NOT NULL,
	usr varchar(64) DEFAULT NULL,
	lastaction int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (id),
	UNIQUE KEY compos (roomid,usr)
	)";
$chat_usersCreateMysqlMemory = $chat_usersCreateMysql . " ENGINE=MEMORY";

$mysqlInstall['create']['default'][] = array('query' => $chat_usersCreateMysql, 
	'success' => 'chat_users table with the default type engine created', 
	'error' => 'could not create chat_users table with the default type engine. MYSQL error text:');
$mysqlInstall['create']['memory'][] = array('query' => $chat_usersCreateMysqlMemory, 
	'success' => 'chat_users table with the memory type engine created', 
	'error' => 'could not create chat_users table with the memory type engine. MYSQL error text:');

//chat_invites
$chat_invitesCreateMysql = "CREATE TABLE chat_invites (
	id int(10) unsigned NOT NULL AUTO_INCREMENT,
	targetuser varchar(64) NOT NULL,
	usr varchar(64) NOT NULL,
	requesttime int(10) unsigned NOT NULL,
	roomid int(10) unsigned NOT NULL,
	joined tinyint(1) unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (id)
	)";
$chat_invitesCreateMysqlMemory = $chat_invitesCreateMysql . " ENGINE=MEMORY";

$mysqlInstall['create']['default'][] = array('query' => $chat_invitesCreateMysql, 
	'success' => 'chat_invites table with the default type engine created', 
	'error' => 'could not create chat_invites table with the default type engine. MYSQL error text:');
$mysqlInstall['create']['memory'][] = array('query' => $chat_invitesCreateMysqlMemory, 
	'success' => 'chat_invites table with the memory type engine created', 
	'error' => 'could not create chat_invites table with the memory type engine. MYSQL error text:');

/**
* POSTGRESQL install vars
*/

//chats
$chatsCreatePostgre = 'CREATE TABLE chats (
	id serial NOT NULL,
	msg character varying(255) NOT NULL,
	roomid integer NOT NULL,
	identifier character varying(32) NOT NULL,
	stamp integer NOT NULL DEFAULT 0,
	usr character varying(32) NOT NULL,
	PRIMARY KEY (id)
	)';

$postgreInstall['create'][] = array('query' => $chatsCreatePostgre, 
	'success' => 'chats table with the default type engine created', 
	'error' => 'could not create chats table with the default type engine. MYSQL error text:');

//additional index needs to be create, cannot be part of create statement
$chatsIndexPosrgre = 'CREATE INDEX roomid ON chats (roomid)';
$postgreInstall['index'][] = array('query' => $chatsIndexPosrgre, 
	'success' => 'index created on table chats', 
	'error' => 'could not create index on table chats');

//chat_users
$chat_usersCreatePostgre = 'CREATE TABLE chat_users (
	id serial NOT NULL,
	roomid integer NOT NULL,
	usr character varying(64) DEFAULT NULL,
	lastaction integer DEFAULT NULL,
	PRIMARY KEY (id),
	UNIQUE (roomid,usr)
	)';

$postgreInstall['create'][] = array('query' => $chat_usersCreatePostgre, 
	'success' => 'chat_users table with the default type engine created', 
	'error' => 'could not create chat_users table with the default type engine. MYSQL error text:');

//chat_invites
$chat_invitesCreatePostgre = 'CREATE TABLE chat_invites (
	id serial NOT NULL,
	targetuser character varying(64) NOT NULL,
	usr character varying(64) NOT NULL,
	requesttime integer NOT NULL,
	roomid integer NOT NULL,
	joined smallint NOT NULL DEFAULT 0,
	PRIMARY KEY (id)
	)';

$postgreInstall['create'][] = array('query' => $chat_invitesCreatePostgre, 
	'success' => 'chat_invites table with the default type engine created', 
	'error' => 'could not create chat_invites table with the default type engine. MYSQL error text:');
