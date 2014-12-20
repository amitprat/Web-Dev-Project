<?php 
session_start();
if($_SESSION['authuser']!='a')
{
header("location:admin.php?login please");
}
?>

<?php
include '../database.php';
if(isset($_POST['submit1']))
{
	$link_id=mysql_connect($host,$user,$password) or die("unable to connect");
    mysql_select_db($database) or die("unable to connect database");
	$heading=$_POST['textfield'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	$result1=mysql_query("INSERT INTO blog(Heading,Article,author,published) values('$heading','$post','$name','1')",$link_id) or die("problem inserting");
	header("location:a.php");
}
else if(isset($_POST['submit2']))
{
	$link_id=mysql_connect($host,$user,$password) or die("unable to connect");
    mysql_select_db($database) or die("unable to connect database");
	$heading=$_POST['textfield'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	$result1=mysql_query("INSERT INTO blog(Heading,Article,author) values('$heading','$post','$name')",$link_id) or die("problem inserting");
}

?>



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<script language="javascript">
function clearForms()
{
  var i;
  for (i = 0; (i < document.forms.length); i++) {
    document.forms[i].reset();
  }
}
</script>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Create new Article</title>
</head>

<body onLoad="clearForms()" onUnload="clearForms()"><h1 align="center"> Post New Article-Zinedu</h1>
<br>
<br><br><br>
<table align="center" width="60%">
<form id="form1" name="form1" method="post" action="">
 <tr><td> Title</td> 	
    <td><input type="text" name="textfield" /></td></tr>
  <tr><td>
    <label>Article</label></td>
    <td><textarea name="textarea" cols="100" rows="20"></textarea></td></tr>
  
  <tr><td>
    
    
   </td><td align="center"><input type="submit" name="submit1" value="Publish" />
    <input type="submit" name="submit2" value="Save Now" />
    </td>
  
</form>
</body>
</html>
