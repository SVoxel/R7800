/*
 *	var index = cf.WRegion.selectedIndex;
	index=parseInt(index)+1;
 *	this is for one more region, select.
*/
//for bug 34916: region list index, if region list change, this list need change too
var Africa=1;
var Asia=2;
var Australia=3;
var Canada=-1;
var China=-1;
var Europe=4;
var India=-1;
var Israel=5;
var Japan=6;
var Korea=7;
var Malaysia=-1;
var Mexico=9;
var Middle_East_Algeria_Syria_Yemen=-1;
var Middle_East_Iran_Lebanon_Qatar=-1;
var Middle_East_Turkey_Egypt_Tunisia_Kuwait=-1;
var Middle_East_Saudi_Arabia=-1;
var Middle_East_United_Arab_Emirates=-1;
var Middle_East=10;
var Russia=11;
var Singapore=-1;
var South_America=12;
var Taiwan=-1;
var United_States=8;

function getObj(name)
{
       if (document.getElementById)
       {
               return document.getElementById(name);
       }
       else if (document.all)
       {
               return document.all[name];
       }
       else if (document.layers)
       {
               return document.layers[name];
       }
}

function changekeylen(form)
{
        if(form.wepenc.options[0].selected == true)
        {
                form.KEY1.size=form.KEY2.size=form.KEY3.size=form.KEY4.size=18;
                form.KEY1.value = wep_64_key1;
                form.KEY2.value = wep_64_key2;
                form.KEY3.value = wep_64_key3;
                form.KEY4.value = wep_64_key4;
        }
        else
        {
                form.KEY1.size=form.KEY2.size=form.KEY3.size=form.KEY4.size=34;
                form.KEY1.value = wep_128_key1;
                form.KEY2.value = wep_128_key2;
                form.KEY3.value = wep_128_key3;
                form.KEY4.value = wep_128_key4;
        }
        form.generate_flag.value=0;
}

function opmode_disabled()
{
        document.getElementById("opmode_all").style.display="none";
        document.getElementById("opmode_54").style.display="";
}

function opmode_abled()
{
        document.getElementById("opmode_all").style.display="";
        document.getElementById("opmode_54").style.display="none";
}

function setSecurity_func(num)
{
        var form=document.forms[0];
        form.wpa1_press_flag.value=0;
        form.wpa2_press_flag.value=0;
        form.wpas_press_flag.value=0;
        if(num==2)
        {
                getObj("view").innerHTML=str_wep;

                /* to fix bug 25282:The length of the wep key is changed shorter when you click it.*/
                changekeylen(form);

                var keyno=wl_get_keyno;
                var keylength=wl_get_keylength;
                form.wep_key_no[eval(keyno)-1].checked = true;
                form.KEY1.value=wl_key1;
                form.KEY2.value=wl_key2;
                form.KEY3.value=wl_key3;
                form.KEY4.value=wl_key4;
                form.old_length.value=keylength;
        }
        else if(num==3)
                getObj("view").innerHTML=str_wpa;
        else if(num==4)
                getObj("view").innerHTML=str_wpa2;
        else if(num==5)
                getObj("view").innerHTML=str_wpas;
        else if (num==6)
        {
                getObj("view").innerHTML=str_wpae;
                form.wpae_mode.value = get_wpae_mode;
                if( get_radiusSerIp != "" && get_radiusSerIp != "0.0.0.0" )
                {
                        radiusIPArray = get_radiusSerIp.split(".");
                        form.radiusIPAddr1.value = radiusIPArray[0];
                        form.radiusIPAddr2.value = radiusIPArray[1];
                        form.radiusIPAddr3.value = radiusIPArray[2];
                        form.radiusIPAddr4.value = radiusIPArray[3];
                }
                form.textWpaeRadiusPort.value = get_radiusPort;
        }
        else
                getObj("view").innerHTML=str_none;
}

function wl_sectype_change()
{
        var form=document.forms[0];
        if(form.opmode.options[0].selected == true)
                document.getElementById("wep_54").style.display="";
        else
                document.getElementById("wep_54").style.display="none";
}

