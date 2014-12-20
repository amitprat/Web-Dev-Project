<?php
session_register('uname');
if(isset($_POST['post']))
{
$uname=$_SESSION['uname'];
$gname=$_GET['gname'];
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$sub=$_POST['sub'];
$post=$_POST['textarea'];
$sql1="insert into g_post(gname,subject,post,postedby) values('$gname','$sub','$post','$uname')";
mysql_query($sql1);
?><script language="javascript">window.location="http://localhost/group/first.php"</script><?php
}
if(isset($_POST['unjoin']))
{
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$gname=$_GET['gname'];
$sql="delete from g_join  where gname='$gname' ";
mysql_query($sql);	
?><script language="javascript">window.location="http://localhost/group/first.php"</script><?php
}
?>




<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<h1 style="color:#009900;" align="center">Zinedu-IT</h1>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
<script language="javascript">
function fun1()
{
document.all['newpost'].style.display = ""; 
}
function fun2()
{
document.all['div1'].style.display = ""; 
}
</script>
</head>

<body>
<form name="form1" method="post" action="">
<br /><br /><br />
<table border="0"  style="margin-left:60px;" width="40%">
<tr><td width="50%">Group Name</td><td width="50%" align="left"><?php echo $_GET['gname']; ?></td></tr>
<tr><td width="50%">Owner</td><td align="left" width="50%">
<?php $gname=$_GET['gname']; 
$link_id=mysql_connect("localhost","root","");
mysql_select_db("zinedu_db");
$res=mysql_query("select creater from g_info where gname='$gname'"); 
$query_data=mysql_fetch_row($res);
echo $query_data[0];?>
</td></tr>
<tr><td width="50%">Members</td>
<td width="50%" align="left">
<?php
$gname=$_GET['gname'];
$result=mysql_query("select members from g_join where gname='$gname'"); 
while($res=mysql_fetch_row($result))
{ echo $res[0]."<br>"; 
}
?>
</td></tr>
<?php
$count=mysql_query("select count(*) from g_post where gname='$gname' ");
$query_data=mysql_fetch_row($count);
?>
<tr><td width="50%">No of Posts</td><td align="left"><?php echo $query_data[0]; ?></td></tr>
</table><br /><br />
<input type="button" name="grunjoin" value="Unjoin this group" style="margin-left:60px;" onClick="fun2()" />
<div id="div1" style="display:none;">
<table border="0" style="margin-left:60px; ">
<tr><td>You surely want to unjoin this group</td></tr>
<tr><td><input type="submit" name="unjoin" value="Confirm" /></td></tr>
</table></div><br />
<p style="margin-left:60px;">
<input type="button" name="create" value="New Post" onClick="fun1()" />
<div id="newpost" style="display:none;">
<table style="margin-left:60px;">
<tr><td>Subject</td><td><input type="text" name="sub" /></td></tr>
<tr><td>Post</td><td><textarea name="textarea" rows="4" cols="30"></textarea></td></tr>
<tr><td>&nbsp;</td><td><input type="submit" name="post" value="Post" /></td></tr>
</table>
</div>
<br />
<label style="margin-left:60px;">Posts</label>
<br /><br /><br />
<?php
$uname=$_SESSION['uname'];
$gname=$_GET['gname'];
$posts=mysql_query("select post,subject,date,postedby from g_post  where gname='$gname' ",$link_id);
$x=0;
?>
<table style="margin-left:60px;" border="1" width="50%">
<tr><td align="center" width="20%">Subject</td><td align="center" width="20%">Created By</td><td align="center" width="40%">Posts</td><td align="center" width="20%">Date/Time</td></tr>
<?php
while($x<mysql_num_rows($posts))
{$post=mysql_result($posts,$x,'post');
$sub=mysql_result($posts,$x,'subject');
$date=mysql_result($posts,$x,'date');
$postedby=mysql_result($posts,$x,'postedby');
$x++;
?>
<tr><td  align="center"><?php echo $sub ; ?> </td>
<td  align="center"><?php echo $postedby ; ?> </td>
<td ><?php echo $post; ?></td>
<td align="center"><?php echo $date ; ?> </td></tr>
<?php
}
?>
</table>
</form>
</body>
</html>
