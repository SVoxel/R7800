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

function myip_update()
{
    var cf = document.forms[0];

    if((cf.myip_1.value.length>0)&&(cf.myip_2.value.length>0)&&(cf.myip_3.value.length>0)&&(cf.myip_4.value.length>0))
    {
        setDisabled(false, cf.mymask_1, cf.mymask_2, cf.mymask_3, cf.mymask_4);
    }
    else
    {
       	setDisabled(true, cf.mymask_1, cf.mymask_2, cf.mymask_3, cf.mymask_4);
    }

}
function setIP(cf,page)
{
	var dflag = cf.WANAssign[0].checked;
	setDisabled(dflag,cf.WPethr1,cf.WPethr2,cf.WPethr3,cf.WPethr4);

	/* to fix bug 26929
	if(russian_pppoe_flag == 1 && gui_region == "Russian" && page == "bas" )
		setDisabled(dflag,cf.WMask1,cf.WMask2,cf.WMask3,cf.WMask4);
	*/
        DisableFixedIP = dflag;
	if( !document.getElementsByTagName("span")[0] ) return false;
	if (cf.WANAssign[1].checked)
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

function setDNS(cf)
{
	var dflag = cf.DNSAssign[0].checked;
	setDisabled(dflag,cf.DAddr1,cf.DAddr2,cf.DAddr3,cf.DAddr4,cf.PDAddr1,cf.PDAddr2,cf.PDAddr3,cf.PDAddr4);
	DisableFixedDNS = dflag;
}

function change_pppoe_password(obj)
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
			obj.outerHTML = '<input type="text" name="pppoe_passwd" maxlength="64" size="16" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
			document.forms[0].pppoe_passwd.focus();
			document.forms[0].pppoe_passwd.focus();
		}
	}
	document.forms[0].hidden_pwd_change.value="1";
}

function check_wizard_pppoe(check,servername,page)
{
	var form=document.forms[0];
	if(servername=="")
	{
		alert("$login_name_null");
		return false;
	}
	for(i=0;i<servername.length;i++)
	{
		if(isValidChar(servername.charCodeAt(i))==false)
		{
			alert("$loginname_not_allowed");
			return false;
		}
	}

	for(i=0;i<form.pppoe_passwd.value.length;i++)
	{
		if(isValidChar(form.pppoe_passwd.value.charCodeAt(i))==false)
		{
			alert("$password_error");
			return false;
		}
	}
	for(i=0;i<servername.length;i++)
	{
		if(isValidChar(servername.charCodeAt(i))==false)
		{
			alert("$servname_not_allowed");
			return false;
		}
	}
	if(form.pppoe_dod.value == "1")
	{
		if(form.pppoe_idletime.value.length<=0)
		{
			alert("$idle_time_null");
			return false;
		}
		else if(!_isNumeric(form.pppoe_idletime.value))
		{
			alert("$invalid_idle_time");
			return false;
		}
	}
        if (check == 1)
        {
                form.run_test.value="test";
                if( page == "wiz")
                        form.action="/apply.cgi?/WIZ_update.htm timestamp="+ts;
        }
        else
                form.run_test.value="no";

	return true;
}

