// All framework entry points use the same function address, including clients
// that register the exported procedure directly and CBT superclass comparisons.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowCreationSupport.h"
#pragma comment(linker,"/alternatename:?AfxWndProc@@YA_JPEAUHWND__@@I_K_J@Z=impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z")
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
// Symbol: ?AfxWndProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" LRESULT MS_ABI impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    if(message==0x360)return TRUE;
    CWnd* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwnd);
    if(window && window->m_hWnd==hwnd)return impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(window,hwnd,message,wparam,lparam);
    return ::DefWindowProcW(hwnd,message,wparam,lparam);
}
