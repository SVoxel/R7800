function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_no_coverage);
	head_tag[0].appendChild(head_text);
	
	//paragrah
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_no_coverage_msg1);
	paragraph[0].appendChild(paragraph_text);
	
 	paragraph_text = document.createTextNode(bh_no_coverage_msg2);
	paragraph[1].appendChild(paragraph_text);
	
	//button
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function() 
	{
		return click_tryagain();
	}
								
	var btn = document.getElementById("btn_div");	
	var btn_text = document.createTextNode(bh_try_again);
	btn.appendChild(btn_text);
	
	//show Fireware Version
	showFirmVersion("");
}

function click_tryagain()
{
	location.href="BRS_start.html";
}

addLoadEvent(initPage);
