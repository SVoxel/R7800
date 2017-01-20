function choose_other()
{
	document.getElementById("3g_pppoe_login").style.display="";
	document.getElementById("3g_pppoe_pwd").style.display="";
}

function check_default()
{
	var cf = document.forms[0];
	var isp_type_tr = document.getElementById("coun"+wan_cdma_region);
	var isp_type = isp_type_tr.getElementsByTagName("select")[0];
	if(cf.WRegion.selectedIndex == wan_cdma_region && isp_type.value == wan_cdma_isp){
		cf.G3g_username.value=wan_cdma_username;
		cf.G3g_passwd.value=wan_cdma_password;
		cf.usb_3g_pincode.value=wan_cdma_pincode;
		cf.access_num.selectedIndex=wan_cdma_access_num;
		cf.hsdpa_apn.value=wan_cdma_apn;
		cf.pdptype.selectedIndex=wan_cdma_pdp_type;
	}
}

function isp_apn()
{
	var cf=document.forms[0];
	var index = cf.WRegion.selectedIndex;
	document.getElementById("3g_pppoe_login").style.display="none";
	document.getElementById("3g_pppoe_pwd").style.display="none";
	document.getElementById("evdo_apn").style.display="";
	cf.G3g_username.value="";
	cf.G3g_passwd.value="";
	cf.hsdpa_apn.value="";
	cf.usb_3g_pincode.value="";
	cf.access_num.selectedIndex="0";
	cf.pdptype.selectedIndex="0";
	if(index == 0)
	{
		if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 7 || cf.isp_type1.selectedIndex == 8 || cf.isp_type1.selectedIndex == 9 || cf.isp_type1.selectedIndex == 11 || cf.isp_type1.selectedIndex == 13)
			cf.access_num.selectedIndex=0;
		else if(cf.isp_type1.selectedIndex == 3)
			cf.access_num.selectedIndex=3;
		else if(cf.isp_type1.selectedIndex == 5)
			cf.access_num.selectedIndex=2;
		else
			cf.access_num.selectedIndex=1;
		
		if(cf.isp_type1.selectedIndex == 0)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type1.selectedIndex == 1 || cf.isp_type1.selectedIndex == 14)
			cf.hsdpa_apn.value="connect";
		else if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 3)
			cf.hsdpa_apn.value="telstra.internet";
		else if(cf.isp_type1.selectedIndex == 4)
			cf.hsdpa_apn.value="telstra.pcpack";
		else if(cf.isp_type1.selectedIndex == 5)
			cf.hsdpa_apn.value="telstra.datapack";
		else if(cf.isp_type1.selectedIndex == 6)
			cf.hsdpa_apn.value="telstra.bigpond";
		else if(cf.isp_type1.selectedIndex == 7)
			cf.hsdpa_apn.value="3netaccess";
		else if(cf.isp_type1.selectedIndex == 8)
			cf.hsdpa_apn.value="3services";
		else if(cf.isp_type1.selectedIndex == 9)
			cf.hsdpa_apn.value="VirginBroadband";
		else if(cf.isp_type1.selectedIndex == 10)
			cf.hsdpa_apn.value="vfinternet.au";
		else if(cf.isp_type1.selectedIndex == 11)
			cf.hsdpa_apn.value="WirelessBroadband";
		else if(cf.isp_type1.selectedIndex == 12)
			cf.hsdpa_apn.value="dodolns1";
		else if(cf.isp_type1.selectedIndex == 13)
			cf.hsdpa_apn.value="primuslns1";
		else if(cf.isp_type1.selectedIndex == 15)
			cf.hsdpa_apn.value="3services";
		else if(cf.isp_type1.selectedIndex == 16)
			cf.hsdpa_apn.value="splns555a1";
		else
			choose_other();
	}
	else if(index == 1)
	{
		cf.access_num.selectedIndex=0;
		if(cf.isp_type2.selectedIndex == 0)
			cf.hsdpa_apn.value="A1.net";
		else if(cf.isp_type2.selectedIndex == 1)
			cf.hsdpa_apn.value="gprsinternet";
		else if(cf.isp_type2.selectedIndex == 2)
			cf.hsdpa_apn.value="web.one.at";
		else if(cf.isp_type2.selectedIndex == 3)
			cf.hsdpa_apn.value="drei.at";
		else if(cf.isp_type2.selectedIndex == 4 || cf.isp_type2.selectedIndex == 5)
			cf.hsdpa_apn.value="web";
		else
			choose_other();
	}
	else if(index == 2)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type3.selectedIndex == 0)
			cf.hsdpa_apn.value="gprs.base.be";
		else if(cf.isp_type3.selectedIndex == 1)
			cf.hsdpa_apn.value="internet.be";
		else if(cf.isp_type3.selectedIndex == 2)
			cf.hsdpa_apn.value="web.pro.be";
		else if(cf.isp_type3.selectedIndex == 3)
			cf.hsdpa_apn.value="orangeinternet";
		else if(cf.isp_type3.selectedIndex == 4)
			cf.hsdpa_apn.value="internet.proximus.be";
		else
			choose_other();
	}
	else if(index == 3)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type4.selectedIndex == 0)
			cf.hsdpa_apn.value="claro.com.br";
		else if(cf.isp_type4.selectedIndex == 1)
			cf.hsdpa_apn.value="gprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 2)
			cf.hsdpa_apn.value="wapgprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 3)
			cf.hsdpa_apn.value="tim.br";
		else if(cf.isp_type4.selectedIndex == 4)
			cf.hsdpa_apn.value="wap.telcel.com";
		else
			choose_other();
	}
	else if(index == 4)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type5.selectedIndex == 0)
			cf.hsdpa_apn.value="wap.clarochile.cl";
		else if(cf.isp_type5.selectedIndex == 1)
			cf.hsdpa_apn.value="imovil.entelpcs.cl";
		else if(cf.isp_type5.selectedIndex == 2)
			cf.hsdpa_apn.value="wap.tmovil.cl";
		else if(cf.isp_type5.selectedIndex == 3)
			cf.hsdpa_apn.value="wep.tmovil.cl";
		else
			choose_other();
	}
	else if(index == 5)
	{
		if(cf.isp_type6.selectedIndex == 0){
			document.getElementById("3g_pppoe_login").style.display="none";
			document.getElementById("3g_pppoe_pwd").style.display="none";
			document.getElementById("evdo_apn").style.display="";
			cf.hsdpa_apn.value="cmnet";
			cf.access_num.selectedIndex=5;
		}
		else if(cf.isp_type6.selectedIndex == 1){
			document.getElementById("3g_pppoe_login").style.display="none";
			document.getElementById("3g_pppoe_pwd").style.display="none";
			document.getElementById("evdo_apn").style.display="";
			cf.hsdpa_apn.value="3gnet";
			cf.access_num.selectedIndex=0;
		}
		else if(cf.isp_type6.selectedIndex == 2){
			document.getElementById("3g_pppoe_login").style.display="";
			document.getElementById("3g_pppoe_pwd").style.display="";
			document.getElementById("evdo_apn").style.display="none"
			cf.G3g_username.value="ctnet@mycdma.cn";
			cf.G3g_passwd.value="vnet.mobi";
			cf.access_num.selectedIndex=6;
		}else
			choose_other();
	}
	else if(index == 6)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type7.selectedIndex == 0 || cf.isp_type7.selectedIndex == 1  || cf.isp_type7.selectedIndex == 3)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type7.selectedIndex == 2)
			cf.hsdpa_apn.value="internet.saunalahti";
		else if(cf.isp_type7.selectedIndex == 4)
			cf.hsdpa_apn.value="internet.song.fi";
		else
			choose_other();
	}
	else if(index == 7)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type8.selectedIndex == 0)
			cf.hsdpa_apn.value="surfo2";
		else if(cf.isp_type8.selectedIndex == 1)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type8.selectedIndex == 2)
			cf.hsdpa_apn.value="tagesflat.eplus.de";
		else if(cf.isp_type8.selectedIndex == 3)
			cf.hsdpa_apn.value="internet.t-mobile";
		else if(cf.isp_type8.selectedIndex == 4)
			cf.hsdpa_apn.value="web.vodafone.de";
		else
			choose_other();
	}
	else if(index == 8)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type9.selectedIndex == 0 || cf.isp_type9.selectedIndex == 1 || cf.isp_type9.selectedIndex == 3 ||cf.isp_type9.selectedIndex == 4 || cf.isp_type9.selectedIndex == 5)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type9.selectedIndex == 2)
			cf.hsdpa_apn.value="web.orangehk.com";
		else if(cf.isp_type9.selectedIndex == 6)
			cf.hsdpa_apn.value="mobile.three.com.hk";
		else
			choose_other();
	}
	else if(index == 9)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type10.selectedIndex == 0)
			cf.hsdpa_apn.value="internet.postemobile.it";
		else if(cf.isp_type10.selectedIndex == 1)
			cf.hsdpa_apn.value="tre.it";
		else if(cf.isp_type10.selectedIndex == 2)
			cf.hsdpa_apn.value="ibox.tim.it";
		else if(cf.isp_type10.selectedIndex == 3)
			cf.hsdpa_apn.value="web.omnitel.it";
		else if(cf.isp_type10.selectedIndex == 4)
			cf.hsdpa_apn.value="internet.wind";
		else
			choose_other();
	}
	else if(index == 10)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type11.selectedIndex == 0)
			cf.hsdpa_apn.value="portalmmm.nl";
		else if(cf.isp_type11.selectedIndex == 1 || cf.isp_type11.selectedIndex == 2 || cf.isp_type11.selectedIndex == 3 || cf.isp_type11.selectedIndex == 4)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type11.selectedIndex == 5)
			cf.hsdpa_apn.value="live.vodafone.com";
		else if(cf.isp_type11.selectedIndex == 6)
			cf.hsdpa_apn.value="office.vodafone.nl";
		else
			choose_other();
	}
	else if(index == 11)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type12.selectedIndex == 0)
			cf.hsdpa_apn.value="live.vodafone.com";
		else if(cf.isp_type12.selectedIndex == 1)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type12.selectedIndex == 2)
			cf.hsdpa_apn.value="www.vodafone.net.nz";
		else
			choose_other();
	}
	else if(index == 12)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type13.selectedIndex == 0 || cf.isp_type13.selectedIndex == 1)
			cf.hsdpa_apn.value="internet.netcom.no";
		else if(cf.isp_type13.selectedIndex == 2)
			cf.hsdpa_apn.value="internet.telenor.no";
		else if(cf.isp_type13.selectedIndex == 3)
			cf.hsdpa_apn.value="internet.ventelo.no";
		else
			choose_other();
	}
	else if(index == 13)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type14.selectedIndex == 0)
			cf.hsdpa_apn.value="tim.pe";
		else
			choose_other();
	}
	else if(index == 14)
	{
		cf.access_num.selectedIndex=0;
		if(cf.isp_type15.selectedIndex == 0)
		{
			document.getElementById("3g_pppoe_login").style.display="";
			document.getElementById("3g_pppoe_pwd").style.display="";
			document.getElementById("evdo_apn").style.display="";
			cf.G3g_username.value="mts";
			cf.G3g_passwd.value="mts";
			cf.hsdpa_apn.value="internet.mts.ru";
		}
		else if(cf.isp_type15.selectedIndex == 1)
		{
			document.getElementById("3g_pppoe_login").style.display="";
			document.getElementById("3g_pppoe_pwd").style.display="";
			document.getElementById("evdo_apn").style.display="";
			cf.G3g_username.value="gdata";
			cf.G3g_passwd.value="gdata";
			cf.hsdpa_apn.value="internet";
		}
		else if(cf.isp_type15.selectedIndex == 2)
		{
			document.getElementById("3g_pppoe_login").style.display="";
			document.getElementById("3g_pppoe_pwd").style.display="";
			document.getElementById("evdo_apn").style.display="";
			cf.G3g_username.value="beeline";
			cf.G3g_passwd.value="beeline";
			cf.hsdpa_apn.value="internet.beeline.ru";
		}
		else if(cf.isp_type15.selectedIndex == 3)
		{
			document.getElementById("3g_pppoe_login").style.display="none";
			document.getElementById("3g_pppoe_pwd").style.display="none";
			document.getElementById("evdo_apn").style.display="";
			cf.hsdpa_apn.value="internet.TELE2.ru";
		}
		else if(cf.isp_type15.selectedIndex == 4)
		{
			document.getElementById("3g_pppoe_login").style.display="";
			document.getElementById("3g_pppoe_pwd").style.display="";
			cf.G3g_username.value="mobile";
			cf.G3g_passwd.value="internet";
			document.getElementById("evdo_apn").style.display="none";
			cf.access_num.selectedIndex=6;
		}else
			choose_other();
	}
	else if(index == 15)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type16.selectedIndex == 0)
			cf.hsdpa_apn.value="sunsurf";
		else if(cf.isp_type16.selectedIndex == 1)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type16.selectedIndex == 2)
			cf.hsdpa_apn.value="shwap";
		else
			choose_other();
	}
	else if(index == 16)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type17.selectedIndex == 0 || cf.isp_type17.selectedIndex == 1 ||cf.isp_type17.selectedIndex == 3)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type17.selectedIndex == 2)
			cf.hsdpa_apn.value="vdata";
		else
			choose_other();
	}
	else if(index == 17)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type18.selectedIndex == 0 || cf.isp_type18.selectedIndex == 1)
			cf.hsdpa_apn.value="internet.tele2.se";
		else if(cf.isp_type18.selectedIndex == 2)
			cf.hsdpa_apn.value="internet.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 3)
			cf.hsdpa_apn.value="services.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 4)
			cf.hsdpa_apn.value="online.telia.se";
		else if(cf.isp_type18.selectedIndex == 5)
			cf.hsdpa_apn.value="internet.djuice.se";
		else
			choose_other();
	}
	else if(index == 18)
	{
		cf.access_num.selectedIndex=1;
		cf.hsdpa_apn.value="internet";
		if(cf.isp_type19.selectedIndex == 7 )
			choose_other();
	}
	else if(index == 19)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type20.selectedIndex == 0)
			cf.hsdpa_apn.value="three.co.uk";
		else if(cf.isp_type20.selectedIndex == 1)
			cf.hsdpa_apn.value="3internet";
		else if(cf.isp_type20.selectedIndex == 2)
			cf.hsdpa_apn.value="airtel-ci-gprs.com";
		else if(cf.isp_type20.selectedIndex == 3)
			cf.hsdpa_apn.value="pepper";
		else if(cf.isp_type20.selectedIndex == 4)
			cf.hsdpa_apn.value="wap.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 5)
			cf.hsdpa_apn.value="mobile.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 6)
			cf.hsdpa_apn.value="bb-m.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 7)
			cf.hsdpa_apn.value="payandgo.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 8 || cf.isp_type20.selectedIndex == 9)
			cf.hsdpa_apn.value="orangeinternet";
		else if(cf.isp_type20.selectedIndex == 10)
			cf.hsdpa_apn.value="general.t-mobile.uk";
		else if(cf.isp_type20.selectedIndex == 11)
			cf.hsdpa_apn.value="prepay.tesco-mobile.com";
		else if(cf.isp_type20.selectedIndex == 12)
			cf.hsdpa_apn.value="goto.virginmobile.uk";
		else if(cf.isp_type20.selectedIndex == 13)
			cf.hsdpa_apn.value="internet";
		else if(cf.isp_type20.selectedIndex == 14)
			cf.hsdpa_apn.value="pp.vodafone.co.uk";
		else
			choose_other();
	}
	else if(index == 20)
	{
		cf.access_num.selectedIndex=1;
		if(cf.isp_type21.selectedIndex == 0 || cf.isp_type21.selectedIndex == 2)
			cf.hsdpa_apn.value="isp.cingular";
		else if(cf.isp_type21.selectedIndex == 1)
			cf.hsdpa_apn.value="internet";
		else
			choose_other();
	}
	check_default();
}
