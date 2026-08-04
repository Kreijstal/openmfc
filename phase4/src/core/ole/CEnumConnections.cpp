// CEnumConnections — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CEnumConnections@@QEAA@PEBXI@Z
extern "C" void* MS_ABI impl___0CEnumConnections__QEAA_PEBXI_Z(void* pThis, const void* unused0, unsigned int unused1) {
    (void)unused0;
    (void)unused1;
    return pThis;
}
// Symbol: ??1CEnumConnections@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumConnections__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCEnumConnectionsState(pThis);
    }
}
// Symbol: ?AddConnection@CEnumConnections@@QEAAXPEAUtagCONNECTDATA@@@Z
extern "C" void MS_ABI impl__AddConnection_CEnumConnections__QEAAXPEAUtagCONNECTDATA___Z(
    void* pThis, void* pConnectData) {
    if (pThis == nullptr || pConnectData == nullptr) {
        return;
    }
    auto& state = EnsureCEnumConnectionsState(pThis);
    const CONNECTDATA connectData = *static_cast<CONNECTDATA*>(pConnectData);
    state.m_connections.push_back(connectData);
    if (connectData.pUnk != nullptr) {
        connectData.pUnk->AddRef();
    }
}
// Symbol: ?OnClone@CEnumConnections@@MEAAPEAVCEnumArray@@XZ
extern "C" void* MS_ABI impl__OnClone_CEnumConnections__MEAAPEAVCEnumArray__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::vector<CONNECTDATA> originalConnections;
    {
        std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
        const auto it = g_enumConnectionsStates.find(pThis);
        if (it == g_enumConnectionsStates.end()) {
            return nullptr;
        }
        originalConnections = it->second.m_connections;
    }

    void* pClone = ::operator new(sizeof(void*));
    if (pClone == nullptr) {
        return nullptr;
    }
    impl___0CEnumConnections__QEAA_PEBXI_Z(pClone, nullptr, 0);
    auto& cloneState = EnsureCEnumConnectionsState(pClone, true);
    cloneState.m_connections = std::move(originalConnections);
    for (const CONNECTDATA& connection : cloneState.m_connections) {
        if (connection.pUnk != nullptr) {
            connection.pUnk->AddRef();
        }
    }
    return pClone;
}
// Symbol: ?OnNext@CEnumConnections@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumConnections__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pThis == nullptr || pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    CONNECTDATA* pConnectData = static_cast<CONNECTDATA*>(pInterface);
    std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
    const auto it = g_enumConnectionsStates.find(pThis);
    if (it == g_enumConnectionsStates.end() || it->second.m_position >= it->second.m_connections.size()) {
        pConnectData->pUnk = nullptr;
        pConnectData->dwCookie = 0;
        return static_cast<int>(S_FALSE);
    }

    CONNECTDATA result = it->second.m_connections[it->second.m_position++];
    pConnectData->pUnk = result.pUnk;
    pConnectData->dwCookie = result.dwCookie;
    if (pConnectData->pUnk != nullptr) {
        pConnectData->pUnk->AddRef();
    }
    return static_cast<int>(S_OK);
}
// Symbol: ?GetInterfaceMap@CEnumConnections@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumConnections__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@CEnumConnections@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumConnections__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
