
function check_ipv6_6rd(cf)
{
	var i;
	
	cf.ipv6_hidden_6rd_pre.value = "";
	for( i=0; i<cf.ipv6_6rd_pre.length; i++)
	{
		if(cf.ipv6_6rd_pre[i].value == "")
			cf.ipv6_6rd_pre[i].value = "0";
		if( i < (cf.ipv6_6rd_pre.length-1) )
		{
			cf.ipv6_hidden_6rd_pre.value = cf.ipv6_hidden_6rd_pre.value + cf.ipv6_6rd_pre[i].value + ":";
		}
		else if(i == (cf.ipv6_6rd_pre.length-1))
		{
			cf.ipv6_hidden_6rd_pre.value = cf.ipv6_hidden_6rd_pre.value + cf.ipv6_6rd_pre[i].value;
		}
	}
	
	if(cf.ipv6_hidden_6rd_pre.value == "0:0:0:0")
	{
		alert("Invalid IPv6 Address!");
		return false
	}

	if(cf.ipv6_pre_len.value =="" || cf.ipv6_mask_len.value=="")
	{
		alert("Please enter a valid Prefix Length!");
		return false;
	}

	cf.ipv6_hidden_6rd_relay.value = "";
	for( i=0; i<cf.relay_addr.length; i++)
	{
		if( i < (cf.relay_addr.length-1) )
		{
			cf.ipv6_hidden_6rd_relay.value = cf.ipv6_hidden_6rd_relay.value + cf.relay_addr[i].value + ".";
		}
		else if( i == (cf.relay_addr.length-1) )
		{
			cf.ipv6_hidden_6rd_relay.value = cf.ipv6_hidden_6rd_relay.value + cf.relay_addr[i].value;
		}
	}
	if( checkipaddr(cf.ipv6_hidden_6rd_relay.value) == false )
	{
		alert("$invalid_ip");
		return false;
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

