// OpenMFC real implementation: CControlSiteFactoryMgr
//
// CControlSiteFactoryMgr manages a list of IControlSiteFactory* objects and
// dispatches control-site creation to them (falling back to a default OLE
// control-site factory).  It derives from CNoTrackObject (single vfptr) and
// owns a CList<IControlSiteFactory*,IControlSiteFactory*> plus a default
// factory pointer.
//
// Harvested layout (size 72):
//   0  : {vfptr}   (from CNoTrackObject)
//   8  : m_lstFactory  (CList, 56 bytes)
//   64 : m_pOleControlSiteDefaultFactory  (COleControlSiteFactory*)

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

// One node of an MFC CList<TYPE,ARG_TYPE>: { CNode* pNext; CNode* pPrev; TYPE data; }
struct CNode {
    CNode* pNext;
    CNode* pPrev;
    void*  data;   // IControlSiteFactory*
};

// Layout-faithful mirror of MFC CList<TYPE,ARG_TYPE> (derives from CObject).
//   0  : {vfptr}
//   8  : m_pNodeHead
//   16 : m_pNodeTail
//   24 : m_nCount     (INT_PTR)
//   32 : m_pNodeFree
//   40 : m_pBlocks    (CPlex*)
//   48 : m_nBlockSize (INT_PTR)
struct CListMirror {
    void*      vfptr;
    CNode*     m_pNodeHead;
    CNode*     m_pNodeTail;
    long long  m_nCount;
    CNode*     m_pNodeFree;
    void*      m_pBlocks;
    long long  m_nBlockSize;
};

struct S {
    void*        vfptr;                             // 0
    CListMirror  m_lstFactory;                      // 8
    void*        m_pOleControlSiteDefaultFactory;   // 64
};

static_assert(sizeof(CListMirror) == 56, "CList mirror must be 56 bytes");
static_assert(sizeof(S) == 72, "CControlSiteFactoryMgr must be 72 bytes");
static_assert(offsetof(S, m_lstFactory) == 8, "m_lstFactory offset");
static_assert(offsetof(S, m_pOleControlSiteDefaultFactory) == 64, "default factory offset");

// Vtable slot signature for the single pure-virtual IControlSiteFactory method:
//   virtual COleControlSite* CreateSite(COleControlContainer*, const CControlCreationInfo&);
// The factory objects are real (MSVC-compiled), so dispatch through their
// vtable with the MS x64 ABI.  const-ref arrives as a pointer.
typedef void* (MS_ABI* PFN_FactoryCreateSite)(void* self, void* pCtrlCont, const void* creationInfo);

// --- CList helpers (faithful AddTail / Find / RemoveAt semantics) ----------

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

// Remove the first node whose data equals value; returns TRUE if removed.
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

} // anonymous namespace

// Forward declaration: the real (non-deleting) destructor thunk.
extern "C" void MS_ABI impl___1CControlSiteFactoryMgr__UEAA_XZ(void* pThis);

namespace {

// Single vtable slot: the vector-deleting destructor (MSVC folds ~dtor here).
void* MS_ABI vdtor(void* p, unsigned flags) {
    impl___1CControlSiteFactoryMgr__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}

void* const g_CControlSiteFactoryMgr_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor),
};

} // anonymous namespace

// Symbol: ??0CControlSiteFactoryMgr@@QEAA@XZ
extern "C" void* MS_ABI impl___0CControlSiteFactoryMgr__QEAA_XZ(void* pThis) {
    S* p = static_cast<S*>(pThis);
    p->vfptr = (void*)&g_CControlSiteFactoryMgr_vtbl[0];

    // Construct the embedded CList: empty, default block size 10.
    p->m_lstFactory.vfptr        = nullptr;
    p->m_lstFactory.m_pNodeHead  = nullptr;
    p->m_lstFactory.m_pNodeTail  = nullptr;
    p->m_lstFactory.m_nCount     = 0;
    p->m_lstFactory.m_pNodeFree  = nullptr;
    p->m_lstFactory.m_pBlocks    = nullptr;
    p->m_lstFactory.m_nBlockSize = 10;

    p->m_pOleControlSiteDefaultFactory = nullptr;
    return pThis;
}

// Symbol: ??1CControlSiteFactoryMgr@@UEAA@XZ
extern "C" void MS_ABI impl___1CControlSiteFactoryMgr__UEAA_XZ(void* pThis) {
    S* p = static_cast<S*>(pThis);
    // Free every list node (RemoveAll); the factories themselves are not owned.
    list_destroy(&p->m_lstFactory);
    // The default factory is owned by the framework, not deleted here.
    p->m_pOleControlSiteDefaultFactory = nullptr;
}

// Symbol: ?RegisterSiteFactory@CControlSiteFactoryMgr@@QEAAHPEAVIControlSiteFactory@@@Z
extern "C" BOOL MS_ABI impl__RegisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(
        void* pThis, void* pFactory) {
    if (pFactory == nullptr)
        return FALSE;
    S* p = static_cast<S*>(pThis);
    list_add_tail(&p->m_lstFactory, pFactory);
    return TRUE;
}

// Symbol: ?UnregisterSiteFactory@CControlSiteFactoryMgr@@QEAAHPEAVIControlSiteFactory@@@Z
extern "C" BOOL MS_ABI impl__UnregisterSiteFactory_CControlSiteFactoryMgr__QEAAHPEAVIControlSiteFactory___Z(
        void* pThis, void* pFactory) {
    if (pFactory == nullptr)
        return FALSE;
    S* p = static_cast<S*>(pThis);
    return list_remove_value(&p->m_lstFactory, pFactory);
}

// Symbol: ?CreateSite@CControlSiteFactoryMgr@@QEAAPEAVCOleControlSite@@PEAVCOleControlContainer@@AEBVCControlCreationInfo@@@Z
extern "C" void* MS_ABI impl__CreateSite_CControlSiteFactoryMgr__QEAAPEAVCOleControlSite__PEAVCOleControlContainer__AEBVCControlCreationInfo___Z(
        void* pThis, void* pCtrlCont, const void* creationInfo) {
    S* p = static_cast<S*>(pThis);

    // Give each registered factory a chance to create the site; first non-null wins.
    for (CNode* node = p->m_lstFactory.m_pNodeHead; node != nullptr; node = node->pNext) {
        void* pFactory = node->data;
        if (pFactory == nullptr)
            continue;
        void** vtbl = *static_cast<void***>(pFactory);
        PFN_FactoryCreateSite pfn = reinterpret_cast<PFN_FactoryCreateSite>(vtbl[0]);
        void* pSite = pfn(pFactory, pCtrlCont, creationInfo);
        if (pSite != nullptr)
            return pSite;
    }

    // Fall back to the default OLE control-site factory when present.
    if (p->m_pOleControlSiteDefaultFactory != nullptr) {
        void* pDefault = p->m_pOleControlSiteDefaultFactory;
        void** vtbl = *static_cast<void***>(pDefault);
        PFN_FactoryCreateSite pfn = reinterpret_cast<PFN_FactoryCreateSite>(vtbl[0]);
        return pfn(pDefault, pCtrlCont, creationInfo);
    }
    return nullptr;
}
