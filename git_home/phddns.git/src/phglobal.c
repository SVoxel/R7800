#include "phglobal.h"
#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <stdlib.h>
#include <stdio.h>

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
	struct in_addr addr;
	addr.s_addr = ip;
	return inet_ntoa(addr);
}

static void defOnStatusChanged(int status, int data)
{
	FILE *fp;
	if((fp = fopen(STATUS_FILE, "w")) != NULL) {
			fprintf(fp, "%d", status);
			fclose(fp);
	}
	if (status == okDomainsRegistered)
	{
		if((fp = fopen(USERTP_FILE, "w")) != NULL) {
			fprintf(fp, "%d", data);
			fclose(fp);
		}
	}
}

static void defOnDomainRegistered(char *domain)
{
	printf("defOnDomainRegistered %s\n", domain);
}

static void defOnDomainList(char *domain)
{
	FILE *fp;
	if((fp = fopen(DOMAINLS_FILE, "a")) != NULL) {
		fprintf(fp, "%s\n", domain);
		fclose(fp);
	}
}

static void defOnUserInfo(char *userInfo, int len)
{
	printf("defOnUserInfo %s\n", userInfo);
}

static void defOnAccountDomainInfo(char *domainInfo, int len)
{
	FILE *fp;
	if((fp = fopen(DOMAINIF_FILE, "w")) != NULL) {
		fprintf(fp, "%s\n", domainInfo);
		fclose(fp);
	}
}

void init_global(PHGlobal *global)
{
	global->m_httpsocket = INVALID_SOCKET;
	global->httpPort = 80;
	strcpy(global->httpHost, "phservice2.oray.net");

	strcpy(global->szHost,"phddns60.oray.net");
	strcpy(global->szUserID,"");
	strcpy(global->szUserPWD,"");
	strcpy(global->szBindAddress,"");
	global->nUserType = 0;
	global->nPort = 6060;

	global->bTcpUpdateSuccessed = FALSE;
	strcpy(global->szChallenge,"");
	global->nChallengeLen = 0;
	global->nChatID = global->nStartID = global->nLastResponseID = global->nAddressIndex = 0;
	global->tmLastResponse = -1;
	global->ip = 0;
	strcpy(global->szTcpConnectAddress,"");

	global->cLastResult = -1;

	global->uptime = time(0);
	global->lasttcptime = 0;

	strcpy(global->szActiveDomains[0],".");

	global->bNeed_connect = TRUE;
	global->tmLastSend = 0;

	global->m_tcpsocket = global->m_udpsocket = INVALID_SOCKET;
	
	global->cbOnStatusChanged = NULL;
	global->cbOnDomainRegistered = NULL;
	global->cbOnUserInfo = NULL;
	global->cbOnAccountDomainInfo = NULL;
}

void set_default_callback(PHGlobal *global)
{
	global->cbOnStatusChanged = defOnStatusChanged;
	global->cbOnDomainRegistered = defOnDomainRegistered;
	global->cbOnDomainList= defOnDomainList;
	global->cbOnUserInfo = defOnUserInfo;
	global->cbOnAccountDomainInfo = defOnAccountDomainInfo;
}

