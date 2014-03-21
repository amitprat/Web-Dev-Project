


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Groups</title>
</head>

<body>
<?php
$get=$_GET['gtype'];
$con=explode("|",$get);
$gtype=$con[0];
$gname=$con[1];
?>
<p style="margin-left:40px;">
<form name="form1" method="post" action="second.php?gtype=<?php echo $_GET['gtype'] ?>">
<table border="0" style="margin-left:40px;">
<tr><td>Group Type :</td><td><?php echo $gtype ;?></td></tr>
<tr><td>Group Name :</td><td><?php echo $gname;?></td></tr>
<tr><td>User id :</td><td><input type="text" name="uid" /></td></tr>
<tr><td>Create UR Post here :</td><td><textarea name="post" cols="30" rows="4"></textarea></td></tr>
<?php
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$result=mysql_query("select gname  from g_join ");
$query_data=mysql_fetch_row($result);
$exists=in_array("$gname",$query_data);?>
<tr><td>&nbsp;</td><td>
<?php if(!$exists) { ?>
<input type="submit" name="join" value="Join" /> <?php  } else {?><label>you are already a member of this group!</label><br />
<input type="submit" name="unjoin" value="Unjoin" /><?php } ?></td></tr>
</table>
</form>
</p>
</body>
</html>
