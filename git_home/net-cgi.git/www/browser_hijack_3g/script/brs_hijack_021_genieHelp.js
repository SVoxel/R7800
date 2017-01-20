function initPage()
{
	var head_tag  = document.getElementsByTagName("h1");
	
	var connect_text = document.createTextNode(bh_config_net_connection);
	head_tag[0].appendChild(connect_text);
	
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_connection_further_action);
	paragraph[0].appendChild(paragraph_text);
	
	paragraph_text = document.createTextNode(bh_want_genie_help);
	paragraph[1].appendChild(paragraph_text);
	
	
	
/*	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");
	
	var choices_text = document.createTextNode(bh_yes_mark);
	insertAfter(choices_text, choices[0]);
	
	choices_text = document.createTextNode(bh_no_genie_help);
	insertAfter(choices_text, choices[1]);
	
	choices_text = document.createTextNode(bh_have_saved_copy);
	insertAfter(choices_text, choices[2]);
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
	{
		this.location.href = "WIZ_sel_3g_adsl.htm";
	}
	else if(choices[1].checked)
	{
		if(confirm(bh_no_genie_help_confirm) == false)
			return false;

		cf.action = "/apply.cgi?/index.htm timestamp=" + ts;
		cf.target = "_top";
		cf.submit_flag.value = "hijack_toBasic";
		cf.submit();
	}
	else if(choices[2].checked)
	{
		this.location.href = "BRS_03B_haveBackupFile.html";
	}
	else
	{
		alert(bh_select_an_option);
		return false;
	}

	return true;
}


addLoadEvent(initPage);
