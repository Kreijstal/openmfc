// CFixedAlloc - fixed-size block allocator (afxmem)
//
// CFixedAlloc is NOT a CObject-derived / DYNAMIC class: it has no base, no
// vtable, and exports only ctor/dtor/Alloc/Free/FreeAll (no CRuntimeClass,
// no GetRuntimeClass, no Serialize). So there is no DYNAMIC pattern to mirror.
// We use the plain-class MS_ABI thunk style used for non-virtual classes
// (e.g. phase4/src/filecore.cpp:721 impl__FreeExtra_CUIntArray...).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"

#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Faithful MSVC x64 layout for the CFixedAlloc allocator state (24 bytes).
//   offset 0:  UINT m_nAllocSize   (4 bytes)  -- size of each fixed block
//   offset 4:  UINT m_nBlockSize   (4 bytes)  -- nodes per plex
//   offset 8:  CNode* m_pNodeFree  (8 bytes)  -- free-list head
//   offset 16: CPlex* m_pBlocks    (8 bytes)  -- allocated plex block list
// Each free node's first 8 bytes hold the pNext pointer (CNode).
// Each plex block is { CPlex* pNext; } header followed by the raw data array
// (data() == header+1), identical to CList's CBlock pattern.
struct CFixedAllocState {
    struct CNode {
        CNode* pNext;
    };
    struct CPlex {
        CPlex* pNext;
        // followed in memory by m_nBlockSize * m_nAllocSize raw bytes
        void* data() { return this + 1; }
    };

    unsigned int m_nAllocSize;
    unsigned int m_nBlockSize;
    CNode*       m_pNodeFree;
    CPlex*       m_pBlocks;
};

static_assert(sizeof(CFixedAllocState) == 24, "CFixedAlloc state must be 24 bytes");

static void fa_construct(CFixedAllocState* p, unsigned int nAllocSize, unsigned int nBlockSize) {
    if (!p) return;
    // MFC asserts nAllocSize >= sizeof(CNode*); ensure a free node can hold a
    // pointer for the intrusive free list.
    if (nAllocSize < sizeof(CFixedAllocState::CNode*))
        nAllocSize = sizeof(CFixedAllocState::CNode*);
    if (nBlockSize < 1)
        nBlockSize = 1;
    p->m_nAllocSize = nAllocSize;
    p->m_nBlockSize = nBlockSize;
    p->m_pNodeFree  = nullptr;
    p->m_pBlocks    = nullptr;
}

static void fa_free_all(CFixedAllocState* p) {
    if (!p) return;
    CFixedAllocState::CPlex* pPlex = p->m_pBlocks;
    while (pPlex) {
        CFixedAllocState::CPlex* pNext = pPlex->pNext;
        std::free(pPlex);
        pPlex = pNext;
    }
    p->m_pNodeFree = nullptr;
    p->m_pBlocks   = nullptr;
}

static void* fa_alloc(CFixedAllocState* p) {
    if (!p) return nullptr;

    if (p->m_pNodeFree == nullptr) {
        // Allocate a new plex block holding m_nBlockSize nodes.
        size_t total = sizeof(CFixedAllocState::CPlex) +
                       (size_t)p->m_nBlockSize * (size_t)p->m_nAllocSize;
        CFixedAllocState::CPlex* pPlex =
            (CFixedAllocState::CPlex*)std::malloc(total);
        if (!pPlex) return nullptr;

        // Push plex onto the block list.
        pPlex->pNext   = p->m_pBlocks;
        p->m_pBlocks   = pPlex;

        // Chain every node in the new block onto the free list.
        char* pBytes = (char*)pPlex->data();
        CFixedAllocState::CNode* pNode =
            (CFixedAllocState::CNode*)pBytes;
        for (unsigned int i = 0; i < p->m_nBlockSize; ++i) {
            pNode->pNext   = p->m_pNodeFree;
            p->m_pNodeFree = pNode;
            pBytes += p->m_nAllocSize;
            pNode  = (CFixedAllocState::CNode*)pBytes;
        }
    }

    // Pop head of free list.
    CFixedAllocState::CNode* pHead = p->m_pNodeFree;
    p->m_pNodeFree = pHead->pNext;
    return pHead;
}

static void fa_free(CFixedAllocState* p, void* ptr) {
    if (!p || !ptr) return;
    CFixedAllocState::CNode* pNode = (CFixedAllocState::CNode*)ptr;
    pNode->pNext   = p->m_pNodeFree;
    p->m_pNodeFree = pNode;
}

} // namespace

// ---------------------------------------------------------------------------
// CFixedAlloc exports
// ---------------------------------------------------------------------------

// Symbol: ??0CFixedAlloc@@QEAA@II@Z
extern "C" void MS_ABI impl___0CFixedAlloc__QEAA_II_Z(CFixedAllocState* pThis, unsigned int nAllocSize, unsigned int nBlockSize) {
    fa_construct(pThis, nAllocSize, nBlockSize);
}

// Symbol: ??1CFixedAlloc@@QEAA@XZ
extern "C" void MS_ABI impl___1CFixedAlloc__QEAA_XZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}

// Symbol: ?Alloc@CFixedAlloc@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Alloc_CFixedAlloc__QEAAPEAXXZ(CFixedAllocState* pThis) {
    return fa_alloc(pThis);
}

// Symbol: ?Free@CFixedAlloc@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__Free_CFixedAlloc__QEAAXPEAX_Z(CFixedAllocState* pThis, void* p) {
    fa_free(pThis, p);
}

// Symbol: ?FreeAll@CFixedAlloc@@QEAAXXZ
extern "C" void MS_ABI impl__FreeAll_CFixedAlloc__QEAAXXZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}

// ---------------------------------------------------------------------------
// CFixedAllocNoSync exports (identical non-sync logic, distinct impl names)
// ---------------------------------------------------------------------------

// Symbol: ??0CFixedAllocNoSync@@QEAA@II@Z
extern "C" void MS_ABI impl___0CFixedAllocNoSync__QEAA_II_Z(CFixedAllocState* pThis, unsigned int nAllocSize, unsigned int nBlockSize) {
    fa_construct(pThis, nAllocSize, nBlockSize);
}

// Symbol: ??1CFixedAllocNoSync@@QEAA@XZ
extern "C" void MS_ABI impl___1CFixedAllocNoSync__QEAA_XZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}

// Symbol: ?Alloc@CFixedAllocNoSync@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(CFixedAllocState* pThis) {
    return fa_alloc(pThis);
}

// Symbol: ?Free@CFixedAllocNoSync@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__Free_CFixedAllocNoSync__QEAAXPEAX_Z(CFixedAllocState* pThis, void* p) {
    fa_free(pThis, p);
}

// Symbol: ?FreeAll@CFixedAllocNoSync@@QEAAXXZ
extern "C" void MS_ABI impl__FreeAll_CFixedAllocNoSync__QEAAXXZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}
