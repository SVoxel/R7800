
function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	
	var head_text = document.createTextNode(bh_no_cable);
	var wait_text = document.createTextNode(bh_detecting_connection);
	head_tag[0].appendChild(head_text);
	var btns_container_div = document.getElementById("btnsContainer_div2");
	btns_container_div.onclick = function() 
	{
		return genienowanChecking();
	}
	
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_wizard_setup_nowan_check);
	paragraph[0].appendChild(paragraph_text);
	var btnskip = document.getElementById("btnskip_div");
	var btnskip_text = document.createTextNode(bh_next_mark);
	btnskip.appendChild(btnskip_text);

	//show firmware version
        showFirmVersion("");
}
function genienowanChecking()
{
	var cf = document.forms[0];
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");
	if(choices[0].checked)
	{
		this.location.href = "BRS_00_01_check_ap_wait.html";
	}
	else{
		top.click_no_flag=1;
		this.location.href = "BRS_security.html";
	}
}
function clickRetry()
{
	this.location.href = "BRS_00_01_check_ap_wait.html";
	return true;
}
function clickskip()
{
	top.wan_port=1;
	this.location.href = "BRS_00_01_check_ap.html";
	return true;
}


addLoadEvent(initPage);
