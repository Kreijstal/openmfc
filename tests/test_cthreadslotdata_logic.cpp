// Behavioral test for CThreadSlotData. Drives the real exported impl_
// functions and asserts concrete outcomes around TLS-backed slot storage.
#include <cstdio>
#include "../phase4/src/global_cthreadslotdata.cpp"

static int g_fail = 0;

static void check(const char* name, bool cond) {
    printf("[%s] %s\n", cond ? "PASS" : "FAIL", name);
    if (!cond)
        g_fail = 1;
}

int main() {
    CThreadSlotData d;
    impl___0CThreadSlotData__QEAA_XZ(&d);

    check("ctor: tlsIndex valid", d.m_tlsIndex != TLS_OUT_OF_INDEXES);
    check("ctor: rover==1", d.m_nRover == 1);
    check("ctor: max==0", d.m_nMax == 0);
    check("ctor: list empty", d.m_list.m_pHead == nullptr);

    // Allocate three slots; they must be distinct, positive, increasing.
    int s1 = impl__AllocSlot_CThreadSlotData__QEAAHXZ(&d);
    int s2 = impl__AllocSlot_CThreadSlotData__QEAAHXZ(&d);
    int s3 = impl__AllocSlot_CThreadSlotData__QEAAHXZ(&d);
    check("AllocSlot s1>0", s1 > 0);
    check("AllocSlot distinct", s1 != s2 && s2 != s3 && s1 != s3);
    check("AllocSlot increasing", s2 > s1 && s3 > s2);

    // SetValue / GetThreadValue round-trip on this thread.
    int v1 = 111, v2 = 222, v3 = 333;
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s1, &v1);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s2, &v2);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s3, &v3);
    check("Get s1 round-trip", d.GetThreadValue(s1) == &v1);
    check("Get s2 round-trip", d.GetThreadValue(s2) == &v2);
    check("Get s3 round-trip", d.GetThreadValue(s3) == &v3);

    // A thread node should now exist and be linked into m_list.
    check("thread node linked", d.m_list.m_pHead != nullptr);

    // Overwrite a value.
    int v1b = 999;
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s1, &v1b);
    check("overwrite value", d.GetThreadValue(s1) == &v1b);

    // FreeSlot clears the per-thread value.
    impl__FreeSlot_CThreadSlotData__QEAAXH_Z(&d, s2);
    check("FreeSlot clears value", d.GetThreadValue(s2) == nullptr);
    check("FreeSlot leaves others", d.GetThreadValue(s1) == &v1b &&
                                     d.GetThreadValue(s3) == &v3);

    // A freed slot should be reused by the next AllocSlot.
    int s4 = impl__AllocSlot_CThreadSlotData__QEAAHXZ(&d);
    check("AllocSlot reuses freed", s4 == s2);

    // Out-of-range / unset slot returns null.
    check("Get unset slot null", d.GetThreadValue(9999) == nullptr);

    // AssignInstance tags used slots; DeleteValues(hInst) clears matching ones.
    HINSTANCE hMod = (HINSTANCE)0x1234;
    impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(&d, hMod);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s4, &v2);
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(&d, hMod, 1);
    check("DeleteValues(hInst) cleared tagged",
          d.GetThreadValue(s1) == nullptr && d.GetThreadValue(s3) == nullptr);

    // DeleteValues with null hInst (current thread) clears everything.
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s1, &v1);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s3, &v3);
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z(
        &d, nullptr, nullptr);
    check("DeleteValues(all) cleared",
          d.GetThreadValue(s1) == nullptr && d.GetThreadValue(s3) == nullptr);

    // Harder: module-scoped DeleteValues must NOT clear a slot tagged to a
    // different module. Fresh slot allocated AFTER AssignInstance(hMod) is
    // untagged, so tag it explicitly to a second module via AssignInstance.
    HINSTANCE hMod2 = (HINSTANCE)0x5678;
    int s5 = impl__AllocSlot_CThreadSlotData__QEAAHXZ(&d);
    impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(&d, hMod2);
    int vx = 4242;
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(&d, s5, &vx);
    // Delete values for hMod (the first module): s5 belongs to hMod2, survives.
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(&d, hMod, 1);
    check("DeleteValues scoping spares other module", d.GetThreadValue(s5) == &vx);
    // Now delete for hMod2: s5 must clear.
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(&d, hMod2, 1);
    check("DeleteValues clears own module", d.GetThreadValue(s5) == nullptr);

    // NULL-guard: must not crash.
    impl__AllocSlot_CThreadSlotData__QEAAHXZ(nullptr);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(nullptr, 1, &v1);
    impl__FreeSlot_CThreadSlotData__QEAAXH_Z(nullptr, 1);
    check("null-guards survived", true);

    impl___1CThreadSlotData__QEAA_XZ(&d);
    check("dtor: list emptied", d.m_list.m_pHead == nullptr);
    check("dtor: tls freed", d.m_tlsIndex == TLS_OUT_OF_INDEXES);

    printf("RESULT: %s\n", g_fail ? "FAILURE" : "SUCCESS");
    return g_fail;
}
