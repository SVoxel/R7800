function initPage()
{

	//button back
	var btns_div1 = document.getElementById("btnsContainer_div1");
	if( master == "admin" )
	btns_div1.onclick = function()
	{
		return goto_url(0);
	}
	
	var btn = btns_div1.getElementsByTagName("div");
	var btn_text = document.createTextNode(bh_back_mark);
	btn[0].appendChild(btn_text);
	
	
	//button next
	var btns_div2 = document.getElementById("btnsContainer_div2");
	if( master == "admin" )
	btns_div2.onclick = function()
	{
		return goto_url(1);
	}
	
	btn = btns_div2.getElementsByTagName("div");
	btn_text = document.createTextNode(bh_next_mark);
	btn[0].appendChild(btn_text);
}

function goto_url(tag)
{
	var pre_url = document.referrer;
	if(tag == 0)
	{
		if(pre_url.indexOf("BRS_ap_detect_01_02.html") != -1)	
			this.location.href = "BRS_ap_detect_01_02.html";
		else if(pre_url.indexOf("BRS_ap_detect_01_03.html") != -1)
			this.location.href = "BRS_ap_detect_01_03.html"
		else
		{
			if(pre_url.indexOf("BRS_ap_detect_01_ap_02.html?id=0") != -1)
				this.location.href = "BRS_ap_detect_01_02.html";
			else
				this.location.href = "BRS_ap_detect_01_03.html"
		}

	}
	else
	{	
		if(pre_url.indexOf("BRS_ap_detect_01_02.html") != -1)
			this.location.href = "BRS_ap_detect_01_ap_02.html?id=0";
		else
			this.location.href = "BRS_ap_detect_01_ap_02.html?id=1";
	}
}

addLoadEvent(initPage);
