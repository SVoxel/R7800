function show_hidden_help_top_button(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >= 8)
		Not_IE_show_hidden_help(help_flag);
	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=frame_height-285;//30+120+135
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=frame_height-165;//30+135
		}
	}
}

function myip_update(page)
{
    var cf = document.forms[0];

    if((cf.myip_1.value.length>0)&&(cf.myip_2.value.length>0)&&(cf.myip_3.value.length>0)&&(cf.myip_4.value.length>0))
    {
        setDisabled(false, cf.mygw_1, cf.mygw_2, cf.mygw_3, cf.mygw_4);
	if( russian_ppp_flag == 1 && page == "bas" )
	{
		setDisabled(false, cf.mymask_1, cf.mymask_2, cf.mymask_3, cf.mymask_4);
		pptp_servip_update();
	}
    }
    else
    {
        setDisabled(true, cf.mygw_1, cf.mygw_2, cf.mygw_3, cf.mygw_4);
	if( russian_ppp_flag == 1 && page == "bas" )
	{
		setDisabled(true, cf.mymask_1, cf.mymask_2, cf.mymask_3, cf.mymask_4);
		pptp_servip_update();
	}
    }
}

function change_pptp_password(obj)
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
			obj.outerHTML='<input type="text" name="pptp_passwd" maxlength="50" size="16" onFocus="this.select();change_pptp_password(this);" onKeyPress="return getkey(\'ssid\', event)" value="">';
			document.forms[0].pptp_passwd.select();
                }
        }
	document.forms[0].hidden_pwd_change.value="1";
}

function pptp_servip_update()
{
	var disable=false;
	var cf = document.forms[0];
	var serv_array=cf.pptp_serv_ip.value.split('.');
	if (serv_array.length == 4)
	{
		var flag = 0;
		for (iptab=0; iptab<4; iptab++)
			if (isNumeric(serv_array[iptab], 255))
				flag++;
		if (flag == 4)
		{
			if (checkipaddr(cf.pptp_serv_ip.value) == false)
				disable=true;
		}
		else
		{
			disable=true;
		}
	}
	else if (cf.pptp_serv_ip.value.length > 0)
	{
		disable=true;
	}

	if( russian_ppp_flag == 1 )
	{
	   	if( disable == true && cf.myip_1.value == "") 
	   	{
			disable = false;
	   	}				
		setDisabled(disable, cf.DNSAssign[0]);
		if ( disable == true && cf.DNSAssign[1].checked == false)
		{
			cf.DNSAssign[1].checked = true;
			setDNS(cf);
		}		
	   
	}
	/*else
	{
		setBlank(disable, cf.myip_1, cf.myip_2, cf.myip_3, cf.myip_4);	
		setDisabled(disable, cf.myip_1, cf.myip_2, cf.myip_3, cf.myip_4);
	}*/
}

function setDNS(cf)
{
	var dflag = cf.DNSAssign[0].checked;
	setDisabled(dflag,cf.DAddr1,cf.DAddr2,cf.DAddr3,cf.DAddr4,cf.PDAddr1,cf.PDAddr2,cf.PDAddr3,cf.PDAddr4);
	DisableFixedDNS = dflag;
}

/* check each charactor in server address, if all numbers, return true */
function is_IP_addr(serv_array)
{
        var i, j, charct;
        for( i=0; i<serv_array.length; i++ )
        {
                for( j=0; j<serv_array[i].length; j++)
                {
                        charct = serv_array[i].charAt(j);
                        if("0123456789".indexOf(charct,0)<0)
                        {
                                return false;
                        }
                }
        }
        return true;
}

