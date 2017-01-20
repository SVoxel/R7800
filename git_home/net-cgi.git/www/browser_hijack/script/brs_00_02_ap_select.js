function initPage()
{
	var head_tag  = document.getElementsByTagName("h1");
	
	var connect_text = document.createTextNode(bh_ap_select_title);
	head_tag[0].appendChild(connect_text);
	
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_ap_select_doc);
	paragraph[0].appendChild(paragraph_text);
	
	
	
	/*var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");
	
	var choices_text = document.createTextNode(bh_router_mode);
	insertAfter(choices_text, choices[0]);
	
	choices_text = document.createTextNode(bh_ap_mode);
	insertAfter(choices_text, choices[1]);
     */
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function() 
	{
		return genieHelpChecking();
	}
	
	var btn = document.getElementById("btn_div");	
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);

	//show firmware version
        showFirmVersion("none");
}


function genieHelpChecking()
{
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");

	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	if(choices[0].checked)
             location.href="BRS_01_checkNet_ping.html";
	else if(choices[1].checked)
	{
		location.href="BRS_00_03_ap_setup.html";
	}
	else
	{
		alert(bh_select_an_option);
		return false;
	}

	return true;
}

function goto_url()
{
	window.open('ap_learn.html', 'show_statistic');
	//location.href="BRS_ap_learn_more.html";
}
addLoadEvent(initPage);
