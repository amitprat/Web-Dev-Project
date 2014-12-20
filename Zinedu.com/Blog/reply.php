<?php
include 'pass.php';
    $subject=$_GET['subject'];
	global $con,$seltopic,$selsub;
	$con=explode("|",$subject);
	$seltopic=$con[1];
	$selsub=$con[0];
if(isset($_POST['submit']))
{
   
	$link_id=mysql_connect("localhost",$user,$pass);
    mysql_select_db($database);
	session_register('username');
	$uname=$_SESSION['username'];
	$post=$_POST['textarea'];
	$result1=mysql_query("INSERT INTO reply (topic,subject,post,replyby) values('$seltopic','$selsub','$post','$uname')",$link_id);
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<h2 align="center">Zinedu IT</h2>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Forum</title>
<style type="text/css">
<!--
.style1 {font-family: Calibri}
-->
</style>
<script language="javascript">
function reply()
{
document.all['post'].style.display = ""; 
}
</script>
</head>

<body>
<br/></br/><br/><label style="position:absolute; text-decoration:underline;left:20%;">Query Posted</label> <br/><br/></br/>
<br/>
<table width="60%" border="1" cellpadding="2" cellspacing="1" align="center" top="300px">
<tr >
       <td align="center">Topic</td>
        <td align="center">Subject</td>
        <td align="center">Question/Post</td>
		<td align="center"> Author and Date</td>
      </tr> 
<?php
	include 'pass.php';
    $subject=$_GET['subject'];
	global $con,$seltopic,$selsub,$selpost;
	$con=explode("|",$subject);
	$seltopic=$con[1];
	$selsub=$con[0];
	$link_id=mysql_connect("localhost",$user,$pass);
    mysql_select_db($database); 
	$result=mysql_query("SELECT post,author,date from forum1 where topic='$seltopic' and subject='$selsub'",$link_id);
	$x=0;
	while($x<mysql_numrows($result)){
	$post=mysql_result($result,$x,'post');
	$Date=mysql_result($result,$x,'date');
	$author=mysql_result($result,$x,'author');
	$x++;
    ?>
      <tr>
	  <td align="center"> <?php echo $seltopic; ?></td>
	  <td align="center"> <?php echo $selsub; ?></td>
	  <td align="center" width="60%"> <?php echo $post; ?> </a></td>
	  <td align="center"> <?php echo $author; ?> </br> <?php echo $Date; ?>  </td>
	  
	 
	  </tr><?php
	  }
	  ?>
	  </table>
	  <br/></br/><label style="position:absolute; text-decoration:underline;left:20%;">Replies</label><br/> <br/>
	  <table width="60%" border="1" cellpadding="2" cellspacing="1" align="center" top="300px">
    <?php
	$link_id=mysql_connect("localhost",$user,$pass);
    mysql_select_db($database); 
	$result=mysql_query("SELECT post,replyby,date from reply where topic='$seltopic' and subject='$selsub'",$link_id);
	$x=0;
	while($x < mysql_numrows($result))
    {
	$post=mysql_result($result,$x,'post');
	$Date=mysql_result($result,$x,'date');
	$replyby=mysql_result($result,$x,'replyby');
	$x++;
    ?>
      <tr>
	  <td align="center" width="70%"> <?php echo $post; ?> </td>
	  <td align="center"> <?php echo $replyby; ?> </br> <?php echo $Date; ?>  </td>
	  </tr>
	<?php
    }
    ?>
	</table>
	<br><br>
	<form name="form1" method="POST">
	<p style="position:absolute;left:69%;">
   <input  type='button' name='button' value='post ur reply' onclick="reply()"></p><br><br><br>
    <div id="post" style="display:none;">
	<label style="position:absolute; text-decoration:underline;left:20%;">Write your Post here</label><br><br>
	<table width="60%" cellpadding="2" cellspacing="1" align="center"><tr><td><textarea name="textarea" rows="14" cols="50" ></textarea></td></tr>
	<tr><td style="position:absolute;left:63%;"><input type="submit" name="submit" value="Post"></td></tr></table>

	</div>
	</form>
	<br><br>
	<a href="topic.php">Back To Forum Topics</a>
</body>
</html>
