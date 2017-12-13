function dhcponoff()
{
	var cf=document.forms[0];
	if(cf.dhcp_server.checked ==true)
	{
		cf.sysPoolStartingAddr4.disabled =false;
		cf.sysPoolFinishAddr4.disabled =false;
	}
	else
	{
		cf.sysPoolStartingAddr4.disabled =true;
		cf.sysPoolFinishAddr4.disabled =true;
	}
}

function valid_add()
{   
	if(array_num==64)  
	{
		alert("$reservation_length_64");
		return false;
	}

	location.href="reservation_add_wait.htm";
}

function lanip_change(cf)
{
	var addr_array = new Array();
	cf.lan_ipaddr.value=cf.sysLANIPAddr1.value+'.'+cf.sysLANIPAddr2.value+'.'+cf.sysLANIPAddr3.value+'.'+cf.sysLANIPAddr4.value;
	addr_array = cf.lan_ipaddr.value.split('.');
	cf.sysPoolStartingAddr1.value = cf.sysPoolFinishAddr1.value=addr_array[0];
	cf.sysPoolStartingAddr2.value = cf.sysPoolFinishAddr2.value= addr_array[1];
	cf.sysPoolStartingAddr3.value = cf.sysPoolFinishAddr3.value=addr_array[2];	
}

function check_clientNumber(cf)
{
	cf.lan_subnet.value=cf.sysLANSubnetMask1.value+'.'+cf.sysLANSubnetMask2.value+'.'+cf.sysLANSubnetMask3.value+'.'+cf.sysLANSubnetMask4.value;
	var lan_mask=cf.lan_subnet.value;
	cf.lan_ipaddr.value=cf.sysLANIPAddr1.value+'.'+cf.sysLANIPAddr2.value+'.'+cf.sysLANIPAddr3.value+'.'+cf.sysLANIPAddr4.value;
	var lan_ip=cf.lan_ipaddr.value;
	var mask_array=lan_mask.split('.');
	var lan_array=lan_ip.split('.');
	var netmask = parseInt(mask_array[3], 10);
	var net_number = 256 / (256 - netmask);
	var client = 255-netmask;
	var localip = parseInt(lan_array[3],10);
	var net_start = (netmask & localip) + 1;
	var net_end = (net_start + client) - 1;
	if (localip == (net_start-1)) {
		cf.lan_ipaddr.focus();
		return false;
	}
	if (localip >= (net_end)) {
		cf.lan_ipaddr.focus();
		return false;
	}
	if ((parseInt(cf.sysPoolStartingAddr4.value,10) < net_start) || (parseInt(cf.sysPoolStartingAddr4.value,10) >= net_end)) {
		cf.sysPoolStartingAddr4.focus();
		return false;
	}
	if ((parseInt(cf.sysPoolFinishAddr4.value,10) < net_start) || (parseInt(cf.sysPoolFinishAddr4.value,10) >= net_end)) {
		cf.sysPoolFinishAddr4.focus();
		return false;
	}
	return true;
}

