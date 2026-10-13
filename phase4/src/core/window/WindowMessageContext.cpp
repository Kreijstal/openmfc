#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" LRESULT MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(CException*,const MSG*);
extern "C" void MS_ABI impl__CenterWindow_CWnd__QEAAXPEAV1__Z(CWnd*,CWnd*);
namespace openmfc { namespace detail {
void SetSentMessage(MSG& record,HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam,bool refresh) {
    record.hwnd=hwnd;record.message=message;record.wParam=wparam;record.lParam=lparam;
    if(refresh) {
        record.time=static_cast<DWORD>(GetMessageTime());DWORD point=GetMessagePos();
        record.pt={static_cast<short>(LOWORD(point)),static_cast<short>(HIWORD(point))};
    }
}
}}
const MSG* CWnd::GetCurrentMessage() {
    auto* state=AfxGetThreadState();
    if(!state) { impl__AfxThrowInvalidArgException__YAXXZ();return nullptr; }
    MSG& message=state->m_lastSentMsg;
    openmfc::detail::SetSentMessage(message,message.hwnd,message.message,message.wParam,message.lParam,true);
    return &message;
}
// Symbol: ?GetCurrentMessage@CWnd@@KAPEBUtagMSG@@XZ
extern "C" const MSG* MS_ABI impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ() { return CWnd::GetCurrentMessage(); }
namespace {
struct SentScope {
    MSG& current;MSG previous;
    explicit SentScope(MSG& message) : current(message),previous(message) {}
    ~SentScope() { current=previous; }
};
DWORD Style(CWnd* window) {
    if(window->m_pCtrlCont) {
        using Getter=DWORD (MS_ABI*)(void*);
        return openmfc::detail::WindowSlot<Getter>(window->m_pCtrlCont,32)(window->m_pCtrlCont);
    }
    return static_cast<DWORD>(GetWindowLongPtrW(window->m_hWnd,GWL_STYLE));
}
void PostInit(CWnd* window,const RECT& before,DWORD originalStyle) {
    if(originalStyle&WS_VISIBLE || Style(window)&(WS_VISIBLE|WS_CHILD))return;
    RECT after{};GetWindowRect(window->m_hWnd,&after);
    if(before.left!=after.left || before.top!=after.top)return;
    HWND owner=GetWindow(window->m_hWnd,GW_OWNER);
    if(owner && GetWindowLongPtrW(owner,GWL_STYLE)&WS_CHILD)return;
    using Check=BOOL (MS_ABI*)(void*);
    if(openmfc::detail::WindowSlot<Check>(window,85)(window))impl__CenterWindow_CWnd__QEAAXPEAV1__Z(window,nullptr);
}
}
// Native AfxCallWndProc (0x28aa70) preserves the entire sent-message record.
// Symbol: ?AfxCallWndProc@@YA_JPEAVCWnd@@PEAUHWND__@@I_K_J@Z
extern "C" LRESULT MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(
    CWnd* window,HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    auto* state=AfxGetThreadState();
    if(!state) { impl__AfxThrowInvalidArgException__YAXXZ();return 0; }
    SentScope scope(state->m_lastSentMsg);
    openmfc::detail::SetSentMessage(scope.current,hwnd,message,wparam,lparam,false);
    if(message==WM_DESTROY && window->m_pCtrlCont) {
        using Cleanup=void (MS_ABI*)(void*,BOOL);
        openmfc::detail::WindowSlot<Cleanup>(window->m_pCtrlCont,26)(window->m_pCtrlCont,FALSE);
    }
    RECT before{};DWORD originalStyle=0;
    try {
        if(message==WM_INITDIALOG) { GetWindowRect(window->m_hWnd,&before);originalStyle=Style(window); }
        using Procedure=LRESULT (MS_ABI*)(void*,UINT,WPARAM,LPARAM);
        LRESULT result=openmfc::detail::WindowSlot<Procedure>(window,71)(window,message,wparam,lparam);
        if(message==WM_INITDIALOG)PostInit(window,before,originalStyle);
        return result;
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        return impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(error,&scope.current);
    }
}

LONGLONG CWnd::Default() {
    const MSG* message=GetCurrentMessage();
    using Procedure=LRESULT (MS_ABI*)(void*,UINT,WPARAM,LPARAM);
    return openmfc::detail::WindowSlot<Procedure>(this,73)(this,message->message,message->wParam,message->lParam);
}
// Symbol: ?Default@CWnd@@IEAA_JXZ
extern "C" LONGLONG MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* self) {return self->Default();}
