function initPage()
{
	var head_tag = document.getElementsByTagName("h1");

	var connect_text = document.createTextNode(bh_unable_detect_msg);
	head_tag[0].appendChild(connect_text);

	var paragraph = document.getElementsByTagName("p");

	var paragraph_text = document.createTextNode(bh_unable_detect_msg1);
	paragraph[0].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_want_try_again);
	paragraph[1].appendChild(paragraph_text);


	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");

	var choices_text = document.createTextNode(bh_yes_mark);
	insertAfter(choices_text, choices[0]);

	choices_text = document.createTextNode(bh_no_genie_help);
	insertAfter(choices_text, choices[1]);


	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function()
	{
		return genieHelpChecking();
	}

	var btn = document.getElementById("btn_div");
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);
}

function genieHelpChecking()
{
	var choices_div = document.getElementById("choices_div");
	var choices = choices_div.getElementsByTagName("input");

	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	if(choices[0].checked)
	{
		if(top.have_lte_flag=="1")
			cf.submit_flag.value="wizard_lte_detwan";
		cf.submit();
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
	else
	{
		alert(bh_select_an_option);
		return false;
	}

	return true;
}

addLoadEvent(initPage);
