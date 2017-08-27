function initPage()
{
	//head text
	var head_tag = document.getElementsByTagName("h1");
	var head_text = document.createTextNode(bh_apply_connection);
	head_tag[0].appendChild(head_text);
	
	//paragraph
	var paragraph = document.getElementsByTagName("p");
	
	var paragraph_text = document.createTextNode(bh_plz_waite_apply_connection);
	paragraph[0].appendChild(paragraph_text);

	//show Fireware Version
	showFirmVersion("none");

	loadValue();
}

function loadValue()
{
   if(top.dsl_enable_flag == 1)
   {
	if(select_basic == "0" && select_type == "7")//pppoe quick
	{
	   if(country == "Germany" && isp == "Deutsche Telekom")
	   {
		
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_vdsl_Gemany_pppoe_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";

	   }
	   else  if(country == "Germany" && isp == "1&1")
	   {
		
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_vdsl_Gemany11_pppoe_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";

	   }
	   else{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_vdsl_pppoe_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
          }
	}
	else if(select_basic == "0" && select_type == "5")//pppoa
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_pppoa_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
	else if(select_basic == "0" && select_type == "0")//pppoe
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_pppoe_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
	else if(select_basic == "0" && select_type == "1")//pptp
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_C_pptp_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
	else if(select_basic == "0" && select_type == "4")//l2tp
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_F_l2tp_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
   
   }else{
	if(select_basic == "0" && select_type == "0")//pppoe
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_B_pppoe_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
	else if(select_basic == "0" && select_type == "1")//pptp
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_C_pptp_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
	else if(select_basic == "0" && select_type == "4")//l2tp
	{
		if(wget_result == "failed")
			this.location.href = "BRS_03A_F_l2tp_reenter.html";
		else if(wget_result == "success")
			this.location.href = "BRS_security.html";
	}
   }
}

addLoadEvent(initPage);
