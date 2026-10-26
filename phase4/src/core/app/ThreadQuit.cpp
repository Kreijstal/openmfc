#include "detail/ModuleThreadStateSupport.h"
// Native mfc140u 0x1cc850: terminate OLE services before posting WM_QUIT.
// Symbol: ?AfxPostQuitMessage@@YAXH@Z
extern "C" void MS_ABI impl__AfxPostQuitMessage__YAXH_Z(int code) {
    auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    if(thread) {
        using Terminate=void (MS_ABI*)(BOOL,BOOL);
        auto callback=*reinterpret_cast<Terminate*>(reinterpret_cast<BYTE*>(thread)+0x78);
        if(callback)callback(TRUE,TRUE);
    }
    ::PostQuitMessage(code);
}
