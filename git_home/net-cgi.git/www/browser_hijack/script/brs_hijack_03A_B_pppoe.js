function initPage()
{
	var head_tag = document.getElementsByTagName("h2");
	var head_text = document.createTextNode(bh_pppoe_connection);
	head_tag[0].appendChild(head_text);
	
	
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_enter_info_below);
	paragraph[0].appendChild(paragraph_text);
	
	
	var login_name = document.getElementById("loginName");
	var login_text = document.createTextNode(bh_pppoe_login_name);
	login_name.appendChild(login_text);

	var passwd = document.getElementById("passwd");
	var passwd_text = document.createTextNode(bh_ddns_passwd);
	passwd.appendChild(passwd_text);

	
	//set event action
	var name_input = document.getElementById("pppoe_name");
	name_input.onkeypress = ssidKeyCode;

	var passwd_input = document.getElementById("pppoe_password");
	passwd_input.onkeypress = ssidKeyCode;
	if(top.dsl_enable_flag == 1)
	{
		 cf=document.forms[0];
		 cf.pppoe_username.value="";
		 cf.pppoe_passwd.value="";
	}

	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )
	{
		btn.onclick = function()
		{
			return checkPPPoE();
		}
	}
	else
		btn.className="grey_short_btn";
	
	//show firmware version
        showFirmVersion("none");
}

function checkPPPoE()
{
	var forms = document.getElementsByTagName("form");
        var cf = forms[0];
	
	var pppoe_name = document.getElementById("pppoe_name");
	var pppoe_passwd = document.getElementById("pppoe_password");

	if(pppoe_name.value == "")
	{
		alert(bh_login_name_null);
		return false;
	}

	var i;
	for(i=0;i<pppoe_passwd.value.length;i++)
	{
		if(isValidChar(pppoe_passwd.value.charCodeAt(i))==false)
		{
			alert(bh_password_error);
			return false;
		}
	}

	cf.submit();

	return true;
}

addLoadEvent(initPage);
