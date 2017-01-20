function convert(value, dataType) {
	switch(dataType) {
    case "int":
		var each_info = value.split(".");
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

function sortTable(tableId, col, dataType, tag) 
{
	if(enable_block_device == 1 || tag == 1)
	{
		var table = document.getElementById(tableId);
		var tbody = table.tBodies[0];
		var tr = tbody.rows;
		var trValue = new Array();
		for (var i=0; i<tr.length; i++ ) {
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
			fragment.appendChild(trValue[i]);
		}
		tbody.appendChild(fragment); 
		tbody.sortCol = col;
	}
}

function load_sortTable(tableId, num, col, dataType, tag)
{
	var j=0;
	var k=0;
	var m=0;
	if(enable_block_device == 1 || tag == 1)
	{
		var table = document.getElementById(tableId);
		var tbody = table.tBodies[0];
		var tr = tbody.rows;
		var trValue1 = new Array();
		var trValue2 = new Array();
		for (var i=0; i<tr.length; i++ ) {
			if(tr[i].cells[num].innerHTML.indexOf("Blocked") > -1)
			{
				trValue1[j] = tr[i]; 
				j++;
			}
		}
		for (var i=0; i<tr.length; i++ ) {
			if(tr[i].cells[num].innerHTML.indexOf("Allowed") > -1)
			{
				trValue2[k] = tr[i]; 
				k++;
			}
		}

		trValue1.sort(compareCols(col, dataType));
		trValue2.sort(compareCols(col, dataType));
		
		var fragment = document.createDocumentFragment(); 
		
		for (var i=0; i<trValue1.length; i++ ) {
			if( i%2== 1 )
				trValue1[i].className = "even_line";
			else
				trValue1[i].className = "odd_line";
			fragment.appendChild(trValue1[i]);
		}
		m = i;
		for (var i=0; i<trValue2.length; i++ ) {

			if( m%2== 1)
				trValue2[i].className = "even_line";
			else
				trValue2[i].className = "odd_line";
					
			fragment.appendChild(trValue2[i]);
			m++;
		}
		tbody.appendChild(fragment);  
		tbody.sortCol = col;
	}	
}

function access_cancel()
{
	location.href="AccessControl_show.htm";	
}

function check_all_device(this_e, start, id)
{
	var cf = document.forms[0];
	var i = start;
	var e;
	var type=this_e.checked;
	
	while( e = eval('document.getElementById("'+id+i+'")'))
	{
		e.checked = type;
		i++;
	}

    if(id == "checkbox_index")
        toggle_edit();
    else if(id == "checkbox_index_white")
        toggle_edit_allow();
    else if(id == "checkbox_index_black")
        toggle_edit_block();
}

function set_allow_block(cf, flag)
{
	access_control_apply(cf, '0');
	if(flag == 1)
		cf.submit_flag.value = "acc_control_allow";
	else
		cf.submit_flag.value = "acc_control_block";

	var sel_num=0;
	var sel_list="";
	if(access_control_device_num > 0)
	{
		for(i=1;i<=access_control_device_num;i++)
		{
			var listName = "checkbox_index"+i;
			if(document.getElementById(listName).checked == true)
			{
				if(flag == 0 && document.getElementById(listName).value.toLowerCase() == wan_remote_mac.toLowerCase())
				{
					alert("$not_block_device_msg");
					return false;
				}
				sel_list+= document.getElementById(listName).value;
				sel_list+= "#";
				sel_num++;
			}
		}
		if(sel_num == 0)
			return false;
		if(flag == 0 && sel_num!=0 && confirm("$acc_block_check") == false)
			return false;
		
		cf.hidden_change_list.value = sel_list;
		cf.hidden_change_num.value = sel_num;
		cf.submit();
	}
	else
		return false;
}

function check_edit()
{
	var count=0;
	var select_num;
	var cf = document.forms[0];

	if(access_control_device_num==0)
	{
		alert("$port_edit");
		return false;
	}

	for(i=1;i<=access_control_device_num;i++)
	{
		var listName="checkbox_index"+i;
		if(document.getElementById(listName).checked==true)
		{
			select_num=i+1;
			count++;
		}
	}
	if(count==0||count!=1)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		access_control_apply(cf, '0');
		cf.select_edit.value=select_num-1;
		cf.submit_flag.value="editnum_connect_device";
		cf.action="/apply.cgi?/edit_connect_device.htm timestamp="+ts;
		cf.submit();
		return true;
	}
}

function delete_block()
{
	var cf = document.forms[0];

	access_control_apply(cf, '0');
	cf.submit_flag.value = "delete_acc";

	var sel_list="";
	var count=0;
	if( blocked_no_connect_num > 0 )
	{
		for( i=1; i <=blocked_no_connect_num; i++)
		{
			var listName = "checkbox_index_black"+i;
			if(document.getElementById(listName).checked == true)
			{
				sel_list+= document.getElementById(listName).value;
				sel_list+= "#";
				count++;
			}
		}
		if(sel_list == "")
			return false;
		if(confirm("$acc_del_check") == false)
			return false;
		cf.hidden_del_list.value = sel_list;
		cf.hidden_del_num.value = count;
		cf.submit();
	}
	else
		return false;
}

function delete_allow()
{
        var cf = document.forms[0];

	access_control_apply(cf, '0');

        cf.submit_flag.value = "delete_acc";
	
	var sel_list="";
	var count=0;
        if( allowed_no_connect_num > 0 )
        {
                for( i = 1; i <=allowed_no_connect_num; i++)
                {
			var listName = "checkbox_index_white"+i;
			if(document.getElementById(listName).checked == true)
			{
				sel_list+= document.getElementById(listName).value;
				sel_list+= "#";
				count++;
			}
                }
		if(sel_list == "")
			return false;
		if(confirm("$acc_del_check") == false)
			return false;
		cf.hidden_del_list.value = sel_list;
		cf.hidden_del_num.value = count;
		cf.submit();
        }
	else
		return false;
}


function access_control_apply(cf, flag)
{
	if(cf.enable_acl.checked == false)
		cf.hid_able_block_device.value = 0;
	else
		cf.hid_able_block_device.value = 1;
		
	if(cf.access_all[0].checked == false)
		cf.hid_new_device_status.value = "Block";
	else
		cf.hid_new_device_status.value = "Allow";

	cf.submit_flag.value = "apply_acc_control";
	if(flag == '1')
		cf.submit();
}

function check_status()
{
	var cf = document.forms[0];
	var flag;
	flag = (!(cf.enable_acl.checked));
    setDisabled(flag, cf.access_all[0], cf.access_all[1], cf.Allow, cf.Block, cf.checkbox_index);
    setDisabled(flag, document.getElementById("delete_white"), document.getElementById("delete_black"), cf.checkbox_index_white, cf.checkbox_index_black,document.getElementById("add_black"),document.getElementById("add_white"));
	if(cf.enable_acl.checked == false)
	{
		setDisabled(true, cf.edit_attached, cf.edit_white, cf.edit_black);

		enable_block_device = 0;
		cf.Allow.className = "common_gray_bt";
		cf.Block.className = "common_gray_bt";
		cf.edit_attached.className="common_gray_bt";
		for(i=1;i<=access_control_device_num;i++)
			document.getElementById("checkbox_index"+i).disabled = true;

		document.getElementById("delete_white").className= "common_big_gray_bt";
		document.getElementById("add_white").className = "common_gray_bt";
		cf.edit_white.className="common_gray_bt";
		for(i=1;i<=allowed_no_connect_num;i++)
			document.getElementById("checkbox_index_white"+i).disabled = true;

		document.getElementById("delete_black").className= "common_big_gray_bt";
		document.getElementById("add_black").className = "common_gray_bt";
		cf.edit_black.className = "common_gray_bt";
		for(i=1;i<=blocked_no_connect_num;i++)
			document.getElementById("checkbox_index_black"+i).disabled = true;
	}
	else
	{
		toggle_edit();
		toggle_edit_allow();
		toggle_edit_block();

		enable_block_device = 1;
		cf.Allow.className = "common_bt";
		cf.Block.className = "common_bt";
		for(i=1;i<=access_control_device_num;i++)
			document.getElementById("checkbox_index"+i).disabled = false;

		document.getElementById("delete_white").className= "common_big_bt";
		document.getElementById("add_white").className = "common_bt";
		for(i=1;i<=allowed_no_connect_num;i++)
			document.getElementById("checkbox_index_white"+i).disabled = false;
		document.getElementById("delete_black").className= "common_big_bt";
		document.getElementById("add_black").className = "common_bt";
		for(i=1;i<=blocked_no_connect_num;i++)
			document.getElementById("checkbox_index_black"+i).disabled = false;
	}
}

function check_acc_add(cf,flag)
{
	if(cf.mac_addr.value.length==12 && cf.mac_addr.value.indexOf(":")==-1)
	{
		var mac=cf.mac_addr.value;
		cf.mac_addr.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);
	}
	else if ( cf.mac_addr.value.split("-").length == 6 )
	{
		var tmp_mac = cf.mac_addr.value.replace(/-/g,":");
		cf.mac_addr.value=tmp_mac;
	}
	if(maccheck(cf.mac_addr.value) == false)
		return false;
	if(flag!='edit_allow'&&flag!='edit_block'&&flag!='edit_connect_device')
	{
		for(i=0;i<acc_mac_num;i++)
		{
			var str = eval ( 'acc_mac' + i );
			if(str.toLowerCase() == cf.mac_addr.value.toLowerCase())
			{
				alert("$mac_dup");
				return false;
			}
		}
	}
	if(flag=='edit_allow')
	{
		for(i=0;i<allowed_no_connect_num;i++)
		{
			var str=eval('allowed_no_connect'+i);
			var each_info=str.split(' ');
			if(select_editnum!=i)
			{
				if(each_info[1].toLowerCase() == cf.mac_addr.value.toLowerCase())
				{
					alert("$mac_dup");
					return false;
				}
			}
		}
		for(i=0;i<blocked_no_connect_num;i++)
                {
                        var str=eval('blocked_no_connect'+i);
                        var each_info=str.split(' ');
                        if(each_info[1].toLowerCase() == cf.mac_addr.value.toLowerCase())
                        {
                                        alert("$mac_dup");
                                        return false;
                        }
                }
		for(i=0;i<access_control_device_num;i++)
                {
                        var str=eval('access_control_device'+i);
                        var each_info=str.split('*');
                        if(each_info[2].toLowerCase() == cf.mac_addr.value.toLowerCase())
                        {
                                alert("$mac_dup");
                                return false;
                        }
		}
		if(cf.allow_or_block.value == "Blocked")
			cf.hidden_acc_edit_type.value = "block";
	}
	if(flag=='edit_block')
	{
                for(i=0;i<blocked_no_connect_num;i++)
                {
                        var str=eval('blocked_no_connect'+i);
                        var each_info=str.split(' ');
                        if(select_editnum!=i)
                        {
                                if(each_info[1].toLowerCase() == cf.mac_addr.value.toLowerCase())                                {
                                        alert("$mac_dup");
                                        return false;
                                }
                        }
                }
		for(i=0;i<allowed_no_connect_num;i++)
                {
                        var str=eval('allowed_no_connect'+i);
                        var each_info=str.split(' ');
                        if(each_info[1].toLowerCase() == cf.mac_addr.value.toLowerCase())
                        {
                                        alert("$mac_dup");
                                        return false;
                        }
                }
		for(i=0;i<access_control_device_num;i++)
                {
                        var str=eval('access_control_device'+i);
                        var each_info=str.split('*');
                        if(each_info[2].toLowerCase() == cf.mac_addr.value.toLowerCase())
			{
				alert("$mac_dup");
				return false;
			}
		}
		if(cf.allow_or_block.value == "Allowed")
			cf.hidden_acc_edit_type.value = "allow";
	}
	if(flag=='edit_connect_device')
    {
        if(access_mac.toLowerCase() == cf.hidden_edit_mac.value.toLowerCase())
        {
            var str=document.getElementById("allow_or_block_connect_device")
            if(str.options[1].selected==true)
            {
                alert("$not_block_device_msg");
                return false;
            }
        }
    }
    	cf.submit();
	return true;
}

