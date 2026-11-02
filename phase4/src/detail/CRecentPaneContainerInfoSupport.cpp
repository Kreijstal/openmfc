#include "CRecentPaneContainerInfoSupport.h"

// The list constructor at RVA 0xd17b0 (mfc140u) writes its head/tail/count/
// free-node/blocks/block-size stores at these offsets; afxtempl.h confirms the
// member order and INT_PTR types.
static_assert(offsetof(ListS, vfptr) == 0x00, "CList vfptr");
static_assert(offsetof(ListS, pNodeHead) == 0x08, "CList head");
static_assert(offsetof(ListS, pNodeTail) == 0x10, "CList tail");
static_assert(offsetof(ListS, nCount) == 0x18, "CList count");
static_assert(offsetof(ListS, pNodeFree) == 0x20, "CList free node");
static_assert(offsetof(ListS, pBlocks) == 0x28, "CList blocks");
static_assert(offsetof(ListS, nBlockSize) == 0x30, "CList block size");
static_assert(sizeof(ListS) == 0x38, "ListS must be 56 bytes");
static_assert(sizeof(S_Crecentpanecontainerinfo) == 0x68, "S_Crecentpanecontainerinfo must be size(104)");
static_assert(offsetof(S_Crecentpanecontainerinfo, m_rectDockedRect) == 8, "m_rectDockedRect @8");
static_assert(offsetof(S_Crecentpanecontainerinfo, m_lstRecentListOfBars) == 32, "list @32");
static_assert(offsetof(S_Crecentpanecontainerinfo, m_pRecentBarContainer) == 88, "container @88");
static_assert(offsetof(S_Crecentpanecontainerinfo, m_pRecentContainerOfTabWnd) == 96, "tabWnd @96");
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void list_removeall(S_Crecentpanecontainerinfo* s) {
    CNode* p = s->m_lstRecentListOfBars.pNodeHead;
    while (p) { CNode* nx = p->pNext; ::operator delete(p); p = nx; }
    s->m_lstRecentListOfBars.pNodeHead = nullptr;
    s->m_lstRecentListOfBars.pNodeTail = nullptr;
    s->m_lstRecentListOfBars.pNodeFree = nullptr;
    s->m_lstRecentListOfBars.pBlocks   = nullptr;
    s->m_lstRecentListOfBars.nCount    = 0;
}
void list_addtail(S_Crecentpanecontainerinfo* s, HWND h) {
    CNode* n = static_cast<CNode*>(::operator new(sizeof(CNode)));
    n->data = h;
    n->pNext = nullptr;
    n->pPrev = s->m_lstRecentListOfBars.pNodeTail;
    if (s->m_lstRecentListOfBars.pNodeTail)
        s->m_lstRecentListOfBars.pNodeTail->pNext = n;
    else
        s->m_lstRecentListOfBars.pNodeHead = n;
    s->m_lstRecentListOfBars.pNodeTail = n;
    s->m_lstRecentListOfBars.nCount++;
}
void list_init_empty(S_Crecentpanecontainerinfo* s) {
    s->m_lstRecentListOfBars.vfptr      = nullptr;
    s->m_lstRecentListOfBars.pNodeHead  = nullptr;
    s->m_lstRecentListOfBars.pNodeTail  = nullptr;
    s->m_lstRecentListOfBars.nCount     = 0;
    s->m_lstRecentListOfBars.pNodeFree  = nullptr;
    s->m_lstRecentListOfBars.pBlocks    = nullptr;
    s->m_lstRecentListOfBars.nBlockSize = 10;
}
void copy_from(S_Crecentpanecontainerinfo* dst, S_Crecentpanecontainerinfo* src) {
    dst->m_rectDockedRect          = src->m_rectDockedRect;
    dst->m_nRecentPercent          = src->m_nRecentPercent;
    dst->m_bIsRecentLeftBar        = src->m_bIsRecentLeftBar;
    dst->m_pRecentBarContainer     = src->m_pRecentBarContainer;
    dst->m_pRecentContainerOfTabWnd= src->m_pRecentContainerOfTabWnd;
    list_removeall(dst);
    for (CNode* p = src->m_lstRecentListOfBars.pNodeHead; p; p = p->pNext)
        list_addtail(dst, p->data);
}
void* MS_ABI vslot_GetRuntimeClass_Crecentpanecontainerinfo(void* /*pThis*/) {
    return &CObject::classCObject;
}
void* MS_ABI vslot_deleting_dtor(void* pThis, unsigned flags) {
    impl___1CRecentPaneContainerInfo__UEAA_XZ(pThis);
    if (flags & 1) ::operator delete(pThis);
    return pThis;
}
void MS_ABI vslot_Serialize_Crecentpanecontainerinfo(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI vslot_AssertValid_Crecentpanecontainerinfo(void* pThis) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vslot_Dump_Crecentpanecontainerinfo(void* pThis, void* /*pDC*/) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::Dump();
}
void MS_ABI vslot_StoreDockInfo(void* pThis, void* a, void* b, void* c) {
    impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(pThis, a, b, c);
}
void MS_ABI vslot_SetInfo(void* pThis, void* pSrc) {
    impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(pThis, pSrc);
}
} } }  // namespace openmfc::detail::crecentpanecontainerinfo
