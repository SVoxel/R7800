/*
 *	var index = cf.WRegion.selectedIndex;
	index=parseInt(index)+1;
 *	this is for one more region, select.
*/
//for bug 34916: region list index, if region list change, this list need change too
if(top.an_router_flag == 1){
	var Africa=1;
	var Asia=2;
	var Australia=3;
	var Canada=4;
	var China=5;
	var Europe=6;
	var India=7;
	var Israel=8;
	var Japan=9;
	var Korea=10;
	var Malaysia=11;
	var Mexico=12;
	var Middle_East_Algeria_Syria_Yemen=13;
	var Middle_East_Iran_Lebanon_Qatar=14;
	var Middle_East_Turkey_Egypt_Tunisia_Kuwait=15;
	var Middle_East_Saudi_Arabia=16;
	var Middle_East_United_Arab_Emirates=17;
	var Middle_East=-1;
	var Russia=18;
	var Singapore=19;
	var South_America=20;
	var Taiwan=21;
	var United_States=22;
}else{
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
}

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
                form.KEY1.size=form.KEY2.size=form.KEY3.size=form.KEY4.size=12;
                form.KEY1.value = wep_64_key1;
                form.KEY2.value = wep_64_key2;
                form.KEY3.value = wep_64_key3;
                form.KEY4.value = wep_64_key4;
        }
        else
        {
                form.KEY1.size=form.KEY2.size=form.KEY3.size=form.KEY4.size=28;
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
                form.KEY1_an.size=form.KEY2_an.size=form.KEY3_an.size=form.KEY4_an.size=12;
                form.KEY1_an.value = wep_64_key1_a;
                form.KEY2_an.value = wep_64_key2_a;
                form.KEY3_an.value = wep_64_key3_a;
                form.KEY4_an.value = wep_64_key4_a;
        }
        else
        {
                form.KEY1_an.size=form.KEY2_an.size=form.KEY3_an.size=form.KEY4_an.size=28;
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

/*function opmode_abled()
{
        document.getElementById("opmode_all").style.display="";
        document.getElementById("opmode_54").style.display="none";
}*/

function setSecurity(num)
{
        var form=document.forms[0];
        form.wpa1_press_flag.value=0;
        form.wpa2_press_flag.value=0;
        form.wpas_press_flag.value=0;
        if(num==2)
        {
             //   opmode_disabled();
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
           //     opmode_abled();
              //  wl_sectype_change();
                getObj("view").innerHTML=str_wpa;
        }
        else if(num==4)
        {
            //    opmode_abled();
               // wl_sectype_change();
                getObj("view").innerHTML=str_wpa2;
        }
        else if(num==5)
        {
             //   opmode_abled();
              //  wl_sectype_change();
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
                  //      opmode_abled();
                   //     wl_sectype_change();
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
             //   opmode_abled();
               // wl_sectype_change();
                getObj("view").innerHTML=str_none;
        }
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
             //   opmode_an_disabled();
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
             //   opmode_an_abled();
              //  wla_sectype_change();
                getObj("view_a").innerHTML=str_wpa_an;
        }
        else if(num==4)
        {
              //  opmode_an_abled();
             //   wla_sectype_change();
                getObj("view_a").innerHTML=str_wpa2_an;
        }
        else if(num==5)
        {
             //   opmode_an_abled();
             //   wla_sectype_change();
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
                      //  opmode_an_abled();
                    //    wla_sectype_change();
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
            //    opmode_an_abled();
               // wla_sectype_change();
                getObj("view_a").innerHTML=str_none_an;

        }
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

function wla_sectype_change()
{
        var form=document.forms[0];
        if(form.opmode_an.value == '1')
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
	var channel_info;

	var channel = cf.w_channel_an;
        var ch_index = channel.selectedIndex;
        var ch_name = channel.options[ch_index].text;
	var ch_value = channel.options[ch_index].value;

	if( ch_name.indexOf('(DFS)') == -1 )
	{ // not a DFS channel, return true, continue other check.
		return true;
	}
	for ( i=0; i<each_info.length; i++ )
	{
		channel_info = each_info[i].split(' '); //channel; channel_flag; channe_priflag; left_time
		var sec = channel_info[3]%60;		//change left time format
		var min = parseInt(channel_info[3]/60);
		if( ch_value == channel_info[0] )
		{
			alert("$using_dfs_1" + min + "$using_dfs_2" + sec + "$using_dfs_3");
			return false;
		}
	}

	if( confirm("$select_dfs") == false)
		return false;

	return true;
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
		cf.opmode.options[0].text = wlan_mode_54;
		cf.opmode.options[1].text = wlan_mode_130;
		cf.opmode.options[0].value = "1";
		cf.opmode.options[1].value = "2";
		if (currentMode <= 1)
			cf.opmode.selectedIndex = currentMode;
		else
			cf.opmode.selectedIndex = 1;
	} else {
		cf.opmode.options.length = 3;
		cf.opmode.options[0].text = wlan_mode_54;
		cf.opmode.options[1].text = wlan_mode_130;
		cf.opmode.options[2].text = wlan_mode_300;
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
        var index = cf.WRegion.selectedIndex;
        index=parseInt(index)+1;
        var currentMode = cf.opmode_an.selectedIndex;

        // bug 34916, change index number to region name, make the code easy to read and change.
        if (index == Africa || index==Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Middle_East_Saudi_Arabia)
        { //Israel,Middle East(Turkey/Egypt/Tunisia/Kuwait) Middle East(Saudi Arabia) Africa
                cf.opmode_an.options.length = 1;
                cf.opmode_an.options[0].text = wlan_mode_192;
                cf.opmode_an.options[0].value = "7";
                if (currentMode <= 0)
                        cf.opmode_an.selectedIndex = currentMode;
                else
                        cf.opmode_an.selectedIndex = 0;
                cf.w_channel_an.disabled=false;
                cf.opmode_an.disabled=false;
        }
        else if ( index == Middle_East_Algeria_Syria_Yemen ){           // Middle East(Algeria/Syria/Yemen), this country do not support HT20 HT40,grayout channel
                cf.w_channel_an.selectedIndex=0;
                //cf.opmode_an.selectedIndex=0;
                cf.w_channel_an.disabled=true;
                cf.opmode_an.options.length = 3;
                cf.opmode_an.options[0].text = wlan_mode_192;
                cf.opmode_an.options[1].text = wlan_mode_400;
                cf.opmode_an.options[2].text = wlan_mode_867;
                cf.opmode_an.options[0].value = "7";
                cf.opmode_an.options[1].value = "8";
                cf.opmode_an.options[2].value = "9";
                cf.opmode_an.disabled=true;// bug 34916, grey out mode, this region not support both HT20 and HT40
        }
	else if(index == Korea || index == Russia){
		cf.opmode_an.options.length = 2;

                cf.opmode_an.options[0].text = wlan_mode_192;
                cf.opmode_an.options[1].text = wlan_mode_400;
                //cf.opmode_an.options[2].text = wlan_mode_867;
                cf.opmode_an.options[0].value = "7";
                cf.opmode_an.options[1].value = "8";
                //cf.opmode_an.options[2].value = "9";
    
                if (currentMode <= 1)
			cf.opmode_an.selectedIndex = currentMode;
                else
			cf.opmode_an.selectedIndex = 0;
		
		 cf.w_channel_an.disabled=false;
                cf.opmode_an.disabled=false;
	}
        else{
                cf.opmode_an.options.length = 3;

                cf.opmode_an.options[0].text = wlan_mode_192;
                cf.opmode_an.options[1].text = wlan_mode_400;
                cf.opmode_an.options[2].text = wlan_mode_867;
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
	var index = cf.WRegion.selectedIndex;
	index=parseInt(index)+1;
	var currentMode = cf.opmode_an.selectedIndex;

	// bug 34916, change index number to region name, make the code easy to read and change.
	if (index == Africa || index==Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Middle_East_Saudi_Arabia)
	{ //Israel,Middle East(Turkey/Egypt/Tunisia/Kuwait) Middle East(Saudi Arabia) Africa
		cf.opmode_an.options.length = 2;
		cf.opmode_an.options[0].text = wlan_mode_54;
		cf.opmode_an.options[1].text = wlan_mode_130;
		cf.opmode_an.options[0].value = "1";
		cf.opmode_an.options[1].value = "2";
		if (currentMode <= 1)
			cf.opmode_an.selectedIndex = currentMode;
		else
			cf.opmode_an.selectedIndex = 1;
		cf.w_channel_an.disabled=false;
		cf.opmode_an.disabled=false;			
	}
	else if ( index == Middle_East_Algeria_Syria_Yemen ){		// Middle East(Algeria/Syria/Yemen), this country do not support HT20 HT40,grayout channel
		cf.w_channel_an.selectedIndex=0;
		//cf.opmode_an.selectedIndex=0;
		cf.w_channel_an.disabled=true;
		cf.opmode_an.options.length = 3;
		cf.opmode_an.options[0].text = wlan_mode_54;
		cf.opmode_an.options[1].text = wlan_mode_130;
		cf.opmode_an.options[2].text = wlan_mode_300;
		cf.opmode_an.options[0].value = "1";
		cf.opmode_an.options[1].value = "2";
		cf.opmode_an.options[2].value = "3";
		cf.opmode_an.disabled=true;// bug 34916, grey out mode, this region not support both HT20 and HT40
	}

	else{
		if (ac_router_flag == 1)
		{
			cf.opmode_an.options.length = 4;
			cf.opmode_an.options[3].text = wlan_mode_867;
	                cf.opmode_an.options[3].value = "9";
		}
		else
			cf.opmode_an.options.length = 3;

		cf.opmode_an.options[0].text = wlan_mode_54;
		cf.opmode_an.options[1].text = wlan_mode_130;
		cf.opmode_an.options[2].text = wlan_mode_300;
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
	var index = cf.WRegion.selectedIndex;
	/* to fix bug 27403 */
	var option_array=document.getElementById("select_channel_an").options;
	var select_region=parseInt(index,10)+1;
	var old_region=parseInt(wl_get_countryA,10);
	index=select_region;
	var chIndex = channel.selectedIndex;
	var chValue = channel.options[chIndex].value;
	var currentMode = cf.opmode_an.value;
	var endChannel;
   if(dfs_channel_router_flag == 1){
	var AChannel = new Array(0, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165);

	channel.options[0].text = "$auto_mark";
	channel.options[0].value = 0;	 
 	
	// bug 34916 follow spec V11, change all DFS channels
	if( index == Korea )                //Korean
	{
		endChannel = 16;
		channel.options.length = 21;
		for( i = 1; i<= endChannel; i++ )
		{
			channel.options[i].value = AChannel[i];
			if( i > 4 )
				channel.options[i].text = AChannel[i]+"(DFS)";
			else
				channel.options[i].text = AChannel[i];
		}
		channel.options[17].value = channel.options[17].text = 149;
		channel.options[18].value = channel.options[18].text = 153;
		channel.options[19].value = channel.options[19].text = 157;
		channel.options[20].value = channel.options[20].text = 161;
	}
	else if( index == Africa )
	{
		endChannel = 13;
		channel.options.length = 16;
		for( i = 1; i<= endChannel; i++ )
		{
			channel.options[i].value = AChannel[i];
			if( i > 4 )
				channel.options[i].text = AChannel[i]+"(DFS)";
			else
				channel.options[i].text = AChannel[i];
		}
		channel.options[14].value = 136;
		channel.options[14].text = "136(DFS)";
		channel.options[15].value = 140;
		channel.options[15].text = "140(DFS)";
	}
	else if( index ==Asia || index == South_America )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = AChannel.length;
			for( i = 1; i < AChannel.length; i++ )
			{
				channel.options[i].value = AChannel[i];

				if( i > 4 && i < 20)
					channel.options[i].text = AChannel[i]+"(DFS)";
				else
					channel.options[i].text = AChannel[i];
			}
		}
		else
		{
			channel.options.length = 23; //36101 show single channel
			for(i=1; i<=channel.options.length-4; i++)
			{
				channel.options[i].value = AChannel[i];
				if(i>4)
					channel.options[i].text = AChannel[i]+"(DFS)";
				else
					channel.options[i].text = AChannel[i];
			}
			channel.options[19].value=channel.options[19].text=149;
			channel.options[20].value=channel.options[20].text=153;
			channel.options[21].value=channel.options[21].text=157;
			channel.options[22].value=channel.options[22].text=161;
		}
	}
	else if( index == Australia )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 22;
			for( i = 1; i< channel.options.length ; i++ )
			{
				if( i > 13 )
					j = i + 3;
				else
					j = i;

				channel.options[i].value = AChannel[j];

				if( i > 4 && i < 17)
					channel.options[i].text = AChannel[j]+"(DFS)";
				else
					channel.options[i].text = AChannel[j];
			}
		}
		else
		{
			channel.options.length = 19; //36101 show single channel
			for(i=1; i< channel.options.length-6; i++)
			{
				channel.options[i].value = AChannel[i];
				if(i>4)
					channel.options[i].text = AChannel[i]+"(DFS)";
				else
					channel.options[i].text = AChannel[i];
			}
			channel.options[13].value =  132;
			channel.options[13].text = "132(DFS)";
			channel.options[14].value =  136;
			channel.options[14].text = "136(DFS)";
			channel.options[15].value = channel.options[15].text =  149;
			channel.options[16].value = channel.options[16].text =  153;
			channel.options[17].value = channel.options[17].text =  157;
			channel.options[18].value = channel.options[18].text =  161;
		}
	}
	else if( index == Canada || index == United_States )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 21;
			for( i = 1; i< channel.options.length ; i++ )
			{
				if( i > 13 )
					j = i + 4;
				else
					j = i;

				channel.options[i].value = AChannel[j];

				if( i > 4 && i < 16)
					channel.options[i].text = AChannel[j]+"(DFS)";
				else
					channel.options[i].text = AChannel[j];
			}
		}
		else
		{
			channel.options.length = 17; //36101 show single channel
			for(i=1; i< channel.options.length-4; i++)
			{
				channel.options[i].value = AChannel[i];
				if(i>4)
					channel.options[i].text = AChannel[i]+"(DFS)";
				else
					channel.options[i].text = AChannel[i];
			}
			channel.options[13].value = channel.options[13].text =  149;
			channel.options[14].value = channel.options[14].text =  153;
			channel.options[15].value = channel.options[15].text =  157;
			channel.options[16].value = channel.options[16].text =  161;
		}
	}
	else if( index == India || index == Mexico || index == Malaysia || index == Middle_East_Saudi_Arabia || index == Singapore )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 14;
			for( i = 1; i< channel.options.length ; i++ )
			{
				if( i > 8 )
					j = i + 11;
				else
					j = i;

				channel.options[i].value = AChannel[j];

				if( i > 4 && i < 9)
					channel.options[i].text = AChannel[j]+"(DFS)";
				else
					channel.options[i].text = AChannel[j];
			}
		}
		else
		{
			channel.options.length = 13; //36101 show single channel
			channel.options[1].value = channel.options[1].text =  36;
			channel.options[2].value = channel.options[2].text =  40;
			channel.options[3].value = channel.options[3].text =  44;
			channel.options[4].value = channel.options[4].text =  48;

			channel.options[5].value = 52;
			channel.options[5].text = "52(DFS)";
			channel.options[6].value = 56;
			channel.options[6].text = "56(DFS)";
			channel.options[7].value = 60;
			channel.options[7].text = "60(DFS)";
			channel.options[8].value = 64;
			channel.options[8].text = "64(DFS)";
			channel.options[9].value = channel.options[9].text =  149;
			channel.options[10].value = channel.options[10].text =  153;
			channel.options[11].value = channel.options[11].text =  157;
			channel.options[12].value = channel.options[12].text =  161;
		}
	}
	else if( index == China || index == Middle_East_Iran_Lebanon_Qatar )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 6;
			channel.options[1].value = channel.options[1].text = 149;
			channel.options[2].value = channel.options[2].text = 153;
			channel.options[3].value = channel.options[3].text = 157;
			channel.options[4].value = channel.options[4].text = 161;
			channel.options[5].value = channel.options[5].text = 165;
		}
		else
		{
			channel.options.length = 5; //36101 show single channel
			channel.options[1].value = channel.options[1].text = 149;
			channel.options[2].value = channel.options[2].text = 153;
			channel.options[3].value = channel.options[3].text = 157;
			channel.options[4].value = channel.options[4].text = 161;
		}
	}
	else if( index == Europe || index == Japan  || index == Middle_East_United_Arab_Emirates )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 20;
			for( i = 1; i< channel.options.length; i++ )
			{
				j=i;
                                channel.options[i].value = AChannel[j];
                                if( i > 4)
					channel.options[i].text = AChannel[j]+"(DFS)";
				else
					channel.options[i].text = AChannel[j];
			}
		}
		else
		{
			channel.options.length = 19; //36101 show single channel
			for(i=1; i< channel.options.length; i++)
			{
				channel.options[i].value = AChannel[i];

				if(i>4)
					channel.options[i].text = AChannel[i]+"(DFS)";
				else
					channel.options[i].text = AChannel[i];
			}
		}
	}
	else if( index == Israel || index == Middle_East_Turkey_Egypt_Tunisia_Kuwait )
	{ // not support HT40
		channel.options.length = 9;
		for( i = 1; i< channel.options.length ; i++ )
		{
			j=i;
			channel.options[i].value = AChannel[j];
			if( i > 4)
				channel.options[i].text = AChannel[j]+"(DFS)";
			else
				channel.options[i].text = AChannel[j];
		}
	}
	else if( index == Russia )
	{ //36101 RU support 5G HT40.
		endChannel = 5;
		channel.options.length = 5;
		for( i = 1; i< endChannel; i++ )
		{
			channel.options[i].value = AChannel[i];
			channel.options[i].text = AChannel[i];
		}
	}
	else if( index == Taiwan )
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 20;
			for( i = 1; i< channel.options.length ; i++ )
			{
				j=i+5;
				channel.options[i].value = AChannel[j];
				if( i > 3 && i < 15)
					channel.options[i].text = AChannel[j]+"(DFS)";
				else
					channel.options[i].text = AChannel[j];
			}
		}
		else
		{
			channel.options.length = 17; //36101 show single channel
			channel.options[1].value = channel.options[1].text = 60;
			channel.options[2].value = channel.options[2].text = 64;
			channel.options[3].value = 100;
                        channel.options[3].text = "100(DFS)";
                        channel.options[4].value = 104;
                        channel.options[4].text = "104(DFS)";
                        channel.options[5].value = 108;
                        channel.options[5].text = "108(DFS)";
                        channel.options[6].value = 112;
                        channel.options[6].text = "112(DFS)";
                        channel.options[7].value = 116;
                        channel.options[7].text = "116(DFS)";
                        channel.options[8].value = 120;
                        channel.options[8].text = "120(DFS)";
                        channel.options[9].value = 124;
                        channel.options[9].text = "124(DFS)";
                        channel.options[10].value = 128;
                        channel.options[10].text = "128(DFS)";
                        channel.options[11].value = 132;
                        channel.options[11].text = "132(DFS)";
                        channel.options[12].value = 136;
                        channel.options[12].text = "136(DFS)";
                        channel.options[13].value = channel.options[13].text = 149;
                        channel.options[14].value = channel.options[14].text = 153;
                        channel.options[15].value = channel.options[15].text = 157;
                        channel.options[16].value = channel.options[16].text = 161;
		}
	}
   }
   else{	
	//      var AChannel = new Array(0, 36, 40, 44, 48, 149, 153, 157, 161, 165);
	var AChannel = new Array(36, 40, 44, 48, 149, 153, 157, 161, 165);

	var BChannel = new Array(36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165);

	//bug 34916, change index number to region name , make the code easy to read and change
	if(index == Asia || index == Australia || index == Canada || index == India || index == Malaysia || index == Mexico || index == Singapore || index == South_America || index == United_States)
	{										   //middle east move up two line
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			endChannel = 9;
			channel.options.length = 9;
			for( i = 0; i< endChannel; i++ )
			{
				channel.options[i].value = AChannel[i];
				channel.options[i].text = AChannel[i];
			}
		}
		else
		{
			endChannel = 8;
			channel.options.length = 8; //36101 show single channel
			for( i = 0; i < channel.options.length; i++)
			{
				channel.options[i].value = AChannel[i];
				channel.options[i].text = AChannel[i];
			}
		}
	}
	else if( index == Korea )
	{
			endChannel = 8;
			channel.options.length = 8;
			for( i = 0; i< endChannel; i++ )
			{
				channel.options[i].value = AChannel[i];
				channel.options[i].text = AChannel[i];
			}
	}
