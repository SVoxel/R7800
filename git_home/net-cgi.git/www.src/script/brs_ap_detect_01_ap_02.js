function goto_next(cf, wl_login)
{
	var pre_url_2 = document.referrer;
	var pre_url_info_2 = pre_url_2.split("/");
	
	if(cf.use_ap[0].checked)
	{
		var ssid_bgn = document.getElementById("ESSID").value;
		var ssid_an = document.getElementById("ESSID_an").value;

		if(ssid_bgn == "" || ssid_an == "")
		{
			alert(ssid_null);
			return false;
		}

		for(i=0;i<ssid_bgn.length;i++)
		{
			if(isValidChar_space(ssid_bgn.charCodeAt(i))==false)
			{
				alert(ssid_not_allowed);
				return false;
			}
		}

		for(i=0;i<ssid_an.length;i++)
		{
			if(isValidChar_space(ssid_an.charCodeAt(i))==false)
			{
				alert(ssid_not_allowed);
				return false;
			}
		}

		if(checkpsk(cf.passphrase_an, cf.wla_sec_wpaphrase_len)== false)
			return;
		if( checkpsk(cf.passphrase, cf.wl_sec_wpaphrase_len)== false)
			return;
		cf.wl_hidden_wpa_psk.value = cf.passphrase.value;
		cf.wla_hidden_wpa_psk.value = cf.passphrase_an.value;
		cf.method="post";
		if(wl_login == 1)
		{
			if(pre_url_info_2[3] == "BRS_ap_detect_01_ap_01.html"){
				cf.ap_pre_link.value = "0";
				cf.action="/apply.cgi?/BRS_ap_detect_01_04.html timestamp="+ts;
			}
			else{
				cf.ap_pre_link.value = "1";
				cf.action="/apply.cgi?/BRS_ap_detect_01_04.html timestamp="+ts;
			}
		}
		else
		{
			if(pre_url_info_2[3] == "BRS_ap_detect_01_ap_01.html"){
				cf.ap_pre_link.value = "0";
				cf.action="/apply.cgi?/BRS_00_03_ap_setup.html timestamp="+ts;
			}
			else{
				cf.ap_pre_link.value = "1";
				cf.action="/apply.cgi?/BRS_00_03_ap_setup.html timestamp="+ts;
			}
		}
		cf.submit_flag.value="wl_ssid_password";

		cf.submit();
		return true;
	}
	else if(cf.use_ap[1].checked)
	{			
		if(wl_login == 1)
		{ 
			if(pre_url_info_2[3] == "BRS_ap_detect_01_ap_01.html")
				this.location.href="BRS_ap_detect_01_03_note.html?id=0";
			else
				this.location.href="BRS_ap_detect_01_03_note.html?id=1";
		}
		else 
		{
			if(pre_url_info_2[3] == "BRS_ap_detect_01_ap_01.html")
				this.location.href = "BRS_00_03_ap_setup.html?id=0";
			else
				this.location.href = "BRS_00_03_ap_setup.html?id=1";
		}	
	}
	else
	{
		alert(bh_warning_info);
		return false;
	}	
}
function goback()
{
	var pre_url_1 = document.referrer;
	var pre_url_info_1 = pre_url_1.split("/");

	if(pre_url_info_1[3] == "BRS_ap_detect_01_ap_01.html" )
	{
		this.location.href = "BRS_ap_detect_01_ap_01.html";
	}
	else if(pre_url_info_1[3] == "BRS_00_02_ap_select.html")
	{
		this.location.href = "BRS_00_02_ap_select.html";
	}
	else if(pre_url_info_1[3].indexOf("id=") != -1)
	{
		var pre_url_id = pre_url_info_1[3].split("=");
		if(pre_url_id[1] == "0")
			this.location.href = "BRS_ap_detect_01_ap_01.html";
		else
			this.location.href = "BRS_00_02_ap_select.html";		
	}
	else{
		if(ap_pre_link == "0")
			this.location.href = "BRS_ap_detect_01_ap_01.html";
		else
			this.location.href = "BRS_00_02_ap_select.html";
	}
}
