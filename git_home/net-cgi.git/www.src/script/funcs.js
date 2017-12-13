function check_DNS(dnsaddr1,dnsaddr2,wan_assign,wan_ip)
{
        if(dnsaddr1!="...")
        {
                if(checkipaddr(dnsaddr1)==false)
                {
                        alert(bh_invalid_primary_dns);
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr1,wan_ip))
                {
                        alert(bh_invalid_primary_dns);
                        return false;
                }
        }
        if(dnsaddr2!="...")
        {
                if(checkipaddr(dnsaddr2)==false)
                {
                        alert(bh_invalid_second_dns);
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr2,wan_ip))
                {
                        alert(bh_invalid_second_dns);
                        return false;
                }
        }

        if(dnsaddr1=="..." && dnsaddr2=="...")
        {
                alert(bh_dns_must_specified);
                return false;
        }
        return true;
}

function check_three_DNS(dnsaddr1,dnsaddr2,dnsaddr3,wan_assign,wan_ip)
{
        if(dnsaddr1!="...")
        {
                if(checkipaddr(dnsaddr1)==false)
                {
                        alert(bh_invalid_primary_dns);
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr1,wan_ip))
                {
                        alert(bh_invalid_primary_dns);
                        return false;
                }
        }
        if(dnsaddr2!="...")
        {
                if(checkipaddr(dnsaddr2)==false)
                {
                        alert(bh_invalid_second_dns);
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr2,wan_ip))
                {
                        alert(bh_invalid_second_dns);
                        return false;
                }
        }

        if(dnsaddr3!="...")
        {
                if(checkipaddr(dnsaddr3)==false)
                {
                        alert(hb_invalid_third_dns);
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr3,wan_ip))
                {
                        alert(hb_invalid_third_dns);
                        return false;
                }
        }

        if(dnsaddr1=="..." && dnsaddr2=="..." && dnsaddr3 == "...")
        {
                alert(bh_dns_must_specified);
                return false;
        }
        return true;
}

/* input ip, if input length = 3 or input '.' , change focus to next text, */
function SelectionTextLength(text)
{
	var select_text="";
	if (document.selection&& document.selection.createRange)//IE
		select_text=document.selection.createRange().text;
	else if(select_text == "" && text.selectionStart != undefined )
		select_text=text.value.substring(text.selectionStart,text.selectionEnd);
	return select_text.length;
}
function keydown(e,text)
{	
	if((e.keyCode == 190 || e.keyCode == 110) && text.value.length !=0 && SelectionTextLength(text)==0)
		text.form[(getIndex(text)+1) % text.form.length].focus();	
}
function keyup(e,text)
{
	if(text.value.length == 3 && ((e.keyCode >47 && e.keyCode <58) ||(e.keyCode >95 && e.keyCode <106)))
		text.form[(getIndex(text)+1) % text.form.length].focus();	
}
function getIndex(input) {
    	var index = -1;
	var i = 0;
	var found = false;
    	while (i < input.form.length && index == -1)
    		if (input.form[i] == input)index = i;
    			else i++;
    	return index;
}

function get_browser()
{
	if(navigator.userAgent.indexOf("MSIE") != -1)
		return "IE";
	else if(navigator.userAgent.indexOf("Firefox") != -1)
		return "Firefox";
	else if(navigator.userAgent.indexOf("Safari") != -1 )
		return "Safari";
	else if(navigator.userAgent.indexOf("Camino") != -1) 
		return "Camino"; 
 	else if(navigator.userAgent.indexOf("Gecko/") != -1)
   		return "Gecko"; 
	else
   		return "";		
}

function _isNumeric(str) {
	var i;
	for(i = 0; i<str.length; i++) {
		var c = str.substring(i, i+1);
		if("0" <= c && c <= "9") {
			continue;
		}
		return false;
	}
	return true;
}

function isSameIp(ipstr1,ipstr2)
{
	var count = 0;
	var ip1_array=ipstr1.split('.');
	var ip2_array=ipstr2.split('.');
	for(i = 0;i<4;i++)
	{
		num1 = parseInt(ip1_array[i]);
		num2 = parseInt(ip2_array[i]);
		if( num1 == num2)
			count++;
	}
	if( count == 4)
		return true;
	else
		return false;
}

function is_sub_or_broad(be_checkip, ip, mask)
{
        addr_arr = be_checkip.split('.');
        var ip_addr=0;
        for (i = 0; i < 4; i++)
        {
                addr_str = parseInt(addr_arr[i],10);
                ip_addr=ip_addr*256+parseInt(addr_str);
        }
        var ip_sub=isSub(ip, mask);
        var ip_broadcast=isBroadcast(ip, mask)
	if(ip_addr == ip_sub || ip_addr == ip_broadcast){
		return false;
	}
	return true;
}

