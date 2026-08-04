#pragma once
// Shared internals of the former collections_csimplelist.cpp translation unit.
// Definitions live in detail/CSimpleListSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace csimplelist {} } }
using namespace openmfc::detail::csimplelist;
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




// Inline out-of-line member bodies delegating to the exported impls so the
// class methods behave identically when called via the object.