function checklan(form)
{
	var cf=document.forms[0];
	var change_count = 0;
	form.change_network_flag.value=0;
	form.change_network2_flag.value=0;
	form.change_ip_flag.value=0;
	form.dmz_ip.value=dmz_ip;
	form.bs_trustedip.value=bs_trustedip;	
	form.dhcp_start.value=form.sysPoolStartingAddr1.value+'.'+form.sysPoolStartingAddr2.value+'.'+form.sysPoolStartingAddr3.value+'.'+form.sysPoolStartingAddr4.value;
	form.dhcp_end.value=form.sysPoolFinishAddr1.value+'.'+form.sysPoolFinishAddr2.value+'.'+form.sysPoolFinishAddr3.value+'.'+form.sysPoolFinishAddr4.value;
	form.lan_ipaddr.value=form.sysLANIPAddr1.value+'.'+form.sysLANIPAddr2.value+'.'+form.sysLANIPAddr3.value+'.'+form.sysLANIPAddr4.value;
	form.lan_subnet.value=form.sysLANSubnetMask1.value+'.'+form.sysLANSubnetMask2.value+'.'+form.sysLANSubnetMask3.value+'.'+form.sysLANSubnetMask4.value;
	var lan_array=form.lan_ipaddr.value.split('.');

	form.hid_array_num.value=array_num;
/*	
	if(cf.device_name.value=="")
	{
		alert("$device_name_null");
		return false;
	}
	for(i=0;i<cf.device_name.value.length;i++)
	{
		if(isValidChar_space(cf.device_name.value.charCodeAt(i))==false)
		{
			alert("$device_name_error");
			return false;
		}
	}*/
	if (checkipaddr(form.lan_ipaddr.value)== false || is_sub_or_broad(form.lan_ipaddr.value,form.lan_ipaddr.value, form.lan_subnet.value) == false)
	{	
		alert("$invalid_ip");
		return false;
	}

        if(dmz_ip!="")
        {
                var dmz_array=dmz_ip.split('.');
                if( isSameIp(form.lan_ipaddr.value, dmz_ip) )
		{
			alert("$invalid_ip");
			return false;
		}
			
        }
	
	if( checksubnet(form.lan_subnet.value, 0) == false )
	{
		alert("$invalid_mask");
		return false;
	}
/*
	if( isSameSubNet(form.cfTrusted_IPAddress.value, form.lan_subnet.value, form.lan_ipaddr.value, form.lan_subnet.value) == false)
	{
		form.Trusted_IP_Enable.value = "0";
		form.cfTrusted_IPAddress.value = "";	
	}
	if( isSameSubNet(form.dmz_ipaddr.value, form.lan_subnet.value, form.lan_ipaddr.value, form.lan_subnet.value) == false)
	{
		form.dmz_value.value = "0";
		form.dmz_ipaddr.value = "";
	}
*/
	if (endis_wl_radio == "1" && wds_endis_fun == "1" && wds_repeater_basic == "0")
	{}
	else if(form.dhcp_server.checked == true)
	{
		if( checkipaddr(form.dhcp_start.value)== false )
		{
			alert("$invalid_dhcp_startip");
			return false;
		}
		form.dhcp_start.value = address_parseInt(form.dhcp_start.value);
		if( checkipaddr(form.dhcp_end.value)== false )
		{
			alert("$invalid_dhcp_endip");
			return false;	
		}
		form.dhcp_end.value = address_parseInt(form.dhcp_end.value);
		if(parseInt(form.sysPoolStartingAddr4.value,10)>parseInt(form.sysPoolFinishAddr4.value,10))
		{
			alert("$invalid_dhcp_startendip");
			return false;
		}

		/*if(lan_array[3]==form.sysPoolStartingAddr4.value || lan_array[3]==form.sysPoolFinishAddr4.value)
		{
			alert("$invalid_dhcp_startendip");
			return false;
		}*/
		if(!check_clientNumber(form))
		{	
			alert("$invalid_dhcp_startendip");
			return false;
		}
		form.lan_ipaddr.value = address_parseInt(form.lan_ipaddr.value);
		form.lan_subnet.value = address_parseInt(form.lan_subnet.value);
		if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,form.dhcp_start.value,form.lan_subnet.value) == false)
		{
			alert("$same_subnet_ip_dhcpstart");
			return false;
		}
		if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,form.dhcp_end.value,form.lan_subnet.value) == false)
		{
			alert("$same_subnet_ip_dhcpend");
			return false
		}
