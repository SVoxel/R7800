function check_vlan_input(cf,flag)
{
	if(!(flag=='edit' && default_internet ==1))
	{
		
		if (cf.vlan_name.value.length>10 || cf.vlan_name.value.length==0)
		{
			alert("$vlan_error11");
			return false;
		}
		for(i=0; i<cf.vlan_name.value.length; i++)
		{
			if(isValidChar_space(cf.vlan_name.value.charCodeAt(i)) == false)
			{
				alert("$vlan_error11");
				return false;
			}
		}
		for(i=1;i<=array_num;i++)
		{
			var str=eval( 'vlanArray' + i )
			var str_info=str.split(' ');
			if(str_info[1] == cf.vlan_name.value && (!(flag == 'edit' && sel_num==i )))
			{
				alert("$vlan_error4_1 " +cf.vlan_name.value +" $vlan_error4_2");
				return false;
			}
		}
		if(!_isNumeric(cf.vlan_id.value))
		{
			alert("$vlan_error3");
			return false;
		}
		var str_tmp=parseInt(cf.vlan_id.value,10);
		if(str_tmp <1 || str_tmp >4094)
		{
			alert("$vlan_error3");
			return false;
		}
	}
	if(cf.vlan_id.value.length == 0)
	{
		alert("$vlan_error1");
		return false;
	}
	if(cf.vlan_priority.value=="")
		cf.vlan_priority.value="0";
	if(!_isNumeric(cf.vlan_priority.value))
	{
		alert("$vlan_error2");
		return false;
	}
	str_tmp=parseInt(cf.vlan_priority.value,10);
	if(str_tmp >7)
	{
		alert("$vlan_error2");
		return false;
	}

	if(!(flag=='edit' && default_internet ==1))
	{
		var wired=0;
		var wireless=0;
		if(cf.iptv_ports_3.checked==true)
			wired += 8;
		if(cf.iptv_ports_2.checked==true)
			wired += 4;
		if(cf.iptv_ports_1.checked==true)
			wired += 2;
		if(cf.iptv_ports_0.checked==true)
			wired += 1;
		if(cf.iptv_ports_10.checked==true)
			wireless += 1;
		if(cf.iptv_ports_12.checked==true)
			wireless += 4;
		if(cf.iptv_ports_11.checked==true)
			wireless += 2;
		if(cf.iptv_ports_13.checked==true)
			wireless += 8;
		if(wired==15 && wireless==3)
		{
			alert("$vlan_error6");
			return false;
		}
		if(wired==0 && wireless==0)
		{
			alert("$vlan_error5");
			return false;
		}
		cf.hid_wired_port.value=wired;
		cf.hid_wireless_port.value=wireless;
	}

	var wifi_port_flag=0;
	var wireless_port_1=wireless_port_2=wireless_port_3=0;
	var inter_wireless_flag=1;
	var str_inter=eval( 'vlanArray1');
	var str_inter_info=str_inter.split(' ');
	
	if( parseInt(cf.hid_wireless_port.value,10)>0 )
		wifi_port_flag=1;
	for(i=1;i<=array_num;i++)
	{
		var str_1=eval( 'vlanArray' + i );
		var str_info_1=str_1.split(' ');
		
		if(parseInt(str_info_1[5],10)==1)
			wireless_port_1=1;
		if(parseInt(str_info_1[5],10)==2)
			wireless_port_2=1;
		if(parseInt(str_info_1[5],10)==3)
			wireless_port_3=1;
	}

	if((wireless_port_1==1 && wireless_port_2==1) || wireless_port_3==1)
		inter_wireless_flag=0;

	for(i=1;i<=array_num;i++)
	{
		var str_2=eval( 'vlanArray' + i );
		var str_info_2=str_2.split(' ');
		var wireless_flag=0;

		if( parseInt(str_info_2[5],10)>0 )
			wireless_flag=1;
		
		if(flag=='edit' && default_internet ==1)
		{
			if(str_info_2[2]==cf.vlan_id.value && wireless_flag==1 && inter_wireless_flag==1 &&
			   (!(flag == 'edit' && sel_num==i )))
			{
				alert("$vlan_id:"+cf.vlan_id.value+" $vlan_error15");
				return false;
			}
		}
		else if(str_inter_info[2]==cf.vlan_id.value && wifi_port_flag==1 && inter_wireless_flag==1 && 
			(!(flag == 'edit' && sel_num==i )))
		{
			alert("$vlan_id:"+cf.vlan_id.value+" $vlan_error15");
			return false;
		}
		else if(str_info_2[2]==cf.vlan_id.value && wifi_port_flag==1 && wireless_flag==1 && 
			(!(flag == 'edit' && sel_num==i )))
		{
			alert("$vlan_id:"+cf.vlan_id.value+" $vlan_error15");
			return false;
		}

		if(str_info_2[2]==cf.vlan_id.value && str_info_2[3]==cf.vlan_priority.value && 
		   (!(flag == 'edit' && sel_num==i )))
		{
			alert("$vlan_id:"+cf.vlan_id.value+ " / $qos_priority:"+cf.vlan_priority.value+" $vlan_error14");
			return false;
		}			
	}

	if(flag=='edit')
	{
		if(default_internet ==1)
		{
			cf.hid_vlan_name.value=each_info[1];
			
			var str_tmp=parseInt(cf.vlan_id.value,10);
			if(str_tmp <0 || str_tmp >4094)
			{
				alert("$vlan_error13");
				return false;
			}
			if(str_tmp==0)
					cf.vlan_priority.value="0";
		}
		else
			cf.hid_vlan_name.value=cf.vlan_name.value;
	}

	return true;
}

