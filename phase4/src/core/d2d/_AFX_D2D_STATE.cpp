// _AFX_D2D_STATE — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ??0_AFX_D2D_STATE@@QEAA@XZ
extern "C" _AFX_D2D_STATE* MS_ABI impl___0_AFX_D2D_STATE__QEAA_XZ(_AFX_D2D_STATE* pThis) {
    if (!pThis) return nullptr;
    return new(pThis) _AFX_D2D_STATE();
}
// Symbol: ??1_AFX_D2D_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1_AFX_D2D_STATE__UEAA_XZ(_AFX_D2D_STATE* pThis) {
    if (!pThis) return;
    _AFX_D2D_STATE& state = *pThis;
    state.initialized = false;
    state.d2dFactoryType = 0;
    state.dWriteFactoryType = 0;
    {
        std::lock_guard<std::mutex> lock(g_userToolMutex);
        g_d2dInitialized.erase(pThis);
    }
    pThis->~_AFX_D2D_STATE();
}
// Symbol: ?InitD2D@_AFX_D2D_STATE@@QEAAHW4D2D1_FACTORY_TYPE@@W4DWRITE_FACTORY_TYPE@@@Z
extern "C" int MS_ABI impl__InitD2D__AFX_D2D_STATE__QEAAHW4D2D1_FACTORY_TYPE__W4DWRITE_FACTORY_TYPE___Z(void* pThis, int d2dFactoryType, int writeFactoryType) {
    auto* pState = static_cast<_AFX_D2D_STATE*>(pThis);
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    if (pState) {
        pState->initialized = true;
        pState->d2dFactoryType = d2dFactoryType;
        pState->dWriteFactoryType = writeFactoryType;
    }
    g_d2dInitialized.insert(pThis);
    return TRUE;
}
// Symbol: ?ReleaseD2DRefs@_AFX_D2D_STATE@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseD2DRefs__AFX_D2D_STATE__QEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    g_d2dInitialized.erase(pThis);
}
