// CFrameImpl — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    g_frameImplStates.insert_or_assign(frameImpl, FrameImplState{pFrame, nullptr, manager != nullptr ? manager->GetRuntimeClass() : nullptr});
    return pThis;
}
// Symbol: ??1CFrameImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis) {
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    (void)g_frameImplStates.erase(frameImpl);
}
// Symbol: ?OnShowPopupMenu@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@PEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd) {
    (void)pThis;
    (void)pPopupMenu;
    (void)pWnd;
    return FALSE;
}
// Symbol: ?OnShowCustomizePane@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@I@Z
extern "C" int MS_ABI impl__OnShowCustomizePane_CFrameImpl__IEAAHPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, UINT uiToolbarID) {
    (void)pThis;
    (void)pMenuPopup;
    (void)uiToolbarID;
    return FALSE;
}
// Symbol: ?OnCloseFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCloseFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates.erase(frameImpl);
}
// Symbol: ?OnLoadFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnLoadFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    FrameImplState& state = g_frameImplStates[frameImpl];
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
}
// Symbol: ?OnChangeVisualManager@CFrameImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }

    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    CFrameWnd* ownerFrame = nullptr;
    CWnd* menuBarWnd = nullptr;

    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        FrameImplState& state = g_frameImplStates[frameImpl];
        ownerFrame = state.ownerFrame;
        menuBarWnd = reinterpret_cast<CWnd*>(state.menuBar);
        CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
        state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
    }

    if (ownerFrame != nullptr && ownerFrame->GetSafeHwnd() != nullptr) {
        ownerFrame->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
    if (menuBarWnd != nullptr && menuBarWnd->GetSafeHwnd() != nullptr) {
        menuBarWnd->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
}
// Symbol: ?SetMenuBar@CFrameImpl@@IEAAXPEAVCMFCMenuBar@@@Z
extern "C" void MS_ABI impl__SetMenuBar_CFrameImpl__IEAAXPEAVCMFCMenuBar___Z(void* pThis, void* pMenuBar) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates[frameImpl].menuBar = pMenuBar;
}
