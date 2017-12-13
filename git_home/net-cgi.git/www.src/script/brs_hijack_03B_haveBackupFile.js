function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_settings_restoration);
	head_tag[0].appendChild(head_text);
	
	//paragrah
	var paragraph = document.getElementsByTagName("p");
	
	document.getElementById("browse").value = bh_browse_mark;
	
	//buttons left
	var btns_div1 = document.getElementById("back");
	btns_div1.value = bh_back_mark;

	if( master == "admin" )
	btns_div1.onclick = function()
	{
		return goBack();
	}
	else
		btns_div1.className = "grey_short_btn";
	
	
	//buttons right
	var btns_div2 = document.getElementById("next");
	btns_div2.value = bh_next_mark;
	if( master == "admin" )
	btns_div2.onclick = function()
	{
		return retoreSettings();
	}
	else
		btns_div2.className = "grey_short_btn";
}

function goBack()
{
	if(top.dsl_enable_flag == "0")
		this.location.href = "BRS_02_genieHelp.html";
	else	
	{
		if(top.location.href.indexOf("BRS_index.htm") > -1)
			this.location.href = "BRS_ISP_country_help.html";
		else
			this.location.href = "DSL_WIZ_sel.htm";
	}	
	return true;
}

function retoreSettings()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];

	var file_upgrade = document.getElementById("file_upgrade");
	var filestr = file_upgrade.value;

	if(filestr.length == 0)
	{
		alert(bh_filename_null);
		return false;
	}
	var file_format = filestr.substr(filestr.lastIndexOf(".") + 1);
	if(file_format != "cfg")
	{
		alert(bh_not_correct_file+"cfg");
		return false;
	}

	if(confirm(bh_ask_for_restore))
	{
		cf.action="/restore.cgi?/BRS_03B_haveBackupFile_fileRestore.html timestamp="+ts;
		cf.submit();
	}
	else
		return false;
}

function IE_version()
{
	var Sys = {};
	var ua = navigator.userAgent.toLowerCase();
	var s;
	(s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :0;
	if(Sys.ie == '6.0')
		return 6;
	else if(Sys.ie == '7.0')
		return 7;
	else if(Sys.ie == '8.0')
		return 8;
	else if(Sys.ie == '9.0')
		return 9;
	else if(Sys.ie == '10.0')
		return 10;
	else
		return 11;
}

addLoadEvent(initPage);
