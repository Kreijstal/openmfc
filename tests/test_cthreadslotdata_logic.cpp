// Windows/MSVC behavioral coverage of native TLS slot ownership.
#include "detail/CThreadSlotDataSupport.h"
#include <cstdio>
extern "C" CThreadSlotData* MS_ABI impl___0CThreadSlotData__QEAA_XZ(CThreadSlotData*);
extern "C" void MS_ABI impl___1CThreadSlotData__QEAA_XZ(CThreadSlotData*);
extern "C" int MS_ABI impl__AllocSlot_CThreadSlotData__QEAAHXZ(CThreadSlotData*);
extern "C" void MS_ABI impl__FreeSlot_CThreadSlotData__QEAAXH_Z(CThreadSlotData*, int);
extern "C" void MS_ABI impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(CThreadSlotData*, int, void*);
extern "C" void MS_ABI
impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(CThreadSlotData*, HINSTANCE);
extern "C" void MS_ABI
impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(CThreadSlotData*, HINSTANCE, BOOL);
static int failures, deleted;
static void check(bool result, const char* text) {
    printf("%s: %s\n", result ? "PASS" : "FAIL", text);
    if (!result)
        ++failures;
}
struct Value : CNoTrackObject {
    ~Value() override { ++deleted; }
};
int main() {
    alignas(8) BYTE storage[88];
    memset(storage, 0xcd, sizeof(storage));
    auto* slots = reinterpret_cast<CThreadSlotData*>(storage);
    check(impl___0CThreadSlotData__QEAA_XZ(slots) == slots &&
              slots->m_tlsIndex != TLS_OUT_OF_INDEXES && slots->m_nRover == 1 && !slots->m_nMax &&
              slots->m_list.m_nNextOffset == 8,
          "constructor installs native TLS storage and the thread-node link offset");
    int first = impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots),
        second = impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots),
        third = impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots);
    check(first == 1 && second == 2 && third == 3 && slots->m_nAlloc == 32,
          "slot storage grows by 32 entries and reserves slot zero");
    auto* a = new Value;
    auto* b = new Value;
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(slots, first, a);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(slots, second, b);
    auto* node = static_cast<CThreadData*>(slots->m_list.m_pHead);
    check(node && node->nCount == slots->m_nMax && node->pData[first] == a &&
              slots->GetThreadValue(second) == b,
          "thread values occupy the native 32-byte node and round-trip through Windows TLS");
    impl__FreeSlot_CThreadSlotData__QEAAXH_Z(slots, second);
    check(deleted == 1 && !slots->GetThreadValue(second) && slots->GetThreadValue(first) == a,
          "FreeSlot destroys owned values and preserves other slots");
    int next = impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots);
    check(next == 4, "allocation prefers its free rover before scanning older holes");
    while (slots->m_nRover < slots->m_nAlloc)
        impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots);
    check(impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots) == second,
          "allocation scans and reuses a released slot after reaching the table boundary");
    HINSTANCE one = reinterpret_cast<HINSTANCE>(UINT_PTR(0x1234)),
              two = reinterpret_cast<HINSTANCE>(UINT_PTR(0x5678));
    impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(slots, one);
    int other = impl__AllocSlot_CThreadSlotData__QEAAHXZ(slots);
    auto* c = new Value;
    impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(slots, two);
    impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(slots, other, c);
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(slots, one, TRUE);
    check(deleted == 2 && !slots->GetThreadValue(first) && slots->GetThreadValue(other) == c,
          "module-filtered deletion destroys matching values and retains foreign values");
    impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(slots, two, FALSE);
    check(deleted == 3 && !slots->m_list.m_pHead && !TlsGetValue(slots->m_tlsIndex),
          "deleting the final value unlinks and destroys its thread node");
    DWORD tls = slots->m_tlsIndex;
    impl___1CThreadSlotData__QEAA_XZ(slots);
    check(slots->m_tlsIndex == tls,
          "destruction releases resources without rewriting native scalar fields");
    UINT64 guard;
    memcpy(&guard, storage + 80, 8);
    check(guard == 0xcdcdcdcdcdcdcdcdULL,
          "TLS operations preserve the native 80-byte object boundary");
    printf("%d failures\n", failures);
    return failures ? 1 : 0;
}
