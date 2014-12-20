<?php

class user
		{
			private $username;
			private $password;
			private $userid;
			private $email;
			private $timestamp;
			private $activeuser;
			private $id;
			private $banneduser;
			private $activeguest;
			
			function checkuser($username)
				{
					include '../database.php';
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db('login');
					$result=mysql_query("SELECT userid from users where username='$username'",$link_id);
					if(!$result || (mysql_numrows($result) < 1))
						{
							return 0;
						}
					else 
							return 1;
					
				}
				
			function checkpassword($username,$p)
				{
					include '../database.php';
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("SELECT password from users where username='$username'",$link_id);
					$x=0;
					echo $pass=mysql_result($result,$x,'password');
					
					if($pass==$p)
						{
							return 1;
						}
					else 
							return 0;
				}
			function setactiveuser($username)
				{
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("insert into active_users(username) values('$username')",$link_id) or die('can not insert');
				}
				
			function logout($username)
				{
					include '../database.php';
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("delete from active_users where username='$username'",$link_id) or die('can not delete');
				}
			function countactiveuser()
				{
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("SELECT count(*) from active_users where 1",$link_id);
					$x=0;
					$y=mysql_result($result,$x);
					echo $y;
				}
			function listactiveuser()
				{
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("SELECT username from active_users where 1",$link_id);
					$x=0;
					while($x<mysql_numrows($result))
					{
						$user=mysql_result($result,$x,'username');
						echo $user;
						$x++;
					}
				}
			function addguest()
				{	$ipaddress="127.0.0.1";
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("insert into active_guests(ip) values('$ipaddress')",$link_id) or die("Problem inserting");
				}
			
			function removeactiveuser($username)
				{
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("delete from active_users where username='$username'",$link_id) or die('can not delete');
				}
			function removeguest($ip)
				{
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$result=mysql_query("delete from active_guests where ip='$ip'",$link_id) or die('can not delete');
				}
				
					
				 
				
					
		}
			