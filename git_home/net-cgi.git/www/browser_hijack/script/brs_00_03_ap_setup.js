
function initPage()
{
	var head_tag  = document.getElementsByTagName("h1");

	var btns_container_div = document.getElementById("btnsContainer_div2");
	if( master=="admin" ){
		btns_container_div.onclick = function() 
		{
			return checkap(document.forms[0]);
		}
	}
	var btns_container_div = document.getElementById("btnsContainer_div1");
	btns_container_div.onclick = function() 
	{
		return go_back();
	}
	loadvalue()
	//show firmware version
    showFirmVersion("none");
}

addLoadEvent(initPage);
function loadvalue()
{
}

function goto_route()
{
	top.location.href = "http://www.routerlogin.net";
}

function go_back()
{
	this.location.href = "BRS_00_02_ap_select.html";
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

