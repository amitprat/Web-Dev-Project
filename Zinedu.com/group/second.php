<?php
session_register('gtype');
$get=$_GET['gtype'];
$con=explode("|",$get);
//$gtype=$_GET['gtype'];
$gtype=$con[0];
$_SESSION['gtype']=$gtype;
?>

<?php
session_register('uname');
$get=$_GET['gtype'];
$con=explode("|",$get);
$gtype=$con[0];
$gname=$con[1];
$member=$_SESSION['uname'];
if(isset($_POST['join']))
{
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$sql="insert into g_join(gname,members) values('$gname','$member')";
mysql_query($sql);
?><script language="javascript">window.location="http://localhost/group/first.php"</script><?php
}
if(isset($_POST['unjoin']))
{$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$sql="delete from g_join where gname='$gname' ";
mysql_query($sql);	
?><script language="javascript">window.location="http://localhost/group/first.php"</script><?php
}
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<h1 style="color:#009900;" align="center">Zinedu-IT</h1>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Groups</title>
<script language="javascript">

</script>
</head>

<body>
<pre style="margin-left:40px;">
here..............
...................
..................
...................
.................
</pre>
<br /><br /><br />
<p style="margin-left:40px;">
<label>Group Type : <?php echo $con[0]; ?></label><br /><br />
<label>Groups:</label><br /><br />


<?php 
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$result=mysql_query("select gname  from g_info where gtype='$gtype' order by date desc");
while($query_data=mysql_fetch_row($result))
    { ?><a href=""><ul><li style=" font-size:22px;"><b><?php echo $query_data[0]; ?></b></li></ul><br /></a>
	
	<?php
	$result1=mysql_query("select creater,date,des  from g_info where gname='$query_data[0]' order by date desc");
	$x=0;
	$y=0;
	while($x < mysql_numrows($result1)) 
		{
		$creater=mysql_result($result1,$x,'creater');
		$date=mysql_result($result1,$x,'date');
		$des=mysql_result($result1,$x,'des');
		$x++;
		?>
		<table border="0" style="margin-left:40px;">
		<tr><td><?php echo $des ?><br /><br /><br /></td></tr>
		
		<tr><td>Created By : <?php echo $creater; ?></td></tr>
		<tr><td>Ceeated On : <?php echo $date; ?><br /><br /></td></tr>
		<tr><td>
	    <a name="join" href="fourth.php?gtype=<?php echo $_GET['gtype'].'|'.$query_data[0].'|';?>">Join this group</a></td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td>&nbsp;</td></tr><tr><td>&nbsp;</td></tr><tr><td>&nbsp;</td></tr>
		</table>
		<?php
		}
	}
?>	

<br /><br />
<!--<input type="button" name="create" value="Create UR Own Group"  />
-->
<a href="third.php?gtype=<?php echo $gtype ?>">Create UR Own Group</a>
</p>
</body>
</html>
