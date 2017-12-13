/*
 * bug 23820: The displayed address has no underline

ipv6 ip address have 8 cell, one cell have 4 char
the ipv6 address format should follow RFC 4291: the syntax compressing the zeros
where the prefix length (64, in this example) is displayed after the slash '/'.. The prefix part of the
IPv6 address is underlined, and, if the prefix length is not an integral multiple of 4, the underline 
extends to the hex digit containing the last prefix bit 
(for example, <u>2001:b11</u>2::2314:6d64:dcd5:5c19:40ed/25). 
*/
function show_ipv6_ip(ip)
{
/*
		use 2001:b112::6d64:cd5:5c19:40ed/88 for example
                full ipv6 address is:
                2001:b112:0000:0000:6d64:0cd5:5c19:40ed/88
                add underline, it should show as:
                <u>2001:b112::6d64:c</u>d5:5c19:40ed/88
*/
	var show_ip, cell_ip, pre_ip, suf_ip;
	var length, cell_num, char_num, blank_num, blank_lenth;
	var i;
	show_ip = ip.split('/');
	
	if( show_ip.length > 1)
	{
		if( parseInt(show_ip[1]) >= 128 )
		{// all ip are prefix
			document.write('<u>'+show_ip[0]+'</u>'+'/'+show_ip[1]);
			return;
		}
		
		length = Math.ceil(parseInt(show_ip[1])/4);	// prefix char length: 88/4 = 22
		cell_num = Math.floor(length/4); //cell_num in full address is ( 22/4 = 5), array begin with 0.
		char_num = length % 4;	// 22 % 4 = 2: the last cell in prefix have 2 char

		cell_ip = show_ip[0].split(':');		
		blank_num = -1;
		for( i=0; i< cell_ip.length; i++)
		{
			if(cell_ip[i] == "")
				blank_num = i;	// find the "::" position, one ipv6 most have one "::"
		}
		blank_length = 8 - cell_ip.length;	// "::" contains many cells, at this case, :: contains two cells 

		// find cell_num in compressing the zeros ipv6 address. cell_num = 4
		if( cell_num > blank_num + blank_length )
			cell_num = cell_num - blank_length;
		else if( blank_num < cell_num && cell_num <=blank_num + blank_length)
			cell_num = blank_num;

		char_num = char_num - (4 - cell_ip[cell_num].length); // cell_num[4] = "cd5" = "0cd5",so char_num should Minus one

		// get the prefix before cell_ip[cell_num] (:cd5:)
		pre_ip = "";
		for( i=0; i< cell_num; i++)
		{
			pre_ip = pre_ip +cell_ip[i]+':';
		}
	
		// get the prefix in cell_ip[cell_num] (cd5, prefix = c, suffix = d5)
		if( char_num > 0)
		{	

			pre_ip = pre_ip + cell_ip[cell_num].substring(0, char_num);

			suf_ip = cell_ip[cell_num].substring(char_num, cell_ip[cell_num].length);
		}
		else
			suf_ip = cell_ip[cell_num];
		
		// get the last suffix
		for( i = cell_num+1; i< cell_ip.length; i++)
		{
			suf_ip = suf_ip + ':' + cell_ip[i];
		}

		// if at the end of prefix have ':', move ':' to suffix
		for( i = pre_ip.length - 1; i >= 0; i--)
		{
			if( pre_ip[i] == ':' )
			{
				suf_ip = ':' + suf_ip;
			}
			else
				break;
		}
		pre_ip = pre_ip.substring(0, i+1);
		
		document.write('<u>'+pre_ip+'</u>'+suf_ip+'/'+show_ip[1]);
	}
	else
	{
		document.write(ip);
	}
}


function ipv6_write_ip(ipv6_ip_addr)
{
	if(ipv6_ip_addr != "")
		ipv6_ip_addr = remove_space(ipv6_ip_addr);

	if(ipv6_ip_addr == "")
	{
		document.write("<TR><TD nowrap>$spacebar"+"$ipv6_not_available</TD></TR>");
	}
	else
	{
		var each_ip = ipv6_ip_addr.split(' ');
		var i;

		for(i=0; i<each_ip.length; i++)
		{
			document.write("<TR><TD nowrap>$spacebar")
			show_ipv6_ip(each_ip[i])
			document.write("</TD></TR>");
		}
	} 
}

