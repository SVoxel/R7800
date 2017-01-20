function setDNS(cf)
{
	var dflag = cf.DNSAssign[0].checked;
	setDisabled(dflag,cf.DAddr1,cf.DAddr2,cf.DAddr3,cf.DAddr4,cf.PDAddr1,cf.PDAddr2,cf.PDAddr3,cf.PDAddr4);
	DisableFixedDNS = dflag;
}

function change_bpa_password(obj)
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
			obj.outerHTML='<input type="text" name="bpa_passwd" size="16" maxlength="50" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
			document.forms[0].bpa_passwd.focus();
			document.forms[0].bpa_passwd.focus();
		}
	}
}					

function check_wizard_bpa(check,page)
{
	var cf=document.forms[0];
	if(cf.bpa_username.value == "")
	{
		alert("$login_name_null");
		return false;
	}
	for(i=0;i<cf.bpa_username.value.length;i++)
	{
		if(isValidChar(cf.bpa_username.value.charCodeAt(i))==false)
		{
			alert("$loginname_not_allowed");
			return false;
		}
	}

	for(i=0;i<cf.bpa_passwd.value.length;i++)
	{
		if(isValidChar(cf.bpa_passwd.value.charCodeAt(i))==false)
		{
			alert("$password_error");
			return false;
		}
	}
	if(cf.bpa_idletime.value.length<=0)
	{
		alert("$idle_time_null");
		return false;
	}
	else if(!_isNumeric(cf.bpa_idletime.value))
	{
		alert("$invalid_idle_time");
		return false;
	}	
	
	if( cf.bpa_server.value.length <= 0 )
	{
		alert("$bpa_invalid_serv_name");
		return false;
	}
	
	var server_addr_array = cf.bpa_server.value.split('.');
        var i;
        for( i=0; i<server_addr_array.length; i++ )
        {
                if( server_addr_array[i].length > 63 )
                {
                        alert("$invalid_servip_length");
                        return false;
                }
        }

	for(i=0;i<cf.bpa_server.value.length;i++)
	{
		if(isValidChar(cf.bpa_server.value.charCodeAt(i))==false)
		{
			alert("$bpa_invalid_serv_name");
			return false;
		}
	}
	
	/*Bug 20096, domain name should be able to input as Bigpond authentication server field,such as abc.a.co.com.
	var serv_array=cf.bpa_server.value.split('.');
	if(serv_array.length==4)
	{
		if(checkipaddr(cf.bpa_server.value)==false)
		{
			alert("$bpa_invalid_serv_name");
			return false;
		}
	}*/		
	if (check == 1)
	{
		cf.run_test.value="test";
		if( page == "wiz")
			cf.action="/apply.cgi?/WIZ_update.htm timestamp="+ts;
	}
	else
		cf.run_test.value="no";
                cf.submit();

	return true;
}

function check_bpa(cf,check)
{
	//bug 23249, change the input value format
	format_IP("DAddr1", "DAddr2", "DAddr3", "DAddr4", "PDAddr1", "PDAddr2", "PDAddr3", "PDAddr4");

	if(cf.auto_conn_24hr.checked == true)
		cf.hidden_auto_conn_flag.value=1;
	else
		cf.hidden_auto_conn_flag.value=0;
	cf.hidden_conn_time_num.value=cf.auto_conn_time.selectedIndex;

	if(check_wizard_bpa(check,"bas")==false)
		return false;
	cf.hidden_bpa_idle_time.value=cf.bpa_idletime.value
	if (cf.MACAssign[2].checked == true)
	{
		the_mac=cf.Spoofmac.value;
		if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
		{
			var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
			cf.Spoofmac.value = tmp_mac;
		}
		else if ( the_mac.split("-").length == 6 )
		{
			var tmp_mac = the_mac.replace(/-/g,":");
			cf.Spoofmac.value=tmp_mac;
		}	
		if(maccheck_multicast(cf.Spoofmac.value) == false)
			return false;
	}
	if(cf.DNSAssign[1].checked == true)
	{
		cf.bpa_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
		cf.bpa_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;

                if(cf.bpa_dnsaddr1.value=="...")
                        cf.bpa_dnsaddr1.value="";
		if(cf.bpa_dnsaddr2.value=="...")
			cf.bpa_dnsaddr2.value="";
		if( !check_DNS(cf.bpa_dnsaddr1.value,cf.bpa_dnsaddr2.value,false,""))
			return false;
	}
	cf.hid_mtu_value.value=wan_mtu_now;
	if(!(old_wan_type == "bigpond"))
	{
		cf.change_wan_type.value=0;
		mtu_change(wanproto_type);
	}
	else
		cf.change_wan_type.value=1;

	return true;
}


function check_welcome_bpa()
{
	var cf = document.forms[0];
	if(check_wizard_bpa(0,"welcome")==false)
		return false;

	// Other settings
	parent.telstra_bigpond_user_name = cf.bpa_username.value;
	parent.telstra_bigpond_password = cf.bpa_passwd.value;
	parent.telstra_idletime = cf.bpa_idletime.value;
	parent.telstra_bigpond_ip = cf.bpa_server.value;
	parent.welcome_wan_type=5;
	return true;
}
