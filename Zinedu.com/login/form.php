
<?php
	class forms{
	function show()
	{
			session_start();
			
		if(isset($_SESSION['username']))
			{
			?>
				<form method="post" action="login/logout.php">
				<div id="search">
				<input type="submit" class="submit" value="Logout" />
				<input type="hidden" value="0" id="hiddenvalue">
				</div>
				</form>
			<?php
			echo "Welcome ".$_SESSION['username'];
			}
		else
			{
			?>
				<form method="post" action="login/trial.php">
				<div id="search">
				<input type="text" class="text" maxlength="64" name="username" value="Username" onfocus="this.value=''" onblur="if(!this.value) {this.value='Username'}"/>
				<input type="text" class="text" maxlength="64" name="password" value="Password" onfocus="{this.value=''; this.type='Password'}" />
				<input type="submit" class="submit" value="Login" />
				<input type="hidden" value="1" id="hiddenvalue">
				</div>
				</form>
			<?php
			}
	}
	}
?>