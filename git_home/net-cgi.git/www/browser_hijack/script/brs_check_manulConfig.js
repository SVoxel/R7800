function loadValue()
{
	var forms = document.getElementsByTagName("form");
	var cf = forms[0];
	
	/*if(checking_result == "failed") //failed
	{
		if( toBasic == "1" )
			goto_home_page();
		else
		{	
			//bug 22037
			cf.action = "/apply.cgi?/welcomeok.htm timestamp=" + ts;
			cf.submit_flag.value = "hijack_toStatus";
			cf.submit();
		}
	}
	else if(checking_result == "success") //success */
	cf.submit();
}

addLoadEvent(loadValue);

