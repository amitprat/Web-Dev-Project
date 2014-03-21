<?php
session_start();
$_SESSION['uname']="Saurabh";
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Blog</title>
<script language="javascript">
function reply()
{
document.all['post'].style.display = ""; 
}
</script>
</head>
<body>
<h1 align="center"> Zinedu </h1> 
<br/>
<br/>

<table border="1" width="60%" align="center" cellpadding="2">
<?php
	$link_id=mysql_connect("localhost","root","");
    mysql_select_db('forumtopics_db');
    $result=mysql_query("SELECT Heading,Article,author,date,blog_id from blog where published=1 order by Date desc limit 0,1",$link_id);
	$x=0;
	while($x < mysql_numrows($result))
    {
	$topic=mysql_result($result,$x,'Heading');
	$article=mysql_result($result,$x,'article');
	$Date=mysql_result($result,$x,'date');
	$author=mysql_result($result,$x,'author');
	$_SESSION['blog_id']=mysql_result($result,$x,'blog_id');
	$x++;
    ?>
      <tr><td align="center"> <h3><?php echo $topic; ?> </h3></a></td></tr>
	  <tr><td align="justify"> <?php echo $article; ?> </td></tr>
	  <tr><td>  Written by <?php  echo $author;echo $Date; ?> </td></tr>
		</table>
	
     <?php
    }
    ?>
	<br/>
	<br/>
	<br/>
	<form name="form1"  action="send.php">
   <input  type='button' name='submit1' value='Post Your Comment' style="position:relative; left:20%;" onclick="reply()"><br><br><br>
    <div id="post" style="display:none;">
	<label style="position:absolute;left:20%;">Write your Comments here</label><br><br>
	<table width="60%" cellpadding="2" cellspacing="1" align="center"><tr><td>
	<textarea name="textarea1" rows="14" cols="50" ></textarea></td></tr>
	<tr><td style="position:absolute;left:63%;"><input type="submit" name="submit" value="Add Comment"></td></tr></table>
	</div>
	</form>

<div id='my_page2' style="z-index: 2; position: absolute; left: 82%; top: 200px; background-color: #cccc33; width: 13%; padding: 10px; color: white; border: #0000cc 2px dashed; ">
 Recent Aricles  <br>
 <hr>
 <a href="writearticle.php"> Submit your own article </a>
<table>
<?php 
$link_id=mysql_connect("localhost","root","");
    mysql_select_db('forumtopics_db');
    $result=mysql_query("SELECT Heading from blog where published=1 order by Date desc limit 0,10",$link_id);
	$x=0;
	while($x < mysql_numrows($result))
    {
	$topic=mysql_result($result,$x,'Heading');
	$x++;
    ?>
      <tr><td><hr/><a href="view.php?topic=<?php echo $topic; ?>" style="text-decoration:none;" > <?php echo $topic; ?> </a></td></tr>	
     <?php
    }
    ?>
	</table>

</div> 
</body>
</html>