<?php 
session_start();
if($_SESSION['authuser']!='a')
{
header("location:admin.php?login please");
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
</head>
<a href="newpost.php">Create a new post</a><br/> 
<a href="approve.php">Approve or reject post</a><br/>
<a href="#">View comments on your posts</a><br/>
<a href="#">Remove your post</a><br/>
<a href="#"> unpublish your post</a><br/>
<body>
</body>
</html>
