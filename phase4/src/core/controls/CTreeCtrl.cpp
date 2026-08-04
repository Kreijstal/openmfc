// CTreeCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// Symbol: ?CreateDragImage@CTreeCtrl@@QEAAPEAVCImageList@@PEAU_TREEITEM@@@Z
extern "C" CImageList* MS_ABI impl__CreateDragImage_CTreeCtrl__QEAAPEAVCImageList__PEAU_TREEITEM___Z(
    CTreeCtrl* pThis, HTREEITEM hItem) {
    HIMAGELIST hImage = (HIMAGELIST)::SendMessageW(TreeCtrlHwnd(pThis), TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
    if (!hImage) return nullptr;
    CImageList* pImageList = new CImageList();
    pImageList->Attach(hImage);
    return pImageList;
}
// Symbol: ?CreateEx@CTreeCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CTreeCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CTreeCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, L"SysTreeView32", L"", dwStyle, pRect->left, pRect->top,
        pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
// Symbol: ?GetCheck@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__GetCheck_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(const CTreeCtrl* pThis, HTREEITEM hItem) {
    UINT state = (UINT)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMSTATE, (WPARAM)hItem, TVIS_STATEIMAGEMASK);
    return ((state >> 12) - 1) > 0 ? TRUE : FALSE;
}
// Symbol: ?GetItemData@CTreeCtrl@@QEBA_KPEAU_TREEITEM@@@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMW item = {};
    return TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_PARAM, &item) ? (uintptr_t)item.lParam : 0;
}
// Symbol: ?GetItemText@CTreeCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAU_TREEITEM@@@Z
extern "C" void MS_ABI impl__GetItemText_CTreeCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_TREEITEM___Z(
    CString* pRet, const CTreeCtrl* pThis, HTREEITEM hItem) {
    if (!pRet) {
        return;
    }
    new (pRet) CString();
    HWND hWnd = TreeCtrlHwnd(pThis);
    if (!hWnd || !hItem) {
        return;
    }

    int capacity = 128;
    for (;;) {
        wchar_t* buffer = pRet->GetBuffer(capacity);
        TVITEMW item = {};
        item.mask = TVIF_TEXT;
        item.hItem = hItem;
        item.pszText = buffer;
        item.cchTextMax = capacity;
        BOOL ok = (BOOL)::SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)&item);
        int copied = ok ? (int)wcslen(buffer) : 0;
        pRet->ReleaseBuffer(copied);
        if (!ok || copied < capacity - 1 || capacity >= 32768) {
            return;
        }
        capacity *= 2;
    }
}
// Symbol: ?GetItemExpandedImageIndex@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__GetItemExpandedImageIndex_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMEXW item = {};
    return TreeCtrlGetItemEx(TreeCtrlHwnd(pThis), hItem, TVIF_EXPANDEDIMAGE, &item) ? item.iExpandedImage : -1;
}
// Symbol: ?GetItemImage@CTreeCtrl@@QEBAHPEAU_TREEITEM@@AEAH1@Z
extern "C" int MS_ABI impl__GetItemImage_CTreeCtrl__QEBAHPEAU_TREEITEM__AEAH1_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, int* pImage, int* pSelectedImage) {
    if (pImage) *pImage = 0;
    if (pSelectedImage) *pSelectedImage = 0;
    TVITEMW item = {};
    if (!TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE, &item)) return FALSE;
    if (pImage) *pImage = item.iImage;
    if (pSelectedImage) *pSelectedImage = item.iSelectedImage;
    return TRUE;
}
// Symbol: ?GetItemPartRect@CTreeCtrl@@QEAAHPEAU_TREEITEM@@W4_TVITEMPART@@PEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetItemPartRect_CTreeCtrl__QEAAHPEAU_TREEITEM__W4_TVITEMPART__PEAUtagRECT___Z(
    CTreeCtrl* pThis, HTREEITEM hItem, TVITEMPART partID, RECT* pRect) {
    if (!pRect) return FALSE;
    TVGETITEMPARTRECTINFO info = {};
    info.hti = hItem;
    info.prc = pRect;
    info.partID = partID;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMPARTRECT, 0, (LPARAM)&info);
}
// Symbol: ?GetItemRect@CTreeCtrl@@QEBAHPEAU_TREEITEM@@PEAUtagRECT@@H@Z
extern "C" int MS_ABI impl__GetItemRect_CTreeCtrl__QEBAHPEAU_TREEITEM__PEAUtagRECT__H_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, RECT* pRect, int bTextOnly) {
    if (!pRect) return FALSE;
    *(HTREEITEM*)pRect = hItem;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMRECT, bTextOnly ? TRUE : FALSE, (LPARAM)pRect);
}
// Symbol: ?GetItemState@CTreeCtrl@@QEBAIPEAU_TREEITEM@@I@Z
extern "C" UINT MS_ABI impl__GetItemState_CTreeCtrl__QEBAIPEAU_TREEITEM__I_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, UINT nStateMask) {
    return (UINT)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMSTATE, (WPARAM)hItem, nStateMask);
}
// Symbol: ?GetItemStateEx@CTreeCtrl@@QEBAIPEAU_TREEITEM@@@Z
extern "C" UINT MS_ABI impl__GetItemStateEx_CTreeCtrl__QEBAIPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMEXW item = {};
    return TreeCtrlGetItemEx(TreeCtrlHwnd(pThis), hItem, TVIF_STATEEX, &item) ? item.uStateEx : 0;
}
// Symbol: ?GetMessageMap@CTreeCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTreeCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CTreeCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CTreeCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTreeCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CTreeCtrl* pThis) {
    return CTreeCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CTreeCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTreeCtrl__SAPEAUCRuntimeClass__XZ() {
    return CTreeCtrl::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CTreeCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?HitTest@CTreeCtrl@@QEBAPEAU_TREEITEM@@VCPoint@@PEAI@Z
extern "C" HTREEITEM MS_ABI impl__HitTest_CTreeCtrl__QEBAPEAU_TREEITEM__VCPoint__PEAI_Z(
    const CTreeCtrl* pThis, CPoint pt, UINT* pFlags) {
    TVHITTESTINFO hit = {};
    hit.pt.x = pt.x;
    hit.pt.y = pt.y;
    HTREEITEM hItem = (HTREEITEM)::SendMessageW(TreeCtrlHwnd(pThis), TVM_HITTEST, 0, (LPARAM)&hit);
    if (pFlags) *pFlags = hit.flags;
    return hItem;
}
// Symbol: ?InsertItem@CTreeCtrl@@QEAAPEAU_TREEITEM@@IPEB_WHHII_JPEAU2@2@Z
extern "C" HTREEITEM MS_ABI impl__InsertItem_CTreeCtrl__QEAAPEAU_TREEITEM__IPEB_WHHII_JPEAU2_2_Z(
    CTreeCtrl* pThis,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam,
    HTREEITEM hParent,
    HTREEITEM hInsertAfter) {
    TVINSERTSTRUCTW insert = {};
    insert.hParent = hParent;
    insert.hInsertAfter = hInsertAfter;
    insert.item.mask = nMask;
    insert.item.pszText = const_cast<wchar_t*>(lpszItem);
    insert.item.iImage = nImage;
    insert.item.iSelectedImage = nSelectedImage;
    insert.item.state = nState;
    insert.item.stateMask = nStateMask;
    insert.item.lParam = lParam;
    return (HTREEITEM)::SendMessageW(TreeCtrlHwnd(pThis), TVM_INSERTITEMW, 0, (LPARAM)&insert);
}
// Symbol: ?ItemHasChildren@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__ItemHasChildren_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMW item = {};
    return TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_CHILDREN, &item) ? (item.cChildren != 0) : FALSE;
}
// Symbol: ?OnDestroy@CTreeCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTreeCtrl__QEAAXXZ(CTreeCtrl* pThis) {
    (void)pThis;
}
// Symbol: ?RemoveImageList@CTreeCtrl@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CTreeCtrl__IEAAXH_Z(CTreeCtrl* pThis, int nImageList) {
    ::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETIMAGELIST, nImageList, 0);
}
// Symbol: ?SetCheck@CTreeCtrl@@QEAAHPEAU_TREEITEM@@H@Z
extern "C" int MS_ABI impl__SetCheck_CTreeCtrl__QEAAHPEAU_TREEITEM__H_Z(
    CTreeCtrl* pThis, HTREEITEM hItem, int fCheck) {
    TVITEMW item = {};
    item.mask = TVIF_STATE;
    item.hItem = hItem;
    item.stateMask = TVIS_STATEIMAGEMASK;
    item.state = INDEXTOSTATEIMAGEMASK(fCheck ? 2 : 1);
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}
// Symbol: ?SetItem@CTreeCtrl@@QEAAHPEAU_TREEITEM@@IPEB_WHHII_J@Z
extern "C" int MS_ABI impl__SetItem_CTreeCtrl__QEAAHPEAU_TREEITEM__IPEB_WHHII_J_Z(
    CTreeCtrl* pThis,
    HTREEITEM hItem,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam) {
    TVITEMW item = {};
    item.mask = nMask;
    item.hItem = hItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.iSelectedImage = nSelectedImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}
// Symbol: ?SetItemEx@CTreeCtrl@@QEAAHPEAU_TREEITEM@@IPEB_WHHII_JIPEAUHWND__@@H@Z
extern "C" int MS_ABI impl__SetItemEx_CTreeCtrl__QEAAHPEAU_TREEITEM__IPEB_WHHII_JIPEAUHWND____H_Z(
    CTreeCtrl* pThis,
    HTREEITEM hItem,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam,
    UINT uStateEx,
    HWND hwnd,
    int iExpandedImage) {
    TVITEMEXW item = {};
    item.mask = nMask;
    item.hItem = hItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.iSelectedImage = nSelectedImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    item.uStateEx = uStateEx;
    item.hwnd = hwnd;
    item.iExpandedImage = iExpandedImage;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}
