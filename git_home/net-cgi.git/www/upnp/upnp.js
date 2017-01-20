function click_upnp(time,timelive)
{
	if(document.forms[0].UpnP.checked == true)
	{
		document.forms[0].AdverTime.disabled=false;
		document.forms[0].TimeToLive.disabled=false;
		if(time=="")
			document.forms[0].AdverTime.value="";
		else
		{
			time_value=parseInt(time);
			time_value=time_value/60;
			document.forms[0].AdverTime.value=time_value;
		}
		document.forms[0].TimeToLive.value=timelive;
	}
	else
	{
		document.forms[0].AdverTime.disabled=true;										     
		document.forms[0].TimeToLive.disabled=true;	 									  
		if(time=="")	
			document.forms[0].AdverTime.value="";             
		else
		{
			time_value=parseInt(time);
			time_value=time_value/60;
			document.forms[0].AdverTime.value=time_value;
		}	 
		document.forms[0].TimeToLive.value=timelive;
	}
	document.forms[0].hidden_advertime.value=time_value;
	document.forms[0].hidden_timetolive.value=parseInt(timelive, 10);
}

function checkupnp()
{
	if(document.forms[0].UpnP.checked == true)
		document.forms[0].upnp_onoff.value = 1;
	else
		document.forms[0].upnp_onoff.value = 0;

	if(document.forms[0].UpnP.checked == true)
	{
		if(!((document.forms[0].AdverTime.value<=1440)&&(document.forms[0].AdverTime.value>=1)))
		{
			alert("$upnp_AdverTime_1_1440");
			return false;
		}
		document.forms[0].TimeToLive.value = parseInt(document.forms[0].TimeToLive.value, 10);
		if(!((document.forms[0].TimeToLive.value<=255)&&(document.forms[0].TimeToLive.value>=1)))
		{
			alert("$upnp_TimeToLive_1_255");
			return false;
		}
		var minute_time=parseInt(document.forms[0].AdverTime.value,10);
		var second_time=minute_time*60;
		document.forms[0].changetime.value=second_time;
	}
	document.forms[0].hidden_advertime.value=document.forms[0].changetime.value;
	document.forms[0].submit();
	
	return true;
}
