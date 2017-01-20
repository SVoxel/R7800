function check_qos_mac(flag)
{
	var cf = document.forms[0];
	var select_num = 0;
	
	if(flag == "edit" || flag == "delete")
	{
		var i;
		
		if(qos_mac_num == 1)
		{
			if(cf.qos_mac_select.checked == true)
			{
				select_num = 1;
			}
		}
		else
		{
			for(i=1; i<=qos_mac_num; i++)
			{
				if(cf.qos_mac_select[i-1].checked == true)
				{
					select_num = i;
				}
			}
		}	
	}	
	if(select_num == 0 && flag != "add")
	{
		if(flag == "edit")
		{
			alert("$port_edit");
		}
		else if(flag == "delete")
		{
			alert("$port_del");
		}
		
		return false;
	}

	if(flag == "add")
	{
		if(qos_mac_num >= 8)
			return false;

		cf.plc_qos_mac_flag.value = "add";
		cf.plc_qos_mac_select.value = select_num;
		cf.submit_flag.value = "plc_qos_dev_mac_config";
		cf.action = "/apply.cgi?/PLC_qos_mac.htm timestamp="+ts;
	}
	else if(flag == "edit")
	{
		cf.plc_qos_mac_flag.value = "edit";
		cf.plc_qos_mac_select.value = select_num;
		cf.submit_flag.value = "plc_qos_dev_mac_config";
		cf.action = "/apply.cgi?/PLC_qos_mac.htm timestamp="+ts;
	}
	else if(flag == "delete")
	{
		cf.plc_qos_mac_select.value = select_num;
		cf.submit_flag.value = "plc_qos_dev_mac_delete";
		cf.action = "/apply.cgi?/PLC_dev_configure.htm timestamp="+ts;
	}
	return true;
}

function check_qos_port(flag)
{
	var cf = document.forms[0];
	var select_num = 0;

	if(flag == "edit" || flag == "delete")
	{
		var i;

		if(qos_port_num == 1)
		{
			if(cf.qos_port_select.checked == true)
			{
				select_num = 1;
			}
		}
		else
		{
			for(i=1; i<=qos_port_num; i++)
			{
				if(cf.qos_port_select[i-1].checked == true)
				{
					select_num = i;
				}
			}
		}
	}
	if(select_num == 0 && flag != "add")
	{
		if(flag == "edit")
                {
                        alert("$port_edit");
                }
                else if(flag == "delete")
                {
                        alert("$port_del");
                }

                return false;
	}

	if(flag == "add")
	{
		if(qos_port_num >= 8)
			return false;

		cf.plc_qos_port_flag.value = "add";
		cf.plc_qos_port_select.value = select_num;
                cf.submit_flag.value = "plc_qos_dev_port_config";
                cf.action = "/apply.cgi?/PLC_qos_port.htm timestamp="+ts;
	}
	else if(flag == "edit")
        {
		cf.plc_qos_port_flag.value = "edit";
                cf.plc_qos_port_select.value = select_num;
                cf.submit_flag.value = "plc_qos_dev_port_config";
                cf.action = "/apply.cgi?/PLC_qos_port.htm timestamp="+ts;
        }
        else if(flag == "delete")
        {
                cf.plc_qos_port_select.value = select_num;
                cf.submit_flag.value = "plc_qos_dev_port_delete";
                cf.action = "/apply.cgi?/PLC_dev_configure.htm timestamp="+ts;
        }

	return true;
}

function check_DAK_status()
{
	var i;
	var cf = document.forms[0];

	//when there is only one device it must be loacl device, so the DAK shoud not be able to edit
	if(plc_avail_dev_num == 1)	
		cf.DAK.disabled = true;
	else if(plc_avail_dev_num > 1)
	{
		for(i=0; i<plc_avail_dev_num; i++)
		{
			if(cf.DAK[i].readOnly != true)
			{
				if(cf.select_dev[i].checked == true)
					cf.DAK[i].disabled = false;
				if(cf.select_dev[i].checked == false)
					cf.DAK[i].disabled = true;
			}
			else
				cf.DAK[i].disabled = true;
		}
	}

	return true;
}

function create_default_NMK()
{
	var cf = document.forms[0];

	cf.NMK.value = "HomePlugAV";
	cf.NMK.focus();
	cf.NMK.select();
}

//Bug 22008
function check_DAK_value(dak, correct_key)
{
	if(dak.indexOf("-") == -1)
	{
		var dak_parts = correct_key.split("-");
		var dak_correct_key = dak_parts[0] + dak_parts[1] + dak_parts[2] + dak_parts[3];
		
		if(dak != dak_correct_key)
			return false;
	}
	else if(dak != correct_key)
		return false;
	
	return true;
}

