<?php
if (!isset($_COOKIE['name'])) {

  setcookie("name", $_POST['name']);
  setcookie("visits", 1);

  echo "Hello $_POST[name]. &lt;br /&gt;";
  echo "It appears that this is your first visit!";

} else {
  setcookie("visits", ++$_COOKIE["visits"]);

  echo "Welcome back, $_COOKIE["name"]. &lt;br /&gt;";
  echo "You have visited us $_COOKIE['visits'] times!";
}
?>