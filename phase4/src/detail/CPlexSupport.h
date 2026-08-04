#pragma once
// Shared internals of the former collections_cplex.cpp translation unit.
// Definitions live in detail/CPlexSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cplex {} } }
using namespace openmfc::detail::cplex;
// CPlex: MFC's fixed-block "plex" allocator node, used as the backing store
// for collection bucket arrays (CMapStringToOb, CList, etc.).
//
// Authoritative layout (verified against export signatures and standard MFC
// plex.cpp semantics):
//
//     struct CPlex { CPlex* pNext; };   // sizeof == 8 on x64
//
// CPlex is NOT a CObject-derived class: it has no base, no vtable, no
// CRuntimeClass, and no virtual methods. The exported symbols confirm this
// (S = static factory, Q = public non-virtual; no U = virtual). The element
// data for a block immediately follows the 8-byte header in the SAME
// allocation, i.e. data() == (BYTE*)(this + 1).
//
// Exports implemented here (def maps mangled = impl):
//   ?Create@CPlex@@SAPEAU1@AEAPEAU1@_K1@Z = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z
//   ?FreeDataChain@CPlex@@QEAAXXZ        = impl__FreeDataChain_CPlex__QEAAXXZ

// CPlex is a standalone, non-polymorphic helper (no CObject base, no vtable,
// no CRuntimeClass), so — like phase4/src/collections_csimplelist.cpp — this
// TU includes only <windows.h> rather than the heavy MFC class headers. The
// build still picks it up automatically via the collections_*.cpp shard glob.
#include <windows.h>
#include <cstdlib>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// The real CPlex node. Single 8-byte forward pointer; the element block of
// nMax*cbElement bytes is carved out of the tail of the same allocation.
struct CPlex {
    CPlex* pNext;

    // data() points just past the header, where the element bytes live.
    void* data() { return this + 1; }
};


// Round/guard against multiplication overflow when computing the element area
// size. Returns false if nMax*cbElement (or +header) would overflow size_t.
namespace openmfc { namespace detail { namespace cplex {
inline bool ComputeBlockSize(unsigned __int64 nMax,
                             unsigned __int64 cbElement,
                             std::size_t& outBytes) {
    if (nMax == 0 || cbElement == 0)
        return false;
    // Overflow check for nMax * cbElement.
    if (nMax > (SIZE_MAX / cbElement))
        return false;
    std::size_t elemBytes = static_cast<std::size_t>(nMax * cbElement);
    if (elemBytes > SIZE_MAX - sizeof(CPlex))
        return false;
    outBytes = sizeof(CPlex) + elemBytes;
    return true;
}
} } }


