//Class: PLC device
function device(dev_info)
{
	var _this = this;	//save this to a private class variable to avoid some error may caused by event
	var each_info;
	
	each_info = dev_info.split(' ');
	
	var init = function()
	{
		_this.Model_name = each_info[0];
		_this.MAC = each_info[1];
		_this.IP = each_info[2];
		_this.FW_VERSION = each_info[3];
		_this.Linkrate = each_info[4];
		_this.Led_status = each_info[5];
		_this.Reset_status = each_info[6];
		_this.DAK = each_info[7];
		_this.Dev_type = each_info[8];
		_this.DAK_passwd = each_info[9];
		_this.Record_status = each_info[10];
	}
	
	init(_this);
}

//function: loadDevices()
function loadDevices()
{
	var i, dev_array;
	devices_set[0] = " ";
	
	for(i=1; i<=plc_avail_dev_num; i++)
	{
		dev_array = eval("avail_dev_Array" + i);
		dev_array = dev_array.replace(/\s+/g," ");		

		devices_set[i] = new device(dev_array);
	}
}
