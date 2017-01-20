function loadValue()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	if(ping_result == "failed")     //failed
		this.location.href = "BRS_05_networkIssue.html";
	else if(ping_result == "success") //success
		this.location.href = "BRS_security.html";
	setTimeout("loadValue();", 1000);
}

addLoadEvent(loadValue);
