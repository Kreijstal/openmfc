// Modal owner selection: mfc140u GetSafeOwner_ 0x1ced10.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxstat_.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
// Symbol: ?GetSafeOwner_@CWnd@@SAPEAUHWND__@@PEAU2@PEAPEAU2@@Z
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND parent,HWND* disabled) {
    HWND owner=parent;
    if(!owner) {
        auto* state=AfxGetThreadState();
        if(state->m_pRoutingFrame)owner=reinterpret_cast<CWnd*>(state->m_pRoutingFrame)->GetSafeHwnd();
        else if(auto* thread=AfxGetThread())if(auto* window=thread->GetMainWnd())owner=window->GetSafeHwnd();
    }
    while(owner && (::GetWindowLongW(owner,GWL_STYLE)&WS_CHILD))owner=::GetParent(owner);
    HWND top=owner;
    while(top) {HWND next=::GetParent(top);if(!next)break;top=next;}
    if(!parent && owner)owner=::GetLastActivePopup(owner);
    if(disabled) {
        *disabled=nullptr;
        if(top && top!=owner && ::IsWindowEnabled(top)) {*disabled=top;::EnableWindow(top,FALSE);}
    }
    return owner;
}
// Symbol: ?GetSafeOwner@CWnd@@SAPEAV1@PEAV1@PEAPEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__GetSafeOwner_CWnd__SAPEAV1_PEAV1_PEAPEAUHWND_____Z(CWnd* parent,HWND* disabled) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(parent?parent->GetSafeHwnd():nullptr,disabled));
}
void* CWnd::GetSafeOwner(void* parent,HWND* disabled) {return impl__GetSafeOwner_CWnd__SAPEAV1_PEAV1_PEAPEAUHWND_____Z(static_cast<CWnd*>(parent),disabled);}
HWND CWnd::GetSafeOwner_(void* parent,void** disabled) {return impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(static_cast<HWND>(parent),reinterpret_cast<HWND*>(disabled));}
