// CHotKeyCtrl — OpenMFC implementation.
// Sources: ctrlcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?GetKeyName@CHotKeyCtrl@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IH@Z
extern "C" void MS_ABI impl__GetKeyName_CHotKeyCtrl__SA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IH_Z(
    CString* pRet, unsigned int vk, int extended) {
    new (pRet) CString(OpenMfcKeyName(vk, extended));
}
// Symbol: ?GetHotKeyName@CHotKeyCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetHotKeyName_CHotKeyCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CWnd* pThis) {
    CString result;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (hwnd) {
        DWORD hotKey = static_cast<DWORD>(::SendMessageW(hwnd, HKM_GETHOTKEY, 0, 0));
        UINT vk = LOBYTE(LOWORD(hotKey));
        UINT modifiers = HIBYTE(LOWORD(hotKey));
        if (modifiers & HOTKEYF_CONTROL) result += L"Ctrl+";
        if (modifiers & HOTKEYF_SHIFT) result += L"Shift+";
        if (modifiers & HOTKEYF_ALT) result += L"Alt+";
        result += OpenMfcKeyName(vk, (modifiers & HOTKEYF_EXT) != 0);
    }
    new (pRet) CString(result);
}
// Symbol: ??1CHotKeyCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CHotKeyCtrl__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCHotKeyCtrlState(pThis);
    }
}
// Symbol: ?Create@CHotKeyCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID);
// Symbol: ?CreateEx@CHotKeyCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID) {
    if (pThis == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    const RECT* rect = static_cast<const RECT*>(pRect);
    HWND parentWnd = static_cast<CWnd*>(pParentWnd)->GetSafeHwnd();
    if (parentWnd == nullptr) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        dwExStyle, HOTKEY_CLASSW, nullptr,
        nStyles | WS_CHILD,
        rect->left, rect->top,
        rect->right - rect->left,
        rect->bottom - rect->top,
        parentWnd, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (hWnd == nullptr) {
        return FALSE;
    }

    static_cast<CWnd*>(pThis)->m_hWnd = hWnd;
    auto& state = EnsureCHotKeyCtrlState(pThis);
    state.m_virtualKey = 0;
    state.m_modifierFlags = 0;
    return TRUE;
}
// Symbol: ?GetHotKey@CHotKeyCtrl@@QEBAXAEAG0@Z
extern "C" void MS_ABI impl__GetHotKey_CHotKeyCtrl__QEBAXAEAG0_Z(
    const void* pThis, unsigned short* pVirtualKey, unsigned short* pModifierFlags) {
    if (pThis == nullptr) {
        if (pVirtualKey != nullptr) {
            *pVirtualKey = 0;
        }
        if (pModifierFlags != nullptr) {
            *pModifierFlags = 0;
        }
        return;
    }

    auto& state = EnsureCHotKeyCtrlState(const_cast<void*>(pThis));
    const auto* pWnd = static_cast<const CWnd*>(pThis);
    if (pWnd->GetSafeHwnd() == nullptr) {
        if (pVirtualKey != nullptr) {
            *pVirtualKey = state.m_virtualKey;
        }
        if (pModifierFlags != nullptr) {
            *pModifierFlags = state.m_modifierFlags;
        }
        return;
    }

    const LRESULT hotkey = ::SendMessageW(pWnd->GetSafeHwnd(), HKM_GETHOTKEY, 0, 0);
    const unsigned short virtualKey = static_cast<unsigned short>(LOWORD(static_cast<unsigned long>(hotkey)));
    const unsigned short modifierFlags = static_cast<unsigned short>(HIWORD(static_cast<unsigned long>(hotkey)));
    state.m_virtualKey = virtualKey;
    state.m_modifierFlags = modifierFlags;
    if (pVirtualKey != nullptr) {
        *pVirtualKey = virtualKey;
    }
    if (pModifierFlags != nullptr) {
        *pModifierFlags = modifierFlags;
    }
}
