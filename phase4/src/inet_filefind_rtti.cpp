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
#include <cstdlib>
#include <new>
#include <string>
#include <unordered_map>

#ifdef GetObject
#undef GetObject
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct GopherFindState {
    GOPHER_FIND_DATAW data;
    CString locator;
};

std::unordered_map<const void*, GopherFindState>& GopherStates() {
    static std::unordered_map<const void*, GopherFindState> states;
    return states;
}

bool HasFileTime(const FILETIME& ft) {
    return ft.dwLowDateTime != 0 || ft.dwHighDateTime != 0;
}

long long FileTimeToCTimeValue(const FILETIME& ft) {
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return static_cast<long long>(u.QuadPart / 10000000ULL) - 11644473600LL;
}

const wchar_t* LocatorString(const CGopherLocator* pLocator) {
    if (!pLocator || !pLocator->m_lpBuffer || pLocator->m_dwBufferLength == 0) {
        return L"";
    }
    return reinterpret_cast<const wchar_t*>(pLocator->m_lpBuffer);
}

CString FinderName(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end() && it->second.data.DisplayString[0]) {
        return CString(it->second.data.DisplayString);
    }
    if (!pThis->m_strFileName.IsEmpty()) return pThis->m_strFileName;
    return CString(pThis->m_findFileData.cFileName);
}

CString FinderLocator(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) {
        if (it->second.data.Locator[0]) return CString(it->second.data.Locator);
        return it->second.locator;
    }
    return pThis->m_strRoot;
}

FILETIME FinderWriteTime(const CFtpFileFind* pThis) {
    FILETIME ft = {};
    if (!pThis) return ft;
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) return it->second.data.LastModificationTime;
    return pThis->m_findFileData.ftLastWriteTime;
}

ULONGLONG FinderLength(const CFtpFileFind* pThis) {
    if (!pThis) return 0;
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) {
        return (static_cast<ULONGLONG>(it->second.data.SizeHigh) << 32) | it->second.data.SizeLow;
    }
    return (static_cast<ULONGLONG>(pThis->m_findFileData.nFileSizeHigh) << 32) |
           pThis->m_findFileData.nFileSizeLow;
}

CString FinderUrl(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    CString locator = FinderLocator(pThis);
    CInternetConnection* pConnection =
        reinterpret_cast<CInternetConnection*>(pThis->m_pFtpConnection);
    if (pConnection && !pConnection->GetServerName().IsEmpty()) {
        return CString(L"gopher://") + pConnection->GetServerName() + L"/" + locator;
    }
    return locator;
}

void FillLocator(CGopherLocator* pRet, const CString& locator) {
    new (pRet) CGopherLocator();
    const wchar_t* psz = locator.GetString();
    size_t chars = psz ? wcslen(psz) : 0;
    pRet->m_dwBufferLength = static_cast<DWORD>((chars + 1) * sizeof(wchar_t));
    pRet->m_lpBuffer = std::malloc(pRet->m_dwBufferLength);
    if (pRet->m_lpBuffer) {
        std::memcpy(pRet->m_lpBuffer, psz ? psz : L"", pRet->m_dwBufferLength);
    } else {
        pRet->m_dwBufferLength = 0;
    }
}

void CloseGopherFinder(CFtpFileFind* pThis) {
    if (!pThis) return;
    if (pThis->m_hFindHandle) {
        ::InternetCloseHandle(pThis->m_hFindHandle);
        pThis->m_hFindHandle = nullptr;
    }
    GopherStates().erase(pThis);
}

