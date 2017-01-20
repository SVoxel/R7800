function trans_firmware_name(name)
{
	var each_segment = name.split('-');
	var ver_name;

	ver_name = "v"+each_segment[2].charAt(0)+"."+each_segment[2].charAt(1)+"."+each_segment[3].charAt(0)+"."+each_segment[3].charAt(1)+each_segment[1];

	return ver_name;
}

function set_led_status(mac, led_status)
{
	var cf = document.forms[0];	
	
	cf.plc_dev_led_set.value = led_status;
	cf.plc_dev_led_mac.value = mac;
	cf.submit_flag.value = "plc_dev_led_control";
	cf.action = "/apply.cgi?/PLC_wait_scan.htm timestamp="+ts;

	return true;
}

function check_dev_reset(mac)
{
	var cf = document.forms[0];
	
	cf.plc_dev_reset_mac.value = mac;
	cf.submit_flag.value = "plc_dev_reset";
	cf.action = "/apply.cgi?/PLC_wait_scan.htm timestamp="+ts;	

	return true;
}
