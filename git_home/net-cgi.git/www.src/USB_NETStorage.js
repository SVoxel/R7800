/*
As long as users set the read access of a shared folder to 'Admin', 
the write access of the shared folder is changed to 'Admin' automatically.
Similarly, when users set the write access to 'All - no password', 
the read access of the shared folder is changed to 'All - no password' automatically.
*/
function set_read(form, read)
{
	if(read == "1")
		form.writeAccess.value = "1";
}

function set_write(form, write)
{
	if(write == "0")
		form.readAccess.value = "0";
}

/*Check share path is valid or not,invalid return 1,valid return 0*/
function checkPathValid(path)
{
	var new_path;

	new_path = path.replace(/\\/g,"\/");
	if( new_path.indexOf("\/") == -1)
		return 0;
	else
	{
		var up = 0,down=0;
		var path_array = new Array();

		path_array = new_path.split("\/");
		for(var i=1;i<path_array.length;i++)
		{
			if(path_array[i] == "")
				continue;
			else if(path_array[i] == "..")
				up++;
			else
				down++;
		}
		if(up > down)
			return 1;
		else
			return 0;
	}
}

function clickadvanced()
{
	var form=document.forms[0];
	if(consolidate_device_name != 1){
		if(form.deviceName.value == show_deviceName)
			form.device_name_change_flag.value = 0;
		else
			form.device_name_change_flag.value = 1;
		var dev_name=form.deviceName.value;
		if(dev_name == ""){
			alert("$deviceName_NULL");
			return false;
		}
		for(i=0;i<dev_name.length;i++){
			if(isValidDevName(dev_name.charCodeAt(i))==false)
			{
				alert("$device_name_error");
				return false;   
			}
		}
		
	}else		
		form.device_name_change_flag.value = 0;
	var space_flag=0;
	

	if( form.enable_samba.checked == true)
		form.enableNet_value.value = '0';
	else
		form.enableNet_value.value = '1';
	if( form.enable_http.checked == true)
		form.enableHTTP_value.value = '0';
	else
		form.enableHTTP_value.value = '1';
	if( form.enable_wan_http.checked == true )
		form.enableHvia_value.value = '0';
	else
		form.enableHvia_value.value = '1';
	
	if(form.passwd_samba.checked == true)
		form.passwdNet.value = "1";
	else
		form.passwdNet.value = "0";
	if(form.passwd_http.checked == true)
		form.passwdHttp.value = "1";
	else
		form.passwdHttp.value = "0";
	if(form.passwd_http_via.checked == true)
		form.passwdHvia.value = "1";
	else
		form.passwdHvia.value = "0";

	if(form.workGroup.value == ""){
		alert("$workGroup_NULL");
		return false;
	}
/*	for(i=0;i<form.workGroup.value.length;i++){
		if(isValidChar_space(form.workGroup.value.charCodeAt(i))==false)
		{
			alert("$workgroup"+"$not_allow_2bytes");
			return false;
		}
	}
*/
/*	if( form.workGroup.value.checkShareName()){
		alert("$invalid_char "+"$workgroup");
		return false;
	}
*/
	portvalue1=parseInt(form.http_via_port.value,10);
	if( isNaN(portvalue1) || portvalue1 < 443 || (portvalue1 > 443 && portvalue1 < 1024) || portvalue1 > 65534)
	{
		alert("$usb_httpvia_port");
		return false;
	}
	form.http_via_port.value=portvalue1;
   if(is_ftp == 1)
   {	
        if( form.enable_ftp.checked == true )
                form.enableFTP_value.value = '0';
        else
                form.enableFTP_value.value = '1';

        if( form.enable_wan_ftp.checked == true )
                form.enableFvia_value.value = '0';
        else
                form.enableFvia_value.value = '1';
			
		if(form.passwd_ftp.checked == true)
			form.passwdFtp.value = "1";
		else
			form.passwdFtp.value = "0";
		if(form.passwd_ftp_via.checked == true)
			form.passwdFvia.value = "1";
		else
			form.passwdFvia.value = "0";

	portvalue2=parseInt(form.ftp_via_port.value,10);
	if( isNaN(portvalue2) || portvalue2 < 21 || (portvalue2 > 21 & portvalue2 < 1024) || portvalue2 > 65534)
	{
		alert("$usb_ftpvia_port");
		return false;
	}
	/*Bug 30038 - [USB]When change FTP(via internet) and HTTP(via internet) port to a same port,user should not apply the configuration successfully */
	if(portvalue1==portvalue2)
	{
		alert("$invalid_port_used");
		return false;
	}
	form.ftp_via_port.value=portvalue2;
   }
	
	if(check_all_port() == false)
	{
		alert("$invalid_port_used");
		return false;
	}
        form.http_via_port.value=port_range_interception(form.http_via_port.value);
        form.ftp_via_port.value=port_range_interception(form.ftp_via_port.value);
	/* to fix bug 31198
	if(shared_folder_num != 0 && nopassword_num == 0)
		alert("$usb_nopassward_msg");
	*/
	form.submit();
	return true;
}