/* this can be used when region is China and Middle Eeast(Iran/Lebanon/Qatar)*/
        else if( index == China || index == Middle_East_Iran_Lebanon_Qatar )
        {
                if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
                {
			channel.options.length = 5;
			channel.options[0].value = channel.options[0].text = 149;
			channel.options[1].value = channel.options[1].text = 153;
			channel.options[2].value = channel.options[2].text = 157;
			channel.options[3].value = channel.options[3].text = 161;
			channel.options[4].value = channel.options[4].text = 165;
                }
		else
		{
			channel.options.length = 4; //36101 show single channel
			channel.options[0].value = channel.options[0].text = 149;
			channel.options[1].value = channel.options[1].text = 153;
			channel.options[2].value = channel.options[2].text = 157;
			channel.options[3].value = channel.options[3].text = 161;
		}
	}
	else if( index == Middle_East_Saudi_Arabia)   //Middle Eeast(Saudi Arabia)
	{
		endChannel = 9;
		channel.options.length = 9;
		for( i = 0; i< endChannel; i++ )
		{
			channel.options[i].value = AChannel[i];
			channel.options[i].text = AChannel[i];
		}
	}
	else if( index == Taiwan)   //Taiwan
	{
		if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
		{
			channel.options.length = 8;
			channel.options[0].value = channel.options[0].text = 56;
			channel.options[1].value = channel.options[1].text = 60;
			channel.options[2].value = channel.options[2].text = 64;
			channel.options[3].value = channel.options[3].text = 149;
			channel.options[4].value = channel.options[4].text = 153;
			channel.options[5].value = channel.options[5].text = 157;
			channel.options[6].value = channel.options[6].text = 161;
			channel.options[7].value = channel.options[7].text = 165;
		}
		else if( currentMode == 9 ) //HT80
		{
                        channel.options.length = 4;
                        channel.options[0].value = channel.options[0].text = 149;
                        channel.options[1].value = channel.options[1].text = 153;
                        channel.options[2].value = channel.options[2].text = 157;
                        channel.options[3].value = channel.options[3].text = 161;
		}
		else
		{
			channel.options.length = 6; ////36101 show single channel
			channel.options[0].value = channel.options[0].text = 60;
			channel.options[1].value = channel.options[1].text = 64;
			channel.options[2].value = channel.options[2].text = 149;
			channel.options[3].value = channel.options[3].text = 153;
			channel.options[4].value = channel.options[4].text = 157;
			channel.options[5].value = channel.options[5].text = 161;
		}
	}
	else
	{ // index == Middle_East_Turkey_Egypt_Tunisia_Kuwait || index == Russia || index == Europe || index == Japan
		endChannel = 4;
		channel.options.length = 4;
		for( i = 0; i< endChannel; i++ )
		{
			channel.options[i].value = AChannel[i];
			channel.options[i].text = AChannel[i];
		}
	}
	if(dfs_channel2_router_flag == 1)//bug 25665:DFS channel should be supported in Canada, Europe and Austrilia
	{
			if( index == Europe )
			{
				if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
				{
					endChannel = 19;
					channel.options.length = 19;
					for( i = 0; i< 4; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i];
					}
					for( i = 4; i< endChannel; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i]+'(DFS)';
					}
				}
				else
				{	//bug 23934:modify the channel selection in HT40 mode
					channel.options.length = 18; //36101 show single channel
					for( i = 0; i< channel.options.length ; i++ )
					{
						channel.options[i].value = BChannel[i];
						if( i > 3)
							channel.options[i].text = BChannel[i]+"(DFS)";
						else
							channel.options[i].text = BChannel[i];
					}
				}
				
			}
			/*else if(index == Canada )
			{				
				if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
				{
					endChannel = 12;
					channel.options.length = 20;
					for( i = 0; i< 4; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i];
					}
					for( i = 4; i< endChannel; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i]+'(DFS)';
					}
					channel.options[12].value = 116;
					channel.options[13].value = 136;
					channel.options[14].value = 140;
					channel.options[12].text = 116+'(DFS)';
					channel.options[13].text = 136+'(DFS)';
					channel.options[14].text = 140+'(DFS)';
					channel.options[15].value = channel.options[15].text = 149;
					channel.options[16].value = channel.options[16].text = 153;
					channel.options[17].value = channel.options[17].text = 157;
					channel.options[18].value = channel.options[18].text = 161;
					channel.options[19].value = channel.options[19].text = 165;
				}
				else
				{
					channel.options.length = 8;
					channel.options[0].value = 36;
					channel.options[0].text = '36+40';
					channel.options[1].value = 44;
					channel.options[1].text = '44+48';
					channel.options[2].value = 52;
					channel.options[2].text = '52+56(DFS)';
					channel.options[3].value = 60;
					channel.options[3].text = '60+64(DFS)';
					channel.options[4].value = 100;
					channel.options[4].text = '100+104(DFS)';
					channel.options[5].value = 108;
					channel.options[5].text = '108+112(DFS)';
					channel.options[6].value = 149;
					channel.options[6].text = '149+153';
					channel.options[7].value = 157;
					channel.options[7].text = '157+161';
				}
			}*/
			else if(index == Australia )//bug 23853:NETGEAR correct the DFS channel list for Austrilia
			{				
				if( currentMode == 1 || currentMode == 2 || currentMode == 7) //HT20
				{
					endChannel = 12;
					channel.options.length = 21;
					for( i = 0; i< 4; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i];
					}
					for( i = 4; i< endChannel; i++ )
					{
						channel.options[i].value = BChannel[i];
						channel.options[i].text = BChannel[i]+'(DFS)';
					}
					channel.options[12].value = 116;
					channel.options[13].value = 132;
					channel.options[14].value = 136;
					channel.options[15].value = 140;
					channel.options[12].text = 116+'(DFS)';
					channel.options[13].text = 132+'(DFS)';
					channel.options[14].text = 136+'(DFS)';
					channel.options[15].text = 140+'(DFS)';

					channel.options[16].value = channel.options[16].text = 149;
					channel.options[17].value = channel.options[17].text = 153;
					channel.options[18].value = channel.options[18].text = 157;
					channel.options[19].value = channel.options[19].text = 161;
					channel.options[20].value = channel.options[20].text = 165;
				}
				else
				{
					channel.options.length = 18; //36101 show single channel
					for(i=0; i< channel.options.length-6; i++)
					{
						channel.options[i].value = BChannel[i];
						if(i>3)
							channel.options[i].text = BChannel[i]+"(DFS)";
						else
							channel.options[i].text = BChannel[i];
					}
					channel.options[12].value =  132;
					channel.options[12].text = "132(DFS)";
					channel.options[13].value =  136;
					channel.options[13].text = "136(DFS)";
					channel.options[14].value = channel.options[14].text =  149;
					channel.options[15].value = channel.options[15].text =  153;
					channel.options[16].value = channel.options[16].text =  157;
					channel.options[17].value = channel.options[17].text =  161;
				}
			}
		}
   }
	
	var find_value = 0;
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
	/*if( check_dfs() == false)
	{
		return false;
	}*/
	//fix bug 29094
	var tag1=0;//when the value is 1, not pop up "guest_tkip_300_150" for 5G 
	var tag2=0;//when the value is 1, not pop up "guest_tkip_aes_300_150" for 5G
	var tag3=0;//when the value is 1, not pop up "wlan_tkip_aes_300_150" for 5G
	var cf=document.forms[0];
	
	/*bug 41791 
	var ssid_bgn = document.forms[0].ssid.value.replace(/\\/g,"\\\\\\\\").replace(/`/g,"\\\\\\`").replace(/"/g,"\\\"");*/
	var ssid_bgn = document.forms[0].ssid.value;
	var ssid_an = document.forms[0].ssid_an.value;
	//var space_flag=0;
	var haven_wpe=0;

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
	
	

	cf.wl_ssid.value = ssid_bgn;
		
	//cf.wl_apply_flag.value = "1";//bug 30924,if click the 'Apply' wl_apply_flag is '1',otherwise is '0'
