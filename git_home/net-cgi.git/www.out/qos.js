function qosMain()
{
	location.href="QOS_main.htm";
}
function qosRule()
{
	location.href="QOS_rule.htm";
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
function check_name_space(name)
{
	var allowed_str_array=["MSN Messenger","Yahoo Messenger","IP Phone","Vonage IP Phone","Google Talk","Netgear EVA","On-line Game",
	"LAN Port 1","LAN Port 2","LAN Port 3","LAN Port 4","Counter Strike", "Age of Empires","Diablo II","Half Life",
	"Quake 2","Quake 3","Unreal Tourment","Return to Castle Wolfenstein"];
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
				alert("$qos_policy"+" "+"$not_allow_2bytes_space");
				return false;
			}
		}
	}	
}
function check_qos()
{
	var cf=document.forms[0];
	if(cf.wmm_enable.checked == true)
		cf.qos_endis_wmm.value=1;
	else
		cf.qos_endis_wmm.value=0;
		
	if(cf.qosEnable.checked == true)
		cf.qos_endis_on.value=1;
	else
		cf.qos_endis_on.value=0;
	cf.submit_flag.value="apply_qos";
	cf.submit();
}

function change_priority()
{
	var cf=document.forms[0];
	if( cf.category.selectedIndex == 0)
		location.href="QOS_adva_add.htm";
	else if( cf.category.selectedIndex == 1)
		location.href="QOS_online_add.htm";
	else if( cf.category.selectedIndex == 2)
		location.href="QOS_ether_add.htm";
	else if( cf.category.selectedIndex == 3)
		location.href="QOS_mac_add.htm";
}

function change_serv_adva()
{
	var cf=document.forms[0];
	if (cf.apps.selectedIndex == 24)
	{
		cf.name.value='';
		cf.priority.selectedIndex=0;
		getObj("real_app_port").innerHTML=str_div;
	}
	else
	{
		getObj("real_app_port").innerHTML='';
		var i=cf.apps.selectedIndex*2;

		if(serv_array[i][3]=="IP_Phone")
			cf.name.value="IP Phone";
		else if(serv_array[i][3]=="Netgear_EVA")
			cf.name.value="Netgear EVA";
		else if(serv_array[i][3]=="Vonage_IP_Phone")
			cf.name.value="Vonage IP Phone";
		else if(serv_array[i][3]=="Google_Talk")
			cf.name.value="Google Talk";
		else if(serv_array[i][3]=="MSN_messenger")
			cf.name.value="MSN Messenger";
		else if(serv_array[i][3]=="Yahoo_messenger")
			cf.name.value="Yahoo Messenger";
		else if(serv_array[i][3]=="On_line_Game")
			cf.name.value="On-line Game";
		else if(serv_array[i][3]=="bt_azureus")
			cf.name.value="BT/Azureus";
		else if(serv_array[i][3]=="eMule")
			cf.name.value="eMule/eDonkey";
		else if(serv_array[i][3]=="Netmeeting")
			cf.name.value="Netmeeting";
		else
			cf.name.value=serv_array[i][3];
		cf.priority.selectedIndex=serv_array[i][4];
	}
}
function change_serv_online()
{
	var cf=document.forms[0];
	if (cf.apps.selectedIndex == 7)
	{
		cf.name.value='';
		cf.priority.selectedIndex=1;
		getObj("real_app_port").innerHTML=str_div;
	}
	else
	{
		getObj("real_app_port").innerHTML='';
		var i=cf.apps.selectedIndex*2;
		if(serv_array[i][3]=="Counter-Strike")
			cf.name.value="Counter Strike";
		else if(serv_array[i][3]=="Age-of-Empires")
			cf.name.value="Age of Empires";
		else if(serv_array[i][3]=="Quake-2")
			cf.name.value="Quake 2";
		else if(serv_array[i][3]=="Quake-3")
			cf.name.value="Quake 3";
		else if(serv_array[i][3]=="Unreal-Tourment")
			cf.name.value="Unreal Tourment";	
		else
			cf.name.value=serv_array[i][3];
		cf.hidden_qos_policy_name.value=serv_array[i][3];
		cf.priority.selectedIndex=serv_array[i][4];
	}
}

