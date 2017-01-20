function initPage()
{

	//button back
	var btns_div1 = document.getElementById("btnsContainer_div1");
	if( master == "admin" )
	btns_div1.onclick = function()
	{
		return goback();
	}
	
	var btn = btns_div1.getElementsByTagName("div");
	var btn_text = document.createTextNode(bh_back_mark);
	btn[0].appendChild(btn_text);
	
	
	//button next
	var btns_div2 = document.getElementById("btnsContainer_div2");
	if( master == "admin" )
	btns_div2.onclick = function()
	{
		return goto_next();
	}
	
	btn = btns_div2.getElementsByTagName("div");
	btn_text = document.createTextNode(bh_next_mark);
	btn[0].appendChild(btn_text);
}

function goto_next()
{
	this.location.href = "BRS_ap_detect_01_router_02.html";	
}
function goback()
{
	this.location.href = "BRS_ap_detect_01_03.html";
}
addLoadEvent(initPage);