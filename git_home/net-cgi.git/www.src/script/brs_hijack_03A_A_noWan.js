
function initPage()
{
	var head_tag = document.getElementsByTagName("h1");
	
	var head_text = document.createTextNode(bh_no_cable);
	var wait_text = document.createTextNode(bh_detecting_connection);
	head_tag[0].appendChild(head_text);
	head_tag[1].appendChild(wait_text);
	
	
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_wizard_setup_nowan_check);
	paragraph[0].appendChild(paragraph_text);

	//paragraph_text = document.createTextNode(bh_click_try_again);
	//paragraph[1].appendChild(paragraph_text);

	paragraph_text = document.createTextNode(bh_plz_wait_process);
	paragraph[2].appendChild(paragraph_text);

	var btns_container_div = document.getElementById("btnsContainer_div");
	if( master == "admin" )
	{
		btns_container_div.onclick = function()
		{
			return clickRetry();
		}
	}
	
	var btn = document.getElementById("btn_div");
	var btn_text = document.createTextNode(bh_try_again);
	btn.appendChild(btn_text);

	//show firmware version
        showFirmVersion("");
}

function clickRetry()
{
	document.getElementById("no_wan").style.display="none";
	document.getElementById("please_wait").style.display="";
	showFirmVersion("none");

	/* to fix bug22377: change the refresh time from 15 seconds to 30 seconds. wan lan conflict need time to change ip address*/
        if( hijack_process == "1" )
        {
                if( location.href.indexOf("www.routerlogin.net") == -1 )
                        setTimeout("top.location.href='http://www.routerlogin.net/BRS_retry.htm';", 30000);
                else
                        setTimeout("top.location.href='http://www.routerlogin.com/BRS_retry.htm';", 30000);
        }
        else
        {
                if( location.href.indexOf("www.routerlogin.net") == -1 )
                        setTimeout("top.location.href='http://www.routerlogin.net/adv_index.htm';", 30000);
                else
                        setTimeout("top.location.href='http://www.routerlogin.com/adv_index.htm';", 30000);
        }

	return true;
}


addLoadEvent(initPage);