function click_add_btn(cf)
{
	if(array_num>=10)
	{
		alert("$vlan_error9");
		return false;
	}
	else
	{
		cf.hid_enable_vlan.value="1";
		cf.hid_vlan_type.value="1";
		cf.action="/apply.cgi?/VLAN_add.htm timestamp="+ts;
	}
	cf.submit_flag.value="vlan_add";
	cf.submit();
	return true;
}

function check_iptv_input(cf)
{
	var wired=0;
	var wireless=0;
	if(cf.iptv_ports_3.checked==true)
	{
		wired += 8;
		cf.hid_bri_lan4.value="1";
	}
	else
		cf.hid_bri_lan4.value="0";
	if(cf.iptv_ports_2.checked==true)
	{
		wired += 4;
		cf.hid_bri_lan3.value="1";
	}
	else
		cf.hid_bri_lan3.value="0";
	if(cf.iptv_ports_1.checked==true)
	{
		wired += 2;
		cf.hid_bri_lan2.value="1";
	}
	else
		cf.hid_bri_lan2.value="0";
	if(cf.iptv_ports_0.checked==true)
	{
		wired += 1;
		cf.hid_bri_lan1.value="1";
	}
	else
		cf.hid_bri_lan1.value="0";
	if(cf.iptv_ports_10.checked==true)
	{
		wireless += 1;
		cf.hid_brig_ssid1.value="1";
	}
	else
		cf.hid_brig_ssid1.value="0";
	if(cf.iptv_ports_12.checked==true)
	{
		wireless += 4;
		cf.hid_brig_guest_ssid1.value="1";
	}
	else
		cf.hid_brig_guest_ssid1.value="0";
	if(cf.iptv_ports_11.checked==true)
	{
		wireless += 2;
		cf.hid_brig_ssid2.value="1";
	}
	else
		cf.hid_brig_ssid2.value="0";
	if(cf.iptv_ports_13.checked==true)
	{
		wireless += 8;
		cf.hid_brig_guest_ssid2.value="1";
	}
	else
		cf.hid_brig_guest_ssid2.value="0";
	if(wired==15 && wireless==3)
	{
		alert("$vlan_error6");
		return false;
	}
	if(wired==0 && wireless==0)
	{
		alert("$vlan_error5");
		return false;
	}
	cf.hid_iptv_mask.value=wired;
	return true;
}

function click_edit_btn(cf)
{
	var select_num;
	var count_select=0;
	if (array_num == 1 && is_for_RU != 1)
	{
		if(cf.ruleSelect.checked == true)
		{
			count_select++;
			select_num=parseInt(cf.ruleSelect.value);
		}
	}
	else
	{
		for(i=0; (is_for_RU == 1)? i<=array_num : i<array_num; i++)
		{
			if(cf.ruleSelect[i].checked == true)
			{
				count_select++;
				select_num=parseInt(cf.ruleSelect[i].value);
			}
		}
	}
	if(count_select==0)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		cf.hid_enable_vlan.value="1";
		cf.hid_vlan_type.value="1";
		cf.select_edit_num.value =select_num;
		cf.submit_flag.value="vlan_edit";
		cf.action="/apply.cgi?/VLAN_edit.htm timestamp="+ts;
	}
	cf.submit();
	return true;
}