function plc_device_config(cf)
{
	var i;

	cf.NMK.value = remove_space(cf.NMK.value);
	if(cf.NMK.value == "")	
	{
		alert("$plc_encryp_key_error");
		return false;
	}
	else
	{
		if((cf.NMK.value.length < 8) || (cf.NMK.value.length > 64))
		{
			alert("$plc_encryp_key_error");
			return false;
		}

		for(i=0; i<cf.NMK.value.length; i++)
		{
			if(isValidNumLetter(cf.NMK.value.charCodeAt(i)) == false)
			{
				alert("$plc_encryp_key_error");
				return false;
			}
		}
	}

	if(plc_avail_dev_num == 1)
	{
		if(cf.select_dev.checked == false)
		{
			alert("$plc_dev_select");
			return false;
		}
		else 
		{
			cf.DAK.value = devices_set[1].DAK_passwd;
		}
	}
	else if(plc_avail_dev_num > 1)
	{
		var select_dev = 0;

		for(i=0; i<plc_avail_dev_num; i++)
		{
			if(cf.select_dev[i].checked == true)
			{
				select_dev = select_dev + 1;
				if(cf.DAK[i].value == "")
				{
					alert("$password_error");
					cf.DAK[i].focus() ;
					return false;
				}
				
				if(cf.DAK[i].readOnly != true)
				{
					var j;
					for(j=0; j<cf.DAK[i].value.length; j++)
					{
						if(isValidDdnsHost(cf.DAK[i].value.charCodeAt(j)) == false)
						{
							alert("$password_error");
							cf.DAK[i].value = "";
							cf.DAK[i].focus() ;
							return false;
						}
						
					}
					//devices_set[i] start from devices_set[1], cf.DAK[i] start from cf.DAK[0]
					if(check_DAK_value(cf.DAK[i].value, devices_set[i+1].DAK_passwd) == false)
                                	{
                                        	alert("$password_error");
                                        	cf.DAK[i].value = "";
                                        	cf.DAK[i].focus() ;
                                        	return false;
                                	}
				}
			}
		}

		if(select_dev == 0)
		{
			alert("$plc_dev_select");
			return false;
		}

		//Bug 22008
		//set cf.DAK[i].value to the correct password, and this action MUST be after all checkings had finished
		for(i=0; i<plc_avail_dev_num; i++)
		{
			if(cf.select_dev[i].checked == true)
			{
				//devices_set[i] start from devices_set[1], cf.DAK[i] start from cf.DAK[0]
				cf.DAK[i].value = devices_set[i+1].DAK_passwd;
			}
		}
	}

	cf.plc_dev_cfg_info.value = "";
	cf.plc_dev_select_num.value = "";
	var dev_cfg = "";	
	var total_select = 0;

	if(plc_avail_dev_num == 1)
	{
		if(cf.select_dev.checked == true)
		{
			//Bug 22008
			dev_cfg = devices_set[1].Dev_type + '-' + cf.NMK.value + '-' + cf.DAK.value + '#' + devices_set[1].MAC;
		}

		cf.plc_dev_cfg_info.value = cf.plc_dev_cfg_info.value + dev_cfg + "*";	
		total_select = 1;
	}
	else if(plc_avail_dev_num > 1)
	{
		for(i=1; i<=plc_avail_dev_num; i++)
		{
			if(cf.select_dev[i-1].checked == true)
			{
				//Bug 22008
				dev_cfg = devices_set[i].Dev_type + '-' + cf.NMK.value + '-' + cf.DAK[i-1].value + '#' + devices_set[i].MAC;
				total_select = total_select + 1;	
				cf.plc_dev_cfg_info.value = cf.plc_dev_cfg_info.value + dev_cfg + "*";
			}
		}
	}

	
	cf.plc_dev_select_num.value = total_select.toString();
	return true;
}

function plc_qos_config(cf)
{
	var i, each_info, mac_info;

	cf.plc_dev_cfg_mac.value = "";

	if(plc_avail_dev_num == 1)
        {
                var str = eval("avail_dev_Array" + 1);
                str = str.replace(/\s+/g, " ");
                var each_info = str.split(' ');

                if(cf.select_dev.checked == true)
                {
			cf.plc_dev_cfg_mac.value = cf.plc_dev_cfg_mac.value + each_info[1] + "-";
                }

        }
        else if(plc_avail_dev_num > 1)
	{
		for(i=1; i<=plc_avail_dev_num; i++)
		{
			var str = eval("avail_dev_Array" + i);
			str = str.replace(/\s+/g," ");
			var each_info = str.split(' ');

			if(cf.select_dev[i-1].checked == true)
			{
				cf.plc_dev_cfg_mac.value = cf.plc_dev_cfg_mac.value + each_info[1] + "-";
			}
		}
	}

	mac_info = cf.plc_dev_cfg_mac.value;
	if(mac_info != "")
	{
		cf.plc_dev_cfg_mac.value = mac_info.substring(0, mac_info.length-1);
	}
	
	cf.submit_flag.value = "plc_qos_setting";
	cf.action = "/apply.cgi?/PLC_wait_scan.htm timestamp="+ts;

	return true;
}
