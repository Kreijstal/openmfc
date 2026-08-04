// CMFCToolBarDateTimeCtrlImpl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?OnDateTimeChange@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeChange_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}
// Symbol: ?OnDateTimeCloseUp@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeCloseUp_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}
// Symbol: ?OnDateTimeDropDown@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeDropDown_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}
// Symbol: ?GetMessageMap@CMFCToolBarDateTimeCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetMessageMap_CMFCToolBarDateTimeCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisMessageMap@CMFCToolBarDateTimeCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetThisMessageMap_CMFCToolBarDateTimeCtrlImpl__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}
