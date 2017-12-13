function RU_manual_check_summary()
{
    var cf=document.forms[0];
    if(parent.welcome_wan_type == 1)
    {
			cf.ether_ipaddr.value=parent.static_ip;
			cf.ether_subnet.value=parent.static_subnet;
			cf.ether_gateway.value=parent.static_gateway;
			cf.ether_dnsaddr1.value=parent.static_dns1;
			cf.ether_dnsaddr2.value=parent.static_dns2;
			cf.DNSAssign.value="1";
			cf.domain_name.value="";
			cf.WANAssign.value="static";
	  }
	  else if(parent.welcome_wan_type == 2)
	  {
			cf.ether_ipaddr.value=parent.static_ip;
			cf.ether_subnet.value=parent.static_subnet;
			cf.ether_gateway.value=parent.static_gateway;
			cf.ether_dnsaddr1.value=parent.static_dns1;
			cf.ether_dnsaddr2.value=parent.static_dns2;
			if(parent.static_dns1 == "" && parent.static_dns2 == "")
				cf.DNSAssign.value="0";
			else
				cf.DNSAssign.value="1";
			cf.domain_name.value="";
			cf.WANAssign.value="dhcp";
	  }
	 	else if(parent.welcome_wan_type == 3)
	  {
			cf.pppoe_username.value=parent.pppoe_username;
			cf.pppoe_passwd.value=parent.pppoe_password;
			cf.pppoe_servername.value=parent.pppoe_server;
			cf.pppoe_ipaddr.value=parent.pppoe_static_ip;
			cf.dual_access.value=parent.dual_access;
			cf.pppoe_dod.value="0";
			cf.pppoe_dual_ipaddr.value=parent.pppoe_eth_ip;
			cf.pppoe_dual_subnet.value=parent.pppoe_eth_netmask;
			cf.pppoe_dual_gateway.value="";
			cf.pppoe_dual_assign.value=parent.pppoe_dual_assign;
			cf.hidden_pppoe_idle_time.value="5";
			cf.pppoe_dnsaddr1.value=parent.pppoe_dns1;
			cf.pppoe_dnsaddr2.value=parent.pppoe_dns2;
			if(parent.pppoe_dns1 == "" && parent.pppoe_dns2 == "")
				cf.DNSAssign.value="0";
			else
				cf.DNSAssign.value="1";
			if(parent.pppoe_wan_assign == "0")
				cf.WANAssign.value="Dynamic";
			else
				cf.WANAssign.value="Fixed";
		}
		else if(parent.welcome_wan_type == 4)
		{
			cf.pptp_username.value=parent.pptp_username;
			cf.pptp_passwd.value=parent.pptp_password;
			cf.pptp_myip.value=parent.pptp_local_ipaddr;
			cf.pptp_mynetmask.value=parent.pptp_local_netmask;
			cf.pptp_serv_ip.value=parent.pptp_server_ipaddr;
			cf.pptp_gateway.value=parent.pptp_local_gateway;
			cf.pptp_conn_id.value="";
			cf.pptp_dnsaddr1.value=parent.pptp_dns1;
			cf.pptp_dnsaddr2.value=parent.pptp_dns2;
			cf.pptp_dod.value="0";
			if(parent.pptp_dns1 == "" && parent.pptp_dns2 == "")
				cf.DNSAssign.value="0";
			else
				cf.DNSAssign.value="1";
			cf.WANAssign.value=parent.pptp_wan_assign;
			cf.STATIC_DNS.value=parent.STATIC_DNS;
			cf.hidden_pptp_idle_time.value="5";
		}
		else if(parent.welcome_wan_type == 5)
		{
			cf.l2tp_username.value=parent.l2tp_username;
			cf.l2tp_passwd.value=parent.l2tp_password;
			cf.l2tp_myip.value=parent.l2tp_local_ipaddr;
			cf.l2tp_mynetmask.value=parent.l2tp_local_netmask;
			cf.l2tp_serv_ip.value=parent.l2tp_server_ipaddr;
			cf.l2tp_gateway.value=parent.l2tp_local_gateway;
			cf.l2tp_conn_id.value="";
			cf.l2tp_dnsaddr1.value=parent.l2tp_dns1;
			cf.l2tp_dnsaddr2.value=parent.l2tp_dns2;
			cf.l2tp_dod.value="0";
			if(parent.l2tp_dns1 == "" && parent.l2tp_dns2 == "")
				cf.DNSAssign.value="0";
			else
				cf.DNSAssign.value="1";
			cf.WANAssign.value=parent.l2tp_wan_assign;
			cf.STATIC_DNS.value=parent.STATIC_DNS;
			cf.hidden_l2tp_idle_time.value="5";
		}
	  cf.welcome_wan_type.value=parent.welcome_wan_type;
	  cf.MACAssign.value=parent.mac_spoof;
	  cf.Spoofmac.value=parent.Spoofmac;
	  cf.conflict_wanlan.value=parent.conflict_wanlan;
	  cf.change_wan_type.value="0";
	  cf.run_test.value="no";
	  parent.RU_manual_flag="1";
		cf.submit();
} 
