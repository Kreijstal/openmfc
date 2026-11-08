// Native thread hook installation and module/TLS termination.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <process.h>
extern "C" BOOL MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int);
namespace openmfc { namespace detail { namespace winthread {
LRESULT CALLBACK MessageFilterHook(int code,WPARAM wparam,LPARAM lparam) {
    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if(!module->m_bDLL && code>=0) {
        auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
        if(thread)return thread->ProcessMessageFilter(code,reinterpret_cast<MSG*>(lparam));
    }
    auto* state=AfxGetThreadState();
    return ::CallNextHookEx(state->m_hHookOldMsgFilter,code,wparam,lparam);
}
} } }
// Symbol: ?AfxInitThread@@YAXXZ
extern "C" void MS_ABI impl__AfxInitThread__YAXXZ() {
    if(!impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_bDLL) {
        auto* state=AfxGetThreadState();
        state->m_hHookOldMsgFilter=::SetWindowsHookExW(WH_MSGFILTER,
            &openmfc::detail::winthread::MessageFilterHook,nullptr,::GetCurrentThreadId());
    }
}
// Symbol: ?AfxTermThread@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(HINSTANCE instance) {
    try {
        auto& count=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_nTempMapLock;
        count=static_cast<int>(static_cast<uint32_t>(count)+1);
        impl__AfxUnlockTempMaps__YAHH_Z(-1);
    }catch(CException* error) {error->Delete();}
    try {
        if(!instance) {
            auto* state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
            if(state->m_pToolTip) {
                reinterpret_cast<CWnd*>(state->m_pToolTip)->DestroyWindow();
                if(state->m_pToolTip) {
                    using Delete=void* (MS_ABI*)(void*,unsigned);
                    reinterpret_cast<Delete>((*reinterpret_cast<void***>(state->m_pToolTip))[1])(state->m_pToolTip,1);
                }
                state->m_pToolTip=nullptr;
            }
        }
    }catch(CException* error) {error->Delete();}
    if(auto* slots=openmfc::detail::cthreadlocalobject::threadSlots)
        slots->DeleteValues(instance,FALSE);
}
void AfxEndThread(UINT result,BOOL deleteThread) {
    auto* state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    auto* thread=state->m_pCurrentWinThread;
    if(thread) {
        if(thread->m_lpfnOleTermOrFreeLib)thread->m_lpfnOleTermOrFreeLib(TRUE,FALSE);
        if(deleteThread)thread->Delete();
        state->m_pCurrentWinThread=nullptr;
    }
    impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(nullptr);
    ::_endthreadex(result);
}
// Symbol: ?AfxEndThread@@YAXIH@Z
extern "C" void MS_ABI impl__AfxEndThread__YAXIH_Z(UINT result,BOOL deleteThread) {AfxEndThread(result,deleteThread);}