function check_pppoe(form,check)
{
	//bug 23249, change the input value format
	format_IP("myip_1", "myip_2", "myip_3", "myip_4", "mymask_1", "mymask_2", "mymask_3", "mymask_4", "WPethr1", "WPethr2", "WPethr3", "WPethr4", "DAddr1", "DAddr2", "DAddr3", "DAddr4", "PDAddr1", "PDAddr2", "PDAddr3", "PDAddr4");

	if(form.auto_conn_24hr.checked == true)
		form.hidden_auto_conn_flag.value=1;
	else
		form.hidden_auto_conn_flag.value=0;
	form.hidden_conn_time_num.value=form.auto_conn_time.selectedIndex;
	
	if((form.country.value == "Germany" && form.isp.value == "Deutsche Telekom" && form.gisp_type[1].checked == true) || form.country.value == "Germany" && form.isp.value == "1&1"){
		if(form.g1login[0].checked == true )
		{
			form.hid_radio_11.value = "0";
			form.hid_pppoe_username.value = "1und1/"+form.pppoe_gusername1.value+"@online.de";
			form.hid_pppoe_usernamep.value = form.pppoe_gusername1.value
		}else if(form.g1login[1].checked == true )
		{
			form.hid_radio_11.value = "1";
			form.hid_pppoe_username.value = "H1und1/"+form.pppoe_gusername2.value+"@online.de";
			form.hid_pppoe_usernamep.value = form.pppoe_gusername2.value
		}else
		{
			form.hid_radio_11.value = "2";
			form.hid_pppoe_username.value =form.pppoe_gusername3.value;
			form.hid_pppoe_usernamep.value = form.pppoe_gusername3.value
		}
		if(check_wizard_pppoe(check,form.hid_pppoe_username.value,"bas")==false)
			return false;
	}
	else if (form.country.value == "Germany" && form.isp.value == "Deutsche Telekom" && form.gisp_type[0].checked == true){
		form.hid_pppoe_username.value = form.connection_name.value+form.pppoe_onlnum.value +form.pppoe_suffix.value;	
		if(check_wizard_pppoe(check,form.hid_pppoe_username.value,"bas")==false)
			return false;
		form.hid_pppoe_username.value = form.hid_pppoe_username.value + "@t-online.de";
		}
	else{
		form.hid_pppoe_username.value = form.pppoe_username.value;	
		if(check_wizard_pppoe(check,form.hid_pppoe_username.value,"bas")==false)
			return false;
		}
	form.hidden_pppoe_idle_time.value = form.pppoe_idletime.value;
	form.hid_pppoe_dod.value = form.pppoe_dod.value;//bug 22322:when connection mode gray out,need a hidden-value to transfer
	if(pppoe_myip_flag == 1)
	{
      		form.pppoe_myip.value=form.myip_1.value+'.'+form.myip_2.value+'.'+form.myip_3.value+'.'+form.myip_4.value;
        	form.pppoe_mask.value=form.mymask_1.value+'.'+form.mymask_2.value+'.'+form.mymask_3.value+'.'+form.mymask_4.value;
		if(form.pppoe_myip.value !="...")
		{
			form.intranet_wan_assign.value = 1;
	                if(checkipaddr(form.pppoe_myip.value)==false)
        	        {
                	        alert("$invalid_myip");
                	        return false;
                	}
                	if(isSameSubNet(form.pppoe_myip.value,lan_subnet,lan_ip,lan_subnet) == true)
                	{
                        	form.conflict_wanlan.value=1;
                	}
                	if(isSameIp(form.pppoe_myip.value,lan_ip) == true)
                	{
                	        form.conflict_wanlan.value=1;
                	}
                        if( form.pppoe_mask.value == "..." )
                        {
                                if( parseInt(form.myip_1.value) < 128 )
                                        form.pppoe_mask.value="255.0.0.0";
                                else if(parseInt(form.myip_1.value) < 192 )
                                        form.pppoe_mask.value="255.255.0.0";
                                else
                                        form.pppoe_mask.value="255.255.255.0";
		
                        }
                        if(checksubnet(form.pppoe_mask.value, 1)==false)
                        {
                                alert("$invalid_mask");
                                return false;
                        }
                        if(isSameSubNet(form.pppoe_myip.value,form.pppoe_mask.value,lan_ip,form.pppoe_mask.value) == true)
                        {
                                form.conflict_wanlan.value=1;
                        }
                        if(isSameSubNet(form.pppoe_myip.value,form.pppoe_mask.value,lan_ip,lan_subnet) == true)
                        {
                                cf.conflict_wanlan.value=1;
                        }
			form.pppoe_myip.value = address_parseInt(form.pppoe_myip.value);
			form.pppoe_mask.value = address_parseInt(form.pppoe_mask.value);
		}
	        else{
	                form.pppoe_myip.value="";
	                form.pppoe_mask.value="";
	                form.intranet_wan_assign.value = 0;
	        }	
	}

	if(form.WANAssign[1].checked == true)
	{	
		form.pppoe_ipaddr.value=form.WPethr1.value+'.'+form.WPethr2.value+'.'+form.WPethr3.value+'.'+form.WPethr4.value;
                if(checkipaddr(form.pppoe_ipaddr.value)==false || is_sub_or_broad(form.pppoe_ipaddr.value, lan_ip, lan_subnet) == false)
		{
			alert("$invalid_ip");
			return false;
		}
                if(isSameSubNet(form.pppoe_ipaddr.value,lan_subnet,lan_ip,lan_subnet) == true)
                {
                        form.conflict_wanlan.value=1;
                }
                if(isSameIp(form.pppoe_ipaddr.value,lan_ip) == true)
                {
                        form.conflict_wanlan.value=1;
                }

		/* to fix bug 26929
		if(russian_pppoe_flag == 1 && gui_region == "Russian")
                {
                	form.pppoe_subnet.value=form.WMask1.value+'.'+form.WMask2.value+'.'+form.WMask3.value+'.'+form.WMask4.value;
                	if((form.pppoe_subnet.value != "...")&&(form.pppoe_subnet.value != "0.0.0.0")) 
                        {
				if (checksubnet(form.pppoe_subnet.value, 0)==false)
        			{
	        			alert("$invalid_mask");
	        			return false;
        			}
                		if(isSameSubNet(form.pppoe_ipaddr.value,form.pppoe_subnet.value,lan_ip,lan_subnet) == true)
				{
					form.conflict_wanlan.value=1;	
				}
                		if(isSameSubNet(form.pppoe_ipaddr.value,form.pppoe_subnet.value,lan_ip,form.pppoe_subnet.value) == true)
				{
					form.conflict_wanlan.value=1;
				}
			}
			else
				form.pppoe_subnet.value = "";	
		}
		*/
	}
	if (form.DNSAssign[1].checked == true)
	{
		form.pppoe_dnsaddr1.value=form.DAddr1.value+'.'+form.DAddr2.value+'.'+form.DAddr3.value+'.'+form.DAddr4.value;
		form.pppoe_dnsaddr2.value=form.PDAddr1.value+'.'+form.PDAddr2.value+'.'+form.PDAddr3.value+'.'+form.PDAddr4.value;
		form.pppoe_ipaddr.value=form.WPethr1.value+'.'+form.WPethr2.value+'.'+form.WPethr3.value+'.'+form.WPethr4.value;

                if(form.pppoe_dnsaddr1.value=="...")
                        form.pppoe_dnsaddr1.value="";
		else
			form.pppoe_dnsaddr1.value = address_parseInt(form.pppoe_dnsaddr1.value);
	
		if(form.pppoe_dnsaddr2.value=="...")
			form.pppoe_dnsaddr2.value="";
		else
			form.pppoe_dnsaddr2.value = address_parseInt(form.pppoe_dnsaddr2.value);
		
		if( !check_DNS(form.pppoe_dnsaddr1.value,form.pppoe_dnsaddr2.value,form.WANAssign[1].checked,form.pppoe_ipaddr.value))
			return false;
	}
	form.hid_mtu_value.value=wan_mtu_now;
	form.pppoe_ipaddr.value = address_parseInt(form.pppoe_ipaddr.value);
	if ( !(old_wan_type=="pppoe"))
	{
		form.change_wan_type.value=0;
		mtu_change(wanproto_type);
	}
	else if ( old_pppoe_wan_assign == "1")
	{
		if( old_wan_ip!= form.pppoe_ipaddr.value && form.WANAssign[1].checked)
			form.change_wan_type.value=0;
		else if(form.WANAssign[0].checked)
			form.change_wan_type.value=0;
		else
			form.change_wan_type.value=1;	
	}
	else if( old_pppoe_wan_assign == "0")
	{
		if( old_wan_ip!=form.pppoe_ipaddr.value && form.WANAssign[1].checked)
			form.change_wan_type.value=0;
		else
			form.change_wan_type.value=1;
	}
        if (form.MACAssign[2].checked )
        {
                the_mac=form.Spoofmac.value;
                if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
                {
                        var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
                        form.Spoofmac.value = tmp_mac;
                }
                else if ( the_mac.split("-").length == 6 )
                {
                        var tmp_mac = the_mac.replace(/-/g,":");
                        form.Spoofmac.value=tmp_mac;
                }
                if(maccheck_multicast(form.Spoofmac.value) == false)
                        return false;
        }
	form.hid_enable_vpn.value = vpn_enable;
	if(old_endis_ddns != "1" && vpn_enable == "1" )
        {
                if(confirm("$no_change_static_ip") == true)
                        form.hid_enable_vpn.value = "0";
                else{
                        form.hid_enable_vpn.value = "1";
                        return false;
                }
        }
		
		form.hid_dsl_country.value = form.country.value;
		form.hid_dsl_isp.value = form.isp.value;
		
		form.hid_transfermode.value = form.transfermode.value;
		form.hid_dslmode.value = form.dslmode.value;
		//if(form.hid_wan_num.value == "2")
		//	form.hid_adslChoice.value = form.adslChoice.value;
		//else
		form.hid_adslChoice.value ="0";
		form.hid_dsl_vpi.value = form.dsl_vpi.value;
		form.hid_dsl_vci.value = form.dsl_vci.value;
		form.hid_dsl_multiplex.value = form.dsl_multiplex.value;
		form.hid_vlan_id.value = form.vlan_id.value;
		if(form.enable_wan.checked == true)
			form.hid_enable_wan.value = "1";
		else
			form.hid_enable_wan.value = "0"; 
		
		if(form.vlanidActive.checked == true)
			form.hid_enable_vlanidActive.value = "1";
		else
			form.hid_enable_vlanidActive.value = "0";
		if (dsl_wan_preference == "1")
			form.hid_port_mode.value = "1";
		else if (dsl_wan_preference == "2")
			form.hid_port_mode.value = "0";
		form.hid_priority_value.value = form.priority_value.value;
		if(form.gisp_type[0].checked == true)
			form.hid_gisp_type.value = "0";
		else if(form.gisp_type[1].checked == true)
			form.hid_gisp_type.value = "1";
		else
			form.hid_gisp_type.value = "2";
	if (dsl_wan_preference =="2")
	{
		form.hid_wan1_config.value = "1";
		form.hid_wan2_config.value = "0";
	}
	else if(form.transfermode.value == "adsl" && dsl_wan_preference =="1")
	{
		if((form.country.value == "France" && form.isp.value == "SFR") || (form.country.value == "Australia" && form.isp.value == "TPG") || (form.country.value == "Germany" && form.isp.value == "Deutsche Telekom")||(form.country.value == "UK" && form.isp.value == "BT")){
		if(form.country.value != true_country || form.isp.value != true_isp) // fisrt to config this country and ISP
		{
			if(form.adslChoice.value == "0" && dsl_wan_preference =="1" )//submit is wan1
			{
				form.hid_wan1_config.value = "1";
				form.hid_wan2_config.value = "0";
			}
		}else if(form.country.value == true_country && form.isp.value == true_isp)	//sencond to config this country and isp 
		{
			if(form.adslChoice.value == "0" && dsl_wan_preference =="1" )//submit is wan1
			{
				form.hid_wan1_config.value = "1";
				form.hid_wan2_config.value = wan2_flag;
			}
		}
	  }else{
		form.hid_wan1_config.value = "1";
		form.hid_wan2_config.value = "0";
	  }
	  if((form.country.value == "Australia" && form.isp.value == "TPG") || (form.country.value == "UK" && form.isp.value == "BT"))
		form.hid_double_vpi.value="1";
	}else{ //vdsl
		if(form.country.value != true_country || form.isp.value != true_isp) // fisrt(wan1 or wan2) to config this country and ISP
		{
			if(form.adslChoice.value == "0" && dsl_wan_preference =="1") //submit is wan1
			{
				form.hid_wan1_config.value = "1";
				form.hid_wan2_config.value = "0";
			}
			/*else if(form.adslChoice.value == "1") // submit is wan2
			{
				form.hid_wan1_config.value = "0";
				form.hid_wan2_config.value = "1";
			}*/
		}else if(form.country.value == true_country && form.isp.value == true_isp)	//sencond to config this country and isp 
		{
			if(form.adslChoice.value == "0" && dsl_wan_preference =="1") //submit is wan1
			{
				form.hid_wan1_config.value = "1";
				form.hid_wan2_config.value = wan2_flag;
			}
		}
	}
		return true;

}

