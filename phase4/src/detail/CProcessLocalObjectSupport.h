#pragma once
// Shared internals of the former global_cprocesslocalobject.cpp translation unit.
// Definitions live in detail/CProcessLocalObjectSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cprocesslocalobject {} } }
using namespace openmfc::detail::cprocesslocalobject;
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


// Layout-faithful mirror of CProcessLocalObject.
namespace openmfc { namespace detail { namespace cprocesslocalobject {
struct S_Cprocesslocalobject {
    void* m_pObject;   // +0: owned CNoTrackObject*
};
} } }


// Process-wide guard for the lazy-create double-checked path. The real MFC uses
// AfxLockGlobals(CRIT_PROCESSLOCAL); an SRWLOCK needs no explicit init and gives
// the same single-writer serialization for construction.
namespace openmfc { namespace detail { namespace cprocesslocalobject {
extern SRWLOCK g_processLocalLock;
} } }

// CNoTrackObject is polymorphic (virtual dtor at vtable slot 0). Deleting it via
// the vector-deleting-destructor with flag&1 runs its dtor and frees storage
// through CNoTrackObject::operator delete.
namespace openmfc { namespace detail { namespace cprocesslocalobject {
typedef void* (MS_ABI *DeletingDtor)(void*, unsigned);
} } }

// Factory pointer type: CNoTrackObject* (*)()
namespace openmfc { namespace detail { namespace cprocesslocalobject {
typedef void* (MS_ABI *CreateFn)();
} } }



