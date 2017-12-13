function manuallyConfig()
{
        var forms = document.getElementsByTagName("form");
        var cf = forms[0];

	this.location.href = "BRS_security.html";
        return true;
}


function initPage()
{
	//head
	var head_tag  = document.getElementsByTagName("h1");
	
	var connect_text = document.createTextNode(bh_no_internet_ip);
	head_tag[0].appendChild(connect_text);
	
	
	//input radio choices
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");
	
	var choices_text = document.createTextNode(bh_select_no_IP_option1);
	insertAfter(choices_text, choices[0]);
	
	choices_text = document.createTextNode(bh_select_no_IP_option2);
	insertAfter(choices_text, choices[1]);
	
	choices_text = document.createTextNode(bh_select_no_IP_option3);
	insertAfter(choices_text, choices[2]);

        var btn = document.getElementById("next");
	btn.value = bh_next_mark;
		if( master == "admin" )
        btn.onclick = function()
        {
                return  modemCycleChoice();
        }
	else
		btn.className="grey_short_btn";
	
	//show firmware version
	showFirmVersion("");
}


function modemCycleChoice()
{
	var forms = document.getElementsByTagName("form");
        var cf = forms[0];

	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");

	if(choices[0].checked || choices[1].checked)
		cf.submit();
	else if(choices[2].checked)
		this.location.href = "BRS_03A_E_IP_problem_staticIP.html";
	else
	{
		alert(bh_select_an_option);
		return false;
	}

	return true;
}


addLoadEvent(initPage);
