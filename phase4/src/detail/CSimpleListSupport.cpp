#include "CSimpleListSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__AddHead_CSimpleList__QEAAXPEAX_Z(
    CSimpleList* pThis, void* pNewElement);
extern "C" int MS_ABI impl__Remove_CSimpleList__QEAAHPEAX_Z(
    CSimpleList* pThis, void* pOldElement);

static_assert(sizeof(CSimpleList) == 16, "CSimpleList must be 16 bytes");
static_assert(offsetof(CSimpleList, m_pHead) == 0, "m_pHead at offset 0");
static_assert(offsetof(CSimpleList, m_nNextOffset) == 8, "m_nNextOffset at offset 8");
inline void CSimpleList::AddHead(void* pNewElement) {
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(this, pNewElement);
}
inline int CSimpleList::Remove(void* pOldElement) {
    return impl__Remove_CSimpleList__QEAAHPEAX_Z(this, pOldElement);
}
