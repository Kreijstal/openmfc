// CPtrList - faithful binary-compatible OpenMFC implementation
//
// Real MFC CPtrList is a DECLARE_DYNAMIC CObject subclass with a doubly-linked
// list of CNode allocated out of CPlex blocks via a free list. The public
// include/openmfc/afx.h currently aliases CPtrList to CList<void*,void*>, which
// is NOT binary compatible (no vtable, 4-byte counts, wrong tail field order).
// We therefore implement against a private faithful layout struct here, named
// OpenMFC_CPtrList, with the exact MSVC x64 ABI (sizeof 56 / 0x38):
//
//   +0x00  void** vfptr            (CObject vtable)
//   +0x08  CNode* m_pNodeHead
//   +0x10  CNode* m_pNodeTail
//   +0x18  INT_PTR m_nCount
//   +0x20  CNode* m_pNodeFree
//   +0x28  CPlex* m_pBlocks
//   +0x30  INT_PTR m_nBlockSize
//
// CNode { CNode* pNext; CNode* pPrev; void* data; }  => sizeof 24
// POSITION is a CNode* reinterpreted as struct __POSITION*.

#define OPENMFC_APPCORE_IMPL
// Only the core CObject / CRuntimeClass / DECLARE_DYNAMIC machinery is needed.
// afx.h supplies the real repo CObject base (vtable layout) and CRuntimeClass.
#include "openmfc/afx.h"

#include <cstring>
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// POSITION opaque type used in the mangled signatures.
struct __POSITION;

namespace openmfc_cptrlist_detail {

// Plex block: a singly-linked list of raw allocation blocks. Each block stores
// its CPlex header followed by nMax elements of element size. Mirrors the layout
// used by phase4/src/collections_cplex.cpp but kept private to this TU.
struct CPlex {
    CPlex* pNext;
    // data follows immediately after the header.
    void* data() { return reinterpret_cast<void*>(this + 1); }

    static CPlex* Create(CPlex*& pHead, size_t nMax, size_t cbElement) {
        CPlex* p = static_cast<CPlex*>(::malloc(sizeof(CPlex) + nMax * cbElement));
        if (p == nullptr) return nullptr;
        p->pNext = pHead;
        pHead = p;
        return p;
    }

    void FreeDataChain() {
        CPlex* p = this;
        while (p != nullptr) {
            CPlex* pNext = p->pNext;
            ::free(p);
            p = pNext;
        }
    }
};

} // namespace openmfc_cptrlist_detail

// Faithful CPtrList layout (binary compatible with real MFC's CPtrList).
class OpenMFC_CPtrList : public CObject {
public:
    DECLARE_DYNAMIC(OpenMFC_CPtrList)

    struct CNode {
        CNode* pNext;
        CNode* pPrev;
        void*  data;
    };

    // Members must follow CObject's vptr in this exact order/width.
    CNode*  m_pNodeHead;
    CNode*  m_pNodeTail;
    INT_PTR m_nCount;
    CNode*  m_pNodeFree;
    openmfc_cptrlist_detail::CPlex* m_pBlocks;
    INT_PTR m_nBlockSize;

public:
    explicit OpenMFC_CPtrList(INT_PTR nBlockSize = 10);
    virtual ~OpenMFC_CPtrList();

    INT_PTR GetCount() const { return m_nCount; }
    bool    IsEmpty() const { return m_nCount == 0; }

    __POSITION* AddHead(void* newElement);
    __POSITION* AddTail(void* newElement);
    void        AddHead(OpenMFC_CPtrList* pNewList);
    void        AddTail(OpenMFC_CPtrList* pNewList);

    void* RemoveHead();
    void* RemoveTail();
    void  RemoveAll();
    void  RemoveAt(__POSITION* pos);

    __POSITION* Find(void* searchValue, __POSITION* startAfter) const;
    __POSITION* FindIndex(INT_PTR nIndex) const;

    __POSITION* InsertBefore(__POSITION* position, void* newElement);
    __POSITION* InsertAfter(__POSITION* position, void* newElement);

    CNode* NewNode(CNode* pPrev, CNode* pNext);
    void   FreeNode(CNode* pNode);
};

