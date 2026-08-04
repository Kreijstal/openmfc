// CPagerCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Create@CPagerCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CPagerCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, const void* pRect, void* pParentWnd, unsigned int nID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    auto* rect = static_cast<const RECT*>(pRect);
    if (rect == nullptr) {
        return FALSE;
    }

    HWND hParent = static_cast<CWnd*>(pParentWnd)->m_hWnd;
    if (!hParent) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        0, L"SysPager", nullptr,
        WS_CHILD | WS_VISIBLE,
        rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
        hParent, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (!hWnd) {
        return FALSE;
    }

    pWnd->Attach(hWnd);
    return TRUE;
}
// Symbol: ?CreateEx@CPagerCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CPagerCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, const void* pRect, void* pParentWnd, unsigned int nID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    auto* rect = static_cast<const RECT*>(pRect);
    if (rect == nullptr) {
        return FALSE;
    }

    HWND hParent = static_cast<CWnd*>(pParentWnd)->m_hWnd;
    if (!hParent) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        static_cast<DWORD>(dwExStyle),
        L"SysPager", nullptr,
        WS_CHILD | WS_VISIBLE,
        rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
        hParent, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (!hWnd) {
        return FALSE;
    }

    pWnd->Attach(hWnd);
    return TRUE;
}
// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
