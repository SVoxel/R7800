function goto_next(cf, wl_login)
{
	if(cf.choose[0].checked)
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
			cf.action="/apply.cgi?/BRS_ap_detect_01_04.html timestamp="+ts;
		else
			cf.action="/apply.cgi?/BRS_01_checkNet_ping.html timestamp="+ts;
		cf.submit_flag.value="wl_ssid_password";
	
		cf.submit();
	}
	else if(cf.choose[1].checked)
		this.location.href="BRS_01_checkNet_ping.html";
	else
	{
		alert(bh_warning_info);
		return false;
	}	
}

function goback()
{	
	var pre_url = document.referrer;
	var pre_url_info = pre_url.split("/");
	
	if(pre_url_info[3] == "BRS_ap_detect_01_router_01.html")
		this.location.href = "BRS_ap_detect_01_router_01.html";
	else if(pre_url_info[3] == "BRS_00_02_ap_select.html")
		this.location.href = "BRS_00_02_ap_select.html";
	else
		return false;
}

