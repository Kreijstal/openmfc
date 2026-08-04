// CEnumConnPoints — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CEnumConnPoints@@QEAA@PEBXI@Z
extern "C" void* MS_ABI impl___0CEnumConnPoints__QEAA_PEBXI_Z(void* pThis, const void* unused0, unsigned int unused1) {
    (void)unused0;
    (void)unused1;
    return pThis;
}
// Symbol: ??1CEnumConnPoints@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumConnPoints__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCEnumConnPointsState(pThis);
    }
}
// Symbol: ?AddConnPoint@CEnumConnPoints@@QEAAXPEAUIConnectionPoint@@@Z
extern "C" void MS_ABI impl__AddConnPoint_CEnumConnPoints__QEAAXPEAUIConnectionPoint___Z(
    void* pThis, void* pConnectionPoint) {
    if (pThis == nullptr || pConnectionPoint == nullptr) {
        return;
    }
    auto& state = EnsureCEnumConnPointsState(pThis);
    state.m_connectionPoints.push_back(pConnectionPoint);
    static_cast<IUnknown*>(pConnectionPoint)->AddRef();
}
// Symbol: ?OnNext@CEnumConnPoints@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumConnPoints__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pThis == nullptr || pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto** ppInterface = static_cast<void**>(pInterface);
    std::lock_guard<std::mutex> lock(g_enumConnPointsStateMutex);
    const auto it = g_enumConnPointsStates.find(pThis);
    if (it == g_enumConnPointsStates.end() ||
        it->second.m_position >= it->second.m_connectionPoints.size()) {
        *ppInterface = nullptr;
        return static_cast<int>(S_FALSE);
    }

    void* connectionPoint = it->second.m_connectionPoints[it->second.m_position++];
    *ppInterface = connectionPoint;
    if (connectionPoint != nullptr) {
        static_cast<IUnknown*>(connectionPoint)->AddRef();
    }
    return static_cast<int>(S_OK);
}
// Symbol: ?GetInterfaceMap@CEnumConnPoints@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumConnPoints__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@CEnumConnPoints@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumConnPoints__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
