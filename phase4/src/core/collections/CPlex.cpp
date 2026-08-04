// CPlex — OpenMFC implementation.
// Sources: collections_cplex.cpp

#include "detail/CPlexSupport.h"

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
