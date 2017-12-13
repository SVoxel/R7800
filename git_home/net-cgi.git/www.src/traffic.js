function change_volumn_time()
{
	var cf=document.forms[0];
	if(cf.tm_type[0].checked == true)
	{
		cf.conntime_monthly_limit.disabled = true;
		cf.traff_dir.disabled = false;
		cf.volume_monthly_limit.disabled = false;
		if(basic_type == 1)
			cf.round_up_volume.disabled = true;
		else
			cf.round_up_volume.disabled = false;
	}
	else
	{
		cf.conntime_monthly_limit.disabled = false;
		cf.traff_dir.disabled = true;
		cf.volume_monthly_limit.disabled = true;
		cf.round_up_volume.disabled = true;	
	}
}

function set_gray()
{
	var cf=document.forms[0];
	var dflag;
	dflag=(!(cf.checkTraffic.checked));
	setDisabled ( dflag, cf.tm_type[0], cf.tm_type[1], cf.traff_dir, cf.volume_monthly_limit, cf.round_up_volume, cf.conntime_monthly_limit, cf.hour, cf.minute,  cf.day,  cf.waterMark, cf.checkLed, cf.checkBlock, cf.ampm,cf.restartCounter, cf.refresh, cf.trafficStatus, cf.wan_interface);	//cf.restartCounter, cf.refresh, cf.trafficStatus,
	if( basic_type == 1)
		setDisabled ( true, cf.round_up_volume, cf.tm_type[1], cf.conntime_monthly_limit);
	if(cf.checkTraffic.checked == true)
	{
	document.getElementById("restart").className  = "long_common_bt";
	document.getElementById("refresh").className  = "common_key_bt";
	document.getElementById("status").className  = "common_key_bt";
	change_volumn_time();
	check_limit();
	}
	else
	{
	document.getElementById("restart").className  = "long_common_gray_bt";
	document.getElementById("refresh").className  = "common_key_gray_bt";
	document.getElementById("status").className  = "common_key_gray_bt";
	}

}

