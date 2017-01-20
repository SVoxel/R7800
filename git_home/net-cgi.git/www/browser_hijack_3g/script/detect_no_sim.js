function initPage()
{
	var head_tag = document.getElementsByTagName("h1");

	var connect_text = document.createTextNode(bh_detect_not_simcard);
        head_tag[0].appendChild(connect_text);

	var paragraph = document.getElementsByTagName("p");

	var paragraph_text = document.createTextNode(bh_detect_not_simcard_msg1);
        paragraph[0].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_detect_not_simcard_msg2);
	paragraph[1].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_detect_not_simcard_msg3);
        paragraph[2].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_detect_not_simcard_msg4);
        paragraph[3].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_detect_not_simcard_msg5);
        paragraph[4].appendChild(paragraph_text);

	//next button
	var btns_div1 = document.getElementById("btnsContainer_div1");
	btns_div1.onclick = function()
	{
		return backToUp();
	}

	var btn = btns_div1.getElementsByTagName("div");
        var btn_text = document.createTextNode(bh_back_mark);
        btn[0].appendChild(btn_text);

	//try again button
	var btns_div2 = document.getElementById("btnsContainer_div2");
	btns_div2.onclick = function()
	{
		return retry_detect_simcard();
	}

	var btn = btns_div2.getElementsByTagName("div");
	var btn_text = document.createTextNode(bh_try_again);
	btn[0].appendChild(btn_text);

	//back button
	var btns_div3 = document.getElementById("btnsContainer_div3");
	btns_div3.onclick = function()
	{
		return back();
	}

	var btn = btns_div3.getElementsByTagName("div");
        var btn_text = document.createTextNode(bh_quit_mark);
        btn[0].appendChild(btn_text);
}

function backToUp()
{
	if(top.have_lte_flag=="1")
		this.location.href="lte_detect_succ_hsdpa.htm";
	else
		this.location.href="detect_succ_hsdpa.htm";
}

function retry_detect_simcard()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	cf.action = "/apply.cgi?/config_3g_wait_page.htm timestamp=" + ts;
	if(top.have_lte_flag=="1")
		cf.submit_flag.value = "wizard_lte_detwan";
	else
		cf.submit_flag.value = "wizard_3g_detwan";
	cf.submit();
}

function back()
{
	this.location.href="WIZ_sel_3g_adsl.htm";
}


addLoadEvent(initPage);
