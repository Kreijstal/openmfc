// CSimpleList — OpenMFC implementation.
// Sources: collections_csimplelist.cpp

#include "detail/CSimpleListSupport.h"

// Symbol: ?AddHead@CSimpleList@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__AddHead_CSimpleList__QEAAXPEAX_Z(
    CSimpleList* pThis, void* pNewElement) {
    if (pThis == nullptr || pNewElement == nullptr)
        return;
    // *GetNextPtr(pNewElement) = m_pHead; m_pHead = pNewElement;
    *(void**)((BYTE*)pNewElement + pThis->m_nNextOffset) = pThis->m_pHead;
    pThis->m_pHead = pNewElement;
}
// Symbol: ?Remove@CSimpleList@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__Remove_CSimpleList__QEAAHPEAX_Z(
    CSimpleList* pThis, void* pOldElement) {
    if (pThis == nullptr || pOldElement == nullptr)
        return 0;
    const size_t off = pThis->m_nNextOffset;
    int found = 0;
    if (pThis->m_pHead == pOldElement) {
        // Unlink head.
        pThis->m_pHead = *(void**)((BYTE*)pOldElement + off);
        found = 1;
    } else {
        void* cur = pThis->m_pHead;
        while (cur != nullptr) {
            void** pNext = (void**)((BYTE*)cur + off);
            if (*pNext == pOldElement) {
                *pNext = *(void**)((BYTE*)pOldElement + off);
                found = 1;
                break;
            }
            cur = *pNext;
        }
    }
    return found;
}
