function hidelogin()
{
popup.style.visibility="hidden";
search.style.visibility="visible";
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