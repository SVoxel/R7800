function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_enter_pukcode);
	head_tag[0].appendChild(head_text);
	
	
	var paragraph = document.getElementsByTagName("p");
	var paragraph_text = document.createTextNode(bh_enter_puk_msg);
	paragraph[1].appendChild(paragraph_text);
	
	
	//set event action
	
	var btns_container_div = document.getElementById("btnsContainer_div");
	btns_container_div.onclick = function()
	{
		return checkpuk();
	}
	
	var btn = document.getElementById("btn_text_div");
	var btn_text = document.createTextNode(bh_next_mark);
	btn.appendChild(btn_text);

	//show firmware version
        showFirmVersion("none");
}

function checkpuk()
{
        var cf = document.forms[0];
	if(cf.puk_code.value.length !=8)
	{
		alert("PUK Code should be 8 digits");
		return false;
	}
	if(cf.new_pin1.value.length <4)
	{
		alert("PIN Code should be 4-8 digits!");
		return false;
	}
	if(cf.new_pin1.value != cf.new_pin2.value)
	{
		alert("Please confirm new PIN Code again!");
		return false;
	}
	cf.submit();

	return true;
}

addLoadEvent(initPage);