function click_delete_btn(cf)
{
	var count_select=0;
	var select_num;
	if (array_num == 1 && is_for_RU != 1)
	{
		if(cf.ruleSelect.checked == true)
		{
			count_select++;
			select_num=parseInt(cf.ruleSelect.value);
		}
	}
	else
	{
		for(i=0; (is_for_RU == 1)? i<=array_num : i<array_num; i++)
		{
			if(cf.ruleSelect[i].checked == true)
			{
				count_select++;
				select_num=parseInt(cf.ruleSelect[i].value);
			}
		}
	}
	if(count_select==0)
	{
		alert("$port_del");
		return false;
	}
	else
	{
		var sel_str=eval( 'vlanArray' + select_num )
		var sel_info=sel_str.split(' ');

		if(confirm("$vlan_warn1"+" "+ sel_info[1]+"?") ==false)
			return false;
		if(sel_info[1]=="Internet" || (sel_info[1]=="Intranet" && is_for_RU==1))
		{
			alert(sel_info[1]+" $vlan_port_del_msg");
			return false;
		}
		cf.hid_enable_vlan.value="1";
		cf.hid_vlan_type.value="1";
		cf.select_del_num.value =select_num;
		cf.submit_flag.value="vlan_delete";
	}
	cf.submit();
	return true;
}

function click_apply(cf)
{
	if(cf.vlan_iptv_enable.checked==true)
	{
		if(cf.vlan_iptv_select[1].checked==true)
		{					
			var count_enable=0;
			var sel_list="";
			var port1=port2=port3=port4=port5=port6=port7=port8=0;
			for(i=1;i<=array_num;i++)
			{
				var boxName= "vlan_check"+i;
				if(document.getElementById(boxName).checked == true)
				{
					var sel_str=eval( 'vlanArray' + i );
					var sel_info=sel_str.split(' ');
					var lan_port=parseInt(sel_info[4],10);
					var wlan_port=parseInt(sel_info[5],10);
					if(lan_port>=8 && lan_port<=15)
						port4++;
					if((lan_port>=12 && lan_port<=15) || (lan_port>=4 && lan_port<=7))
						port3++;
					if(lan_port==15 ||lan_port==14 ||lan_port==11 ||lan_port==10 ||lan_port==7 ||lan_port==6 ||lan_port==3 ||lan_port==2)
						port2++;
					if(lan_port%2 ==1)
						port1++;
					if(wlan_port>=8 && wlan_port<=15)
						port8++;
					if((wlan_port>=12 && wlan_port<=15) || (wlan_port>=4 && wlan_port<=7))
						port7++;
					if(wlan_port==15 ||wlan_port==14 ||wlan_port==11 ||wlan_port==10 ||wlan_port==7 ||wlan_port==6 ||wlan_port==3 ||wlan_port==2)
						port6++;
					if(wlan_port%2 ==1)
						port5++;
					sel_list+= i;
					sel_list+= "#";
					count_enable++;
				}
			}
			if( array_num >1 && cf.hid_inter_lan1.value=="0" && cf.hid_inter_lan2.value=="0" && cf.hid_inter_lan3.value=="0" &&
                        cf.hid_inter_lan4.value=="0" && cf.hid_inter_wireless1.value=="0" && cf.hid_inter_wireless2.value=="0" )	
			{	alert("$vlan_error16");
				return false;
			}
			if(port1>1 ||port2>1 ||port3>1 ||port4>1 ||port5>1 ||port6>1 ||port7>1 ||port8>1)
			{
				alert("$vlan_port_dup");
				return false;
			}
			if(count_enable>6)
			{
				alert("$vlan_error10");
				return false;
			}
			else
			{
				cf.hid_enable_vlan.value="1";
				cf.hid_vlan_type.value="1";
				cf.hid_sel_list.value=sel_list;
				cf.hid_enabled_num.value=count_enable;
				cf.submit_flag.value="apply_vlan";
			}
		}
		else//bridge
		{
			if(check_iptv_input(cf) == false)
				return false;
			cf.hid_enable_vlan.value="1";
			cf.hid_vlan_type.value="0";
			cf.submit_flag.value="apply_iptv_edit";
		}
	}
	else
	{
		cf.hid_enable_vlan.value="0";
		cf.submit_flag.value="disable_vlan_iptv";
	}
	return true;
}