function check_allow_edit()
{
	var cf = document.forms[0];
	if(allowed_no_connect_num==0)
	{
		alert("$port_edit");
		return false;
	}
	var count=0;
	var select_num;
	for(i=1;i<=allowed_no_connect_num;i++)
	{
		var listName="checkbox_index_white"+i;
		if(document.getElementById(listName).checked==true)
		{
			select_num=i+1;
			count++;
		}
	}
	if(count==0||count!=1)
	{
		alert("$port_edit");
		return false;
	}
	else
	{
		access_control_apply(cf, '0');
		cf.select_edit.value=select_num-1;
		cf.submit_flag.value="editnum_acc_allow";
		cf.action="/apply.cgi?/edit_allowed.htm timestamp="+ts;
		cf.submit();
		return true;
	}
}

function check_block_edit()
{
	var cf=document.forms[0];
	if(blocked_no_connect_num==0)
        {
                alert("$port_edit");
                return false;
        }
        var count=0;
	var select_num;
        for(i=1;i<=blocked_no_connect_num;i++)
        {
		var listName="checkbox_index_black"+i;
		if(document.getElementById(listName).checked==true)
                {
			select_num=i+1;
                        count++;
                }
	}
        if(count==0||count!=1)
        {
		alert("$port_edit");
                return false;
        }
	access_control_apply(cf, '0');
	cf.select_edit.value=select_num-1;
	cf.submit_flag.value="editnum_acc_block";
	cf.action="/apply.cgi?/edit_blocked.htm timestamp="+ts;
	cf.submit();
	return true;
}