function close_window()
{
	window.close();
}
function closeBrowserwindow()
{
	window.open('', '_self', '');
	window.parent.close();
}
function check_input()
{
	form=document.forms[0];
			
}

//check if the access port in in the range of port forwarding.
function check_port_range(name, length, port_using)
{
	var i, str, each_info, sertype, startport, endport, serflag;
	for(i=1; i<=length; i++)
	{
		switch(name)
		{
			case "forwardingArray":
				str = eval(name + i);
				each_info=str.split(' ');
				sertype=each_info[1];
				startport=each_info[2];
				endport=each_info[3];
				break;
			case "triggeringArray":
				str = eval(name + i);
				each_info=str.split(' ');
				sertype=each_info[5];
				startport=each_info[6];
				endport=each_info[7];
				break;
			case "upnpArray":
				str = eval(name + i);
				each_info=str.split(';');
				sertype=each_info[1];
				startport=each_info[3];
				endport=each_info[3];
				break;
			default:
				break;
		}
		
		if(sertype != "UDP")
		{
			if(parseInt(startport)<=parseInt(port_using) && parseInt(endport)>=parseInt(port_using))
				return false;
		}
	}

	return true;
}
function check_single_port(port)
{
	//check with remote
        if(endis_remote=="1" && (parseInt(port)==parseInt(remote_port)))
		return false;
}
//if the Access Method is enabled, check if the port is being taken up by forwarding list, triggering list, upnp list and remote, which use protocal of TCP. 
function check_all_port()
{
	var ret=true;	

	if(form.enable_wan_http.checked == true)
	{
		if(check_port_range("forwardingArray", forward_array_num, form.http_via_port.value) == false)
			ret = false;
	
		if(check_port_range("triggeringArray", trigger_array_num, form.http_via_port.value) == false)
			ret = false;

		if(check_port_range("upnpArray", upnp_array_num, form.http_via_port.value) == false)
			ret = false;

		if(check_single_port(form.http_via_port.value) == false)
			ret = false;
	}

	if(is_ftp == 1)
	{
		if(form.enable_ftp.checked == true)
		{
			if(check_port_range("forwardingArray", forward_array_num, 21) == false)
				ret = false;
			
			if(check_port_range("triggeringArray", trigger_array_num, 21) == false)
				ret = false;

			if(check_port_range("upnpArray", upnp_array_num, 21) == false)
				ret = false;

			if(check_single_port(21) == false)
				ret = false;
		}
		if(form.enable_wan_ftp.checked == true)
		{
			if(check_port_range("forwardingArray", forward_array_num, form.ftp_via_port.value) == false)
				ret = false;

			if(check_port_range("triggeringArray", trigger_array_num, form.ftp_via_port.value) == false)
				ret = false;

			if(check_port_range("upnpArray", upnp_array_num, form.ftp_via_port.value) == false)
				ret = false;

			if(check_single_port(form.ftp_via_port.value) == false)
				ret = false;
		}
	}

	return ret;
}

function open_browser(form)
{
	form.submit_flag.value="browser_open";
	form.submit();
}

function close_browser(form)
{
	form.submit_flag.value="browser_close";
	form.submit();
}

