<?php 
	include '1.php';
	$users=new user();
	$username=$_POST['username'];
	$password=$_POST['password'];
	if($users->checkuser($username))
		{
			if($users->checkpassword($username,$password))
			{
			session_start();
			$_SESSION['username']=$username;
			//$users->setactiveuser($username);
			header( 'Location: http://localhost/zinedu.com/index.php' ) ;
			}
			else
			echo 'wrong password';
		}
	else
		echo 'Username does not exists';	
?>