var invalid_port_used="$invalid_port_used";
//var isIE = document.all;
var disable = false;
var serv_array=[["TCP","20","21","20","21","FTP","1"],["TCP","80","80","80","80","HTTP","1"],["TCP","23566","23566","23566","23566","ICUII","1"],["TCP","6670","6670","6670","6670","IP_Phone","1"],["TCP","1720","1720","1720","1720","NetMeeting","1"],["TCP","119","119","119","119","News","1"],["TCP","1723","1723","1723","1723","PPTP","1"],["TCP/UDP","27960","27960","27960","27960","QuakeII/III","1"],["TCP/UDP","6970","7170","6970","7170","Real-Audio","1"],["TCP","23","23","23","23","Telnet","1"]];
 //fix bug 21344 change forwarding page
function show_servip()
{
	var cf=document.forms[0];
	var lanip_array=new Array();
	lanip_array=lan_ip.split('.');
	cf.SV_IP1.value=lanip_array[0];
	cf.SV_IP2.value=lanip_array[1];
	cf.SV_IP3.value=lanip_array[2];
}

function check_list_length(forward_num)
{
	var flag1720=0;
	for(i=1;i<=forward_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		serflag=each_info[7];
		if(serflag==1)
			flag1720=1;
	}
	if(flag1720 == 0)
	{	
		if(forward_num==64)
		{
			alert("$forward_length_64");
			return false;
		}
	}
	else
	{
		if(forward_num==65)
		{
			alert("$forward_length_64");
			return false;
		}
	}
}

function check_port(cf)
{
	// check port	
	if(cf.port_start.value==""||cf.port_end.value==""||cf.internal_port_start.value==""||cf.internal_port_end.value=="")
	{
		alert("$invalid_port");
		return false;
	}
	txt=parseInt(cf.port_start.value,10);
	if( isNaN(txt) )
	{
		alert("$invalid_start_port");
		return false;
	}
	cf.port_start.value=txt;
	txt=parseInt(cf.port_end.value,10);
	if(isNaN(txt))
	{
		alert("$invalid_end_port");
		return false;
	}
	cf.port_end.value=txt;
	txt=parseInt(cf.internal_port_start.value,10);
	if( isNaN(txt) )
	{
		alert("$invalid_start_port");
		return false;
	}
	cf.internal_port_start.value=txt;
	txt=parseInt(cf.internal_port_end.value,10);
	if(isNaN(txt))
	{
		alert("$invalid_end_port");
		return false;
	}
	cf.internal_port_end.value=txt;
	if(parseInt(cf.port_start.value)<1||parseInt(cf.port_start.value)>65534||parseInt(cf.internal_port_start.value)<1||parseInt(cf.internal_port_start.value)>65534)
	{
		alert("$invalid_start_port");
		return false;
	}
	if(parseInt(cf.port_end.value)<1||parseInt(cf.port_end.value)>65534||parseInt(cf.internal_port_end.value)<1||parseInt(cf.internal_port_end.value)>65534)
	{
		alert("$invalid_end_port");
		return false;
	}
	if(parseInt(cf.port_end.value)<parseInt(cf.port_start.value) || parseInt(cf.internal_port_end.value)<parseInt(cf.internal_port_start.value))
	{
		alert("$end_port_greater");
		return false;
	}
	cf.port_start.value=port_range_interception(cf.port_start.value);
	cf.port_end.value=port_range_interception(cf.port_end.value);
	cf.internal_port_start.value=port_range_interception(cf.internal_port_start.value);
	cf.internal_port_end.value=port_range_interception(cf.internal_port_end.value);
	return true;
}

