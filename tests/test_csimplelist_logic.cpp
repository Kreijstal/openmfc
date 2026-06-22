// Behavioral test for CSimpleList: drives the exported impl_ functions directly
// on a real CSimpleList object and asserts genuine LIFO linked-list semantics.
#include <cstdio>
#include <cstddef>

#include "../phase4/src/collections_csimplelist.cpp"

static int g_fail = 0;
#define CHECK(cond, msg) do { \
    if (cond) { printf("PASS: %s\n", msg); } \
    else { printf("FAIL: %s\n", msg); g_fail = 1; } \
} while (0)

// A node whose `next` pointer sits at a NON-zero offset, exercising
// m_nNextOffset arithmetic (not just offset 0).
struct Node {
    int   value;     // offset 0
    void* pad;       // padding so next is not at offset 0
    void* next;      // the link CSimpleList threads through
};

int main() {
    const int kOff = (int)offsetof(Node, next);

    CSimpleList list;
    list.Construct(kOff);

    Node a{1, nullptr, nullptr};
    Node b{2, nullptr, nullptr};
    Node c{3, nullptr, nullptr};

    CHECK(list.IsEmpty(), "new list is empty");
    CHECK(list.GetCount() == 0, "new list count is 0");
    CHECK(list.GetHead() == nullptr, "new list head is null");

    // AddHead via exported impl -> LIFO ordering.
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, &a);
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, &b);
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, &c);

    CHECK(!list.IsEmpty(), "list non-empty after 3 adds");
    CHECK(list.GetCount() == 3, "count is 3 after 3 adds");
    CHECK(list.GetHead() == &c, "head is last-added (c)");

    // Verify traversal order c -> b -> a.
    void* p0 = list.GetHead();
    void* p1 = list.GetNext(p0);
    void* p2 = list.GetNext(p1);
    void* p3 = list.GetNext(p2);
    CHECK(p0 == &c && p1 == &b && p2 == &a, "traversal order c,b,a");
    CHECK(p3 == nullptr, "tail next is null");

    // Remove from the middle (b).
    int r = impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &b);
    CHECK(r != 0, "Remove(b) returned true");
    CHECK(list.GetCount() == 2, "count is 2 after removing middle");
    CHECK(list.GetHead() == &c, "head still c after middle remove");
    CHECK(list.GetNext(&c) == &a, "c now links directly to a");

    // Remove the head (c).
    r = impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &c);
    CHECK(r != 0, "Remove(c) head returned true");
    CHECK(list.GetHead() == &a, "head is a after removing c");
    CHECK(list.GetCount() == 1, "count is 1");

    // Remove an element not in the list returns false.
    r = impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &b);
    CHECK(r == 0, "Remove(b) absent returned false");
    CHECK(list.GetCount() == 1, "count still 1 after absent remove");

    // Remove the last remaining element.
    r = impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &a);
    CHECK(r != 0, "Remove(a) last returned true");
    CHECK(list.IsEmpty(), "list empty after removing all");
    CHECK(list.GetCount() == 0, "count back to 0");

    // NULL guards: must not crash, Remove returns 0.
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, nullptr); // no-op
    CHECK(list.IsEmpty(), "AddHead(null) is a no-op");
    CHECK(impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, nullptr) == 0,
          "Remove(null) returns 0");
    CHECK(impl__Remove_CSimpleList__QEAAHPEAX_Z(nullptr, &a) == 0,
          "Remove with null this returns 0");

    // Re-add to confirm reusable after emptying.
    impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, &a);
    CHECK(list.GetHead() == &a && list.GetCount() == 1,
          "list reusable after empty");

    // Harder: large N, then remove every node by value and confirm structural
    // integrity (count decreasing, no corruption, all found, then absent).
    list.RemoveAll();
    const int N = 1000;
    static Node big[N];
    for (int i = 0; i < N; ++i) {
        big[i].value = i; big[i].pad = nullptr; big[i].next = nullptr;
        impl__AddHead_CSimpleList__QEAAXPEAX_Z(&list, &big[i]);
    }
    CHECK(list.GetCount() == N, "large list count is N");
    // Remove from the tail end (oldest-added, deepest in chain) outward.
    int ok = 1;
    for (int i = 0; i < N; ++i) {
        int rr = impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &big[i]);
        if (rr == 0) ok = 0;
        if (list.GetCount() != N - i - 1) ok = 0;
    }
    CHECK(ok, "removed all N nodes (deep traversal) with correct counts");
    CHECK(list.IsEmpty(), "large list empty after removing all N");
    // Absent removal on now-empty list.
    CHECK(impl__Remove_CSimpleList__QEAAHPEAX_Z(&list, &big[0]) == 0,
          "Remove on empty large list returns 0");

    if (g_fail) { printf("RESULT: FAILED\n"); return 1; }
    printf("RESULT: ALL PASSED\n");
    return 0;
}
