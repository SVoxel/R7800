function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h2");
	var head_text = document.createTextNode(bh_pptp_connection);
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

	var idleTimeout = document.getElementById("idleTimeout");
	var idleTimeout_text = document.createTextNode(bh_basic_pppoe_idle);
	idleTimeout.appendChild(idleTimeout_text);
	
	var IP_addr_div = document.getElementById("IP_addr");
	var IP_addr_text = document.createTextNode(bh_info_mark_ip);
	IP_addr_div.appendChild(IP_addr_text);
	
	var serverIP_div = document.getElementById("serverIP");
        var serverIP_text = document.createTextNode(bh_basic_pptp_servip);
        serverIP_div.appendChild(serverIP_text);

	var Gateway_div = document.getElementById("GatewayAddr");
	var Gateway_text = document.createTextNode(bh_sta_routes_gtwip);
	Gateway_div.appendChild(Gateway_text);
	
	var connectionID_div = document.getElementById("connectionID");
        var connectionID_text = document.createTextNode(bh_basic_pptp_connection_id);
        connectionID_div.appendChild(connectionID_text);


	//set input event action
	var name_input = document.getElementById("pptp_username");
	name_input.onkeypress = ssidKeyCode;

	var passwd_input = document.getElementById("pptp_passwd");
	passwd_input.onkeypress = ssidKeyCode;

	var idle_input = document.getElementById("pptp_idletime");
	idle_input.onkeypress = numKeyCode;

	var severIP_input = document.getElementById("pptp_serv_ip");
	severIP_input.value = "10.0.0.138";
	severIP_input.disabled = true;

	//buttons 
	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )
		btn.onclick = function()
		{
			return checkPPTP();
		}
	else
		btn.className="grey_short_btn";

	//show firmware version
        showFirmVersion("none");
}

function checkPPTP()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	var pptp_username = document.getElementById("pptp_username");
	var pptp_passwd = document.getElementById("pptp_passwd");
	var pptp_idletime = document.getElementById("pptp_idletime");

	if(pptp_username.value=="")
	{
		alert(bh_login_name_null);
		return false;
	}

	var i;
	for(i=0; i<pptp_passwd.value.length; i++)
	{
		if(isValidChar(pptp_passwd.value.charCodeAt(i))==false)
		{
			alert(bh_password_error);
			return false;
		}
	}
	if(pptp_idletime.value.length <= 0)
	{
		alert(bh_idle_time_null);
		return false;
	}
	else if(!_isNumeric(pptp_idletime.value))
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

	cf.ip_address.value = cf.myip_1.value+'.'+cf.myip_2.value+'.'+cf.myip_3.value+'.'+cf.myip_4.value;
	cf.gateway.value = cf.mygw_1.value+'.'+cf.mygw_2.value+'.'+cf.mygw_3.value+'.'+cf.mygw_4.value;

	if(cf.ip_address.value != "...")
	{
		cf.WANAssign.value = 1;

                /* To fix Bug27179: [New GUI][CD-less]DUT should pop up error message if Gateway IP Address
		 * and My IP Address are in different network.
		 */

		var pptp_mask="255.255.255.0";

		if( parseInt(cf.myip_1.value) < 128 )
			pptp_mask="255.0.0.0";
		else if( parseInt(cf.myip_1.value) < 192 )
			pptp_mask="255.255.0.0";
		else
			pptp_mask="255.255.255.0";

		cf.pptp_subnet.value=pptp_mask;


		if(checkipaddr(cf.ip_address.value)==false)
		{
			alert(bh_invalid_myip);
			return false;
		}
		/*Bug 30115 - [GUI][CD-less/Setup Wizard]I can set My IP address as 10.0.0.138 when detected as PPTP mode*/
		if(isSameIp(cf.ip_address.value,cf.pptp_serv_ip.value) == true)
		{
			alert(bh_same_server_wan_ip);
			return false;
		}
		if(cf.gateway.value != "..." && checkgateway(cf.gateway.value) == false)
		{
			alert(bh_invalid_gateway);
			return false;
		}
		if(cf.gateway.value != "...")
		{
			if(isSameIp(cf.ip_address.value,cf.gateway.value) == true)
			{
				alert(bh_invalid_gateway);
				return false;
			}
			if(isGateway(cf.ip_address.value,cf.pptp_subnet.value,cf.gateway.value) == false)
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
