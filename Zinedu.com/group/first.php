<?php
include '../database.php';
session_register('uname');
$_SESSION['uname']="atul";
$uname=$_SESSION['uname'];
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu-Groups</title>
<h1 style="color:#009900;" align="center">Zinedu-IT</h1>
<script language="javascript">
function disp()
{
} 
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
<form name="form1" method="post" action="search.php">
<p style="margin-left:40px;">
<input type="submit" name="search" value="Search" accesskey="return" />
<input type="text" name="text1" width="100" />




<p style="margin-left:40px;">
<br /><br /><br />
<label>Group Types</label><br /><br />
<?php 
$link_id=mysql_connect($host,$user,$password);
mysql_select_db("zinedu_db");
$result=mysql_query("select  gtype  from g_info");
while($query_data=mysql_fetch_row($result))
    { ?><a href="second.php?gtype=<?php echo $query_data[0] ?>"><ul><li><?php echo $query_data[0] ?></li></ul></a>
	<?php
	}
?>	
</p>

<table border="1" style="position:absolute;top:250px; left:500px;" width="20%"><th>Your Groups</th>	
<?php
$result1=mysql_query("select gname from g_join where members='$uname' order by gname");
while($query_data=mysql_fetch_row($result1))
{
?><tr><td align="center"><a href="group.php?gname=<?php echo $query_data[0]; ?>"><?php echo $query_data[0]; ?></a></td></tr><?php
}
?>
</table>

</body>
</html>
