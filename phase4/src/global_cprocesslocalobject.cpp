// OpenMFC — CProcessLocalObject (real implementation)
//
// CProcessLocalObject is a non-polymorphic MFC helper that lazily creates and
// owns a single CNoTrackObject-derived instance shared across a process. It is
// the backing store for AFX_MAINTAIN_STATE-style process-local state.
//
// Layout (from cl.exe /d1reportSingleClassLayout):
//   class CProcessLocalObject size(8):
//     +0  m_pObject   (CNoTrackObject*)

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful mirror of CProcessLocalObject.
struct S {
    void* m_pObject;   // +0: owned CNoTrackObject*
};

static_assert(sizeof(S) == 8, "CProcessLocalObject size mismatch");
static_assert(offsetof(S, m_pObject) == 0, "m_pObject offset mismatch");

// Process-wide guard for the lazy-create double-checked path. The real MFC uses
// AfxLockGlobals(CRIT_PROCESSLOCAL); an SRWLOCK needs no explicit init and gives
// the same single-writer serialization for construction.
SRWLOCK g_processLocalLock = SRWLOCK_INIT;

// CNoTrackObject is polymorphic (virtual dtor at vtable slot 0). Deleting it via
// the vector-deleting-destructor with flag&1 runs its dtor and frees storage
// through CNoTrackObject::operator delete.
typedef void* (MS_ABI *DeletingDtor)(void*, unsigned);

// Factory pointer type: CNoTrackObject* (*)()
typedef void* (MS_ABI *CreateFn)();

} // namespace

// Symbol: ??1CProcessLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CProcessLocalObject__QEAA_XZ(void* pThis)
{
    S* self = static_cast<S*>(pThis);
    if (self->m_pObject != nullptr) {
        void** vt = *reinterpret_cast<void***>(self->m_pObject);
        DeletingDtor dtor = reinterpret_cast<DeletingDtor>(vt[0]);
        dtor(self->m_pObject, 1u);   // run virtual dtor + free
        self->m_pObject = nullptr;
    }
}

// Symbol: ?GetData@CProcessLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
extern "C" void* MS_ABI impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
    void* pThis, CreateFn pfnCreateObject)
{
    S* self = static_cast<S*>(pThis);

    // Double-checked lazy creation, matching CProcessLocalObject::GetData.
    if (self->m_pObject == nullptr) {
        AcquireSRWLockExclusive(&g_processLocalLock);
        if (self->m_pObject == nullptr) {
            self->m_pObject = pfnCreateObject();
        }
        ReleaseSRWLockExclusive(&g_processLocalLock);
    }
    return self->m_pObject;
}
