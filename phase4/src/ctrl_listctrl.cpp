// CListCtrl exported wrappers
//
// Keep list-view exports in their own shard so future common-control work can
// be assigned by file without touching the shared control core.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

class CHeaderCtrl;

namespace {
static HWND ListCtrlHwnd(const CListCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}

static BOOL ListCtrlGetItem(HWND hWnd, int nItem, UINT mask, LVITEMW* pItem) {
    if (!hWnd || !pItem || nItem < 0) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->iItem = nItem;
    return (BOOL)::SendMessageW(hWnd, LVM_GETITEMW, 0, (LPARAM)pItem);
}
}  // namespace

// Symbol: ?CreateEx@CListCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CListCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CListCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, L"SysListView32", L"", dwStyle, pRect->left, pRect->top,
        pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?CreateDragImage@CListCtrl@@QEAAPEAVCImageList@@HPEAUtagPOINT@@@Z
extern "C" CImageList* MS_ABI impl__CreateDragImage_CListCtrl__QEAAPEAVCImageList__HPEAUtagPOINT___Z(
    CListCtrl* pThis, int nItem, POINT* lpPoint) {
    HIMAGELIST hImage = (HIMAGELIST)::SendMessageW(ListCtrlHwnd(pThis), LVM_CREATEDRAGIMAGE, nItem, (LPARAM)lpPoint);
    if (!hImage) return nullptr;
    CImageList* pImageList = new CImageList();
    pImageList->Attach(hImage);
    return pImageList;
}

// Symbol: ?DrawItem@CListCtrl@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CListCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CListCtrl* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    if (!pDraw || !pDraw->hDC) return;
    HBRUSH brush = ::GetSysColorBrush((pDraw->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW);
    ::FillRect(pDraw->hDC, &pDraw->rcItem, brush);
}

// Symbol: ?GetCheck@CListCtrl@@QEBAHH@Z
extern "C" int MS_ABI impl__GetCheck_CListCtrl__QEBAHH_Z(const CListCtrl* pThis, int nItem) {
    UINT state = (UINT)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETITEMSTATE, nItem, LVIS_STATEIMAGEMASK);
    return ((state >> 12) - 1) > 0 ? TRUE : FALSE;
}

// Symbol: ?GetColumnOrderArray@CListCtrl@@QEBAHPEAHH@Z
extern "C" int MS_ABI impl__GetColumnOrderArray_CListCtrl__QEBAHPEAHH_Z(
    const CListCtrl* pThis, int* piArray, int iCount) {
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETCOLUMNORDERARRAY, iCount, (LPARAM)piArray);
}

// Symbol: ?GetHeaderCtrl@CListCtrl@@QEBAPEAVCHeaderCtrl@@XZ
extern "C" CHeaderCtrl* MS_ABI impl__GetHeaderCtrl_CListCtrl__QEBAPEAVCHeaderCtrl__XZ(const CListCtrl* pThis) {
    return (CHeaderCtrl*)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETHEADER, 0, 0);
}

// Symbol: ?GetItemData@CListCtrl@@QEBA_KH@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CListCtrl__QEBA_KH_Z(const CListCtrl* pThis, int nItem) {
    LVITEMW item = {};
    return ListCtrlGetItem(ListCtrlHwnd(pThis), nItem, LVIF_PARAM, &item) ? (uintptr_t)item.lParam : 0;
}

// Symbol: ?GetItemRect@CListCtrl@@QEBAHHPEAUtagRECT@@I@Z
extern "C" int MS_ABI impl__GetItemRect_CListCtrl__QEBAHHPEAUtagRECT__I_Z(
    const CListCtrl* pThis, int nItem, RECT* lpRect, UINT nCode) {
    if (!lpRect) return FALSE;
    lpRect->left = (LONG)nCode;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETITEMRECT, nItem, (LPARAM)lpRect);
}

// Symbol: ?GetItemText@CListCtrl@@QEBAHHHPEA_WH@Z
extern "C" int MS_ABI impl__GetItemText_CListCtrl__QEBAHHHPEA_WH_Z(
    const CListCtrl* pThis, int nItem, int nSubItem, wchar_t* lpszText, int nLen) {
    if (!lpszText || nLen <= 0) return 0;
    LVITEMW item = {};
    item.iSubItem = nSubItem;
    item.pszText = lpszText;
    item.cchTextMax = nLen;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETITEMTEXTW, nItem, (LPARAM)&item);
}

