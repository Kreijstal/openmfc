// OpenMFC real implementation of CRecentPaneContainerInfo (mfc140u).
// Layout-faithful; MS_ABI thunks aliased from the .def as impl_ symbols.
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// --------------------------------------------------------------------------
// Layout-faithful mirror. Harvested size(104), CObject base (vfptr @0).
//   0   {vfptr}
//   8   CRect m_rectDockedRect            (RECT, 16 bytes)
//  24   int   m_nRecentPercent
//  28   BOOL  m_bIsRecentLeftBar
//  32   CList<HWND,HWND> m_lstRecentListOfBars  (56 bytes, own CObject base)
//  88   CPaneContainer* m_pRecentBarContainer
//  96   CPaneContainer* m_pRecentContainerOfTabWnd
// --------------------------------------------------------------------------
namespace {

struct CNode {
    CNode* pNext;
    CNode* pPrev;
    HWND   data;
};

// Mirror of CList<HWND,HWND> (derives from CObject).
struct ListS {
    void*   vfptr;        // +0  CObject vfptr
    CNode*  pNodeHead;    // +8
    CNode*  pNodeTail;    // +16
    INT_PTR nCount;       // +24
    CNode*  pNodeFree;    // +32
    void*   pBlocks;      // +40  CPlex*
    int     nBlockSize;   // +48
};

struct S {
    void*   vfptr;                       // 0
    RECT    m_rectDockedRect;            // 8
    int     m_nRecentPercent;           // 24
    int     m_bIsRecentLeftBar;         // 28  (BOOL)
    ListS   m_lstRecentListOfBars;      // 32
    void*   m_pRecentBarContainer;      // 88
    void*   m_pRecentContainerOfTabWnd; // 96
};

static_assert(sizeof(ListS) == 56, "ListS must be 56 bytes");
static_assert(sizeof(S) == 104, "S must be size(104)");
static_assert(offsetof(S, m_rectDockedRect) == 8, "m_rectDockedRect @8");
static_assert(offsetof(S, m_lstRecentListOfBars) == 32, "list @32");
static_assert(offsetof(S, m_pRecentBarContainer) == 88, "container @88");
static_assert(offsetof(S, m_pRecentContainerOfTabWnd) == 96, "tabWnd @96");

// ----- minimal but faithful CList helpers (nodes we own end-to-end) --------
static void list_removeall(S* s) {
    CNode* p = s->m_lstRecentListOfBars.pNodeHead;
    while (p) { CNode* nx = p->pNext; ::operator delete(p); p = nx; }
    s->m_lstRecentListOfBars.pNodeHead = nullptr;
    s->m_lstRecentListOfBars.pNodeTail = nullptr;
    s->m_lstRecentListOfBars.pNodeFree = nullptr;
    s->m_lstRecentListOfBars.pBlocks   = nullptr;
    s->m_lstRecentListOfBars.nCount    = 0;
}

static void list_addtail(S* s, HWND h) {
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

static void list_init_empty(S* s) {
    s->m_lstRecentListOfBars.vfptr      = nullptr;
    s->m_lstRecentListOfBars.pNodeHead  = nullptr;
    s->m_lstRecentListOfBars.pNodeTail  = nullptr;
    s->m_lstRecentListOfBars.nCount     = 0;
    s->m_lstRecentListOfBars.pNodeFree  = nullptr;
    s->m_lstRecentListOfBars.pBlocks    = nullptr;
    s->m_lstRecentListOfBars.nBlockSize = 10;
}

// Copy all persistent state from src into dst (shared by operator= / SetInfo).
static void copy_from(S* dst, S* src) {
    dst->m_rectDockedRect          = src->m_rectDockedRect;
    dst->m_nRecentPercent          = src->m_nRecentPercent;
    dst->m_bIsRecentLeftBar        = src->m_bIsRecentLeftBar;
    dst->m_pRecentBarContainer     = src->m_pRecentBarContainer;
    dst->m_pRecentContainerOfTabWnd= src->m_pRecentContainerOfTabWnd;
    list_removeall(dst);
    for (CNode* p = src->m_lstRecentListOfBars.pNodeHead; p; p = p->pNext)
        list_addtail(dst, p->data);
}

} // namespace

// forward decls of exported thunks used by the vtable
extern "C" void  MS_ABI impl___1CRecentPaneContainerInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(void* pThis, void* pSrc);
extern "C" void  MS_ABI impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
    void* pThis, void* pRecentContainer, void* pBar, void* pTabbedBar);

