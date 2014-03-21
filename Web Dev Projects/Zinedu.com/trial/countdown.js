function DateStringToDateObject( dateString )
{
	var theDateTime = null;
	try
	{
		theDateTime = new Date(dateString);
		
		if( theDateTime == "Invalid Date" )
			throw new Error('Invalid Date Format: ' + dateString);
		
		return theDateTime;
	}
	catch( exception )
	{
		// JavaScript doesn't accept: MM-DD-YYYY hh:mm:ss - so we handle that here...
	
		// See if the string is in a format we handle, but JavaScript doesn't.
		// TODO: Come up with a regular expression for our supported date formats.
		
		// This functionality assumes MM-DD-YYYY hh:mm:ss
		// TODO: Change it to handle more varieties and extract Month, Date and Year accordingly
		var dateTimeParts = dateString.split(" ");
		var dateParts = dateTimeParts[0].split(/[-\/]/);
		var timeParts;
		if( dateTimeParts.length > 1 )
			timeParts = dateTimeParts[1].split(":");

		//theDateTime = new Date();
		var m = null;
		var d = null;
		var y = null;
		var hr = null;
		var min = null;
		var sec = null;
		
		// Extract date parts
		if( dateParts.length > 0 )
		{
			m = parseInt(dateParts[0], 10) - 1;		// I had to force 10-based to cover amiguity (padded with zero numbers like 03)
			d = parseInt(dateParts[1], 10);
			y = parseInt(dateParts[2], 10);
		}
		
		// Extract time parts.
		if( timeParts && timeParts.length > 0 )
		{
			hr = parseInt(timeParts[0], 10);
			min = parseInt(timeParts[1], 10);
			sec = parseInt(timeParts[2], 10);
		}
		
		theDateTime = new Date(y, m, d, hr, min, sec);
		return theDateTime;
	}
}

//====================================================================
/*
				CreateCountdown
				
		iTitle: The title for the Countdown Timer (a div within the whole timer div)
		iCountdownDate: Must be passed in the format ("MM-DD-YYYY hh:mm:ss")
		iAttachToElement: the Element Node you want the timer attached as a child to.
			examples:	document.body
								document.getElementById('myDiv')
								document.body.myDiv
		iCountdownNodeId (optional): the Id you want the elements to have.
			The following id's will be assigned if you pass "countdownDiv" as 
			iCountdownNodeId:
					<div id="countdownDiv">
						<div id="countdownDiv-title">[ iTitle gets put here]</div>
						<div id="countdownDiv-timer">[ timer data is updated here ]</div>
					</div>
*/
//====================================================================                                                                      
function CreateCountdown( iTitle, iCountdownDate, iAttachToElement, iCountdownNodeId )
{
	var nodeId = 'countdownDiv';
	if( iCountdownNodeId && typeof(iCountdownNodeId) == 'string' && iCountdownNodeId.length > 0 )
		nodeId = iCountdownNodeId;

	var rightNow = new Date();
	var endDate = DateStringToDateObject(iCountdownDate);
		
	if( rightNow.getTime() >= endDate.getTime() )
	{
		// I'm torn on throwing an error... cause it will start throwing it as soon as a timer expires... the next hit to the page.
		// We'll leave it out for now - possibly as an extra parameter later for debugging.
		//iAttachToElement.innerHTML += "CreateCountdown Exception: Invalid Date/Time (Trying to create Countdown with ID: '" + nodeId + "')";
		return false;	//break out so an interval doesn't start.
	}
	
	if( iAttachToElement )
	{
		var countdownDiv = document.createElement('div');
		countdownDiv.id = nodeId;
		countdownDiv.className = 'countdownContainer';
		countdownDiv.endDateMilliseconds = endDate.getTime();
		var titleDiv = document.createElement('div');
		titleDiv.id = nodeId + '-title';
		titleDiv.className = 'countdownTitle';
		titleDiv.innerHTML = iTitle;
		countdownDiv.appendChild(titleDiv);
		var timerDiv = document.createElement('div');
		timerDiv.id = nodeId + '-timer';
		timerDiv.className = 'countdownTimer';
		countdownDiv.appendChild(timerDiv);
		countdownDiv.interval = setInterval('UpdateTimer(document.getElementById("' + nodeId + '"));', 1000);
		iAttachToElement.appendChild(countdownDiv);
	}
}


function UpdateTimer( iCountdownNode )
{
	var now = new Date();
	if( iCountdownNode && ((iCountdownNode.endDateMilliseconds - now.getTime()) > 0) )
	{
		var timerNodeId = iCountdownNode.id.split("-")[0];
		var timerDiv = document.getElementById(timerNodeId + '-timer');
		var timeStr = "";
		var diff = iCountdownNode.endDateMilliseconds - now.getTime(); //returned in milliseconds
		//convert to Days, Hours, Minutes, Seconds
		var msPerSec = 1000;
		var msPerMin = 60 * msPerSec;
		var msPerHour = 60 * msPerMin;
		var msPerDay = 24 * msPerHour;
		var days = Math.floor(diff/msPerDay);
		var left = diff % msPerDay;
		var hours = Math.floor(left/msPerHour);
		left = left % msPerHour;
		var min = Math.floor(left/msPerMin);
		left = left % msPerMin;
		var sec = Math.floor(left/msPerSec);
		timeStr = days + " days " + hours + " hours " + min + " minutes " + sec + " seconds left";
		timerDiv.innerHTML = timeStr;
	}
	else
	{
		clearInterval(iCountdownNode.interval)
		iCountdownNode.parentNode.removeChild(iCountdownNode);
	}
}


function AddCountdownTester( iCountdownSeconds, iAttachToElement )
{
	var testerId = 'ctTester';
	
	if( document.getElementById(testerId) )
		return;

	var dtDelim = "-";
	var tmDelim = ":";
	var maxSeconds = 59;
	
	var dt = new Date();
	var s = dt.getSeconds() + iCountdownSeconds;
	if( s > maxSeconds )
	{
		dt.setMinutes(dt.getMinutes() + 1);	//increment minutes and get seconds back to real.
		s = 0 + (s - maxSeconds);
	} 
	dt.setSeconds(s);
	
	/*
	var m = dt.getMonth() + 1;
	var d = dt.getDate();
	var y = dt.getFullYear();
	var h = dt.getHours();
	var i = dt.getMinutes();
	var s = dt.getSeconds() + iCountdownSeconds;
	if( s > maxSeconds )
	{
		i++;	//increment minutes and get seconds back to real.
		s = 0 + (s - maxSeconds);
	}
	*/
	
	var dateStr = MDYHIS_DateString(dt);
	var parentElement = (!iAttachToElement) ? document.body : iAttachToElement;
		
	CreateCountdown("... Tester will disappear in ...", dateStr, parentElement, testerId);
}


function MDYHIS_DateString( iDate )
{
	var maxSeconds = 59;
	var maxMinutes = 59;
	var maxHours = 23;
	
	var dateDelim = '-';
	var timeDelim = ':';
	
	var dt = iDate;
	var m = dt.getMonth() + 1;
	var d = dt.getDate();
	var y = dt.getFullYear();
	var h = dt.getHours();
	var i = dt.getMinutes();
	var s = dt.getSeconds();
	
	return (m + dateDelim + d + dateDelim + y + " " + h + timeDelim + i + timeDelim + s);
}

// Debug stuff
//document.getElementById('debug').innerHTML += "<info>" + "<br />";
