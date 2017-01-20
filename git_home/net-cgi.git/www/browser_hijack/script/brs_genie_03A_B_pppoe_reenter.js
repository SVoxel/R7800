function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_ISP_namePasswd_error);
	head_tag[0].appendChild(head_text);
	
	
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_enter_info_again);
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
	
	var btns1 = document.getElementById("self_config");
	btns1.value = bh_manual_config_connection;

	if( master == "admin" )
	{
		btns1.onclick = function()
		{
			return manuallyConfig();
		}
	}
	else
		btns1.className="grey_long_btn";

	var btns2 = document.getElementById("again");
	btns2.value = bh_try_again;
	if( master == "admin" )
	 btns2.onclick = function()
	  {
		return checkPPPoE();
	 }
	 else
	 	btns2.className="grey_common_btn";

	//show firmware version
        showFirmVersion("");
}

function manuallyConfig()
{
	if(confirm(bh_no_genie_help_confirm) == false)
		return false;
        if(top.dsl_enable_flag == 1)
		this.location.href="BRS_log12_incorrect_go_to_internet.html";
	else{
		var forms = document.getElementsByTagName("form");
		var cf = forms[0];

		if( hijack_process == "1")
		{
			location.href = "BRS_security.html";
		}
		else
			location.href = "BAS_basic.htm";
	}
	return true;
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