function check_wizard_pptp(check,page)
{
	var cf=document.forms[0];
	if(cf.pptp_username.value=="")
	{
		alert("$login_name_null");
		return false;
	}
	for(i=0;i<cf.pptp_username.value.length;i++)
	{
		if(isValidChar(cf.pptp_username.value.charCodeAt(i))==false)
		{
			alert("$loginname_not_allowed");
			return false;
		}
	}

	for(i=0;i<cf.pptp_passwd.value.length;i++)
	{
		if(isValidChar(cf.pptp_passwd.value.charCodeAt(i))==false)
		{
			alert("$password_error");
			return false;
		}
	}
	if(cf.pptp_dod.value == "1")
	{
		if(cf.pptp_idletime.value.length<=0)
		{
			alert("$idle_time_null");
			return false;
		}
		else if(!_isNumeric(cf.pptp_idletime.value))
		{
			alert("$invalid_idle_time");
			return false;
		}
	}
	cf.pptp_myip.value=cf.myip_1.value+'.'+cf.myip_2.value+'.'+cf.myip_3.value+'.'+cf.myip_4.value;
	cf.pptp_gateway.value=cf.mygw_1.value+'.'+cf.mygw_2.value+'.'+cf.mygw_3.value+'.'+cf.mygw_4.value;
	if( russian_ppp_flag == 1 && page == 'bas' )
		cf.pptp_subnet.value=cf.mymask_1.value+'.'+cf.mymask_2.value+'.'+cf.mymask_3.value+'.'+cf.mymask_4.value;
	else
		cf.pptp_subnet.value="";

	if(cf.pptp_subnet.value == "" || cf.pptp_subnet.value=="..." )
	{
                var pptp_mask="255.255.255.0";

                if( parseInt(cf.myip_1.value) < 128 )
                        pptp_mask="255.0.0.0";
                else if(parseInt(cf.myip_1.value) < 192 )
                        pptp_mask="255.255.0.0";
                else
                        pptp_mask="255.255.255.0";
		cf.pptp_subnet.value=pptp_mask;
	}
	
    	if( cf.pptp_myip.value != "..." )
    	{
		cf.WANAssign.value=1;

        	if(checkipaddr(cf.pptp_myip.value)==false)
        	{
            		alert("$invalid_myip");
            		return false;
        	}
                if(isSameSubNet(cf.pptp_myip.value,lan_subnet,lan_ip,lan_subnet) == true)
                {
                        cf.conflict_wanlan.value=1;
                }
                if(isSameIp(cf.pptp_myip.value,lan_ip) == true)
                {
                       	cf.conflict_wanlan.value=1;
                }
		if( russian_ppp_flag == 1 && page == 'bas' )
		{
                        if(checksubnet(cf.pptp_subnet.value, 1)==false)
                    	{
                        	alert("$invalid_mask");
                               	return false;
                 	}
                   	if(isSameSubNet(cf.pptp_myip.value,cf.pptp_subnet.value,lan_ip,cf.pptp_subnet.value) == true)
			{
                               	cf.conflict_wanlan.value=1;
                        }
                        if(isSameSubNet(cf.pptp_myip.value,cf.pptp_subnet.value,lan_ip,lan_subnet) == true)
                     	{
                               	cf.conflict_wanlan.value=1;
                     	}
		

		}
        	if ( cf.pptp_gateway.value != "..." && (checkgateway(cf.pptp_gateway.value) == false || is_sub_or_broad(cf.pptp_gateway.value, cf.pptp_gateway.value, cf.pptp_subnet.value) == false) )
        	{
            		alert("$invalid_gateway");
            		return false;
        	}
        	if( cf.pptp_gateway.value != "..." )
        	{
			/*if(isGateway(cf.pptp_myip.value,cf.pptp_subnet.value,cf.pptp_gateway.value) == false)
                	{
                        	alert("$invalid_gateway");
                        	return false;
			}*/
                	if(isSameIp(cf.pptp_myip.value,cf.pptp_gateway.value) == true)
                	{
                        	alert("$invalid_gateway");
                        	return false;
                	}
			cf.pptp_gateway.value = address_parseInt(cf.pptp_gateway.value);
        	}
		cf.pptp_myip.value = address_parseInt(cf.pptp_myip.value);
		cf.pptp_subnet.value = address_parseInt(cf.pptp_subnet.value);

    	}
    	else
    	{
        	cf.pptp_myip.value="";
        	cf.pptp_gateway.value="";
		cf.pptp_subnet.value="";
        	cf.WANAssign.value=0;
    	}

    	if(cf.pptp_serv_ip.value=="")
    	{
                alert("$invalid_servip");
                return false;
    	}
    	for(i=0;i<cf.pptp_serv_ip.value.length;i++)
    	{
		if( isValidDdnsHost(cf.pptp_serv_ip.value.charCodeAt(i))==false )
        	{
            		alert("$invalid_servip");
            		return false;
        	}
    	}
    	if( cf.pptp_serv_ip.value == cf.pptp_myip.value )
    	{
		alert("$same_server_wan_ip");
		return false;
    	}
	
	var serv_array=cf.pptp_serv_ip.value.split('.');
        var i;
	var is_domain_name=0;
        for( i=0; i<serv_array.length; i++ )
        {
                if( serv_array[i].length > 63 )
                {
                        alert("$invalid_servip_length");
                        return false;
                }
        }
	/*  bug 31100 if server address is an invalid IP address, DUT should treat as a FQDN, 
	 *  valid ip address or invalid ip address both are the valid server ip
	 *  so not need to check ip address is valid or not.
	if( (serv_array.length==4) && (is_IP_addr(serv_array)==true) )
	{
		if ( checkipaddr(cf.pptp_serv_ip.value) == false )
		{
			alert("$invalid_servip");
			return false;
		}
	}*/
    	if( serv_array.length==4 )
    	{
        	var flag = 0;
        	for( iptab=0; iptab<4; iptab++ )
        	    if( isNumeric(serv_array[iptab], 255) ) flag++;
        	if( flag == 4 )
        	{
			/* bug 31100 if server address is an invalid IP address, DUT should treat as a FQDN 
				so when checkipaddr(cf.pptp_serv_ip.value) == false, should not alert a warning.
			*/
            		if ( checkipaddr(cf.pptp_serv_ip.value) == true )
            		{
				if( cf.pptp_myip.value != "" )
				{
					if(cf.pptp_gateway.value == "..." || cf.pptp_gateway.value == "" )
					{
						alert("$invalid_gateway");
						return false;
					}				
				}
			}
		}
    	}	
						
/*	for(i=0;i<cf.pptp_conn_id.value.length;i++)
	{
		if(isValidChar(cf.pptp_conn_id.value.charCodeAt(i))==false)
		{
			alert("$invalid_pptp_connection_id");
			return false;
		}
	}
*/
        if (check == 1)
        {
                cf.run_test.value="test";
                if( page == "wiz")
                        cf.action="/apply.cgi?/WIZ_update.htm timestamp="+ts;
        }
        else
                cf.run_test.value="no";
	return true;
}

