// Native menu visibility policy and delayed menu restoration live in the frame.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
DWORD CFrameWnd::GetMenuBarVisibility() const {return m_retailTail.m_dwMenuBarVisibility;}
DWORD CFrameWnd::GetMenuBarState() const {return m_retailTail.m_dwMenuBarState;}
void CFrameWnd::SetMenuBarVisibility(DWORD style) {
    if(style!=1 && style!=2 && style!=6) {impl__AfxThrowInvalidArgException__YAXXZ();__assume(0); }
    if(m_retailTail.m_dwMenuBarVisibility!=style) {
        m_retailTail.m_dwMenuBarVisibility=style;
        SetMenuBarState(style==1 ? 1:2);
    }
}
BOOL CFrameWnd::SetMenuBarState(DWORD state) {
    if(state!=1 && state!=2) {impl__AfxThrowInvalidArgException__YAXXZ();__assume(0); }
    if(m_retailTail.m_dwMenuBarState==state)return FALSE;
    HMENU menu;
    if(state==1) {
        OnShowMenuBar();
        menu=m_retailTail.m_hMenu;
    } else {
        m_retailTail.m_hMenu=::GetMenu(m_hWnd);
        OnHideMenuBar();
        menu=nullptr;
    }
    ::SetMenu(m_hWnd,menu);
    m_retailTail.m_dwMenuBarState=state;
    return TRUE;
}
// Symbol: ?GetMenuBarVisibility@CFrameWnd@@UEBAKXZ
extern "C" DWORD MS_ABI impl__GetMenuBarVisibility_CFrameWnd__UEBAKXZ(const CFrameWnd* self) {return self->CFrameWnd::GetMenuBarVisibility();}
// Symbol: ?GetMenuBarState@CFrameWnd@@UEBAKXZ
extern "C" DWORD MS_ABI impl__GetMenuBarState_CFrameWnd__UEBAKXZ(const CFrameWnd* self) {return self->CFrameWnd::GetMenuBarState();}
// Symbol: ?SetMenuBarVisibility@CFrameWnd@@UEAAXK@Z
extern "C" void MS_ABI impl__SetMenuBarVisibility_CFrameWnd__UEAAXK_Z(CFrameWnd* self,DWORD style) {self->CFrameWnd::SetMenuBarVisibility(style);}
// Symbol: ?SetMenuBarState@CFrameWnd@@UEAAHK@Z
extern "C" BOOL MS_ABI impl__SetMenuBarState_CFrameWnd__UEAAHK_Z(CFrameWnd* self,DWORD state) {return self->CFrameWnd::SetMenuBarState(state);}
