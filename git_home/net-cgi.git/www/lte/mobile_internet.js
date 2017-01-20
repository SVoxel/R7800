function click_loginreq()
{
	var cf= document.forms[0];
	if(cf.loginreq[1].checked == true)
	{
		cf.username.disabled=true;
		cf.passwd.disabled=true;
	}
	else
	{
		cf.username.disabled=false;
		cf.passwd.disabled=false;
	}
}

function apply_submit(cf)
{
	cf.hid_mtu_value.value=wan_mtu_now;
	if(cf.conn_type.value=="4g")
		wanproto_type="lte";
	else
		wanproto_type="3g";
	mtu_change(wanproto_type);
	return true;
}

function click_mobile_info()
{
	window.open('mobile_info.htm', 'mobile_info', 'width=961,height=400,top=200,left=200,status=yes,resizable=yes');
}

function click_conn_status()
{
	window.open('get_mobile_conn_status.htm', 'conn_status', 'width=961,height=400,top=200,left=200,status=yes,resizable=yes');
}

function change_isp(index)
{
	var cf=document.forms[0];
	
	cf.username.value="Guest";
	cf.passwd.value="Guest";
	if(index == 0)
        {
		cf.operator_txt0.value="";
		cf.hid_isp.value=cf.isp_type0.value;
		if(cf.isp_type0.selectedIndex == 2 || cf.isp_type0.selectedIndex == 7 || cf.isp_type0.selectedIndex == 8 || cf.isp_type0.selectedIndex == 9 
			|| cf.isp_type0.selectedIndex == 11 || cf.isp_type0.selectedIndex == 13)
			cf.hid_accnum.value=0;
		else if(cf.isp_type0.selectedIndex == 3)
			cf.hid_accnum.value=3;
		else if(cf.isp_type0.selectedIndex == 5)
			cf.hid_accnum.value=2;
		else
			cf.hid_accnum.value=1;

		if(cf.isp_type0.selectedIndex == 0)
			cf.operator_txt0.value="internet";
		else if(cf.isp_type0.selectedIndex == 1 || cf.isp_type0.selectedIndex == 14)
			cf.operator_txt0.value="connect";
		else if(cf.isp_type0.selectedIndex == 2 || cf.isp_type0.selectedIndex == 3)
			cf.operator_txt0.value="telstra.internet";
		else if(cf.isp_type0.selectedIndex == 4)
			cf.operator_txt0.value="telstra.pcpack";
		else if(cf.isp_type0.selectedIndex == 5)
			cf.operator_txt0.value="telstra.datapack";
		else if(cf.isp_type0.selectedIndex == 6)
			cf.operator_txt0.value="telstra.bigpond";
		else if(cf.isp_type0.selectedIndex == 7)
			cf.operator_txt0.value="3netaccess";
		else if(cf.isp_type0.selectedIndex == 8)
			cf.operator_txt0.value="3services";
		else if(cf.isp_type0.selectedIndex == 9)
			cf.operator_txt0.value="VirginBroadband";
		else if(cf.isp_type0.selectedIndex == 10)
			cf.operator_txt0.value="vfinternet.au";
		else if(cf.isp_type0.selectedIndex == 11)
			cf.operator_txt0.value="WirelessBroadband";
		else if(cf.isp_type0.selectedIndex == 12)
			cf.operator_txt0.value="dodolns1";
		else if(cf.isp_type0.selectedIndex == 13)
			cf.operator_txt0.value="primuslns1";
		else if(cf.isp_type0.selectedIndex == 15)
			cf.operator_txt0.value="3services";
		else if(cf.isp_type0.selectedIndex == 16)
			cf.operator_txt0.value="splns555a1";
	}
	else if(index == 1)
	{
		cf.operator_txt1.value="";
		cf.hid_isp.value=cf.isp_type1.value;
		cf.hid_accnum.value=0;
		if(cf.isp_type1.selectedIndex == 0)
			cf.operator_txt1.value="A1.net";
		else if(cf.isp_type1.selectedIndex == 1)
			cf.operator_txt1.value="gprsinternet";
		else if(cf.isp_type1.selectedIndex == 2)
			cf.operator_txt1.value="web.one.at";
		else if(cf.isp_type1.selectedIndex == 3)
			cf.operator_txt1.value="drei.at";
		else if(cf.isp_type1.selectedIndex == 4 || cf.isp_type1.selectedIndex == 5)
			cf.operator_txt1.value="web";
		cf.hid_apn.value=cf.operator_txt1.value;
	}
	else if(index == 2)
	{
		cf.operator_txt2.value="";
		cf.hid_isp.value=cf.isp_type2.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type2.selectedIndex == 0)
			cf.operator_txt2.value="gprs.base.be";
		else if(cf.isp_type2.selectedIndex == 1)
                        cf.operator_txt2.value="internet.be";
		else if(cf.isp_type2.selectedIndex == 2)
			cf.operator_txt2.value="web.pro.be";
		else if(cf.isp_type2.selectedIndex == 3)
			cf.operator_txt2.value="orangeinternet";
		else if(cf.isp_type2.selectedIndex == 4)
                        cf.operator_txt2.value="internet.proximus.be";
		cf.hid_apn.value=cf.operator_txt2.value;
	}
	else if(index == 3)
	{
		cf.operator_txt3.value="";
		cf.hid_isp.value=cf.isp_type3.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type3.selectedIndex == 0)
			cf.operator_txt3.value="claro.com.br";
		else if(cf.isp_type3.selectedIndex == 1)
			cf.operator_txt3.value="gprs.oi.com.br";
		else if(cf.isp_type3.selectedIndex == 2)
			cf.operator_txt3.value="wapgprs.oi.com.br";
		else if(cf.isp_type3.selectedIndex == 3)
			cf.operator_txt3.value="tim.br";
		else if(cf.isp_type3.selectedIndex == 4)
			cf.operator_txt3.value="wap.telcel.com";
		cf.hid_apn.value=cf.operator_txt3.value;
	}
	else if(index == 4)
	{
		cf.operator_txt4.value="";
		cf.hid_isp.value=cf.isp_type4.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type4.selectedIndex == 0)
			cf.operator_txt4.value="wap.clarochile.cl";
		else if(cf.isp_type4.selectedIndex == 1)
			cf.operator_txt4.value="imovil.entelpcs.cl";
		else if(cf.isp_type4.selectedIndex == 2)
			cf.operator_txt4.value="wap.tmovil.cl";
		else if(cf.isp_type4.selectedIndex == 3)
			cf.operator_txt4.value="wep.tmovil.cl";
		cf.hid_apn.value=cf.operator_txt4.value;
	}
	else if(index == 5)
	{
		cf.username.value="";
		cf.passwd.value="";
		cf.operator_txt5.value="";
		cf.hid_isp.value=cf.isp_type5.value;
		if(cf.isp_type5.selectedIndex == 0)
		{
			cf.operator_txt5.value="cmnet";
			cf.hid_accnum.value=5;
		}
		else if(cf.isp_type5.selectedIndex == 1)
		{
			cf.operator_txt5.value="3gnet";
			cf.hid_accnum.value=0;
		}
		else if(cf.isp_type5.selectedIndex == 2)
		{
			cf.hid_accnum.value=6;
			cf.username.value="ctnet@mycdma.cn";
			cf.passwd.value="vnet.mobi";
		}
		cf.hid_apn.value=cf.operator_txt5.value;
	}
	else if(index == 6)
	{
		cf.operator_txt6.value="";
		cf.hid_isp.value=cf.isp_type6.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type6.selectedIndex == 0 || cf.isp_type6.selectedIndex == 1  || cf.isp_type6.selectedIndex == 3)
			cf.operator_txt6.value="internet";
		else if(cf.isp_type6.selectedIndex == 2)
			cf.operator_txt6.value="internet.saunalahti";
		else if(cf.isp_type6.selectedIndex == 4)
			cf.operator_txt6.value="internet.song.fi";
		cf.hid_apn.value=cf.operator_txt6.value;
	}
	else if(index == 7)
	{
		cf.operator_txt7.value="";
		cf.hid_isp.value=cf.isp_type7.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type7.selectedIndex == 0)
			cf.operator_txt7.value="surfo2";
		else if(cf.isp_type7.selectedIndex == 1)
			cf.operator_txt7.value="internet";
		else if(cf.isp_type7.selectedIndex == 2)
			cf.operator_txt7.value="tagesflat.eplus.de";
		else if(cf.isp_type7.selectedIndex == 3)
			cf.operator_txt7.value="internet.t-mobile";
		else if(cf.isp_type7.selectedIndex == 4)
			cf.operator_txt7.value="web.vodafone.de";
		cf.hid_apn.value=cf.operator_txt7.value;
	}
	else if(index == 8)
	{
		cf.operator_txt8.value="";
		cf.hid_isp.value=cf.isp_type8.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type8.selectedIndex == 0 || cf.isp_type8.selectedIndex == 1 || cf.isp_type8.selectedIndex == 3 ||cf.isp_type8.selectedIndex == 4 || cf.isp_type8.selectedIndex == 5)
			cf.operator_txt8.value="internet";
		else if(cf.isp_type8.selectedIndex == 2)
			cf.operator_txt8.value="web.orangehk.com";
		else if(cf.isp_type8.selectedIndex == 6)
			cf.operator_txt8.value="mobile.three.com.hk";
		cf.hid_apn.value=cf.operator_txt8.value;
	}
	else if(index == 9)
	{
		cf.operator_txt9.value="";
		cf.hid_isp.value=cf.isp_type9.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type9.selectedIndex == 0)
			cf.operator_txt9.value="internet.postemobile.it";
		else if(cf.isp_type9.selectedIndex == 1)
			cf.operator_txt9.value="tre.it";
		else if(cf.isp_type9.selectedIndex == 2)
			cf.operator_txt9.value="ibox.tim.it";
		else if(cf.isp_type9.selectedIndex == 3)
			cf.operator_txt9.value="web.omnitel.it";
		else if(cf.isp_type9.selectedIndex == 4)
			cf.operator_txt9.value="internet.wind";
		cf.hid_apn.value=cf.operator_txt9.value;
	}
	else if(index == 10)
	{
		cf.operator_txt10.value="";
		cf.hid_isp.value=cf.isp_type10.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type10.selectedIndex == 0)
			cf.operator_txt10.value="portalmmm.nl";
		else if(cf.isp_type10.selectedIndex == 1 || cf.isp_type10.selectedIndex == 2 || cf.isp_type10.selectedIndex == 3 || cf.isp_type10.selectedIndex == 4)
			cf.operator_txt10.value="internet";
		else if(cf.isp_type10.selectedIndex == 5)
			cf.operator_txt10.value="live.vodafone.com";
		else if(cf.isp_type10.selectedIndex == 6)
			cf.operator_txt10.value="office.vodafone.nl";
		cf.hid_apn.value=cf.operator_txt10.value;
	}
	else if(index == 11)
	{
		cf.operator_txt11.value="";
		cf.hid_isp.value=cf.isp_type11.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type11.selectedIndex == 0)
			cf.operator_txt11.value="live.vodafone.com";
		else if(cf.isp_type11.selectedIndex == 1)
			cf.operator_txt11.value="internet";
		else if(cf.isp_type11.selectedIndex == 2)
			cf.operator_txt11.value="www.vodafone.net.nz";
		cf.hid_apn.value=cf.operator_txt11.value;
	}
	else if(index == 12)
	{
		cf.operator_txt12.value="";
		cf.hid_isp.value=cf.isp_type12.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type12.selectedIndex == 0 || cf.isp_type12.selectedIndex == 1)
			cf.operator_txt12.value="internet.netcom.no";
		else if(cf.isp_type12.selectedIndex == 2)
			cf.operator_txt12.value="internet.telenor.no";
		else if(cf.isp_type12.selectedIndex == 3)
			cf.operator_txt12.value="internet.ventelo.no";
		cf.hid_apn.value=cf.operator_txt12.value;
	}
	else if(index == 13)
	{
		cf.operator_txt13.value="";
		cf.hid_isp.value=cf.isp_type13.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type13.selectedIndex == 0)
			cf.operator_txt13.value="tim.pe";
		cf.hid_apn.value=cf.operator_txt13.value;
	}
	else if(index == 14)
	{
		cf.operator_txt14.value="";
		cf.hid_isp.value=cf.isp_type14.value;
		cf.hid_accnum.value=0;
		if(cf.isp_type14.selectedIndex == 0)
		{
			cf.operator_txt14.value="internet.mts.ru";
			cf.username.value="mts";
			cf.passwd.value="mts";
		}
		else if(cf.isp_type14.selectedIndex == 1)
		{
			cf.operator_txt14.value="internet";
			cf.username.value="gdata";
			cf.passwd.value="gdata";
		}
		else if(cf.isp_type14.selectedIndex == 2)
		{
			cf.operator_txt14.value="internet.beeline.ru";
			cf.username.value="beeline";
			cf.passwd.value="beeline";
		}
		else if(cf.isp_type14.selectedIndex == 3)
		{
			cf.operator_txt14.value="internet.TELE2.ru";
			cf.username.value="";
			cf.passwd.value="";
		}
		else if(cf.isp_type14.selectedIndex == 4)
		{
			cf.username.value="mobile";
			cf.passwd.value="internet";
		}
		cf.hid_apn.value=cf.operator_txt14.value;
	}
	else if(index == 15)
	{
		cf.operator_txt15.value="";
		cf.hid_isp.value=cf.isp_type15.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type15.selectedIndex == 0)
			cf.operator_txt15.value="sunsurf";
		else if(cf.isp_type15.selectedIndex == 1)
			cf.operator_txt15.value="internet";
		else if(cf.isp_type15.selectedIndex == 2)
			cf.operator_txt15.value="shwap";
		cf.hid_apn.value=cf.operator_txt15.value;
	}
	else if(index == 16)
	{
		cf.operator_txt16.value="";
		cf.hid_isp.value=cf.isp_type16.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type16.selectedIndex == 0 || cf.isp_type16.selectedIndex == 1 ||cf.isp_type16.selectedIndex == 3)
			cf.operator_txt16.value="internet";
		else if(cf.isp_type16.selectedIndex == 2)
			cf.operator_txt16.value="vdata";
		cf.hid_apn.value=cf.operator_txt16.value;
	}
	else if(index == 17)
	{
		cf.operator_txt17.value="";
		cf.hid_isp.value=cf.isp_type17.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type17.selectedIndex == 0 || cf.isp_type17.selectedIndex == 1)
                        cf.operator_txt17.value="internet.tele2.se";
		else if(cf.isp_type17.selectedIndex == 2)
			cf.operator_txt17.value="internet.vodafone.net";
		else if(cf.isp_type17.selectedIndex == 3)
			cf.operator_txt17.value="services.vodafone.net";
		else if(cf.isp_type17.selectedIndex == 4)
			cf.operator_txt17.value="online.telia.se";
		else if(cf.isp_type17.selectedIndex == 5)
			cf.operator_txt17.value="internet.djuice.se";
		cf.hid_apn.value=cf.operator_txt17.value;
	}
	else if(index == 18)
	{
		cf.operator_txt18.value="";
		cf.operator_txt18.value=cf.isp_type18.value;
		cf.hid_accnum.value=1;
                if(cf.isp_type18.selectedIndex != 7)
			cf.hid_apn.value="internet";
		cf.hid_apn.value=cf.operator_txt18.value;
        }
	else if(index == 19)
	{
		cf.operator_txt19.value="";
		cf.hid_isp.value=cf.isp_type19.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type19.selectedIndex == 0)
			cf.operator_txt19.value="three.co.uk";
		else if(cf.isp_type19.selectedIndex == 1)
			cf.operator_txt19.value="3internet";
		else if(cf.isp_type19.selectedIndex == 2)
			cf.operator_txt19.value="airtel-ci-gprs.com";
		else if(cf.isp_type19.selectedIndex == 3)
			cf.operator_txt19.value="pepper";
		else if(cf.isp_type19.selectedIndex == 4)
			cf.operator_txt19.value="wap.o2.co.uk";
		else if(cf.isp_type19.selectedIndex == 5)
			cf.operator_txt19.value="mobile.o2.co.uk";
		else if(cf.isp_type19.selectedIndex == 6)
			cf.operator_txt19.value="bb-m.o2.co.uk";
		else if(cf.isp_type19.selectedIndex == 7)
			cf.operator_txt19.value="payandgo.o2.co.uk";
		else if(cf.isp_type19.selectedIndex == 8 || cf.isp_type19.selectedIndex == 9)
			cf.operator_txt19.value="orangeinternet";
		else if(cf.isp_type19.selectedIndex == 10)
			cf.operator_txt19.value="general.t-mobile.uk";
		else if(cf.isp_type19.selectedIndex == 11)
			cf.operator_txt19.value="prepay.tesco-mobile.com";
		else if(cf.isp_type19.selectedIndex == 12)
			cf.operator_txt19.value="goto.virginmobile.uk";
		else if(cf.isp_type19.selectedIndex == 13)
			cf.operator_txt19.value="internet";
		else if(cf.isp_type19.selectedIndex == 14)
			cf.operator_txt19.value="pp.vodafone.co.uk";
		cf.hid_apn.value=cf.operator_txt19.value;
	}
	else if(index == 20)
	{
		cf.operator_txt20.value="";
		cf.username.value="ISP@CINGULARGPRS.COM";
		cf.passwd.value="CINGULAR1";
		cf.hid_isp.value=cf.isp_type20.value;
		cf.hid_accnum.value=1;
		if(cf.isp_type20.selectedIndex == 0 || cf.isp_type20.selectedIndex == 2)
			cf.operator_txt20.value="isp.cingular";
		else if(cf.isp_type20.selectedIndex == 1)
			cf.operator_txt20.value="internet";
		cf.hid_apn.value=cf.operator_txt20.value;
	}
}