function change_name_online(name)
{
		if(name=="Counter-Strike")
			new_name="Counter Strike";
		else if(name=="Age-of-Empires")
			new_name="Age of Empires";
		else if(name=="Diablo-II")
			new_name="Diablo II";
		else if(name=="Half-Life")
			new_name="Half Life";
		else if(name=="Quake-2")
			new_name="Quake 2";
		else if(name=="Quake-3")
			new_name="Quake 3";
		else if(name=="Unreal-Tourment")
			new_name="Unreal Tourment";	
		else if(name=="Return-to-Castle-Wolfenstein")
			new_name="Return to Castle Wolfenstein";		
		else if(name=="LAN_Port_1")
			new_name="LAN Port 1";
		else if(name=="LAN_Port_2")
			new_name="LAN Port 2";
		else if(name=="LAN_Port_3")
			new_name="LAN Port 3";
		else if(name=="LAN_Port_4")
			new_name="LAN Port 4";			
		else if(name=="MSN_messenger")
			new_name="MSN Messenger";
		else if(name=="Yahoo_messenger")
			new_name="Yahoo Messenger";
		else if(name=="IP_Phone")
			new_name="IP Phone";
		else if(name=="Vonage_IP_Phone")
			new_name="Vonage IP Phone";
		else if(name=="Google_Talk")
			new_name="Google Talk";	
		else if(name=="Netgear_EVA")
			new_name="Netgear EVA";
		else if(name=="On_line_Game")
			new_name="On-line Game";
		else if(name=="bt_azureus")
			new_name="BT/Azureus";
		else if(name=="eMule")
			new_name="eMule/eDonkey";
		else if(name=="Netmeeting")
			new_name="Netmeeting";
		else
			new_name=name;
		return new_name;
}

function change_serv_ether()
{
	var cf=document.forms[0];
	if(cf.name.value=="LAN Port 1" ||cf.name.value=="LAN Port 2"||cf.name.value=="LAN Port 3" || cf.name.value=="LAN Port 4" || cf.name.value=="")
	{
		cf.name.value="LAN Port "+cf.port_num.options[cf.port_num.selectedIndex].value;
		cf.hidden_qos_policy_name.value="LAN_Port_"+cf.port_num.options[cf.port_num.selectedIndex].value;
	}
		
}

