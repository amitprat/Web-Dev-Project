<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
include '../database.php';
?>
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />

<!-- dd menu -->
<script type="text/javascript">
<!--
var timeout         = 500;
var closetimer		= 0;
var ddmenuitem      = 0;
 
// open hidden layer
function mopen(id)
{	
	// cancel close timer
	mcancelclosetime();

	// close old layer
	if(ddmenuitem) ddmenuitem.style.visibility = 'hidden';

	// get new layer and show it
	ddmenuitem = document.getElementById(id);
	ddmenuitem.style.visibility = 'visible';

}
// close showed layer
function mclose()
{
if(flag!=1)
	if(ddmenuitem) ddmenuitem.style.visibility = 'hidden';
}

// go close timer
function mclosetime()
{
	closetimer = window.setTimeout(mclose, timeout);
}

// cancel close timer
function mcancelclosetime()
{
	if(closetimer)
	{
		window.clearTimeout(closetimer);
		closetimer = null;
	}
}

// close layer when click-out
document.onclick = mclose; 
// -->

</script>

<script type="text/javascript">
function open1()
{
div3.style.visibility = "visible";
var flag=1;
}
function close1()
{ flag=0;
div3.style.visibility = "hidden";
}
function open2()
{
div4.style.visibility = "visible";
var flag=1;
}
function close2()
{ flag=0;
div4.style.visibility = "hidden";
}
function exp()
{ 
exp1.style.visibility = "visible";
}
function expout()
{ 
exp1.style.visibility = "hidden";
}

