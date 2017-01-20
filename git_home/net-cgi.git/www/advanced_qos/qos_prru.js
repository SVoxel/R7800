function gray_rule_table()
{
	var cf = document.forms[0];

	var queue_lists = getElementsByTagAndName("div", "queue_list");
	//if(cf.qosEnable.checked == true)
	//{
		document.getElementById("qos_rules").style.color="black";
		document.getElementById("qos_show_rule").style.color="black";

		for(var q=0; q<queue_lists.length; q++)
		{
			c_nodes = queue_lists[q].childNodes;
			for(var g=0; g<c_nodes.length; g++)
			{
				c_nodes[g].className="grey_DragBox";
			}
		}
	/*}
	else
	{
		document.getElementById("qos_rules").style.color="gray";
		document.getElementById("qos_show_rule").style.color="gray";

		for(var q=0; q<queue_lists.length; q++)
		{
			c_nodes = queue_lists[q].childNodes;
			for(var g=0; g<c_nodes.length; g++)
			{
				c_nodes[g].className="DragBox";
			}
		}
	}*/
}

function trun_bandwidth()
{
	var cf = document.forms[0];

	//if(cf.qosEnable.checked == true){
		setDisabled(false, cf.tcbw_value, cf.tcbw_unit);
	/*}
	else{
		setDisabled(true, cf.tcbw_value, cf.tcbw_unit);
	}*/
}

function show_fasttrack()
{
	var cf = document.forms[0];

	/*if(cf.qos_mode[1].checked == true)
	{
		document.getElementById("blue_line_1").style.display="none";
		document.getElementById("qos_app").style.display="none";
		document.getElementById("qos_ser_policy").style.display="none";
		document.getElementById("qos_ser_pri").style.display="none";
		document.getElementById("qos_port_type").style.display="none";
		document.getElementById("qos_port_start").style.display="none";
		document.getElementById("qos_port_end").style.display="none";
		document.getElementById("qos_device").style.display="";
		document.getElementById("qos_dev_table").style.display="none";
		document.getElementById("blue_line_2").style.display="";
		document.getElementById("qos_dev_policy").style.display="none";
		document.getElementById("qos_dev_mac").style.display="none";
		document.getElementById("qos_dev_name").style.display="none";
		document.getElementById("qos_dev_pri").style.display="none";
		document.getElementById("qos_button_1").style.display="none";
		document.getElementById("qos_rules").style.display="none";
		document.getElementById("qos_show_rule").style.display="none";
		document.getElementById("qos_button_2").style.display="none";
		document.getElementById("blue_line_4").style.display="none";

		document.getElementById("qos_fast_status").style.display="";
		document.getElementById("qos_tru_table").style.display="";
		document.getElementById("qos_fast_onoff").style.display="";
		document.getElementById("blue_line_3").style.display="";
		document.getElementById("qos_trust").style.display="";
		document.getElementById("qos_band").style.display="";
		document.getElementById("qos_wps").style.display="";
		document.getElementById("qos_wps1").style.display="";
		document.getElementById("qos_wps2").style.display="";
	}
	else
	{*/
		change_qos_type();
		document.getElementById("qos_fast_status").style.display="none";
		document.getElementById("qos_tru_table").style.display="none";
		document.getElementById("qos_fast_onoff").style.display="none";
		document.getElementById("blue_line_3").style.display="none";
		document.getElementById("qos_trust").style.display="none";
		document.getElementById("qos_band").style.display="none";
		document.getElementById("qos_wps").style.display="none";
		document.getElementById("qos_wps1").style.display="none";
		document.getElementById("qos_wps2").style.display="none";
	//}

}

function change_qos_mode()
{
	var cf = document.forms[0];

	//if(cf.qos_mode[0].checked == true)
//	{
		cf.hid_qos_mode.value = "0";
		document.getElementById("qos_select").style.display="";
		click_fasttrack=0;
	/*}
	else
	{
		cf.hid_qos_mode.value = "1";
		document.getElementById("qos_select").style.display="none";
		click_fasttrack=1;
	}*/
	show_fasttrack();
	document.getElementsByName("help_iframe")[0].src="/help/_qos_help.html";

}

