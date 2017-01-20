var invalid_ip="$invalid_des_ip";
var routes_length_10="$routes_length_10";
var routes_name_dup="$routes_name_dup";
var routes_condition_dup="$routes_condition_dup";
var routes_name_null="$routes_name_null";
var routes_diff_wan_gateway="$routes_diff_wan_gateway";
var routes_metric_error="$routes_metric_error";

function countmask(num)
{
	var i = 0;
	var count=0;
	var numArr = [128, 64, 32, 16, 8, 4, 2, 1];
	for ( i = 0; i < numArr.length; i++ )
		if ( (num & numArr[i]) != 0 )
			count++;
	return count;
}

function check_routers_add(cf)
{
	if( array_num == 32 )
	{
		alert("$routes_length_10");
		return false;
	}
	else
		location.href="STR_routes_add.htm";
}
function check_router_add(cf,flag)
{
	var name = cf.route_name.value;
	if(flag == 'add' && array_num == 32 )
	{
		alert("$routes_length_10");
		return false;
	}
	if(name == "" )
	{
		alert("$routes_name_null");
		return false;
	}
        for(i=0;i<name.length;i++)/* to fix bug 24889,change the value of i start with 0, but not 1.*/
        {
		if(isValidChar_space(name.charCodeAt(i))==false)
		{
			alert("$invalid_router_name");
			return false;
		}
	}
	/* To fix bug27367: [https/http][NEW GUI][Static Route]When set route name
	 * as space,it should not show blank as route name
	 *
	 * According to spec2.0:
	 * 3.2 Static Routing
	 * Route Name
	 * This is a mnemonic name for the route. A name consists of printable ASCII characters (ASCII
	 * 0x20 ~ 0x7E) and is up to 30 characters.
	 *
	 * So it should not remove_space, for the space can be route name(ASCII 0x20)
	 */
	//cf.route_name.value = remove_space(cf.route_name.value);
	cf.SRouteDestAddr.value=cf.SRouteDestAddr1.value+'.'+cf.SRouteDestAddr2.value+'.'+cf.SRouteDestAddr3.value+'.'+cf.SRouteDestAddr4.value;
	var ipaddr_array=cf.SRouteDestAddr.value.split('.');
	if(checkIP(ipaddr_array[0],ipaddr_array[1],ipaddr_array[2],ipaddr_array[3],254)==false)
	{
		alert("$invalid_des_ip");
		return false;
	}
	if(cf.SRouteDestAddr1.value=="127")
	{
		alert("$invalid_des_ip");
		return false;
	}
	if(parseInt(ipaddr_array[0])>=224)
	{
		alert("$invalid_des_ip");
		return false;
	}
	cf.SRouteSubnetMask.value=cf.SRouteSubnetMask1.value+'.'+cf.SRouteSubnetMask2.value+'.'+cf.SRouteSubnetMask3.value+'.'+cf.SRouteSubnetMask4.value;
	if(checksubnet(cf.SRouteSubnetMask.value, 1)==false)
	{
		alert("$invalid_mask");
		return false;
	}
	cf.SRouteSubnetMask.value = address_parseInt(cf.SRouteSubnetMask.value);
	if(isSameSubNet(cf.SRouteDestAddr.value, cf.SRouteSubnetMask.value, lan_ip, cf.SRouteSubnetMask.value) == true || isSameSubNet(cf.SRouteDestAddr.value, lan_mask, lan_ip, lan_mask) == true ){
		alert("$invalid_des_ip");
		return false;
	}
	//fixed bug 29923
	if (cf.SRouteDestAddr4.value=="0" && cf.SRouteSubnetMask.value=="255.255.255.255")
	{
		alert("$invalid_des_ip");
		return false;
	}
	if (gui_region == "Russian" || netgear_region.toUpperCase() == "RU"){
		if( wan_ip != "0.0.0.0" && ( isSameSubNet(cf.SRouteDestAddr.value, cf.SRouteSubnetMask.value, wan_ip, cf.SRouteSubnetMask.value) == true || isSameSubNet(cf.SRouteDestAddr.value, wan_mask, wan_ip, wan_mask) == true ) && isSameSubNet(cf.SRouteDestAddr.value, cf.SRouteSubnetMask.value, wan_ip, wan_mask) == true ){
			alert("$conflicted_with_wanip");
			return false;
		}
	}
	else {

		if( wan_ip != "0.0.0.0" && ( isSameSubNet(cf.SRouteDestAddr.value, cf.SRouteSubnetMask.value, wan_ip, cf.SRouteSubnetMask.value) == true || isSameSubNet(cf.SRouteDestAddr.value, wan_mask, wan_ip, wan_mask) == true )){
			alert("$conflicted_with_wanip");
			return false;
		}
	}
	
	cf.SRouteGatewayAddr.value=cf.SRouteGatewayAddr1.value+'.'+cf.SRouteGatewayAddr2.value+'.'+cf.SRouteGatewayAddr3.value+'.'+cf.SRouteGatewayAddr4.value;
	if(checkgateway(cf.SRouteGatewayAddr.value)==false || is_sub_or_broad(cf.SRouteGatewayAddr.value, cf.SRouteGatewayAddr.value, cf.SRouteSubnetMask.value) == false)
	{
		alert("$invalid_gateway");
		return false;
	}
	cf.SRouteGatewayAddr.value = address_parseInt(cf.SRouteGatewayAddr.value);
	/*27957 if(isSameIp(cf.SRouteGatewayAddr.value, wan_ip) == true || isSameIp(cf.SRouteGatewayAddr.value, lan_ip) == true)
	{
		alert("$invalid_gateway");
		return false;
	}*/
/* Bug 30290 - [spec rev.10] 38. Static Route: allow a static route to be added even if the gateway is not located on any interface currently active
	if(wan_ip == "0.0.0.0" && (isSameSubNet(cf.SRouteGatewayAddr.value,lan_mask,lan_ip,lan_mask) == false || isGateway(lan_ip, lan_mask, cf.SRouteGatewayAddr.value) == false))
	{
		alert("$routes_diff_wan_gateway");
		return false;
	}
	else if ( (wan_proto == "pptp") || (wan_proto == "l2tp" && (gui_region == "Russian" || netgear_region.toUpperCase() == "RU")) || (wan_proto == "pppoe" && russian_pppoe_flag == 1 && (gui_region == "Russian" || netgear_region.toUpperCase() == "RU")))
	{
		if( (isSameSubNet(cf.SRouteGatewayAddr.value,wan_mask,wan_ip,wan_mask) == false )  && (isSameSubNet(cf.SRouteGatewayAddr.value,pptp_eth1_wanmask,pptp_eth1_wanip,pptp_eth1_wanmask) == false || isGateway(pptp_eth1_wanip,pptp_eth1_wanmask,cf.SRouteGatewayAddr.value) == false) && (isSameSubNet(cf.SRouteGatewayAddr.value,lan_mask,lan_ip,lan_mask) == false || isGateway(lan_ip, lan_mask, cf.SRouteGatewayAddr.value) == false))
		{
			alert("$routes_diff_wan_gateway");
			return false;
		}		
	}
	else if ( wan_proto == "pppoe" )
	{
		if( (isSameSubNet(cf.SRouteGatewayAddr.value,wan_mask,wan_ip,wan_mask) == false ) && (isSameSubNet(cf.SRouteGatewayAddr.value,lan_mask,lan_ip,lan_mask) == false || isGateway(lan_ip, lan_mask, cf.SRouteGatewayAddr.value) == false))
		{
			alert("$routes_diff_wan_gateway");
			return false;
		}
	}
	else if((isSameSubNet(cf.SRouteGatewayAddr.value,wan_mask,wan_ip,wan_mask) == false || isGateway(wan_ip,wan_mask,cf.SRouteGatewayAddr.value) == false) && (isSameSubNet(cf.SRouteGatewayAddr.value,lan_mask,lan_ip,lan_mask) == false || isGateway(lan_ip, lan_mask, cf.SRouteGatewayAddr.value) == false))	
	{
		alert("$routes_diff_wan_gateway");
		return false;
	}

*/
	var ipArray = cf.SRouteDestAddr.value.split(".");
	var subnetArray = cf.SRouteSubnetMask.value.split(".");
	var addr = new Array();
	var count = 0;
	for (i=0;i<4;i++)
	{
		addr[i] = ipArray[i] & subnetArray[i];
		count += countmask(subnetArray[i]);
	}
	cf.count.value = count;
	var route_dest = addr[0]+'.'+addr[1]+'.'+addr[2]+'.'+addr[3];
	document.forms[0].route_dest.value=route_dest;
	if(isNaN(parseInt(cf.route_metric.value,10))==true || parseInt(cf.route_metric.value,10)<2|| parseInt(cf.route_metric.value,10)>15)
	{
		alert("$routes_metric_error");
		return false;
	}
	cf.route_metric.value = parseInt(cf.route_metric.value,10);/* to fix bug 25552 */
	for(i=1;i<=array_num;i++)
	{
		var str = eval ( 'routerArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");;
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&nbsp;/g, " ");
		//31091, if input "&nbsp;" as router name, page can't show "&nbsp;", it show " ". 
		each_info[0] = each_info[0]. replace(/&nbsp;/g, " ").replace(/&#38;/g,"&"); 
	
		if(flag == 'edit')
		{	
			if( cf.route_name.value == each_info[0]&& select_editnum != i)
			{
				alert("$routes_name_dup");
				return false;
			}
			if( route_dest == each_info[3] && select_editnum != i)
			{
				alert("$routes_condition_dup");
				return false;
			}
		}
		else
		{
			if( cf.route_name.value == each_info[0])
			{
				alert("$routes_name_dup");
				return false;
			}
			if( route_dest == each_info[3] )
			{
				alert("$routes_condition_dup");
				return false;
			}
		}
	}

	if (cf.SRouteActive.checked==false)
		cf.route_ac.value = 0;
	else
		cf.route_ac.value = 1;
	if (cf.SRoutePrivate.checked==false)
		cf.route_pr.value = 0;
	else
		cf.route_pr.value = 1;

	/*31091, if input "&nbsp;" as router name, page can't show "&nbsp;", it show " ". 
	Add replace(/&/g, "&#38;"), then, "&nbsp;" will save as "&38;nbsp;" and page will show "&nbsp", not show " " */
	cf.route_name.value = cf.route_name.value.replace(/&/g, "&#38;").replace(/\s/g, "&nbsp;");

	cf.submit();
	return true;
}
function check_router_editnum()
{
	if (array_num == 0)
	{
		alert("$port_edit");
		return false;
	}
	var count_select=0;
	var select_num;
	if(array_num == 1)
	{
		if(document.forms[0].select.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(document.forms[0].select[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		document.forms[0].select_edit.value=select_num;
		document.forms[0].submit_flag.value="st_router_editnum";
		document.forms[0].action="/apply.cgi?/STR_routes_edit.htm timestamp="+ts;
		document.forms[0].submit();
		return true;
	}
}

function check_router_del()
{
	if ( array_num == 0 )
	{
		alert("$port_del");
		location.href="STR_routes.htm";
		return false;
	}
	var count_select=0;
	var del_num;
	if( array_num == 1)
	{
		if(document.forms[0].select.checked == true)
			del_num=1;
		else
		{
			alert("$port_del");
			location.href="STR_routes.htm";
			return false;
		}
	}
	else
	{
		for(i=0;i<array_num;i++)
			if(document.forms[0].select[i].checked == true)
			{
				count_select++;
				del_num=i+1;
			}
		if(count_select==0)
		{
			alert("$port_del");
			location.href="STR_routes.htm";
			return false;
		}
	}
	document.forms[0].select_del.value=del_num;
	document.forms[0].submit();
	return true;
}
function check_routers_add2(cf)
{
	if(ipv6_array_num == 32)
	{
		alert("$routes_length_10");
		return false;
	}
	else
		location.href="STR_routes_add2.htm";
}

function check_router_add2(cf,flag)
{
	if(flag == 'add' && ipv6_array_num == 32 )
	{
		alert("$routes_length_10");
		return false;
	}
	var name = cf.route_name.value;
	if(name == "" )
	{
		alert("$routes_name_null");
		return false;
	}
	for(i=0;i<name.length;i++)
	{
		if(isValidChar_space(name.charCodeAt(i))==false)
		{
			alert("$invalid_router_name");
			return false;
		}
	}
	if(isNaN(parseInt(cf.route_metric.value,10))==true || parseInt(cf.route_metric.value,10)<2|| parseInt(cf.route_metric.value,10)>15)
	{
		alert("$routes_metric_error");
		return false;
	}
	cf.route_metric.value = parseInt(cf.route_metric.value,10);
	
	var route_dest, route_gtw;
	route_dest = "";
	route_gtw = "";
	for(i=0; i<cf.SRouteDestAddr.length; i++)
	{
		if(check_ipv6_IP_address(cf.SRouteDestAddr[i].value) == false)
		{
			alert("$invalid_des_ipv6_hex");
			return false;
		}
		if(cf.SRouteDestAddr[i].value != "")
		{
			cf.SRouteDestAddr[i].value = parseInt(cf.SRouteDestAddr[i].value, 16).toString(16);
			route_dest = route_dest + cf.SRouteDestAddr[i].value + ":";
		}
		else
			route_dest = route_dest + "0" + ":";
		if(check_ipv6_IP_address(cf.SRouteGatewayAddr[i].value) == false)
		{
			alert("$invalid_gtw_ipv6_hex");
			return false;
		}
		if(cf.SRouteGatewayAddr[i].value != "")
		{
			cf.SRouteGatewayAddr[i].value = parseInt(cf.SRouteGatewayAddr[i].value, 16).toString(16);
			route_gtw = route_gtw + cf.SRouteGatewayAddr[i].value +":";
		}
		else
			route_gtw = route_gtw + "0" + ":";
	}
	route_dest = route_dest.substring(0, route_dest.length - 1);
	route_gtw = route_gtw.substring(0, route_gtw.length - 1);
	if(route_dest == ":::::::")
		route_dest = ""
	else if(route_dest == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_des_ip");
		return false;
	}
	if(check_addr_legality(route_dest) == false)
	{
		alert("$invalid_des_ip");
		return false;
	}
	if(check_ipv6_IP_address(cf.SRouteDestAddrPrefix.value) == false)
	{
		alert("$invalid_des_ipv6_pre_length");
		return false;
	}
	if(cf.SRouteDestAddrPrefix.value == "" ||  parseInt(cf.SRouteDestAddrPrefix.value, 10) > 126 || parseInt(cf.SRouteDestAddrPrefix.value, 10) < 4)
	{
		alert("$invalid_des_ipv6_pre_length");
		return false;
	}
	
	if(route_gtw == ":::::::")
		route_gtw = "";
	else if(route_gtw == "0:0:0:0:0:0:0:0")
	{
		alert("$invalid_gateway");
		return false;
	}
	if(check_addr_legality(route_gtw) == false)
	{
		alert("$invalid_gateway");
		return false;
	}

	for(i=1;i<=ipv6_array_num;i++)
	{
		var str = eval ( 'ipv6_routerArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
		var each_info=str.split(' ');
		each_info[0] = each_info[0].replace(/&nbsp;/g, " ").replace(/&#38;/g,"&");
		if(flag == 'edit')
		{
			if( cf.route_name.value == each_info[0]&& select_editnum != i)
			{
				alert("$routes_name_dup");
				return false;
			}
			if( route_dest == each_info[2] && select_editnum != i)
			{
				alert("$routes_condition_dup");
				return false;
			}
		}
		else
		{
			if( cf.route_name.value == each_info[0])
			{
				alert("$routes_name_dup");
				return false;
			}
			if( route_dest == each_info[2] )
			{
				alert("$routes_condition_dup");
				return false;
			}
		}
	}
	cf.route_name.value = cf.route_name.value.replace(/&/g, "&#38;").replace(/\s/g, "&nbsp;");
	cf.route_dest.value = route_dest;
	cf.route_dest_pre.value = cf.SRouteDestAddrPrefix.value;
	cf.route_gtw.value = route_gtw;
	if (cf.SRouteActive.checked==false)
		cf.route_ac.value = 0;
	else
		cf.route_ac.value = 1;
	return true;
}

function check_router_editnum2()
{
	var count_select=0;
	var select_num;
	if (ipv6_array_num == 0)
	{
		alert("$port_edit");
		return false;
	}
	if(ipv6_array_num == 1)
	{
		if(document.forms[1].select.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<ipv6_array_num;i++)
		if(document.forms[1].select[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$port_edit");
		return false;
	}
	document.forms[1].select_edit.value=select_num;
	document.forms[1].submit_flag.value="st_router_editnum2";
	document.forms[1].action="/apply.cgi?/STR_routes_edit2.htm timestamp="+ts;
	return true;
}

function check_router_del2()
{
	var count_select=0;
	var del_num;
	if(ipv6_array_num == 0)
	{
		alert("$port_del");
		location.href="STR_routes.htm";
		return false;
	}
	if(ipv6_array_num == 1)
	{
		if(document.forms[1].select.checked == true)
		{
			del_num=1;
			count_select++;
		}
	}
	else for(i=0; i<ipv6_array_num; i++)
		if(document.forms[1].select[i].checked == true)
		{
			count_select++;
			del_num=i+1;
		}
	if(count_select==0)
	{
		alert("$port_del");
		location.href="STR_routes.htm";
		return false;
	}
	document.forms[1].select_del.value=del_num;
	document.forms[1].action = "/apply.cgi?/STR_routes.htm timestamp="+ts;
	return true;
}
