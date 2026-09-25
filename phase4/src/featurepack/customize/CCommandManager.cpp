// CCommandManager — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CCommandManager (afxcommandmanager.h) is NOT a CObject: it has a vfptr of
// its own (virtual ~CCommandManager) followed by five afxtempl.h containers.
// It is not declared in include/openmfc, so every thunk here takes a void*
// `this` and addresses the members through the in-file shadow layout below.
// The retail bodies were read from mfc140u.dll (bodies are byte-identical to
// the mfc140.dll twin; only the RVAs differ) and are cited by their mfc140u
// entry RVAs.
//
// The only live instance is the static singleton GetCmdMgr() hands out
// (featurepack/CMFC_misc_stubs.cpp, g4_cmdMgrStorage): 0x120 bytes with the
// retail constructor's m_nHashTableSize / m_nBlockSize constants applied and
// all six vfptrs left NULL (OpenMFC has no MSVC-layout vftable for
// CCommandManager, CMap<UINT,UINT,int,int> or CList<UINT,UINT>).  Clients
// compile the header-inline accessors (IsMenuItemWithoutImage ->
// CList::Find, GetMenuUserImage -> CMap::Lookup) into their own code and run
// them against this object, and featurepack/menu/CMFCPopupMenuBar.cpp walks
// m_lstCommandsWithoutImages by raw offsets too, so the containers are kept in
// exactly the retail afxtempl.h node/assoc layout.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct CPlex;   // detail/CPlexSupport.h: { CPlex* pNext; }, element data at +8

// core/collections/CPlex.cpp -- the exported CPlex allocator the retail
// NewAssoc / NewNode instantiations call; parameter lists from the mangled names.
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void   MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* pHead);

