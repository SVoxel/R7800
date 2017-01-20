function setGray(cf)
{
	if( cf.enable_autorefresh_status.checked )
		cf.refresh_time.disabled = false;
	else
		cf.refresh_time.disabled = true;
}

function show_Graypage()
{
	var cf=document.forms[0];
	if(disk_num == 0)
	{
		document.getElementById("save_path").style.color="gray";
		document.getElementById("path").style.color="gray";
		cf.browser.disabled=true;
		document.getElementById("max_id1").style.color="gray";
		document.getElementById("max_id2").style.color="gray";
		document.getElementById("max_id3").style.color="gray";
		document.getElementById("max_id4").style.color="gray";
		document.getElementById("max_id5").style.color="gray";
		document.getElementById("max_id6").style.color="gray";
		document.getElementById("max_id7").style.color="gray";
		document.getElementById("max_id8").style.color="gray";
		document.getElementById("max_id9").style.color="gray";
		cf.max_tasks.disabled = true;
		cf.Cancel.disabled = true;
		cf.Apply.disabled = true;
		cf.edit.disabled = true;
		cf.Cancel.className ="common_gray_bt";
		cf.Apply.className ="common_gray_bt";
		cf.edit.className ="long_edit_gray_bt";
		cf.green_download_max_downrate.disabled = true;
		cf.green_download_max_uprate.disabled = true;
		cf.enable_autorefresh_status.disabled = true;
		cf.edit.disabled = true;
		cf.edit.className = "long_edit_gray_bt";

		if(enable_autorefresh_status == "1")
			cf.refresh_time.disabled = true;
	}
	else
	{
		cf.save_folder.value = green_disk_lable.replace(/\//g,"\\");
		document.getElementById("save_path").style.color="black";
		document.getElementById("path").style.color="black";
		cf.browser.disabled=false;
		document.getElementById("max_id1").style.color="black";
		document.getElementById("max_id2").style.color="black";
		document.getElementById("max_id3").style.color="black";
		document.getElementById("max_id4").style.color="black";
		document.getElementById("max_id5").style.color="black";
		document.getElementById("max_id6").style.color="black";
		document.getElementById("max_id7").style.color="black";
		document.getElementById("max_id8").style.color="black";	
		document.getElementById("max_id9").style.color="black";
		cf.max_tasks.disabled = false;
		cf.Cancel.disabled = false;
		cf.Apply.disabled = false;
		cf.Cancel.className ="cancel_bt";
		cf.Apply.className ="apply_bt";
		cf.green_download_max_downrate.disabled = false;
		cf.green_download_max_uprate.disabled = false;
		cf.enable_autorefresh_status.disabled = false;
		cf.edit.disabled = false;
		cf.edit.className = "long_edit_bt";
	}
}

function chg_max_task()
{
	var cf=document.forms[0];
	var max_tasks = cf.max_tasks.value;
	var i;
	var choose=document.getElementById("max_tasks");

	for(i=0; i<top.green_downloading_item_num; i++)
	{
		choose.options[i].disabled=true;
	}
	for(i=top.green_downloading_item_num; i<8; i++)
	{
		choose.options[i].disabled=false;
	}
}
