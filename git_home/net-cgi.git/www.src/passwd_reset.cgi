<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<% hijack_title() %>
<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
<link rel="stylesheet" type="text/css" href="style/hijack_style.css" />
<link rel="stylesheet" type="text/css" href="style/form.css" />
<link rel="stylesheet" type="text/css" href="style/top_style.css" />
<% hijack_language() %>
</head>

<style>
.wizard_body_container .wizard_words_div div ul{margin: 5px 0 5px 23px !important; padding:5px 0;}
#passwd_hint_content img{width:20px;height:20px;margin-left:-22px;}
.wizard_words_div span img{width:22px;height:21px;}
li {list-style-type:none;}
</style>

<body onload="loadvalue();">
<div id="page_container">
<div id="header">
	<div id="header_top">
		<div class="top_name">
			<div class="title_div"></div>
			<% cd_less_top_hardware_version() %>
		</div>
	</div>
</div>

<script>
var wait = 1000;
var timeoutID = null;
function loadvalue()
{
	document.getElementById("next").disabled = true;
	document.getElementById("next").className = "grey_short_btn";
	document.getElementById("pwd_warn").src="image/warning_flag.jpg";
	document.getElementById("passwd_hint_content").style.backgroundImage="url(../image/passwd_bg.jpg)";
}
function debounce() {
	clearTimeout(timeoutID);
	timeoutID = setTimeout(function() {
   	validatepassword();
  	}, wait);
};