function qosmac_data_select(num)
{
	var cf=document.forms[0];
	var str = eval ( 'qosmac_Array' + num ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
	var each_info=str.split(' ');
	cf.dev_name.value=each_info[2];
	if(each_info[0]!='----')
		cf.name.value=each_info[0];
	else
		cf.name.value='';
	var edit_mac=each_info[3];
	cf.mac.value=edit_mac;
	/*var mac_array=edit_mac.split(':');
	cf.mac_addr1.value=mac_array[0];
	cf.mac_addr2.value=mac_array[1];
	cf.mac_addr3.value=mac_array[2];
	cf.mac_addr4.value=mac_array[3];
	cf.mac_addr5.value=mac_array[4];
	cf.mac_addr6.value=mac_array[5];*/
	cf.priority.value=each_info[1];
	cf.select_editnum_mac.value=num;
	var new_num=0;
	for(i=1;i<=qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_qoslist_info=str.split(' ');
		if( each_qoslist_info[0].toLowerCase() == each_info[0].toLowerCase())
			new_num=i;

		i = i+2;
	}
	cf.select_qoslist_num.value=new_num;
}

function attached_qosmac_data_select(num,j)
{
	var cf=document.forms[0];
	cf.name.value=attach_policy[j];
	if( attach_mac_array[j] == "&lt;$unknown_mark&gt;" )
		cf.mac.value="";
	else
		cf.mac.value=attach_mac_array[j];

	if( attach_name_array[j] == "&lt;$unknown_mark&gt;" )
		cf.dev_name.value="";
	else
		cf.dev_name.value=attach_name_array[j].replace(/&gt;/g, ">").replace(/&lt;/g, "<");
	cf.priority.value='2';
	cf.select_editnum_mac.value=num;
}

function check_qoslist_editnum(cf)
{
	if( qos_array_num == 0 )
	{
		alert("$port_edit");
		return false;
	}
	var count_select=0;
	var select_num;
	var num = 2;
	var array_num = qos_array_num/2;
	if( qos_array_num == num)
	{
		if(cf.ruleSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.ruleSelect[i].checked == true)
		{
			count_select++;
			select_num= i*2+1;
		}
	if(count_select==0)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="qos_editnum";
		var edit_str=eval('qosArray' + select_num).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var edit_each_info=edit_str.split(' ');
		if(edit_each_info[1]=='0')
			cf.action="/apply.cgi?/QOS_adva_edit.htm timestamp="+ts;
		else if(edit_each_info[1]=='1')
			cf.action="/apply.cgi?/QOS_online_edit.htm timestamp="+ts;
		else if(edit_each_info[1]=='2')
			cf.action="/apply.cgi?/QOS_ether_edit.htm timestamp="+ts;	
		else if(edit_each_info[1]=='3')
			cf.action="/apply.cgi?/QOS_mac_edit.htm timestamp="+ts;	
		cf.submit();
	}
}

function check_qos_port(cf)
{
	if(cf.port_start.value==""||cf.port_end.value=="")
	{
		alert("$invalid_port");
		return false;
	}
	txt=cf.port_start.value;
	for(i=0;i<txt.length;i++)
	{
		c=txt.charAt(i);
		if("0123456789".indexOf(c,0)<0)
		{
			alert("$invalid_start_port");
			return false;
		}
	}
	txt=cf.port_end.value;
	for(i=0;i<txt.length;i++)
	{
		c=txt.charAt(i);
		if("0123456789".indexOf(c,0)<0)
		{
			alert("$invalid_end_port");
			return false;
		}
	}
	if(parseInt(cf.port_start.value)<1||parseInt(cf.port_start.value)>65535)
	{
		alert("$invalid_start_port");
		return false;
	}
	if(parseInt(cf.port_end.value)<1||parseInt(cf.port_end.value)>65535)
	{
		alert("$invalid_end_port");
		return false;
	}
	if(parseInt(cf.port_end.value)<parseInt(cf.port_start.value))
	{
		alert("$end_port_greater");
		return false;
	}
	return true;
}
function check_qos_adva(cf,flag)
{
	if(cf.name.value == "")
	{
		alert("$qos_policy_name_null");
		return false;
	}
	if(check_name_space(cf.name.value)==false)
		return false;
    	if (cf.apps.selectedIndex == 24)
    	{
        	if (check_qos_port(cf)==false)
            	return false;
    	}

 	if(cf.apps.selectedIndex == 24)
	{
		var input_start_port=cf.port_start.value;
		var input_end_port=cf.port_end.value;
	}
	for(i=1;i<=qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0]; //.replace(/&nbsp;/g, " ")
        var startport=each_info[5];
 		var endport=each_info[6];
		if(flag == 'edit')
		{
			if(cf.name.value == change_name_online(each_info[0]) && select_editnum!=i )
			{
				alert("$qos_policy_name_dup");
				return false;
			}
			if( cf.apps.value == each_info[2] &&  cf.apps.value != "Add" && select_editnum!=i )
			{
				alert("$qos_port_used");
				return false;
			}
            		if(cf.apps.selectedIndex == 24)//each_info[4] : Connection Type
			{
	                        if(cf.protocol.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.protocol.value == each_info[4])
	                        {
					if((!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10))) && select_editnum!=i && each_info[2]=="Add")
        	    			{
               					alert("$qos_port_used");
                				return false;
            				}
				}
			}
		}
		else
		{
			if( cf.name.value == change_name_online(each_info[0]))
			{
				alert("$qos_policy_name_dup");
				return false;
			}
                        if( cf.apps.value == each_info[2] &&  cf.apps.value != "Add" )
                        {
                                alert("$qos_port_used");
                                return false;
                        }

            		if(cf.apps.selectedIndex == 24)
            		{
	                        if(cf.protocol.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.protocol.value == each_info[4])
	                        {
					if(!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10)) && each_info[2]=="Add")
            				{
                				alert("$qos_port_used");
                				return false;
            				}
				}
			}
		}
		i = i+2;
	}

