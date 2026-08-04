#include "CMFCDynamicLayoutDataSupport.h"

static_assert(sizeof(S_Cmfcdynamiclayoutdata) == 56, "CMFCDynamicLayoutData must be 56 bytes");
static_assert(offsetof(S_Cmfcdynamiclayoutdata, m_pNodeHead) == 8,  "m_pNodeHead @8");
static_assert(offsetof(S_Cmfcdynamiclayoutdata, m_nCount)    == 24, "m_nCount @24");
static_assert(offsetof(S_Cmfcdynamiclayoutdata, m_pBlocks)   == 40, "m_pBlocks @40");
static_assert(sizeof(CNode) == 32, "CNode 32 bytes");
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
CNode* ListNewNode(S_Cmfcdynamiclayoutdata* s, CNode* pPrev, CNode* pNext) {
    if (s->m_nBlockSize == 0)
        s->m_nBlockSize = 10; // CList default block size

    if (s->m_pNodeFree == NULL) {
        const UINT n = (UINT)s->m_nBlockSize;
        CPlex* pBlock = (CPlex*)std::malloc(sizeof(CPlex) + (size_t)n * sizeof(CNode));
        if (pBlock == NULL)
            return NULL;
        pBlock->pNext = s->m_pBlocks;
        s->m_pBlocks = pBlock;

        CNode* pNode = (CNode*)(void*)(pBlock + 1);
        pNode += n - 1;
        for (UINT i = 0; i < n; ++i, --pNode) {
            pNode->pNext = s->m_pNodeFree;
            s->m_pNodeFree = pNode;
        }
    }

    CNode* pNode = s->m_pNodeFree;
    s->m_pNodeFree = s->m_pNodeFree->pNext;
    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    ++s->m_nCount;
    return pNode;
}
void ListAddTail(S_Cmfcdynamiclayoutdata* s, const Item& item) {
    CNode* pNew = ListNewNode(s, s->m_pNodeTail, NULL);
    if (pNew == NULL)
        return;
    pNew->data = item;
    if (s->m_pNodeTail != NULL)
        s->m_pNodeTail->pNext = pNew;
    else
        s->m_pNodeHead = pNew;
    s->m_pNodeTail = pNew;
}
void ListRemoveAll(S_Cmfcdynamiclayoutdata* s) {
    s->m_nCount     = 0;
    s->m_pNodeHead  = NULL;
    s->m_pNodeTail  = NULL;
    s->m_pNodeFree  = NULL;
    CPlex* p = s->m_pBlocks;
    while (p != NULL) {
        CPlex* pNext = p->pNext;
        std::free(p);
        p = pNext;
    }
    s->m_pBlocks = NULL;
}
} } }  // namespace openmfc::detail::cmfcdynamiclayoutdata
