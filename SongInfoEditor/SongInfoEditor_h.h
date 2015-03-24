

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Jul 05 16:47:43 2014
 */
/* Compiler settings for SongInfoEditor.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __SongInfoEditor_h_h__
#define __SongInfoEditor_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISongInfoEditor_FWD_DEFINED__
#define __ISongInfoEditor_FWD_DEFINED__
typedef interface ISongInfoEditor ISongInfoEditor;
#endif 	/* __ISongInfoEditor_FWD_DEFINED__ */


#ifndef __SongInfoEditor_FWD_DEFINED__
#define __SongInfoEditor_FWD_DEFINED__

#ifdef __cplusplus
typedef class SongInfoEditor SongInfoEditor;
#else
typedef struct SongInfoEditor SongInfoEditor;
#endif /* __cplusplus */

#endif 	/* __SongInfoEditor_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SongInfoEditor_LIBRARY_DEFINED__
#define __SongInfoEditor_LIBRARY_DEFINED__

/* library SongInfoEditor */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SongInfoEditor;

#ifndef __ISongInfoEditor_DISPINTERFACE_DEFINED__
#define __ISongInfoEditor_DISPINTERFACE_DEFINED__

/* dispinterface ISongInfoEditor */
/* [uuid] */ 


EXTERN_C const IID DIID_ISongInfoEditor;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("325B416B-17F1-4E89-9743-F73242C8A2AA")
    ISongInfoEditor : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISongInfoEditorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISongInfoEditor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISongInfoEditor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISongInfoEditor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISongInfoEditor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISongInfoEditor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISongInfoEditor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISongInfoEditor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISongInfoEditorVtbl;

    interface ISongInfoEditor
    {
        CONST_VTBL struct ISongInfoEditorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISongInfoEditor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISongInfoEditor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISongInfoEditor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISongInfoEditor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISongInfoEditor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISongInfoEditor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISongInfoEditor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISongInfoEditor_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SongInfoEditor;

#ifdef __cplusplus

class DECLSPEC_UUID("3B6D88D7-0D0E-4066-9579-DFF9972D3EDC")
SongInfoEditor;
#endif
#endif /* __SongInfoEditor_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


