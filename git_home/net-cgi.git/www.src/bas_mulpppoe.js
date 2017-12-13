function show_hidden_help(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >=9)
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

function show_hidden_help_top_button(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >=8)
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

function loadMPhelp(fname,anchname)
{
    if ((loadMPhelp.arguments.length == 1 ) || (anchname == "" )){
                this.helpframe.location.href="/h"+fname+".html";
        //"/h"+fname+".html";
    }
        else{
                this.helpframe.location.href="/h"+fname+".html#" + anchname;
        //"/h"+fname+".html#" + anchname;
    }
    return;
}
function getObj(name)
{
    if (document.getElementById)
    {
        return document.getElementById(name);
    }
    else if (document.all)
    {
        return document.all[name];
    }
    else if (document.layers)
    {
        return document.layers[name];
    }
}
function setIP(cf)
{
	var dflag = cf.WANAssign[0].checked;
	setDisabled(dflag,cf.WPethr1,cf.WPethr2,cf.WPethr3,cf.WPethr4);
	if (cf.WANAssign[1].checked)
	{	
		cf.DNSAssign[1].checked = true;
		setDNS(cf);
	}
	DisableFixedIP = dflag;
}
function setIP2(cf)
{
         
	 var dflag2 = cf.WANAssign2[0].checked;
         setDisabled(dflag2,cf.pppoe2_ip1,cf.pppoe2_ip2,cf.pppoe2_ip3,cf.pppoe2_ip4);
         if (cf.WANAssign2[1].checked)
         {
                 cf.DNSAssign2[1].checked = true;
                 setDNS2(cf);
         }
         DisableFixedIP = dflag2;

}

function setDNS(cf)
{	
	var dflag = cf.DNSAssign[0].checked;
	if (cf.WANAssign[1].checked)
	{
		cf.DNSAssign[1].checked=true;
		dflag = false;
	}
	setDisabled(dflag,cf.PDAddr1,cf.PDAddr2,cf.PDAddr3,cf.PDAddr4,cf.DAddr1,cf.DAddr2,cf.DAddr3,cf.DAddr4);
	DisableFixedDNS = dflag;

}
function setDNS2(cf)
{
        var dflag2 = cf.DNSAssign2[0].checked;
        if (cf.WANAssign2[1].checked)
        {
     	        cf.DNSAssign2[1].checked=true;
                dflag2 = false;
        }
        setDisabled(dflag2,cf.pppoe2_dnsp1,cf.pppoe2_dnsp2,cf.pppoe2_dnsp3,cf.pppoe2_dnsp4,cf.pppoe2_dnss1,cf.pppoe2_dnss2,cf.pppoe2_dnss3,cf.pppoe2_dnss4);
        DisableFixedDNS = dflag2;

}

function change_mulpppoe_password(obj)
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
			if( obj.name == "pppoe_passwd" )
			{
				obj.outerHTML = '<input type="text" name="pppoe_passwd" maxlength="64" size="16" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
				document.forms[0].pppoe_passwd.focus();
				document.forms[0].pppoe_passwd.focus();
			}
			else if( obj.name == "pppoe2_password" )
			{
				obj.outerHTML = '<input type="text" name="pppoe2_password" maxlength="64" size="16" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" value="">';
				document.forms[0].pppoe2_password.focus();
				document.forms[0].pppoe2_password.focus();
			}
		}
	}
}

function setPolicy()
{
	var cf = document.forms[0];
	var select_index=cf.pppoe2_policy.selectedIndex;
	if(select_index== 0)
	{
		getObj("mul_doname").innerHTML=str3_div;
		cf.userdefined.disabled =false;
	}
	else if(select_index == 2)
	{
		getObj("mul_protocol").innerHTML=str1_div;	
		getObj("mul_port").innerHTML=str2_div;
		cf.protocol.disabled=false;
		cf.portstart.disabled=false;
		cf.portend.disabled=false;
		
	}
	else if(select_index == 1)
	{
		getObj("mul_protocol").innerHTML=str1_div;
		getObj("mul_ip").innerHTML=str4_div;
		cf.protocol.disabled=false;
		cf.start_ip1.disabled =false;
                cf.start_ip2.disabled =false;
                cf.start_ip3.disabled =false;
                cf.start_ip4.disabled =false;
            //    cf.end_ip1.disabled =false;
            //    cf.end_ip2.disabled =false;
            //    cf.end_ip3.disabled =false;
                cf.end_ip4.disabled =false;

	}
	else if (select_index == 3)
	{
		cf.protocol.disabled=false;
		cf.portstart.disabled=false;
		cf.portend.disabled=false;
		cf.userdefined.disabled=true;
		cf.start_ip1.disabled=false;
		cf.start_ip2.disabled=false;
		cf.start_ip3.disabled=false;
		cf.start_ip4.disabled=false;
		cf.end_ip4.disabled=false;

	}
}