// Symbol: ?GetItemText@CListCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HH@Z
extern "C" void MS_ABI impl__GetItemText_CListCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HH_Z(
    CString* pRet, const CListCtrl* pThis, int nItem, int nSubItem) {
    if (!pRet) {
        return;
    }
    new (pRet) CString();
    HWND hWnd = ListCtrlHwnd(pThis);
    if (!hWnd) {
        return;
    }

    int capacity = 128;
    for (;;) {
        wchar_t* buffer = pRet->GetBuffer(capacity);
        int copied = impl__GetItemText_CListCtrl__QEBAHHHPEA_WH_Z(pThis, nItem, nSubItem, buffer, capacity);
        pRet->ReleaseBuffer(copied > 0 ? copied : 0);
        if (copied < capacity - 1 || capacity >= 32768) {
            return;
        }
        capacity *= 2;
    }
}

// Symbol: ?GetMessageMap@CListCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CListCtrl__MEBAPEBUAFX_MSGMAP__XZ(const CListCtrl* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CListCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CListCtrl__UEBAPEAUCRuntimeClass__XZ(const CListCtrl* pThis) {
    (void)pThis;
    return CListCtrl::GetThisClass();
}

// Symbol: ?GetSubItemRect@CListCtrl@@QEBAHHHHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetSubItemRect_CListCtrl__QEBAHHHHAEAVCRect___Z(
    const CListCtrl* pThis, int iItem, int iSubItem, int nArea, CRect* ref) {
    if (!ref) return FALSE;
    ref->top = iSubItem;
    ref->left = nArea;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_GETSUBITEMRECT, iItem, (LPARAM)ref);
}

// Symbol: ?GetThisClass@CListCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListCtrl__SAPEAUCRuntimeClass__XZ() {
    return CListCtrl::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CListCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CListCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?HitTest@CListCtrl@@QEBAHVCPoint@@PEAI@Z
extern "C" int MS_ABI impl__HitTest_CListCtrl__QEBAHVCPoint__PEAI_Z(
    const CListCtrl* pThis, CPoint pt, UINT* pFlags) {
    LVHITTESTINFO hit = {};
    hit.pt.x = pt.x;
    hit.pt.y = pt.y;
    int item = (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_HITTEST, 0, (LPARAM)&hit);
    if (pFlags) *pFlags = hit.flags;
    return item;
}

// Symbol: ?InsertColumn@CListCtrl@@QEAAHHPEB_WHHH@Z
extern "C" int MS_ABI impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(
    CListCtrl* pThis, int nCol, const wchar_t* lpszColumnHeading, int nFormat, int nWidth, int nSubItem) {
    return pThis ? pThis->InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem) : -1;
}

// Symbol: ?InsertItem@CListCtrl@@QEAAHIHPEB_WIIH_J@Z
extern "C" int MS_ABI impl__InsertItem_CListCtrl__QEAAHIHPEB_WIIH_J_Z(
    CListCtrl* pThis, UINT nMask, int nItem, const wchar_t* lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam) {
    LVITEMW item = {};
    item.mask = nMask;
    item.iItem = nItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.state = nState;
    item.stateMask = nStateMask;
    item.iImage = nImage;
    item.lParam = lParam;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_INSERTITEMW, 0, (LPARAM)&item);
}

// Symbol: ?OnChildNotify@CListCtrl@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CListCtrl__MEAAHI_K_JPEA_J_Z(
    CListCtrl* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)pThis;
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CListCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?OnNcDestroy@CListCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CListCtrl__IEAAXXZ(CListCtrl* pThis) {
    if (pThis) pThis->m_hWnd = nullptr;
}

// Symbol: ?RemoveImageList@CListCtrl@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CListCtrl__IEAAXH_Z(CListCtrl* pThis, int nImageList) {
    ::SendMessageW(ListCtrlHwnd(pThis), LVM_SETIMAGELIST, nImageList, 0);
}

// Symbol: ?SetBkImage@CListCtrl@@QEAAHPEA_WHHH@Z
extern "C" int MS_ABI impl__SetBkImage_CListCtrl__QEAAHPEA_WHHH_Z(
    CListCtrl* pThis, wchar_t* pszUrl, int bTile, int xOffsetPercent, int yOffsetPercent) {
    LVBKIMAGEW image = {};
    image.ulFlags = LVBKIF_SOURCE_URL | (bTile ? LVBKIF_STYLE_TILE : LVBKIF_STYLE_NORMAL);
    image.pszImage = pszUrl;
    image.xOffsetPercent = xOffsetPercent;
    image.yOffsetPercent = yOffsetPercent;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETBKIMAGEW, 0, (LPARAM)&image);
}

