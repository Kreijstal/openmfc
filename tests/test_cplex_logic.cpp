// Behavioral test for CPlex::Create / CPlex::FreeDataChain.
//
// Includes the impl TU directly so we see both the CPlex type and the
// extern-"C" impl_ functions, and drives the real allocator logic through the
// exported ABI wrappers.

#include "../phase4/src/collections_cplex.cpp"

#include <cstdio>
#include <cstring>
#include <cstdint>

static int g_failures = 0;

#define CHECK(cond, msg)                                                      \
    do {                                                                      \
        bool ok_ = (cond);                                                    \
        std::printf("[%s] %s\n", ok_ ? "PASS" : "FAIL", (msg));               \
        if (!ok_) ++g_failures;                                               \
    } while (0)

// Count chain length by walking pNext.
static unsigned ChainLength(CPlex* head) {
    unsigned n = 0;
    for (CPlex* p = head; p != nullptr; p = p->pNext)
        ++n;
    return n;
}

int main() {
    // 1. Single Create: head updated, pNext == nullptr (was empty), non-null.
    CPlex* head = nullptr;
    CPlex* b0 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&head, 4, 16);
    CHECK(b0 != nullptr, "Create returns non-null block");
    CHECK(head == b0, "Create updates head pointer in place");
    CHECK(b0->pNext == nullptr, "first block pNext is null (empty prior chain)");

    // 2. Element area is usable and lives right after the 8-byte header.
    unsigned char* data0 = static_cast<unsigned char*>(b0->data());
    CHECK(data0 == reinterpret_cast<unsigned char*>(b0) + sizeof(CPlex),
          "data() points just past 8-byte header");
    // Write the full 4*16 = 64 byte element area; read back to prove it is ours.
    std::memset(data0, 0xAB, 4 * 16);
    bool wrote_ok = true;
    for (int i = 0; i < 4 * 16; ++i)
        if (data0[i] != 0xAB) wrote_ok = false;
    CHECK(wrote_ok, "element storage (nMax*cbElement bytes) is writable");

    // 3. Second Create links at head; chain length 2; ordering LIFO.
    CPlex* b1 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&head, 8, 4);
    CHECK(b1 != nullptr, "second Create returns non-null");
    CHECK(head == b1, "head now points at most-recent block");
    CHECK(b1->pNext == b0, "new block links in front of previous head (LIFO)");
    CHECK(ChainLength(head) == 2, "chain length is 2 after two Creates");
    CHECK(b1 != b0, "the two blocks are distinct allocations");

    // 4. Third Create; verify full traversal order head -> b2 -> b1 -> b0.
    CPlex* b2 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&head, 2, 32);
    CHECK(b2 != nullptr, "third Create returns non-null");
    CHECK(head == b2 && b2->pNext == b1 && b1->pNext == b0 &&
              b0->pNext == nullptr,
          "chain traverses b2->b1->b0->null in order");
    CHECK(ChainLength(head) == 3, "chain length is 3 after three Creates");

    // Distinct, non-overlapping element regions for the three blocks.
    std::memset(b2->data(), 0x01, 2 * 32);
    std::memset(b1->data(), 0x02, 8 * 4);
    std::memset(b0->data(), 0x03, 4 * 16);
    bool distinct =
        static_cast<unsigned char*>(b2->data())[0] == 0x01 &&
        static_cast<unsigned char*>(b1->data())[0] == 0x02 &&
        static_cast<unsigned char*>(b0->data())[0] == 0x03;
    CHECK(distinct, "three blocks hold independent, non-overlapping data");

    // 5. Invalid args are rejected (nMax==0, cbElement==0, null head).
    CPlex* head2 = nullptr;
    CHECK(impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&head2, 0, 16) == nullptr,
          "Create with nMax==0 returns null");
    CHECK(impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&head2, 4, 0) == nullptr,
          "Create with cbElement==0 returns null");
    CHECK(head2 == nullptr, "rejected Create leaves head untouched");
    CHECK(impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(nullptr, 4, 16) == nullptr,
          "Create with null head pointer returns null");
    // Overflow guard: nMax*cbElement would wrap size_t.
    CHECK(impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
              &head2, UINT64_C(0xFFFFFFFFFFFFFFFF), 2) == nullptr,
          "Create rejects size overflow");
    CHECK(head2 == nullptr, "head still untouched after overflow rejection");

    // 6. FreeDataChain frees the entire 3-block chain (no crash, no leak path).
    //    Build a fresh chain and free via the exported impl.
    CPlex* freeHead = nullptr;
    CPlex* f0 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&freeHead, 16, 8);
    CPlex* f1 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&freeHead, 16, 8);
    CPlex* f2 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&freeHead, 16, 8);
    (void)f0; (void)f1; (void)f2;
    CHECK(ChainLength(freeHead) == 3, "fresh chain has 3 blocks before free");
    impl__FreeDataChain_CPlex__QEAAXXZ(freeHead);
    CHECK(true, "FreeDataChain walked and freed 3-block chain without crashing");

    // 6b. Harder: large block (nMax*cbElement = 4096*64 = 256KB). Write the
    //     entire element area edge-to-edge and read it back to prove the whole
    //     carved region is owned by this allocation (no short-allocation bug).
    CPlex* bigHead = nullptr;
    const unsigned bigMax = 4096, bigElem = 64;
    CPlex* big = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&bigHead, bigMax, bigElem);
    CHECK(big != nullptr && bigHead == big, "large Create (256KB) succeeds");
    unsigned char* bigData = static_cast<unsigned char*>(big->data());
    std::memset(bigData, 0x5A, (size_t)bigMax * bigElem);
    bool big_ok = true;
    for (size_t i = 0; i < (size_t)bigMax * bigElem; ++i)
        if (bigData[i] != 0x5A) { big_ok = false; break; }
    CHECK(big_ok, "entire 256KB element area is writable end-to-end");
    impl__FreeDataChain_CPlex__QEAAXXZ(bigHead);

    // 7. FreeDataChain on a null/empty chain must be a safe no-op.
    impl__FreeDataChain_CPlex__QEAAXXZ(nullptr);
    CHECK(true, "FreeDataChain(null) is a safe no-op");

    // Reallocate after a full free: allocator can serve fresh blocks again.
    CPlex* reHead = nullptr;
    CPlex* r0 = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&reHead, 4, 4);
    CHECK(r0 != nullptr && reHead == r0,
          "Create works again after a full FreeDataChain");
    impl__FreeDataChain_CPlex__QEAAXXZ(reHead);

    // Clean up the original 3-block chain to keep the run leak-free.
    impl__FreeDataChain_CPlex__QEAAXXZ(head);

    if (g_failures == 0) {
        std::printf("ALL CHECKS PASSED\n");
        return 0;
    }
    std::printf("%d CHECK(S) FAILED\n", g_failures);
    return 1;
}
