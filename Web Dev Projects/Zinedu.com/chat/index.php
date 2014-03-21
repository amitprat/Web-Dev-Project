<?php
include_once('PHPSimpleChat.php');
session_start();
$user=$_SESSION['username'];
$mychat = new PHPSimpleChat();
?>

<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />
<?php $mychat->displayHeader(); ?>

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
			<li><a href="../index.php" >Home</a></li>
			<li><a href=../blog/blog.php>Blog</a></li>
			<li><a href=../forum/topic.php>Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			<li><a href=../discuss/discuss1.php>Discuss</a><li>
			<li><a href="login.php"> Login</a>
			<li><a href=chat/index.php class="active" >Chat</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
	<div id="content">

		<!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		<div id="normalcontent" align="Center">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea" >
				<!-- Normal content area start -->
						<?php $mychat->displayChat(); ?>
				<!-- Normal content area end -->
			</div>
		</div>
	</div>
	
	<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div><br/><br/><br/>
			
	</div>

	

</body>
</html>