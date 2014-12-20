<?php
	include '../database.php';
	$_SESSION['uname']="amit";
	$link_id=mysql_connect($host,$user,$password) or die("unable to connect");
    mysql_select_db($database) or die("unable to connect database");
	$heading=$_POST['textfield'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	if($heading=='' or $post=='')
	{
	$message=urlencode("Some Fields left blank");
	header("location:ask.php?$message");
	}
	else
	{
	$result1=mysql_query("INSERT INTO discuss_question(subject,question,author) values('$heading','$post','$name')",$link_id) or die("problem inserting");
	}
?>
<meta http-equiv="refresh" content="0;URL=http:../discuss/ask.php" />