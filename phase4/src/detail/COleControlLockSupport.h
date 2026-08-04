#pragma once
// Shared internals of the former global_colecontrollock.cpp translation unit.
// Definitions live in detail/COleControlLockSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colecontrollock {} } }
using namespace openmfc::detail::colecontrollock;
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


namespace openmfc { namespace detail { namespace colecontrollock {
struct S_Colecontrollock {
    void*  vfptr;             // 0
    GUID   m_clsid;           // 8
    void*  m_pClassFactory;   // 24  (LPCLASSFACTORY)
    void*  m_pNextLock;       // 32  (COleControlLock*)
};
} } }



// Forward decls of the exported thunks so the vtable can reference them.
extern "C" void  MS_ABI impl___1COleControlLock__UEAA_XZ(void* pThis);


// Vector-deleting-destructor wrapper occupying the single vtable slot
// (COleControlLock::{dtor}). Runs the real destructor logic, then frees
// storage when the low bit of the flags is set (delete[] / delete semantics).
namespace openmfc { namespace detail { namespace colecontrollock {
void* MS_ABI vdtor_Colecontrollock(void* p, unsigned flags);
} } }

namespace openmfc { namespace detail { namespace colecontrollock {
extern void* const g_COleControlLock_vtbl[1];
} } }



