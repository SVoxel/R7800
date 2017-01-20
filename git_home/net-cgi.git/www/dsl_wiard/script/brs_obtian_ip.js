function initPage()
{
	if( apmode_flag == "1" && ap_mode == "1")
	{
		if(ping_result == "success")
			this.location.href = "BRS_success.html";
		else if(ping_result == "failed")
			this.location.href = "BRS_06_networkIssue.html";
	}else
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
				if( count < 10 && (text[0] == "" || text[0] == "..." || text[0] == "0.0.0.0"  || text[0] == "10.64.64.64"))
				{
					setTimeout("getip()", 10000);
				}else if(count == 10)
				{
					location.href="BRS_log09_obtionIP_no.html";
				}else
					location.href="BRS_log09_obtionIP_yes_ping.html";
		} else {
			  if ( tHttp.readyState == 4 && tHttp.status == 0 )
			  {
				if(top.location.href.indexOf("BRS_index.htm") > -1){
				  if(top.location.href.indexOf("routerlogin.net") > -1){
					setTimeout("top.location.href='http://routerlogin.com/BRS_index.htm';", 20000);
				  }else{
					setTimeout("top.location.href='http://routerlogin.net/BRS_index.htm';", 20000);
				  }
				}else{
					setTimeout("this.location.href='BRS_04_applySettings_ppp_second_obtain_ip.html';", 1000);
				}
			  }
		}
  }
  tHttp.open("GET",url,true);
  tHttp.send(null);
	
}
addLoadEvent(initPage);
