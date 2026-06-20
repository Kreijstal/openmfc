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

//=============================================================================
// CFileFind  (class fully defined + IMPLEMENT_DYNAMIC in inetcore.cpp)
//=============================================================================

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

// Symbol: ?CloseContext@CFileFind@@MEAAXXZ
// Protected virtual: no-op for the base file finder (no internet context to close).
extern "C" void MS_ABI impl__CloseContext_CFileFind__MEAAXXZ(CFileFind* pThis) {
    (void)pThis;
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

//=============================================================================
// CFtpFileFind  (class fully defined + IMPLEMENT_DYNAMIC in inetcore.cpp)
//=============================================================================

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

// Symbol: ?CloseContext@CFtpFileFind@@MEAAXXZ
// Protected virtual: no-op (handle teardown happens in Close()).
extern "C" void MS_ABI impl__CloseContext_CFtpFileFind__MEAAXXZ(CFtpFileFind* pThis) {
    (void)pThis;
}

// Symbol: ?GetFileURL@CFtpFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CFtpFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    if (pThis) new (pRet) CString(pThis->GetFileURL());
    else       new (pRet) CString();
}

//=============================================================================
// CGopherConnection  (class defined + IMPLEMENT_DYNAMIC in inetcore.cpp)
//=============================================================================

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

//=============================================================================
// CGopherFile  (CInternetFile-derived; no DECLARE_DYNAMIC, so provide RTTI here)
//=============================================================================

__attribute__((used)) static CRuntimeClass g_classCGopherFile = {
    "CGopherFile", (int)sizeof(CGopherFile), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};

// Symbol: ?GetThisClass@CGopherFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGopherFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCGopherFile;
}

// Symbol: ?GetRuntimeClass@CGopherFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGopherFile__UEBAPEAUCRuntimeClass__XZ(
    const CGopherFile* pThis) {
    (void)pThis;
    return &g_classCGopherFile;
}

// Symbol: ?Write@CGopherFile@@UEAAXPEBXI@Z
// Gopher files are read-only; Write is a documented no-op override.
extern "C" void MS_ABI impl__Write_CGopherFile__UEAAXPEBXI_Z(
    CGopherFile* pThis, const void* lpBuf, unsigned int nCount) {
    (void)pThis; (void)lpBuf; (void)nCount;
}

// Symbol: ?WriteString@CGopherFile@@UEAAXPEB_W@Z
// Gopher files are read-only; WriteString is a documented no-op override.
extern "C" void MS_ABI impl__WriteString_CGopherFile__UEAAXPEB_W_Z(
    CGopherFile* pThis, const wchar_t* pstr) {
    (void)pThis; (void)pstr;
}
