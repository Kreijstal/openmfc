// OpenMFC — CThreadLocalObject real implementation.
//
// CThreadLocalObject manages a per-thread instance of a CNoTrackObject-derived
// value. It owns a "slot" index (m_nSlot); the value for the current thread is
// looked up (GetDataNA) or lazily created (GetData) through that slot. The
// destructor releases the slot and destroys every thread's value bound to it.
//
// Layout (cl.exe /d1reportSingleClassLayout):
//   class CThreadLocalObject size(4):
//     +---
//   0 | m_nSlot
//     +---
// Non-polymorphic (no vfptr).

#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful mirror of CThreadLocalObject.
struct S {
    DWORD m_nSlot;   // 0 == unallocated
};
static_assert(sizeof(S) == 4, "CThreadLocalObject must be 4 bytes");
static_assert(offsetof(S, m_nSlot) == 0, "m_nSlot at offset 0");

// A CNoTrackObject is a polymorphic type with a virtual destructor; slot 0 of
// its vtable is the vector-deleting-destructor. We only ever treat instances
// opaquely, invoking that slot to destroy them faithfully.
namespace {

// Global thread-local slot store. Values are keyed by (threadId, slot) so a
// single slot index yields an independent value per thread — exactly the
// semantics of MFC's CThreadSlotData.
struct Store {
    CRITICAL_SECTION cs;
    std::unordered_map<uint64_t, void*> values;
    DWORD nextSlot;
    Store() : nextSlot(1) { InitializeCriticalSection(&cs); }
    ~Store() {
        // Release the lock object. We intentionally do NOT walk `values` calling
        // each object's virtual destructor here: this runs at static-destruction
        // (DLL unload / process teardown), where those objects' vtables may live
        // in a module already unloaded — dispatching through them would crash.
        // Any surviving thread-local values are process-lifetime and reclaimed by
        // the OS at teardown. Per-slot values are freed in the exported destructor.
        DeleteCriticalSection(&cs);
    }
};

// C++11 guarantees thread-safe initialization of function-local statics.
Store& store() {
    static Store s;
    return s;
}

inline uint64_t makeKey(DWORD slot) {
    return (static_cast<uint64_t>(GetCurrentThreadId()) << 32)
         | static_cast<uint64_t>(slot);
}

DWORD allocSlot() {
    Store& s = store();
    EnterCriticalSection(&s.cs);
    DWORD slot = s.nextSlot++;
    LeaveCriticalSection(&s.cs);
    return slot;
}

// Destroy a CNoTrackObject value via its virtual (vector-)deleting destructor.
void destroyValue(void* val) {
    if (val == nullptr) return;
    void** vtbl = *reinterpret_cast<void***>(val);
    if (vtbl == nullptr) return;
    typedef void* (MS_ABI *DelDtor)(void*, unsigned);
    reinterpret_cast<DelDtor>(vtbl[0])(val, 1u); // flag bit0 => operator delete
}

} // namespace

// Symbol: ??1CThreadLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(void* pThis) {
    S* p = reinterpret_cast<S*>(pThis);
    if (p->m_nSlot != 0) {
        Store& s = store();
        EnterCriticalSection(&s.cs);
        const DWORD slot = p->m_nSlot;
        for (auto it = s.values.begin(); it != s.values.end(); ) {
            if (static_cast<DWORD>(it->first & 0xFFFFFFFFu) == slot) {
                destroyValue(it->second);
                it = s.values.erase(it);
            } else {
                ++it;
            }
        }
        LeaveCriticalSection(&s.cs);
        p->m_nSlot = 0;
    }
}

// Symbol: ?GetData@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
// CNoTrackObject* GetData(CNoTrackObject* (*pfnCreateObject)());
extern "C" void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
        void* pThis, void* pfnCreateObject) {
    S* p = reinterpret_cast<S*>(pThis);

    // Allocate a slot lazily on first use.
    if (p->m_nSlot == 0) {
        p->m_nSlot = allocSlot();
    }

    Store& s = store();
    const uint64_t k = makeKey(p->m_nSlot);

    EnterCriticalSection(&s.cs);
    auto it = s.values.find(k);
    void* val = (it != s.values.end()) ? it->second : nullptr;
    LeaveCriticalSection(&s.cs);

    // Create the per-thread value on first access.
    if (val == nullptr && pfnCreateObject != nullptr) {
        typedef void* (MS_ABI *CreateFn)();
        val = reinterpret_cast<CreateFn>(pfnCreateObject)();
        EnterCriticalSection(&s.cs);
        s.values[k] = val;
        LeaveCriticalSection(&s.cs);
    }
    return val;
}

// Symbol: ?GetDataNA@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@XZ
// CNoTrackObject* GetDataNA();  (no-allocate lookup)
extern "C" void* MS_ABI impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(
        void* pThis) {
    S* p = reinterpret_cast<S*>(pThis);
    if (p->m_nSlot == 0) {
        return nullptr;
    }
    Store& s = store();
    EnterCriticalSection(&s.cs);
    auto it = s.values.find(makeKey(p->m_nSlot));
    void* val = (it != s.values.end()) ? it->second : nullptr;
    LeaveCriticalSection(&s.cs);
    return val;
}
