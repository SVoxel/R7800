function close_all_sub(click_name)/* fold all menus, except the menu which user click */
{
	var sub_list = "setup,usb,security,green,admin,advanced";
	var sub_array = sub_list.split(',');
	for ( i=0; i< sub_array.length; i++)
	{
		var button_name = sub_array[i]+"_bt";
		var sub_name = sub_array[i]+"_sub";
		if( sub_name != click_name )
		{
			var div_name = top.document.getElementById(button_name);
			if( !div_name)
				continue;
			settingClass(div_name, "advanced_white_close_button");

			top.document.getElementById(sub_name).style.display="none";
		}
	}
}

function open_or_close_sub(name)
{
	/* to fix bug 23268, when user want to unfold one menus, fold the other menus. */
	var button_name= name+"_bt";
	var sub_name= name+"_sub";
	var open_flag= top.document.getElementById(sub_name).style.display;
	
	close_all_sub(sub_name);/* fold all menus first, except the menu which user click*/

	var button_div = top.document.getElementById(button_name);
	if( open_flag == "none")
	{
		settingClass(button_div, "advanced_white_open_button");
		top.document.getElementById(sub_name).style.display="";
		subItemsClass(sub_name);
		enabledItemsClass();
	}
	else
	{
		settingClass(button_div, "advanced_white_close_button");
		top.document.getElementById(sub_name).style.display="none";
	}
	change_menu_height();
}

/* change the min-height of the fromframe if unfold "Advanced Setup" */
function change_menu_height()
{
	var footer_div = document.getElementById("footer");
	var is_double = (footer_div.className == "footer_double");
	var con_height = document.documentElement.clientHeight - 97;
	var menu_height = document.getElementById("menu").clientHeight > 510 ? document.getElementById("menu").clientHeight : 510;
	var sUserAgent = navigator.userAgent;
	
	if(sUserAgent.indexOf("MSIE 9.0") > -1)
		document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (con_height)+"px";
	else if(isIE_or_Opera() && IE_version() < 9)
	{
		var height = is_double ? document.documentElement.clientHeight - 190 : document.documentElement.clientHeight - 147;
		menu_height = height > menu_height ? height : menu_height;
		document.getElementById("container").style.height = is_double ? (menu_height+93)+"px" : (con_height)+"px";
		document.getElementById("middle").style.height = is_double ?  (menu_height+87)+"px" : (con_height-9)+"px";
	}
	document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (menu_height+ 45)+"px";
	if(menu_height > 510)
		document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (menu_height+ 45)+"px";
	else
	{
		if(isIE() == true)
			document.getElementById("middle").style.height = is_double ? (menu_height + 87)+"px" : (con_height - 9)+"px";
		else
			document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (con_height - 5)+"px";
	}
}

function change_size()
{
	setFooterClass();
	var footer_div = document.getElementById("footer");
	var is_double = (footer_div.className == "footer_double");

	var sUserAgent = navigator.userAgent;
	if(sUserAgent.indexOf("MSIE 9.0") > -1)
	{
		var menu_words = document.getElementById("menu");
		menu_words.className = "menu_ie9_words";
	}
	
	if(isIE_or_Opera() && IE_version() < 9)
	{
		/* to calculate the width */
		var width = document.documentElement.clientWidth - 40;
		document.getElementById("top").style.width = width > 820 ? width : "820px" ;
		document.getElementById("container").style.width = width > 820 ? width : "820px" ;
		document.getElementById("formframe_div").style.width = width > 820 ? width - 195 : "625px";
	}
	document.getElementById("formframe_div").style.bottom = is_double ? "88px" : "45px";
	change_menu_height();
}

function settingClass(obj, class_name)
{
	var height = obj.getElementsByTagName("span")[0].clientHeight;
	if(height > 30)
		obj.className = class_name + "_triple";
	else if(height > 15)
		obj.className = class_name + "_double";
	else
		obj.className = class_name;
}

function subItemsClass(argv)
{
	var sub_menu;
	var items;
	var words_height;
	var i, num;

	for(num=0; num<arguments.length; num++)
	{
		sub_menu = top.document.getElementById(arguments[num]);
		if(sub_menu.style.display == "none") continue;
		items = sub_menu.getElementsByTagName("dt");

		for(i=0; i<items.length; i++)
		{
			if( top.old_div == items[i] ) break;
			words_height = items[i].getElementsByTagName("a")[0].clientHeight;
			if(words_height > 28 )
				items[i].className = "long_name";
			else if( words_height > 14 )
				items[i].className = "middle_name";
			else
				items[i].className = "sub_back";
		}
	}
}

