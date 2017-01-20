function show_subnet_trustedip()
{
	var cf=document.forms[0];
	var lan_subnet_array=new Array();
	lan_subnet_array=lan_subnet.split('.');
	if(cf.trustipenble.checked)
	{
		if(parseInt(lan_subnet_array[0])!=255)
			cf.cfTrusted_IPAddress1.disabled=false;
		if(parseInt(lan_subnet_array[1])!=255)
			cf.cfTrusted_IPAddress2.disabled=false;
		if(parseInt(lan_subnet_array[2])!=255)
			cf.cfTrusted_IPAddress3.disabled=false;
		if(parseInt(lan_subnet_array[3])!=255)
			cf.cfTrusted_IPAddress4.disabled=false;
	}
	else
	{
		cf.cfTrusted_IPAddress1.disabled=true;
		cf.cfTrusted_IPAddress2.disabled=true;
		cf.cfTrusted_IPAddress3.disabled=true;
		cf.cfTrusted_IPAddress4.disabled=true;
	}
}
function show_trustedip()
{
	var cf=document.forms[0];
	if(endis_Trusted_IP=='0')
	{
		cf.trustipenble.checked=false;
		cf.cfTrusted_IPAddress1.value=lanip_array[0];
		cf.cfTrusted_IPAddress2.value=lanip_array[1];
		cf.cfTrusted_IPAddress3.value=lanip_array[2];
		cf.cfTrusted_IPAddress4.value="";
	}
	else
	{
		cf.trustipenble.checked=true;
		cf.cfTrusted_IPAddress1.value=trustedip_array[0];
		cf.cfTrusted_IPAddress2.value=trustedip_array[1];
		cf.cfTrusted_IPAddress3.value=trustedip_array[2];
		cf.cfTrusted_IPAddress4.value=trustedip_array[3];
	}
	show_subnet_trustedip();
}
function trim(mystr){
while ((mystr.indexOf(" ")==0) && (mystr.length>1)){
mystr=mystr.substring(1,mystr.length);
}
while ((mystr.lastIndexOf(" ")==mystr.length-1)&&(mystr.length>1)){
mystr=mystr.substring(0,mystr.length-1);
}
if (mystr==" "){
mystr="";
}
return mystr;
}

function check_blocksites()
{
	var cf = document.forms[0];
	cf.cfTrusted_IPAddress.value="";
	if (cf.trustipenble.checked == true)
	{
		cf.Trusted_IP_Enable.value=1;
		cf.cfTrusted_IPAddress.value=cf.cfTrusted_IPAddress1.value+'.'+cf.cfTrusted_IPAddress2.value+'.'+cf.cfTrusted_IPAddress3.value+'.'+cf.cfTrusted_IPAddress4.value;
		if(checkipaddr(cf.cfTrusted_IPAddress.value)==false || is_sub_or_broad(cf.cfTrusted_IPAddress.value, lan_ip, lan_subnet) == false || isSameIp(cf.cfTrusted_IPAddress.value,lan_ip) == true )
		{
			alert("$invalid_ip");
			return false;
		}
		if(isSameSubNet(cf.cfTrusted_IPAddress.value,lan_subnet,lan_ip,lan_subnet) == false)
		{	
			alert("$same_subnet_ip_trusted");
			return false;
		}
	}
	else
		cf.Trusted_IP_Enable.value=0;
	cf.Text.value = "";
	if(cf.cfKeyWord_DomainList.length>0)
	{
		for(i=0;i<cf.cfKeyWord_DomainList.length-1;i++){
			if(cf.cfKeyWord_DomainList.options[i].value!=""){
				var domainlist2 = trim(cf.cfKeyWord_DomainList.options[i].value); // delete space of two side
				var domainlist1 = encodeURI(domainlist2);
				cf.Text.value += domainlist1 + " ";
			}
		}
		var domainlist2 = trim(cf.cfKeyWord_DomainList.options[i].value);
		var domainlist1 = encodeURI(domainlist2);
		cf.Text.value+= domainlist1;
	}
	for(i=0;i<3;i++)
		if(document.forms[0].skeyword[i].checked == true)
			cf.skeyword.value=i;
	if(is_jp_version == 1)
	{
		if( cf.session[0].checked == true)
			cf.session.value="session1";
		else
			cf.session.value="session2";
	}

	cf.submit();
	return true;
}
function checkKeyWord()
{
	var cf = document.forms[0];
	//add  to cfKeyWord_DomainList
	var tbox=cf.cfKeyWord_DomainList;
	var tbox_length=cf.cfKeyWord_DomainList.length;
	if ( tbox_length == 255 )
	{
		alert("$keyword255");
		cf.cfKeyWord_Domain.value="";
		return false;
	}	
	if ( cf.cfKeyWord_Domain.value == "")
		return false;
	for(i=0;i<cf.cfKeyWord_Domain.value.length;i++)
	{
		if(isValidChar_space(cf.cfKeyWord_Domain.value.charCodeAt(i))==false)
		{
			alert("$error_keyword");
			return false;
		}
	}	
	var new_str=cf.cfKeyWord_Domain.value;
	var check_str=new_str.toLowerCase();
	if(check_str.indexOf("://")>0 && check_str.length>7)
	{
		if(check_str.substring(0,7)=="http://")
			new_str=new_str.substr(7);
		else if(check_str.substring(0,8)=="https://")
			new_str=new_str.substr(8);
	}

	for(var i=0;i<tbox_length;i++)
	{
		if(new_str==tbox.options[i].value)
		{
			cf.cfKeyWord_Domain.value="";
			return false;
		}
	}
	if(tbox_length == 1 && tbox.options[0].value=="")
		var new_length=0;
	else 
		var new_length=tbox_length;
	tbox.options[new_length]=new Option(new_str,new_str);
	cf.cfKeyWord_Domain.value="";
	return true;
}

function checkKeyWordDomainList(act)
{
	var cf = document.forms[0];
	cf.cfKeyWord_Domain.value = "";
	if(cf.cfKeyWord_DomainList.options[0] == null)
	{
		return false;
	}
	if (act=='delete' && cf.cfKeyWord_DomainList.selectedIndex==-1)
	{
		alert("$select_serv_del");
		return false;
	}
	if (act=='delete' && cf.cfKeyWord_DomainList.selectedIndex!=-1)
	{
		cf.cfKeyWord_DomainList.options[cf.cfKeyWord_DomainList.selectedIndex]=null;
	}
	if (act=='clear')
	{
		var DomainList_length=cf.cfKeyWord_DomainList.length;
		for(var i=0;i<DomainList_length;i++)
		{
			cf.cfKeyWord_DomainList.options[0]=null;
		}
	}
	return true;
}
function checkTrustIP()
{
	show_subnet_trustedip();
}
