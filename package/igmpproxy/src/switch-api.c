#include "defs.h"
#include <linux/sockios.h>
#include "switch-api.h"
#include <unistd.h>
#include <sys/ioctl.h>

static void 
IpAddrToMacAddr(uint32 group, fal_mac_addr_t *mac)
{
    group = ntohl(group);
    
    mac->addr[0] = 0x01;
    mac->addr[1] = 0x00;
    mac->addr[2] = 0x5e;
    mac->addr[3] = (group >> 16) & 0x7f;
    mac->addr[4] = (group >> 8) & 0xff;
    mac->addr[5] = group & 0xff;
    
}

#if 0
static void
error_print(sw_error_t rtn)
{
	switch (rtn) {
    case SW_FAIL:{
        atlog(LOG_WARNING, 0, "Operation failed\n");
        break;
    }
    case SW_BAD_VALUE:{
        atlog(LOG_WARNING, 0, "Illegal value\n");
        break;
    }
    case SW_OUT_OF_RANGE:{
        atlog(LOG_WARNING, 0, "Value is out of range\n");
        break;
    }
    case SW_BAD_PARAM:{
        atlog(LOG_WARNING, 0, "Illegal parameter(s)\n");
        break;
    }
    case SW_BAD_PTR:{
        atlog(LOG_WARNING, 0, "Illegal pointer value\n");
        break;
    }
    case SW_BAD_LEN:{
        atlog(LOG_WARNING, 0, "Wrong length\n");
        break;
    }
    case SW_READ_ERROR:{
        atlog(LOG_WARNING, 0, "Read operation failed\n");
        break;
    }
    case SW_WRITE_ERROR:{
        atlog(LOG_WARNING, 0, "Write operation failed\n");
        break;
    }
    case SW_CREATE_ERROR:{
        atlog(LOG_WARNING, 0, "Fail in creating an entry\n");
        break;
    }
    case SW_DELETE_ERROR:{
        atlog(LOG_WARNING, 0, "Fail in deleteing an entry\n");
        break;
    }
    case SW_NOT_FOUND:{
        atlog(LOG_WARNING, 0, "Entry not found\n");
        break;
    }
    case SW_NO_CHANGE:{
        atlog(LOG_WARNING, 0, "The parameter(s) is the same\n");
        break;
    }
    case SW_NO_MORE:{
        atlog(LOG_WARNING, 0, "No more entry found \n");
        break;
    }
    case SW_NO_SUCH:{
        atlog(LOG_WARNING, 0, "No such entry \n");
        break;
    }
    case SW_ALREADY_EXIST:{
        atlog(LOG_WARNING, 0, "Tried to create existing entry \n");
        break;
    }
    case SW_FULL:{
        atlog(LOG_WARNING, 0, "Table is full \n");
        break;
    }
    case SW_EMPTY:{
        atlog(LOG_WARNING, 0, "Table is empty \n");
        break;
    }
    case SW_NOT_SUPPORTED:{
        atlog(LOG_WARNING, 0, "This request is not support   \n");
        break;
    }
    case SW_NOT_IMPLEMENTED:{
        atlog(LOG_WARNING, 0, "This request is not implemented \n");
        break;
    }
    case SW_NOT_INITIALIZED:{
        atlog(LOG_WARNING, 0, "The item is not initialized \n");
        break;
    }
    case SW_BUSY:{
        atlog(LOG_WARNING, 0, "Operation is still running n");
        break;
    }
    case SW_TIMEOUT:{
        atlog(LOG_WARNING, 0, "Operation Time Out \n");
        break;
    }
    case SW_DISABLE:{
        atlog(LOG_WARNING, 0, "Operation is disabled \n");
        break;
    }
    case SW_NO_RESOURCE:{
        atlog(LOG_WARNING, 0, "Resource not available (memory ...)\n");
        break;
    }
    case SW_INIT_ERROR:{
        atlog(LOG_WARNING, 0, "Error occured while INIT process\n");
        break;
    }
    case SW_NOT_READY:{
        atlog(LOG_WARNING, 0, "The other side is not ready yet \n");
        break;
    }
    case SW_OUT_OF_MEM:{
        atlog(LOG_WARNING, 0, "Cpu memory allocation failed\n");
        break;
    }
    case SW_ABORTED:{
        atlog(LOG_WARNING, 0, "Operation has been aborted.\n");
        break;
    }

    default:
        atlog(LOG_WARNING, 0, "ioctl error return code <%d>\n", rtn);
    }
}
#endif