function check_pptp(cf,check)
{
	//bug 23249, change the input value format
	format_IP("myip_1", "myip_2", "myip_3", "myip_4", "mymask_1", "mymask_2", "mymask_3", "mymask_4", "mygw_1", "mygw_2", "mygw_3", "mygw_4", "DAddr1", "DAddr2", "DAddr3", "DAddr4", "PDAddr1", "PDAddr2", "PDAddr3", "PDAddr4");

	if(cf.auto_conn_24hr.checked == true)
		cf.hidden_auto_conn_flag.value=1;
	else
		cf.hidden_auto_conn_flag.value=0;
	cf.hidden_conn_time_num.value=cf.auto_conn_time.selectedIndex;
		
	var wan_assgin = false;
	if(check_wizard_pptp(check,"bas")==false)
		return false;
	cf.hidden_pptp_idle_time.value=parseInt(cf.pptp_idletime.value, 10);
	cf.hid_pptp_dod.value = cf.pptp_dod.value;//bug 22322:when connection mode gray out,need a hidden-value to transfer
	if(cf.WANAssign.value == 1)
		wan_assgin = true;
	 
	if(cf.DNSAssign[1].checked == true)
	{
		cf.pptp_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
		cf.pptp_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;

                if(cf.pptp_dnsaddr1.value=="...")
                        cf.pptp_dnsaddr1.value="";
		else
			cf.pptp_dnsaddr1.value = address_parseInt(cf.pptp_dnsaddr1.value);
		if(cf.pptp_dnsaddr2.value=="...")
			cf.pptp_dnsaddr2.value="";
		else
			cf.pptp_dnsaddr2.value = address_parseInt(cf.pptp_dnsaddr2.value);
	
		if(!check_DNS(cf.pptp_dnsaddr1.value,cf.pptp_dnsaddr2.value,wan_assgin,cf.pptp_myip.value))
			return false;

		if( cf.WANAssign.value == 1  )	
		{
			if( (cf.pptp_dnsaddr1.value != "" && !isSameSubNet(cf.pptp_dnsaddr1.value, cf.pptp_subnet.value, cf.pptp_myip.value, cf.pptp_subnet.value)) || (cf.pptp_dnsaddr2.value !="" && !isSameSubNet(cf.pptp_dnsaddr2.value, cf.pptp_subnet.value, cf.pptp_myip.value, cf.pptp_subnet.value)) )
			{
				if(cf.pptp_gateway.value == "" || cf.pptp_gateway.value == "...")
				{
					alert("$invalid_gateway");
					return false;
				}
			}
		}

	}
	if (cf.MACAssign[2].checked )
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
	cf.hid_mtu_value.value=wan_mtu_now;
	if(!(old_wan_type=="pptp"))
	{
		cf.change_wan_type.value=0;
		mtu_change(wanproto_type);
	}
	else
		cf.change_wan_type.value=1;
	cf.hid_enable_vpn.value = vpn_enable;
	if(old_endis_ddns != "1" && vpn_enable == "1")
        {
                if(confirm("$no_change_static_ip") == true)
                        cf.hid_enable_vpn.value = "0";
                else{
                        cf.hid_enable_vpn.value = "1";
                        return false;
                }
        }
	if( cf.pptp_dod.value == "1" || cf.pptp_dod.value == "2" )
	{
		if( readycloud_enable == "1" || vpn_enable == "1" || upnp_enableMedia == "1" || parent.geniecloud_flag == 1)
		{
			if(confirm("$ppp_dial_on_demand_query") == false)
					return false;
		}
	}
	cf.submit();
	return true;
}

