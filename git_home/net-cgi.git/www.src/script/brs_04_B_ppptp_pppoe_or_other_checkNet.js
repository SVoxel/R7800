function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_apply_connection);
	head_tag[0].appendChild(head_text);
	
	//paragraph
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_plz_waite_apply_connection);
	paragraph[0].appendChild(paragraph_text);

	//show Fireware Version
	showFirmVersion("none");
	
	loadValue();
}

function loadValue()
{
	if(ping_result == "success") //success
	{
		this.location.href='BRS_04_applySettings_ping.html';
	  
	}else if(count > 3)	//failed
	{
		this.location.href='BRS_04_applySettings_ping.html';
            
	}
	setTimeout("loadValue();", 1000);
}

addLoadEvent(initPage);
