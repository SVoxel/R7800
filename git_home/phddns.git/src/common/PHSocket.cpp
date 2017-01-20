
#include "PHSocket.h"
#include <time.h>

#ifdef _WIN32
#define socklen_t int
#pragma comment(lib, "Ws2_32.lib")
#else
#define INVALID_SOCKET	-1
#define closesocket close
#define ioctlsocket ioctl
#endif
CPHSocket::CPHSocket()
{
	m_LastError = 0;
	m_hSocket = INVALID_SOCKET;
}

CPHSocket::~CPHSocket()
{
	if(m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

bool CPHSocket::Accept(CPHSocket& rConnectSocket, sockaddr_in *lpsockaddr_in, int* lpsockaddr_inLen)
{
	int hTemp = accept(m_hSocket, (sockaddr *)lpsockaddr_in, (socklen_t *)lpsockaddr_inLen);
	if(hTemp == INVALID_SOCKET)
	{
		m_LastError = getSocketError();
		return false;
	}
	else
		rConnectSocket.Attach(hTemp);
	return true;
}

bool CPHSocket::Attach(int s)
{
	m_hSocket = s;
	return true;
}

bool CPHSocket::Bind(unsigned short nSocketPort, char * lpszSocketAddress)
{
	struct sockaddr_in mysockaddr_in;
	memset(&mysockaddr_in,0,sizeof(sockaddr_in));

	mysockaddr_in.sin_family = AF_INET;

	if(lpszSocketAddress == NULL)
		mysockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		long lResult = inet_addr(lpszSocketAddress);
		if(lResult == INADDR_NONE)
		{
			//m_LastError = -1;
			//return false;
			lResult = INADDR_ANY;
		}
		mysockaddr_in.sin_addr.s_addr = lResult;
	}

	mysockaddr_in.sin_port = htons((u_short)nSocketPort);

	if(bind(m_hSocket, (const sockaddr*)&mysockaddr_in, sizeof(sockaddr_in)) < 0)
	{
		m_LastError = getSocketError();
		return false;
	}
	return true;
}

void CPHSocket::Close()
{
	if(m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

bool CPHSocket::Connect(char * lpszHostAddress, unsigned short nHostPort, int *nAddressIndex, char *szSelectedAddress)
{
	if (lpszHostAddress == NULL) return false;

	struct sockaddr_in mysockaddr_in;
	memset(&mysockaddr_in,0,sizeof(sockaddr_in));

	mysockaddr_in.sin_family = AF_INET;
	mysockaddr_in.sin_addr.s_addr = inet_addr(lpszHostAddress);

	if (mysockaddr_in.sin_addr.s_addr == INADDR_NONE)
	{
		hostent * lphost;
		lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
		{
			int index = 0;
			int total = 0;
			int i=0;
			while (lphost->h_addr_list[i]) {total++; i++;}

			if (nAddressIndex)
			{
				if (*nAddressIndex==-1)
				{
					if (total>1)
					{
						index = time(0)%total;
						*nAddressIndex = index;
					}
					else index = 0;
				}
				else
				{
					if (*nAddressIndex >= total) *nAddressIndex = 0;
					index = *nAddressIndex;
				}
			}
			
			mysockaddr_in.sin_addr.s_addr = ((in_addr *)lphost->h_addr_list[index])->s_addr;
			if (szSelectedAddress)
			{
				strcpy(szSelectedAddress,inet_ntoa(mysockaddr_in.sin_addr));
				//AfxMessageBox(szSelectedAddress);
			}
		}
		else
		{
			m_LastError = -1;
			return false;
		}
	}
	else
	{
		if (szSelectedAddress) strcpy(szSelectedAddress,inet_ntoa(mysockaddr_in.sin_addr));
	}

	mysockaddr_in.sin_port = htons((u_short)nHostPort);

	//////////////////////////////////////////////////////////////////////////
	int ret = -1;
//	unsigned long ul;
//	ul = 100;
//	ret = ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul);
//	if(ret < 0)
//	{
//		m_LastError = getSocketError();
//		return false;
//	}
//	
//	if (0!=connect(m_hSocket, (sockaddr_in*)&sockaddr_in, sizeof(sockaddr_in)))
//	{
//		if(getSocketError() != WSAEWOULDBLOCK)
//		{
//			m_LastError = getSocketError();
//			closesocket(m_hSocket);
//			return false;
//		}
//	}
//	//select 模型，即设置超时
//	struct timeval timeout ;
//	fd_set r;
//	
//	FD_ZERO(&r);
//	FD_SET(m_hSocket, &r);
//	timeout.tv_sec = 20; //连接超时15秒
//	timeout.tv_usec = 0;
//	ret = select(0, 0, &r, 0, &timeout);
//	if ( ret <= 0 )
//	{
//		m_LastError = getSocketError();
//		::closesocket(m_hSocket);
//		return false;
//	}
//	//一般非锁定模式套接比较难控制，可以根据实际情况考虑 再设回阻塞模式
//	
//	ul = 0;
//	ret = ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul);

	ret = connect(m_hSocket, (const sockaddr*)&mysockaddr_in, sizeof(sockaddr_in));
	if(ret < 0)
	{
		m_LastError = getSocketError();
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool CPHSocket::Connect(sockaddr_in* psockaddr_in, int nsockaddr_inLen)
{
	if (psockaddr_in == NULL) return false;

	int nConResult = connect(m_hSocket, (const sockaddr*)psockaddr_in, nsockaddr_inLen);
	if(nConResult < 0)
	{
		m_LastError = getSocketError();
		return false;
	}
	return true;
}

bool CPHSocket::Create(unsigned short nSocketPort, int nSocketType, char * lpszSocketAddress)
{
	if(m_hSocket != INVALID_SOCKET)
	{
		m_LastError = -1;
		return false;
	}

	m_hSocket = socket(PF_INET, nSocketType, 0);
	if(m_hSocket == INVALID_SOCKET)
	{
		m_LastError = getSocketError();
		return false;
	}
	if(Bind(nSocketPort, lpszSocketAddress))
		return true;	//Normal exit
	//Bind error
	Close();
	return false;
}

int CPHSocket::Detach()
{
	return m_hSocket;
}

int CPHSocket::GetLastError()
{
	return m_LastError;
}

bool CPHSocket::Listen(int nConnectionBacklog/*=5*/)
{
	if(listen(m_hSocket, nConnectionBacklog) != 0)	//error occured
	{
		m_LastError = getSocketError();
		return false;
	}
	return true;
}

int CPHSocket::Receive(void* lpBuf, int nBufLen, int nFlag)
{
	int nResult;
	nResult = recv(m_hSocket, (char*)lpBuf, nBufLen, nFlag);
	if(nResult < 0)
		m_LastError = getSocketError();
	return nResult;
}

int CPHSocket::ReceiveFrom(void* lpBuf, int nBufLen, char *rSocketAddress, unsigned short& rSocketPort, int nFlags)
{
	struct sockaddr_in mysockaddr_in;

	memset(&mysockaddr_in, 0, sizeof(sockaddr_in));

	int nsockaddr_inLen = sizeof(sockaddr_in);
	int nResult;

	nResult = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (sockaddr*)&mysockaddr_in, (socklen_t *)&nsockaddr_inLen);
	if(nResult < 0)
	{
		m_LastError = getSocketError();
		return nResult;
	}
	else
	{
		rSocketPort = ntohs(mysockaddr_in.sin_port);
		strcpy(rSocketAddress,inet_ntoa(mysockaddr_in.sin_addr));
	}
	return nResult;
}

int CPHSocket::ReceiveFrom(void* lpBuf, int nBufLen, sockaddr_in* psockaddr_in, int* pnsockaddr_inLen, int nFlags)
{
	memset(psockaddr_in, 0, sizeof(sockaddr_in));

	*pnsockaddr_inLen = sizeof(sockaddr_in);
	int nResult;

	nResult = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (sockaddr*)psockaddr_in,(socklen_t *) pnsockaddr_inLen);
	if(nResult < 0)
		m_LastError = getSocketError();

	return nResult;
}

int CPHSocket::Send(void* lpBuf, int nBufLen, int nFlag)
{
	int nResult;

	nResult = send(m_hSocket, (char*)lpBuf, nBufLen, nFlag);
	if(nResult < 0)
		m_LastError = getSocketError();
	return nResult;
}

int CPHSocket::SendTo(const void* lpBuf, int nBufLen, unsigned short nHostPort, char * lpszHostAddress, int nFlags)
{
	struct sockaddr_in mysockaddr_in;

	memset(&mysockaddr_in, 0, sizeof(sockaddr_in));

	mysockaddr_in.sin_family = AF_INET;

	if (lpszHostAddress == NULL)
		mysockaddr_in.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		mysockaddr_in.sin_addr.s_addr = inet_addr(lpszHostAddress);
		if (mysockaddr_in.sin_addr.s_addr == INADDR_NONE)
		{
			hostent * lphost;
			lphost = gethostbyname(lpszHostAddress);
			if (lphost != NULL)
				mysockaddr_in.sin_addr.s_addr = ((in_addr *)lphost->h_addr)->s_addr;
			else
			{
				m_LastError = -1;
				return -1;
			}
		}
	}

	mysockaddr_in.sin_port = htons((u_short)nHostPort);
	int nsockaddr_inLen = sizeof(sockaddr_in);
	int nResult;
	nResult = sendto(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (const sockaddr*)&mysockaddr_in, nsockaddr_inLen);
	if(nResult < 0)
		m_LastError = getSocketError();
	return nResult;
}

int CPHSocket::SendTo(const void* lpBuf, int nBufLen, sockaddr_in* psockaddr_in, int nsockaddr_inLen, int nFlags)
{
	int nResult;
	nResult = sendto(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (const sockaddr*)psockaddr_in, nsockaddr_inLen);
	if(nResult < 0)
		m_LastError = getSocketError();
	return nResult;
}

int CPHSocket::ReadOneLine(char * lpszBuf, int nBufLen)
{
	int nReadCount = 0;
	int nRecv = 0;
	
	lpszBuf[0] = '\0';
	
	for (; ; )
	{
		if (DataReadable(30) <= 0) return 0;
		nRecv = recv(m_hSocket, lpszBuf + nReadCount, 1, 0);
		//printf("recvoneline:%s,count:%d\n",lpszBuf,nRecv);
		//if (nRecv == 0) continue;
		if (nRecv > 0)
		{
			nReadCount += nRecv;
			
			if (lpszBuf[nReadCount - 1] == '\n')
			{
				if (nReadCount > 1 && lpszBuf[nReadCount - 2] == '\r')
				{
					lpszBuf[nReadCount - 2] = '\0';
				}
				else
				{
					lpszBuf[nReadCount - 1] = '\0';
				}
				break;
			}
			
			if (nReadCount == nBufLen)
			{
				break;
			}
		}
		else
		{
			m_LastError = getSocketError();
			nReadCount = -1;
			break;
		}
	}
	return nReadCount;
}

int CPHSocket::DataReadable(int nTimeout)
{
	fd_set  fdR;  
	int ret;
	struct  timeval timeout;  
	timeout.tv_sec = nTimeout;
	timeout.tv_usec = 0;
	
	FD_ZERO(&fdR);  
	FD_SET(m_hSocket, &fdR);  
	switch (ret = select(m_hSocket+1, &fdR, NULL,NULL, &timeout)) 
	{  
	case -1: 
		return -1;
	break;
	case 0:  
		return 0;
	break; 
	default:  
		return ret;
		break;
	}  
}

int CPHSocket::SendTo(int s, const void* lpBuf, int nBufLen, unsigned short nHostPort, char * lpszHostAddress, int nFlags)
{
	struct sockaddr_in mysockaddr_in;
	
	memset(&mysockaddr_in, 0, sizeof(sockaddr_in));
	
	mysockaddr_in.sin_family = AF_INET;
	
	if (lpszHostAddress == NULL)
		mysockaddr_in.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		mysockaddr_in.sin_addr.s_addr = inet_addr(lpszHostAddress);
		if (mysockaddr_in.sin_addr.s_addr == INADDR_NONE)
		{
			hostent * lphost;
			lphost = gethostbyname(lpszHostAddress);
			if (lphost != NULL)
				mysockaddr_in.sin_addr.s_addr = ((in_addr *)lphost->h_addr)->s_addr;
			else
			{
				return -1;
			}
		}
	}
	
	mysockaddr_in.sin_port = htons((u_short)nHostPort);
	int nsockaddr_inLen = sizeof(sockaddr_in);
	int nResult;
	nResult = sendto(s, (char*)lpBuf, nBufLen, nFlags, (const sockaddr*)&mysockaddr_in, nsockaddr_inLen);
	return nResult;
}

void CPHSocket::SetBlockingMode(bool bBlocking)
{
	unsigned long on = 1;
	if (bBlocking) on = 0;
	if (ioctlsocket(m_hSocket, FIONBIO, &on) < 0) {
		return;
	}
	return;
}