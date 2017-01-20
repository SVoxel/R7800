function initPage()
{
	if(an_router_flag == "0")
	{
		if(wl_sectype == 1)
			noPreSecurityInit();
		else
			preSecurityInit();
	}
	else
	{
		if(wl_sectype == 1 && wla_sectype == 1)
			noPreSecurityInit();
		else
			preSecurityInit();
	}
}

function preSecurityInit()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_congratulations);
	head_tag[0].appendChild(head_text);
	
	//paragrah
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_connect_success_1);
	paragraph[0].appendChild(paragraph_text);
	
	//create tootip for network key (password)
	if(wl_tooltip1 == 1 || wla_tooltip1 == 1)
	{
		var tooltip = document.getElementById("key_tooltip_1");
		addTooltip(tooltip);
	}
	
	//2.4GHz
	var networkName_div = document.getElementById("network_name");
	var networkName_text;
	if(an_router_flag == "0")
	 	networkName_text = document.createTextNode(bh_wirless_name);
	else if(an_router_flag == "1")
		networkName_text = document.createTextNode("2.4GHz " + bh_wirless_name);

	networkName_div.appendChild(networkName_text);

	//5GHz
	networkName_div = document.getElementById("network_name_5G");
	networkName_text = document.createTextNode("5GHz " + bh_wirless_name);
	networkName_div.appendChild(networkName_text);

	var div_5G = document.getElementById("div_5G");
	var passwd_5G = document.getElementById("passwd_5G");
	if(an_router_flag == "0")
	{
		div_5G.style.display = "none";
		passwd_5G.style.display = "none";
	}
	else if(an_router_flag == "1")
	{
		div_5G.style.display = "";
		if(wl_sectype == wla_sectype && wl_passphrase == wla_passphrase)
			passwd_5G.style.display = "none";
		else
			passwd_5G.style.display = "";
	}
	
	//create tooltip for Network Key (Password)
	if(wl_tooltip1 == 1 || wla_tooltip1 == 1)
	{
		tooltip = document.getElementById("key_tooltip_2");
		addTooltip(tooltip);
	}
	if(wla_tooltip1 == 1)
	{
		tooltip = document.getElementById("key_tooltip_3");
		addTooltip(tooltip);
	}

	//initial the buttons text and onclick action
	initBtnsAction();
}

function noPreSecurityInit()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_congratulations);
	head_tag[0].appendChild(head_text);

	//paragrah
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_connect_success_1);
	paragraph[0].appendChild(paragraph_text);
	if(an_router_flag == "0")
	{
		var security_info_div_24 = document.getElementById("info_div_24");
		security_info_div_24.style.display = "none";
	}
	else
	{
		var security_info_div = document.getElementById("info_div");
		security_info_div.style.display = "none";
	}
	

	//initial the buttons text and onclick action
	initBtnsAction();
}

function initBtnsAction()
{
	//buttons middle
	var btns_div2 = document.getElementById("btnsContainer_div2");
	btns_div2.onclick = function()
	{
		return printSucessPage();
	}
	
	btn = btns_div2.getElementsByTagName("div");
	btn_text = document.createTextNode(bh_print_this);
	btn[0].appendChild(btn_text);
	
	//buttons middle
	var btns_div2 = document.getElementById("btnsContainer_div3");
	btns_div2.onclick = function()
	{
		return toInternet();
	}
	
	btn = btns_div2.getElementsByTagName("div");
	btn_text = document.createTextNode(bh_take_to_internet);
	btn[0].appendChild(btn_text);

	//show firmware version
        showFirmVersion("none");
}

function addTooltip(tooltip_name)
{
	tooltip_name.setAttribute("title", bh_rollover_help_text);
	tooltip_name.className = "tooltip";
}

function printSucessPage()
{
	if (window.print)
		window.print();
	else
	{
		alert(bh_not_support_print);
		return false;
	}

	return true;
}

function saveSettings()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	cf.action="/brs_backup.cgi";
	cf.submit_flag.value = "";
	cf.submit();

	return true;
}

function toInternet()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	var winoptions = "width=960,height=800,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes";
	window.open("to_internet_no_auth.htm","brs_ng",winoptions);

	return true;
}

function click_here()/* to fix bug 26919 */
{
	var cf = document.getElementsByTagName("form")[0];
	cf.click_flag.value="1";

	cf.action="apply.cgi?/BRS_success.html timestamp=" + ts;

	if(cf.encoding)
		cf.encoding = "application/x-www-form-urlencoded";
	else if(cf.enctype)
		cf.enctype = "application/x-www-form-urlencoded";

	cf.submit_flag.value = "hijack_success";
	cf.submit();

	var winoptions = "width=960,height=800,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes";
	window.open("change_to_wireless.htm","chge_towl",winoptions);
}

function XXXtoInternet()
{
        var winoptions = "width=960,height=800,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes"
        window.open("BRS_netgear_success.html",null,winoptions);
}

addLoadEvent(initPage);
