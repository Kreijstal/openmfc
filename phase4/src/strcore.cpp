// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstdint>

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

// CString::LoadString(UINT nID)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QAAHI@Z
extern "C" int MS_ABI stub__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* pThis, UINT nID)
{
    // Try fixed buffer first (usually enough)
    const int nFixedBuffer = 256;
    wchar_t szFixedBuffer[nFixedBuffer];
    
    HINSTANCE hInst = GetResourceHandle();

    int nLen = ::LoadStringW(hInst, nID, szFixedBuffer, nFixedBuffer);
    if (nLen == 0) {
        pThis->Empty();
        return 0;
    }

    if (nLen < nFixedBuffer - 1) {
        *pThis = szFixedBuffer;
        return nLen;
    }

    // Too big, try larger buffer
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

// AfxExtractSubString
// Symbol: ?AfxExtractSubString@@YAHAAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PB_WH_W@Z
extern "C" int MS_ABI stub__AfxExtractSubString__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WH_W_Z(
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
// Symbol: ?AfxFormatString1@@YAXAAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPB_W@Z
extern "C" void MS_ABI stub__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(
    CString* rString, UINT nIDS, const wchar_t* lpsz1)
{
    CString strFormat;
    // Call our implementation of LoadString
    stub__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strFormat, nIDS);
    
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
// Symbol: ?AfxFormatString2@@YAXAAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPB_W1@Z
extern "C" void MS_ABI stub__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z(
    CString* rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2)
{
    CString strFormat;
    stub__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strFormat, nIDS);

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

// AfxMessageBox (text)
// Symbol: ?AfxMessageBox@@YAHPB_WII@Z
extern "C" int MS_ABI stub__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, UINT nType, UINT nIDHelp)
{
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = (pMainWnd != nullptr) ? pMainWnd->GetSafeHwnd() : nullptr;
    return ::MessageBoxW(hWnd, lpszText, AfxGetApp() ? AfxGetApp()->m_pszAppName : L"OpenMFC", nType);
}

// AfxMessageBox (ID)
// Symbol: ?AfxMessageBox@@YAHIII@Z
extern "C" int MS_ABI stub__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
    CString strPrompt;
    stub__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&strPrompt, nIDPrompt);
    
    if (strPrompt.IsEmpty())
    {
        return 0;
    }
    return stub__AfxMessageBox__YAHPEB_WII_Z(strPrompt.GetString(), nType, nIDHelp);
}
