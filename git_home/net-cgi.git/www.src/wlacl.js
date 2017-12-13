var device_name_null="$device_name_null";
var device_name_error="$device_name_error";
var device_name_dup="$device_name_dup";
var mac_dup="$mac_dup";

function check_wlacl_add(cf,flag)
{
	if (array_num == 64)
	{
		alert("$acl_length_64");
		return false;
	}
	if(cf.device.value=="")
	{
		alert(device_name_null);
		return false;
	}	
	for(i=0;i<cf.device.value.length;i++)
	{
		if(isValidChar(cf.device.value.charCodeAt(i))==false)
		{
			alert(device_name_error);
			return false;
		}
	}
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'wlaclArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if (flag == 'add')
		{		
			if(each_info[0]==cf.device.value)
			{
				alert(device_name_dup);
				return false;
			}
		}
		else
		{
			if( each_info[0]==cf.device.value && i != select_edit )
			{
				alert(device_name_dup);
				return false;
			}
		}
	}
	if(cf.adr.value.length==12 && cf.adr.value.indexOf(":")==-1)
	{
		var mac=cf.adr.value; 
		cf.adr.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);

	}
	else if ( cf.adr.value.split("-").length == 6 )
	{
		var tmp_mac = cf.adr.value.replace(/-/g,":");
		cf.adr.value=tmp_mac;
	}	

	if(maccheck(cf.adr.value) == false)
		return false;
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'wlaclArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if (flag == 'add')
		{
			if(each_info[1].toLowerCase()==cf.adr.value.toLowerCase())
			{
				alert(mac_dup);
				return false;
			}
		}
		else
		{
			if( each_info[1].toLowerCase() == cf.adr.value.toLowerCase() && i!= select_edit )
			{
				alert(mac_dup);
				return false;
			}
		}
	}	
	return true;
}

function check_wlacl_edit(cf,flag)
{
	if(cf.device.value=="")
	{
		alert(device_name_null);
		return false;
	}
	for(i=0;i<cf.device.value.length;i++)
	{
		if(isValidChar(cf.device.value.charCodeAt(i))==false)
		{
			alert(device_name_error);
			return false;
		}
	}
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'wlaclArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if (flag == 'add')
		{
			if(each_info[0]==cf.device.value)
			{
				alert(device_name_dup);
				return false;
			}
		}
		else
		{
			if( each_info[0]==cf.device.value && i != select_edit )
			{
				alert(device_name_dup);
				return false;
			}
		}
	}
	if(cf.adr.value.length==12 && cf.adr.value.indexOf(":")==-1)
	{
		var mac=cf.adr.value; 
		cf.adr.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);

	}
	else if ( cf.adr.value.split("-").length == 6 )
	{
		var tmp_mac = cf.adr.value.replace(/-/g,":");
		cf.adr.value=tmp_mac;
	}	
	if(maccheck(cf.adr.value) == false)
		return false;
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'wlaclArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if (flag == 'add')
		{
			if(each_info[1].toLowerCase()==cf.adr.value.toLowerCase())
			{
				alert(mac_dup);
				return false;
			}
		}
		else
		{
			if( each_info[1].toLowerCase() == cf.adr.value.toLowerCase() && i!= select_edit )
			{
				alert(mac_dup);
				return false;
			}
		}
	}
	return true;
}

function check_wlacl_editnum(cf)
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
		if(cf.MacSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.MacSelect[i].checked == true)
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
		cf.submit_flag.value="wlacl_editnum";
		cf.action="/apply.cgi?/WLG_acl_edit.htm timestamp="+ts;
		return true;
	}
}

function check_wlacl_del(cf)
{
	if (array_num == 0)
	{
		alert("$port_del");
		return false;
	}
	var count_select=0;
	var select_num;
	if (array_num == 1)
	{
		if(cf.MacSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.MacSelect[i].checked == true)
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
		//bug 33156
		if( cf.accessLimit.checked == true && array_num == 1
		&& (parent.an_router_flag == 1 && endis_wla_radio == "1" || endis_wl_radio == "1"))
		{
			if(!confirm("$wps_warning4"))
				return false;
		}
		cf.select_del.value=select_num;
		cf.submit_flag.value="wlacl_del";
		return true;
	}
}

function check_wlacl_apply(cf)
{
	if( cf.accessLimit.checked == true )
		cf.wl_access_ctrl_on.value=1;
	else
		cf.wl_access_ctrl_on.value=0;
	
	//bug 33156
	if( cf.wl_access_ctrl_on.value == 1 && array_num == 0 
	&& (parent.an_router_flag == 1 && endis_wla_radio == "1" || endis_wl_radio == "1"))
	{
		return confirm("$wps_warning4");
	}
	return true;
}

function mac_data_select(num)
{
	var cf = document.forms[0];
	if(sta_name_array[num] == "&lt;$unknown_mark&gt;")
		cf.device.value = "";
	else
		cf.device.value = sta_name_array[num].replace(/&lt;/g,"<").replace(/&gt;/g,">");
	cf.adr.value=sta_mac_array[num].toUpperCase();
}
