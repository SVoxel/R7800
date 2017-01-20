function set_other_isp()
{
	var cf=document.forms[0];
	if(cf.hsdpa_username.value == "")
	{
		alert("Ivalid value!");
		cf.hsdpa_username.focus();
		return false;
	}
	if(cf.hsdpa_passwd.value == "")
	{
		alert("Ivalid value!");
		cf.hsdpa_passwd.focus();
		return false;
	}
	if(cf.hsdpa_apn.value == "")
	{
		alert("Ivalid value!");
		cf.hsdpa_apn.focus();
		return false;
	}
	cf.hid_apn.value=cf.hsdpa_apn.value;
	cf.hid_accnum.value=cf.access_num.value;
}

function set_hsdpa()
{
	var cf=document.forms[0];
	var index = cf.WRegion.selectedIndex;
	cf.hid_region.value=index;
	if((index == 5 && (cf.isp_type6.selectedIndex != 0 && cf.isp_type6.selectedIndex != 1)) ||(index==14 && cf.isp_type15.selectedIndex != 3))
	{
		if(cf.hsdpa_username.value == "")
		{
			alert("Ivalid value!");
			cf.hsdpa_username.focus();
			return false;
		}
		if(cf.hsdpa_passwd.value == "")
		{
			alert("Ivalid value!");
			cf.hsdpa_passwd.focus();
			return false;
		}
	}

	if(index == 0)
	{
		if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 7 || cf.isp_type1.selectedIndex == 8 || cf.isp_type1.selectedIndex == 9 || cf.isp_type1.selectedIndex == 11 || cf.isp_type1.selectedIndex == 13)
			cf.hid_accnum.value=0;
		else if(cf.isp_type1.selectedIndex == 3)
			cf.hid_accnum.value=3;
		else if(cf.isp_type1.selectedIndex == 5)
			cf.hid_accnum.value=2;
		else
			cf.hid_accnum.value=1;
		
		cf.hid_isp.value=cf.isp_type1.value;
		if(cf.isp_type1.selectedIndex == 0)
			cf.hid_apn.value="internet";
		else if(cf.isp_type1.selectedIndex == 1 || cf.isp_type1.selectedIndex == 14)
			cf.hid_apn.value="connect";
		else if(cf.isp_type1.selectedIndex == 2 || cf.isp_type1.selectedIndex == 3)
			cf.hid_apn.value="telstra.internet";
		else if(cf.isp_type1.selectedIndex == 4)
			cf.hid_apn.value="telstra.pcpack";
		else if(cf.isp_type1.selectedIndex == 5)
			cf.hid_apn.value="telstra.datapack";
		else if(cf.isp_type1.selectedIndex == 6)
			cf.hid_apn.value="telstra.bigpond";
		else if(cf.isp_type1.selectedIndex == 7)
			cf.hid_apn.value="3netaccess";
		else if(cf.isp_type1.selectedIndex == 8)
			cf.hid_apn.value="3services";
		else if(cf.isp_type1.selectedIndex == 9)
			cf.hid_apn.value="VirginBroadband";
		else if(cf.isp_type1.selectedIndex == 10)
			cf.hid_apn.value="vfinternet.au";
		else if(cf.isp_type1.selectedIndex == 11)
			cf.hid_apn.value="WirelessBroadband";
		else if(cf.isp_type1.selectedIndex == 12)
			cf.hid_apn.value="dodolns1";
		else if(cf.isp_type1.selectedIndex == 13)
			cf.hid_apn.value="primuslns1";
		else if(cf.isp_type1.selectedIndex == 15)
			cf.hid_apn.value="3services";
		else if(cf.isp_type1.selectedIndex == 16)
			cf.hid_apn.value="splns555a1";
		else
			set_other_isp();
	}
	else if(index == 1)
	{
		cf.hid_accnum.value=0;
		cf.hid_isp.value=cf.isp_type2.value;
		if(cf.isp_type2.selectedIndex == 0)
			cf.hid_apn.value="A1.net";
		else if(cf.isp_type2.selectedIndex == 1)
			cf.hid_apn.value="gprsinternet";
		else if(cf.isp_type2.selectedIndex == 2)
			cf.hid_apn.value="web.one.at";
		else if(cf.isp_type2.selectedIndex == 3)
			cf.hid_apn.value="drei.at";
		else if(cf.isp_type2.selectedIndex == 4 || cf.isp_type2.selectedIndex == 5)
			cf.hid_apn.value="web";
		else
			set_other_isp();
	}
	else if(index == 2)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type3.value;
		if(cf.isp_type3.selectedIndex == 0)
			cf.hid_apn.value="gprs.base.be";
		else if(cf.isp_type3.selectedIndex == 1)
			cf.hid_apn.value="internet.be";
		else if(cf.isp_type3.selectedIndex == 2)
			cf.hid_apn.value="web.pro.be";
		else if(cf.isp_type3.selectedIndex == 3)
			cf.hid_apn.value="orangeinternet";
		else if(cf.isp_type3.selectedIndex == 4)
			cf.hid_apn.value="internet.proximus.be";
		else
			set_other_isp();
	}
	else if(index == 3)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type4.value;
		if(cf.isp_type4.selectedIndex == 0)
			cf.hid_apn.value="claro.com.br";
		else if(cf.isp_type4.selectedIndex == 1)
			cf.hid_apn.value="gprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 2)
			cf.hid_apn.value="wapgprs.oi.com.br";
		else if(cf.isp_type4.selectedIndex == 3)
			cf.hid_apn.value="tim.br";
		else if(cf.isp_type4.selectedIndex == 4)
			cf.hid_apn.value="wap.telcel.com";
		else
			set_other_isp();
	}
	else if(index == 4)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type5.value;
		if(cf.isp_type5.selectedIndex == 0)
			cf.hid_apn.value="wap.clarochile.cl";
		else if(cf.isp_type5.selectedIndex == 1)
			cf.hid_apn.value="imovil.entelpcs.cl";
		else if(cf.isp_type5.selectedIndex == 2)
			cf.hid_apn.value="wap.tmovil.cl";
		else if(cf.isp_type5.selectedIndex == 3)
			cf.hid_apn.value="wep.tmovil.cl";
		else
			set_other_isp();
	}
	else if(index == 5)
	{
		cf.hid_isp.value=cf.isp_type6.value;
		if(cf.isp_type6.selectedIndex == 0){
			cf.hid_apn.value="cmnet";
			cf.hid_accnum.value=5;
        	}
        	else if(cf.isp_type6.selectedIndex == 1){
			cf.hid_apn.value="3gnet";
			cf.hid_accnum.value=0;
		}
		else if(cf.isp_type6.selectedIndex == 2){
			cf.hid_apn.value="";
			cf.hid_accnum.value=6;
		}else
			set_other_isp();
	}
	else if(index == 6)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type7.value;
		if(cf.isp_type7.selectedIndex == 0 || cf.isp_type7.selectedIndex == 1  || cf.isp_type7.selectedIndex == 3)
			cf.hid_apn.value="internet";
		else if(cf.isp_type7.selectedIndex == 2)
			cf.hid_apn.value="internet.saunalahti";
		else if(cf.isp_type7.selectedIndex == 4)
			cf.hid_apn.value="internet.song.fi";
		else
			set_other_isp();
	}
	else if(index == 7)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type8.value;
		if(cf.isp_type8.selectedIndex == 0)
			cf.hid_apn.value="surfo2";
		else if(cf.isp_type8.selectedIndex == 1)
			cf.hid_apn.value="internet";
		else if(cf.isp_type8.selectedIndex == 2)
			cf.hid_apn.value="tagesflat.eplus.de";
		else if(cf.isp_type8.selectedIndex == 3)
			cf.hid_apn.value="internet.t-mobile";
		else if(cf.isp_type8.selectedIndex == 4)
			cf.hid_apn.value="web.vodafone.de";
		else
			set_other_isp();
	}
	else if(index == 8)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type9.value;
		if(cf.isp_type9.selectedIndex == 0 || cf.isp_type9.selectedIndex == 1 || cf.isp_type9.selectedIndex == 3 ||cf.isp_type9.selectedIndex == 4 || cf.isp_type9.selectedIndex == 5)
			cf.hid_apn.value="internet";
		else if(cf.isp_type9.selectedIndex == 2)
			cf.hid_apn.value="web.orangehk.com";
		else if(cf.isp_type9.selectedIndex == 6)
			cf.hid_apn.value="mobile.three.com.hk";
		else
			set_other_isp();
	}
	else if(index == 9)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type10.value;
		if(cf.isp_type10.selectedIndex == 0)
			cf.hid_apn.value="internet.postemobile.it";
		else if(cf.isp_type10.selectedIndex == 1)
			cf.hid_apn.value="tre.it";
		else if(cf.isp_type10.selectedIndex == 2)
			cf.hid_apn.value="ibox.tim.it";
		else if(cf.isp_type10.selectedIndex == 3)
			cf.hid_apn.value="web.omnitel.it";
		else if(cf.isp_type10.selectedIndex == 4)
			cf.hid_apn.value="internet.wind";
		else
			set_other_isp();
	}
	else if(index == 10)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type11.value;
		if(cf.isp_type11.selectedIndex == 0)
			cf.hid_apn.value="portalmmm.nl";
		else if(cf.isp_type11.selectedIndex == 1 || cf.isp_type11.selectedIndex == 2 || cf.isp_type11.selectedIndex == 3 || cf.isp_type11.selectedIndex == 4)
			cf.hid_apn.value="internet";
		else if(cf.isp_type11.selectedIndex == 5)
			cf.hid_apn.value="live.vodafone.com";
		else if(cf.isp_type11.selectedIndex == 6)
			cf.hid_apn.value="office.vodafone.nl";
		else
			set_other_isp();
	}
	else if(index == 11)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type12.value;
		if(cf.isp_type12.selectedIndex == 0)
			cf.hid_apn.value="live.vodafone.com";
		else if(cf.isp_type12.selectedIndex == 1)
			cf.hid_apn.value="internet";
		else if(cf.isp_type12.selectedIndex == 2)
			cf.hid_apn.value="www.vodafone.net.nz";
		else
			set_other_isp();
	}
	else if(index == 12)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type13.value;
		if(cf.isp_type13.selectedIndex == 0 || cf.isp_type13.selectedIndex == 1)
			cf.hid_apn.value="internet.netcom.no";
		else if(cf.isp_type13.selectedIndex == 2)
			cf.hid_apn.value="internet.telenor.no";
		else if(cf.isp_type13.selectedIndex == 3)
			cf.hid_apn.value="internet.ventelo.no";
		else
			set_other_isp();
	}
	else if(index == 13)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type14.value;
		if(cf.isp_type14.selectedIndex == 0)
			cf.hid_apn.value="tim.pe";
		else
			set_other_isp();
	}
	else if(index == 14)
	{
		cf.hid_accnum.value=0;
		cf.hid_isp.value=cf.isp_type15.value;
		if(cf.isp_type15.selectedIndex == 0)
			cf.hid_apn.value="internet.mts.ru";
		else if(cf.isp_type15.selectedIndex == 1)
			cf.hid_apn.value="internet";
		else if(cf.isp_type15.selectedIndex == 2)
			cf.hid_apn.value="internet.beeline.ru";
		else if(cf.isp_type15.selectedIndex == 3)
			cf.hid_apn.value="internet.TELE2.ru";
		else if(cf.isp_type15.selectedIndex == 4){
			cf.hid_apn.value="";
			cf.hid_accnum.value=6;
		}else
			set_other_isp();
			
	}
	else if(index == 15)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type15.value;
		if(cf.isp_type16.selectedIndex == 0)
			cf.hid_apn.value="sunsurf";
		else if(cf.isp_type16.selectedIndex == 1)
			cf.hid_apn.value="internet";
		else if(cf.isp_type16.selectedIndex == 2)
			cf.hid_apn.value="shwap";
		else
			set_other_isp();
	}
	else if(index == 16)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type17.value;
		if(cf.isp_type17.selectedIndex == 0 || cf.isp_type17.selectedIndex == 1 ||cf.isp_type17.selectedIndex == 3)
			cf.hid_apn.value="internet";
		else if(cf.isp_type17.selectedIndex == 2)
			cf.hid_apn.value="vdata";
		else
			set_other_isp();
	}
	else if(index == 17)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type18.value;
		if(cf.isp_type18.selectedIndex == 0 || cf.isp_type18.selectedIndex == 1)
			cf.hid_apn.value="internet.tele2.se";
		else if(cf.isp_type18.selectedIndex == 2)
			cf.hid_apn.value="internet.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 3)
			cf.hid_apn.value="services.vodafone.net";
		else if(cf.isp_type18.selectedIndex == 4)
			cf.hid_apn.value="online.telia.se";
		else if(cf.isp_type18.selectedIndex == 5)
			cf.hid_apn.value="internet.djuice.se";
		else
			set_other_isp();
	}
	else if(index == 18)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type19.value;
		cf.hid_apn.value="internet";
		if(cf.isp_type19.selectedIndex == 7)
			set_other_isp();
	}
	else if(index == 19)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type20.value;
		if(cf.isp_type20.selectedIndex == 0)
			cf.hid_apn.value="three.co.uk";
		else if(cf.isp_type20.selectedIndex == 1)
			cf.hid_apn.value="3internet";
		else if(cf.isp_type20.selectedIndex == 2)
			cf.hid_apn.value="airtel-ci-gprs.com";
		else if(cf.isp_type20.selectedIndex == 3)
			cf.hid_apn.value="pepper";
		else if(cf.isp_type20.selectedIndex == 4)
			cf.hid_apn.value="wap.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 5)
			cf.hid_apn.value="mobile.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 6)
			cf.hid_apn.value="bb-m.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 7)
			cf.hid_apn.value="payandgo.o2.co.uk";
		else if(cf.isp_type20.selectedIndex == 8 || cf.isp_type20.selectedIndex == 9)
			cf.hid_apn.value="orangeinternet";
		else if(cf.isp_type20.selectedIndex == 10)
			cf.hid_apn.value="general.t-mobile.uk";
		else if(cf.isp_type20.selectedIndex == 11)
			cf.hid_apn.value="prepay.tesco-mobile.com";
		else if(cf.isp_type20.selectedIndex == 12)
			cf.hid_apn.value="goto.virginmobile.uk";
		else if(cf.isp_type20.selectedIndex == 13)
			cf.hid_apn.value="internet";
		else if(cf.isp_type20.selectedIndex == 14)
			cf.hid_apn.value="pp.vodafone.co.uk";
		else
			set_other_isp();
	}
	else if(index == 20)
	{
		cf.hid_accnum.value=1;
		cf.hid_isp.value=cf.isp_type21.value;
		if(cf.isp_type21.selectedIndex == 0 || cf.isp_type21.selectedIndex == 2)
			cf.hid_apn.value="isp.cingular";
		else if(cf.isp_type21.selectedIndex == 1)
			cf.hid_apn.value="internet";
		else
			set_other_isp();
	}
		
	cf.submit();
}

function set_evdo()
{
	var cf=document.forms[0];
	cf.submit_flag.value="succ_conn_evdo";
	cf.action="/apply.cgi?/BRS_04_applySettings.html timestamp="+ts;
	cf.submit();
}
