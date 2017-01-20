function check_reservation_add(cf,flag)
{
	var rsvipaddr = new Array();
	if( array_num == 64 && flag== 'add')
	{
		alert("$reservation_length_64");
		return false;
	}

	rsvipaddr = cf.rsv_ip1.value+'.'+cf.rsv_ip2.value+'.'+cf.rsv_ip3.value+'.'+cf.rsv_ip4.value;
	if(cf.dv_name.value == "")
	{
		alert("$device_name_null");
		return false;
	}
	for(i=0;i<cf.dv_name.value.length;i++)
	{ 
		if(isValidChar(cf.dv_name.value.charCodeAt(i))==false)
		{
			alert("$device_name_error");
			return false;
		}
	}

	if(checkipaddr(rsvipaddr)==false)
	{
		alert("$invalid_ip");
		return false;
	}
	if(rsvipaddr == lanip)
	{
		alert("$invalid_ip");
		return false;
	}
	if(cf.rsv_mac.value.length==12 && cf.rsv_mac.value.indexOf(":")==-1)
	{
		var mac=cf.rsv_mac.value; 
		cf.rsv_mac.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);
	}
	else if ( cf.rsv_mac.value.split("-").length == 6 )
	{
		var tmp_mac = cf.rsv_mac.value.replace(/-/g,":");
		cf.rsv_mac.value=tmp_mac;
	}	
	if(maccheck(cf.rsv_mac.value) == false)
		return false;
	if(isSameSubNet(rsvipaddr,lanmask,lanip,lanmask) == false)
	{
		alert("$diff_lan_this_subnet");
		return false;
	}	
/*	var start_array=startip.split('.');
	var end_array=endip.split('.');
	var msg_ip="$rsvip_dhcp_rang"+"[ "+startip+" ~ "+endip+" ].";

	if(!(parseInt(start_array[3]) <= parseInt(cf.rsv_ip4.value,10) && parseInt(cf.rsv_ip4.value,10) <= parseInt(end_array[3]) && start_array[2] == parseInt(cf.rsv_ip3.value,10) && start_array[1] == parseInt(cf.rsv_ip2.value,10) && start_array[0] == parseInt(cf.rsv_ip1.value,10)))
	{
		alert(msg_ip);
		return false;
	}
*/
	cf.duplicated_reservation.value="";
	cf.duplicated_num.value=0;
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'resevArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if(flag == 'edit')
		{
			if(select_editnum!=i)
			{
				if( rsvipaddr == each_info[0] || cf.rsv_mac.value.toLowerCase() == each_info[1].toLowerCase())//bug40808
				{//44938
					if( cf.duplicated_num.value == 0 )
					{
						if( confirm("$reservation_dup") )
						{
							cf.duplicated_num.value=1;
							cf.duplicated_reservation.value=i.toString();
						}
						else
							return false;
					}
					else
					{
						cf.duplicated_num.value++;
						cf.duplicated_reservation.value=cf.duplicated_reservation.value+" "+i.toString();
					}
				}
			}
		}
		else
		{
			if( rsvipaddr == each_info[0] || cf.rsv_mac.value.toLowerCase() == each_info[1].toLowerCase())//bug40808
			{//44938
				if( cf.duplicated_num.value == 0 )
				{
					if( confirm("$reservation_dup") )
					{
						cf.duplicated_num.value=1;
						cf.duplicated_reservation.value=i.toString();
					}
					else
						return false;
				}
				else
				{
					cf.duplicated_num.value++;
					cf.duplicated_reservation.value=cf.duplicated_reservation.value+" "+i.toString();
				}
			}	
		}
	}
	cf.reservation_ipaddr.value=address_parseInt(rsvipaddr);

	cf.submit();

	return true;
}
function check_reservation_del(cf)
{
	if(array_num == 0)
	{
		alert("$port_del");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.ruleSelect.checked == true)
		{
			if(is_ipmac == 1)
			{
				alert("$ipmac_del_warning");
				return false;
			}

			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.ruleSelect[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$port_del");
		return false;
	}
	else
	{
		cf.select_del.value=select_num;
		cf.submit_flag.value="reservation_del";
		cf.submit();//add
		return true;
	}	
}

function check_reservation_editnum(cf)
{
	if (array_num == 0)
	{
		alert("$port_edit");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.ruleSelect.checked == true)
		{
			count_select++;
			select_num=1;

			if(is_ipmac == 1)
			{
				alert("$ipmac_edit_warning");
			}
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.ruleSelect[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="reservation_editnum";
		cf.action="/apply.cgi?/reservation_edit.htm timestamp="+ts;
		cf.submit();//add
		return true;
	}	
}

function data_select(num)
{
	var cf=document.forms[0];
	if( show_name_array[num] == "<unknown>" || show_name_array[num] == "&lt;unknown&gt;" )
		cf.dv_name.value='<$unknown_mark>';
	else
		cf.dv_name.value=show_name_array[num].replace(/&lt;/g,"<").replace(/&gt;/g,">");

	if( show_mac_array[num] == "<unknown>" ||  show_mac_array[num] =="&lt;unknown&gt;" )
		cf.rsv_mac.value="";
	else
		cf.rsv_mac.value=show_mac_array[num];

	if( show_ip_array[num] != "----" )
	{
		var ip_array=show_ip_array[num].split('.');
		cf.rsv_ip1.value=ip_array[0];
		cf.rsv_ip2.value=ip_array[1];
		cf.rsv_ip3.value=ip_array[2];
		cf.rsv_ip4.value=ip_array[3];	
	}
}