function setIP_welcome_pptp()
{
	var cf = document.forms[0];
	var dflag = cf.WANAssign[0].checked;
	setDisabled(dflag,cf.myip_1,cf.myip_2,cf.myip_3,cf.myip_4);
	DisableFixedIP = dflag;
}


function check_welcome_pptp()
{
	var cf = document.forms[0];
	if(check_wizard_pptp(0,"welcome")==false)
		return false;
	parent.pptp_username=cf.pptp_username.value;
	parent.pptp_password=cf.pptp_passwd.value;
	parent.pptp_idle_time=cf.pptp_idletime.value;
	cf.pptp_myip.value=cf.myip_1.value+'.'+cf.myip_2.value+'.'+cf.myip_3.value+'.'+cf.myip_4.value;
	if(cf.pptp_myip.value=="...")
	{
		cf.pptp_myip.value="";	
		cf.WANAssign.value=0;
		parent.pptp_wan_assign=0;
	}
	else
	{
		cf.WANAssign.value=1;				
		parent.pptp_wan_assign=1;
	}	
	parent.pptp_local_ipaddr=cf.pptp_myip.value;
	parent.pptp_local_gateway=cf.pptp_gateway.value;
	parent.pptp_server_ipaddr=cf.pptp_serv_ip.value;
	parent.pptp_connect_id=cf.pptp_conn_id.value;
	parent.welcome_wan_type=4;
	return true;
}

