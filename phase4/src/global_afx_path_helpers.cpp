// Global residuals shard 06/12: global
//
// Real implementations for a small set of global AFX helper functions whose
// behavior is unambiguous from the MFC contract. Conservative: only thin Win32
// forwards / pure path helpers are implemented here.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <cstring>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// ?AfxIsDescendant@@YAHPEAUHWND__@@0@Z
//   int AfxIsDescendant(HWND hWndParent, HWND hWndChild)
// Returns nonzero if hWndChild is hWndParent itself or a descendant window.
// =============================================================================
// Symbol: ?AfxIsDescendant@@YAHPEAUHWND__@@0@Z
extern "C" int MS_ABI impl__AfxIsDescendant__YAHPEAUHWND____0_Z(HWND hWndParent,
                                                                HWND hWndChild)
{
    if (hWndParent == nullptr || hWndChild == nullptr)
        return 0;
    if (hWndParent == hWndChild)
        return 1;
    return ::IsChild(hWndParent, hWndChild) ? 1 : 0;
}

// =============================================================================
// ?AfxGetFileName@@YAIPEB_WPEA_WI@Z
//   UINT AfxGetFileName(LPCWSTR lpszPathName, LPWSTR lpszTitle, UINT nMax)
// Extracts the file name portion (with extension) of a path. If lpszTitle is
// NULL, returns the buffer length required (including the terminating null).
// Otherwise copies up to nMax characters and returns 0.
// =============================================================================
// Symbol: ?AfxGetFileName@@YAIPEB_WPEA_WI@Z
extern "C" unsigned int MS_ABI impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(
    const wchar_t* lpszPathName, wchar_t* lpszTitle, unsigned int nMax)
{
    if (lpszPathName == nullptr)
        return 0;

    // Find the start of the file name: character after the last '\\', '/' or ':'.
    const wchar_t* lpszName = lpszPathName;
    for (const wchar_t* p = lpszPathName; *p != L'\0'; ++p) {
        if (*p == L'\\' || *p == L'/' || *p == L':')
            lpszName = p + 1;
    }

    size_t nLen = wcslen(lpszName);

    if (lpszTitle == nullptr)
        return static_cast<unsigned int>(nLen) + 1; // include terminating null

    if (nMax == 0)
        return 0;

    // Copy with truncation, always null-terminate (lstrcpyn semantics).
    unsigned int nCopy = static_cast<unsigned int>(nLen);
    if (nCopy > nMax - 1)
        nCopy = nMax - 1;
    if (nCopy > 0)
        wmemcpy(lpszTitle, lpszName, nCopy);
    lpszTitle[nCopy] = L'\0';
    return 0;
}

// =============================================================================
// ?AfxFullPath@@YAHPEA_WPEB_W@Z
//   BOOL AfxFullPath(LPWSTR lpszPathOut, LPCWSTR lpszFileIn)
// Resolves lpszFileIn to a fully-qualified path written to lpszPathOut, which
// must be at least _MAX_PATH characters. Returns nonzero on success.
// =============================================================================
// Symbol: ?AfxFullPath@@YAHPEA_WPEB_W@Z
extern "C" int MS_ABI impl__AfxFullPath__YAHPEA_WPEB_W_Z(wchar_t* lpszPathOut,
                                                         const wchar_t* lpszFileIn)
{
    if (lpszPathOut == nullptr || lpszFileIn == nullptr)
        return 0;

    wchar_t* lpszFilePart = nullptr;
    DWORD dwRet = ::GetFullPathNameW(lpszFileIn, MAX_PATH, lpszPathOut, &lpszFilePart);
    // Success requires the result to fit within the MAX_PATH output buffer.
    return (dwRet != 0 && dwRet < MAX_PATH) ? 1 : 0;
}
