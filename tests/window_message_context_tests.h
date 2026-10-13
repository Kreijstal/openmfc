#pragma once
#include "detail/WindowMessageSupport.h"
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd*,UINT,WPARAM,LPARAM);
namespace window_message_tests {
static int deleted=0,processed=0;
class Failure : public CException {
public:
    Failure() : CException(TRUE) {}
    ~Failure() override { ++deleted; }
};
class Window : public CWnd {
public:
    MSG observed{};
    Window* nested=nullptr;
    bool throwFramework=false,throwOther=false,handled=true;
    int calls=0;
    int probed=0;
    void PreSubclassWindow() override { probed=22; }
    BOOL OnGestureRotate(CPoint,double angle) override { probed=81;return angle==0.5; }
    _AFX_OCC_DIALOG_INFO* GetOccDialogInfo() override { probed=90;return reinterpret_cast<_AFX_OCC_DIALOG_INFO*>(UINT_PTR(0x9090)); }
    LRESULT WindowProc(UINT message,WPARAM wparam,LPARAM lparam) override {
        ++calls;observed=*GetCurrentMessage();
        if(nested) {
            impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(nested,m_hWnd,WM_APP+2,17,-18);
            check(GetCurrentMessage()->message==message && GetCurrentMessage()->wParam==wparam && GetCurrentMessage()->lParam==lparam,
                "nested window dispatch restores the outer sent-message arguments");
        }
        if(throwFramework)throw static_cast<CException*>(new Failure);
        if(throwOther)throw 73;
        return 1234;
    }
    BOOL OnWndMsg(UINT message,WPARAM wparam,LPARAM lparam,LRESULT* out) override {
        ++calls;observed=*GetCurrentMessage();if(out)*out=0x2345;return handled;
    }
    LRESULT DefWindowProcW(UINT,WPARAM,LPARAM) override { return 0x3456; }
};
static LRESULT Process(CException*,const MSG* message) {
    ++processed;
    check(message==&AfxGetThreadState()->m_lastSentMsg && message->message==WM_APP+3,
        "framework window failures reach the exception processor with the active native message");
    return -456;
}
static void messages() {
    auto* state=AfxGetThreadState();MSG previous{};
    previous.hwnd=reinterpret_cast<HWND>(UINT_PTR(0x191));previous.message=WM_APP+1;
    previous.wParam=0x8765;previous.lParam=-123;previous.time=91;previous.pt={-37,42};
    state->m_lastSentMsg=previous;
    Window outer,inner;outer.nested=&inner;
    using Pre=void (MS_ABI*)(void*);
    using Rotate=BOOL (MS_ABI*)(void*,CPoint,double);
    using Occ=_AFX_OCC_DIALOG_INFO* (MS_ABI*)(void*);
    openmfc::detail::WindowSlot<Pre>(&outer,22)(&outer);
    bool early=outer.probed==22;
    bool middle=openmfc::detail::WindowSlot<Rotate>(&outer,81)(&outer,CPoint(1,2),0.5) && outer.probed==81;
    bool last=openmfc::detail::WindowSlot<Occ>(&outer,90)(&outer)==reinterpret_cast<_AFX_OCC_DIALOG_INFO*>(UINT_PTR(0x9090)) && outer.probed==90;
    check(early && middle && last,"native CWnd vtable slots include early, gesture and final overrides with their exact argument ABI");
    check(impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(&outer,previous.hwnd,WM_APP+3,UINT_PTR(0x123456789),-900)==1234 &&
        outer.calls==1 && inner.calls==1 && outer.observed.hwnd==previous.hwnd && outer.observed.message==WM_APP+3 &&
        outer.observed.wParam==UINT_PTR(0x123456789) && outer.observed.lParam==-900 &&
        !memcmp(&state->m_lastSentMsg,&previous,sizeof(MSG)),
        "native WindowProc slot dispatch preserves 64-bit arguments and restores the complete sent-message record");
    outer.nested=nullptr;outer.throwFramework=true;g_wndExceptionHandler=Process;
    check(impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(&outer,nullptr,WM_APP+3,0,0)==-456 &&
        processed==1 && deleted==1 && !memcmp(&state->m_lastSentMsg,&previous,sizeof(MSG)),
        "window exception boundaries delete owned framework errors and restore message context");
    outer.throwFramework=false;outer.throwOther=true;bool caught=false;
    try { impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(&outer,nullptr,WM_APP+4,0,0); }catch(int value) { caught=value==73; }
    check(caught && !memcmp(&state->m_lastSentMsg,&previous,sizeof(MSG)),
        "non-framework exceptions propagate while sent-message storage unwinds");
    outer.throwOther=false;g_wndExceptionHandler=nullptr;
    const MSG* current=impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();DWORD position=GetMessagePos();
    check(current==&state->m_lastSentMsg && current->message==previous.message && current->lParam==previous.lParam &&
        current->time==static_cast<DWORD>(GetMessageTime()) && current->pt.x==static_cast<short>(LOWORD(position)) &&
        current->pt.y==static_cast<short>(HIWORD(position)),
        "GetCurrentMessage refreshes native sent-message time and signed cursor coordinates without requiring CWinThread");
    outer.handled=true;
    check(impl__WindowProc_CWnd__MEAA_JI_K_J_Z(&outer,WM_APP,1,2)==0x2345,
        "the base WindowProc invokes the client's native OnWndMsg override");
    outer.handled=false;
    check(impl__WindowProc_CWnd__MEAA_JI_K_J_Z(&outer,WM_APP,1,2)==0x3456,
        "unhandled messages invoke the client's native DefWindowProc override");
}
struct App {
    void* vtable=table;
    unsigned char prefix[56]{};
    void* main=nullptr;
    BOOL support=FALSE;
    int queries=0,recoveryQueries=0;
    void* recovery=nullptr;
    void* table[66]{};
    static BOOL MS_ABI Supports(App* self) { ++self->queries;return self->support; }
    static void* MS_ABI Recovery(App* self) { ++self->recoveryQueries;return self->recovery; }
    App() { table[56]=reinterpret_cast<void*>(Supports);table[65]=reinterpret_cast<void*>(Recovery); }
};
struct Recovery {
    void* vtable=table;
    BOOL marked=FALSE;
    int calls=0;
    void* table[16]{};
    static void MS_ABI Mark(Recovery* self,BOOL value) { ++self->calls;self->marked=value; }
    Recovery() { table[15]=reinterpret_cast<void*>(Mark); }
};
static void dialogs() {
    // Raw app prefix: vptr at zero, main window at 0x40.
    App app;void* appPointer=&app.vtable;void* module[4]{};module[1]=appPointer;
    auto* previous=g_cmdTestCurrentModule;g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
    alignas(void*) unsigned char dialog[304]{};auto* target=reinterpret_cast<CDialog*>(dialog);app.main=target;
    auto* state=AfxGetThreadState();MSG saved=state->m_lastSentMsg;
    state->m_lastSentMsg.lParam=0;
    check(impl__OnQueryEndSession_CDialog__IEAAHXZ(target)==TRUE && !app.queries,
        "ordinary shutdown queries bypass restart-manager support checks");
    state->m_lastSentMsg.lParam=ENDSESSION_CLOSEAPP;
    check(!impl__OnQueryEndSession_CDialog__IEAAHXZ(target) && app.queries==1,
        "restart-manager close queries consult the main app's native SupportsRestartManager override");
    app.support=TRUE;
    check(impl__OnQueryEndSession_CDialog__IEAAHXZ(target)==TRUE && app.queries==2,
        "supported restart-manager shutdown queries return the application result");
    impl__OnEndSession_CDialog__IEAAXH_Z(target,FALSE);
    check(!app.recoveryQueries,"cancelled shutdown does not request a recovery handler");
    Recovery recovery;app.recovery=&recovery;
    impl__OnEndSession_CDialog__IEAAXH_Z(target,TRUE);
    check(app.recoveryQueries==1 && recovery.calls==1 && recovery.marked==TRUE,
        "completed restart-manager shutdown marks the application's actual recovery handler");
    app.recovery=nullptr;impl__OnEndSession_CDialog__IEAAXH_Z(target,TRUE);
    check(app.recoveryQueries==2 && recovery.calls==1,
        "shutdown tolerates a missing recovery handler without an extra mark");
    app.main=nullptr;
    check(impl__OnQueryEndSession_CDialog__IEAAHXZ(target)==TRUE && app.queries==2,
        "non-main dialogs permit shutdown without calling app restart hooks");
    g_cmdTestCurrentModule=previous;state->m_lastSentMsg=saved;
}
static MSG printed{};
static BOOL printHandled=TRUE;
static LRESULT printResult=0;
static BOOL MS_ABI Print(void*,UINT,WPARAM,LPARAM,LRESULT* out) {
    printed=*CWnd::GetCurrentMessage();*out=printResult;return printHandled;
}
static void printDialogs() {
    WNDCLASSW cls{};cls.lpfnWndProc=DefWindowProcW;cls.cbWndExtra=sizeof(LONG_PTR);
    cls.hInstance=GetModuleHandleW(nullptr);cls.lpszClassName=L"OpenMfcMessageContextParent";
    RegisterClassW(&cls);
    HWND parent=CreateWindowExW(0,cls.lpszClassName,L"",0,0,0,10,10,nullptr,nullptr,cls.hInstance,nullptr);
    HWND child=CreateWindowExW(0,L"STATIC",L"",WS_CHILD,0,0,1,1,parent,nullptr,cls.hInstance,nullptr);
    alignas(void*) unsigned char object[0x138]{};void* table[91]{};table[72]=reinterpret_cast<void*>(Print);
    void* vtable=table;memcpy(object,&vtable,sizeof(vtable));void* callback=object+0x130;
    MSG old=AfxGetThreadState()->m_lastSentMsg;LRESULT result=0;
    printResult=0x123456789;
    check(child && impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(callback,child,WM_NOTIFY,43,-44,&result)==S_OK &&
        printed.hwnd==child && printed.message==WM_NOTIFY && printed.wParam==43 && printed.lParam==-44 &&
        GetWindowLongPtrW(parent,DWLP_MSGRESULT)==printResult &&
        AfxGetThreadState()->m_lastSentMsg.message==WM_NOTIFY,
        "PrintDlgEx attaches its native owner, invokes OnWndMsg, preserves sent-message state and writes the parent result");
    printResult=0;
    check(impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(callback,child,WM_INITDIALOG,0,0,&result)==S_OK,
        "handled print-dialog initialization with a zero result returns S_OK");
    printResult=0x100000001;
    check(impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(callback,child,WM_INITDIALOG,0,0,&result)==S_FALSE,
        "print initialization treats the low DWORD of a nonzero result as S_FALSE");
    printHandled=FALSE;
    check(impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(callback,child,WM_APP,0,0,&result)==S_FALSE,
        "unhandled print-dialog callbacks return S_FALSE");
    printHandled=TRUE;AfxGetThreadState()->m_lastSentMsg=old;
    DestroyWindow(child);DestroyWindow(parent);UnregisterClassW(cls.lpszClassName,cls.hInstance);
}
}
static void windowMessageContext() { window_message_tests::messages();window_message_tests::dialogs();window_message_tests::printDialogs(); }
