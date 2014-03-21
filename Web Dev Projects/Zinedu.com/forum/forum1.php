<?php
session_register('uname');
session_register('count');
$_SESSION['uname']="amit";
$i=$_SESSION['count'];
if(isset($_POST['submit1']))
{
	$link_id=mysql_connect($host,$user,$password);
    mysql_select_db('forumtopics_db');
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
    mysql_select_db('forumtopics_db');
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
	<table width="685" height="231" border="1" cellpadding="2" cellspacing="1">
      <tr>
        <td height="31">Topics</td>
        <td>Subjects</td>
        <td>Auther</td>
        <td>Posts</td>
      </tr>
      <tr>
        <td>
		<?php
		$result2=mysql_query("SELECT topic from forum1 limit $i,5",$link_id);
	    while($query_data=mysql_fetch_row($result2))
       {
	     echo "<p>";
        echo "$query_data[0]";
		echo "</p>";
	   }
      ?>
		</td>
        <td>
		<?php
		$result3=mysql_query("SELECT distinct subject from forum1 limit $i,5",$link_id);
	    while($query_data=mysql_fetch_row($result3))
       {
	     echo "<p>";
		 ?><a href="report.php"><?php
        echo "$query_data[0]";?></a><?php
		echo "</p>";
	   }
        ?>
		</td>
		<td>
		<?php
		$result4=mysql_query("SELECT auther from forum1 limit $i,5",$link_id);
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
		$count=mysql_query("select noofposts ,count(*) from forum1 limit $i,5",$link_id);
		while($query_data=mysql_fetch_row($count))
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
 <?php
 
 $_SESSION['count']=$_SESSION['count']+5;


?>

</body>
</html>
