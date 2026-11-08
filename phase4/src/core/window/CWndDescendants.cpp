// Native descendant dispatch shared by dialog initialization and thread idle.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);

void CWnd::SendMessageToDescendants(HWND parent,UINT message,ULONGLONG wparam,LONGLONG lparam,int deep,int permanent) {
    for(HWND child=::GetTopWindow(parent);child;child=::GetWindow(child,GW_HWNDNEXT)) {
        if(permanent) {
            CWnd* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(child);
            if(window)impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(window,window->m_hWnd,message,wparam,lparam);
        } else ::SendMessageW(child,message,static_cast<WPARAM>(wparam),static_cast<LPARAM>(lparam));
        if(deep && ::GetTopWindow(child))SendMessageToDescendants(child,message,wparam,lparam,deep,permanent);
    }
}
// Symbol: ?SendMessageToDescendants@CWnd@@SAXPEAUHWND__@@I_K_JHH@Z
extern "C" void MS_ABI impl__SendMessageToDescendants_CWnd__SAXPEAUHWND____I_K_JHH_Z(HWND parent,UINT message,ULONGLONG wparam,LONGLONG lparam,int deep,int permanent) {CWnd::SendMessageToDescendants(parent,message,wparam,lparam,deep,permanent);}
