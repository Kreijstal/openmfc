// Native rectangle conversion, including right-to-left layout and OLE styles.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
DWORD CWnd::GetStyle() const {
    if(m_pCtrlSite) {
        using Style=DWORD (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Style>(m_pCtrlSite,32)(m_pCtrlSite);
    }
    return static_cast<DWORD>(::GetWindowLongW(m_hWnd,GWL_STYLE));
}
int CWnd::IsWindowEnabled() const {
    if(m_pCtrlSite) {
        using Enabled=BOOL (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Enabled>(m_pCtrlSite,43)(m_pCtrlSite);
    }
    return ::IsWindowEnabled(m_hWnd);
}
int CWnd::EnableWindow(int enable) {
    if(m_pCtrlSite) {
        using Enable=BOOL (MS_ABI*)(void*,BOOL);
        return openmfc::detail::WindowSlot<Enable>(m_pCtrlSite,44)(m_pCtrlSite,enable);
    }
    return ::EnableWindow(m_hWnd,enable);
}
DWORD CWnd::GetExStyle() const {
    if(m_pCtrlSite) {
        using Style=DWORD (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Style>(m_pCtrlSite,33)(m_pCtrlSite);
    }
    return static_cast<DWORD>(::GetWindowLongW(m_hWnd,GWL_EXSTYLE));
}
void CWnd::ClientToScreen(RECT* rectangle) const {
    auto* first=reinterpret_cast<POINT*>(rectangle);
    auto* second=first+1;
    ::ClientToScreen(m_hWnd,first);
    ::ClientToScreen(m_hWnd,second);
    if(GetExStyle()&WS_EX_LAYOUTRTL) {LONG old=rectangle->left;rectangle->left=rectangle->right;rectangle->right=old;}
}
void CWnd::ScreenToClient(RECT* rectangle) const {
    auto* first=reinterpret_cast<POINT*>(rectangle);
    ::ScreenToClient(m_hWnd,first);
    ::ScreenToClient(m_hWnd,first+1);
    if(GetExStyle()&WS_EX_LAYOUTRTL) {LONG old=rectangle->left;rectangle->left=rectangle->right;rectangle->right=old;}
}
// Symbol: ?GetStyle@CWnd@@QEBAKXZ
extern "C" DWORD MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* self) {return self->GetStyle();}
// Symbol: ?IsWindowEnabled@CWnd@@QEBAHXZ
extern "C" BOOL MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* self) {return self->IsWindowEnabled();}
// Symbol: ?EnableWindow@CWnd@@QEAAHH@Z
extern "C" BOOL MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* self,BOOL enable) {return self->EnableWindow(enable);}
// Symbol: ?GetExStyle@CWnd@@QEBAKXZ
extern "C" DWORD MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* self) {return self->GetExStyle();}
// Symbol: ?ClientToScreen@CWnd@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* self,RECT* rectangle) {self->ClientToScreen(rectangle);}
// Symbol: ?ScreenToClient@CWnd@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* self,RECT* rectangle) {self->ScreenToClient(rectangle);}
