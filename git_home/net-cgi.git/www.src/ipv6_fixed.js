function check_ipv6_fixed(cf)
{
	var i;
	var ip_trans_array = new Array("","","","","");//save IP address, if the IP input value is blank, change it to 0
	cf.ipv6_wan_ip_fixed.value = "";
	cf.ipv6_wan_gw_fixed.value = "";
	cf.ipv6_primary_dns_fixed.value = "";
	cf.ipv6_second_dns_fixed.value = "";
	cf.ipv6_lan_ip_fixed.value = "";

	var check_pri_dns = 1;/* 1: Primary DNS is not empty;*/
	var check_sec_dns = 1;/* 1: Second DNS is not empty;*/
	var check_gate_way = 1; /* 1: gateway is not empty; for bug 26010*/
	var pri_dns = "";
	var sec_dns = "";
	var gate_way = ""; // for bug 26010
	for( i=1; i<=8; i++ )
	{
		pri_dns = pri_dns + eval('document.getElementsByName("PDAddr'+i+'")[0]').value;
		sec_dns = sec_dns + eval('document.getElementsByName("SDAddr'+i+'")[0]').value;
		gate_way = gate_way + eval('document.getElementsByName("IPv6Gateway'+i+'")[0]').value;
	}
	if( pri_dns == "" )
	{
		check_pri_dns = 0;
	}
	if( sec_dns == "")
	{
		check_sec_dns = 0;
	}
	if( gate_way == "") 
	{
		check_gate_way = 0;
	}
	

	/****************************check the IP legality***************************************/	
	for( i=1; i<=8; i++ )
	{
		var ipv6_wan_value = eval('document.getElementsByName("IPv6WanAddr'+i+'")[0]').value;
		/*---------------------------WAN IP---------------------------------*/
		if(check_ipv6_IP_address(ipv6_wan_value) == false)
		{
			alert("$invalid_wan_ipv6_hex");
                        return false;
		}

		if(ipv6_wan_value != "")			//00xx  ------>   xx
			ipv6_wan_value = parseInt(ipv6_wan_value, 16).toString(16);

		if(ipv6_wan_value == "")
		{
			ip_trans_array[0] = ip_trans_array[0] + "0" + ":";
			cf.ipv6_wan_ip_fixed.value = cf.ipv6_wan_ip_fixed.value + "0" + ":";/* to fix bug 24430 */
		}
		else
		{
			ip_trans_array[0] = ip_trans_array[0] + ipv6_wan_value + ":"; 
			cf.ipv6_wan_ip_fixed.value = cf.ipv6_wan_ip_fixed.value + ipv6_wan_value + ":";
		}


		/*----------------------------Gateway-------------------------------------*/
		if( check_gate_way ){
		var ipv6_gateway_value = eval('document.getElementsByName("IPv6Gateway'+i+'")[0]').value;
		if(check_ipv6_IP_address(ipv6_gateway_value) == false)
		{
			alert("$invalid_wan_ipv6_gateway");
			return false;
		}

		if(ipv6_gateway_value != "")
			ipv6_gateway_value = parseInt(ipv6_gateway_value, 16).toString(16);

		if(ipv6_gateway_value == "")
		{
			ip_trans_array[1] = ip_trans_array[1] + "0" + ":";
			cf.ipv6_wan_gw_fixed.value = cf.ipv6_wan_gw_fixed.value +"0" + ":";/* to fix bug 24430 */
		}
		else
		{
			ip_trans_array[1] = ip_trans_array[1] + ipv6_gateway_value + ":";
			cf.ipv6_wan_gw_fixed.value = cf.ipv6_wan_gw_fixed.value + ipv6_gateway_value + ":";
		}
     }
 
		/*----------------------------Primary DNS------------------------------------*/	
		if( check_pri_dns ){// to fix bug 26923
			var ipv6_pdaddr_value = eval('document.getElementsByName("PDAddr'+i+'")[0]').value;
			if(check_ipv6_DNS_address(ipv6_pdaddr_value) == false)
			{
				alert("$invalid_ipv6_primary_dns_hex");
				return false;
			}

			if(ipv6_pdaddr_value != "")
				ipv6_pdaddr_value = parseInt(ipv6_pdaddr_value, 16).toString(16);

			if(ipv6_pdaddr_value == "")
			{
				ip_trans_array[2] = ip_trans_array[2] + "0" + ":";
				cf.ipv6_primary_dns_fixed.value = cf.ipv6_primary_dns_fixed.value + "0" + ":";/* to fix bug 24430  bug33138*/
			}
			else
			{
				ip_trans_array[2] = ip_trans_array[2] + ipv6_pdaddr_value + ":";
				cf.ipv6_primary_dns_fixed.value = cf.ipv6_primary_dns_fixed.value + ipv6_pdaddr_value + ":";
			}
		}


		/*------------------------------Second DNS------------------------------------*/
		if( check_sec_dns ){// to fix bug 26923
			var ipv6_sdaddr_value = eval('document.getElementsByName("SDAddr'+i+'")[0]').value;
			if(check_ipv6_DNS_address(ipv6_sdaddr_value) == false)
			{
				alert("$invalid_ipv6_second_dns_hex");
				return false;
			}

			if(ipv6_sdaddr_value != "")
				ipv6_sdaddr_value = parseInt(ipv6_sdaddr_value, 16).toString(16);

			if(ipv6_sdaddr_value == "")
			{
				ip_trans_array[3] = ip_trans_array[3] + "0" + ":";
				cf.ipv6_second_dns_fixed.value = cf.ipv6_second_dns_fixed.value + "0" + ":";/* to fix bug 24430  bug33138*/
			}
			else
			{
				ip_trans_array[3] = ip_trans_array[3] + ipv6_sdaddr_value + ":";
				cf.ipv6_second_dns_fixed.value = cf.ipv6_second_dns_fixed.value + ipv6_sdaddr_value + ":";
			}
		}


		/*---------------------------------LAN----------------------------------------*/
		if(check_ipv6_IP_address(cf.IPv6_lan[i-1].value) == false)
		{
			alert("$invalid_lan_ipv6_hex");
			return false;
		}
	
		if(cf.IPv6_lan[i-1].value != "")
			cf.IPv6_lan[i-1].value = parseInt(cf.IPv6_lan[i-1].value, 16).toString(16);

		if(cf.IPv6_lan[i-1].value == "")
		{
			ip_trans_array[4] = ip_trans_array[4] + "0" + ":";
			cf.ipv6_lan_ip_fixed.value = cf.ipv6_lan_ip_fixed.value + "0" + ":";/* to fix bug 24430 */
		}
		else
		{
			ip_trans_array[4] = ip_trans_array[4] + cf.IPv6_lan[i-1].value + ":";
			cf.ipv6_lan_ip_fixed.value = cf.ipv6_lan_ip_fixed.value + cf.IPv6_lan[i-1].value + ":";
		}
	}

        if(check_ipv6_IP_address(cf.ProfixWanLength.value) == false)
        {
                alert("$invalid_wan_ipv6_addr_pre_length");
                return false;
        }
        if(check_ipv6_IP_address(cf.IPv6_lan_prefix.value) == false)
        {
                alert("$invalid_lan_ipv6_addr_pre_length");
                return false;
        }
	/****************************************************************************************************/

	/*------------------------------WAN-------------------------------------*/
	var str = cf.ipv6_wan_ip_fixed.value;
	cf.ipv6_wan_ip_fixed.value = str.substring(0, str.length-1);

	if(cf.ipv6_wan_ip_fixed.value == ":::::::")
	{
		cf.ipv6_wan_ip_fixed.value = "";
	}
	else if(cf.ipv6_wan_ip_fixed.value == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_wan_ipv6_addr");
		return false;
	}
	else if(ip_trans_array[0] == "0:0:0:0:0:0:0:0:")
	{
		alert("$invalid_wan_ipv6_addr");
		return false;
	}

	cf.ipv6_wan_fixed_prefix.value = cf.ProfixWanLength.value;
	if(cf.ipv6_wan_ip_fixed.value == "")
	{
		alert("$wan_lack_ipv6_pre_length");
		return false;
	}
	if(cf.ipv6_wan_fixed_prefix.value == "" || parseInt(cf.ipv6_wan_fixed_prefix.value, 10) > 126 || parseInt(cf.ipv6_wan_fixed_prefix.value, 10) < 4)
	{
		alert("$invalid_wan_ipv6_addr_pre_length");
		return false;
	}
	//for bug 20078, and special checking temply
	if(check_addr_legality(cf.ipv6_wan_ip_fixed.value) == false)
	{
		alert("$invalid_wan_ipv6_addr");
		return false;
	}

	/*-----------------------------gateway----------------------------------*/
	str = cf.ipv6_wan_gw_fixed.value;
	cf.ipv6_wan_gw_fixed.value = str.substring(0, str.length-1);

	if(cf.ipv6_wan_gw_fixed.value == ":::::::")
	{
		cf.ipv6_wan_gw_fixed.value = "";
	}
	else if(cf.ipv6_wan_gw_fixed.value == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_ipv6_gateway");
		return false;
	}
	else if(ip_trans_array[1] == "0:0:0:0:0:0:0:0:")
	{
		alert("$invalid_ipv6_gateway");
		return false;
	}
	//for bug 20078, and special checking temply
	if(cf.ipv6_wan_gw_fixed.value != "")
	{
		if(check_addr_legality(cf.ipv6_wan_gw_fixed.value) == false)
		{
			alert("$invalid_ipv6_gateway");
			return false;
		}
	}

	/*-----------------------------primary DNS----------------------------------*/
	str = cf.ipv6_primary_dns_fixed.value;
	cf.ipv6_primary_dns_fixed.value = str.substring(0, str.length-1);
	
	if(cf.ipv6_primary_dns_fixed.value == ":::::::")
	{
		cf.ipv6_primary_dns_fixed.value = "";
	}
	else if(cf.ipv6_primary_dns_fixed.value == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_ipv6_primary_dns");
		return false;
	}
	else if(ip_trans_array[2] == "0:0:0:0:0:0:0:0:")
        {
		alert("$invalid_ipv6_primary_dns");
                return false;
        }
	//for bug 20078, and special checking temply
	if(cf.ipv6_primary_dns_fixed.value != "")
	{
		if(check_addr_legality(cf.ipv6_primary_dns_fixed.value) == false)
		{
			alert("$invalid_ipv6_primary_dns");
			return false;
		}
		else if(cf.ipv6_primary_dns_fixed.value == cf.ipv6_wan_ip_fixed.value )/* to fix bug 26445 */
		{
			alert("$invalid_wan_primary_second_dns");
			return false;
		}
	}

	/*-----------------------------second DNS-------------------------------------*/
	str = cf.ipv6_second_dns_fixed.value;
	cf.ipv6_second_dns_fixed.value = str.substring(0, str.length-1);

	if(cf.ipv6_second_dns_fixed.value == ":::::::")
	{
		cf.ipv6_second_dns_fixed.value = "";
	}
	else if(cf.ipv6_second_dns_fixed.value == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_ipv6_second_dns");
		return false;
	}
	else if(ip_trans_array[3] == "0:0:0:0:0:0:0:0:")
	{
		alert("$invalid_ipv6_second_dns");
		return false;
	}
	//for bug 20078, and special checking temply
	if(cf.ipv6_second_dns_fixed.value != "")
	{
		if(check_addr_legality(cf.ipv6_second_dns_fixed.value) == false)
		{
			alert("$invalid_ipv6_second_dns");
			return false;
		}
		else
			if(cf.ipv6_second_dns_fixed.value == cf.ipv6_wan_ip_fixed.value)
			{
				alert("$invalid_wan_primary_second_dns");
				return false;
			}
			else
				if(cf.ipv6_primary_dns_fixed.value == cf.ipv6_second_dns_fixed.value )
				{
					alert("$invalid_primary_second_dns");
					return false;
				}
	}

	if(cf.IpAssign[0].checked == true)
		cf.ipv6_hidden_ip_assign.value = "1";
	else
		cf.ipv6_hidden_ip_assign.value = "0";

	/*-------------------------------LAN--------------------------------------*/
	str = cf.ipv6_lan_ip_fixed.value;
	cf.ipv6_lan_ip_fixed.value = str.substring(0, str.length-1);

	if(cf.ipv6_lan_ip_fixed.value == ":::::::")
	{
		cf.ipv6_lan_ip_fixed.value = "";
	}
	else if(cf.ipv6_lan_ip_fixed.value == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_lan_ipv6_addr");
		return false;
	}
	else if(ip_trans_array[4] == "0:0:0:0:0:0:0:0:")
	{
		alert("$invalid_lan_ipv6_addr");
		return false;
	}

	cf.ipv6_lan_fixed_prefix.value = cf.IPv6_lan_prefix.value;
	if(cf.ipv6_lan_ip_fixed.value == "")
	{
		alert("$lan_lack_ipv6_pre_length");
		return false;
	}
	if(cf.IPv6_lan_prefix.value == "" || parseInt(cf.IPv6_lan_prefix.value, 10) > 126 || parseInt(cf.IPv6_lan_prefix.value, 10) < 4)
	{
		alert("$invalid_lan_ipv6_addr_pre_length");
		return false;
	}
	//for bug 20078, and special checking temply
	if(check_addr_legality(cf.ipv6_lan_ip_fixed.value) == false)
	{
		alert("$invalid_lan_ipv6_addr");
		return false;
	}

	/*************check IP confilict****************/
	var wan_ip = ip_trans_array[0];
	var gate_way = ip_trans_array[1];
	var primary_dns = ip_trans_array[2];
	var second_dns = ip_trans_array[3];
	var lan_ip = ip_trans_array[4];
	if(wan_ip!="" && (wan_ip.toUpperCase()==lan_ip.toUpperCase()))
	{
		alert("$same_ipv6_lan_wan_ip");
		return false;
	}
	if(wan_ip!="" && (wan_ip==primary_dns||wan_ip==second_dns))
	{
		alert("$invalid_wan_ipv6");
		return false;
	}
	if(lan_ip!="" && (lan_ip==primary_dns||lan_ip==second_dns))
	{
		alert("$invalid_lan_ipv6");
		return false;
	}
	/***********************************************/


	/* save IPv6 Filtering */
        if(cf.IPv6Filtering[0].checked == true)
        {
                cf.ipv6_hidden_filtering.value = "0";
        }
        else if(cf.IPv6Filtering[1].checked == true)
        {
                cf.ipv6_hidden_filtering.value = "1";
        }

	if(have_mapt && cf.cb_enable.checked == true && check_mapt(cf) == false)
		return false;
	return true;
} 


function check_addr_legality(value)
{
	//for bug 20078, and special checking temply

	//multicase IP address
	if(value.charAt(0).toLowerCase()=="f" && value.charAt(1).toLowerCase()=="f")
		return false;

	//loopback address
	if(value==":::::::1" || value=="0:0:0:0:0:0:0:1")
		return false;

	//The IP address' scope type must be global, so just can start with 2xxx or 3xxx

	//bug 25367, the checking is not needed
	/*
	var each_info = value.split(":");
	if(each_info[0].length != 4)
		return false;
	else if(each_info[0].charAt(0)!="2" && each_info[0].charAt(0)!="3")
		return false;
	*/

	return true;
}