// IMPLEMENT_DYNAMIC equivalent: real CRuntimeClass with correct object size and
// CObject as the base. Mirrors include/openmfc/afx.h IMPLEMENT_DYNAMIC and the
// classCObject literal in phase4/src/cobject_impl.cpp.
CRuntimeClass OpenMFC_CPtrList::classOpenMFC_CPtrList = {
    "CPtrList",
    sizeof(OpenMFC_CPtrList),
    0xFFFF,
    nullptr,
    nullptr,
    &CObject::classCObject,
    nullptr
};

static_assert(sizeof(OpenMFC_CPtrList) == 56, "CPtrList must be 56 bytes (faithful MSVC x64 ABI)");

// ---------------------------------------------------------------------------
// Member implementations (the real linked-list / free-list algorithm).
// ---------------------------------------------------------------------------

OpenMFC_CPtrList::OpenMFC_CPtrList(INT_PTR nBlockSize)
    : m_pNodeHead(nullptr),
      m_pNodeTail(nullptr),
      m_nCount(0),
      m_pNodeFree(nullptr),
      m_pBlocks(nullptr),
      m_nBlockSize(nBlockSize > 0 ? nBlockSize : 10) {}

OpenMFC_CPtrList::~OpenMFC_CPtrList() {
    RemoveAll();
}

void OpenMFC_CPtrList::RemoveAll() {
    // Drop the logical list and free every plex block. Nodes were allocated out
    // of the plex chain, so freeing the chain reclaims them all at once.
    m_nCount = 0;
    m_pNodeHead = nullptr;
    m_pNodeTail = nullptr;
    m_pNodeFree = nullptr;
    if (m_pBlocks != nullptr) {
        m_pBlocks->FreeDataChain();
        m_pBlocks = nullptr;
    }
}

OpenMFC_CPtrList::CNode* OpenMFC_CPtrList::NewNode(CNode* pPrev, CNode* pNext) {
    if (m_pNodeFree == nullptr) {
        // Grow the free list by one plex block of m_nBlockSize nodes.
        using openmfc_cptrlist_detail::CPlex;
        CPlex* pNewBlock = CPlex::Create(m_pBlocks, (size_t)m_nBlockSize, sizeof(CNode));
        if (pNewBlock == nullptr) return nullptr;
        CNode* pNode = static_cast<CNode*>(pNewBlock->data());
        // Thread the fresh nodes onto the free list (pNext chains them).
        // Index instead of decrementing past the start (the latter is UB).
        for (INT_PTR i = m_nBlockSize - 1; i >= 0; --i) {
            pNode[i].pNext = m_pNodeFree;
            m_pNodeFree = &pNode[i];
        }
    }
    CNode* pNode = m_pNodeFree;
    m_pNodeFree = m_pNodeFree->pNext;
    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data = nullptr;
    ++m_nCount;
    return pNode;
}

void OpenMFC_CPtrList::FreeNode(CNode* pNode) {
    if (pNode == nullptr) return;
    pNode->pNext = m_pNodeFree;
    m_pNodeFree = pNode;
    --m_nCount;
    // If the list became empty, reset the head/tail (matches real MFC).
    if (m_nCount == 0) {
        m_pNodeHead = nullptr;
        m_pNodeTail = nullptr;
    }
}

__POSITION* OpenMFC_CPtrList::AddHead(void* newElement) {
    CNode* pNewNode = NewNode(nullptr, m_pNodeHead);
    if (pNewNode == nullptr) return nullptr;
    pNewNode->data = newElement;
    if (m_pNodeHead != nullptr)
        m_pNodeHead->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;
    m_pNodeHead = pNewNode;
    return reinterpret_cast<__POSITION*>(pNewNode);
}

__POSITION* OpenMFC_CPtrList::AddTail(void* newElement) {
    CNode* pNewNode = NewNode(m_pNodeTail, nullptr);
    if (pNewNode == nullptr) return nullptr;
    pNewNode->data = newElement;
    if (m_pNodeTail != nullptr)
        m_pNodeTail->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;
    m_pNodeTail = pNewNode;
    return reinterpret_cast<__POSITION*>(pNewNode);
}

void OpenMFC_CPtrList::AddHead(OpenMFC_CPtrList* pNewList) {
    if (pNewList == nullptr) return;
    // Add elements of pNewList to the head, preserving their relative order
    // (iterate from tail to head of source so the source head ends up first).
    for (CNode* p = pNewList->m_pNodeTail; p != nullptr; p = p->pPrev)
        AddHead(p->data);
}

