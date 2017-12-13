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

function check_list_length(count)
{
	if(count==64)
	{
		alert("$forward_length_64");
		return false;
	}
}

function check_forwarding_edit(cf)
{
	if (forward_array_num == 0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if (count == 0)
	{
		if(cf.RouteSelect.checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect.value);
		}
	}
	else for(i=0;i<=count;i++)
		if(cf.RouteSelect[i].checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect[i].value);
		}
	if(count_select==0)
	{
		location.href="edit_fail.htm";
		return false;
	}
	else
	{
		cf.select_edit.value=parseInt(forward_table[select_num]);
		cf.select_edit_num.value=parseInt(item_count[select_num]);
		cf.submit_flag.value="forwarding_editnum_range";
		cf.action="/apply.cgi?/forwarding_edit_wait.htm timestamp="+ts;
		cf.submit();
		return true;
	}
}


function check_forwarding_del(cf)
{
	if (forward_array_num == 0 )
	{
		location.href="del_fail.htm";
		return false;
	}
	var count_select=0;
	var select_num;
	if (count == 0)
	{
		if(cf.RouteSelect.checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect.value);
		}
	}
	else for(i=0;i<=count;i++)
		if(cf.RouteSelect[i].checked == true)
		{
			count_select++;
			select_num=parseInt(cf.RouteSelect[i].value);
		}
	if(count_select==0)
	{
		location.href="del_fail.htm";
		return false;
	}
	else
	{
		cf.select_del.value=parseInt(forward_table[select_num]);
		cf.select_del_num.value=parseInt(item_count[select_num]);
		cf.submit_flag.value="forwarding_del_range";
		cf.submit();
		return true;
	}
	
}

