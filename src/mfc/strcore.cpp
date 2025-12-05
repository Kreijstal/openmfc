#include "openmfc/afxwin.h"
#include <cwchar>
#include <cstdio>
#include <vector>

// Windows API for LoadString
#ifdef _WIN32
    #include <windows.h>
#else
    // Stub for non-Windows
    #ifndef _WINDOWS_
        typedef void* HINSTANCE;
        typedef unsigned int UINT;
        typedef wchar_t* LPWSTR;
        typedef const wchar_t* LPCWSTR;
        #define WINAPI
    #endif
    
    extern "C" int WINAPI LoadStringW(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int cchBufferMax) {
        // Mock implementation for testing without resources
        // In a real Linux implementation, this would read from a resource file or .so
        // For now, we just return a dummy string based on ID
        if (cchBufferMax > 0) {
            if (uID == 999) {
                // Special case for formatting tests
                wcscpy(lpBuffer, L"Format %1 and %2");
                return static_cast<int>(wcslen(lpBuffer));
            }
            int len = swprintf(lpBuffer, cchBufferMax, L"Resource String %u", uID);
            return len > 0 ? len : 0;
        }
        return 0;
    }
    
    extern "C" int WINAPI MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
        std::printf("MessageBox: %ls\n", lpText);
        return 1; // IDOK
    }
#endif

// Prevent macro collision with CString::LoadString
#ifdef LoadString
#undef LoadString
#endif

// CString::LoadString implementation
int CString::LoadString(UINT nID) {
    // Try fixed buffer first (typical max length 256)
    wchar_t szBuffer[256];
    int nLen = ::LoadStringW(AfxGetResourceHandle(), nID, szBuffer, 255);
    
    if (nLen > 0) {
        // Success with fixed buffer
        *this = szBuffer;
        return nLen;
    }
    
    // If failed, might be too long or not found.
    // Try larger buffer if needed, but for now 256 is standard for MFC string resources.
    // Real MFC does a more complex loop here.
    return 0;
}

// Global String Helpers

bool AFXAPI AfxExtractSubString(CString& rString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep) {
    if (lpszFullString == nullptr) return false;
    
    int iCurrent = 0;
    const wchar_t* pStart = lpszFullString;
    
    while (iCurrent < iSubString) {
        const wchar_t* pSep = wcschr(pStart, chSep);
        if (pSep == nullptr) return false; // Not enough substrings
        pStart = pSep + 1;
        iCurrent++;
    }
    
    // Found start of substring
    const wchar_t* pEnd = wcschr(pStart, chSep);
    if (pEnd == nullptr) {
        // Last substring
        rString = pStart;
    } else {
        // Middle substring
        int nLen = static_cast<int>(pEnd - pStart);
        wchar_t* pBuf = rString.GetBuffer(nLen);
        wmemcpy(pBuf, pStart, nLen);
        rString.ReleaseBuffer(nLen);
    }
    
    return true;
}

static void AfxFormatString(CString& rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2) {
    CString strTemplate;
    if (!strTemplate.LoadString(nIDS)) {
        // Failed to load template
        rString.Empty();
        return;
    }
    
    // Replace %1 and %2
    // Simple implementation: scan and build new string
    CString strResult;
    const wchar_t* p = strTemplate.GetString();
    
    while (*p != L'\0') {
        if (*p == L'%' && (*(p+1) == L'1' || *(p+1) == L'2')) {
            if (*(p+1) == L'1' && lpsz1 != nullptr) {
                strResult += lpsz1;
                p += 2;
            } else if (*(p+1) == L'2' && lpsz2 != nullptr) {
                strResult += lpsz2;
                p += 2;
            } else {
                // Argument missing, treat as literal
                wchar_t buf[2] = { *p, L'\0' };
                strResult += buf;
                p++;
            }
        } else {
            // Append char
            // CString optimization: could batch append, but char-by-char is safe for now
            wchar_t buf[2] = { *p, L'\0' };
            strResult += buf;
            p++;
        }
    }
    
    rString = strResult;
}

void AFXAPI AfxFormatString1(CString& rString, UINT nIDS, const wchar_t* lpsz1) {
    AfxFormatString(rString, nIDS, lpsz1, nullptr);
}

void AFXAPI AfxFormatString2(CString& rString, UINT nIDS, const wchar_t* lpsz1, const wchar_t* lpsz2) {
    AfxFormatString(rString, nIDS, lpsz1, lpsz2);
}

int AFXAPI AfxMessageBox(const wchar_t* lpszText, UINT nType, UINT nIDHelp) {
    (void)nIDHelp; // Help context ignored for now
    
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = pMainWnd ? pMainWnd->GetSafeHwnd() : nullptr;
    
    // Get app name for caption
    CWinApp* pApp = AfxGetApp();
    const wchar_t* lpszCaption = (pApp && pApp->m_pszAppName) ? pApp->m_pszAppName : L"Error";
    
    return ::MessageBoxW(hWnd, lpszText, lpszCaption, nType);
}

int AFXAPI AfxMessageBox(UINT nIDPrompt, UINT nType, UINT nIDHelp) {
    CString strText;
    if (!strText.LoadString(nIDPrompt)) {
        // Fallback if resource load fails
        wchar_t szBuf[32];
        swprintf(szBuf, 32, L"Message ID %u", nIDPrompt);
        strText = szBuf;
    }
    return AfxMessageBox(strText, nType, nIDHelp);
}
