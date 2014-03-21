<?php
	$link_id=mysql_connect('localhost','root','') or die('problem connecting');
	mysql_select_db('login') or die('problem selecting database');
	$user = strip_tags(substr('amit',0,32));
	$pw = strip_tags(substr('pratap',0,32));
	echo $user;
	echo $pw;
	
	$cleanpw = crypt(md5($pw),md5($user));
	echo $cleanpw;
	$cleanuser=mysql_real_escape_string($user);
	$cleanpassword=mysql_real_escape_string($cleanpw);
	
	$sql = mysql_query("INSERT INTO users(username,password,userid,userlevel,email,timestamp) values('$cleanuser','$cleanpassword','3','1','amit_ptp@gmail.com','0')",$link_id) or die('problem inserting');
	//$result1=mysql_query("INSERT INTO blog(Heading,Article,author,published) values('$heading','$post','$name','1')",$link_id) or die("problem inserting");

	
?>