function check_forwarding_add(cf,flag)
{
	//to fix bug29779
	/*cf.internal_port_end.value = parseInt(cf.internal_port_start.value)+(parseInt(cf.port_end.value)-parseInt(cf.port_start.value));*/
	int_port_value();
	if(cf.same_range.checked == true){
		cf.hidden_port_range.value ="1";
		cf.hidden_port_int_start.value = port_range_interception(cf.port_start.value);
		cf.hidden_port_int_end.value = port_range_interception(cf.port_end.value);
	}
	else
	{
		cf.hidden_port_range.value ="0";
		cf.hidden_port_int_start.value = port_range_interception(cf.internal_port_start.value);
		cf.hidden_port_int_end.value = port_range_interception(cf.internal_port_end.value);
	}
	cf.serflag.value=0;
	if (flag != "edit" && check_list_length(forward_array_num) == false)
		return false;
	txt=cf.portname.value;
	if(txt=="")
	{
		alert("$invalid_ser_name");
		return false;
	}
	for(i=0;i<cf.portname.value.length;i++)
	{
		if(isValidChar_space(cf.portname.value.charCodeAt(i))==false)
		{
			alert("$invalid_ser_name");
			return false;
		}
	}
	cf.portname.value=cf.portname.value.replace(/ /g, "&harr;");	
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&harr;/g, " ");
		if(flag == 'edit')
		{
			if(each_info[0]==cf.portname.value && select_editnum!=i && each_info[7]==0)
			{
				alert("$forwarding_ser_name_dup");
				return false;
			}
		}
		else
		{
			if(each_info[0]==cf.portname.value)
			{
				alert("$forwarding_ser_name_dup");
				return false;
			}
		}
	}
	if(check_port(cf)==false)
		return false;
	cf.service_ip.value=cf.server_ip1.value+'.'+cf.server_ip2.value+'.'+cf.server_ip3.value+'.'+cf.server_ip4.value;		
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
	var input_ext_start_port=cf.port_start.value;
	var input_ext_end_port=cf.port_end.value;
	var input_int_start_port=cf.internal_port_start.value;
	var input_int_end_port=cf.internal_port_end.value;
	var input_sertype=cf.srvtype.value;
	var input_ip=cf.service_ip.value;
	//port_forwarding
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		sertype=each_info[1];
		ext_startport=each_info[2];
		ext_endport=each_info[3];
		int_startport=each_info[4];
		int_endport=each_info[5];
		//fixed bug 29931
		forwardingip=each_info[6];
		serflag=each_info[7];
		if(sertype=="UDP"&&serflag=="1") 
		{
		}
		else if (sertype=="TCP/UDP"||sertype==input_sertype||input_sertype=="TCP/UDP")
		{
			if(flag == 'edit')
			{
				if (((!(parseInt(ext_endport)<parseInt(input_ext_start_port)||parseInt(input_ext_end_port)<parseInt(ext_startport))) && select_editnum!=i))
 				{
					if ( (sertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == sertype) && select_editnum!=i
					&& ( input_ip != forwardingip 
						|| parseInt(int_startport) != (parseInt(input_int_start_port)+parseInt(ext_startport)-parseInt(input_ext_start_port))))
					{
						alert(invalid_port_used);
						return false;
					}
				}	
			}
			else
				if (!(parseInt(ext_endport)<parseInt(input_ext_start_port)||parseInt(input_ext_end_port)<parseInt(ext_startport)))
 				{
					if ( (sertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == sertype)
					&& ( input_ip != forwardingip 
						|| parseInt(int_startport) != (parseInt(input_int_start_port)+parseInt(ext_startport)-parseInt(input_ext_start_port))))
					{
						alert(invalid_port_used);
						return false;
					}
				}	
		}
	}	

	//port_triggering	
	for(i=1;i<=trigger_array_num;i++)
	{
		var str = eval ( 'triggeringArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		triggeringip=each_info[2];
		consertype=each_info[5];
		constart_port=each_info[6];	
		conend_port=each_info[7];	
		if (!(parseInt(conend_port)<parseInt(input_ext_start_port)||parseInt(input_ext_end_port)<parseInt(constart_port) ) || !(parseInt(conend_port)<parseInt(input_int_start_port)||parseInt(input_int_end_port)<parseInt(constart_port) ))
		{//to fix bug33787same port and same ip, not need to pop warning message;same port and diff ip, need to pop warning message
			if ( (consertype == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == consertype) && (triggeringip == "any" || input_ip != triggeringip ))
			{
				alert(invalid_port_used);
				return false;
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

		if( ( parseInt(upnp_ext) >=parseInt(input_ext_start_port) && parseInt(upnp_ext) <=parseInt(input_ext_end_port)) || ( parseInt(upnp_ext)>=parseInt(input_int_start_port) && parseInt(upnp_ext)<=parseInt(input_int_end_port) ))
		{
			if ( (upnp_protocal == "TCP/UDP" || input_sertype == "TCP/UDP" || input_sertype == upnp_protocal) && input_ip != upnp_ip )
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
			if(check_readyshare_port(input_ext_start_port, input_ext_end_port, "WAN") == false || check_readyshare_port(input_int_start_port, input_int_end_port, "WAN") == false)
			{	
				alert(invalid_port_used);
				return false;
			}
		}
	}

	//check with remote, only use TCP
	if(endis_remote=="1" && cf.srvtype.value != "UDP" ) //15873: remote only use TCP 
	{
		if ((!(parseInt(remote_port)<parseInt(input_ext_start_port)||parseInt(input_ext_end_port)<parseInt(remote_port)) && remote_port!="") || (!(parseInt(remote_port)<parseInt(input_int_start_port)||parseInt(input_int_end_port)<parseInt(remote_port)) && remote_port!=""))
 		{
			alert(invalid_port_used);
			return false;
		}		
	}
	//enable ntp, only use UDP
	if( (parseInt(input_ext_start_port)<=123 && parseInt(input_ext_end_port)>=123 && endis_ntp == "1" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port)<=123 && parseInt(input_int_end_port)>=123 && endis_ntp == "1" && cf.srvtype.value != "TCP"))
	{
		alert(invalid_port_used);
		return false;
	}
	//enable upnp, only use UDP
	if((parseInt(input_ext_start_port)<=1900 && parseInt(input_ext_end_port)>=1900 && endis_upnp == "1" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port)<=1900 && parseInt(input_int_end_port)>=1900 && endis_upnp == "1" && cf.srvtype.value != "TCP"))
	{
		alert(invalid_port_used);
		return false;
	}
	//enable bigpond   15873: bigpond only use UDP
	if( (parseInt(input_ext_start_port)<=5050 && parseInt(input_ext_end_port)>=5050 && info_get_wanproto == "bigpond" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port)<=5050 && parseInt(input_int_end_port)>=5050 && info_get_wanproto == "bigpond" && cf.srvtype.value != "TCP"))
 	{
		alert(invalid_port_used);
		return false;
	}

	if (cf.srvtype.value=="TCP"||cf.srvtype.value=="TCP/UDP")
        {
				if ((parseInt(cf.port_end.value)>="1720"&&parseInt(cf.port_start.value)<="1720") || (parseInt(cf.internal_port_end.value)>="1720"&&parseInt(cf.internal_port_start.value)<="1720"))
                        cf.serflag.value=1;
                else
                        cf.serflag.value=0;
        }
	cf.submit();
	return true;
}

