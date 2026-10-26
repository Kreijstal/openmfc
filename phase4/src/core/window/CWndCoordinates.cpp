// Native rectangle conversion, including right-to-left layout and OLE styles.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
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
// Symbol: ?GetExStyle@CWnd@@QEBAKXZ
extern "C" DWORD MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* self) {return self->GetExStyle();}
// Symbol: ?ClientToScreen@CWnd@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* self,RECT* rectangle) {self->ClientToScreen(rectangle);}