namespace {

// ---------------------------------------------------------------------------
// CMap<UINT, UINT, int, int>, retail layout.
//
// Map header, from the retail constructor (RVA 0x2ed80, mfc140u), which for
// each map stores vfptr +0, m_pHashTable = 0 at +8, m_nHashTableSize = 17
// (0x11) at +0x10, zero at +0x18/+0x20/+0x28, and m_nBlockSize = 10 at +0x30.
// CAssoc, from the SetAt instantiation (RVA 0x2346c, mfc140u): key +0 (the
// `mov %r14d,(%rsi)`), value +4 (`mov %ebp,0x4(%rsi)`), pNext +8, nHashValue
// +0x10; the CPlex::Create call there passes 0x18 as the element size.
struct CmdAssoc {
    UINT      key;          // +0x00
    int       value;        // +0x04
    CmdAssoc* pNext;        // +0x08
    UINT      nHashValue;   // +0x10 (+4 pad)
};
struct CmdMap {
    void*      vfptr;              // +0x00 (left NULL, see file header)
    CmdAssoc** m_pHashTable;       // +0x08
    UINT       m_nHashTableSize;   // +0x10 (+4 pad)
    INT_PTR    m_nCount;           // +0x18
    CmdAssoc*  m_pFreeList;        // +0x20
    CPlex*     m_pBlocks;          // +0x28
    INT_PTR    m_nBlockSize;       // +0x30
};
static_assert(sizeof(CmdAssoc) == 0x18, "retail CMap<UINT,UINT,int,int>::CAssoc is 24 bytes");
static_assert(offsetof(CmdAssoc, value) == 0x04 && offsetof(CmdAssoc, pNext) == 0x08 &&
              offsetof(CmdAssoc, nHashValue) == 0x10, "retail CAssoc layout");
static_assert(sizeof(CmdMap) == 0x38, "retail CMap is 56 bytes");
static_assert(offsetof(CmdMap, m_pHashTable) == 0x08 && offsetof(CmdMap, m_nHashTableSize) == 0x10 &&
              offsetof(CmdMap, m_nCount) == 0x18 && offsetof(CmdMap, m_pFreeList) == 0x20 &&
              offsetof(CmdMap, m_pBlocks) == 0x28 && offsetof(CmdMap, m_nBlockSize) == 0x30,
              "retail CMap layout");

// ---------------------------------------------------------------------------
// CList<UINT, UINT>, retail layout: {vfptr, m_pNodeHead +8, m_pNodeTail +0x10,
// m_nCount +0x18, m_pNodeFree +0x20, m_pBlocks +0x28, m_nBlockSize +0x30}
// (the retail constructor stores m_nBlockSize = 10 at list+0x30).  CNode:
// pNext +0, pPrev +8, data +0x10 -- the offsets the inlined RemoveAt in
// EnableMenuItemImage (RVA 0x2f060, mfc140u) and the Find instantiation
// (RVA 0x11f1c, mfc140u) use.
struct CmdListNode {
    CmdListNode* pNext;   // +0x00
    CmdListNode* pPrev;   // +0x08
    UINT         data;    // +0x10 (+4 pad)
};
struct CmdList {
    void*        vfptr;         // +0x00 (left NULL, see file header)
    CmdListNode* m_pNodeHead;   // +0x08
    CmdListNode* m_pNodeTail;   // +0x10
    INT_PTR      m_nCount;      // +0x18
    CmdListNode* m_pNodeFree;   // +0x20
    CPlex*       m_pBlocks;     // +0x28
    INT_PTR      m_nBlockSize;  // +0x30
};
static_assert(sizeof(CmdListNode) == 0x18, "retail CList<UINT,UINT>::CNode is 24 bytes");
static_assert(sizeof(CmdList) == 0x38, "retail CList<UINT,UINT> is 56 bytes");
static_assert(offsetof(CmdList, m_pNodeHead) == 0x08 && offsetof(CmdList, m_pNodeTail) == 0x10 &&
              offsetof(CmdList, m_nCount) == 0x18 && offsetof(CmdList, m_pNodeFree) == 0x20 &&
              offsetof(CmdList, m_pBlocks) == 0x28 && offsetof(CmdList, m_nBlockSize) == 0x30,
              "retail CList layout");

// ---------------------------------------------------------------------------
// CCommandManager itself.  Member offsets are the five member-vfptr stores of the
// retail constructor (RVA 0x2ed80, mfc140u; its sixth store is the own vfptr at
// +0) -- +0x08, +0x40, +0x78, +0xb0,
// +0xe8 -- which match the declaration order in afxcommandmanager.h; the
// total size is the 0x120 the scalar deleting destructor (RVA 0x2ee70,
// mfc140u; it tests only bit 0 of the flags and has no array path) passes in
// EDX to its operator delete call (0x2b77b0, a jmp to the free() forwarder).
struct S_CommandManager {
    void*   vfptr;                        // +0x000
    CmdMap  m_CommandIndex;               // +0x008
    CmdMap  m_CommandIndexUser;           // +0x040
    CmdMap  m_DefaultImages;              // +0x078
    CmdList m_lstCommandsWithoutImages;   // +0x0b0
    CmdMap  m_mapMenuUserImages;          // +0x0e8
};
static_assert(offsetof(S_CommandManager, m_CommandIndex) == 0x08, "m_CommandIndex at +0x08");
static_assert(offsetof(S_CommandManager, m_CommandIndexUser) == 0x40, "m_CommandIndexUser at +0x40");
static_assert(offsetof(S_CommandManager, m_DefaultImages) == 0x78, "m_DefaultImages at +0x78");
static_assert(offsetof(S_CommandManager, m_lstCommandsWithoutImages) == 0xb0, "m_lstCommandsWithoutImages at +0xb0");
static_assert(offsetof(S_CommandManager, m_mapMenuUserImages) == 0xe8, "m_mapMenuUserImages at +0xe8");
static_assert(sizeof(S_CommandManager) == 0x120, "retail sizeof(CCommandManager) == 0x120");

inline S_CommandManager* Mgr(void* pThis) { return static_cast<S_CommandManager*>(pThis); }

constexpr INT_PTR kDefaultBlockSize = 10;   // afxtempl.h default, and what the ctor stores

// HashKey<UINT>, inlined in GetAssocAt (RVA 0x236e0, mfc140u) and RemoveKey
// (RVA 0x2f418, mfc140u):  ldiv((long)key, 127773 /*0x1f31d*/);
//   h = 16807*rem - 2836*quot  (0x41a7 / 0xb14);  if (h < 0) h += 0x7fffffff.
inline UINT HashCmd(UINT key)
{
    const std::int32_t k    = static_cast<std::int32_t>(key);
    const std::int32_t quot = k / 127773;
    const std::int32_t rem  = k % 127773;
    std::int32_t h = static_cast<std::int32_t>(16807u * static_cast<std::uint32_t>(rem) -
                                               2836u * static_cast<std::uint32_t>(quot));
    if (h < 0) h += 0x7fffffff;
    return static_cast<UINT>(h);
}

// CMap::GetAssocAt (RVA 0x236e0, mfc140u), transcribed:
//     nHash = HashKey(key);  nBucket = nHash % m_nHashTableSize;
//     if (m_pHashTable == NULL) return NULL;
//     for (p = m_pHashTable[nBucket]; p; p = p->pNext)
//         if (p->nHashValue == nHash && p->key == key) return p;
//     return NULL;
// Deviation: retail divides by m_nHashTableSize unconditionally; a zero size
// is guarded here (nBucket 0, NULL result) instead of faulting.
inline CmdAssoc* MapGetAssocAt(const CmdMap* m, UINT key, UINT& nHash, UINT& nBucket)
{
    nHash = HashCmd(key);
    if (m->m_nHashTableSize == 0) { nBucket = 0; return nullptr; }
    nBucket = nHash % m->m_nHashTableSize;
    if (m->m_pHashTable == nullptr) return nullptr;
    for (CmdAssoc* p = m->m_pHashTable[nBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHash && p->key == key) return p;
    }
    return nullptr;
}

// CMap::Lookup (RVA 0x2f610, mfc140u): GetAssocAt; if found, rValue = value
// (+4) and TRUE, else FALSE with rValue untouched.
inline bool MapLookup(const CmdMap* m, UINT key, int& rValue)
{
    UINT nHash = 0, nBucket = 0;
    const CmdAssoc* p = MapGetAssocAt(m, key, nHash, nBucket);
    if (p == nullptr) return false;
    rValue = p->value;
    return true;
}

// CMap::RemoveAll, the body at RVA 0x1ba40 (mfc140u) -- the same bytes mfc140.dll
// exports as ?RemoveAll@CMapWordToPtr@@QEAAXXZ, i.e. ICF-folded:
//     if (m_pHashTable) { free(m_pHashTable); m_pHashTable = NULL; }
//     m_nCount = 0;  m_pFreeList = NULL;
//     for (p = m_pBlocks; p; p = next) free(p);     // inlined CPlex::FreeDataChain
//     m_pBlocks = NULL;
// The hash table here comes from calloc (see MapSetAt), so free() pairs with
// it; the blocks come from the exported CPlex::Create, so they are released
// with the matching exported FreeDataChain (this tree's CPlex uses new[]).
inline void MapRemoveAll(CmdMap* m)
{
    if (m->m_pHashTable != nullptr) {
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    if (m->m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(m->m_pBlocks);
    m->m_pBlocks = nullptr;
}

// CMap::SetAt (RVA 0x2346c, mfc140u), transcribed:
//     if ((pAssoc = GetAssocAt(key, nHash, nBucket)) == NULL) {
//         if (m_pHashTable == NULL) {                         // InitHashTable, inlined
//             m_pHashTable = new CAssoc*[m_nHashTableSize];   // ??2 + memset 0
//             m_nHashTableSize = <same size>;
//         }
//         if (m_pFreeList == NULL) {                          // NewAssoc, inlined
//             CPlex* p = CPlex::Create(m_pBlocks, m_nBlockSize, 0x18);
//             CAssoc* a = (CAssoc*)p->data() + m_nBlockSize - 1;
//             for (i = m_nBlockSize - 1; i >= 0; i--, a--) { a->pNext = m_pFreeList; m_pFreeList = a; }
//         }
//         pAssoc = m_pFreeList;  next = pAssoc->pNext;
//         memset(pAssoc, 0, 0x18);  pAssoc->pNext = next;
//         m_nCount++;  m_pFreeList = next;
//         pAssoc->key = key;  pAssoc->nHashValue = nHash;
//         pAssoc->pNext = m_pHashTable[nBucket];  m_pHashTable[nBucket] = pAssoc;
//     }
//     pAssoc->value = value;
// Deviations: a failed hash-table or block allocation throws in retail (the
// shared throw site at 0x23597 inside SetAt); here it leaves the map
// unchanged.  A zero m_nHashTableSize (a divide fault in retail) is refused.
// A non-positive m_nBlockSize (retail passes it to CPlex::Create as is, gets
// no free nodes and throws) is replaced by the default 10.
inline void MapSetAt(CmdMap* m, UINT key, int value)
{
    UINT nHash = 0, nBucket = 0;
    CmdAssoc* pAssoc = MapGetAssocAt(m, key, nHash, nBucket);
    if (pAssoc == nullptr) {
        if (m->m_nHashTableSize == 0) return;
        if (m->m_pHashTable == nullptr) {
            m->m_pHashTable = static_cast<CmdAssoc**>(std::calloc(m->m_nHashTableSize, sizeof(CmdAssoc*)));
            if (m->m_pHashTable == nullptr) return;
        }
        if (m->m_pFreeList == nullptr) {
            const INT_PTR nBlock = m->m_nBlockSize > 0 ? m->m_nBlockSize : kDefaultBlockSize;
            CPlex* pPlex = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
                &m->m_pBlocks, static_cast<unsigned long long>(nBlock), sizeof(CmdAssoc));
            if (pPlex == nullptr) return;
            CmdAssoc* a = reinterpret_cast<CmdAssoc*>(reinterpret_cast<char*>(pPlex) + 8) + (nBlock - 1);
            for (INT_PTR i = nBlock - 1; i >= 0; --i, --a) {
                a->pNext = m->m_pFreeList;
                m->m_pFreeList = a;
            }
        }
        pAssoc = m->m_pFreeList;
        CmdAssoc* pNextFree = pAssoc->pNext;
        std::memset(pAssoc, 0, sizeof(CmdAssoc));
        pAssoc->pNext = pNextFree;
        ++m->m_nCount;
        m->m_pFreeList = pNextFree;
        pAssoc->key = key;
        pAssoc->nHashValue = nHash;
        pAssoc->pNext = m->m_pHashTable[nBucket];
        m->m_pHashTable[nBucket] = pAssoc;
    }
    pAssoc->value = value;
}

// CMap::RemoveKey (RVA 0x2f418, mfc140u), transcribed:
//     if (m_pHashTable == NULL) return FALSE;
//     nHash = HashKey(key);
//     for (pp = &m_pHashTable[nHash % m_nHashTableSize]; *pp; pp = &(*pp)->pNext)
//         if ((*pp)->nHashValue == nHash && (*pp)->key == key) {
//             CAssoc* a = *pp;  *pp = a->pNext;
//             a->pNext = m_pFreeList;  m_pFreeList = a;          // FreeAssoc
//             if (--m_nCount == 0) RemoveAll();
//             return TRUE;
//         }
//     return FALSE;
// (A non-NULL table with a zero size cannot be produced by MapSetAt; the
// guard only avoids a divide fault.)
inline bool MapRemoveKey(CmdMap* m, UINT key)
{
    if (m->m_pHashTable == nullptr || m->m_nHashTableSize == 0) return false;
    const UINT nHash = HashCmd(key);
    CmdAssoc** ppPrev = &m->m_pHashTable[nHash % m->m_nHashTableSize];
    for (CmdAssoc* p = *ppPrev; p != nullptr; ppPrev = &p->pNext, p = *ppPrev) {
        if (p->nHashValue == nHash && p->key == key) {
            *ppPrev = p->pNext;
            p->pNext = m->m_pFreeList;
            m->m_pFreeList = p;
            if (--m->m_nCount == 0) MapRemoveAll(m);
            return true;
        }
    }
    return false;
}

// CList::RemoveAll, the body at RVA 0x8350 (mfc140u) -- mfc140.dll exports
// the same bytes as ?RemoveAll@CPtrList@@QEAAXXZ:
//     m_nCount = 0;  m_pNodeFree = m_pNodeTail = m_pNodeHead = NULL;
//     for (p = m_pBlocks; p; p = next) free(p);   m_pBlocks = NULL;
// Blocks go back through the exported CPlex::FreeDataChain (see MapRemoveAll).
inline void ListRemoveAll(CmdList* l)
{
    l->m_nCount = 0;
    l->m_pNodeFree = nullptr;
    l->m_pNodeTail = nullptr;
    l->m_pNodeHead = nullptr;
    if (l->m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(l->m_pBlocks);
    l->m_pBlocks = nullptr;
}

// CList::Find (RVA 0x11f1c, mfc140u): head-to-tail scan comparing data (+0x10);
// returns the node (the POSITION) or NULL.  That body is a two-argument clone
// with startAfter folded to NULL -- it reads only RCX/EDX and always starts at
// m_pNodeHead (EnableMenuItemImage calls it with R8 still holding bEnable).
// Its ENSURE on the node's data address (AfxThrowInvalidArgException, the call
// at 0x11f40) cannot fire for a live node and is not reproduced.
inline CmdListNode* ListFind(const CmdList* l, UINT v)
{
    for (CmdListNode* n = l->m_pNodeHead; n != nullptr; n = n->pNext) {
        if (n->data == v) return n;
    }
    return nullptr;
}

// CList::AddTail (RVA 0x12284, mfc140u) with its NewNode (RVA 0x121f8,
// mfc140u, which the CMFCToolBar.cpp UIntListNewNode transcription documents):
//     if (m_pNodeFree == NULL) {
//         CPlex* p = CPlex::Create(m_pBlocks, m_nBlockSize, 0x18);
//         CNode* n = (CNode*)p->data() + m_nBlockSize - 1;
//         for (i = m_nBlockSize - 1; i >= 0; i--, n--) { n->pNext = m_pNodeFree; m_pNodeFree = n; }
//     }
//     n = m_pNodeFree;  m_pNodeFree = n->pNext;  n->pPrev = m_pNodeTail;  n->pNext = NULL;  m_nCount++;
//     n->data = v;
//     if (m_pNodeTail) m_pNodeTail->pNext = n; else m_pNodeHead = n;
//     m_pNodeTail = n;
// Deviations: an allocation failure throws in retail; here the add is dropped.
// A non-positive m_nBlockSize is replaced by the default 10, as in MapSetAt.
inline void ListAddTail(CmdList* l, UINT v)
{
    if (l->m_pNodeFree == nullptr) {
        const INT_PTR nBlock = l->m_nBlockSize > 0 ? l->m_nBlockSize : kDefaultBlockSize;
        CPlex* pPlex = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
            &l->m_pBlocks, static_cast<unsigned long long>(nBlock), sizeof(CmdListNode));
        if (pPlex == nullptr) return;
        CmdListNode* n = reinterpret_cast<CmdListNode*>(reinterpret_cast<char*>(pPlex) + 8) + (nBlock - 1);
        for (INT_PTR i = nBlock - 1; i >= 0; --i, --n) {
            n->pNext = l->m_pNodeFree;
            l->m_pNodeFree = n;
        }
    }
    CmdListNode* n = l->m_pNodeFree;
    if (n == nullptr) return;
    l->m_pNodeFree = n->pNext;
    n->pPrev = l->m_pNodeTail;
    n->pNext = nullptr;
    ++l->m_nCount;
    n->data = v;
    if (l->m_pNodeTail != nullptr) l->m_pNodeTail->pNext = n; else l->m_pNodeHead = n;
    l->m_pNodeTail = n;
}

// CList::RemoveAt, inlined in EnableMenuItemImage (0x2f093..0x2f0d5 inside
// the body at RVA 0x2f060, mfc140u):
//     if (n == m_pNodeHead) m_pNodeHead = n->pNext; else n->pPrev->pNext = n->pNext;
//     if (n == m_pNodeTail) m_pNodeTail = n->pPrev; else n->pNext->pPrev = n->pPrev;
//     n->pNext = m_pNodeFree;  m_pNodeFree = n;                 // FreeNode
//     if (--m_nCount == 0) RemoveAll();
inline void ListRemoveAt(CmdList* l, CmdListNode* n)
{
    if (n == l->m_pNodeHead) l->m_pNodeHead = n->pNext; else n->pPrev->pNext = n->pNext;
    if (n == l->m_pNodeTail) l->m_pNodeTail = n->pPrev; else n->pNext->pPrev = n->pPrev;
    n->pNext = l->m_pNodeFree;
    l->m_pNodeFree = n;
    if (--l->m_nCount == 0) ListRemoveAll(l);
}

} // namespace

// Symbol: ??0CCommandManager@@IEAA@XZ
extern "C" void* MS_ABI impl___0CCommandManager__IEAA_XZ(void* pThis) {
    return pThis;
}

// Retail ~CCommandManager (RVA 0x2eeb0, mfc140u), transcribed:
//     vfptr = CCommandManager::`vftable';
//     m_mapMenuUserImages.~CMap();          // vfptr store + RemoveAll (0x1ba40)
//     m_lstCommandsWithoutImages.~CList();  // vfptr store + RemoveAll (0x8350)
//     m_DefaultImages.~CMap();              // 0x1ba40
//     m_CommandIndexUser.~CMap();           // 0x1ba40
//     m_CommandIndex.~CMap();               // tail-jump to 0x1ba40
// i.e. members in reverse declaration order.  Deviation: the six vftable
// stores are skipped -- OpenMFC has no MSVC-layout vftable to store (see the
// file header) -- so only the RemoveAll halves of the member destructors run.
// Symbol: ??1CCommandManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CCommandManager__UEAA_XZ(void* pThis) {
    S_CommandManager* p = Mgr(pThis);
    MapRemoveAll(&p->m_mapMenuUserImages);
    ListRemoveAll(&p->m_lstCommandsWithoutImages);
    MapRemoveAll(&p->m_DefaultImages);
    MapRemoveAll(&p->m_CommandIndexUser);
    MapRemoveAll(&p->m_CommandIndex);
}

