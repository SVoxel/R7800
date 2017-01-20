var error_email_addr="$error_email_addr";
var user_name_error="$user_name_error";
var user_name_null="$user_name_null";
var password_error="$password_error";
var password_null="$password_null";
function setPort(cf)
{
	if(cf.portnum_auto){
	if (cf.portnum_auto[0].checked){
		cf.email_port.disabled = true;
	}
	else
		cf.email_port.disabled = false;
	}
}

function setGray(cf,click,hour_24_flag)
{
	if(cf.email_notify.checked)
	{

		cf.block_site.disabled = false;
		cf.email_smtp.disabled = false;
		cf.email_addr.disabled = false;
		if(cf.email_addr2)
			cf.email_addr2.disabled = false;
		cf.email_port.disabled = false;
		if(cf.portnum_auto){
			cf.portnum_auto[0].disabled = false;
			cf.portnum_auto[1].disabled = false;
			
		}
		cf.cfAlert_Select.disabled = false;
		cf.smtp_auth.disabled = false;

		if( cf.sec_connection )
			cf.sec_connection.disabled = false;		
		if( click == 1 )
			cf.block_site.checked = true;
	}
	else
	{
		cf.block_site.disabled = true;
		cf.email_smtp.disabled = true;
		cf.email_addr.disabled = true;
		if(cf.email_addr2)
			cf.email_addr2.disabled = true;
		cf.email_port.disabled = true;
		if(cf.portnum_auto){
			cf.portnum_auto[0].disabled = true;
			cf.portnum_auto[1].disabled = true;
		}
		cf.cfAlert_Select.disabled = true;
		cf.smtp_auth.disabled = true;
		cf.cfAlert_Day.disabled = true;
		cf.cfAlert_Hour.disabled = true;
		if(hour_24_flag == 0)
		{
			cf.cfAlert_am[0].disabled = true;
			cf.cfAlert_am[1].disabled = true;
		}
		if( cf.sec_connection )
			cf.sec_connection.disabled = true;

	}
	setAuth(cf)
}

function setAuth(cf)
{
	if(cf.email_notify.checked)
	{
		if(cf.smtp_auth.checked == true)
		{
			cf.auth_user.disabled = false;
			cf.auth_pwd.disabled = false;
		}
		else
		{
			cf.auth_user.disabled = true;
			cf.auth_pwd.disabled = true;
		}
	}
	else
	{
		cf.auth_user.disabled = true;
		cf.auth_pwd.disabled = true;
	}
}

function change_type(check,cf)
{
	if(check == 0)
		cf.email_this_addr.disabled =true;
	else
		cf.email_this_addr.disabled =false;
}

function disable_am(disable_flag,cf)
{
	cf.cfAlert_am[0].disabled = disable_flag;
	cf.cfAlert_am[1].disabled = disable_flag;
}

function OnAlertChange(cf,hour_24_flag)
{
	var index = cf.cfAlert_Select.selectedIndex;
	if ( (index == 0) || (index == 1) || (index == 4) )
	{
		cf.cfAlert_Day.selectedIndex = 0;
		cf.cfAlert_Hour.selectedIndex= 0;
		cf.cfAlert_Day.disabled = true;
		cf.cfAlert_Hour.disabled = true;
		AlertTimeDisabled = true;
		AlertHourDisabled = true;
		if(hour_24_flag == 0)
			disable_am(true,cf);
	}
	else if(index == 2) // daily
	{
		cf.cfAlert_Day.selectedIndex = 0;
		cf.cfAlert_Day.disabled = true;
		cf.cfAlert_Hour.disabled = false;
		AlertTimeDisabled = true;
		AlertHourDisabled = false;
		if(hour_24_flag == 0)
			disable_am(false,cf);
	}
	else if(index == 3) // weekly
	{
		cf.cfAlert_Day.disabled = false;
		cf.cfAlert_Hour.disabled = false;
		AlertTimeDisabled = false;
		AlertHourDisabled = false;
		if(hour_24_flag == 0)
			disable_am(false,cf);
	}
}

