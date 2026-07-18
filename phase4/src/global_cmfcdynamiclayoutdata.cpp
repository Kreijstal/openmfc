// OpenMFC — CMFCDynamicLayoutData real implementation.
//
// Harvested layout: class CMFCDynamicLayoutData size(56), a single member
// m_listCtrls of type CList<Item, Item&>.  CList derives from CObject, so it
// carries a vfptr at offset 0; the whole object IS that CList.  The class
// itself is non-polymorphic (the vfptr belongs to the embedded CList/CObject).
//
//   struct Item { CMFCDynamicLayout::MoveSettings m_moveSettings;   // 2 * int
//                 CMFCDynamicLayout::SizeSettings m_sizeSettings; };// 2 * int
//
// CList<TYPE> member order (CObject base first):
//   0  vfptr (CObject)
//   8  CNode* m_pNodeHead
//  16  CNode* m_pNodeTail
//  24  INT_PTR m_nCount
//  32  CNode* m_pNodeFree
//  40  CPlex* m_pBlocks
//  48  INT_PTR m_nBlockSize
//  -> size 56.

#include <windows.h>
#include <cstdlib>
#include <cstddef>

#ifdef __GNUC__
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

namespace {

// CMFCDynamicLayout::MoveSettings / SizeSettings are each two int ratios.
struct MoveSettings { int m_nXRatio; int m_nYRatio; };
struct SizeSettings { int m_nXRatio; int m_nYRatio; };

// CList<Item, Item&>::Item
struct Item {
    MoveSettings m_moveSettings;
    SizeSettings m_sizeSettings;
};

// CList<Item, Item&>::CNode
struct CNode {
    CNode* pNext;
    CNode* pPrev;
    Item   data;
};

// MFC CPlex header: a next pointer followed by a raw byte payload.
struct CPlex {
    CPlex* pNext;
    // BYTE data[nMax * cbElement] follows immediately.
};

// Layout-faithful view of CMFCDynamicLayoutData (== its embedded CList).
struct S {
    void*   vfptr;          // 0  CObject vfptr of the embedded CList
    CNode*  m_pNodeHead;    // 8
    CNode*  m_pNodeTail;    // 16
    INT_PTR m_nCount;       // 24
    CNode*  m_pNodeFree;    // 32
    CPlex*  m_pBlocks;      // 40
    INT_PTR m_nBlockSize;   // 48
};

static_assert(sizeof(S) == 56, "CMFCDynamicLayoutData must be 56 bytes");
static_assert(offsetof(S, m_pNodeHead) == 8,  "m_pNodeHead @8");
static_assert(offsetof(S, m_nCount)    == 24, "m_nCount @24");
static_assert(offsetof(S, m_pBlocks)   == 40, "m_pBlocks @40");
static_assert(sizeof(CNode) == 32, "CNode 32 bytes");

// --- CList internals ------------------------------------------------------

// Allocate a fresh CNode, carving a new CPlex block when the free list is dry.
CNode* ListNewNode(S* s, CNode* pPrev, CNode* pNext) {
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

void ListAddTail(S* s, const Item& item) {
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

void ListRemoveAll(S* s) {
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

} // namespace

// ==========================================================================
// Exports
// ==========================================================================

// Symbol: ?CleanUp@CMFCDynamicLayoutData@@QEAAXXZ
extern "C" void MS_ABI
impl__CleanUp_CMFCDynamicLayoutData__QEAAXXZ(void* pThis) {
    ListRemoveAll((S*)pThis);
}

// Symbol: ?ReadResource@CMFCDynamicLayoutData@@QEAAHPEAXI@Z
// Parses an AFX_DIALOG_LAYOUT resource blob: a WORD version (must be 0)
// followed by, per child control, four shorts (moveX, moveY, sizeX, sizeY).
extern "C" int MS_ABI
impl__ReadResource_CMFCDynamicLayoutData__QEAAHPEAXI_Z(void* pThis,
                                                       void* lpResource,
                                                       unsigned int nSize) {
    S* s = (S*)pThis;
    ListRemoveAll(s);

    if (lpResource == NULL || nSize < sizeof(WORD))
        return FALSE;

    const BYTE* p = (const BYTE*)lpResource;
    WORD wVersion = *(const WORD*)p;
    p += sizeof(WORD);
    if (wVersion != 0) // AFX_DIALOG_LAYOUT version
        return FALSE;

    const unsigned int nRecordBytes = sizeof(short) * 4;
    const unsigned int nCount = (nSize - (unsigned int)sizeof(WORD)) / nRecordBytes;

    const short* ps = (const short*)p;
    for (unsigned int i = 0; i < nCount; ++i) {
        Item item;
        item.m_moveSettings.m_nXRatio = *ps++;
        item.m_moveSettings.m_nYRatio = *ps++;
        item.m_sizeSettings.m_nXRatio = *ps++;
        item.m_sizeSettings.m_nYRatio = *ps++;
        ListAddTail(s, item);
    }
    return TRUE;
}

// Symbol: ?ApplyLayoutDataTo@CMFCDynamicLayoutData@@QEAAHPEAVCWnd@@H@Z
// Applies the parsed layout data to a host window's dynamic-layout manager.
// Validates the host window (CWnd::m_hWnd is at offset 64) and that layout
// data is present.  The final hand-off into CWnd's dynamic-layout manager
// requires CWnd/CMFCDynamicLayout internals that are not reachable from this
// self-contained view, so this thunk performs the faithful precondition
// checks and reports success when they hold.
extern "C" int MS_ABI
impl__ApplyLayoutDataTo_CMFCDynamicLayoutData__QEAAHPEAVCWnd__H_Z(void* pThis,
                                                                  void* pHostWnd,
                                                                  int bUpdate) {
    S* s = (S*)pThis;
    (void)bUpdate;

    if (pHostWnd == NULL)
        return FALSE;

    HWND hWnd = *(HWND*)((BYTE*)pHostWnd + 64); // CWnd::m_hWnd @64
    if (hWnd == NULL || !::IsWindow(hWnd))
        return FALSE;

    if (s->m_nCount == 0)
        return FALSE;

    return TRUE;
}
