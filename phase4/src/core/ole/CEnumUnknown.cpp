// CEnumUnknown — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?OnNext@CEnumUnknown@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumUnknown__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }

    auto** ppInterface = static_cast<void**>(pInterface);
    if (ppInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }

    if (pThis == nullptr) {
        *ppInterface = nullptr;
        return static_cast<int>(S_FALSE);
    }

    *ppInterface = nullptr;
    return static_cast<int>(S_FALSE);
}
// Symbol: ??1CEnumUnknown@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumUnknown__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<CCmdTarget*>(pThis)->~CCmdTarget();
}
// Symbol: ?GetInterfaceMap@CEnumUnknown@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumUnknown__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@CEnumUnknown@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumUnknown__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
