

function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_netword_issue);
	head_tag[0].appendChild(head_text);
	
	//paragrah
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_cannot_connect_internet);
	paragraph[0].appendChild(paragraph_text);
	
	paragraph_text = document.createTextNode(bh_try_again_or_manual_config);
	paragraph[1].appendChild(paragraph_text);
	
	//radio text
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");
		
	var choices_text = document.createTextNode(bh_yes_mark);
	insertAfter(choices_text, choices[0]);
					
	choices_text = document.createTextNode(bh_I_want_manual_config);
	insertAfter(choices_text, choices[1]);
	
	//button
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function() 
	{
		return clickNext();
	}
								
	var btn = document.getElementById("btn_div");	
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);
	
	//show Fireware Version
	showFirmVersion("");
}

function clickNext()
{
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");

	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	
	if(choices[0].checked)
	{
		location.href="BRS_start.html";
	}
	else if(choices[1].checked)
	{
		if(confirm(bh_no_genie_help_confirm) == false)
			return false;

		cf.submit();
		goto_home_page();

		return true;
	}
}

addLoadEvent(initPage);
