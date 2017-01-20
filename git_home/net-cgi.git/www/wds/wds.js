function click_endis_wds()
{
	var cf=document.forms[0];
	var dflag;
	dflag=(!(cf.wds_enable.checked));
	
	if(top.enable_ap_flag ==1)
		setDisabled (true, cf.sysLANIPAddr1,cf.sysLANIPAddr2,cf.sysLANIPAddr3,cf.sysLANIPAddr4,cf.wds_mode[0],cf.ptp_sta,cf.ptp_mac);
	else
		setDisabled (dflag, cf.sysLANIPAddr1,cf.sysLANIPAddr2,cf.sysLANIPAddr3,cf.sysLANIPAddr4,cf.wds_mode[0],cf.ptp_sta,cf.ptp_mac);
	
	setDisabled ( dflag, cf.pmp_sta, cf.wds_mode[1], cf.pmp_sta, cf.pmp_mac1,cf.pmp_mac2,cf.pmp_mac3,cf.pmp_mac4);
	if(cf.wds_enable.checked)
		click_repeater_basic();
	else
		cf.wds_enable_an.disabled=false;
}
function click_endis_wds_a()
{
	//for a/n
	var cf=document.forms[0];
	var dflag_a;
	dflag_a=(!(cf.wds_enable_an.checked));
	
	if(top.enable_ap_flag ==1)
		setDisabled (true, cf.sysLANIPAddr1_an,cf.sysLANIPAddr2_an,cf.sysLANIPAddr3_an,cf.sysLANIPAddr4_an,cf.wds_mode_an[0],cf.ptp_sta_an,cf.ptp_mac_1_an);
	else
		setDisabled (dflag_a, cf.sysLANIPAddr1_an,cf.sysLANIPAddr2_an,cf.sysLANIPAddr3_an,cf.sysLANIPAddr4_an,cf.wds_mode_an[0],cf.ptp_sta_an,cf.ptp_mac_1_an);
	
	setDisabled ( dflag_a, cf.pmp_sta_an, cf.wds_mode_an[1], cf.pmp_sta_an, cf.pmp_mac1_1_an,cf.pmp_mac2_1_an,cf.pmp_mac3_1_an,cf.pmp_mac4_1_an);//cf.sysLANIPAddr1_an,cf.sysLANIPAddr2_an,cf.sysLANIPAddr3_an,cf.sysLANIPAddr4_an,
	if(cf.wds_enable_an.checked)
		click_repeater_basic_a();
	else
		cf.wds_enable.disabled=false;
}

function click_repeater_basic()
{
	var cf=document.forms[0];
	var aflag;
	aflag=(!(cf.wds_mode[0].checked));
	bflag=(!(cf.wds_mode[1].checked));
	
	if(top.enable_ap_flag ==1)
		setDisabled (true, cf.sysLANIPAddr1,cf.sysLANIPAddr2,cf.sysLANIPAddr3,cf.sysLANIPAddr4,cf.ptp_sta,cf.ptp_mac);
	else
		setDisabled (aflag, cf.sysLANIPAddr1,cf.sysLANIPAddr2,cf.sysLANIPAddr3,cf.sysLANIPAddr4,cf.ptp_sta,cf.ptp_mac);

	setDisabled ( bflag, cf.pmp_sta, cf.pmp_mac1, cf.pmp_mac2,cf.pmp_mac3,cf.pmp_mac4);
	
	if(cf.wds_mode[0].checked)
	{
		cf.wds_enable_an.checked = false;
		click_endis_wds_a();
		cf.wds_enable_an.disabled = true;
	}
	else
	{
		cf.wds_enable_an.disabled = false;
	}
}
function click_repeater_basic_a()
{
        //for a/n
	var cf=document.forms[0];
        var aflag_a;
        aflag_a=(!(cf.wds_mode_an[0].checked));
        bflag_a=(!(cf.wds_mode_an[1].checked));
		
	if(top.enable_ap_flag ==1)
		setDisabled (true, cf.sysLANIPAddr1_an,cf.sysLANIPAddr2_an,cf.sysLANIPAddr3_an,cf.sysLANIPAddr4_an,cf.ptp_sta_an,cf.ptp_mac_1_an);
	else
		setDisabled (aflag_a, cf.sysLANIPAddr1_an,cf.sysLANIPAddr2_an,cf.sysLANIPAddr3_an,cf.sysLANIPAddr4_an,cf.ptp_sta_an,cf.ptp_mac_1_an);

        setDisabled ( bflag_a, cf.pmp_sta_an, cf.pmp_mac1_1_an, cf.pmp_mac2_1_an,cf.pmp_mac3_1_an,cf.pmp_mac4_1_an);
	
	if(cf.wds_mode_an[0].checked)
	{
		cf.wds_enable.checked = false;
		click_endis_wds();
		cf.wds_enable.disabled = true;
	}
	else
	{
		cf.wds_enable.disabled = false;
	}
}

