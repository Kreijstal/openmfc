#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" int MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
void* MainApp(CDialog* dialog) {
    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if(!module)return nullptr;
    void* app=*reinterpret_cast<void**>(reinterpret_cast<char*>(module)+8);
    if(!app || *reinterpret_cast<void**>(static_cast<char*>(app)+0x40)!=dialog)return nullptr;
    return app;
}
}
// Symbol: ?OnQueryEndSession@CDialog@@IEAAHXZ
extern "C" int MS_ABI impl__OnQueryEndSession_CDialog__IEAAHXZ(CDialog* dialog) {
    void* app=MainApp(dialog);
    if(!app || !(AfxGetThreadState()->m_lastSentMsg.lParam&ENDSESSION_CLOSEAPP))return TRUE;
    using Supports=BOOL (MS_ABI*)(void*);
    return openmfc::detail::WindowSlot<Supports>(app,56)(app);
}
// Symbol: ?OnEndSession@CDialog@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEndSession_CDialog__IEAAXH_Z(CDialog* dialog,BOOL ending) {
    if(!ending)return;
    void* app=MainApp(dialog);
    if(!app || !(AfxGetThreadState()->m_lastSentMsg.lParam&ENDSESSION_CLOSEAPP))return;
    using GetHandler=void* (MS_ABI*)(void*);
    void* handler=openmfc::detail::WindowSlot<GetHandler>(app,65)(app);
    if(handler) {
        using Mark=void (MS_ABI*)(void*,BOOL);
        openmfc::detail::WindowSlot<Mark>(handler,15)(handler,TRUE);
    }
}
// PrintDlgEx updates the sent-message record persistently and invokes the
// client's OnWndMsg override; unlike AfxCallWndProc, this callback has no restore.
// Symbol: ?HandleMessage@CPrintDialogEx@@UEAAJPEAUHWND__@@I_K_JPEA_J@Z
extern "C" long MS_ABI impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(
    void* self,HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam,LRESULT* out) {
    auto* window=reinterpret_cast<CWnd*>(static_cast<char*>(self)-0x130);
    if(!window->m_hWnd)impl__Attach_CWnd__QEAAHPEAUHWND_____Z(window,hwnd);
    auto* state=AfxGetThreadState();
    if(!state) { impl__AfxThrowInvalidArgException__YAXXZ();return E_UNEXPECTED; }
    openmfc::detail::SetSentMessage(state->m_lastSentMsg,hwnd,message,wparam,lparam,true);
    using Handler=BOOL (MS_ABI*)(void*,UINT,WPARAM,LPARAM,LRESULT*);
    BOOL handled=openmfc::detail::WindowSlot<Handler>(window,72)(window,message,wparam,lparam,out);
    HRESULT result=handled ? S_OK : S_FALSE;
    if(handled && message==WM_NOTIFY)SetWindowLongPtrW(GetParent(hwnd),DWLP_MSGRESULT,*out);
    else if(message==WM_INITDIALOG && handled && static_cast<int>(*out))result=S_FALSE;
    return result;
}
