function initPage()
{
	getip();
}
function getip()
{
	count = count + 1;
	var tHttp = createXMLHttpRequest();
	var url = "/obtain_ip.txt?ts=" + new Date().getTime();
	tHttp.onreadystatechange = function()
	{
		if(tHttp.readyState == 4 && tHttp.status == 200)
		{
			var respText = tHttp.responseText;
				var text = respText.split("\n");
				if( count < 5 && (text[0] == "" || text[0] == "..." || text[0] == "0.0.0.0"  || text[0] == "10.64.64.64"))
				{
					setTimeout("getip()", 10000);
				}else if(count == 5)
				{
					location.href="BRS_log09_obtionIP_no.html";
				}else
					location.href="BRS_log09_obtionIP_yes_ping.html";
		} 
	  }
  tHttp.open("GET",url,true);
  tHttp.send(null);
	
}
addLoadEvent(initPage);
