function clearData1()
{
	var cf = document.forms[0];

	cf.local_ip_1.disabled = true;
	cf.local_ip_2.disabled = true;
	cf.local_ip_3.disabled = true;
	cf.local_ip_4.disabled = true;

	cf.start_ip_1.disabled = true;
	cf.start_ip_2.disabled = true;
	cf.start_ip_3.disabled = true;
	cf.start_ip_4.disabled = true;
	cf.fin_ip_1.disabled = true;
	cf.fin_ip_2.disabled = true;
	cf.fin_ip_3.disabled = true;
	cf.fin_ip_4.disabled = true;
}
function check_remote()
{
	var cf = document.forms[0];
	if(cf.remote_mg_enable.checked == true)
		cf.http_rmenable.value = 1;
	else
		cf.http_rmenable.value = 0;
	cf.local_ip.value=cf.local_ip_1.value+'.'+cf.local_ip_2.value+'.'+cf.local_ip_3.value+'.'+cf.local_ip_4.value;
	warningMessage="$include_wan_ip";
	if(cf.rm_access[0].checked) 
	{
		if (checkipaddr(cf.local_ip.value)==false)
		{
			alert("$invalid_ip");
			return false;
		}
		cf.local_ip.value = address_parseInt(cf.local_ip.value);
		if(isSameSubNet(cf.local_ip.value,lan_subnet,lan_ip,lan_subnet) == true)
		{
			alert("$diff_thisip_lanip");
			return false;
		}
		if (isSameIp(cf.local_ip.value,remote_manage_ip)==true)
		{
			alert(warningMessage);
			return false;
		}		
	}
	else if(cf.rm_access[1].checked)
	{
		var start_ip=cf.start_ip_1.value+'.'+cf.start_ip_2.value+'.'+cf.start_ip_3.value+'.'+cf.start_ip_4.value;
		var end_ip=cf.fin_ip_1.value+'.'+cf.fin_ip_2.value+'.'+cf.fin_ip_3.value+'.'+cf.fin_ip_4.value;

		if (checkipaddr(start_ip)==false)
		{
			alert("$invalid_start_ip");
			return false;
		}
		if (checkipaddr(end_ip)==false)
		{
			alert("$invalid_end_ip");
			return false;
		}
		if(cp_ip2(start_ip,end_ip)==false)
		{
			alert("$invalid_remote_startendip");
			return false;
		}
		if(isSameSubNet(start_ip,lan_subnet,lan_ip,lan_subnet) == true)
		{
			alert("$invalid_ip");
			return false;
		}
		if(isSameSubNet(end_ip,lan_subnet,lan_ip,lan_subnet) == true)
		{
			alert("$invalid_ip");
			return false;
		}
		startipNum=ipNum(start_ip);
		endipNum=ipNum(end_ip);
		wanipNum=ipNum(remote_manage_ip);
		lanipNum=ipNum(lan_ip);
		if(lanipNum >= startipNum && lanipNum <= endipNum )
		{
			alert("$invalid_ip");
			return false;
		}
		if(wanipNum >= startipNum && wanipNum <= endipNum )
		{
			alert(warningMessage);
			return false;
		}
		cf.local_ip.value=address_parseInt(start_ip)+'-'+address_parseInt(end_ip);
	}
	var remote_port=parseInt(cf.http_rmport.value,10);
	/*if (isNaN(parseInt(cf.http_rmport.value,10)) ||
		parseInt(cf.http_rmport.value) < 1024 ||
		parseInt(cf.http_rmport.value) > 65534 ||
		cf.http_rmport.value.indexOf(".") != -1)*/
	if(isNaN(remote_port) || remote_port < 1024 || remote_port >65534 || cf.http_rmport.value.indexOf(".") != -1)
	{
		alert("$invalid_port_number");
		cf.http_rmport.focus();
		return false;
	}
	cf.http_rmport.value=remote_port;

	//port_forwarding
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i );
		var each_info=str.split(' ');
		sertype=each_info[1];
		startport=each_info[2];
		endport=each_info[3];
		serflag=each_info[7];
		if(sertype=="UDP" || serflag=="1") //15873:  only use TCP protocol, so no need check UDP
		{
		}
		else if(remote_port>=parseInt(startport)&& remote_port<=parseInt(endport) )
		{
			alert("$invalid_remote_port_used");
			return false;
		}
	}	
	//port_triggering	
	for(i=1;i<=trigger_array_num;i++)
	{
		var str = eval ( 'triggeringArray' + i );
		var each_info=str.split(' ');
		constart_port=each_info[6];	
		conend_port=each_info[7];	
		tri_type=each_info[5];
		if( tri_type == "UDP" ) //15873: only use TCP protocol, so no need check UDP
		{
		}
		else if(remote_port>=parseInt(constart_port)&& remote_port<=parseInt(conend_port) )
		{
			alert("$used_by_system");
			return false;
		}
	}
	//upnp	
	for(i=1;i<=upnp_array_num;i++)
	{
		var str = eval ( 'upnpArray' + i );
		var each_info=str.split(';');
		upnp_ext=each_info[3];
		upnp_type=each_info[1];
	
		if( upnp_type == "UDP" )//15873: only use TCP protocol, so no need check UDP
		{
		}
		if(remote_port==parseInt(upnp_ext))
		{
			alert("$invalid_remote_port_used");
			return false;
		}
	}

	//check USB ready share port
	if( usb_router_flag == 1)
	{
		if(check_readyshare_port(remote_port, remote_port, "WAN") == false)
		{
			alert("$invalid_remote_port_used");
			return false;
		}
	}
	cf.http_rmport.value=port_range_interception(cf.http_rmport.value);

	cf.submit();
	return true;
}
function clearData2()
{
	var cf=document.forms[0];

	cf.start_ip_1.disabled = true;
	cf.start_ip_2.disabled = true;
	cf.start_ip_3.disabled = true;
	cf.start_ip_4.disabled = true;
	cf.fin_ip_1.disabled = true;
	cf.fin_ip_2.disabled = true;
	cf.fin_ip_3.disabled = true;
	cf.fin_ip_4.disabled = true;
	cf.local_ip_1.disabled = false;
	cf.local_ip_2.disabled = false;
	cf.local_ip_3.disabled = false;
	cf.local_ip_4.disabled = false;
}

function clearData3()
{
	var cf = document.forms[0];

	cf.start_ip_1.disabled = false;
	cf.start_ip_2.disabled = false;
	cf.start_ip_3.disabled = false;
	cf.start_ip_4.disabled = false;
	cf.fin_ip_1.disabled = false;
	cf.fin_ip_2.disabled = false;
	cf.fin_ip_3.disabled = false;
	cf.fin_ip_4.disabled = false;
	cf.local_ip_1.disabled = true;
	cf.local_ip_2.disabled = true;
	cf.local_ip_3.disabled = true;
	cf.local_ip_4.disabled = true;


}
