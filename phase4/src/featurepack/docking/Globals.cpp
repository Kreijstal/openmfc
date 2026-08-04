// Globals — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?m_pTabWndRTC@CTabbedPane@@2PEAUCRuntimeClass@@EA
extern "C" __attribute__((used)) CRuntimeClass* impl__m_pTabWndRTC_CTabbedPane__2PEAUCRuntimeClass__EA = nullptr;
// Symbol: ?AfxGetTypeLibCache@@YAPEAVCTypeLibCache@@PEBU_GUID@@@Z
extern "C" void* MS_ABI impl__AfxGetTypeLibCache__YAPEAVCTypeLibCache__PEBU_GUID___Z(const GUID* guid) {
    if (!guid) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_typeLibCacheMutex);
    return GetGuidTypeLibCache(*guid);
}
// Symbol: ?AfxGetD2DState@@YAPEAV_AFX_D2D_STATE@@XZ
extern "C" _AFX_D2D_STATE* MS_ABI impl__AfxGetD2DState__YAPEAV_AFX_D2D_STATE__XZ() {
    return &g_d2dState;
}
