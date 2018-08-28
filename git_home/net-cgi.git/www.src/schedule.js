function BlockAllClickCheck(cf)
{
	if(top.enable_ap_flag == 1 || enable_extender_flag == "1") //for bug 30286,grey this when ap mode is on
	{
		cf.checkboxNameAll.disabled = true;
		
		cf.checkboxNameMon.disabled = true;
		cf.checkboxNameTue.disabled = true;
		cf.checkboxNameWed.disabled = true;
		cf.checkboxNameThu.disabled = true;
		cf.checkboxNameFri.disabled = true;
		cf.checkboxNameSat.disabled = true;
		cf.checkboxNameSun.disabled = true;
	}
	else
	{
		if(cf.checkboxNameAll.checked)
		{
			cf.checkboxNameMon.checked = true;
			cf.checkboxNameTue.checked = true;
			cf.checkboxNameWed.checked = true;
			cf.checkboxNameThu.checked = true;
			cf.checkboxNameFri.checked = true;
			cf.checkboxNameSat.checked = true;
			cf.checkboxNameSun.checked = true;


			cf.checkboxNameMon.disabled = true;
			cf.checkboxNameTue.disabled = true;
			cf.checkboxNameWed.disabled = true;
			cf.checkboxNameThu.disabled = true;
			cf.checkboxNameFri.disabled = true;
			cf.checkboxNameSat.disabled = true;
			cf.checkboxNameSun.disabled = true;
		}
		else
		{
			cf.checkboxNameMon.disabled = false;
			cf.checkboxNameTue.disabled = false;
			cf.checkboxNameWed.disabled = false;
			cf.checkboxNameThu.disabled = false;
			cf.checkboxNameFri.disabled = false;
			cf.checkboxNameSat.disabled = false;
			cf.checkboxNameSun.disabled = false;
		}
	}
	
	BlockPeriodClick(cf);
	return;
}

function BlockPeriodClick(cf)
{
	if(top.enable_ap_flag == 1 || enable_extender_flag == "1") // for bug 30286,grey this when ap mode is on
	{
		cf.checkboxNamehours.disabled = true;
		cf.start_hour.disabled = true;
		cf.start_minute.disabled = true;
		cf.end_hour.disabled = true;
		cf.end_minute.disabled = true;
	}
	else
	{
		if(cf.checkboxNamehours.checked == true)
		{
			cf.start_hour.disabled = true;
			cf.start_minute.disabled = true;
			cf.end_hour.disabled = true;
			cf.end_minute.disabled = true;
			TimePeriodDisabled = true;
			ClearData1();
		}
		else
		{
			cf.start_hour.disabled = false;
			cf.start_minute.disabled = false;
			cf.end_hour.disabled = false;
			cf.end_minute.disabled = false;
			TimePeriodDisabled = false;
		}
	}
	return;
}

function ClearData1()
{
	var cf = document.forms[0];

	cf.start_hour.value = "0";
	cf.start_minute.value = "0";
	cf.end_hour.value = "24";
	cf.end_minute.value = "0";
}

function check_schedule_apply(cf)
{
	var cf = document.forms[0];
	var value = 0;
	var start_time,end_time;
	var day_str="";
	var tmp_all_day;

	if (cf.checkboxNameAll.checked)
		day_str = "everyday";
	else
	{
		if (cf.checkboxNameSun.checked)
			day_str += "0,";
		if (cf.checkboxNameMon.checked)
			day_str += "1,";
		if (cf.checkboxNameTue.checked)
			day_str += "2,";
		if (cf.checkboxNameWed.checked)
			day_str += "3,";
		if (cf.checkboxNameThu.checked)
			day_str += "4,";
		if (cf.checkboxNameFri.checked)
			day_str += "5,";
		if (cf.checkboxNameSat.checked)
			day_str += "6,";
	}

	cf.days_to_block.value = day_str;

	if( day_str == "" )
	{
		alert("$invalid_day");
		return false;
	}
	
	if (cf.checkboxNamehours.checked == true)
		tmp_all_day = 1;
	else
	{		
		tmp_all_day = 0;	
		if (cf.start_hour.value == cf.end_hour.value && cf.start_minute.value == cf.end_minute.value)
		{
			alert("$invalid_time");
			return false;
		}
		if( !_isNumeric(cf.start_hour.value) || !_isNumeric(cf.end_hour.value) || !_isNumeric(cf.start_minute.value) || !_isNumeric(cf.end_minute.value) )
		{
			alert("$invalid_time");
			return false;
		}
		if ((cf.start_hour.value < 0) || (cf.start_hour.value > 23) ||
				(cf.end_hour.value < 0) || (cf.end_hour.value > 23) ||
				(cf.start_minute.value < 0) || (cf.start_minute.value > 59) ||
				(cf.end_minute.value < 0) || (cf.end_minute.value > 59))
		{
			if( (cf.start_hour.value == "24" && cf.start_minute.value == "0") ||(cf.start_hour.value == "24" && cf.start_minute.value == "00") || (cf.end_hour.value == "24" && cf.end_minute.value == "0" ) || (cf.end_hour.value == "24" && cf.end_minute.value == "00" ) )
			{
				if ((cf.start_hour.value < 0) || (cf.start_hour.value > 23) ||
						(cf.start_minute.value < 0) || (cf.start_minute.value > 59) )
				{
					alert("$invalid_time");
					return false;	
				}
			}
			else
			{
				alert("$invalid_time");
				return false;
			}
		}

		if ((cf.start_hour.value == '') || (cf.end_hour.value == ''))
		{
			alert("$invalid_time");
			return false;
		}


		if ((cf.start_hour.value != '') && (cf.end_hour.value != ''))
		{
			if (cf.start_minute.value == '')
				cf.start_minute.value = 0;
			if (cf.end_minute.value == '')
				cf.end_minute.value = 0;
		}
		start_time = cf.start_hour.value + ':' + cf.start_minute.value;
		end_time = cf.end_hour.value + ':' + cf.end_minute.value;
		cf.start_block_time.value = start_time;
		cf.end_block_time.value = end_time;
	}

	if (tmp_all_day == 1)
	{
		cf.start_block_time.value = "0:0";
		cf.end_block_time.value = "24:0";
	}
	else
	{
		var start_time = cf.start_block_time.value.split(":");
		var end_time = cf.end_block_time.value.split(":");

		cf.start_block_time.value = start_time[0]+":"+start_time[1];
		cf.end_block_time.value = end_time[0]+":"+end_time[1];
	}
	cf.hidden_all_day.value=tmp_all_day;

	cf.submit();
	return true;
}


