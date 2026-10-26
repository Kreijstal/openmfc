// Native release-x64 window and frame traversal (mfc140u 0x28e200..0x28e4ea).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" {
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
}
namespace {
CWnd* Window(HWND handle) { return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(handle); }
BOOL Frame(const CWnd* window) {
    using Predicate=BOOL (MS_ABI*)(const CWnd*);
    return openmfc::detail::WindowSlot<Predicate>(const_cast<CWnd*>(window),86)(window);
}
CFrameWnd* ParentFrame(const CWnd* self) {
    if(!self || !self->m_hWnd)return nullptr;
    HWND parent=::GetParent(self->m_hWnd);
    while(CWnd* window=Window(parent)) {
        if(Frame(window))return reinterpret_cast<CFrameWnd*>(window);
        parent=::GetParent(window->m_hWnd);
    }
    return nullptr;
}
// MFC's HWND overload first honors the permanent wrapper's explicit owner.
HWND ParentOwner(HWND handle) {
    if(auto* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(handle)) {
        CWnd* parent=Window(window->m_hWndOwner ? window->m_hWndOwner : ::GetParent(window->m_hWnd));
        return parent ? parent->m_hWnd : nullptr;
    }
    return (::GetWindowLongW(handle,GWL_STYLE)&WS_CHILD) ? ::GetParent(handle) : ::GetWindow(handle,GW_OWNER);
}
}
// Symbol: ?AfxGetParentOwner@@YAPEAUHWND__@@PEAU1@@Z
extern "C" HWND MS_ABI impl__AfxGetParentOwner__YAPEAUHWND____PEAU1__Z(HWND handle) { return ParentOwner(handle); }
// Symbol: ?GetParentFrame@CWnd@@QEBAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* self) { return ParentFrame(self); }
// Symbol: ?GetTopLevelFrame@CWnd@@QEBAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* self) {
    if(!self || !self->m_hWnd)return nullptr;
    auto* frame=Frame(self) ? reinterpret_cast<CFrameWnd*>(const_cast<CWnd*>(self)) : ParentFrame(self);
    if(frame)while(auto* parent=ParentFrame(frame))frame=parent;
    return frame;
}
// Symbol: ?GetTopLevelParent@CWnd@@QEBAPEAV1@XZ
extern "C" CWnd* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd* self) {
    if(!self || !self->m_hWnd)return nullptr;
    HWND handle=self->m_hWnd;
    while(HWND parent=ParentOwner(handle))handle=parent;
    return Window(handle);
}
// Symbol: ?GetTopLevelOwner@CWnd@@QEBAPEAV1@XZ
extern "C" CWnd* MS_ABI impl__GetTopLevelOwner_CWnd__QEBAPEAV1_XZ(const CWnd* self) {
    if(!self || !self->m_hWnd)return nullptr;
    HWND handle=self->m_hWnd;
    while(HWND owner=::GetWindow(handle,GW_OWNER))handle=owner;
    return Window(handle);
}
// Symbol: ?GetParentOwner@CWnd@@QEBAPEAV1@XZ
extern "C" CWnd* MS_ABI impl__GetParentOwner_CWnd__QEBAPEAV1_XZ(const CWnd* self) {
    if(!self || !self->m_hWnd)return nullptr;
    HWND handle=self->m_hWnd;
    while(::GetWindowLongW(handle,GWL_STYLE)&WS_CHILD) {
        HWND parent=::GetParent(handle);if(!parent)break;handle=parent;
    }
    return Window(handle);
}
// Symbol: ?IsTopParentActive@CWnd@@QEBAHXZ
extern "C" BOOL MS_ABI impl__IsTopParentActive_CWnd__QEBAHXZ(const CWnd* self) {
    CWnd* top=impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(self);
    if(!top)impl__AfxThrowInvalidArgException__YAXXZ();
    CWnd* foreground=Window(::GetForegroundWindow());
    return foreground==Window(::GetLastActivePopup(top->m_hWnd));
}
CFrameWnd* CWnd::GetParentFrame() const { return impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(this); }
CFrameWnd* CWnd::GetTopLevelFrame() const { return impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(this); }
CWnd* CWnd::GetTopLevelParent() const { return impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(this); }
CWnd* CWnd::GetTopLevelOwner() const { return impl__GetTopLevelOwner_CWnd__QEBAPEAV1_XZ(this); }
CWnd* CWnd::GetParentOwner() const { return impl__GetParentOwner_CWnd__QEBAPEAV1_XZ(this); }
int CWnd::IsTopParentActive() const { return impl__IsTopParentActive_CWnd__QEBAHXZ(this); }
