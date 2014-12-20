<?php
if(isset($_POST['search']))
{session_register('search');
$_SESSION['search']=$_POST['text1'];
$gname=$_POST['text1'];
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu-IT</title>
<h1 style="color:#009900;" align="center">Zinedu-IT</h1>
</head>

<body>
<table border="0" width="400" style="margin-left:40px;" cellpadding="2" cellspacing="1">
 <caption>
   Search Results
  </caption><br /><br />
<?php
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$result=mysql_query("select gname,gtype,creater,date,des from g_info where gname like '%$gname%' group by gname");
while($query_data=mysql_fetch_row($result))
{echo "<tr><td width='40%'>"."Group Name:"."</td>"."<td width='60%'>".$query_data[0]."</td></tr>";
echo "<tr><td>"."Group Type:"."</td>"."<td>".$query_data[1]."</td></tr>";
echo "<tr><td>"."Group Creater:"."</td>"."<td>".$query_data[2]."</td></tr>";
echo "<tr><td>"."Created On:"."</td>"."<td>".$query_data[3]."</td></tr>";
echo "<tr><td>"."Description:"."</td>"."<td>".$query_data[4]."</td></tr>";
echo "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
echo "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
echo "<tr><td>&nbsp;</td><td><a name='join' href='fourth.php?gtype=".$query_data[1].'|'.$query_data[0]."'>Join This group</a>"."</td></tr>";
echo "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
echo "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
}
 ?>
</table>

</body>
</html>
