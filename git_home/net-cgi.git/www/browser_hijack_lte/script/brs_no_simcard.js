function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	var connect_text="";
	var paragraph_text="";
	if(lte_card_status !='1')
		connect_text = document.createTextNode(bh_unable_detect_msg);
	else
		connect_text = document.createTextNode(bh_no_simcard);
        head_tag[0].appendChild(connect_text);

	var paragraph = document.getElementsByTagName("p");
	
	if(lte_card_status !='1')
		paragraph_text = document.createTextNode(bh_unable_detect_msg1);
	else
		paragraph_text = document.createTextNode(bh_not_simcard_msg1);
        paragraph[0].appendChild(paragraph_text);

	if(lte_card_status !='1')
		paragraph_text = document.createTextNode(bh_not_device_msg1);
	else
		paragraph_text = document.createTextNode(bh_detect_not_simcard_msg3);
	paragraph[1].appendChild(paragraph_text);

	//try again button
	var btns_div = document.getElementById("btnsContainer_div");
	btns_div.onclick = function()
	{
		return retry_detect_simcard();
	}

	var btn = btns_div.getElementsByTagName("div");
	var btn_text = document.createTextNode(bh_try_again);
	btn[0].appendChild(btn_text);
}

function retry_detect_simcard()
{
	location.href="BRS_start.html";
}

addLoadEvent(initPage);