function setGray()
{
	var cf = document.forms[0];

	trun_bandwidth();
	change_qos_mode();
	gray_rule_table();

	//if(cf.qosEnable.checked == true)
	//{
		cf.qos_endis_on.value = "1";
	//	document.getElementById("qos_modes").style.color="black";
		//setDisabled(false,cf.qos_mode[0],cf.qos_mode[1]);
		//if(cf.qos_mode[0].checked == true)
		//{
			document.getElementById("qos_select").style.color="black";
			setDisabled(false,cf.qos_type[0],cf.qos_type[1]);

			if(cf.hid_qos_type.value == "0")
			{
				document.getElementById("qos_app").style.color="black";
				document.getElementById("qos_ser_policy").style.color="black";
				document.getElementById("qos_ser_pri").style.color="black";
				document.getElementById("qos_port_type").style.color="black";
				document.getElementById("qos_port_start").style.color="black";
				document.getElementById("qos_port_end").style.color="black";
				cf.qos_apps.disabled = false;
				cf.ser_poli_name.disabled = false;
				cf.ser_priority.disabled = false;
				cf.port_type.disabled = false;
				cf.portstart.disabled = false;
				cf.portend.disabled = false;
			}
			else if(cf.hid_qos_type.value == "1")
			{
				document.getElementById("qos_device").style.color="black";
				document.getElementById("qos_dev_table").style.color="black";
				document.getElementById("qos_dev_policy").style.color="black";
				document.getElementById("qos_dev_mac").style.color="black";
				document.getElementById("qos_dev_name").style.color="black";
				document.getElementById("qos_dev_pri").style.color="black";
				cf.dev_poli_name.disabled = false;
				cf.mac.disabled = false;
				cf.dev_name.disabled = false;
				cf.dev_priority.disabled = false;
			}
			if( master == "admin" )
			{	
				cf.Add.disabled = false;
				cf.Add.className = "new_apply_bt ui-corner-all";
				cf.Reset.disabled = false;
				cf.Reset.className = "new_apply_bt2 ui-corner-all";

				cf.Edit.disabled = false;
				cf.Edit.className = "new_apply_bt ui-corner-all";
				cf.Deleteone.disabled = false;
				cf.Deleteone.className = "new_apply_bt ui-corner-all";
				cf.DeleteAll.disabled = false;
				cf.DeleteAll.className = "new_apply_bt2 ui-corner-all";
			}
	/*	}
		else
		{
			document.getElementById("qos_fast_status").style.color="black";
			document.getElementById("qos_fast_onoff").style.color="black";
			document.getElementById("qos_device").style.color="black";
			document.getElementById("qos_trust").style.color="black";
			document.getElementById("qos_band").style.color="black";
			document.getElementById("qos_wps").style.color="black";
			document.getElementById("qos_wps1").style.color="black";
			document.getElementById("qos_wps2").style.color="black";
			cf.qos_fast.disabled = false;
			cf.bandwidth_reser.disabled = false;
			cf.wps_fastlane[0].disabled = false;
			cf.wps_fastlane[1].disabled = false;
			cf.MacSelect.disabled = false;
		}*/
	/*}
	else
	{
		cf.qos_endis_on.value = "0";
	//	document.getElementById("qos_modes").style.color="gray";
	//	setDisabled(true,cf.qos_mode[0],cf.qos_mode[1]);
		//if(cf.qos_mode[0].checked == true)
		//{
			document.getElementById("qos_select").style.color="gray";
                        setDisabled(true,cf.qos_type[0],cf.qos_type[1]);
			if(cf.hid_qos_type.value == "0")
			{
				document.getElementById("qos_app").style.color="gray";
				document.getElementById("qos_ser_policy").style.color="gray";
				document.getElementById("qos_ser_pri").style.color="gray";
				document.getElementById("qos_port_type").style.color="gray";
				document.getElementById("qos_port_start").style.color="gray";
				document.getElementById("qos_port_end").style.color="gray";
				cf.qos_apps.disabled = true;
				cf.ser_poli_name.disabled = true;
				cf.ser_priority.disabled = true;
				cf.port_type.disabled = true;
				cf.portstart.disabled = true;
				cf.portend.disabled = true;
			}
			else if(cf.hid_qos_type.value == "1")
			{
				document.getElementById("qos_device").style.color="gray";
				document.getElementById("qos_dev_table").style.color="gray";
				document.getElementById("qos_dev_policy").style.color="gray";
				document.getElementById("qos_dev_mac").style.color="gray";
				document.getElementById("qos_dev_name").style.color="gray";
				document.getElementById("qos_dev_pri").style.color="gray";
				cf.dev_poli_name.disabled = true;
				cf.mac.disabled = true;
				cf.dev_name.disabled = true;
				cf.dev_priority.disabled = true;
			}

			cf.Add.disabled = true;
			cf.Add.className = "new_greyapply_bt ui-corner-all";
			cf.Reset.disabled = true;
			cf.Reset.className = "new_greyapply_bt ui-corner-all";

			cf.Edit.disabled = true;
			cf.Edit.className = "new_greyapply_bt ui-corner-all";
			cf.Deleteone.disabled = true;
			cf.Deleteone.className = "new_greyapply_bt ui-corner-all";
			cf.DeleteAll.disabled = true;
			cf.DeleteAll.className = "new_greyapply_bt ui-corner-all";
		/*}
		else
		{
			document.getElementById("qos_fast_status").style.color="gray";
			document.getElementById("qos_fast_onoff").style.color="gray";
			document.getElementById("qos_device").style.color="gray";
			document.getElementById("qos_trust").style.color="gray";
			document.getElementById("qos_band").style.color="gray";
			document.getElementById("qos_wps").style.color="gray";
			document.getElementById("qos_wps1").style.color="gray";
			document.getElementById("qos_wps2").style.color="gray";
			cf.qos_fast.disabled = true;
			cf.bandwidth_reser.disabled = true;
			cf.wps_fastlane[0].disabled = true;
			cf.wps_fastlane[1].disabled = true;
			cf.MacSelect.disabled = true;
		}*/
	//}
}