function disover()
{ 
dis.style.visibility = "visible";
}
function disout()
{ 
dis.style.visibility = "hidden";
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
	include 'login/form.php';
	$forms=new forms();
	$forms->show();
	?>
	


	<div id="headerpic"></div>

	
	<div id="menu">
		<!-- HINT: Set the class of any menu link below to "active" to make it appear active -->
		<ul>
			<li><a href="#" class="active">Home</a></li>
			<li><a href=blog/blog.php>Blog</a></li>
			<li><a href=group/first.php>Group</a></li>
			<li><a href="#"onmouseover="mopen('m3')" onmouseout="mclosetime()">Discussion</a><li>
			
		<!-- HINT: about the submeu of discussion tag -->	
		<div id="m3" onmouseover="mcancelclosetime()" onmouseout="mclosetime()">
        <ul style="position:absolute; left:140px; top:0px;"><li style=" float:left; margin-left:0px;">	
		<a href="#" style="display:block;" onmouseover="open1()" onmouseout="close1()">Discussion With Colleagues</a>
		<div id="div3" style="visibility:hidden; top:28px;" onmouseover="open1()" onmouseout="close1()">
		<a href="#" style="display:block;">Recently Posted Forums</a>
		
		
		
		<!-- HINT: About database.............................................................................-->	
		<?php
                include '../database.php';
		$link_id=mysql_connect('sql208.1free.ws','freew_3422045','lakshya');
		mysql_select_db('freew_3422045_zinedu');
		$result=mysql_query("SELECT subject from forum1 order by date desc limit 0,4",$link_id);
		$x=0;
	while($x< mysql_num_rows($result))
		{
	$subject=mysql_result($result,$x,'subject');
        //extra
	$result1=mysql_query("select topic from forum1 where subject='$subject'",$link_id);
	$y=mysql_result($result1,0,'topic');
	//extra ends here   
	$x++;
	$z='|';
    ?>
	
	  <a href="forum/reply.php?subject=<?php echo $subject.$z.$y; ?>" style="text-decoration:none;display:block;" > <?php echo $subject;?> </a>
	
	 
     <?php
    }
	?>
	<!-- HINT: About database ends here................. -->		
		
		
		
		
		<a href="forum/topic.php" style="display:block;">Get More Forums......>></a>
		</div>
		</li></ul>
		
		 <ul style="position:absolute; left:290px; top:0px;"><li>	
		<a href="#" style="display:block;"  onmouseover="open2()" onmouseout="close2()">Discussion With Experts</a>
		<div id="div4" style="visibility:hidden; top:28px;" onmouseover="open2()" onmouseout="close2()">
		
		<a href="#" style="display:block;" onmouseover="exp()" onmouseout="expout()">Select Type of Experts</a>
		<div id="exp1" style="visibility:hidden; z-index:0; left:163px; top:0px;" onmouseover="exp()" onmouseout="expout()">
		<a href="#" style="display:block;">Discuss by email with IIT Alumni </a>
		<a href="#" style="display:block;">discuss by e-mail with current IIT students </a>
		<a href="#" style="display:block;">discuss by phone with IIT Alumni </a>
		<a href="#" style="display:block;">discuss by e-mail with current IIT students </a>
		</div>
		<a href="#" style="display:block;top:auto+100px;" onmouseover="disover()" onmouseout="disout()">Select a topics of Discussion</a>
		<div id="dis" style="visibility:hidden; z-index:1;left:163px; top:31px;" onmouseover="disover()" onmouseout="disout()">
		<a href="#" style="display:block;">Doubts in Physics </a>
		<a href="#" style="display:block;">Doubts in Chemistry </a>
		<a href="#" style="display:block;">Doubts in Maths </a>
		<a href="#" style="display:block;">Doubts regarding preparation for JEE </a>
		<a href="#" style="display:block;">Gathering information about IITs </a>
		</div>
		<a href="query/submitqueryform.php" style="display:block;">Submit a query</a>
		
		
		</div>
		
		</li></ul>
		
		</div>
			
			
		<!-- submeu of discussion tag eds here................ -->	
			
			<li><a href="login.php"> Login</a>
			<li><a href=chat/index.php>Chat</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
<object align="right" type="application/x-shockwave-flash" data="http://www.oneplusyou.com/bb/files/countdown/countdown.swf?co=FF6B4D&bgcolor=FFFFFF&date_month=04&date_day=11&date_year=1&un=IIT JEE&size=small&mo=04&da=11&yr=2010" width="188" height="60"><param name="movie" value="http://www.oneplusyou.com/bb/files/countdown/countdown.swf?co=FF6B4D&bgcolor=FFFFFF&date_month=04&date_day=11&date_year=1&un=IIT JEE&size=small&mo=04&da=11&yr=2010" /><param name="bgcolor" value="#FFFFFF" /></object><img src="http://www.oneplusyou.com/q/img/bb_badges/countdown.jpg" alt="" style="display: none;" height="1" width="1" />

	
	<div id="content">

		<!-- Normal content: Stuff that's not going to be put in the left or right column. -->
		<div id="normalcontent">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea">
				<!-- Normal content area start -->

				<img src="images/pic1.jpg" class="left" alt="A chinese lion statue" />
				<p>Zinedu is an initiative by ......
				<!-- Normal content area end -->
			</div>
		</div>

	
		<div class="divider1"></div>


		<!-- Primary content: Stuff that goes in the primary content column (by default, the left column) -->
		<div id="primarycontainer">
			<div id="primarycontent">
				<!-- Primary content area start -->
		
				<div class="post">
					<h4>Activity1 at Zinedu</h4>
					<div class="contentarea">
						<div class="details">Posted by <a href="#">Saurabh </a> on August 25, 2009</div>
						<img src="images/pic3.jpg" class="left" alt="A chinese dragon" />						
						<p> Here is some place for the latest activity like latest news, Article with a link to read more e.t.c<br/> <br/><ul class="controls">
							<li><a href="#" class="printerfriendly">Printer Friendly</a></li>
							<li><a href="#" class="comments">Comments (18)</a></li>
							<li><a href="#" class="more">Read More</a></li>				
						</ul>
					</div>
				</div>
		
				<div class="divider2"></div>
		
				<div class="post">
					<h4>Activity2 at Zinedu</h4>
					<div class="contentarea">
						<div class="details">Posted by <a href="#">Amit Pratap</a> on August 25, 2009</div>	
						<img src="images/pic3.jpg" class="left" alt="A chinese dragon" />
						<p>Here is some place for the latest activity like latest news, Article with a link to read more<ul class="controls">
							<li><a href="#" class="printerfriendly">Printer Friendly</a></li>
							<li><a href="#" class="comments">Comments (18)</a></li>
							<li><a href="#" class="more">Read More</a></li>				
						</ul>
					</div>
				</div>
	
				<div class="divider2"></div>
		
				<div class="post">
					<h4>Activities at Zinedu</h4>
					<div class="contentarea">
						<div class="details">Posted by <a href="#">Saurabh</a> on August 25, 2009</div>	
						<img src="images/pic3.jpg" class="left" alt="A chinese dragon" />
						<p><ul class="controls">
							<li><a href="#" class="printerfriendly">Printer Friendly</a></li>
							<li><a href="#" class="comments">Comments (18)</a></li>
							<li><a href="#" class="more">Read More</a></li>				
						</ul>
					</div>
				</div>

				<!-- Primary co			