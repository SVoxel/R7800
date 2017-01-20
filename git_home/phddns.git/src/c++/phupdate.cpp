#include <time.h>
#include "phupdate.h"
#include "common/log.h"
//#include "common/blowfish.h"

#include "common/generate.h"
#include "common/lutil.h"
#include "common/coder.h"


bool CUpdateBase::InitializeSockets()
{
	DestroySockets();
	if (!m_tcpsocket.Create(0,SOCK_STREAM,phglobal.szBindAddress))
	{
		return false;
	}

	if (!m_udpsocket.Create(0,SOCK_DGRAM,phglobal.szBindAddress))
	{
		return false;
	}
	return true;
}

bool CUpdateBase::DestroySockets()
{
	m_tcpsocket.Close();
	m_udpsocket.Close();
	return true;
}

bool CUpdateBase::BeginKeepAlive()
{
	if (!m_udpsocket.Connect(phglobal.szTcpConnectAddress,phglobal.nPort,&phglobal.nAddressIndex)) return false;
	phglobal.nLastResponseID = time(0);
	return true;
}

bool CUpdateBase::SendKeepAlive(int opCode)
{
	if (!phglobal.bTcpUpdateSuccessed) return false;

	LOG(1) ("SendKeepAlive() %d\n",opCode);

	DATA_KEEPALIVE data;
	memset(&data,0,sizeof(data));
	data.lChatID = phglobal.nChatID;
	data.lID = phglobal.nStartID;
	data.lOpCode = opCode;
	data.lSum = 0 - (data.lID + data.lOpCode);
	data.lReserved = 0;

	char p1[KEEPALIVE_PACKET_LEN],p2[KEEPALIVE_PACKET_LEN];
	memcpy(p1,&data,KEEPALIVE_PACKET_LEN);
	memcpy(p2,&data,KEEPALIVE_PACKET_LEN);

	CCoder cr;
	cr.KeyEncode((UCHAR*)p2+4, (UCHAR*)p1+4, KEEPALIVE_PACKET_LEN-4, (UCHAR*)phglobal.szChallenge,phglobal.nChallengeLen, BLOWFISH);

	m_udpsocket.Send(p2,KEEPALIVE_PACKET_LEN,0);
	//RecvKeepaliveResponse();
	return true;
}

int CUpdateBase::RecvKeepaliveResponse()
{
	if (!phglobal.bTcpUpdateSuccessed) return errorOccupyReconnect;

	//prevent the thread to be suspended while waiting for data
	if (m_udpsocket.DataReadable(0)<=0) 
	{
		return okNoData;
	}
	//DATA_KEEPALIVE data;
	//if (m_udpsocket.Receive(&data,sizeof(DATA_KEEPALIVE),0)<=0) return FALSE;
	char temp[100];
	DATA_KEEPALIVE_EXT rdata;
	if (m_udpsocket.Receive(temp,sizeof(temp),0)<=0) return okNoData;
	memcpy(&rdata, temp, sizeof(DATA_KEEPALIVE_EXT));

	DATA_KEEPALIVE data = rdata.keepalive;

	char p1[KEEPALIVE_PACKET_LEN],p2[KEEPALIVE_PACKET_LEN];
	memcpy(p1,&data,KEEPALIVE_PACKET_LEN);
	memcpy(p2,&data,KEEPALIVE_PACKET_LEN);
	CCoder cr;
	cr.KeyDecode((UCHAR*)p2+4, (UCHAR*)p1+4, KEEPALIVE_PACKET_LEN-4, (UCHAR*)phglobal.szChallenge,phglobal.nChallengeLen, BLOWFISH);
	memcpy(&data,p2,KEEPALIVE_PACKET_LEN);
	phglobal.nStartID = data.lID + 1;
	
	LOG(1) (("RecvKeepaliveResponse() Data comes, OPCODE:%d\n"),data.lOpCode);
	if (data.lID - phglobal.nLastResponseID > 3 && phglobal.nLastResponseID != -1)
	{
		return errorOccupyReconnect;
	}

	phglobal.nLastResponseID = data.lID;
	phglobal.tmLastResponse = time(0);

	phglobal.ip = rdata.ip;

	if (data.lOpCode == UDP_OPCODE_UPDATE_ERROR) return okServerER;
	//if (data.lOpCode == UDP_OPCODE_LOGOUT) return okNormal;
	
	return okKeepAliveRecved;
}

