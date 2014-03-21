<?php 
include '../database.php';
$x=$_GET['textarea1'];
session_start();
$blog_id=$_SESSION['blog_id'];
$user=$_SESSION['username'];
if($x==NULL)
{
$message=urlencode(" No comment Posted");
header("location:blog.php?$message");
}
else
{
$link_id=mysql_connect($host,'root',$password) or die("cannot connnect");
mysql_select_db($database) or die("cannot select database");
$result1=mysql_query("INSERT INTO blogpost(comments,author,blog_id) values('$x','$user','$blog_id')",$link_id) or die("cannot insert into table");
echo "Your comment has been added ";
echo "Redirecting...";
//header("Refresh:2;url=blog.php");
}
?>
<html>
<head> <title> confirmation</title>
<body>
<meta http-equiv="refresh" content="0;URL=http:../blog/blog.php" />
<br><br><br><br><br>
<table align="center">
<tr><td> Your Comment has been Added</td></tr>
<tr><td> Redirecting...</td></tr>
</table>
</body>
</html>
