// CHandleMap — OpenMFC implementation.
// Sources: global_chandlemap.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CHandleMapSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?FromHandle@CHandleMap@@QEAAPEAVCObject@@PEAX@Z
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?DeleteTemp@CHandleMap@@QEAAXXZ
// ---------------------------------------------------------------------------
// public: __cdecl CHandleMap::CHandleMap(CRuntimeClass*, void (*)(CObject*),
//                                        void (*)(CObject*), size_t, int)
//
// Field-for-field reproduction of the retail mfc140u constructor. Retail:
//   m_nObjectSize          = max(pClass->m_nObjectSize, 8)
//   m_nMaxTemp             = 0x40
//   reserved 0x08 / 0x10   = 0
//   permanent map          : hash size 17, block size 10
//   temporary map          : hash size 17, block size 4  -- then forced to 7
//                            once both destructor callbacks are known good
//   m_pClass/m_nOffset/m_nHandles from the arguments
// Retail treats a null callback as a hard error (it branches to a noreturn
// helper); we mirror that as a defensive early-out instead of faulting.
//
// The embedded map blocks exist purely for binary-layout fidelity -- the live
// handle bookkeeping is the side table above -- so their vptr slot is pointed
// at a private sentinel. Nothing in OpenMFC virtual-dispatches through them.
// Symbol: ??0CHandleMap@@QEAA@PEAUCRuntimeClass@@P6AXPEAVCObject@@@Z2_KH@Z
// Symbol: ?DeleteTemp@CHandleMap@@QEAAXXZ
extern "C" void MS_ABI impl__DeleteTemp_CHandleMap__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_handleMapMutex);
    auto it = g_tempHandleMap.find(pThis);
    if (it == g_tempHandleMap.end()) {
        return;
    }

    for (auto& entry : it->second) {
        delete entry.second;
    }
    g_tempHandleMap.erase(it);
}
// Symbol: ?FromHandle@CHandleMap@@QEAAPEAVCObject@@PEAX@Z
extern "C" void* MS_ABI impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(
    void* pThis, void* hHandle) {
    if (pThis == nullptr || hHandle == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_handleMapMutex);
    auto& table = g_tempHandleMap[pThis];
    auto it = table.find(hHandle);
    if (it != table.end()) {
        return it->second;
    }

    auto* object = new (std::nothrow) CTempHandleObject_ManualSmallStubImplementations();
    if (object == nullptr) {
        return nullptr;
    }
    object->m_handle = hHandle;
    table.emplace(hHandle, object);
    return object;
}
