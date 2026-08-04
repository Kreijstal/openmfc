// Globals — OpenMFC implementation.
// Sources: collections_cwordarray.cpp, filecore.cpp, global_cstring_core.cpp, global_string_compare_afx.cpp, strcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWordArraySupport.h"
#include "detail/FilecoreSupport.h"
#include "detail/StrcoreSupport.h"

// OpenMFC: core exported CStringT<wchar_t, StrTraitMFC_DLL> / CSimpleStringT methods.
//
// A drop-in MSVC client does NOT inline these — MFC exports the explicit template
// instantiations and the client imports them BY ORDINAL (e.g. CString default ctor =
// ord 296, operator=(PCXSTR)=1503, GetLength=5674, operator PCXSTR=1641). They were
// weak stubs, so a client `CString d;` ran a no-op ctor -> garbage m_pszData -> reads
// empty / NULL. OpenMFC's CString is now ATL-binary-compatible (same 8-byte object,
// m_pszData@0, ATL CStringData layout — see afxstr.h), so these thunks just forward
// to it: the client's CStringT object IS an OpenMFC CString.

#include "openmfc/afxstr.h"
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif









#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleauto.h>

#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Global Win32 forwards
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// CStringT<wchar_t> comparison helpers
//
// The ABI 'this' pointer is the exported CStringT<wchar_t,...> object, whose
// memory layout (a single m_pszData pointer with a CStringData header stored
// before the character buffer) is faithfully modelled by openmfc's CString.
// ---------------------------------------------------------------------------





// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* pThis, UINT nID);

// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCWordArray@@@Z
#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) \
extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { \
    return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; \
}
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDWordArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDWordArray___Z, CDWordArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCByteArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCByteArray___Z, CByteArray)
#undef OPENMFC_WRAP_SERIAL_EXTRACT
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCWordArray@@@Z
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCWordArray___Z(
    CArchive* ar, CWordArray** ppArray) {
    if (ar && ppArray) {
        if (!*ppArray) *ppArray = new CWordArray();
        impl__Serialize_CWordArray__UEAAXAEAVCArchive___Z(*ppArray, ar);
    }
    return ar;
}
#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) \
extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { \
    return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; \
}
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCObArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCObArray___Z, CObArray)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCStringArray@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCStringArray___Z, CStringArray)
#undef OPENMFC_WRAP_SERIAL_EXTRACT
#define OPENMFC_WRAP_SERIAL_EXTRACT(fn_name, class_name) extern "C" CArchive* MS_ABI fn_name(CArchive* ar, class_name** pOb) { return (ar && pOb) ? &operator>>(*ar, *pOb) : ar; }
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCStringList@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCStringList___Z, CStringList)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCObList@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCObList___Z, CObList)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapStringToOb@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapStringToOb___Z, CMapStringToOb)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapStringToString@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapStringToString___Z, CMapStringToString)
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMapWordToOb@@@Z
OPENMFC_WRAP_SERIAL_EXTRACT(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMapWordToOb___Z, CMapWordToOb)
#undef OPENMFC_WRAP_SERIAL_EXTRACT
// Symbol: ??$SerializeElements@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@@YAXAEAVCArchive@@PEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL____YAXAEAVCArchive__PEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___J_Z(
    CArchive* ar, void* pElements, __int64 nCount) {
    CString* elems = static_cast<CString*>(pElements);
    if (!ar) return;
    if (ar->IsStoring())
        for (__int64 i = 0; i < nCount; ++i) (*ar) << elems[i];
    else
        for (__int64 i = 0; i < nCount; ++i) (*ar) >> elems[i];
}
// Symbol: ??$SerializeElements@V?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@@YAXAEAVCArchive@@PEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_V__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL____YAXAEAVCArchive__PEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___J_Z(
    CArchive* ar, void* pElements, __int64 nCount) {
    using SA = openmfc_str::AtlStrT<char>;
    SA* elems = static_cast<SA*>(pElements);
    if (!ar) return;
    if (ar->IsStoring())
        for (__int64 i = 0; i < nCount; ++i) {
            UINT n = (UINT)elems[i].GetLength();
            (*ar) << n;
            if (n) ar->Write(elems[i].GetString(), n * sizeof(char));
        }
    else
        for (__int64 i = 0; i < nCount; ++i) {
            UINT n = 0; ar->Read(&n, sizeof(n));
            if (n && n < 0x10000000) {
                char* b = elems[i].GetBuffer((int)n + 1);
                ar->Read(b, n * sizeof(char)); b[n] = '\0';
                elems[i].ReleaseBuffer((int)n);
            } else elems[i].Empty();
        }
}
// Symbol: ??B?$CSimpleStringT@_W$00@ATL@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl___B__CSimpleStringT__W_00_ATL__QEBAPEB_WXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->GetString();
}
// AfxPostQuitMessage(int nExitCode): thin forward to ::PostQuitMessage.
// Symbol: ?AfxPostQuitMessage@@YAXH@Z
extern "C" void MS_ABI impl__AfxPostQuitMessage__YAXH_Z(int nExitCode) {
    ::PostQuitMessage(nExitCode);
}
// AfxSetWindowText(HWND, LPCWSTR): thin forward to ::SetWindowTextW.
// Symbol: ?AfxSetWindowText@@YAXPEAUHWND__@@PEB_W@Z
extern "C" void MS_ABI impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(
        HWND hWnd, const wchar_t* lpszText) {
    if (hWnd == nullptr) {
        return;
    }
    ::SetWindowTextW(hWnd, lpszText ? lpszText : L"");
}
// AfxVariantInit(VARIANT*): clear a VARIANT to VT_EMPTY. Thin forward to
// ::VariantInit (which simply zeroes the discriminant fields).
// Symbol: ?AfxVariantInit@@YAXPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__AfxVariantInit__YAXPEAUtagVARIANT___Z(VARIANTARG* pVar) {
    if (pVar == nullptr) {
        return;
    }
    ::VariantInit(pVar);
}
// AfxExtractSubString
// Symbol: ?AfxExtractSubString@@YAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_WH_W@Z
extern "C" int MS_ABI impl__AfxExtractSubString__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WH_W_Z(
    CString* rString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep)
{
    if (rString == nullptr || lpszFullString == nullptr || iSubString < 0)
        return FALSE;

    while (iSubString > 0)
    {
        const wchar_t* lpszNext = wcschr(lpszFullString, chSep);
        if (lpszNext == nullptr)
            return FALSE;
        lpszFullString = lpszNext + 1;
        iSubString--;
    }

    const wchar_t* lpszNext = wcschr(lpszFullString, chSep);
    if (lpszNext == nullptr)
    {
        *rString = lpszFullString;
    }
    else
    {
        int nLen = static_cast<int>(lpszNext - lpszFullString);
        wchar_t* pBuf = rString->GetBuffer(nLen);
        wmemcpy(pBuf, lpszFullString, nLen);
        pBuf[nLen] = L'\0';
        rString->ReleaseBuffer(nLen);
    }
    return TRUE;
}
// AfxFormatString1
// Symbol: ?AfxFormatString1@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W@Z
extern "C" void MS_ABI impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(
    CString* rString, UINT nIDS, const wchar_t* lpsz1)
{
    if (rString == nullptr) {
        return;
    }

    CString strFormat;
    // Call our implementation of LoadString
    impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strFormat, nIDS);
    
    if (!strFormat.IsEmpty())
    {
        LPWSTR pMessage = nullptr;
        DWORD_PTR args[] = { (DWORD_PTR)lpsz1 };
        
        DWORD dwLen = ::FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
            strFormat.GetString(),
            0,
            0,
            (LPWSTR)&pMessage,
            0,
            (va_list*)args
        );
        
        if (dwLen > 0 && pMessage != nullptr) {
            *rString = pMessage;
            ::LocalFree(pMessage);
        } else {
             *rString = strFormat;
        }
    } else {
        rString->Empty();
    }
}
// AfxFormatString2
// Symbol: ?AfxFormatString2@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W1@Z
extern "C" void MS_ABI impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z(
    CString* rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2)
{
    if (rString == nullptr) {
        return;
    }

    CString strFormat;
    impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strFormat, nIDS);

    if (!strFormat.IsEmpty())
    {
        LPWSTR pMessage = nullptr;
        DWORD_PTR args[] = { (DWORD_PTR)lpsz1, (DWORD_PTR)lpsz2 };
        
        DWORD dwLen = ::FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
            strFormat.GetString(),
            0,
            0,
            (LPWSTR)&pMessage,
            0,
            (va_list*)args
        );
        
        if (dwLen > 0 && pMessage != nullptr) {
            *rString = pMessage;
            ::LocalFree(pMessage);
        } else {
             *rString = strFormat;
        }
    } else {
        rString->Empty();
    }
}
// AfxFormatStrings(CString&, UINT, const wchar_t* const*, int)
// Symbol: ?AfxFormatStrings@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEBQEB_WH@Z
extern "C" void MS_ABI impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z(
    CString* rString, UINT nIDS, const wchar_t* const* rglpsz, int nString)
{
    CString strFormat;
    CStringLoadStringImpl(&strFormat, GetResourceHandle(), nIDS);
    AfxFormatStringsCore(rString, strFormat.GetString(), rglpsz, nString);
}
// AfxFormatStrings(CString&, const wchar_t*, const wchar_t* const*, int)
// Symbol: ?AfxFormatStrings@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_WPEBQEB_WH@Z
extern "C" void MS_ABI impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WPEBQEB_WH_Z(
    CString* rString, const wchar_t* lpszFormat, const wchar_t* const* rglpsz, int nString)
{
    AfxFormatStringsCore(rString, lpszFormat, rglpsz, nString);
}
// AfxA2WHelper
// Symbol: ?AfxA2WHelper@@YAPEA_WPEA_WPEBDH@Z
extern "C" wchar_t* MS_ABI impl__AfxA2WHelper__YAPEA_WPEA_WPEBDH_Z(wchar_t* lpw, const char* lpa, int nChars)
{
    if (lpw == nullptr) return nullptr;
    if (lpa == nullptr || nChars <= 0) {
        lpw[0] = L'\0';
        return lpw;
    }

    int nRet = ::MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpw, nChars);
    if (nRet == 0) {
        lpw[0] = L'\0';
    }
    return lpw;
}
// AfxW2AHelper
// Symbol: ?AfxW2AHelper@@YAPEADPEADPEB_WH@Z
extern "C" char* MS_ABI impl__AfxW2AHelper__YAPEADPEADPEB_WH_Z(char* lpa, const wchar_t* lpw, int nChars)
{
    if (lpa == nullptr) return nullptr;
    if (lpw == nullptr || nChars <= 0) {
        lpa[0] = '\0';
        return lpa;
    }

    int nRet = ::WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, nChars, nullptr, nullptr);
    if (nRet == 0) {
        lpa[0] = '\0';
    }
    return lpa;
}
// AfxBSTR2CString
// Symbol: ?AfxBSTR2CString@@YAXPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEA_W@Z
extern "C" void MS_ABI impl__AfxBSTR2CString__YAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEA_W_Z(
    CString* pStr, wchar_t* bstr)
{
    if (pStr == nullptr) {
        return;
    }
    if (bstr == nullptr) {
        pStr->Empty();
        return;
    }
    *pStr = bstr;
}
// AfxComparePath
// Symbol: ?AfxComparePath@@YAHPEB_W0@Z
extern "C" int MS_ABI impl__AfxComparePath__YAHPEB_W0_Z(const wchar_t* pszPath1, const wchar_t* pszPath2)
{
    if (pszPath1 == nullptr || pszPath2 == nullptr) {
        if (pszPath1 == pszPath2) return 0;
        return pszPath1 ? 1 : -1;
    }

    int nLen1 = static_cast<int>(wcslen(pszPath1));
    int nLen2 = static_cast<int>(wcslen(pszPath2));
    while (nLen1 > 0 && (pszPath1[nLen1 - 1] == L'\\' || pszPath1[nLen1 - 1] == L'/')) --nLen1;
    while (nLen2 > 0 && (pszPath2[nLen2 - 1] == L'\\' || pszPath2[nLen2 - 1] == L'/')) --nLen2;

    int nCommon = (nLen1 < nLen2) ? nLen1 : nLen2;
    for (int i = 0; i < nCommon; ++i) {
        wchar_t c1 = NormalizePathChar(pszPath1[i]);
        wchar_t c2 = NormalizePathChar(pszPath2[i]);
        if (c1 != c2) {
            return (c1 < c2) ? -1 : 1;
        }
    }

    if (nLen1 == nLen2) return 0;
    return (nLen1 < nLen2) ? -1 : 1;
}
// AfxMessageBox (text)
// Symbol: ?AfxMessageBox@@YAHPEB_WII@Z
extern "C" int MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, UINT nType, UINT nIDHelp)
{
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = (pMainWnd != nullptr) ? pMainWnd->GetSafeHwnd() : nullptr;
    return ::MessageBoxW(hWnd, lpszText, AfxGetApp() ? AfxGetApp()->m_pszAppName : L"OpenMFC", nType);
}
// AfxMessageBox (ID)
// Symbol: ?AfxMessageBox@@YAHIII@Z
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
    CString strPrompt;
    impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strPrompt, nIDPrompt);
    
    if (strPrompt.IsEmpty())
    {
        return 0;
    }
    return impl__AfxMessageBox__YAHPEB_WII_Z(strPrompt.GetString(), nType, nIDHelp);
}
