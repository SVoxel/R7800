function check_fastlane()
{
	var cf = document.forms[0];

	if( cf.fast_lane_enable.checked == true)
		cf.http_fastlane_enable.value = "1";
	else
		cf.http_fastlane_enable.value = "0";

	if( cf.cfTrusted_IPAddress4.value != "" )
	{
		cf.cfTrusted_IPAddress.value=cf.cfTrusted_IPAddress1.value+'.'+cf.cfTrusted_IPAddress2.value+'.'+cf.cfTrusted_IPAddress3.value+'.'+cf.cfTrusted_IPAddress4.value;
		if(checkipaddr(cf.cfTrusted_IPAddress.value)==false || is_sub_or_broad(cf.cfTrusted_IPAddress.value, lan_ip, lan_subnet) == false || isSameIp(cf.cfTrusted_IPAddress.value,lan_ip) == true )
		{
			alert("$invalid_ip");
			return false;
		}
		if(isSameSubNet(cf.cfTrusted_IPAddress.value,lan_subnet,lan_ip,lan_subnet) == false)
		{
			alert("$same_subnet_ip_trusted");
			return false;
		}
	}
	else
		cf.cfTrusted_IPAddress.value = "";

	return true;
}