function RU_pptp_user_info()
{
    var cf=document.forms[0];
    if(cf.pptp_username.value=="")
    {
        alert(login_name_null);
        return false;
    }
    for(i=0;i<cf.pptp_username.value.length;i++)
    {
        if(isValidChar(cf.pptp_username.value.charCodeAt(i))==false)
        {
            alert(loginname_not_allowed);
            return false;
        }
    }
    for(i=0;i<cf.pptp_passwd.value.length;i++)
    {
        if(isValidChar(cf.pptp_passwd.value.charCodeAt(i))==false)
        {
            alert(password_not_allowed);
            return false;
        }
    }
    if(cf.pptp_serv_ip.value=="")
    {
        alert(invalid_servip);
        return false;
    }
    for(i=0;i<cf.pptp_serv_ip.value.length;i++)
    {
        if(isValidChar(cf.pptp_serv_ip.value.charCodeAt(i))==false)
        {
            alert(invalid_servip);
            return false;
        }
    }
    var server_addr_array = cf.pptp_serv_ip.value.split('.');
    var i;
    for( i=0; i<server_addr_array.length; i++ )
    {
        if( server_addr_array[i].length > 63 )
        {
            alert(invalid_servip);
            return false;
        }
    }
    parent.pptp_username=cf.pptp_username.value;
    parent.pptp_password=cf.pptp_passwd.value;
    parent.pptp_server_ipaddr=cf.pptp_serv_ip.value;
    parent.welcome_wan_type=4;

    if(parent.isp_type=="12" )
    {
          cf.basic_type.value="0";
          cf.ppp_login_type.value="1";
          cf.welcome_wan_type.value="4";
          cf.pptp_dnsaddr1.value=parent.static_dns1;
          cf.pptp_dnsaddr2.value=parent.static_dns2;
          if(cf.pptp_dnsaddr1.value != "" || cf.pptp_dnsaddr2.value!= "")
                cf.DNSAssign.value="1";
          else
                cf.DNSAssign.value="0";
         cf.WANAssign.value="0";
         cf.STATIC_DNS.value="0";
         cf.submit();
    }
    else if(parent.isp_type=="11")
    {
          location.href="RU_isp_spoof.htm";

    }
    else if(parent.isp_type=="9" || parent.isp_type=="10")
    {
          location.href="RU_isp_pptp_static.htm";

    }
    else
          location.href="RU_manu_local_resources.htm";
}

function setIP(cf)
{
    var dflag = cf.DualAssign[0].checked;
    setDisabled(dflag,cf.myip_1,cf.myip_2,cf.myip_3,cf.myip_4,cf.mymask_1,cf.mymask_2,cf.mymask_3,cf.mymask_4,cf.mygw_1,cf.mygw_2,cf.mygw_3,cf.mygw_4);
    DisableFixedIP = dflag;

	if( !document.getElementsByTagName("span")[0] ) return false;
	if (cf.DualAssign[1].checked)
	{
		cf.DNSAssign[1].checked = true;
		setDNS(cf);
		cf.DNSAssign[0].disabled = true;
		document.getElementsByTagName("span")[0].style.color = 'gray';
	}
	else
	{
		document.getElementsByTagName("span")[0].style.color = 'black';
		cf.DNSAssign[0].disabled = false;
	}



}


function RU_pptp_servip_FQDN(I)
{
    var cf = document.forms[0];
    servName = cf.pptp_serv_ip.value;
    if(servName.search(/[A-Za-z]/i) != -1){
        parent.pptp_server_FQDN=1;
    }else{
        parent.pptp_server_FQDN=0;
    }
}

function RU_pptp_servip_update(I)
{
    var cf = document.forms[0];
    if((parent.pptp_server_FQDN == 1) &&
        ((cf.myip_1.value.length > 0) ||
         (cf.myip_2.value.length > 0) ||
         (cf.myip_3.value.length > 0) ||
         (cf.myip_4.value.length > 0) ) ){
        if(cf.DNSAssign[0].checked == true){
            cf.DAddr1.disabled=false;
            cf.DAddr2.disabled=false;
            cf.DAddr3.disabled=false;
            cf.DAddr4.disabled=false;

            cf.PDAddr1.disabled=false;
            cf.PDAddr2.disabled=false;
            cf.PDAddr3.disabled=false;
            cf.PDAddr4.disabled=false;
        }
        cf.DNSAssign[0].checked=false;
        cf.DNSAssign[1].checked=true;
        cf.DNSAssign[0].disabled=true;
        cf.STATIC_DNS.value=1;

    }else{
        cf.DNSAssign[0].disabled=false;
        cf.STATIC_DNS.value=0;
    }
}

