<?php
include '../database.php'
if(isset($_POST['submit']))
{
	$link_id=mysql_connect($host,$user,$password) or die("unable to connect");
    mysql_select_db($database) or die("unable to connect database");
	//$result1=mysql_query("update blog set published=1 where ,$link_id) or die("problem inserting");
}
?>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Zinedu Blog</title>
</head>
<body>
<h1 align="center"> Approve or Reject the Articles</h1>
<br><br> <br>


	<?php
	include '../database.php';
	$link_id=mysql_connect($host,$user,$password) or die("cannot connect");
    mysql_select_db($database) or die("cannot connect");
    $result=mysql_query("SELECT Heading,Article,author,date from blog where published=0 order by date",$link_id) or die("cannot connect");
	$x=0;
	$l=1;
	while($x < mysql_numrows($result))
    {
	$topic=mysql_result($result,$x,'Heading');
	$article=mysql_result($result,$x,'Article');
	$author=mysql_result($result,$x,'author');
	$date=mysql_result($result,$x,'date');
	$x++;
    ?>
	  <table width="60%" border="1" cellpadding="2" cellspacing="1"  align="center" >
	  <tr> <td>Written by: &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp <?php echo $author ?> </td></tr>
	  <tr> <td>Date:  &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp <?php echo $date; ?> </td></tr>
	  <tr> <td align="center"><?php echo $topic; ?> </td></tr>
	  <tr><td><?php echo $article; ?> </td></tr>
	  <tr><td><input type="submit" name="submit<?php echo $l++; ?>" value="Approve" >
	  <input type="submit" name="submit<?php echo $l++; ?>" value="Reject" ></tr>
	  </table>
	  <br> <br> <br> <br>
	  
     <?php
    }
    ?>
	
	</body>
	</html>