// Symbol: ?SetBkImage@CListCtrl@@QEAAHPEAUHBITMAP__@@HHH@Z
extern "C" int MS_ABI impl__SetBkImage_CListCtrl__QEAAHPEAUHBITMAP____HHH_Z(
    CListCtrl* pThis, HBITMAP hBitmap, int bTile, int xOffsetPercent, int yOffsetPercent) {
    LVBKIMAGEW image = {};
    image.ulFlags = LVBKIF_SOURCE_HBITMAP | (bTile ? LVBKIF_STYLE_TILE : LVBKIF_STYLE_NORMAL);
    image.hbm = hBitmap;
    image.xOffsetPercent = xOffsetPercent;
    image.yOffsetPercent = yOffsetPercent;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETBKIMAGEW, 0, (LPARAM)&image);
}

// Symbol: ?SetCheck@CListCtrl@@QEAAHHH@Z
extern "C" int MS_ABI impl__SetCheck_CListCtrl__QEAAHHH_Z(CListCtrl* pThis, int nItem, int fCheck) {
    LVITEMW item = {};
    item.stateMask = LVIS_STATEIMAGEMASK;
    item.state = INDEXTOSTATEIMAGEMASK(fCheck ? 2 : 1);
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETITEMSTATE, nItem, (LPARAM)&item);
}

// Symbol: ?SetColumnOrderArray@CListCtrl@@QEAAHHPEAH@Z
extern "C" int MS_ABI impl__SetColumnOrderArray_CListCtrl__QEAAHHPEAH_Z(
    CListCtrl* pThis, int iCount, int* piArray) {
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETCOLUMNORDERARRAY, iCount, (LPARAM)piArray);
}

// Symbol: ?SetIconSpacing@CListCtrl@@QEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__SetIconSpacing_CListCtrl__QEAA_AVCSize__HH_Z(
    CSize* pRet, CListCtrl* pThis, int cx, int cy) {
    DWORD previous = (DWORD)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETICONSPACING, 0, MAKELPARAM(cx, cy));
    if (pRet) *pRet = CSize(LOWORD(previous), HIWORD(previous));
}

// Symbol: ?SetIconSpacing@CListCtrl@@QEAA?AVCSize@@V2@@Z
extern "C" void MS_ABI impl__SetIconSpacing_CListCtrl__QEAA_AVCSize__V2__Z(
    CSize* pRet, CListCtrl* pThis, CSize size) {
    impl__SetIconSpacing_CListCtrl__QEAA_AVCSize__HH_Z(pRet, pThis, size.cx, size.cy);
}

// Symbol: ?SetItem@CListCtrl@@QEAAHHHIPEB_WHII_J@Z
extern "C" int MS_ABI impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_J_Z(
    CListCtrl* pThis, int nItem, int nSubItem, UINT nMask, const wchar_t* lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam) {
    LVITEMW item = {};
    item.mask = nMask;
    item.iItem = nItem;
    item.iSubItem = nSubItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETITEMW, 0, (LPARAM)&item);
}

// Symbol: ?SetItem@CListCtrl@@QEAAHHHIPEB_WHII_JH@Z
extern "C" int MS_ABI impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_JH_Z(
    CListCtrl* pThis, int nItem, int nSubItem, UINT nMask, const wchar_t* lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam, int nIndent) {
    LVITEMW item = {};
    item.mask = nMask | LVIF_INDENT;
    item.iItem = nItem;
    item.iSubItem = nSubItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    item.iIndent = nIndent;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETITEMW, 0, (LPARAM)&item);
}

// Symbol: ?SetItemCountEx@CListCtrl@@QEAAHHK@Z
extern "C" int MS_ABI impl__SetItemCountEx_CListCtrl__QEAAHHK_Z(
    CListCtrl* pThis, int iCount, DWORD dwFlags) {
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETITEMCOUNT, iCount, dwFlags);
}

// Symbol: ?SetItemState@CListCtrl@@QEAAHHII@Z
extern "C" int MS_ABI impl__SetItemState_CListCtrl__QEAAHHII_Z(
    CListCtrl* pThis, int nItem, UINT nState, UINT nMask) {
    LVITEMW item = {};
    item.state = nState;
    item.stateMask = nMask;
    return (int)::SendMessageW(ListCtrlHwnd(pThis), LVM_SETITEMSTATE, nItem, (LPARAM)&item);
}

// Symbol: ?SetItemText@CListCtrl@@QEAAHHHPEB_W@Z
extern "C" int MS_ABI impl__SetItemText_CListCtrl__QEAAHHHPEB_W_Z(
    CListCtrl* pThis, int nItem, int nSubItem, const wchar_t* lpszText) {
    return pThis ? pThis->SetItemText(nItem, nSubItem, lpszText) : FALSE;
}
