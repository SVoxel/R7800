#include "phupdate.h"
#include "common/log.h"
#include <signal.h>     /* for singal handle */

#include "common/generate.h"
#include "common/lutil.h"
#include "common/coder.h"

#ifndef WIN32
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>
#include <unistd.h>     /* for close() */

static void create_pidfile()
{
    FILE *pidfile;
    char pidfilename[128];
    sprintf(pidfilename, "%s", "/var/run/phddns.pid");
	
    if ((pidfile = fopen(pidfilename, "w")) != NULL) {
		fprintf(pidfile, "%d\n", getpid());
		(void) fclose(pidfile);
    } else {
		printf("Failed to create pid file %s: %m", pidfilename);
		pidfilename[0] = 0;
    }
}
#endif

class CPhUpdater : public CUpdateBase
{
public:
protected:
	virtual void OnStatusChanged(int status, long data)
	{
		printf("OnStatusChanged %s\n", convert_status_code(status));
		if (status == okKeepAliveRecved)
		{
			printf("IP: %d\n", data);
		}
		if (status == okDomainsRegistered)
		{
			printf("UserType: %d\n", data);
		}
	}
	virtual void OnDomainRegistered(std::string domain)
	{
		printf("OnDomainRegistered %s\n", domain.c_str());
	}
	virtual void OnUserInfo(std::string userInfo)
	{
		printf("OnUserInfo %s\n", userInfo.c_str());
	}
	virtual void OnAccountDomainInfo(std::string domainInfo)
	{
		printf("OnAccountDomainInfo %s\n", domainInfo.c_str());
	}
private:
};
CPhUpdater phup;

// process terminate & interrupt signal handler
static void my_handleSIG (int sig)
{
	if (sig == SIGINT)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		printf ("signal = SIGINT\n");
		phup.stop();
		exit(0);
	}
	if (sig == SIGTERM)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		printf ("signal = SIGTERM\n");
		phup.stop();
	}
	signal (sig, my_handleSIG);
}

int main(int argc, char *argv[])
{
#if 0
	char szChallenge[64];
	char buffer[256] = "eCnA7ZGFCjRKTlh8SSnoLg==";
	int nChallengeLen =  lutil_b64_pton(buffer, (unsigned char *)szChallenge, 256);

	char temp[100] = {0x0A,0xFC,0x60,0x00,0x48,0xA7,0x56,0x62,0xCC,0x46,0xAB,0x83,0xD9,0x1B,0x18,0xA5,0xDF,0x09,0xA9,0x2D};
	DATA_KEEPALIVE_EXT rdata;

	//if (m_udpsocket.Receive(temp,sizeof(temp),0)<=0) return okNoData;

	memcpy(&rdata, temp, sizeof(DATA_KEEPALIVE_EXT));

	DATA_KEEPALIVE data = rdata.keepalive;

	char p1[KEEPALIVE_PACKET_LEN],p2[KEEPALIVE_PACKET_LEN];
	memcpy(p1,&data,KEEPALIVE_PACKET_LEN);
	memcpy(p2,&data,KEEPALIVE_PACKET_LEN);
	CCoder cr;
	cr.KeyDecode((UCHAR*)p2+4, (UCHAR*)p1+4, KEEPALIVE_PACKET_LEN-4, (UCHAR*)szChallenge,nChallengeLen, BLOWFISH);
	memcpy(&data,p2,KEEPALIVE_PACKET_LEN);
#endif
	if (argc < 4)
	{
		printf("This is a phddns sample by Oray\r\n\trun with argument: phddns phddns60.oray.net <account> <password>\r\n");
		return -1;
	}
#ifdef WIN32
	WORD VersionRequested;		// passed to WSAStartup
	WSADATA  WsaData;			// receives data from WSAStartup
	int error;
	
	VersionRequested = MAKEWORD(2, 0);
	
	//start Winsock 2
	error = WSAStartup(VersionRequested, &WsaData); 
	Log::open("c:\\phclientlog.log", 1);	//empty file will cause we printf to stdout
#else


	Log::open("/var/log/phddns.log", 1);	//empty file will cause we printf to stdout
	create_pidfile();
#endif
	void (*ohandler) (int);
	ohandler = signal (SIGINT, my_handleSIG);
	if (ohandler != SIG_DFL) {
		printf ("previous signal handler for SIGINT is not a default handler\n");
		signal (SIGINT, ohandler);
	}

	//注意！！！！！！！！！！！！！！！！！！！！！！！！
	//以下两个值用户您临时测试（Oray可能随时删除或修改），正式发布前请填写您的实际分配值
	phup.phglobal.clientinfo = 0x26AB9779; 		//这里填写刚才第二步算出的值
	phup.phglobal.challengekey = 0xCD9674D;	//这里填写嵌入式认证码
	//注意！！！！！！！！！！！！！！！！！！！！！！！！

	strcpy(phup.phglobal.szHost, 
			argv[1]);			//你所拿到的服务器地址
	strcpy(phup.phglobal.szUserID, 
			argv[2]);							//Oray账号
	strcpy(phup.phglobal.szUserPWD, 
			argv[3]);							//对应的密码

	for (;;)
	{
		int next = phup.step();
		sleep(next);
	}
	phup.stop();

	return 0;
}