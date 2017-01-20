#include "PHGlobal.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <stdlib.h>

#ifndef WIN32
#include <termios.h>
#endif

#include <time.h>

const char *convert_status_code(int nCode)
{
	static char buf[64] = "";

	switch (nCode)
	{
	case okConnecting:
		strcpy(buf,"okConnecting");
		break;
	case okConnected:
		strcpy(buf,"okConnected");
		break;
	case okAuthpassed:
		strcpy(buf,"okAuthpassed");
		break;
	case okDomainListed:
		strcpy(buf,"okDomainListed");
		break;
	case okDomainsRegistered:
		strcpy(buf,"okDomainsRegistered");
		break;
	case okKeepAliveRecved:
		strcpy(buf,"okKeepAliveRecved");
		break;
	case okRetrievingMisc:
		strcpy(buf,"okRetrievingMisc");
		break;
	case errorConnectFailed:
		strcpy(buf,"errorConnectFailed");
		break;
	case errorSocketInitialFailed:
		strcpy(buf,"errorSocketInitialFailed");
		break;
	case errorAuthFailed:
		strcpy(buf,"errorAuthFailed");
		break;
	case errorDomainListFailed:
		strcpy(buf,"errorDomainListFailed");
		break;
	case errorDomainRegisterFailed:
		strcpy(buf,"errorDomainRegisterFailed");
		break;
	case errorUpdateTimeout:
		strcpy(buf,"errorUpdateTimeout");
		break;
	case errorKeepAliveError:
		strcpy(buf,"errorKeepAliveError");
		break;
	case errorRetrying:
		strcpy(buf,"errorRetrying");
		break;
	case okNormal:
		strcpy(buf,"okNormal");
		break;
	case okNoData:
		strcpy(buf,"okNoData");
		break;
	case okServerER:
		strcpy(buf,"okServerER");
		break;
	case errorOccupyReconnect:
		strcpy(buf,"errorOccupyReconnect");
		break;
	case okRedirecting:
		strcpy(buf,"okRedirecting");
		break;
	case errorAuthBusy:
		strcpy(buf,"errorAuthBusy");
		break;
	case errorStatDetailInfoFailed:
		strcpy(buf,"errorAuthBusy");
		break;
	}

	return buf;
}


const char *my_inet_ntoa(int ip)
{
	in_addr addr;
	addr.s_addr = ip;
	return inet_ntoa(addr);
}


PHGlobal::PHGlobal()
{
	init();
}

PHGlobal::~PHGlobal()
{
}

void PHGlobal::init()
{
	strcpy(szHost,"phddns60.oray.net");
	strcpy(szUserID,"");
	strcpy(szUserPWD,"");
	strcpy(szBindAddress,"");
	nUserType = 0;
	nPort = 6060;

	bTcpUpdateSuccessed = false;
	strcpy(szChallenge,"");
	nChallengeLen = 0;
	nChatID = nStartID = nLastResponseID = nAddressIndex = 0;
	tmLastResponse = -1;
	ip = 0;
	strcpy(szTcpConnectAddress,"");

	cLastResult = -1;

	uptime = time(0);
	lasttcptime = 0;

	strcpy(szActiveDomains[0],".");
}


