// Behavioral test for the faithful CPtrList implementation.
// Drives the class THROUGH the exported extern-C impl_ thunks.
#include "../phase4/src/collections_cptrlist.cpp"

#include <cstdio>
#include <cstdlib>

static int g_fail = 0;
#define CHECK(cond, msg) do { \
    bool _r = (cond); \
    std::printf("[%s] %s\n", _r ? "PASS" : "FAIL", msg); \
    if (!_r) g_fail = 1; \
} while (0)

int main() {
    // Three distinct, identifiable pointer values.
    int a = 1, b = 2, c = 3, d = 4;
    void* pa = &a; void* pb = &b; void* pc = &c; void* pd = &d;

    // Construct via the exported ctor thunk with a small block size.
    alignas(OpenMFC_CPtrList) unsigned char storage[sizeof(OpenMFC_CPtrList)];
    OpenMFC_CPtrList* L = reinterpret_cast<OpenMFC_CPtrList*>(storage);
    OpenMFC_CPtrList* ret = impl___0CPtrList__QEAA__J_Z(L, 4);
    CHECK(ret == L, "ctor returns this");
    CHECK(sizeof(OpenMFC_CPtrList) == 56, "sizeof CPtrList == 56");
    CHECK(L->GetCount() == 0, "fresh list empty");

    // GetThisClass / GetRuntimeClass sanity.
    CRuntimeClass* rc = impl__GetThisClass_CPtrList__SAPEAUCRuntimeClass__XZ();
    CHECK(rc != nullptr && rc->m_nObjectSize == 56, "runtime class object size 56");
    CHECK(rc->m_pBaseClass == &CObject::classCObject, "base class is CObject");
    CRuntimeClass* rc2 = impl__GetRuntimeClass_CPtrList__UEBAPEAUCRuntimeClass__XZ(L);
    CHECK(rc2 == rc, "GetRuntimeClass matches GetThisClass");

    // AddTail pa, pb, pc -> order [pa, pb, pc]
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, pa);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, pb);
    __POSITION* posC = impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, pc);
    CHECK(L->GetCount() == 3, "count 3 after 3 AddTail");
    CHECK(posC != nullptr, "AddTail returns position");

    // AddHead pd -> [pd, pa, pb, pc]
    impl__AddHead_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, pd);
    CHECK(L->GetCount() == 4, "count 4 after AddHead");

    // FindIndex(0) should be pd (head).
    __POSITION* p0 = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 0);
    CHECK(p0 != nullptr && reinterpret_cast<OpenMFC_CPtrList::CNode*>(p0)->data == pd, "FindIndex(0)==pd");
    __POSITION* p2 = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 2);
    CHECK(p2 != nullptr && reinterpret_cast<OpenMFC_CPtrList::CNode*>(p2)->data == pb, "FindIndex(2)==pb");
    CHECK(impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 99) == nullptr, "FindIndex out of range null");

    // Find pc, starting from head.
    __POSITION* fc = impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(L, pc, nullptr);
    CHECK(fc == posC, "Find pc returns its position");
    CHECK(impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(L, (void*)0xdead, nullptr) == nullptr, "Find missing null");

    // RemoveHead returns pd (FIFO from head).
    void* h = impl__RemoveHead_CPtrList__QEAAPEAXXZ(L);
    CHECK(h == pd, "RemoveHead==pd");
    CHECK(L->GetCount() == 3, "count 3 after RemoveHead");

    // RemoveTail returns pc.
    void* t = impl__RemoveTail_CPtrList__QEAAPEAXXZ(L);
    CHECK(t == pc, "RemoveTail==pc");
    CHECK(L->GetCount() == 2, "count 2 after RemoveTail");

    // Now list is [pa, pb]. InsertAfter(pa) pd -> [pa, pd, pb].
    __POSITION* posA = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 0);
    CHECK(reinterpret_cast<OpenMFC_CPtrList::CNode*>(posA)->data == pa, "head is pa");
    impl__InsertAfter_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(L, posA, pd);
    __POSITION* mid = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 1);
    CHECK(reinterpret_cast<OpenMFC_CPtrList::CNode*>(mid)->data == pd, "InsertAfter placed pd at index1");
    CHECK(L->GetCount() == 3, "count 3 after InsertAfter");

    // InsertBefore(pa) pc -> [pc, pa, pd, pb].
    posA = impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(L, pa, nullptr);
    impl__InsertBefore_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(L, posA, pc);
    __POSITION* nh = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 0);
    CHECK(reinterpret_cast<OpenMFC_CPtrList::CNode*>(nh)->data == pc, "InsertBefore placed pc at head");
    CHECK(L->GetCount() == 4, "count 4 after InsertBefore");

    // RemoveAt the pd node (currently index 2: [pc, pa, pd, pb]).
    __POSITION* pdpos = impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(L, pd, nullptr);
    impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(L, pdpos);
    CHECK(L->GetCount() == 3, "count 3 after RemoveAt");
    CHECK(impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(L, pd, nullptr) == nullptr, "pd gone after RemoveAt");

    // Free-list reuse: NewNode after RemoveAt should reuse the freed node memory.
    // Drain the list, then add a node and confirm allocator hands back memory.
    impl__RemoveAll_CPtrList__QEAAXXZ(L);
    CHECK(L->GetCount() == 0, "RemoveAll empties list");
    CHECK(impl__RemoveHead_CPtrList__QEAAPEAXXZ(L) == nullptr, "RemoveHead on empty null");
    CHECK(impl__RemoveTail_CPtrList__QEAAPEAXXZ(L) == nullptr, "RemoveTail on empty null");

    // After RemoveAll, add 6 (> block size 4) to force a second plex block.
    for (int i = 0; i < 6; ++i)
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, (void*)(intptr_t)(100 + i));
    CHECK(L->GetCount() == 6, "count 6 spanning two plex blocks");
    // Verify ordering across the blocks.
    bool order_ok = true;
    for (int i = 0; i < 6; ++i) {
        __POSITION* p = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, i);
        if (!p || reinterpret_cast<OpenMFC_CPtrList::CNode*>(p)->data != (void*)(intptr_t)(100 + i)) {
            order_ok = false; break;
        }
    }
    CHECK(order_ok, "ordering preserved across plex blocks");

    // Free-list reuse: remember a freed node address, re-add, confirm reuse.
    void* freed_node = (void*)reinterpret_cast<OpenMFC_CPtrList::CNode*>(
        impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, 5));
    impl__RemoveTail_CPtrList__QEAAPEAXXZ(L); // frees that node
    __POSITION* re = impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L, pa);
    CHECK((void*)re == freed_node, "free-list reuses the just-freed node");

    // AddTail(list*) splice variant.
    alignas(OpenMFC_CPtrList) unsigned char st2[sizeof(OpenMFC_CPtrList)];
    OpenMFC_CPtrList* L2 = reinterpret_cast<OpenMFC_CPtrList*>(st2);
    impl___0CPtrList__QEAA__J_Z(L2, 10);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L2, pb);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L2, pc);
    INT_PTR before = L->GetCount();
    impl__AddTail_CPtrList__QEAAXPEAV1__Z(L, L2);
    CHECK(L->GetCount() == before + 2, "AddTail(list) appends 2 elements");
    __POSITION* lastpos = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L, L->GetCount() - 1);
    CHECK(reinterpret_cast<OpenMFC_CPtrList::CNode*>(lastpos)->data == pc, "spliced tail is pc");

    // Harder stress: a fresh list, push 100 elements (block size 4 => ~25 plex
    // blocks), verify count and exact ordering across many block boundaries,
    // then drain via RemoveHead confirming strict FIFO order and final empty.
    alignas(OpenMFC_CPtrList) unsigned char st3[sizeof(OpenMFC_CPtrList)];
    OpenMFC_CPtrList* L3 = reinterpret_cast<OpenMFC_CPtrList*>(st3);
    impl___0CPtrList__QEAA__J_Z(L3, 4);
    const int N = 100;
    for (int i = 0; i < N; ++i)
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(L3, (void*)(intptr_t)(1000 + i));
    CHECK(L3->GetCount() == N, "count 100 across many plex blocks");
    bool big_order_ok = true;
    for (int i = 0; i < N; ++i) {
        __POSITION* p = impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(L3, i);
        if (!p || reinterpret_cast<OpenMFC_CPtrList::CNode*>(p)->data != (void*)(intptr_t)(1000 + i)) {
            big_order_ok = false; break;
        }
    }
    CHECK(big_order_ok, "FindIndex ordering correct over 100 elements");
    bool drain_ok = true;
    for (int i = 0; i < N; ++i) {
        void* v = impl__RemoveHead_CPtrList__QEAAPEAXXZ(L3);
        if (v != (void*)(intptr_t)(1000 + i)) { drain_ok = false; break; }
    }
    CHECK(drain_ok, "RemoveHead drains 100 in FIFO order");
    CHECK(L3->GetCount() == 0, "list empty after draining 100");
    impl___1CPtrList__UEAA_XZ(L3);

    // Self-append must be bounded to the original nodes (the buggy version chases
    // newly-appended nodes until OOM). Reaching the CHECK at all proves it
    // terminates; the content proves correctness.
    alignas(OpenMFC_CPtrList) unsigned char storageS[sizeof(OpenMFC_CPtrList)];
    OpenMFC_CPtrList* LS = reinterpret_cast<OpenMFC_CPtrList*>(storageS);
    impl___0CPtrList__QEAA__J_Z(LS, 4);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(LS, (void*)(intptr_t)1);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(LS, (void*)(intptr_t)2);
    impl__AddTail_CPtrList__QEAAXPEAV1__Z(LS, LS);   // self-append
    CHECK(LS->GetCount() == 4, "self AddTail bounded -> count doubles to 4");
    {
        intptr_t expect[4] = {1, 2, 1, 2};
        bool ok = true;
        for (int i = 0; i < 4; ++i) {
            void* v = impl__RemoveHead_CPtrList__QEAAPEAXXZ(LS);
            if (v != (void*)expect[i]) { ok = false; break; }
        }
        CHECK(ok, "self AddTail produced [1,2,1,2]");
    }
    impl___1CPtrList__UEAA_XZ(LS);

    // NULL-guard checks.
    CHECK(impl__RemoveHead_CPtrList__QEAAPEAXXZ(nullptr) == nullptr, "NULL this RemoveHead safe");
    CHECK(impl__GetThisClass_CPtrList__SAPEAUCRuntimeClass__XZ() != nullptr, "GetThisClass non-null");

    // Tear down both via dtor thunk.
    impl___1CPtrList__UEAA_XZ(L2);
    impl___1CPtrList__UEAA_XZ(L);

    if (g_fail) {
        std::printf("RESULT: FAILURES PRESENT\n");
        return 1;
    }
    std::printf("RESULT: ALL CHECKS PASSED\n");
    return 0;
}