function ipv6_write_dns(ipv6_ip_addr, pppoe_get_dns1, pppoe_get_dns2)
{
	if(ipv6_ip_addr == "")
	{
		document.write("<TR><TD nowrap>$spacebar"+"$ipv6_not_available</TD></TR>");
	}
	else
	{
		if(pppoe_get_dns1 == "" && pppoe_get_dns2 == "")
			document.write("<TR><TD nowrap>$spacebar"+"$ipv6_not_available</TD></TR>");
		else
		{
			if(pppoe_get_dns1 != "")
			{
				document.write("<TR><TD nowrap>$spacebar");
				document.write(pppoe_get_dns1);
				document.write("</TD></TR>");
			}
			if(pppoe_get_dns2 != "")
			{
				document.write("<TR><TD nowrap>$spacebar");
				document.write(pppoe_get_dns2);
				document.write("</TD></TR>");
			}
		}
	}
}

function ipv6_load_common(cf)
{
	/* IP Address Assignment */
        if( ipv6_ip_assign == "1" )
        {
                cf.IpAssign[0].checked = true;
        }
        else if( ipv6_ip_assign == "0" )
        {
                cf.IpAssign[1].checked = true;
        }

        /* Use This Interface ID  */
        if(ipv6_interface_type == "1")
        {
                cf.useInterfaceId.checked = true;
        }
        else
        {
                cf.useInterfaceId.checked = false;
        }
        set_interface();
        var interface_id_array = ipv6_interface_id.split(':');
	cf.IPv6_interface_id1.value = interface_id_array[0];
	cf.IPv6_interface_id2.value = interface_id_array[1];
	cf.IPv6_interface_id3.value = interface_id_array[2];
	cf.IPv6_interface_id4.value = interface_id_array[3];

	/* IPv6 Filtering */
	if(ipv6_cone_fitering == 0)
	{
		cf.IPv6Filtering[0].checked = true;
	}
	else if(ipv6_cone_fitering == 1)
	{
		cf.IPv6Filtering[1].checked = true;
	}
}

function ipv6_save_common(cf)
{
	var i;

	/* Use This Interface ID */

	if(cf.IpAssign[0].checked == true)
		cf.ipv6_hidden_ip_assign.value = "1";
	else
		cf.ipv6_hidden_ip_assign.value = "0";
	if( cf.useInterfaceId.checked == true )
        {
		cf.ipv6_hidden_enable_interface.value = "1";
		cf.ipv6_hidden_interface_id.value = "";
		for( i=1; i<=4; i++ )
		{
			var interface_value = eval('document.getElementsByName("IPv6_interface_id'+i+'")[0]').value;
			if( check_ipv6_IP_address(interface_value) == false )
			{
				alert("$ipv6_invalid_interface_id");
				return false;
			}
			if( interface_value == "" )
			{
				interface_value = "0";
			}
			if( i < 4 )
			{
				cf.ipv6_hidden_interface_id.value = cf.ipv6_hidden_interface_id.value + interface_value + ":";
			}
			else if( i == 4 )
			{
				//to fix bug29794:"interface ID" can be set to "x:x:x:0",and it wi    ll make ipv6 address of lan to be a network segment.
				if(interface_value == "0")
				{
					alert("$ipv6_invalid_interface_id");
					return false;
				}
				else
					cf.ipv6_hidden_interface_id.value = cf.ipv6_hidden_interface_id.value + interface_value;
                	}
		}
        }
        else
        {
                cf.ipv6_hidden_enable_interface.value = "0";
        }

	/* save IPv6 Filtering */
	if(cf.IPv6Filtering[0].checked == true)
	{
		cf.ipv6_hidden_filtering.value = "0"; 
	}
	else if(cf.IPv6Filtering[1].checked == true)
	{
		cf.ipv6_hidden_filtering.value = "1";
	}
	return true;
}

function set_interface()
{
	var cf = document.forms[0];
	if( cf.useInterfaceId.checked == true )
	{
		cf.IPv6_interface_id1.disabled = false;
		cf.IPv6_interface_id2.disabled = false;
		cf.IPv6_interface_id3.disabled = false;
		cf.IPv6_interface_id4.disabled = false;
	}
	else if( cf.useInterfaceId.checked == false )
	{	
		cf.IPv6_interface_id1.disabled = true;
		cf.IPv6_interface_id2.disabled = true;
		cf.IPv6_interface_id3.disabled = true;
		cf.IPv6_interface_id4.disabled = true;
	}
}
//bug 26966 dns server ipv6 address should be able to leave these fields unspecified
function check_ipv6_DNS_address(ipv6_dns_value)
{
	var i;

	if(ipv6_dns_value != "")
	{
		for(i=0; i<ipv6_dns_value.length;)
		{
			if((ipv6_dns_value.charAt(i)>="0" && ipv6_dns_value.charAt(i)<="9") || (ipv6_dns_value.charAt(i)>="a" && ipv6_dns_value.charAt(i)<="f") || (ipv6_dns_value.charAt(i)>="A" && ipv6_dns_value.charAt(i)<="F"))
			{
				i++;
			}
			else
			{
				return false;
			}
		}
	}

	return true;	
}