function show_other()
{
	var cf = document.forms[0];

	if((parent.is_pr_version == 1 && cf.qos_apps.selectedIndex == 40) || (parent.is_ru_version == 1 && cf.qos_apps.selectedIndex == 33) || (parent.is_pr_version == 0 && parent.is_ru_version == 0 && cf.qos_apps.selectedIndex == 31))
	{
		document.getElementById("qos_ser_policy").style.display="";
		document.getElementById("qos_port_type").style.display="";
		document.getElementById("qos_port_start").style.display="";
		document.getElementById("qos_port_end").style.display="";
		return true;
	}
	else
	{
		document.getElementById("qos_ser_policy").style.display="none";
		document.getElementById("qos_port_type").style.display="none";
		document.getElementById("qos_port_start").style.display="none";
		document.getElementById("qos_port_end").style.display="none";
		return false;
	}
}

function change_qos_type()
{
	var cf = document.forms[0];

	if(cf.qos_type[0].checked == true)
	{
		cf.hid_qos_type.value = "0";
		show_other();
		document.getElementById("qos_app").style.display="";
		document.getElementById("qos_ser_pri").style.display="";
		document.getElementById("qos_device").style.display="none";
		document.getElementById("qos_dev_table").style.display="none";
		document.getElementById("blue_line_2").style.display="none";
		document.getElementById("qos_dev_policy").style.display="none";
		document.getElementById("qos_dev_mac").style.display="none";
		document.getElementById("qos_dev_name").style.display="none";
		document.getElementById("qos_dev_pri").style.display="none";
	}
	else if(cf.qos_type[1].checked == true)
	{
		cf.hid_qos_type.value = "1";
		document.getElementById("qos_app").style.display="none";
		document.getElementById("qos_ser_policy").style.display="none";
		document.getElementById("qos_port_type").style.display="none";
		document.getElementById("qos_port_start").style.display="none";
		document.getElementById("qos_port_end").style.display="none";
		document.getElementById("qos_ser_pri").style.display="none";
		document.getElementById("qos_device").style.display="";
		document.getElementById("qos_dev_table").style.display="";
		document.getElementById("blue_line_2").style.display="";
		document.getElementById("qos_dev_policy").style.display="";
		document.getElementById("qos_dev_mac").style.display="";
		document.getElementById("qos_dev_name").style.display="";
		document.getElementById("qos_dev_pri").style.display="";
	}
	document.getElementById("blue_line_1").style.display="";
	document.getElementById("qos_button_1").style.display="";
	document.getElementById("qos_button_2").style.display="";
	document.getElementById("qos_rules").style.display="";
	document.getElementById("qos_show_rule").style.display="";
	document.getElementById("blue_line_4").style.display="";
}

function change_serv_apps()
{
	var cf = document.forms[0];
	var index = cf.qos_apps.selectedIndex;

	show_other();

	if(parent.is_pr_version == 1)
	{
		cf.qos_apps.options.length = 41;
	}
	else if(parent.is_ru_version == 1)
	{
		cf.qos_apps.options.length = 34;
	}
	else
	{
		cf.qos_apps.options.length = 32;
	}
}

