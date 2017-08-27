function setRemote()
{
	var cf = document.forms[0];
	var i;
	if( cf.relay[0].checked == true )
	{
		cf.WIp1.disabled = true;
		cf.WIp2.disabled = true;
		cf.WIp3.disabled = true;
		cf.WIp4.disabled = true;
	}
	else if( cf.relay[1].checked == true )
	{
		cf.WIp1.disabled = false;
		cf.WIp2.disabled = false;
		cf.WIp3.disabled = false;
		cf.WIp4.disabled = false;
	}
}	

function check_ipv6_6to4(cf)
{
	var i;

	if(internet_basic_type == 0)	//not dhcp
	{	
		if((internet_ppp_type == 0) || (internet_ppp_type == 1) || (internet_ppp_type == 4))	//(pptp, 1),(pppoe, 0),(L2tp,4)
		{
			if(confirm("$ipv6_6to4_warning") == false)
			{
				return false;
			}	
		}
	}

	/* Remote 6to4 Relay Router */
	if(cf.relay[1].checked == true)
	{
		cf.ipv6_hidden_6to4_relay.value = cf.WIp1.value + "." + cf.WIp2.value + "." + cf.WIp3.value + "." + cf.WIp4.value;
		if( checkipaddr(cf.ipv6_hidden_6to4_relay.value) == false )
		{
			alert("$invalid_ip");
			return false;
		}
		cf.ipv6_hidden_remote.value = "1";
	}
	else
	{
		cf.ipv6_hidden_6to4_relay.value = ipv6_6to4_relay_ip;
		cf.ipv6_hidden_remote.value = "0";
	}
	
	if(top.ipv6_dns_manual == 1)
	{
		if(checkIPv6DNS(cf) == false)
			return false;
	}

	if( ipv6_save_common(cf) == false )
	{
		return false;
	}
	return true;
}