function check_ipv6_IP_address(ipv6_ip_value)
{
	var i;

	if(ipv6_ip_value != "")
	{
		for(i=0; i<ipv6_ip_value.length;)
		{
			if((ipv6_ip_value.charAt(i)>="0" && ipv6_ip_value.charAt(i)<="9") || (ipv6_ip_value.charAt(i)>="a" && ipv6_ip_value.charAt(i)<="f") || (ipv6_ip_value.charAt(i)>="A" && ipv6_ip_value.charAt(i)<="F"))
			{
				i++;
			}
			else
			{
				return false;
			}
		}
	}
	// 2lines for bug 26010
	//else//bug 23597:the address can't blank
	//	return false;

	return true;	
}

function change_conn_type_name(conn_type)
{
	var type;
	if(conn_type=="Detecting...")
		type="$ipv6_detecting";
	else if(conn_type=="6to4 Tunnel")
		type="$ipv6_6to4_tunnel";
	else if(conn_type=="Pass Through")
		type="$ipv6_pass_through";
	else if(conn_type=="Auto Config")
		type="$ipv6_auto_config";
	else if(conn_type=="DHCP")
		type="$router_status_dhcp";
	else
		type=conn_type;
	return type;
}

function setIPv6DNS(cf)
{
        var dflag = cf.DNSAssign[0].checked;
	for(i=1; i<=8 ; i++)
	{
		eval('document.getElementsByName("PDAddr'+i+'")[0]').disabled = dflag;
		eval('document.getElementsByName("SDAddr'+i+'")[0]').disabled = dflag;
	}
}

function check_addr_legality(value)
{
	if(value.charAt(0).toLowerCase()=="f" && value.charAt(1).toLowerCase()=="f")
		return false;

	if(value==":::::::1" || value=="0:0:0:0:0:0:0:1")
		return false;

	return true;
}

function checkIPv6DNS(cf)
{
	if(cf.DNSAssign[1].checked)
	{
		cf.ipv6_hidden_primary_dns.value = "";
		cf.ipv6_hidden_second_dns.value = "";
		var check_pri_dns = 1;/* 1: Primary DNS is not empty;*/
		var check_sec_dns = 1;/* 1: Second DNS is not empty;*/
		var pri_dns = "";
		var sec_dns = "";

		for( i=1; i<=8; i++ )
		{
			pri_dns = pri_dns + eval('document.getElementsByName("PDAddr'+i+'")[0]').value;
			sec_dns = sec_dns + eval('document.getElementsByName("SDAddr'+i+'")[0]').value;
		}
		if( pri_dns == "" )
			check_pri_dns = 0;
		if( sec_dns == "")
			check_sec_dns = 0;

		if(check_pri_dns == 0 && check_sec_dns == 0)
		{
			alert("$dns_must_specified");
			return false;
		}

		for( i=1; i<=8; i++ )
		{
			if( check_pri_dns )
			{
				var pri_dns_value = eval('document.getElementsByName("PDAddr'+i+'")[0]').value;				
				if(check_ipv6_DNS_address(pri_dns_value) == false)
				{
					alert("$invalid_ipv6_primary_dns_hex");
					return false;
				}

				if(pri_dns_value != "")
					pri_dns_value = parseInt(pri_dns_value, 16).toString(16);

				if(pri_dns_value == "")
					cf.ipv6_hidden_primary_dns.value = cf.ipv6_hidden_primary_dns.value + "0" + ":";
				else
					cf.ipv6_hidden_primary_dns.value = cf.ipv6_hidden_primary_dns.value + pri_dns_value + ":";
			}

			if( check_sec_dns )
			{
				var sec_dns_value = eval('document.getElementsByName("SDAddr'+i+'")[0]').value;
				if(check_ipv6_DNS_address(sec_dns_value) == false)
				{
					alert("$invalid_ipv6_second_dns_hex");
					return false;
				}

				if(sec_dns_value != "")
					sec_dns_value = parseInt(sec_dns_value, 16).toString(16);

				if(sec_dns_value == "")
					cf.ipv6_hidden_second_dns.value = cf.ipv6_hidden_second_dns.value + "0" + ":";
				else
					cf.ipv6_hidden_second_dns.value = cf.ipv6_hidden_second_dns.value + sec_dns_value + ":";
			}
		}

		var str = cf.ipv6_hidden_primary_dns.value;
		cf.ipv6_hidden_primary_dns.value = str.substring(0, str.length-1);
		if(cf.ipv6_hidden_primary_dns.value == ":::::::")
			cf.ipv6_hidden_primary_dns.value = "";
		else if(cf.ipv6_hidden_primary_dns.value == "0:0:0:0:0:0:0:0")
		{
			alert("$invalid_ipv6_primary_dns");
			return false;
		}

		if(cf.ipv6_hidden_primary_dns.value != "")
		{
			if(check_addr_legality(cf.ipv6_hidden_primary_dns.value) == false)
			{
				alert("$invalid_ipv6_primary_dns");
				return false;
			}
		}

		str = cf.ipv6_hidden_second_dns.value;
		cf.ipv6_hidden_second_dns.value = str.substring(0, str.length-1);
		
		if(cf.ipv6_hidden_second_dns.value == ":::::::")
			cf.ipv6_hidden_second_dns.value = "";
		else if(cf.ipv6_hidden_second_dns.value == "0:0:0:0:0:0:0:0")
		{
			alert("$invalid_ipv6_second_dns");
			return false;
		}

		if(cf.ipv6_hidden_second_dns.value != "")
		{
			if(check_addr_legality(cf.ipv6_hidden_second_dns.value) == false)
			{
				alert("$invalid_ipv6_second_dns");
				return false;
			}
			else if(cf.ipv6_hidden_primary_dns.value == cf.ipv6_hidden_second_dns.value )
			{
				alert("$invalid_primary_second_dns");
				return false;
			}
		}
	}
}

