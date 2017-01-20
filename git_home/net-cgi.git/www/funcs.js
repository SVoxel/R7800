var vdsl_list={"Australia":{
					"TPG":{"num":"1","vlanid1":"2"}
					},
				"Austria":{
					"A1 Telekom":{"num":"2","vlanid1":"2","vlanid2":"4"},
					"Comteam.at":{"num":"1","vlanid1":"2"}
					},
				"Belgium":{
						"Proximus(Belgacom)":{"num":"2","vlanid1":"10","vlanid2":"30"}
					},
				"Canada":{
						"SaskTel":{"num":"1","vlanid1":"2238"},
						"TekSavvy":{"num":"1","vlanid1":"35"}
					},
				"Denmark":{
						"Telenor":{"num":"1","vlanid1":"101"},
						"TDC":{"num":"1","vlanid1":"101"}
					},
				"Finland":{
						"DNA Welho":{"num":"1","vlanid1":""}
					},
				"France":{
						"Free":{"num":"2","vlanid1":"","vlanid2":"100"},
						"Orange":{"num":"2","vlanid1":"835","vlanid2":"838"}
					},
				"Germany":{
						"Deutsche Telekom":{"num":"2","vlanid1":"7","vlanid2":"8"},
						"1&1":{"num":"1","vlanid1":"7"}
					},
				"Ireland":{
						"Eircom":{"num":"1","vlanid1":"10"}
					},
				"Italy":{
						"Fastweb":{"num":"1","vlanid1":"1"},
						"Telecom Italia":{"num":"1","vlanid1":"835"}
					},
				"New Zealand":{
						"IHug":{"num":"1","vlanid1":"10"},
						"Kiwi Online(KOL)":{"num":"1","vlanid1":"10"},
						"Orcon":{"num":"1","vlanid1":"10"},
						"Paradise":{"num":"1","vlanid1":"10"},
						"Slingshot":{"num":"1","vlanid1":"10"},
						"Spark":{"num":"1","vlanid1":"10"},
						"Xnet(World Exchange)":{"num":"1","vlanid1":"10"},
						"Bigpipe":{"num":"1","vlanid1":"10"},
						"Vodafone":{"num":"1","vlanid1":"10"},
						"Trust power":{"num":"1","vlanid1":"10"},
						"Snap":{"num":"1","vlanid1":"10"},
						"Telstra Clear":{"num":"1","vlanid1":"10"},
						"Other":{"num":"1","vlanid1":"10"}
					},
				"Singapore":{
						"SingTel":{"num":"2","vlanid1":"10","vlanid2":"20"}
					},
				"Spain":{
						"Jazztel":{"num":"1","vlanid1":"1079"},
						"Movistar":{"num":"2","vlanid1":"6","vlanid2":"2"}
					},
				"Sweden":{
						"Telia":{"num":"2","vlanid1":"835","vlanid2":"845"}
					},
				"Switzerland":{
						"Swisscom":{"num":"1","vlanid1":"10"},
						"Swisscom All IP(with phone)":{"num":"1","vlanid1":""},
						"Sunrise":{"num":"1","vlanid1":""},
						"Telfort":{"num":"1","vlanid1":"34"}
					},
				"UK":{
						"BT":{"num":"2","vlanid1":"101","vlanid2":"101"},
						"PlusNet":{"num":"2","vlanid1":"101","vlanid2":""},
						"TalkTalk":{"num":"2","vlanid1":"101","vlanid2":"101"},
						"Sky":{"num":"1","vlanid1":"101"}
					},
					"USA":{
						"CenturyLink":{"num":"1","vlanid1":"201"}
					}
};