function change_service_name_show(name)
{
	if(name=="IP_Phone")
		new_name="$qos_ipphone";
	else if(name=="Skype")
		new_name="$qos_skype";
	else if(name=="Netgear_EVA")
		new_name="$qos_netgear";
	else if(name=="Vonage_IP_Phone")
		new_name="$qos_vonage";
	else if(name=="Google_Talk")
		new_name="$qos_google";
	else if(name=="MSN_messenger")
		new_name="$qos_msn";
	else if(name=="Yahoo_messenger")
		new_name="$qos_yahoo";
	else if(name=="Netmeeting")
		new_name="$qos_netmeeting";
	else if(name=="AIM")
		new_name="$qos_aim";
	else if(name=="SlindStream")
		new_name="$qos_slingstream";
	else if(name=="SSH")
		new_name="$block_ser_setup_ssh";
	else if(name=="Telnet")
		new_name="$block_ser_setup_telnet";
	else if(name=="VPN")
		new_name="$qos_vpn";
	else if(name=="On_line_Game")
		new_name="On-line Game";
	else if(name=="FTP")
		new_name="$ftp_mark";
	else if(name=="SMTP")
		new_name="$block_ser_setup_smtp";
	else if(name=="PPlive")
		new_name="$qos_pplive";
	else if(name=="WWW")
		new_name="$qos_www";
	else if(name=="DNS")
		new_name="$block_ser_setup_dns";
	else if(name=="ICMP")
		new_name="$qos_icmp";
	else if(name=="eMule")
		new_name="$qos_emule";
	else if(name=="Kazaa")
		new_name="$qos_kazaa";
	else if(name=="Gnutella")
		new_name="$qos_gnutella";
	else if(name=="bt_azureus")
		new_name="$qos_bt_azureus";
	else if(name=="Counter-Strike")
		new_name="$qos_counter_strike";
	else if(name=="Age-of-Empires")
		new_name="$qos_ageof_empires";
	else if(name=="Everquest")
		new_name="$qos_everquest";
	else if(name=="Quake-2")
		new_name="$qos_quake2";
	else if(name=="Quake-3")
		new_name="$qos_quake3";
	else if(name=="Unreal-Tourment")
		new_name="$qos_unreal";
	else if(name=="Warcraft")
		new_name="$qos_warcraft";
	else if(name=="Thunder")
		new_name="$qos_pr_thunder";
	else if(name=="QQ_xuanfeng")
		new_name="$qos_pr_qq_xuanfeng";
	else if(name=="LOL")
		new_name="$qos_pr_lol";
	else if(name=="YY_yuyin")
		new_name="$qos_pr_yy";
	else if(name=="Tonghuashun")
		new_name="$qos_pr_tonghuashun";
	else if(name=="Dazhihui")
		new_name="$qos_pr_dazhihui";
	else if(name=="sipnet.ru")
		new_name="$qos_ru_sipnet";
	else
		new_name=name;
	return new_name;
}