function isGateway(lanIp, lanMask, gtwIp)
{
	gtw_arr = gtwIp.split('.');
        var ip_gtw=0;
        for (i = 0; i < 4; i++)
        {
	        gtw_str = parseInt(gtw_arr[i],10);
                ip_gtw=ip_gtw*256+parseInt(gtw_str);
        }

        addr_arr = lanIp.split('.');
        var ip_addr=0;
        for (i = 0; i < 4; i++)
        {
                addr_str = parseInt(addr_arr[i],10);
                ip_addr=ip_addr*256+parseInt(addr_str);
        }

        var ip_sub=isSub(lanIp, lanMask);
        var ip_broadcast=isBroadcast(lanIp, lanMask)
        if((parseInt(ip_sub)<parseInt(ip_gtw))&&(parseInt(ip_gtw)<parseInt(ip_broadcast)))
	        return true;
        else
                return false;
}

function isSub(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_sub=0
        for (i = 0; i < 4; i++)
        {
    	        ip_str = parseInt(ip_arr[i],10);
                mask_str = parseInt(mask_arr[i],10);
                ip_sub=ip_sub*256+parseInt(ip_str & mask_str)
        }
        return(ip_sub);
}

function isBroadcast(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_broadcast=0
        for (i = 0; i < 4; i++)
        {
                ip_str = parseInt(ip_arr[i],10);
                mask_str = parseInt(mask_arr[i],10);
                n_str = ~mask_str+256;
                ip_broadcast=ip_broadcast*256+parseInt(ip_str | n_str)
        }
        return(ip_broadcast);
}

