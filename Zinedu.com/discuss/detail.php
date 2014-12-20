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
document.all['submit1'].style.display="none";
}
function cancel()
{
document.all['post'].style.display="none";
document.all['submit1'].style.display="";
}
</script>
</head>
<body><div id="upbg"></div>

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
			<li><a href="../index.html" >Home</a></li>
			<li><a href="#" class="active">Discuss With Collegues</a></li>
			<li><a href="askexpert.php">Discuss With Experts</a></li>
			<li><a href=../blog/blog.php>Blog</a></li>
			<li><a href=../forum/topic.php>Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			
		</ul>
	</div>
	<div id="menubottom"></div>
	<div id="content"><!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		<div id="normalcontent">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea">
			<img src="images/pic1.jpg" class="left" alt="A chinese lion statue" /> 
				<!-- Normal content area start --><br><br><br><br>
				<h4>  </h4><br><br>
				
				<!-- Normal content area end -->
			</div>
		</div>
		<!-- Primary content: display the posts by the user ordered by date (by default, the left column) -->
	<?php
			include '1.php';
			$a=new search();
			$a->detail();
	?>
	<form name="form1"  action="send.php">
	<div id="hide" >
   <input  type='button' name='submit1' value='Post Your Comment' style="position:relative; left:0%;" onclick="reply()"><br><br><br></div>
    <div id="post" style="display:none;">
	<label style="position:absolute;left:20%;">&nbsp; &nbsp &nbsp Write your Comments here</label><br><br>
	<table width="60%" cellpadding="2" cellspacing="1" align="center"><tr><td>
	<textarea name="textarea1" rows="7" cols="70" ></textarea></td></tr>
	<tr><td style="position:relative;left:100%;"><input type="submit" name="submit" value="Post">
	<input type="button" name="submits" value="Cancel" onclick="cancel()">
	
	</td></tr></table><br/>
	</div>
	</form>
	<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>

				</body>
				</html>