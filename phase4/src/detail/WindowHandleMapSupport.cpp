// Native HWND ownership uses AFX_MODULE_THREAD_STATE+0x28, not a global registry.
#include "detail/WindowHandleMapSupport.h"
#include <cstdlib>
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
extern "C" void* AFXAPI impl___0CWnd__QEAA_XZ(void*);
extern "C" void AFXAPI impl___1CWnd__UEAA_XZ(CWnd*);
namespace {
void AFXAPI ConstructWindow(CObject* object) { impl___0CWnd__QEAA_XZ(object); }
void AFXAPI DestructWindow(CObject* object) { impl___1CWnd__UEAA_XZ(static_cast<CWnd*>(object)); }
}
namespace openmfc { namespace detail {
CHandleMap* GetWindowHandleMap(bool create) {
    auto* original=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    if(!original->m_pmapHWND && create) {
        auto* lease=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        auto previous=lease->m_pfnNewHandler;
        lease->m_pfnNewHandler=&modulethreadstate::HandleAllocationFailure;
        auto* storage=static_cast<CHandleMap*>(impl___2_YAPEAX_K_Z(sizeof(CHandleMap)));
        CHandleMap* map=nullptr;
        if(storage) {
            try { map=::new(storage) CHandleMap(&CWnd::classCWnd,ConstructWindow,DestructWindow,64,1); }
            catch(...) { std::free(storage);throw; }
        }
        // Native EH frees failed construction storage, but does not restore
        // the allocation handler on failure. Publication precedes restoration.
        original->m_pmapHWND=map;
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler=previous;
    }
    return original->m_pmapHWND;
}
} }
