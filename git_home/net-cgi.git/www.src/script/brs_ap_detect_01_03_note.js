function initPage()
{

	//button back
	var btns_div1 = document.getElementById("btnsContainer_div1");
	if( master == "admin" )
	btns_div1.onclick = function()
	{
		return go_back();
	}
	
	var btn = btns_div1.getElementsByTagName("div");
	var btn_text = document.createTextNode(bh_back_mark);
	btn[0].appendChild(btn_text);
	
	
	//button next
	var btns_div2 = document.getElementById("btnsContainer_div2");
	if( master == "admin" )
	btns_div2.onclick = function()
	{
		return checkap(document.forms[0]);
	}
	
	btn = btns_div2.getElementsByTagName("div");
	btn_text = document.createTextNode(bh_next_mark);
	btn[0].appendChild(btn_text);
}

function go_back(tag)
{
	var pre_url = document.referrer;
	if(pre_url.indexOf("BRS_ap_detect_01_ap_02.html?id=0") != -1)
		this.location.href = "BRS_ap_detect_01_ap_02.html?id=0";
	else if(pre_url.indexOf("BRS_ap_detect_01_ap_02.html?id=1") != -1)
		this.location.href = "BRS_ap_detect_01_ap_02.html?id=1";
	else
		this.location.href = "BRS_ap_detect_01_ap_02.html";
	return true;
}
function checkap(form) //for bug 30286
{
	if( wps_progress_status == "2" || wps_progress_status == "3" || wps_progress_status == "start" )
	{
		alert(bh_wps_in_progress);
		return false;
	}
	form.hid_enable_apmode.value="1";
	form.hid_dyn_get_ip.value="1"; //for dynamic
	form.submit();
	//location.href="BRS_00_04_ap_wait.html";
}

addLoadEvent(initPage);
