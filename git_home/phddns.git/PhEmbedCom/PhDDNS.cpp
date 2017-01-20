// PhDDNS.cpp : Implementation of CPhDDNS
#include "stdafx.h"
#include "PhEmbedCom.h"
#include "PhDDNS.h"
#include "..\src\common\log.h"

/////////////////////////////////////////////////////////////////////////////
// CPhDDNS

void CPhDDNS::OnStatusChanged(int status, long data)
{
	std::string status_string = convert_status_code(status);
	char szData[MAX_PATH];
	switch(status)
	{
	case okKeepAliveRecved:
		strcpy(szData, my_inet_ntoa(data));
		break;
	case okDomainsRegistered:
		sprintf(szData, "%d", data);
		break;
	case okRedirecting:
		sprintf(szData, "%s", phglobal.szHost);
	    break;
	default:
		strcpy(szData, "");
	    break;
	}

	Fire_StatusChanged(CComBSTR(status_string.c_str()), CComBSTR(szData));
}

void CPhDDNS::OnDomainRegistered(std::string domain)
{
	//printf("OnDomainRegistered %s\n", domain.c_str());
	Fire_DomainRegistered(CComBSTR(domain.c_str()));
}

void CPhDDNS::OnUserInfo(std::string userInfo)
{
	//printf("OnUserInfo %s\n", userInfo.c_str());
	Fire_UserInfoCome(CComBSTR(userInfo.c_str()));
}

void CPhDDNS::OnAccountDomainInfo(std::string domainInfo)
{
	//printf("OnAccountDomainInfo %s\n", domainInfo.c_str());
	Fire_DomainInfoCome(CComBSTR(domainInfo.c_str()));
}

STDMETHODIMP CPhDDNS::Initialize(BSTR Host, BSTR Account, BSTR Password, long clientInfo, long embedKey, VARIANT_BOOL *pRet)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	Log::open("", 1);	//empty file will cause we printf to stdout
	if (wcscmp(Host, L"") != 0)	strcpy(phglobal.szHost, W2CA(Host));

	strcpy(phglobal.szUserID, W2CA(Account));
	strcpy(phglobal.szUserPWD, W2CA(Password));
	phglobal.clientinfo = clientInfo;
	phglobal.challengekey = embedKey;

	WORD VersionRequested;		// passed to WSAStartup
	WSADATA  WsaData;			// receives data from WSAStartup
	int error;
	
	VersionRequested = MAKEWORD(2, 0);
	
	//start Winsock 2
	error = WSAStartup(VersionRequested, &WsaData); 

	*pRet = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CPhDDNS::Step(long *Next)
{
	// TODO: Add your implementation code here
	*Next = step();
	return S_OK;
}

STDMETHODIMP CPhDDNS::Stop()
{
	// TODO: Add your implementation code here
	stop();
	Log::closeLog();
	return S_OK;
}
