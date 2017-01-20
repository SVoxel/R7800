function clickUpgrade(form)
{
	if(form.mtenFWUpload.value=="")
	{
		alert("$in_upgrade");
		return false;
	}
	var filestr=form.mtenFWUpload.value;
	var file_format=filestr.substr(filestr.lastIndexOf(".")+1); 
	if(file_format.toUpperCase()!="IMG")
	{
		alert("$not_correct_file"+"img");
		return false;
	}

	var page_title_div = document.getElementById("page_title_div");
	var FW_checking_div = document.getElementById("FW_checking_div"); 
	var FW_upg_div = document.getElementById("FW_upg_div");
	var pls_wait_div = document.getElementById("pls_wait_div");

	page_title_div.style.display = "none";
	FW_checking_div.style.display = "none";
	FW_upg_div.style.display = "none";
	pls_wait_div.style.display = "block";

	form.submit();
	return true;
}

function check_if_show()
{
	var cf = document.forms[0];
	if (cf.enable.checked == true)
		cf.auto_check_for_upgrade.value=1;
	else
	{
		if(confirm("$upgrade_turnoff_auto"))
			cf.auto_check_for_upgrade.value=0;
		else
			return false;
	}
	cf.submit();
}

function check_if_show_auto()
{
	var cf = document.forms[0];
	if (cf.enable.checked == true)
		cf.auto_check_for_upgrade.value=1;
	else
	{
		if(confirm("$upgrade_turnoff_auto"))
			cf.auto_check_for_upgrade.value=0;
		else
		{
			cf.enable.checked = true;
			return false;
		}
	}
	cf.submit();
}

function click_check()
{
	var cf=document.forms[0];
	cf.submit_flag.value="download_confile";
	cf.action="/func.cgi?/AUTO_search.htm timestamp="+ts;
	cf.submit();	
}

function clickUpgradeLanguage(form)
{
        if(form.filename.value=="")
        {
                alert("$in_upgrade");
                return false;
        }
    var filestr=form.filename.value;
    var file_format=filestr.substr(filestr.lastIndexOf("-")+1);
        if(file_format.toUpperCase()!="TABLE")
        {
                alert("$not_correct_file"+netgear_module+"-*-Language-table");
        	return false;
        }
        var win_file_name=filestr.substr(filestr.lastIndexOf("\\")+1);
        var unix_file_name=filestr.substr(filestr.lastIndexOf("/")+1);
        if(win_file_name == filestr && unix_file_name != filestr)
                file_name=unix_file_name;
        else if( win_file_name != filestr && unix_file_name == filestr)
                file_name=win_file_name;
        else if (win_file_name == filestr && unix_file_name == filestr)
                file_name=unix_file_name;
        else
        {
		alert("$not_correct_file"+netgear_module+"-*-Language-table");
                return false;
        }
        var file_array=file_name.split('-');
        if(file_array.length!=4)
        {
		alert("$not_correct_file"+netgear_module+"-*-Language-table");
                return false;
        }
        var file_module=file_array[0];
        if(file_module.toUpperCase()!=netgear_module.toUpperCase())
        {
		alert("$not_correct_file"+netgear_module+"-*-Language-table");
                return false;
        }
        var file_end=file_array[2];
        if(file_end.toUpperCase()!="LANGUAGE")
        {
		alert("$not_correct_file"+netgear_module+"-*-Language-table");
                return false;
        }
        return true;
}