if( cf.bridge_sec_type.selectedIndex == 0)
{
	if(ssid_bgn == "")
	{
		alert("$ssid_null");
		return false;
	}
	
	/*if(ssid_bgn == wlg1_ssid)
	{
		alert("$ssid_not_allowed_same");
		return false;
	}*/
	
	for(i=0;i<ssid_bgn.length;i++)
	{
		if(isValidChar_space(ssid_bgn.charCodeAt(i))==false)
		{
			alert("$ssid_not_allowed");
			return false;
		}
	}
	
	cf.hid_bridge_sec_type.value=24;//bgn;
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
}
else
{
	cf.hid_bridge_sec_type.value=5;//an;
	if(an_router_flag == 1)
	{		
		cf.wla_ssid.value = ssid_an;
		
		if(ssid_an == "")
		{
			alert("$ssid_null");
			return false;
		}
		
		/*if(ssid_an == wlg1_ssid)
		{
			alert("$ssid_not_allowed_same");
			return false;
		}*/
		
		for(i=0;i<ssid_bgn.length;i++)
		{
			if(isValidChar_space(ssid_an.charCodeAt(i))==false)
			{
				alert("$ssid_not_allowed");
				return false;
			}
		}
		//cf.wla_WRegion.value =cf.WRegion.value;

		//16400
		/*if(cf.ssid_bc_an.checked == true)
			cf.wla_enable_ssid_broadcast.value="1";
		else
			cf.wla_enable_ssid_broadcast.value="0";
		if(cf.enable_video.checked == true)
			cf.hidden_enable_video.value=1;
		else
			cf.hidden_enable_video.value=0;	*/
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
	//	cf.wla_hidden_wlan_channel.value = cf.w_channel_an.value;
			
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
		/*if((cf.opmode_an.value=="2") || (cf.opmode_an.value=="3"))
		{
			if(cf.security_type_an[1].checked == true)
				cf.wla_hidden_wlan_mode.value = "1"; //save for wla_simple_mode
			else if(cf.security_type_an[2].checked == true)
			{
				if(confirm("$wlan_tkip_300_150") == false)
				{
					return false;
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
					if(confirm("$wlan_tkip_300_150") == false)
						return false;
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
	     alert(msg);
	
	}*/
	
		
		/*countryIndex=cf.WRegion.value;
		if( (countryIndex == 5 || countryIndex == 14) && !confirm("$notSupportWLA") )
			return false;
		*/
		//cf.wla_hidden_wlan_mode.value = cf.opmode_an.value;
		
		/*if(cf.security_type_an[1].checked == true && endis_wla_radio == 1)
		{
			/*To fix Bug 33991
		        if ( cf.authAlgm_an.value == 1)
	                {
				if(haven_alert == '0')
	                        	if (!confirm("$wep_or_wps"))
	                                	return false;
			}*/
		
			/*if(cf.security_type[1].checked == false)
				alert("$wep_just_one_ssid_an");
	        }*/
		//var channel_a=cf.w_channel_an.value;
		//var country=cf.wl_WRegion.value;
		//transmit power control, according to the change of country, change values of wl_txctrl and wla_txctrl.
		//wlan_txctrl(cf, wl_txctrl_web, wla_txctrl_web, channel_a, country);

	}

	//bug 33156
	/*if( endis_wl_radio == 1 && cf.ssid_bc.checked == false ||
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
	}*/

	/*if((endis_wl_radio == 1 && cf.wl_hidden_sec_type.value == "6" ) ||
	(an_router_flag ==1 && cf.wla_hidden_sec_type.value == "6" && endis_wla_radio == 1))
	{
		if(haven_wpe == 0)
		{
			if (!confirm("$wpae_or_wps"))
				return false;
		}
	}*/
}

	opener.opener_apply_click = 1;
	cf.submit();
}
