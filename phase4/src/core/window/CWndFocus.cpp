// Focus, OLE dialog navigation, and the base tooltip-filter callback.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"
#include "detail/WindowMessageSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
// Symbol: ?SetFocus@CWnd@@QEAAPEAV1@XZ
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* self) {
    if(self->m_pCtrlSite) {
        using Focus=CWnd* (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Focus>(self->m_pCtrlSite,46)(self->m_pCtrlSite);
    }
    auto parent=[&] {return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(self->m_hWnd));};
    if(parent() && parent()->m_pCtrlCont) {
        void* container=parent()->m_pCtrlCont;
        *reinterpret_cast<void**>(static_cast<char*>(container)+0xd8)=nullptr;
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetFocus(self->m_hWnd));
}
// Symbol: ?IsDialogMessageW@CWnd@@QEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd* self,MSG* message) {
    if(self->m_nFlags&0x100) {
        void* manager=*reinterpret_cast<void**>(reinterpret_cast<char*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ())+0xa0);
        using Dialog=BOOL (MS_ABI*)(void*,CWnd*,MSG*);
        return openmfc::detail::WindowSlot<Dialog>(manager,10)(manager,self,message);
    }
    return ::IsDialogMessageW(self->m_hWnd,message);
}
// Symbol: ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* self,MSG* message) {
    if (auto* module = self->m_pModuleState) {
        if (auto filter = module->m_pfnFilterToolTipMessage) filter(message, self);
    }
    return FALSE;
}

BOOL CWnd::PreTranslateInput(MSG* message) {
    if (!message) return FALSE;
    const UINT id = message->message;
    const bool keyboard = id >= WM_KEYFIRST && id <= WM_KEYLAST;
    const bool mouse = id >= WM_MOUSEFIRST && id <= WM_MBUTTONDBLCLK;
    return (keyboard || mouse) ? IsDialogMessageW(message) : FALSE;
}

// Symbol: ?PreTranslateInput@CWnd@@QEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateInput_CWnd__QEAAHPEAUtagMSG___Z(CWnd* self, MSG* message) {
    return self->PreTranslateInput(message);
}

namespace {
bool IsComboBox(HWND window, DWORD type) {
    if (!window || (static_cast<DWORD>(::GetWindowLongW(window, GWL_STYLE)) & 0xf) != type) {
        return false;
    }
    wchar_t name[10]{};
    return ::GetClassNameW(window, name, _countof(name)) &&
           ::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, name, -1, L"ComboBox", -1) == CSTR_EQUAL;
}
}

// Symbol: ?AfxCancelModes@@YAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__AfxCancelModes__YAXPEAUHWND_____Z(HWND receiver) {
    HWND combo = ::GetFocus();
    if (!combo || combo == receiver) return;
    if (!IsComboBox(combo, CBS_DROPDOWNLIST)) {
        combo = ::GetParent(combo);
        if (combo == receiver || !IsComboBox(combo, CBS_DROPDOWN)) return;
    }
    if (receiver && (::GetWindowLongW(receiver, GWL_STYLE) & WS_CHILD) &&
        ::GetParent(receiver) == ::GetDesktopWindow()) return;
    ::SendMessageW(combo, CB_SHOWDROPDOWN, FALSE, 0);
}

CWnd* CWnd::SetFocus() { return impl__SetFocus_CWnd__QEAAPEAV1_XZ(this); }
BOOL CWnd::IsDialogMessageW(MSG* message) { return impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(this,message); }