function wl_54_sectype_change()
{
        var form=document.forms[0];
        document.getElementById("wep_54").style.display="";
        document.getElementById("wpa_psk_54").style.display="";
}

function wpaemode()
{
        var form=document.forms[0];
        document.getElementById("opmode_all").style.display="";
        document.getElementById("opmode_54").style.display="none";
        wl_sectype_change();
}

function chgCh(from)
{   
	if (from == 2)
	{
		var cf = document.forms[0];
		setChannel();
	}
	else
	{
		setwlan_mode();
		setChannel();
	}
}

function setwlan_mode()
{
	var cf = document.forms[0];
	var index = cf.WRegion.selectedIndex;
	index=parseInt(index)+1;
	var currentMode = cf.opmode.selectedIndex;

	if (index == Africa || index == Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Middle_East_Saudi_Arabia) {
		cf.opmode.options.length = 2;
		cf.opmode.options[0].text = wlan_mode_1;
		cf.opmode.options[1].text = wlan_mode_2;
		cf.opmode.options[0].value = "1";
		cf.opmode.options[1].value = "2";
		if (currentMode <= 1)
			cf.opmode.selectedIndex = currentMode;
		else
			cf.opmode.selectedIndex = 1;
	} else {
		cf.opmode.options.length = 3;
		cf.opmode.options[0].text = wlan_mode_1;
		cf.opmode.options[1].text = wlan_mode_2;
		cf.opmode.options[2].text = wlan_mode_3;
		cf.opmode.options[0].value = "1";
		cf.opmode.options[1].value = "2";
		cf.opmode.options[2].value = "3";
		cf.opmode.selectedIndex = currentMode;
	}
	return;
}

function setChannel()
{
	var cf = document.forms[0];
	var index = cf.WRegion.selectedIndex;
	index=parseInt(index)+1;
	var chIndex = cf.w_channel.selectedIndex;
	var currentMode = cf.opmode.selectedIndex;
	var endChannel;

/* change it like before produce 20000, just 11 and 13
 *
	if ( currentMode == 2 && (index == 4 || index == 9 || index == 11 ))
		endChannel = 7;	
	else if ( currentMode == 2 )
		endChannel = 9;
	else
*/	
  
		endChannel = FinishChannel[index];
	if (FinishChannel[index]==14 && cf.opmode.selectedIndex!=0)
		cf.w_channel.options.length = endChannel - StartChannel[index];
	else
		cf.w_channel.options.length = endChannel - StartChannel[index] + 2;

	cf.w_channel.options[0].text = "$auto_mark";
	cf.w_channel.options[0].value = 0;

	for (var i = StartChannel[index]; i <= endChannel; i++) {
		if (i==14 && cf.opmode.selectedIndex!=0)
			continue;
		cf.w_channel.options[i - StartChannel[index] + 1].value = i;
		cf.w_channel.options[i - StartChannel[index] + 1].text = (i < 10)? "0" + i : i;
	}
	cf.w_channel.selectedIndex = ((chIndex > -1) && (chIndex < cf.w_channel.options.length)) ? chIndex : 0 ;
}

