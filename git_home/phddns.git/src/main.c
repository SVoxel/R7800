#include "config.h"
#include "phupdate.h"
#include "log.h"
#include <signal.h>     /* for singal handle */
#ifndef WIN32
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>
#include <unistd.h>     /* for close() */

static void create_pidfile()
{
    FILE *pidfile;
	
    if ((pidfile = fopen(PID_FILE, "w")) != NULL) {
		fprintf(pidfile, "%d\n", getpid());
		(void) fclose(pidfile);
    } else {
		printf("Failed to create pid file %s: %m", PID_FILE);
    }
}
#endif


PHGlobal global;
static void my_handleSIG (int sig)
{
	if (sig == SIGINT)
	{
#ifndef WIN32
		remove(PID_FILE);
		remove(STATUS_FILE);
		remove(DOMAINLS_FILE);
		remove(DOMAINIF_FILE);
		remove(USERTP_FILE);
#endif
		printf ("signal = SIGINT\n");
		phddns_stop(&global);
		exit(0);
	}
	if (sig == SIGTERM)
	{
#ifndef WIN32
		remove(PID_FILE);
#endif
		printf ("signal = SIGTERM\n");
		phddns_stop(&global);
	}
	signal (sig, my_handleSIG);
}

//状态更新回调
static void myOnStatusChanged(int status, long data)
{
	FILE *fp = fopen(STATUS_FILE, "w");
	if (fp) {
		fprintf(fp, "%d", status);
		fclose(fp);
	}
	if((fp = fopen(USERTP_FILE, "w"))) {
		fprintf(fp, "%d", data);
		fclose(fp);
	}
}

//域名注册回调
static void myOnDomainRegistered(char *domain)
{
	printf("myOnDomainRegistered %s\n", domain);
}

static void myOnDomainList(char *domain)
{
	FILE *fp;
	if((fp = fopen(DOMAINLS_FILE, "a")) != NULL) {
		fprintf(fp, "%s\n", domain);
		fclose(fp);
	}
}

//用户信息XML数据回调
static void myOnUserInfo(char *userInfo, int len)
{
	printf("myOnUserInfo %s\n", userInfo);
}

//域名信息XML数据回调
static void myOnAccountDomainInfo(char *domainInfo, int len)
{
	FILE *fp = fopen(DOMAINIF_FILE, "w");
	if(fp) {
		fprintf(fp, "%s\n", domainInfo);
		fclose(fp);
	}
}

int main(int argc, char *argv[])
{
	void (*ohandler) (int);
#ifdef WIN32
	WORD VersionRequested;		// passed to WSAStartup
	WSADATA  WsaData;			// receives data from WSAStartup
	int error;

	VersionRequested = MAKEWORD(2, 0);

	//start Winsock 2
	error = WSAStartup(VersionRequested, &WsaData); 
	log_open("c:\\phclientlog.log", 1);	//empty file will cause we printf to stdout
#else
	
	remove(PID_FILE);
	remove(STATUS_FILE);
	remove(DOMAINLS_FILE);
	remove(DOMAINIF_FILE);
	remove(USERTP_FILE);
	if (argc < 4)
	{
		printf("This is a phddns sample by Oray\r\n\trun with argument: phddns phddns60.oray.net <account> <password>\r\n");
		return -1;
	}

	
	daemon(0,0);
	log_open("/tmp/phddns.log", 1);	//empty file will cause we printf to stdout
	create_pidfile();
#endif


	ohandler = signal (SIGINT, my_handleSIG);
	if (ohandler != SIG_DFL) {
		printf ("previous signal handler for SIGINT is not a default handler\n");
		signal (SIGINT, ohandler);
	}

	init_global(&global);

	global.cbOnStatusChanged = myOnStatusChanged;
	global.cbOnDomainRegistered = myOnDomainRegistered;
	global.cbOnDomainList= myOnDomainList;
	global.cbOnUserInfo = myOnUserInfo;
	global.cbOnAccountDomainInfo = myOnAccountDomainInfo;

//	set_default_callback(&global);
	//注意！！！！！！！！！！！！！！！！！！！！！！！！
	//以下两个值用户您临时测试（Oray可能随时删除或修改），正式发布前请填写您的实际分配值
	global.clientinfo = ORAY_CLIENT_INFO; 
	global.challengekey = ORAY_CHALLE_KEY;
	//注意！！！！！！！！！！！！！！！！！！！！！！！！

	strcpy(global.szHost, 
		argv[1]);			//你所拿到的服务器地址
	strcpy(global.szUserID, 
		argv[2]);							//Oray账号
	strcpy(global.szUserPWD, 
		argv[3]);							//对应的密码
	strcpy(global.szUserInterface,
		argv[4]);
	for (;;)
	{
		int next = phddns_step(&global);
		sleep(next);
	}
	phddns_stop(&global);
	return 0;
}