int CUpdateBase::ExecuteUpdate()
{
	char buffer[1024];
    
    char username[128] = "";
	char key[128] = "";
	char sendbuffer[256];
	
    char domains[255][255];
    char regicommand[255];
	int i,len, totaldomains;
	long challengetime = 0;


	LOG(1) ("ExecuteUpdate Connecting %s.\n",phglobal.szHost);
	
	if (!m_tcpsocket.Connect(phglobal.szHost,phglobal.nPort,&phglobal.nAddressIndex,phglobal.szTcpConnectAddress))
	{
		LOG(1) ("ExecuteUpdate errorConnectFailed.\n");
		phglobal.nAddressIndex++;
		return errorConnectFailed;
	}
	//////////////////////////////////////////////////////////////////////////
	//Recv server hello string
	memset(buffer, 0, 128);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	if (len <=0 )
	{
		LOG(1) ("ExecuteUpdate Recv server hello string failed.\n");
		m_tcpsocket.Close();
		phglobal.nAddressIndex++;
		return errorConnectFailed;
	}

	LOG(1) (("SEND AUTH REQUEST COMMAND..."));
	m_tcpsocket.Send((char*)COMMAND_AUTH,sizeof(COMMAND_AUTH),0);
    LOG(1) (("OK.\n"));

	//////////////////////////////////////////////////////////////////////////
	//Recv server key string
	memset(buffer, 0, 128);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	if (len <=0 )
	{
		LOG(1) (("ExecuteUpdate Recv server key string failed.\n"));
		m_tcpsocket.Close();
		return errorConnectFailed;
	}
    LOG(1) (("SERVER SIDE KEY \"%s\" RECEIVED.\n"),buffer);

	//strcpy(buffer, "334 dOdgnlwYcYzkTV1tJ9Oe7Q==");
	phglobal.nChallengeLen =  lutil_b64_pton(buffer+4, (unsigned char *)phglobal.szChallenge, 256);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//Generate encoded auth string
	len = GenerateCrypt(phglobal.szUserID, phglobal.szUserPWD, buffer+4, phglobal.clientinfo, phglobal.challengekey, sendbuffer);
	strcat(sendbuffer, "\r\n");
    //Generate ok.
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//send auth data
	LOG(1) (("SEND AUTH DATA..."));
	m_tcpsocket.Send(sendbuffer,strlen(sendbuffer),0);
    LOG(1) (("OK\n"));

	memset(buffer, 0, 128);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	buffer[3] = 0;

	if (len <=0 )
	{
		LOG(1) (("ExecuteUpdate Recv server auth response failed.\n"));
		m_tcpsocket.Close();
		//modified skyvense 2005/10/08, for server db conn lost bug
		//return errorAuthFailed;
		return errorConnectFailed;
	}
	if (strcmp(buffer,"250")!=0 && strcmp(buffer,"536")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate auth failed.\n");
		m_tcpsocket.Close();
		
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
				strcpy(phglobal.szHost, pos0 + 1);
				
				m_tcpsocket.Close();
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
				phglobal.nUserType = atoi(pos0 + 1);
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//list domains
	for (i=0,totaldomains=0;i<255;i++)
    {
        memset(domains[i], 0, 255);
        m_tcpsocket.ReadOneLine(domains[i],255);
        LOG(1) (("ExecuteUpdate domain \"%s\"\n"),domains[i]);
        totaldomains++;
		strcpy(phglobal.szActiveDomains[i],domains[i]);
        if (domains[i][0] == '.') break;
    }
	if (totaldomains<=0)
	{
		LOG(1) (("ExecuteUpdate Domain List Failed.\n"));
		m_tcpsocket.Close();
		return errorDomainListFailed;
	}

	phglobal.cLastResult = okDomainListed;
	OnStatusChanged(phglobal.cLastResult, 0);
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
        m_tcpsocket.Send(regicommand,strlen(regicommand),0);
    }

	//////////////////////////////////////////////////////////////////////////
	//send confirm
	LOG(1) (("SEND CNFM DATA..."));
    m_tcpsocket.Send((char*)COMMAND_CNFM,strlen(COMMAND_CNFM),0);
    LOG(1) (("OK\n"));
	
	for (i=0;i<totaldomains-1;i++)
    {
		memset(buffer, 0, 128);
		len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
		if (len <= 0)
		{
			LOG(1) (("ExecuteUpdate Recv server confirm response failed.\n"));
			m_tcpsocket.Close();
			return errorDomainRegisterFailed;
		}
		LOG(1) (("ExecuteUpdate %s\n"),buffer);
		OnDomainRegistered(domains[i]);
    }
	
	memset(buffer, 0, 128);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	if (len <= 0)
	{
		LOG(1) (("ExecuteUpdate Recv server confirmed chatID response failed.\n"));
		m_tcpsocket.Close();
		return errorDomainRegisterFailed;
	}
	LOG(1) (("%s\n"),buffer);

	//////////////////////////////////////////////////////////////////////////
	//find chatid & startid
	char *chatid = buffer + 4;
	char *startid = NULL;
	
	for (i=4;i<strlen(buffer);i++)
	{
		if (buffer[i] == ' ')
		{
			buffer[i] = 0;
			startid = buffer + i + 1;
			break;
		}
	}
	phglobal.nChatID = atoi(chatid);
	if (startid) phglobal.nStartID = atoi(startid);
	LOG(1) (("ExecuteUpdate nChatID:%d, nStartID:%d\n"),phglobal.nChatID,phglobal.nStartID);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//after confirm domain register, we begin to get user information
	m_tcpsocket.Send((void *)COMMAND_STAT_USER,sizeof(COMMAND_STAT_USER),0);
	memset(buffer, 0, 1024);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	buffer[3] = 0;
	if (len <= 0 || strcmp(buffer,"250")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate Recv server confirmed stat user response failed.\n");
		m_tcpsocket.Close();
		return errorStatDetailInfoFailed;
	}
	
	phglobal.szUserInfo = "";
	for (;;)
	{
		memset(buffer, 0, 1024);
        len = m_tcpsocket.ReadOneLine(buffer,1024);
        if (buffer[0] == '.' || len <= 0) break;
		phglobal.szUserInfo += buffer;
	}
	LOG(1) ("userinfo: \r\n%s\r\n", phglobal.szUserInfo.c_str());
	

	m_tcpsocket.Send((void *)COMMAND_STAT_DOM,sizeof(COMMAND_STAT_DOM),0);
	memset(buffer, 0, 1024);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	buffer[3] = 0;
	if (len <= 0 || strcmp(buffer,"250")!=0)
	{
		LOG(1) ("CTcpThread::ExecuteUpdate Recv server confirmed stat user response failed.\n");
		m_tcpsocket.Close();
		return errorStatDetailInfoFailed;
	}
	
	phglobal.szDomainInfo = "";
	for (;;)
	{
		memset(buffer, 0, 1024);
        len = m_tcpsocket.ReadOneLine(buffer,1024);
        if (buffer[0] == '.' || len <= 0) break;
		phglobal.szDomainInfo += buffer;
	}
	LOG(1) ("domaininfo: \r\n%s\r\n", phglobal.szDomainInfo.c_str());

	//////////////////////////////////////////////////////////////////////////
	//good bye!
    LOG(1) (("SEND QUIT COMMAND..."));
	m_tcpsocket.Send((char*)COMMAND_QUIT,sizeof(COMMAND_QUIT),0);
    LOG(1) (("OK.\n"));
	
    memset(buffer, 0, 128);
	len = m_tcpsocket.ReadOneLine(buffer,sizeof(buffer));
	if (len <= 0)
	{
		LOG(1) (("ExecuteUpdate Recv server goodbye response failed.\n"));
		m_tcpsocket.Close();
		return okDomainsRegistered;
	}
	LOG(1) (("%s\n"),buffer);                               
	m_tcpsocket.Close();
	return okDomainsRegistered;
}