function clickSubMenu(current_div, change_id)
{
	if( top.old_div != "")
	{
		var old_div_class = top.old_div.className;
		if( old_div_class == "sub_back_purple" || old_div_class == "sub_back_purple_usb")
			top.old_div.className = "sub_back";
		else if ( old_div_class == "sub_back_purple_double" || old_div_class == "sub_back_purple_usb_double")
			top.old_div.className = "middle_name";
		else if ( old_div_class == "sub_back_purple_triple")
			top.old_div.className = "long_name";
	}
	if( current_div == "0" )
	{
		top.old_div = "";
		return;
	}
	var current_class = current_div.className;
	if( current_class == "sub_back" )
	{
		if( change_id == "usb_bas" || change_id == "usb_adv" || change_id == "media" || change_id == "userset")
			current_div.className = "sub_back_purple_usb";
		else
			current_div.className = "sub_back_purple";
	}
	else if( current_class == "middle_name")
	{
		if( change_id == "usb_bas" || change_id == "usb_adv" || change_id == "media" || change_id == "userset")
			current_div.className = "sub_back_purple_usb_double";
		else
			current_div.className = "sub_back_purple_double";
	}
	else if( current_class == "long_name")
		current_div.className = "sub_back_purple_triple";
	top.old_div = current_div;
}
var array_name = ["wds_items", "ap_items", "bridge_items", "extender_items", "mapt_items", "devmode_items"];
var enable_flags = ["enabled_wds", "enable_ap_flag", "enable_bridge_flag", "enable_extender_flag", "enable_mapt", "device_mode"];
var wds_items = [ "internet", "wan", "block_site", "block_services", "forwarding_triggering","qos","vpn", "dns", "static", "remote", "upnp", "traffic", "wl_bridge", "guest", "email", "schedule", "log"]; 
var ap_items = [ "internet", "wan", "block_site", "block_services", "forwarding_triggering","qos","vpn", "remote", "upnp", "traffic", "wl_bridge", "lan", "ipv6", "parental", "access_control", "usb_set", "devname"];
var extender_items = [ "internet", "wan", "block_site", "block_services", "forwarding_triggering","vpn",  "remote", "upnp", "traffic", "wl_bridge", "lan", "ipv6", "led", "fbwifi"];
var bridge_items = ["internet", "wan", "block_site", "block_services", "forwarding_triggering","qos","vpn", "dns", "static", "remote", "upnp", "traffic", "wireless", "guest", "lan", "parental","access_control",  "email", "schedule", "wds", "ipv6", "wlap"];
var mapt_items= [ "internet" ];
var devmode_items = [ "internet", "wireless", "guest", "wan", "lan", "qos", "readyshare_stor", "usb_bas", "usb_adv", "media", "print", "vault", "block_site", "block_services", "schedule", "email", "wladv", "rae", "forwarding_triggering", "dns", "vpn", "static", "remote", "upnp", "traffic", "wl_bridge", "ipv6", "parental", "access_control", "usb_set", "vlan", "led", "devname" ];
function enabledItemsClass()
{
	var i, j;
	var enable_flag;

	for(i=0; i<array_name.length; i++)
	{
		var item_group;
		enable_flag = eval("top."+enable_flags[i]);
		item_group = eval(array_name[i]);
		for(j=0; j<item_group.length; j++)
		{
			var cur_div = top.document.getElementById(item_group[j]);
			if( !cur_div)// if not find cur_div, continue
				continue;
			var height  = cur_div.getElementsByTagName("a")[0].clientHeight;
			if( cur_div.style.display == "none" ) break;

			if(enable_flag == "1" )
			{
				if( height > 28 )
					cur_div.className = "long_grey";
				else if( height > 14 )
					cur_div.className = "middle_grey"
				else
					cur_div.className = "sub_grey";
			}
		}
	}
	if(top.have_lte_flag ==1 )
	{
		var cur_div = top.document.getElementById("mobile_internet");
		var cur_div_1= top.document.getElementById("internet");
		var height = cur_div.getElementsByTagName("a")[0].clientHeight;
		var height_1 = cur_div_1.getElementsByTagName("a")[0].clientHeight;
		if(top.broadband_wan_type == "AutoDetc")
		{
			if( height > 28 )
				cur_div.className = "long_grey";
			else if( height > 14 )
				cur_div.className = "middle_grey"
			else
				cur_div.className = "sub_grey";
		}
		else
		{
			if( height > 28 )
				cur_div.className = "long_name";
			else if( height > 14 )
				cur_div.className = "middle_name";
			else
				cur_div.className = "sub_back";
		}
		if(top.broadband_wan_type == "MyDetc")
		{
			if( height_1 > 28 )
				cur_div_1.className = "long_grey";
			else if( height_1 > 14 )
				cur_div_1.className = "middle_grey";
			else
				cur_div_1.className = "sub_grey";
		}
		else
		{
			if( height_1 > 28 )
				cur_div_1.className = "long_name";
			else if( height_1 > 14 )
				cur_div_1.className = "middle_name";
			else
				cur_div_1.className = "sub_back";
		}
	}
}