//add new info	
	if (cf.apps.selectedIndex == 24)
	{
			var type = cf.protocol.options[cf.protocol.selectedIndex].value;
			if( type == "TCP/UDP" )
			{
				cf.hidden_port_type.value = "TCP";
				cf.hidden_port_type2.value = "UDP";
				cf.hidden_portstart2.value=cf.port_start.value;
				cf.hidden_portend2.value=cf.port_end.value;
			}
			else
			{
				cf.hidden_port_type.value=type;
				cf.hidden_portstart2.value="0";
				cf.hidden_portend2.value="0";
			}

		cf.hidden_portstart.value=cf.port_start.value;
		cf.hidden_portend.value=cf.port_end.value;

	}
	else
	{
		var i=cf.apps.selectedIndex*2;
		cf.hidden_port_type.value=serv_array[i][0];
		cf.hidden_portstart.value=serv_array[i][1];
		cf.hidden_portend.value=serv_array[i][2];
		cf.hidden_port_type2.value=serv_array[i+1][0];
		cf.hidden_portstart2.value=serv_array[i+1][1];
		cf.hidden_portend2.value=serv_array[i+1][2];
	}
        var str_adva=cf.name.value;
        var str_new="";
        for(var j=0;j<=str_adva.length;j++)
        {
                if(str_adva.substr(j,1)==' ')
                        str_new+='_';
                else
                        str_new+=str_adva.substr(j,1);
        }
        cf.name.value=str_new;
}

function check_qos_online(cf,flag)
{
	if(cf.name.value=='')
	{
		alert("$qos_policy_name_null");
		return false;
	}
	if(check_name_space(cf.name.value)==false)
		return false;    
	if (cf.apps.selectedIndex == 7)
    	{
        	if (check_qos_port(cf)==false)
            		return false;
    	}
	if (cf.apps.selectedIndex == 7)
	{
	var input_start_port=cf.port_start.value;
	var input_end_port=cf.port_end.value;
	}
	for(i=1;i<=qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0]; //replace(/&nbsp;/g, " ")
        	startport=each_info[5];
        	endport=each_info[6];
    		if(flag == 'edit')
		{
			if( cf.name.value == change_name_online(each_info[0]) && select_editnum!=i )
			{
				alert("$qos_policy_name_dup");
				return false;
			}
                	if( cf.apps.value == each_info[2] &&  cf.apps.value != "Add" && select_editnum!=i )
                	{
                        	alert("$qos_port_used");
                        	return false;
                	}
       			if (cf.apps.selectedIndex == 7)//each_info[4] : Connection Type
 			{
				if(cf.protocol.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.protocol.value == each_info[4])
				{
					if((!(parseInt(endport)<parseInt(input_start_port)||parseInt(input_end_port)<parseInt(startport))) && select_editnum!=i  && each_info[2]=="Add")
            				{
                				alert("$qos_port_used");
                				return false;
            				}
				}
			}
		}
		else
		{
			if( cf.name.value == change_name_online(each_info[0]))
			{
				alert("$qos_policy_name_dup");
				return false;
			}
                	if( cf.apps.value == each_info[2] &&  cf.apps.value != "Add" )
                	{
                        	alert("$qos_port_used");
                        	return false;
                	}

            		if (cf.apps.selectedIndex == 7)
            		{
				if(cf.protocol.value == "TCP\/UDP" || each_info[4] == "TCP\/UDP" || cf.protocol.value == each_info[4])
				{
					if(!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10))  && each_info[2]=="Add")
            				{
                				alert("$qos_port_used");
                				return false;
            				}
				}
			}
		}
		i = i+2;
	}
