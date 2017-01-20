var Africa=0;
var Asia=1;
var Australia=2;
var Canada=3;
var China=11;
var Europe=4;
var India=12;
var Israel=5;
var Japan=6;
var Korea=7;
var Malaysia=13;
var Mexico=8;
var Middle_East_Algeria_Syria_Yemen=14;
var Middle_East_Iran_Lebanon_Qatar=15;
var Middle_East_Turkey_Egypt_Tunisia_Kuwait=16;
var Middle_East_Saudi_Arabia=17;
var Middle_East_United_Arab_Emirates=18;
var Middle_East=22;
var Russia=19;
var Singapore=20;
var South_America=9;
var Taiwan=21;
var United_States=10;
var qca_region_arr=new Array("za", "none", "au", "ca", "eu", "il", "jp","kr", "mx", "none", "us", "cn", "none", "my", "none", "none", "tr", "sa", "ae", "ru", "sg", "tw","");
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

function changekeylen_a(form)
{
        if(form.wepenc_an.options[0].selected == true)
        {
                form.KEY1_an.size=form.KEY2_an.size=form.KEY3_an.size=form.KEY4_an.size=18;
                form.KEY1_an.value = wep_64_key1_a;
                form.KEY2_an.value = wep_64_key2_a;
                form.KEY3_an.value = wep_64_key3_a;
                form.KEY4_an.value = wep_64_key4_a;
        }
        else
        {
                form.KEY1_an.size=form.KEY2_an.size=form.KEY3_an.size=form.KEY4_an.size=34;
                form.KEY1_an.value = wep_128_key1_a;
                form.KEY2_an.value = wep_128_key2_a;
                form.KEY3_an.value = wep_128_key3_a;
                form.KEY4_an.value = wep_128_key4_a;
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

function setSecurity(num)
{
        var form=document.forms[0];
        form.wpa1_press_flag.value=0;
        form.wpa2_press_flag.value=0;
        form.wpas_press_flag.value=0;
        if(num==2)
        {
                opmode_disabled();
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
        {
                //opmode_disabled();to fix bug 31373
                opmode_abled();
                wl_sectype_change();
                getObj("view").innerHTML=str_wpa;
        }
        else if(num==4)
        {
                opmode_abled();
                wl_sectype_change();
                getObj("view").innerHTML=str_wpa2;
        }
        else if(num==5)
        {
                opmode_abled();
                wl_sectype_change();
                getObj("view").innerHTML=str_wpas;
        }
        else if (num==6)
        {
                getObj("view").innerHTML=str_wpae;
                form.wpae_mode.value = get_wpae_mode;
                /*if(form.wpae_mode.value == 'WPAE-TKIP')
                        opmode_disabled();
                else
                {*/
                        opmode_abled();
                        wl_sectype_change();
                //}
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
        {
                opmode_abled();
                wl_sectype_change();
                getObj("view").innerHTML=str_none;
        }

	if(top.guest_router_flag == 1 && wlg1_sectype == 2)
		opmode_disabled();
}

function opmode_an_disabled()
{
        document.getElementById("opmode_an_all").style.display="none";
        document.getElementById("opmode_an_54").style.display="";
}
function opmode_an_abled()
{
        document.getElementById("opmode_an_all").style.display="";
        document.getElementById("opmode_an_54").style.display="none";
}

function setSecurity_an(num)
{
        var form=document.forms[0];
        form.wla_wpa1_press_flag.value=0;
        form.wla_wpa2_press_flag.value=0;
        form.wla_wpas_press_flag.value=0;
        if(num==2)
        {
                opmode_an_disabled();
                getObj("view_a").innerHTML=str_wep_an;

                /* to fix bug 25282:The length of the wep key is changed shorter when you click it.*/
                changekeylen_a(form);

                var keyno=wla_get_keyno;
                var keylength=wla_get_keylength;
                form.wep_key_no_an[eval(keyno)-1].checked = true;
                form.KEY1_an.value=wla_key1;
                form.KEY2_an.value=wla_key2;
                form.KEY3_an.value=wla_key3;
                form.KEY4_an.value=wla_key4;
                form.old_length_a.value=keylength;
        }
        else if(num==3)
        {
                //opmode_an_disabled();to fix bug 31373
                opmode_an_abled();
                wla_sectype_change();
                getObj("view_a").innerHTML=str_wpa_an;
        }
        else if(num==4)
        {
                opmode_an_abled();
                wla_sectype_change();
                getObj("view_a").innerHTML=str_wpa2_an;
        }
        else if(num==5)
        {
                opmode_an_abled();
                wla_sectype_change();
                getObj("view_a").innerHTML=str_wpas_an;
        }
        else if (num==6)
        {
                getObj("view_a").innerHTML=str_wpae_an;
                form.wpae_mode_an.value = get_wpae_mode_a;
                /*if(form.wpae_mode_an.value == 'WPAE-TKIP')
                        opmode_an_disabled();
                else
                {*/
                        opmode_an_abled();
                        wla_sectype_change();
                //}
                if( get_radiusSerIp_a != "" && get_radiusSerIp_a != "0.0.0.0" )
                {
                        radiusIPArray = get_radiusSerIp_a.split(".");
                        form.radiusIPAddr1_an.value = radiusIPArray[0];
                        form.radiusIPAddr2_an.value = radiusIPArray[1];
                        form.radiusIPAddr3_an.value = radiusIPArray[2];
                        form.radiusIPAddr4_an.value = radiusIPArray[3];
                }
                form.textWpaeRadiusPort_an.value = get_radiusPort_a;
        }
        else
        {
                opmode_an_abled();
                wla_sectype_change();
                getObj("view_a").innerHTML=str_none_an;

        }

	if(top.guest_router_flag == 1 && wla1_sectype == 2)
		opmode_an_disabled();
}

function wl_sectype_change()
{
        var form=document.forms[0];
        if(form.opmode.options[0].selected == true && bgn_mode1_value == 54)
	{
                document.getElementById("wep_54").style.display="";
	}
        else
	{
                document.getElementById("wep_54").style.display="none";
	}
}

function wl_54_sectype_change()
{
        var form=document.forms[0];
        document.getElementById("wep_54").style.display="";
        document.getElementById("wpa_psk_54").style.display="";
}

function wla_sectype_change()
{
        var form=document.forms[0];
        if(form.opmode_an.value == '1' && an_mode1_value == 54 )
                document.getElementById("wep_an_54").style.display="";
        else
                document.getElementById("wep_an_54").style.display="none";
}

function wla_54_sectype_change()
{
        var form=document.forms[0];
        document.getElementById("wep_an_54").style.display="";
        document.getElementById("wpa_psk_an_54").style.display="";
}

function wpaemode()
{
        var form=document.forms[0];
        document.getElementById("opmode_all").style.display="";
        document.getElementById("opmode_54").style.display="none";
        wl_sectype_change();
}

function wpaemode_an()
{
        var form=document.forms[0];
        document.getElementById("opmode_an_all").style.display="";
        document.getElementById("opmode_an_54").style.display="none";
        wla_sectype_change();
}
//bug 23854:The dialogue of DFS channel is not implemented
function check_dfs()
{
	var cf = document.forms[0]; 
	var each_info = dfs_info.split(':');
	var currentMode = cf.opmode_an.value;
	var index = cf.WRegion.value;
	var channel_info;
	var channel = cf.w_channel_an;
        var ch_index = channel.selectedIndex;
        var ch_name = channel.options[ch_index].text;
	var ch_value = channel.options[ch_index].value;
	var ht160_enabled= (top.support_ht160_flag == 1 && enable_ht160 == "1" && ((index == 10 || index == 4) && (currentMode != 1 && currentMode != 2 && currentMode != 7 && currentMode != 8)))

	if( ch_name.indexOf('(DFS)') == -1 && !ht160_enabled)
	{ // not a DFS channel and  ht160 disabled, return true, continue other check.
		return true;
	}
	if(top.dfs_radar_detect_flag == 1){	
		var tmp_array;
		if(ht160_enabled)
		{
			if(dfs_radar_160 == undefined)
				return true;
			tmp_array = dfs_radar_160;
		}
		else if ( 1 == currentMode || 2 == currentMode || 7 == currentMode )
		{
			if(dfs_radar_20 == undefined)
				return true;

			tmp_array = dfs_radar_20;
		}
	        else if( 9 == currentMode)
        	{
			if(dfs_radar_80 == undefined)
				return true;

	                tmp_array = dfs_radar_80;
        	}
		else
		{
			if(dfs_radar_40 == undefined)
				return true;
			tmp_array = dfs_radar_40;
		}
		for( var i=0; i<tmp_array.length-1; i++)
		{
			var channel = tmp_array[i].channel;
			var min = tmp_array[i].expire/60;
			var sec = tmp_array[i].expire%60;

			if( channel == ch_value)
			{
				alert("$using_dfs_1" + min.toFixed(0) + "$using_dfs_2" + sec + "$using_dfs_3");
				return false;
			}
		}
	}else{
	  for ( i=0; i<each_info.length; i++ )
	  {
		channel_info = each_info[i].split(' '); //channel; channel_flag; channe_priflag; left_time
		var sec = channel_info[3]%60;		//change left time format
		var min = parseInt(channel_info[3]/60);
		if( (5000 + 5*(parseInt(ch_value, 10))) == parseInt(channel_info[0], 10) )
		{
			alert("$using_dfs_1" + min + "$using_dfs_2" + sec + "$using_dfs_3");
			return false;
		}
	  }
	}
	if( ch_name.indexOf('(DFS)') != -1  && confirm("$select_dfs") == false)
		return false;

	return true;
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

function setBChannel()
{
	var cf = document.forms[0];
	var index = cf.WRegion.selectedIndex;
	index = parseInt(index)+1;
	var chIndex = cf.w_channel.selectedIndex;
	var currentMode = cf.opmode.selectedIndex;
	var endChannel;

	endChannel = FinishChannelB[index];
	if (FinishChannelB[index]==14 && cf.opmode.selectedIndex!=0)
		cf.w_channel.options.length = endChannel - StartChannelB[index];
	else
		cf.w_channel.options.length = endChannel - StartChannelB[index] + 2;

	cf.w_channel.options[0].text = "$auto_mark";
	cf.w_channel.options[0].value = 0;

	for (var i = StartChannelB[index]; i <= endChannel; i++) {
		if (i==14 && cf.opmode.selectedIndex!=0)
			continue;
		cf.w_channel.options[i - StartChannelB[index] + 1].value = i;
		cf.w_channel.options[i - StartChannelB[index] + 1].text = (i < 10)? "0" + i : i;
	}
	cf.w_channel.selectedIndex = ((chIndex > -1) && (chIndex < cf.w_channel.options.length)) ? chIndex : 0 ;
}

function chgChA(from)
{   
	var cf = document.forms[0];
	if (from == 2)
	{
		setAChannel(cf.w_channel_an);
	}
	else
	{
		setAwlan_mode();
		setAChannel(cf.w_channel_an);
	}
}

function setAwlan_mode()
{
	if(ac_router_flag == 1)
		setACwlan_mode()
	else
		setANwlan_mode()
}

function setACwlan_mode()
{
        var cf = document.forms[0];
        var index = cf.WRegion.value;
        var currentMode = cf.opmode_an.selectedIndex;

        // bug 34916, change index number to region name, make the code easy to read and change.
	if (index == Africa || index == Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Middle_East_Saudi_Arabia)
        { //Israel,Middle East(Turkey/Egypt/Tunisia/Kuwait) Middle East(Saudi Arabia) Africa
                cf.opmode_an.options.length = 1;
                cf.opmode_an.options[0].text = an_wlan_mode_1;
                cf.opmode_an.options[0].value = "7";
                if (currentMode <= 0)
                        cf.opmode_an.selectedIndex = currentMode;
                else
                        cf.opmode_an.selectedIndex = 0;
                cf.w_channel_an.disabled=false;
                cf.opmode_an.disabled=false;
        }
	else if ( index == Middle_East_Algeria_Syria_Yemen )
	{           // Middle East(Algeria/Syria/Yemen), this country do not support HT20 HT40,grayout channel
                cf.w_channel_an.selectedIndex=0;
                //cf.opmode_an.selectedIndex=0;
                cf.w_channel_an.disabled=true;
                cf.opmode_an.options.length = 3;
                cf.opmode_an.options[0].text = an_wlan_mode_1;
                cf.opmode_an.options[1].text = an_wlan_mode_2;
                cf.opmode_an.options[2].text = an_wlan_mode_3;
                cf.opmode_an.options[0].value = "7";
                cf.opmode_an.options[1].value = "8";
                cf.opmode_an.options[2].value = "9";
                cf.opmode_an.disabled=true;// bug 34916, grey out mode, this region not support both HT20 and HT40
        }
	else if(index == Russia){
		cf.opmode_an.options.length = 2;
		cf.opmode_an.options[0].text = an_wlan_mode_1;
		cf.opmode_an.options[1].text = an_wlan_mode_2;
		cf.opmode_an.options[0].value = "7";
		cf.opmode_an.options[1].value = "8";
		if (currentMode > 1)
			cf.opmode_an.selectedIndex = 1;
		else
			cf.opmode_an.selectedIndex = currentMode;
		cf.w_channel_an.disabled=false;
		cf.opmode_an.disabled=false;
	}
        else{
                cf.opmode_an.options.length = 3;

                cf.opmode_an.options[0].text = an_wlan_mode_1;
                cf.opmode_an.options[1].text = an_wlan_mode_2;
                cf.opmode_an.options[2].text = an_wlan_mode_3;
                cf.opmode_an.options[0].value = "7";
                cf.opmode_an.options[1].value = "8";
                cf.opmode_an.options[2].value = "9";
                cf.opmode_an.selectedIndex = currentMode;
                cf.w_channel_an.disabled=false;
                cf.opmode_an.disabled=false;
        }
        return;
}

function setANwlan_mode()
{
	var cf = document.forms[0];
	var index = cf.WRegion.value;
	var currentMode = cf.opmode_an.selectedIndex;

	// bug 34916, change index number to region name, make the code easy to read and change.
	if (index == Africa || index==Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Middle_East_Saudi_Arabia)
	{ //Israel,Middle East(Turkey/Egypt/Tunisia/Kuwait) Middle East(Saudi Arabia) Africa
		cf.opmode_an.options.length = 2;
		cf.opmode_an.options[0].text = an_wlan_mode_1;
		cf.opmode_an.options[1].text = an_wlan_mode_2;
		cf.opmode_an.options[0].value = "1";
		cf.opmode_an.options[1].value = "2";
		if (currentMode <= 1)
			cf.opmode_an.selectedIndex = currentMode;
		else
			cf.opmode_an.selectedIndex = 1;
		cf.w_channel_an.disabled=false;
		cf.opmode_an.disabled=false;			
	}
	else if ( index == Middle_East_Algeria_Syria_Yemen )
	{		// Middle East(Algeria/Syria/Yemen), this country do not support HT20 HT40,grayout channel
		cf.w_channel_an.selectedIndex=0;
		//cf.opmode_an.selectedIndex=0;
		cf.w_channel_an.disabled=true;
		cf.opmode_an.options.length = 3;
		cf.opmode_an.options[0].text = an_wlan_mode_1;
		cf.opmode_an.options[1].text = an_wlan_mode_2;
		cf.opmode_an.options[2].text = an_wlan_mode_3;
		cf.opmode_an.options[0].value = "1";
		cf.opmode_an.options[1].value = "2";
		cf.opmode_an.options[2].value = "3";
		cf.opmode_an.disabled=true;// bug 34916, grey out mode, this region not support both HT20 and HT40
	}

	else{
		cf.opmode_an.options.length = 3;

		cf.opmode_an.options[0].text = an_wlan_mode_1;
		cf.opmode_an.options[1].text = an_wlan_mode_2;
		cf.opmode_an.options[2].text = an_wlan_mode_3;
		cf.opmode_an.options[0].value = "1";
		cf.opmode_an.options[1].value = "2";
		cf.opmode_an.options[2].value = "3";
		cf.opmode_an.selectedIndex = currentMode;
		cf.w_channel_an.disabled=false;
		cf.opmode_an.disabled=false;		
	}
	return;
}
function setAChannel(channel)
{
	var cf = document.forms[0];
	var index = cf.WRegion.value;
	var currentMode = cf.opmode_an.value;
	var option_array=document.getElementById("wireless_channel_an").options;
	var chValue = channel.value;
	var find_value = 0;
	var i, j=0, val;
	var tmp_array = ht40_array[index];

	if(enable_ht160 == "1" && currentMode == 9 && index == 10)
	{
		tmp_array = ht160_array10;
	}
	else if ( 1 == currentMode || 2 == currentMode || 7 == currentMode )
	{
		tmp_array = ht20_array[index];
	}
	else if( 9 == currentMode)
	{
		tmp_array = ht80_array[index];
	}

	channel.options.length = tmp_array.length+1;

	if ( dfs_channel_router_flag == 1 ) //Australia, Canada, Europe
	{
		channel.options[j].value = 0;
		channel.options[j].text = "$auto_mark";
		j++;
	}

	for ( i = 0; i < tmp_array.length; i++ )
	{
		if ( 0 == hidden_dfs_channel && ( 1 == dfs_channel_router_flag ||
			( dfs_canada_router_flag == 1 &&  index == 3 ) || //Australia, Canada, Europe
			( dfs_australia_router_flag == 1 &&  index == 2 ) ||
			( dfs_europe_router_flag == 1 &&  index == 4 ) ||
			( dfs_japan_router_flag && index == 6 ) || index == 10 ) ) //Japan, United States
		{
			if(currentMode == 9 && index == 21)//50244
				if(tmp_array[i] == "60" || tmp_array[i] == "64")
					continue;
			if( index == 17 && (tmp_array[i] == "149" || tmp_array[i] == "153" || tmp_array[i] == "157" || tmp_array[i] == "161" || tmp_array[i] == "165") )//53381
				continue;
			if ( tmp_array[i].indexOf("(DFS)") > -1 )
			{
				val =  tmp_array[i].split("(DFS)")[0];
				channel.options[j].value = val;
				channel.options[j].text = tmp_array[i];
				j++;
			}
			else
			{
				channel.options[j].value = channel.options[j].text = tmp_array[i];
				j++
			}
		}
		else
		{
			if ( tmp_array[i].indexOf("(DFS)") > -1 )
				continue;
			if(currentMode == 9 && index == 21)//50244
				if(tmp_array[i] == "60" || tmp_array[i] == "64")
					continue;
			if( index == 17 && (tmp_array[i] == "149" || tmp_array[i] == "153" || tmp_array[i] == "157" || tmp_array[i] == "161" || tmp_array[i] == "165") )//53381
				continue;
			channel.options[j].value = channel.options[j].text = tmp_array[i];
			j++;
		}

	}
	channel.options.length = j;

	for(i=0; i<option_array.length; i++)
	{
		if(option_array[i].value == chValue)
		{
			find_value = 1;
			channel.selectedIndex = i;
			break
		}
	}
	if (find_value == 0)
	{/* to fix bug 27403 */
		for(i=0;i<option_array.length;i++)
		{
			if(option_array[i].value == wla_get_channel)
			{
				find_value = 1;
				channel.selectedIndex = i;
				break;
			}
		}	
	}
	if(find_value == 0)
		channel.selectedIndex = 0;
}

function check_wlan()
{
	if( check_dfs() == false)
	{
		return false;
	}
	//fix bug 29094
	var tag1=0;//when the value is 1, not pop up "guest_tkip_300_150" for 5G 
	var tag2=0;//when the value is 1, not pop up "guest_tkip_aes_300_150" for 5G
	var tag3=0;//when the value is 1, not pop up "wlan_tkip_aes_300_150" for 5G
	var cf=document.forms[0];
	
	/*bug 41791 
	var ssid_bgn = document.forms[0].ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var ssid_bgn = document.forms[0].ssid.value;
	//var space_flag=0;
	var haven_wpe=0;
	var haven_alert_tkip=0;

	/*bug 41791
	var wla1_ssid=document.forms[0].wla1ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
	var wlg1_ssid=document.forms[0].wlg1ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var wla1_ssid=document.forms[0].wla1ssid.value;
	var wlg1_ssid=document.forms[0].wlg1ssid.value;
	
/*	if( wps_progress_status == "2" || wps_progress_status == "3" || wps_progress_status == "start" )
	{
		alert("$wps_in_progress");
		return false;
	}
*/
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

	/* to fix bug 25082 */
	/*for(i=0;i<ssid_bgn.length;i++)
	{
		if(ssid_bgn.charCodeAt(i)!=32)
			space_flag++;
	}
	if(space_flag==0)
	{
		alert("$ssid_null");
		return false;
	}*/
	cf.wl_ssid.value = ssid_bgn;
	
	//16400
	if(cf.ssid_bc.checked == true)
		cf.wl_enable_ssid_broadcast.value="1";
	else
		cf.wl_enable_ssid_broadcast.value="0";

	/* 	in our old web page, the cancel button value is alway the cancel mark, so the condition will never be 
		true, so just remove it.
	
	if(cf.Cancel.value=="WLG_wireless.htm")
		cf.opmode_bg.value=cf.opmode.value;
	*/
	
	/*        remove select region for new spec	
	if(cf.WRegion.selectedIndex == 0)
	{
		alert("$coun_select");
		return false;
	}
	*/
	
	cf.wl_apply_flag.value = "1";//bug 30924,if click the 'Apply' wl_apply_flag is '1',otherwise is '0'
	cf.wl_WRegion.value = cf.WRegion.value;
	cf.qca_wireless_region.value = qca_region_arr[parseInt(cf.WRegion.value)];
	if ( wds_endis_fun == 1 )
	{
		if ( cf.w_channel.selectedIndex == 0 )
		{
			alert("$wds_auto_channel");
			return false;
		}
	}
	cf.wl_hidden_wlan_channel.value = cf.w_channel.value;
	if( cf.enable_coexist.checked == true)
                cf.hid_enable_coexist.value="0";
        else
                cf.hid_enable_coexist.value="1";

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
/*
		if(isSameSubNet(radiusServerIP,lanSubnet,lanIP,lanSubnet) == false && isSameSubNet(radiusServerIP,wanSubnet,wanIP,wanSubnet) == false )
        {
            alert("$diff_LanWan_subnet");
			return false;
        }
*/
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
			if ( !haven_alert_tkip )
			{
				if(confirm("$wlan_tkip_300_150") == false)
				{
					return false;
				}
				haven_alert_tkip = 1;
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
			if(cf.wpae_mode.value == 'WPAE-TKIPAES')
			{
				tag3 = 1;
				if(confirm("$wlan_tkip_aes_300_150") == false)
					return false;

				cf.wl_hidden_wlan_mode.value = cf.opmode.value;
			}
			else
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
		}
		else
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
			else
				cf.wl_hidden_wlan_mode.value = cf.opmode.value;
		}
	}
	else
	{
		cf.wl_hidden_wlan_mode.value = cf.opmode.value;
	}

	var flad_op = false;
	if(parent.bgn_mode3_value > 150 && cf.enable_coexist.checked == true && (cf.opmode.value!="1") && (cf.opmode.value!="2"))
	{
	    flad_op = true;
	     alert(msg);
	}
	
	//cf.wl_mode.value = cf.opmode.value;
	//var haven_alert = '0';
	if(cf.security_type[1].checked == true)
	{
		/* To fix Bug 33991
		if ( cf.authAlgm.value == 1 && endis_wl_radio == 1)
        	{
			haven_alert = '1';
                	if (!confirm("$wep_or_wps"))
	                	return false;
		}*/
		
		if(an_router_flag == 1 ){	
			if( wla_mode == '1' && an_mode1_value == 54 )
				alert("$wep_just_one_ssid_an");
			else if( guest_router_flag == 1 )
				alert("$wep_just_one_ssid"+" (2.4GHz).");		
		}
		else if( guest_router_flag == 1 )
			alert("$wep_just_one_ssid");
	}		


	
	if(an_router_flag == 1)
	{		
		/* bug 41791
		var ssid_an = document.forms[0].ssid_an.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
		var ssid_an = document.forms[0].ssid_an.value;
		if( ssid_an == "")
		{
			alert("$ssid_null");
			return false;
		}
		if(ssid_bgn == wlg1_ssid || ssid_bgn == wla1_ssid || ssid_an == wlg1_ssid || ssid_an == wla1_ssid)
		{
			alert("$ssid_not_allowed_same");
			return false;
		}
		for(i=0;i<ssid_an.length;i++)
		{
			if(isValidChar_space(ssid_an.charCodeAt(i))==false)
			{
				alert("$ssid_not_allowed");
				return false;
			}
		}

		/* to fix bug 25082 */
		/*space_flag=0;
		for(i=0;i<ssid_an.length;i++)
		{
			if(ssid_an.charCodeAt(i)!=32)
				space_flag++;
		}
		if(space_flag==0)
		{
			alert("$ssid_null");
			return false;
		}*/
		
		cf.wla_ssid.value = ssid_an;
		cf.wla_WRegion.value =cf.WRegion.value;

		//16400
		if(cf.ssid_bc_an.checked == true)
			cf.wla_enable_ssid_broadcast.value="1";
		else
			cf.wla_enable_ssid_broadcast.value="0";
		if(cf.enable_video_an.checked == true)
			cf.hidden_enable_video.value=1;
		else
			cf.hidden_enable_video.value=0;	
/*
		if ( wla_wds_endis_fun == 1 )
		{
			if ( cf.w_channel_an.selectedIndex == 0 )
			{
				alert("$wds_auto_channel");
				return false;
			}
		}	
*/
		cf.wla_hidden_wlan_channel.value = cf.w_channel_an.value;
			
		//for a/n
		if(cf.security_type_an[1].checked == true)
		{
			if( checkwep_a(cf)== false)
				return false;
			cf.wla_hidden_sec_type.value=2;
		}
		else if(cf.security_type_an[2].checked == true)
		{
			if( checkpsk(cf.passphrase_an, cf.wla_sec_wpaphrase_len)== false)
				return false;
			cf.wla_hidden_sec_type.value=3;
			//bug 41791cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
			cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value;
		}
		else if(cf.security_type_an[3].checked == true)
		{
			if( checkpsk(cf.passphrase_an, cf.wla_sec_wpaphrase_len)== false)
				return false;
			cf.wla_hidden_sec_type.value=4;
			//bug 41791cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
			cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value;
		}	
		else if(cf.security_type_an[4].checked == true)
		{
			if( checkpsk(cf.passphrase_an, cf.wla_sec_wpaphrase_len)== false)
				return false;
			cf.wla_hidden_sec_type.value=5;
			//bug 41791cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
			cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value;
		}	
		else if(cf.security_type_an[5].checked == true)
		{
			radiusServerIP = cf.radiusIPAddr1_an.value+'.'+ cf.radiusIPAddr2_an.value + '.' + cf.radiusIPAddr3_an.value + '.' + cf.radiusIPAddr4_an.value;
			if( radiusServerIP == "" || checkipaddr(radiusServerIP) == false )
			{
				alert("$invalid_ip");
				return false;
			}
/*
			if(isSameSubNet(radiusServerIP,lanSubnet,lanIP,lanSubnet) == false && isSameSubNet(radiusServerIP,wanSubnet,wanIP,wanSubnet) == false )
			{
	            		alert("$invalid_ip");
				return false;
			}
*/
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
			cf.radiusServerIP_a.value = radiusServerIP;
			
			radiusPort=parseInt(cf.textWpaeRadiusPort_an.value,10);
			if( isNaN(radiusPort) || radiusPort < 1 || radiusPort > 65535 )
			{
				alert("$radiusPort65535");
				return false;
			}
			cf.textWpaeRadiusPort_an.value=radiusPort;
			if( cf.textWpaeRadiusSecret_an.value == "" )
			{
				alert("$radiusSecret128");
				return false;
			}
			if( cf.textWpaeRadiusSecret_an.length > 128 )
			{
				alert("$radiusSecret128");
				return false;
			}
			for(i=0;i<cf.textWpaeRadiusSecret_an.value.length;i++)
			{
			        if(isValidChar(cf.textWpaeRadiusSecret_an.value.charCodeAt(i))==false)
			        {
			        	alert("$radiusSecret128");
					cf.textWpaeRadiusSecret_an.focus();
					return false;
				}
			}
			//bug 41791cf.hidden_WpaeRadiusSecret_a.value = cf.textWpaeRadiusSecret_an.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
			cf.hidden_WpaeRadiusSecret_a.value = cf.textWpaeRadiusSecret_an.value;

			cf.textWpaeRadiusPort_an.value=port_range_interception(cf.textWpaeRadiusPort_an.value);
			cf.wla_hidden_sec_type.value=6;
		}	
		else
			cf.wla_hidden_sec_type.value=1;

		//5GHz a/n : When user selects WPA-PSK(TKIP)+150Mbps and WPA-PSK(TKIP)+300Mbps, set wl_simple_mode=1,Bug No.19591
		//or select "WPA-PSK [TKIP] + WPA2-PSK [AES]"+150Mbps and "WPA-PSK [TKIP] + WPA2-PSK [AES]"+300Mbps 
		if((cf.opmode_an.value=="2") || (cf.opmode_an.value=="3"))
		{
			if(cf.security_type_an[1].checked == true)
				cf.wla_hidden_wlan_mode.value = "1"; //save for wla_simple_mode
			else if(cf.security_type_an[2].checked == true)
			{
				if ( !haven_alert_tkip )
				{
					if(confirm("$wlan_tkip_300_150") == false)
					{
						return false;
					}
					haven_alert_tkip = 1;
				}

				cf.wla_hidden_wlan_mode.value = "1"; //save for wla_simple_mode
			}
			else if(cf.security_type_an[3].checked == true)
			{
				if(wla_guest_mode_flag == 1)
				{
					if(tag1 == 0)
					{
						if(confirm("$guest_tkip_300_150") == false)
							return false;
					}
					cf.wla_hidden_wlan_mode.value = "1"; 
				}
				else if(wla_guest_mode_flag == 2)
				{
					if(tag2 == 0)
					{
						if(confirm("$guest_tkip_aes_300_150") == false)
							return false;		
					}
					cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;	
				}
				else
					cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;	
			}
			else if(cf.security_type_an[4].checked == true)
			{
				if(tag3 == 0)
				{
					if(confirm("$wlan_tkip_aes_300_150") == false)
						return false;	
				}
				cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
			}
			else if(cf.security_type_an[5].checked == true)//Bug 19803 WPA/WPA2 Enterprise, has three WPA Mode
			{
				if(cf.wpae_mode_an.value == 'WPAE-TKIP')
				{
					if ( !haven_alert_tkip )
					{
						if(confirm("$wlan_tkip_300_150") == false)
							return false;
						haven_alert_tkip = 1;
					}
					cf.wla_hidden_wlan_mode.value = "1"; //save for wl_simple_mode
				}
				else if(cf.wpae_mode_an.value == 'WPAE-TKIPAES')
				{
					if(tag3 == 0)
					{
						if(confirm("$wlan_tkip_aes_300_150") == false)
							return false;
					}
					cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
				}
				else
				{
					if(wla_guest_mode_flag == 1)
					{
						if(tag1 == 0)
						{
							if(confirm("$guest_tkip_300_150") == false)
								return false;
						}
						cf.wla_hidden_wlan_mode.value = "1"; 
					}
					else if(wla_guest_mode_flag == 2)
					{
						if(tag2 == 0)
						{
							if(confirm("$guest_tkip_aes_300_150") == false)
								return false;	
						}		
						cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;	
					}
					else
						cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;	
				}
			}
			else
			{
				if(wla_guest_mode_flag == 1)
				{
					if(tag1 == 0)
					{
						if(confirm("$guest_tkip_300_150") == false)
							return false;
					}
					cf.wla_hidden_wlan_mode.value = "1";
				}
				else
					cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
			}
		}
		else
		{
			cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
		}
		
		
	if(parent.an_mode3_value > 150 && wla_disablecoext != 1 && (cf.opmode_an.value!="1") && (cf.opmode_an.value!="2"))
	{
	   if(flad_op != true)
	     alert(an_msg);
	
	}
	
		
		/*countryIndex=cf.WRegion.value;
		if( (countryIndex == 5 || countryIndex == 14) && !confirm("$notSupportWLA") )
			return false;
		*/
		//cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
		
		if(cf.security_type_an[1].checked == true && endis_wla_radio == 1)
		{
			/*To fix Bug 33991
		        if ( cf.authAlgm_an.value == 1)
	                {
				if(haven_alert == '0')
	                        	if (!confirm("$wep_or_wps"))
	                                	return false;
			}*/
		
			if(cf.security_type[1].checked == false)
				alert("$wep_just_one_ssid_an");
	        }
		var channel_a=cf.w_channel_an.value;
		var country=cf.wl_WRegion.value;
		//transmit power control, according to the change of country, change values of wl_txctrl and wla_txctrl.
		wlan_txctrl(cf, wl_txctrl_web, wla_txctrl_web, channel_a, country);

	}

	//bug 33156
	if( endis_wl_radio == 1 && cf.ssid_bc.checked == false ||
	(an_router_flag ==1 && endis_wla_radio == 1 && cf.ssid_bc_an.checked == false) )
	{
		if(!confirm("$wps_warning1"))
			return false;
		haven_wpe = 1;
	}

	if( endis_wl_radio == 1 && (cf.wl_hidden_sec_type.value == "2" || cf.wl_hidden_sec_type.value == "3") ||
	(an_router_flag ==1 && endis_wla_radio == 1 &&( cf.wla_hidden_sec_type.value == "2" || cf.wla_hidden_sec_type.value == "3" )) )
	{
		if(haven_wpe == 0)
		{
			if(!confirm("$wps_warning2"))
				return false;
		}
	}

	if( cf.wl_hidden_sec_type.value == "1" || (an_router_flag ==1 && cf.wla_hidden_sec_type.value == "1" ) )
	{
		if(!confirm("$wps_warning3"))
			return false;
	}

	if((endis_wl_radio == 1 && cf.wl_hidden_sec_type.value == "6" ) ||
	(an_router_flag ==1 && cf.wla_hidden_sec_type.value == "6" && endis_wla_radio == 1))
	{
		if(haven_wpe == 0)
		{
			if (!confirm("$wpae_or_wps"))
				return false;
		}
	}
	if(document.getElementsByName("WRegion")[0].value != wl_get_countryA && cf.ssid_bc_an.checked == true)
		cf.change_region_flag.value = 1;
	else
		cf.change_region_flag.value = 0;
	
	cf.submit();
	return true;	
}

function check_wlan_guest(type)
{
	var cf=document.forms[0];
	
	/*bug 41791
	var ssid = document.forms[0].ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
	var ssid_an = document.forms[0].ssid_an.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	//var space_flag=0;
	var ssid = document.forms[0].ssid.value;
	var ssid_an = document.forms[0].ssid_an.value;
	cf.s_gssid.value=ssid;
	cf.s_gssid_an.value=ssid_an;

	/*bug 41791
	var wl_ssid=document.forms[0].wlssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
	var wla_ssid=document.forms[0].wlassid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var wl_ssid=document.forms[0].wlssid.value;
	var wla_ssid=document.forms[0].wlassid.value;
	var tag1 = 0;
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
	if(ssid_an == "")
	{
		alert("$ssid_null");
		return false;
	}
        if(ssid == wl_ssid || ssid == wla_ssid)
        {
                alert("$ssid_not_allowed_same");
                return false;
        }
	if(ssid_an == wl_ssid || ssid_an == wla_ssid)
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
	for(i=0;i<ssid_an.length;i++)
	{
		if(isValidChar_space(ssid_an.charCodeAt(i))==false)
		{
			alert(ssid_an + "$ssid_not_allowed");
			return false;
		}
	}
	if(type == 'gre')
	{
	    if(cf.enable_gre.checked == true){
		cf.hidden_enable_gre.value = 1;
		if(cf.enable_dhcp_relay.checked = true)
			cf.hidden_enable_dhcp_relay.value = "1";	
		else
			cf.hidden_enable_dhcp_relay.value = "0";
		cf.hidden_oppsite_ip.value = cf.GRE_op_ip1.value + "." + cf.GRE_op_ip2.value + "." + cf.GRE_op_ip3.value + "." + cf.GRE_op_ip4.value;
		if(cf.enable_vlan_id.checked == true)
		{
			var wl_gre_vid = cf.vlan_id_24.value;
			if(wl_gre_vid == "" )
			{
				alert("$wlan_gre_24_null");
				return false;
			}
			if(!(wl_gre_vid >= 1 && wl_gre_vid <= 4094))
			{
				alert("$wlan_gre_24_invalid");
				return false;
			}
			cf.hidden_wl_gre_vlanid.value = wl_gre_vid;
			if(an_router_flag == 1)
			{
				var wla_gre_vid = cf.vlan_id_50.value;
				if(wla_gre_vid == "" )
				{
					alert("$wlan_gre_50_null");
					return false;
				}
				if( !(wla_gre_vid >= 1 && wla_gre_vid <= 4094))
				{
					alert("$wlan_gre_50_invalid");
					return false;
				}
				cf.hidden_wla_gre_vlanid.value = wla_gre_vid;
			}
			cf.hidden_enable_gre_vlan.value = 1;
		}
		else
			cf.hidden_enable_gre_vlan.value = 0;
	
		if(checkipaddr(cf.hidden_oppsite_ip.value)==false)
		{
			alert("$invalid_ip");
			return false;

		}
           } else
		cf.hidden_enable_gre.value = 0;
	}

	/* to fix bug 25082 */
	/*for(i=0;i<ssid.length;i++)
	{
		if(ssid.charCodeAt(i)!=32)
			space_flag++;
	}
	for(i=0;i<ssid_an.length;i++)
	{
		if(ssid_an.charCodeAt(i)!=32)
			space_flag++;
	}
	if(space_flag==0)
	{
		alert("$ssid_null");
		return false;

	}*/

	if(cf.enable_bssid.checked == true)
		cf.hidden_enable_guestNet.value=1;
	else
		cf.hidden_enable_guestNet.value=0;
	if(cf.enable_bssid_an.checked == true)
		cf.hidden_enable_guestNet_an.value=1;
	else
		cf.hidden_enable_guestNet_an.value=0;
		
	if(cf.enable_ssid_bc.checked == true)
		cf.hidden_enable_ssidbro.value=1;
	else
		cf.hidden_enable_ssidbro.value=0;
	if(cf.enable_ssid_bc_an.checked == true)
		cf.hidden_enable_ssidbro_an.value=1;
	else
		cf.hidden_enable_ssidbro_an.value=0;
		
	if(cf.enable_video_an.checked == true)
		cf.hidden_enable_video_an.value=1;
	else
		cf.hidden_enable_video_an.value=0;

	if(type == 'bgn')
	{
		if(cf.allow_access.checked == true)
			cf.hidden_allow_see_and_access.value=1;
		else
			cf.hidden_allow_see_and_access.value=0;
		if(cf.allow_access_an.checked == true)
			cf.hidden_allow_see_and_access_an.value=1;
		else
			cf.hidden_allow_see_and_access_an.value=0;
	}

	var haven_alert_tkip = 0;
	cf.wl_hidden_wlan_mode.value = wl_simple_mode;
	cf.wl_hidden_wlan_mode_an.value = wl_simple_mode_an;
	
	if(wireless_sectype=="2" && cf.enable_bssid.checked == true && cf.security_type[1].checked == true)// to fix bug 30740
	{
		if(parent.an_router_flag == 1){
			if(wl_simple_mode_an == "1" && an_mode1_value == 54)
				alert("$wep_just_one_ssid_an");
			else
				alert("$wep_just_one_ssid"+" (2.4GHz).");
		}
		else
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

		if(parent.an_router_flag == 1){
			if(wl_simple_mode_an == "1" && an_mode1_value == 54)
				alert("$wep_just_one_ssid_an");
			else
				alert("$wep_just_one_ssid"+" (2.4GHz).");
		}
		else
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
			if ( !haven_alert_tkip )
			{
				if(confirm("$wlan_tkip_300_150") == false)
				{
					return false;
				}
				haven_alert_tkip = 1;
			}
			cf.hidden_guest_network_mode_flag.value=0;
		}
		cf.hidden_guest_network_mode_flag.value=1;
		cf.wl_hidden_wlan_mode.value = "1";

		cf.hidden_sec_type.value=3;
		//bug 41791 cf.hidden_wpa_psk.value = cf.passphrase.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
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
			tag1 = 1;
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
				if ( !haven_alert_tkip )
				{
					if(confirm("$wlan_tkip_300_150") == false)
					{
						return false
					}
					haven_alert_tkip = 1;
				}
				cf.hidden_guest_network_mode_flag.value=0;
			}
			cf.hidden_guest_network_mode_flag.value=1;
			cf.wl_hidden_wlan_mode.value = "1";
		}
		else if(cf.wpae_mode.value == 'WPAE-TKIPAES')
		{
			if(wl_simple_mode != "1")
			{
				tag1 = 1;
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
/*
		if(isSameSubNet(radiusServerIP,lanSubnet,lanIP,lanSubnet) == false && isSameSubNet(radiusServerIP,wanSubnet,wanIP,wanSubnet) == false)
        {
            alert("$invalid_ip");
			return false;
        }
*/		
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

/*		if (!confirm("$wpae_or_wps"))
			return false;			
*/
		cf.hidden_sec_type.value=6;
	}	
	else
		cf.hidden_sec_type.value=1;
		
	if(wla_sectype=="2" && cf.enable_bssid.checked == true && cf.security_type_an[1].checked == true)//to fix bug 30740
	{
		if(parent.an_router_flag == 1)
			alert("$wep_just_one_ssid_an");
		else
			alert("$wep_just_one_ssid");
		return false;
	}

	if(cf.security_type_an[1].checked == true)
	{
		cf.hidden_guest_network_mode_flag_an.value=0;
		cf.wl_hidden_wlan_mode_an.value = "1";
		if( checkwep_a(cf)== false)
			return false;
		cf.hidden_sec_type_an.value=2;

		if(cf.security_type[1].checked == false)
			alert("$wep_just_one_ssid_an");
		//else
			//alert("$wep_just_one_ssid");
	}
	else if(cf.security_type_an[2].checked == true)
	{
		if( checkpsk(cf.passphrase_an, cf.sec_wpaphrase_len_an)== false)
			return false;

		/* Bug 20177, the same as bug 19591 and 19803.
		   When user selects WPA-PSK(TKIP)+150Mbps and WPA-PSK(TKIP)+300Mbps, set wl_simple_mode=1,
		   or select "WPA-PSK [TKIP] + WPA2-PSK [AES]"+150Mbps and "WPA-PSK [TKIP] + WPA2-PSK [AES]"+300Mbps */
		if(wl_simple_mode != "1")
		{
			if ( !haven_alert_tkip )
			{
				if(confirm("$wlan_tkip_300_150") == false)
				{
					return false;
				}
				haven_alert_tkip = 1;
			}
			cf.hidden_guest_network_mode_flag.value=0;
		}
		cf.hidden_guest_network_mode_flag_an.value=1;
		cf.wl_hidden_wlan_mode_an.value = "1";

		cf.hidden_sec_type_an.value=3;
		//bug 41791cf.hidden_wpa_psk_an.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk_an.value = cf.passphrase_an.value;
	}
	else if(cf.security_type_an[3].checked == true)
	{
		cf.hidden_guest_network_mode_flag_an.value=0;
		if( checkpsk(cf.passphrase_an, cf.sec_wpaphrase_len_an)== false)
			return false;
		cf.hidden_sec_type_an.value=4;
		//bug 41791cf.hidden_wpa_psk_an.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk_an.value = cf.passphrase_an.value;
	}	
	else if(cf.security_type_an[4].checked == true)
	{
		if( checkpsk(cf.passphrase_an, cf.sec_wpaphrase_len_an)== false)
			return false;
		
		if(wl_simple_mode_an != "1")
        {
			if(tag1 == 0)
			{
				if(confirm("$wlan_tkip_aes_300_150") == false)
				{
					cf.hidden_guest_network_mode_flag_an.value=0;
					return false;
				}
			}
		}
		cf.hidden_guest_network_mode_flag_an.value=2;
		cf.wl_hidden_wlan_mode_an.value = wl_simple_mode_an;

		cf.hidden_sec_type_an.value=5;
		//bug 41791cf.hidden_wpa_psk_an.value = cf.passphrase_an.value.replace(/\\/g,"\\\\").replace(/`/g,"\\`").replace(/"/g,"\\\"");
		cf.hidden_wpa_psk_an.value = cf.passphrase_an.value;
	}	
	else if(cf.security_type_an[5].checked == true)
	{
		if(cf.wpae_mode_an.value == "WPAE-TKIP")
		{
			if(wl_simple_mode != "1")
			{
				if ( !haven_alert_tkip )
				{
					if(confirm("$wlan_tkip_300_150") == false)
					{
						return false
					}
					haven_alert_tkip = 1;
				}
				cf.hidden_guest_network_mode_flag.value=0;
			}
			cf.hidden_guest_network_mode_flag_an.value=1;
			cf.wl_hidden_wlan_mode_an.value = "1";
		}
		else if(cf.wpae_mode_an.value == 'WPAE-TKIPAES')
		{
			if(wl_simple_mode_an != "1")
			{
				if(tag1 == 0)
				{
					if(confirm("$wlan_tkip_aes_300_150") == false)
					{
						cf.hidden_guest_network_mode_flag_an.value=0;
						return false;
					}
				}
			}
			cf.hidden_guest_network_mode_flag_an.value=2;
			cf.wl_hidden_wlan_mode_an.value = wl_simple_mode_an;
			cf.textWpaeRadiusPort_an.value=port_range_interception(cf.textWpaeRadiusPort_an.value);
		}
		else
		{
			cf.hidden_guest_network_mode_flag_an.value=0;
			cf.wl_hidden_wlan_mode_an.value = wl_simple_mode_an;
		}
			
		radiusServerIP = cf.radiusIPAddr1_an.value+'.'+ cf.radiusIPAddr2_an.value + '.' + cf.radiusIPAddr3_an.value + '.' + cf.radiusIPAddr4_an.value;
		if( radiusServerIP == "" || checkipaddr(radiusServerIP) == false )
		{
			alert("$invalid_ip");
			return false;
		}
/*
		if(isSameSubNet(radiusServerIP,lanSubnet,lanIP,lanSubnet) == false && isSameSubNet(radiusServerIP,wanSubnet,wanIP,wanSubnet) == false)
        {
            alert("$invalid_ip");
			return false;
        }
*/		
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
		cf.radiusServerIP_a.value = radiusServerIP;
		
		radiusPort=parseInt(cf.textWpaeRadiusPort_an.value,10);
		if( isNaN(radiusPort) || radiusPort < 1 || radiusPort > 65535 )
		{
			alert("$radiusPort65535");
			return false;
		}
		cf.textWpaeRadiusPort_an.value=radiusPort;
		if( cf.textWpaeRadiusSecret_an.value == "" )
		{
			alert("$radiusSecret128");
			return false;
		}
		if( cf.textWpaeRadiusSecret_an.length > 128 )
		{
			alert("$radiusSecret128");
			return false;
		}
                for(i=0;i<cf.textWpaeRadiusSecret_an.value.length;i++)
		{
			if(isValidChar(cf.textWpaeRadiusSecret_an.value.charCodeAt(i))==false)
			{
				alert("$radiusSecret128");
				cf.textWpaeRadiusSecret_an.focus();
				return false;
			}
		}
		//bug 41791cf.hidden_WpaeRadiusSecret_a.value = cf.textWpaeRadiusSecret_an.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");
		cf.hidden_WpaeRadiusSecret_a.value = cf.textWpaeRadiusSecret_an.value;

/*		if (!confirm("$wpae_or_wps"))
			return false;			
*/
		cf.hidden_sec_type_an.value=6;
	}	
	else
		cf.hidden_sec_type_an.value=1;
		
/*        if(cf.security_type[1].checked == true)
        {
                if ( cf.authAlgm.value == 1 && cf.enable_guestNet.checked == true)
                {
                        if (!confirm(wep_or_wps))
                                return false;
                }
        }
*/
	cf.submit();
	return true;
}

var ht20_array = new Array(
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "136(DFS)", "140(DFS)" ), //0
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ), //1
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ), //2
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ), //3
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)" ), //4
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)" ), //5
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)" ), //6
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ), //7
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161", "165" ), //8
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ), //9
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)","136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ), //10
	new Array ( "149", "153", "157", "161", "165" ), //11
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161", "165" ), //12
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161", "165" ), //13
	new Array ( "" ), //14
	new Array ( "149", "153", "157", "161", "165" ), //15
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)" ), //16
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161", "165" ), //17
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)" ), //18
	new Array ( "36", "40", "44", "48" ), //19
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161", "165" ), //20
	new Array ( "56", "60", "64", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161", "165" ) //21
);
var ht40_array = new Array(
	new Array ( "" ), //0
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "149", "153", "157", "161" ), //1
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "132(DFS)", "136(DFS)", "149", "153", "157", "161" ), //2
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "149", "153", "157", "161" ), //3
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)" ), //4
	new Array ( "" ), //5
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)" ), //6
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ), //7
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //8
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "149", "153", "157", "161" ), //9
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161" ), //10
	new Array ( "149", "153", "157", "161" ), //11
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //12
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //13
	new Array ( "" ), //14
	new Array ( "149", "153", "157", "161" ), //15
	new Array ( "" ), //16
	new Array ( "" ), //17
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)" ), //18
	new Array ( "36", "40", "44", "48" ), //19
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //20
	new Array ( "60", "64", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "149", "153", "157", "161" ) //21
);
var ht80_array = new Array(
	new Array ( "" ), //0
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ), //1
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "149", "153", "157", "161" ), //2
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "149", "153", "157", "161" ), //3
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)" ), //4
	new Array ( "" ), //5
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)" ), //6
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ), //7
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //8
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ), //9
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161" ), //10
	new Array ( "149", "153", "157", "161" ), //11
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //12
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //13
	new Array ( "" ), //14
	new Array ( "149", "153", "157", "161" ), //15
	new Array ( "" ), //16
	new Array ( "" ), //17
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)" ), //18
	new Array ( "36", "40", "44", "48" ), //19
	new Array ( "36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "149", "153", "157", "161" ), //20
	new Array ( "60", "64", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "149", "153", "157", "161" ) //21
);
var ht160_array10 = new Array("36", "40", "44", "48", "52(DFS)", "56(DFS)", "60(DFS)", "64(DFS)", "100(DFS)", "104(DFS)", "108(DFS)", "112(DFS)", "116(DFS)", "120(DFS)", "124(DFS)", "128(DFS)", "132(DFS)", "136(DFS)", "140(DFS)", "149", "153", "157", "161");