var adsl_list = {"Australia":{
					"Soul":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Exetel":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"People Telcom":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Adam":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Telstra":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Internode":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Westnet":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Optus":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Primus":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"TPG":{"vpi_num":"0","vpi2":"0","vci2":"32","multi2":"llc","vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"iiNet":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Dodo":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Telstra Bigpond":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"TransACT":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Clubtelco":{"vpi":"8","vci":"35","encap":"pppoa","multi":"llc"}, 
					"UPC Austria":{"vpi":"9","vci":"35","encap":"pppoe","multi":"llc"},
					"UTA2":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Engin":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Austria":{
					"UPC Austria":{"vpi":"9","vci":"35","encap":"pppoe","multi":"llc"},
					"UTA2":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"A1 Telekom":{"vpi":"8","vci":"48","encap":"pppoa","multi":"llc"},
					"Other":{"vpi":"8","vci":"48","encap":"pppoe","multi":"llc"}
					},
				"Belgium":{
					"Proximus(Belgacom)":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Brazil":{
					"Gvt-Global Village Telecom":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Telemar Oi Velox":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"Oi":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"Canada":{
					"Bell Canada(Western)":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"SaskTel":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"TekSavvy":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"China":{
					"China Telecom":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Bell Canada(Western)":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Czech Republic":{
					"O2":{"vpi":"8","vci":"48","encap":"pppoe","multi":"llc"},
					"Tiscali":{"vpi":"8","vci":"48","encap":"pppoe","multi":"llc"},
					"T-Mobile":{"vpi":"8","vci":"48","encap":"pppoe","multi":"llc"}
					},
				"Denmark":{
					"Telenor":{"vpi":"0","vci":"35","encap":"pppoa","multi":"vc"},
					"TDC":{"vpi":"0","vci":"35","encap":"pppoa","multi":"vc"}
					},
				"Finland":{
					"Teliasonera":{"vpi":"0","vci":"33","encap":"pppoa","multi":"llc"},
					"Elisa Oyj":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"}
					},
				"France":{
					"SFR":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Orange":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"OVH":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Free":{"vpi":"8","vci":"36","encap":"ipoa","multi":"vc"},
					"Nordnet":{"vpi":"8","vci":"35","encap":"pppoa","multi":"llc"},
					"Other":{"vpi":"0","vci":"35","encap":"pppo5","multi":"llc"}
					},
				"Germany":{
					"1&1":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"Deutsche Telekom":{"vpi_num":"0","vpi2":"1","vci2":"33","multi2":"llc","vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"Telefonica O2":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"m-net":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"Telstra":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Vodafone":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"Netcologne":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Hongkong":{
					"PCCW":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"Hungary":{
					"Actel":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"GTS Datanet":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"Invitel":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"},
					"T-Home":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"India":{
					"MTNL":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"TataIndicom":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"BSNL":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Inedit":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Indonesia":{
					"Telkom Speedy":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Speedy":{"vpi":"8","vci":"81","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"8","vci":"35","encap":"pppoa","multi":"llc"}
					},
				"Italy":{
					"Telecom Italia":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Infostrada":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Aruba":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Tiscali":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Infostrada":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Twt":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Acantho":{"vpi":"8","vci":"35","encap":"pppoa","multi":"vc"},
					"Fastweb":{"vpi":"8","vci":"36","encap":"dhcp","multi":"llc"},
					"Wind(Infostrada)":{"vpi":"8","vci":"35","encap":"pppoe","multi":"vc"},
					"Tiscali":{"vpi":"0","vci":"38","encap":"pppoa","multi":"vc"},
					"TELE2":{"vpi":"8","vci":"36","encap":"pppoe","multi":"llc"},
					"MC-link":{"vpi":"8","vci":"75","encap":"pppoa","multi":"vc"},
					"Intred":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"KPN Qwest Italia":{"vpi":"8","vci":"35","encap":"ipoa","multi":"vc"}
					},
				"Malaysia":{
					"Streamyx":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Netherlands":{
					"Vodafonevast":{"vpi":"0","vci":"34","encap":"pppoe","multi":"llc"},
					"Telfort":{"vpi":"0","vci":"34","encap":"pppoe","multi":"llc"},
					"Online-nl":{"vpi":"8","vci":"35","encap":"dhcp","multi":"llc"}
					},
				"New Zealand":{
					"IHug":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Kiwi Online(KOL)":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Orcon":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Paradise":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Slingshot":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Telstra Clear":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Spark":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Xnet(World Exchange)":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Snap":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Bigpipe":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Vodafone":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Trust power":{"vpi":"0","vci":"100","encap":"pppoa","multi":"vc"},
					"Other":{"vpi":"0","vci":"100","encap":"pppoa","multi":"llc"}
					},
				"Other":{
					"Streamyx":{"vpi":"0","vci":"38","encap":"pppoa","multi":"vc"}
					},
				"Philippine":{
					"PLDT":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"},
					"TelPlus":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"}
					},
				"Poland":{
					"Multimo":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Tele2":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Netia":{"vpi":"0","vci":"35","encap":"pppoa","multi":"vc"},
					"Orange":{"vpi":"0","vci":"35","encap":"pppoa","multi":"vc"},
					"Other":{"vpi":"0","vci":"35","encap":"pppoa","multi":"llc"}
					},
				"Portugal":{
					"PT":{"vpi":"0","vci":"35","encap":"pppoe","multi":"vc"},
					"Vodafone":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Zon":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Russia":{
					"Beltelecom":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"Domolink CentrTelekom":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Avangard-DSL":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"MTS Stream":{"vpi":"1","vci":"50","encap":"pppoe","multi":"llc"},
					"MGTS":{"vpi":"1","vci":"50","encap":"pppoe","multi":"llc"},
					"Jdsl Volgatelekom":{"vpi":"1","vci":"100","encap":"pppoe","multi":"llc"},
					"Combellga":{"vpi":"8","vci":"63","encap":"pppoa","multi":"vc"},
					"Rostelecom":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"Saudi Arabia":{
					"STC":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Singapore":{
					"PacificNet":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"},
					"Singnet":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"0","vci":"100","encap":"pppoe","multi":"vc"}
					},
				"Spain":{
					"Telefonica Movistar(dynamic IP)":{"vpi":"8","vci":"32","encap":"pppoe","multi":"llc"},
					"Orange":{"vpi":"8","vci":"32","encap":"pppoe","multi":"llc"},
					"Jazztel":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Vodafone":{"vpi":"0","vci":"33","encap":"pppoa","multi":"vc"},
					"Pepephone":{"vpi":"0","vci":"33","encap":"pppoa","multi":"vc"},
					"Telefonica Movistar(fixed IP)":{"vpi":"8","vci":"32","encap":"ipoa","multi":"llc"},
					"Other":{"vpi":"8","vci":"35","encap":"ipoa","multi":"llc"}
					},
				"Sweden":{
					"Telia":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Ljusnet":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Bahnhof AB":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Bredbandsbolaget":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"8","vci":"35","encap":"pppoa","multi":"llc"}
					},
				"Switzerland":{
					"Sunrise":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Monzoon Networks":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Siwsscom":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Telfort":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"}
					},
				"Thailand":{
					"TT&T":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"3BB":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"CAT-Telecom":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"CS-Loxinfo":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Samart":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"TRUE Internet":{"vpi":"0","vci":"100","encap":"pppoe","multi":"llc"},
					"TOT":{"vpi":"1","vci":"32","encap":"pppoe","multi":"llc"}
					},
				"Turkey":{
					"Other":{"vpi":"8","vci":"35","encap":"pppoe","multi":"vc"}
					},
				"UK":{
					"BT":{"vpi_num":"0","vpi2":"0","vci2":"35","multi2":"vc","vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"Sky":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"TalkTalk":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"PlusNet":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"Virgin":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"Media":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"EE(Orange)":{"vpi":"0","vci":"38","encap":"pppoe","multi":"llc"},
					"Other":{"vpi":"8","vci":"36","encap":"pppoe","multi":"vc"}
					},
				"USA":{
					"AT&T":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Earthlink":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Verizon":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Windstream":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Frontier Communications":{"vpi":"0","vci":"35","encap":"pppoa","multi":"llc"},
					"Rivercity Internet Group":{"vpi":"0","vci":"35","encap":"pppoa","multi":"llc"},					
					"CenturyLink":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Bigriver.net DSL":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"Qwest":{"vpi":"0","vci":"32","encap":"pppoe","multi":"llc"},
					"cox communications":{"vpi":"8","vci":"129","encap":"dhcp","multi":"vc"},
					"BEVCOMM":{"vpi":"0","vci":"35","encap":"dhcp","multi":"llc"},
					"Other":{"vpi":"0","vci":"32","encap":"pppoa","multi":"llc"}
					},
				"Vietnam":{
					"FPT":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"NATNAM":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"SPT":{"vpi":"0","vci":"33","encap":"pppoe","multi":"llc"},
					"VNN(in Hanoi)":{"vpi":"0","vci":"35","encap":"pppoe","multi":"llc"},
					"Viettel":{"vpi":"8","vci":"35","encap":"pppoe","multi":"llc"},
					"VNN(in HAM)":{"vpi":"8","vci":"38","encap":"pppoe","multi":"llc"}
					}
				

};
function set_checkbox_value(cb_obj, hid_obj)
{
	if(cb_obj.checked == true)
		hid_obj.value = "1";
	else
		hid_obj.value = "0";
}

function load_checkbox_value(cfg_val, cb_obj)
{
	if(cfg_val == "1")
		cb_obj.checked = true;
	else
		cb_obj.checked = false;
}

function sAlert(str, callback_ok, callback_cancel, dwidth, anc, button_lang){

	var mode = "alert";
	callback_ok = callback_ok || 0;
	callback_cancel = callback_cancel || 0;

	var anc_v=anc || 0; // anc£º click apply button, not close div
	
	var msgw,msgh,bordercolor;
	msgw=dwidth||420;//warning width
	msgh=160;//warning height

	//if have cancel callback function, show cancel button
	if(typeof(callback_cancel) == "function" )
		mode = "confirm";

	if(document.getElementById("bgDiv") != undefined)
		removeObj();

	titleheight=25;//warning title height
	var sWidth,sHeight;
	sWidth=document.body.offsetWidth;//page width
	sHeight=screen.height;//page height
	//background div
	var bgObj=document.createElement("div");//create a div
	//setting div Attribute
	bgObj.setAttribute('id','bgDiv');
	bgObj.style.position="absolute";
	bgObj.style.top="0";
	bgObj.style.filter="progid:DXImageTransform.Microsoft.Alpha(style=3,opacity=25,finishOpacity=75";
	bgObj.style.opacity="0.6";
	bgObj.style.left="0";
	bgObj.style.width=sWidth + "px";
	bgObj.style.height=sHeight + "px";
	bgObj.style.zIndex = "10000";
	document.body.appendChild(bgObj);//add this div into body
	//create warning div
	var msgObj=document.createElement("div");
	msgObj.setAttribute("id","msgDiv");
	msgObj.setAttribute("align","center");
	//msgObj.style.background=" url(image/confirm.gif) no-repeat";
	msgObj.style.position = "absolute";
	msgObj.style.left = "40%";
	msgObj.style.top = "30%";
	msgObj.style.marginLeft = "-225px" ;
	msgObj.style.width = msgw + "px";
	//msgObj.style.height = msgh + "px";
	msgObj.style.zIndex = "10001";
	msgObj.className = msgObj.className+ " pop_box";
	var div1=document.createElement("div");//create msg div
	//setting msg div
	div1.setAttribute("id","msgTitle");
	div1.setAttribute("align","left");
	//div1.style.marginTop="30px";
	div1.style.marginLeft="30px";
	div1.style.marginRight="30px";
	div1.style.marginBottom="20px";
	div1.style.padding="0px";
	div1.style.filter="progid:DXImageTransform.Microsoft.Alpha(startX=20, startY=20, finishX=100, finishY=100,style=1,opacity=75,finishOpacity=100);";
	div1.style.opacity="0.75";
	//div1.style.height="85px";
	div1.style.font="12px Verdana, Geneva, Arial, Helvetica, sans-serif";
	div1.style.cursor="pointer";
	div1.innerHTML=str;
	var button=document.createElement("input");//create ok button
	//setting ok button
	button.setAttribute("type","button");
	if(button_lang == "yes/no")
		button.setAttribute("value","$yes_mark");
	else if(button_lang == "cancel/apply")
		button.setAttribute("value","$cancel_mark");
	else
		button.setAttribute("value","$ok_mark");
	button.style.width="80px";
	button.style.marginLeft="140px";
	button.style.marginBottom="10px";
	if(button_lang == "cancel/apply")
	{
		button.style.background=" #5bb6e5";
		button.style.border="1px solid "+ " #5bb6e5";
	}
	else
	{
		button.style.background=" #702b86";
		button.style.border="1px solid "+ " #702b86";
	}
	button.style.color="white";
	button.onclick=click_ok;
	var button1=document.createElement("input");//create cancel button
	//setting cancel button
	button1.setAttribute("type","button");
	if(button_lang == "yes/no")
		button1.setAttribute("value","$no_mark");
	else if(button_lang == "cancel/apply")
		button1.setAttribute("value","$apply_mark");
	else
		button1.setAttribute("value","$cancel_mark");
	button1.style.width="80px";
	button1.style.marginLeft="10px";
	button1.style.marginBottom="10px";
	if(button_lang == "cancel/apply")
	{
		button1.style.background="#702b86";
		button1.style.border="1px solid "+ "#702b86";
	}
	else
	{
		button1.style.background="#5bb6e5";
		button1.style.border="1px solid "+ "#5bb6e5";
	}
	button1.style.color="white";
	button1.onclick=click_cancel;
	function removeObj(){//close warning
		document.body.removeChild(document.getElementById("bgDiv"));//delete background div
		document.getElementById("msgDiv").removeChild(document.getElementById("msgTitle"));//delete title
		document.forms[0].removeChild(document.getElementById("msgDiv"));//delete warning
	}
	function click_ok(){//for Ok button

		if(anc_v == 0 )
			removeObj();

		if(typeof(callback_ok)== "function")
			callback_ok();
	}

	function click_cancel(){// for cancel button
		if(typeof(callback_cancel) == "function")
			callback_cancel();
		if(button_lang != "yes/no" && button_lang != "cancel/apply")
			removeObj();
	}

	document.forms[0].appendChild(msgObj);//add warning div into body
	document.getElementById("msgDiv").appendChild(div1);// add title into warning div
	var txt=document.createElement("p");//add warning infomation into warning div
	document.getElementById("msgDiv").appendChild(div1);
	document.getElementById("msgDiv").appendChild(button);//add ok button into warning div
	if(mode == "confirm")
		document.getElementById("msgDiv").appendChild(button1); //add cancel button into warning div
	if(button_lang == "yes/no"){
		document.getElementById("fix_button").style.zIndex = "10001";
		document.getElementById("new_apply_bt").onclick=function(){return false};
	}
}


function createXMLHttpRequest()
{
        if(typeof XMLHttpRequest == "undefined")
                XMLHttpRequest = function(){
                        try{return new ActiveXObject("Msxml2.XMLHTTP.6.0");}
                                catch(e){}
                        try{return new ActiveXObject("Msxml2.XMLHTTP.3.0");}
                                catch(e){}
                        try{return new ActiveXObject("Msxml2.XMLHTTP");}
                                catch(e){}
                        return false;
                }
        return new XMLHttpRequest();
}


function edit_devicename(name)
{
	location.href="edit_device_name.htm";
	//lert('<table cellpadding=0 cellspacing=2  border=0><tr><td colspan=3 nowrap><h3>$edit_devname</h3></td></tr><tr><td colspan=3 >$devame_note</td></tr><tr><td nowrap style="height:40px">$lan_mark_name:</td><td><div id="devn_error" style="display: none;color:red">$devname_long</div></td>	<td nowrap align=right style="width:100px"><input type="text" name="new_devname" value="'+name+'" size="20" autocomplete="off" maxlength="20" onFocus="this.select();" onKeyPress="return getkey(\'ssid\', event)" ></td> </tr><tr><td colspan=3 >$devname_warning</td></tr><tr><td  colspan=3 height="20px"></td></tr></table>',check_dev, function(){return false;}, 600, 1);
}


function showWps_alert()
{
	if(wps_alert == "1")
	{
		alert("$wps_in_progress");
	}
}

function getDaysInMonth(year,month){
      month = parseInt(month,10)+1;
      var temp = new Date(year+"/"+month+"/0");
      return temp.getDate();
}

function num_month(month)
{
    var show_month;
    if( month == "Jan" )
        show_month="1";
    else if( month == "Feb" )
        show_month="2";
    else if( month == "Mar" )
        show_month="3";
    else if( month == "Apr" )
        show_month="4";
    else if( month == "May" )
        show_month="5";
    else if( month == "Jun" )
        show_month="6";
    else if( month == "Jul" )
        show_month="7";
    else if( month == "Aug" )
        show_month="8";
    else if( month == "Sep" )
        show_month="9";
    else if( month == "Oct" )
        show_month="10";
    else if( month == "Nov" )
        show_month="11";
    else if( month == "Dec" )
        show_month="12";
    else
        show_month=month;
		
    return show_month;
}

function setFooterClass()
{
	var footer_div = top.document.getElementById("footer");
	/*var content = footer_div.innerHTML.replace(/<\/?.+?>/g,"").replace(/[\r\n]/g, "").replace(/\s+/g, "");
	var content_len = content.length;
	var width = top.document.documentElement.clientWidth;

	if( width > 967 ){
		footer_div.className = "footer";
	}
	else{
		if(content_len > 75)
			footer_div.className = "footer_double";
		else
			footer_div.className = "footer";
	}*/
	
	var go_btn = top.document.getElementById("search_button");
	content_len = go_btn.value.length;
	
	if(content_len >= 7)
		go_btn.className = go_btn.className + "_long";
	else if(content_len >= 4)
		go_btn.className = go_btn.className + "_middle";
	else 
		go_btn.className = go_btn.className;
	
	var width = top.document.getElementById("support").clientWidth + top.document.getElementById("search").clientWidth;
	var screen_width = top.document.documentElement.clientWidth;

	if( width < screen_width - 60 )
		footer_div.className = "footer";
	else
		footer_div.className = "footer_double";
}

function open_window( page )
{
        var winoptions = "width=640,height=480,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes"
        window.open(page,null,winoptions);

}

function goto_home_page()
{
	if( top.location.href.indexOf("adv_index.htm") != -1 ){
		top.location.href="adv_index.htm";	
	}
	else if ( top.location.href.indexOf("BRS_index.htm") > -1 ){
		toInternet();
	}
	else{
		top.location.href="index.htm";	
	}
}

function existClassName(classname)
{
	if ( document.getElementsByClassName ) {
		return document.getElementsByClassName(classname);
	} else {
		var divs = document.getElementsByTagName("div");
		for ( i=0; i< divs.length; i++) {
			if (divs[i].className == classname )
				return divs[i];
		}
		return null;
	}
}
function please_wait(flag)
{
	var obj = top.document.getElementById("formframe_wait_div");
	var frame = top.document.getElementById("formframe");

	if ( typeof(obj) == "undefined" || typeof(frame) == "undefined" 
		|| obj == null || frame == null)
		return;

	if ( flag ) {
		frame.style.display = "none";
		obj.style.display = "";
		if(top.timeoutfunc)
			top.clearTimeout(top.timeoutfunc);
		top.timeoutfunc=top.setTimeout("please_wait(false);", 30000)
	} else {
		if(top.timeoutfunc)
			top.clearTimeout(top.timeoutfunc);
		obj.style.display = "none";
		frame.style.display = "";
		setTimeout(function(){
		if ( isIE() && document.getElementById("help_switch")) {
			if ( document.getElementById("buttom") )
				show_hidden_help_green(1);
			else if ( existClassName("fix_button") )
				show_hidden_help_top_button(1);
			else
				show_hidden_help(1);		
		}}, 10);
	}
}

function goto_formframe(page)
{
	please_wait(true);
	top.formframe.location.href=page;
}

function goto_page(page)
{
	location.href=page;
}

//38280 for WAN_wan.htm Chinese page
function main_top_button_div_show_scroll()
{// for pages have top button
        if( isIE_or_Opera() && IE_version() < 9 )
        {
                var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
                //51: page_title height: 20, help height:30; top button: 30 add 1 px change
                document.getElementById("main").style.height= frame_height-81;
        }
}

//38280 for USB basic and USB advanced page
function main_div_show_scroll()
{// for pages have page_title, but not have top button, 
        if( isIE_or_Opera() && IE_version() < 9 )
        {
                var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		//51: page_title height: 20, help height:30;  add 1 px change
                document.getElementById("main").style.height= frame_height-51; 
        }
}

function IE_RST_status()
{
	if( isIE_or_Opera() && IE_version() < 9 )
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		document.getElementById("content_icons").style.height= frame_height;
	}
}

function Not_IE_show_hidden_help(help_flag)
{
	if(isIE() == true)
	{
		document.getElementById("help").style.height="120px";
		document.getElementById("helpframe").style.height="98%";
	}
	if( help_flag % 2 == 0 )
	{
		document.getElementById("main").style.bottom="150px";
		document.getElementById("help").style.display="";
		document.getElementById("help_switch").className="open_help";
	}
	else
	{
		document.getElementById("main").style.bottom="30px";
		document.getElementById("help").style.display="none";
		document.getElementById("help_switch").className="close_help";
	}
}

function show_hidden_help_green(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >= 9)
		Not_IE_show_hidden_help(help_flag);
	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if(!frame_height) frame_height= top.document.getElementById("formframe_div").clientHeight;
		var top_height = document.getElementById("top").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=(frame_height-150)+"px";//30+120
			document.getElementById("buttom").style.height=(frame_height-150-top_height)+"px";
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=(frame_height-30)+"px";//20+30
			document.getElementById("buttom").style.height=(frame_height-30-top_height)+"px";
		}
	}
}

function show_hidden_help(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >= 8)
		Not_IE_show_hidden_help(help_flag);
	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=frame_height-170;//20+30+120
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=frame_height-50;//20+30
		}
	}
}

