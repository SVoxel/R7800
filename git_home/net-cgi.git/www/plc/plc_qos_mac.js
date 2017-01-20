function check_qos_mac(cf)
{
	var qos_mac = cf.plc_qos_mac_addr.value;
	if(qos_mac.indexOf(":")==-1 && qos_mac.length=="12")
	{
        	var tmp_mac=qos_mac.substr(0,2)+":"+qos_mac.substr(2,2)+":"+qos_mac.substr(4,2)+":"+qos_mac.substr(6,2)+":"+qos_mac.substr(8,2)+":"+qos_mac.substr(10,2);
		cf.plc_qos_mac_addr.value = tmp_mac;
        }
	else if ( qos_mac.split("-").length == 6 )
	{
		cf.plc_qos_mac_addr.value = qos_mac.replace(/-/g,":");
	}	

	if(maccheck_multicast(cf.plc_qos_mac_addr.value) == false)
		return false;

	var i;
	for(i=1; i<=qos_mac_num; i++)
	{
		var qos_mac_each = eval('qos_mac_Array' + i);
		var each_info = qos_mac_each.split(' ');
		if(cf.plc_qos_mac_addr.value == each_info[1])
		{
			if( (add_edit_flag == "edit") && (select_num == i) )	
			{
				continue;
			}
			else
			{
				alert("$mac_dup");
				return false;
			}
		}
	}

	if(add_edit_flag == "edit")
	{
		cf.submit_flag.value = "plc_qos_mac_edit";
		cf.action = "/apply.cgi?/PLC_dev_configure.htm timestamp="+ts;
	}
	cf.submit();		
	return true;
}