function attached_qosmac_data_selected(j)
{
	var cf=document.forms[0];

	cf.dev_poli_name.value=attach_policy[j];
	if( attach_mac_array[j] == "&lt;$unknown_mark&gt;" )
		cf.mac.value="";
	else
		cf.mac.value=attach_mac_array[j];

	if( attach_name_array[j] == "&lt;$unknown_mark&gt;" )
		cf.dev_name.value="";
	else
		cf.dev_name.value=attach_name_array[j].replace(/&gt;/g, ">").replace(/&lt;/g, "<").replace(/&#38;/g,'&').replace(/&#35;/g,'#').replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#40;/g,"(").replace(/&#41;/g,")");
	cf.dev_priority.value='2';
}

function attached_qosmac_selected(j)
{
	var cf=document.forms[0];

	if( attach_mac_array[j] == "&lt;$unknown_mark&gt;" )
		cf.trust_mac.value="";
	else
		cf.trust_mac.value=attach_mac_array[j];
}

function check_qos_port(cf)
{
	if(cf.portstart.value==""||cf.portend.value=="")
	{
		sAlert("$invalid_port");
		return false;
	}
	txt=cf.portstart.value;
	for(i=0;i<txt.length;i++)
	{
		c=txt.charAt(i);
		if("0123456789".indexOf(c,0)<0)
		{
			sAlert("$invalid_start_port");
			return false;
		}
	}
	txt=cf.portend.value;
	for(i=0;i<txt.length;i++)
	{
		c=txt.charAt(i);
		if("0123456789".indexOf(c,0)<0)
		{
			sAlert("$invalid_end_port");
			return false;
		}
	}
	if(parseInt(cf.portstart.value)<1||parseInt(cf.portstart.value)>65535)
	{
		sAlert("$invalid_start_port");
		return false;
	}
	if(parseInt(cf.portend.value)<1||parseInt(cf.portend.value)>65535)
	{
		sAlert("$invalid_end_port");
		return false;
	}
	if(parseInt(cf.portend.value)<parseInt(cf.portstart.value))
	{
		sAlert("$end_port_greater");
		return false;
	}
	return true;
}

function q_ser_queue()
{
	var cf = document.forms[0];

	if(cf.ser_priority.value == "0")
		q_index = 0;
	else if(cf.ser_priority.value == "1")
		q_index = 1;
	else if(cf.ser_priority.value == "2")
		q_index = 2;
	else if(cf.ser_priority.value == "3")
		q_index = 3;
	return q_index;
}

function q_dev_queue()
{
	var cf = document.forms[0];

        if(cf.dev_priority.value == "0")
                q_index = 0;
        else if(cf.dev_priority.value == "1")
                q_index = 1;
        else if(cf.dev_priority.value == "2")
                q_index = 2;
        else if(cf.dev_priority.value == "3")
                q_index = 3;
        return q_index;
}

function add_rules()
{
	var cf = document.forms[0];

	var queue_lists = getElementsByTagAndName("div", "queue_list");

	if(cf.qos_type[0].checked == true)
	{
		q_ser_queue();
		change_serv_apps();

		index = cf.qos_apps.selectedIndex;

		o_value = cf.qos_apps[index].value;
		o_app_type = cf.qos_apps[index].value;

		if((parent.is_pr_version == 1 && index == 40) || (parent.is_ru_version == 1 && index == 33) || (parent.is_pr_version == 0 && parent.is_ru_version == 0 && index == 31))
		{
			if(!check_qos_add_info())
				return false;
			o_value = cf.ser_poli_name.value.replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
			o_app_type = "1";
			o_app_conn = cf.port_type.value;
			o_app_start = cf.portstart.value;
			o_app_end = cf.portend.value;
			remove_ser_from_queue(o_value, o_app_type);
			newitem = creat_other_new_item(o_value, o_app_type, o_app_conn, o_app_start, o_app_end);
			queue_lists[q_index].appendChild(newitem);
		}
		else
		{
			remove_ser_from_queue(o_value, o_app_type);
			newitem = creat_a_new_item(o_value, o_app_type);
			queue_lists[q_index].appendChild(newitem);
		}
	}
	else
	{
		q_dev_queue();
		if(!check_qosMac_info(cf, "add"))
			return false;
		o_value = cf.dev_poli_name.value;
		o_app_type = "3";
		o_dev_name = cf.dev_name.value;
		o_dev_mac = cf.the_mac.value;

		newitem = creat_a_dev_new_item(o_value, o_app_type, o_dev_name, o_dev_mac);
		queue_lists[q_index].appendChild(newitem);
	}

	update_qos_rules();

	cf.submit_flag.value="add_qoslist";
	cf.action="/apply.cgi?/QOS_basic_setting.htm timestamp=" +ts;
	return true;
}

function reset_rules()
{
	var cf = document.forms[0];

	if(cf.qos_type[0].checked == true)
	{
		if(show_other())
		{
			cf.ser_poli_name.value = "";
			cf.ser_priority.value = "1";
			cf.port_type.value = "TCP/UDP";
			cf.portstart.value = "";
			cf.portend.value = "";
		}
		else
		{
			cf.qos_apps.value = "MSN_messenger";
			cf.ser_priority.value = "1";
		}
	}
	else
	{
		cf.dev_poli_name.value = "";
		cf.mac.value = "";
		cf.dev_name.value = "";
		cf.dev_priority.value = "2";
	}
}

function edit_qos_rules()
{
	var cf = document.forms[0];
	var queue_lists = getElementsByTagAndName("div", "queue_list");
	var array_num = qos_array_num/2;
	if( selected_node !=0)
	{
		var node_value = selected_node.getAttribute('val0_name').replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var node_app_type = selected_node.getAttribute('val2_app_name');
		for(var i=1; i<=qos_array_num; i=i+2)
		{
			var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
			var each_info=str.split(' ');
			if(node_value == each_info[0])
			{
				cf.select_node_editnum.value = i;
				if(node_app_type == "3")
					cf.action="/apply.cgi?/QOS_device_edit.htm timestamp=" +ts;
				else
				{
					cf.select_node_name.value = node_value;
					cf.action="/apply.cgi?/QOS_service_edit.htm timestamp=" +ts;
				}
				cf.submit_flag.value = "qos_editnode";
				return true;
			}
		}
	}
	else
	{
		sAlert("$port_edit");
		return false;
	}
}

function delete_qos_rule()
{
	var cf = document.forms[0];
	var queue_lists = getElementsByTagAndName("div", "queue_list");

	if( selected_node != 0 )
	{
		if(selected_node.parentNode == queue_lists[0])
			queue_lists[0].removeChild(selected_node);
		else if(selected_node.parentNode == queue_lists[1])
			queue_lists[1].removeChild(selected_node);
		else if(selected_node.parentNode == queue_lists[2])
			queue_lists[2].removeChild(selected_node);
		else if(selected_node.parentNode == queue_lists[3])
			queue_lists[3].removeChild(selected_node);

		update_qos_rules();
		cf.submit_flag.value = "qos_deletenode";
		cf.action="/apply.cgi?/QOS_wait.htm timestamp=" +ts;
		return true;
	}
	else
	{
		sAlert("$port_del");
		return false;
	}
}

function clear_qos_rules()
{
	var cf = document.forms[0];
	var queue_lists = getElementsByTagAndName("div", "queue_list");

	for(var i=0; i<4; i++)
	{
		var queue_l = queue_lists[i];
		var length = queue_l.childNodes.length;
		while(queue_l.childNodes.length > 0)
		{
			queue_l.removeChild(queue_l.childNodes[0]);
		}
	}

	update_qos_rules();
	cf.submit_flag.value = "qos_deletenode";
	cf.action="/apply.cgi?/QOS_wait.htm timestamp=" +ts;
}

function create_one_rule(n, value)
{
	var cf = document.forms[0];

	value = value.replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");

	one_rule = document.createElement('input');
	one_rule.type = "hidden";
	one_rule.name = "hid_qos_list"+n;
	one_rule.value=""+value+"";
	cf.appendChild(one_rule);
}

function find_qos_item_details(o_value, l_name, l_num, p_value, compare_type,  num)
{// o_value: service name, l_name: config list name, l_num: list number, p_value: priory value (queue num), num: hid_qos_list num
// compare_type: 2: applications name or on-line game name, 8: mac address

	cf = document.forms[0];
	for( var x =1; x <=l_num; x=x+2) // edit a exist item
	{
		var str = eval(l_name+x);
		var each_info = str.split(' ');
		if( o_value == each_info[compare_type]) //each_info[2]applications name or on-line game name, each_info[8]: mac address
		{// find a exist item
			num++;
			var i_value = each_info[0];
			for( var t=1; t< each_info.length; t++) // change the priority value.
			{
				if(t == 3)
					i_value = i_value + ' ' + p_value; // change each_info[3] to p_value
				else
					i_value = i_value + ' ' + each_info[t];
			}

			create_one_rule(num, i_value);

			// new qos, every rule have 2 configs
			num++;
			var str2 = eval(l_name+(x+1));

			if(str2 == "0")
				i_value = "0";
			else
			{
				each_info = str2.split(' ');
				i_value = each_info[0];
				for(t=1; t< each_info.length; t++)
				{
					if(t == 3)
						i_value = i_value + ' ' + p_value; // change each_info[3] to p_value
					else
						i_value = i_value + ' ' + each_info[t];
				}
			}

			create_one_rule(num, i_value);
			break;
		}
	}
	return num;
}

function creat_other_qos_item_details(p_name, p_category, p_other, p_priority, p_connect, p_start, p_stop, num)
{//p_name: service_name, p_category: application, p_other: Add, p_priority: priory value (queue num), p_connect: connect type, p_start: start port, p_stop: end port, num: hid_qos_list num

	cf = document.forms[0];
	var i_value = p_name;
	if(i_value != "")
	{
		num++;
		if(p_connect == "TCP/UDP")
		{
				i_value = p_name + ' ' + p_category + ' ' + p_other + ' ' + p_priority + ' ' + "TCP" + ' ' + p_start + ' ' + p_stop + ' ' + "----" + ' ' + "----";
				create_one_rule(num, i_value);
				num++;
				i_value = p_name + ' ' + p_category + ' ' + p_other + ' ' + p_priority + ' ' + "UDP" + ' ' + p_start + ' ' + p_stop + ' ' + "----" + ' ' + "----";
				create_one_rule(num, i_value);
		}
		else
		{
				i_value = p_name + ' ' + p_category + ' ' + p_other + ' ' + p_priority + ' ' + p_connect + ' ' + p_start + ' ' + p_stop + ' ' + "----" + ' ' + "----";
				create_one_rule(num, i_value);
				num++;
				i_value = "0";
				create_one_rule(num, i_value);
		}
	}
	return num;
}

function creat_a_dev_qos_item_details(node_value, i, node_dev_name, node_dev_mac, num)
{
        cf = document.forms[0];
	var i_value = node_value;
	if(i_value != "")
	{
		num++;
		i_value = node_value + ' 3 ---- '+ i +' ---- ---- ---- '+ node_dev_name +' '+ node_dev_mac;
		create_one_rule(num, i_value);
		num++;
		create_one_rule(num, i_value);
	}
	return num;
}

function update_qos_rules()
{
	var cf=document.forms[0];

	var n=0;
	if(cf.qos_endis_on.value == '1') //enable qos
	{
		queue_lists = getElementsByTagAndName("div", "queue_list");

		for(var i=0; i<4; i++) //every queue list
		{
			qlist = queue_lists[i];
			for(var j = 0; j< qlist.childNodes.length; j++) // every qos item
			{
				childnode = qlist.childNodes[j];
				node_value = childnode.getAttribute('val0_name');
				node_app_type = childnode.getAttribute('val2_app_name');

				// find the same service qos item in "qosArray",and add a manully qos rule.
				if(node_app_type == "1")//Add a manully add qos rule.
				{
					node_app_conn = childnode.getAttribute('val3_app_conn');
					node_app_start = childnode.getAttribute('val4_app_start');
					node_app_end = childnode.getAttribute('val5_app_end');
					c_n = creat_other_qos_item_details(node_value, 0, 1, i, node_app_conn, node_app_start, node_app_end, n);
				}
				else if(node_app_type == "3")
				{
					node_dev_name = childnode.getAttribute('val3_dev_name');
					node_dev_mac = childnode.getAttribute('val4_mac');
					c_n = creat_a_dev_qos_item_details(node_value, i, node_dev_name, node_dev_mac, n);
				}
				else
					c_n = find_qos_item_details(node_app_type, "qosArray", qos_array_num, i, 2, n);
				if(c_n == n) // not find in "qosArray", find the service qos item in default qos values qosDftArray
					c_n = find_qos_item_details(node_value, "qosDftArray", qos_dft_array_num,i, 2, n);
				n = c_n;
			}
		}
	}
}

function check_prru_qos_apply(cf)
{
	/*if(cf.wmm_enable.checked == true)
	  	cf.qos_endis_wmm.value="1";
	  else
	  	cf.qos_endis_wmm.value="0";
	  if(cf.wmm_enable_a.checked == true)
	  	cf.qos_endis_wmm_a.value="1";
	  else
	  	cf.qos_endis_wmm_a.value="0";*/

	if(cf.qos_fast.checked == true)
		cf.hid_fast_status.value="1";
	else
		cf.hid_fast_status.value="0";

	if( cf.wps_fastlane[0].checked == true )
		cf.hid_wps_fastlen.value = "wps";
	else
		cf.hid_wps_fastlen.value = "fastlane";

	if( cf.hid_wps_fastlen.value == "fastlane" && w_f_value == "wps")
		sAlert("$wps_to_fasttrack", check_prru_qos_apply2());
	else
		check_prru_qos_apply2();
}

function check_prru_qos_apply2()
{
	var cf = document.forms[0];
	qos_bandwith=parseInt(cf.tcbw_value.value);
	//if ( cf.qosEnable.checked == true )
	//{
		if (max_bandwidth == 1000)
		{
			if(cf.tcbw_unit.selectedIndex == 0)
			{
				if(!(qos_bandwith > 0 && qos_bandwith < 1024001))
				{
					sAlert("$qos_bandwith1000M");
					return false;
				}
				else
					cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value);
			}
			else
			{
				if(!(qos_bandwith > 0 && qos_bandwith < 1001))
				{
					sAlert("$qos_bandwith1000M");
					return false;
				}
				else
					cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value)*1024;
			}
		}
		else if (max_bandwidth == 100)
		{
			if(cf.tcbw_unit.selectedIndex == 0)
			{
				if(!(qos_bandwith > 0 && qos_bandwith < 102401))
				{
					sAlert("$qos_bandwith100M");
					return false;
				}
				else
					cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value);
			}
			else
			{
				if(!(qos_bandwith > 0 && qos_bandwith < 101))
				{
					sAlert("$qos_bandwith100M");
					return false;
				}
				else
					cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value)*1024;
			}
		}
	/*}
	else
	{
		cf.qos_hidden_uprate.value=tcbw_value;
		cf.tcbw_unit.value=tcbw_unit;
	}*/

	cf.submit();
}