function show_hidden_help_top_button(help_flag)
{
	if(!isIE_or_Opera() || IE_version() >=8)
		Not_IE_show_hidden_help(help_flag);
	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=frame_height-200;//50+30+120
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=frame_height-80;//50+30
		}
	}
}

function port_range_interception(port)
{
	port=parseInt(port,10);
	return port;
}

function address_parseInt(addr)/*to fix bug 26258*/
{
	if( "" == addr )
		return addr;

	addr_array = addr.split(".");
	for(i=0; i< 4; i++)
	{
		if( "" == addr_array[i] )
			addr_array[i] = 0;
		if( 0 == i )
			addr = parseInt(addr_array[i], 10);
		else
			addr = addr + "." +parseInt(addr_array[i], 10);
	}
	return addr;
}

function mtu_change(wanpro)
{
	var cf=document.forms[0];//bug 23492:'wan_mtu_now' is a number not a string,so delete '"'
	if (old_wan_type=="dhcp" && wan_mtu_now!=1500)
	{	//bug 23546 [L2TP]L2TP MTU Should be 1428
		if((wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="pptp" && wan_mtu_now!=1436)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="pppoe" && wan_mtu_now!=1492)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="mulpppoe1" && wan_mtu_now!=1454)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="bigpond" && wan_mtu_now!=1500)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="static" && wan_mtu_now!=1500)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="l2tp" && wan_mtu_now!=1428)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;	
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="3g" && wan_mtu_now!=1448)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="lte" && wan_mtu_now<1486))
			cf.hid_mtu_value.value=wan_mtu_now;
		else
			default_mtu(wanpro);
	}
	else if(old_wan_type=="lte" && wan_mtu_now!=1486)
	{
		if((wanpro=="dhcp" && wan_mtu_now<1500) || (wanpro=="pptp" && wan_mtu_now<1436) || (wanpro=="l2tp" && wan_mtu_now<1428) || (wanpro=="mulpppoe1" && wan_mtu_now<1454) || (wanpro=="pppoe" && wan_mtu_now<1492) || (wanpro=="bigpond" && wan_mtu_now<1500) || (wanpro=="3g" && wan_mtu_now<1448))
			cf.hid_mtu_value.value=wan_mtu_now;
		else
			default_mtu(wanpro);
	}
	else
		default_mtu(wanpro);
}

function default_mtu(wanpro)
{
	var cf=document.forms[0];
	if(wanpro=="pptp")
		cf.hid_mtu_value.value="1436";
	else if(wanpro=="pppoe")
		cf.hid_mtu_value.value="1492";
	else if(wanpro=="mulpppoe1")
		cf.hid_mtu_value.value="1454";
	else if(wanpro=="l2tp")
		cf.hid_mtu_value.value="1428";
	else if(wanpro=="lte")
		cf.hid_mtu_value.value="1486";
	else if(wanpro=="3g")
		cf.hid_mtu_value.value="1448";
	else
		cf.hid_mtu_value.value="1500";
}

function check_DNS(dnsaddr1,dnsaddr2,wan_assign,wan_ip)
{
        if(dnsaddr1!="")
        {
                if(checkipaddr(dnsaddr1)==false)
                {
                        alert("$invalid_primary_dns");
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr1,wan_ip))
                {
                        alert("$invalid_primary_dns");
                        return false;
                }
        }
        if(dnsaddr2!="")
        {
                if(checkipaddr(dnsaddr2)==false)
                {
                        alert("$invalid_second_dns");
                        return false;
                }
                if( wan_assign == true && isSameIp(dnsaddr2,wan_ip))
                {
                        alert("$invalid_second_dns");
                        return false;
                }
        }

        if(dnsaddr1=="" && dnsaddr2=="" )
        {
                alert("$dns_must_specified");
                return false;
        }
        return true;
}

function check_RU_DNS(dnsaddr1,dnsaddr2,wan_assign,wan_ip)
{
	if(dnsaddr1!="")
	{
		if(checkipaddr(dnsaddr1)==false)
		{
			alert(bh_invalid_primary_dns);
			return false;
		}
		if( wan_assign == true && isSameIp(dnsaddr1,wan_ip))
		{
			alert(bh_invalid_primary_dns);
			return false;
		}
	}
	if(dnsaddr2!="")
	{
		if(checkipaddr(dnsaddr2)==false)
		{
			alert(bh_invalid_second_dns);
			return false;
		}
		if( wan_assign == true && isSameIp(dnsaddr2,wan_ip))
		{
			alert(bh_invalid_second_dns);
			return false;
		}
	}

	if(dnsaddr1=="" && dnsaddr2=="" )
	{
		alert(bh_dns_must_specified);
		return false;
	}
	return true;
}

/*
        Saturday, Jan 01, 2000 10:51:13
*/
function change_cur_time(old_str,gui_region)
{
        each_str=old_str.split(', ');

        week=each_str[0];
        show_week=change_full_week(week);

        date_info=each_str[1].split(' ');
        month=date_info[0];
        show_month=change_month(month);
        day=date_info[1];

        time=each_str[2];

        if( gui_region == "German" )
                new_str = show_week+', '+day+'.'+show_month+', '+time;
        else
                new_str = show_week+', '+show_month+' '+day+', '+time;

        return new_str;
}

function change_week(week)
{
        var show_week;
        if(week == "Mon")
                show_week="$schedule_mon";
        else if(week == "Tue")
                show_week="$schedule_tue";
        else if(week == "Wed")
                show_week="$schedule_wed";
        else if(week == "Thu")
                show_week="$schedule_thu";
        else if(week == "Fri")
                show_week="$schedule_fri";
        else if(week == "Sat")
                show_week="$schedule_sat";
        else if(week == "Sun")
                show_week="$schedule_sun";
        else
                show_week=week;

        return show_week;
}

function change_full_week(week)
{
        var show_week;
        if(week == "Monday")
                show_week="$schedule_mon";
        else if(week == "Tuesday")
                show_week="$schedule_tue";
        else if(week == "Wednesday")
                show_week="$schedule_wed";
        else if(week == "Thursday")
                show_week="$schedule_thu";
        else if(week == "Friday")
                show_week="$schedule_fri";
        else if(week == "Saturday")
                show_week="$schedule_sat";
        else if(week == "Sunday")
                show_week="$schedule_sun";
        else
                show_week=week;

        return show_week;
}

function change_month(month)
{
        var show_month;
        if( month == "Jan" )
                show_month="$January";
        else if( month == "Feb" )
                show_month="$February";
        else if( month == "Mar" )
                show_month="$March";
        else if( month == "Apr" )
                show_month="$April";
        else if( month == "May" )
                show_month="$May";
        else if( month == "Jun" )
                show_month="$June";
        else if( month == "Jul" )
                show_month="$July";
        else if( month == "Aug" )
                show_month="$August";
        else if( month == "Sep" )
                show_month="$September";
        else if( month == "Oct" )
                show_month="$October";
        else if( month == "Nov" )
                show_month="$November";
        else if( month == "Dec" )
                show_month="$December";
        else
                show_month=month;
        return show_month;
}

/* input ip, if input length = 3 or input '.' , change focus to next text, */
function SelectionTextLength(text)
{
	var select_text="";
	if (document.selection&& document.selection.createRange)//IE
		select_text=document.selection.createRange().text;
	else if(select_text == "" && text.selectionStart != undefined )
		select_text=text.value.substring(text.selectionStart,text.selectionEnd);
	return select_text.length;
}
function keydown(e,text)
{	
	if((e.keyCode == 190 || e.keyCode == 110) && text.value.length !=0 && SelectionTextLength(text)==0)
		text.form[(getIndex(text)+1) % text.form.length].focus();	
}
function keyup(e,text)
{
	if(text.value.length == 3 && ((e.keyCode >47 && e.keyCode <58) ||(e.keyCode >95 && e.keyCode <106)))
		text.form[(getIndex(text)+1) % text.form.length].focus();	
}
function keyup_n(e,text,num)
{
	if(text.value.length == num && (e.keyCode != 190 && e.keyCode != 110 && e.keyCode != 9))
		text.form[(getIndex(text)+1) % text.form.length].focus();
}
function getIndex(input) {
    	var index = -1;
	var i = 0;
	var found = false;
    	while (i < input.form.length && index == -1)
    		if (input.form[i] == input)index = i;
    			else i++;
    	return index;
}

function getObj(name)
{
	if (document.getElementById)
	{
		return document.getElementById(name);
	}
	else if (document.all)
	{
		return document.all[name];
	}
	else if (document.layers)
	{
		return document.layers[name];
	}
}

function check_maxNumAdd(form,nowNum,maxNum,msg,go2href)
{
	nowNum=parseInt(nowNum,10);
	if (nowNum >= maxNum)
	{
		alert(msg);
		return false;
	}
	else
		location.href=go2href;
}

function checkIPMain(ip,max) 
{
	if (isNumeric(ip, max) == false) 
		return false;
}

function checkIP(ip1, ip2, ip3, ip4,max) 
{
	if(checkIPMain(ip1,255)==false) return false; 
	if(checkIPMain(ip2,255)==false) return false;
	if(checkIPMain(ip3,255)==false) return false;
	if(checkIPMain(ip4,max)==false) return false;
	//if((parseInt(ip1)==0)||(parseInt(ip1)==0)&&(parseInt(ip2)==0)&&(parseInt(ip3)==0)&&(parseInt(ip4)==0))
	if((parseInt(ip1)==0)&&(parseInt(ip2)==0)&&(parseInt(ip3)==0)&&(parseInt(ip4)==0)) //for bug 23797,the line above is Semantic error
		return false;
	if(parseInt(ip1)==0)
		return false;
	return true;
}

/* Check Numeric*/
function isNumeric(str, max) 
{
	if(str.length == 0 || str == null || str == "") 
		return false;
	for(i=0; i<str.length; i++) 
	{
		var c = str.substring(i, i+1);
		if("0" <= c && c <= "9") 
			continue;
		else
			return false;
	}
	var i = parseInt(str);
	if(i>max) 
		return false;

	return true;
}

function isIE()
{
	var browser = new Object();
	browser.version = parseInt(navigator.appVersion);
	browser.isNs = false;
	browser.isIe = false;
	if(navigator.appName.indexOf("Microsoft") != -1 || navigator.userAgent.indexOf("Trident") != -1)
		browser.isIe = true;
	else if(navigator.appName.indexOf("Netscape") != -1 )
		browser.isNs = true;
	if(browser.isNs)
		return false;
	else if (browser.isIe)
		return true;
	else
		return false;
}

