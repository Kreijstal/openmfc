#include "CMFCToolBarButtonSupport.h"

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void tbb_Serialize(CMFCToolBarButton* b, CArchive* par) {
    CArchive& ar = *par;
    if (ar.IsStoring()) {
        ar << b->m_nID;
        ar << b->m_nStyle;
        ar << (int)b->GetImage();          // active image (user->iUserImage else iImage)
        ar << b->m_strText;
        ar << (UINT)b->m_bUserButton;
        ar << (UINT)0;                      // reserved DWORD, always 0 for base button
        ar << (UINT)b->m_bText;
        ar << (UINT)b->m_bImage;
        ar << (UINT)b->m_bVisible;
    } else {
        int img; UINT u, reserved;
        ar >> b->m_nID;
        ar >> b->m_nStyle;
        ar >> img;
        ar >> b->m_strText;
        ar >> u; b->m_bUserButton = u;
        ar >> reserved;
        ar >> u; b->m_bText = u;
        ar >> u; b->m_bImage = u;
        ar >> u; b->m_bVisible = u;
        if (b->m_bUserButton) b->m_iUserImage = img; else b->m_iImage = img;
    }
}
void tbb_CopyFrom(CMFCToolBarButton* d, const CMFCToolBarButton* s) {
    d->m_nID = s->m_nID;
    d->m_nStyle = s->m_nStyle;
    d->m_dwdItemData = s->m_dwdItemData;
    d->m_strText = s->m_strText;
    d->m_strTextCustom = s->m_strTextCustom;
    d->m_bUserButton = s->m_bUserButton;
    d->m_bText = s->m_bText;
    d->m_bImage = s->m_bImage;
    d->m_bWrap = s->m_bWrap;
    d->m_bWholeText = s->m_bWholeText;
    d->m_bTextBelow = s->m_bTextBelow;
    d->m_bDragFromCollection = s->m_bDragFromCollection;
    d->m_iImage = s->m_iImage;
    d->m_iUserImage = s->m_iUserImage;
    d->m_bLocked = s->m_bLocked;
    d->m_bIsHidden = s->m_bIsHidden;
    d->m_bDisableFill = s->m_bDisableFill;
    d->m_bExtraSize = s->m_bExtraSize;
    d->m_bHorz = s->m_bHorz;
    d->m_bVisible = s->m_bVisible;
}
int  tbb_CompareWith(const CMFCToolBarButton* a, const CMFCToolBarButton* b) {
    return a->m_nID == b->m_nID ? TRUE : FALSE;
}
void tbb_SetImage(CMFCToolBarButton* b, int iImage) {
    if (b->m_bUserButton) b->m_iUserImage = iImage; else b->m_iImage = iImage;
}
CRuntimeClass* MS_ABI v00_GetRuntimeClass(const void* p) { return impl__GetRuntimeClass_CMFCToolBarButton__UEBAPEAUCRuntimeClass__XZ(p); }
void* MS_ABI v01_dtor(CMFCToolBarButton* p, unsigned int flags) { p->CMFCToolBarButton::~CMFCToolBarButton(); if (flags & 1) ::operator delete(p); return p; }
void  MS_ABI v02_Serialize(CMFCToolBarButton* p, CArchive* ar) { tbb_Serialize(p, ar); }
void  MS_ABI v03_AssertValid(const void* pThis) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void  MS_ABI v04_Dump(const void* pThis, void*) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
BOOL  MS_ABI v05_PrepareDrag(CMFCToolBarButton* p, void*) {
    return (p && p->m_bVisible && !p->m_bIsHidden && !p->m_bLocked) ? TRUE : FALSE;
}
BOOL  MS_ABI v06_CanBeDropped(CMFCToolBarButton* p, void*) {
    return (p && p->m_nID != 0 && !p->m_bLocked) ? TRUE : FALSE;
}
void  MS_ABI v07_CopyFrom(CMFCToolBarButton* p, const CMFCToolBarButton* s) { tbb_CopyFrom(p, s); }
void  MS_ABI v08_OnDraw(void* pThis, void* pDC, const void* pRect, void* pImageList, BOOL bHighlight, BOOL bPushed, BOOL bChecked, BOOL bDisabled, BOOL bFocused) {
    if (!pThis) return;
    auto* p = static_cast<CMFCToolBarButton*>(pThis);
    if (!p || p->m_bIsHidden || !p->m_bVisible) return;
    (void)pDC;
    (void)pRect;
    (void)pImageList;
    (void)bHighlight;
    (void)bPushed;
    (void)bChecked;
    (void)bDisabled;
    (void)bFocused;
}
SIZE  MS_ABI v09_OnCalculateSize(void*, void*, const SIZE* sizeDefault, BOOL) { return sizeDefault ? *sizeDefault : SIZE{0,0}; }
BOOL  MS_ABI v10_OnClick(CMFCToolBarButton* p, void*, BOOL) {
    return (p && p->m_bVisible && !p->m_bIsHidden && !p->m_bLocked && p->m_nID != 0) ? TRUE : FALSE;
}
BOOL  MS_ABI v11_OnClickUp(CMFCToolBarButton* p) {
    return (p && p->m_bVisible && !p->m_bIsHidden && !p->m_bLocked) ? TRUE : FALSE;
}
void  MS_ABI v12_OnChangeParentWnd(CMFCToolBarButton* p, CWnd* pWndParent) { p->m_pWndParent = pWndParent; }
BOOL  MS_ABI v13_ExportToMenuButton(CMFCToolBarButton* p, void*) { return (p && p->m_nID != 0) ? TRUE : FALSE; }
void  MS_ABI v14_OnMove(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* p = static_cast<CMFCToolBarButton*>(pThis);
    if (p->m_pWndParent) {
        ::InvalidateRect(p->m_pWndParent->GetSafeHwnd(), nullptr, TRUE);
    }
}
void  MS_ABI v15_OnSize(void* pThis, int nSize) {
    if (pThis) static_cast<CMFCToolBarButton*>(pThis)->m_bExtraSize = nSize != 0;
}
HWND  MS_ABI v16_GetHwnd(CMFCToolBarButton* p) {
    return p && p->m_pWndParent ? p->m_pWndParent->GetSafeHwnd() : NULL;
}
BOOL  MS_ABI v17_CanBeStretched(CMFCToolBarButton* p) {
    return (p && !p->m_bLocked && p->m_bImage) ? TRUE : FALSE;
}
BOOL  MS_ABI v18_NotifyCommand(CMFCToolBarButton* p, int nID) {
    if (!p || !p->m_pWndParent) return FALSE;
    HWND hWnd = p->m_pWndParent->GetSafeHwnd();
    if (!hWnd) return FALSE;
    UINT cmd = nID ? static_cast<UINT>(nID) : p->m_nID;
    return ::PostMessageW(hWnd, WM_COMMAND, static_cast<WPARAM>(cmd), 0);
}
void  MS_ABI v19_OnAddToCustomizePage(void* pThis) {
    if (!pThis) return;
    static_cast<CMFCToolBarButton*>(pThis)->m_bVisible = TRUE;
}
HBRUSH MS_ABI v20_OnCtlColor(void*, void*, UINT nCtlType) {
    return nCtlType == WM_CTLCOLOREDIT ? (HBRUSH)::GetSysColorBrush(COLOR_WINDOW) : (HBRUSH)::GetSysColorBrush(COLOR_3DFACE);
}
void  MS_ABI v21_OnDblClk(void* pThis, void*) { if (pThis) v10_OnClick(static_cast<CMFCToolBarButton*>(pThis), nullptr, TRUE); }
BOOL  MS_ABI v22_CanBeStored(CMFCToolBarButton* p) { return (p && p->m_nID != 0) ? TRUE : FALSE; }
BOOL  MS_ABI v23_HaveHotBorder(const CMFCToolBarButton* p) { return p ? !p->m_bDisableFill : TRUE; }
void  MS_ABI v24_OnCancelMode(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* p = static_cast<CMFCToolBarButton*>(pThis);
    p->m_bLocked = FALSE;
    if (p->m_pWndParent) {
        ::InvalidateRect(p->m_pWndParent->GetSafeHwnd(), nullptr, TRUE);
    }
}
void  MS_ABI v25_OnGlobalFontsChanged(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* p = static_cast<CMFCToolBarButton*>(pThis);
    p->m_bText = TRUE;
}
BOOL  MS_ABI v26_IsEditable(const CMFCToolBarButton* p) { return p ? !p->m_bLocked : TRUE; }
BOOL  MS_ABI v27_OnContextHelp(CMFCToolBarButton* p, void*) { return p ? (p->m_nID != 0) : FALSE; }
BOOL  MS_ABI v28_OnCustomizeMenu(CMFCToolBarButton* p, void*) { return p ? (p->m_nID != 0) : FALSE; }
int   MS_ABI v29_OnDrawOnCustomizeList(CMFCToolBarButton* p, void*, const void*, BOOL) { return (p && !p->m_bIsHidden) ? 1 : 0; }
BOOL  MS_ABI v30_IsDroppedDown(const CMFCToolBarButton* p) { return p ? ((p->m_nStyle & 0x0001) != 0) : FALSE; }
BOOL  MS_ABI v31_OnBeforeDrag(const CMFCToolBarButton* p) { return (p && !p->m_bLocked) ? TRUE : FALSE; }
BOOL  MS_ABI v32_OnBeforeDrop(CMFCToolBarButton* p, void*) { return (p && !p->m_bIsHidden) ? TRUE : FALSE; }
BOOL  MS_ABI v33_OnToolHitTest(CMFCToolBarButton* p, const void*, void*) { return p ? static_cast<BOOL>(p->m_nID) : FALSE; }
void  MS_ABI v34_SaveBarState(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* p = static_cast<CMFCToolBarButton*>(pThis);
    if (p->m_pWndParent) {
        ::InvalidateRect(p->m_pWndParent->GetSafeHwnd(), nullptr, TRUE);
    }
}
void  MS_ABI v35_OnShow(CMFCToolBarButton* p, BOOL bShow) { if (p) p->m_bVisible = bShow; }
CRect MS_ABI v36_GetInvalidateRect(const CMFCToolBarButton* p) { return p->m_rect; }
void  MS_ABI v37_SetStyle(CMFCToolBarButton* p, UINT nStyle) { p->m_nStyle = nStyle; }
void  MS_ABI v38_ResetImageToDefault(void*) { /* base class: no-op (harvested) */ }
int   MS_ABI v39_CompareWith(const CMFCToolBarButton* p, const CMFCToolBarButton* o) { return tbb_CompareWith(p, o); }
void  MS_ABI v40_EnableWindow(CMFCToolBarButton* p, BOOL bEnable) { if (p) p->m_bVisible = bEnable; }
BOOL  MS_ABI v41_IsWindowVisible(CMFCToolBarButton* p) {
    return (p && p->m_bVisible && !p->m_bIsHidden &&
            (!p->m_pWndParent || ::IsWindowVisible(p->m_pWndParent->GetSafeHwnd()))) ? TRUE : FALSE;
}
BOOL  MS_ABI v42_IsOwnerOf(CMFCToolBarButton* p, HWND hwnd) {
    return p && p->m_pWndParent && (p->m_pWndParent->GetSafeHwnd() == hwnd) ? TRUE : FALSE;
}
BOOL  MS_ABI v43_HasFocus(const CMFCToolBarButton* p) {
    return p && p->m_pWndParent && (::GetFocus() == p->m_pWndParent->GetSafeHwnd()) ? TRUE : FALSE;
}
BOOL  MS_ABI v44_OnGetCustomToolTipText(CMFCToolBarButton* p, void*) {
    return (p && (!p->m_strText.IsEmpty() || p->m_bText || p->m_bImage)) ? TRUE : FALSE;
}
BOOL  MS_ABI v45_OnUpdateToolTip(CMFCToolBarButton* p, void*, int, void*, void*) {
    return (p && !p->m_bIsHidden) ? TRUE : FALSE;
}
BOOL  MS_ABI v46_IsFirstInGroup(CMFCToolBarButton* p) { return (p && (p->m_nStyle & 0x40000000u)) ? TRUE : FALSE; }
BOOL  MS_ABI v47_IsLastInGroup(CMFCToolBarButton* p) { return (p && (p->m_nStyle & 0x20000000u)) ? TRUE : FALSE; }
BOOL  MS_ABI v48_SetACCData(CMFCToolBarButton* p, void*, void*) { return p ? TRUE : FALSE; }
int   MS_ABI v49_GetAccCount(CMFCToolBarButton* p) { return p ? (p->m_bText ? 1 : 0) : 0; }
void  MS_ABI v50_SetImage(CMFCToolBarButton* p, int iImage) { tbb_SetImage(p, iImage); }
void  MS_ABI v51_SetRadio(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* p = static_cast<CMFCToolBarButton*>(pThis);
    p->m_bUserButton = FALSE;
    p->m_bImage = TRUE;
}
BOOL  MS_ABI v52_IsExtraSize(const CMFCToolBarButton* p) { return p->m_bExtraSize; }
void* const g_CMFCToolBarButton_msvtbl[53] = {
    (void*)v00_GetRuntimeClass,   (void*)v01_dtor,               (void*)v02_Serialize,          (void*)v03_AssertValid,
    (void*)v04_Dump,              (void*)v05_PrepareDrag,        (void*)v06_CanBeDropped,       (void*)v07_CopyFrom,
    (void*)v08_OnDraw,            (void*)v09_OnCalculateSize,    (void*)v10_OnClick,            (void*)v11_OnClickUp,
    (void*)v12_OnChangeParentWnd, (void*)v13_ExportToMenuButton, (void*)v14_OnMove,             (void*)v15_OnSize,
    (void*)v16_GetHwnd,           (void*)v17_CanBeStretched,     (void*)v18_NotifyCommand,      (void*)v19_OnAddToCustomizePage,
    (void*)v20_OnCtlColor,        (void*)v21_OnDblClk,           (void*)v22_CanBeStored,        (void*)v23_HaveHotBorder,
    (void*)v24_OnCancelMode,      (void*)v25_OnGlobalFontsChanged,(void*)v26_IsEditable,        (void*)v27_OnContextHelp,
    (void*)v28_OnCustomizeMenu,   (void*)v29_OnDrawOnCustomizeList,(void*)v30_IsDroppedDown,    (void*)v31_OnBeforeDrag,
    (void*)v32_OnBeforeDrop,      (void*)v33_OnToolHitTest,      (void*)v34_SaveBarState,       (void*)v35_OnShow,
    (void*)v36_GetInvalidateRect, (void*)v37_SetStyle,           (void*)v38_ResetImageToDefault,(void*)v39_CompareWith,
    (void*)v40_EnableWindow,      (void*)v41_IsWindowVisible,    (void*)v42_IsOwnerOf,          (void*)v43_HasFocus,
    (void*)v44_OnGetCustomToolTipText,(void*)v45_OnUpdateToolTip,(void*)v46_IsFirstInGroup,     (void*)v47_IsLastInGroup,
    (void*)v48_SetACCData,        (void*)v49_GetAccCount,        (void*)v50_SetImage,           (void*)v51_SetRadio,
    (void*)v52_IsExtraSize,
};
extern "C" void* OpenMFC_PatchToolBarButtonVtable(void* pObj) {
    if (pObj) *(const void**)pObj = (const void*)&g_CMFCToolBarButton_msvtbl[0];
    return pObj;
}
extern "C" const void* OpenMFC_ToolBarButtonVtableAddr() { return (const void*)&g_CMFCToolBarButton_msvtbl[0]; }
} } }  // namespace openmfc::detail::cmfctoolbarbutton
