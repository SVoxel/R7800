function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h2");
	var head_text = document.createTextNode(bh_fix_ip_setting);
	head_tag[0].appendChild(head_text);
	
	var paragraph = document.getElementsByTagName("p");
	
	//main content items
	var IP_addr_div = document.getElementById("IP_addr");
	var IP_addr_text = document.createTextNode(bh_info_mark_ip);
	IP_addr_div.appendChild(IP_addr_text);
	
	var Sub_mask_div = document.getElementById("Sub_mask");
	var Sub_mask_text = document.createTextNode(bh_info_mark_mask);
	Sub_mask_div.appendChild(Sub_mask_text);
	
	var Gateway_div = document.getElementById("GatewayAddr")
	var Gateway_text = document.createTextNode(bh_constatus_defgtw);
	Gateway_div.appendChild(Gateway_text);
	
	var preDNS_div = document.getElementById("preDNS")
	var preDNS_text = document.createTextNode(bh_preferred_dns);
	preDNS_div.appendChild(preDNS_text);
	
	var alteDNS_div = document.getElementById("alteDNS")
	var alteDNS_text = document.createTextNode(bh_alternate_dns);
	alteDNS_div.appendChild(alteDNS_text);

        var thirdDNS_div = document.getElementById("thirdDNS");
	var thirdDNS_input = document.getElementById("third_dns_input");

	if( dns_third_flag == 1 )
	{
        	var thirdDNS_text = document.createTextNode(bh_basic_int_third_dns);
        	thirdDNS_div.appendChild(thirdDNS_text);
	}
	else
	{
		thirdDNS_div.style.display = "none";
		thirdDNS_input.style.display = "none";
	}

	//buttons 
	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )
	btn.onclick = function()
	{
		return checkStaticIP();
	}
	else
		btn.className="grey_short_btn";

	//show firmware version
        showFirmVersion("none");
}

function checkStaticIP()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	cf.ip_address.value = cf.WPethr1.value+'.'+cf.WPethr2.value+'.'+cf.WPethr3.value+'.'+cf.WPethr4.value;
	cf.subnet_mask.value = cf.WMask1.value+'.'+cf.Wmask2.value+'.'+cf.Wmask3.value+'.'+cf.Wmask4.value;
	cf.gateway.value = cf.WGateway1.value+'.'+cf.Wgateway2.value+'.'+cf.Wgateway3.value+'.'+cf.Wgateway4.value;

	if(check_static_ip_mask_gtw()==false)
		return false;
	if(check_static_dns(true)==false)
		return false;
	if(check_ether_samesubnet()==false)
		return false;

	cf.submit();

	return true;
}

function check_static_ip_mask_gtw()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	if(checkipaddr(cf.ip_address.value)==false || is_sub_or_broad(cf.ip_address.value, cf.ip_address.value, cf.subnet_mask.value) == false)
	{
		alert(bh_invalid_ip);
		return false;
	}
	if(checksubnet(cf.subnet_mask.value)==false)
	{
		alert(bh_invalid_mask);
		return false;
	}
	if(checkgateway(cf.gateway.value)==false)
	{
		alert(bh_invalid_gateway);
		return false;
	}
	if(isSameIp(cf.ip_address.value, cf.gateway.value) == true)
	{
		alert(bh_invalid_gateway);
		return false;
	}

	return true;
}

function check_ether_samesubnet()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	if(isSameSubNet(cf.ip_address.value,cf.subnet_mask.value,lan_ip,lan_subnet) == true)
	{
		alert(bh_same_lan_wan_subnet);
		return false;
	}
	if(isSameSubNet(cf.ip_address.value,lan_subnet,lan_ip,lan_subnet) == true)
	{
		alert(bh_same_lan_wan_subnet);
		return false;
	}
	if(isSameSubNet(cf.ip_address.value,cf.subnet_mask.value,lan_ip,cf.subnet_mask.value) == true)
	{
		alert(bh_same_lan_wan_subnet);
		return false;
	}
	if(isSameIp(cf.ip_address.value,lan_ip) == true)
	{
		alert(bh_same_lan_wan_subnet);
		return false;
	}
	
	return true;
}

function check_static_dns(wan_assign)
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	cf.preferred_dns.value = cf.DAddr1.value+'.'+cf.DAddr2.value+'.'+cf.DAddr3.value+'.'+cf.DAddr4.value;
	cf.alternate_dns.value = cf.PDAddr1.value+'.'+cf.PDAddr2.value+'.'+cf.PDAddr3.value+'.'+cf.PDAddr4.value;
	if( dns_third_flag == 1 )
		cf.third_dns.value = cf.TDAddr1.value+'.'+cf.TDAddr2.value+'.'+cf.TDAddr3.value+'.'+cf.TDAddr4.value;

	if( dns_third_flag == 1 )
		return check_three_DNS(cf.preferred_dns.value, cf.alternate_dns.value, cf.third_dns.value, wan_assign,cf.ip_address.value);
	else
		return check_DNS(cf.preferred_dns.value,cf.alternate_dns.value,wan_assign,cf.ip_address.value);
}

addLoadEvent(initPage);