int CUpdateBase::step()
{
	if (bNeed_connect)
	{
		strcpy(phglobal.szActiveDomains[0],".");
		
		phglobal.cLastResult = okConnecting;
		OnStatusChanged(phglobal.cLastResult, 0);
		
		if (!InitializeSockets())
		{
			LOG(1) ("InitializeSockets failed, waiting for 5 seconds to retry...\n");
			phglobal.cLastResult = errorConnectFailed;
			OnStatusChanged(phglobal.cLastResult, 0);
			return 5;
		}
		
		int ret = ExecuteUpdate();
		phglobal.cLastResult = ret;
		OnStatusChanged(phglobal.cLastResult, ret == okDomainsRegistered ? phglobal.nUserType : 0);
		if (ret == okDomainsRegistered) 
		{
			OnUserInfo(phglobal.szUserInfo);
			OnAccountDomainInfo(phglobal.szDomainInfo);
			LOG(1) ("ExecuteUpdate OK, BeginKeepAlive!\n");
			phglobal.bTcpUpdateSuccessed = true;
			phglobal.tmLastResponse = time(0);
			bNeed_connect = false;
			BeginKeepAlive();
			phglobal.lasttcptime = tmLastSend = time(0);

		}
		else 
		{
			if (ret == okRedirecting)
			{
				phglobal.bTcpUpdateSuccessed = false;
				bNeed_connect = true;
				LOG(1) ("Need redirect, waiting for 5 seconds...\n");
				return 5;
			}
			if (ret == errorAuthFailed)
			{
				LOG(1) ("ExecuteUpdate AuthFailed, waiting for 600 seconds to retry...\n");
				return 600;
			}
			
			LOG(1) ("ExecuteUpdate failed, waiting for 30 seconds to retry...\n");
			return 30;
		}
		phglobal.nLastResponseID = -1;
	}
	else
	{
		if (time(0) - tmLastSend > (phglobal.nUserType >= 1 ? 30 : 60))
		{
			SendKeepAlive(UDP_OPCODE_UPDATE_VER2);
			tmLastSend = time(0);
		}
		int ret = RecvKeepaliveResponse();
		if (ret != okNormal && ret != okNoData) phglobal.cLastResult = ret;
		if (ret == errorOccupyReconnect)
		{
			LOG(1) ("RecvKeepaliveResponse failed, waiting for 30 seconds to reconnect...\n");
			bNeed_connect = true;
			phglobal.bTcpUpdateSuccessed = false;
			return 30;
		}
		else
		{
			if (ret == okKeepAliveRecved)
			{
				in_addr t;
				t.s_addr = phglobal.ip;
				LOG(1) ("Keepalive response received, client ip: %s\n",inet_ntoa(t));
				OnStatusChanged(phglobal.cLastResult, phglobal.ip);
			}
		}
		if (time(0) - phglobal.tmLastResponse > (phglobal.nUserType >= 1 ? 160 : 320) && phglobal.tmLastResponse != -1)
		{
			LOG(1) ("No response from server for %d seconds, reconnect immediately...\n", (phglobal.nUserType == 1 ? 160 : 320));
			phglobal.bTcpUpdateSuccessed = false;
			bNeed_connect = true;
			return 1;
		}
	}
	return 1;
}

void CUpdateBase::stop()
{
	SendKeepAlive(UDP_OPCODE_LOGOUT);
	tmLastSend = time(0);
	sleep(1); //ensure data sent
	DestroySockets();
}
