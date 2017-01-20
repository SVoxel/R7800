// PhDDNS.h : Declaration of the CPhDDNS

#ifndef __PHDDNS_H_
#define __PHDDNS_H_

#include "resource.h"       // main symbols
#include "..\src\phupdate.h"
#include "PhEmbedComCP.h"

/////////////////////////////////////////////////////////////////////////////
// CPhDDNS
class ATL_NO_VTABLE CPhDDNS : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPhDDNS, &CLSID_PhDDNS>,
	public IConnectionPointContainerImpl<CPhDDNS>,
	public IDispatchImpl<IPhDDNS, &IID_IPhDDNS, &LIBID_PHEMBEDCOMLib>,
	public CProxy_IPhDDNSEvents< CPhDDNS >,
	public CUpdateBase
{
public:
	CPhDDNS()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PHDDNS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPhDDNS)
	COM_INTERFACE_ENTRY(IPhDDNS)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CPhDDNS)
CONNECTION_POINT_ENTRY(DIID__IPhDDNSEvents)
END_CONNECTION_POINT_MAP()


// IPhDDNS
public:
	STDMETHOD(Stop)();
	STDMETHOD(Step)(/*[out, retval]*/ long *Next);
	STDMETHOD(Initialize)(/*[in]*/ BSTR Host, /*[in]*/ BSTR Account, /*[in]*/ BSTR Password, /*[in]*/ long clientInfo, /*[in]*/ long embedKey, /*[out, retval]*/ VARIANT_BOOL *pRet);
protected:
	virtual void OnStatusChanged(int status, long data);
	virtual void OnDomainRegistered(std::string domain);
	virtual void OnUserInfo(std::string userInfo);
	virtual void OnAccountDomainInfo(std::string domainInfo);
private:
};

#endif //__PHDDNS_H_
