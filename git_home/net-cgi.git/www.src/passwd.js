function change_answer1(obj)
{
	if(security_enhance_flag){
		if( answer1_empty != 0 && focus_trigger_flag1 == 0)
		{
			focus_trigger_flag1 = 1;
			if( get_browser() == "Firefox" )
			{		
				obj.value = "";
				obj.type = "text";
			}
			else
			{
				obj.outerHTML = '<input type="text" autocomplete="off" maxLength="64" size="30" name="answer1" id="answer1"  onFocus="this.select();change_answer1();" >';
				document.forms[0].answer1.select();
			}
		}
	}

}
function change_answer2(obj)
{
	if(security_enhance_flag){
		if( answer2_empty != 0 && focus_trigger_flag2 == 0)
		{
			focus_trigger_flag2 = 1;
			if( get_browser() == "Firefox" )
			{		
				obj.value = "";
				obj.type = "text";
			}
			else
			{
				obj.outerHTML = '<input type="text" autocomplete="off" maxLength="64" size="30" name="answer2" id="answer2"  onFocus="this.select();change_answer2();" >';
				document.forms[0].answer2.select();
			}
		}
	}
}

function validatepassword()
{
	var disabled="image/checkbox-selected-gry.svg";
	var enabled="image/checkbox-selected.svg";
	var meet_complex=0, meet_len=0,no_same=0;
	var cf = document.forms[0];
	var passwd=cf.sysNewPasswd.value;
	if(passwd == ""){
		cf.apply.disabled = true;
		cf.apply.className ="apply1_bt";
	}
	var re = new RegExp("[a-z]");
	var len=re.test(passwd);
	if(len){
		cf.pwd_complex2.src=enabled;
		meet_complex++;
	}else
		cf.pwd_complex2.src=disabled;
	re = new RegExp("[A-Z]");
	len=re.test(passwd);
	if(len)
	{
		cf.pwd_complex1.src=enabled;
		meet_complex++;
	}else
		cf.pwd_complex1.src=disabled;
	re = new RegExp("[0-9]");
	len=re.test(passwd);
	if(len)
	{
		cf.pwd_complex3.src=enabled;
		meet_complex++;
	}else
		cf.pwd_complex3.src=disabled;
	re = new RegExp("((?=[\x21-\x7e]+)[^A-Za-z0-9])");
	len=re.test(passwd);
	if(len)
	{
		cf.pwd_complex4.src=enabled;
		meet_complex++;
	}else
		cf.pwd_complex4.src=disabled;
	if(meet_complex>2)
		cf.pwd_complex.src=enabled;
	else
		cf.pwd_complex.src=disabled;
	if(passwd.length>=6 && passwd.length<=32)
	{
		cf.pwd_len.src=enabled;
		meet_len=1;
	}else
		cf.pwd_len.src=disabled;
	
	re = new RegExp("^.*(.)\\1{2}.*$");
	len=re.test(passwd);
	if(!len)
	{
		cf.pwd_identical.src=enabled;
		no_same=1;
	}else
		cf.pwd_identical.src=disabled;
	if(meet_complex>2 && meet_len && no_same){
		document.getElementById("passwd_hint").style.display="none";
		cf.pwd_warn.src="image/check.png";
	}else{
		document.getElementById("passwd_hint").style.display="";
		cf.pwd_warn.src="image/warning_flag.jpg";
	}
}
function change_display()
{
	if(security_enhance_flag)
	{
		document.forms[0].apply.disabled = "";
		document.getElementById("apply").className ="apply_bt";
		document.forms[0].pwd_warn.style.display="";
		document.getElementById("passwd_hint").style.display="none";
	}
}

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
	if(security_enhance_flag && cf.sysNewPasswd.value.length < 6)
	{
		alert("Your password must contain six or more characters.");
		return false;
	}
	if(security_enhance_flag && (cf.sysNewPasswd.value == "password"))
	{
		cf.apply.disabled = true;
		document.getElementById("apply").className ="apply_gray_bt";
		alert("$must_change_passwd");
		return false;
	}
	if(security_enhance_flag && cf.sysNewPasswd.value== "")
	{
		alert("You must enter a password");
		cf.apply.disabled = true;
		document.getElementById("apply").className ="apply_gray_bt";
		return false;
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

	cf.sysOldPasswd.value=$$.base64.encode(cf.sysOldPasswd.value);
	cf.sysNewPasswd.value=$$.base64.encode(cf.sysNewPasswd.value);
	if(security_enhance_flag){
	
		cf.hid_answer1_change.value=focus_trigger_flag1;
		cf.hid_answer2_change.value=focus_trigger_flag2;
	}
	cf.hid_answer1.value=$$.base64.encode(cf.answer1.value);
	cf.hid_answer2.value=$$.base64.encode(cf.answer2.value);
	cf.sysConfirmPasswd.value=cf.sysNewPasswd.value;
	cf.answer1.value="******";
	cf.answer2.value="******";
	cf.submit();

	return true;

}
