function check_static_ip_mask_gtw()
{
	form=document.forms[0];
	form.hid_bridge_ipaddr.value=form.BRIDGEaddr1.value+'.'+form.BRIDGEaddr2.value+'.'+form.BRIDGEaddr3.value+'.'+form.BRIDGEaddr4.value;
    form.hid_bridge_subnet.value=form.BRIDGEmask1.value+'.'+form.BRIDGEmask2.value+'.'+form.BRIDGEmask3.value+'.'+form.BRIDGEmask4.value;
    form.hid_bridge_gateway.value=form.BRIDGEgateway1.value+'.'+form.BRIDGEgateway2.value+'.'+form.BRIDGEgateway3.value+'.'+form.BRIDGEgateway4.value;
	
	if(checkipaddr(form.hid_bridge_ipaddr.value)==false || is_sub_or_broad(form.hid_bridge_ipaddr.value, form.hid_bridge_ipaddr.value, form.hid_bridge_subnet.value) == false)
	{
		alert("$invalid_ip");
		return false;
	}
	if(checksubnet(form.hid_bridge_subnet.value, 0)==false)
	{
		alert("$invalid_mask");
		return false;
	}
	if(checkgateway(form.hid_bridge_gateway.value)==false)
	{
		alert("$invalid_gateway");
		return false;
	}
	if(isGateway(form.hid_bridge_ipaddr.value,form.hid_bridge_subnet.value,form.hid_bridge_gateway.value)==false)
	{
		alert("$invalid_gateway");
		return false;		
	}
	if( isSameIp(form.hid_bridge_ipaddr.value, form.hid_bridge_gateway.value) == true )
	{
		alert("$invalid_gateway");
		return false;
	}
	if(isSameSubNet(form.hid_bridge_ipaddr.value,form.hid_bridge_subnet.value,form.hid_bridge_gateway.value,form.hid_bridge_subnet.value) == false)
	{
		alert("$same_subnet_ip_gtw");
		return false;
	}
	return true;
}

function check_static_dns( wan_assign )
{
	var form=document.forms[0];
	form.bridge_dnsaddr1.value=form.BRIDGEDAddr1.value+'.'+form.BRIDGEDAddr2.value+'.'+form.BRIDGEDAddr3.value+'.'+form.BRIDGEDAddr4.value;
    form.bridge_dnsaddr2.value=form.BRIDGEPDAddr1.value+'.'+form.BRIDGEPDAddr2.value+'.'+form.BRIDGEPDAddr3.value+'.'+form.BRIDGEPDAddr4.value;
	form.hid_bridge_ipaddr.value=form.BRIDGEaddr1.value+'.'+form.BRIDGEaddr2.value+'.'+form.BRIDGEaddr3.value+'.'+form.BRIDGEaddr4.value;

	if(form.bridge_dnsaddr1.value=="...")
		form.bridge_dnsaddr1.value="";

	if(form.bridge_dnsaddr2.value=="...")
		form.bridge_dnsaddr2.value="";
	if( check_DNS(form.bridge_dnsaddr1.value,form.bridge_dnsaddr2.value,wan_assign,form.hid_bridge_ipaddr.value))
		return true;
	else
		return false;
}

function checkbridge(form) //for bug 30286
{
     form.hid_bridge_ipaddr.value=form.BRIDGEaddr1.value+'.'+form.BRIDGEaddr2.value+'.'+form.BRIDGEaddr3.value+'.'+form.BRIDGEaddr4.value;
    form.hid_bridge_subnet.value=form.BRIDGEmask1.value+'.'+form.BRIDGEmask2.value+'.'+form.BRIDGEmask3.value+'.'+form.BRIDGEmask4.value;
    form.hid_bridge_gateway.value=form.BRIDGEgateway1.value+'.'+form.BRIDGEgateway2.value+'.'+form.BRIDGEgateway3.value+'.'+form.BRIDGEgateway4.value;
	form.bridge_dnsaddr1.value=form.BRIDGEDAddr1.value+'.'+form.BRIDGEDAddr2.value+'.'+form.BRIDGEDAddr3.value+'.'+form.BRIDGEDAddr4.value;
    form.bridge_dnsaddr2.value=form.BRIDGEPDAddr1.value+'.'+form.BRIDGEPDAddr2.value+'.'+form.BRIDGEPDAddr3.value+'.'+form.BRIDGEPDAddr4.value;

       /*if(form.enable_bridge_mode.checked == true)
       {
               if(form.device_name.value=="")
               {
                       alert("$device_name_null");
                      return false;
              }
              for(i=0;i<form.device_name.value.length;i++)
               {
                      if(isValidChar_space(form.device_name.value.charCodeAt(i))==false)
                       {
                               alert("$device_name_error");
                              return false;
                      }
               }
       }*/

        /*if( wps_progress_status == "2" || wps_progress_status == "3" || wps_progress_status == "start" )
       {
                alert("$wps_in_progress");
                return false;
        }*/

       if(form.enable_bridge_mode.checked == true)
       {
               form.hid_enable_bridgemode.value="1";
			   
               if(form.dyn_bridge_get_ip[1].checked == true)
               {
                       if(check_static_ip_mask_gtw()==false)
                               return false;			
                       if(check_static_dns(!(form.dyn_bridge_get_ip.checked)) == false)
                                return false;	
                       form.hid_dyn_bridge_get_ip.value="0";//for static
				}
               else
                       form.hid_dyn_bridge_get_ip.value="1"; //for dynamic
			   }
       else
       {
               form.hid_enable_bridgemode.value="0"; //when bridge mode is off 
			   form.hid_dyn_bridge_get_ip.value="1"; //for dynamic
       }

       form.submit();
}