function setIP_welcome_pppoe()
{
	cf=document.forms[0];
	var dflag = cf.WANAssign[0].checked;
	setDisabled(dflag,cf.WPethr1,cf.WPethr2,cf.WPethr3,cf.WPethr4);
	DisableFixedIP = dflag;
}

function setDNS_welcome_pppoe()
{
	cf=document.forms[0];
	var dflag = cf.DNSAssign[0].checked;
	setDisabled(dflag,cf.DAddr1,cf.DAddr2,cf.DAddr3,cf.DAddr4,cf.PDAddr1,cf.PDAddr2,cf.PDAddr3,cf.PDAddr4);
	DisableFixedDNS = dflag;
}

function check_wizard_pppoe_new(check)
{
	var cf=document.forms[0];
	if(check_wizard_pppoe(check,cf.pppoe_servicename.value,"wiz")==false)
		return false;
	cf.pppoe_ipaddr.value=cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;
	if(cf.WANAssign[1].checked)
	{
		if(checkipaddr(cf.pppoe_ipaddr.value)==false)
		{
			alert("$invalid_ip");
			return false;
		}
	}
	if(cf.DNSAssign[1].checked == true)
	{
		cf.pppoe_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
		cf.pppoe_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;

                if(cf.pppoe_dnsaddr1.value=="...")
                        cf.pppoe_dnsaddr1.value="";

                if(cf.pppoe_dnsaddr2.value=="...")
                        cf.pppoe_dnsaddr2.value="";

		if( !check_DNS(cf.pppoe_dnsaddr1.value,cf.pppoe_dnsaddr2.value,cf.WANAssign[1].checked,cf.pppoe_ipaddr.value))
			return false;

		cf.pppoe_dnsaddr1.value = address_parseInt(cf.pppoe_dnsaddr1.value);
		cf.pppoe_dnsaddr2.value = address_parseInt(cf.pppoe_dnsaddr2.value);
	}
	cf.pppoe_ipaddr.value = address_parseInt(cf.pppoe_ipaddr.value);
}
function check_welcome_pppoe()
{
	var cf=document.forms[0];
	if(check_wizard_pppoe(0, cf.pppoe_servername.value)==false)
		return false;
	cf.pppoe_ipaddr.value=cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;	
	if(cf.WANAssign[1].checked)
	{
		if(checkipaddr(cf.pppoe_ipaddr.value)==false || is_sub_or_broad(cf.pppoe_ipaddr.value, lan_ip, lan_subnet) == false)
		{
			alert("$invalid_ip");
			return false;
		}
	}
	if(cf.DNSAssign[1].checked == true)
	{
		cf.pppoe_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
		cf.pppoe_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;
	
                if(cf.pppoe_dnsaddr1.value=="...")
                        cf.pppoe_dnsaddr1.value="";

                if(cf.pppoe_dnsaddr2.value=="...")
                        cf.pppoe_dnsaddr2.value="";

		if( !check_DNS(cf.pppoe_dnsaddr1.value,cf.pppoe_dnsaddr2.value,cf.WANAssign[1].checked,cf.pppoe_ipaddr.value))
			return false;	
	}
	parent.pppoe_username=cf.pppoe_username.value;
	parent.pppoe_passwd=cf.pppoe_passwd.value;
	parent.pppoe_server=cf.pppoe_servername.value;
	parent.pppoe_idle=cf.pppoe_idletime.value;
	if(cf.WANAssign[1].checked)
	{
		parent.pppoe_wan_assign=1;
		parent.pppoe_static_ip=cf.pppoe_ipaddr.value;
	}
	else
	{
		parent.pppoe_wan_assign=0;
		parent.pppoe_static_ip="";
	}
	if(cf.DNSAssign[1].checked == true)
	{
		parent.pppoe_dns_assign=1;
		parent.pppoe_dns1.value=cf.pppoe_dnsaddr1.value;
		parent.pppoe_dns2.value=cf.pppoe_dnsaddr2.value;
	}
	else
	{
		parent.pppoe_dns_assign=0;
		parent.pppoe_dns1.value="";
		parent.pppoe_dns2.value="";
	}
	parent.welcome_wan_type=3;
	return true;
}

