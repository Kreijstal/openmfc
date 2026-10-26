// Native release-x64 per-object slot allocation and thread-value lookup.
#include "detail/CThreadLocalObjectSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
using namespace openmfc::detail::cthreadlocalobject;
// Symbol: ??1CThreadLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject* self) {
    int slot = self->m_nSlot;
    if (slot && threadSlots)
        threadSlots->FreeSlot(slot);
    self->m_nSlot = 0;
}
// Symbol: ?GetData@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
extern "C" void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
    S_Cthreadlocalobject* self, void* factory) {
    if (!factory)
        impl__AfxThrowInvalidArgException__YAXXZ();
    int slot = self->m_nSlot;
    if (!slot) {
        auto* slots = InitializeThreadSlots();
        if (!slots)
            impl__AfxThrowInvalidArgException__YAXXZ();
        slot = slots->AllocSlot();
        self->m_nSlot = slot;
        if (!slot)
            impl__AfxThrowInvalidArgException__YAXXZ();
    }
    void* value = threadSlots->GetThreadValue(slot);
    if (!value) {
        using Factory = void*(MS_ABI*)();
        value = reinterpret_cast<Factory>(factory)();
        threadSlots->SetValue(self->m_nSlot, value);
    }
    return value;
}
// Symbol: ?GetDataNA@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@XZ
extern "C" void* MS_ABI
impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(S_Cthreadlocalobject* self) {
    int slot = self->m_nSlot;
    auto* slots = threadSlots;
    return slot && slots ? slots->GetThreadValue(slot) : nullptr;
}

// Symbol: ?AfxInitLocalData@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z(HINSTANCE module) {
    if (auto* slots = threadSlots)
        slots->AssignInstance(module);
}
// Symbol: ?AfxTermLocalData@@YAXPEAUHINSTANCE__@@H@Z
extern "C" void MS_ABI impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z(HINSTANCE module, BOOL all) {
    if (auto* slots = threadSlots)
        slots->DeleteValues(module, all);
}
// Symbol: ?AfxTlsAddRef@@YAXXZ
extern "C" void MS_ABI impl__AfxTlsAddRef__YAXXZ() { ++tlsReferences; }
// Symbol: ?AfxTlsRelease@@YAXXZ
extern "C" void MS_ABI impl__AfxTlsRelease__YAXXZ() {
    // The native counter saturates at zero on release, and an already-zero
    // release still destroys any published slot container. AddRef wraps.
    if (tlsReferences)
        --tlsReferences;
    if (!tlsReferences && threadSlots) {
        threadSlots->~CThreadSlotData();
        threadSlots = nullptr;
    }
}