int FindGopher(CFtpFileFind* pThis, const wchar_t* locator,
               const wchar_t* search, DWORD flags) {
    if (!pThis) return 0;
    CloseGopherFinder(pThis);
    CInternetConnection* pConnection =
        reinterpret_cast<CInternetConnection*>(pThis->m_pFtpConnection);
    if (!pConnection || !pConnection->m_hConnection) return 0;

    GopherFindState state;
    std::memset(&state.data, 0, sizeof(state.data));
    state.locator = locator ? locator : L"";
    HINTERNET hFind = ::GopherFindFirstFileW(pConnection->m_hConnection,
                                             locator,
                                             search,
                                             &state.data,
                                             flags,
                                             pThis->m_dwContext);
    if (!hFind) return 0;

    pThis->m_hFindHandle = hFind;
    std::memset(&pThis->m_findFileData, 0, sizeof(pThis->m_findFileData));
    wcsncpy(pThis->m_findFileData.cFileName, state.data.DisplayString,
            MAX_PATH - 1);
    pThis->m_findFileData.nFileSizeLow = state.data.SizeLow;
    pThis->m_findFileData.nFileSizeHigh = state.data.SizeHigh;
    pThis->m_findFileData.ftLastWriteTime = state.data.LastModificationTime;
    pThis->m_strFileName = state.data.DisplayString;
    GopherStates()[pThis] = state;
    return 1;
}

} // namespace

// Symbol: ?GetThisClass@CFileFind@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileFind__SAPEAUCRuntimeClass__XZ() {
    return CFileFind::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CFileFind@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileFind__UEBAPEAUCRuntimeClass__XZ(
    const CFileFind* pThis) {
    if (!pThis) return CFileFind::GetThisClass();
    return CFileFind::GetThisClass();
}

// Symbol: ?FindNextFileW@CFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFileFind__UEAAHXZ(CFileFind* pThis) {
    if (!pThis) return 0;
    return pThis->FindNextFile();
}

// Symbol: ?CloseContext@CFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CFileFind__MEAAXXZ(CFileFind* pThis) {
    if (pThis) pThis->Close();
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
    return CFtpFileFind::GetThisClass();
}

// Symbol: ?FindNextFileW@CFtpFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CFtpFileFind__UEAAHXZ(CFtpFileFind* pThis) {
    if (!pThis) return 0;
    return pThis->FindNextFile();
}

// Symbol: ?CloseContext@CFtpFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CFtpFileFind__MEAAXXZ(CFtpFileFind* pThis) {
    if (pThis) pThis->Close();
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
    return CGopherConnection::GetThisClass();
}

// Symbol: ?Write@CGopherFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CGopherFile__UEAAXPEBXI_Z(
    CGopherFile* pThis, const void* lpBuf, unsigned int nCount) {
    if (pThis) pThis->CInternetFile::Write(lpBuf, nCount);
}

// Symbol: ?WriteString@CGopherFile@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__WriteString_CGopherFile__UEAAXPEB_W_Z(
    CGopherFile* pThis, const wchar_t* pstr) {
    if (pThis) pThis->CInternetFile::WriteString(pstr);
}

// Symbol: ?CloseContext@CGopherFileFind@@MEAAXXZ
extern "C" void MS_ABI impl__CloseContext_CGopherFileFind__MEAAXXZ(CFtpFileFind* pThis) {
    CloseGopherFinder(pThis);
    if (pThis) pThis->CFileFind::Close();
}

// Symbol: ?FindFile@CGopherFileFind@@UEAAHAEAVCGopherLocator@@PEB_WK@Z
extern "C" int MS_ABI impl__FindFile_CGopherFileFind__UEAAHAEAVCGopherLocator__PEB_WK_Z(
    CFtpFileFind* pThis, CGopherLocator* pLocator, const wchar_t* pstrString, unsigned long dwFlags) {
    return FindGopher(pThis, LocatorString(pLocator), pstrString, dwFlags);
}

// Symbol: ?FindFile@CGopherFileFind@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__FindFile_CGopherFileFind__UEAAHPEB_WK_Z(
    CFtpFileFind* pThis, const wchar_t* pstrString, unsigned long dwFlags) {
    return FindGopher(pThis, nullptr, pstrString, dwFlags);
}