int addMFdb(uint32 group, uint32 port){


    int s = -1;
    struct ifreq ifr = {};
    arl_struct_t arl;
    fal_mac_addr_t macAddr;

    if(port > 31) return -1;
    
    if(0 > (s=socket(AF_INET, SOCK_DGRAM, 0))) {
        return -1;
    }

    memcpy(ifr.ifr_name, IF_NAME, IFNAMSIZ);
    IpAddrToMacAddr(group, &macAddr);
    memcpy(&arl.hwaddr,&macAddr,6);

    arl.port_map = port;
    arl.sa_drop = 0;

    memcpy(&ifr.ifr_ifru.ifru_mtu,&arl,sizeof(arl_struct_t));

    if (ioctl(s, SIOCDEVPRIVATE|0xd, &ifr) < 0) {
        close(s);
        return -1;
    } 
    atlog(LOG_DEBUG, 0, "addMFdb group %x port %d.\n",group,port);
    close(s);
    return 0;
}

int delMFdb(uint32 group){

    int s = -1;
    struct ifreq ifr = {};
    arl_struct_t arl;
    fal_mac_addr_t macAddr;

    if(0 >= (s=socket(AF_INET, SOCK_DGRAM, 0))) {
        return -1;
    }

    memcpy(ifr.ifr_name, IF_NAME, IFNAMSIZ);
    IpAddrToMacAddr(group, &macAddr);
    memcpy(&arl.hwaddr,&macAddr,6);
   
    memcpy(&ifr.ifr_ifru.ifru_mtu,&arl,sizeof(arl_struct_t));

    if (ioctl(s, ATHSIOCDEVPRIVATE|0xe, &ifr) < 0) {
        close(s);
        return -1;
    }

    atlog(LOG_DEBUG, 0, "delMFdb group %x.OK.\n",group);
    close(s);
    return 0;
}

//0: switch off the unkown multicast packets over vlan. 1: allow the unknown multicaset packets over vlans.
int setMultiOnVlan(int enable)
{
    int s = -1;
    struct ifreq ifr;
      
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    memcpy(ifr.ifr_name, IF_NAME, IFNAMSIZ);

    ifr.ifr_ifru.ifru_ivalue = enable;

    if (ioctl(s, ATHSIOCDEVPRIVATE|0xf, &ifr) != 0) {
	   close(s);
	   return -1;
    }

    close(s);    atlog(LOG_DEBUG, 0,"setMultiOnVlan %d.\n",enable);
    return 0;
}


int enableHwIGMPS()
{
    int s = -1;
    struct ifreq ifr;
    uint32 port_id;
      
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    memcpy(ifr.ifr_name, IF_NAME, IFNAMSIZ);
    for(port_id = 1; port_id <= SW_MAX_NR_PORTS; port_id++){

        ifr.ifr_ifru.ifru_ivalue = port_id;

        if (ioctl(s, ATHSIOCDEVPRIVATE|0x8, &ifr) != 0) {
	   close(s);
	   return -1;
        }
    } 
    close(s);    atlog(LOG_DEBUG, 0,"enableHwIGMPS OK.\n");
    return 0;
}

int disableHwIGMPS()
{
    int s = -1;
    struct ifreq ifr;
    uint32 port_id;
       
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    memcpy(ifr.ifr_name, IF_NAME, IFNAMSIZ);
    for(port_id = 1; port_id <= SW_MAX_NR_PORTS; port_id++){

        ifr.ifr_ifru.ifru_ivalue = port_id;

        if (ioctl(s, ATHSIOCDEVPRIVATE|0x9, &ifr) != 0) {
	   close(s);
	   return -1;
        }
    } 
    close(s);
    atlog(LOG_DEBUG, 0,"disableHwIGMPS OK.\n");
    return 0;
}