function check_wlan()
{
	var cf=document.forms[0];
	/* bug 41791var ssid_bgn = document.forms[0].ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var ssid_bgn = document.forms[0].ssid.value;
	//var space_flag=0;
	var haven_wpe=0;

	/*bug 41791var wlg1_ssid=document.forms[0].wlg1ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var wlg1_ssid=document.forms[0].wlg1ssid.value;
/*	if( wps_progress_status == "2" || wps_progress_status == "3" || wps_progress_status == "start" )
	{
		alert("$wps_in_progress");
		return false;
	}*/

	if(ssid_bgn == "")
	{
		alert("$ssid_null");
		return false;
	}
	
	if(ssid_bgn == wlg1_ssid)
	{
		alert("$ssid_not_allowed_same");
		return false;
	}
	
	for(i=0;i<ssid_bgn.length;i++)
	{
		if(isValidChar_space(ssid_bgn.charCodeAt(i))==false)
		{
			alert("$ssid_not_allowed");
			return false;
		}
	}

	cf.wl_ssid.value = ssid_bgn;
	
	//16400
	if(cf.ssid_bc.checked == true)
		cf.wl_enable_ssid_broadcast.value="1";
	else
		cf.wl_enable_ssid_broadcast.value="0";

	cf.wl_apply_flag.value = "1";//bug 30924,if click the 'Apply' wl_apply_flag is '1',otherwise is '0'
	cf.wl_WRegion.value = cf.WRegion.value;
	if ( wds_endis_fun == 1 )
	{
		if ( cf.w_channel.selectedIndex == 0 )
		{
			alert("$wds_auto_channel");
			return false;
		}
	}
	cf.wl_hidden_wlan_channel.value = cf.w_channel.value;

	if(cf.security_type[1].checked == true)
	{
		if( checkwep(cf)== false)
			return false;
		cf.wl_hidden_sec_type.value=2;
	}
	else if(cf.security_type[2].checked == true)
	{
		if( checkpsk(cf.passphrase, cf.wl_sec_wpaphrase_len)== false)
			return false;
		cf.wl_hidden_sec_type.value=3;
		//bug 41791cf.wl_hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.wl_hidden_wpa_psk.value = cf.passphrase.value;
	}
	else if(cf.security_type[3].checked == true)
	{
		if( checkpsk(cf.passphrase, cf.wl_sec_wpaphrase_len)== false)
			return false;
		cf.wl_hidden_sec_type.value=4;
		//bug 41791cf.wl_hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.wl_hidden_wpa_psk.value = cf.passphrase.value;
	}	
	else if(cf.security_type[4].checked == true)
	{
		if( checkpsk(cf.passphrase, cf.wl_sec_wpaphrase_len)== false)
			return false;
		cf.wl_hidden_sec_type.value=5;
		//bug 41791cf.wl_hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.wl_hidden_wpa_psk.value = cf.passphrase.value;
	}	
	else if(cf.security_type[5].checked == true)
	{
		radiusServerIP = cf.radiusIPAddr1.value+'.'+ cf.radiusIPAddr2.value + '.' + cf.radiusIPAddr3.value + '.' + cf.radiusIPAddr4.value;
		if( radiusServerIP == "" || checkipaddr(radiusServerIP) == false )
		{
			alert("$invalid_ip");
			return false;
		}
		if( isSameIp(lanIP, radiusServerIP) == true )
		{
			alert("$invalid_ip");
			return false;
		}
		if( isSameIp(wanIP, radiusServerIP) == true )
		{
			alert("$conflicted_with_wanip");
			return false;
		}	
		cf.radiusServerIP.value = radiusServerIP;
		
		radiusPort=parseInt(cf.textWpaeRadiusPort.value,10);
		if( isNaN(radiusPort) || radiusPort < 1 || radiusPort > 65535 )
		{
			alert("$radiusPort65535");
			return false;
		}
		cf.textWpaeRadiusPort.value=radiusPort;
		if( cf.textWpaeRadiusSecret.value == "" )
		{
			alert("$radiusSecret128");
			return false;
		}
		if( cf.textWpaeRadiusSecret.length > 128 )
		{
			alert("$radiusSecret128");
			return false;
		}
		for(i=0;i<cf.textWpaeRadiusSecret.value.length;i++)
		{
		    if(isValidChar(cf.textWpaeRadiusSecret.value.charCodeAt(i))==false)
		    {
		        alert("$radiusSecret128");
				cf.textWpaeRadiusSecret.focus();
				return false;
			}
		}
		//bug 41791cf.hidden_WpaeRadiusSecret.value = cf.textWpaeRadiusSecret.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
		cf.hidden_WpaeRadiusSecret.value = cf.textWpaeRadiusSecret.value;
		cf.wl_hidden_sec_type.value=6;
        cf.textWpaeRadiusPort.value=port_range_interception(cf.textWpaeRadiusPort.value);		
	}	
	else
		cf.wl_hidden_sec_type.value=1;

	//When user selects WPA-PSK(TKIP)+150Mbps and WPA-PSK(TKIP)+300Mbps, set wl_simple_mode=1,Bug No.19591
	//or select "WPA-PSK [TKIP] + WPA2-PSK [AES]"+150Mbps and "WPA-PSK [TKIP] + WPA2-PSK [AES]"+300Mbps 
	if((cf.opmode.value=="2") || (cf.opmode.value=="3"))
	{
	    if(cf.security_type[1].checked == true || cf.security_type[2].checked == true)
		{
			
			if(confirm("$wlan_tkip_300_150") == false)
			{
				return false;
			}

			cf.wl_hidden_wlan_mode.value = "1"; //save for wl_simple_mode

		}
		else if(cf.security_type[3].checked == true)
		{
			if(guest_mode_flag == 1)
			{
				tag1 = 1;
				if(confirm("$guest_tkip_300_150") == false)
				{
					return false;
				}
				cf.wl_hidden_wlan_mode.value = "1"; 
			}
			else if(guest_mode_flag == 2)
			{
				tag2 = 1;
				if(confirm("$guest_tkip_aes_300_150") == false)
				{
					return false;	
				}		
				cf.wl_hidden_wlan_mode.value = cf.opmode.value;	
			}
			else
				cf.wl_hidden_wlan_mode.value = cf.opmode.value;	
		}
		else if(cf.security_type[4].checked == true)
		{
			tag3 = 1;
			if(confirm("$wlan_tkip_aes_300_150") == false)
			{
				return false;	
			}
			
			cf.wl_hidden_wlan_mode.value = cf.opmode.value;
		}
		else if(cf.security_type[5].checked == true)//Bug 19803 WPA/WPA2 Enterprise, has three WPA Mode
		{
			if(cf.wpae_mode.value == 'WPAE-TKIP')
			{
				if(confirm("$wlan_tkip_300_150") == false)
					return false;
				
				cf.wl_hidden_wlan_mode.value = "1"; //save for wl_simple_mode
			}
			else if(cf.wpae_mode.value == 'WPAE-TKIPAES')
			{
				if(confirm("$wlan_tkip_aes_300_150") == false)
					return false;

				cf.wl_hidden_wlan_mode.value = cf.opmode.value;
			}
			else
			{
				if(guest_mode_flag == 1)
				{
					if(confirm("$guest_tkip_300_150") == false)
					{
						return false;
					}
					cf.wl_hidden_wlan_mode.value = "1"; 
				}
				else if(guest_mode_flag == 2)
				{
					if(confirm("$guest_tkip_aes_300_150") == false)
					{
						return false;	
					}		
					cf.wl_hidden_wlan_mode.value = cf.opmode.value;	
				}
				else 
					cf.wl_hidden_wlan_mode.value = cf.opmode.value;	
			}
		}
		else
		{
			if(guest_mode_flag == 1)
			{
				if(confirm("$guest_tkip_300_150") == false)
				{
					return false;
				}
				cf.wl_hidden_wlan_mode.value = "1";
			}
			else
				cf.wl_hidden_wlan_mode.value = cf.opmode.value;
		}
	}
	else
	{
		cf.wl_hidden_wlan_mode.value = cf.opmode.value;
	}

	var flad_op = false;
	if(parent.mode_is_300 == 1 && wl_disablecoext != 1 && (cf.opmode.value!="1") && (cf.opmode.value!="2"))
	{
	    flad_op = true;
	     alert(msg);
	}
	
	if(cf.security_type[1].checked == true)
	{
		alert("$wep_just_one_ssid");
	}		

	//bug 33156
	if( endis_wl_radio == 1 && cf.ssid_bc.checked == false )
	{
		if(!confirm("$wps_warning1"))
			return false;
		haven_wpe = 1;
	}

	if( endis_wl_radio == 1 && (cf.wl_hidden_sec_type.value == "2" || cf.wl_hidden_sec_type.value == "3"))
	{
		if(haven_wpe == 0)
		{
			if(!confirm("$wps_warning2"))
				return false;
		}
	}

	if( cf.wl_hidden_sec_type.value == "1" )
	{
		if(!confirm("$wps_warning3"))
			return false;
	}

	if((endis_wl_radio == 1 && cf.wl_hidden_sec_type.value == "6" ))
	{
		if(haven_wpe == 0)
		{
			if (!confirm("$wpae_or_wps"))
				return false;
		}
	}
	cf.submit();
	return true;	
}