function isValidChar_space(each_char)
{
	if( each_char < 32 || each_char > 127)
		return false;
}
function validatepassword()
{
	var disabled="image/checkbox-selected-gry.svg";
	var enabled="image/checkbox-selected.svg";
	var meet_complex=0, meet_len=0,no_same=0;
	var cf = document.forms[0];
	var passwd=cf.sysNewPasswd.value;
	if(passwd == ""){
		cf.next.disabled = true;
		document.getElementById("next").className = "grey_short_btn";
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
	document.forms[0].next.disabled = "";
	document.getElementById("next").className = "purple_short_btn";
	document.forms[0].pwd_warn.style.display="";
	document.getElementById("passwd_hint").style.display="none";
}
function checkpasswd(cf)
{
	if(cf.sysNewPasswd.value == "")
	{
		alert(bh_must_enter_passwd);
		return false;
	}
	if(cf.sysNewPasswd.value == "password")
	{
		alert(bh_must_change_passwd);
		return false;
	}
	if(cf.sysNewPasswd.value.length < 6)
	{
		alert("Your password must contain six or more characters.");
		return false;
	}
	if (cf.sysNewPasswd.value.length >= 33 || cf.sysConfirmPasswd.value.length >= 33)
	{
		alert(bh_max_pwd_len);
		return false;
	}
	if(cf.sysNewPasswd.value != cf.sysConfirmPasswd.value)
	{ 
		alert(bh_pwd_no_match);
		return false;	
	}

	cf.hidden_enable_recovery.value="1";
	if( cf.question1.value == "0" || cf.question2.value == "0")
	{
		alert(bh_select_quest);
		return false;
	}
	
	if( cf.answer1.value == "" || cf.answer2.value == "" )
	{
		alert(bh_enter_answer);
		return false;
	}
	if( cf.answer1.value.length > 64 || cf.answer2.value.length > 64 )/* to fix bug 26920 */
	{
		alert(bh_invalid_answer);
		return false;
	}
	
	for(i=0; i< cf.answer1.value.length; i++)/* to fix bug 26920 */
	{
		if( isValidChar_space(cf.answer1.value.charCodeAt(i))==false )
		{
			alert(bh_invalid_answer);
			return false;
		}
	}
	for(i=0; i< cf.answer2.value.length; i++)/* to fix bug 26920 */
	{
		if( isValidChar_space(cf.answer2.value.charCodeAt(i))==false )
		{
			alert(bh_invalid_answer);
			return false;
		}
	}
	cf.submit();
	return true;
}

</script>
<div class="wizard_body_container" style="overflow:auto;">
	<form method="post" action="/recover.cgi?/passwd_reset.cgi">
	<input type=hidden name=submit_flag value="passwd_reset">
	<input type="hidden" name="hidden_enable_recovery" value="1"/>

		<div class="wizard_words_div">
			
			<h2 id="message"><script>document.write(bh_password_reset);</script></h2>
			<div style="margin-bottom:50px;"><script>document.write(bh_verify_passwd_reset);</script></div>
			<div id="security">
			<table cellpadding="2" cellspacing="0" border="0">
			<TR>
				<TD nowrap><script>document.write(bh_pppoe_login_name);</script></TD>
				<TD nowrap><% cfg_get("http_loginname") %></TD>
			</TR>
			<TR>
				<TD nowrap><script>document.write(bh_new_password);</script></TD>
				<TD nowrap><input type="password" autocomplete="off" size="20" name="sysNewPasswd" id="sys_new_passwd" onFocus="this.select();" class="validate" style="vertical-align:bottom;" onkeyup="debounce()" onkeydown="change_display()" >
					<span style="vertical-align:bottom;"><img id="pwd_warn" src="" style="display:none;"></span>	
				</TD>
			</TR>

			<TR id="passwd_hint" class="passwd_hint" style="display:none;">
				<TD nowrap class="top0"></TD>
				<TD nowrap class="top0">
				<div id="passwd_hint_content" style="border-radius:20px;">
					<ul>
						<li>
							<span>$passwd_condition</span>
						</li>
						<li>
							<ul>
								<li>
									<img id="pwd_len" src="">
									<span>$password_reset_conditions5_1</span>
								</li>
								<li>
									<img id="pwd_identical" src="">
									<span>$password_reset_conditions6</span>
								</li>
							</ul>
						</li>
						<li>
							<img id="pwd_complex" src="">
							<span>$password_reset_tip3</span>
						</li>
						<li>
							<ul>
								<li>
									<img id="pwd_complex1" src="">
									<span>$password_reset_conditions1</span>
								</li>
								<li>
									<img id="pwd_complex2" src="">
									<span>$password_reset_conditions2</span>
								</li>
								<li>
									<img id="pwd_complex3" src="">
									<span>$password_reset_conditions3</span>
								</li>
								<li>
									<img id="pwd_complex4" src="">
									<span>$password_reset_conditions4</span>
								</li>
							</ul>
						</li>
					</ul>
				</div>
				</TD>
			</TR>
			<TR>
				<TD nowrap><script>document.write(bh_passwd_repas);</script></TD>
				<TD nowrap><input type="password" autocomplete="off" size="20" name="sysConfirmPasswd" id="sys_confirm_passwd" onFocus="this.select();" ></TD>
			</TR>
			<TR><TD nowrap colspan="2">&nbsp;</TD></TR>
			</table>
			<table cellpadding="2" cellspacing="0" border="0">
			<TR>
				<TD nowrap align="right"><script>document.write(bh_security_question_1+"*:");</script></TD>
				<TD nowrap align="left">
					<select  name="question1" id="question1" size=1 >
						<option value="0" selected><script>document.write(bh_select_a_question);</script></option>
						<option value="1"><script>document.write(bh_quest1_1);</script></option>
						<option value="2"><script>document.write(bh_quest1_2);</script></option>
                		<option value="3"><script>document.write(bh_quest1_3);</script></option>
                		<option value="4"><script>document.write(bh_quest1_4);</script></option>
                		<option value="5"><script>document.write(bh_quest1_5);</script></option>
                		<option value="6"><script>document.write(bh_quest1_6);</script></option>
                		<option value="7"><script>document.write(bh_quest1_7);</script></option>
                		<option value="8"><script>document.write(bh_quest1_8);</script></option>
                		<option value="9"><script>document.write(bh_quest1_9);</script></option>
					</select>
				</TD>
			</TR>
			<TR>
				<TD nowrap align="right"><script>document.write(bh_answer+"*:");</script></TD>
				<TD nowrap  align="left"><input type="text" autocomplete="off" maxLength="64" size="30" name="answer1" id="answer1"  onFocus="this.select();" ></TD>
			</TR>
			<TR>
				<TD nowrap align="right"><script>document.write(bh_security_question_2+"*:");</script></TD>
				<TD nowrap align="left">
					<select  name="question2" id="question2" size=1>
						<option value="0" selected><script>document.write(bh_select_a_question);</script></option>
                		<option value="1"><script>document.write(bh_quest2_1);</script></option>
                		<option value="2"><script>document.write(bh_quest2_2);</script></option>
                		<option value="3"><script>document.write(bh_quest2_3);</script></option>
                		<option value="4"><script>document.write(bh_quest2_4);</script></option>
                		<option value="5"><script>document.write(bh_quest2_5);</script></option>
                		<option value="6"><script>document.write(bh_quest2_6);</script></option>
                		<option value="7"><script>document.write(bh_quest2_7);</script></option>
                		<option value="8"><script>document.write(bh_quest2_8);</script></option>
					</select>
				</TD>
			</TR>
			<TR>
				<TD nowrap align="right"><script>document.write(bh_answer+"*:");</script></TD>
				<TD nowrap  align="left"><input type="text" autocomplete="off" maxLength="64" size="30" name="answer2" id="answer2" onFocus="this.select();" ></TD>
			</TR>
			<TR>
        		<TD nowrap colspan="2"><script>document.write(bh_required_information);</script></TD>
			</TR>
			</table>
			</div>
		</div>
	
		<div id="allBtns_div" class="wizard_buttons_div" align="right">
			<script>document.write('<input type="button" name="Next" id="next" class="purple_short_btn" value="'+bh_next_mark+'" onclick="checkpasswd(document.forms[0])">');</script> 
		</div>
	</form>
</div>
<div class="bottom_margin"></div>
</div>
</body>

</html>
