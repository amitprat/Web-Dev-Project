<?php
include '../database.php';
session_register('uname');
session_register('count');
$_SESSION['uname']="amit";
if(isset($_POST['submit1']))
{
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
	$topic=$_POST['select1'];
	$sub=$_POST['textfield1'];
	$post=$_POST['textarea'];
	$name=$_SESSION['uname'];
	$count="select subject,count(*) from forum1";
	$result1=mysql_query("INSERT INTO forum1 (topic,post,subject,auther,noofposts) values('$topic','$post','$sub','$name','$count')",$link_id);
}

?>




<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Untitled Document</title>
<style type="text/css">
<!--
.style1 {font-family: Calibri}
-->
</style>
<script language="javascript">
function fun1()
{
windows.alert("gandu");
}
</script>
</head>

<body>
 <form id="form1" name="form1" method="post" action="">
    <label> Zinedu -IT</label>
  <p>Select Topic:     
    <select name="select1">
   	<?php
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db($database);
    $result=mysql_query("SELECT distinct topics from topics",$link_id);
	while($query_data=mysql_fetch_row($result))
    {
     ?>
       <option><?php echo "$query_data[0]" ?></option>
     <?php

    }
    ?>
</select>
  </p>
  <p>Subject:   
    <input type="text" name="textfield1" maxlength="100"  />
  </p>
 <p>Post:
     <textarea name="textarea" rows="14" cols="70" ></textarea>
	 
   <p>
	<input type="submit" name="submit1" value="Insert" />
   </p>
	<p>&nbsp;</p>
	<p>Forums</p>
	<table  border="1" cellpadding="2" cellspacing="1" width="75%">
      <tr>
        
        <td width="20%" >Auther</td>
        <td  width="60%">Post</td>
        <td  width="20%">Date</td>
      </tr>
      <tr>
       <td>
		<?php
		$result4=mysql_query("SELECT auther from forum1",$link_id);
	    while($query_data=mysql_fetch_row($result4))
       {
	     echo "<p>";
        echo "$query_data[0]";
		echo "</p>";
	   }
        ?>
		</td>
		<td>
		<?php
		$result4=mysql_query("SELECT post from forum1",$link_id);
	    while($query_data=mysql_fetch_row($result4))
       {
	     echo "<p>";?><a href="report.php" style="text-decoration:none;"><?php
        echo "$query_data[0]";?></a><?php
		echo "</p>";
	   }
        ?>
		</td>
        <td>
		<?php
		$date=mysql_query("select date from forum1",$link_id);
		while($query_data=mysql_fetch_row($date))
		{
		echo "<p>";
		echo "$query_data[0]";
		echo "</p>";
		}
		?>
		
		</td>
      </tr>
   </table>
	<p>
	 
	</p>
 </form>
 <p>


</body>
</html>
