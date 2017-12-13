function add_item()
{

}

function reset_form()
{
	if ( this_download_type == "http" )
	{
		document.forms[0].download_url.value = "";
		document.forms[0].user_name.value = "";
		document.forms[0].password.value = "";
	}
	else if ( this_download_type == "bt" )
	{
		document.forms[0].folder.value="";
		document.forms[0].magnet_url_usb.value="";
		document.forms[0].magnet_url.value="";
		document.forms[0].mtenFWUpload.value="";
		document.forms[1].mtenFWUpload.value="";
		document.forms[1].magnet_url_pc.value="";
	}
	else if ( this_download_type == "emule" )
	{
		document.forms[0].download_url.value="";
	}
}

function set_action(cf)
{
	if(this_download_type == "http")
		cf.action="/apply.cgi?/GREEN_http_basic.htm timestamp=" + ts;
	else if(this_download_type == "bt")
		cf.action="/apply.cgi?/GREEN_bt_basic.htm timestamp=" + ts;
	else if(this_download_type == "emule")
		cf.action="/apply.cgi?/GREEN_emule_basic.htm timestamp=" + ts;
	else
		cf.action="/apply.cgi?/GREEN_basic.htm timestamp=" + ts;
}

function change_path(path)
{
	var i,save_file,vol_path,tmp_str,new_str;
	var pos = path.indexOf("/",5);

	if( pos == -1)
	{
		save_file = "";
		vol_path = path;
	}
	else
	{
		save_file = path.substr(pos);
		vol_path = path.substr(0,pos);
	}

	for(i=0; i<sda_num; i++)
	{
		var sda_str = eval('sda_U' + i);
		var e_info = sda_str.split('*');
		if( vol_path == e_info[0] )
		{
			vol_path = e_info[1];
			break;
		}
	}

	if(pos == -1)
		tmp_str=""+vol_path+":/"+save_file+"";
	else
		tmp_str=""+vol_path+":"+save_file+"";

	new_str = tmp_str.replace(/\//g,"\\");

	return new_str;
}

function get_ids(cf,method)
{
	var i = 0;
	var pause_count = 0;
	var resume_count = 0;
	var normal_count = 0;
	var objs = document.getElementsByName("chbx_item");
	var check_result = 0;

	for( i=0; i< objs.length; i++ )
	{
		if( objs[i].checked == true )
		{
			var each_info = objs[i].value.split('*');
			var id = each_info[0];
			var stat = each_info[1];
			if(method == "pause")
			{
				if( stat != "$greendl_common_showStatusConnecting"  && stat != "$greendl_common_showStatusDownloading" && stat != "$greendl_common_showStatusUSBfull")
				{
					pause_count++;
					continue;
				}
			}
			else if(method == "resume")
			{
				if( stat != "$greendl_common_showStatusPaused" && stat != "FAILURE")
				{
					resume_count++;
					continue;
				}
			}
			if( cf.select_ids.value == "" )
				cf.select_ids.value = id;
			else
				cf.select_ids.value = cf.select_ids.value + " " + id;

			normal_count++;
		}
	}		

	if(normal_count > 0)
		check_result = 1;
	else if(pause_count > 0 || resume_count > 0)
		check_result = 2;
	else if( cf.select_ids.value == "" && pause_count == 0 && resume_count == 0)
	{	
		alert("$greendl_common_alert_noSelect\n");
		check_result = 3;
	}

	return check_result;
}

function pause_download(cf)
{
	var check_result;

	check_result = get_ids(cf,"pause");

		if( cf.select_ids_queuen.value != "" )
		{
			alert("$greendl_common_alert_queuen_wrong_action_1\n");
			return false;
		}

		if(check_result == 1)
		{
			set_action(cf);
			cf.submit_flag.value = "pause_download";
			cf.submit();
		}
		else if(check_result == 2)
			refresh_download(cf);
		else if(check_result == 3)
			return false;
}

function start_download(cf)
{
	var check_result;

	check_result = get_ids(cf,"start");

		if( cf.select_ids.value == "" && top.green_download_item_num < fileTP_max_concurrent_tasks )
		{
			var index_blank = cf.select_ids_queuen.value.indexOf(" ");

			if( index_blank >= 0 )
			{
				alert("$greendl_common_alert_queuen_wrong_action_3\n");
				return;
			}
		}
		else if( cf.select_ids_queuen.value != "" )
		{
			alert("$greendl_common_alert_queuen_wrong_action_2\n");
			return false;
		}

	if(check_result == 1)
	{
		set_action(cf);
		cf.submit_flag.value = "start_download";
		cf.submit();
	}
	else if(check_result == 3)
		return false;
}

function refresh_download(cf)
{
	set_action(cf);
	cf.submit_flag.value = "refresh_download";
	cf.submit();
}

function resume_download(cf)
{
	var check_result;

	check_result = get_ids(cf,"resume");

		if( cf.select_ids.value == "" && top.green_download_item_num < fileTP_max_concurrent_tasks )
		{
			var index_blank = cf.select_ids_queuen.value.indexOf(" ");
			if( index_blank >= 0 )
			{
				alert("$greendl_common_alert_queuen_wrong_action_3\n");
				return;
			}
		}
		else if( cf.select_ids_queuen.value != "" )
		{
			alert("$greendl_common_alert_queuen_wrong_action_2\n");
			return false;
		}

		if(check_result == 1)
		{
			set_action(cf);
			cf.submit_flag.value = "resume_download";
			cf.submit();
		}
		else if(check_result == 2)
			refresh_download(cf);
		else if(check_result == 3)
			return false;
}

function remove_item(cf)
{
	var check_result;

	check_result = get_ids(cf,"remove");

	if(check_result == 1)
        {
		set_action(cf);
		cf.submit_flag.value = "remove_download";
		cf.submit();
	}
	else if(check_result == 3)
		return false;
}

function remove_history(cf)
{
	set_action(cf);
	cf.submit_flag.value = "remove_history";
	cf.submit();
}

function change_priority(id, cf, sel, pre)
{
	set_action(cf);
	cf.select_ids.value = id;
	cf.priority_val.value = sel.value;
	cf.submit_flag.value = "change_priority";
	cf.submit();
}

function open_browser(form)
{
	form.submit_flag.value="file_browser_open";
	form.submit();
}
function close_browser(form)
{
    form.submit_flag.value="browser_close";
    form.submit();
}

function redirect_config_download()
{
	this.location.href="green_download.htm";
	top.menu_color_change("green_basic");
}

function enable_greendownload(form,type)
{
	if(form.enable_green_download.checked == true)
		form.green_download_open_flag.value = "1";
	else
		form.green_download_open_flag.value = "0";

	form.submit_flag.value="enable_green_download";

	set_action(form);
	form.submit();
}

var greenErrorHandling = function(cf) {
	var is_fail = 0;
	var type = this_download_type;

	if(mlnet_error == "1" && confirm("$greendl_seed_alreay_exist") == true)
		is_fail = 1;
	else if(mlnet_error == "2" && confirm("$greendl_file_alreay_exist") == true)
		is_fail = 1;
	else if(mlnet_error == "3" && confirm("$greendl_insufficient_space") == true)
		is_fail = 1;
	else if(mlnet_error == "4")
		alert("$greendl_fail_connection");
	else if(mlnet_error == "5")
		alert("$greendl_ftp_authenticate_fail");
	else if(mlnet_error == "6")
		alert("$greendl_not_correct_magnet_link");
	else if(mlnet_error == "7")
		alert("$greendl_partition_not_read");

	if(is_fail == 1) {
		cf.submit_flag.value="green_download_fail";
		set_action(cf);
		cf.submit();
	}
}

function check_magnet(magnet_url)
{
	var tmp_url,pos;
	var str = "mod=attachment"

	if(magnet_url.substr(0,5).toUpperCase() == "HTTPS")
	{
		alert("$greendl_not_support_https");
		return false;
	}
	else if(magnet_url.substr(0,8).toUpperCase() == "MAGNET:?")
	{
		return true;
	}
	else if(magnet_url.substr(0,4).toUpperCase() == "HTTP")
	{
		tmp_url = magnet_url.substr(magnet_url.lastIndexOf(".")+1);
		pos = magnet_url.indexOf(str);

		if(tmp_url.toUpperCase() != "TORRENT" && pos == -1)
		{
			alert("$greendl_invalid_url");
			return false;
		}
		else if(pos == (magnet_url.length-str.length))
		{
			alert("$greendl_invalid_url");
			return false;
		}
		else
			return true;
	}
	else
	{
		alert("$greendl_invalid_url");
		return false;
	}
}

function select_item(value1, value2)
{
	if ( value1 == value2 )
		return "selected";
	else
		return "";
}

function refresh_download_table()
{
	var speed_str,status_str, item_id, sub_item;
	var dl_tb_html;
	var wform = "document.forms[0]";
	if ( this_download_type == "bt" )
		wform = "document.forms[2]";
	dl_tb_html = "<TABLE border=1 cellpadding=2 cellspacing=0 width=100%>"+
		"<TR>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showSelect<input type=checkbox name=\"chbx_all_item\" value=\"0\" onclick=\"select_all_items()\"></span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showNo</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showFileName</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showFileSize</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$qos_priority</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showSpeed</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showDownload</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$stalist_Status</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$block_ser_setup_pro</span></TD>" +
		"</TR>";

	for( i=0; i< top.green_download_item_num; i++)
	{
		var info = item[i].toString();
		var each_info = info.split('*');

		if( each_info[6] == "Paused" )
		{
			speed_str = "-";
			status_str = "$greendl_common_showStatusPaused";
		}
		else if( each_info[6] == "-" )
		{
			speed_str = each_info[4];
			status_str = "$greendl_common_showStatusConnecting";
		}
		else if( each_info[6] == "Queued" )
		{
			speed_str = "-";
			status_str = "$greendl_common_showStatusUSBfull";
		}
		else if( each_info[6] == "Failure" )
		{
			speed_str = "-";
			status_str = "$wps_add_failure";
		}
		else if( each_info[6] == "Disable")
		{
			speed_str = "-";
			status_str = "$pppoe2_disable";
		}
		else
		{
			speed_str = each_info[4];
			status_str = "$greendl_common_showStatusDownloading";
		}

		if(each_info[7] == "EMULE")
			each_info[1] = Convert(each_info[1]);

		item_id = each_info[0];
		sub_item = "<TR>" +
			"<TD nowrap align=center><input type=checkbox name=\"chbx_item\" value=\""+item_id+"*"+status_str+"\"></TD>" +
			"<TD nowrap align=center>" + (i+1) + "</TD>" +
			"<TD nowrap>" + each_info[1] + "</TD>" +
			"<TD nowrap align=center>"+each_info[2]+"</TD>" +
			"<TD nowrap align=center>" +
			"<select name=\"priority_sel\" onchange=\"change_priority('"+item_id+"', "+wform+", this, '"+each_info[3]+"');\">" +
			"<option value=\"6\"" +  select_item("6", each_info[3]) + ">$qos_high</option>" +
			"<option value=\"4\"" +  select_item("4", each_info[3]) + ">$qos_normal</option>" +
			"<option value=\"2\"" +  select_item("2", each_info[3]) + ">$qos_low</option>" +
			"</select>" +
			"<TD nowrap align=center>"+speed_str+"</TD>" +
			"<TD nowrap align=center>"+each_info[5]+"%</TD>" +
			"<TD nowrap align=center>"+status_str+"</TD>" +
			"<TD nowrap align=center>"+each_info[7]+"</TD>" +
			"</TR>";
		dl_tb_html += sub_item;
	}
	dl_tb_html += "</TABLE>";
	document.getElementById("download_tb").innerHTML = dl_tb_html;
}

function refresh_downloaded_table()
{
	var dled_tb_html;
	var sUserAgent = navigator.userAgent;
	var isWin = (navigator.platform == "Win32") || (navigator.platform == "Windows");
	var isVista = (navigator.platform == "");
	var isWin7 = false;
	var isXP = false;
	if(isWin &&( sUserAgent.indexOf("Windows NT 6.1") > -1 || sUserAgent.indexOf("Windows 7") > -1))
		isWin7 = true;
	else if(isWin)
		isXP = true;

	var i, sub_item;

	dled_tb_html = "<TABLE border=1 cellpadding=2 cellspacing=0 width=100%>" +
		"<TR>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showNo</span> </TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showFileName</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showFileSize</span></TD>" +
		"<TD nowrap align=center><span class=\"subhead\">$greendl_common_showSavedPath</span></TD>" +
		"</TR>";


	for( i=file_num-1; i>= 0; i--)
	{
		var no_i=file_num-i;
		var info = filed[i];
		var each_info = info.split('*');
		var display_path = each_info[2].replace(/\//g,"\\");
		sub_item = "<TR>" +
			"<TD nowrap align=center>" + no_i + "</TD>" +
			"<TD nowrap>"+each_info[0]+"</TD>" +
			"<TD nowrap align=center>"+each_info[1]+"</TD>" +
			"<TD nowrap>"+display_path+"</TD>";
		dled_tb_html += sub_item;
	}

	dled_tb_html += "</TABLE>";
	document.getElementById("downloaded_tb").innerHTML = dled_tb_html;
}

function refresh_content()
{
	var xmlhttp, xmlDoc;
	var dl_num, dling_num, tasks, files, i;

	if ( window.XMLHttpRequest)
	{
		xmlhttp = new XMLHttpRequest();
	}
	else
	{
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}

	xmlhttp.onreadystatechange = function()
	{
		if ( xmlhttp.readyState == 4 && xmlhttp.status == 200 )
		{
			xmlDoc = xmlhttp.responseXML;
			dl_num = xmlDoc.getElementsByTagName("Download_num");
			dling_num = xmlDoc.getElementsByTagName("Downloading_num");
			tasks = xmlDoc.getElementsByTagName("Task");
			files = xmlDoc.getElementsByTagName("File");


			if ( dl_num.length > 0 )
				top.green_download_item_num = parseInt(dl_num[0].childNodes[0].nodeValue, 10);

			if ( dling_num.length > 0 )
				top.green_downloading_item_num = parseInt(dling_num[0].childNodes[0].nodeValue, 10);

			for ( i = 0; i < tasks.length; i++ )
			{
				item[i] = tasks[i].childNodes[0].nodeValue;
			}

			for ( i = 0; i < files.length; i++ )
			{
				filed[i] = files[i].childNodes[0].nodeValue;
			}

			file_num = i;
			refresh_download_table();
			refresh_downloaded_table();
		}
	};
	xmlhttp.open("GET", "download_info.xml", true);
	xmlhttp.send();

	if( green_download_enable == "1" && autorefresh_onoff == 1 && top.green_download_item_num > 0 )
		setTimeout("refresh_content()", delay_time);
}

function change_downlaod_type(obj)
{
	var type = obj.value;
	if( type != this_download_type )
	{
		if(type == "bt")
			this.location.href="GREEN_bt_basic.htm";
		else if( type == "emule")
			this.location.href="GREEN_emule_basic.htm";
		else if( type == "http")
			this.location.href="GREEN_http_basic.htm";
	}
}

function select_all_items()
{
	var cf = document.forms[0];
	var i=0;
	var objs = document.getElementsByName("chbx_item");
	for( i=0; i< objs.length; i++ )
	{
		if(cf.chbx_all_item.checked)
			objs[i].checked=true;
		else
			objs[i].checked=false;
	}
}

function Convert(originStr)
{
	originStr = originStr.replace(/\.\[VeryCD\.com\]/g, "");
	originStr = originStr.replace(/\[/g, "%5B");
	originStr = originStr.replace(/\]/g, "%5D");
	return unescape(decodeURIComponent(originStr));
}

function check_url_for_lan(url, lanip, lanmask)
{
	var ip_start = url.indexOf("://") > -1? url.indexOf("://") :0;
	var tmp_url = url.slice(ip_start+3);
	var ip_end = tmp_url.indexOf(":") > -1? tmp_url.indexOf(":"): tmp_url.indexOf("/") > -1? tmp_url.indexOf("/"): tmp_url.length;
	var ip = tmp_url.slice(0, ip_end);
	if(checkipaddr(ip)) {
		if(isSameSubNet(ip, lanmask, lanip, lanmask)) {
			return true;
		}
	}

	return false;
}