function get_browser()
{
	if (!!window.ActiveXObject || "ActiveXObject" in window)
		return "IE";
	else if(navigator.userAgent.indexOf("Chrome") != -1 )
		return "Chrome";//bug 21975:spec1.9-p228,[usb] the real links are different for different browsers
	else if(navigator.userAgent.indexOf("Firefox") != -1)
		return "Firefox";
	else if(navigator.userAgent.indexOf("Safari") != -1 )
		return "Safari";
	else if(navigator.userAgent.indexOf("Camino") != -1) 
		return "Camino"; 
 	else if(navigator.userAgent.indexOf("Gecko/") != -1)
   		return "Gecko"; 
	else if(navigator.userAgent.indexOf("Opera") != -1)
		return "Opera";
	else
   		return "";		
}

function isIE_or_Opera()
{
	var browser = get_browser();
	if( browser == "IE" || browser == "Opera" )
		return true;
	else
		return false;
}
function IE_version()
{
	var Sys = {};
	var ua = navigator.userAgent.toLowerCase();
	var s;
	(s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :0;
	if(Sys.ie == '6.0')
		return 6;
	else if(Sys.ie == '7.0')
		return 7;
	else if(Sys.ie == '8.0')
		return 8;
	else if(Sys.ie == '9.0')
		return 9;
	else if(Sys.ie == '10.0')
		return 10;
	else
		return 11;
}

function _isNumeric(str) {
	var i;
	for(i = 0; i<str.length; i++) {
		var c = str.substring(i, i+1);
		if("0" <= c && c <= "9") {
			continue;
		}
		return false;
	}
	return true;
}

function isSameIp(ipstr1,ipstr2)
{
	var count = 0;
	var ip1_array=ipstr1.split('.');
	var ip2_array=ipstr2.split('.');
	for(i = 0;i<4;i++)
	{
		num1 = parseInt(ip1_array[i], 10);
		num2 = parseInt(ip2_array[i], 10);
		if( num1 == num2)
			count++;
	}
	if( count == 4)
		return true;
	else
		return false;
}

function cp_ip2(from,to)
	//true invalid from and to ip;  false valid from and to ip;
{
	var total1 = 0;
	var total2 = 0;
	var from_array=from.split('.');
	var to_array = to.split('.');
	var from1=from_array[0];
	var to1=to_array[0];
	if(parseInt(from1,10) <= 127 && parseInt(to1,10) >= 127 )
	{
		return false;
	}
	var from2=from_array[1];
	var from3=from_array[2];
	var from4=from_array[3];
	var to2=to_array[1];
	var to3=to_array[2];
	var to4=to_array[3];

	total1 += parseInt(from4,10);
	total1 += parseInt(from3,10)*256;
	total1 += parseInt(from2,10)*256*256;
	total1 += parseInt(from1,10)*256*256*256;

	total2 += parseInt(to4,10);
	total2 += parseInt(to3,10)*256;
	total2 += parseInt(to2,10)*256*256;
	total2 += parseInt(to1,10)*256*256*256;
	if(total1 <= total2)
		return true;
	return false;
}
 
function is_sub_or_broad(be_checkip, ip, mask)
{
        addr_arr = be_checkip.split('.');
        var ip_addr=0;

	if(mask == "255.255.255.255")
		return true;

        for (i = 0; i < 4; i++)
        {
                addr_str = parseInt(addr_arr[i],10);
                ip_addr=ip_addr*256+parseInt(addr_str);
        }
        var ip_sub=isSub(ip, mask);
        var ip_broadcast=isBroadcast(ip, mask)
	if(ip_addr == ip_sub || ip_addr == ip_broadcast){
		return false;
	}
	return true;
}

function isGateway(lanIp, lanMask, gtwIp)
{
	gtw_arr = gtwIp.split('.');
        var ip_gtw=0;
        for (i = 0; i < 4; i++)
        {
	        gtw_str = parseInt(gtw_arr[i],10);
                ip_gtw=ip_gtw*256+parseInt(gtw_str);
        }

        addr_arr = lanIp.split('.');
        var ip_addr=0;
        for (i = 0; i < 4; i++)
        {
                addr_str = parseInt(addr_arr[i],10);
                ip_addr=ip_addr*256+parseInt(addr_str);
        }

        var ip_sub=isSub(lanIp, lanMask);
        var ip_broadcast=isBroadcast(lanIp, lanMask)
        if((parseInt(ip_sub)<parseInt(ip_gtw))&&(parseInt(ip_gtw)<parseInt(ip_broadcast)))
	        return true;
        else
                return false;
}

function isSub(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_sub=0
        for (i = 0; i < 4; i++)
        {
    	        ip_str = parseInt(ip_arr[i],10);
                mask_str = parseInt(mask_arr[i],10);
                ip_sub=ip_sub*256+parseInt(ip_str & mask_str)
        }
        return(ip_sub);
}

function isBroadcast(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_broadcast=0
        for (i = 0; i < 4; i++)
        {
                ip_str = parseInt(ip_arr[i],10);
                mask_str = parseInt(mask_arr[i],10);
                n_str = ~mask_str+256;
                ip_broadcast=ip_broadcast*256+parseInt(ip_str | n_str)
        }
        return(ip_broadcast);
}

function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask) 
{
	var count = 0;
	lan1a = lan1Ip.split('.');
	lan1m = lan1Mask.split('.');
	lan2a = lan2Ip.split('.');
	lan2m = lan2Mask.split('.');
	for (i = 0; i < 4; i++) 
	{
		l1a_n = parseInt(lan1a[i],10);
		l1m_n = parseInt(lan1m[i],10);
		l2a_n = parseInt(lan2a[i],10);
		l2m_n = parseInt(lan2m[i],10);
		if ((l1a_n & l1m_n) == (l2a_n & l2m_n))
			count++;
	}
	if (count == 4)
		return true;
	else
		return false;
}
function checkipaddr(ipaddr)
{
	var form = document.forms[0];
	var ipArray = ipaddr.split(".");
	var ipstr = ipArray[0]+ipArray[1]+ipArray[2]+ipArray[3];
	var i = 0;

	if((ipArray[0]=="")||(ipArray[0]<0)||(ipArray[0]>255)||(ipArray[1]=="")||(ipArray[1]<0)||(ipArray[1]>255)
			||(ipArray[2]=="")||(ipArray[2]<0)||(ipArray[2]>255)||(ipArray[3]=="")||(ipArray[3]<0)||(ipArray[3]>255))
	{
		return false;
	}
	for(i=0;i<ipstr.length;i++)
	{
		if((ipstr.charAt(i)!='0')&&(ipstr.charAt(i)!='1')&&(ipstr.charAt(i)!='2')
				&&(ipstr.charAt(i)!='3')&&(ipstr.charAt(i)!='4')&&(ipstr.charAt(i)!='5')
				&&(ipstr.charAt(i)!='6')&&(ipstr.charAt(i)!='7')&&(ipstr.charAt(i)!='8')
				&&(ipstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if( ipArray[0] > 223 || ipArray[0] == 0 )
		return false;
	if (ipaddr == "0.0.0.0" || ipaddr == "255.255.255.255")
	{
		return false;
	}

	var each=ipaddr.split(".");
	if (each[0] == "127")
	{
		return false;
	}
	if (!ipArray || ipArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			thisSegment = ipArray[i];
			if (thisSegment != "")
			{
				if(i==3){
					if (!((ipArray[3] > 0) && (ipArray[3] < 255)))
					{
						return false;
					}	
				}
				else if (!(thisSegment >=0 && thisSegment <= 255))
				{
					return false;
				}
			} 
			else
			{
				return false;
			}
		}

	}
	return true;
}
//sometiems it is true to set the netmask 255.255.255.255, so we set a flag for this case
function checksubnet(subnet, flag)
{
	var subnetArray = subnet.split(".");
	var subnetstr = subnetArray[0]+subnetArray[1]+subnetArray[2]+subnetArray[3];
	var i = 0;
	var maskTest = 0;
	var validValue = true;

	if(flag == 0)
	{
		/* to fix bug 23900, if user enter "255.255.255.255" as subnet mask return false.*/
		if(subnetstr == "255255255255")
			return false;
	}

	if((subnetArray[0]=="")||(subnetArray[0]<0)||(subnetArray[0]>255)||(subnetArray[1]=="")||(subnetArray[1]<0)||(subnetArray[1]>255)
			||(subnetArray[2]=="")||(subnetArray[2]<0)||(subnetArray[2]>255)||(subnetArray[3]=="")||(subnetArray[3]<0)||(subnetArray[3]>255))
	{
		return false;
	}
	for(i=0;i<subnetstr.length;i++)
	{
		if((subnetstr.charAt(i)!='0')&&(subnetstr.charAt(i)!='1')&&(subnetstr.charAt(i)!='2')
				&&(subnetstr.charAt(i)!='3')&&(subnetstr.charAt(i)!='4')&&(subnetstr.charAt(i)!='5')
				&&(subnetstr.charAt(i)!='6')&&(subnetstr.charAt(i)!='7')&&(subnetstr.charAt(i)!='8')
				&&(subnetstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if (!subnetArray || subnetArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++) {
			thisSegment = subnetArray[i];
			if (thisSegment != "") {
				if (!(thisSegment >=0 && thisSegment <= 255)) { //check if number?

					return false;
				}
			} else {
				return false;
			}
		}
	}
	if( subnetArray[0] < 255 ) 
	{
		if( (subnetArray[1] > 0) || (subnetArray[2] > 0) || (subnetArray[3] > 0))
			validValue = false;
		else
			maskTest = subnetArray[0];
	} 
	else 
	{
		if( subnetArray[1] < 255 ) 
		{
			if( (subnetArray[2] > 0) || (subnetArray[3] > 0))
				validValue = false;
			else
				maskTest = subnetArray[1];
		} 
		else
		{
			if( subnetArray[2] < 255 ) 
			{
				if( (subnetArray[3] > 0) )
					validValue = false;
				else
					maskTest = subnetArray[2];
			} 
			else
				maskTest = subnetArray[3];
		}
	}
	if( validValue ) {
		switch( maskTest ) {
			case "0":
			case "00":
			case "000":
				case "128":
				case "192":
				case "224":
				case "240":
				case "248":
				case "252":
				case "254":
				case "255":
				case "000":  //bug23797, for input 000
				break;
			default:
			validValue = false;
		}
		if( subnet == "0.0.0.0" )
			validValue = false;
	}
	else
		validValue = false;


	return validValue;
}

function ipNum(ipStr)/* to fix bug 25203 */
{
	var total = 0;
	var ip_array=ipStr.split('.');
	var ip1=ip_array[0];
	var ip2=ip_array[1];
	var ip3=ip_array[2];
	var ip4=ip_array[3];
	total += parseInt(ip4);
	total += parseInt(ip3)*256;
	total += parseInt(ip2)*256*256;
	total += parseInt(ip1)*256*256*256;
	total = parseInt(total);
	return total;
}

function isIncludeInvIp(inv_ip, start_ip, end_ip)/* to fix bug 25203 */
{
	inv_ip_num = ipNum(inv_ip);
	start_ip_num = ipNum(start_ip);
	end_ip_num = ipNum(end_ip);

	if( inv_ip_num >= start_ip_num && inv_ip_num <= end_ip_num)
		return true;

	return false;
}

function checkgateway(gateway)
{
	var form = document.forms[0];
	var dgArray = gateway.split(".");
	var dgstr = dgArray[0]+dgArray[1]+dgArray[2]+dgArray[3];
	var i = 0;	

	if((dgArray[0]=="")||(dgArray[0]<0)||(dgArray[0]>255)||(dgArray[1]=="")||(dgArray[1]<0)||(dgArray[1]>255)
			||(dgArray[2]=="")||(dgArray[2]<0)||(dgArray[2]>255)||(dgArray[3]=="")||(dgArray[3]<0)||(dgArray[3]>255))
	{
		return false;
	}	
	for(i=0;i<dgstr.length;i++)
	{
		if((dgstr.charAt(i)!='0')&&(dgstr.charAt(i)!='1')&&(dgstr.charAt(i)!='2')
				&&(dgstr.charAt(i)!='3')&&(dgstr.charAt(i)!='4')&&(dgstr.charAt(i)!='5')
				&&(dgstr.charAt(i)!='6')&&(dgstr.charAt(i)!='7')&&(dgstr.charAt(i)!='8')
				&&(dgstr.charAt(i)!='9'))
		{
			return false;
		}
	}
	if( dgArray[0] > 223 || dgArray[0] == 0 )
		return false;
	if (gateway == "0.0.0.0" || gateway == "255.255.255.255")
	{
		return false;
	}
	if (gateway == "127.0.0.1")
	{
		return false;
	}
	if (!dgArray || dgArray.length != 4)
	{
		return false;
	}
	else
	{
		for (i = 0; i < 4; i++) {
			thisSegment = dgArray[i];
			if (thisSegment != "") 
			{
				if (!(thisSegment >=0 && thisSegment <= 255)) 
				{ //check if number?
					return false;
				}
			} 
			else 
			{
				return false;
			}
		}
	}
	return true;
}
function loadhelp(fname,anchname)
{
	var page="/help/"+fname+"_help.html";
	
	if(this.help_iframe)
	{
		this.help_iframe.location.href=page+"#"+ anchname;
		this.help_iframe.location.href=page+"#"+ anchname; // to fix bug 29847.
	}
	else if(parent.help_iframe)     //dowmload manager
	{
		page=parent.help_iframe.location.href.split("#")[0];
		parent.help_iframe.location.href=page+"#"+ anchname;
		parent.help_iframe.location.href=page+"#"+ anchname;
	}
	return;
}

function getkeya(e)
{
	var keycode;
	if (window.event) 
	{
		keycode = window.event.keyCode;
		if (((keycode>47) && (keycode<58))||(keycode==8)||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))) { return true; }
		else return false;
	}
	else if (e) 
	{
		keycode = e.which;
		if (((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))) { return true; }
		else return false;
	}
	else 
	{
		return true;
	}
}
function getkey(type, e)
{
	var keycode, event;

	if (window.event) 
	{
		event = window.event;
		keycode = window.event.keyCode;
	}	
	else if (e)
	{
		event = e;
		keycode = e.which;
	}	
	else return true;

	if(event.ctrlKey && ( keycode==99 || keycode==118 || keycode==120) )
		return true;

	if(type == "apname")
	{
		if ((keycode==34) ||(keycode==39)||(keycode==92)) { return false; }
		else return true;
	}
	else if(type == "ipaddr")
	{	
		if (((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||(keycode==46)) { return true; }
		else return false;
	}
	else if(type == "ssid")
	{
		if (keycode==32) return false;
		else return true;
	}
	else if(type == "wep")
	{
		if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
			return true;
		else return false;
	}
	else if(type == "num-")	
	{	
		if	(((keycode>47) && (keycode<56)) || (keycode==8)||(keycode==0))
			return true;		
		else return false;	
	}
	else if(type == "num")	
	{	
		if	(((keycode>47) && (keycode<58)) || (keycode==8)||(keycode==0))
			return true;		
		else return false;	
	}else if(type == "wps_pin")
	{
		if(((keycode>47) && (keycode<58)) || (8==keycode)||(0==keycode)||(32==keycode)||(45==keycode)) //38293
			return true;
		else return false;
	}
	else if(type == "num_letter")
	{
		if((keycode>47 && keycode<58)||(keycode>64 && keycode<91)||(keycode>96 && keycode<123)||(keycode==8)||(keycode==0))
			return true; 
		else
			return false
	}
	else if(type == "hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "ddns_hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||(keycode==46)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "mac")
	{
		if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0) || (keycode == 58) || (keycode == 45))
			return true;
		else return false;
	}
	else if(type == "folderPath")// not / * ? " <>
	{
		if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34) || (keycode==60) || (keycode==62) ||(keycode==124))
			return false;
		else 
			return true;
	}
	else if(type == "shareName")// not / * ? " <> \ : 
	{
		if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34) || (keycode==58) || (keycode==60) || (keycode==62) || (keycode==92) || (keycode==93) || (keycode==124))
			return false;
		else
			return true;
	}
	else if(type == "deviceName")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
        else if(type == "mediaServerName")// not / * ? " <> \ :
        {
                if((keycode==47) || (keycode==42) || (keycode==63) || (keycode==34)  || (keycode==60) || (keycode==62)  || (keycode==92) || (keycode==93) || (keycode==124)){
			alert("$media_server_name_colon");  // || (keycode==58)
                        return false;
		}	
                else
                        return true;
        }
	else if(type == "colon_num")
        {
                if(((keycode>47) && (keycode<58)) || (keycode==46) || (keycode==8)||(keycode==0))
                        return true;
                else return false;
        }
	else	
		return false;
}



