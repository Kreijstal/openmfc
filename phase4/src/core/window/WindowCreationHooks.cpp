// Native release-x64 CBT attachment (0x28b130) and activation (0x28af80).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowCreationSupport.h"
#include "detail/WindowActivationSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
#include <cwchar>
extern "C" {
BOOL MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
LRESULT MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(CException*,const MSG*);
}
namespace openmfc { namespace detail {
ATOM menuWindowAtom=0;
WNDPROC ModuleWindowProcedure() {
    auto* module=reinterpret_cast<BYTE*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ());
    return *reinterpret_cast<WNDPROC*>(module+0x70);
}
namespace {
const wchar_t oldProcedureProperty[]=L"AfxOldWndProc423";
CWnd* Window(HWND hwnd) {return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwnd);}
class ModuleScope {
    _AFX_THREAD_STATE* thread_;
    AFX_MODULE_STATE* previous_;
public:
    explicit ModuleScope(AFX_MODULE_STATE* module):thread_(AfxGetThreadState()),previous_(thread_->m_pModuleState) {thread_->m_pModuleState=module;}
    ~ModuleScope(){thread_->m_pModuleState=previous_;}
};
void HandleActivate(CWnd* self,WPARAM state,CWnd* other) {
    if(self->GetStyle()&WS_CHILD)return;
    CWnd* top=self->GetTopLevelParent();
    if(!top)return;
    if(other && ::IsWindow(other->m_hWnd) && top==other->GetTopLevelParent())return;
    HWND pair[2]={self->m_hWnd,other?other->m_hWnd:nullptr};
    ::SendMessageW(top->m_hWnd,0x36e,state,reinterpret_cast<LPARAM>(pair));
}
BOOL HandleSetCursor(CWnd* self,int hit,UINT message) {
    if(hit!=HTERROR || (message!=WM_LBUTTONDOWN && message!=WM_RBUTTONDOWN && message!=WM_MBUTTONDOWN))return FALSE;
    CWnd* top=self->GetTopLevelParent();
    if(!top)return FALSE;
    CWnd* popup=Window(::GetLastActivePopup(top->m_hWnd));
    if(!popup || popup==Window(::GetForegroundWindow()) || !popup->IsWindowEnabled())return FALSE;
    ::SetForegroundWindow(popup->m_hWnd);
    return TRUE;
}
}
LRESULT CALLBACK ActivationWindowProcedure(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    // Native property lookup precedes the framework exception boundary.
    auto previous=reinterpret_cast<WNDPROC>(::GetPropW(hwnd,oldProcedureProperty));
    try {
        switch(message) {
        case WM_INITDIALOG: {
            CWnd* self=Window(hwnd);
            RECT rectangle{};::GetWindowRect(self->m_hWnd,&rectangle);
            DWORD style=self->GetStyle();
            LRESULT result=::CallWindowProcW(previous,hwnd,message,wparam,lparam);
            PostInitializeWindow(self,rectangle,style);
            return result;
        }
        case WM_NCDESTROY:
            ::SetWindowLongPtrW(hwnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(previous));
            ::RemovePropW(hwnd,oldProcedureProperty);
            ::GlobalDeleteAtom(::GlobalFindAtomW(oldProcedureProperty));
            break;
        case WM_SETCURSOR:
            if(HandleSetCursor(Window(hwnd),static_cast<short>(LOWORD(lparam)),HIWORD(lparam)))return 0;
            break;
        case WM_ACTIVATE: {
            CWnd* other=Window(reinterpret_cast<HWND>(lparam));
            HandleActivate(Window(hwnd),wparam,other);
            break;
        }
        }
        return ::CallWindowProcW(previous,hwnd,message,wparam,lparam);
    } catch(CException* error) {
        FrameworkExceptionCleanup cleanup{error};
        MSG record;
        record.hwnd=hwnd;record.message=message;record.wParam=wparam;record.lParam=lparam;
        return impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(error,&record);
    }
}
LRESULT CALLBACK WindowCreationHook(int code,WPARAM wparam,LPARAM lparam) {
    auto* thread=AfxGetThreadState();
    if(!thread)impl__AfxThrowInvalidArgException__YAXXZ();
    if(code!=HCBT_CREATEWND)return ::CallNextHookEx(thread->m_hHookOldCbtFilter,code,wparam,lparam);
    auto* create=reinterpret_cast<CBT_CREATEWNDW*>(lparam)->lpcs;
    CWnd* pending=thread->m_pWndInit;
    auto* module=reinterpret_cast<BYTE*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ());
    HWND hwnd=reinterpret_cast<HWND>(wparam);
    if(pending) {
        ModuleScope scope(pending->m_pModuleState);
        impl__Attach_CWnd__QEAAHPEAUHWND_____Z(pending,hwnd);
        using Subclass=void (MS_ABI*)(void*);
        using Super=WNDPROC* (MS_ABI*)(void*);
        WindowSlot<Subclass>(pending,22)(pending);
        WNDPROC* saved=WindowSlot<Super>(pending,65)(pending);
        WNDPROC procedure=ModuleWindowProcedure();
        auto previous=reinterpret_cast<WNDPROC>(::SetWindowLongPtrW(hwnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(procedure)));
        if(previous!=procedure)*saved=previous;
        thread->m_pWndInit=nullptr;
    } else if(!(create->style&WS_CHILD) && !module[0x28]) {
        bool menu=false;
        if(!menuWindowAtom) {
            WNDCLASSEXW info{};info.cbSize=sizeof(info);
            menuWindowAtom=static_cast<ATOM>(ContextMenuClassInfo(&info));
            if(!menuWindowAtom) {
                wchar_t name[256];
                if(::GetClassNameW(hwnd,name,256)) {name[255]=0;menu=std::wcscmp(name,L"#32768")==0;}
            } else menu=static_cast<ATOM>(::GetClassLongPtrW(hwnd,GCW_ATOM))==menuWindowAtom;
        } else menu=static_cast<ATOM>(::GetClassLongPtrW(hwnd,GCW_ATOM))==menuWindowAtom;
        if(!menu) {
            auto previous=reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(hwnd,GWLP_WNDPROC));
            if(previous && !::GetPropW(hwnd,oldProcedureProperty)) {
                ::SetPropW(hwnd,oldProcedureProperty,reinterpret_cast<HANDLE>(previous));
                if(::GetPropW(hwnd,oldProcedureProperty)==reinterpret_cast<HANDLE>(previous)) {
                    ::GlobalAddAtomW(oldProcedureProperty);
                    ::SetWindowLongPtrW(hwnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&ActivationWindowProcedure));
                }
            }
        }
    }
    return ::CallNextHookEx(thread->m_hHookOldCbtFilter,HCBT_CREATEWND,wparam,lparam);
}
} }
// Symbol: ?AfxHookWindowCreate@@YAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(CWnd* window) {
    auto* thread=AfxGetThreadState();
    if(!thread)impl__AfxThrowInvalidArgException__YAXXZ();
    if(thread->m_pWndInit==window)return;
    if(!thread->m_hHookOldCbtFilter) {
        thread->m_hHookOldCbtFilter=::SetWindowsHookExW(WH_CBT,&openmfc::detail::WindowCreationHook,nullptr,::GetCurrentThreadId());
        if(!thread->m_hHookOldCbtFilter)impl__AfxThrowMemoryException__YAXXZ();
    }
    thread->m_pWndInit=window;
}
// Symbol: ?AfxUnhookWindowCreate@@YAHXZ
extern "C" BOOL MS_ABI impl__AfxUnhookWindowCreate__YAHXZ() {
    auto* thread=AfxGetThreadState();
    if(!thread)impl__AfxThrowInvalidArgException__YAXXZ();
    if(!thread->m_pWndInit)return TRUE;
    thread->m_pWndInit=nullptr;
    return FALSE;
}
// Symbol: ?AfxGetAfxWndProc@@YAP6A_JPEAUHWND__@@I_K_J@ZXZ
extern "C" WNDPROC MS_ABI impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ() {return openmfc::detail::ModuleWindowProcedure();}
