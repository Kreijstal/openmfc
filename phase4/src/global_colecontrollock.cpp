// OpenMFC — real implementation of COleControlLock
//
// class COleControlLock  size(40):
//   0  | {vfptr}
//   8  | CLSID m_clsid            (16 bytes)
//  24  | LPCLASSFACTORY m_pClassFactory
//  32  | COleControlLock* m_pNextLock
//
#include <windows.h>
#include <objbase.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct S {
    void*  vfptr;             // 0
    GUID   m_clsid;           // 8
    void*  m_pClassFactory;   // 24  (LPCLASSFACTORY)
    void*  m_pNextLock;       // 32  (COleControlLock*)
};

static_assert(sizeof(S) == 40, "COleControlLock must be size 40");
static_assert(offsetof(S, m_clsid) == 8, "m_clsid @8");
static_assert(offsetof(S, m_pClassFactory) == 24, "m_pClassFactory @24");
static_assert(offsetof(S, m_pNextLock) == 32, "m_pNextLock @32");

} // namespace

// Forward decls of the exported thunks so the vtable can reference them.
extern "C" void  MS_ABI impl___1COleControlLock__UEAA_XZ(void* pThis);

namespace {

// Vector-deleting-destructor wrapper occupying the single vtable slot
// (COleControlLock::{dtor}). Runs the real destructor logic, then frees
// storage when the low bit of the flags is set (delete[] / delete semantics).
void* MS_ABI vdtor(void* p, unsigned flags) {
    impl___1COleControlLock__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}

void* const g_COleControlLock_vtbl[1] = {
    (void*)&vdtor,   // COleControlLock::{dtor}
};

} // namespace

// Symbol: ??0COleControlLock@@QEAA@AEBU_GUID@@@Z
// COleControlLock::COleControlLock(REFCLSID clsid)
//   Copies the requested class id and clears the class-factory / lock-list
//   pointers; the object begins life holding no lock.
extern "C" void* MS_ABI impl___0COleControlLock__QEAA_AEBU_GUID___Z(void* pThis, const GUID* clsid) {
    S* self = (S*)pThis;
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
    S* self = (S*)pThis;
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
