

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Tue Dec 02 08:48:56 2014
 */
/* Compiler settings for test_activex.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

MIDL_DEFINE_GUID(IID, LIBID_test_activexLib,0xD73D0B09,0xDAA6,0x4076,0x95,0xA9,0xF0,0x99,0x5B,0xA3,0xBD,0xB1);


MIDL_DEFINE_GUID(IID, DIID__Dtest_activex,0x94A191F0,0xD0D6,0x46ED,0xA8,0x2D,0x7B,0xAA,0x86,0x28,0x07,0x31);


MIDL_DEFINE_GUID(IID, DIID__Dtest_activexEvents,0x3E382D9E,0x3FFF,0x433D,0x87,0x17,0x5A,0x60,0xF8,0xC7,0xBD,0x70);


MIDL_DEFINE_GUID(CLSID, CLSID_test_activex,0x458FE3FF,0x3843,0x49C6,0x80,0x2D,0xAC,0x10,0xD9,0x76,0x96,0x09);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