function changesectype(fname)
{
	var html_href;
	if(fname.options[0].selected == true) html_href = "security_off.asp";
	else if(fname.options[1].selected == true) html_href = "wep.asp";
	else if(fname.options[2].selected == true) html_href = "wpa.asp";
	else if(fname.options[3].selected == true) html_href = "wpa2.asp";
	else if(fname.options[4].selected == true) html_href = "wpas.asp";

	location.href = html_href;
}

function printableKeyFilter() 
{
	if (event.keyCode < 32 || event.keyCode > 126)
		event.returnValue = false;
}

function checkpsk(passphrase, passphrase_len)
{
	var cd_less_flag = 0;
	if(top.location.href.search("BRS_index.htm") != -1)
		cd_less_flag = 1;
	var len = passphrase.value.length;
	if ( len == 64 )
	{
		for(i=0;i<len;i++)
		{
			if(isValidHex(passphrase.value.charAt(i))==false)
			{
				if(cd_less_flag == 1)
					alert(notallowpassps);
				else
					alert("$notallowpassps");
				return false;
			}
		}
	}
	else
	{
		if(len < 8 )
		{
			if(cd_less_flag == 1)
				alert(passphrase_short8);
			else
				alert("$passphrase_short8");
		        return false;
		} 
		if(len > 63){
			if(cd_less_flag == 1)
				alert(passphrase_long63);
			else
				alert("$passphrase_long63");
			return false;
		}
		for(i=0;i<passphrase.value.length;i++)
		{
			if(isValidChar_space(passphrase.value.charCodeAt(i))==false)
			{
				if(cd_less_flag == 1)
					alert(notallowpassps);
				else
					alert("$notallowpassps");
				return false;
			}
		}
	}
	passphrase_len.value=len;
	return true;
}

var wep_128="$wep_128";
var wep_64="$wep_64";

function show_alert(wep_bit)
{
	if(wep_bit == 10)
		alert(wep_64);
	else if(wep_bit == 26)
		alert(wep_128);
}
function check_key(key,wep_bit)
{
	var ascii_len;
	var i;

	if(wep_bit == 10)
		ascii_len = 5;
	else if(wep_bit == 26)
		ascii_len = 13;

	if(key.length == wep_bit)
	{
		for(i=0;i<key.length;i++)
		{
			if(isValidHex(key.charAt(i))==false)
			{
				show_alert(wep_bit);
				return false;
			}
		}
	}
	else
	{
		if(key.length != 0 && key.length != wep_bit && key.length != ascii_len)
		{
			show_alert(wep_bit);
			return false;
		}

		for(i=0;i<key.length;i++)
		{
			if(isValidChar_space(key.charCodeAt(i))==false)
			{
				show_alert(wep_bit);
				return false;
			}
		}
	}
	return true;
}

