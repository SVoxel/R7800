function check_media(form)
{
	if(form.media_server.checked)
		form.media_server_onoff.value="1";
	else
		form.media_server_onoff.value="0";
	if(form.media_server_tivo.checked == true)
		form.media_server_tivo_flag.value="yes";
	else
		form.media_server_tivo_flag.value="no";	
	if(form.enable_itunes.checked == true)
                form.itunes_onoff.value="1";
        else
                form.itunes_onoff.value="0";	

	check_name();

	form.submit();
}
function check_media_scan(form)
{
	//<!-- bug 23351:the "conent scan->automatic" button is a radio button not checkbox,it must be enabled always and CAN NOT be turned off by users -->
	form.submit_flag.value="upnp_media_scan";
	form.submit();
}
