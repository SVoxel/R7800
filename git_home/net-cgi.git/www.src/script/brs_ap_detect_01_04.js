function initPage()
{
	//button Finish
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function() 
	{
		return goto_finish();
	}
	
	var btn = document.getElementById("btn_div");	
	var btn_text = document.createTextNode("Finish");
	btn.appendChild(btn_text);

	setTimeout("show_content_div()",25000);	
}

function goto_finish()
{
	if(router_or_ap == "0")
		this.location.href="BRS_01_checkNet_ping.html";
	if(router_or_ap == "1")
		this.location.href="BRS_00_03_ap_setup.html";
}

function show_content_div()
{
	document.getElementById("wait_div").style.display="none";
	document.getElementById("content_div").style.display="";
}

addLoadEvent(initPage);