function check_email(cf,hour_24_flag)
{
	if (cf.email_notify.checked) 
	{
		cf.email_notify_enabled.value = "1";
	        for(i=0;i<cf.email_smtp.value.length;i++)
		{
			if(isValidChar(cf.email_smtp.value.charCodeAt(i))==false)
			{
				alert("$error_email_smtp");
				cf.email_smtp.focus();
			        return false;
			}
		}
		if( cf.email_smtp.value == "" )
		{
			alert("$error_email_smtp");
			cf.email_smtp.focus();
			return false;
		}	
		for(i=0;i<cf.email_addr.value.length;i++)
		{
			if(isValidChar(cf.email_addr.value.charCodeAt(i))==false)
			{
				alert("$error_email_addr");
				cf.email_addr.focus();	
				return false;
			}
		}
		if(cf.email_addr2 && cf.email_addr2.value != ""){
			for(i=0;i<cf.email_addr2.value.length;i++)
			{

				if(isValidChar(cf.email_addr2.value.charCodeAt(i))==false)
				{
					alert("$error_email_addr");
					cf.email_addr2.focus();
					return false;
				}
			}
			if((cf.email_addr2.value.indexOf("@", 0) == -1 || cf.email_addr2.value.indexOf(".", 0) == -1) && cf.email_addr2.value != "")
			{
				alert("$error_email_addr");
				cf.email_addr2.focus();
				return false;
			}
		}
		if(cf.email_addr.value == "")
		{
			alert("please fill Primary E-mail Address!");
			return false;
		}
		if(cf.email_addr.value.indexOf("@", 0) == -1 || cf.email_addr.value.indexOf(".", 0) == -1)
		{
			alert("$error_email_addr");
			cf.email_addr.focus();
			return false;
		}

		if( cf.portnum_auto ){
			if(cf.portnum_auto[0].checked == true)
			{
				cf.email_port_sta.value=1;
			}else{
				cf.email_port_sta.value=0;
				cf.email_port_num.value=parseInt(cf.email_port.value,10);
			}
		}


		var emailPort = parseInt(cf.email_port.value, 10);
		if( (cf.portnum_auto && cf.portnum_auto[1].checked == true) || !cf.portnum_auto )
			if(isNaN(emailPort) || emailPort <= 0 || emailPort >= 65535) {
				alert("$invalid_email_port");
			}

		//port_forwarding
		for(i=1;i<=forward_array_num;i++)
		{
			var str = eval ( 'forwardingArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
			var each_info=str.split(' '),
			ext_startport=parseInt(each_info[2], 10),
			ext_endport=parseInt(each_info[3], 10),
			int_startport=parseInt(each_info[4], 10),
			int_endport=parseInt(each_info[5], 10);
			if((emailPort >= ext_startport && emailPort <= ext_endport) || (emailPort >= int_startport && emailPort <= int_endport)) {
				alert("$invalid_port_used");
				return false;
			}
		}
		//port_triggering
		for(i=1;i<=trigger_array_num;i++)
		{
			 var str = eval ( 'triggeringArray' + i ).replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
			var each_info=str.split(' '),
			constart_port=parseInt(each_info[6], 10),
			conend_port=parseInt(each_info[7], 10);
			if(emailPort >= constart_port && emailPort <= conend_port) {
				alert("$invalid_port_used");
				return false;
			}
		}
		//upnp
		for(i=1;i<=upnp_array_num;i++)
		{
			var str = eval ( 'upnpArray' + i );
			var each_info=str.split(';'),
			upnp_int=parseInt(each_info[2], 10),
			upnp_ext=parseInt(each_info[3], 10);
			if(emailPort == upnp_int || emailPort == upnp_ext) {
				alert("$invalid_port_used");
				return false;
			}
		}

		//check USB ready share port
		if( usb_router_flag == 1)
		{
			if(check_readyshare_port(cf.email_port.value, cf.email_port.value, "WAN") == false)
			{
				alert("$invalid_port_used");
				return false;
			}
		}

		//check with remote
		if (parseInt(remote_port) == emailPort && remote_port != "")
		{
			alert("$invalid_port_used");
			return false;
		}

		if( cf.email_port )
			cf.email_port.value=parseInt(cf.email_port.value,10);

		if(cf.smtp_auth.checked == true)
		{	
			for(i=0;i<cf.auth_user.value.length;i++)
			{
				if(isValidChar(cf.auth_user.value.charCodeAt(i))==false)
				{
					alert("$user_name_error");
					return false;
				}
			}
			for(i=0;i<cf.auth_pwd.value.length;i++)
			{
				if(isValidChar(cf.auth_pwd.value.charCodeAt(i))==false)
				{
					alert("$password_error");
					return false;
				}
			}
			if(cf.auth_user.value=="")
			{
				alert("$user_name_null");
				return false;
			}
			if(cf.auth_pwd.value=="")
			{
				alert("$password_null");
				return false;
			}
			cf.email_endis_auth.value=1;
		}
		else {
			cf.email_endis_auth.value=0;
		}
	}
	else 
	{
		cf.email_notify_enabled.value = "0";	
		if(cf.smtp_auth.checked == true)
			cf.email_endis_auth.value=1;
		else
			cf.email_endis_auth.value=0;
	}

		if(cf.send_email_noti.checked)
			cf.send_email_noti_hid.value = "1";
		else
			cf.send_email_noti_hid.value = "0";

		if(cf.block_site.checked)
			cf.send_alert_immediately.value = "1";
		else
			cf.send_alert_immediately.value = "0";

		var schedule_time=parseInt(cf.cfAlert_Hour.value);
		if(cf.cfAlert_Select.selectedIndex == 2)// daily
		{
			if(hour_24_flag == 0)
			{
				if(cf.cfAlert_am[1].checked) 
					schedule_time = schedule_time + 12;
			}
			cf.schedule_hour.value = schedule_time;
		}
		if(cf.cfAlert_Select.selectedIndex == 3) // weekly
		{
			if(hour_24_flag == 0)
			{
				if(cf.cfAlert_am[1].checked) 
					schedule_time = schedule_time + 12;
			}
			cf.schedule_hour.value = schedule_time;
		}
		cf.email_addr_hid1.value = cf.email_addr.value;
		if(cf.email_addr2)
			cf.email_addr_hid2.value = cf.email_addr2.value;
	cf.email_smtp_hid.value = cf.email_smtp.value;
	cf.auth_user_hid.value = cf.auth_user.value;
	cf.auth_pwd_hid.value = cf.auth_pwd.value;
	cf.cfAlert_Select_hid.value = cf.cfAlert_Select.value;
	cf.cfAlert_Day_hid.value = cf.cfAlert_Day.value;
	if( cf.portnum_auto && cf.portnum_auto[0].checked == true)
		parent.smtp_auto_detect = 1
	cf.submit();
	return true;
}
