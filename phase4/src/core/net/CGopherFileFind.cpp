// CGopherFileFind — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ?CloseContext@CGopherFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CGopherFileFind__MEAAXXZ(GopherFileFind* pThis) {
    if (pThis) pThis->Close();
}
// Symbol: ?FindFile@CGopherFileFind@@UEAAHAEAVCGopherLocator@@PEB_WK@Z
extern "C" int MS_ABI impl__FindFile_CGopherFileFind__UEAAHAEAVCGopherLocator__PEB_WK_Z(
    GopherFileFind* pThis, CGopherLocator* pLocator, const wchar_t* pstrName, unsigned long dwFlags) {
    (void)pLocator;
    return pThis ? pThis->FindFile(pstrName ? pstrName : L"*", dwFlags) : FALSE;
}
// Symbol: ?FindFile@CGopherFileFind@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__FindFile_CGopherFileFind__UEAAHPEB_WK_Z(
    GopherFileFind* pThis, const wchar_t* pstrName, unsigned long dwFlags) {
    return pThis ? pThis->FindFile(pstrName ? pstrName : L"*", dwFlags) : FALSE;
}
// Symbol: ?FindNextFileW@CGopherFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CGopherFileFind__UEAAHXZ(GopherFileFind* pThis) {
    return pThis ? pThis->FindNextFile() : FALSE;
}
// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetCreationTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(const GopherFileFind* pThis, void* pTime) {
    FILETIME ft = {};
    return pThis && pThis->GetCreationTime(&ft) ? openmfcFileTimeToCTimeStorage(ft, pTime) : FALSE;
}
// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetCreationTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(const GopherFileFind* pThis, FILETIME* pFileTime) {
    return pThis ? pThis->GetCreationTime(pFileTime) : FALSE;
}
// Symbol: ?GetFileName@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileName() : CString());
}
// Symbol: ?GetFilePath@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFilePath() : CString());
}
// Symbol: ?GetFileTitle@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetFileTitle() : CString());
}
// Symbol: ?GetFileURL@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    CString url;
    if (pThis) {
        url = pThis->GetFileURL();
        if (url.Left(6) == L"ftp://") {
            url = L"gopher://" + url.Mid(6);
        }
    }
    openmfcConstructString(pRet, url);
}
// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastAccessTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(const GopherFileFind* pThis, void* pTime) {
    FILETIME ft = {};
    return pThis && pThis->GetLastAccessTime(&ft) ? openmfcFileTimeToCTimeStorage(ft, pTime) : FALSE;
}
// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetLastAccessTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(const GopherFileFind* pThis, FILETIME* pFileTime) {
    return pThis ? pThis->GetLastAccessTime(pFileTime) : FALSE;
}
// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastWriteTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(const GopherFileFind* pThis, void* pTime) {
    FILETIME ft = {};
    return pThis && pThis->GetLastWriteTime(&ft) ? openmfcFileTimeToCTimeStorage(ft, pTime) : FALSE;
}
// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetLastWriteTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(const GopherFileFind* pThis, FILETIME* pFileTime) {
    return pThis ? pThis->GetLastWriteTime(pFileTime) : FALSE;
}
// Symbol: ?GetLength@CGopherFileFind@@UEBA_KXZ
extern "C" unsigned __int64 MS_ABI impl__GetLength_CGopherFileFind__UEBA_KXZ(const GopherFileFind* pThis) {
    return pThis ? pThis->GetLength() : 0;
}
// Symbol: ?GetLocator@CGopherFileFind@@QEBA?AVCGopherLocator@@XZ
extern "C" void MS_ABI impl__GetLocator_CGopherFileFind__QEBA_AVCGopherLocator__XZ(CGopherLocator* pRet, const GopherFileFind* pThis) {
    (void)pThis;
    if (pRet) new (pRet) CGopherLocator();
}
// Symbol: ?GetRoot@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetRoot_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetRoot() : CString());
}
// GetRuntimeClass/GetThisClass for CGopherFileFind live in core/net/RuntimeClasses.cpp
// (classCGopherFileFind: name "CGopherFileFind", size 72, schema 0xFFFF, base CFileFind),
// which matches the retail descriptor at 0x180329f30 in mfc140u.dll (0x180327d70 in
// mfc140.dll, reached by ?GetThisClass@CGopherFileFind@@SAPEAUCRuntimeClass@@XZ). The
// copies that used to be here returned GopherFileFind::classGopherFileFind, built by
// IMPLEMENT_DYNAMIC(GopherFileFind, CFtpFileFind) in detail/InetcoreSupport.cpp: the
// class-name string is "GopherFileFind" and the base link is CFtpFileFind, neither of
// which matches retail. GetRuntimeClass also self-dispatched through
// pThis->GetRuntimeClass(), which recurses through this very export in a drop-in client.
// Symbol: ?GetScreenName@CGopherFileFind@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetScreenName_CGopherFileFind__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const GopherFileFind* pThis) {
    openmfcConstructString(pRet, pThis ? pThis->GetScreenName() : CString());
}
// Symbol: ?IsDots@CGopherFileFind@@UEBAHXZ
extern "C" int MS_ABI impl__IsDots_CGopherFileFind__UEBAHXZ(const GopherFileFind* pThis) {
    return pThis ? pThis->IsDots() : FALSE;
}
