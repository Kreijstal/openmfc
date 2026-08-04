// CSpinButtonCtrl — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

// Symbol: ?GetRuntimeClass@CSpinButtonCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSpinButtonCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CSpinButtonCtrl* pThis) {
    return CSpinButtonCtrl::GetThisClass();
}
// Symbol: ?GetThisClass@CSpinButtonCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSpinButtonCtrl__SAPEAUCRuntimeClass__XZ() {
    return CSpinButtonCtrl::GetThisClass();
}
// Symbol: ?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CSpinButtonCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CSpinButtonCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?CreateEx@CSpinButtonCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CSpinButtonCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CSpinButtonCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CreateEx(dwExStyle, L"msctls_updown32", L"", dwStyle, pRect->left, pRect->top,
                           pRect->right - pRect->left, pRect->bottom - pRect->top,
                           pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}
// Symbol: ?GetRange@CSpinButtonCtrl@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__GetRange_CSpinButtonCtrl__QEBAXAEAH0_Z(
    const CSpinButtonCtrl* pThis, int* pLower, int* pUpper) {
    if (pLower) *pLower = 0;
    if (pUpper) *pUpper = 100;
    if (!pThis || !pThis->m_hWnd) return;

    DWORD dwRange = (DWORD)::SendMessageW(pThis->m_hWnd, UDM_GETRANGE, 0, 0);
    if (pLower) *pLower = (short)HIWORD(dwRange);
    if (pUpper) *pUpper = (short)LOWORD(dwRange);
}
