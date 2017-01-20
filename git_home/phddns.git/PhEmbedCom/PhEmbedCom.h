

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue May 11 14:03:48 2010
 */
/* Compiler settings for .\PhEmbedCom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PhEmbedCom_h__
#define __PhEmbedCom_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPhDDNS_FWD_DEFINED__
#define __IPhDDNS_FWD_DEFINED__
typedef interface IPhDDNS IPhDDNS;
#endif 	/* __IPhDDNS_FWD_DEFINED__ */


#ifndef ___IPhDDNSEvents_FWD_DEFINED__
#define ___IPhDDNSEvents_FWD_DEFINED__
typedef interface _IPhDDNSEvents _IPhDDNSEvents;
#endif 	/* ___IPhDDNSEvents_FWD_DEFINED__ */


#ifndef __PhDDNS_FWD_DEFINED__
#define __PhDDNS_FWD_DEFINED__

#ifdef __cplusplus
typedef class PhDDNS PhDDNS;
#else
typedef struct PhDDNS PhDDNS;
#endif /* __cplusplus */

#endif 	/* __PhDDNS_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IPhDDNS_INTERFACE_DEFINED__
#define __IPhDDNS_INTERFACE_DEFINED__

/* interface IPhDDNS */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPhDDNS;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B27BDD74-1AF4-469B-97FD-CD5E871DBBAF")
    IPhDDNS : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ BSTR Host,
            /* [in] */ BSTR Account,
            /* [in] */ BSTR Password,
            /* [in] */ long clientInfo,
            /* [in] */ long embedKey,
            /* [retval][out] */ VARIANT_BOOL *pRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Step( 
            /* [retval][out] */ long *Next) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPhDDNSVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPhDDNS * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPhDDNS * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPhDDNS * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPhDDNS * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPhDDNS * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPhDDNS * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPhDDNS * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IPhDDNS * This,
            /* [in] */ BSTR Host,
            /* [in] */ BSTR Account,
            /* [in] */ BSTR Password,
            /* [in] */ long clientInfo,
            /* [in] */ long embedKey,
            /* [retval][out] */ VARIANT_BOOL *pRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Step )( 
            IPhDDNS * This,
            /* [retval][out] */ long *Next);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IPhDDNS * This);
        
        END_INTERFACE
    } IPhDDNSVtbl;

    interface IPhDDNS
    {
        CONST_VTBL struct IPhDDNSVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPhDDNS_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPhDDNS_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPhDDNS_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPhDDNS_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPhDDNS_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPhDDNS_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPhDDNS_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPhDDNS_Initialize(This,Host,Account,Password,clientInfo,embedKey,pRet)	\
    (This)->lpVtbl -> Initialize(This,Host,Account,Password,clientInfo,embedKey,pRet)

#define IPhDDNS_Step(This,Next)	\
    (This)->lpVtbl -> Step(This,Next)

#define IPhDDNS_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPhDDNS_Initialize_Proxy( 
    IPhDDNS * This,
    /* [in] */ BSTR Host,
    /* [in] */ BSTR Account,
    /* [in] */ BSTR Password,
    /* [in] */ long clientInfo,
    /* [in] */ long embedKey,
    /* [retval][out] */ VARIANT_BOOL *pRet);


void __RPC_STUB IPhDDNS_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPhDDNS_Step_Proxy( 
    IPhDDNS * This,
    /* [retval][out] */ long *Next);


void __RPC_STUB IPhDDNS_Step_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPhDDNS_Stop_Proxy( 
    IPhDDNS * This);


void __RPC_STUB IPhDDNS_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPhDDNS_INTERFACE_DEFINED__ */



#ifndef __PHEMBEDCOMLib_LIBRARY_DEFINED__
#define __PHEMBEDCOMLib_LIBRARY_DEFINED__

/* library PHEMBEDCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PHEMBEDCOMLib;

#ifndef ___IPhDDNSEvents_DISPINTERFACE_DEFINED__
#define ___IPhDDNSEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPhDDNSEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IPhDDNSEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F835598B-7780-4BF7-92FC-F20FDF81CE92")
    _IPhDDNSEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IPhDDNSEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IPhDDNSEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IPhDDNSEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IPhDDNSEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IPhDDNSEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IPhDDNSEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IPhDDNSEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IPhDDNSEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IPhDDNSEventsVtbl;

    interface _IPhDDNSEvents
    {
        CONST_VTBL struct _IPhDDNSEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IPhDDNSEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IPhDDNSEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IPhDDNSEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IPhDDNSEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IPhDDNSEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IPhDDNSEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IPhDDNSEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IPhDDNSEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PhDDNS;

#ifdef __cplusplus

class DECLSPEC_UUID("260F073F-F564-499C-B7C3-0020F0330C7D")
PhDDNS;
#endif
#endif /* __PHEMBEDCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


