function refresh_log(form)
{
	form.action="/apply.cgi?/FW_log.htm timestamp="+ts;
	form.submit_flag.value="logs_refresh";
	form.submit();
	return true;
}

function check_logs_clear(form)
{
	form.action="/func.cgi?/FW_log.htm timestamp="+ts;
	form.submit_flag.value="logs_clear";
	form.submit();
	return true;
}

function check_logs_send(form)
{
	if (email_get_notify == '1')
	{
		form.action="/apply.cgi?/FW_log.htm timestamp="+ts;
		form.submit_flag.value="logs_send";
		form.submit();
		return true;
	}
	else
	{
		alert("$change_to_email");
		return false;
	}
}
function check_log_apply(form)
{
	var cf=document.forms[0];
	cf.action="/apply.cgi?/FW_log.htm timestamp="+ts;
	if(cf.log_site.checked == true)
		cf.hidden_log_site.value="1";
	else
		cf.hidden_log_site.value="0";
	if(cf.log_block.checked == true)
		cf.hidden_log_block.value="1";
	else
		cf.hidden_log_block.value="0";
	if(cf.log_conn.checked == true)
		cf.hidden_log_conn.value="1";
	else
		cf.hidden_log_conn.value="0";
	if(cf.log_router.checked == true)
		cf.hidden_log_router.value="1";
	else
		cf.hidden_log_router.value="0";
	if(cf.log_dosport.checked == true)
		cf.hidden_log_dosport.value="1";
	else
		cf.hidden_log_dosport.value="0";
	if(cf.log_port.checked == true)
		cf.hidden_log_port.value="1";
	else
		cf.hidden_log_port.value="0";
	if(cf.log_wire.checked == true)
		cf.hidden_log_wire.value="1";
	else
		cf.hidden_log_wire.value="0";
	if(cf.log_conn_reset.checked == true)
		cf.hidden_log_conn_reset.value="1";
	else
		cf.hidden_log_conn_reset.value="0";
	if(cf.log_wire_sched.checked == true)
		cf.hidden_log_wire_sched.value="1";
	else
		cf.hidden_log_wire_sched.value="0";
	if(cf.log_readyshare.checked == true)
		cf.hidden_log_readyshare.value="1";
	else
		cf.hidden_log_readyshare.value="0";
	if(cf.log_mobile_conn.checked == true)
		cf.hidden_log_mobile_conn.value="1";
	else
		cf.hidden_log_mobile_conn.value="0";
	if(cf.log_vpn_head.checked == true)
		cf.hidden_log_vpn_head.value="1";
	else
		cf.hidden_log_vpn_head.value="0";
	cf.submit();
	return true;
}