function check_name_space(name)
{
	var allowed_str_array=["MSN Messenger","Yahoo Messenger","IP Phone","Vonage IP Phone","Google Talk","Netgear EVA","On-line Game","LAN Port 1","LAN Port 2","LAN Port 3","LAN Port 4","Counter Strike", "Age of Empires","Diablo II","Half Life","Quake 2","Quake 3","Unreal Tourment","Return to Castle Wolfenstein"];
	var flag = 0;
	for(i=0;i<allowed_str_array.length;i++)
	{
		if(name == allowed_str_array[i])
		{
			flag = 1;
		}
	}

	if(flag == 0)
	{
		for(i=0;i<name.length;i++)
		{
			if(isValidChar(name.charCodeAt(i))==false)
			{
				sAlert("$qos_policy"+" "+"$not_allow_2bytes_space");
				return false;
			}
		}
	}
}

function check_qos_add_info()
{
	var cf=document.forms[0];
	var index = cf.qos_apps.selectedIndex;
	if(cf.ser_poli_name.value == "")
	{
		sAlert("$qos_policy_name_null");
		return false;
	}
	if(check_name_space(cf.ser_poli_name.value)==false)
		return false;
	if((parent.is_pr_version == 1 && index == 40) || (parent.is_ru_version == 1 && index == 33) || (parent.is_pr_version == 0 && parent.is_ru_version == 0 && index == 31))
	{
		if(check_qos_port(cf)==false)
			return false;
	}
	if((parent.is_pr_version == 1 && index == 40) || (parent.is_ru_version == 1 && index == 33) || (parent.is_pr_version == 0 && parent.is_ru_version == 0 && index == 31))
	{
		var input_start_port=cf.portstart.value;
		var input_end_port=cf.portend.value;
	}
	for(i=1;i<=qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		var startport=each_info[5];
		var endport=each_info[6];

		var j=i+1;
		var str_next = eval ( 'qosArray' + j ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		if(str_next != "0")
			each_info[4] = "TCP\/UDP";

		if( cf.ser_poli_name.value == change_service_name_show(each_info[0]))
		{
			sAlert("$qos_policy_name_dup");
			return false;
		}
		if((parent.is_pr_version == 1 && index == 40) || (parent.is_ru_version == 1 && index == 33) || (parent.is_pr_version == 0 && parent.is_ru_version == 0 && index == 31))
		{
			if(cf.port_type.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.port_type.value == each_info[4])
			{
				if(each_info[5] != '----' && each_info[6] != '----')
				{
					if(!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10)))
					{
						sAlert("$qos_port_used");
						return false;
					}
				}
			}
		}
		i = i+2;
	}
	return true;
}