//add new info	
	if (cf.apps.selectedIndex == 7)
	{
			var type=cf.protocol.options[cf.protocol.selectedIndex].value;
			if( type == "TCP/UDP" )
			{
				cf.hidden_port_type.value = "TCP";
				cf.hidden_port_type2.value = "UDP";
				cf.hidden_portstart2.value=cf.port_start.value;
				cf.hidden_portend2.value=cf.port_end.value;
			}
			else
			{
				cf.hidden_port_type.value = type;
				cf.hidden_portstart2.value = "0";
				cf.hidden_portend2.value = "0";
			}
		cf.hidden_portstart.value=cf.port_start.value;
		cf.hidden_portend.value=cf.port_end.value;
	}
	else
	{
		var i=cf.apps.selectedIndex*2;

		cf.hidden_port_type.value=serv_array[i][0];
		cf.hidden_portstart.value=serv_array[i][1];
		cf.hidden_portend.value=serv_array[i][2];
		cf.hidden_port_type2.value=serv_array[i+1][0];
		cf.hidden_portstart2.value=serv_array[i+1][1];
		cf.hidden_portend2.value=serv_array[i+1][2];
	}
    	var str_game=cf.name.value;
	var str_new="";
	for(var j=0;j<=str_game.length;j++)
	{
		if(str_game.substr(j,1)==' ')
			str_new+='-';
		else
			str_new+=str_game.substr(j,1);
	}
	cf.hidden_qos_policy_name.value=str_new;
}

