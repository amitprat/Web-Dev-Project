<?php
include 'database.php';
session_start();
$_SESSION['uname']="Saurabh";
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
function cancel()
{
document.all['post'].style.display="none";
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
	<div id="headerpic"></div>
	<div id="menu">
		<!-- HINT: Set the class of any menu link below to "active" to make it appear active -->
		<ul>
			<li><a href="../index.html" >Home</a></li>
			<li><a href=../blog/blog.php class="active">Blog</a></li>
			<li><a href=../forum/topic.php>Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			<li><a href=../discuss/discuss1.php>Discuss</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
<div id="content">

		<!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		
			<h3><strong>Recently Posted Blog</strong></h3><br/><br/>
			<div class="contentarea">
		
				<!-- Normal content area start -->
					<?php
						include '../database.php';
						$link_id=mysql_connect($host,$user,$password);
						mysql_select_db($database);
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
      <h4><?php echo $topic; ?> </h4>
	  <p> <?php echo $article; ?> </p>
	  <p> Written by <?php  echo $author; echo " on ";echo $Date; ?> </p>
		</table>
	
     <?php
    }
    ?>
	
	<br/>
	<br/>
	<br/>
	<form name="form1"  action="send.php">
   <input  type='button' name='submit1' value='Post Your Comment' style="position:relative; left:0%;" onclick="reply()"><br><br><br>
    <div id="post" style="display:none;">
	<label style="position:absolute;left:20%;">&nbsp; &nbsp &nbsp Write your Comments here</label><br><br>
	<table width="60%" cellpadding="2" cellspacing="1" align="center"><tr><td>
	<textarea name="textarea1" rows="7" cols="70" ></textarea></td></tr>
	<tr><td style="position:relative;left:100%;"><input type="submit" name="submit" value="Post">
	<input type="button" name="submits" value="Cancel" onclick="cancel()">
	
	</td></tr></table><br/>
	</div>
	</form>
	

	
	<!-- Normal content area end -->
	<div id="secondarycontent">
			<!-- Secondary content area start -->
			
			<!-- HINT: Set any div's class to "box" to encapsulate it in (you guessed it) a box -->
			<div class="box">
					<h4> Related Articles</h4>
					<hr>
					
					<div class="contentarea">
						<img src="images/pic2.jpg" class="left" alt="Random foliage" />
						<p><table>
						<tr><td><a href="writearticle.php"> Submit your own article </a></td>
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
			</div>
		</div>
		

		
<div class="divider1"></div>
	<div id="primarycontainer">
			<div id="primarycontent">
				<h4>Comments</h4>
				<!-- Primary content area start -->
					<?php
						include '../database.php';
						$link_id=mysql_connect($host,$user,$password);
						mysql_select_db($database);
						$u=$_SESSION['blog_id'];
						$result=mysql_query("SELECT comments,date,author from blogpost where blog_id=$u order by date desc",$link_id);
						$x=0;
						while($x < mysql_numrows($result))
							{
								$comments=mysql_result($result,$x,'comments');
								$date=mysql_result($result,$x,'date');
								$author=mysql_result($result,$x,'author');
								$x++;
					?>
					<div class="post">
						<div class="contentarea">
							<div class="details">Posted by <a href="#"> <?php  echo $author;?> </a>on <?php echo $date; ?>   </div>
							<img src="images/pic3.jpg" class="left" alt="A chinese dragon" />	
							<p> <?php echo $comments; ?><br/> <br/><ul class="controls">
							<br>
						</div>
					</div>
						<div class="divider2">
						</div>
					<?php
						}
					?> 		
			</div>
	

		
	</div>
</div>
<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>
</body>
</html>