/*
		if(parseInt(form.sysPoolStartingAddr4.value) <= parseInt(lan_array[3]) && parseInt(lan_array[3]) <= parseInt(form.sysPoolFinishAddr4.value))
		{
			alert("$ip_dhcp_rang");
			return false;
		}
*/
		var out_of_range="";
		if (array_num > 0)
		{
			for(i=1;i<=array_num;i++)
			{
				var start_array = form.sysPoolStartingAddr4.value;
				var end_array = form.sysPoolFinishAddr4.value; 

				var str = eval ( 'resevArray' + i );
				var each_info=str.split(' ');
				var each4=each_info[0].split('.');
				if((parseInt(each4[3]) < parseInt(start_array)) || (parseInt(each4[3]) > parseInt(end_array)))
				{
					if(out_of_range == "")
						out_of_range = i;
					else
						out_of_range += " " + i;
				}
			}
		}
		form.out_of_range.value=out_of_range;
	}
	if( form.dhcp_server.checked == true )
		form.dhcp_mode.value = 1;
	else
		form.dhcp_mode.value = 0;

	var conflicted_with_wanip="$conflict_with_wanip";
	if( wan_ip!="0.0.0.0" )
	{
		if( wan_type == "pppoe" || wan_type == "pptp" || wan_type == "mulpppoe1" )
		{
			if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,wan_ip,form.lan_subnet.value) == true)
			{
				alert("$conflict_with_wanip");
				return false;
			}
		}
		else
		{
			if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,wan_ip,wan_mask) == true)
			{
				alert("$conflict_with_wanip");
				return false;
			}
			if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,wan_ip,form.lan_subnet.value) == true)
			{
				alert("$conflict_with_wanip");
				return false;
			}
			if(isSameSubNet(form.lan_ipaddr.value,wan_mask,wan_ip,wan_mask) == true)
			{
				alert("$conflict_with_wanip");
				return false;
			}

		}
		if(isSameIp(wan_ip,form.lan_ipaddr.value) == true)
		{
			alert("$conflict_with_wanip");
			return false;
		}
	}
	if(bas_pptp_ip != "0.0.0.0" )
	{
                if(wan_type == "pptp")
                {
                    	if(isSameSubNet(form.lan_ipaddr.value,form.lan_subnet.value,bas_pptp_ip,form.lan_subnet.value) == true )
                      	{
                           	alert("$conflicted_with_wanip");
                                return false;
                        }
                        if(isSameIp(bas_pptp_ip,form.lan_ipaddr.value) == true)
                        {
                                alert("$conflicted_with_wanip");
                                return false;
                        }
               	}
	}

	/* when change subnet or change IP pool it shuldn't change the value of change_ip_flag */
	if( isSameIp(old_lanmask ,form.lan_subnet.value)== false || isSameIp(form.dhcp_start_old.value,form.dhcp_start.value)== false || isSameIp(form.dhcp_end_old.value,form.dhcp_end.value)== false )	//30916 add mask, start ip, end ip check
		change_count++;

	if(isSameIp(old_lanip,form.lan_ipaddr.value)== false)
	{
/*
		var askstr="$changelanip"+form.lan_ipaddr.value+" ?";
		if(!confirm(askstr))
			return false;
		alert("$changelanip_renew");	
*/
		change_count++;

		if(isSameSubNet(form.lan_ipaddr.value,"255.255.255.0",old_lanip,"255.255.255.0")==false)//To fix bug 32125
		{		
			//lan subnet
			new_lanip_array=form.lan_ipaddr.value.split('.');
			new_lansubnet_array=form.lan_subnet.value.split('.');
			addr1=  new_lanip_array[0] & new_lansubnet_array[0];
			addr2=  new_lanip_array[1] & new_lansubnet_array[1];
			addr3=  new_lanip_array[2] & new_lansubnet_array[2];
			addr4=  new_lanip_array[3] & new_lansubnet_array[3];
			var route_dest = addr1+'.'+addr2+'.'+addr3+'.'+addr4;

			var sub_leng=3;
			form.net_leng.value=3;
			form.network.value=new_lanip_array[0]+'.'+new_lanip_array[1]+'.'+new_lanip_array[2];
			var new_reoute_dest_str=new_lanip_array[0]+'.'+new_lanip_array[1]+'.'+new_lanip_array[2]+'.';

		/*To fix bug 32125
			var old_mask_array = old_lanmask.split('.');
			if(old_mask_array[1] != "0" && old_mask_array[2] == "0" && old_mask_array[3] == "0")
			{
				sub_leng=2;
				form.net_leng.value=2;
				form.network.value=addr1+'.'+addr2;
				var new_reoute_dest_str=addr1+'.'+addr2+'.';
			}
			else if(old_mask_array[0] != "0" && old_mask_array[1] == "0" && old_mask_array[2] == "0" && old_mask_array[3] == "0")
			{
				sub_leng=1;
				form.net_leng.value=1;
				form.network.value=addr1;
				var new_reoute_dest_str=addr1+'.';
			}
			else
			{
				sub_leng=3;
				form.net_leng.value=3;
				form.network.value=addr1+'.'+addr2+'.'+addr3;
				var new_reoute_dest_str=addr1+'.'+addr2+'.'+addr3+'.';
			}*/

			//dmz
			if(dmz_ip!="")
			{
				var dmz_array=dmz_ip.split('.');
				if( sub_leng == 1 )
					form.dmz_ip.value=new_reoute_dest_str+dmz_array[1]+'.'+dmz_array[2]+'.'+dmz_array[3];
				else if( sub_leng == 2 )
					form.dmz_ip.value=new_reoute_dest_str+dmz_array[2]+'.'+dmz_array[3];
				else
					form.dmz_ip.value=new_reoute_dest_str+dmz_array[3];
			}

			//block sites
			if(bs_trustedip!="")
			{
				var bs_trustedip_array=bs_trustedip.split('.');
				if( sub_leng == 1 )
					form.bs_trustedip.value=new_reoute_dest_str+bs_trustedip_array[1]+'.'+bs_trustedip_array[2]+'.'+bs_trustedip_array[3];
				else if( sub_leng == 2 )
					form.bs_trustedip.value=new_reoute_dest_str+bs_trustedip_array[2]+'.'+bs_trustedip_array[3];
				else
					form.bs_trustedip.value=new_reoute_dest_str+bs_trustedip_array[3];
			}
			form.change_network_flag.value=1;
		}
		form.change_ip_flag.value=1;
		top_left_nolink();
		top.enable_action=0;
	}
	//To fix bug 32125.Either lan ip is changed or not ,it should check whether subnet is changed
	if(isSameIp(old_lanmask,form.lan_subnet.value)==false)
	{
		var oldNetwork = old_lanmask.split(".");
		var nowNetwork = form.lan_subnet.value.split(".");
		oldNumofNetwork = getNumOfNetwork(oldNetwork);
		newNumofNetwork = getNumOfNetwork(nowNetwork);
		if ( oldNumofNetwork < newNumofNetwork ) //If subnet is changed and the subnet range is from big to small, router SHOULD flush all the corresponding configuration.
		{
			form.change_network2_flag.value=1;
		}
	}
	//form.action="/apply.cgi?/lan.asp";
	//form.action="/apply.cgi?/welcomeok.htm timestamp="+ts;

	if(form.dhcp_start_old.value == form.dhcp_start.value && form.dhcp_end_old.value == form.dhcp_end.value)
		form.dhcp_pool_tag.value = 0;
	else
		form.dhcp_pool_tag.value = 1;
	if( change_count > 0)
		alert("$change_ip_manually1");
	cf.submit();//add
	return true;
}
function getNumOfNetwork(netArray)
{
	var getNum=0;
	getNum=netArray[0]*255*255*255;
	getNum+=netArray[1]*255*255;
	getNum+=netArray[2]*255;
	getNum+=netArray[3];
	return getNum;
}