// Defined below (CleanUp forwards to it).
extern "C" void MS_ABI impl__ClearAllCmdImages_CCommandManager__QEAAXXZ(void* pThis);

// Retail CleanUp (RVA 0x2f050, mfc140u) is a single `jmp` to ClearAllCmdImages
// (RVA 0x2f010, mfc140u).
// Symbol: ?CleanUp@CCommandManager@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CCommandManager__QEAAXXZ(void* pThis) {
    impl__ClearAllCmdImages_CCommandManager__QEAAXXZ(pThis);
}

// Retail ClearAllCmdImages (RVA 0x2f010, mfc140u), transcribed:
//     m_CommandIndex.RemoveAll();               // 0x1ba40 on this+0x08
//     m_CommandIndexUser.RemoveAll();           // 0x1ba40 on this+0x40
//     m_lstCommandsWithoutImages.RemoveAll();   // 0x8350  on this+0xb0
//     m_mapMenuUserImages.RemoveAll();          // tail-jump to 0x1ba40 on this+0xe8
// m_DefaultImages (+0x78) is not touched.
// Symbol: ?ClearAllCmdImages@CCommandManager@@QEAAXXZ
extern "C" void MS_ABI impl__ClearAllCmdImages_CCommandManager__QEAAXXZ(void* pThis) {
    S_CommandManager* p = Mgr(pThis);
    MapRemoveAll(&p->m_CommandIndex);
    MapRemoveAll(&p->m_CommandIndexUser);
    ListRemoveAll(&p->m_lstCommandsWithoutImages);
    MapRemoveAll(&p->m_mapMenuUserImages);
}

