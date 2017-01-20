function click_ping()
{
	var cf=document.forms[0];
	if(cf.adsl_3g[0].checked || cf.adsl_3g[1].checked)
	{
		setDisabled(true, cf.hostname_tx);
		setDisabled(true, cf.myip_1,cf.myip_2,cf.myip_3,cf.myip_4);
	}
	else if(cf.adsl_3g[2].checked)
	{
		if(cf.detect_method_rd[0].checked)
		{
			setDisabled(false, cf.hostname_tx);
			setDisabled(true, cf.myip_1,cf.myip_2,cf.myip_3,cf.myip_4);
		}
		else if(cf.detect_method_rd[1].checked)
		{
			setDisabled(true, cf.hostname_tx);
			setDisabled(false, cf.myip_1,cf.myip_2,cf.myip_3,cf.myip_4);
		}
	}
}

function check_broadband()
{
	cf = document.forms[0];
	if(cf.adsl_3g[0].checked == true){
		cf.hidden_wan_type.value = "AutoDetc";
		if(top.dsl_enable_flag == 0)
		  cf.action="/apply.cgi?/BAS_basic.htm timestamp=" + ts;
		else
		  cf.action="/apply.cgi?/BAS_basici_dsl.htm timestamp=" + ts;
	}
	else if(cf.adsl_3g[1].checked == true){
		cf.hidden_wan_type.value = "MyDetc";
		cf.action="/apply.cgi?/BAS_3g.htm timestamp=" + ts;
	}
	else
		cf.hidden_wan_type.value = "multiDetc";

	if(cf.primary_link.value == "ethernet")
	{
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
	}
	else
		cf.primary_link_hid.value = "3g";

	if( cf.detect_method_rd[0].checked == true )
		cf.hidden_detect_method.value = "0";
	else if( cf.detect_method_rd[1].checked == true )
		cf.hidden_detect_method.value = "1";

	cf.hidden_detect_ip.value = cf.myip_1.value+'.'+cf.myip_2.value+'.'+cf.myip_3.value+'.'+cf.myip_4.value;
	if( cf.detect_method_rd[0].checked == true )
	{
		if( cf.hostname_tx.value == "" )
		{
			alert("Hostname should not be NULL");
			return false;
		}
	}

	if( cf.detect_method_rd[1].checked == true )
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

	if( cf.retry_inter_tx.value == "" || cf.failover_tx.value == "" )
	{
		alert("parameter miss");
		return false;
	}

	if( cf.enable_hardware_ch.checked == true )
		cf.hidden_enable_hardware.value = "1";
	else
		cf.hidden_enable_hardware.value = "0";

	return true;
}
