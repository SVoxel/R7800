function initPage()
{
	//head
	var head_tag  = document.getElementsByTagName("h1");
	
	var connect_text = document.createTextNode(bh_no_internet_ip3);
	head_tag[0].appendChild(connect_text);
	
	
	//paragraph
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_use_pc_mac);
	paragraph[0].appendChild(paragraph_text);
	
	paragraph_text = document.createTextNode(bh_mac_in_product_label);
	paragraph[1].appendChild(paragraph_text);
	
	paragraph_text = document.createTextNode(bh_enter_mac);
	paragraph[2].appendChild(paragraph_text);
	
	//input
	var input_div = document.getElementById("mac_input_div")
	var input_item = input_div.getElementsByTagName("input");
	
	var input_text = document.createTextNode(bh_mac_format);
	insertAfter(input_text, input_item[0]);
	

	//set input event action
	var mac_input = document.getElementById("spoofmac");
	mac_input.onkeypress = macKeyCode;
	

	//buttons

	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )	
	btn.onclick = function() 
	{
		return 	checkMacApply();
	}
	
	//show firmwar version
	showFirmVersion("");
}


function checkMacApply()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	//set input event action
	var mac_input = document.getElementById("spoofmac");
	var the_mac = mac_input.value;

	if(the_mac.indexOf(":")==-1 && the_mac.length=="12")
	{
		var tmp_mac=the_mac.substr(0,2)+":"+the_mac.substr(2,2)+":"+the_mac.substr(4,2)+":"+the_mac.substr(6,2)+":"+the_mac.substr(8,2)+":"+the_mac.substr(10,2);
		
		mac_input.value = tmp_mac;
	}
	else if (the_mac.split("-").length == 6)
	{
		var tmp_mac = the_mac.replace(/-/g,":");
		mac_input.value = tmp_mac;
	}

	if(maccheck_multicast(mac_input.value) == false)
		return false;

	cf.submit();
	
	return true;
}


addLoadEvent(initPage);