function RU_check_pptp_dual()
{
    var cf=document.forms[0];
    var wan_assgin = false;
    cf.pptp_myip.value=cf.myip_1.value+'.'+cf.myip_2.value+'.'+cf.myip_3.value+'.'+cf.myip_4.value;
    cf.pptp_mynetmask.value=cf.mymask_1.value+'.'+cf.mymask_2.value+'.'+cf.mymask_3.value+'.'+cf.mymask_4.value;
    cf.pptp_gateway.value=cf.mygw_1.value+'.'+cf.mygw_2.value+'.'+cf.mygw_3.value+'.'+cf.mygw_4.value;
    if(cf.DualAssign[1].checked == true)
    {
        wan_assgin = true;
        parent.pptp_wan_assign=1;
        if(checkipaddr(cf.pptp_myip.value)==false)
        {
            alert(invalid_myip);
            return false;
        }
        if(isSameSubNet(cf.pptp_myip.value,lan_subnet,lan_ip,lan_subnet) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(isSameIp(cf.pptp_myip.value,lan_ip) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(isSameSubNet(cf.pptp_myip.value,cf.pptp_mynetmask.value,lan_ip,cf.pptp_mynetmask.value) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(isSameSubNet(cf.pptp_myip.value,cf.pptp_mynetmask.value,lan_ip,lan_subnet) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(checksubnet(cf.pptp_mynetmask.value)==false)
        {
            alert(invalid_mask);
            return false;
        }
        if ( cf.pptp_gateway.value != "..." && (checkgateway(cf.pptp_gateway.value) == false || is_sub_or_broad(cf.pptp_gateway.value, cf.pptp_gateway.value, cf.pptp_mynetmask.value) == false))
        {
            alert(invalid_gateway);
            return false;
        }
        if( cf.pptp_gateway.value != "..." )
        {
            /*if(isGateway(cf.pptp_myip.value,cf.pptp_mynetmask.value,cf.pptp_gateway.value) == false)
            {
                alert(invalid_gateway);
                return false;
            }*/
            if(isSameIp(cf.pptp_myip.value,cf.pptp_gateway.value) == true)
            {
                alert(invalid_gateway);
                return false;
            }
        }else
            cf.pptp_gateway.value="";
        /*if(isSameSubNet(cf.pptp_myip.value,cf.pptp_mynetmask.value,pptp_serv_ip,cf.pptp_mynetmask.value) != true){
            if(isSameSubNet(cf.pptp_myip.value,cf.pptp_mynetmask.value,cf.pptp_gateway.value,cf.pptp_mynetmask.value) != true){
                alert(invalid_gateway);
                return false;
            }
        }*/
    }else
    {
        cf.pptp_myip.value="";
        cf.pptp_mynetmask.value="";
    	cf.pptp_gateway.value="";
        parent.pptp_wan_assign=0;
    }
    if(cf.DNSAssign[1].checked == true)
    {
        cf.pptp_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
        cf.pptp_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;

        if(cf.pptp_dnsaddr1.value=="...")
            cf.pptp_dnsaddr1.value="";
        if(cf.pptp_dnsaddr2.value=="...")
            cf.pptp_dnsaddr2.value="";

        if(!check_RU_DNS(cf.pptp_dnsaddr1.value,cf.pptp_dnsaddr2.value,wan_assgin,cf.pptp_myip.value))
            return false;
    }else
    {
        cf.pptp_dnsaddr1.value="";
        cf.pptp_dnsaddr2.value="";
    }
    if(cf.DualAssign[1].checked == true)
        alert(attention_static_ip);
    parent.pptp_local_ipaddr=cf.pptp_myip.value;
    parent.pptp_local_netmask=cf.pptp_mynetmask.value;
    parent.pptp_local_gateway=cf.pptp_gateway.value;
    parent.pptp_dns1=cf.pptp_dnsaddr1.value;
    parent.pptp_dns2=cf.pptp_dnsaddr2.value;
    parent.STATIC_DNS=cf.STATIC_DNS.value;
    parent.conflict_wanlan=cf.conflict_wanlan.value;
    location.href="RU_manual_spoof.htm";
}

