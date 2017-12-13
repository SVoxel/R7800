function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_plz_wait_moment);
	head_tag[0].appendChild(head_text);

	var image = document.getElementById("waiting_img");
        image.setAttribute("src", "image/wait30.gif");

	setTimeout("formSubmit()", 50000);

	//hide the firmware version
	showFirmVersion("none");
}

function formSubmit()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	if(top.dsl_enable_flag == "1")
		this.location.href = "BRS_dsl_type_detc.html";
	else
		cf.submit();
}

addLoadEvent(initPage);
