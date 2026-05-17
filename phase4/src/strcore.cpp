// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstdint>
#include <cwctype>

// MS ABI calling convention for x64
#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Helper to get resource handle
static HINSTANCE GetResourceHandle() {
    HINSTANCE hInst = AfxGetResourceHandle();
    if (hInst == nullptr) {
        hInst = AfxGetInstanceHandle();
    }
    return hInst;
}

static int CStringLoadStringImpl(CString* pThis, HINSTANCE hInst, UINT nID) {
    if (pThis == nullptr) {
        return 0;
    }
    if (hInst == nullptr) {
        hInst = GetResourceHandle();
    }

    const int nFixedBuffer = 256;
    wchar_t szFixedBuffer[nFixedBuffer];

    int nLen = ::LoadStringW(hInst, nID, szFixedBuffer, nFixedBuffer);
    if (nLen == 0) {
        pThis->Empty();
        return 0;
    }

    if (nLen < nFixedBuffer - 1) {
        *pThis = szFixedBuffer;
        return nLen;
    }

    int nSize = nFixedBuffer * 2;
    while (nSize < 32768) {
        wchar_t* pBuf = pThis->GetBuffer(nSize);
        nLen = ::LoadStringW(hInst, nID, pBuf, nSize);
        if (nLen < nSize - 1) {
            pThis->ReleaseBuffer(nLen);
            return nLen;
        }
        pThis->ReleaseBuffer(0);
        nSize *= 2;
    }

    pThis->Empty();
    return 0;
}

// CString::LoadString(UINT nID)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* pThis, UINT nID)
{
    return CStringLoadStringImpl(pThis, GetResourceHandle(), nID);
}

// CString::LoadString(HINSTANCE, UINT)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@I@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID)
{
    return CStringLoadStringImpl(pThis, hInst, nID);
}

// CString::LoadString(HINSTANCE, UINT, LANGID)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@IG@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(
    CString* pThis, HINSTANCE hInst, UINT nID, unsigned short /*wLanguageID*/)
{
    return CStringLoadStringImpl(pThis, hInst, nID);
}

// AfxExtractSubString
// Symbol: ?AfxExtractSubString@@YAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_WH_W@Z
extern "C" int MS_ABI impl__AfxExtractSubString__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WH_W_Z(
    CString* rString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep)
{
    if (lpszFullString == nullptr)
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
    }
}

// AfxFormatString2
// Symbol: ?AfxFormatString2@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W1@Z
extern "C" void MS_ABI impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z(
    CString* rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2)
{
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
    }
}

static void AfxFormatStringsCore(CString* rString, const wchar_t* lpszFormat, const wchar_t* const* rglpsz, int nString)
{
    if (rString == nullptr) {
        return;
    }
    if (lpszFormat == nullptr) {
        rString->Empty();
        return;
    }

    CString strResult;
    const wchar_t* p = lpszFormat;
    while (*p != L'\0')
    {
        if (*p == L'%' && p[1] >= L'1' && p[1] <= L'9')
        {
            int nIndex = static_cast<int>(p[1] - L'1');
            if (rglpsz != nullptr && nIndex >= 0 && nIndex < nString && rglpsz[nIndex] != nullptr) {
                strResult += rglpsz[nIndex];
            }
            p += 2;
            continue;
        }
        wchar_t ch[2] = { *p, L'\0' };
        strResult += ch;
        ++p;
    }

    *rString = strResult;
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

static wchar_t NormalizePathChar(wchar_t ch)
{
    if (ch == L'/') return L'\\';
    return static_cast<wchar_t>(std::towlower(static_cast<wint_t>(ch)));
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
