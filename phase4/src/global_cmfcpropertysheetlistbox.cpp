// OpenMFC — real implementations for CMFCPropertySheetListBox stub exports.
//
// CMFCPropertySheetListBox (base CListBox) is the owner-draw list box used
// inside the CMFCPropertySheet navigation pane (list-style). It draws page
// icons and names and tracks mouse hover for highlighting.
// This file provides minimal implementations.

#include <windows.h>
#include <cstring>
#include <new>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ??0CMFCPropertySheetListBox@@AEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertySheetListBox__AEAA_XZ(void* pThis) {
    // CListBox is the base class for this control; zero initialize object memory
    // so dependent fields start known before method calls.
    std::memset(pThis, 0, sizeof(CListBox));
    return pThis;
}

// Symbol: ?DrawItem@CMFCPropertySheetListBox@@EEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CMFCPropertySheetListBox__EEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDrawItemStruct) {
    if (!pThis || !lpDrawItemStruct || !lpDrawItemStruct->hDC ||
        lpDrawItemStruct->itemID == static_cast<UINT>(-1)) {
        return;
    }

    auto* pListBox = static_cast<CListBox*>(pThis);
    const bool selected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
    const bool disabled = (lpDrawItemStruct->itemState & ODS_DISABLED) != 0;
    const COLORREF background = selected ? GetSysColor(COLOR_HIGHLIGHT) :
        GetSysColor(COLOR_WINDOW);
    const COLORREF foreground = disabled ? GetSysColor(COLOR_GRAYTEXT) :
        (selected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));

    RECT rect = lpDrawItemStruct->rcItem;
    HBRUSH brush = CreateSolidBrush(background);
    if (brush) {
        FillRect(lpDrawItemStruct->hDC, &rect, brush);
        DeleteObject(brush);
    }

    HWND hWnd = pListBox->m_hWnd;
    wchar_t text[512] = {};
    if (IsWindow(hWnd)) {
        SendMessageW(hWnd, LB_GETTEXT, lpDrawItemStruct->itemID,
                     reinterpret_cast<LPARAM>(text));
    }
    SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
    SetTextColor(lpDrawItemStruct->hDC, foreground);
    rect.left += 4;
    DrawTextW(lpDrawItemStruct->hDC, text, -1, &rect,
              DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

    if (lpDrawItemStruct->itemState & ODS_FOCUS) {
        RECT focus = lpDrawItemStruct->rcItem;
        DrawFocusRect(lpDrawItemStruct->hDC, &focus);
    }
}

// Symbol: ?MeasureItem@CMFCPropertySheetListBox@@EEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CMFCPropertySheetListBox__EEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, MEASUREITEMSTRUCT* lpMeasureItemStruct) {
    (void)pThis;
    if (lpMeasureItemStruct) {
        lpMeasureItemStruct->itemHeight = 20; // Default item height
    }
}

// Symbol: ?OnMouseLeave@CMFCPropertySheetListBox@@AEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCPropertySheetListBox__AEAAXXZ(void* pThis) {
    if (!pThis) return;
    HWND hWnd = static_cast<CListBox*>(pThis)->m_hWnd;
    if (IsWindow(hWnd)) InvalidateRect(hWnd, nullptr, FALSE);
}

// Symbol: ?OnMouseMove@CMFCPropertySheetListBox@@AEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCPropertySheetListBox__AEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    HWND hWnd = static_cast<CListBox*>(pThis)->m_hWnd;
    if (!IsWindow(hWnd)) return;
    TRACKMOUSEEVENT event = { sizeof(event), TME_LEAVE, hWnd, 0 };
    TrackMouseEvent(&event);
    (void)point;
    InvalidateRect(hWnd, nullptr, FALSE);
}
