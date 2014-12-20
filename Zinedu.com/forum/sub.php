<?php
include '../database.php';
session_register('uname');
session_register('count');
session_register('topic');
$_SESSION['uname']="amit";
$_SESSION['topic']=$_GET['topic'];
if(isset($_POST['submit1']))
{
$con1=$_POST['textarea'];
	$con2=$_POST['textfield1'];
	$con=$con1." ".$con2;
	$list=explode(" ",$con);
	global $abusive;
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
	$topic=$_GET['topic'];
	$sub=$_POST['textfield1'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	$result1=mysql_query("INSERT INTO forum1 (topic,subject,post,author) values('$topic','$sub','$post','$name')",$link_id);
	$sql = "select Numberofpost from topics where topic='$topic'";
	$result=mysql_query($sql);
	$x=0;
	$no=mysql_result($result,$x,'Numberofpost');
	$xx=$no;
	$no++;
	$sql ="UPDATE topics SET Numberofpost='$no' WHERE topic='$topic'";
    $result = mysql_query($sql);}
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
			<li><a href=../index.html >Home</a></li>
			<li><a href=../blog/blog.php>Blog</a></li>
			<li><a href=../forum/topic.php class="active">Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			<li><a href=../login.php> Login</a>
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
				<form id="form1" name="form1" method="post" action=""><br><br>
				<table></td>
					<tr> <td>Selected Topic: </td>
						<td> <textbox name="topic"> <?php echo $_GET['topic']; ?> </textbox> </td>
						</tr>
						<tr><td>  Subject:</td>
  <td>  <input type="text" name="textfield1" maxlength="100"  /></td></tr>

 <tr><td>   Post:</td>
  <tr><td> </td> 
  <td> <textarea name="textarea" rows="14" cols="50" ></textarea></td></tr></table>
   <p style="position:absolute;left:53%;">
	<input type="submit" name="submit1" value="Submit" />
	</p>
	</form>
				
				<!-- Normal content area end -->
			</div>
		</div>
		<div id="primarycontainer">
			<div id="primarycontent">
				<!-- Primary content area start -->
		
				<div class="post">
					<h4>Related Posts</h4>
					<div class="contentarea">
						<?php 
		if($abusive=="Abuse")
	echo $abusive." contents can't be posted";
		?>
	<p >&nbsp;</p>
	<div id="primarycontainer">
			<div id="primarycontent">
				<!-- Primary content area start -->
		
				<div class="post">
				<div class="contentarea">
	 <table width="60%" border="1" cellpadding="2" cellspacing="1" style="position:absolute;left:12%;"> 
      <tr>
       
        <td >Subject</td>
        <td >Post</td>
        <td >Posted on</td>
		<td>  Author</td>
      </tr> 
	<?php
		include '../database.php';
		$y=$_GET['topic'];
		$link_id=mysql_connect($host,$user,$password);
		mysql_select_db($database);
		$result=mysql_query("SELECT subject,post,author,date from forum1 where topic='$y' order by date desc",$link_id);
		$x=0;
	while($x<mysql_numrows($result))
		{
	$subject=mysql_result($result,$x,'subject');
	$date=mysql_result($result,$x,'date');
	$post=mysql_result($result,$x,'post');
	$author=mysql_result($result,$x,'author');
	$x++;
	$z='|';
    ?>
      <tr>
	  <td> <a href="reply.php?subject=<?php echo $subject.$z.$y ?>" style="text-decoration:none;" > <?php echo $subject; ?> </a></td>
	  <td> <?php echo $post; ?> </td>
	  <td> <?php echo $date; ?> </td>
	  <td> <?php echo $author; ?> </td>
	  </tr>
     <?php
    }
    ?>
	    
	</ul>
		</div>
			<div id="footer">
				<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			</div>
			
	
		
 
	
</body>
</html>
