function click_ddns(form)
{
	var new_sysDNSHost = str_replace(old_sysDNSHost);
	var new_sysDNSUser = str_replace(old_sysDNSUser);
	var password_change = 0;
	if (form.sysDNSActive.checked)
		form.ddns_enabled.value = "1";
	else
		form.ddns_enabled.value = "0";

	if(ddns_wildcards_flag==1 && form.sysDNSProviderlist.value == "www/var/www.3322.org")
	{
		if (form.sysDNSWildCard.checked)
			form.wildcards_enabled.value="1";
		else
			form.wildcards_enabled.value="0";
	}
	else
		form.wildcards_enabled.value="0";
    form.hidden_sysDNSProviderlist.value = form.sysDNSProviderlist.value;
	form.sysDNSHost.value=form.sysDNSHost.value.replace(/\s/ig,'');
	form.hidden_sysDNSHost.value = form.sysDNSHost.value;
	if (form.sysDNSActive.checked)
	{
		if(form.sysDNSHost.value=="" && form.sysDNSProviderlist.value != "www/var/www.oray.cn")
		{
			alert("$hostname_null");
			return false;
		}
		if(form.sysDNSUser.value=="")
		{
			alert("$user_name_null");
			return false;
		}
		if(form.sysDNSPassword.value=="")
		{
			alert("$password_null");
			return false;
		}
		if(form.sysDNSProviderlist.value != "www/var/www.oray.cn")//hostname
		{
			for (i=0;i<form.sysDNSHost.value.length;i++)
			{
				if(isValidDdnsHost(form.sysDNSHost.value.charCodeAt(i))==false)
				{
					alert("$hostname_error");
					return false;
				}
			}
		}
		if(form.sysDNSProviderlist.value == "www/var/www.oray.cn")//username
		{
			for (i=0;i<form.sysDNSUser.value.length;i++)
			{//37033
				if(isValidDdnsOrayUserName(form.sysDNSUser.value.charCodeAt(i))==false)
				{
					alert("$user_name_error");
					return false;
				}
			}
		}
		else
		{
			for (i=0;i<form.sysDNSUser.value.length;i++)
			{
				if(isValidChar_space(form.sysDNSUser.value.charCodeAt(i))==false)
				{
					alert("$user_name_error");
					return false;
				}
			}
		}
		for (i=0;i<form.sysDNSPassword.value.length;i++)//password
		{
			if(isValidChar_space(form.sysDNSPassword.value.charCodeAt(i))==false)
			{
				alert("$password_error");
				return false;
			}
		}
		if((form.sysDNSProviderlist.value == 'www/var/www.DynDNS.org' && form.hidden_pwd_change_1.value == "1") ||
		 (form.sysDNSProviderlist.value == 'www/var/www.oray.cn' && form.hidden_pwd_change_2.value == "1") ||
		 (form.sysDNSProviderlist.value == 'www/var/www.3322.org' && form.hidden_pwd_change_3.value == "1") ||
		 (form.sysDNSProviderlist.value == 'www/var/dynupdate.no-ip.com' && form.hidden_pwd_change_4.value == "1"))
			password_change=1;
		if ((new_sysDNSHost == form.sysDNSHost.value) && (new_sysDNSUser == form.sysDNSUser.value) && password_change == 0 && (old_endis_wildcards == form.wildcards_enabled.value) && (old_endis_ddns == form.ddns_enabled.value) && (dns_list == form.sysDNSProviderlist.value))
		{
			alert("$ddns_warning_message");
			return false;
		}
	}
	if (old_endis_ddns != form.ddns_enabled.value || old_sysDNSHost != form.hidden_sysDNSHost.value || old_sysDNSUser != form.sysDNSUser.value || password_change == 1 || old_endis_wildcards != form.wildcards_enabled.value)
		form.change_wan_type.value=0;
	else
		form.change_wan_type.value=1;
	if((dns_list != form.sysDNSProviderlist.value || (dns_list == form.sysDNSProviderlist.value && old_sysDNSHost != form.sysDNSHost.value)) && vpn_enable == "1" && form.sysDNSActive.checked == true)
		form.hid_vpn_detect_ddns_change.value = 1;
	else
		form.hid_vpn_detect_ddns_change.value = 0;
	parent.ddns_post_flag = 1;
	return true;
}

function str_replace(str)
{
	str = str.replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");

	return str;
}

function parse_xss_str()
{
	old_sysDNSHost_1 = str_replace(old_sysDNSHost_1);
	old_sysDNSUser_1 = str_replace(old_sysDNSUser_1);

	old_sysDNSHost_2 = str_replace(old_sysDNSHost_2);
	old_sysDNSUser_2 = str_replace(old_sysDNSUser_2);

	old_sysDNSHost_3 = str_replace(old_sysDNSHost_3);
	old_sysDNSUser_3 = str_replace(old_sysDNSUser_3);

	old_sysDNSHost_4 = str_replace(old_sysDNSHost_4);
	old_sysDNSUser_4 = str_replace(old_sysDNSUser_4);
}

function change_ddns_password(obj)
{
	var cf = document.forms[0];
	if( obj.type == "password" )
	{
		if( get_browser() == "Firefox" )
		{		
			obj.value = "";
			obj.type = "text";
		}
		else
		{
			obj.outerHTML = '<input type="text" name="sysDNSPassword" onfocus="this.select();change_ddns_password(this);" size="32" maxlength="64">';
			cf.sysDNSPassword.select();
		}
	}

	if( cf.sysDNSProviderlist.value == 'www/var/www.DynDNS.org' )
		cf.hidden_pwd_change_1.value="1";
	else if( cf.sysDNSProviderlist.value == 'www/var/www.oray.cn')
		cf.hidden_pwd_change_2.value="1";
	else if( cf.sysDNSProviderlist.value == 'www/var/www.3322.org' )
		cf.hidden_pwd_change_3.value="1";
	else if( cf.sysDNSProviderlist.value == 'www/var/dynupdate.no-ip.com' )
		cf.hidden_pwd_change_4.value="1";
}
