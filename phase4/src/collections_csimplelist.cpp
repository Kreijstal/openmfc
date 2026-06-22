// CSimpleList implementation for OpenMFC
//
// CSimpleList is a standalone, non-polymorphic MFC helper class declared in
// afxwin.h via _AFX_INLINE in real MFC. It is NOT CObject-derived: there is no
// CRuntimeClass, no vtable, no Serialize. Most members (Construct, GetHead,
// GetNext, GetNextPtr, IsEmpty, RemoveAll, GetCount) are inline in the MFC
// header and therefore NOT exported. Only AddHead and Remove are out-of-line
// and exported here.
//
// MSVC x64 layout:
//   offset 0 = void*  m_pHead
//   offset 8 = size_t m_nNextOffset (default 0)
//   sizeof = 16, align 8
//
// Both exported symbols are QEAA (public, non-virtual, x64 member): `this` is
// passed in RCX and MUST be the first parameter of each impl. This mirrors the
// plain non-virtual member-function ABI used elsewhere in the repo, e.g.
// phase4/src/dbcore.cpp impl__Empty_CDBException__QEAAXXZ(CDBException* pThis).

#include <windows.h>
#include <cstdint>
#include <cstddef>

// MS ABI calling convention (matches collections_strings.cpp)
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Exact 16-byte layout per research. No base class (CSimpleList is standalone),
// no vtable, no CRuntimeClass. Inline members reproduced from the MFC header so
// the test can drive real list semantics.
struct CSimpleList {
    void*  m_pHead;
    size_t m_nNextOffset;

    CSimpleList() : m_pHead(nullptr), m_nNextOffset(0) {}
    explicit CSimpleList(int nNextOffset)
        : m_pHead(nullptr), m_nNextOffset((size_t)nNextOffset) {}

    void Construct(int nNextOffset) { m_nNextOffset = (size_t)nNextOffset; }

    // Out-of-line in real MFC; declared here, defined as the exported impls below.
    void AddHead(void* pNewElement);
    int  Remove(void* pOldElement);

    BOOL  IsEmpty() const { return m_pHead == nullptr; }
    void  RemoveAll() { m_pHead = nullptr; }
    void* GetHead() const { return m_pHead; }
    void* GetNext(void* pElement) const { return GetNextPtrConst(pElement); }
    void*& GetNextPtr(void* p) const {
        return *(void**)((BYTE*)p + m_nNextOffset);
    }
    void* GetNextPtrConst(void* p) const {
        return *(void**)((BYTE*)p + m_nNextOffset);
    }
    int GetCount() const {
        int n = 0;
        for (void* p = m_pHead; p != nullptr; p = *(void**)((BYTE*)p + m_nNextOffset))
            ++n;
        return n;
    }
};

static_assert(sizeof(CSimpleList) == 16, "CSimpleList must be 16 bytes");
static_assert(offsetof(CSimpleList, m_pHead) == 0, "m_pHead at offset 0");
static_assert(offsetof(CSimpleList, m_nNextOffset) == 8, "m_nNextOffset at offset 8");

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

// Inline out-of-line member bodies delegating to the exported impls so the
// class methods behave identically when called via the object.
inline void CSimpleList::AddHead(void* pNewElement) {
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(this, pNewElement);
}
inline int CSimpleList::Remove(void* pOldElement) {
    return impl__Remove_CSimpleList__QEAAHPEAX_Z(this, pOldElement);
}
