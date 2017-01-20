function show_idleout(cf)
{
	if( cf.conn_modetype[0].selected == true)
		document.getElementById("tim").disabled =true;

	else
		document.getElementById("tim").disabled =false;
		
}

function click_status()
{
	window.open('mobile_conn_status.htm','mobile_connect_status','width=625,height=600,top=50,left=200,status=yes,resizable=yes');
}

function click_detect_install()
{
	var cf=document.forms[0];
	if(cf.use_file.checked==true)
	{
		if (cf.choose_file.value.length == 0)
		{
			alert("$filename_null");
			return false;
		}
		cf.action="/mobile_install.cgi";
		cf.enctype="multipart/form-data";
		cf.submit();
	}
	else
	{
		cf.submit_flag.value="mobile_detect_install";
		cf.action="/apply.cgi?/BAS_mobile.htm timestamp="+ts;
		cf.submit();
	}
	return true;
}

function change_mode()
{
	var cf=document.forms[0];
	if(cf.conn_mode.value=="ethonly")
	{
		document.getElementById("btn_0").style.display="";
		document.getElementById("btn_1").style.display="none";
		document.getElementById("mobile_tr").style.display="none";
		document.getElementById("mobile_tr_1").style.display="none";
		document.getElementById("mobile_tr_2").style.display="none";
		document.getElementById("bluebar_tr").style.display="none";
		document.getElementById("bluebar_tr_1").style.display="none";
		document.getElementById("bluebar_tr_2").style.display="none";
		document.getElementById("3g_conn_mode").style.display="none";
		document.getElementById("3g_conn_mode_1").style.display="none";
	}
	else
	{
		document.getElementById("btn_0").style.display="none";
		document.getElementById("btn_1").style.display="";
		document.getElementById("bluebar_tr").style.display="";
		document.getElementById("mobile_tr").style.display="";
		
		isp_display();
		if(cf.conn_mode.value=="3g")
		{
			document.getElementById("bluebar_tr_2").style.display="";
			document.getElementById("3g_conn_mode").style.display="";
			document.getElementById("3g_conn_mode_1").style.display="";
		}
	/*	if(cf.conn_mode.value=="failover")
		{
			document.getElementById("bluebar_tr_2").style.display="none";
			document.getElementById("3g_conn_mode").style.display="none";
			document.getElementById("3g_conn_mode_1").style.display="none";
		}*/
		//if(install_status=='0')
		//{
			//cf.use_file.disabled=true;
			//cf.choose_file.disabled=true;
			//cf.detect_file.disabled=true;
			cf.Apply_1.disabled=false;
			//document.getElementById("detect_file").className="common_key_gray_bt";
			document.getElementById("Apply_1").className="apply_bt";
			setDisabled(false,cf.mobile_login,cf.mobile_passwd,cf.mobile_access_num,cf.mobile_apn,cf.mobile_pdptype);
			setDisabled(false,cf.WRegion,cf.mobile_pincode);
			for(i=1;i<=21;i++)
				document.getElementsByName("isp_type"+i)[0].disabled=false;
		//}
		/*else
		{
			cf.detect_file.disabled=false;
			cf.Apply_1.disabled=true;
			document.getElementById("detect_file").className="common_key_bt";
			document.getElementById("Apply_1").className="common_gray_bt";
			setDisabled(true,cf.mobile_login,cf.mobile_passwd,cf.mobile_access_num,cf.mobile_apn,cf.mobile_pdptype);
			setDisabled(true,cf.WRegion,cf.mobile_pincode,cf.mobile_conn_mode[0],cf.mobile_conn_mode[1]);
			for(i=1;i<=21;i++)
				document.getElementsByName("isp_type"+i)[0].disabled=true;
			if(install_status=='1' || install_status=='3')
			{
				cf.use_file.disabled=true;
				cf.choose_file.disabled=true;
			}
			else
			{
				cf.use_file.disabled=false;
				cf.choose_file.disabled=false;
			}
		}*/
	}
}

function isp_display()
{
	var cf=document.forms[0];
	var index = cf.WRegion.selectedIndex;
	var i;
	for(i=0;i<21;i++)
	{
		if(i == index)
			document.getElementById("coun"+i).style.display="";
		else
			document.getElementById("coun"+i).style.display="none";
	}
	isp_apn();
}