function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask) 
{
	var count = 0;
	lan1a = lan1Ip.split('.');
	lan1m = lan1Mask.split('.');
	lan2a = lan2Ip.split('.');
	lan2m = lan2Mask.split('.');
	for (i = 0; i < 4; i++) 
	{
		l1a_n = parseInt(lan1a[i],10);
		l1m_n = parseInt(lan1m[i],10);
		l2a_n = parseInt(lan2a[i],10);
		l2m_n = parseInt(lan2m[i],10);
		if ((l1a_n & l1m_n) == (l2a_n & l2m_n))
			count++;
	}
	if (count == 4)
		return true;
	else
		return false;
}
function checkipaddr(ipaddr)
{
	var form = document.forms[0];
	var ipArray = ipaddr.split(".");
	var ipstr = ipArray[0]+ipArray[1]+ipArray[2]+ipArray[3];
	var i = 0;

	if((ipArray[0]=="")||(ipArray[0]<0)||(ipArray[0]>255)||(ipArray[1]=="")||(ipArray[1]<0)||(ipArray[1]>255)
			||(ipArray[2]=="")||(ipArray[2]<0)||(ipArray[2]>255)||(ipArray[3]=="")||(ipArray[3]<0)||(ipArray[3]>255))
	{
		return false;
	}
	for(i=0;i<ipstr.length;i++)
	{
		if((ipstr.charAt(i)!='0')&&(ipstr.charAt(i)!='1')&&(ipstr.charAt(i)!='2')
				&&(ipstr.charAt(i)!='3')&&(ipstr.charAt(i)!='4')&&(ipstr.charAt(i)!='5')
				&&(ipstr.charAt(i)!='6')&&(ipstr.charAt(i)!='7')&&(ipstr.charAt(i)!='8')
				&&(ipstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if( ipArray[0] > 223 || ipArray[0] == 0 )
		return false;
	if (ipaddr == "0.0.0.0" || ipaddr == "255.255.255.255")
	{
		return false;
	}

	var each=ipaddr.split(".");
	if (each[0] == "127")
	{
		return false;
	}
	if (!ipArray || ipArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			thisSegment = ipArray[i];
			if (thisSegment != "")
			{
				if(i==3){
					if (!((ipArray[3] > 0) && (ipArray[3] < 255)))
					{
						return false;
					}	
				}
				else if (!(thisSegment >=0 && thisSegment <= 255))
				{
					return false;
				}
			} 
			else
			{
				return false;
			}
		}

	}
	return true;
}

function checksubnet(subnet)
{
	var subnetArray = subnet.split(".");
	var subnetstr = subnetArray[0]+subnetArray[1]+subnetArray[2]+subnetArray[3];
	var i = 0;
	var maskTest = 0;
	var validValue = true;

	if((subnetArray[0]=="")||(subnetArray[0]<0)||(subnetArray[0]>255)||(subnetArray[1]=="")||(subnetArray[1]<0)||(subnetArray[1]>255)
			||(subnetArray[2]=="")||(subnetArray[2]<0)||(subnetArray[2]>255)||(subnetArray[3]=="")||(subnetArray[3]<0)||(subnetArray[3]>255))
	{
		return false;
	}
	for(i=0;i<subnetstr.length;i++)
	{
		if((subnetstr.charAt(i)!='0')&&(subnetstr.charAt(i)!='1')&&(subnetstr.charAt(i)!='2')
				&&(subnetstr.charAt(i)!='3')&&(subnetstr.charAt(i)!='4')&&(subnetstr.charAt(i)!='5')
				&&(subnetstr.charAt(i)!='6')&&(subnetstr.charAt(i)!='7')&&(subnetstr.charAt(i)!='8')
				&&(subnetstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if (!subnetArray || subnetArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++) {
			thisSegment = subnetArray[i];
			if (thisSegment != "") {
				if (!(thisSegment >=0 && thisSegment <= 255)) { //check if number?

					return false;
				}
			} else {
				return false;
			}
		}
	}
	if( subnetArray[0] < 255 ) 
	{
		if( (subnetArray[1] > 0) || (subnetArray[2] > 0) || (subnetArray[3] > 0))
			validValue = false;
		else
			maskTest = subnetArray[0];
	} 
	else 
	{
		if( subnetArray[1] < 255 ) 
		{
			if( (subnetArray[2] > 0) || (subnetArray[3] > 0))
				validValue = false;
			else
				maskTest = subnetArray[1];
		} 
		else
		{
			if( subnetArray[2] < 255 ) 
			{
				if( (subnetArray[3] > 0) )
					validValue = false;
				else
					maskTest = subnetArray[2];
			} 
			else
				maskTest = subnetArray[3];
		}
	}
	if( validValue ) {
		switch( maskTest ) {
			case "0":
				case "128":
				case "192":
				case "224":
				case "240":
				case "248":
				case "252":
				case "254":
				case "255":
				break;
			default:
			validValue = false;
		}
		if( subnet == "0.0.0.0" )
			validValue = false;
	}
	else
		validValue = false;


	return validValue;
}

function checkgateway(gateway)
{
	var form = document.forms[0];
	var dgArray = gateway.split(".");
	var dgstr = dgArray[0]+dgArray[1]+dgArray[2]+dgArray[3];
	var i = 0;	

	if((dgArray[0]=="")||(dgArray[0]<0)||(dgArray[0]>255)||(dgArray[1]=="")||(dgArray[1]<0)||(dgArray[1]>255)
			||(dgArray[2]=="")||(dgArray[2]<0)||(dgArray[2]>255)||(dgArray[3]=="")||(dgArray[3]<0)||(dgArray[3]>255))
	{
		return false;
	}	
	for(i=0;i<dgstr.length;i++)
	{
		if((dgstr.charAt(i)!='0')&&(dgstr.charAt(i)!='1')&&(dgstr.charAt(i)!='2')
				&&(dgstr.charAt(i)!='3')&&(dgstr.charAt(i)!='4')&&(dgstr.charAt(i)!='5')
				&&(dgstr.charAt(i)!='6')&&(dgstr.charAt(i)!='7')&&(dgstr.charAt(i)!='8')
				&&(dgstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if( dgArray[0] > 223 || dgArray[0] == 0 )
		return false;
	if (gateway == "0.0.0.0" || gateway == "255.255.255.255")
	{
		return false;
	}
	if (gateway == "127.0.0.1")
	{
		return false;
	}
	if (!dgArray || dgArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++) {
			thisSegment = dgArray[i];
			if (thisSegment != "") {
				if (!(thisSegment >=0 && thisSegment <= 255)) { //check if number?

					return false;
				}
			} else {
				return false;
			}
		}
	}
	return true;
}

function getkey(type, e)
{
	var keycode, event;

	if (window.event) 
	{
		event = window.event;
		keycode = window.event.keyCode;
	}	
	else if (e)
	{
		event = e;
		keycode = e.which;
	}	
	else return true;

	if(event.ctrlKey && ( keycode==99 || keycode==118 || keycode==120) )
		return true;

	if(type == "apname")
	{
		if ((keycode==34) ||(keycode==39)||(keycode==92)) { return false; }
		else return true;
	}
	else if(type == "ipaddr")
	{	
		if (((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||(keycode==46)) { return true; }
		else return false;
	}
	else if(type == "ssid")
	{
		if (keycode==32) return false;
		else return true;
	}
	else if(type == "wep")
	{
		if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
			return true;
		else return false;
	}
	else if(type == "num")	
	{	
		if	(((keycode>47) && (keycode<58)) || (keycode==8)||(keycode==0))
			return true;		
		else return false;	
	}
	else if(type == "num_letter")
	{
		if((keycode>47 && keycode<58)||(keycode>64 && keycode<91)||(keycode>96 && keycode<123)||(keycode==8)||(keycode==0))
			return true; 
		else
			return false
	}
	else if(type == "hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "ddns_hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||(keycode==46)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "mac")
	{
		if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0) || (keycode == 58) || (keycode == 45))
			return true;
		else return false;
	}
	else if(type == "folderPath")// not / * ? " <>
	{
		if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34) || (keycode==60) || (keycode==62) ||(keycode==124))
			return false;
		else 
			return true;
	}
	else if(type == "shareName")// not / * ? " <> \ : 
	{
		if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34) || (keycode==58) || (keycode==60) || (keycode==62) || (keycode==92) || (keycode==93) || (keycode==124))
			return false;
		else
			return true;
	}
        else if(type == "mediaServerName")// not / * ? " <> \ :
        {
                if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34) || (keycode==58) || (keycode==60) || (keycode==62) || (keycode==92) || (keycode==93) || (keycode==124)){
			alert("$media_server_name_colon");
                        return false;
		}	
                else
                        return true;
        }
	else	
		return false;
}


