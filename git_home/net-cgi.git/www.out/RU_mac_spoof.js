function RU_check_spoof()
{
    var cf=document.forms[0];
    if (cf.MACAssign[2].checked )
    {
         the_mac=cf.Spoofmac.value;
         if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
         {
                 var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
                 cf.Spoofmac.value = tmp_mac;
         }
         else if ( the_mac.split("-").length == 6 )
         {
                 var tmp_mac = the_mac.replace(/-/g,":");
                 cf.Spoofmac.value=tmp_mac;
         }
         if(maccheck_multicast(cf.Spoofmac.value) == false)
                 return false;
         parent.Spoofmac=cf.Spoofmac.value;
    }
    
    if (cf.MACAssign[0].checked )
    		parent.mac_spoof="0";
    else if (cf.MACAssign[1].checked )
    		parent.mac_spoof="1";
    else if (cf.MACAssign[2].checked )
    		parent.mac_spoof="2";
    		
	  //parent.mac_spoof=cf.MACAssign.value;
 
    if(parent.isp_type=="1") //dhcp
	  {
		    cf.basic_type.value="1";
		    cf.ppp_login_type.value="0";
		    cf.welcome_wan_type.value=2;
      	cf.WANAssign.value="dhcp";
      	cf.DNSAssign.value="0";
		cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	  }
    else if(parent.isp_type=="3") //static
	  {
		    cf.basic_type.value="1";
		    cf.ppp_login_type.value="0";
		    cf.welcome_wan_type.value=1;
	      cf.ether_ipaddr.value=parent.static_ip;
	      cf.ether_subnet.value=parent.static_subnet;
	      cf.ether_gateway.value=parent.static_gateway;
	      cf.ether_dnsaddr1.value=parent.static_dns1;
	      cf.ether_dnsaddr2.value=parent.static_dns2;
	      cf.WANAssign.value="static";
	      cf.DNSAssign.value="1";	
		  cf.conflict_wanlan.value=parent.conflict_wanlan;		  
	      cf.submit();
	  }
	  else if(parent.isp_type=="5") //pppoe+static
	  {
        cf.pppoe_username.value=parent.pppoe_username;
	      cf.pppoe_passwd.value=parent.pppoe_password;
	      cf.pppoe_servername.value=parent.pppoe_server;
			  cf.dual_access.value="1";
			  cf.pppoe_dnsaddr1.value=parent.static_dns1;
			  cf.pppoe_dnsaddr2.value=parent.static_dns2;
			  if(cf.pppoe_dnsaddr1.value != "" || cf.pppoe_dnsaddr2.value!= "")
			      cf.DNSAssign.value="1";
			  else 
			  	  cf.DNSAssign.value="0";    
			  cf.pppoe_dod.value="0";
	      cf.pppoe_dual_assign.value="1";  //eth
	      cf.WANAssign.value="Dynamic";    //ppp
	      cf.pppoe_dual_ipaddr.value=parent.static_ip;
			  cf.pppoe_dual_subnet.value=parent.static_subnet;
			  cf.pppoe_dual_gateway.value="";
	      cf.pppoe_ipaddr.value=""; 
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="0";
		    cf.welcome_wan_type.value="3";
			cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	
	  }
	  else if(parent.isp_type=="7") //pppoe+dynamic
	  {   
	      cf.pppoe_username.value=parent.pppoe_username;
	      cf.pppoe_passwd.value=parent.pppoe_password;
	      cf.pppoe_servername.value=parent.pppoe_server;
	      cf.dual_access.value="1";
	      cf.WANAssign.value="Dynamic";
	      cf.pppoe_dnsaddr1.value=parent.static_dns1;
		    cf.pppoe_dnsaddr2.value=parent.static_dns2;
		    if(cf.pppoe_dnsaddr1.value != "" || cf.pppoe_dnsaddr2.value!= "")
			      cf.DNSAssign.value="1";
		  	else 
			  	  cf.DNSAssign.value="0";  
	      cf.pppoe_dod.value="0";
	      cf.pppoe_dual_assign.value="0";
	      cf.pppoe_ipaddr.value=""; 
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="0";
		    cf.welcome_wan_type.value="3";
			cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();	
	  }
	  else if(parent.isp_type=="9") //pptp+static
	  {
	  	  cf.pptp_username.value=parent.pptp_username;
	      cf.pptp_passwd.value=parent.pptp_password;
	      cf.pptp_serv_ip.value=parent.pptp_server_ipaddr;
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="1";
		    cf.welcome_wan_type.value="4";
		    cf.pptp_myip.value=parent.static_ip;
			  cf.pptp_mynetmask.value=parent.static_subnet;
			  cf.pptp_gateway.value=parent.static_gateway;
			  cf.pptp_conn_id.value="";
			  cf.pptp_dnsaddr1.value=parent.static_dns1;
			  cf.pptp_dnsaddr2.value=parent.static_dns2;
			  cf.pptp_dod.value="0";
			  cf.DNSAssign.value="1";
    	  cf.WANAssign.value="1";
    	  cf.STATIC_DNS.value="0";
		  cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	  }
	  else if(parent.isp_type=="11") //pptp+dynamic
	  {
	  	  cf.pptp_username.value=parent.pptp_username;
	      cf.pptp_passwd.value=parent.pptp_password;
	      cf.pptp_serv_ip.value=parent.pptp_server_ipaddr;
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="1";
		    cf.welcome_wan_type.value="4";
		    cf.pptp_dnsaddr1.value=parent.static_dns1;
		    cf.pptp_dnsaddr2.value=parent.static_dns2;
		    if(cf.pptp_dnsaddr1.value != "" || cf.pptp_dnsaddr2.value!= "")
			      cf.DNSAssign.value="1";
		    else 
			   	  cf.DNSAssign.value="0";  
    	  cf.WANAssign.value="0";
    	  cf.STATIC_DNS.value="0";
		  cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	  }
	  else if(parent.isp_type=="13")//l2tp+static
	  {
	  	  cf.l2tp_username.value=parent.l2tp_username;
	      cf.l2tp_passwd.value=parent.l2tp_password;
	      cf.l2tp_serv_ip.value=parent.l2tp_server_ipaddr;
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="1";
		    cf.welcome_wan_type.value="5";
		    cf.l2tp_myip.value=parent.static_ip;
			  cf.l2tp_mynetmask.value=parent.static_subnet;
			  cf.l2tp_gateway.value=parent.static_gateway;
			  cf.l2tp_conn_id.value="";
			  cf.l2tp_dnsaddr1.value=parent.static_dns1;
			  cf.l2tp_dnsaddr2.value=parent.static_dns2;
			  cf.l2tp_dod.value="0";
			  cf.DNSAssign.value="1";
    	  cf.WANAssign.value="1";
    	  cf.STATIC_DNS.value="0";
		  cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	  }
	  else if(parent.isp_type=="15") //l2tp+dynamic
	  {
	  	  cf.l2tp_username.value=parent.l2tp_username;
	      cf.l2tp_passwd.value=parent.l2tp_password;
	      cf.l2tp_serv_ip.value=parent.l2tp_server_ipaddr;
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="1";
		    cf.welcome_wan_type.value="5";
		    cf.l2tp_dnsaddr1.value=parent.static_dns1;
		    cf.l2tp_dnsaddr2.value=parent.static_dns2;
		    if(cf.l2tp_dnsaddr1.value != "" || cf.l2tp_dnsaddr2.value!= "")
			      cf.DNSAssign.value="1";
		    else 
			   	  cf.DNSAssign.value="0";  
    	  cf.WANAssign.value="0";
    	  cf.STATIC_DNS.value="0";
		  cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();
	  }
	  else if(parent.isp_type=="17") //pppoe
	  {   
	      cf.pppoe_username.value=parent.pppoe_username;
	      cf.pppoe_passwd.value=parent.pppoe_password;
	      cf.pppoe_servername.value=parent.pppoe_server;
	      cf.dual_access.value="0";
	      cf.WANAssign.value="Dynamic";
	      cf.DNSAssign.value="0";
	      cf.pppoe_dod.value="0";
	      cf.pppoe_dual_assign.value="0";
	      cf.pppoe_ipaddr.value=""; 
	      cf.basic_type.value="0";
		    cf.ppp_login_type.value="0";
		    cf.welcome_wan_type.value="3";
			cf.conflict_wanlan.value=parent.conflict_wanlan;
	      cf.submit();	
	  }
	  else
	  {
	  	if (cf.MACAssign[0].checked )
    		parent.mac_spoof="0";
    	else if (cf.MACAssign[1].checked )
    		parent.mac_spoof="1";
    	else if (cf.MACAssign[2].checked )
    		parent.mac_spoof="2";
    		
	  	//parent.mac_spoof=cf.MACAssign.value;
               if(parent.vlan_iptv_flag==1)
                       location.href="RU_check_vlan.htm";
               else
                       location.href="RU_finish_manual.htm";
	}

}

function RU_spoof_back()
{
	  if(parent.isp_type=="1")
	      location.href="RU_welcome.htm";
	  else if(parent.isp_type=="3")
	      location.href="RU_isp_static.htm";
	  else if(parent.isp_type=="5")
	      location.href="RU_isp_pppoe_static.htm";
	  else if(parent.isp_type=="7")
	      location.href="RU_isp_pppoe.htm";    
	  else if(parent.isp_type=="9")
	      location.href="RU_isp_pptp_static.htm";
	  else if(parent.isp_type=="11")
	      location.href="RU_isp_pptp.htm";
	  else if(parent.isp_type=="13")
	      location.href="RU_isp_l2tp_static.htm";
	  else if(parent.isp_type=="15")
	      location.href="RU_isp_l2tp.htm";
	  else if(parent.isp_type=="17")
	      location.href="RU_isp_pppoe.htm";
	
}
