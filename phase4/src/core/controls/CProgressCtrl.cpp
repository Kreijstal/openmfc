// CProgressCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CreateEx@CProgressCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CProgressCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CProgressCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_progress32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
// Retail mfc140u RVA 0x29c3f0 sends PBM_GETRANGE with wParam 0 and copies both limits.
// Symbol: ?GetRange@CProgressCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CProgressCtrl__QEBAXAEAH0_Z(
    const CProgressCtrl* pThis, int* pLower, int* pUpper) {
    PBRANGE range;
    ::SendMessage(pThis->m_hWnd, PBM_GETRANGE, 0, (LPARAM)&range);
    *pLower = range.iLow;
    *pUpper = range.iHigh;
}
// Retail mfc140u RVA 0x296f50 returns the fixed class descriptor (LEA + RET).
// Symbol: ?GetRuntimeClass@CProgressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CProgressCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CProgressCtrl* pThis) {
    (void)pThis;
    return &CProgressCtrl::classCProgressCtrl;
}
// mfc140.dll (ANSI twin) RVA 0x294ff0 is LEA of the class descriptor followed by RET;
// the mfc140u symbol is not resolved by the supplied --u map.
// Symbol: ?GetThisClass@CProgressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CProgressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &CProgressCtrl::classCProgressCtrl;
}
