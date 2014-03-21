<?php 
include '../database.php';
$x=$_GET['textarea1'];
session_start();
$qid=$_SESSION['qid'];
$user=$_SESSION['uname'];
if($x==NULL)
{
$message=urlencode(" No comment Posted");
header("location:../discuss/discuss1.php?$message");
}
else
{
$link_id=mysql_connect("localhost",'root','') or die("cannot connnect");
mysql_select_db('forumtopics_db') or die("cannot select database");
$result1=mysql_query("INSERT INTO discuss_reply(reply,qid,author) values('$x','$qid','$user')",$link_id) or die("cannot insert into table");
}
?>


<html>
<head> <title> confirmation</title>
<body>
<meta http-equiv="refresh" content="0;URL=http:../discuss/discuss1.php" />
<br><br><br><br><br>
<table align="center">
<tr><td> Your Comment has been Added</td></tr>
<tr><td> Redirecting...</td></tr>
</table>
</body>
</html>