// Symbol: ?FindNextFileW@CGopherFileFind@@UEAAHXZ
extern "C" int MS_ABI impl__FindNextFileW_CGopherFileFind__UEAAHXZ(CFtpFileFind* pThis) {
    if (!pThis || !pThis->m_hFindHandle) return 0;
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) {
        if (!::InternetFindNextFileW(pThis->m_hFindHandle, &it->second.data)) return 0;
        std::memset(&pThis->m_findFileData, 0, sizeof(pThis->m_findFileData));
        wcsncpy(pThis->m_findFileData.cFileName, it->second.data.DisplayString, MAX_PATH - 1);
        pThis->m_findFileData.nFileSizeLow = it->second.data.SizeLow;
        pThis->m_findFileData.nFileSizeHigh = it->second.data.SizeHigh;
        pThis->m_findFileData.ftLastWriteTime = it->second.data.LastModificationTime;
        pThis->m_strFileName = it->second.data.DisplayString;
        return 1;
    }
    return pThis->CFtpFileFind::FindNextFile();
}

// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetCreationTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(
    const CFtpFileFind* pThis, long long* pRefTime) {
    FILETIME ft = FinderWriteTime(pThis);
    if (!pRefTime || !HasFileTime(ft)) return 0;
    *pRefTime = FileTimeToCTimeValue(ft);
    return 1;
}

// Symbol: ?GetCreationTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetCreationTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(
    const CFtpFileFind* pThis, FILETIME* pFileTime) {
    FILETIME ft = FinderWriteTime(pThis);
    if (!pFileTime || !HasFileTime(ft)) return 0;
    *pFileTime = ft;
    return 1;
}

// Symbol: ?GetFileName@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileName_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    new (pRet) CString(FinderName(pThis));
}

// Symbol: ?GetFilePath@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFilePath_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    CString locator = FinderLocator(pThis);
    new (pRet) CString(locator.IsEmpty() ? FinderName(pThis) : locator);
}

// Symbol: ?GetFileTitle@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileTitle_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    CString s = FinderName(pThis);
    int pos = s.ReverseFind(L'.');
    new (pRet) CString(pos > 0 ? s.Left(pos) : s);
}

// Symbol: ?GetFileURL@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    new (pRet) CString(FinderUrl(pThis));
}

// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastAccessTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(
    const CFtpFileFind* pThis, long long* pRefTime) {
    return impl__GetCreationTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(pThis, pRefTime);
}

// Symbol: ?GetLastAccessTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetLastAccessTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(
    const CFtpFileFind* pThis, FILETIME* pFileTime) {
    return impl__GetCreationTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(pThis, pFileTime);
}

// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetLastWriteTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(
    const CFtpFileFind* pThis, long long* pRefTime) {
    return impl__GetCreationTime_CGopherFileFind__UEBAHAEAVCTime_ATL___Z(pThis, pRefTime);
}

// Symbol: ?GetLastWriteTime@CGopherFileFind@@UEBAHPEAU_FILETIME@@@Z
extern "C" int MS_ABI impl__GetLastWriteTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(
    const CFtpFileFind* pThis, FILETIME* pFileTime) {
    return impl__GetCreationTime_CGopherFileFind__UEBAHPEAU_FILETIME___Z(pThis, pFileTime);
}

// Symbol: ?GetLength@CGopherFileFind@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CGopherFileFind__UEBA_KXZ(
    const CFtpFileFind* pThis) {
    return FinderLength(pThis);
}

// Symbol: ?GetLocator@CGopherFileFind@@QEBA?AVCGopherLocator@@XZ
extern "C" void MS_ABI impl__GetLocator_CGopherFileFind__QEBA_AVCGopherLocator__XZ(
    CGopherLocator* pRet, const CFtpFileFind* pThis) {
    FillLocator(pRet, FinderLocator(pThis));
}

// Symbol: ?GetRoot@CGopherFileFind@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetRoot_CGopherFileFind__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    new (pRet) CString(FinderLocator(pThis));
}

// Symbol: ?GetScreenName@CGopherFileFind@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetScreenName_CGopherFileFind__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CFtpFileFind* pThis) {
    new (pRet) CString(FinderName(pThis));
}

// Symbol: ?IsDots@CGopherFileFind@@UEBAHXZ
extern "C" int MS_ABI impl__IsDots_CGopherFileFind__UEBAHXZ(const CFtpFileFind* pThis) {
    CString name = FinderName(pThis);
    return wcscmp(name.GetString(), L".") == 0 || wcscmp(name.GetString(), L"..") == 0;
}
