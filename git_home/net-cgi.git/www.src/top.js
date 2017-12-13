function languageShowlist()
{
	document.write('<option value=Auto>$auto_mark</option>');
	if (language_oriArray != "")
	{
		for (j=0; j<language_oriArray.length; j++)
		{
			document.write('<option value='+language_oriArray[j]+'>'+language_showArray[j]+'</option>');
		}
	}
	else
		document.write('<option value=English>'+language_oriArray[0]+'</option>');
}

function change_container_posision()
{
	if( document.body.clientWidth < document.body.scrollWidth )
		document.getElementById("container").className="container_left";
	else
		document.getElementById("container").className="container_center";
}

function goto_top( page)
{
	top.location.href=page;
}

function change_select()
{
	form=document.forms[0];

        if( form.language.value == lang_select && !(lang_select == "Auto" && browser_lang != gui_region))
        {
                if(jp_multiPPPoE == "1" && form.language.value != "Japanese")
                {
                        form.hidden_lang_avi.value=form.language.value;
                        form.action="/apply.cgi?/index.htm timestamp="+ts;
                        form.submit_flag.value="change_multiPPPoE_status";
                        form.submit();
                }else
                        location.href="/top.html";
        }
        else
        {
		if(bgn_pr_wds_support_wpa == 1 || an_pr_wds_support_wpa == 1 )
		{
			alert("$pr_wps_support_wpa_others_not");
			goto_formframe('WLG_wireless.htm');
			document.forms[0].language.value = lang_select;
			return false;
		}
                form.language.disabled=true;
                form.hidden_lang_avi.value=form.language.value;
		form.target="formframe";
                form.submit();
        }
}

function firmwareUpgrade()
{
	goto_formframe('UPG_recheck_version.htm');
}

function do_search()
{
	var key = top.document.getElementsByTagName("input")[0].value.replace(/\'/g, "&apos;");
	var winoptions = "width=960,height=800,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes";
	var url="";
	if(key == "" || key == "$ent_srh_item") {
		url = "http://support.netgear.com/product/"+top.host_name;
	} else {
		key = key.replace(/ /g,"%20")
		url = "http://kb.netgear.com/app/answers/list/kw/"+key;
	}

	window.open(url,'_blank',winoptions);
}

function setLabelClass(label, className)
{
	var words_len = label.getElementsByTagName("span")[0].innerHTML.length;

	if(words_len >= 16)
		label.className = className + "_longest";
	else if(words_len >= 12)
		label.className = className + "_long";
	else if(words_len >= 10)
		label.className = className + "_middle";
	else
		label.className = className;
}

/*function setUpgradeMessClass(upgrade_mess)
{
	var message_len = upgrade_mess.innerHTML.length;
	if( message_len >= 75 )
		upgrade_mess.className = "i_long";
	else if( message_len >= 69 || gui_region == "Korean" || gui_region == "Czech")
		upgrade_mess.className = "i_middle";
	else
		upgrade_mess.className = "i_normal";
}*/

function load_top_value()
{
	form=document.forms[0];
	
	var sUserAgent = navigator.userAgent;

	var width = top.document.documentElement.clientWidth;
	var upgrade_div = document.getElementById("update_info");
	if(upgrade_div != null)
	{
		if(wan_status==1 && config_status==9999 && updateFirmware==1)
			upgrade_div.style.display = "inline";
		else
			upgrade_div.style.display = "none";
	}
	
	var basic_label = document.getElementById("basic_lab");
	var advanced_label = document.getElementById("advanced_lab");

	if(parent.type == "basic")
	{
		basic_label.className = "label_click";
		advanced_label.className = "label_unclick";
	}
	else if(parent.type == "advanced")
	{
		basic_label.className = "label_unclick";
		advanced_label.className = "label_click";
	}

	/* to fix bug 25107 */
	if( upgrade_div.style.display != "none")
	{
		var upgrade_mess = upgrade_div.getElementsByTagName("i")[0];
		var left = document.getElementById("labels").clientWidth + 20;
		var free_width = width - left - 181;
		var info_width = document.getElementById("update_info_middle").clientWidth + 34;

		if( free_width > info_width )
		{
			var upgrade_left = (free_width - info_width)/2 > 15 ? 15 : (free_width - info_width)/2;
			upgrade_div.className = "update_info_down";
			upgrade_div.style.left = (left + upgrade_left) + "px";
		}
		else
		{
			upgrade_div.className="update_info_up";
			upgrade_div.style.left="270px";
		}
	}
}


function detectEnter(type, e)
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
        else 
			return true;
			
		if(type == "num")
		{
	  if(keycode==13)
			do_search();
		}
		else
		return false;
}