function check_wds (cf)
{
	var count_mac=0;
	cf.change_ip_flag.value=0;

	if( cf.wds_enable_an.checked || cf.wds_enable.checked ){
		if(!confirm("$wds_or_wps"))
			return false;
	}
	if(cf.wds_enable_an.checked)
	{
		cf.wds_endis_fun_a.value=1;
		if(cf.wds_mode_an[0].checked == true)
		{
			new_type_is_repeater="1";	
			cf.hidden_wds_mode_a.value=0;
			cf.repeater_ip_a.value=cf.sysLANIPAddr1_an.value+'.'+cf.sysLANIPAddr2_an.value+'.'+cf.sysLANIPAddr3_an.value+'.'+cf.sysLANIPAddr4_an.value;
			if( checkipaddr(cf.repeater_ip_a.value)== false || is_sub_or_broad(cf.repeater_ip_a.value, old_lanip, lan_subnet) == false)
			{
				alert("$invalid_wds_ip");
				return false;
			}
                        if(cf.ptp_sta_an.checked == true)
                                cf.wds_endis_ip_client_a.value=1;
                        else
                                cf.wds_endis_ip_client_a.value=0;
			var the_mac = cf.ptp_mac_1_an.value;
			if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
                        {
                                var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
                                the_mac = tmp_mac;
                                cf.ptp_mac_1_an.value = tmp_mac;
                        }
                        else if ( the_mac.split("-").length == 6 )
                        {
                                var tmp_mac = the_mac.replace(/-/g,":");
                                the_mac=tmp_mac;
                        }
                        if(maccheck_wds(the_mac,0,cf) == 1 )
                        {
                                alert("$invalid_mac_basic_station");
                                return false;
                        }
                        else if(maccheck_wds(the_mac,0,cf) == 2)
                        {
                                alert("$mac_basic_station_null");
                                return false;
                        }
                        else if((lan_mac_a.toLowerCase() == the_mac.toLowerCase()) || (lan_mac.toLowerCase() == the_mac.toLowerCase()) || (wan_mac.toLowerCase() == the_mac.toLowerCase()))
                        {
                                alert("$mac_basic_station_dup");
                                return false;
                        }
                        else
                                cf.basic_station_mac_a.value = the_mac;

			if ( "1" == used_dfs )
			{
				alert("$repeater_non_dfs");
			}
		}
		else if(cf.wds_mode_an[1].checked == true)
		{
			cf.hidden_wds_mode_a.value=1;		
                        if(cf.pmp_sta_an.checked == true)
                                cf.wds_endis_mac_client_a.value=1;
                        else
                                cf.wds_endis_mac_client_a.value=0;
                        the_mac1 = cf.pmp_mac1_1_an.value;
                        the_mac2 = cf.pmp_mac2_1_an.value;
                        the_mac3 = cf.pmp_mac3_1_an.value;
                        the_mac4 = cf.pmp_mac4_1_an.value;
			for( j=1;j<=4;j++)
			{
				mac_base_str=eval('the_mac'+j);
				var tmp_base_mac = mac_base_str;
				if(mac_base_str.indexOf(":")==-1&&mac_base_str.length=="12")
				{
					tmp_base_mac = mac_base_str.substr(0,2)+":"+mac_base_str.substr(2,2)+":"+mac_base_str.substr(4,2)+":"+mac_base_str.substr(6,2)+":"+mac_base_str.substr(8,2)+":"+mac_base_str.substr(10,2);
				}
					switch(j)
					{
						case 1:
							the_mac1=tmp_base_mac;
							cf.pmp_mac1_1_an.value = tmp_base_mac;
							break;
						case 2:
							the_mac2=tmp_base_mac;
							cf.pmp_mac2_1_an.value = tmp_base_mac;
							break;
						case 3:
							the_mac3=tmp_base_mac;
							cf.pmp_mac3_1_an.value = tmp_base_mac;
							break;
						case 4:
							the_mac4=tmp_base_mac;
							cf.pmp_mac4_1_an.value = tmp_base_mac;
							break;
					}
			}
			check_same_mac1 = the_mac1; 
			check_same_mac2 = the_mac2;
			check_same_mac3 = the_mac3;
			check_same_mac4 = the_mac4;
                        for( j=1;j<=4;j++ )
                        {
                                wrong_message="";
                                mac_base_str=eval('the_mac'+j);
                                if(mac_base_str.indexOf(":")==-1&&mac_base_str.length=="12")
                                {
                                        var tmp_base_mac = mac_base_str.substr(0,2)+":"+mac_base_str.substr(2,2)+":"+mac_base_str.substr(4,2)+":"+mac_base_str.substr(6,2)+":"+mac_base_str.substr(8,2)+":"+mac_base_str.substr(10,2);
                                        mac_base_str = tmp_base_mac;
                                        switch( j )
                                        {
                                                case 1:
                                                       cf.pmp_mac1_1_an.value = tmp_base_mac;
                                                        break;
                                                case 2:
							cf.pmp_mac2_1_an.value = tmp_base_mac;
	                                                break;
                                                case 3:
                                                        cf.pmp_mac3_1_an.value = tmp_base_mac;
                                                        break;
                                                case 4:
                                                        cf.pmp_mac4_1_an.value = tmp_base_mac;
                                                        break;
	                                 }
		        	}
				else if ( mac_base_str.split("-").length == 6 )
				{
					var tmp_base_mac = mac_base_str.replace(/-/g,":");
					mac_base_str=tmp_base_mac;
				}
				switch( maccheck_wds(mac_base_str,j,cf) )
				{
					case 0:
						eval('cf.repeater_mac'+j+'_a.value=mac_base_str');
						break;
				        case 1:
					 	wrong_message=invalid_repeater_mac(j);
						break;
					case 2:
						count_mac++;
						break;
					case 3:
						wrong_message="$invalid_mac_same";
						break;
					default:
						wrong_message=invalid_repeater_mac(j);
						break;
				}
				if( wrong_message!="" )
				{
					alert(wrong_message);
					return false;
				}
				if((lan_mac_a.toLowerCase() == mac_base_str.toLowerCase()) || (lan_mac.toLowerCase() == mac_base_str.toLowerCase()) || (wan_mac.toLowerCase() == mac_base_str.toLowerCase()))
				{
					alert("$mac_basic_repeater_dup");
					return false;
				}
			}
			if(count_mac==4)
			{
				alert("$all_mac_null");
				return false;
			}
		}
	}
	else
	{
		cf.wds_endis_fun_a.value = 0;
	}//end for a/n
	// for b/g/n
	count_mac=0;
	if(cf.wds_enable.checked)
	{
		cf.wds_endis_fun.value=1;
		if(cf.wds_mode[0].checked == true)
		{
			new_type_is_repeater="1";
			cf.hidden_wds_mode.value=0;
			cf.repeater_ip.value=cf.sysLANIPAddr1.value+'.'+cf.sysLANIPAddr2.value+'.'+cf.sysLANIPAddr3.value+'.'+cf.sysLANIPAddr4.value;
			if( checkipaddr(cf.repeater_ip.value)== false || is_sub_or_broad(cf.repeater_ip.value, old_lanip, lan_subnet) == false)
			{	
				alert("$invalid_wds_ip");
				return false;
			}
			if (old_wds_endis_fun == 1 && old_wds_repeater_basic == 0)
			{
				if(isSameIp(old_repeater_ip,cf.repeater_ip.value) == false)
					cf.change_ip_flag.value=1;
				else
					cf.change_ip_flag.value=0;
			}	
			else
			{
				if(isSameIp(old_lanip,cf.repeater_ip.value) == false)
					cf.change_ip_flag.value=1;
				else
					cf.change_ip_flag.value=0;
			}
			if(cf.ptp_sta.checked == true)
				cf.wds_endis_ip_client.value=1;
			else
				cf.wds_endis_ip_client.value=0;
			var the_mac = cf.ptp_mac.value;
			if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
			{
				var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
				the_mac = tmp_mac;
				cf.ptp_mac.value = tmp_mac;
			}	
			else if ( the_mac.split("-").length == 6 )
			{
				var tmp_mac = the_mac.replace(/-/g,":");
				the_mac=tmp_mac;
			}				
			if(maccheck_wds(the_mac,0,cf) == 1 )
			{
				alert("$invalid_mac_basic_station");
				return false;
			}
			else if(maccheck_wds(the_mac,0,cf) == 2)
			{
				alert("$mac_basic_station_null");
				return false;
			}
			else if((lan_mac.toLowerCase() == the_mac.toLowerCase()) || (lan_mac_a.toLowerCase() == the_mac.toLowerCase()) || (wan_mac.toLowerCase() == the_mac.toLowerCase()))
			{
				alert("$mac_basic_station_dup");
				return false;
			}
			else
				cf.basic_station_mac.value = the_mac;
			if(cf.repeater_ip.value != cf.repeater_ip_a.value && cf.wds_enable_an.checked)
			{
				alert("$repeater_ip_should_same");
				return false;
			}
		}
		else if(cf.wds_mode[1].checked == true)
		{
			cf.hidden_wds_mode.value=1;
			if(cf.pmp_sta.checked == true)
				cf.wds_endis_mac_client.value=1;
			else
				cf.wds_endis_mac_client.value=0;
			the_mac1 = cf.pmp_mac1.value;
			the_mac2 = cf.pmp_mac2.value;
			the_mac3 = cf.pmp_mac3.value;
			the_mac4 = cf.pmp_mac4.value;
                        for( j=1;j<=4;j++)
                        {
                                mac_base_str=eval('the_mac'+j);
				var tmp_base_mac = mac_base_str;
                                if(mac_base_str.indexOf(":")==-1&&mac_base_str.length=="12")
                                {
                                        tmp_base_mac = mac_base_str.substr(0,2)+":"+mac_base_str.substr(2,2)+":"+mac_base_str.substr(4,2)+":"+mac_base_str.substr(6,2)+":"+mac_base_str.substr(8,2)+":"+mac_base_str.substr(10,2);
				}
                                        switch(j)
                                        {
                                                case 1:
                                                        the_mac1=tmp_base_mac;
							cf.pmp_mac1.value = tmp_base_mac;
							break;
                                                case 2:
                                                        the_mac2=tmp_base_mac;
							cf.pmp_mac2.value = tmp_base_mac;
							break;
                                                case 3:
		                                        the_mac3=tmp_base_mac;
							cf.pmp_mac3.value = tmp_base_mac;
							break;
                                                case 4:
                                                        the_mac4=tmp_base_mac;
							cf.pmp_mac4.value = tmp_base_mac;
							break;
					}
			}
			check_same_mac5 = the_mac1;
			check_same_mac6 = the_mac2;
			check_same_mac7 = the_mac3;
			check_same_mac8 = the_mac4;
			for( j=1;j<=4;j++ )
			{
				wrong_message="";
				mac_base_str=eval('the_mac'+j);
				if(mac_base_str.indexOf(":")==-1&&mac_base_str.length=="12")
				{
					var tmp_base_mac = mac_base_str.substr(0,2)+":"+mac_base_str.substr(2,2)+":"+mac_base_str.substr(4,2)+":"+mac_base_str.substr(6,2)+":"+mac_base_str.substr(8,2)+":"+mac_base_str.substr(10,2);
					mac_base_str = tmp_base_mac;
                                        switch( j )
                                        {
  	 	                                case 1:
             		                                cf.pmp_mac1.value = tmp_base_mac;
                                                        break;
                                                case 2:
                                                        cf.pmp_mac2.value = tmp_base_mac;
                                                        break;
                                                case 3:
                                                        cf.pmp_mac3.value = tmp_base_mac;
                                                        break;
                                                case 4:
                                                        cf.pmp_mac4.value = tmp_base_mac;
                                                        break;
                                        }
				}
				else if ( mac_base_str.split("-").length == 6 )
				{
					var tmp_base_mac = mac_base_str.replace(/-/g,":");
					mac_base_str=tmp_base_mac;
				}				
				switch( maccheck_wds(mac_base_str,j,cf) )
				{
					case 0:
						eval('cf.repeater_mac'+j+'.value=mac_base_str');
						break;
					case 1:
						wrong_message=invalid_repeater_mac(j);
						break;
					case 2:
						count_mac++;
						break
					case 3:
						wrong_message="$invalid_mac_same";
						break
					default:
						wrong_message=invalid_repeater_mac(j);
						break;
				}
				if( wrong_message!="" )
				{
					alert(wrong_message);
					return false;
				}
                                if((lan_mac.toLowerCase() == mac_base_str.toLowerCase()) ||(lan_mac_a.toLowerCase() == mac_base_str.toLowerCase()) || (wan_mac.toLowerCase() == mac_base_str.toLowerCase()))
                                {
                                        alert("$mac_basic_repeater_dup");
                                        return false;
                                }
			}
			if(count_mac==4)
			{
				alert("$all_mac_null");
				return false;
			}
		}
		/*if (cf.wds_enable_a.checked && ((cf.wds_mode[0].checked && cf.wds_mode_a[1].checked) || (cf.wds_mode[1].checked && cf.wds_mode_a[0].checked)))
		{
			alert("$mode_should_same");
			return false;
		}*/

		if(cf.wds_enable_an.checked && (cf.wds_mode[1].checked == true && cf.wds_mode_an[1].checked == true))
		{
			for(i=1;i<=8;i++)
			{
				var mac=eval('check_same_mac'+i);
				for(j=i+1;j<=8;j++)
				{
					var mac2=eval('check_same_mac'+j);
					if(mac!="" && mac2!="")
					{
						if(mac.toLowerCase()==mac2.toLowerCase())
						{
							alert("$invalid_mac_same");
							return false;
						}
					}
				}
			}
		}

		top.enable_action=0;

	}
	else
	{
		cf.wds_endis_fun.value=0;
		if(old_wds_endis_fun==0)
			cf.change_ip_flag.value=0;
		else
			cf.change_ip_flag.value=1;

		top.enable_action=0;

		if(cf.wds_enable_an.checked && cf.wds_mode_an[0].checked)
			cf.repeater_ip.value = cf.repeater_ip_a.value;
	}

	if( old_type_is_repeater == "0" && new_type_is_repeater == "1" )
		cf.rep_old_lan_ipaddr.value = old_lanip;  // lan ip 
	else
		cf.rep_old_lan_ipaddr.value = rep_old_lanip; //old lan ip
	if( old_type_is_repeater == "1" && new_type_is_repeater == "0" )
		cf.change_to_old_ip.value = "1";
	else
		cf.change_to_old_ip.value = "0";
	
	return true;
}