function toggle_edit()
{
    var num = 0;
    var cf = document.forms[0];
    if(access_control_device_num > 0) {
        for(var i=1;i<=access_control_device_num;i++) {
            var listName = "checkbox_index"+i;
            if(document.getElementById(listName).checked == true) {
                num++;
            }
        }
    }
    if(num == 1) {
        cf.edit_attached.className = "common_bt";
        cf.edit_attached.disabled = false;
    } else {
        cf.edit_attached.className = "common_gray_bt";
        cf.edit_attached.disabled = true;
    }
}
function toggle_edit_allow()
{
    var num = 0;
    var cf = document.forms[0];
    if(allowed_no_connect_num > 0) {
        for(var i=1; i<=allowed_no_connect_num; i++) {
            var listName = "checkbox_index_white"+i;
            if(document.getElementById(listName).checked == true) {
                num++;
            }
        }
    }
    if(num == 1) {
        cf.edit_white.className = "common_bt";
        cf.edit_white.disabled = false;
    } else {
        cf.edit_white.className = "common_gray_bt";
        cf.edit_white.disabled = true;
    }
}

function toggle_edit_block()
{
    var num = 0;
    var cf = document.forms[0];
    if(blocked_no_connect_num > 0) {
        for(var i=1; i<=blocked_no_connect_num; i++) {
            var listName = "checkbox_index_black"+i;
            if(document.getElementById(listName).checked == true) {
                num++;
            }
        }
    }
    if(num == 1) {
        cf.edit_black.className = "common_bt";
        cf.edit_black.disabled = false;
    } else {
        cf.edit_black.className = "common_gray_bt";
        cf.edit_black.disabled = true;
    }
}

