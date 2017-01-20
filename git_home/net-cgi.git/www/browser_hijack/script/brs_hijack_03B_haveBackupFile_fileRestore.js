function initPage()
{
	if(restore_status != "2")
	{
		//head text
		var head_tag = document.getElementsByTagName("h1");
		var head_text = document.createTextNode(bh_settings_restoring);
		head_tag[0].appendChild(head_text);
	
		//paragrah
		var paragraph = document.getElementsByTagName("p");
		var paragraph_text = document.createTextNode(bh_plz_waite_restore);
		paragraph[0].appendChild(paragraph_text);
	}
	else
	{
		//head text
		var head_tag = document.getElementsByTagName("h1");
		head_tag[0].setAttribute("class", "h1_red");
		var head_text = document.createTextNode(bh_failure_head);
		head_tag[0].appendChild(head_text);
		
		//paragrah
		var paragraph = document.getElementsByTagName("p");
		var paragraph_text = document.createTextNode(bh_few_second);
		paragraph[0].appendChild(paragraph_text);
	}
	
	statusChecking();
}

function statusChecking()
{
	if(restore_status == "0" || restore_status == "1")
	{
		setTimeout("pageRedirect(1)", 1000);
	}
	else if(restore_status == "2")
	{
		setTimeout("pageRedirect(2)", 3000);
	}
	else if(restore_status == "3")
	{
		var forms = document.getElementsByTagName("form");
		var cf = forms[0];

		cf.submit();
	}
}

function pageRedirect(stat)
{
	if(stat == 1)
		this.location.href = "BRS_03B_haveBackupFile_fileRestore.html";
	else if(stat == 2)
		this.location.href = "BRS_03B_haveBackupFile.html";
}


addLoadEvent(initPage);
