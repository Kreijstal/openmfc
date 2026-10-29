// CTabCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

extern "C" CImageList* MS_ABI impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(
    HIMAGELIST hImageList);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);

// Symbol: ?DrawItem@CTabCtrl@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CTabCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CTabCtrl* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}
// Symbol: ?GetItemState@CTabCtrl@@QEBAKHK@Z
extern "C" DWORD MS_ABI impl__GetItemState_CTabCtrl__QEBAKHK_Z(
    const CTabCtrl* pThis, int nItem, DWORD dwMask) {
    if (!pThis || !pThis->m_hWnd) return 0;
    TCITEMW item = {};
    item.mask = TCIF_STATE;
    item.dwStateMask = dwMask;
    return ::SendMessageW(pThis->m_hWnd, TCM_GETITEMW, nItem, (LPARAM)&item) ? item.dwState : 0;
}
// Symbol: ?GetMessageMap@CTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTabCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CTabCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CTabCtrl* pThis) {
    return CTabCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CTabCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabCtrl__SAPEAUCRuntimeClass__XZ() {
    return CTabCtrl::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTabCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?InsertItem@CTabCtrl@@QEAAJIHPEB_WH_J@Z
extern "C" __int64 MS_ABI impl__InsertItem_CTabCtrl__QEAAJIHPEB_WH_J_Z(
    CTabCtrl* pThis, int nItem, int nMask, const wchar_t* lpszItem, int nImage, LPARAM lParam) {
    if (!pThis || !pThis->m_hWnd) return -1;
    TCITEMW item = {};
    item.mask = (UINT)nMask;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.lParam = lParam;
    return (__int64)::SendMessageW(pThis->m_hWnd, TCM_INSERTITEMW, nItem, (LPARAM)&item);
}
// Symbol: ?InsertItem@CTabCtrl@@QEAAJIHPEB_WH_JKK@Z
extern "C" __int64 MS_ABI impl__InsertItem_CTabCtrl__QEAAJIHPEB_WH_JKK_Z(
    CTabCtrl* pThis,
    int nItem,
    int nMask,
    const wchar_t* lpszItem,
    int nImage,
    LPARAM lParam,
    DWORD dwState,
    DWORD dwStateMask) {
    if (!pThis || !pThis->m_hWnd) return -1;
    TCITEMW item = {};
    item.mask = (UINT)nMask;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.lParam = lParam;
    item.dwState = dwState;
    item.dwStateMask = dwStateMask;
    return (__int64)::SendMessageW(pThis->m_hWnd, TCM_INSERTITEMW, nItem, (LPARAM)&item);
}
// Symbol: ?OnChildNotify@CTabCtrl@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CTabCtrl__MEAAHI_K_JPEA_J_Z(
    CTabCtrl* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CTabCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}
// Retail RVA 0x2968e0 (mfc140u): retrieve the tab control's image list, clear
// the control's association only when FromHandlePermanent finds a permanent
// wrapper, then invoke CWnd::OnDestroy.
// Symbol: ?OnDestroy@CTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTabCtrl__IEAAXXZ(CTabCtrl* pThis) {
    HIMAGELIST hImageList = reinterpret_cast<HIMAGELIST>(
        ::SendMessage(pThis->m_hWnd, TCM_GETIMAGELIST, 0, 0));
    if (impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hImageList)) {
        ::SendMessage(pThis->m_hWnd, TCM_SETIMAGELIST, 0, 0);
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}
// Symbol: ?SetItemState@CTabCtrl@@QEAAHHKK@Z
extern "C" int MS_ABI impl__SetItemState_CTabCtrl__QEAAHHKK_Z(
    CTabCtrl* pThis, int nItem, DWORD dwMask, DWORD dwState) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    TCITEMW item = {};
    item.mask = TCIF_STATE;
    item.dwStateMask = dwMask;
    item.dwState = dwState;
    return (int)::SendMessageW(pThis->m_hWnd, TCM_SETITEMW, nItem, (LPARAM)&item);
}
// Symbol: ?CreateEx@CTabCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CTabCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CTabCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"SysTabControl32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