function check_qos_app(cf)
{
	var cf=document.forms[0];
	var select_editnum = selected_node;
	var str_edit = eval ( 'qosArray' + select_editnum ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
	var str_info=str_edit.split(' ');
	var startport=str_info[5];
	var endport=str_info[6];
	if(str_info[2] == '1')
	{
		if(cf.ser_poli_name.value == "")
		{
			sAlert("$qos_policy_name_null");
			return false;
		}
		if(check_name_space(cf.ser_poli_name.value)==false)
			return false;
		if(check_qos_port(cf)==false)
			return false;
	}
	for(var i=1; i<=qos_array_num; i=i+2)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		var startport=each_info[5];
		var endport=each_info[6];

		var j=i+1;
		var str_next = eval ( 'qosArray' + j ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		if(str_next != "0")
			each_info[4]= "TCP\/UDP";

		if(str_info[2] == '1')
		{
			var input_start_port=cf.portstart.value;
			var input_end_port=cf.portend.value;
			if(cf.ser_poli_name.value == change_service_name_show(each_info[0]) && select_editnum != i)
			{
				sAlert("$qos_policy_name_dup");
				return false;
			}
			if(cf.port_type.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.port_type.value == each_info[4])
			{
				if(each_info[5] != '----' && each_info[6] != '----')
				{
					if((!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10))) && select_editnum !=i)
					{
						sAlert("$qos_port_used");
						return false;
					}
				}
			}
		}
		if(select_editnum == i)
			if( each_info[2]== '1')
				creat_other_qos_item_details(cf.ser_poli_name.value, each_info[1], each_info[2], cf.ser_priority.value, cf.port_type.value, cf.portstart.value, cf.portend.value, i-1);
			else
				find_qos_item_details(each_info[2], "qosArray", qos_array_num, cf.ser_priority.value, 2, i-1);
	}
}