function check_wlan_guest(type)
{
	var cf=document.forms[0];
	//bug 41791var ssid = document.forms[0].ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
	var ssid = document.forms[0].ssid.value;
	//var space_flag=0;
	cf.s_gssid.value=ssid;
	//bug 41791var wl_ssid=document.forms[0].wlssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
	var wl_ssid=document.forms[0].wlssid.value;
	/* Fixed Bug28645: if WPS is in progress, it must not be interrupt when user want to enable Guest Network 
	if( wps_progress_status == "2" || wps_progress_status == "3" || wps_progress_status == "start" )
	{
		alert("$wps_in_progress");
		return false;
	}*/
	if(ssid == "")
	{
		alert("$ssid_null");
		return false;
	}
        if(ssid == wl_ssid)
        {
                alert("$ssid_not_allowed_same");
                return false;
        }
	for(i=0;i<ssid.length;i++)
	{
		if(isValidChar_space(ssid.charCodeAt(i))==false)
		{
			alert(ssid + "$ssid_not_allowed");
			return false;
		}
	}

	if(cf.enable_bssid.checked == true)
		cf.hidden_enable_guestNet.value=1;
	else
		cf.hidden_enable_guestNet.value=0;
		
	if(cf.enable_ssid_bc.checked == true)
		cf.hidden_enable_ssidbro.value=1;
	else
		cf.hidden_enable_ssidbro.value=0;
		
	if(cf.allow_access.checked == true)
		cf.hidden_allow_see_and_access.value=1;
	else
		cf.hidden_allow_see_and_access.value=0;

	cf.wl_hidden_wlan_mode.value = wl_simple_mode;
	
	if(wireless_sectype=="2" && cf.enable_bssid.checked == true && cf.security_type[1].checked == true)// to fix bug 30740
	{
		alert("$wep_just_one_ssid");
		return false;
	}

	if(cf.security_type[1].checked == true)
	{
		cf.hidden_guest_network_mode_flag.value=0;
		cf.wl_hidden_wlan_mode.value = "1";
		if( checkwep(cf)== false)
			return false;
		cf.hidden_sec_type.value=2;

		alert("$wep_just_one_ssid");
	}
	else if(cf.security_type[2].checked == true)
	{
		if( checkpsk(cf.passphrase, cf.sec_wpaphrase_len)== false)
			return false;

		/* Bug 20177, the same as bug 19591 and 19803.
		When user selects WPA-PSK(TKIP)+150Mbps and WPA-PSK(TKIP)+300Mbps, set wl_simple_mode=1,
		or select "WPA-PSK [TKIP] + WPA2-PSK [AES]"+150Mbps and "WPA-PSK [TKIP] + WPA2-PSK [AES]"+300Mbps */
		if(wl_simple_mode != "1")
		{
			if(confirm("$wlan_tkip_300_150") == false)
			{
				cf.hidden_guest_network_mode_flag.value=0;
				return false;
			}
		}
		cf.hidden_guest_network_mode_flag.value=1;
		cf.wl_hidden_wlan_mode.value = "1";

		cf.hidden_sec_type.value=3;
		//bug 41791cf.hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk.value = cf.passphrase.value;
	}
	else if(cf.security_type[3].checked == true)
	{
		cf.hidden_guest_network_mode_flag.value=0;
		if( checkpsk(cf.passphrase, cf.sec_wpaphrase_len)== false)
			return false;
		cf.hidden_sec_type.value=4;
		//bug 41791cf.hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk.value = cf.passphrase.value;
	}	
	else if(cf.security_type[4].checked == true)
	{
		if( checkpsk(cf.passphrase, cf.sec_wpaphrase_len)== false)
			return false;
		
		if(wl_simple_mode != "1")
        {
			if(confirm("$wlan_tkip_aes_300_150") == false)
			{
				cf.hidden_guest_network_mode_flag.value=0;
				return false;
			}
		}
		cf.hidden_guest_network_mode_flag.value=2;
		cf.wl_hidden_wlan_mode.value = wl_simple_mode;

		cf.hidden_sec_type.value=5;
		//bug 41791cf.hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk.value = cf.passphrase.value;
	}	
	else if(cf.security_type[5].checked == true)
	{
		if(cf.wpae_mode.value == "WPAE-TKIP")
		{
			if(wl_simple_mode != "1")
			{
				if(confirm("$wlan_tkip_300_150") == false)
				{
					cf.hidden_guest_network_mode_flag.value=0;
					return false
				}
			}
			cf.hidden_guest_network_mode_flag.value=1;
			cf.wl_hidden_wlan_mode.value = "1";
		}
		else if(cf.wpae_mode.value == 'WPAE-TKIPAES')
		{
			if(wl_simple_mode != "1")
			{
				if(confirm("$wlan_tkip_aes_300_150") == false)
				{
					cf.hidden_guest_network_mode_flag.value=0;
					return false;
				}
			}
			cf.hidden_guest_network_mode_flag.value=2;
			cf.wl_hidden_wlan_mode.value = wl_simple_mode;
			cf.textWpaeRadiusPort.value=port_range_interception(cf.textWpaeRadiusPort.value);
		}
		else
		{
			cf.hidden_guest_network_mode_flag.value=0;
			cf.wl_hidden_wlan_mode.value = wl_simple_mode;
		}
			
		radiusServerIP = cf.radiusIPAddr1.value+'.'+ cf.radiusIPAddr2.value + '.' + cf.radiusIPAddr3.value + '.' + cf.radiusIPAddr4.value;
		if( radiusServerIP == "" || checkipaddr(radiusServerIP) == false )
		{
			alert("$invalid_ip");
			return false;
		}
		if( isSameIp(lanIP, radiusServerIP) == true )
		{
			alert("$invalid_ip");
			return false;
		}
		if( isSameIp(wanIP, radiusServerIP) == true )
		{
			alert("$conflicted_with_wanip");
			return false;
		}	
		cf.radiusServerIP.value = radiusServerIP;
		
		radiusPort=parseInt(cf.textWpaeRadiusPort.value,10);
		if( isNaN(radiusPort) || radiusPort < 1 || radiusPort > 65535 )
		{
			alert("$radiusPort65535");
			return false;
		}
		cf.textWpaeRadiusPort.value=radiusPort;
		if( cf.textWpaeRadiusSecret.value == "" )
		{
			alert("$radiusSecret128");
			return false;
		}
		if( cf.textWpaeRadiusSecret.length > 128 )
		{
			alert("$radiusSecret128");
			return false;
		}
                for(i=0;i<cf.textWpaeRadiusSecret.value.length;i++)
		{
			if(isValidChar(cf.textWpaeRadiusSecret.value.charCodeAt(i))==false)
			{
				alert("$radiusSecret128");
				cf.textWpaeRadiusSecret.focus();
				return false;
			}
		}
		//bug 41791cf.hidden_WpaeRadiusSecret.value = cf.textWpaeRadiusSecret.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
		cf.hidden_WpaeRadiusSecret.value = cf.textWpaeRadiusSecret.value;

		cf.hidden_sec_type.value=6;
	}	
	else
		cf.hidden_sec_type.value=1;
		
	cf.submit();
	return true;
}