void OpenMFC_CPtrList::AddTail(OpenMFC_CPtrList* pNewList) {
    if (pNewList == nullptr) return;
    for (CNode* p = pNewList->m_pNodeHead; p != nullptr; p = p->pNext)
        AddTail(p->data);
}

void* OpenMFC_CPtrList::RemoveHead() {
    if (m_pNodeHead == nullptr) return nullptr;
    CNode* pOldNode = m_pNodeHead;
    void* returnValue = pOldNode->data;
    m_pNodeHead = pOldNode->pNext;
    if (m_pNodeHead != nullptr)
        m_pNodeHead->pPrev = nullptr;
    else
        m_pNodeTail = nullptr;
    FreeNode(pOldNode);
    return returnValue;
}

void* OpenMFC_CPtrList::RemoveTail() {
    if (m_pNodeTail == nullptr) return nullptr;
    CNode* pOldNode = m_pNodeTail;
    void* returnValue = pOldNode->data;
    m_pNodeTail = pOldNode->pPrev;
    if (m_pNodeTail != nullptr)
        m_pNodeTail->pNext = nullptr;
    else
        m_pNodeHead = nullptr;
    FreeNode(pOldNode);
    return returnValue;
}

void OpenMFC_CPtrList::RemoveAt(__POSITION* position) {
    CNode* pOldNode = reinterpret_cast<CNode*>(position);
    if (pOldNode == nullptr) return;
    if (pOldNode == m_pNodeHead) {
        m_pNodeHead = pOldNode->pNext;
    } else {
        if (pOldNode->pPrev != nullptr)
            pOldNode->pPrev->pNext = pOldNode->pNext;
    }
    if (pOldNode == m_pNodeTail) {
        m_pNodeTail = pOldNode->pPrev;
    } else {
        if (pOldNode->pNext != nullptr)
            pOldNode->pNext->pPrev = pOldNode->pPrev;
    }
    FreeNode(pOldNode);
}

__POSITION* OpenMFC_CPtrList::Find(void* searchValue, __POSITION* startAfter) const {
    CNode* pNode = reinterpret_cast<CNode*>(startAfter);
    pNode = (pNode == nullptr) ? m_pNodeHead : pNode->pNext;
    for (; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->data == searchValue)
            return reinterpret_cast<__POSITION*>(pNode);
    }
    return nullptr;
}

__POSITION* OpenMFC_CPtrList::FindIndex(INT_PTR nIndex) const {
    if (nIndex >= m_nCount || nIndex < 0)
        return nullptr;
    CNode* pNode = m_pNodeHead;
    while (nIndex-- > 0 && pNode != nullptr)
        pNode = pNode->pNext;
    return reinterpret_cast<__POSITION*>(pNode);
}

__POSITION* OpenMFC_CPtrList::InsertBefore(__POSITION* position, void* newElement) {
    if (position == nullptr)
        return AddHead(newElement); // insert before nothing == head
    CNode* pOldNode = reinterpret_cast<CNode*>(position);
    CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
    if (pNewNode == nullptr) return nullptr;
    pNewNode->data = newElement;
    if (pOldNode->pPrev != nullptr)
        pOldNode->pPrev->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;
    pOldNode->pPrev = pNewNode;
    return reinterpret_cast<__POSITION*>(pNewNode);
}

__POSITION* OpenMFC_CPtrList::InsertAfter(__POSITION* position, void* newElement) {
    if (position == nullptr)
        return AddTail(newElement); // insert after nothing == tail
    CNode* pOldNode = reinterpret_cast<CNode*>(position);
    CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
    if (pNewNode == nullptr) return nullptr;
    pNewNode->data = newElement;
    if (pOldNode->pNext != nullptr)
        pOldNode->pNext->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;
    pOldNode->pNext = pNewNode;
    return reinterpret_cast<__POSITION*>(pNewNode);
}

// ---------------------------------------------------------------------------
// Exported C thunks (MSVC ABI). One per research.exports entry.
// ---------------------------------------------------------------------------

