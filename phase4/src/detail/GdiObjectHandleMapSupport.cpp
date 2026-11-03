// Native HGDIOBJ ownership uses AFX_MODULE_THREAD_STATE+0x40.
#include "detail/GdiObjectHandleMapSupport.h"
#include <cstdlib>
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
namespace {
void MS_ABI ConstructGdiObject(CObject* object) {
    ::new (object) CGdiObject();
}
void MS_ABI DestructGdiObject(CObject* object) {
    static_cast<CGdiObject*>(object)->CGdiObject::~CGdiObject();
}
}
namespace openmfc { namespace detail {
CHandleMap* GetGdiObjectHandleMap(bool create) {
    auto* original = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    if (!original->m_pmapHGDIOBJ && create) {
        auto* lease = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        auto previous = lease->m_pfnNewHandler;
        lease->m_pfnNewHandler = &modulethreadstate::HandleAllocationFailure;
        auto* storage = static_cast<CHandleMap*>(impl___2_YAPEAX_K_Z(sizeof(CHandleMap)));
        CHandleMap* map = nullptr;
        if (storage) {
            try {
                map = ::new (storage) CHandleMap(&CGdiObject::classCGdiObject,
                                                  ConstructGdiObject, DestructGdiObject, 8, 1);
            } catch (...) {
                std::free(storage);
                throw;
            }
        }
        original->m_pmapHGDIOBJ = map;
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler = previous;
    }
    return original->m_pmapHGDIOBJ;
}
} }
