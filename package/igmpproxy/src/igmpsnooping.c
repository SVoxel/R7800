#include "defs.h"

static int igmpSState = 0;
    
// Local function Prototypes
int igmpProxyInit();
void igmpProxyCleanUp();

void 
setIgmpSState(int state)
{
    igmpSState = state;
}

int
getIgmpSState()
{
    return igmpSState;
}

/**
*   Handles the initial startup of IGMP SNOOPING
*/
int
igmpSnoopInit()
{
    atlog(LOG_DEBUG, 0, "igmpSnoopInit...");
    initFdbTable();
//    enableHwIGMPS();

    //Disable unknown multicast packets over vlans.
    // marked by Kane, to avoid switch block ipv6 packets
//    setMultiOnVlan(0);

    //Insert 224.0.0.1 into ARL table
    addMFdb(0xe0000001,30);  

    //initSnoopBr();
    
    return 1;
}

/**
*   Clean up all on exit...
*/
void
igmpSnoopCleanUp()
{
    atlog(LOG_DEBUG, 0, "igmpSnoopCleanUp...");
    //clearSnoopBr();

    //Enable unknow multicaset packets over vlans.
    // marked by Kane, to avoid switch block ipv6 packets
//    setMultiOnVlan(1);

    // delete 224.0.0.1 from ARL table.
    delMFdb(0xe0000001);  
  
    clearAllFdbs();
    disableHwIGMPS();

}
