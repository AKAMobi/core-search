

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed May 19 22:48:09 2004
 */
/* Compiler settings for _wASPExt.idl:
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

#ifndef ___wASPExt_h__
#define ___wASPExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IATLSearch_FWD_DEFINED__
#define __IATLSearch_FWD_DEFINED__
typedef interface IATLSearch IATLSearch;
#endif 	/* __IATLSearch_FWD_DEFINED__ */


#ifndef __CATLSearch_FWD_DEFINED__
#define __CATLSearch_FWD_DEFINED__

#ifdef __cplusplus
typedef class CATLSearch CATLSearch;
#else
typedef struct CATLSearch CATLSearch;
#endif /* __cplusplus */

#endif 	/* __CATLSearch_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IATLSearch_INTERFACE_DEFINED__
#define __IATLSearch_INTERFACE_DEFINED__

/* interface IATLSearch */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IATLSearch;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A5003C20-C0B8-437E-A1A1-13BD7716E168")
    IATLSearch : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_init_query( 
            /* [in] */ LONG bk,
            /* [in] */ BSTR dd,
            /* [in] */ LONG bigmem,
            /* [retval][out] */ LONG *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_done_query( 
            /* [retval][out] */ LONG *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_highlight( 
            /* [in] */ BSTR fn,
            /* [in] */ BSTR qn,
            /* [in] */ BSTR preTag,
            /* [in] */ BSTR postTag,
            /* [retval][out] */ BSTR *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_fetch_total( 
            /* [retval][out] */ LONG *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_do_query( 
            /* [in] */ LONG i,
            /* [in] */ LONG j,
            /* [in] */ BSTR q,
            /* [retval][out] */ VARIANT *ret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE asp_highlight_content( 
            /* [in] */ BSTR content,
            /* [in] */ LONG len,
            /* [in] */ BSTR qn,
            /* [in] */ BSTR preTag,
            /* [in] */ BSTR postTag,
            /* [retval][out] */ BSTR *ret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IATLSearchVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IATLSearch * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IATLSearch * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IATLSearch * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IATLSearch * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IATLSearch * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IATLSearch * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IATLSearch * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_init_query )( 
            IATLSearch * This,
            /* [in] */ LONG bk,
            /* [in] */ BSTR dd,
            /* [in] */ LONG bigmem,
            /* [retval][out] */ LONG *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_done_query )( 
            IATLSearch * This,
            /* [retval][out] */ LONG *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_highlight )( 
            IATLSearch * This,
            /* [in] */ BSTR fn,
            /* [in] */ BSTR qn,
            /* [in] */ BSTR preTag,
            /* [in] */ BSTR postTag,
            /* [retval][out] */ BSTR *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_fetch_total )( 
            IATLSearch * This,
            /* [retval][out] */ LONG *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_do_query )( 
            IATLSearch * This,
            /* [in] */ LONG i,
            /* [in] */ LONG j,
            /* [in] */ BSTR q,
            /* [retval][out] */ VARIANT *ret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asp_highlight_content )( 
            IATLSearch * This,
            /* [in] */ BSTR content,
            /* [in] */ LONG len,
            /* [in] */ BSTR qn,
            /* [in] */ BSTR preTag,
            /* [in] */ BSTR postTag,
            /* [retval][out] */ BSTR *ret);
        
        END_INTERFACE
    } IATLSearchVtbl;

    interface IATLSearch
    {
        CONST_VTBL struct IATLSearchVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IATLSearch_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IATLSearch_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IATLSearch_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IATLSearch_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IATLSearch_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IATLSearch_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IATLSearch_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IATLSearch_asp_init_query(This,bk,dd,bigmem,ret)	\
    (This)->lpVtbl -> asp_init_query(This,bk,dd,bigmem,ret)

#define IATLSearch_asp_done_query(This,ret)	\
    (This)->lpVtbl -> asp_done_query(This,ret)

#define IATLSearch_asp_highlight(This,fn,qn,preTag,postTag,ret)	\
    (This)->lpVtbl -> asp_highlight(This,fn,qn,preTag,postTag,ret)

#define IATLSearch_asp_fetch_total(This,ret)	\
    (This)->lpVtbl -> asp_fetch_total(This,ret)

#define IATLSearch_asp_do_query(This,i,j,q,ret)	\
    (This)->lpVtbl -> asp_do_query(This,i,j,q,ret)

#define IATLSearch_asp_highlight_content(This,content,len,qn,preTag,postTag,ret)	\
    (This)->lpVtbl -> asp_highlight_content(This,content,len,qn,preTag,postTag,ret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_init_query_Proxy( 
    IATLSearch * This,
    /* [in] */ LONG bk,
    /* [in] */ BSTR dd,
    /* [in] */ LONG bigmem,
    /* [retval][out] */ LONG *ret);


void __RPC_STUB IATLSearch_asp_init_query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_done_query_Proxy( 
    IATLSearch * This,
    /* [retval][out] */ LONG *ret);


void __RPC_STUB IATLSearch_asp_done_query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_highlight_Proxy( 
    IATLSearch * This,
    /* [in] */ BSTR fn,
    /* [in] */ BSTR qn,
    /* [in] */ BSTR preTag,
    /* [in] */ BSTR postTag,
    /* [retval][out] */ BSTR *ret);


void __RPC_STUB IATLSearch_asp_highlight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_fetch_total_Proxy( 
    IATLSearch * This,
    /* [retval][out] */ LONG *ret);


void __RPC_STUB IATLSearch_asp_fetch_total_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_do_query_Proxy( 
    IATLSearch * This,
    /* [in] */ LONG i,
    /* [in] */ LONG j,
    /* [in] */ BSTR q,
    /* [retval][out] */ VARIANT *ret);


void __RPC_STUB IATLSearch_asp_do_query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IATLSearch_asp_highlight_content_Proxy( 
    IATLSearch * This,
    /* [in] */ BSTR content,
    /* [in] */ LONG len,
    /* [in] */ BSTR qn,
    /* [in] */ BSTR preTag,
    /* [in] */ BSTR postTag,
    /* [retval][out] */ BSTR *ret);


void __RPC_STUB IATLSearch_asp_highlight_content_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IATLSearch_INTERFACE_DEFINED__ */



#ifndef __wASPExt_LIBRARY_DEFINED__
#define __wASPExt_LIBRARY_DEFINED__

/* library wASPExt */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_wASPExt;

EXTERN_C const CLSID CLSID_CATLSearch;

#ifdef __cplusplus

class DECLSPEC_UUID("8FF7A0F2-DA87-4231-972F-B7776E5A7E58")
CATLSearch;
#endif
#endif /* __wASPExt_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