function checkwep(form)
{
	form.wep_press_flag.value=0;
	var KEY1=form.KEY1.value;
	var KEY2=form.KEY2.value;
	var KEY3=form.KEY3.value;
	var KEY4=form.KEY4.value;

	if(form.wepenc.value==13)
	{
		if( form.wep_key_no[0].checked == true )
		{
			if(form.KEY1.value.length!=26 && form.KEY1.value.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY1,26) == false)
			return false;

		if( form.wep_key_no[1].checked == true )
		{
			if(form.KEY2.value.length!=26 && form.KEY2.value.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}

		if(check_key(KEY2,26) == false)
			return false;

		if( form.wep_key_no[2].checked == true)
		{
			if(form.KEY3.value.length!=26 && form.KEY3.value.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY3,26) == false)
			return false;

		if( form.wep_key_no[3].checked == true)
		{
			if(form.KEY4.value.length!=26 && form.KEY4.value.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY4,26) == false)
			return false;
	}
	if(form.wepenc.value==5)
	{
		if( form.wep_key_no[0].checked == true)
		{
			if(form.KEY1.value.length!=10 && form.KEY1.value.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY1,10) == false)
			return false;

		if( form.wep_key_no[1].checked == true)
		{
			if(form.KEY2.value.length!=10 && form.KEY2.value.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY2,10) == false)
			return false;

		if( form.wep_key_no[2].checked == true)
		{
			if(form.KEY3.value.length!=10 && form.KEY3.value.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY3,10) == false)
			return false;

		if( form.wep_key_no[3].checked == true)
		{
			if(form.KEY4.value.length!=10 && form.KEY4.value.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY4,10) == false)
			return false;
	}	
	return true;
}

function checkwep_a(form)
{
	form.wep_press_flag.value=0;
	var KEY1=form.KEY1_an.value;
	var KEY2=form.KEY2_an.value;
	var KEY3=form.KEY3_an.value;
	var KEY4=form.KEY4_an.value;

	if(form.wepenc_an.value==13)
	{
		if( form.wep_key_no_an[0].checked == true )
		{
			if(KEY1.length!=26 && KEY1.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}	
		if(check_key(KEY1,26) == false)
			return false;

		if( form.wep_key_no_an[1].checked == true )
		{
			if(KEY2.length!=26 && KEY2.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY2,26) == false)
			return false;

		if( form.wep_key_no_an[2].checked == true)
		{
			if(KEY3.length!=26 && KEY3.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY3,26) == false)
			return false;

		if( form.wep_key_no_an[3].checked == true)
		{
			if(KEY4.length!=26 && KEY4.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY4,26) == false)
			return false;

	}
	if(form.wepenc_an.value==5)
	{
		if( form.wep_key_no_an[0].checked == true)
		{
			if(KEY1.length!=10 && KEY1.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY1,10) == false)
			return false;

		if( form.wep_key_no_an[1].checked == true)
		{
			if(KEY2.length!=10 && KEY2.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY2,10) == false)
			return false;

		if( form.wep_key_no_an[2].checked == true)
		{
			if(KEY3.length!=10 && KEY3.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY3,10) == false)
			return false;

		if( form.wep_key_no_an[3].checked == true)
		{
			if(KEY4.length!=10 && KEY4.length!=5)
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY4,10) == false)
			return false;
	}	
	return true;
}

/////////////////////////////////////generate wep key by md5////////////////////////////////////////////////////
function HexToAscii(wepenc,wep_key_no,I,S,D) {
	var temp1="";

	S = S.toUpperCase();

	var optionindex=wepenc.selectedIndex;

	if( wepenc.options[optionindex].value=="13" )
	{
		wordCount = 26;
	}
	else {
		wordCount = 10;
	}

	//if(F.keyno_11g[I].checked)
	if(1)
	{
		if( (S.length!=wordCount) )
		{
			if(wep_key_no[I].checked)
			{
				var s="Hex type key length must be ";
				alert(s + wordCount);
			}
			D.value="";
			S="";
			return S;
		}
		for(i=0;i<wordCount;i+=2)
		{
			var c=S.charCodeAt(i);
			var d=S.charCodeAt(i+1);

			if( (c>=48)&&(c<=57) )
				c=c-48;
			else if( (c>=65)&&(c<=70) )
				c=c-55;
			else
			{
				var s="Over Hex range (0~F)";
				alert(s);
				return S;
			}

			if( (d>=48)&&(d<=57) )
				d=d-48;
			else if( (d>=65)&&(d<=70) )
				d=d-55;
			else
			{
				var s="Over Hex range (0~F)";
				alert(s);
				return S;
			}
			var value=c*16+d;

			if( ((value>=0)&&(value<32)) || ((value>128)&&(value<=255)) )
			{
				temp1+=String.fromCharCode(92);
				temp1+=S.substring(i,i+2);

			}
			else
			{
				if(value==92)
				{
					temp1+=String.fromCharCode(value);
					temp1+=String.fromCharCode(value);
				}
				else
					temp1+=String.fromCharCode(value);
			}
		}
		D.value=temp1;
	}
	return S;
}

function PassPhrase40(passphraseStr, wepenc, wep_key_no, KEY1, KEY2, KEY3, KEY4)
{
	var seed = 0;
	var pseed = new Array(0, 0, 0, 0);
	var pkey = new Array(4);
	var asciiObj = new Array(4);
	Length = passphraseStr.value.length;

	if(Length != 0) {
		for (i=0; i<Length; i++ ) {
			pseed[i%4] ^= passphraseStr.value.charCodeAt(i);
		}
		seed = pseed[0];
		seed += pseed[1] << 8;
		seed += pseed[2] << 16;
		seed += pseed[3] << 24;
	}

	KEY1.value = KEY2.value = "";
	KEY3.value = KEY4.value = "";

	// init key array
	pkey[0] = KEY1;
	pkey[1] = KEY2;
	pkey[2] = KEY3;
	pkey[3] = KEY4;

	for(j=0; j<4; j++) {
		for (i=0; i<5 ;i++ )  {
			seed = (214013 * seed) & 0xffffffff;

			if(seed & 0x80000000) {
				seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
			}
			else {
				seed = (seed & 0x7fffffff) + 0x269ec3;
			}

			temp = ((seed >> 16) & 0xff);
			if(temp < 0x10) {
				pkey[j].value += "0" + temp.toString(16).toUpperCase();
			}
			else {
				pkey[j].value += temp.toString(16).toUpperCase();
			}
		}
	}

	asciiObj[0] = "";
	asciiObj[1] = "";
	asciiObj[2] = "";
	asciiObj[3] = "";

	for(k=0; k<4; k++) {
		HexToAscii(wepenc, wep_key_no, k, pkey[k].value, asciiObj[k]);
	}

	wepkey1 = pkey[0].value;
	wepkey2 = pkey[1].value;
	wepkey3 = pkey[2].value;
	wepkey4 = pkey[3].value;
}

/*
 * A JavaScript implementation of the RSA Data Security, Inc. MD5 Message
 * Digest Algorithm, as defined in RFC 1321.
 * Copyright (C) Paul Johnston 1999 - 2000.
 * Updated by Greg Holt 2000 - 2001.
 * See http://pajhome.org.uk/site/legal.html for details.
 */

/*
 * Convert a 32-bit number to a hex string with ls-byte first
 */
var hex_chr = "0123456789abcdef";
function rhex(num)
{
	str = "";
	for(j = 0; j <= 3; j++)
		str += hex_chr.charAt((num >> (j * 8 + 4)) & 0x0F) +
			hex_chr.charAt((num >> (j * 8)) & 0x0F);
	return str;
}

/*
 * Convert a string to a sequence of 16-word blocks, stored as an array.
 * Append padding bits and the length, as described in the MD5 standard.
 */
function str2blks_MD5(str)
{
	nblk = ((str.length + 8) >> 6) + 1;
	blks = new Array(nblk * 16);
	for(i = 0; i < nblk * 16; i++) blks[i] = 0;
	for(i = 0; i < str.length; i++)
		blks[i >> 2] |= str.charCodeAt(i) << ((i % 4) * 8);
	blks[i >> 2] |= 0x80 << ((i % 4) * 8);
	blks[nblk * 16 - 2] = str.length * 8;
	return blks;
}

/*
 * Add integers, wrapping at 2^32. This uses 16-bit operations internally 
 * to work around bugs in some JS interpreters.
 */
function add(x, y)
{
	var lsw = (x & 0xFFFF) + (y & 0xFFFF);
	var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
	return (msw << 16) | (lsw & 0xFFFF);
}

/*
 * Bitwise rotate a 32-bit number to the left
 */
function rol(num, cnt)
{
	return (num << cnt) | (num >>> (32 - cnt));
}

/*
 * These functions implement the basic operation for each round of the
 * algorithm.
 */
function cmn(q, a, b, x, s, t)
{
	return add(rol(add(add(a, q), add(x, t)), s), b);
}
function ff(a, b, c, d, x, s, t)
{
	return cmn((b & c) | ((~b) & d), a, b, x, s, t);
}
function gg(a, b, c, d, x, s, t)
{
	return cmn((b & d) | (c & (~d)), a, b, x, s, t);
}
function hh(a, b, c, d, x, s, t)
{
	return cmn(b ^ c ^ d, a, b, x, s, t);
}
function ii(a, b, c, d, x, s, t)
{
	return cmn(c ^ (b | (~d)), a, b, x, s, t);
}

/*
 * Take a string and return the hex representation of its MD5.
 */
function calcMD5(str)
{
	x = str2blks_MD5(str);
	a =  1732584193;
	b = -271733879;
	c = -1732584194;
	d =  271733878;

	for(i = 0; i < x.length; i += 16)
	{
		olda = a;
		oldb = b;
		oldc = c;
		oldd = d;

		a = ff(a, b, c, d, x[i+ 0], 7 , -680876936);
		d = ff(d, a, b, c, x[i+ 1], 12, -389564586);
		c = ff(c, d, a, b, x[i+ 2], 17,  606105819);
		b = ff(b, c, d, a, x[i+ 3], 22, -1044525330);
		a = ff(a, b, c, d, x[i+ 4], 7 , -176418897);
		d = ff(d, a, b, c, x[i+ 5], 12,  1200080426);
		c = ff(c, d, a, b, x[i+ 6], 17, -1473231341);
		b = ff(b, c, d, a, x[i+ 7], 22, -45705983);
		a = ff(a, b, c, d, x[i+ 8], 7 ,  1770035416);
		d = ff(d, a, b, c, x[i+ 9], 12, -1958414417);
		c = ff(c, d, a, b, x[i+10], 17, -42063);
		b = ff(b, c, d, a, x[i+11], 22, -1990404162);
		a = ff(a, b, c, d, x[i+12], 7 ,  1804603682);
		d = ff(d, a, b, c, x[i+13], 12, -40341101);
		c = ff(c, d, a, b, x[i+14], 17, -1502002290);
		b = ff(b, c, d, a, x[i+15], 22,  1236535329);    

		a = gg(a, b, c, d, x[i+ 1], 5 , -165796510);
		d = gg(d, a, b, c, x[i+ 6], 9 , -1069501632);
		c = gg(c, d, a, b, x[i+11], 14,  643717713);
		b = gg(b, c, d, a, x[i+ 0], 20, -373897302);
		a = gg(a, b, c, d, x[i+ 5], 5 , -701558691);
		d = gg(d, a, b, c, x[i+10], 9 ,  38016083);
		c = gg(c, d, a, b, x[i+15], 14, -660478335);
		b = gg(b, c, d, a, x[i+ 4], 20, -405537848);
		a = gg(a, b, c, d, x[i+ 9], 5 ,  568446438);
		d = gg(d, a, b, c, x[i+14], 9 , -1019803690);
		c = gg(c, d, a, b, x[i+ 3], 14, -187363961);
		b = gg(b, c, d, a, x[i+ 8], 20,  1163531501);
		a = gg(a, b, c, d, x[i+13], 5 , -1444681467);
		d = gg(d, a, b, c, x[i+ 2], 9 , -51403784);
		c = gg(c, d, a, b, x[i+ 7], 14,  1735328473);
		b = gg(b, c, d, a, x[i+12], 20, -1926607734);

		a = hh(a, b, c, d, x[i+ 5], 4 , -378558);
		d = hh(d, a, b, c, x[i+ 8], 11, -2022574463);
		c = hh(c, d, a, b, x[i+11], 16,  1839030562);
		b = hh(b, c, d, a, x[i+14], 23, -35309556);
		a = hh(a, b, c, d, x[i+ 1], 4 , -1530992060);
		d = hh(d, a, b, c, x[i+ 4], 11,  1272893353);
		c = hh(c, d, a, b, x[i+ 7], 16, -155497632);
		b = hh(b, c, d, a, x[i+10], 23, -1094730640);
		a = hh(a, b, c, d, x[i+13], 4 ,  681279174);
		d = hh(d, a, b, c, x[i+ 0], 11, -358537222);
		c = hh(c, d, a, b, x[i+ 3], 16, -722521979);
		b = hh(b, c, d, a, x[i+ 6], 23,  76029189);
		a = hh(a, b, c, d, x[i+ 9], 4 , -640364487);
		d = hh(d, a, b, c, x[i+12], 11, -421815835);
		c = hh(c, d, a, b, x[i+15], 16,  530742520);
		b = hh(b, c, d, a, x[i+ 2], 23, -995338651);

		a = ii(a, b, c, d, x[i+ 0], 6 , -198630844);
		d = ii(d, a, b, c, x[i+ 7], 10,  1126891415);
		c = ii(c, d, a, b, x[i+14], 15, -1416354905);
		b = ii(b, c, d, a, x[i+ 5], 21, -57434055);
		a = ii(a, b, c, d, x[i+12], 6 ,  1700485571);
		d = ii(d, a, b, c, x[i+ 3], 10, -1894986606);
		c = ii(c, d, a, b, x[i+10], 15, -1051523);
		b = ii(b, c, d, a, x[i+ 1], 21, -2054922799);
		a = ii(a, b, c, d, x[i+ 8], 6 ,  1873313359);
		d = ii(d, a, b, c, x[i+15], 10, -30611744);
		c = ii(c, d, a, b, x[i+ 6], 15, -1560198380);
		b = ii(b, c, d, a, x[i+13], 21,  1309151649);
		a = ii(a, b, c, d, x[i+ 4], 6 , -145523070);
		d = ii(d, a, b, c, x[i+11], 10, -1120210379);
		c = ii(c, d, a, b, x[i+ 2], 15,  718787259);
		b = ii(b, c, d, a, x[i+ 9], 21, -343485551);

		a = add(a, olda);
		b = add(b, oldb);
		c = add(c, oldc);
		d = add(d, oldd);
	}
	return rhex(a) + rhex(b) + rhex(c) + rhex(d);
}

function PassPhrase104(passphraseStr, KEY1, KEY2, KEY3, KEY4) 
{

	var     pseed2 = "";
	Length2 = passphraseStr.value.length;

	for(p=0; p<64; p++) {
		tempCount = p % Length2;
		pseed2 += passphraseStr.value.substring(tempCount, tempCount+1);
	}
	md5Str = calcMD5(pseed2);

	KEY1.value = md5Str.substring(0, 26).toUpperCase();
	KEY2.value = md5Str.substring(0, 26).toUpperCase();
	KEY3.value = md5Str.substring(0, 26).toUpperCase();
	KEY4.value = md5Str.substring(0, 26).toUpperCase();
}

function clickgenerate(form)
{

	if(form.passphraseStr.value.length == 0 )
	{
	//	alert("$gene_phrase")
			return false;
	} 
	for(i=0;i<form.passphraseStr.value.length;i++)
	{
		if(isValidChar_space(form.passphraseStr.value.charCodeAt(i))==false)
		{
			alert("$notallowpassp");
			return false;
		}
	}			
	if(form.wepenc.options[0].selected == true)
		PassPhrase40(form.passphraseStr, form.wepenc, form.wep_key_no, form.KEY1, form.KEY2, form.KEY3, form.KEY4);
	else
		PassPhrase104(form.passphraseStr, form.KEY1, form.KEY2, form.KEY3, form.KEY4);
	form.generate_flag.value=1;
}
function clickgenerate_a(form)
{
	if(form.passphraseStr_an.value.length == 0 )
	{
	//	alert("$gene_phrase")
			return false;
	} 
	for(i=0;i<form.passphraseStr_an.value.length;i++)
	{
		if(isValidChar_space(form.passphraseStr_an.value.charCodeAt(i))==false)
		{
			alert("$notallowpassp");
			return false;
		}
	}			
	if(form.wepenc_an.options[0].selected == true)
		PassPhrase40(form.passphraseStr_an, form.wepenc_an, form.wep_key_no_an, form.KEY1_an, form.KEY2_an, form.KEY3_an, form.KEY4_an);
	else
		PassPhrase104(form.passphraseStr_an, form.KEY1_an, form.KEY2_an, form.KEY3_an, form.KEY4_an);
	form.generate_flag.value=1;
}

function doPortChange(check)
{
	if(check == 0)
	{
		location.href="forwarding.htm";
	}
	else
		location.href="triggering.htm";
}

function setBlank(OnOffFlag,formFields)
{
	if(OnOffFlag == true)
	{
	        for (var i = 1; i < setBlank.arguments.length; i++)
                	setBlank.arguments[i].value = "";	
	}
}

function setDisabled(OnOffFlag,formFields)
{
	for (var i = 1; i < setDisabled.arguments.length; i++)
		setDisabled.arguments[i].disabled = OnOffFlag;
}

function change_serv(cf)
{
/*
	if ( cf.login_type[2].selected == true )
		location.href="BAS_pppoe.htm";
	else if ( cf.login_type[0].selected == true )
		location.href="BAS_pptp.htm";
	else if (cf.login_type[1].selected == true )
		location.href="BAS_bpa.htm";	
	else if (cf.login_type[3].selected == true )
		location.href="BAS_mulpppoe.htm";
*/
	if( cf.login_type.value == "PPTP" )
		goto_formframe("BAS_pptp.htm");
	else if( cf.login_type.value == "L2TP" )
		goto_formframe("BAS_l2tp.htm");
	else if( cf.login_type.value == "Telstra Bigpond" )
		goto_formframe("BAS_bpa.htm");
	else if( cf.login_type.value == "PPPoE" )
	{
	   if(top.dsl_enable_flag == 0)
		goto_formframe("BAS_pppoe.htm");
	   else
		goto_formframe("BAS_pppoe_dsl.htm");
	}
	else if(cf.login_type.value == "multiPPPoE" )
	{
		if(top.netgear_region != "JP")
			goto_formframe("BAS_mulpppoe.htm");
		else
			goto_formframe("BAS_mulpppoe_ww.htm");
	}
}

function open_all_browser(form,flag)
{
	if(flag == 1)
		greendl_open = 1;
	else
		greendl_open = 0;

	window.open('BT_browse.htm','browse','resizable=0,scrollvars=yes,width=400,height=400,left=400,top=300').focus();
}

function change_ipv6(type)
{

	switch(type)
	{
		case "disabled":
			top.formframe.location.href = "IPv6_disabled.htm";
			break;
		case "autoDetect":
			top.formframe.location.href = "IPv6_auto.htm";
			break;
		case "autoConfig":
			top.formframe.location.href = "IPv6_autoConfig.htm";
			break;
		case "6to4":
			top.formframe.location.href = "IPv6_tunnel.htm";
			break;
		case "bridge":
			top.formframe.location.href = "IPv6_passThrougth.htm";
			break;
		case "fixed":
			top.formframe.location.href = "IPv6_fixed.htm";
			break;
		case "dhcp":
			top.formframe.location.href = "IPv6_dhcp.htm";
			break;
		case "pppoe":
			top.formframe.location.href = "IPv6_pppoe.htm";
			break;
		case "6rd":
			top.formframe.location.href = "IPv6_6rd.htm";
                        break;
		default:
			top.formframe.location.href = "IPv6_disabled.htm";
			break;
	} 
}

function maccheck_multicast(mac_addr)
{
	mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
	mac11 = mac11.substr(1,1);
	if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||(mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||(mac11=="B")||(mac11=="D")||(mac11=="F"))
	{
		//Reject MAC address like 11:xx:xx:xx:xx:xx
		alert("$invalid_mac");	
		return false;
	}
	if(mac_array.length!=6)
	{
	        alert("$invalid_mac");
		return false;
	}
	if(( mac_array[0]=="")||( mac_array[1]=="")||( mac_array[2]=="")||( mac_array[3]=="")||( mac_array[4]=="")||( mac_array[5]==""))
	{
	        alert("$invalid_mac");
		return false;
	}
	if((( mac_array[0]=="00")&&( mac_array[1]=="00")&&
	                        ( mac_array[2]=="00")&&( mac_array[3]=="00")&&
				( mac_array[4]=="00")&&( mac_array[5]=="00"))||
			(( mac_array[0]=="ff")&&( mac_array[1]=="ff")&&
			 ( mac_array[2]=="ff")&&( mac_array[3]=="ff")&&
			  ( mac_array[4]=="ff")&&( mac_array[5]=="ff"))||
			(( mac_array[0]=="FF")&&( mac_array[1]=="FF")&&
			 ( mac_array[2]=="FF")&&( mac_array[3]=="FF")&&
			 ( mac_array[4]=="FF")&&( mac_array[5]=="FF")))
	{
		alert("$invalid_mac");
		return false;
	}
        if(( mac_array[0].length!=2)||( mac_array[1].length!=2)||
                        ( mac_array[2].length!=2)||( mac_array[3].length!=2)||
                        ( mac_array[4].length!=2)||( mac_array[5].length!=2))
        {
                alert("$invalid_mac");
                return false;
        }
        for(i=0;i<mac_addr.length;i++)
        {
                if(isValidMac(mac_addr.charAt(i))==false)
                {
                        alert("$invalid_mac");
                        return false;
                }
        }
        return true;
}

function maccheck(mac_addr)
{
	/*if ( mac_addr.indexOf(":")==-1 && mac_addr.length=="12" )
	{
		var tmp_mac=mac_addr.substr(0,2)+":"+mac_addr.substr(2,2)+":"+mac_addr.substr(4,2)+":"+mac_addr.substr(6,2)+":"+mac_addr.substr(8,2)+":"+mac_addr.substr(10,2);
		mac_addr = tmp_mac;
	}*/
	mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
	mac11 = mac11.substr(1,1);
	if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||(mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||(mac11=="B")||(mac11=="D")||(mac11=="F"))
	{
		alert("$invalid_mac");
		return false;
	}
	if(mac_array.length!=6)
	{
		alert("$invalid_mac");
		return false;
	}
	if(( mac_array[0]=="")||( mac_array[1]=="")||( mac_array[2]=="")||( mac_array[3]=="")||( mac_array[4]=="")||( mac_array[5]==""))
	{
		alert("$invalid_mac");
		return false;
	}
	if((( mac_array[0]=="00")&&( mac_array[1]=="00")&&
				( mac_array[2]=="00")&&( mac_array[3]=="00")&&
				( mac_array[4]=="00")&&( mac_array[5]=="00"))||
			(( mac_array[0]=="ff")&&( mac_array[1]=="ff")&&
			 ( mac_array[2]=="ff")&&( mac_array[3]=="ff")&&
			 ( mac_array[4]=="ff")&&( mac_array[5]=="ff"))||
			(( mac_array[0]=="FF")&&( mac_array[1]=="FF")&&
			 ( mac_array[2]=="FF")&&( mac_array[3]=="FF")&&
			 ( mac_array[4]=="FF")&&( mac_array[5]=="FF")))
	{
		alert("$invalid_mac");
		return false;
	}
	if(( mac_array[0].length!=2)||( mac_array[1].length!=2)||
			( mac_array[2].length!=2)||( mac_array[3].length!=2)||
			( mac_array[4].length!=2)||( mac_array[5].length!=2))
	{
		alert("$invalid_mac");
		return false;
	}

	for(i=0;i<mac_addr.length;i++)
	{
		if(isValidMac(mac_addr.charAt(i))==false)
		{
			alert("$invalid_mac");
			return false;
		}
	}
	return true;
}

function setMAC(cf,this_mac)
{
	var dflag;
	
	if (cf.MACAssign[0].checked || cf.MACAssign[1].checked)
	{
		dflag = true;
		cf.Spoofmac.value=this_mac;
		setDisabled(dflag,cf.Spoofmac);
	}
	else
	{
		dflag = false;
		setDisabled(dflag,cf.Spoofmac);
		cf.Spoofmac.value=this_mac;
	}
}

function maccheck_wds(mac_addr,num,form)
{
	var return_num=0;
	//if(mac_addr==":::::")
	if(mac_addr=="")
		return 2;
	var mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
	mac11 = mac11.substr(1,1);
	if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||
			(mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||
			(mac11=="B")||(mac11=="D")||(mac11=="F"))
	{	
		if( mac11 == "1" && mac_array[0]=="11")
		{}
		else
			return 1;
	}
	if(mac_addr.length!=17 && mac_addr.length!=0)
		return 1;
	if((mac_array[0]=="")||(mac_array[1]=="")||(mac_array[2]=="")||(mac_array[3]=="")|| (mac_array[4]=="")||(mac_array[5]==""))
	{
		if((mac_array[0]=="")&&(mac_array[1]=="")&&(mac_array[2]=="")&&(mac_array[3]=="")&& (mac_array[4]=="")&&(mac_array[5]==""))
			return 2;
		else
			return 1;
	}
	if(((mac_array[0]=="00")&&(mac_array[1]=="00")&&
				(mac_array[2]=="00")&&(mac_array[3]=="00")&&
				(mac_array[4]=="00")&&(mac_array[5]=="00"))||
			((mac_array[0]=="ff")&&(mac_array[1]=="ff")&&
			 (mac_array[2]=="ff")&&(mac_array[3]=="ff")&&
			 (mac_array[4]=="ff")&&(mac_array[5]=="ff"))||
			((mac_array[0]=="FF")&&(mac_array[1]=="FF")&&
			 (mac_array[2]=="FF")&&(mac_array[3]=="FF")&&
			 (mac_array[4]=="FF")&&(mac_array[5]=="FF")))
		return 1;
	if((mac_array[0].length!=2)||(mac_array[1].length!=2)||
			(mac_array[2].length!=2)||(mac_array[3].length!=2)||
			(mac_array[4].length!=2)||(mac_array[5].length!=2))
		return 1;
	for(i=0;i<mac_addr.length;i++)
		if(isValidMac(mac_addr.charAt(i))==false)
			return 1;
	if( num != "" && num != 0 && num != 1 )
	{
		for( k=1;k<num;k++)
		{
			mac_str=eval('the_mac'+k);
			if( mac_str != "" && mac_str.toLowerCase() == mac_addr.toLowerCase())
			{
				return 3;
			}
		}
	}	
	return return_num;
}

function change_sec_to_time(uptime)
{
	var sec=uptime;
	var sec=parseInt(sec);
	var day_sec=sec%86400;
	if(day_sec!=sec)
		new_day=(sec-day_sec)/86400;
	else
		new_day=0;
	var hour_sec=day_sec%3600;
	if(hour_sec!=day_sec)
		new_hour=(day_sec-hour_sec)/3600;
	else
		new_hour=0;
	var min_sec=hour_sec%60;
	if(min_sec!=hour_sec)
		new_min=(hour_sec-min_sec)/60;
	else
		new_min=0;
	var new_sec=min_sec;
	new_day=new_day.toString();
	new_hour=new_hour.toString();
	new_min=new_min.toString();
	new_sec=new_sec.toString();
	if(new_day.length==1)
		new_day='0'+new_day;
	if(new_hour.length==1)
		new_hour='0'+new_hour;
	if(new_min.length==1)
		new_min='0'+new_min;
	if(new_sec.length==1)
		new_sec='0'+new_sec;
	if(new_day == '00')
	{
		var new_time=new_hour+':'+new_min+':'+new_sec;
	}
	else
	{
		var new_time=new_day+':'+new_hour+':'+new_min+':'+new_sec;
	}
	return new_time;
}

function goTestApply()
{
	var winoptions = "width=640,height=480,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes"
	if( run_test == "test")
	{
		if( in_hijack == true)// to fix bug 32600
			window.open("BRS_check_manulConfig.html", null, winoptions);
		else
			window.open("$success_href",null,winoptions);

	}
}
function isValidForwardPort(each_char)
{//0-9,',',' ','-'
	if (!(((each_char>47) && (each_char<58))||(each_char==45)|| (each_char==32) || (each_char==44)|| (each_char==8)||(each_char==0)))
		return false;
}
function isValidMac(digit)
{
	var macVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9","A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f",":");
	var len = macVals.length;
	var i = 0;
	var ret = false;
	for ( i = 0; i < len; i++ )
		if ( digit == macVals[i] ) break;
	if ( i < len )
		ret = true;
	return ret;
}

function isValidHex(each_char)
{
	var macVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9","A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
	var len = macVals.length;
	var i = 0;
	var ret = false;
	for ( i = 0; i < len; i++ )
		if ( each_char == macVals[i] ) break;
	if ( i < len )
		ret = true;
	return ret;
}

function isValidDevName(each_char)
{
	if(!( each_char == 45 || (each_char > 47 && each_char < 58) || (each_char > 64 && each_char < 91) || (each_char>96 && each_char<123)))
		return false;
}

function isValidChar_space(each_char)
{
	if( each_char < 32 || each_char > 127)
		return false;
}

function isValidChar(each_char)
{
	if( each_char < 33 || each_char > 126)
		return false;
}

function isValidCharFolderName(each_char)
{
	if( each_char < 32 || each_char > 126)
		return false;
	else if( each_char == 34 || each_char == 42 || each_char == 47 || each_char == 58 || each_char == 60  || each_char == 62 || each_char == 63 || each_char == 92 || each_char == 124)
		return false;
}

function isValidDdnsHost(each_char)
{
	if (!(((each_char>47) && (each_char<58))||(each_char==45)||(each_char==46)||((each_char>64) && (each_char<91))||((each_char>96) && (each_char<123)) || (each_char==8)||(each_char==0))) 
		return false;
}

function isValidDdnsOrayUserName(each_char)
{// 0-9, a-z, A-Z, -, _ 
	if (!(((each_char>47) && (each_char<58))||(each_char==45)||(each_char==95)||((each_char>64) && (each_char<91))||((each_char>96) && (each_char<123)) || (each_char==8)||(each_char==0))) 
		return false;
}

function isValidNumLetter(each_char)
{
	if((each_char>47 && each_char<58)||(each_char>64 && each_char<91)||(each_char>96 && each_char<123))
		return true;
	else
		return false;
}

function isValidLetter(each_char)
{
	if((each_char>64 && each_char<91)||(each_char>96 && each_char<123))
		return true;
	else
		return false;
}

function remove_space(str)
{
	str = delete_end_space(str);
	str = str.replace(/^\s*/, "");

	return str;
}

function printPage()
{
	if (window.print)
		window.print();
	else
		alert("$not_support_print");
}
function top_left_nolink()
{
	if( parent.multi_lang_router_flag == 1 )
	{
		parent.topframe.document.forms[0].language.disabled = true;
		//parent.topframe.document.forms[0].apply.disabled = true;
	}
}

function wlan_txctrl(form, tx_power_ctrl, tx_power_ctrl_an, wla_channel, country)
{
        if((netgear_region == "WW" || netgear_region == "") && (country != "3" && country != "10"))
        {
		if(tx_power_ctrl == "100")
                        form.wl_tx_ctrl.value="44";
                else if(tx_power_ctrl == "75")
                        form.wl_tx_ctrl.value="10.5";
                else if(tx_power_ctrl == "50")
                        form.wl_tx_ctrl.value="7";
                else if(tx_power_ctrl == "25")
                        form.wl_tx_ctrl.value="3.5";

                //for 5G
                if(tx_power_ctrl_an == "100")
                        form.wla_tx_ctrl.value="44";
                if(parseInt(wla_channel) >= 36 && parseInt(wla_channel) <= 64)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="12.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="8.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="4";
                }
                else if(parseInt(wla_channel) >= 100)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="15.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="5";
		}
	}
        else
        {
                if(tx_power_ctrl == "100")
                        form.wl_tx_ctrl.value="44";
                else if(tx_power_ctrl == "75")
                        form.wl_tx_ctrl.value="18";
                else if(tx_power_ctrl == "50")
                        form.wl_tx_ctrl.value="12";
                else if(tx_power_ctrl == "25")
                        form.wl_tx_ctrl.value="6";

		//for 5G
                if(tx_power_ctrl_an == "100")
                        form.wla_tx_ctrl.value="44";
                if(parseInt(wla_channel) >= 36 && parseInt(wla_channel) <= 48)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="7";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="3.5";
                }
                else if(parseInt(wla_channel) >= 52 && parseInt(wla_channel) <= 165)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="15.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="5";
                }
	}
	
	//the current values should be changed to (urrent_value+6)*2.
	form.wl_tx_ctrl.value = ( parseFloat(form.wl_tx_ctrl.value) + 6 ) * 2;
	form.wla_tx_ctrl.value = ( parseFloat(form.wla_tx_ctrl.value) + 6 ) * 2;
	if(tx_power_ctrl_an == "super_wifi"){
		form.hid_super_wifi_an.value = "1";
	}
	if(tx_power_ctrl == "super_wifi"){
                form.hid_super_wifi.value = "1";
	}
	
}