function choose_other()
{
	document.getElementById("mobile_tr_1").style.display="";
	document.getElementById("bluebar_tr_1").style.display="";
	document.getElementById("mobile_tr_2").style.display="none";
	document.forms[0].puk_code.value = wan_mobile_puk_code;
	document.forms[0].mobile_access_num.selectedIndex="0";
}

function check_default()
{
	var cf = document.forms[0];
	var isp_type_tr = document.getElementById("coun"+wan_mobile_region);
	var isp_type = isp_type_tr.getElementsByTagName("select")[0];
	if(cf.WRegion.selectedIndex == wan_mobile_region && isp_type.value == wan_mobile_isp){
		cf.mobile_login.value=wan_mobile_login;
		cf.mobile_passwd.value=wan_mobile_password;
		cf.mobile_pincode.value=wan_mobile_pincode;
		cf.mobile_access_num.selectedIndex=wan_mobile_access_num;
		cf.mobile_apn.value=wan_mobile_apn;
		cf.mobile_pdptype.value=wan_mobile_pdp_type;
	}
}

function isp_apn()
{
	var cf=document.forms[0];
	document.getElementById("mobile_tr_1").style.display="none";
	document.getElementById("bluebar_tr_1").style.display="none";
	
	cf.mobile_login.value="";
	cf.mobile_passwd.value="";
	cf.mobile_apn.value="";
	cf.mobile_access_num.selectedIndex="0";
	cf.mobile_pdptype.selectedIndex="0";
	var index = cf.WRegion.selectedIndex;
	var isp_coun = index + 1;
	if(mobile_status == "3" && document.getElementsByName("isp_type"+isp_coun)[0].value != "Other"){
		document.getElementById("mobile_tr_2").style.display="";
		document.getElementById("bluebar_tr_1").style.display="";
		document.getElementById("mobile_tr_1").style.display="none";
		cf.pin_error_puk_code.value = wan_mobile_puk_code;
        }
	if(index == 0)
	{
		if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 7 || cf.isp_type1.selectedIndex == 8 || cf.isp_type1.selectedIndex == 9 || cf.isp_type1.selectedIndex == 11 || cf.isp_type1.selectedIndex == 13){
			cf.mobile_access_num.value=0;
			cf.hid_access_number.value="*99#";
		}
		else if(cf.isp_type1.selectedIndex == 3){
			cf.mobile_access_num.selectedIndex=3;
			cf.hid_access_number.value="*99***3#";
		}
		else if(cf.isp_type1.selectedIndex == 5){
			cf.mobile_access_num.selectedIndex=2;
			cf.hid_access_number.value="*99***2#";
		}
		else{
			cf.mobile_access_num.selectedIndex=1;
			cf.hid_access_number.value="*99***1#";
		}
		if(cf.isp_type1.selectedIndex == 0)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type1.selectedIndex == 1 || cf.isp_type1.selectedIndex == 14)
			cf.mobile_apn.value="connect";
		else if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 3)
			cf.mobile_apn.value="telstra.internet";
		else if(cf.isp_type1.selectedIndex == 4)
			cf.mobile_apn.value="telstra.pcpack";
		else if(cf.isp_type1.selectedIndex == 5)
			cf.mobile_apn.value="telstra.datapack";
		else if(cf.isp_type1.selectedIndex == 6)
		{
			cf.mobile_apn.value="telstra.bigpond";
			cf.mobile_login.value="User";
			cf.mobile_passwd.value="password";
		}
		else if(cf.isp_type1.selectedIndex == 7)
			cf.mobile_apn.value="3netaccess";
		else if(cf.isp_type1.selectedIndex == 8 || cf.isp_type1.selectedIndex == 15)
			cf.mobile_apn.value="3services";
		else if(cf.isp_type1.selectedIndex == 9)
			cf.mobile_apn.value="VirginBroadband";
		else if(cf.isp_type1.selectedIndex == 10)
			cf.mobile_apn.value="vfinternet.au";
		else if(cf.isp_type1.selectedIndex == 11)
			cf.mobile_apn.value="WirelessBroadband";
		else if(cf.isp_type1.selectedIndex == 12)
			cf.mobile_apn.value="dodolns1";
		else if(cf.isp_type1.selectedIndex == 13)
			cf.mobile_apn.value="primuslns1";
		else if(cf.isp_type1.selectedIndex == 16)
			cf.mobile_apn.value="splns555a1";
		else
			choose_other();
	}
	else if(index == 1)
	{
		cf.mobile_access_num.selectedIndex=0;
		cf.hid_access_number.value="*99#";
		if(cf.isp_type2.selectedIndex == 0)
		{
			cf.mobile_apn.value="A1.net";
			cf.mobile_login.value="ppp@A1plus.at";
			cf.mobile_passwd.value="ppp";
		}
		else if(cf.isp_type2.selectedIndex == 1)
		{
			cf.mobile_apn.value="gprsinternet";
			cf.mobile_login.value="gprs";
		}
		else if(cf.isp_type2.selectedIndex == 2)
		{
			cf.mobile_apn.value="web.one.at";
			cf.mobile_login.value="web";
			cf.mobile_passwd.value="web";
		}
		else if(cf.isp_type2.selectedIndex == 3)
			cf.mobile_apn.value="drei.at";
		else if(cf.isp_type2.selectedIndex == 4)
		{
			cf.mobile_apn.value="web";
			cf.mobile_login.value="web@telering.at";
			cf.mobile_passwd.value="web";
		}
		else if(cf.isp_type2.selectedIndex == 5)
			cf.mobile_apn.value="web";
		else
			choose_other();
	}
	else if(index == 2)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type3.selectedIndex == 0)
			cf.mobile_apn.value="gprs.base.be";
		else if(cf.isp_type3.selectedIndex == 1)
			cf.mobile_apn.value="internet.be";
		else if(cf.isp_type3.selectedIndex == 2)
			cf.mobile_apn.value="web.pro.be";
		else if(cf.isp_type3.selectedIndex == 3)
			cf.mobile_apn.value="orangeinternet";
		else if(cf.isp_type3.selectedIndex == 4)
			cf.mobile_apn.value="internet.proximus.be";
		else
			choose_other();
	}
	else if(index == 3)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type4.selectedIndex == 0)
			cf.mobile_apn.value="claro.com.br";
		else if(cf.isp_type4.selectedIndex == 1)
			cf.mobile_apn.value="gprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 2)
			cf.mobile_apn.value="wapgprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 3)
			cf.mobile_apn.value="tim.br";
		else if(cf.isp_type4.selectedIndex == 4)
			cf.mobile_apn.value="wap.telcel.com";
		else
			choose_other();
	}
	else if(index == 4)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type5.selectedIndex == 0)
			cf.mobile_apn.value="wap.clarochile.cl";
		else if(cf.isp_type5.selectedIndex == 1)
			cf.mobile_apn.value="imovil.entelpcs.cl";
		else if(cf.isp_type5.selectedIndex == 2)
			cf.mobile_apn.value="wap.tmovil.cl";
		else if(cf.isp_type5.selectedIndex == 3)
			cf.mobile_apn.value="web.tmovil.cl";
		else 
			choose_other();
	}
	else if(index == 5)
	{
		if(cf.isp_type6.selectedIndex == 0)
		{
			cf.mobile_apn.value="cmnet";
			cf.mobile_access_num.selectedIndex=5;
			cf.hid_access_number.value="*98*1#";
		}
		else if(cf.isp_type6.selectedIndex == 1)
		{
			cf.mobile_apn.value="3gnet";
			cf.mobile_access_num.selectedIndex=0;
			cf.hid_access_number.value="*99#";
		}
		else if(cf.isp_type6.selectedIndex == 2)
		{
			cf.mobile_login.value="ctnet@mycdma.cn";
			cf.mobile_passwd.value="vnet.mobi";
			cf.mobile_access_num.selectedIndex=6;
			cf.hid_access_number.value="#777";
		}
		else
			choose_other()
	}
	else if(index == 6)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type7.selectedIndex == 0 || cf.isp_type7.selectedIndex == 1  || cf.isp_type7.selectedIndex == 3)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type7.selectedIndex == 2)
			cf.mobile_apn.value="internet.saunalahti";
		else if(cf.isp_type7.selectedIndex == 4)
			cf.mobile_apn.value="internet.song.fi";
		else
			choose_other();
	}
	else if(index == 7)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type8.selectedIndex == 0)
			cf.mobile_apn.value="surfo2";
		else if(cf.isp_type8.selectedIndex == 1)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type8.selectedIndex == 2)
		{
			cf.mobile_apn.value="tagesflat.eplus.de";
			cf.mobile_login.value="eplus";
			cf.mobile_passwd.value="gprs";
		}
		else if(cf.isp_type8.selectedIndex == 3)
		{
			cf.mobile_apn.value="internet.t-mobile";
			cf.mobile_login.value="t-mobile";
			cf.mobile_passwd.value="tm";
		}
		else if(cf.isp_type8.selectedIndex == 4)
			cf.mobile_apn.value="web.vodafone.de";
		else
			choose_other();
	}
	else if(index == 8)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type9.selectedIndex == 0 || cf.isp_type9.selectedIndex == 1 || cf.isp_type9.selectedIndex == 3 ||cf.isp_type9.selectedIndex == 4 || cf.isp_type9.selectedIndex == 5)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type9.selectedIndex == 2)
			cf.mobile_apn.value="web.orangehk.com";
		else if(cf.isp_type9.selectedIndex == 6)
			cf.mobile_apn.value="mobile.three.com.hk";
		else
			choose_other();
	}
	else if(index == 9)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type10.selectedIndex == 0)
			cf.mobile_apn.value="internet.postemobile.it";
		else if(cf.isp_type10.selectedIndex == 1)
			cf.mobile_apn.value="tre.it";
		else if(cf.isp_type10.selectedIndex == 2)
			cf.mobile_apn.value="ibox.tim.it";
		else if(cf.isp_type10.selectedIndex == 3)
			cf.mobile_apn.value="web.omnitel.it";
		else if(cf.isp_type10.selectedIndex == 4)
			cf.mobile_apn.value="internet.wind";
		else
			choose_other();
	}
	else if(index == 10)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type11.selectedIndex == 0)
			cf.mobile_apn.value="portalmmm.nl";
		else if(cf.isp_type11.selectedIndex == 1 || cf.isp_type11.selectedIndex == 2 || cf.isp_type11.selectedIndex == 3 || cf.isp_type11.selectedIndex == 4)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type11.selectedIndex == 5)
			cf.mobile_apn.value="live.vodafone.com";
		else if(cf.isp_type11.selectedIndex == 6)
			cf.mobile_apn.value="office.vodafone.nl";
		else
			choose_other();
	}
	else if(index == 11)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type12.selectedIndex == 0)
			cf.mobile_apn.value="live.vodafone.com";
		else if(cf.isp_type12.selectedIndex == 1)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type12.selectedIndex == 2)
			cf.mobile_apn.value="www.vodafone.net.nz";
		else
			choose_other();
	}
	else if(index == 12)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type13.selectedIndex == 0 || cf.isp_type13.selectedIndex == 1)
			cf.mobile_apn.value="internet.netcom.no";
		else if(cf.isp_type13.selectedIndex == 2)
			cf.mobile_apn.value="internet.telenor.no";
		else if(cf.isp_type13.selectedIndex == 3)
			cf.mobile_apn.value="internet.ventelo.no";
		else
			choose_other();
	}
	else if(index == 13)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type14.selectedIndex == 0)
			cf.mobile_apn.value="tim.pe";
		else
			choose_other();
	}
	else if(index == 14)
	{
		cf.mobile_access_num.selectedIndex=0;
		cf.hid_access_number.value="*99#";
		if(cf.isp_type15.selectedIndex == 0)
		{
			cf.mobile_apn.value="internet.mts.ru";
			cf.mobile_login.value="mts";
			cf.mobile_passwd.value="mts";
		}
		else if(cf.isp_type15.selectedIndex == 1)
		{
			cf.mobile_apn.value="internet";
			cf.mobile_login.value="gdata";
			cf.mobile_passwd.value="gdata";
		}
		else if(cf.isp_type15.selectedIndex == 2)
		{
			cf.mobile_apn.value="internet.beeline.ru";
			cf.mobile_login.value="beeline";
			cf.mobile_passwd.value="beeline";
		}
		else if(cf.isp_type15.selectedIndex == 3)
			cf.mobile_apn.value="internet.TELE2.ru";
		else if(cf.isp_type15.selectedIndex == 4)
		{
			cf.mobile_login.value="mobile";
			cf.mobile_passwd.value="internet";
			cf.mobile_access_num.selectedIndex=6;
			cf.hid_access_number.value="#777";
		}
	}
	else if(index == 15)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type16.selectedIndex == 0)
			cf.mobile_apn.value="sunsurf";
		else if(cf.isp_type16.selectedIndex == 1)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type16.selectedIndex == 2)
			cf.mobile_apn.value="shwap";
		else
			choose_other();
	}
	else if(index == 16)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type17.selectedIndex == 0 || cf.isp_type17.selectedIndex == 1 ||cf.isp_type17.selectedIndex == 3)
			cf.mobile_apn.value="internet";
		else if(cf.isp_type17.selectedIndex == 2)
			cf.mobile_apn.value="vdata";
		else
			choose_other();
	}
	else if(index == 17)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type18.selectedIndex == 0 || cf.isp_type18.selectedIndex == 1)
			cf.mobile_apn.value="internet.tele2.se";
		else if(cf.isp_type18.selectedIndex == 2)
			cf.mobile_apn.value="internet.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 3)
			cf.mobile_apn.value="services.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 4)
			cf.mobile_apn.value="online.telia.se";
		else if(cf.isp_type18.selectedIndex == 5)
			cf.mobile_apn.value="internet.djuice.se";
		else
			choose_other();
	}
	else if(index == 18)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		cf.mobile_apn.value="internet";
		if(cf.isp_type19.selectedIndex == 7 )
			choose_other();
	}
	else if(index == 19)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type20.selectedIndex == 0)
			cf.mobile_apn.value="three.co.uk";
		else if(cf.isp_type20.selectedIndex == 1)
			cf.mobile_apn.value="3internet";
		else if(cf.isp_type20.selectedIndex == 2)
			cf.mobile_apn.value="airtel-ci-gprs.com";
		else if(cf.isp_type20.selectedIndex == 3)
			cf.mobile_apn.value="pepper";
		else if(cf.isp_type20.selectedIndex == 4)
		{
			cf.mobile_apn.value="wap.o2.co.uk";
			cf.mobile_login.value="o2wap";
			cf.mobile_passwd.value="password";
		}
		else if(cf.isp_type20.selectedIndex == 5)
		{
			cf.mobile_apn.value="mobile.o2.co.uk";
			cf.mobile_login.value="faster";
			cf.mobile_passwd.value="web";
		}
		else if(cf.isp_type20.selectedIndex == 6)
		{
			cf.mobile_apn.value="bb-m.o2.co.uk";
			cf.mobile_login.value="o2bb";
			cf.mobile_passwd.value="password";
		}
		else if(cf.isp_type20.selectedIndex == 7)
			cf.mobile_apn.value="payandgo.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 8 || cf.isp_type20.selectedIndex == 9)
			cf.mobile_apn.value="orangeinternet";
		else if(cf.isp_type20.selectedIndex == 10)
			cf.mobile_apn.value="general.t-mobile.uk";
		else if(cf.isp_type20.selectedIndex == 11)
			cf.mobile_apn.value="prepay.tesco-mobile.com";
		else if(cf.isp_type20.selectedIndex == 12)
			cf.mobile_apn.value="goto.virginmobile.uk";
		else if(cf.isp_type20.selectedIndex == 13)
		{
			cf.mobile_apn.value="internet";
			cf.mobile_login.value="web";
			cf.mobile_passwd.value="web";
		}
		else if(cf.isp_type20.selectedIndex == 14)
			cf.mobile_apn.value="pp.vodafone.co.uk";
		else
			choose_other();
	}
	else if(index == 20)
	{
		cf.mobile_access_num.selectedIndex=1;
		cf.hid_access_number.value="*99***1#";
		if(cf.isp_type21.selectedIndex == 0 || cf.isp_type21.selectedIndex == 2)
			cf.mobile_apn.value="isp.cingular";
		else if(cf.isp_type21.selectedIndex == 1)
			cf.mobile_apn.value="internet";
		else
			choose_other();
	}
	check_default();
}

function check_mobile_conn(cf)
{
	var isp_coun=cf.WRegion.selectedIndex+1;
	cf.hid_mobile_isp.value=document.getElementsByName("isp_type"+isp_coun)[0].value;
	if(mobile_status == "3" && cf.hid_mobile_isp.value != "Other")
		cf.hid_puk_code.value = cf.pin_error_puk_code.value;
	else
		cf.hid_puk_code.value = cf.puk_code.value;
	return true;
}
