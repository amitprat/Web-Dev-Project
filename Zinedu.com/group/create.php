
<?php
session_register('uname');
session_register('gtype');
?>
<?php
if(isset($_POST['create']))
{
$gtype=$_SESSION['gtype'];
$auther=$_SESSION['uname'];
$gname=$_POST['gname'];
$description=$_POST['descrip'];
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$sql="insert into g_info(gname,gtype,creater,des) values ('$gname','$gtype','$auther','$description')";
mysql_query($sql);
}
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<h1 style="color:#009900;" align="center">Zinedu-IT</h1>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Groups</title>
</head>

<body>
<p style="margin-left:40px;">
<form name="creategroup" method="post" action="">
<table cellspacing="3" cellpadding="3" border="0">
<tr><td>Group Type :</td><td><?php echo $_SESSION['gtype'] ?></td></tr>
<tr><td>Group Name :</td><td><input type="text" name="gname" width="100" /></td></tr>
<tr><td>Creater : </td><td><?php echo $_SESSION['uname'] ?></td></tr>
<tr><td>Discription : </td><td><textarea name="descrip" cols="30" rows="8" ></textarea></tr>
<tr><td>&nbsp;</td><td><input type="submit" name="create" value="Create Group" /></td></tr>
</table>
</form>
</body>
</html>
