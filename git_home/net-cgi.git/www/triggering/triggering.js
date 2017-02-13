var invalid_port_used="$invalid_port_used";

function checkPort(name, port)
{
	var msg = "";
	var portNum = parseInt(port,10);

	if (port.length == 0)
	{
		msg = name + " " + "$trigger_null";
		alert(msg);
		return false;
	}
	for(i=0;i<port.length;i++)
	{
		c=port.charAt(i);
		if("0123456789".indexOf(c,0)<0)
		{
			msg = "$trigger_invalid" + name;
			alert(msg);			
			return false;
		}	
	}
	if (isNaN(portNum) || portNum < 1 || portNum > 65535)
	{
		msg = "$trigger_invalid" + name + "$trigger_1_65535";
		alert(msg);
		return false;
	}
	return true;
}

function setsrc(cf)
{
	var type = cf.src_ip_type.selectedIndex;
	if (type == 1)
	{	
		cf.src_ip1.disabled = false;
		cf.src_ip2.disabled = false;
		cf.src_ip3.disabled = false;
		cf.src_ip4.disabled = false;
	}
	else
	{
		cf.src_ip1.disabled = true;
		cf.src_ip2.disabled = true;
		cf.src_ip3.disabled = true;
		cf.src_ip4.disabled = true;
	}
}

function check_triggering_add(cf,flag)
{
	var all_port_num=0;
	if(flag== 'add')
		for(i=1;i<=trigger_array_num;i++)
		{
			var str = eval ( 'triggeringArray' + i );
			var each_info=str.split(' ');
			constart_port=each_info[6];	
			conend_port=each_info[7];		
			all_port_num=all_port_num+parseInt(conend_port)-parseInt(constart_port)+1;
		}
	else if(flag == 'edit')
		for(i=1;i<=trigger_array_num;i++)
		{
			if(select_editnum!=i)
			{
				var str = eval ( 'triggeringArray' + i );
				var each_info=str.split(' ');
				constart_port=each_info[6];	
				conend_port=each_info[7];		
				all_port_num=all_port_num+parseInt(conend_port)-parseInt(constart_port)+1;
			}
		}

	if(trigger_array_num==20 && flag== 'add')
	{
		alert("$trigger_length_20");
		return false;
	}
	if(cf.pt_name.value == "" )
	{
		alert("$trigger_ser_name_null");
		return false;
	}
	for(i=0;i<cf.pt_name.value.length;i++)
	{
		if(isValidChar_space(cf.pt_name.value.charCodeAt(i))==false)
		{
			alert("$servname_not_allowed");
			return false;
		}
	}

	for(i=1;i<=trigger_array_num;i++)
	{
		var str = eval ( 'triggeringArray' + i );
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&harr;/g, " ").replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		if(flag == 'edit')
		{
			if( cf.pt_name.value == each_info[0] && select_editnum!=i )
			{
				alert("$trigger_ser_name_dup");
				return false;
			}
		}
		else
		{
			if( cf.pt_name.value == each_info[0])
			{
				alert("$trigger_ser_name_dup");
				return false;
			}
		}
	}
	cf.pt_name.value=cf.pt_name.value.replace(/&/g, "&#38;").replace(/ /g, "&harr;");

	var type = cf.src_ip_type.selectedIndex;
	if (type == 0)
		cf.service_ip.value="any";
	else
	{
		cf.service_ip.value=cf.src_ip1.value+'.'+cf.src_ip2.value+'.'+cf.src_ip3.value+'.'+cf.src_ip4.value;
		if(checkipaddr(cf.service_ip.value)==false || is_sub_or_broad(cf.service_ip.value, lan_ip, lan_subnet) == false)
		{
			alert("$invalid_ip");
			return false;
		}
		cf.service_ip.value = address_parseInt(cf.service_ip.value);
		if(isSameSubNet(cf.service_ip.value,lan_subnet,lan_ip,lan_subnet)== false)
		{
			alert("$diff_lan_this_subnet");
			return false;
		}
		if(isSameIp(cf.service_ip.value,lan_ip) == true)
		{
			alert("$invalid_ip");
			return false;
		} 
	}
	if(checkPort("$trigger_port", cf.pt_port.value)==false)
		return false;
	cf.pt_port.value=parseInt(cf.pt_port.value,10);
/*	if(cf.pt_port.value == "21" )
	{
		alert("$trigger_port_invalid_21");
		return false;
	} */
        
	if(checkPort("$block_ser_start_port", cf.in_port_start.value)==false)
		return false;
	cf.in_port_start.value=parseInt(cf.in_port_start.value,10);

	if (cf.in_port_end.value.length > 0)
	{
		if(checkPort("$block_ser_end_port", cf.in_port_end.value)==false)
			return false;
		cf.in_port_end.value=parseInt(cf.in_port_end.value,10);
		if (parseInt(cf.in_port_end.value) < parseInt(cf.in_port_start.value))
		{//bug 23479:use parseInt to parsing string
			alert("$trigger_invalid_port_rang");
			return false;
		}
	}
	if (cf.in_port_end.value.length == 0)
		cf.in_port_end.value = cf.in_port_start.value;
	var now_port_num=cf.in_port_end.value-cf.in_port_start.value+1;
	all_port_num=all_port_num+now_port_num;