// Symbol: ??0CPtrList@@QEAA@_J@Z
extern "C" OpenMFC_CPtrList* MS_ABI impl___0CPtrList__QEAA__J_Z(OpenMFC_CPtrList* pThis, INT_PTR nBlockSize) {
    if (pThis == nullptr) return nullptr;
    new (pThis) OpenMFC_CPtrList(nBlockSize);
    return pThis;
}

// Symbol: ??1CPtrList@@UEAA@XZ
extern "C" void MS_ABI impl___1CPtrList__UEAA_XZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return;
    pThis->~OpenMFC_CPtrList();
}

// Symbol: ?AddHead@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
extern "C" __POSITION* MS_ABI impl__AddHead_CPtrList__QEAAPEAU__POSITION__PEAX_Z(OpenMFC_CPtrList* pThis, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->AddHead(newElement);
}

// Symbol: ?AddHead@CPtrList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CPtrList__QEAAXPEAV1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList* pNewList) {
    if (pThis == nullptr) return;
    pThis->AddHead(pNewList);
}

// Symbol: ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
extern "C" __POSITION* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(OpenMFC_CPtrList* pThis, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->AddTail(newElement);
}

// Symbol: ?AddTail@CPtrList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CPtrList__QEAAXPEAV1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList* pNewList) {
    if (pThis == nullptr) return;
    pThis->AddTail(pNewList);
}

// Symbol: ?Find@CPtrList@@QEBAPEAU__POSITION@@PEAXPEAU2@@Z
extern "C" __POSITION* MS_ABI impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(const OpenMFC_CPtrList* pThis, void* searchValue, __POSITION* startAfter) {
    if (pThis == nullptr) return nullptr;
    return pThis->Find(searchValue, startAfter);
}

// Symbol: ?FindIndex@CPtrList@@QEBAPEAU__POSITION@@_J@Z
extern "C" __POSITION* MS_ABI impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(const OpenMFC_CPtrList* pThis, INT_PTR nIndex) {
    if (pThis == nullptr) return nullptr;
    return pThis->FindIndex(nIndex);
}

// Symbol: ?FreeNode@CPtrList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CPtrList__IEAAXPEAUCNode_1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList::CNode* pNode) {
    if (pThis == nullptr) return;
    pThis->FreeNode(pNode);
}

// Symbol: ?GetRuntimeClass@CPtrList@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPtrList__UEBAPEAUCRuntimeClass__XZ(const OpenMFC_CPtrList* pThis) {
    return pThis ? pThis->GetRuntimeClass() : OpenMFC_CPtrList::GetThisClass();
}

// Symbol: ?GetThisClass@CPtrList@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPtrList__SAPEAUCRuntimeClass__XZ() {
    return OpenMFC_CPtrList::GetThisClass();
}

// Symbol: ?InsertAfter@CPtrList@@QEAAPEAU__POSITION@@PEAU2@PEAX@Z
extern "C" __POSITION* MS_ABI impl__InsertAfter_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(OpenMFC_CPtrList* pThis, __POSITION* position, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->InsertAfter(position, newElement);
}

// Symbol: ?InsertBefore@CPtrList@@QEAAPEAU__POSITION@@PEAU2@PEAX@Z
extern "C" __POSITION* MS_ABI impl__InsertBefore_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(OpenMFC_CPtrList* pThis, __POSITION* position, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->InsertBefore(position, newElement);
}

// Symbol: ?NewNode@CPtrList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" OpenMFC_CPtrList::CNode* MS_ABI impl__NewNode_CPtrList__IEAAPEAUCNode_1_PEAU21_0_Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList::CNode* pPrev, OpenMFC_CPtrList::CNode* pNext) {
    if (pThis == nullptr) return nullptr;
    return pThis->NewNode(pPrev, pNext);
}

// Symbol: ?RemoveAll@CPtrList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CPtrList__QEAAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return;
    pThis->RemoveAll();
}

// Symbol: ?RemoveAt@CPtrList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(OpenMFC_CPtrList* pThis, __POSITION* position) {
    if (pThis == nullptr) return;
    pThis->RemoveAt(position);
}

// Symbol: ?RemoveHead@CPtrList@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__RemoveHead_CPtrList__QEAAPEAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return nullptr;
    return pThis->RemoveHead();
}

// Symbol: ?RemoveTail@CPtrList@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__RemoveTail_CPtrList__QEAAPEAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return nullptr;
    return pThis->RemoveTail();
}
