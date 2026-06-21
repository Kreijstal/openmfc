// OpenMFC shard: internet_wininet 01
// Real implementations for selected CFileFind / CFtpFileFind / CGopherConnection /
// CGopherFile exports. Each function overrides the corresponding weak stub.
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxstr.h"
#include <windows.h>
#include <wininet.h>
#include <cstring>
#include <new>

#ifdef GetObject
#undef GetObject
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@CFileFind@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileFind__SAPEAUCRuntimeClass__XZ() {
    return CFileFind::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CFileFind@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileFind__UEBAPEAUCRuntimeClass__XZ(
    const CFileFind* pThis) {
    if (!pThis) return CFileFind::GetThisClass();
    return pThis->GetRuntimeClass();
}

// Symbol: ?FindNextFileW@CFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFileFind__UEAAHXZ(CFileFind* pThis) {
    if (!pThis) return 0;
    return pThis->FindNextFile();
}

// Symbol: ?GetFileName@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFileName());
    else       new (pRet) CString();
}

// Symbol: ?GetFilePath@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFilePath());
    else       new (pRet) CString();
}

// Symbol: ?GetFileTitle@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFileTitle());
    else       new (pRet) CString();
}

// Symbol: ?GetFileURL@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFileURL());
    else       new (pRet) CString();
}

// Symbol: ?GetRoot@CFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetRoot_CFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetRoot());
    else       new (pRet) CString();
}

// Symbol: ?GetThisClass@CFtpFileFind@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFtpFileFind__SAPEAUCRuntimeClass__XZ() {
    return CFtpFileFind::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CFtpFileFind@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFtpFileFind__UEBAPEAUCRuntimeClass__XZ(
    const CFtpFileFind* pThis) {
    if (!pThis) return CFtpFileFind::GetThisClass();
    return pThis->GetRuntimeClass();
}

// Symbol: ?FindNextFileW@CFtpFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFtpFileFind__UEAAHXZ(CFtpFileFind* pThis) {
    if (!pThis) return 0;
    return pThis->FindNextFile();
}

// Symbol: ?GetFileURL@CFtpFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CFtpFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFileURL());
    else       new (pRet) CString();
}

// Symbol: ?GetThisClass@CGopherConnection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGopherConnection__SAPEAUCRuntimeClass__XZ() {
    return CGopherConnection::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CGopherConnection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGopherConnection__UEBAPEAUCRuntimeClass__XZ(
    const CGopherConnection* pThis) {
    if (!pThis) return CGopherConnection::GetThisClass();
    return pThis->GetRuntimeClass();
}
