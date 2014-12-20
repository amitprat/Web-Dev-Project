/**
* PHPSimpleChat - General PHP AJAX framework function
* 
* LICENSE: Creative Commons Attribution-Noncommercial-No Derivative Works 3.0 unless arranged with copyright holder.
*
* @copyright  2008 Elliott Brueggeman 
* @license    http://www.ebrueggeman.com/phpsimplechat/documentation_usage.php
* @version    0.2
* @link       http://www.ebrueggeman.com/phpsimplechat 
* @since      File available since Release 0.1
*/

function ajaxHelper(functionName, base) {
  var xmlHttp;
  // Firefox, Opera 8.0+, Safari, SeaMonkey
  try {    
    xmlHttp=new XMLHttpRequest();
  }
  catch (e) {   
    // Internet Explorer 
    try {    
      xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
    }
    catch (e) {   
      try {      
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");      
      }
      catch (e) {      
        alert("Sorry, your browser does not support AJAX.");
        return false;      
      }    
    }  
  } 
 
  xmlHttp.onreadystatechange=function() {
    //The request is complete == state 4
    if (xmlHttp.readyState==4) {
      var response=xmlHttp.responseText;
      //Send reponse to _ajax hook of passed function name
      eval(functionName + "_ajax" + '(\'' + response + '\')');
    }
  }
 
  //Get request string from _setup hook of passed function name
  var requestString = eval(functionName + "_init" + '()');
  if (requestString) {
	//xmlHttp.open("GET", base + '/' + requestString, true);
	xmlHttp.open("GET", requestString, true);
    xmlHttp.send(null);
  }
}