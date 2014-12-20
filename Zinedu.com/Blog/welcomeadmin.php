<?php
$username=$_POST['username'];
$password=$_POST['password'];
$x=0;$p=1;$u=1;
$message1="Both the fields are empty";
$message2="Username does not exist";
$message3="You have entered an invalid password corresponding to the username specified";
$message4="some fields left blank";
if( $username=="" || $password=="" )
  { header("Location:admin.php?message=$message4");
  	exit(); }
else {mysql_connect('localhost','root','') or die('not connected');
mysql_select_db('forumtopics_db') or die('no database');
$query="Select name,password from admin where name='$username'" or die('no');
$result=mysql_query($query);
$x=mysql_numrows($result);
if($x!=0)
 { 	$pass=mysql_result($result,0,'password');
	if ($password==$pass)
 		{ 
			$p=1;
		}
	else { 	header("Locationadmin.php?message=$message3");
  			exit(); }	
 }
else
 { header("Location:admin.php?message=$message2");
  			exit(); }
 } 
mysql_close();
?> 	
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Home Page</title>
</head>

<body>
<?php 

if ($p==1 )
 { 
 session_start();
$_SESSION['authuser']='a';
 header("Location:a.php");
}
else
   header("location:admin.php");
?> 
</body>
</html>
