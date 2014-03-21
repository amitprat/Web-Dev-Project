<?php
session_register('uname');
session_register('count');
$_SESSION['uname']="amit";

?>
<?php
include '../database.php';
if(isset($_POST['posted']))
{	global $abusive;
	$con1=$_POST['textarea'];
	$con2=$_POST['textfield1'];
	$con=$con1." ".$con2;
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
	$topic=$_POST['select1'];
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
			<li><a href="../index.html" >Home</a></li>
			<li><a href=../blog/blog.php>Blog</a></li>
			<li><a href=topic.php class="active">Forum</a></li>
			<li><a href=../group/first.php>Group</a></li>
			<li><a href=../discuss/discuss1.php>Discuss</a></li>
			<li><a href="#">Contact</a></li>
		</ul>
	</div>
	<div id="menubottom"></div>
	
		<div id="content">

<div id="normalcontent">
			<h3><strong>Zinedu </strong><span>Personal Preparation and Personal Guide</span></h3>
			<div class="contentarea">
				<!-- Normal content area start -->

				<img src="images/pic1.jpg" class="left" alt="A chinese lion statue" />
				<form id="form1" name="form1" method="post" action="" ><br><br><br><br>
 <input type="hidden" name="posted" value="true">
   <table></td>
  <tr> <td>   Select Topic: </td>
    <td><select name="select1">
   	<?php
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
    $result=mysql_query("SELECT distinct topic from topics",$link_id);
	while($query_data=mysql_fetch_row($result))
    {
     ?>
       <option><?php echo "$query_data[0]" ?></option>
     <?php

    }
    ?>
</select></td></tr>
 <tr><td>  Subject:</td>
  <td>  <input type="text" name="textfield1" maxlength="100"  /></td></tr>

 <tr><td>   Post:</td></tr>
  <tr><td> </td> 
  <td> <textarea name="textarea" rows="14" cols="50" ></textarea></td></tr></table>
	 
   <p style="position:absolute;left:51%;">
	<input type="submit" name="submit1" value="Submit" />
	</p>
	<?php
		if($abusive=="Abuse")
	echo " contents can't be posted"; ?>
	<p>&nbsp;</p>
	<br>
	</form>
				
				<!-- Normal content area end -->
			</div>
		</div>
		<div class="divider1"></div>
		
	
	<div id="primarycontainer">
			<div id="primarycontent">
				<!-- Primary content area start -->
		
				<div class="post">
					<h4>Forums</h4>
					<table width="100%" border="1" cellpadding="2" cellspacing="1" align="center" style="margin-left:90px;" >
      <tr>
       
        <td >Topic</td>
        <td align="center">Total Number of Posts</td>
        <td align="center">Last Posted</td>
		<td align="center"> Last Post by </td>
      </tr> 
	<?php
	

	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
    $result=mysql_query("SELECT topic,Numberofpost,Date,Lastpostby from topics order by Date desc",$link_id);
	$x=0;
	while($x < mysql_numrows($result))
    {
	$topic=mysql_result($result,$x,'topic');
	$Numberofpost=mysql_result($result,$x,'Numberofpost');
	$Date=mysql_result($result,$x,'Date');
	$Lastpostby=mysql_result($result,$x,'Lastpostby');
	$x++;
    ?>
      <tr>
	  <td> <a href="sub.php?topic=<?php echo $topic; ?>" style="text-decoration:none;" > <?php echo $topic; ?> </a></td>
	  <td align="center"> <?php echo $Numberofpost; ?> </td>
	  <td align="center"> <?php echo $Date; ?> </td>
	  <td align="center"> <?php echo $Lastpostby; ?> </td>
	  </tr>
     <?php
    }
    ?>
	</table>
					</div>
				</div>
			</div>
	</div>
	<div id="footer">
			<div class="left">&copy; 2009 Zinedu. All rights reserved.</div>
			
	</div>

 
	
</body>
</html>


