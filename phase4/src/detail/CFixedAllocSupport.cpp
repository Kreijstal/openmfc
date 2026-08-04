#include "CFixedAllocSupport.h"

static_assert(sizeof(CFixedAllocState) == 24, "CFixedAlloc state must be 24 bytes");
namespace openmfc { namespace detail { namespace cfixedalloc {
void fa_construct(CFixedAllocState* p, unsigned int nAllocSize, unsigned int nBlockSize) {
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
void fa_free_all(CFixedAllocState* p) {
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
void* fa_alloc(CFixedAllocState* p) {
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
void fa_free(CFixedAllocState* p, void* ptr) {
    if (!p || !ptr) return;
    CFixedAllocState::CNode* pNode = (CFixedAllocState::CNode*)ptr;
    pNode->pNext   = p->m_pNodeFree;
    p->m_pNodeFree = pNode;
}
} } }  // namespace openmfc::detail::cfixedalloc