function RU_pppoe_user_info()
{
    var cf=document.forms[0];
    if(cf.pppoe_username.value=="")
    {
        alert(login_name_null);
        return false;
    }
    for(i=0;i<cf.pppoe_username.value.length;i++)
    {
        if(isValidChar(cf.pppoe_username.value.charCodeAt(i))==false)
        {
            alert(loginname_not_allowed);
            return false;
        }
    }
    for(i=0;i<cf.pppoe_passwd.value.length;i++)
    {
        if(isValidChar(cf.pppoe_passwd.value.charCodeAt(i))==false)
        {
            alert(password_not_allowed);
            return false;
        }
    }
    for(i=0;i<cf.pppoe_servername.value.length;i++)
    {
        if(isValidChar(cf.pppoe_servername.value.charCodeAt(i))==false)
        {
            alert(servname_not_allowed);
            return false;
        }
    }

    parent.pppoe_username=cf.pppoe_username.value;
    parent.pppoe_password=cf.pppoe_passwd.value;
    parent.pppoe_server=cf.pppoe_servername.value;
    parent.welcome_wan_type=3;

    if(parent.isp_type=="8")
    {
          cf.basic_type.value="0";
          cf.ppp_login_type.value="0";
          cf.welcome_wan_type.value="3";
          cf.dual_access.value="1";
          cf.pppoe_dual_assign.value=0;
          cf.conflict_wanlan.value=0;
          cf.pppoe_dual_ipaddr.value="";
          cf.pppoe_dual_subnet.value="";
          cf.pppoe_dnsaddr1.value=parent.static_dns1;
          cf.pppoe_dnsaddr2.value=parent.static_dns2;
          if(cf.pppoe_dnsaddr1.value != "" || cf.pppoe_dnsaddr2.value!= "")
                cf.DNSAssign.value="1";
          else
                cf.DNSAssign.value="0";
          cf.WANAssign.value="Dynamic";
          cf.pppoe_dod.value="0";
          cf.pppoe_servername.value=parent.pppoe_server;
          cf.submit();
    }
    else if(parent.isp_type=="18")
    {
          cf.basic_type.value="0";
          cf.ppp_login_type.value="0";
          cf.welcome_wan_type.value="3";
          cf.dual_access.value="0";
          cf.pppoe_dual_assign.value=0;
          cf.conflict_wanlan.value=0;
          cf.pppoe_dual_ipaddr.value="";
          cf.pppoe_dual_subnet.value="";
          cf.DNSAssign.value="0";
          cf.WANAssign.value="Dynamic";
          cf.pppoe_dod.value="0";
          cf.pppoe_servername.value=parent.pppoe_server;
          cf.submit();
    }
    else if(parent.isp_type=="7" || parent.isp_type=="17")
    {
          location.href="RU_isp_spoof.htm";
    }
    else if(parent.isp_type=="5" || parent.isp_type=="6")
    {
          location.href="RU_isp_pppoe_static.htm";
    }
    else if(parent.isp_type=="88")
    {
          location.href="RU_isp_pppoe_static.htm";
    }
    else
          location.href="RU_manual_pppoe2.htm";
}
function RU_check_pppoe()
{
    var cf=document.forms[0];
    cf.pppoe_ipaddr.value=cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;
    if(cf.WANAssign[0].checked == true)
    {
        cf.pppoe_ipaddr.value="";
        parent.pppoe_wan_assign="0";
    }
    else
    {
        if(checkipaddr(cf.pppoe_ipaddr.value)==false || is_sub_or_broad(cf.pppoe_ipaddr.value, lan_ip, lan_subnet) == false)
        {
            alert(invalid_myip);
            return false;
        }
        if(isSameSubNet(cf.pppoe_ipaddr.value,lan_subnet,lan_ip,lan_subnet) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(isSameIp(cf.pppoe_ipaddr.value,lan_ip) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        parent.pppoe_wan_assign="1";
    }
    if (cf.DNSAssign[1].checked == true)
    {
        cf.pppoe_dnsaddr1.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
        cf.pppoe_dnsaddr2.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;

        if(cf.pppoe_dnsaddr1.value=="...")
            cf.pppoe_dnsaddr1.value="";

        if(cf.pppoe_dnsaddr2.value=="...")
            cf.pppoe_dnsaddr2.value="";

        var wan_assign_flag;
        if(parent.pppoe_wan_assign == "0")
            wan_assign_flag=false;
        else
            wan_assign_flag=true;
        if( !check_RU_DNS(cf.pppoe_dnsaddr1.value,cf.pppoe_dnsaddr2.value,wan_assign_flag,parent.pppoe_static_ip))
            return false;
    }
    else
    {
        cf.pppoe_dnsaddr1.value="";
        cf.pppoe_dnsaddr2.value="";
    }
    parent.pppoe_static_ip=cf.pppoe_ipaddr.value;
    parent.pppoe_dns1=cf.pppoe_dnsaddr1.value;
    parent.pppoe_dns2=cf.pppoe_dnsaddr2.value;
    location.href="RU_manu_local_resources.htm";
}

function setdualIP(cf)
{
    var dflag = cf.DualAssign[0].checked;
    setDisabled(dflag,cf.Duethr1,cf.Duethr2,cf.Duethr3,cf.Duethr4,cf.DuMask1,cf.DuMask2,cf.DuMask3,cf.DuMask4);
    DisableFixedIP = dflag;
}

function RU_check_pppoe_dual()
{
    var cf=document.forms[0];
    cf.pppoe_dual_ipaddr.value=cf.Duethr1.value+'.'+cf.Duethr2.value+'.'+cf.Duethr3.value+'.'+cf.Duethr4.value;
    cf.pppoe_dual_subnet.value=cf.DuMask1.value+'.'+cf.DuMask2.value+'.'+cf.DuMask3.value+'.'+cf.DuMask4.value;
    cf.conflict_wanlan.value=0;
    if(cf.DualAssign[1].checked == true)
    {
        cf.pppoe_dual_assign.value=1;
        if(checkipaddr(cf.pppoe_dual_ipaddr.value)==false || is_sub_or_broad(cf.pppoe_dual_ipaddr.value, cf.pppoe_dual_ipaddr.value, cf.pppoe_dual_subnet.value) == false)
        {
            alert(invalid_myip);
            return false;
        }
        if(checksubnet(cf.pppoe_dual_subnet.value)==false)
        {
            alert(invalid_mask);
            return false;
        }
        cf.pppoe_dual_assign.value=1;
        if(isSameSubNet(cf.pppoe_dual_ipaddr.value,lan_subnet,lan_ip,lan_subnet) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        if(isSameIp(cf.pppoe_dual_ipaddr.value,lan_ip) == true)
        {
            cf.conflict_wanlan.value=1;
        }
        alert(attention_static_ip);
    }else{
        cf.pppoe_dual_assign.value=0;
        cf.pppoe_dual_ipaddr.value="";
        cf.pppoe_dual_subnet.value="";
    }

    parent.conflict_wanlan=cf.conflict_wanlan.value;
    parent.pppoe_dual_assign=cf.pppoe_dual_assign.value;
    parent.pppoe_eth_ip=cf.pppoe_dual_ipaddr.value;
    parent.pppoe_eth_netmask=cf.pppoe_dual_subnet.value;
    location.href="RU_manual_spoof.htm";
}

