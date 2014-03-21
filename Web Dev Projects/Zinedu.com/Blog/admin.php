<html>
<head>
<meta http-equiv="Content-Language" content="en-us">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>Login Page - Zinedu</title>

</head>

<body>
<form method='post' action='welcomeadmin.php'>
<p align="right">Username&nbsp;&nbsp;&nbsp; <input type="text" name="username" size="20"></p>
<p align="right">Password&nbsp;&nbsp;&nbsp; <input type="password" name="password" size="20"></p>

<p align="right"><input type="submit" value="Submit" name="B1"></p>

</form>
<?php
if (isset ($_GET['message']))
 { 	echo "<p align=right>";
 	echo $_GET['message'];
	echo "</p>";
 }
?>


</body>

</html>