function check_forwarding_edit(cf)
{
	if (forward_num == 0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if (forward_num == 1)
	{
		if(cf.RouteSelect.checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect.value)+1;
		}
	}
	else for(i=0;i<forward_num;i++)
		if(cf.RouteSelect[i].checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect[i].value)+1;
		}
	if(count_select==0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="forwarding_editnum";
		cf.action="/apply.cgi?/forwarding_edit_wait.htm timestamp="+ts;
		cf.submit();
		return true;
	}
}

function check_forwarding_del(cf)
{
	if (forward_num == 0 )
	{
		location.href="del_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if (forward_num == 1)
	{
		if(cf.RouteSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<forward_num;i++)
		if(cf.RouteSelect[i].checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect[i].value)+1;
		}
	if(count_select==0)
	{
		location.href="del_fail.htm";
		return false;
	}
	else
	{
		cf.select_del.value=select_num;
		cf.submit_flag.value="forwarding_del";
		cf.submit();
		return true;
	}
}


function Check_add(cf)     
{
	cf.serflag.value=0;
	if(check_list_length(forward_array_num)==false)
		return false;
	cf.service_ip.value=cf.SV_IP1.value+'.'+cf.SV_IP2.value+'.'+cf.SV_IP3.value+'.'+cf.SV_IP4.value;
	if(checkipaddr(cf.service_ip.value)==false || is_sub_or_broad(cf.service_ip.value, lan_ip, lan_subnet) ==false)
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
	var selectService=cf.svs_gm.options[cf.svs_gm.selectedIndex].value;
	var s = cf.svs_gm.selectedIndex;
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&harr;/g, " ");
		if(each_info[0]==serv_array[s][5])
		{
			alert("$forwarding_ser_name_dup");
			return false;
		}
	}
	var input_ip=cf.service_ip.value;
	cf.hidden_protocol.value = serv_array[s][0];
	cf.hidden_external_portstart.value = serv_array[s][1];
	cf.hidden_external_portend.value = serv_array[s][2];
	cf.hidden_internal_portstart.value = serv_array[s][3];
	cf.hidden_internal_portend.value = serv_array[s][4];
	cf.hidden_service_name.value = serv_array[s][5];
	var input_sername=serv_array[s][5];
	var input_sertype=serv_array[s][0];
	var input_external_start_port=serv_array[s][1];
	var input_external_end_port=serv_array[s][2];
	var input_internal_start_port=serv_array[s][3];
	var input_internal_end_port=serv_array[s][4];
	//port_forwarding
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		sertype=each_info[1];
		ext_startport=each_info[2];
		ext_endport=each_info[3];
		int_startport=each_info[4];
		int_endport=each_info[5];
		forwardingip=each_info[6];
		serflag=each_info[7];
		if(sertype=="UDP"&&serflag=="1")
		{
		}
		else if (sertype=="TCP/UDP"||sertype==input_sertype||input_sertype=="TCP/UDP")
		{
			if (!(parseInt(ext_endport)<parseInt(input_external_start_port)||parseInt(input_external_end_port)<parseInt(ext_startport)) && forwardingip != input_ip)
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
		constart_port=each_info[6];	
		conend_port=each_info[7];	
		if (!(parseInt(conend_port)<parseInt(input_external_start_port)||parseInt(input_external_end_port)<parseInt(constart_port) ) || !(parseInt(conend_port)<parseInt(input_internal_start_port)||parseInt(input_internal_end_port)<parseInt(constart_port) ))
 		{
			alert(invalid_port_used);
			return false;
		}
	}
	//upnp	
	for(i=1;i<=upnp_array_num;i++)
	{
		var str = eval ( 'upnpArray' + i );
		var each_info=str.split(';');
		upnp_int=each_info[2];
		upnp_ext=each_info[3];
		upnp_ip=each_info[4];
		if((parseInt(upnp_ext)>=parseInt(input_external_start_port) && parseInt(upnp_ext)<=parseInt(input_external_end_port) && input_ip != upnp_ip) || (parseInt(upnp_ext)>=parseInt(input_internal_start_port) && parseInt(upnp_ext)<=parseInt(input_internal_end_port) && input_ip != upnp_ip))
		{
			alert(invalid_port_used);
			return false;
		}
	}		
	
	//check USB ready share port
	if( usb_router_flag == 1)
	{
		if(check_readyshare_port(input_external_start_port, input_external_end_port, "WAN") == false || check_readyshare_port(input_internal_start_port, input_internal_end_port, "WAN") == false)
		{	
			alert(invalid_port_used);
			return false;
		}
	}

	//check with remote
	if ((!(parseInt(remote_port)<parseInt(input_external_start_port)||parseInt(input_external_end_port)<parseInt(remote_port)) && remote_port!="") || (!(parseInt(remote_port)<parseInt(input_internal_start_port)||parseInt(input_internal_end_port)<parseInt(remote_port)) && remote_port!=""))
	{
		alert(invalid_port_used);
		return false;
	}		
	if(cf.hidden_service_name.value=="NetMeeting")
		cf.serflag.value=1;
	else
		cf.serflag.value=0;

	cf.submit_flag.value="forwarding_hidden_add";
	cf.submit();
	return true;
}

//to fix bug 25641
function int_port_value()
{
	var cf=document.forms[0];
	if(cf.same_range.checked == true){
		cf.internal_port_start.value = cf.port_start.value;
		cf.internal_port_end.value = cf.port_end.value;
	}
	else{
		if(_isNumeric(cf.internal_port_start.value) && cf.internal_port_start.value != "")
			cf.internal_port_end.value = (cf.internal_port_start.value*1)+((cf.port_end.value-cf.port_start.value)*1);
		else
			cf.internal_port_end.value = "";
	}
}
