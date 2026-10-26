#pragma once
// Shared internals of the former collections_cfixedalloc.cpp translation unit.
// Definitions live in detail/CFixedAllocSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cfixedalloc {} } }
using namespace openmfc::detail::cfixedalloc;
// CFixedAlloc - fixed-size block allocator (afxmem)
//
// CFixedAlloc is NOT a CObject-derived / DYNAMIC class: it has no base, no
// vtable, and exports only ctor/dtor/Alloc/Free/FreeAll (no CRuntimeClass,
// no GetRuntimeClass, no Serialize). So there is no DYNAMIC pattern to mirror.
// We use the plain-class MS_ABI thunk style used for non-virtual classes
// (e.g. phase4/src/filecore.cpp:721 impl__FreeExtra_CUIntArray...).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include "detail/CPlexSupport.h"

#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Faithful MSVC x64 layout for the CFixedAlloc allocator state (24 bytes).
//   offset 0:  UINT m_nAllocSize   (4 bytes)  -- size of each fixed block
//   offset 4:  UINT m_nBlockSize   (4 bytes)  -- nodes per plex
//   offset 8:  CPlex* m_pBlocks    (8 bytes)  -- allocated plex block list
//   offset 16: CNode* m_pNodeFree  (8 bytes)  -- free-list head
// Each free node's first 8 bytes hold the pNext pointer (CNode).
// Each plex block is { CPlex* pNext; } header followed by the raw data array
// (data() == header+1), identical to CList's CBlock pattern.
namespace openmfc { namespace detail { namespace cfixedalloc {
struct CFixedAllocState {
    struct CNode {
        CNode* pNext;
    };
    using CPlex = ::CPlex;

    unsigned int m_nAllocSize;
    unsigned int m_nBlockSize;
    CPlex*       m_pBlocks;
    CNode*       m_pNodeFree;
};
struct CFixedAllocSynchronizedState : CFixedAllocState {
    // The synchronized allocator is 64 bytes; its non-polymorphic prefix is
    // shared with CFixedAllocNoSync and CHandleMap's fixed-block storage.
    CRITICAL_SECTION m_lock;
};
} } }


namespace openmfc { namespace detail { namespace cfixedalloc {
void fa_construct(CFixedAllocState* p, unsigned int nAllocSize, unsigned int nBlockSize);
} } }

namespace openmfc { namespace detail { namespace cfixedalloc {
void fa_free_all(CFixedAllocState* p);
} } }

namespace openmfc { namespace detail { namespace cfixedalloc {
void* fa_alloc(CFixedAllocState* p);
} } }

namespace openmfc { namespace detail { namespace cfixedalloc {
void fa_free(CFixedAllocState* p, void* ptr);
} } }


// ---------------------------------------------------------------------------
// CFixedAlloc exports
// ---------------------------------------------------------------------------






// ---------------------------------------------------------------------------
// CFixedAllocNoSync exports (identical non-sync logic, distinct impl names)
// ---------------------------------------------------------------------------