function check_qos_ether(cf,flag)
{	
	if(cf.name.value=='')
	{
		cf.name.value="LAN Port "+cf.port_num.options[cf.port_num.selectedIndex].value;
		cf.hidden_qos_policy_name.value="LAN_Port_"+cf.port_num.options[cf.port_num.selectedIndex].value;
	}
	else if(cf.name.value=='LAN Port 1' || cf.name.value=='LAN Port 2' || cf.name.value=='LAN Port 3' || cf.name.value=='LAN Port 4')
	{
		cf.hidden_qos_policy_name.value = cf.name.value.replace(/ /g, "_");
	}
	else
	        cf.hidden_qos_policy_name.value = cf.name.value;
	if(check_name_space(cf.name.value)==false)
		return false;	
	for(i=1;i<=qos_array_num;)
	{
		var str = eval ( 'qosArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0]; //replace(/&nbsp;/g, " ")
		if(flag == 'edit')
		{
			if( cf.name.value == change_name_online(each_info[0]) && select_editnum!=i )
			{
				alert("$qos_policy_name_dup");
				return false;
			}
			if(cf.port_num.options[cf.port_num.selectedIndex].value==each_info[2] && select_editnum!=i )
			{
				alert("$qos_ether_port_dup");
				return false;
			}
		}
		else
		{
			if( cf.name.value == change_name_online(each_info[0]))
			{
				alert("$qos_policy_name_dup");
				return false;
			}
				if(cf.port_num.options[cf.port_num.selectedIndex].value==each_info[2] )
			{
				alert("$qos_ether_port_dup");
				return false;
			}
		}
		i = i+2;
	}
}

function valid_add_qosmac(cf,flag,from_page)
{
	if (qosmac_array_num ==0 && attach_array.length ==0 && flag == 'edit')
	{
		alert("$port_edit");
		return false;
	}
	else if( cf.select_editnum_mac.value == '' && flag == 'edit')
	{
		alert("$port_edit");
		return false;
	}
	if(cf.dev_name.value=='')
	{
		alert("$device_name_null");
		return false;
	}
	if(check_name_space(cf.dev_name.value)==false)
		return false;
	for(i=0;i<cf.dev_name.value.length;i++)
        {
                if(isValidChar(cf.dev_name.value.charCodeAt(i))==false)
                {
                        alert("$device_name_error");
                        cf.dev_name.focus();
                        return false;
                }
        }
	//cf.the_mac.value = cf.mac_addr1.value+':'+cf.mac_addr2.value+':'+cf.mac_addr3.value+':'+
	//				  cf.mac_addr4.value+':'+cf.mac_addr5.value+':'+cf.mac_addr6.value;
	
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

	for(i=1; i <= qosmac_array_num; i++)
	{
		var str = eval ( 'qosmac_Array' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0]; //.replace(/&nbsp;/g, " ")
		if( (flag == 'edit' && cf.select_editnum_mac.value != i ) || flag == 'add')
		{
			if( cf.dev_name.value == each_info[2])
			{
				alert("$device_name_dup");
				return false;
			}
			if( cf.name.value == each_info[0])
			{
				alert("$qos_policy_name_dup");
				return false;
			}
			if( cf.the_mac.value.toLowerCase() == each_info[3].toLowerCase())
			{
				alert("$mac_dup");
				return false;
			}
		}
	}
        for(i=0; i<attach_array.length; i++)
        {
                if( attach_mac_array[i].toLowerCase() != cf.the_mac.value.toLowerCase())
                {
                        if( attach_name_array[i] == cf.dev_name.value && attach_name_array[i] != "" )
                        {
                                alert("$device_name_dup");
                                return false;
                        }
                        if( attach_policy[i] == cf.name.value && attach_policy[i] != "")
                        {
                                alert("$qos_policy_name_dup");
                                return false;
                        }
                }
        }
	
	if(flag == 'edit')
		cf.submit_flag.value="qos_editmac";
	else if(flag == 'add')
		cf.submit_flag.value="qos_addmac";
	var def_name=cf.the_mac.value.substring(9,11)+cf.the_mac.value.substring(12,14)+cf.the_mac.value.substring(15,17);
	if(cf.name.value=='')
		cf.name.value='Pri_MAC_'+def_name;
	if(check_name_space(cf.name.value)==false)
		return false;
        for(i=0;i<cf.name.value.length;i++)
        {
                if(isValidChar(cf.name.value.charCodeAt(i))==false)
                {
                        alert("$qos_policy_deny_2bytes");
                        cf.name.focus();
                        return false;
                }
        }
	if( from_page == "add_page" )
		cf.action="/apply.cgi?/QOS_mac_add.htm timestamp="+ts;
	else
		cf.action="/apply.cgi?/QOS_mac_edit.htm timestamp="+ts;	
}

function valid_delete_qosmac(cf)
{
	if (qosmac_array_num ==0)
	{
		alert("$port_del");
		return false;
	}
	else if( cf.select_editnum_mac.value == '')
	{
		alert("$port_del");
		return false;
	}
	else
	{
		cf.submit_flag.value="qos_delmac";
		cf.action="/apply.cgi?/QOS_mac_add.htm timestamp="+ts;
	}
}

function check_qos_info(cf,flag)
{
	if(cf.dev_name.value=='')
	{
		alert("$device_name_null");
		return false;
	}
	if(check_name_space(cf.dev_name.value)==false)
		return false;
	for(i=0;i<cf.dev_name.value.length;i++)
        {
                if(isValidChar(cf.dev_name.value.charCodeAt(i))==false)
                {
                        alert("$device_name_error");
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
		each_info[0] = each_info[0]; //.replace(/&nbsp;/g, " ")
		if((flag == 'edit' && select_editnum != i) || flag == 'add')
		{
			if(cf.dev_name.value == each_info[7])
			{
				alert("$device_name_dup");
				return false;
			}
			if(cf.name.value == each_info[0])
			{
				alert("$qos_policy_name_dup");
				return false;
			}
			if(cf.the_mac.value.toLowerCase() == each_info[8].toLowerCase())
			{
				alert("$mac_dup");
				return false;
			}
		}
		i = i+2;
	}

        for(i=1;i<=qosmac_array_num;i++)
        {
                var str = eval ( 'qosmac_Array' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0]; //.replace(/&nbsp;/g, " ")
                if( cf.dev_name.value == each_info[2] && cf.select_editnum_mac.value!=i )
                {
                        alert("$device_name_dup");
                        return false;
                }
                if( cf.name.value == each_info[0] && cf.select_editnum_mac.value!=i)
                {
                        alert("$qos_policy_name_dup");
                        return false;
                }
                if( cf.the_mac.value.toLowerCase()  == each_info[3].toLowerCase()  && cf.select_editnum_mac.value!=i)
                {
                        alert("$mac_dup");
                        return false;
                }
        }

	for(i=0; i<attach_array.length; i++)
	{
		if( attach_mac_array[i].toLowerCase() != cf.the_mac.value.toLowerCase())
		{
			if( attach_name_array[i] == cf.dev_name.value && attach_name_array[i] != "" )
			{
				alert("$device_name_dup");
				return false;
			}
			if( attach_policy[i] == cf.name.value && attach_policy[i] != "")
			{
				alert("$qos_policy_name_dup");
				return false;
			}
		}
	}

	var def_name=cf.the_mac.value.substring(9,11)+cf.the_mac.value.substring(12,14)+cf.the_mac.value.substring(15,17);
	if(cf.name.value=='')
		cf.name.value='Pri_MAC_'+def_name;
	if(check_name_space(cf.name.value)==false)
		return false;
        for(i=0;i<cf.name.value.length;i++)
        {
                if(isValidChar(cf.name.value.charCodeAt(i))==false)
                {
                        alert("$qos_policy_deny_2bytes");
                        cf.name.focus();
                        return false;
                }
        }
	return true;
}

function check_qos_mac(cf,flag)
{
	var cf=document.forms[0];
	var array_num = qos_array_num/2;
	if(!check_qos_info(cf, flag))
		return false;
	if(cf.select_editnum_mac.value > array_num)
		cf.attached_mac.value = "1";
	if(qosmac_array_num >0)
	{
		for(i=1;i<=qosmac_array_num; i++)
		{
			var str = eval('qosmac_Array' +i);
			var each_info=str.split(' ');
			if(each_info[3].toLowerCase() == cf.mac.value.toLowerCase())
				cf.attached_mac.value = "0";
		}
	}
}

function check_qoslist_delnum(cf)
{
	if( qos_array_num == 0 )
	{
		alert("$port_del");
		return false;
	}
	var count_select=0;
	var select_num;
	var num = 2;
	var array_num = qos_array_num/2;
	if( qos_array_num == num )
	{
		if(cf.ruleSelect.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.ruleSelect[i].checked == true)
		{
			count_select++;
			select_num=i*2+1;
		}
	if(count_select==0)
	{
		alert("$port_del");
		return false;
	}
	else
	{
		cf.select_del.value=select_num;
		cf.submit_flag.value="qos_del";
		str=eval("qosArray"+select_num).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		each_info=str.split(' ');
		if ( each_info[1] == 3 )
		{
			var new_num=0;
			for(i=1;i<=qosmac_array_num;i++)
			{
				var str_mac = eval ( 'qosmac_Array' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
				var each_qosmac_info=str_mac.split(' ');
				if( each_qosmac_info[3].toLowerCase() == each_info[8].toLowerCase())
					new_num=i;
			}
			cf.select_del_mac.value=new_num;		
		}
		cf.action="/apply.cgi?/QOS_rule.htm timestamp="+ts;	
		cf.submit();
	}
}
function check_qoslist_delall(cf)
{
	if( qos_array_num == 0 )
	{
		alert("$port_del");
		return false;
	}
	if ( !confirm("$qos_deleteall"))
		return false;
	else
	{
		cf.submit_flag.value="qos_del_all";
		cf.action="/apply.cgi?/QOS_rule.htm timestamp="+ts;
		cf.submit();
	}
}
function check_qos_apply(cf)
{
	if(cf.wmm_enable.checked == true)
		cf.qos_endis_wmm.value=1;
	else
		cf.qos_endis_wmm.value=0;
	if(cf.wmm_enable_an.checked == true)
                cf.qos_endis_wmm_a.value=1;
        else
                cf.qos_endis_wmm_a.value=0;
	
	if((cf.qosEnable.checked == true || cf.bwEnable.checked == true) && quick_qos ==1 && fast_lane == 2)
	{
	 if ( !confirm("To enable this function, Fast Lane will be disabled. Do you want to continue?"))
		return false;
	}
	
	if(cf.qosEnable.checked == true)
		cf.qos_endis_on.value=1;
	else
		cf.qos_endis_on.value=0;
	if(cf.bwEnable.checked == true)
		cf.qos_endis_bandwidth.value=1;
	else
		cf.qos_endis_bandwidth.value=0;

	qos_bandwith=parseInt(cf.tcbw_value.value);
	if ( cf.bwEnable.checked == true )
	{
		var qos_bandwith_alert = ("$qos_bandwith1000M").replace("1000",max_bandwidth);
		if(cf.tcbw_unit.selectedIndex == 0)
		{
			if(!(qos_bandwith > 0 && qos_bandwith < (max_bandwidth*1024+1)))
			{
				alert(qos_bandwith_alert);
				return false;
			}
			else
				cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value);
		}
		else
		{
			if(!(qos_bandwith > 0 && qos_bandwith <(max_bandwidth + 1)))
			{
				alert(qos_bandwith_alert);
				return false;
			}
			else
				cf.qos_hidden_uprate.value = parseInt(cf.tcbw_value.value)*1024;
		}
	}
	else
	{
		cf.qos_hidden_uprate.value=tcbw_value;
		cf.tcbw_unit.value=tcbw_unit;
	}

        if ( parent.support_qos_trusted_ip == 1 && cf.enable_trustip.checked == true )
        {
                cf.qosTrusted_IP_Enable.value=1;
                cf.qosTrusted_IPAddress.value=cf.qosTrusted_IPAddress1.value+'.'+cf.qosTrusted_IPAddress2.value+'.'+cf.qosTrusted_IPAddress3.value+'.'+cf.qosTrusted_IPAddress4.value;
                if(checkipaddr(cf.qosTrusted_IPAddress.value)==false || is_sub_or_broad(cf.qosTrusted_IPAddress.value, lan_ip, lan_subnet) == false || isSameIp(cf.qosTrusted_IPAddress.value,lan_ip) == true )
                {
                        alert("$invalid_ip");
                        return false;
                }
                if(isSameSubNet(cf.qosTrusted_IPAddress.value,lan_subnet,lan_ip,lan_subnet) == false)
                {
                        alert("$same_subnet_ip_trusted");
                        return false;
                }
        }
        else
	{
                cf.qosTrusted_IP_Enable.value=0;
		cf.qosTrusted_IPAddress.value="";
	}

}

function check_ookla_speedtest(form)
{
	if(internet_status == "0"){
		alert("$internet_down");
		return false;
	}
	form.action="/func.cgi?/QOS_main.htm timestamp="+ts;
	form.submit_flag.value="ookla_speedtest";
	parent.ookla_speedtest_flag = 1;
	return true;
}
