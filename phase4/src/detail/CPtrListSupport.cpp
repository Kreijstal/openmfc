#include "CPtrListSupport.h"

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
    // When pNewList == this, stop at the original head so we don't chase the
    // nodes we're inserting (which would loop until OOM).
    CNode* pStop = (pNewList == this) ? pNewList->m_pNodeHead : nullptr;
    for (CNode* p = pNewList->m_pNodeTail; p != nullptr; p = p->pPrev) {
        AddHead(p->data);
        if (p == pStop) break;
    }
}
void OpenMFC_CPtrList::AddTail(OpenMFC_CPtrList* pNewList) {
    if (pNewList == nullptr) return;
    // Bound self-append at the original tail (see AddHead above).
    CNode* pStop = (pNewList == this) ? pNewList->m_pNodeTail : nullptr;
    for (CNode* p = pNewList->m_pNodeHead; p != nullptr; p = p->pNext) {
        AddTail(p->data);
        if (p == pStop) break;
    }
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
