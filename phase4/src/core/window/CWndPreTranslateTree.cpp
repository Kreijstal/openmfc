// Native pretranslation follows the target window's parent chain.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);

int CWnd::WalkPreTranslateTree(HWND stop,MSG* message) {
    for(HWND handle=message->hwnd;handle;handle=::GetParent(handle)) {
        if(auto* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(handle)) {
            if(window->PreTranslateMessage(message))return TRUE;
        }
        if(handle==stop)break;
    }
    return FALSE;
}
// Symbol: ?WalkPreTranslateTree@CWnd@@SAHPEAUHWND__@@PEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__WalkPreTranslateTree_CWnd__SAHPEAUHWND____PEAUtagMSG___Z(
    HWND stop,MSG* message) {
    return CWnd::WalkPreTranslateTree(stop,message);
}
