function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_umts_3g_mode);
	head_tag[0].appendChild(head_text);

	//paragrah
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_plz_wait_process);
	paragraph[0].appendChild(paragraph_text);

	setTimeout("loadValue()", 30000);
}

function loadValue()
{
	this.location.href = "detect_succ_or_not.htm";
}


addLoadEvent(initPage);
