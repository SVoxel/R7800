

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPhDDNS,0xB27BDD74,0x1AF4,0x469B,0x97,0xFD,0xCD,0x5E,0x87,0x1D,0xBB,0xAF);


MIDL_DEFINE_GUID(IID, LIBID_PHEMBEDCOMLib,0x5CE20F14,0xF268,0x4198,0x89,0x9F,0x4D,0x1A,0xFA,0x14,0x06,0x86);


MIDL_DEFINE_GUID(IID, DIID__IPhDDNSEvents,0xF835598B,0x7780,0x4BF7,0x92,0xFC,0xF2,0x0F,0xDF,0x81,0xCE,0x92);


MIDL_DEFINE_GUID(CLSID, CLSID_PhDDNS,0x260F073F,0xF564,0x499C,0xB7,0xC3,0x00,0x20,0xF0,0x33,0x0C,0x7D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