function check_mulpppoe_add(cf,flag)
{
	//bug 23249, change the input value format
	format_IP("start_ip1", "start_ip2", "start_ip3", "start_ip4", "end_ip4");

	var txt,c;
	cf.hidden_portstart.value="--";
	cf.hidden_portend.value="--";
	cf.hidden_protocol_value.value="--";
	cf.hidden_userdefined.value="--";
	cf.hidden_ip_start.value="--";
	cf.hidden_ip_end.value="--";
	if(cf.pppoe2_policy.selectedIndex == 2 || cf.pppoe2_policy.selectedIndex ==3 )
	{	
		if(cf.portstart.value==""||cf.portend.value=="")
		{
			alert("$invalid_port");
			return false;
		}
		txt=cf.portstart.value;
		for(i=0;i<txt.length;i++)
		{
			c=txt.charAt(i);
			if("0123456789".indexOf(c,0)<0)
			{
				alert("$invalid_start_port");
				return false;
			}
		}
		txt=cf.portend.value;
		for(i=0;i<txt.length;i++)
		{
			c=txt.charAt(i);
			if("0123456789".indexOf(c,0)<0)
			{
				alert("$invalid_end_port");
				return false;
			}
		}
		if(parseInt(cf.portstart.value)<1||parseInt(cf.portstart.value)>65535)
		{
			alert("$invalid_start_port");
			return false;
		}
		if(parseInt(cf.portend.value)<1||parseInt(cf.portend.value)>65535)
			{
				alert("$invalid_end_port");
				return false;
			}
		if(parseInt(cf.portend.value)<parseInt(cf.portstart.value))
		{
			alert("$end_port_greater");
			return false;
		}
		for(i=1;i<=array_num;i++)
        	{
				var str = eval ( 'mulpppoeArray' + i );
				var each_info=str.split(' ');
				if(each_info[4] == "--" || each_info[5] == "--")
					continue;
				if(flag == 'edit')
				{
					if(!(parseInt(each_info[4])>parseInt(cf.portend.value) || parseInt(each_info[5])<parseInt(cf.portstart.value)) && i!=select_edit)
					{
						alert("$port_dup");
						return false;
					}	
				}
				else
				{
					if(!(parseInt(each_info[4])>parseInt(cf.portend.value) || parseInt(each_info[5])<parseInt(cf.portstart.value)))
					{
						alert("$port_dup");
						return false;
					}
				}
		}	
		cf.hidden_portstart.value=cf.portstart.value;
		cf.hidden_portend.value=cf.portend.value;
	}
	if(cf.pppoe2_policy.selectedIndex == 0 )
	{
		txt=cf.userdefined.value;
		if(txt=="")
		{
			alert("$invalid_user_type");
			return false;
		}
		for(i=0;i<cf.userdefined.value.length;i++)
	        {
        	        if(isValidDdnsHost(cf.userdefined.value.charCodeAt(i))==false)
               		{
                        	alert("$invalid_user_type");
                        	return false;
                	}
        	}

		for(i=1;i<=array_num;i++)
        	{
				var str = eval ( 'mulpppoeArray' + i );
				var each_info=str.split(' ');
				if(flag == 'edit')
				{
	                		if(each_info[1]==txt && i!=select_edit)
	                		{
	                        		alert("$doname_dup");
	                        		return false;
	                		}	
				}
				else
				{
					if(each_info[1]==txt)
	                		{	
	                        		alert("$doname_dup");
	                        		return false;
	       				}
				}
        	}	
		cf.hidden_userdefined.value=cf.userdefined.value;
	}
        else
		cf.hidden_protocol_value.value=cf.protocol.value;

	if(cf.pppoe2_policy.selectedIndex == 1 || cf.pppoe2_policy.selectedIndex == 3)	
	{
	        cf.ip_start.value=cf.start_ip1.value+'.'+cf.start_ip2.value+'.'+cf.start_ip3.value+'.'+cf.start_ip4.value;
        	cf.ip_end.value=cf.start_ip1.value+'.'+cf.start_ip2.value+'.'+cf.start_ip3.value+'.'+cf.end_ip4.value;
		if(checkipaddr(cf.ip_start.value)==false)
		{
			alert("$invalid_ip");
			return false;
		}
		if(checkipaddr(cf.ip_end.value)==false)
		{
			alert("$invalid_ip");
			return false;
		}
		if(cp_ip2(cf.ip_start.value,cf.ip_end.value)==false)
		{
			alert("$invalid_ip_rang");
			return false;
		}

		cf.hidden_ip_start.value=cf.ip_start.value;
		cf.hidden_ip_end.value=cf.ip_end.value

	}
	return true;
}