function menu_class_default()
{
	var menu_div;
	var content_length;

	menu_div = top.document.getElementById("adv_home");
	settingClass(menu_div, "advanced_black_button");

	if(top.have_lte_flag !=1)
	{
		menu_div = top.document.getElementById("adv_setup_wizard");
	if(master == "admin" && (top.enabled_wds == 1 || top.enable_ap_flag == 1 || top.enable_bridge_flag == 1 || top.enable_extender_flag == "1" || top.device_mode == "1"))
			settingClass(menu_div, "advanced_grey_button");
		else
			settingClass(menu_div, "advanced_black_button");
	}
    if(typeof(top.have_wifi_flag) != "undefined" && top.have_wifi_flag == 1){
	menu_div = top.document.getElementById("adv_wps");
	if( top.enabled_wps == 1 && top.enabled_wds == 0 && top.enable_bridge_flag == 0 && top.device_mode != "1" )
		settingClass(menu_div, "advanced_black_button");
	else
		settingClass(menu_div, "advanced_grey_button");
     }
        var extensible_items = ["setup_bt", "usb_bt", "security_bt", "green_bt", "admin_bt", "advanced_bt"];
	var i;
	for(i=0; i<extensible_items.length; i++)
	{
		menu_div = top.document.getElementById(extensible_items[i]);
		if( !menu_div )
			continue;
		settingClass(menu_div, "advanced_white_close_button");
	}

	enabledItemsClass();
}

function menu_color_change( change_id )
{
	menu_class_default();

	var current_div = top.document.getElementById(change_id);
	
	if( change_id == "adv_home" ||  change_id == "adv_setup_wizard" || change_id == "adv_wps" )
	{
		settingClass(current_div, "advanced_purple_button");
		clickSubMenu(0, change_id);
	}
	else
	{
		var parent_id = top.document.getElementById(change_id).parentNode.parentNode.id;
                var btn_id = parent_id.replace('sub', 'bt');
                var btn_div = top.document.getElementById(btn_id);
		settingClass(btn_div, "advanced_white_open_button");

		clickSubMenu(current_div, change_id);
	}
}

