// Behavioral test for CFixedAlloc: drive the allocator THROUGH the exported
// impl_ thunks and assert real allocator semantics.
#include "../phase4/src/collections_cfixedalloc.cpp"

#include <cstdio>
#include <cstring>
#include <set>

static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if (cond) { std::printf("PASS: %s\n", msg); } \
    else { std::printf("FAIL: %s\n", msg); ++g_failures; } \
} while (0)

int main() {
    // The exported ctor/dtor/Alloc/Free/FreeAll operate on the 24-byte state.
    // We allocate a state object and drive it solely through the impl_ thunks.
    CFixedAllocState obj;
    std::memset(&obj, 0xCD, sizeof(obj)); // poison to prove ctor initializes it.

    const unsigned int allocSize = 32;
    const unsigned int blockSize = 4;

    // Construct via exported ctor.
    impl___0CFixedAlloc__QEAA_II_Z(&obj, allocSize, blockSize);
    CHECK(obj.m_nAllocSize == allocSize, "ctor stores nAllocSize");
    CHECK(obj.m_nBlockSize == blockSize, "ctor stores nBlockSize");
    CHECK(obj.m_pNodeFree == nullptr, "ctor: free list empty");
    CHECK(obj.m_pBlocks == nullptr, "ctor: block list empty");

    // Alloc several distinct non-null blocks.
    void* p1 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    void* p2 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    void* p3 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    CHECK(p1 && p2 && p3, "Alloc returns non-null blocks");
    CHECK(p1 != p2 && p2 != p3 && p1 != p3, "Alloc returns distinct blocks");

    // First Alloc allocated exactly one plex (blockSize=4 nodes).
    CHECK(obj.m_pBlocks != nullptr, "Alloc created a plex block");
    CHECK(obj.m_pBlocks->pNext == nullptr, "exactly one plex after 3 allocs");

    // Blocks must be usable for full allocSize without overlap.
    std::memset(p1, 0x11, allocSize);
    std::memset(p2, 0x22, allocSize);
    std::memset(p3, 0x33, allocSize);
    CHECK(((unsigned char*)p1)[allocSize - 1] == 0x11 &&
          ((unsigned char*)p2)[allocSize - 1] == 0x22 &&
          ((unsigned char*)p3)[allocSize - 1] == 0x33,
          "blocks are independently writable across full allocSize");

    // Free p2, then Alloc should reuse it (LIFO free list).
    impl__Free_CFixedAlloc__QEAAXPEAX_Z(&obj, p2);
    void* p4 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    CHECK(p4 == p2, "Alloc reuses most-recently-freed block (LIFO)");

    // We've now handed out p1,p2(=p4),p3 + the 4th node from the plex.
    // Allocate the remaining node from the first plex (4 nodes total).
    void* p5 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    CHECK(p5 != nullptr, "Alloc 4th node from first plex");
    CHECK(obj.m_pBlocks->pNext == nullptr, "still one plex after exhausting it");

    // Next Alloc must allocate a second plex.
    void* p6 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    CHECK(p6 != nullptr, "Alloc grows to a second plex");
    CHECK(obj.m_pBlocks != nullptr && obj.m_pBlocks->pNext != nullptr,
          "second plex linked into block list");

    // Verify all live pointers are distinct.
    std::set<void*> live = {p1, p3, p4, p5, p6};
    CHECK(live.size() == 5, "all live blocks are distinct");

    // Free(nullptr) is a no-op and must not corrupt the free list.
    void* freeHeadBefore = obj.m_pNodeFree;
    impl__Free_CFixedAlloc__QEAAXPEAX_Z(&obj, nullptr);
    CHECK(obj.m_pNodeFree == freeHeadBefore, "Free(nullptr) is a no-op");

    // FreeAll releases every plex and resets state.
    impl__FreeAll_CFixedAlloc__QEAAXXZ(&obj);
    CHECK(obj.m_pBlocks == nullptr, "FreeAll clears block list");
    CHECK(obj.m_pNodeFree == nullptr, "FreeAll clears free list");

    // After FreeAll the allocator is reusable: Alloc rebuilds plexes.
    void* p7 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&obj);
    CHECK(p7 != nullptr, "Alloc works again after FreeAll");
    CHECK(obj.m_pBlocks != nullptr, "Alloc rebuilds plex after FreeAll");

    // ctor clamps tiny allocSize up to sizeof(void*) so a node can hold pNext.
    CFixedAllocState tiny;
    impl___0CFixedAlloc__QEAA_II_Z(&tiny, 1u, 0u);
    CHECK(tiny.m_nAllocSize >= sizeof(void*), "ctor clamps allocSize to >= sizeof(void*)");
    CHECK(tiny.m_nBlockSize >= 1u, "ctor clamps blockSize to >= 1");
    void* tp1 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&tiny);
    void* tp2 = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&tiny);
    CHECK(tp1 && tp2 && tp1 != tp2, "clamped allocator yields distinct blocks");
    impl__FreeAll_CFixedAlloc__QEAAXXZ(&tiny);

    // NoSync variant shares the identical logic.
    CFixedAllocState ns;
    impl___0CFixedAllocNoSync__QEAA_II_Z(&ns, 16u, 2u);
    void* n1 = impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(&ns);
    void* n2 = impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(&ns);
    CHECK(n1 && n2 && n1 != n2, "NoSync Alloc returns distinct blocks");
    impl__Free_CFixedAllocNoSync__QEAAXPEAX_Z(&ns, n1);
    void* n3 = impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(&ns);
    CHECK(n3 == n1, "NoSync Free/Alloc reuses freed block");
    impl__FreeAll_CFixedAllocNoSync__QEAAXXZ(&ns);
    CHECK(ns.m_pBlocks == nullptr, "NoSync FreeAll clears blocks");

    // Harder: force many plex grows. allocSize 24, blockSize 8 -> allocating
    // 100 nodes must span ceil(100/8)=13 plexes, all distinct & writable.
    CFixedAllocState big;
    impl___0CFixedAlloc__QEAA_II_Z(&big, 24u, 8u);
    std::set<void*> bigSet;
    for (int i = 0; i < 100; ++i) {
        void* bp = impl__Alloc_CFixedAlloc__QEAAPEAXXZ(&big);
        CHECK(bp != nullptr, "big Alloc non-null");
        std::memset(bp, i & 0xFF, 24); // full-size write must not corrupt
        bigSet.insert(bp);
    }
    CHECK(bigSet.size() == 100, "100 grown blocks all distinct");
    int plexCount = 0;
    for (CFixedAllocState::CPlex* pp = big.m_pBlocks; pp; pp = pp->pNext) ++plexCount;
    CHECK(plexCount == 13, "100 nodes / blockSize 8 == 13 plexes");
    impl__FreeAll_CFixedAlloc__QEAAXXZ(&big);
    CHECK(big.m_pBlocks == nullptr, "FreeAll clears grown plexes");

    // Null-this guards.
    impl___0CFixedAlloc__QEAA_II_Z(nullptr, 8u, 4u);
    impl__Free_CFixedAlloc__QEAAXPEAX_Z(nullptr, (void*)0x1);
    impl__FreeAll_CFixedAlloc__QEAAXXZ(nullptr);
    CHECK(impl__Alloc_CFixedAlloc__QEAAPEAXXZ(nullptr) == nullptr,
          "Alloc(null this) returns null");

    // Clean up the working object.
    impl___1CFixedAlloc__QEAA_XZ(&obj);

    if (g_failures == 0) {
        std::printf("ALL CHECKS PASSED\n");
        return 0;
    }
    std::printf("%d CHECK(S) FAILED\n", g_failures);
    return 1;
}
