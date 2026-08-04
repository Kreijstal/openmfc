#pragma once
// Shared internals of the former collections_cptrlist.cpp translation unit.
// Definitions live in detail/CPtrListSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cptrlist {} } }
using namespace openmfc::detail::cptrlist;
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


// ---------------------------------------------------------------------------
// Member implementations (the real linked-list / free-list algorithm).
// ---------------------------------------------------------------------------

















// ---------------------------------------------------------------------------
// Exported C thunks (MSVC ABI). One per research.exports entry.
// ---------------------------------------------------------------------------


















