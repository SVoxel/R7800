function initPage()
{
        var head_tag = document.getElementsByTagName("h1");

        var head_text = document.createTextNode(bh_detecting_connection);
        head_tag[0].appendChild(head_text);

        var paragraph = document.getElementsByTagName("p");

        var paragraph_text = document.createTextNode(bh_plz_wait_process);
        paragraph[0].appendChild(paragraph_text);

        setTimeout("loadValue()", 1000);
}

function loadValue()
{
	this.location.href="BRS_04_applySettings.html";
}

addLoadEvent(initPage);
