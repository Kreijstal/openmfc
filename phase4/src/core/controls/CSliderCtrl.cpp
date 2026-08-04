// CSliderCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// Symbol: ?GetRuntimeClass@CSliderCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSliderCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CSliderCtrl* pThis) {
    return CSliderCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CSliderCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSliderCtrl__SAPEAUCRuntimeClass__XZ() {
    return CSliderCtrl::GetThisClass();
}
// Symbol: ?CreateEx@CSliderCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CSliderCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CSliderCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_trackbar32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
// Symbol: ?GetRange@CSliderCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CSliderCtrl__QEBAXAEAH0_Z(
    const CSliderCtrl* pThis, int* pMin, int* pMax) {
    if (pMin) *pMin = 0;
    if (pMax) *pMax = 0;
    if (!pThis || !pThis->m_hWnd) return;
    if (pMin) *pMin = (int)::SendMessageW(pThis->m_hWnd, TBM_GETRANGEMIN, 0, 0);
    if (pMax) *pMax = (int)::SendMessageW(pThis->m_hWnd, TBM_GETRANGEMAX, 0, 0);
}
// Symbol: ?SetRange@CSliderCtrl@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetRange_CSliderCtrl__QEAAXHHH_Z(
    CSliderCtrl* pThis, int nMin, int nMax, int bRedraw) {
    if (!pThis || !pThis->m_hWnd) return;
    ::SendMessageW(pThis->m_hWnd, TBM_SETRANGE, bRedraw ? TRUE : FALSE, MAKELPARAM(nMin, nMax));
}
// Symbol: ?GetSelection@CSliderCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetSelection_CSliderCtrl__QEBAXAEAH0_Z(
    const CSliderCtrl* pThis, int* pMinSel, int* pMaxSel) {
    if (pMinSel) *pMinSel = 0;
    if (pMaxSel) *pMaxSel = 0;
    if (!pThis || !pThis->m_hWnd) return;
    if (pMinSel) *pMinSel = (int)::SendMessageW(pThis->m_hWnd, TBM_GETSELSTART, 0, 0);
    if (pMaxSel) *pMaxSel = (int)::SendMessageW(pThis->m_hWnd, TBM_GETSELEND, 0, 0);
}
// Symbol: ?SetSelection@CSliderCtrl@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetSelection_CSliderCtrl__QEAAXHH_Z(
    CSliderCtrl* pThis, int nMinSel, int nMaxSel) {
    if (!pThis || !pThis->m_hWnd) return;
    ::SendMessageW(pThis->m_hWnd, TBM_SETSELSTART, FALSE, nMinSel);
    ::SendMessageW(pThis->m_hWnd, TBM_SETSELEND, FALSE, nMaxSel);
}
