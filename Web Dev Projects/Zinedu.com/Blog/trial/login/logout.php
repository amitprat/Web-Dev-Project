<?php
session_start();
//you can remove a single variable in the session 
unset($_SESSION['username']); 

// or this would remove all the variables in the session, but not the session itself 
session_unset(); 

// this would destroy the session variables 
session_destroy(); 
echo $_SESSION['username'];
header( 'Location: http://localhost/zinedu.com/index.php' ) ;
?>