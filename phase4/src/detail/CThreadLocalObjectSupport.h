#pragma once
// Shared internals of the former global_cthreadlocalobject.cpp translation unit.
// Definitions live in detail/CThreadLocalObjectSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cthreadlocalobject {} } }
using namespace openmfc::detail::cthreadlocalobject;
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
struct S_Cthreadlocalobject {
    DWORD m_nSlot;   // 0 == unallocated
};

// A CNoTrackObject is a polymorphic type with a virtual destructor; slot 0 of
// its vtable is the vector-deleting-destructor. We only ever treat instances
// opaquely, invoking that slot to destroy them faithfully.

// Global thread-local slot store. Values are keyed by (threadId, slot) so a
// single slot index yields an independent value per thread — exactly the
// semantics of MFC's CThreadSlotData.
namespace openmfc { namespace detail { namespace cthreadlocalobject {
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
} } }

// C++11 guarantees thread-safe initialization of function-local statics.
namespace openmfc { namespace detail { namespace cthreadlocalobject {
Store& store();
} } }

namespace openmfc { namespace detail { namespace cthreadlocalobject {
inline uint64_t makeKey(DWORD slot) {
    return (static_cast<uint64_t>(GetCurrentThreadId()) << 32)
         | static_cast<uint64_t>(slot);
}
} } }

namespace openmfc { namespace detail { namespace cthreadlocalobject {
DWORD allocSlot();
} } }

// Destroy a CNoTrackObject value via its virtual (vector-)deleting destructor.
namespace openmfc { namespace detail { namespace cthreadlocalobject {
void destroyValue(void* val);
} } }




