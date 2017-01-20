#ifndef __PHSOCKET__H__
#define __PHSOCKET__H__

#ifdef _WIN32
#include <winsock.h>
inline int getSocketError()
{
	return GetLastError();
}
#else
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
#include <sys/time.h>
#include <sys/timeb.h>
inline int getSocketError()
{
	return errno;
}

#endif

class CPHSocket
{
//Constructors and destructors
public:
	CPHSocket();
	virtual ~CPHSocket();
//Attributes
protected:
	int	m_hSocket;
	int	m_LastError;
public:
	int	GetLastError();
	int ReadOneLine(char * lpszBuf, int nBufLen);
	void SetBlockingMode(bool bBlocking);
	
//Operations
public:
	virtual bool	Accept(CPHSocket& rConnectSocket, sockaddr_in* lpSockAddr = NULL, int* lpSockAddrLen = NULL);
	virtual bool	Attach(int s);
	virtual int		Detach();
	virtual bool	Bind(unsigned short nSocketPort, char *lpszSocketAddress = NULL);
	virtual void	Close();
	virtual bool	Connect(char *lpszHostAddress, unsigned short nHostPort, int *nAddressIndex = NULL, char *szSelectedAddress = NULL);
	virtual bool	Connect(sockaddr_in* pSockAddr, int nSockAddrLen);
	virtual bool	Listen(int nConnectionBacklog = 5);
	virtual int		Receive(void* lpBuf, int nBufLen, int nFlag = 0);
	virtual int		ReceiveFrom(void* lpBuf, int nBufLen, char *rSocketAddress, unsigned short &rSocketPort, int nFlags = 0);
	virtual int		ReceiveFrom(void* lpBuf, int nBufLen, sockaddr_in* pSockAddr, int* pnSockAddrLen, int nFlags = 0);
	virtual int		Send(void* lpBuf, int nBufLen, int nFlag = 0);
	virtual int		SendTo(const void* lpBuf, int nBufLen, unsigned short nHostPort, char *lpszHostAddress = NULL, int nFlags = 0);
	virtual int		SendTo(const void* lpBuf, int nBufLen, sockaddr_in* pSockAddr, int nSockAddrLen, int nFlags = 0);
	virtual bool	Create(unsigned short nSocketPort = 0, int nSocketType = SOCK_STREAM, char *lpszSocketAddress = NULL);
	virtual int		DataReadable(int nTimeout); //in Seconds
public:
	static int SendTo(int s, const void* lpBuf, int nBufLen, unsigned short nHostPort, char *lpszHostAddress = NULL, int nFlags = 0);
};
#endif