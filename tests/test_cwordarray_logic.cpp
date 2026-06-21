// Behavioral test for CWordArray real implementation.
// Drives the exported impl_ thunks directly against a real object and asserts
// genuine outcomes (sizes, element values, FIFO ordering of inserts/removes).

#include <cstdio>
#include <cstdlib>

#include "../phase4/src/collections_cwordarray.cpp"

// The real CArchive insertion/extraction operators live in phase4/src/filecore.cpp,
// which is not part of this standalone test TU. Serialize() references a few of
// them, so provide minimal link-time definitions here. The test never calls
// Serialize, so these bodies never execute; they only satisfy the linker.
CArchive& CArchive::operator<<(unsigned int v) { (void)v; return *this; }
CArchive& CArchive::operator<<(unsigned short v) { (void)v; return *this; }
CArchive& CArchive::operator>>(unsigned int& v) { (void)v; return *this; }
CArchive& CArchive::operator>>(unsigned short& v) { (void)v; return *this; }

static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if (cond) { \
        std::printf("[PASS] %s\n", msg); \
    } else { \
        std::printf("[FAIL] %s\n", msg); \
        ++g_failures; \
    } \
} while (0)

int main() {
    // Construct via the exported ctor thunk on raw storage so we test the real
    // placement-new path and the 40-byte layout.
    alignas(OpenMfcWordArray) unsigned char storage[sizeof(OpenMfcWordArray)];
    OpenMfcWordArray* a = reinterpret_cast<OpenMfcWordArray*>(storage);
    impl___0CWordArray__QEAA_XZ(a);

    CHECK(sizeof(OpenMfcWordArray) == 40, "sizeof(CWordArray) == 40 (faithful layout)");
    CHECK(a->GetSize() == 0, "fresh array is empty");

    // ---- SetSize + zero fill ----
    impl__SetSize_CWordArray__QEAAX_J0_Z(a, 5, -1);
    CHECK(a->GetSize() == 5, "SetSize(5) -> size 5");
    bool allZero = true;
    for (int i = 0; i < 5; ++i) if (a->GetAt(i) != 0) allZero = false;
    CHECK(allZero, "SetSize zero-fills new elements");

    // ---- SetAtGrow grows the array ----
    impl__SetAtGrow_CWordArray__QEAAX_JG_Z(a, 9, (unsigned short)0xBEEF);
    CHECK(a->GetSize() == 10, "SetAtGrow(9) grows size to 10");
    CHECK(a->GetAt(9) == 0xBEEF, "SetAtGrow stored 0xBEEF at index 9");
    CHECK(a->GetAt(5) == 0, "gap element left zero-filled");

    // ---- direct element values ----
    for (int i = 0; i < 5; ++i) a->SetAt(i, (unsigned short)(100 + i));
    CHECK(a->GetAt(0) == 100 && a->GetAt(4) == 104, "SetAt stored values");

    // ---- InsertAt (value, count) shifts tail ----
    impl__InsertAt_CWordArray__QEAAX_JG0_Z(a, 2, (unsigned short)0x1111, 3);
    CHECK(a->GetSize() == 13, "InsertAt(2, val, 3) grows size to 13");
    CHECK(a->GetAt(2) == 0x1111 && a->GetAt(3) == 0x1111 && a->GetAt(4) == 0x1111,
          "InsertAt wrote 3 copies of the value");
    CHECK(a->GetAt(5) == 102, "element formerly at index 2 shifted to index 5");

    // ---- RemoveAt removes the inserted run ----
    impl__RemoveAt_CWordArray__QEAAX_J0_Z(a, 2, 3);
    CHECK(a->GetSize() == 10, "RemoveAt(2,3) shrinks size back to 10");
    CHECK(a->GetAt(2) == 102, "after RemoveAt index 2 holds original value 102");
    CHECK(a->GetAt(9) == 0xBEEF, "tail value 0xBEEF preserved after RemoveAt");

    // ---- Append from another array ----
    alignas(OpenMfcWordArray) unsigned char storage2[sizeof(OpenMfcWordArray)];
    OpenMfcWordArray* b = reinterpret_cast<OpenMfcWordArray*>(storage2);
    impl___0CWordArray__QEAA_XZ(b);
    for (int i = 0; i < 4; ++i) b->Add((unsigned short)(0x2000 + i));
    CHECK(b->GetSize() == 4, "Add x4 on second array -> size 4");

    long long appendBase = impl__Append_CWordArray__QEAA_JAEBV1__Z(a, b);
    CHECK(appendBase == 10, "Append returns old size (10)");
    CHECK(a->GetSize() == 14, "Append grew first array to 14");
    CHECK(a->GetAt(10) == 0x2000 && a->GetAt(13) == 0x2003,
          "Append copied source elements at the end");

    // ---- InsertAt(index, CWordArray*) splices the whole array in ----
    impl__InsertAt_CWordArray__QEAAX_JPEAV1__Z(a, 0, b);
    CHECK(a->GetSize() == 18, "InsertAt(0, array*) grew to 18");
    CHECK(a->GetAt(0) == 0x2000 && a->GetAt(3) == 0x2003,
          "InsertAt(array*) spliced source at the front");
    CHECK(a->GetAt(4) == 100, "original element 100 now at index 4");

    // ---- Copy replaces contents ----
    impl__Copy_CWordArray__QEAAXAEBV1__Z(a, b);
    CHECK(a->GetSize() == 4, "Copy shrank target to source size 4");
    CHECK(a->GetAt(0) == 0x2000 && a->GetAt(3) == 0x2003, "Copy duplicated source values");

    // ---- FreeExtra trims capacity but keeps size/values ----
    a->SetAtGrow(20, (unsigned short)0xCAFE);  // force big capacity
    impl__SetSize_CWordArray__QEAAX_J0_Z(a, 3, -1);
    impl__FreeExtra_CWordArray__QEAAXXZ(a);
    CHECK(a->GetSize() == 3, "after SetSize(3)+FreeExtra size is 3");
    CHECK(a->GetAt(0) == 0x2000, "FreeExtra preserved element values");

    // ---- runtime class fidelity ----
    CRuntimeClass* rc = impl__GetThisClass_CWordArray__SAPEAUCRuntimeClass__XZ();
    CHECK(rc != nullptr, "GetThisClass returns a runtime class");
    CHECK(rc->m_nObjectSize == 40, "CRuntimeClass.m_nObjectSize == 40");
    CHECK(std::strcmp(rc->m_lpszClassName, "CWordArray") == 0,
          "CRuntimeClass name is \"CWordArray\"");
    CHECK(rc->m_pBaseClass == CObject::GetThisClass(), "base class is CObject");

    CRuntimeClass* rcInst = impl__GetRuntimeClass_CWordArray__UEBAPEAUCRuntimeClass__XZ(a);
    CHECK(rcInst == rc, "instance GetRuntimeClass matches GetThisClass");

    CObject* created = impl__CreateObject_CWordArray__SAPEAVCObject__XZ();
    CHECK(created != nullptr, "CreateObject produced an object");
    CHECK(created->GetRuntimeClass() == rc, "created object reports CWordArray runtime class");
    delete created;

    // ---- destruct via exported dtor thunk ----
    impl___1CWordArray__UEAA_XZ(a);
    impl___1CWordArray__UEAA_XZ(b);
    CHECK(true, "destructors ran without crashing");

    // ---- NULL guards ----
    impl__SetSize_CWordArray__QEAAX_J0_Z(nullptr, 5, -1);
    impl___1CWordArray__UEAA_XZ(nullptr);
    CHECK(impl__Append_CWordArray__QEAA_JAEBV1__Z(nullptr, nullptr) == 0,
          "NULL this on Append returns 0 without crashing");

    // ---- harder: large N forces many grow reallocations; check integrity ----
    alignas(OpenMfcWordArray) unsigned char storage3[sizeof(OpenMfcWordArray)];
    OpenMfcWordArray* c = reinterpret_cast<OpenMfcWordArray*>(storage3);
    impl___0CWordArray__QEAA_XZ(c);
    const int N = 5000;
    for (int i = 0; i < N; ++i) c->Add((unsigned short)(i & 0xFFFF));
    CHECK(c->GetSize() == N, "Add x5000 -> size 5000 across many grows");
    bool bigOk = true;
    for (int i = 0; i < N; ++i) if (c->GetAt(i) != (unsigned short)(i & 0xFFFF)) bigOk = false;
    CHECK(bigOk, "all 5000 elements intact after repeated reallocation");
    // remove a middle chunk and verify the splice
    impl__RemoveAt_CWordArray__QEAAX_J0_Z(c, 1000, 2000);
    CHECK(c->GetSize() == N - 2000, "RemoveAt(1000,2000) -> size 3000");
    CHECK(c->GetAt(999) == (unsigned short)999 && c->GetAt(1000) == (unsigned short)3000,
          "RemoveAt spliced tail correctly at large N");
    impl___1CWordArray__UEAA_XZ(c);

    if (g_failures == 0) {
        std::printf("ALL CHECKS PASSED\n");
        return 0;
    }
    std::printf("%d CHECK(S) FAILED\n", g_failures);
    return 1;
}