function wdsWaring()
{
	var cf=document.forms[0];
	if(cf.wds_enable.checked)
	{
		if ( wl_channel == 0 )
		{
			alert("$wds_auto_channel");
			location.href="WLG_wireless.htm";
		}
		if( pr_wds_support_wpa != 1 )
		{
			if(security_mode=="3" ||  security_mode=="4" || security_mode == "5")
			{
				alert("$wds_not_wpa");
				location.href="WLG_wireless.htm";
			}
		}

		if( security_mode == "6" )
		{
			alert("$wds_not_Enterprise");
			location.href="WLG_wireless.htm";
		}
	}
	if(cf.wds_enable_an.checked)
	{
		if (wla_channel == 0)
		{
			alert("$wds_auto_channel");
			location.href="WLG_wireless.htm";
		}
		if( pr_wds_support_wpa !=1 )
		{
			if (security_mode_a == "3" || security_mode_a == "4" || security_mode_a == "5")
			{
				alert("$wds_not_wpa");
				location.href="WLG_wireless.htm";
			}
		}

		if( security_mode_a == "6")
		{
			alert("$wds_not_Enterprise");
			location.href="WLG_wireless.htm";
		}
	}
}
function invalid_repeater_mac(num)
{
	var message;
	switch( num )
	{
		case 1:
			message="$invalid_mac1 !";
			break;
		case 2:
			message="$invalid_mac2 !";
			break;
		case 3:
			message="$invalid_mac3 !";
			break;
		case 4:
			message="$invalid_mac4 !";
			break
		default:
			message="$invalid_mac1 !";	
	}
	return message;
}