function click_adv_action(id)
{
	if(id == "adv_setup_wizard")
		setup_wizard_flag = 1;
	else
		setup_wizard_flag = 0;

        if( enable_action == 1 )
        {	
		for(var i=0; i<array_name.length; i++)
		{
			var item_group;
			var enable_flag = eval("top."+enable_flags[i]);
			var item_group = eval(array_name[i]);		
			for( var j=0; j<item_group.length; j++)
			{
				if(item_group[j] == id && enable_flag == 1 )
				{
					id = "NOTACTIVE";
					break;
				}
			}
			
			if( id == "NOTACTIVE")
				break;
		}
		
		
		if( id == "adv_home" )
                {
			menu_color_change('adv_home');
			if(enable_bridge_flag == "1")
				goto_formframe('RST_bridge_status.htm');
			else
				goto_formframe('RST_status.htm');	
		}
                else if( id == "adv_setup_wizard" && master == "admin" && top.enable_ap_flag != "1" && top.enabled_wds != "1" && top.enable_bridge_flag != "1" && top.enable_extender_flag != "1" && top.device_mode != "1")
                {
			menu_color_change('adv_setup_wizard');
			if(top.dsl_enable_flag == "1")
				goto_formframe('DSL_WIZ_sel.htm');
			else
			   if (((top.netgear_region.toUpperCase() == "WW" || top.netgear_region == "") && top.gui_region == "Russian" ) || top.netgear_region.toUpperCase() == "RU")
				goto_formframe('RU_WIZ_sel.htm');
			  else
				goto_formframe('WIZ_sel.htm');
		}
                
		else if( id == "adv_wps" && top.enabled_wps == "1" && top.enabled_wds != "1" && top.enable_bridge_flag != "1" && top.device_mode != "1" )
                {
			goto_formframe('Add_WPS_Client.htm');
			menu_color_change('adv_wps');
                }
                else if( id == "internet")
                {
			if(!(top.have_lte_flag==1 && top.broadband_wan_type == "MyDetc"))
			{
				if(top.dsl_enable_flag == 0)
					goto_formframe('BAS_basic.htm');
				else
					goto_formframe('BAS_basic_dsl.htm');
				menu_color_change('internet');
			}
                }
                else if( id == "wireless")
                {
			goto_formframe("WLG_wireless.htm");
			menu_color_change('wireless');
                }
                else if( id == "guest" )
                {
			goto_formframe("WLG_wireless_guest1.htm");
			menu_color_change('guest');
                }
                /*else if( id == "guest_a" )
                {
                        goto_formframe("WLG_wireless_guestA1.htm");
                        menu_color_change('guest_a');
                }*/
                else if( id == "wan" )
                {
			goto_formframe("WAN_wan.htm");
			menu_color_change('wan');
                }
                else if( id == "lan" )
                {
			goto_formframe("LAN_lan.htm");
			menu_color_change('lan');
                }
		else if( id == "qos")
		{
			if(top.have_advanced_qos == "1")
				goto_formframe("QOS_wait.htm");
			else
				goto_formframe("QOS_dynamic.htm");
			menu_color_change('qos');
		}
                else if( id == "usb_bas" )
                {
			goto_formframe("USB_basic.htm");
			menu_color_change('usb_bas');
                }
                else if( id == "usb_adv" )
                {
			goto_formframe("USB_advanced.htm");
			menu_color_change('usb_adv');
                }
		else if( id == "userset" )
                {
                       goto_formframe("hdd_usr_setup.htm");
                       menu_color_change('userset');
                }
                else if( id == "media" )
                {
			goto_formframe("UPNP_media.htm");
			menu_color_change('media');
                }
		else if( id == "cloud" )
		{
			goto_formframe("/cgi-bin/RMT_invite.htm");
			menu_color_change('cloud');
		}
		else if( id == "print" )
		{
			goto_formframe("RMT_print.htm");
			menu_color_change('print');
		}
		else if( id == "vault" )
		{
			goto_formframe("RMT_vault.htm");
			menu_color_change('vault');
		}
		else if( id == "mobile" )
		{
			goto_formframe("BAS_mobile.htm");
			menu_color_change('mobile');
		}
		else if( id == "parental" )
                {
			open_window('parental_ctrl.htm');
			menu_color_change('parental');
                }
                else if( id == "block_site" )
                {
			goto_formframe("BKS_keyword.htm");
			menu_color_change('block_site');
                }
                else if( id == "block_services" ) 
                {
			goto_formframe("BKS_service.htm");
			menu_color_change('block_services');
                }
                else if( id == "schedule")//&& top.enable_ap_flag != 1
                {
			goto_formframe("FW_schedule.htm");
			menu_color_change('schedule');
                }
                else if( id == "email" )
                {
			goto_formframe("FW_email.htm");
			menu_color_change('email');
                }
		else if( id == "ipmac")
		{
			goto_formframe("ipmac.htm");
			menu_color_change('ipmac');
		}
                else if( id == "status" )
                {
			if(top.have_lte_flag ==1)
				goto_formframe("get_mobile_status.html");
			else{
				if(enable_bridge_flag == "1")
					goto_formframe('RST_bridge_status.htm');
				else
					goto_formframe('RST_status.htm');
			}
				
			menu_color_change('status');
                }
                else if( id == "log" )
                {
			goto_formframe("FW_log.htm");
			menu_color_change('log');
                }
                else if( id == "attached" )
                {
			if(top.have_dynamic_qos == 1)
			{
				goto_formframe("QOS_device.htm");
				menu_color_change('attached');
			} else {
				goto_formframe("DEV_device.htm");
				menu_color_change('attached');
			}
                }
                else if( id == "bak_set" )
                {
			goto_formframe("BAK_backup.htm");
			menu_color_change('bak_set');
                }
                else if( id == "passwd" )
                {
			goto_formframe("PWD_password.htm");
			menu_color_change('passwd');
                }
                else if( id == "upgrade" )
                {
			goto_formframe("UPG_upgrade.htm");
			menu_color_change('upgrade');
                }
                else if( id == "plc" )
                {
			goto_formframe("PLC_wait_scan.htm");
			menu_color_change('plc');
                }
                else if( id == "wladv" )
                {
			goto_formframe("WLG_adv.htm");
			menu_color_change('wladv');
                }
		else if( id == "wlap"  )
                {
                        goto_formframe("WLG_wap.htm");
                        menu_color_change('wlap');
                }
		else if( id == "wl_bridge" )
                {
                        goto_formframe("WLG_bridge.htm");
                        menu_color_change('wl_bridge');
                }
                else if( id == "wds")//&& top.enable_ap_flag != 1 for bug 30286 moved here
                {
			goto_formframe("WLG_wds.htm");
			menu_color_change('wds');
                }
                else if( id == "forwarding_triggering" )
                {
			goto_formframe("FW_forward.htm");
			menu_color_change('forwarding_triggering');
                }
                else if( id == "dns" )
                {
			goto_formframe("DNS_ddns.htm");
			menu_color_change('dns');
                }
		else if( id == "static" )
		{
			goto_formframe("STR_routes.htm");
			menu_color_change('static');
		}
		else if( id == "remote" )
		{
			goto_formframe("FW_remote.htm");
			menu_color_change('remote');
		}
		else if( id == "usb_set" )
		{
			goto_formframe("USB_settings.htm");
			menu_color_change('usb_set');
		}
		else if( id == "upnp" )
		{
			goto_formframe("UPNP_upnp.htm");
			menu_color_change('upnp');
		}
		else if( id == "ipv6" )
		{
			goto_formframe("IPv6_index.htm");
			menu_color_change('ipv6');
		}
		else if( id == "tr069" )
		{
			goto_formframe("tr069.htm");
			menu_color_change('tr069');
		}
		else if( id == "traffic" )
		{
			goto_formframe("traffic.htm");
			menu_color_change('traffic');
		}
		else if( id == "green_basic")
		{
			goto_formframe("green_download.htm");
			menu_color_change(id);
		}
		else if( id == "green_manager")
		{
			if( download_type == "emule" )
				goto_formframe("GREEN_emule_basic.htm");
			else if( download_type == "http" )
				goto_formframe("GREEN_http_basic.htm");
			else
				goto_formframe("GREEN_bt_basic.htm");
			menu_color_change(id);
		}
		else if( id == "lte_broadband" && top.have_lte_flag==1)
		{
			goto_formframe("BAS_lte_broadband.htm");
			menu_color_change(id);
		}
		else if( id == "mobile_internet" && top.broadband_wan_type != "AutoDetc")
		{
			top.click_lte_sim=0;
			goto_formframe("check_mobile_internet_wait.htm");
			menu_color_change(id);
		}
		else if( id == "sim" )
		{
			top.click_lte_sim=1;
			goto_formframe("check_sim_wait.htm");
			menu_color_change(id);
		}
		else if( id == "mobile_upg" )
		{
			goto_formframe("UPG_module.htm");
			menu_color_change(id);
		}
		else if( id == "access_control" )
		{
			goto_formframe("access_control.htm");
			menu_color_change('access_control');
		}
		else if( id == "led" )
		{
			goto_formframe("LED_control_setting.htm");
			menu_color_change('led');
		}
		else if( id == "vpn" )
		{
			goto_formframe("vpn_service.htm");
			menu_color_change('vpn');
		}
		else if( id == "pptp_vpn" )
		{
			goto_formframe("vpn_pptp.htm");
			menu_color_change(id);
		}
		else if( id == "fbwifi")
		{
			goto_formframe("fbwifi.htm");
                        menu_color_change('fbwifi');
		}
		else if( id == "rae" )
		{
			goto_formframe("rae_main.htm");
			menu_color_change('rae');
		}
		else if( id == "vlan" )
                {
                        goto_formframe("VLAN_IPTV.htm");
                        menu_color_change('vlan');
                }
		else if( id == "devname")
		{
			goto_formframe("edit_device_name.htm");
			menu_color_change('devname');
		}
		else if( id == "priozone")
		{
			goto_formframe("priority_zone.htm");
			menu_color_change('priozone');	
		}
		else if( id == "devmode" )
		{
			goto_formframe("device_mode_setting.htm");
			menu_color_change('devmode');
		}
						                 
        }
	change_menu_height();
}

