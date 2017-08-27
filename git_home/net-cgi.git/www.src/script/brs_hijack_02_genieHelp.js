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
	
	
	var btn = document.getElementById("next");
	btn.value = bh_next_mark;
	if( master == "admin" )
	{
		btn.onclick = function() 
		{
			return genieHelpChecking();
		}
	}
	else
		btn.className="grey_short_btn";

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
		cf.submit();
	}
	else if(choices[1].checked)
	{
		if(confirm(bh_no_genie_help_confirm) == false)
			return false;

		this.location.href = "BRS_security.html";
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
