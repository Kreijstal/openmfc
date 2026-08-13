// CHeaderCtrl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 header-control wrappers.

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

namespace {
HWND HeaderHwnd(const CWnd* pThis) {
    return pThis ? pThis->GetSafeHwnd() : nullptr;
}
}

// Symbol: ?Create@CHeaderCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CHeaderCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(L"SysHeader32", L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?CreateEx@CHeaderCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CHeaderCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, L"SysHeader32", L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}

// Symbol: ?DrawItem@CHeaderCtrl@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CHeaderCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CWnd* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    if (!pDraw || !pDraw->hDC) return;
    HBRUSH brush = ::GetSysColorBrush(
        (pDraw->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_BTNFACE);
    ::FillRect(pDraw->hDC, &pDraw->rcItem, brush);
    if (pDraw->itemState & ODS_SELECTED) {
        ::SetBkColor(pDraw->hDC, ::GetSysColor(COLOR_HIGHLIGHT));
        ::SetTextColor(pDraw->hDC, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    ::DrawFocusRect(pDraw->hDC, &pDraw->rcItem);
}

// Symbol: ?GetOrderArray@CHeaderCtrl@@QEBAHPEAHH@Z
extern "C" int MS_ABI impl__GetOrderArray_CHeaderCtrl__QEBAHPEAHH_Z(
    const CWnd* pThis, int* piArray, int iCount) {
    if (!piArray || iCount <= 0) return FALSE;
    return (int)::SendMessageW(HeaderHwnd(pThis), HDM_GETORDERARRAY, (WPARAM)iCount, (LPARAM)piArray);
}

// Symbol: ?OnChildNotify@CHeaderCtrl@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CHeaderCtrl__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, UINT64 wParam, INT64 lParam, INT64* pResult) {
    (void)pThis; (void)message; (void)wParam; (void)lParam; (void)pResult;
    // No reflected message-map handlers are installed for the header control;
    // returning FALSE lets the parent window handle the notification.
    return FALSE;
}

// Symbol: ?SetOrderArray@CHeaderCtrl@@QEAAHHPEAH@Z
extern "C" int MS_ABI impl__SetOrderArray_CHeaderCtrl__QEAAHHPEAH_Z(
    CWnd* pThis, int iCount, int* piArray) {
    if (!piArray || iCount <= 0) return FALSE;
    return (int)::SendMessageW(HeaderHwnd(pThis), HDM_SETORDERARRAY, (WPARAM)iCount, (LPARAM)piArray);
}