// Retail ClearCmdImage (RVA 0x2eff0, mfc140u) is `add $0x40,%rcx; jmp
// RemoveKey (0x2f418)`, i.e. m_CommandIndexUser.RemoveKey(uiCmd) only -- the
// standard m_CommandIndex entry is left alone.
// Symbol: ?ClearCmdImage@CCommandManager@@QEAAXI@Z
extern "C" void MS_ABI impl__ClearCmdImage_CCommandManager__QEAAXI_Z(void* pThis, unsigned int uiCmd) {
    MapRemoveKey(&Mgr(pThis)->m_CommandIndexUser, uiCmd);
}

// Retail ClearUserCmdImages (RVA 0x2f000, mfc140u) is `add $0x40,%rcx; jmp
// 0x1ba40`, i.e. m_CommandIndexUser.RemoveAll().
// Symbol: ?ClearUserCmdImages@CCommandManager@@QEAAXXZ
extern "C" void MS_ABI impl__ClearUserCmdImages_CCommandManager__QEAAXXZ(void* pThis) {
    MapRemoveAll(&Mgr(pThis)->m_CommandIndexUser);
}

// Retail EnableMenuItemImage (RVA 0x2f060, mfc140u), transcribed:
//     POSITION pos = m_lstCommandsWithoutImages.Find(uiCmd);          // 0x11f1c
//     if (bEnable) {
//         if (pos != NULL) m_lstCommandsWithoutImages.RemoveAt(pos);  // inlined
//         if (nUserImage >= 0) m_mapMenuUserImages.SetAt(uiCmd, nUserImage);   // 0x2346c
//         else                 m_mapMenuUserImages.RemoveKey(uiCmd);           // 0x2f418
//     } else {
//         m_mapMenuUserImages.RemoveKey(uiCmd);                       // 0x2f418
//         if (pos == NULL) m_lstCommandsWithoutImages.AddTail(uiCmd); // 0x12284
//     }
// Symbol: ?EnableMenuItemImage@CCommandManager@@QEAAXIHH@Z
extern "C" void MS_ABI impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(void* pThis, unsigned int uiCmd, int bEnable, int nUserImage) {
    S_CommandManager* p = Mgr(pThis);
    CmdListNode* pos = ListFind(&p->m_lstCommandsWithoutImages, uiCmd);
    if (bEnable) {
        if (pos != nullptr) ListRemoveAt(&p->m_lstCommandsWithoutImages, pos);
        if (nUserImage >= 0) MapSetAt(&p->m_mapMenuUserImages, uiCmd, nUserImage);
        else                 MapRemoveKey(&p->m_mapMenuUserImages, uiCmd);
    } else {
        MapRemoveKey(&p->m_mapMenuUserImages, uiCmd);
        if (pos == nullptr) ListAddTail(&p->m_lstCommandsWithoutImages, uiCmd);
    }
}

