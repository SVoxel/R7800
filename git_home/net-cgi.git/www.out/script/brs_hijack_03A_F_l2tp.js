function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_l2tp_connection);
	head_tag[0].appendChild(head_text);
	
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_enter_info_below);
	paragraph[0].appendChild(paragraph_text);
	
	
	//main content items
	var login_name = document.getElementById("loginName");
	var login_text = document.createTextNode(bh_pptp_login_name + ":");
	login_name.appendChild(login_text);

	var passwd = document.getElementById("passwd");
	var passwd_text = document.createTextNode(bh_ddns_passwd + ":");
	passwd.appendChild(passwd_text);

	var idleTimeout = document.getElementById("idleTimeout");
	var idleTimeout_text = document.createTextNode(bh_basic_pppoe_idle);
	idleTimeout.appendChild(idleTimeout_text);
	
	var IP_addr_div = document.getElementById("IP_addr");
	var IP_addr_text = document.createTextNode(bh_info_mark_ip);
	IP_addr_div.appendChild(IP_addr_text);

	var sub_mask_div = document.getElementById("subnet_mask");
	var sub_mask_text = document.createTextNode(bh_info_mark_mask);
	sub_mask_div.appendChild(sub_mask_text);
	
	var serverIP_div = document.getElementById("serverIP");
        var serverIP_text = document.createTextNode(bh_basic_pptp_servip);
        serverIP_div.appendChild(serverIP_text);

	var Gateway_div = document.getElementById("Gateway");
	var Gateway_text = document.createTextNode(bh_sta_routes_gtwip);
	Gateway_div.appendChild(Gateway_text);
	

	//set input event action
	var name_input = document.getElementById("inputName");
	name_input.onkeypress = ssidKeyCode;

	var passwd_input = document.getElementById("inputPasswd");
	passwd_input.onkeypress = ssidKeyCode;

	var idle_input = document.getElementById("inputIdle");
	idle_input.onkeypress = numKeyCode;

	var IP_addr_input = document.getElementById("inputIPaddr");
	IP_addr_input.onkeypress = ipaddrKeyCode;

	var sub_mask_input = document.getElementById("inputSubnetMask");
	sub_mask_input.onkeypress = ipaddrKeyCode;
	
	var severIP_input = document.getElementById("inputServerIP");
	severIP_input.value = "10.0.0.138";
	severIP_input.disabled = true;

	var gateway_input = document.getElementById("inputGateway");
	gateway_input.onkeypress = ipaddrKeyCode;


	//buttons 
	var btns_container_div = document.getElementById("btnsContainer_div");
	if( master == "admin" )
	btns_container_div.onclick = function()
	{
		return checkL2TP();
	}
	
	var btn = document.getElementById("btn_text_div");
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);

	//show firmware version
        showFirmVersion("none");
}

function checkL2TP()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	var l2tp_username = document.getElementById("inputName");
	var l2tp_passwd = document.getElementById("inputPasswd");
	var l2tp_idletime = document.getElementById("inputIdle");

	if(l2tp_username.value=="")
	{
		alert(bh_login_name_null);
		return false;
	}

	var i;
	for(i=0; i<l2tp_passwd.value.length; i++)
	{
		if(isValidChar(l2tp_passwd.value.charCodeAt(i))==false)
		{
			alert(bh_password_error);
			return false;
		}
	}
	if(l2tp_idletime.value.length <= 0)
	{
		alert(bh_idle_time_null);
		return false;
	}
	else if(!_isNumeric(l2tp_idletime.value))
	{
		alert(bh_invalid_idle_time);
		return false;
	}

	if(!checkIPaddr())
		return false;

	cf.submit();

	return true;
}

function checkIPaddr()
{
	var forms = document.getElementsByTagName("form");
        var cf = forms[0];

	var l2tp_myip = document.getElementById("inputIPaddr");
        var l2tp_gateway = document.getElementById("inputGateway");

	if(l2tp_myip.value != "")
	{
		cf.WANAssign.value = 1;
		if(checkipaddr(l2tp_myip.value)==false)
		{
			alert(bh_invalid_myip);
			return false;
		}
		if(l2tp_gateway.value != "" && checkgateway(l2tp_gateway.value) == false)
		{
			alert(bh_invalid_gateway);
			return false;
		}
		if(l2tp_gateway.value != "")
		{
			if(isSameIp(l2tp_myip.value,pptp_gateway.value) == true)
			{
				alert(bh_invalid_gateway);
				return false;
			}
		}
	}
	else
		cf.WANAssign.value=0;
	
	return true;
}

addLoadEvent(initPage);
