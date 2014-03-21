function showlogin()
{
popup.style.visibility="visible";
}
function hidelogin()
{
popup.style.visibility="hidden";
}
function disp()
{
if(document.login.user.value=="username")
document.login.user.value="";
}
function hide()
{if(document.login.user.value=="")
document.login.user.value="username";
}