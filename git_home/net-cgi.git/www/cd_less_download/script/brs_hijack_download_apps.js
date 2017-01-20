function initPage()
{
	if ( cd_less_download == 0)
		return;
	download_genie = 1;
	download_vault = 1;
	
	var table = document.getElementById("choices_table");
	if(navigator.userAgent.indexOf("MSIE") != -1)
		table.style.width = "100%" ;

	var for_win_div = document.getElementById("for_win");
	if ( top.have_vault && !isMac() )
		for_win_div.style.display = "";
	else
		download_vault = 0;

	var btns_div1 = document.getElementById("btnsContainer_div1");
	btns_div1.onclick = function()
	{
		return toInternet();
	}

	var btns_div2 = document.getElementById("btnsContainer_div2");
	btns_div2.onclick = function()
	{
		location.href = "BRS_success.html";
		return true;
	}

	var btns_div3 = document.getElementById("btnsContainer_div3");
	btns_div3.onclick = function()
	{
		return downloadAppChecking();
	}

	var btns_div4 = document.getElementById("btnsContainer_div4");
	btns_div4.onclick = function()
	{
		return toFwChecking();
	}

	//show firmware version
	showFirmVersion("none");
}

function toFwChecking()
{
	document.forms[0].submit();
	return true;
}

function app_choose(id)
{
	var cf = document.forms[0];
	var obj = document.getElementById(id);
	var flag = 1;

	if ( obj.className == "checked" )
	{
		obj.className = "uncheck";
		flag = 0;
	}
	else
	{
		obj.className = "checked";
	}

	if ( id == "genie_choose" )
		download_genie = flag;
	else
		download_vault = flag;

	if ( !top.have_vault )
		top.download_vault = 0;
}


function downloadAppChecking()
{
	if ( !download_genie && !download_vault )
	{
		return toFwChecking();
	}
	else
	{
		document.getElementById("choices_div").style.display = "none";
		document.getElementById("allBtns_div").style.display = "none";
		document.getElementById("processing").style.display = "";
		setTimeout(function(){
				var obj = document.getElementById("downloading_div");
				var txts = obj.getElementsByTagName("p");
				var txt1;
				var ifr1 = document.createElement("IFRAME");
				var ifr2 = document.createElement("IFRAME");

				ifr2.frameBorder = ifr1.frameBorder =  0;
				ifr2.scrolling = ifr1.scrolling ="no";
				ifr2.width = ifr1.width = 0;
				ifr2.height = ifr1.height = 0;
				if ( isMac() )
				ifr1.src = "http://updates1.netgear.com/netgeargenie/mac/update/NETGEARGenieInstaller.dmg";
				else
				{
					ifr1.src="http://updates1.netgear.com/netgeargenie/update/NETGEARGenie-install.exe";
					ifr2.src="http://updates1.netgear.com/readysharevault/ReadySHAREVault-install.exe";
				}
				if ( download_genie && !download_vault )
				{
					txt1 = document.createTextNode(bh_dl_genie1);
					txts[0].appendChild(txt1);
					txts[1].innerHTML = bh_dl_genie2;
				}
				else if ( !download_genie && download_vault )
				{
					txt1 = document.createTextNode(bh_dl_vault1);
					txts[0].appendChild(txt1);
					txts[1].innerHTML = bh_dl_vault2;
				}
				else
				{
					txt1 = document.createTextNode(bh_dl_all1);
					txts[0].appendChild(txt1);
					txts[1].innerHTML = bh_dl_all2;
				}
				document.getElementById("processing").style.display = "none";
				obj.style.display = "";
				document.getElementById("finishBtn_div").style.display = "";
				if ( download_vault )
					document.body.appendChild(ifr2);
				if ( download_genie )
					document.body.appendChild(ifr1);

		}, 1000);
	}
	return true;
}

function toInternet()
{
	this.parent.location.href="to_internet_no_auth.htm";
	return true;
}

addLoadEvent(initPage);
