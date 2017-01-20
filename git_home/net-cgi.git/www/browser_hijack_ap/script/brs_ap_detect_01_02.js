function initPage()
{

	//button back
	var btns1 = document.getElementById("back");
	btns1.value = bh_back_mark;
	if( master == "admin" )
	btns1.onclick = function()
	{
		return goback();
	}
	else
		btns1.className="grey_short_btn";
	
	//button next
	var btns2 = document.getElementById("next");
	btns2.value = bh_next_mark;
	if( master == "admin" )
	btns2.onclick = function()
	{
		return help_choose_mode();
	}
	else
		btns2.className="grey_short_btn";
}

function help_choose_mode()
{
	//choose yes or no
	var choice_div = document.getElementById("choices_div");
	var choice_radio = choice_div.getElementsByTagName("input")
	
	if(choice_radio[0].checked)
		this.location.href = "BRS_ap_detect_01_ap_01.html";
	else if(choice_radio[1].checked)
		this.location.href = "BRS_ap_detect_01_03.html";
	else
	{
		alert(bh_select_yes_or_no);
		return false;
	}
		
}
function goback()
{
	this.location.href = "BRS_ap_detect_01_01.html";
}
addLoadEvent(initPage);