function Check_add(cf)     
{
	cf.serflag.value=0;
	if(check_list_length(count+1)==false)
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
		{ //bug 41501,to compare the ports in 2 cases: 1,when the ip not the same, no need to compare. 2, when the ip is the same, we should not use the start port and end port to compare the input ports, should compare the input bigger with already in use the smaller one, or compare teh input smaller one with the already in use bigger one. in here, the input port is sure smaller then the end port
		
				var ext_bigger_port = parseInt(ext_endport)>parseInt(ext_startport)?parseInt(ext_endport):parseInt(ext_startport);
				var ext_smaller_port = parseInt(ext_endport)>parseInt(ext_startport)?parseInt(ext_startport):parseInt(ext_endport);
				
				var int_bigger_port = parseInt(int_endport)>parseInt(int_startport)?parseInt(int_endport):parseInt(int_startport);
				var int_smaller_port = parseInt(int_endport)>parseInt(int_startport)?parseInt(int_startport):parseInt(int_endport);
				
			if( forwardingip == input_ip && !(ext_bigger_port<parseInt(input_external_start_port)||parseInt(input_external_end_port)<ext_smaller_port))
 			{
				alert("$ports_error_conflict");
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


function remove_space_commas(str)
{
	var str1="";
	for(i=0;i<str.length;i++)
	{
		if(str.charAt(i)==" ")
			continue;
		else
		{
			str1=str1+str.charAt(i);
		}
			
	}
	var str_each=str1.split(',');
	var str2="";
	for(i=0;i<str_each.length;i++)
	{
		if(str_each[i]=="")
			continue;
		else
		{
			str2=str2+str_each[i]+',';
		}
	}
	str2=str2.substring(0,str2.length-1);//remove the last ","
	return str2;
}

function port_rerange(port)
{
	var each_info=port.split(',');
	for(i=0;i<each_info.length;i++)
	{
		var each_port=each_info[i].split('-');
		if(each_port.length==2)//50-60
		{
			if(parseInt(each_port[0],10)==parseInt(each_port[1],10))//30-30->30
				each_info[i]=each_port[0];
			if(parseInt(each_port[0],10)>parseInt(each_port[1],10))//40-30->30-40
				each_info[i]=each_port[1]+'-'+each_port[0];
		}
	}
	for(i=0;i<each_info.length-1;i++)
	{
		var min_port=each_info[i];
		var k=i;
		for(j=i+1;j<each_info.length;j++)
		{
			var min_tmp=min_port.split('-');
			var cmp_port=each_info[j].split('-');
			if(min_tmp.length==1)
			{
				if(cmp_port.length==1)
				{
					if(parseInt(min_tmp[0])>parseInt(cmp_port[0]))//5,3
					{
						if(parseInt(min_tmp[0])-parseInt(cmp_port[0])==1)//4,3
						{
							each_info[j]=cmp_port[0]+'-'+min_tmp[0];
							each_info[k]="65535";
						}
						min_port=each_info[j];
						k=j;
					}
					else if(parseInt(min_tmp[0])==parseInt(cmp_port[0]))//5,5
					{
						each_info[j]="65535";
					}
					else if(parseInt(cmp_port[0])-parseInt(min_tmp[0])==1)//3,4
					{
						each_info[k]=min_tmp[0]+'-'+cmp_port[0];
						min_port=each_info[k];
						each_info[j]="65535";
					}
				}
				else if(cmp_port.length==2)
				{
					if(parseInt(min_tmp[0])>parseInt(cmp_port[1]))//6,3-5
					{
						if(parseInt(min_tmp[0])-parseInt(cmp_port[1])==1)
						{
							each_info[j]=cmp_port[0]+'-'+min_tmp[0];
							each_info[k]="65535";
						}
						min_port=each_info[j];
						k=j;				
					}
					else if(parseInt(cmp_port[0])-parseInt(min_tmp[0])==1)//2,3-5
					{
						each_info[k]=min_tmp[0]+'-'+cmp_port[1];
						min_port=each_info[k];
						each_info[j]="65535";
					}
					else if(parseInt(min_tmp[0])>=parseInt(cmp_port[0])&&parseInt(min_tmp[0])<=parseInt(cmp_port[1]))//6,6-8 or 8,6-8 or 7,6-8
					{
						min_port=each_info[j];
						each_info[k]="65535";
						k=j;
					}
				}
			}
			if(min_tmp.length==2)
			{
				if(cmp_port.length==1)
				{
					if(parseInt(min_tmp[0])>parseInt(cmp_port[0]))//6-8,4
					{	
						if(parseInt(min_tmp[0])-parseInt(cmp_port[0])==1)//4-5,3
						{
							each_info[j]=cmp_port[0]+'-'+min_tmp[1];
							each_info[k]="65535";
						}
						min_port=each_info[j];
						k=j;
					}
					else if(parseInt(cmp_port[0])-parseInt(min_tmp[1])==1)//2-3,4
					{
						each_info[k]=min_tmp[0]+'-'+cmp_port[0];
						min_port=each_info[k];
						each_info[j]="65535";
					}
					else if(parseInt(min_tmp[0])<=parseInt(cmp_port[0])&&parseInt(min_tmp[1])>=parseInt(cmp_port[0]))//6-8,6 or 6-8,8 or 6-8,7
					{
						each_info[j]="65535";
					}				
				}
				if(cmp_port.length==2)
				{
					if(parseInt(min_tmp[0])>parseInt(cmp_port[1]))//6-8,3-5
					{	
						if(parseInt(min_tmp[0])-parseInt(cmp_port[1])==1)
						{
							each_info[j]=cmp_port[0]+'-'+min_tmp[1];
							each_info[k]="65535";
						}
						min_port=each_info[j];
						k=j;	
					}
					else if(parseInt(cmp_port[0])-parseInt(min_tmp[1])==1)//3-5,6-8
					{
						each_info[k]=min_tmp[0]+'-'+cmp_port[1];
						min_port=each_info[k];
						each_info[j]="65535";
					}
					else if(parseInt(min_tmp[1])>=parseInt(cmp_port[0])&&parseInt(min_tmp[1])<=parseInt(cmp_port[1]))//5-8,7-8
					{
						each_info[k]=min_tmp[0]+'-'+cmp_port[1];
						min_port=each_info[k];
						each_info[j]="65535";
					}
					else if(parseInt(min_tmp[0])>=parseInt(cmp_port[0])&&parseInt(min_tmp[1])<=parseInt(cmp_port[1]))//5-8,4-9
					{
						min_port=each_info[j];
						each_info[k]="65535";
						k=j;
					}				
				}
			}		
		}		 
		if(i!=k)
		{
			var tmp=each_info[i];
			each_info[i]=each_info[k];
			each_info[k]=tmp;
		}		 
	}
	var rerange_port="";
	for(i=0;i<each_info.length;i++)
	{
		if(each_info[i]=="65535")
			continue;
		else
			rerange_port=rerange_port+each_info[i]+',';
	}
	return rerange_port;
}

function forwarding_range_check(str)
{
	if(str=="")
	{
		return false;
	}
	for(i=0;i<str.length;i++)
	{
		if(isValidForwardPort(str.charCodeAt(i))==false)
		{
			return false;
		}
	}
	var each_info=str.split(',');
	for(i=0;i<each_info.length;i++)
	{
		var each_port=each_info[i].split('-');
		if(each_port.length==1)//single num
		{
			if(parseInt(each_info[i],10)<1 || parseInt(each_info[i],10)>65534)
			{
				return false;
			}
		}
		else if(each_port.length==2)//50-60
		{
			if(each_port[0]=="" || each_port[1]=="")//-67 or 98- is not allowed
			{
				return false;
			}
			if(parseInt(each_port[0],10)<1 || parseInt(each_port[0],10)>65534)
			{
				return false;
			}
			if(parseInt(each_port[1],10)<1 || parseInt(each_port[1],10)>65534)
			{
				return false;
			}
			
		}
		else
		{
			return false;
		}
			
	}
	return true;
}
function check_forwarding_add_range(cf,flag)
{
	cf.serflag.value=0;
	/*check service name*/
	if(cf.portname.value=="")
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
	for(i=1;i<=forward_array_num;i++)
	{
		var str = eval ( 'forwardingArray' + i );
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&harr;/g, " ").replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		if(flag == 'edit')
		{
			if(each_info[0]==cf.portname.value && !(select_editnum<=i && i<parseInt(select_editnum)+parseInt(edit_num)) && each_info[7]==0)
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
	cf.portname.value=cf.portname.value.replace(/&/g, "&#38;").replace(/ /g, "&harr;");

	/*check IP*/
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
	
	/*check port*/
	var str_exter_port=remove_space_commas(cf.exter_port.value);
	if(forwarding_range_check(str_exter_port)==false)
	{
		alert("$ports_error_external");
		return false;
	}
	var str_internal_port="";
	if(cf.same_range.checked == true)
	{
		cf.hidden_port_range.value ="1";
		str_internal_port=str_exter_port;
	}
	else
	{
		cf.hidden_port_range.value ="0";
		str_internal_port=remove_space_commas(cf.internal_port.value);
		if(forwarding_range_check(str_internal_port)==false)
		{
			alert("$ports_error_internal");
			return false;
		}
	}
	/*Remove redundant port or port ranges,Shorten port range with same starting and ending ports,Reorder the ports or port ranges by ascending numbers
	str_exter_port=port_rerange(str_exter_port);
	cf.exter_port.value = str_exter_port;
	if(cf.same_range.checked == true)
		str_internal_port=str_exter_port;
	else
		str_internal_port=port_rerange(str_internal_port);
	
	cf.internal_port.value=str_internal_port;	*/

	var ext_start_port="";
	var ext_end_port="";
	var int_start_port="";
	var int_end_port="";
	var ext_each=str_exter_port.split(',');
	var int_each=str_internal_port.split(',');
	if(ext_each.length!=int_each.length)//the lenght should be the same
	{
		alert("$ports_error_mismatch");
		return false;
	}
	for(i=0;i<ext_each.length;i++)
	{
		var tmp_ext=ext_each[i].split('-');
		var tmp_int=int_each[i].split('-');
		if(tmp_ext.length!=tmp_int.length)
		{
			alert("$ports_error_mismatch");
			return false;
		}
		if(tmp_ext.length==1)
		{
			ext_start_port=ext_start_port+tmp_ext[0]+',';
			ext_end_port=ext_end_port+tmp_ext[0]+',';
			int_start_port=int_start_port+tmp_int[0]+',';
			int_end_port=int_end_port+tmp_int[0]+',';
		}
		else
		{
			if((parseInt(tmp_ext[1])-parseInt(tmp_ext[0]))!=(parseInt(tmp_int[1])-parseInt(tmp_int[0])))
			{
				alert("$ports_error_mismatch");
				return false;
			}
			ext_start_port=ext_start_port+tmp_ext[0]+',';
			ext_end_port=ext_end_port+tmp_ext[1]+',';
			int_start_port=int_start_port+tmp_int[0]+',';
			int_end_port=int_end_port+tmp_int[1]+',';	
		}
	}
	var input_sertype=cf.srvtype.value;
	var input_ip=cf.service_ip.value;
	var input_ext_start_port=ext_start_port.split(',');
	var input_ext_end_port=ext_end_port.split(',');
	var input_int_start_port=int_start_port.split(',');
	var input_int_end_port=int_end_port.split(',');
	for(k=0;k<input_ext_start_port.length-1;k++)
	{
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
			{//bug 41501,to compare the ports in 2 cases: 1,when the ip not the same, no need to compare. 2, when the ip is the same, we should not use the start port and end port to compare the input ports, should compare the input bigger with already in use the smaller one, or compare teh input smaller one with the already in use bigger one. in here, the input port is not sure smaller then the end port, so we use another var to stand for the smaller one and the bigger one.
			
				var ext_bigger_port = parseInt(ext_endport)>parseInt(ext_startport)?parseInt(ext_endport):parseInt(ext_startport);
				var ext_smaller_port = parseInt(ext_endport)>parseInt(ext_startport)?parseInt(ext_startport):parseInt(ext_endport);
				
				var int_bigger_port = parseInt(int_endport)>parseInt(int_startport)?parseInt(int_endport):parseInt(int_startport);
				var int_smaller_port = parseInt(int_endport)>parseInt(int_startport)?parseInt(int_startport):parseInt(int_endport);
				
				var input_ext_bigger_port = parseInt(input_ext_end_port[k])>parseInt(input_ext_start_port[k])?parseInt(input_ext_end_port[k]):parseInt(input_ext_start_port[k]);
				var input_ext_smaller_port = parseInt(input_ext_end_port[k])>parseInt(input_ext_start_port[k])?parseInt(input_ext_start_port[k]):parseInt(input_ext_end_port[k]);
				
				var input_int_bigger_port = parseInt(input_int_end_port[k])>parseInt(input_int_start_port[k])?parseInt(input_int_end_port[k]):parseInt(input_int_start_port[k]);
				var input_int_smaller_port = parseInt(input_int_end_port[k])>parseInt(input_int_start_port[k])?parseInt(input_int_start_port[k]):parseInt(input_int_end_port[k]);

				if(flag == 'edit')
				{//bug 41501, in edit case, the ports should compare with itself. so jump the case:!(select_editnum<=i && i<parseInt(select_editnum)+parseInt(edit_num))
					if(!(select_editnum<=i && i<parseInt(select_editnum)+parseInt(edit_num)) && (forwardingip == input_ip && !(ext_bigger_port<input_ext_smaller_port||input_ext_bigger_port<ext_smaller_port)))
					{
						alert("$ports_error_conflict");
						return false;
					}
				}
				else if(forwardingip == input_ip && !(ext_bigger_port<input_ext_smaller_port||input_ext_bigger_port<ext_smaller_port)) // for add 
				{
					alert("$ports_error_conflict");
					return false;
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
			if (!(parseInt(conend_port)<parseInt(input_ext_start_port[k])||parseInt(input_ext_end_port[k])<parseInt(constart_port) ) || !(parseInt(conend_port)<parseInt(input_int_start_port[k])||parseInt(input_int_end_port[k])<parseInt(constart_port) ))
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
				if(check_readyshare_port(input_ext_start_port[k], input_ext_end_port[k], "WAN") == false || check_readyshare_port(input_int_start_port[k], input_int_end_port[k], "WAN") == false)
				{
					alert(invalid_port_used);
					return false;
				}
			
			}
		}

		//check with remote, only use TCP
		if(endis_remote=="1" && cf.srvtype.value != "UDP" ) //15873: remote only use TCP 
		{
			if ((!(parseInt(remote_port)<parseInt(input_ext_start_port[k])||parseInt(input_ext_end_port[k])<parseInt(remote_port)) && remote_port!="") || (!(parseInt(remote_port)<parseInt(input_int_start_port[k])||parseInt(input_int_end_port[k])<parseInt(remote_port)) && remote_port!=""))
			{
				alert(invalid_port_used);
				return false;
			}		
		}
		
		//enable ntp, only use UDP
		if( (parseInt(input_ext_start_port[k])<=123 && parseInt(input_ext_end_port[k])>=123 && endis_ntp == "1" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port[k])<=123 && parseInt(input_int_end_port[k])>=123 && endis_ntp == "1" && cf.srvtype.value != "TCP"))
		{
			alert(invalid_port_used);
			return false;
		}
		
		//enable upnp, only use UDP
		if((parseInt(input_ext_start_port[k])<=1900 && parseInt(input_ext_end_port[k])>=1900 && endis_upnp == "1" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port[k])<=1900 && parseInt(input_int_end_port[k])>=1900 && endis_upnp == "1" && cf.srvtype.value != "TCP"))
		{
			alert(invalid_port_used);
			return false;
		}
		
		//enable bigpond   15873: bigpond only use UDP
		if( (parseInt(input_ext_start_port[k])<=5050 && parseInt(input_ext_end_port[k])>=5050 && info_get_wanproto == "bigpond" && cf.srvtype.value != "TCP") || (parseInt(input_int_start_port[k])<=5050 && parseInt(input_int_end_port[k])>=5050 && info_get_wanproto == "bigpond" && cf.srvtype.value != "TCP"))
		{
			alert(invalid_port_used);
			return false;
		}
		if (cf.srvtype.value=="TCP"||cf.srvtype.value=="TCP/UDP")
		{
			if (parseInt(input_ext_end_port[k])>="1720"&&parseInt(input_ext_start_port[k])<="1720")
			cf.serflag.value=1;
		}
	}
	cf.port_start.value=ext_start_port;
	cf.port_end.value=ext_end_port;
	cf.hidden_port_int_start.value=int_start_port;
	cf.hidden_port_int_end.value=int_end_port;
	cf.submit();
	return true;
}

function int_port_value()
{
	var cf=document.forms[0];
	if(cf.same_range.checked == true)
		cf.internal_port.value=cf.exter_port.value;
}

function click_arrange_by_ip()
{
        var table = document.getElementById("pf_record");
        var tbody = table.tBodies[0];
        var tr = tbody.rows;
        var trValue = new Array();
        for (var i=0; i<tr.length; i++ ){
                trValue[i] = tr[i];
        }
        trValue.sort(compareCols(5,"int"));
        var fragment = document.createDocumentFragment();
        for (var i=0; i<trValue.length; i++ ) {
                if( i%2== 1 )
                        trValue[i].className = "even_line";
                else
                        trValue[i].className = "odd_line";
                trValue[i].cells[1].innerHTML = i+1;
                trValue[i].cells[1].className = "subhead";
                fragment.appendChild(trValue[i]);
        }
        tbody.appendChild(fragment);
}

