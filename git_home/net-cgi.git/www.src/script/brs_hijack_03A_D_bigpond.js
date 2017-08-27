function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_bpa_connection);
	head_tag[0].appendChild(head_text);
	
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_enter_info_below);
	paragraph[0].appendChild(paragraph_text);
	
	
	//main content items
	var login_name = document.getElementById("loginName");
	var login_text = document.createTextNode(bh_pptp_login_name);
	login_name.appendChild(login_text);

	var passwd = document.getElementById("passwd");
	var passwd_text = document.createTextNode(bh_ddns_passwd);
	passwd.appendChild(passwd_text);
	
	var idleTimeout_div = document.getElementById("idleTimeout")
	var idleTimeout_text = document.createTextNode(bh_basic_pppoe_idle);
	idleTimeout_div.appendChild(idleTimeout_text);
	
	var serverIP_div = document.getElementById("serverIP")
	var serverIP_text = document.createTextNode(bh_basic_bpa_auth_serv);
	serverIP_div.appendChild(serverIP_text);


	//set input event action
	var name_input = document.getElementById("inputName");
	name_input.onkeypress = ssidKeyCode;

	var passwd_input = document.getElementById("inputPasswd");
	passwd_input.onkeypress = ssidKeyCode;

	var idle_input = document.getElementById("inputIdle");
	idle_input.onkeypress = numKeyCode;

	var severIP_input = document.getElementById("inputServerIP");
	severIP_input.onkeypress = ssidKeyCode;


	//buttons 
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function()
	{
		return checkBPA();
	}
	
	var btn = document.getElementById("btn_text_div");
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);
}

function checkBPA()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	var bpa_username = document.getElementById("inputName");
        var bpa_passwd = document.getElementById("inputPasswd");
        var bpa_idletime = document.getElementById("inputIdle");
	var bpa_server = document.getElementById("inputServerIP");
	
	if(bpa_username.value == "")
	{
		alert(bh_login_name_null);
		return false;
	}

	for(i=0; i<bpa_passwd.value.length;i++)
	{
		if(isValidChar(bpa_passwd.value.charCodeAt(i))==false)
		{
			alert(bh_password_error);
			return false;
		}
	}

	if(bpa_idletime.value.length <= 0)
	{
		alert(bh_idle_time_null);
		return false;
	}
	else if(!_isNumeric(bpa_idletime.value))
	{
		alert(bh_invalid_idle_time);
		return false;
	}

	if(bpa_server.value.length <= 0 )
	{
		alert(bh_bpa_invalid_serv_name);
		return false;
	}
	var server_addr_array = bpa_server.value.split('.');
	var i;
	for(i=0; i<server_addr_array.length; i++)
	{
		if(server_addr_array[i].length > 63)
		{
			alert(bh_invalid_servip_length);
			return false;
		}
	}
	for(i=0;i<bpa_server.value.length;i++)
	{
		if(isValidChar(bpa_server.value.charCodeAt(i))==false)
		{
			alert(bh_bpa_invalid_serv_name);
			return false;
		}
	}

	cf.submit();

	return true;
}

addLoadEvent(initPage);
