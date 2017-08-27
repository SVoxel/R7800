function change_ipv6_pppoe_password(obj)
{
        if( obj.type == "password" )
        {
                if( get_browser() == "Firefox" )
                {
                        obj.value = "";
                        obj.type = "text";
                }
                else
                {
                        obj.outerHTML = '<input type="text" name="ipv6_pppoe_passwd" maxlength="64" size="18" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
                        document.forms[0].ipv6_pppoe_passwd.focus();
                        document.forms[0].ipv6_pppoe_passwd.focus();
                }
        }
}

function check_ipv6_pppoe(cf)
{
	if(ipv6_save_common(cf)==false)
		return false;

	if(top.ipv6_dns_manual == 1)
	{
		
		if(cf.ipv6_same_info.checked == true)
		{
			if(wan_proto != "pppoe")
			{
				alert("IPv4 Internet type must be PPPoE.");
				return false;
			}

			cf.pppoe_username.value = pppoe_username;
			cf.pppoe_passwd.value = pppoe_password;
			cf.pppoe_servername.value = pppoe_servername;
			cf.pppoe_dod.value = pppoe_dod;

			cf.ipv6_hidden_sameinfo.value = "1";
		}
		else
		{
			cf.ipv6_hidden_sameinfo.value = "0";

			if(cf.pppoe_username.value == "")
			{
				alert("$login_name_null");
				return false;
			}
        		for(i=0;i<cf.pppoe_username.value.length;i++)
        		{
                		if(isValidChar(cf.pppoe_username.value.charCodeAt(i))==false)
                		{
                        		alert("$loginname_not_allowed");
                        		return false;
                		}
        		}
	
			if(cf.pppoe_passwd.value == "")
			{
				alert("$password_null");
				return false;
			}
        		for(i=0;i<cf.pppoe_passwd.value.length;i++)
        		{
                		if(isValidChar(cf.pppoe_passwd.value.charCodeAt(i))==false)
                		{
                        		alert("$password_error");
                      		  	return false;
                		}
        		}

      		  	for(i=0;i<cf.pppoe_servername.value.length;i++)
        		{
                		if(isValidChar(cf.pppoe_servername.value.charCodeAt(i))==false)
                		{
                        		alert("$servname_not_allowed");
                        		return false;
                		}
        		}
		}

		if(checkIPv6DNS(cf) == false)
			return false;
	}

	return true;
}

function setSameinfo()
{
	var cf = document.forms[0];
	if(cf.ipv6_same_info.checked == true)
	{
	
		document.getElementById("login_line").style.display="none";
		document.getElementById("passwd_line").style.display="none";
		document.getElementById("servname_line").style.display="none";
		document.getElementById("conn_mode_line").style.display="none";
		document.getElementById("wan_line_1").style.display="none";
		document.getElementById("wan_line_2").style.display="";
 	}

	else
	{
		document.getElementById("login_line").style.display="";
		document.getElementById("passwd_line").style.display="";
		document.getElementById("servname_line").style.display="";
		document.getElementById("conn_mode_line").style.display="";
		document.getElementById("wan_line_1").style.display="";
		document.getElementById("wan_line_2").style.display="none";
		if(ipv6_pppoe_pwd == "")
		{
			cf.pppoe_passwd.outerHTML='<input type="text" name="pppoe_passwd" id="pppoe_passwd" maxlength="64" size="18" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
		}
		else
		{
			cf.pppoe_passwd.outerHTML='<input type="password" name="pppoe_passwd" id="pppoe_passwd" maxlength="64" size="18" style="width: 143px" onFocus="change_ipv6_pppoe_password(this);" onKeyPress="return getkey(\'ssid\', event)" value="">';
			cf.pppoe_passwd.value = ipv6_pppoe_pwd;
		}
	}
}

function ipv6_write_pppoe_ip(ipv6_ip_addr)
{
	if(ipv6_ip_addr != "")
		ipv6_ip_addr = remove_space(ipv6_ip_addr);
 
	if(ipv6_ip_addr == "")
	{
		document.write("$ipv6_not_available");
	}
	else
	{
		var each_ip = ipv6_ip_addr.split(' ');
		var i;

		for(i=0; i<each_ip.length; i++)
		{
			show_ipv6_ip(each_ip[i]);
		}
	}
}