function load_ipv6_dns(cf)
{
	if (get_dns_assign == '0')
		cf.DNSAssign[0].checked = true;
	else
		cf.DNSAssign[1].checked = true;

	setIPv6DNS(cf);

	if(ipv6_get_dns1 !="")
	{
		var ipv6_primary_dns = ipv6_get_dns1.split(":");
		for( i=1; i<=ipv6_primary_dns.length; i++ )
		{
			if(ipv6_primary_dns[i-1]=="")
				eval('document.getElementsByName("PDAddr'+i+'")[0]').value = "0";
			else
				eval('document.getElementsByName("PDAddr'+i+'")[0]').value = ipv6_primary_dns[i-1];
		}
	}

	if(ipv6_get_dns2 != "")
	{
		var ipv6_second_dns = ipv6_get_dns2.split(":");
		for( i=1; i<=ipv6_second_dns.length; i++ )
		{
			if(ipv6_second_dns[i-1]=="")
				eval('document.getElementsByName("SDAddr'+i+'")[0]').value = "0";
			else
				eval('document.getElementsByName("SDAddr'+i+'")[0]').value = ipv6_second_dns[i-1];
		}
	}
}

function load_mapt()
{
	var cf = document.forms[0];
	var i;

	if(enable_value == "1")
		cf.cb_enable.checked = true;
	else
		cf.cb_enable.checked = false;

	if(type_value == "1")
		cf.rd_type[1].checked = true;
	else
		cf.rd_type[0].checked = true;

	 /* local IPv6 Address/Prefix Length */
	var ipv6_ip = local_ipv6.split(":");
	for( i=0; i<ipv6_ip.length; i++ )
	{
		if(ipv6_ip[i]=="" && local_ipv6!="")
			cf.tx_lol_ipv6[i].value = "0";
		else
			cf.tx_lol_ipv6[i].value = ipv6_ip[i];
	}
	if(local_ipv6_pre_len=="" && local_ipv6!="")
		cf.tx_lol_ipv6_prefix.value = "0";
	else
		cf.tx_lol_ipv6_prefix.value = local_ipv6_pre_len;

	var ipv4_ip = local_ipv4.split(".");
	if(ipv4_ip.length == 4 )
	{
		for(i=0; i<4; i++)
		{
			cf.tx_lol_ipv4[i].value=ipv4_ip[i];
		}
	}
	if(local_ipv4_pre_len=="" && local_ipv4!="")
		cf.tx_lol_ipv4_prefix.value="0";
	else
		cf.tx_lol_ipv4_prefix.value = local_ipv4_pre_len;

	cf.tx_lol_ea.value=local_ea;

	/* remote IPv6 addr */
	var r_ipv6 = remote_ipv6.split(":");
	for(i=0; i<r_ipv6.length; i++)
	{
		if(r_ipv6[i]=="" && remote_ipv6!="")
			cf.tx_rmo_ipv6[i].value = "0";
		else
			cf.tx_rmo_ipv6[i].value = r_ipv6[i];
	}
	if(remote_ipv6_pre_len=="" && remote_ipv6!="")
		cf.tx_rmo_ipv6_prefix.value = "0";
	else
		cf.tx_rmo_ipv6_prefix.value = remote_ipv6_pre_len;
	cf.psidoffset.value=loc_psidoffset;

}

