function change_size()
{
	setFooterClass();
	var footer_div = document.getElementById("footer");
	var is_double = ( footer_div.className == "footer_double") ;

	var sUserAgent = navigator.userAgent;
	var height = document.documentElement.clientHeight - 97;
	if(sUserAgent.indexOf("MSIE 9.0") > -1)
	{
		var menu_words = document.getElementById("menu");
		menu_words.className = "menu_ie9_words";
	}
	
	if(isIE_or_Opera() && IE_version() < 9)
	{
		if( (height > 560 && !is_double) || (height > 603 && is_double) )
		{
			document.getElementById("container").style.height = height;
			document.getElementById("middle").style.height = height-5;
			document.getElementById("menu").style.height = is_double ? height-93 : height-50;
		}
		else
		{
			document.getElementById("container").style.height = is_double ? "603px" : height;
			document.getElementById("middle").style.height = is_double ? "598px" : height-9;
			document.getElementById("menu").style.height = "410px";
		}

		/* calculate the width */
		var width = document.documentElement.clientWidth - 40;
		document.getElementById("top").style.width = width > 820 ? width : "820px" ;
		document.getElementById("container").style.width = width > 820 ? width : "820px" ;
		document.getElementById("formframe_div").style.width = width > 820 ? width - 195 : "625px";

	}
	if(isIE() == true)
		document.getElementById("middle").style.height = is_double ? "598px" : (height-9)+"px";
	else
		document.getElementById("middle").style.minHeight = is_double ? "598px" : height - 5;
	document.getElementById("formframe_div").style.bottom = is_double ? "88px" : "45px";
}

