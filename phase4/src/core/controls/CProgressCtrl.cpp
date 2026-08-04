// CProgressCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CProgressCtrl@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CProgressCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CProgressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CProgressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?CreateEx@CProgressCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CProgressCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CProgressCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_progress32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
// Symbol: ?GetRange@CProgressCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CProgressCtrl__QEBAXAEAH0_Z(
    const CProgressCtrl* pThis, int* pLower, int* pUpper) {
    if (pLower) *pLower = 0;
    if (pUpper) *pUpper = 0;
    if (!pThis || !pThis->m_hWnd) return;

    PBRANGE range = {};
    ::SendMessageW(pThis->m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&range);
    if (pLower) *pLower = range.iLow;
    if (pUpper) *pUpper = range.iHigh;
}
// Symbol: ?GetRuntimeClass@CProgressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CProgressCtrl__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@CProgressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CProgressCtrl__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
