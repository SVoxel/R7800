function checkpasswd(cf)
{
	for(i=0;i<cf.sysNewPasswd.value.length;i++)
	{       
		if(isValidChar_space(cf.sysNewPasswd.value.charCodeAt(i))==false)
		{
			alert("$password_error");
			return false;
		}
	}
	if (cf.sysNewPasswd.value.length >= 33 || cf.sysConfirmPasswd.value.length >= 33)
	{
		alert("$max_password_length");
		return false;
	}
	if(cf.sysNewPasswd.value != cf.sysConfirmPasswd.value)
	{ 
		alert("$newpas_notmatch");
		return false;	
	}
	if(cf.sysOldPasswd.value != "" && cf.sysNewPasswd.value == "")
	{
		alert("$password_null");
		return false;
	}
	if( cf.enable_recovery.checked == true )
	{
		cf.hidden_enable_recovery.value="1";
		if( cf.question1.value == "0" || cf.question2.value == "0")
		{
			alert("$select_quest");
			return false;
		}
	
		if( cf.answer1.value == "" || cf.answer2.value == "" )
		{
			alert("$enter_answer");
			return false;
		}
		if( cf.answer1.value.length > 64 || cf.answer2.value.length > 64 )/* to fix bug 26920 */
		{
			alert("$invalid_answer");
			return false;
		}
		for(i=0; i< cf.answer1.value.length; i++)/* to fix bug 26920 */
		{
			if( isValidChar_space(cf.answer1.value.charCodeAt(i))==false )
			{
				alert("$invalid_answer");
				return false;
			}
		}
		for(i=0; i< cf.answer2.value.length; i++)/* to fix bug 26920 */
		{
			if( isValidChar_space(cf.answer2.value.charCodeAt(i))==false )
			{
				alert("$invalid_answer");
				return false;
			}
		}
	}
	else
	{
		if(cf.sysOldPasswd.value != "")
		{
			if(confirm("$pwd_recovery_to_default \n$are_you_sure_not_enable_recovery") == false)
				return false;
		}

		cf.hidden_enable_recovery.value="0";
	}

	cf.submit();

	return true;

}
