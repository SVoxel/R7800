function check_disable_all_read_user()
{
	
	var form = document.forms[0];
	var check_nub = 0;

	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
				
	form.readAccess_ad.checked = true;
	form.readAccess_ad.disabled = true;
	
	for(i =0; i < array_num; i++)
	{
		objs[i].checked = true;
		objs[i].disabled = true;
	}			
}

function check_disable_read_admin()
{
	
	var form = document.forms[0];
	var check_nub = 0;

	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
				
	form.readAccess_ad.checked = true;
	form.readAccess_ad.disabled = true;
	
	for(i =0; i < array_num; i++)
	{
		objs[i].checked = true;
		objs[i].disabled = false;
	}			
}

function check_disable_all_write_user()
{
	
	var form = document.forms[0];
	var check_nub = 0;

	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
	
	form.writeAccess_ad.checked = true;
	form.writeAccess_ad.disabled = true;
	for(i =0; i < array_num; i++)
	{
		objs_w[i].checked = true;
		objs_w[i].disabled = true;
	}			
}
function check_disable_write_admin()
{
	
	var form = document.forms[0];
	var check_nub = 0;

	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
	
	form.writeAccess_ad.checked = true;
	form.writeAccess_ad.disabled = true;
	for(i =0; i < array_num; i++)
	{
		objs_w[i].checked = true;
		objs_w[i].disabled = false;
	}			
}

function check_all_read()
{
	var form = document.forms[0];
	var check_nub = 0;

	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
	
	for(i =0; i < array_num; i++) // write users is checked or not
			if(objs_w[i].checked == true)
				w_flag = 1;
	for(i =0; i < array_num; i++) // read users is checked or not
			if(objs[i].checked == true)
				r_flag = 1;

	if(form.writeAccess_all.checked == true)
	{
		form.readAccess_all.checked = true;
		/*form.readAccess_ad.checked = false;
		form.readAccess_ad.disabled = false;
		
		for(i =0; i < array_num; i++)
			objs[i].checked = false;*/
		check_disable_all_read_user();
	}
	else
	{
		if( form.readAccess_all.checked == true)
		{
			//form.readAccess_ad.checked = false;
			//form.readAccess_ad.disabled = false;
			
			//for(i =0; i < array_num; i++)
				//objs[i].checked = false;
			check_disable_all_read_user();
		}
		else
		{
			check_disable_read_admin();
			/*for(i =0; i< array_num ; i++)
			{
				if( objs_w[i].checked == true )
				{
						objs[i].checked = true;
						form.readAccess_ad.checked = true;
						form.readAccess_ad.disabled = true;
				}
			}
			
			for(i =0; i < array_num; i++)
			{
				if( objs[i].checked == true )
					check_nub++;
			}
			
			if(check_nub == 0)
			{
				form.readAccess_all.checked = true;
				form.readAccess_ad.disabled = false;
			}*/
		}
	}

}

function check_other_read()
{
	var form = document.forms[0];
	var check_nub = 0;
	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	
	for(i =0; i < array_num; i++)
	{
		if( objs[i].checked == true )
			check_nub++;
	}
	
	if(form.readAccess_ad.checked == true || check_nub != 0)
	{
		form.writeAccess_all.checked = false;
		form.writeAccess_ad.checked = true;
		
		for(i =0; i < array_num; i++)
			if( objs[i].checked == true )
				objs_w[i].checked = true;
		
		if( check_nub != 0 )
			form.writeAccess_ad.disabled = true;
	}
	
	if(form.writeAccess_all.checked == true)
	{
		form.readAccess_all.checked = true;
		form.readAccess_ad.checked = false;
		form.readAccess_ad.disabled = false;
		
		for(i =0; i < array_num; i++)
			objs[i].checked = false;
	}
	else
	{
		
		for(i =0; i< array_num ; i++)
		{
			if( objs[i].checked == true )
			{
					objs_w[i].checked = true;
					form.readAccess_ad.checked = true;
					//form.readAccess_ad.disabled = true;
			}
			else
				objs_w[i].checked = false;
		} 
		
		if(check_nub != 0)
		{
			form.readAccess_all.checked = false;
			form.readAccess_ad.checked = true;
			form.readAccess_ad.disabled = true;
		}
		else
		{
			/*if(form.readAccess_ad.checked == false)
				form.readAccess_all.checked = true;
			else
			{
				form.readAccess_all.checked = false;
			}*/
			
			form.readAccess_ad.checked = true;
			form.readAccess_ad.disabled = true;
		}
	}
	
}

