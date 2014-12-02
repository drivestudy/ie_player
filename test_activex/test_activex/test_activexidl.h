

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __test_activexidl_h__
#define __test_activexidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Dtest_activex_FWD_DEFINED__
#define ___Dtest_activex_FWD_DEFINED__
typedef interface _Dtest_activex _Dtest_activex;

#endif 	/* ___Dtest_activex_FWD_DEFINED__ */


#ifndef ___Dtest_activexEvents_FWD_DEFINED__
#define ___Dtest_activexEvents_FWD_DEFINED__
typedef interface _Dtest_activexEvents _Dtest_activexEvents;

#endif 	/* ___Dtest_activexEvents_FWD_DEFINED__ */


#ifndef __test_activex_FWD_DEFINED__
#define __test_activex_FWD_DEFINED__

#ifdef __cplusplus
typedef class test_activex test_activex;
#else
typedef struct test_activex test_activex;
#endif /* __cplusplus */

#endif 	/* __test_activex_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_test_activex_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_test_activex_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_test_activex_0000_0000_v0_0_s_ifspec;


#ifndef __test_activexLib_LIBRARY_DEFINED__
#define __test_activexLib_LIBRARY_DEFINED__

/* library test_activexLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_test_activexLib;

#ifndef ___Dtest_activex_DISPINTERFACE_DEFINED__
#define ___Dtest_activex_DISPINTERFACE_DEFINED__

/* dispinterface _Dtest_activex */
/* [uuid] */ 


EXTERN_C const IID DIID__Dtest_activex;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("94A191F0-D0D6-46ED-A82D-7BAA86280731")
    _Dtest_activex : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _Dtest_activexVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dtest_activex * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dtest_activex * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dtest_activex * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dtest_activex * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dtest_activex * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dtest_activex * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dtest_activex * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _Dtest_activexVtbl;

    interface _Dtest_activex
    {
        CONST_VTBL struct _Dtest_activexVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dtest_activex_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dtest_activex_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dtest_activex_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dtest_activex_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dtest_activex_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dtest_activex_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dtest_activex_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dtest_activex_DISPINTERFACE_DEFINED__ */


#ifndef ___Dtest_activexEvents_DISPINTERFACE_DEFINED__
#define ___Dtest_activexEvents_DISPINTERFACE_DEFINED__

/* dispinterface _Dtest_activexEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__Dtest_activexEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3E382D9E-3FFF-433D-8717-5A60F8C7BD70")
    _Dtest_activexEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _Dtest_activexEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dtest_activexEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dtest_activexEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dtest_activexEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dtest_activexEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dtest_activexEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dtest_activexEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dtest_activexEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _Dtest_activexEventsVtbl;

    interface _Dtest_activexEvents
    {
        CONST_VTBL struct _Dtest_activexEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dtest_activexEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dtest_activexEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dtest_activexEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dtest_activexEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dtest_activexEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dtest_activexEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dtest_activexEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dtest_activexEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_test_activex;

#ifdef __cplusplus

class DECLSPEC_UUID("458FE3FF-3843-49C6-802D-AC10D9769609")
test_activex;
#endif
#endif /* __test_activexLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


