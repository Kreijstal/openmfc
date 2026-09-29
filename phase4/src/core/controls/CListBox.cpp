// CListBox — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// CWnd::Default thunk, defined in core/window/Thunks.cpp:1183; it forwards to
// CWnd::Default() (core/window/CWnd.cpp). NOTE the OpenMFC Default() is not retail's:
// retail (0x28ac80, mfc140u) replays the thread state's m_lastSentMsg through the
// virtual DefWindowProc (vtable +0x248); OpenMFC has no m_lastSentMsg and instead
// replays CWinThread::m_msgCur (the last PUMPED message) straight into
// ::DefWindowProcW, returning 0 when m_hWnd is NULL.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);

// Symbol: ?CharToItem@CListBox@@UEAAHII@Z
// Retail RVA 0xda30 (mfc140u) is a single `jmp CWnd::Default` (0x28ac80, mfc140u); the
// linker folded CharToItem and VKeyToItem onto that one body (both exports resolve to
// 0xda30 in the mfc140u export table). Both arguments are ignored and the __int64
// LRESULT of Default() is truncated to int by the tail jump.
// Known deviation (in CWnd::Default, not here): during a reflected WM_CHARTOITEM,
// retail's Default() replays WM_CHARTOITEM itself to the listbox, which normally ends
// in DefWindowProc's -1 ("default action"). OpenMFC's Default() replays m_msgCur
// (typically the WM_CHAR/WM_KEYDOWN that triggered it) into ::DefWindowProcW, which
// returns 0 ("select item 0"). This body will become observably correct once
// CWnd::Default tracks the last sent message; the call itself is the retail spec.
extern "C" int MS_ABI impl__CharToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nChar, UINT nIndex) {
    (void)nChar;
    (void)nIndex;
    return (int)impl__Default_CWnd__IEAA_JXZ(pThis);
}
// Symbol: ?CompareItem@CListBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
// Retail-trivial, verified: the export resolves to RVA 0x71e0 (mfc140u), a shared
// COMDAT-folded body `xor %eax,%eax; ret` (also CComboBox::CompareItem and many others).
// So the base implementation returns 0 and ignores this and pCompare; this body matches.
extern "C" int MS_ABI impl__CompareItem_CListBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CListBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;
    (void)pCompare;
    return 0;
}
// Symbol: ?DeleteItem@CListBox@@UEAAXPEAUtagDELETEITEMSTRUCT@@@Z
// Retail-trivial, verified: the export resolves to RVA 0x27d0 (mfc140u), a shared
// COMDAT-folded body that is a bare `ret`. The base implementation does nothing; this
// empty body matches.
extern "C" void MS_ABI impl__DeleteItem_CListBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(
    CListBox* pThis, DELETEITEMSTRUCT* pDelete) {
    (void)pThis;
    (void)pDelete;
}
// Symbol: ?DrawItem@CListBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CListBox* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}
// Symbol: ?GetRuntimeClass@CListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CListBox__UEBAPEAUCRuntimeClass__XZ(const CListBox* pThis) {
    return CListBox::GetThisClass();
}
// Symbol: ?GetThisClass@CListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListBox__SAPEAUCRuntimeClass__XZ() {
    return CListBox::GetThisClass();
}
// Symbol: ?ItemFromPoint@CListBox@@QEBAIVCPoint@@AEAH@Z
extern "C" UINT MS_ABI impl__ItemFromPoint_CListBox__QEBAIVCPoint__AEAH_Z(
    const CListBox* pThis, CPoint pt, int* pOutside) {
    return (UINT)ListBoxItemFromPoint_CtrlOwnerdraw(pThis ? pThis->m_hWnd : nullptr, pt, pOutside);
}
// Symbol: ?MeasureItem@CListBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    CListBox* pThis, MEASUREITEMSTRUCT* pMeasure) {
    (void)pThis;
    if (pMeasure && pMeasure->itemHeight == 0) pMeasure->itemHeight = 16;
}
// Symbol: ?OnChildNotify@CListBox@@MEAAHI_K_JPEA_J@Z
// NOT a retail transcription. Retail RVA 0x293f60 (mfc140u) dispatches through the
// vtable (DrawItem +0x2e0, MeasureItem +0x2e8, CompareItem +0x2f0, DeleteItem +0x2f8,
// VKeyToItem +0x300, CharToItem +0x308), never pre-zeroes *pResult (writes it, sign-
// extended, only for Compare/VKey/CharToItem), has no lParam NULL checks, returns TRUE
// for all six messages and tail-jumps to CWnd::OnChildNotify (0x28f0d0, mfc140u) for
// anything else. This body calls the CListBox thunks directly (derived overrides are
// never reached) and returns FALSE for other messages.
extern "C" int MS_ABI impl__OnChildNotify_CListBox__MEAAHI_K_JPEA_J_Z(
    CListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) *pResult = 0;
    switch (message) {
    case WM_DRAWITEM:
        if (!lParam) return FALSE;
        impl__DrawItem_CListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_MEASUREITEM:
        if (!lParam) return FALSE;
        impl__MeasureItem_CListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(pThis, (MEASUREITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_COMPAREITEM:
        if (!lParam) return FALSE;
        if (pResult) *pResult = impl__CompareItem_CListBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(pThis, (COMPAREITEMSTRUCT*)lParam);
        return TRUE;
    case WM_DELETEITEM:
        if (!lParam) return FALSE;
        impl__DeleteItem_CListBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(pThis, (DELETEITEMSTRUCT*)lParam);
        return TRUE;
    case WM_CHARTOITEM:
        if (pResult) *pResult = impl__CharToItem_CListBox__UEAAHII_Z(pThis, (UINT)LOWORD(wParam), (UINT)HIWORD(wParam));
        return TRUE;
    case WM_VKEYTOITEM:
        if (pResult) *pResult = impl__VKeyToItem_CListBox__UEAAHII_Z(pThis, (UINT)LOWORD(wParam), (UINT)HIWORD(wParam));
        return TRUE;
    default:
        return FALSE;
    }
}
// Symbol: ?VKeyToItem@CListBox@@UEAAHII@Z
// Same retail body as CharToItem: RVA 0xda30 (mfc140u), `jmp CWnd::Default` (0x28ac80,
// mfc140u). Arguments ignored; Default()'s LRESULT truncated to int. Same CWnd::Default
// deviation as noted on CharToItem (WM_VKEYTOITEM is not what OpenMFC replays).
extern "C" int MS_ABI impl__VKeyToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nKey, UINT nIndex) {
    (void)nKey;
    (void)nIndex;
    return (int)impl__Default_CWnd__IEAA_JXZ(pThis);
}
// Symbol: ?GetText@CListBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CListBox* pThis, int nIndex, CString* rString) {
    if (!rString) {
        return;
    }
    rString->Empty();
    int len = impl__GetTextLen_CListBox__QEBAHH_Z(pThis, nIndex);
    if (len <= 0) {
        return;
    }
    wchar_t* buffer = rString->GetBuffer(len + 1);
    int copied = impl__GetText_CListBox__QEBAHPEA_WH_Z(pThis, nIndex, buffer);
    rString->ReleaseBuffer(copied > 0 ? copied : 0);
}
