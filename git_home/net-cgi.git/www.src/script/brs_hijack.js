//goto basic home page
function goto_home_page()
{
	top.location.href="index.htm";
}

//add window onload event
function addLoadEvent(func) 
{
	var oldonload = window.onload;
	
	if (typeof window.onload != 'function') 
	{
		window.onload = func;
	} 
	else 
	{
		window.onload = function() 
		{
			oldonload();
			func();
		}
	}
}

//insert a tree node after another one
function insertAfter(newElement, targetElement) 
{
	var parent = targetElement.parentNode;
	
	if (parent.lastChild == targetElement) 
	{
		parent.appendChild(newElement);
	} 
	else 
	{
		parent.insertBefore(newElement, targetElement.nextSibling);
	}
}

//add a className for an object, but not completely repalce the previous one
function addClass(element,value) 
{
	if (!element.className) 
	{
		element.className = value;
	}
	else 
	{
		newClassName = element.className;
		newClassName += " ";
		newClassName += value;
		element.className = newClassName;
	}
}

function showFirmVersion(flag)
{	
	var iframe = top.document.getElementById("header_frame");

	if(iframe)
	{
		var version_div = iframe.contentWindow.document.getElementById("firmware_version");

		if(version_div)
			version_div.style.display = flag;
	}
}

function ssidKeyCode(event) 
{ 
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );
			
	if(keycode == 32)
		return false;
	else
		return true;
}

function ipaddrKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
        var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );

	if(((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||(keycode==46)) 
		return true;
	else
		return false;
}

function numKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );

	if(((keycode>47) && (keycode<58)) || (keycode==8)||(keycode==0))
		return true;
	else 
		return false;
}

function numLetterKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );

	if((keycode>47 && keycode<58)||(keycode>64 && keycode<91)||(keycode>96 && keycode<123)||(keycode==8)||(keycode==0))
		return true;
	else
		return false;
}

function hostnameKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );

	if(((keycode>47) && (keycode<58))||(keycode==45)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0))
		return true;
	else
		return false;
}

function ddnsHostnameKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );

	if(((keycode>47) && (keycode<58))||(keycode==45)||(keycode==46)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0))
		return true;
	else
		return false;
}

function macKeyCode(event)
{
	var evt = event ? event : (window.event ? window.event : null);
	var keycode = window.event ? window.event.keyCode : (evt ? evt.which : null );
	
	if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0) || (keycode == 58) || (keycode == 45))
		return true;
	else 
		return false;
}

function detectOS()
{   // detect the system type of user computer
	var sUserAgent = navigator.userAgent;
	var isWin = (navigator.platform == "Win32") || (navigator.platform == "Win64") || (navigator.platform == "Windows");
	var isMac = (navigator.platform == "Mac68K") || (navigator.platform == "MacPPC") || (navigator.platform == "Macintosh") || (navigator.platform == "MacIntel");
	if (isMac) return "Mac";
	var isUnix = (navigator.platform == "X11") && !isWin && !isMac;
	if (isUnix) return "Unix";
	var isLinux = (String(navigator.platform).indexOf("Linux") > -1);
	if (isLinux) return "Linux";
	if (isWin) {
		var isWin2K = sUserAgent.indexOf("Windows NT 5.0") > -1 || sUserAgent.indexOf("Windows 2000") > -1;
		if (isWin2K) return "Win2000";
		var isWinXP = sUserAgent.indexOf("Windows NT 5.1") > -1 || sUserAgent.indexOf("Windows XP") > -1;
		if (isWinXP) return "WinXP";
		var isWin2003 = sUserAgent.indexOf("Windows NT 5.2") > -1 || sUserAgent.indexOf("Windows 2003") > -1;
		if (isWin2003) return "Win2003";
		var isWin2003 = sUserAgent.indexOf("Windows NT 6.0") > -1 || sUserAgent.indexOf("Windows Vista") > -1;
		if (isWin2003) return "WinVista";
		var isWin2003 = sUserAgent.indexOf("Windows NT 6.1") > -1 || sUserAgent.indexOf("Windows 7") > -1;
		if (isWin2003) return "Win7";
		var isWin8 = sUserAgent.indexOf("Windows NT 6.2") > -1 || sUserAgent.indexOf("Windows NT 6.3") > -1 || sUserAgent.indexOf("Windows 8") > -1;
		if (isWin8) return "Win8";
		var isWin10 = sUserAgent.indexOf("Windows NT 10.0") > -1 || sUserAgent.indexOf("Windows 10") > -1;
		if (isWin10) return "Win10";
		return "Win";
	}
	return "None";
}

function isMac()
{
	return (detectOS() == "Mac");
}

function control_display(page)
{
	if(page == "dsl_dhcp")
	{
		if(lan_ports_num == 1){
			document.getElementById("bridge_doc").style.display = "none";
			document.getElementById("bridge_lans").style.display = "none";
			document.getElementById("content_div").style.width ="100%";
			document.getElementById("connection_identi").style.height = "60px";	
		}
	}
	else if(page == "dsl_pppoe")
	{
		if(display_iptv2 == "1"){
			document.getElementById("iptv").style.display="";
			if(wan2_bridge == "1" && lan_ports_num != 1 )
			{
				document.getElementById("bridge").style.display="";
				document.getElementById("bridge1").style.display="";
				document.getElementById("bridge2").style.display="";
			}
			else
			{
				document.getElementById("bridge").style.display="none";
				document.getElementById("bridge1").style.display="none";
				document.getElementById("bridge2").style.display="none";
			}
		}
		else
			document.getElementById("iptv").style.display="none";
		if(vlan_id == "")
		{
			var obj = document.getElementById("vlanid_text");
			if(obj != null){
				document.getElementById("vlanid").style.display="none";
				obj.style.display="none";
			}
		}
	}
}
