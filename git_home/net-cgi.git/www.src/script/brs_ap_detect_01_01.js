function initPage()
{
	//button back
	var btns1 = document.getElementById("back");
	btns1.value = bh_back_mark;
	if( master == "admin" )
	btns1.onclick = function()
	{
		return goto_url(0);
	}
	else
		btns1.className="grey_short_btn";
	
	//button next
	var btns2 = document.getElementById("next");
	btns2.value = bh_next_mark;
	if( master == "admin" )
	btns2.onclick = function()
	{
		return goto_url(1);
	}
	else
		btns2.className="grey_short_btn";
}

function goto_url(tag)
{
	if(tag == 0)
	{
		this.location.href = "BRS_00_02_ap_select.html";
		return true;
	}
	else if(tag == 1)
	{
		this.location.href = "BRS_ap_detect_01_02.html";
		return true;
	}
}
addLoadEvent(initPage);