// --------------------------------------------------------------------------
// vtable (exact harvested slot order)
//   0 CObject::GetRuntimeClass
//   1 {deleting dtor}
//   2 CObject::Serialize
//   3 CObject::AssertValid
//   4 CObject::Dump
//   5 CRecentPaneContainerInfo::StoreDockInfo
//   6 CRecentPaneContainerInfo::SetInfo
// --------------------------------------------------------------------------
namespace {

void* MS_ABI vslot_GetRuntimeClass(void* /*pThis*/) {
    // No RTTI descriptor available in this translation unit; CObject default.
    return nullptr;
}
void* MS_ABI vslot_deleting_dtor(void* pThis, unsigned flags) {
    impl___1CRecentPaneContainerInfo__UEAA_XZ(pThis);
    if (flags & 1) ::operator delete(pThis);
    return pThis;
}
void MS_ABI vslot_Serialize(void* /*pThis*/, void* /*pAr*/) {}
void MS_ABI vslot_AssertValid(void* /*pThis*/) {}
void MS_ABI vslot_Dump(void* /*pThis*/, void* /*pDC*/) {}
void MS_ABI vslot_StoreDockInfo(void* pThis, void* a, void* b, void* c) {
    impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(pThis, a, b, c);
}
void MS_ABI vslot_SetInfo(void* pThis, void* pSrc) {
    impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(pThis, pSrc);
}

void* const g_CRecentPaneContainerInfo_vtbl[7] = {
    (void*)&vslot_GetRuntimeClass,
    (void*)&vslot_deleting_dtor,
    (void*)&vslot_Serialize,
    (void*)&vslot_AssertValid,
    (void*)&vslot_Dump,
    (void*)&vslot_StoreDockInfo,
    (void*)&vslot_SetInfo,
};

} // namespace

// --------------------------------------------------------------------------
// exported thunks
// --------------------------------------------------------------------------

// Symbol: ?Init@CRecentPaneContainerInfo@@QEAAXXZ
extern "C" void MS_ABI impl__Init_CRecentPaneContainerInfo__QEAAXXZ(void* pThis) {
    S* s = static_cast<S*>(pThis);
    // Verified byte-exact against real mfc140u (differential dump): the docked
    // rect defaults to {0,0,30,30}, recent percent to 50, left-bar flag TRUE.
    s->m_rectDockedRect.left = s->m_rectDockedRect.top = 0;
    s->m_rectDockedRect.right = s->m_rectDockedRect.bottom = 30;
    s->m_nRecentPercent = 50;
    s->m_bIsRecentLeftBar = TRUE;
    s->m_pRecentBarContainer = nullptr;
    s->m_pRecentContainerOfTabWnd = nullptr;
    list_removeall(s);
}

// Symbol: ??0CRecentPaneContainerInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CRecentPaneContainerInfo__QEAA_XZ(void* pThis) {
    S* s = static_cast<S*>(pThis);
    s->vfptr = (void*)&g_CRecentPaneContainerInfo_vtbl[0];
    list_init_empty(s);
    impl__Init_CRecentPaneContainerInfo__QEAAXXZ(pThis);
    return pThis;
}

// Symbol: ??1CRecentPaneContainerInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecentPaneContainerInfo__UEAA_XZ(void* pThis) {
    S* s = static_cast<S*>(pThis);
    list_removeall(s);
}

// Symbol: ??4CRecentPaneContainerInfo@@QEAAAEAV0@AEAV0@@Z
extern "C" void* MS_ABI impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(void* pThis, void* pSrc) {
    S* s = static_cast<S*>(pThis);
    S* src = static_cast<S*>(pSrc);
    if (s != src) copy_from(s, src);
    return pThis;
}

// Symbol: ?SetInfo@CRecentPaneContainerInfo@@UEAAXAEAV1@@Z
extern "C" void MS_ABI impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(void* pThis, void* pSrc) {
    S* s = static_cast<S*>(pThis);
    S* src = static_cast<S*>(pSrc);
    if (s != src) copy_from(s, src);
}

// Symbol: ?StoreDockInfo@CRecentPaneContainerInfo@@UEAAXPEAVCPaneContainer@@PEAVCDockablePane@@1@Z
// Records the recent container of the pane. The rect/percent/sibling-list
// derivation requires CPaneContainer/CDockablePane internals not modeled here,
// so only the container linkage is captured faithfully (see notes).
extern "C" void MS_ABI impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
    void* pThis, void* pRecentContainer, void* /*pBar*/, void* /*pTabbedBar*/) {
    S* s = static_cast<S*>(pThis);
    s->m_pRecentBarContainer = pRecentContainer;
}
