function check_tr069(form,kick)
{
	if(form.enable_tr069.checked == true)
		form.hidden_enable_tr069.value = "1";
	else
		form.hidden_enable_tr069.value = "0";
	
	if( kick == 1 )
		form.set_kick.value = "kick";
	else
		form.set_kick.value = "no"
	
	return true;

}
