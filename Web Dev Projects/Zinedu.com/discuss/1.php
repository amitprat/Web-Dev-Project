<?php

class search
	{
		private $y;
		function f($y)
			{
				include '../database.php';
				session_start();
				$uname=$_SESSION['username'];
				$link_id=mysql_connect($host,$user,$password);
				mysql_select_db($database);
				$result=mysql_query("SELECT qid,subject,question,date from discuss_question where author='$uname'",$link_id);
				$x=0;
				while($x<mysql_numrows($result))
					{
						$subject=mysql_result($result,$x,'subject');
						$question=mysql_result($result,$x,'question');
						$date=mysql_result($result,$x,'date');
						$qid=mysql_result($result,$x,'qid');
						$x++;
						?>
								<!-- Primary content area start -->
		
									<div class="post">
										<h4><a href="detail.php?sub=<?php echo $subject;?>"><?php echo $subject; ?></a></h4>
											<div class="contentarea">
												<div class="details"> on <?php echo $date;?>
												</div>
																<p> <?php echo $question ?> <ul class="controls">
																<li> <a href="detail.php?sub=<?php echo $subject;?>" class="comments">Replies <?php $this->count_reply($qid); ?></a></li>
																<li><a href="askexpert.php" class="more">Ask Zinedu Experts</a></li>				
																</ul>
												
											</div>
									<div>	
								<div class="divider2"></div>									
						
						<?php
					}
			}
			function detail()
			{
				include '../database.php';
				$link_id=mysql_connect($host,$user,$password);
				mysql_select_db($database);
				$sub=$_GET['sub'];
				$result=mysql_query("SELECT qid,subject,question,date from discuss_question where subject='$sub' ",$link_id);
				$x=0;
				
				$qid=mysql_result($result,$x,'qid');
				session_start();
				$_SESSION['qid']=$qid;
				$sub=mysql_result($result,$x,'subject');
				$ques=mysql_result($result,$x,'question');
				$time=strtotime(mysql_result($result,$x,'date'));				
				echo '<strong>'; echo $sub;echo '&nbsp &nbsp';  echo'</strong>';echo date('d M y H:i:s',$time);	echo '<br>';
				echo $ques; echo '<br>';				
					echo '<br>';
				
				echo '<br><br><h4>Replies</h4>';
				echo '<br><br>';
				
				$result=mysql_query("Select reply,author,date from discuss_reply where qid='$qid'order by date desc",$link_id);
				$x=0;
				while($x<mysql_numrows($result))
				{
					$reply=mysql_result($result,$x,'reply');
					$author=mysql_result($result,$x,'author');
					$time=strtotime(mysql_result($result,$x,'date'));
					echo '<br> Reply by &nbsp &nbsp;'; echo $author; echo '&nbsp &nbsp on'; echo '&nbsp &nbsp; '; echo date('d M y H:i:s',$time);
					echo '<br> ';echo '<br>';			
					 echo $reply; echo '<br>';			
					echo '<br>';			
					
					$x++;
				}
				}
				function g($y)
			{
				include '../database.php';
				$uname=$_SESSION['username'];
				$link_id=mysql_connect($host,$user,$password);
				mysql_select_db($database);
				$result=mysql_query("SELECT qid,subject,question,date from discuss_question where author!='$uname' order by date desc limit $y,10",$link_id);
				$x=0;
				while($x<mysql_numrows($result))
					{
						$subject=mysql_result($result,$x,'subject');
						$question=mysql_result($result,$x,'question');
						$date=mysql_result($result,$x,'date');
						$qid=mysql_result($result,$x,'qid');
						$x++;
						?>
								<!-- Primary content area start -->
		
									<div class="post">
										<h4><a href="detail.php?sub=<?php echo $subject;?>"><?php echo $subject; ?></a></h4>
											<div class="contentarea">
												<div class="details"> on <?php echo $date;?>
												</div>
																<p> <?php echo $question ?> <ul class="controls">
																<li> <a href="detail.php?sub=<?php echo $subject;?>" class="comments">Replies <?php $this->count_reply($qid); ?></a></li>
																<li><a href="askexpert.php" class="more">Ask Zinedu Experts</a></li>				
																</ul>
												
											</div>
									<div>	
								<div class="divider2"></div>									
						
						<?php
					}
			}
			function count_reply($qid)
			{
			include '../database.php';
			$link_id=mysql_connect($host,$user,$password);
			mysql_select_db($database);
			$result=mysql_query("SELECT count(*) from discuss_reply where qid='$qid'",$link_id);
			$x=0;
			$topic=mysql_result($result,$x);
			echo $topic;
			}


				
	}
?>