// CPlex: MFC's fixed-block "plex" allocator node, used as the backing store
// for collection bucket arrays (CMapStringToOb, CList, etc.).
//
// Authoritative layout (verified against export signatures and standard MFC
// plex.cpp semantics):
//
//     struct CPlex { CPlex* pNext; };   // sizeof == 8 on x64
//
// CPlex is NOT a CObject-derived class: it has no base, no vtable, no
// CRuntimeClass, and no virtual methods. The exported symbols confirm this
// (S = static factory, Q = public non-virtual; no U = virtual). The element
// data for a block immediately follows the 8-byte header in the SAME
// allocation, i.e. data() == (BYTE*)(this + 1).
//
// Exports implemented here (def maps mangled = impl):
//   ?Create@CPlex@@SAPEAU1@AEAPEAU1@_K1@Z = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z
//   ?FreeDataChain@CPlex@@QEAAXXZ        = impl__FreeDataChain_CPlex__QEAAXXZ

// CPlex is a standalone, non-polymorphic helper (no CObject base, no vtable,
// no CRuntimeClass), so — like phase4/src/collections_csimplelist.cpp — this
// TU includes only <windows.h> rather than the heavy MFC class headers. The
// build still picks it up automatically via the collections_*.cpp shard glob.
#include <windows.h>
#include <cstdlib>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// The real CPlex node. Single 8-byte forward pointer; the element block of
// nMax*cbElement bytes is carved out of the tail of the same allocation.
struct CPlex {
    CPlex* pNext;

    // data() points just past the header, where the element bytes live.
    void* data() { return this + 1; }
};

static_assert(sizeof(CPlex) == 8, "CPlex must be exactly one 8-byte pointer on x64");

namespace {
// Round/guard against multiplication overflow when computing the element area
// size. Returns false if nMax*cbElement (or +header) would overflow size_t.
inline bool ComputeBlockSize(unsigned __int64 nMax,
                             unsigned __int64 cbElement,
                             std::size_t& outBytes) {
    if (nMax == 0 || cbElement == 0)
        return false;
    // Overflow check for nMax * cbElement.
    if (nMax > (SIZE_MAX / cbElement))
        return false;
    std::size_t elemBytes = static_cast<std::size_t>(nMax * cbElement);
    if (elemBytes > SIZE_MAX - sizeof(CPlex))
        return false;
    outBytes = sizeof(CPlex) + elemBytes;
    return true;
}
} // namespace

// Symbol: ?Create@CPlex@@SAPEAU1@AEAPEAU1@_K1@Z
// public: static struct CPlex* __cdecl CPlex::Create(struct CPlex*& pHead,
//                                                    unsigned __int64 nMax,
//                                                    unsigned __int64 cbElement)
// The C++ reference parameter (CPlex*&) lowers to CPlex** at the ABI level.
// Allocates one block large enough for the CPlex header plus nMax*cbElement
// bytes of element storage, links it at the head of the chain, and returns it.
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
        CPlex** ppHead, unsigned __int64 nMax, unsigned __int64 cbElement) {
    if (ppHead == nullptr)
        return nullptr;

    std::size_t bytes = 0;
    if (!ComputeBlockSize(nMax, cbElement, bytes))
        return nullptr;

    // Allocate as raw bytes; FreeDataChain releases with the matching
    // deallocator (operator delete[] on the same unsigned char buffer).
    unsigned char* raw = nullptr;
    try {
        raw = new unsigned char[bytes];
    } catch (const std::bad_alloc&) {
        return nullptr;
    }
    if (raw == nullptr)
        return nullptr;

    CPlex* pNew = reinterpret_cast<CPlex*>(raw);
    pNew->pNext = *ppHead;   // push onto the front of the existing chain
    *ppHead = pNew;          // update the caller's head pointer in place
    return pNew;
}

// Symbol: ?FreeDataChain@CPlex@@QEAAXXZ
// public: void __cdecl CPlex::FreeDataChain(void)
// Walks the chain starting at `this` (the hidden first arg under the flattened
// MS_ABI wrapper convention), freeing every block with the deallocator that
// matches Create's allocation.
extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* self) {
    CPlex* p = self;
    while (p != nullptr) {
        CPlex* pNext = p->pNext;
        // Each block was allocated as `new unsigned char[bytes]` starting at
        // the CPlex header; release through the same pointer/type.
        unsigned char* raw = reinterpret_cast<unsigned char*>(p);
        delete[] raw;
        p = pNext;
    }
}