function delete_end_space(str) //delete all the space at the end of a sting.
{
        var i;

        for(i=str.length-1; str.charAt(i)==" "; i--)
        {
                str = str.substring(0, i);
        }

        return str;
}

//check if the port input is conflict with the USB ReadyShare port. 
function check_port_range(input_start_port, input_end_port, port_using)
{
        if(parseInt(input_start_port)<=parseInt(port_using) && parseInt(input_end_port)>=parseInt(port_using))
        {
                return false;
        }

        return true;
}
function check_readyshare_port(input_start_port, input_end_port, type)
{
        if(show_enableHTTP == '0' && (type == "LAN" || type == "ALL"))
        {
                if(check_port_range(input_start_port, input_end_port, 80) == false)
                        return false;
        }
        if(show_enableHvia == '0' && (type == "WAN" || type == "ALL"))
        {
                if(check_port_range(input_start_port, input_end_port, show_HTTP_via_port) == false)
                        return false
        }
        if(is_ftp == 1)
        {
                if(show_enableFTP == '0' && (type == "LAN" || type == "ALL"))
                {
                        if(check_port_range(input_start_port, input_end_port, 21) == false)
                                return false;
                }
                if(show_enableFvia == '0' && (type == "WAN" || type == "ALL") )
                {
                        if(check_port_range(input_start_port, input_end_port, show_FTP_via_port) == false)
                                return false;
                }
        }

        return true;
}