/*	if( all_port_num > 256)
	{
		alert("$trigger_port_less256");
		return false;
	} */
	var input_start_port=cf.in_port_start.value;
	var input_end_port=cf.in_port_end.value;
	var input_sertype=cf.in_port_type.value;

	//port_forwarding
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		sertype=each_info[1];
		startport=each_info[2];
		endport=each_info[3];
		serip=each_info[6];
		serflag=each_info[7];
		if(sertype=="UDP"&&serflag=="1")
		{
		}
		else if (!(parseInt(endport)<parseInt(input_start_port)||parseInt(input_end_port)<parseInt(startport)))
		{//bug 23479:same port and same ip, not need to pop warning message;same port and diff ip, need to pop warning message
			if (( sertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == sertype ) && ( cf.service_ip.value == "any" || cf.service_ip.value != serip ))
			{
				alert(invalid_port_used);
				return false;
			}
		}	
	}	

	//port_triggering	
	for(i=1;i<=trigger_array_num;i++)
	{
		var str = eval ( 'triggeringArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		//fixed bug 29931
		trigip=each_info[2];
		consertype=each_info[5];
		constart_port=each_info[6];	
		conend_port=each_info[7];	
		if (flag == 'edit')
		{
			if ((!(parseInt(conend_port)<parseInt(input_start_port)||parseInt(input_end_port)<parseInt(constart_port) ))&& select_editnum!=i)
			{
				if ( (consertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == consertype) && select_editnum != i && cf.service_ip.value != "any" && trigip != "any" && cf.service_ip.value != trigip )
				{
					alert(invalid_port_used);
					return false;
				}
			}
		}
		else
		{
			if (!(parseInt(conend_port)<parseInt(input_start_port)||parseInt(input_end_port)<parseInt(constart_port) ))
			{
				if (( consertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == consertype ) && cf.service_ip.value != "any" && trigip != "any" && cf.service_ip.value != trigip )
				{
					alert(invalid_port_used);
					return false;
				}
			}
		}
	}

	//upnp	
	for(i=1;i<=upnp_array_num;i++)
	{
		var str = eval ( 'upnpArray' + i );
		var each_info=str.split(';');
		upnp_protocal=each_info[1];
		upnp_int=each_info[2];
		upnp_ext=each_info[3];
		upnp_ip=each_info[4];
		if (!( parseInt(upnp_ext)<parseInt(input_start_port) || parseInt(input_end_port)<parseInt(upnp_int) ) && cf.service_ip.value != upnp_ip)
		{
			if ( upnp_protocal == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == upnp_protocal )
			{
				alert(invalid_port_used);
				return false;
			}
		}
	}		

	//check USB ready share port, only use TCP
	if( usb_router_flag == 1)
	{
        if(input_sertype != "UDP")
        {
                if(check_readyshare_port(input_start_port, input_end_port, "WAN") == false)
                {
                        alert(invalid_port_used);
                        return false;
                }
        }
	}

	//check with remote, only use TCP
	if(endis_remote=="1" && input_sertype != "UDP" ) //15873: remote only use TCP
		if (!(parseInt(remote_port)<parseInt(input_start_port)||parseInt(input_end_port)<parseInt(remote_port)) && remote_port!="")
		{
			alert(invalid_port_used);
			return false;
		}		
	//enable ntp, only use UDP
	if(parseInt(input_start_port)<=123 && parseInt(input_end_port)>=123 && endis_ntp == "1" && input_sertype != "TCP")
	{
		alert(invalid_port_used);
		return false;
	}
	//enable upnp, only use UDP
	if(parseInt(input_start_port)<=1900 && parseInt(input_end_port)>=1900 && endis_upnp == "1" && input_sertype != "TCP")
	{
		alert(invalid_port_used);
		return false;
	}
	//enable bigpond   15873: bigpond only use UDP
	if(parseInt(input_start_port)<=5050 && parseInt(input_end_port)>=5050 && info_get_wanproto == "bigpond" && input_sertype != "TCP")
	{
		alert(invalid_port_used);
		return false;
	}
        cf.pt_port.value=port_range_interception(cf.pt_port.value);
        cf.in_port_start.value=port_range_interception(cf.in_port_start.value);
        cf.in_port_end.value=port_range_interception(cf.in_port_end.value);

	cf.submit();
	return true;
}

function check_triggering_edit(cf)
{
	if(array_num == 0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if(array_num == 1)
	{
		if(cf.serviceSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.serviceSelect[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="triggering_editnum";
		cf.action="/apply.cgi?/triggering_edit.htm timestamp="+ts;
		cf.submit();
		return true;
	}
}

function check_triggering_del(cf)
{
	if( array_num == "" )
	{
		location.href="del_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.serviceSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.serviceSelect[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		location.href="del_fail.htm";
		return false;
	}
	else
	{
		cf.select_del.value=select_num;
		cf.submit_flag.value="triggering_del";
		cf.submit();
		return true;
	}
}

function check_triggering_apply(cf)
{
	if(cf.fwpt_enable.checked == true)
		cf.disable_trigger_on.value=1;
	else
		cf.disable_trigger_on.value=0;
	if(cf.fwpt_timeout.value=="")
	{
		alert("$timeout_null");
		return false;
	}	
        else if(!_isNumeric(cf.fwpt_timeout.value))
	{
		alert("$invalid_trigger_timeout");
		return false;
	}
	cf.fwpt_timeout.value = parseInt(cf.fwpt_timeout.value, 10);
	if(array_num == 0)
	{}
	else if ( array_num == 1)
	{
		if(cf.enable_triggering.checked == true)
			cf.endis_trigger_value.value="1";
		else
			cf.endis_trigger_value.value="0";
	}
	else for(i=0;i<array_num;i++)
	{
		if(cf.enable_triggering[i].checked == true)
			cf.endis_trigger_value.value=cf.endis_trigger_value.value+"1,";
		else
			cf.endis_trigger_value.value=cf.endis_trigger_value.value+"0,";
	}
	cf.submit();
	return true;
}
