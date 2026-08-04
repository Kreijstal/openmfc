// COleControlLock — OpenMFC implementation.
// Sources: global_colecontrollock.cpp

#include "detail/COleControlLockSupport.h"

// Symbol: ??0COleControlLock@@QEAA@AEBU_GUID@@@Z
// COleControlLock::COleControlLock(REFCLSID clsid)
//   Copies the requested class id and clears the class-factory / lock-list
//   pointers; the object begins life holding no lock.
extern "C" void* MS_ABI impl___0COleControlLock__QEAA_AEBU_GUID___Z(void* pThis, const GUID* clsid) {
    S_Colecontrollock* self = (S_Colecontrollock*)pThis;
    self->vfptr = (void*)&g_COleControlLock_vtbl[0];
    if (clsid)
        self->m_clsid = *clsid;
    else
        self->m_clsid = GUID_NULL;
    self->m_pClassFactory = nullptr;
    self->m_pNextLock = nullptr;
    return pThis;
}
// Symbol: ??1COleControlLock@@UEAA@XZ
// COleControlLock::~COleControlLock()
//   Releases any class factory the lock still owns (unlocking the server)
//   and drops the reference. A default-constructed / released lock owns
//   nothing, so this is a no-op in that case.
extern "C" void MS_ABI impl___1COleControlLock__UEAA_XZ(void* pThis) {
    S_Colecontrollock* self = (S_Colecontrollock*)pThis;
    if (self->m_pClassFactory) {
        IClassFactory* pcf = (IClassFactory*)self->m_pClassFactory;
        pcf->LockServer(FALSE);
        pcf->Release();
        self->m_pClassFactory = nullptr;
    }
    self->m_pNextLock = nullptr;
    // Keep the vfptr valid through destruction.
    self->vfptr = (void*)&g_COleControlLock_vtbl[0];
}

// COleControlLock's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace colecontrollock {
extern void* const g_COleControlLock_vtbl[1] = {
    (void*)&vdtor_Colecontrollock,   // COleControlLock::{dtor}
};
} } }