function check_all_write()
{
	var form = document.forms[0];
	var check_nub = 0;
	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	var r_flag = 0;
	var w_flag  =0; //
	
	for(i =0; i < array_num; i++) // write users is checked or not
			if(objs_w[i].checked == true)
				w_flag = 1;
	for(i =0; i < array_num; i++) // read users is checked or not
			if(objs[i].checked == true)
				r_flag = 1;

	if( form.writeAccess_all.checked == true)
	{		
		/*form.writeAccess_ad.checked = false;
		form.writeAccess_ad.disabled = false;
		
		for(i =0; i < array_num; i++)
		{
			objs_w[i].checked = false;
			objs[i].checked = false;
		}*/
		check_disable_all_write_user();
		
		form.readAccess_all.checked = true;
		//form.readAccess_ad.checked = false;
		//form.readAccess_ad.disabled = false;
		check_disable_all_read_user();
	}
	else
	{
		//if(form.writeAccess_ad.checked == false && w_flag ==0)
		//	form.writeAccess_all.checked = true;
		check_disable_write_admin();
		
	}
}

function check_other_write()
{
	var form = document.forms[0];
	var check_nub = 0;
	var read_num = 0;
	var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	
	for(i =0; i < array_num; i++)
	{
		if( objs_w[i].checked == true )
		{
			if(form.readAccess_all.checked == false)
			{
				objs[i].checked = true;
				form.readAccess_ad.checked = true;
				form.readAccess_ad.disabled = true;
			}
			//check_nub++;
		}
		else
		{
			if(form.readAccess_all.checked == false)
			{
				objs[i].checked = false;
				form.readAccess_ad.checked = true;
				form.readAccess_ad.disabled = true;
			}
			//check_nub++;
		}
	}
	
	/*for(i =0; i < array_num; i++)
			if( objs[i].checked == true )
				read_num++;
				
	if( read_num == 0 )
			form.readAccess_ad.disabled = false;*/
	
	/*if(check_nub != 0)
	{
		form.writeAccess_all.checked = false;
		form.writeAccess_ad.checked = true;
		form.writeAccess_ad.disabled = true;
	}
	else
	{
		form.writeAccess_ad.disabled = false;
		if(form.writeAccess_ad.checked == false)
		{
			form.writeAccess_all.checked = true;
			form.readAccess_all.checked = true;
			form.readAccess_ad.checked = false;
			form.readAccess_ad.disabled = false;
			for(i =0; i < array_num; i++)
				objs[i].checked = false;
		}
		else
			form.writeAccess_all.checked = false;
	}*/
}

function check_value()
{
/* when apply USB_Folder_edit.htm and USB_Folder_creat.htm,invoke this function to chenk the users value, 0 not exist, 1 exist, 2 read auth, 3 read and write auth.*/

    var form = document.forms[0];
    var objs = document.getElementsByName("readAccess_n");
	var objs_w = document.getElementsByName("writeAccess_n");
	if(form.writeAccess_all.checked == true)
	{
		//form.hid_readAccess_all.value = 1;
		//form.hid_readAccess_ad.value = 0;
		  form.hid_read_a.value = 0;
		//form.hid_writeAccess_all.value = 1;
		//form.hid_writeAccess_ad.value = 0;
		  form.hid_write_a.value = 0;
		
		for(i = 0; i < objs_w.length ; i++)
		{
			var user_num = "hid_user"+i;
			document.getElementById(user_num).value = 3;
		}
		/* form.hid_user0.value = 3;
		 form.hid_user1.value = 3;
		 form.hid_user2.value = 3;
		 form.hid_user3.value = 3;*/
	}
	else
	{
		form.hid_write_a.value = 1;

		for(i = 0; i < objs_w.length ; i++)
		{
			var user_num = "hid_user"+i;
			//alert( user_num);
			if( objs_w[i].checked == true )
			{
				
				document.getElementById(user_num).value = 3; //3 r&w, 2 r, 1,exist. 0 not exist 
				//form.hid_useri.value = 3;
				//if( form.hid_writeAccess.value == 0 )
					//form.hid_writeAccess.value = objs_w[i].value;
				//else
				//form.hid_writeAccess.value = form.hid_writeAccess.value + " " + objs_w[i].value;
			}
			else if( objs[i].checked == true || form.readAccess_all.checked == true)
				document.getElementById(user_num).value = 2;
				//form.hid_writeAccess.value = form.hid_writeAccess.value + " " + 0;
			else
			{
				document.getElementById(user_num).value = 1;
			//	alert( user_num);
			}
		}
		
		if(form.readAccess_all.checked == true)
		{
			/*form.hid_readAccess_all.value = 1;
			form.hid_readAccess_ad.value = 0;
			form.hid_readAccess.value = 0;*/
			form.hid_read_a.value = 0;
		}
		else
		{
			form.hid_read_a.value = 1;
		}
	}

}