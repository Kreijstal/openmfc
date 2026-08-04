#include "CControlSiteFactoryMgrSupport.h"

static_assert(sizeof(CListMirror) == 56, "CList mirror must be 56 bytes");
static_assert(sizeof(S_Ccontrolsitefactorymgr) == 72, "CControlSiteFactoryMgr must be 72 bytes");
static_assert(offsetof(S_Ccontrolsitefactorymgr, m_lstFactory) == 8, "m_lstFactory offset");
static_assert(offsetof(S_Ccontrolsitefactorymgr, m_pOleControlSiteDefaultFactory) == 64, "default factory offset");
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
void list_add_tail(CListMirror* pl, void* value) {
    CNode* pNode = static_cast<CNode*>(::operator new(sizeof(CNode)));
    pNode->data  = value;
    pNode->pNext = nullptr;
    pNode->pPrev = pl->m_pNodeTail;
    if (pl->m_pNodeTail != nullptr)
        pl->m_pNodeTail->pNext = pNode;
    else
        pl->m_pNodeHead = pNode;
    pl->m_pNodeTail = pNode;
    pl->m_nCount++;
}
BOOL list_remove_value(CListMirror* pl, void* value) {
    for (CNode* p = pl->m_pNodeHead; p != nullptr; p = p->pNext) {
        if (p->data == value) {
            if (p->pPrev != nullptr) p->pPrev->pNext = p->pNext;
            else                     pl->m_pNodeHead = p->pNext;
            if (p->pNext != nullptr) p->pNext->pPrev = p->pPrev;
            else                     pl->m_pNodeTail = p->pPrev;
            ::operator delete(p);
            pl->m_nCount--;
            return TRUE;
        }
    }
    return FALSE;
}
void list_destroy(CListMirror* pl) {
    CNode* p = pl->m_pNodeHead;
    while (p != nullptr) {
        CNode* pNext = p->pNext;
        ::operator delete(p);
        p = pNext;
    }
    pl->m_pNodeHead = nullptr;
    pl->m_pNodeTail = nullptr;
    pl->m_pNodeFree = nullptr;
    pl->m_pBlocks   = nullptr;
    pl->m_nCount    = 0;
}
void* MS_ABI vdtor_Ccontrolsitefactorymgr(void* p, unsigned flags) {
    impl___1CControlSiteFactoryMgr__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}
} } }  // namespace openmfc::detail::ccontrolsitefactorymgr
