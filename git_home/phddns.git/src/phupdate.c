#include <time.h>
#include "phupdate.h"
#include "log.h"
#include "blowfish.h"
#include "config.h"
#include "generate.h"
#include "lutil.h"
#include <stdlib.h>
#include <syslog.h>
#include <linux/if.h>

BOOL InitializeSockets(PHGlobal *phglobal)
{
	DestroySockets(phglobal);
	if (!phCreate(&(phglobal->m_tcpsocket),0,SOCK_STREAM,phglobal->szBindAddress))
	{
		return FALSE;
	}

	if (!phCreate(&(phglobal->m_udpsocket),0,SOCK_DGRAM,phglobal->szBindAddress))
	{
		return FALSE;
	}

	if (!phCreate(&(phglobal->m_httpsocket),0,SOCK_STREAM,phglobal->szBindAddress))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL DestroySockets(PHGlobal *phglobal)
{
	phClose(&phglobal->m_tcpsocket);
	phClose(&phglobal->m_udpsocket);
	phClose(&phglobal->m_httpsocket);
	return TRUE;
}

BOOL BeginKeepAlive(PHGlobal *phglobal)
{
	if (!phConnect(phglobal->m_udpsocket, phglobal->szTcpConnectAddress,phglobal->nPort,&phglobal->nAddressIndex, NULL)) return FALSE;
	phglobal->nLastResponseID = time(0);
	return TRUE;
}

void Change_updatepack_Endian(DATA_KEEPALIVE *up)
{
	/* change endianness from x86 */
	Change_Endianness(&(up->lChatID));
	Change_Endianness(&(up->lOpCode));
	Change_Endianness(&(up->lID));
	Change_Endianness(&(up->lSum));
	Change_Endianness(&(up->lReserved));
}

BOOL SendKeepAlive(PHGlobal *phglobal, int opCode)
{
	DATA_KEEPALIVE data;
	blf_ctx blf;
	char p1[KEEPALIVE_PACKET_LEN],p2[KEEPALIVE_PACKET_LEN];

	memset(&data,0,sizeof(data));
	data.lChatID = phglobal->nChatID;
	data.lID = phglobal->nStartID;
	data.lOpCode = opCode;
	data.lSum = 0 - (data.lID + data.lOpCode);
	data.lReserved = 0;

	if (!phglobal->bTcpUpdateSuccessed) return FALSE;

	LOG(1) ("SendKeepAlive() %d\n",opCode);


	InitBlowfish(&blf, (unsigned char*)phglobal->szChallenge,phglobal->nChallengeLen);
	memcpy(p1,&data,KEEPALIVE_PACKET_LEN);
	memcpy(p2,&data,KEEPALIVE_PACKET_LEN);
	Blowfish_EnCode(&blf, p1+4,p2+4,KEEPALIVE_PACKET_LEN-4);

	Change_updatepack_Endian((DATA_KEEPALIVE*)p2);

	phSend(phglobal->m_udpsocket, p2, KEEPALIVE_PACKET_LEN,0);
	//RecvKeepaliveResponse();
	return TRUE;
}

int RecvKeepaliveResponse(PHGlobal *phglobal)
{
	char temp[100];
	DATA_KEEPALIVE_EXT rdata;
	DATA_KEEPALIVE data;
	blf_ctx blf;
	char p1[KEEPALIVE_PACKET_LEN],p2[KEEPALIVE_PACKET_LEN];

	if (!phglobal->bTcpUpdateSuccessed) return errorOccupyReconnect;

	//prevent the thread to be suspended while waiting for data
	if (phDataReadable(phglobal->m_udpsocket, 0)<=0) 
	{
		return okNoData;
	}
	//DATA_KEEPALIVE data;
	//if (m_udpsocket.Receive(&data,sizeof(DATA_KEEPALIVE),0)<=0) return FALSE;
	if (phReceive(phglobal->m_udpsocket, temp,sizeof(temp),0)<=0) return okNoData;
	memcpy(&rdata, temp, sizeof(DATA_KEEPALIVE_EXT));

	data = rdata.keepalive;

	Change_updatepack_Endian(&data);

	InitBlowfish(&blf, (unsigned char*)phglobal->szChallenge,phglobal->nChallengeLen);

	
	memcpy(p1,&data,KEEPALIVE_PACKET_LEN);
	memcpy(p2,&data,KEEPALIVE_PACKET_LEN);
	Blowfish_DeCode(&blf, p1+4,p2+4,KEEPALIVE_PACKET_LEN-4);
	memcpy(&data,p2,KEEPALIVE_PACKET_LEN);
	phglobal->nStartID = data.lID + 1;
	
	LOG(1) (("RecvKeepaliveResponse() Data comes, OPCODE:%d\n"),data.lOpCode);
	if (data.lID - phglobal->nLastResponseID > 3 && phglobal->nLastResponseID != -1)
	{
		return errorOccupyReconnect;
	}

	phglobal->nLastResponseID = data.lID;
	phglobal->tmLastResponse = time(0);

	phglobal->ip = rdata.ip;

	if (data.lOpCode == UDP_OPCODE_UPDATE_ERROR) return okServerER;
	//if (data.lOpCode == UDP_OPCODE_LOGOUT) return okNormal;
	
	return okKeepAliveRecved;
}

int ExecuteUpdate(PHGlobal *phglobal)
{
	char buffer[1024];
    
//      char username[128] = "";
//	char key[128] = "";
	char sendbuffer[256];
	
	char domains[255][255];
	char regicommand[255];
	int i,len, totaldomains;
//	long challengetime = 0;

	char *chatid = NULL;
	char *startid = NULL;
	char *xmldata = NULL;
	int buflen = 0;

	LOG(1) ("ExecuteUpdate Connecting %s.\n",phglobal->szHost);
	
	if (!phConnect(phglobal->m_tcpsocket, phglobal->szHost,phglobal->nPort,&phglobal->nAddressIndex,phglobal->szTcpConnectAddress))
	{
		LOG(1) ("ExecuteUpdate errorConnectFailed.\n");
		phglobal->nAddressIndex++;
		return errorConnectFailed;
	}
	//////////////////////////////////////////////////////////////////////////
	//Recv server hello string
	memset(buffer, 0, 128);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	if (len <=0 )
	{
		LOG(1) ("ExecuteUpdate Recv server hello string failed.\n");
		phClose(&phglobal->m_tcpsocket);
		phglobal->nAddressIndex++;
		return errorConnectFailed;
	}

	LOG(1) (("SEND AUTH REQUEST COMMAND..."));
	phSend(phglobal->m_tcpsocket, (char*)COMMAND_AUTH,sizeof(COMMAND_AUTH),0);
    LOG(1) (("OK.\n"));

	//////////////////////////////////////////////////////////////////////////
	//Recv server key string
	memset(buffer, 0, 128);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	if (len <=0 )
	{
		LOG(1) (("ExecuteUpdate Recv server key string failed.\n"));
		phClose(&phglobal->m_tcpsocket);
		return errorConnectFailed;
	}
    LOG(1) (("SERVER SIDE KEY \"%s\" RECEIVED.\n"),buffer);

	phglobal->nChallengeLen =  lutil_b64_pton(buffer+4, (unsigned char *)phglobal->szChallenge, 256);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//Generate encoded auth string
	len = GenerateCrypt(phglobal->szUserID, phglobal->szUserPWD, buffer+4, phglobal->clientinfo, phglobal->challengekey, sendbuffer);
	strcat(sendbuffer, "\r\n");
    //Generate ok.
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//send auth data
	LOG(1) (("SEND AUTH DATA..."));
	phSend(phglobal->m_tcpsocket, sendbuffer,strlen(sendbuffer),0);
    LOG(1) (("OK\n"));

	memset(buffer, 0, 128);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	buffer[3] = 0;

	if (len <=0 )
	{
		LOG(1) (("ExecuteUpdate Recv server auth response failed.\n"));
		phClose(&phglobal->m_tcpsocket);
		//modified skyvense 2005/10/08, for server db conn lost bug
		//return errorAuthFailed;
		return errorConnectFailed;
	}
	if (strcmp(buffer,"250")!=0 && strcmp(buffer,"536")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate auth failed.\n");
		phClose(&phglobal->m_tcpsocket);
		
		if (strstr(buffer + 4, "Busy.") != NULL) return errorAuthBusy;
		return errorAuthFailed;
	}
	if (strcmp(buffer,"536") == 0) //find redirected server address, and let us connect again to new server
	{
		char *pos0 = strchr(buffer + 4, '<');
		if (pos0)
		{
			char *pos1 = strchr(pos0 + 1, '>');
			if (pos1)
			{
				*pos1 = '\0';
				strcpy(phglobal->szHost, pos0 + 1);
				
				phClose(&phglobal->m_tcpsocket);
				return okRedirecting;
			}
		}
		return errorAuthFailed;
	}
	if (strcmp(buffer,"250") == 0) //get user type level, 0(free),1(pro),2(biz)
	{
		char *pos0 = strchr(buffer + 4, '<');
		if (pos0)
		{
			char *pos1 = strchr(pos0 + 1, '>');
			if (pos1)
			{
				*pos1 = '\0';
				phglobal->nUserType = atoi(pos0 + 1);
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//list domains
#ifndef DNI_ORAY_SUPPORT
    char hostname[128];
    memset(hostname, 0, 128);
    strcpy(hostname,config_get("sysDNSHost"));
    remove(DOMAINLS_FILE);
    for (i=0,totaldomains=0;;){
        memset(domains[i], 0, 256);
        phReadOneLine(phglobal->m_tcpsocket, domains[i],255);
	phglobal->cbOnDomainList(domains[i]);		
	/*if user select wildcards*/
        if(config_match("endis_wildcards","1")){
              if(strstr((char*)domains[i],(char*)hostname) != NULL || (domains[i][0] == '.')){
                    totaldomains++;
                    if(domains[i][0] == '.')  break;
                    i++;
              }
       }else{/*if user don't select wildcards*/
	       if(domains[i][0] == '.'){
	            strcpy((char*)domains[0],(char*)hostname);
	            domains[1][0] = '.';
	            totaldomains = 2;
	            break;
	       }
       }
    }
#else
	remove(DOMAINLS_FILE);
	for (i=0,totaldomains=0;i<255;i++)
    {
        memset(domains[i], 0, 255);
        phReadOneLine(phglobal->m_tcpsocket, domains[i],255);
        LOG(1) (("ExecuteUpdate domain \"%s\"\n"),domains[i]);
        totaldomains++;
	if (domains[i][0] == '.')
		break;
	else
		phglobal->cbOnDomainList(domains[i]);
//		strcpy(phglobal->szActiveDomains[i],domains[i]);
        if (domains[i][0] == '.') break;
    }
#endif
	if (totaldomains<=0)
	{
		LOG(1) (("ExecuteUpdate Domain List Failed.\n"));
		phClose(&phglobal->m_tcpsocket);
		return errorDomainListFailed;
	}

	phglobal->cLastResult = okDomainListed;
	//::SendMessage(theApp.m_hWndController,WM_DOMAIN_UPDATEMSG,okDomainListed,(long)domains);
	//////////////////////////////////////////////////////////////////////////
	//send domain regi commands list
	for (i=0;;i++)
    {
        if (domains[i][0] == '.') break;
		memset(regicommand, 0, 128);
        strcpy(regicommand, COMMAND_REGI);
        strcat(regicommand, " ");
        strcat(regicommand, domains[i]);
        strcat(regicommand, "\r\n");
        //printf("%s",regicommand);
        phSend(phglobal->m_tcpsocket,regicommand,strlen(regicommand),0);
    }

	//////////////////////////////////////////////////////////////////////////
	//send confirm
	LOG(1) (("SEND CNFM DATA..."));
    phSend(phglobal->m_tcpsocket,(char*)COMMAND_CNFM,strlen(COMMAND_CNFM),0);
    LOG(1) (("OK\n"));
	
	for (i=0;i<totaldomains-1;i++)
    {
		memset(buffer, 0, 128);
		len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
		if (len <= 0)
		{
			LOG(1) (("ExecuteUpdate Recv server confirm response failed.\n"));
			phClose(&phglobal->m_tcpsocket);
			return errorDomainRegisterFailed;
		}
		LOG(1) (("ExecuteUpdate %s\n"),buffer);
		//if (phglobal->cbOnDomainRegistered) phglobal->cbOnDomainRegistered(domains[i]);
    }
	
	memset(buffer, 0, 128);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	if (len <= 0)
	{
		LOG(1) (("ExecuteUpdate Recv server confirmed chatID response failed.\n"));
		phClose(&phglobal->m_tcpsocket);
		return errorDomainRegisterFailed;
	}
	LOG(1) (("%s\n"),buffer);

	//////////////////////////////////////////////////////////////////////////
	//find chatid & startid
	chatid = buffer + 4;
	startid = NULL;
	
	for (i=4;i<strlen(buffer);i++)
	{
		if (buffer[i] == ' ')
		{
			buffer[i] = 0;
			startid = buffer + i + 1;
			break;
		}
	}
	phglobal->nChatID = atoi(chatid);
	if (startid) phglobal->nStartID = atoi(startid);
	LOG(1) (("ExecuteUpdate nChatID:%d, nStartID:%d\n"),phglobal->nChatID,phglobal->nStartID);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
#ifndef DNI_ORAY_SUPPORT
	//after confirm domain register, we begin to get user information
	phSend(phglobal->m_tcpsocket,(void *)COMMAND_STAT_USER,sizeof(COMMAND_STAT_USER),0);
	memset(buffer, 0, 1024);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	buffer[3] = 0;
	if (len <= 0 || strcmp(buffer,"250")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate Recv server confirmed stat user response failed.\n");
		phClose(&phglobal->m_tcpsocket);
		return errorStatDetailInfoFailed;
	}
	
	buflen = MAX_PATH;
	xmldata = (char *)malloc(buflen);
	memset(xmldata, 0, buflen);
	
	for (;;)
	{
		memset(buffer, 0, 1024);
        len = phReadOneLine(phglobal->m_tcpsocket, buffer,1024);
        if (buffer[0] == '.' || len <= 0) break;
		if (buflen < strlen(xmldata) + len)
		{
			buflen += MAX_PATH;
			xmldata = realloc(xmldata, buflen);
			memset((xmldata + buflen) - MAX_PATH, 0, MAX_PATH);
		}
		strncat(xmldata, buffer, len);
	}
	LOG(1) ("userinfo: \r\n%s\r\n", xmldata);
	if (phglobal->cbOnUserInfo) phglobal->cbOnUserInfo(xmldata, strlen(xmldata));
	free(xmldata);
	buflen = 0;
#endif	

	phSend(phglobal->m_tcpsocket,(void *)COMMAND_STAT_DOM,sizeof(COMMAND_STAT_DOM),0);
	memset(buffer, 0, 1024);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	buffer[3] = 0;
	if (len <= 0 || strcmp(buffer,"250")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate Recv server confirmed stat user response failed.\n");
		phClose(&phglobal->m_tcpsocket);
		return errorStatDetailInfoFailed;
	}
	
	buflen = MAX_PATH;
	xmldata = (char *)malloc(buflen);
	memset(xmldata, 0, buflen);

	for (;;)
	{
		memset(buffer, 0, 1024);
        len = phReadOneLine(phglobal->m_tcpsocket, buffer,1024);
        if (buffer[0] == '.' || len <= 0) break;
		if (buflen < strlen(xmldata) + len)
		{
			buflen += MAX_PATH;
			xmldata = realloc(xmldata, buflen);
			memset((xmldata + buflen) - MAX_PATH, 0, MAX_PATH);
		}
		strncat(xmldata, buffer, len);
	}
	LOG(1) ("domaininfo: \r\n%s\r\n", xmldata);
	if (phglobal->cbOnAccountDomainInfo) phglobal->cbOnAccountDomainInfo(xmldata, strlen(xmldata));
	free(xmldata);
	buflen = 0;

	//////////////////////////////////////////////////////////////////////////
	//good bye!
    LOG(1) (("SEND QUIT COMMAND..."));
	phSend(phglobal->m_tcpsocket,(char*)COMMAND_QUIT,sizeof(COMMAND_QUIT),0);
    LOG(1) (("OK.\n"));
	
    memset(buffer, 0, 128);
	len = phReadOneLine(phglobal->m_tcpsocket, buffer,sizeof(buffer));
	if (len <= 0)
	{
		LOG(1) (("ExecuteUpdate Recv server goodbye response failed.\n"));
		phClose(&phglobal->m_tcpsocket);
		return okDomainsRegistered;
	}
	LOG(1) (("%s\n"),buffer);                               
	phClose(&phglobal->m_tcpsocket);
	return okDomainsRegistered;
}

int get_if_addr(int sock, char *name, struct sockaddr_in *sin)
{
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, name);
	/* why does this need to be done twice? */
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl(SIOCGIFADDR)");
		memset(sin, 0, sizeof(struct sockaddr_in));
		dprintf((stderr, "%s: %s\n", name, "unknown interface"));
		return -1;
	}
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl(SIOCGIFADDR)");
		memset(sin, 0, sizeof(struct sockaddr_in));
		dprintf((stderr, "%s: %s\n", name, "unknown interface"));
		return -1;
	}
	if(ifr.ifr_addr.sa_family == AF_INET)
	{
		memcpy(sin, &(ifr.ifr_addr), sizeof(struct sockaddr_in));
		if ( (strcmp(name, "ppp0") == 0) && (strcmp(inet_ntoa(sin->sin_addr),"10.64.64.64") == 0) ){
			return -1;
		}
		return 0;
	}
	else{
		memset(sin, 0, sizeof(struct sockaddr_in));
		dprintf((stderr, "%s: %s\n", name, "could not resolve interface"));
		return -1;
	}
	return -1;
}

#define INTERVAL_RETRY_TIME 3600
//3600 means an hour.Oray will retry every hour after 6 times retry.
int phddns_step(PHGlobal *phglobal)
{
	static retrytimes = 0;
	static useWebService = 0;
	int ret = 0;
	char ipaddr[32];
	int sock = -1;
	struct sockaddr_in sin;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(get_if_addr(sock, phglobal->szUserInterface, &sin) == 0){
		close(sock);
		strcpy(ipaddr,inet_ntoa(sin.sin_addr));
	}
	else
		strcpy(ipaddr,"0.0.0.0");

	if (phglobal->bNeed_connect)
	{
		strcpy(phglobal->szActiveDomains[0],".");
		
		phglobal->cLastResult = okConnecting;
		
		if (phglobal->cbOnStatusChanged) phglobal->cbOnStatusChanged(2, -1);
		
		if (!InitializeSockets(phglobal))
		{
			LOG(1) ("InitializeSockets failed, waiting for 5 seconds to retry...\n");
			retrytimes = 0;
			phglobal->cLastResult = errorConnectFailed;
			if (phglobal->cbOnStatusChanged) phglobal->cbOnStatusChanged(phglobal->cLastResult, 0);
			return 5;
		}
		
		if (useWebService == 1) ret = WebServiceLogin(phglobal);
		else  ret = ExecuteUpdate(phglobal);
		phglobal->cLastResult = ret;
		if (ret == okDomainsRegistered) 
		{
			syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration successful",ipaddr );
			retrytimes = 0;
			useWebService = 0;
			//OnUserInfo(phglobal->szUserInfo);
			//OnAccountDomainInfo(phglobal->szDomainInfo);
			phglobal->cbOnStatusChanged(1,phglobal->nUserType);
			LOG(1) ("ExecuteUpdate OK, BeginKeepAlive!\n");
			phglobal->bTcpUpdateSuccessed = TRUE;
			phglobal->tmLastResponse = time(0);
			phglobal->bNeed_connect = FALSE;
			BeginKeepAlive(phglobal);
			phglobal->lasttcptime = phglobal->tmLastSend = time(0);
		}
		else if (ret == okWebServiceRegistered) 
		{
			useWebService = 0;
			LOG(1) ("Web service login connect success,waiting for 5 seconds to start TCP connection...\n");
			return 5;
		}
		else 
		{
			if (ret == okRedirecting)
			{
				phglobal->bTcpUpdateSuccessed = FALSE;
				phglobal->bNeed_connect = TRUE;
				retrytimes = 0;
				LOG(1) ("Need redirect, waiting for 5 seconds...\n");
				return 5;
			}
			if (ret == errorAuthFailed)
			{
				syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration failure,", ipaddr);
				retrytimes = 0;
				phglobal->cbOnStatusChanged(3,-1);
				LOG(1) ("ExecuteUpdate AuthFailed, waiting for 600 seconds to retry...\n");
				return 600;
			}
			if (ret == errorConnectFailed)
			{
				useWebService = 1;
				return 5;
			}
			if (ret == errorWebServiceConnectFailed) 
			{
				//when use web service,and connected fail,just retry 6 time.if still fail just exit.
				useWebService = 1;
				LOG(1) ("Web service login connect failed,waiting for 5 seconds to retry...\n");
			}
			if (ret == errorWebServiceLoginFailed)
			{
				useWebService = 0;
				phglobal->cbOnStatusChanged(4,-1);
				LOG(1) ("Web service login failed, exit...\n");
				exit(-1);
			}
			if (retrytimes == 6)
			{
				//when oray can not get respond from server,it will retry 6 times every 30 second.
				//After 6 times,it still fail,then print failure log.
				//And then retry every hour.
				syslog(LOG_INFO, "[Dynamic DNS] host name %s registeration failure,", ipaddr);
				phglobal->cbOnStatusChanged(4,-1);
				retrytimes = 0;
				return INTERVAL_RETRY_TIME;
			}
			retrytimes++;
			LOG(1) ("ExecuteUpdate failed, waiting for 30 seconds to retry...\n");
			return 30;
		}
		phglobal->nLastResponseID = -1;
	}
	else
	{
		phglobal->cbOnStatusChanged(1,phglobal->nUserType);
		if (time(0) - phglobal->tmLastSend > (phglobal->nUserType >= 1 ? 30 : 60))
		{
			SendKeepAlive(phglobal, UDP_OPCODE_UPDATE_VER2);
			phglobal->tmLastSend = time(0);
		}
		ret = RecvKeepaliveResponse(phglobal);
		if (ret != okNormal && ret != okNoData) phglobal->cLastResult = ret;
		if (ret == errorOccupyReconnect)
		{
			LOG(1) ("RecvKeepaliveResponse failed, waiting for 30 seconds to reconnect...\n");
			phglobal->bNeed_connect = TRUE;
			phglobal->bTcpUpdateSuccessed = FALSE;
			phglobal->cbOnStatusChanged(2,-1);
			return 30;
		}
		else
		{
			if (ret == okKeepAliveRecved)
			{
				struct in_addr t;
				t.s_addr = phglobal->ip;
				LOG(1) ("Keepalive response received, client ip: %s\n",inet_ntoa(t));
			}
		}
		if (time(0) - phglobal->tmLastResponse > (phglobal->nUserType >= 1 ? 160 : 320) && phglobal->tmLastResponse != -1)
		{
			LOG(1) ("No response from server for %d seconds, reconnect immediately...\n", (phglobal->nUserType == 1 ? 160 : 320));
			phglobal->bTcpUpdateSuccessed = FALSE;
			phglobal->bNeed_connect = TRUE;
			return 1;
		}
	}
	return 1;
}

void phddns_stop(PHGlobal *phglobal)
{
	SendKeepAlive(phglobal, UDP_OPCODE_LOGOUT);
	phglobal->tmLastSend = time(0);
	sleep(1); //ensure data sent
	DestroySockets(phglobal);
}

int phBuffer(char *sendbuf, char *account, char *password, char *version, char *host)
{
	char temp[1000];

	memset(temp, 0, 1000);
	char *md5 = "61723192fe4e65dd69a1f0128b29751d";

	sprintf(temp,
       "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
       "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
       "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
       "xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
       "<soap:Header>"
       "<AuthHeader xmlns=\"http://tempuri.org/\">"
       "<Account>%s</Account>"
       "<Password>%s</Password>"
       "</AuthHeader>"
       "</soap:Header>"
       "<soap:Body>"
       "<GetMiscInfo xmlns=\"http://tempuri.org/\">"
       "<clientversion>%s</clientversion>"
       "</GetMiscInfo>"
       "</soap:Body>"
       "</soap:Envelope>", account, md5, version);
   
	int len = strlen(temp);
	
	sprintf(sendbuf,
       "POST /userinfo.asmx HTTP/1.1\r\nHost: %s\r\n"
       "Content-Type: text/xml; charset=utf-8\r\nContent-Length: %d\r\n"
       "SOAPAction: \"http://tempuri.org/GetMiscInfo\"\r\n\r\n"
       "%s",host, len, temp);

	printf("=================================\n");
	printf("send: %s\n", sendbuf);
	printf("=================================\n\n");

	return 0;
}

int GetLoginValue(char *package, const char *split, char *value)
{
	char *start_point, *end_point, *val;
	val = value;
	if((start_point = strstr(package, split)) != NULL) {
		start_point += (strlen(split)+1);
		end_point = strstr(start_point, split);
		end_point -= 2;
		while(start_point != end_point) {
			*val++ = *start_point++;
		}
		*val++ = '\0';
		
		printf("%s: %s\n", split, value);
		return 0;
	}
	return errorWebServiceLoginFailed;
}

int HandleHttpPackage(char *package, PHGlobal *phglobal)
{
	char result[16], phserver[128], usertype[4];
	int p = 0;
	char *start;
	if(strstr(package, "HTTP/1.1 200 OK") == NULL) {
		printf("WebServiceLogin failed...\n");
		return errorWebServiceLoginFailed;
	}
	GetLoginValue(package, "GetMiscInfoResult", result);
	if(strcmp(result, "0")) {
		printf("WebServiceLogin failed...\n");
		return errorWebServiceLoginFailed;
	}
	GetLoginValue(package, "PHServer", phserver);
	GetLoginValue(package, "UserType", usertype);

	while(phserver[p] == ' ' || phserver[p] == '\t')p++;
	start = &phserver[p];
	while(phserver[p] != ' ' && phserver[p] != '\t' && phserver[p] != '\n' && phserver[p])p++;
	phserver[p] = '\0';
	if (!strcpy(phglobal->szHost, start))
	{
		LOG(1) ("Get host fail,reconnect webservice 30 second later...\n");
		return errorWebServiceConnectFailed;
	}
	phglobal->nUserType = (usertype[0]-'0');

	return okWebServiceRegistered;
}

int WebServiceLogin(PHGlobal *phglobal)
{
	
	char buffer[2048];
	char soapbuffer[2048];
	char md5str[32];
	int buflen = 0, len;
	char version[] = "3.1.8.0"; //embeded version

	LOG(1) ("WebServiceLogin Connecting %s.\n",phglobal->httpHost);

	if (!phConnect(phglobal->m_httpsocket, phglobal->httpHost, phglobal->httpPort, &phglobal->nAddressIndex, NULL))
	{
		LOG(1) ("WebServiceLogin errorConnectFailed.\n");
		phglobal->nAddressIndex++;
		return errorWebServiceConnectFailed;
	}

	// http request
	MDEncode(phglobal->szUserPWD, strlen(phglobal->szUserPWD), md5str);
	sprintf(soapbuffer,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
		"xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Header>"
		"<AuthHeader xmlns=\"http://tempuri.org/\">"
		"<Account>%s</Account>"
		"<Password>%s</Password>"
		"</AuthHeader>"
		"</soap:Header>"
		"<soap:Body>"
		"<GetMiscInfo xmlns=\"http://tempuri.org/\">"
		"<clientversion>%s</clientversion>"
		"</GetMiscInfo>"
		"</soap:Body>"
		"</soap:Envelope>", phglobal->szUserID, md5str, version);

	buflen = strlen(soapbuffer);
	
	sprintf(buffer,
		"POST /userinfo.asmx HTTP/1.1\r\nHost: %s\r\n"
		"Content-Type: text/xml; charset=utf-8\r\nContent-Length: %d\r\n"
		"SOAPAction: \"http://tempuri.org/GetMiscInfo\"\r\n\r\n"
		"%s",phglobal->httpHost, buflen, soapbuffer);

	LOG(1) (("SEND WEB SERVICE LOGIN REQUEST COMMAND..."));
	phSend(phglobal->m_httpsocket, buffer,strlen(buffer),0);
	LOG(1) (("OK.\n"));

//////////////////////////////////////////////////////////////////////////
	//Recv server key string
	memset(buffer, 0, sizeof(buffer));
	len = recv(phglobal->m_httpsocket, buffer,sizeof(buffer), 0);
	if (len <=0 )
	{
		LOG(1) (("WebServiceLogin Recv failed.\n"));
		phClose(&phglobal->m_httpsocket);
		return errorWebServiceConnectFailed;
	}

	return HandleHttpPackage(buffer, phglobal);
}

