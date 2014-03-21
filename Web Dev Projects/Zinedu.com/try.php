<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<title>Zinedu</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link rel="stylesheet" type="text/css" href="default.css" />
<script language="javascript" src="login.js"></script>
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
	<!-- To Check The login Status-->
	<input type="hidden" value="<? echo $_SESSION['username'] ?>" name="sessionvalue" >
	<script type="text/javascript" language="javascript">
	document.write(sessionvalue.value);
	</script>
	<?php  session_start(); echo '$_SESSION["username"]'; ?>
	


	<div id="headerpic"></div>

	
	<div id="menu">
		<!-- HINT: Set the class of any menu link below to "active" to make it appear active -->
		<ul>
			<li><a href="#" class="active">Home</a></li>
			<li><a href=blog/blog.php>Blog</a></li>
			<li><a href=group/first.php>Group</a></li>
                         <li><a href="calendar/calander/public_files/calendar.php">Reminder</a></li>
                         <li><a href="test">Test</a></li>
			<li><a href="#"onmouseover="mopen('m3')" onmouseout="mclosetime()">Discussion</a><li>
			
		<!-- HINT: about the submeu of discussion tag -->	
		<div id="m3" onmouseover="mcancelclosetime()" onmouseout="mclosetime()">
        <ul style="position:absolute; left:140px; top:0px;"><li style=" float:left; margin-left:0px;">	
		<a href="#" style="display:block;" onmouseover="open1()" onmouseout="close1()">Discussion With Colleagues</a>
		<div id="div3" style="visibility:hidden; top:28px;" onmouseover="open1()" onmouseout="close1()">
		<a href="#" style="display:block;">Recently Posted Forums</a>
		
		
		
		<!-- HINT: About database.............................................................................-->	
		<?php
		include 'database.php';
		$link_id=mysql_connect($host,$user,$password);
		mysql_select_db($database);
		$result=mysql_query("SELECT topic,Numberofpost from topics order by Date desc limit 0,4",$link_id);
		
	
		$x=0;
		while($x < mysql_numrows($result))
		{
			$topic=mysql_result($result,$x,'topic');
			$Numberofpost=mysql_result($result,$x,'Numberofpost');
			$x++;
		?>
	   <a href="forum/sub.php?topic=<?php echo $topic; ?>" style="text-decoration:none;" > <?php echo $topic; echo '&nbsp'; echo '('; echo $Numberofpost; echo ')' ?> </a><br/><br/>
     <?php
    }
    ?>
	<!-- HINT: About database ends here................. -->		
		
		
		
		
		<a href="forum/topic.php" style="display:block;">Get More Forums......>></a>
		</div>
		</li></ul>
		
		 <ul style="position:absolute; left:290px; top:0px;"><li>	
		<a href="#" style="display:block;"  onclick="showlogin()">Discussion With Experts</a>
		
		</li></ul>
		
		</div>
			
			
		<!-- submeu of discussion tag ends here................ -->	
			
		
			<li><a href=chat/index.php>Chat</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
<object align="right" type="application/x-shockwave-flash" data="http://www.oneplusyou.com/bb/files/countdown/countdown.swf?co=FF6B4D&bgcolor=FFFFFF&date_month=04&date_day=11&date_year=1&un=IIT JEE&size=small&mo=04&da=11&yr=2010" width="188" height="60"><param name="movie" value="http://www.oneplusyou.com/bb/files/countdown/countdown.swf?co=FF6B4D&bgcolor=FFFFFF&date_month=04&date_day=11&date_year=1&un=IIT JEE&size=small&mo=04&da=11&yr=2010" /><param name="bgcolor" value="#FFFFFF" /></object><img src="http://www.oneplusyou.com/q/img/bb_badges/countdown.jpg" alt="" style="display: none;" height="1" width="1" />

	
	<div id="content">
       <!--login popup starts here-->
       <div id="popup">

       
                               <form name="login" action="login/trialloginexperts.php" method="post">
                             <table align="right" name="login">
                             <tr>
                            <td align="center" valign="middle" >&nbsp;</td><td align="right" valign="top"><a href="#" onclick="hidelogin()"><img src="images/exit.jpg" width="20" height="20"></a></td>
                              </tr>
                            <tr>
                             <td>UserName : </td><td><input type="text" class="text" maxlength="64" name="username" value="Username" onfocus="this.value=''" onblur="if(!this.value) {this.value='Username'}"/></td>
                              </tr>
                              <tr>
                               <td>Password :</td><td><input type="text" class="text" maxlength="64" name="password" value="Password" onfocus="{this.value=''; this.type='Password'}" /></td></tr>
                            <tr><td>&nbsp;</td><td><input type="submit" class="submit" value="Login" /></td></tr>
                           </table>

                              <?php 	
							  session_start();
			
		           if(isset($_SESSION['username']))
			    {
			      ?>
				<form method="post" action="login/logout.php">
				<div id="search">
				<input type="submit" class="submit" value="Logout" />
				</div>
				</form>
			   <?php
			  echo "Welcome ".$_SESSION['username'];
			   }
			   ?>
                             </form>
                               
                                
					
	</div>
	<!--Login popup ends here -->

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
			</div>
		</div>
	</div>
	
	</html>
	
	
	
	
	
	
		

					