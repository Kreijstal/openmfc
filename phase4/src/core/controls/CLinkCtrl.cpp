// CLinkCtrl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 syslink-control wrappers.

#define OPENMFC_APPCORE_IMPL

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?Create@CLinkCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CLinkCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(WC_LINK, L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?Create@CLinkCtrl@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CLinkCtrl__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, const wchar_t* lpszLinkMarkup, DWORD dwStyle, const RECT* pRect,
    CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(WC_LINK, lpszLinkMarkup ? lpszLinkMarkup : L"", dwStyle,
        *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?CreateEx@CLinkCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CLinkCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, WC_LINK, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}

// Symbol: ?CreateEx@CLinkCtrl@@UEAAHPEB_WKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CLinkCtrl__UEAAHPEB_WKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, const wchar_t* lpszLinkMarkup, DWORD dwExStyle, DWORD dwStyle,
    const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, WC_LINK, lpszLinkMarkup ? lpszLinkMarkup : L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}