function check_ipv6_ip(iptxt, ipvalue)
{
	var ip="";

	for(var i=0; i<iptxt.length; i++)
	{
		var oneip = iptxt[i].value;
		for(var j=0; j<oneip.length; j++)
		{
			if(isValidHex(oneip.charAt(j)) == false)
				 return false;
		}

		if( oneip !="")
			oneip = parseInt(oneip,16).toString(16);

		if(oneip == "")
			ip = ip+"0"+":";
		else
			ip = ip+oneip+":";

	}

	ipvalue.value = ip.substring(0, ip.length-1);
	return true;
}

function check_mapt(cf)
{
	if(cf.cb_enable.checked == true)
		cf.hid_cb_enable.value="1";
	else
		cf.hid_cb_enable.value="0";


	if(cf.rd_type[1].checked == true)
	{
		if(!check_ipv6_ip(cf.tx_lol_ipv6, cf.hid_tx_lol_ipv6))
		{
			alert("$invalid_loc_ipv6_addr");
			return false;
		}

		cf.hid_tx_lol_ipv4.value=cf.tx_lol_ipv4[0].value+"."+cf.tx_lol_ipv4[1].value+"."+cf.tx_lol_ipv4[2].value+"."+cf.tx_lol_ipv4[3].value;
		if(checkipaddr(cf.hid_tx_lol_ipv4.value)==false)
		{
			alert("$invalid_loc_ipv4_addr");
			return false;
		}
		if(!check_ipv6_ip(cf.tx_rmo_ipv6, cf.hid_tx_rmo_ipv6))
		{
			alert("$invalid_remote_ipv6_addr");
			return false;
		}
	}


	return true;
}

function abbr_ipv6(ip)
{
	var ip_array=ip.split(":");
	var start=-1, end=-1, abbr="";
	for(var i=0; i<ip_array.length; i++)
	{
		if(ip_array[i] == "0")
		{
			if(start == -1 )
				start = i;
			else
				end = i;
		}
		else
		{
			if(end - start > 0)
				break;
			else
			{
				start = -1;
				end = -1;
			}
		}
	}
	if(end - start > 0)
	{
		for(var i=0; i<start; i++)
			abbr += ip_array[i] + ":";
		if(start == 0)
			abbr += "::";
		else
			abbr += ":";
		for(var i=end+1; i<ip_array.length; i++)
			if(i != ip_array.length -1)
				abbr += ip_array[i] + ":";
			else
				abbr += ip_array[i];
	}
	else
		abbr = ip;

	return abbr;
}

function login_type_show(cf)
{
	var login_type_list = cf.login_type;
	var ipv6_6rd_flag = top.ipv6_6rd_flag;
	var ipv6_auto_detect = (typeof(top.ipv6_auto_detect) == "undefined") ? 1 : top.ipv6_auto_detect;
	var ipv6_dhcp_flag = (typeof(top.ipv6_dhcp_flag) == "undefined") ? 1 : top.ipv6_dhcp_flag;
	var ipv6_pppoe_flag = (typeof(top.ipv6_pppoe_flag) == "undefined") ? 1 : top.ipv6_pppoe_flag;
	var ipv6_items = new Array("disabled", "$pppoe2_disable", "autoDetect", "$ipv6_auto_detect", "autoConfig", "$ipv6_auto_config", "6to4", "$ipv6_6to4_tunnel", "6rd", "6rd", "bridge", "$ipv6_pass_through", "fixed", "$ipv6_fixed", "dhcp", "$router_status_dhcp", "pppoe", "$basic_intserv_pppoe");
	var len = 0;
	
	login_type_list.options.length = 20;
	for(i=0; i<ipv6_items.length; i+=2)
	{
		if((ipv6_6rd_flag != 1 && ipv6_items[i] == "6rd") || (ipv6_auto_detect != 1 && ipv6_items[i] == "autoDetect") || (ipv6_dhcp_flag != 1 && ipv6_items[i] == "dhcp") || (ipv6_pppoe_flag != 1 && ipv6_items[i] == "pppoe"))
			continue;
		else{
			login_type_list.options[len].text = ipv6_items[i+1];
			login_type_list.options[len].value = ipv6_items[i];
			len++;
		}
	}	
	login_type_list.options.length = len;
}
