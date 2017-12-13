function initIndex()
{
	var page_body = document.getElementsByTagName("body");
	
	page_body.onload = loadValue();

	changeUrl();
}

function loadValue()
{	
	var content_frame = document.getElementById("content_frame");

	if(top.hdd_multi_user ==1)
	{
		if(first_hdd_nofind == "1")
			content_frame.setAttribute("src", "BRS_01_checkNet_ping.html");
		else{
			if( from_restore == "1" )//fix bug 29078
				content_frame.setAttribute("src", "BRS_03B_haveBackupFile_wait_ping.html");
			else if( from_nowan == "1" )
				content_frame.setAttribute("src", "BRS_03A_A_noWan_check_net.html");
			else if( from_download == "1" )
				content_frame.setAttribute("src", "BRS_hdd_download_href.htm");
			else if( from_setting == "1" )
				content_frame.setAttribute("src", "BRS_04_applySettings_ping.html");
			else if( apmode_flag == "1" && ap_mode == "1" && ap_mode_detection_flag == "1" )// if is ap_mode, goto check Net ping directely
				content_frame.setAttribute("src", "BRS_01_checkNet_ping.html");
			else if( apmode_flag == "1" && ap_mode_detection_flag == "1" )
				content_frame.setAttribute("src", "BRS_00_01_check_ap.html");//the first page (support ap)
			else
				content_frame.setAttribute("src", "BRS_01_checkNet_ping.html");// the first page (not support ap)
		}
	}
	else
	{
		if( from_restore == "1" )//fix bug 29078
			content_frame.setAttribute("src", "BRS_03B_haveBackupFile_wait_ping.html");
		else if( from_nowan == "1" )
			content_frame.setAttribute("src", "BRS_03A_A_noWan_check_net.html");
		else if( form_set2 == "1" )
			content_frame.setAttribute("src", "BRS_04_applySettings_ppp_second_obtain_ip.html");
		else if( from_setting == "1" )
		{
			if(dsl_enable_flag == "1"){
				if( apmode_flag == "1" && ap_mode == "1" )
					content_frame.setAttribute("src", "BRS_04_applySettings_ping.html");
				else
					content_frame.setAttribute("src", "BRS_04_applySettings_ppp_obtain_ip.html");
			}
			else
				content_frame.setAttribute("src", "BRS_04_applySettings_ping.html");
		}
		else if( apmode_flag == "1" && ap_mode == "1" && ap_mode_detection_flag == "1" )// if is ap_mode, goto check Net ping directely
			content_frame.setAttribute("src", "BRS_01_checkNet_ping.html");
		else if(dsl_enable_flag == "1")
				content_frame.setAttribute("src", "BRS_01_ture_check_cable.html");
		else if( apmode_flag == "1" && ap_mode_detection_flag == "1" )
			content_frame.setAttribute("src", "BRS_00_01_check_ap.html");//the first page (support ap)
		else
			content_frame.setAttribute("src", "BRS_01_checkNet_ping.html"); //the first page (not support ap
	}
		
	showFirmVersion("none");
}

function changeUrl()
{
	if(( apmode_flag == "1" && ap_mode == "1"))
	{
		if((dns_hijack == "1") && (this.location.hostname != apmode_get_ip && this.location.hostname.indexOf("routerlogin.net") == -1 && this.location.hostname.indexOf("routerlogin.com") == -1))
			this.location.hostname = apmode_get_ip;
	}
	else
	{
		if((dns_hijack == "1") && (this.location.hostname != lanip && this.location.hostname.indexOf("routerlogin.net") == -1 && this.location.hostname.indexOf("routerlogin.com") == -1))
			this.location.hostname = "www.routerlogin.net";
	}
}

addLoadEvent(initIndex);
