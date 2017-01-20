function click_ping()
{
	var cf=document.forms[0];
	if(cf.checkIPAssign[0].checked)
	{
		setDisabled(true, cf.failover_host_name);
		setDisabled(true, cf.PingIP1,cf.PingIP2,cf.PingIP3,cf.PingIP4);
	}
	else if(cf.checkIPAssign[1].checked)
	{
		setDisabled(false, cf.failover_host_name);
		setDisabled(true, cf.PingIP1,cf.PingIP2,cf.PingIP3,cf.PingIP4);
	}
	else if(cf.checkIPAssign[2].checked)
	{
		setDisabled(true, cf.failover_host_name);
		setDisabled(false, cf.PingIP1,cf.PingIP2,cf.PingIP3,cf.PingIP4);
	}
}

function change_mode()
{
	var cf=document.forms[0];
	if(cf.wan_type.value!="0")
	{
		setDisabled(true, cf.checkIPAssign[0],cf.checkIPAssign[1],cf.checkIPAssign[2]);
		setDisabled(true, cf.failover_host_name);
		setDisabled(true, cf.PingIP1,cf.PingIP2,cf.PingIP3,cf.PingIP4);
		setDisabled(true, cf.failover_retry_interval,cf.failover_count,cf.failover_resume);
		setDisabled(true, cf.w_enable_hw_detect,cf.failover_hw_time);

	}
	else
	{
		setDisabled(false, cf.checkIPAssign[0],cf.checkIPAssign[1],cf.checkIPAssign[2]);
		setDisabled(false, cf.failover_retry_interval,cf.failover_count,cf.failover_resume);
		setDisabled(false, cf.w_enable_hw_detect,cf.failover_hw_time);
		click_ping();

	}

}

function check_lte_broadband()
{
	cf = document.forms[0];

	if(cf.wan_type.value == "0" )
		cf.hidden_wan_type.value ="failover";
	else if(cf.wan_type.value == "1" )
		cf.hidden_wan_type.value ="MyDetc";
	else if(cf.wan_type.value == "2" )
		cf.hidden_wan_type.value ="AutoDetc";

	if(old_wan_type == "dhcp")
		cf.primary_link_hid.value = "dhcp";
	else if(old_wan_type == "pppoe")
		cf.primary_link_hid.value = "pppoe";
	else if(old_wan_type == "pptp")
		cf.primary_link_hid.value = "pptp";
	else if(old_wan_type == "l2tp")
		cf.primary_link_hid.value = "l2tp";
	else if(old_wan_type == "static")
		cf.primary_link_hid.value = "static";

	if( cf.checkIPAssign[0].checked == true )
		cf.hidden_detect_method.value = "0";
	else if( cf.checkIPAssign[1].checked == true )
		cf.hidden_detect_method.value = "1";
	else
		cf.hidden_detect_method.value = "2";

	cf.hidden_detect_ip.value = cf.PingIP1.value+'.'+cf.PingIP2.value+'.'+cf.PingIP3.value+'.'+cf.PingIP4.value;
	if( cf.checkIPAssign[1].checked == true )
	{
		if( cf.failover_host_name.value == "" )
		{
			alert("Hostname should not be NULL");
			return false;
		}
	}

	if( cf.checkIPAssign[2].checked == true )
	{
		if( cf.hidden_detect_ip.value != "..." )
		{
			if(checkipaddr(cf.hidden_detect_ip.value)==false)
			{
				alert("$invalid_myip");
				return false;
			}
		}
		else
		{
			alert("ip miss");
			return false;
		}
	}

	if( cf.failover_retry_interval.value == "" || cf.failover_count.value == "" || cf.failover_resume.value == "" )
	{
		alert("parameter miss");
		return false;
	}

	if( cf.w_enable_hw_detect.checked == true )
	{
		cf.hidden_enable_hardware.value = "1";
		if(cf.failover_hw_time.value == "")
		{
			alert("parameter miss");
			return false;
		}

	}
	else
		cf.hidden_enable_hardware.value = "0";

	if(cf.hidden_wan_type.value == "MyDetc")
	{
		if(lte_vendor == "sierra")
			wanproto_type="3g";
		else
			wanproto_type="lte";
	}
	else if(cf.hidden_wan_type.value == "AutoDetc")
	{
		if(internet_type=="1")
			wanproto_type="dhcp";
		else
		{
			if(internet_ppp_type =="0")
				wanproto_type="pppoe";
			else if(internet_ppp_type =="1")
				wanproto_type="pptp";
			else if(internet_ppp_type =="4")
				wanproto_type="l2tp";
			else
				wanproto_type="pppoe";
		}
	}

	if(cf.hidden_wan_type.value != broadband_wan_type)
		cf.new_wan_type.value=wanproto_type;
	mtu_change(wanproto_type);

	return true;
}
