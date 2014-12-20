<html>
<body>
<?php
session_register('cout');
//$_SESSION['cout']=0;
echo "hi.............".$_SESSION['cout']."<br>";
if(isset($_POST['posted']))
{
$number=rand(1,10);
$gad="gand";
$con1=$_POST['guess'];
$con2="sds";
$con=$con1.$con2;
echo $con;
$list=explode(" ",$con);
$abusive="welcome";
do
{
$abuse=array("chut","lund","gand","fuck","muth","londa","loda","jhant","machod","matherchod","motherfucker","sisterfucker","bhenchod","betichod");
while(list($key,$abus)=each($abuse))
if(($list[key($list)])==$abus)
$abusive="abuse"; }while(next($list));
if($abusive=="abuse")
echo "abuse";
else echo "lund";
if(submitted_value==$gad)
echo "gand";
echo "posted";

if($_POST['guess']>$number){
echo "toolarge";
}
else if($_POST['guess']<number){
echo "small";
}
else if($_POST['guess']=="")
{echo "u win";
}
}
$_SESSION['cout']=90;
?>
<form method="post" action="#">
<input type="hidden" name="posted" value="true">
what number -------1-10 
<input name="guess" type="text">
<br><br>
<input type="submit" name="submit" value="submit">

</form></body></html>
<?php
if($abusive=="abuse")
echo "Abusive contents can't be post";
echo $_SESSION('cout');
?>