function check_mulpppoe_addnum(cf,flag)
{
	cf.submit_flag.value="mulpppoe_addnum";
	if(flag == 1)
		cf.action="/apply.cgi?/BAS_mulpppoe_add.htm timestamp="+ts;
	else
		cf.action="/apply.cgi?/BAS_mulpppoe_add_ww.htm timestamp="+ts;
	return true;
}

function check_mulpppoe_editnum(cf,flag)
{
	if( array_num == "" )
	{
		alert("$port_edit");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.select_mulpppoe.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.select_mulpppoe[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$select_serv_edit");
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="mulpppoe_editnum";
		if(flag == 1)
			cf.action="/apply.cgi?/BAS_mulpppoe_edit.htm timestamp="+ts;
		else
			cf.action="/apply.cgi?/BAS_mulpppoe_edit_ww.htm timestamp="+ts;
		return true;
	}
}

function check_mulpppoe_session1(cf,check)
{
	if(cf.pppoe_username.value=="")
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
	for(i=0;i<cf.pppoe_passwd.value.length;i++)
        {
                if(isValidChar(cf.pppoe_passwd.value.charCodeAt(i))==false)
                {
                        alert("$password_not_allowed");
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
	if(cf.pppoe_idletime.value.length<=0)
	{
		alert("$idle_time_null");
		return false;
	}
	else if(!_isNumeric(cf.pppoe_idletime.value))
	{
		alert("$invalid_idle_time");
		return false;
	}

	cf.pppoe_idletime_value.value = cf.pppoe_idletime.value * 60; // min --> sec

	cf.conflict_wanlan.value=0;	 
	if(cf.WANAssign[1].checked)
	{
		cf.pppoe_ipaddr.value=cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;
		if(checkipaddr(cf.pppoe_ipaddr.value)==false)
		{
			alert("$invalid_ip");
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
	}	

	if(cf.DNSAssign[1].checked)
	{
		cf.pppoe_dnsaddr1.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;
                cf.pppoe_dnsaddr2.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
                if(cf.pppoe_dnsaddr2.value=="...")
                        cf.pppoe_dnsaddr2.value="";

		if(checkipaddr(cf.pppoe_dnsaddr1.value)==false)
		{
			alert("$invalid_primary_dns");
			return false;
		}
		if(cf.pppoe_dnsaddr2.value!="" && cf.pppoe_dnsaddr2.value!="0.0.0.0" )
			if(checkipaddr(cf.pppoe_dnsaddr2.value)==false)
			{
				alert("$invalid_second_dns");
				return false;
			}
	}
	cf.hid_mtu_value.value=wan_mtu_now;
	if( !( old_wan_type=="mulpppoe1"))
	{
        	cf.change_wan_type.value=0;
			mtu_change(wanproto_type);
	}
	else if ( old_pppoe_wan_assign == "1")
	{
        	if( old_wan_ip!=cf.pppoe_ipaddr.value && cf.WANAssign[1].checked)
                	cf.change_wan_type.value=0;
        	else if(cf.WANAssign[0].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;	
	}
	else if( old_pppoe_wan_assign == "0")
	{
        	if( old_wan_ip!=cf.pppoe_ipaddr.value && cf.WANAssign[1].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;
	}
	if (check != 0)
		cf.run_test.value="test"
	else
		cf.run_test.value="no"	
	cf.mul_testnum.value="1"
	return true;
}

function check_mulpppoe_session2(cf,check)
{
	if(cf.pppoe2_username.value=="")
	{
		alert("$login_name_null");
		return false;
	}
	for(i=0;i<cf.pppoe2_username.value.length;i++)
        {
       	        if(isValidChar(cf.pppoe2_username.value.charCodeAt(i))==false)
               	{
                       	alert("$loginname_not_allowed");
                       	return false;
               	}
       	}
	for(i=0;i<cf.pppoe2_password.value.length;i++)
	{
            	if(isValidChar(cf.pppoe2_password.value.charCodeAt(i))==false)
               	{
                       	alert("$password_not_allowed");
                       	return false;
               	}
       	}
       	for(i=0;i<cf.pppoe2_servicename.value.length;i++)
       	{
               	if(isValidChar(cf.pppoe2_servicename.value.charCodeAt(i))==false)
               	{
                       	alert("$servname_not_allowed");
                       	return false;
               	}
       	}

/*	if(cf.pppoe2_idle_time.value.length<=0)
	{
		alert(idle_time_null);
		return false;
	}	
	else if(!_isNumeric(cf.pppoe2_idle_time.value))
	{
		alert(invalid_idle_time);
		return false;
	}
*/
       	if(cf.WANAssign2[1].checked)
       	{
               	cf.pppoe2_ipaddr.value=cf.pppoe2_ip1.value+'.'+cf.pppoe2_ip2.value+'.'+cf.pppoe2_ip3.value+'.'+cf.pppoe2_ip4.value;
               	if(checkipaddr(cf.pppoe2_ipaddr.value)==false)
               	{
                       	alert("$invalid_ip");
                       	return false;
               	}
               	if(isSameSubNet(cf.pppoe2_ipaddr.value,lan_subnet,lan_ip,lan_subnet) == true)
               	{
                       	cf.conflict_wanlan.value=1;
               	}
               	if(isSameIp(cf.pppoe2_ipaddr.value,lan_ip) == true)
               	{
                       	cf.conflict_wanlan.value=1;
               	}
       	}
       	if(cf.DNSAssign2[1].checked)
     	{
               	cf.pppoe2_dnsaddr1.value=cf.pppoe2_dnsp1.value+'.'+cf.pppoe2_dnsp2.value+'.'+cf.pppoe2_dnsp3.value+'.'+cf.pppoe2_dnsp4.value;
               	cf.pppoe2_dnsaddr2.value=cf.pppoe2_dnss1.value+'.'+cf.pppoe2_dnss2.value+'.'+cf.pppoe2_dnss3.value+'.'+cf.pppoe2_dnss4.value;
               	if(cf.pppoe2_dnsaddr2.value=="...")
                       	cf.pppoe2_dnsaddr2.value="";
               	if(checkipaddr(cf.pppoe2_dnsaddr1.value)==false)
               	{
                       	alert("$invalid_primary_dns");
                       	return false;
               	}
               	if(cf.pppoe2_dnsaddr2.value!="" && cf.pppoe2_dnsaddr2.value!="0.0.0.0" )
                       	if(checkipaddr(cf.pppoe2_dnsaddr2.value)==false)
                       	{
                              	alert("$invalid_second_dns");
                               	return false;
                       	}
	}
	if( !( old_wan_type=="mulpppoe1"))
        	cf.change_wan_type.value=0;
	else if ( old_pppoe_wan_assign2 == "1")
	{
        	if( old_wan_ip2!=cf.pppoe2_ipaddr.value && cf.WANAssign2[1].checked)
                	cf.change_wan_type.value=0;
        	else if(cf.WANAssign2[0].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;
	}
	else if( old_pppoe_wan_assign2 == "0")
	{
        	if( old_wan_ip2!=cf.pppoe2_ipaddr.value && cf.WANAssign2[1].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;
	}
	if (check != 0)
		cf.run_test.value="test";
	else
		cf.run_test.value="no";
	cf.mul_testnum.value="2";
	return true;
}

function check_mulpppoe(cf,check)
{	
	//bug 23249, change the input value format
	format_IP("WPethr1", "WPethr2", "WPethr3", "WPethr4", "PDAddr1", "PDAddr2", "PDAddr3", "PDAddr4", "DAddr1", "DAddr2", "DAddr3", "DAddr4", "pppoe2_ip1", "pppoe2_ip2", "pppoe2_ip3", "pppoe2_ip4", "pppoe2_dnsp1", "pppoe2_dnsp2", "pppoe2_dnsp3", "pppoe2_dnsp4", "pppoe2_dnss1", "pppoe2_dnss2", "pppoe2_dnss3", "pppoe2_dnss4");

	if(cf.pppoe_username.value=="")
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
	for(i=0;i<cf.pppoe_passwd.value.length;i++)
        {
                if(isValidChar(cf.pppoe_passwd.value.charCodeAt(i))==false)
                {
                        alert("$password_not_allowed");
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
	if(cf.pppoe_idletime.value.length<=0)
	{
		alert("$idle_time_null");
		return false;
	}
	else if(!_isNumeric(cf.pppoe_idletime.value))
	{
		alert("$invalid_idle_time");
		return false;
	}
	cf.pppoe_idletime_value.value = cf.pppoe_idletime.value * 60; // min --> sec

	cf.conflict_wanlan.value=0;	 
	if(cf.WANAssign[1].checked)
	{
		cf.pppoe_ipaddr.value=cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;
		if(checkipaddr(cf.pppoe_ipaddr.value)==false)
		{
			alert("$invalid_ip");
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
	}	
	

	if(cf.DNSAssign[1].checked)
	{
		cf.pppoe_dnsaddr1.value=cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;
                cf.pppoe_dnsaddr2.value=cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
                if(cf.pppoe_dnsaddr2.value=="...")
                        cf.pppoe_dnsaddr2.value="";

		if(checkipaddr(cf.pppoe_dnsaddr1.value)==false)
		{
			alert("$invalid_primary_dns");
			return false;
		}
		if(cf.pppoe_dnsaddr2.value!="" && cf.pppoe_dnsaddr2.value!="0.0.0.0" )
			if(checkipaddr(cf.pppoe_dnsaddr2.value)==false)
			{
				alert("$invalid_second_dns");
				return false;
			}
	}


	if(cf.enable_session2[1].checked)
	{
		if(cf.pppoe2_username.value=="")
		{
			alert("$login_name_null");
			return false;
		}
		for(i=0;i<cf.pppoe2_username.value.length;i++)
	        {
        	        if(isValidChar(cf.pppoe2_username.value.charCodeAt(i))==false)
                	{
                        	alert("$loginname_not_allowed");
                        	return false;
                	}
        	}
       	 	for(i=0;i<cf.pppoe2_password.value.length;i++)
        	{
                	if(isValidChar(cf.pppoe2_password.value.charCodeAt(i))==false)
                	{
                        	alert("$password_not_allowed");
                        	return false;
                	}
        	}
        	for(i=0;i<cf.pppoe2_servicename.value.length;i++)
        	{
                	if(isValidChar(cf.pppoe2_servicename.value.charCodeAt(i))==false)
                	{
                        	alert("$servname_not_allowed");
                        	return false;
                	}
        	}

/*		if(cf.pppoe2_idle_time.value.length<=0)
		{
			alert(idle_time_null);
			return false;
		}	
		else if(!_isNumeric(cf.pppoe2_idle_time.value))
		{
			alert(invalid_idle_time);
			return false;
		}
*/
          	if(cf.WANAssign2[1].checked)
          	{
                  	cf.pppoe2_ipaddr.value=cf.pppoe2_ip1.value+'.'+cf.pppoe2_ip2.value+'.'+cf.pppoe2_ip3.value+'.'+cf.pppoe2_ip4.value;
                  	if(checkipaddr(cf.pppoe2_ipaddr.value)==false)
                  	{
                          	alert("$invalid_ip");
                          	return false;
                  	}
                  	if(isSameSubNet(cf.pppoe2_ipaddr.value,lan_subnet,lan_ip,lan_subnet) == true)
                  	{
                          	cf.conflict_wanlan.value=1;
                  	}
                  	if(isSameIp(cf.pppoe2_ipaddr.value,lan_ip) == true)
                  	{
                          	cf.conflict_wanlan.value=1;
                  	}
          	}
          	if(cf.DNSAssign2[1].checked)
          	{
                  	cf.pppoe2_dnsaddr1.value=cf.pppoe2_dnsp1.value+'.'+cf.pppoe2_dnsp2.value+'.'+cf.pppoe2_dnsp3.value+'.'+cf.pppoe2_dnsp4.value;
                  	cf.pppoe2_dnsaddr2.value=cf.pppoe2_dnss1.value+'.'+cf.pppoe2_dnss2.value+'.'+cf.pppoe2_dnss3.value+'.'+cf.pppoe2_dnss4.value;
                  	if(cf.pppoe2_dnsaddr2.value=="...")
                          	cf.pppoe2_dnsaddr2.value="";

                  	if(checkipaddr(cf.pppoe2_dnsaddr1.value)==false)
                  	{
                          	alert("$invalid_primary_dns");
                          	return false;
                  	}
                  	if(cf.pppoe2_dnsaddr2.value!="" && cf.pppoe2_dnsaddr2.value!="0.0.0.0" )
                          	if(checkipaddr(cf.pppoe2_dnsaddr2.value)==false)
                          	{
                                  	alert("$invalid_second_dns");
                                  	return false;
                          	}
          	}


	}
	if( !( old_wan_type=="mulpppoe1"))
        	cf.change_wan_type.value=0;
	else if ( old_pppoe_wan_assign == "1")
	{
        	if( old_wan_ip!=cf.pppoe_ipaddr.value && cf.WANAssign[1].checked)
                	cf.change_wan_type.value=0;
        	else if(cf.WANAssign[0].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;
	}
	else if( old_pppoe_wan_assign == "0")
	{
        	if( old_wan_ip!=cf.pppoe_ipaddr.value && cf.WANAssign[1].checked)
                	cf.change_wan_type.value=0;
        	else
                	cf.change_wan_type.value=1;
	}
	if (check != 0)
	{	
		cf.run_test.value="test"
		cf.mul_testnum.value="0";
	}
	else
	{
		cf.run_test.value="no"	
		cf.mul_testnum.value="0";
	}
	if( cf.mulpppoe_demand.value == "1" || cf.mulpppoe_demand.value == "2" )
	{
		if( readycloud_enable == "1" || vpn_enable == "1" || upnp_enableMedia == "1" || parent.geniecloud_flag == "1")
		{
			if(confirm("$ppp_dial_on_demand_query") == false)
					return false;
		}
	}
	return true;
}

function set_gray(check)
{
	cf=document.forms[0];
	if(check == 0)
	{
		cf.pppoe2_username.disabled = true;
		cf.pppoe2_password.disabled = true;
		cf.pppoe2_servicename.disabled = true;
	}
	else
	{
		cf.pppoe2_username.disabled = false;
		cf.pppoe2_password.disabled = false;
		cf.pppoe2_servicename.disabled = false;	
	}
}

function change_session()
{
	cf=document.forms[0];
	if( cf.pppoe2_session[0].selected == true )
	{
		getObj("view_session").innerHTML=westHTML;
		cf.pppoe2_username.value=west_username;
        	cf.pppoe2_password.value=west_password;
		cf.pppoe2_servicename.value=west_servicename;
		
	}
	else if( document.forms[0].pppoe2_session[1].selected == true )
	{
		getObj("view_session").innerHTML=eastHTML;
		cf.pppoe2_username.value=east_username;
        	cf.pppoe2_password.value=east_password;
		cf.pppoe2_servicename.value=east_servicename;

	}
	else if( document.forms[0].pppoe2_session[2].selected == true )
	{
		getObj("view_session").innerHTML=otherHTML;
		cf.pppoe2_username.value=other_username;
        	cf.pppoe2_password.value=other_password;
		cf.pppoe2_servicename.value=other_servicename;
		cf.pppoe2_policy.value=pppoe2_policy;

	}
	
}


function check_mulpppoe_del(cf,flag)
{
	if( array_num == "" )
	{
		alert("$port_del");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.select_mulpppoe.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.select_mulpppoe[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$select_serv_del");
		return false;
	}
	else
	{
		cf.select_del.value=select_num;
		cf.submit_flag.value="mulpppoe_del";
		if(flag == 1)
			cf.action="/apply.cgi?/BAS_mulpppoe.htm timestamp="+ts;
		else
			cf.action="/apply.cgi?/BAS_mulpppoe_ww.htm timestamp="+ts;
		return true;
	}
}