function close_all_sub(click_name)/* fold all menus, except the menu which user click */
{
	var sub_list = "internet";
	var button_name = sub_list+"_bt";
	var sub_name = sub_list+"_sub";
	if( sub_name != click_name )
	{
		var div_name = top.document.getElementById(button_name);

		settingClass(div_name, "advanced_white_close_button");

		top.document.getElementById(sub_name).style.display="none";
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
		enabledItemsClass_basic();
	}
	else
	{
		settingClass(button_div, "advanced_white_close_button");
		top.document.getElementById(sub_name).style.display="none";
	}
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

function basic_menu_class_default()
{
	var menu_div = top.document.getElementById("menu");
	var menu_btns = menu_div.getElementsByTagName("div");

	var i;
	for(i=0; i<menu_btns.length; i++)
	{
		if((top.have_broadband ==1) && (top.is_ru_version==1 || top.is_pr_version==1) && i == 2)
		{
			menu_div = top.document.getElementById("internet_bt");
			settingClass(menu_div, "advanced_white_close_button");			
		}
		else{
			if((top.enable_bridge_flag==1 ||top.enabled_wds==1 || top.enable_ap_flag== 1 || top.broadband_mode == "MyDetc" || top.enable_extender_flag == "1" || top.enable_mapt == 1 || top.device_mode == "1") && i == 2)
			{
				var internet_div = top.document.getElementById("internet");
				internet_div.className = internet_div.className + "_grey";
			}
			var height = menu_btns[i].getElementsByTagName("span")[0].clientHeight;

			if( menu_btns[i].id == "market" )
			{
				if( height > 30 )
					menu_btns[i].className = "basic_button_big_green";
				else if( height > 16 )
					menu_btns[i].className = "basic_button_mid_green";
				else
					menu_btns[i].className = "basic_button_green";
			}
			else{
				if( height > 30 )
					menu_btns[i].className = "basic_button_big";
				else if( height > 16 )
					menu_btns[i].className = "basic_button_mid";
				else
					menu_btns[i].className = "basic_button";
			}

		}
	}

	/*if(top.enabled_wds==1 || top.enable_ap_flag== 1)
	{
		var internet_div = top.document.getElementById("internet");
		internet_div.className = internet_div.className + "_grey";
	}
	*/

	if((top.enable_ap_flag== 1 || top.enable_bridge_flag== 1 || top.enable_extender_flag == "1" || top.device_mode == "1") && top.have_dynamic_qos == 1 )
	{
		var intqos_div = top.document.getElementById("intqos");
		intqos_div.className = intqos_div.className + "_grey";
	}

	if(top.enable_ap_flag== 1 || top.enable_bridge_flag== 1 || top.enable_extender_flag == "1" || top.device_mode == "1" )
	{
		var parental_div = top.document.getElementById("parental");
		if(parental_div != null)
			parental_div.className = parental_div.className + "_grey";
	}

	if(top.enabled_wds == 1 || top.enable_bridge_flag== 1 || top.enable_extender_flag == "1" || top.device_mode == "1" )
	{
		var wds_div = top.document.getElementById("guest");
		wds_div.className = wds_div.className + "_grey";
	}
	
	if( top.enable_bridge_flag== 1 || top.device_mode == "1")
	{
		var wds_div = top.document.getElementById("wireless");
		wds_div.className = wds_div.className + "_grey";
	}
	
	if( top.device_mode == "1")
	{
		var wds_div = top.document.getElementById("readyshare");
		wds_div.className = wds_div.className + "_grey";
	}
	
	/*if(top.have_broadband == 1 && top.is_ru_version == 1 && top.is_pr_version == 1)
	{
		menu_div = top.document.getElementById("internet_bt");
		settingClass(menu_div, "advanced_white_close_button");
	}*/
}

/*function open_or_close_sub(name)
{
	if(name=="internet")
	{
		basic_menu_color_change('internet');
		clickSubMenu(broadband);
		goto_formframe('BAS_broadband.htm');
	}

	subItemsClass(name);
}
*/
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

function enabledItemsClass_basic()
{
	var cur_div = top.document.getElementById("3g");
	var cur_div_1= top.document.getElementById("ethernet");
	if(top.have_broadband ==1 && (top.is_pr_version == 1 || top.is_ru_version == 1))
	{
		var height = cur_div.getElementsByTagName("a")[0].clientHeight;
		if(top.broadband_mode == "AutoDetc")
		{
			if( height > 28 )
				cur_div.className = "long_grey";
			else if( height > 14 )
				cur_div.className = "middle_grey"
			else
				cur_div.className = "sub_grey";
		}else{
			if( height > 28 )
				cur_div.className = "long_name";
			else if( height > 14 )
				cur_div.className = "middle_name";
			else
				cur_div.className = "sub_back";
		}
		
		var height_1 = cur_div_1.getElementsByTagName("a")[0].clientHeight;
		if(top.enabled_wds==1 || top.enable_ap_flag== 1 || top.enable_extender_flag == "1")
		{
			if( height_1 > 28 )
				cur_div_1.className = "long_grey";
			else if( height_1 > 14 )
				cur_div_1.className = "middle_grey"
			else
				cur_div_1.className = "sub_grey";
		}else{
			if( height_1 > 28 )
				cur_div_1.className = "long_name";
			else if( height_1 > 14 )
				cur_div_1.className = "middle_name";
			else
				cur_div_1.className = "sub_back";
		}

	}else{
		var height  = cur_div.getElementsByTagName("a")[0].clientHeight;
		{
			if( height > 28 )
				cur_div.className = "long_name";
			else if( height > 14 )
				cur_div.className = "middle_name";
			else
				cur_div.className = "sub_back";
		}
	}
}

function clickSubMenu(clicked_item)
{
	if( top.old_div != "")
	{
		var old_div_class = top.old_div.className;
		if( old_div_class == "sub_back_purple")
			top.old_div.className = "sub_back";
		else if ( old_div_class == "sub_back_purple_double")
			top.old_div.className = "middle_name";
		else if ( old_div_class == "sub_back_purple_triple")
			top.old_div.className = "long_name";
	}
	if( clicked_item == "0" )
	{
		top.old_div = "";
		return;
	}
	var current_class = clicked_item.className;
	if( current_class == "sub_back" )
		clicked_item.className = "sub_back_purple";
	else if( current_class == "middle_name")
		clicked_item.className = "sub_back_purple_double";
	else if( current_class == "long_name")
		clicked_item.className = "sub_back_purple_triple";
	top.old_div = clicked_item;
}

function basic_menu_color_change( change_id )
{
	basic_menu_class_default();

	var clicked_item = top.document.getElementById(change_id);
	if(change_id=="broadband" || change_id=="ethernet" || change_id=="3g")
	{
		var parent_id = top.document.getElementById(change_id).parentNode.parentNode.id;
		var btn_id = parent_id.replace('sub', 'bt');
		var btn_div = top.document.getElementById(btn_id);
		settingClass(btn_div, "advanced_white_open_button");

		clickSubMenu(clicked_item);
	}
	else
	{
		clicked_item.className = clicked_item.className + "_purple";
		clickSubMenu(clicked_item);
	}
}

function click_action(id)
{
        if( enable_action )
        {
                if( id == "home")
                {
                        basic_menu_color_change('home');
                        goto_formframe('basic_wait.htm');
                }
		else if( id == "internet" && top.enabled_wds == 0 && top.enable_ap_flag != 1 && top.broadband_mode != "MyDetc" && top.enable_bridge_flag == 0 && top.enable_extender_flag != "1" && top.enable_mapt !=1 && top.device_mode != "1")
		{
			if(top.have_broadband==1 && (top.is_ru_version==1 || top.is_pr_version ==1))
			{
				open_or_close_sub('internet');
				basic_menu_color_change('ethernet');
				if(top.dsl_enable_flag == 0)
					goto_formframe('BAS_basic.htm');
				else
					goto_formframe('BAS_basic_dsl.htm');
			}
			else
			{
				basic_menu_color_change('internet');
				if(top.dsl_enable_flag == 0)
					goto_formframe('BAS_basic.htm');
				else
					goto_formframe('BAS_basic_dsl.htm');
			}
		}
                else if( id == "broadband" && top.have_broadband ==1)
		{
			basic_menu_color_change('broadband');
			goto_formframe('BAS_broadband.htm');
		}
		else if( id == "ethernet" && top.have_broadband ==1 && top.enabled_wds == 0 && top.enable_ap_flag != 1)
		{
			basic_menu_color_change('ethernet');
			if(top.dsl_enable_flag == 0)
				goto_formframe('BAS_basic.htm');
			else
				goto_formframe('BAS_basic_dsl.htm');
		}
		else if( id == "3g" && top.have_broadband ==1 && top.broadband_mode != "AutoDetc" )
		{
			basic_menu_color_change('3g');
			goto_formframe('BAS_3g.htm');
		}
                else if( id == "wireless" && top.enable_bridge_flag == 0 && top.device_mode != "1" )
                {
                        basic_menu_color_change('wireless');
			if( endis_wl_radio == '1' || endis_wla_radio == '1' )
				goto_formframe('WLG_wireless.htm');
			else
				goto_formframe('WLG_adv.htm');
                }
                else if( id == "attached" )
                {
			if(top.have_dynamic_qos == 1)
			{
				basic_menu_color_change('attached');
				goto_formframe("QOS_device.htm");
			} else {
				basic_menu_color_change('attached');
				goto_formframe('DEV_device.htm');
			}
                }
		else if( id == "intqos" && top.have_dynamic_qos == 1 && top.enable_ap_flag != 1 && top.enable_bridge_flag == 0 && top.enable_extender_flag == "0" && top.device_mode != "1" )
		{
			basic_menu_color_change('intqos');
			if(top.have_advanced_qos == "1")
				goto_formframe("QOS_wait.htm");
			else
                                goto_formframe("QOS_dynamic.htm");

		}
		else if( id == "parental" && top.enable_ap_flag != 1 && top.enable_bridge_flag == 0 && top.enable_extender_flag == "0" && top.device_mode != "1" )
                {
                        basic_menu_color_change('parental');
			open_window('parental_ctrl.htm');
                }
                else if( id == "readyshare" && top.device_mode != "1" )
                {
                        basic_menu_color_change('readyshare');
                        goto_formframe('USB_basic.htm');
                }
                else if( id == "guest" && top.enabled_wds == 0 && top.enable_bridge_flag == 0 && top.enable_extender_flag == "0" && top.device_mode != "1")
                {
                        basic_menu_color_change('guest');
                        goto_formframe('WLG_wireless_guest1.htm');
                }
		else if( id == "quick_qos" )
		{
			basic_menu_color_change('quick_qos');
			goto_formframe('Quick_wait.htm');
		}
		else if( id == "green" && top.have_green_download ==1 && top.device_mode != "1" )
		{
			if( download_type == "http" )
				goto_formframe("GREEN_http_basic.htm");
			else if( download_type == "emule" )
				goto_formframe("GREEN_emule_basic.htm");
			else
				goto_formframe("GREEN_bt_basic.htm");
			basic_menu_color_change('green');
		}
		else if( id == "fastlane")
		{
			basic_menu_color_change('fastlane');
			goto_formframe('FL_fastLane.htm');
		}
		else if( id == "no_internet" && top.enabled_wds == 0 && top.enable_ap_flag != 1 && top.broadband_mode != "MyDetc" && top.enable_bridge_flag == 0 && top.enable_extender_flag == "0")
		{
			if(top.have_broadband==1 && (top.is_ru_version==1 || top.is_pr_version==1))
			{
				open_or_close_sub('internet');
				basic_menu_color_change('ethernet');
			}
			else
				basic_menu_color_change('internet');

			if(portstatus == 0)
				location.href = "BRS_03A_A_noWan.html"
			else
				document.forms[0].submit();
		}
		else if( id == "mobile_internet" && top.enable_bridge_flag == 0)
		{
			top.click_lte_sim=0;
			basic_menu_color_change('internet');
			goto_formframe('check_mobile_internet_wait.htm');
		}

        }
}

function setIconClass(argv)
{
	var height, words_div;
	for(num=0; num<arguments.length; num++)
	{
		words_div = document.getElementById(arguments[num]);
		if( words_div == null ) continue;
		height = words_div.getElementsByTagName("span")[0].clientHeight;
		if( height > 20 )
			words_div.className = "icon_long_status";
		else
			words_div.className = "icon_status";
	}
}

