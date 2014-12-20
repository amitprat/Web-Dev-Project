<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />
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
			<li><a href="#">Contact Us</a></li>
			
		</ul>
	</div>
	<div id="menubottom"></div>
	<div id="content"><!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		<div id="normalcontent">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea">
				<!-- Normal content area start -->
				<h4>  </h4><br>
				<h4><p>Discussions Started by you &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp;<a href=../discuss/newtopic.php>Create a New topic of Discussion</a></p></h4>
				<?php 
						include '1.php';
						$s=new search();
						$s->f(1);
					?>
				
				<!-- Normal content area end -->
			</div>
		</div>
		<!-- <div class="divider1"></div> -->
		<!-- Primary content: display the posts by the user ordered by date (by default, the left column) -->
		
		
		<div id="normalcontent">
			<div class="contentarea">
				<!-- Normal content area start --><br><br><br><br>
				<h4> Latest 10 Discussions started by others</h4><br>
				
			<div id="primarycontent">
					<?php 
					$s->g(0);
					
					?>
			</div>
		</div>
		<div id="normalcontent">
			<div class="contentarea">
				<!-- Normal content area start --><br><br><br><br>
				<h4>Discussions You Already Replied To</h4><br>
			
		
					<?php 
					$s->g(0);
					
					?>
			</div>
		</div>
		
				
				<!-- Normal content area end -->
		
	</div>
	</div>
	</div>
	</div>
		

<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>
	</body>
	</html>