function setDisabled(OnOffFlag,formFields)
{
	for (var i = 1; i < setDisabled.arguments.length; i++)
		setDisabled.arguments[i].disabled = OnOffFlag;
}

function maccheck_multicast(mac_addr)
{
	mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
	mac11 = mac11.substr(1,1);
	if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||(mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||(mac11=="B")||(mac11=="D")||(mac11=="F"))
	{
		//Reject MAC address like 11:xx:xx:xx:xx:xx
		alert(bh_invalid_mac);
		return false;
	}
	if(mac_array.length!=6)
	{
	        alert(bh_invalid_mac);
		return false;
	}
	if(( mac_array[0]=="")||( mac_array[1]=="")||( mac_array[2]=="")||( mac_array[3]=="")||( mac_array[4]=="")||( mac_array[5]==""))
	{
	        alert(bh_invalid_mac);
		return false;
	}
	if((( mac_array[0]=="00")&&( mac_array[1]=="00")&&
	                        ( mac_array[2]=="00")&&( mac_array[3]=="00")&&
				( mac_array[4]=="00")&&( mac_array[5]=="00"))||
			(( mac_array[0]=="ff")&&( mac_array[1]=="ff")&&
			 ( mac_array[2]=="ff")&&( mac_array[3]=="ff")&&
			  ( mac_array[4]=="ff")&&( mac_array[5]=="ff"))||
			(( mac_array[0]=="FF")&&( mac_array[1]=="FF")&&
			 ( mac_array[2]=="FF")&&( mac_array[3]=="FF")&&
			 ( mac_array[4]=="FF")&&( mac_array[5]=="FF")))
	{
		alert(bh_invalid_mac);
		return false;
	}
        if(( mac_array[0].length!=2)||( mac_array[1].length!=2)||
                        ( mac_array[2].length!=2)||( mac_array[3].length!=2)||
                        ( mac_array[4].length!=2)||( mac_array[5].length!=2))
        {
                alert(bh_invalid_mac);
                return false;
        }
        for(i=0;i<mac_addr.length;i++)
        {
                if(isValidMac(mac_addr.charAt(i))==false)
                {
                        alert(bh_invalid_mac);
                        return false;
                }
        }
        return true;
}

function setMAC(cf,this_mac)
{
	var dflag;
	if (cf.MACAssign[0].checked || cf.MACAssign[1].checked)
	{
		dflag = true;
		cf.Spoofmac.value=this_mac;
		setDisabled(dflag,cf.Spoofmac);
	}
	else
	{
		dflag = false;
		setDisabled(dflag,cf.Spoofmac);
		cf.Spoofmac.value=this_mac;
	}
}

function isValidMac(digit)
{
	var macVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9","A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f",":");
	var len = macVals.length;
	var i = 0;
	var ret = false;
	for ( i = 0; i < len; i++ )
		if ( digit == macVals[i] ) break;
	if ( i < len )
		ret = true;
	return ret;
}

function isValidChar(each_char)
{
	if( each_char < 33 || each_char > 126)
		return false;
}