// Retail GetCmdImage (RVA 0x2efb0, mfc140u), transcribed:
//     int iImage = -1;
//     if ((bUserImage ? m_CommandIndexUser : m_CommandIndex).Lookup(uiCmd, iImage))  // 0x2f610
//         return iImage;
//     return -1;
// Symbol: ?GetCmdImage@CCommandManager@@QEBAHIH@Z
extern "C" int MS_ABI impl__GetCmdImage_CCommandManager__QEBAHIH_Z(const void* pThis, unsigned int uiCmd, int bUserImage) {
    const S_CommandManager* p = static_cast<const S_CommandManager*>(pThis);
    int iImage = -1;
    if (MapLookup(bUserImage ? &p->m_CommandIndexUser : &p->m_CommandIndex, uiCmd, iImage)) return iImage;
    return -1;
}

// Retail LoadState (RVA 0x2f130, mfc140u) does:
//     CString strSection = AFXGetRegPath(L"ToolBars", lpszProfileName);   // 0xd2070, literal 0x33ec30
//     CString strKey;  strKey.Format(L"%TsCommandManager", strSection);   // 0xda00, literal 0x33ec48
//     CSettingsStoreSP regSP;  CSettingsStore& reg = regSP.Create(FALSE, TRUE);   // 0x12a550
//     BOOL bResult = FALSE;
//     if (reg.Open(strKey)) {                                               // vslot 6 (+0x30)
//         m_lstCommandsWithoutImages.RemoveAll();                           // 0x8350
//         if (reg.Read(L"CommandsWithoutImages", m_lstCommandsWithoutImages) // vslot 18 (+0x90),
//          && reg.Read(L"MenuUserImages", m_mapMenuUserImages))              // Read(LPCTSTR, CObject&)
//             bResult = TRUE;
//     }
//     return bResult;
// Left a stub: both Read calls go through Read(LPCTSTR, CObject&), which in
// retail round-trips the container through CArchive via its virtual
// Serialize.  This tree's CSettingsStore::Read(CObject&)
// (core/app/CSettingsStore.cpp) does not deserialize into the object at all,
// and the singleton's containers have NULL vfptrs, so a transcription would
// empty m_lstCommandsWithoutImages and report success without restoring it.
// Symbol: ?LoadState@CCommandManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadState_CCommandManager__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    (void)pThis;
    (void)lpszProfileName;
    return 0;
}