String.prototype.checkFolder=function()   
{//test string    
        var   s   =   ["\\","\"","\'","?",":","<",">","|","*"];   
        for   (var   i=0;   i<s.length;   i++){   
                  if   (this.indexOf(s[i])   !=   -1)   return   true;}   
        return   false;   
}
String.prototype.checkShareName=function()
{//test string
	var   s   =   ["\\","\"","\/","?",":","<",">","|","*","\]"];
	for   (var   i=0;   i<s.length;   i++){
		if   (this.indexOf(s[i])   !=   -1)   return   true;}
	return   false;
}

//for bug 23249, format the input value for IP address, change "012" to "12", "000" to "0"
function format_IP()
{
	var argv = format_IP.arguments;
	var cf = document.forms[0];

	var i;
	var input_form;
	for(i=0; i<argv.length; i++)
	{
		input_form = eval("cf." + argv[i]);
		if( null == input_form ) continue;

		if(input_form.value != "")
			input_form.value = parseInt(input_form.value, 10).toString(10);
	}

	return true;
}

/* to fix bug 24964 */
function close_top_window()
{
	/* for Chrome, it need to use window.open(), and then use window.close() */
	if ( isCDLESS() && !top.hidden_upgrade )
                toInternet();
        else
	{
		window.open('', '_self', '');
		top.window.close();
	}
}

function ru_wizard()
{
	if( isIE_or_Opera() && IE_version() < 9 )
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		document.getElementsByTagName("div")[0].style.height = frame_height;
	}
}

/* to fix bug */
function scroll_show()
{
	if( top.location.href.indexOf("adv_index.htm") != -1 )
	{
		var frame_div = top.document.getElementById("formframe_div");
		ru_wizard();
		if(isIE()){
		frame_div.onresize = ru_wizard;
		}
		if( get_browser() == "Opera"){
		window.onresize = ru_wizard;
		}
	}
	else if ( top.location.href.indexOf("BRS_index.htm") != -1 )
	{
		document.getElementsByTagName("div")[0].style.height = "500";
	}
}

//fixed bug36052[Beta Tester][IR-064]Attached devices screen looks very disorganized
function convert(value, dataType) {
        switch(dataType) {
    case "int":
                var each_info = value.split(".");
		if(each_info.length!=4)
			return 0;
                value = parseInt(each_info[0]) + parseInt(each_info[1]) + parseInt(each_info[2]) + parseInt(each_info[3]);
                return parseInt(value);
                break
    default:
                return value.toString();
  }
}

function compareCols(col, dataType) {
        return function compareTrs(tr1, tr2) {
                value1 = convert(tr1.cells[col].innerHTML, dataType);
                value2 = convert(tr2.cells[col].innerHTML, dataType);
                if (value1 < value2) {
                        return -1;
                } else if (value1 > value2) {
                        return 1;
                } else {
                        return 0;
                }
        };
}

function sortTable(tableId, col, dataType)
{
	var table = document.getElementById(tableId);
	var tbody = table.tBodies[0];
	var tr = tbody.rows;
	var trValue = new Array();
        for (var i=0; i<tr.length; i++ ){
		trValue[i] = tr[i];
	}
	if (tbody.sortCol == col) {
		trValue.reverse();
	} else {
		trValue.sort(compareCols(col, dataType));
	}
	var fragment = document.createDocumentFragment();
	for (var i=0; i<trValue.length; i++ ) {
		if( i%2== 1 )
			trValue[i].className = "even_line";
		else
			trValue[i].className = "odd_line";
		trValue[i].cells[0].innerHTML = i+1;
		trValue[i].cells[0].className = "subhead";
		fragment.appendChild(trValue[i]);
	}
	tbody.appendChild(fragment);
	tbody.sortCol = col;
}

function getElementsByTagAndName(tagName, eName)
{//IE not support getElementsByName to find divs, so use this function to find divs
	tags = document.getElementsByTagName(tagName);
	var elements = new Array();

	if(tags!=null && tags.length>0){
		for(var z=0; z<tags.length; z++){
			if(tags[z].getAttribute("name") == eName){
				elements[elements.length] = tags[z];
			}
		}
	}
	return elements;
}

function isCDLESS()
{
	var flag = top.location.href.indexOf("BRS_index.htm") > -1 ;
	return flag;
}

function isWIZARD()
{
	var flag = top.location.href.indexOf("adv_index.htm") != -1&& top.setup_wizard_flag == 1;
	return flag;
}

function toInternet()
{
	top.location.href = "to_internet_no_auth.htm";
}

function getText(str1, str2)
{
	if ( typeof(str1) != "undefined" && (isCDLESS() || isWIZARD()))
		return str1;
	else
		return str2;
}

function printText(str1, str2)
{
	document.write(getText(str1, str2));
}


/*router ap extender pages*/
function select_type( type)
{
	if(type === undefined)
		type =document.forms[0].operation_type.value;

	if(type == 0 )
		location.href="rae_router.htm";
	else if(type == 1 )
		location.href="rae_ap.htm";
	else if(type == 2 )
		goto_formframe("rae_bridge.htm");

}

function gotto_link(sub, id)
{
	if(sub != "None")
		top.open_or_close_sub(sub);
	top.click_adv_action(id);
}
function makeStr(strSize, fillChar)
{
	var temp = "";
	for (i=0; i < strSize ; i ++)
		temp = temp + fillChar;
	return temp;
}
function grayoutElements(formFields)
{
	for(var i=0; i<grayoutElements.arguments.length; i++)
	{
		var tag = grayoutElements.arguments[i];
		var elements = document.getElementsByTagName(tag);
		for(var j=0, len=elements.length; j<len; j++)
			elements[j].style.color = "gray";
	}
}
function disableElements(formFields)
{
	for(var i=0; i<disableElements.arguments.length; i++)
	{
		var tag = disableElements.arguments[i];
		var elements = document.getElementsByTagName(tag);
		for(var j=0, len=elements.length; j<len; j++)
			elements[j].disabled = true;
	}
}
function setHeight_for_IE6()
{
	if(isIE_or_Opera && IE_version() == 6)
	{
		var pageHeight;
		if(document.compatMode == "CSS1Compat")
			pageHeight = document.documentElement.clientHeight;
		else
			pageHeight = document.body.clientHeight;
		var divHeight = pageHeight - 149;
		document.getElementById("footer").style.top = divHeight + "px";
		document.getElementById("formframe_div").style.height = divHeight + "px";
	}
}

String.prototype.replaceXSSItem = function() {
	return this.replace(/&#92;/g, "\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'\"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");
}
