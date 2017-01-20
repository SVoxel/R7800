// PHGlobal.h: interface for the CBaseThread class.
//
//////////////////////////////////////////////////////////////////////
/*! \file PHGlobal.h
*  \author skyvense
*  \date   2009-09-14
*  \brief PHDDNS 客户端实现	
*/

#ifndef __PHGLOBAL__H__
#define __PHGLOBAL__H__
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#endif

#ifdef WIN32
#include <winsock.h>
#define sleep(x) Sleep(x*1000)
#endif

#include <list>
#include <string>

using namespace std;
#define MAX_TCP_PACKET_LEN	8192

//! 客户端状态
enum MessageCodes
{
	okConnected = 0,
	okAuthpassed,
	okDomainListed,
	okDomainsRegistered,
	okKeepAliveRecved,
	okConnecting,
	okRetrievingMisc,
	okRedirecting,

	errorConnectFailed = 0x100,
	errorSocketInitialFailed,
	errorAuthFailed,
	errorDomainListFailed,
	errorDomainRegisterFailed,
	errorUpdateTimeout,
	errorKeepAliveError,
	errorRetrying,
	errorAuthBusy,
	errorStatDetailInfoFailed,
	

	okNormal = 0x120,
	okNoData,
	okServerER,
	errorOccupyReconnect,
};

//! TCP指令表
#define COMMAND_AUTH	"auth router6\r\n"
#define COMMAND_REGI    "regi a"
#define COMMAND_CNFM    "cnfm\r\n"
#define COMMAND_STAT_USER    "stat user\r\n"
#define COMMAND_STAT_DOM     "stat domain\r\n"
#define COMMAND_QUIT    "quit\r\n"

//! 心跳包更新指令
#define UDP_OPCODE_UPDATE_VER2		0x2010
//! 心跳包服务器返回正常
#define UDP_OPCODE_UPDATE_OK		0x2050
//! 心跳包服务器返回错误
#define UDP_OPCODE_UPDATE_ERROR		1000

//! 心跳包注销登录
#define UDP_OPCODE_LOGOUT			11

//! 心跳包核心大小
#define KEEPALIVE_PACKET_LEN	20

//! 转换状态码到文本串
const char *convert_status_code(int nCode);

//! 转换IP地址到文本串
const char *my_inet_ntoa(int ip);

//! 心跳包结构
struct DATA_KEEPALIVE 
{
	//! 对话ID
	int lChatID;
	//! 操作码
	int lOpCode;
	//! 包ID
	int lID;
	//! 校验和
	int lSum;
	//! 保留字
	int lReserved;
};

//! 更新包扩展结构，用于服务器返回时IP地址
struct DATA_KEEPALIVE_EXT
{
	DATA_KEEPALIVE keepalive;
	int ip;
};

//! 全局变量
struct PHGlobal
{
	PHGlobal();
	~PHGlobal();
//basic system info
	//! 嵌入式客户端信息，4位客户端ID + 4位版本号
	long clientinfo;
	//! 嵌入式验证码
	long challengekey;

	//! 服务器地址
	char szHost[256];
	//! 服务器端口，6060固定
	short nPort;
	//! 用户账号名
	char szUserID[256];
	//! 用户密码明码
	char szUserPWD[256];
	//! 本地绑定地址，默认填空，用于多个网卡时指定出口
	char szBindAddress[256];
//run-time controll variables
	//! 用户类型
	int nUserType;
	bool bTcpUpdateSuccessed;
	char szChallenge[256];
	int nChallengeLen;
	int nChatID,nStartID,nLastResponseID;
	int tmLastResponse;
	int nAddressIndex;
	char szTcpConnectAddress[32];

	int cLastResult;
	int ip;

	int uptime;
	int lasttcptime;

	char szActiveDomains[255][255];

	std::string szUserInfo;
	std::string szDomainInfo;

	void init();
};


#endif