// Retail SaveState (entry RVA 0x2f2b0, mfc140u; not in the mfc140u symbol
// map -- located as the body that follows LoadState, and instruction-for-
// instruction identical to ?SaveState@CCommandManager@@QEAAHPEBD@Z at RVA
// 0x2f370 in mfc140.dll; only RIP-relative displacements to string literals,
// CString helpers and the CFG dispatch slot differ between the images) is the
// mirror image: same key, Create(FALSE, FALSE), reg.CreateKey(strKey)
// (vslot 5, +0x28), then Write(L"CommandsWithoutImages",
// m_lstCommandsWithoutImages) and Write(L"MenuUserImages", m_mapMenuUserImages)
// through Write(LPCTSTR, CObject&) (vslot 11, +0x58); TRUE only if all three
// succeed.  Left a stub for the reason given on LoadState: this tree's
// CSettingsStore::Write(CObject&) does not serialize the object, so nothing
// would reach the store.
// Symbol: ?SaveState@CCommandManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveState_CCommandManager__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    (void)pThis;
    (void)lpszProfileName;
    return 0;
}

// Retail SetCmdImage (entry RVA 0x2ef30, mfc140u; not in the mfc140u symbol
// map -- it is the SetCmdImage call target cited by
// CMFCToolBarsListPropertyPage::OnResetAllToolbars, and byte-identical to
// ?SetCmdImage@CCommandManager@@QEAAXIHH@Z at RVA 0x2eff0 in mfc140.dll),
// transcribed:
//     if (uiCmd == 0 || uiCmd == (UINT)-1) return;      // `uiCmd - 1 > 0xfffffffd`
//     int iImageOld = -1;
//     if (bUserImage) {
//         // already has a standard image -> never shadow it with a user image
//         if (m_CommandIndex.Lookup(uiCmd, iImageOld) && iImageOld >= 0) return;
//         m_CommandIndexUser.SetAt(uiCmd, iImage);
//     } else {
//         if (m_CommandIndexUser.Lookup(uiCmd, iImageOld) && iImageOld >= 0) return;
//         m_CommandIndex.SetAt(uiCmd, iImage);
//     }
// Symbol: ?SetCmdImage@CCommandManager@@QEAAXIHH@Z
extern "C" void MS_ABI impl__SetCmdImage_CCommandManager__QEAAXIHH_Z(void* pThis, unsigned int uiCmd, int iImage, int bUserImage) {
    if (uiCmd - 1u > 0xfffffffdu) return;
    S_CommandManager* p = Mgr(pThis);
    int iImageOld = -1;
    if (bUserImage) {
        if (MapLookup(&p->m_CommandIndex, uiCmd, iImageOld) && iImageOld >= 0) return;
        MapSetAt(&p->m_CommandIndexUser, uiCmd, iImage);
    } else {
        if (MapLookup(&p->m_CommandIndexUser, uiCmd, iImageOld) && iImageOld >= 0) return;
        MapSetAt(&p->m_CommandIndex, uiCmd, iImage);
    }
}
