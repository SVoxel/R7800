// phupdate.h: interface for the CBaseThread class.
//
//////////////////////////////////////////////////////////////////////
/*! \file phupdate.h
*  \author skyvense
*  \date   2009-09-14
*  \brief PHDDNS 客户端实现	
*/

#ifndef _CUPDATECORE_H
#define _CUPDATECORE_H

#include "common/PHSocket.h"
#include "PHGlobal.h"

//! 花生壳DDNS客户端实现基类
/*!
*/
class CUpdateBase
{
public:
	//! 构造, 初始化必要的变量
	CUpdateBase()
	{
		bNeed_connect = true;
		tmLastSend = 0;
	}
	
	//! 保存了启动参数与各种运行时信息
	PHGlobal phglobal;
	
	//! 步进调用，配置好参数后需要立即进入此函数，函数返回下次需要执行本函数的时间（秒数）
	int step();	

	//! 停止花生壳DDNS更新，重新配置参数后可进入另一个
	void stop();
protected:
	//! 重载此函数得到状态变更，必须重载
	/*! status可能的值：
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
		其中：
		1、当status为okDomainsRegistered时，data返回用户级别：0(免费),1(专业),2(商业)
		2、当status为okKeepAliveRecved时，data返回客户端IP地址（整数形式）
		3、其他情况下，data一直为0
	*/
	virtual void OnStatusChanged(int status, long data) = 0;
	//! 重载此函数得到注册的域名，每条域名被执行一次
	virtual void OnDomainRegistered(std::string domain){}
	//! 重载此函数得到用户信息，XML格式
	virtual void OnUserInfo(std::string userInfo){}
	//! 重载此函数得到用户域名信息，XML格式
	virtual void OnAccountDomainInfo(std::string domainInfo){}
private:
	//! 用于与服务器连接的socket句柄
	CPHSocket m_tcpsocket,m_udpsocket;
	//! 初始化socket
	bool InitializeSockets();
	//! 关闭所有socket
	bool DestroySockets();
	//! 与DDNS服务器连接的TCP主过程
	int ExecuteUpdate();
	//! 启动UDP“连接”
	bool BeginKeepAlive();
	//! 发送一个UDP心跳包
	bool SendKeepAlive(int opCode);
	//! 接收心跳包返回
	int RecvKeepaliveResponse();
	//! 当前是否需要进行TCP连接
	bool bNeed_connect;
	//! 最后一次发送心跳包的时间	
	long tmLastSend;
};
#endif