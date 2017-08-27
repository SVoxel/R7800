function initPage()
{
	var head_tag  = document.getElementsByTagName("h1");
	
	var connect_text = document.createTextNode(bh_no_internet_ip2);
	head_tag[0].appendChild(connect_text);
	
	//radio text	
	var choices =document.getElementsByTagName("input");
	
	var choices_text = document.createTextNode(bh_yes_correct);
	insertAfter(choices_text, choices[0]);
	
	choices_text = document.createTextNode(bh_not_have_static_ip);
	insertAfter(choices_text, choices[1]);
	
	choices_text = document.createTextNode(bh_do_not_know);
	insertAfter(choices_text, choices[2]);
	
	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )
	btn.onclick = function() 
	{
		return 	staticIPChoice();
	}
	else
		btn.className="grey_short_btn";
	
	//show firmware version
	showFirmVersion("");
}


function staticIPChoice()
{
	var choices =document.getElementsByTagName("input");

	if(choices[0].checked)
		this.location.href = "BRS_03A_E_IP_problem_staticIP_A_inputIP.html";
	else if(choices[1].checked || choices[2].checked)
		this.location.href = "BRS_03A_E_IP_problem_staticIP_B_macClone.html";
	else
	{
		alert(bh_select_an_option);
		return false;
	}
}


addLoadEvent(initPage);
