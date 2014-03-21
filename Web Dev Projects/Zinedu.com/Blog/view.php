<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<script language="javascript">
function reply()
{
document.all['post'].style.display = ""; 
}
</script>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />
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
			<li><a href=../index.html class="active">Home</a></li>
			<li><a href=blog/blog.php>Blog</a></li>
			<li><a href=forum/topic.php>Forum</a></li>
			<li><a href=group/first.php>Group</a></li>
			<li><a href=discuss/discuss1.php>Discuss</a><li>
			<li><a href="login.php"> Login</a>
			<li><a href="#">Contact</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
	<div id="content">

		<!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		<div id="normalcontent">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea">
				<!-- Normal content area start -->

				<img src="images/pic1.jpg" class="left" alt="A chinese lion statue" />
				
				<?php
					include '../database.php';
					$link_id=mysql_connect($host,$user,$password);
					mysql_select_db($database);
					$topic1=$_GET['topic'];
					$result=mysql_query("SELECT Heading,Article,author,date,blog_id from blog where Heading='$topic1' ",$link_id);
					$x=0;
					$topic=mysql_result($result,$x,'Heading');
					$article=mysql_result($result,$x,'article');
					$Date=mysql_result($result,$x,'date');
					$author=mysql_result($result,$x,'author');
					$_SESSION['blog_id']=mysql_result($result,$x,'blog_id');
					
				?>
					<h4><?php echo $topic ?></h4><br/><br/>
					<?php echo $article; ?> <br/>
					  Written by <?php  echo $author;echo $Date; ?> </td></tr>
					
				<!-- Normal content area end -->
			</div>
		</div>

<br/>
<br/>

		
		

	<br/>
	<br/>
	<br/>
	<form name="form1" method="POST" action="send.php">
   <input  type='button' name='button' value='Post Your Comment' style="position:relative; left:20%;" onclick="reply()"><br><br><br>
    <div id="post" style="display:none;">
	<label style="position:absolute;left:20%;">Write your Comments here:</label><br><br>
	<table width="60%" cellpadding="2" cellspacing="1" align="center"><tr><td>
	<textarea name="textarea1" rows="5" cols="70" ></textarea></td></tr>
	<tr><td style="position:absolute;left:63%;"><input type="submit" name="submit" value="Post"></td></tr></table>
	</div>
	</form>

<div id='my_page2' style="z-index: 2; position: absolute; left: 82%; top: 200px; background-color: #cccc33; width: 13%; padding: 10px; color: white; border: #0000cc 2px dashed; "> 
Recent Aricles  
<br>
 <hr>
 <a href="writearticle.php"> Submit your own article </a>
<table>
<?php 
	include '../database.php';
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
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
<?php
	include '../database.php';
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
	$u=$_SESSION['blog_id'];
    $result=mysql_query("SELECT comments,date,author from blogpost where blog_id=$u order by date",$link_id);
	$x=0;
	while($x < mysql_numrows($result))
    {
	$comments=mysql_result($result,$x,'comments');
	$date=mysql_result($result,$x,'date');
	$author=mysql_result($result,$x,'author');
	$x++;
    ?><br>
		<table align="center" width="60%" border="1">
		<tr><td> <?php  echo $author; echo $date; ?> </td></tr>
      <tr><td> <?php echo $comments; ?> </td></tr>
	  
		</table>
	<br> <br>
     <?php
    }
    ?> 
</body>
</html>