function check_traffic_apply(cf)
{
	var cf=document.forms[0];
	if(cf.checkTraffic.checked == true)
		cf.endis_traffic.value=1;
	else
		cf.endis_traffic.value=0;
	if(readyshare_mobile=="1")
	{
		if(mobile_conn_status!="15" &&cf.wan_interface.value=="1")//no connected
		{
			location.href="traffic.htm";
			return false;
		}
	}
	if(cf.checkTraffic.checked == true)
	{

		if(cf.tm_type[0].checked)
		{
			cf.hidden_tm_type.value = "0";
			if(cf.traff_dir.value == "No limit")
				cf.hidden_traff_dir.value = "0";
			else if(cf.traff_dir.value == "Download only")
				cf.hidden_traff_dir.value = "1";
			else
				cf.hidden_traff_dir.value = "2";
			if(cf.volume_monthly_limit.value=='')
			{
				alert("$monthly_limit_error");
				return false;
			}
			if(!_isNumeric(cf.volume_monthly_limit.value))
        		{
                		alert("$monthly_limit_error");
                		return false;
        		}
			if(parseInt(cf.volume_monthly_limit.value)>999999)
			{
				alert("$monthly_limit_error");
                                return false;				
			}
			if(cf.round_up_volume.value=='')
			{
				alert("$round_up_data_error");
				return false;
			}
			if(!_isNumeric(cf.round_up_volume.value))
                        {
                                alert("$round_up_data_error");
                                return false;
                        }
			if(cf.waterMark.value=='')
			{
				alert("$left_volumn_error");
				return false;
			}
			if(!_isNumeric(cf.waterMark.value))
                        {
                                alert("$left_volumn_error");
                                return false;
                        }
			if(parseInt(cf.waterMark.value)!="0" && parseInt(cf.volume_monthly_limit.value)!="0")//bug 30092
			{
				if(parseInt(cf.waterMark.value) > parseInt(cf.volume_monthly_limit.value))//bug 28355
				{
					alert("$left_volumn_small");
					return false;
				}
			}
			if( basic_type == 1 && cf.round_up_volume.value != "0" )/* to fix bug 23024 */
			{
				cf.round_up_volume.value = "0";
			}
			/*fix bug28360 Round up data volume should be smaller than the monthly limit*/
			if(basic_type == 0 && parseInt(cf.round_up_volume.value) !="0" && parseInt(cf.volume_monthly_limit.value)!="0")//bug 30092
			{
				if(parseInt(cf.round_up_volume.value) >= parseInt(cf.volume_monthly_limit.value))
				{
					alert("$round_volumn_small");
					return false;
				}
			}
			cf.hidden_round_up.value=cf.round_up_volume.value;
		}
		else
		{
			cf.hidden_tm_type.value = "1";
			var str1 = cf.conntime_monthly_limit.value;
			if(cf.conntime_monthly_limit.value=='')
			{
				alert("$monthly_limit_error");
				return false;
			}
			for(var i=0; i < str1.length; i++)
			{
				if(!(str1[i]>=0 && str1[i]<=9))
				{
					alert("$month_linmit_int");
					return false;
				}
			}
			if(!(cf.conntime_monthly_limit.value <= 744))
			{
				alert("$monthly_limit_744");
				return false;
			}
			if(cf.waterMark.value=='')
                        {
                                alert("$left_volumn_error");
                                return false;
                        }
                        if(!_isNumeric(cf.waterMark.value))
                        {
                                alert("$left_volumn_error");
                                return false;
                        }
			if(parseInt(cf.waterMark.value) >= parseInt(cf.conntime_monthly_limit.value)*60)//bug 28355
			{
				alert("$left_volumn_small");
				return false;
			}
		}
		if ((cf.hour.value < 0) || (cf.hour.value > 11) ||
			(cf.minute.value < 0) || (cf.minute.value > 59) )
			{
				alert("$invalid_time");
				return false;
			}
		if ((!_isNumeric(cf.hour.value)) ||
				(!_isNumeric(cf.minute.value)))
			{
				alert("$invalid_time");
				return false;
			}

		if ((cf.hour.value == '') || (cf.minute.value == ''))
			{
				alert("$invalid_time");
				return false;
			}
		var hour=cf.hour.value;
		var minute=cf.minute.value
		if(hour.length<2)
			hour="0"+hour;
		if(minute.length<2)
			minute="0"+minute;
		if(cf.ampm.selectedIndex==1)
		{
			hour=parseInt(hour,10)+12;
			hour=hour.toString();
		}	
		cf.restart_counter_time.value=hour+':'+minute;
		if(cf.checkLed.checked == true)
			cf.traffic_led.value=1;
		else
			cf.traffic_led.value=0;
		if(cf.checkBlock.checked == true)
			cf.traffic_block_all.value=1;
		else
			cf.traffic_block_all.value=0;
	}
	
	cf.submit();
	return true;
}

function click_restart()
{
	var cf=document.forms[0];
	if(cf.checkTraffic.checked == true)
	{
	    if(!confirm("$traffic_restart_counter"))
			return false;
		cf.submit_flag.value="traffic_reset";
		cf.submit();
	}

}

function click_refresh_2()
{
	var cf=document.forms[0];
	if (cf.checkTraffic.checked == true)
	{
		location.href='traffic.htm';
	}
}

function click_status()
{
	var cf=document.forms[0];
	if (cf.checkTraffic.checked == true)
	{
		window.open('show_traffic.htm','show_traffic','width=600,height=400,top=200,left=200,status=yes,resizable=yes');
	}
}

function reset_time()
{
	cf=document.forms[0];
	if( cf.interval.value == "")
	{
		cf.interval.value = "10";
		return true;
	}
	var timeset=cf.interval.value;

        for(i=0;i<timeset.length;i++)
        {
                c=timeset.charAt(i);
                if("0123456789".indexOf(c,0)<0)
                {
			alert("$rang_pool");
                        return false;
                }
        }

	cf.interval.value=parseInt(cf.interval.value,10);
	if(!(cf.interval.value >=5 && cf.interval.value<=  86400))
	{
		alert("$rang_pool");
		return false;
	}
	cf.submit();

	return true;
}

function click_refresh()
{
	cf=document.forms[0];
	cf.submit_flag.value="refresh_traffic"
	cf.action="/cgi-bin/no_commit.cgi?/cgi-bin/traffic.htm"
	cf.submit();
}

