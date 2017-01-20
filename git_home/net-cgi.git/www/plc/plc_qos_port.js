function check_qos_port(cf)
{
	var i;

	var port_value = cf.plc_qos_port.value;
	if(port_value == "")
	{
		alert("$pppoe2_port" + "$trigger_1_65535");
		return false;
	}
	for(i=0; i<port_value.length; i++)
	{
		var charct = port_value.charAt(i);
		if("0123456789".indexOf(charct,0) < 0)
		{
			alert("$pppoe2_port" + "$trigger_1_65535");
			return false;
		}
	}	
	if(parseInt(port_value)<1 || parseInt(port_value)>65535)
	{
		alert("$pppoe2_port" + "$trigger_1_65535");
		return false;
	}
	
        for(i=1; i<=qos_port_num; i++)
        {
                var qos_port_each = eval('qos_port_Array' + i);
                var each_info = qos_port_each.split(' ');
                if(cf.plc_qos_port.value == each_info[1])
                {
                        if( (add_edit_flag == "edit") && (select_num == i) )
                        {
                                continue;
                        }
                        else
                        {
                                alert("$plc_port_dup");
                                return false;
                        }
                }
        }

        if(add_edit_flag == "edit")
        {
                cf.submit_flag.value = "plc_qos_port_edit";
		cf.action = "/apply.cgi?/PLC_dev_configure.htm timestamp="+ts;
        }
	
	return true;
}
