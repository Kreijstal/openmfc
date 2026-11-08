// Native CRT thread creation: initialize the child before publishing success.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include <process.h>
#include <cstddef>
extern "C" {
void MS_ABI impl__AfxInitThread__YAXXZ();
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
CWnd* MS_ABI impl___0CWnd__QEAA_XZ(CWnd*);
void MS_ABI impl___1CWnd__UEAA_XZ(CWnd*);
BOOL MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
HWND MS_ABI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd*);
}
namespace {
struct Startup {
    _AFX_THREAD_STATE* parent;
    CWinThread* thread;
    DWORD flags;
    DWORD reserved14;
    void* reserved18;
    HANDLE ready,start;
    BOOL failed;
};
static_assert(sizeof(Startup)==0x38 && offsetof(Startup,ready)==0x20 && offsetof(Startup,failed)==0x30,
    "native stack startup record");
struct WindowLifetime {
    CWnd* window;
    ~WindowLifetime() {impl___1CWnd__UEAA_XZ(window);}
};
struct ExceptionLifetime {
    CException* error;
    ~ExceptionLifetime() {error->Delete();}
};
unsigned __stdcall Entry(void* opaque) {
    auto* startup=static_cast<Startup*>(opaque);
    auto* self=startup->thread;
    alignas(CWnd) BYTE storage[sizeof(CWnd)];
    auto* fallback=impl___0CWnd__QEAA_XZ(reinterpret_cast<CWnd*>(storage));
    WindowLifetime cleanup{fallback};
    try {
        AfxGetThreadState()->m_pModuleState=startup->parent->m_pModuleState;
        auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
        self->m_pModuleState=module;
        openmfc::detail::modulethreadstate::ForModule(module)->m_pCurrentWinThread=self;
        impl__AfxInitThread__YAXXZ();
        auto* app=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_pCurrentWinApp;
        if(app && !self->m_pMainWnd && app->m_pMainWnd && app->m_pMainWnd->m_hWnd) {
            impl__Attach_CWnd__QEAAHPEAUHWND_____Z(fallback,app->m_pMainWnd->m_hWnd);
            self->m_pMainWnd=fallback;
        }
    }catch(CException* error) {
        ExceptionLifetime exception{error};
        impl__Detach_CWnd__QEAAPEAUHWND____XZ(fallback);
        startup->failed=TRUE;
        ::SetEvent(startup->ready);
        AfxEndThread(UINT(-1),FALSE);
        return 0;
    }
    HANDLE start=startup->start;
    ::SetEvent(startup->ready);
    ::WaitForSingleObject(start,INFINITE);
    ::CloseHandle(start);
    UINT result;
    if(self->m_pfnThreadProc)result=self->m_pfnThreadProc(self->m_pThreadParams);
    else if(self->InitInstance())result=static_cast<UINT>(self->Run());
    else result=static_cast<UINT>(self->ExitInstance());
    impl__Detach_CWnd__QEAAPEAUHWND____XZ(fallback);
    AfxEndThread(result,TRUE);
    return 0;
}
}
BOOL CWinThread::CreateThread(DWORD flags,UINT stack,SECURITY_ATTRIBUTES* security) {
    if(m_hThread)impl__AfxThrowInvalidArgException__YAXXZ();
    Startup startup{};
    startup.parent=AfxGetThreadState();startup.thread=this;
    startup.ready=::CreateEventW(nullptr,TRUE,FALSE,nullptr);
    startup.start=::CreateEventW(nullptr,TRUE,FALSE,nullptr);
    startup.flags=flags;
    if(!startup.ready || !startup.start) {
        if(startup.ready)::CloseHandle(startup.ready);
        if(startup.start)::CloseHandle(startup.start);
        return FALSE;
    }
    m_hThread=reinterpret_cast<HANDLE>(::_beginthreadex(security,stack,&Entry,&startup,
        flags|CREATE_SUSPENDED,reinterpret_cast<unsigned*>(&m_nThreadID)));
    if(!m_hThread) {
        ::CloseHandle(startup.ready);::CloseHandle(startup.start);return FALSE;
    }
    ::ResumeThread(m_hThread);
    ::WaitForSingleObject(startup.ready,INFINITE);
    ::CloseHandle(startup.ready);
    if(flags&CREATE_SUSPENDED)::SuspendThread(m_hThread);
    if(startup.failed) {
        ::WaitForSingleObject(m_hThread,INFINITE);
        ::CloseHandle(m_hThread);m_hThread=nullptr;
        ::CloseHandle(startup.start);return FALSE;
    }
    ::SetEvent(startup.start);
    return TRUE;
}
// Symbol: ?CreateThread@CWinThread@@QEAAHKIPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" BOOL MS_ABI impl__CreateThread_CWinThread__QEAAHKIPEAU_SECURITY_ATTRIBUTES___Z(
    CWinThread* self,DWORD flags,UINT stack,SECURITY_ATTRIBUTES* security) {
    return self->CreateThread(flags,stack,security);
}