function check_qosMac_info(cf,flag)
{
	var cf=document.forms[0];
	var select_editnum = selected_node;
	if(cf.dev_name.value=='')
	{
		sAlert("$device_name_null");
		return false;
	}
	if(check_name_space(cf.dev_name.value)==false)
		return false;
	for(i=0;i<cf.dev_name.value.length;i++)
	{
		if(isValidChar(cf.dev_name.value.charCodeAt(i))==false)
		{
			sAlert("$device_name_error");
			cf.dev_name.focus();
			return false;
		}
	}
	cf.the_mac.value = cf.mac.value;
	var mac_addr = cf.the_mac.value;
	if ( mac_addr.indexOf(":")==-1 && mac_addr.length=="12" )
	{
		var tmp_mac=mac_addr.substr(0,2)+":"+mac_addr.substr(2,2)+":"+mac_addr.substr(4,2)+":"+mac_addr.substr(6,2)+":"+mac_addr.substr(8,2)+":"+mac_addr.substr(10,2);
		mac_addr = tmp_mac;
	}
	cf.the_mac.value = mac_addr;
	if(maccheck(cf.the_mac.value) == false)
		return false;

	for(i=1; i <= qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		if((flag == 'edit' && select_editnum != i) || flag == 'add')
		{
			if(cf.dev_name.value == each_info[7])
			{
				sAlert("$device_name_dup");
				return false;
			}
			if(cf.dev_poli_name.value == each_info[0])
			{
				sAlert("$qos_policy_name_dup");
				return false;
			}
			if(cf.the_mac.value.toLowerCase() == each_info[8].toLowerCase())
			{
				sAlert("$mac_dup");
				return false;
			}
		}
		i = i+2;
	}

	var def_name=cf.the_mac.value.substring(9,11)+cf.the_mac.value.substring(12,14)+cf.the_mac.value.substring(15,17);
	if(cf.dev_poli_name.value=='')
		cf.dev_poli_name.value='Pri_MAC_'+def_name;
	if(check_name_space(cf.dev_poli_name.value)==false)
		return false;
	for(i=0;i<cf.dev_poli_name.value.length;i++)
	{
		if(isValidChar(cf.dev_poli_name.value.charCodeAt(i))==false)
		{
			sAlert("$qos_policy_deny_2bytes");
			cf.dev_poli_name.focus();
			return false;
		}
	}
	return true;
}

function check_qos_mac(cf,flag)
{
	var cf=document.forms[0];
	var array_num = qos_array_num/2;
	if(!check_qosMac_info(cf,flag))
		return false;

	i_value = cf.dev_poli_name.value + ' 3 ---- '+ cf.dev_priority.value +' ---- ---- ---- '+ cf.dev_name.value +' '+ cf.the_mac.value;
	create_one_rule(selected_node, i_value);
	selected_node++;
	create_one_rule(selected_node, i_value);
}
