<?php
	include '../database.php';
    $subject=$_GET['subject'];
	global $con,$seltopic,$selsub;
	$con=explode("|",$subject);
	$seltopic=$con[1];
	$selsub=$con[0];
if(isset($_POST['submit']))
{
	$con=$_POST['textarea'];
	$list=explode(" ",$con);
	$abusive="welcome";
	do{
	$abuse=array("chut","lund","gand","fuck","muth","londa","loda","jhant","machod","matherchod","motherfucker","sisterfucker","bhenchod","betichod");
	while(list($key,$abus)=each($abuse))
	if(($list[key($list)])==$abus)
	 $abusive="Abuse";
	}while(next($list)); 
	if($abusive!="Abuse"){
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database); 
	session_register('uname');
	$uname=$_SESSION['uname'];
	$post=$_POST['textarea'];
	$result1=mysql_query("INSERT INTO reply (topic,subject,post,replyby) values('$seltopic','$selsub','$post','$uname')",$link_id);}
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />

<script language="javascript">
function reply()
{
document.all['post'].style.display = ""; 
}
</script>
</head>

<body>
<div id="upbg"></div>

<div id="outer">


	<div id="header">
		<div id="headercontent">
			<h1>Zinedu</h1>
			<h2>Personal Preparation and Personal Guide</h2>
		</div>
	</div>
	
	
	<?php
	include '../login/form.php';
	$forms=new forms();
	$forms->show();
	?>
	


	<div id="headerpic"></div>

	
	<div id="menu">
		<!-- HINT: Set the class of any menu link below to "active" to make it appear active -->
		<ul>
			<li><a href="#" >Home</a></li>
			<li><a href=blog/blog.php>Blog</a></li>
			<li><a href=forum/topic.php class="active">Forum</a></li>
			<li><a href=group/first.php>Group</a></li>
			<li><a href=discuss/discuss1.php>Discuss</a><li>
			<li><a href="chat/index.php">Chat</a></li>
		</ul>
	</div>
		<div id="menubottom"></div>
	

<table width="60%" border="1" cellpadding="2" cellspacing="1" align="center" top="300px">
<tr >
       <td align="center">Topic</td>
        <td align="center">Subject</td>
        <td align="center">Question/Post</td>
		<td align="center"> Author and Date</td>
      </tr> 
<?php
    $subject=$_GET['subject'];
	global $con,$seltopic,$selsub,$selpost;
	$con=explode("|",$subject);
	$seltopic=$con[1];
	$selsub=$con[0];
	$link_id=mysql_connect($host,$user,$password);
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
	$link_id=mysql_connect($host,$user,$password);
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
	  </tr><?php
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
	<?php 
		if($abusive=="Abuse")
	echo $abusive."contets can't be posted"; ?>
	</form>
	<br><br>
	<a href="topic.php">Back To Forum Topics</a>
	<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>
</body>
</html>
