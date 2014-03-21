<?php
include '../database.php';
session_start();
if(isset($_POST['submit1']))
{
	$link_id=mysql_connect($host,$user,$password) or die("unable to connect");
    mysql_select_db($database) or die("unable to connect database");
	$heading=$_POST['textfield'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	if($heading=='' or $post=='')
	{
	$message=urlencode("Some Fields left blank");
	header("location:writearticle.php?$message");
	}
	else
	{
	$result1=mysql_query("INSERT INTO blog(Heading,Article,author) values('$heading','$post','$name')",$link_id) or die("problem inserting");
	header("Location:confirm.php");
	}
?>
<meta http-equiv="refresh" content="0;URL=http://localhost/zinedu.com/blog/confirm.php" />
<?php
}
?>



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Create Your Article- Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />
</head>

<body>
<div id="upbg"></div>

<div id="outer">


	<div id="header">
		<div id="headercontent">
			<h1>Zinedu</h1>
			<h2>Personal Prepration and Personal Guide</h2>
		</div>
	</div>
	<div id="headerpic"></div>

	
	<div id="menu">
		<!-- HINT: Set the class of any menu link below to "active" to make it appear active -->
		<ul>
			<li><a href=../index.html >Home</a></li>
			<li><a href=../blog/blog.php class="active">Blog</a></li>
			<li><a href=../forum/topic.php>Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			<li><a href="login.php"> Login</a>
			<li><a href="#">Contact</a></li>
		</ul>
	</div><div class="divider1"></div>	
<h1 align="center"> Submit Your Article- Zinedu </h1><br> <br> <br><br>
<form id="form1" name="form1" method="post" action="">
<table align="center">
  <tr><td>Title	</td>
  
  <td>  <input type="text" name="textfield" /></td></tr>
  
    <tr><td>Article</td>
    <td><textarea name="textarea" cols="100" rows="20"></textarea></td></tr>

  <tr ><td align="center" colspan="2">
    <input type="submit" name="submit1" value="Send For Approval" /></td>
  </tr>
  </table>
   
</form>
<div class="divider2"></div>
<br/>
<br/>
<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>
	
</body>
</html>
