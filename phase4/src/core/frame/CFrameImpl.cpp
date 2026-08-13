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
// Symbol: ?m_lstFrames@CFrameImpl@@1V?$CList@PEAVCFrameWnd@@PEAV1@@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_lstFrames_CFrameImpl__1V__CList_PEAVCFrameWnd__PEAV1___A = {};

// Symbol: ?AddDefaultButtonsToCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@I@Z
extern "C" void MS_ABI impl__AddDefaultButtonsToCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu__I_Z(void* /*class*/* p0, unsigned int p1) {}

// Symbol: ?AddFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(void* /*class*/* p0) {}

// Symbol: ?AddTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* /*class*/* p0) {}

// Symbol: ?CreateNewToolBar@CFrameImpl@@IEAAPEBVCMFCToolBar@@PEB_W@Z
extern "C" void* MS_ABI impl__CreateNewToolBar_CFrameImpl__IEAAPEBVCMFCToolBar__PEB_W_Z(const wchar_t* p0) {
    return nullptr;
}

// Symbol: ?DeactivateMenu@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__DeactivateMenu_CFrameImpl__IEAAXXZ() {}

// Symbol: ?DeleteToolBar@CFrameImpl@@IEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetCaptionRect@CFrameImpl@@IEAA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetFreeCtrlBarID@CFrameImpl@@KAIIIAEBVCObList@@@Z
extern "C" unsigned int MS_ABI impl__GetFreeCtrlBarID_CFrameImpl__KAIIIAEBVCObList___Z(unsigned int p0, unsigned int p1, const void* /*class*/* p2) {
    return 0;
}

// Symbol: ?GetSysButton@CFrameImpl@@IEAAPEAVCMFCCaptionButtonEx@@I@Z
extern "C" void* MS_ABI impl__GetSysButton_CFrameImpl__IEAAPEAVCMFCCaptionButtonEx__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?GetUserToolBarByIndex@CFrameImpl@@IEBAPEAVCMFCToolBar@@H@Z
extern "C" void* MS_ABI impl__GetUserToolBarByIndex_CFrameImpl__IEBAPEAVCMFCToolBar__H_Z(int p0) {
    return nullptr;
}

// Symbol: ?InitUserToolbars@CFrameImpl@@IEAAXPEB_WII@Z
extern "C" void MS_ABI impl__InitUserToolbars_CFrameImpl__IEAAXPEB_WII_Z(const wchar_t* p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?IsCustomizePane@CFrameImpl@@IEBAHPEBVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__IsCustomizePane_CFrameImpl__IEBAHPEBVCMFCPopupMenu___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsDockStateValid@CFrameImpl@@IEAAHAEBVCDockState@@@Z
extern "C" int MS_ABI impl__IsDockStateValid_CFrameImpl__IEAAHAEBVCDockState___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsHelpKey@CFrameImpl@@KAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsHelpKey_CFrameImpl__KAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?IsPrintPreview@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__IsPrintPreview_CFrameImpl__IEAAHXZ() {
    return 0;
}

// Symbol: ?IsUserDefinedToolbar@CFrameImpl@@IEBAHPEBVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__IsUserDefinedToolbar_CFrameImpl__IEBAHPEBVCMFCToolBar___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?LoadDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__LoadDockState_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?LoadLargeIconsState@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__LoadLargeIconsState_CFrameImpl__IEAAHXZ() {
    return 0;
}

// Symbol: ?LoadTearOffMenus@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadTearOffMenus_CFrameImpl__IEAAXXZ() {}

// Symbol: ?LoadUserToolbars@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadUserToolbars_CFrameImpl__IEAAXXZ() {}

// Symbol: ?OnActivateApp@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__OnActivateApp_CFrameImpl__IEAAXH_Z(int p0) {}

// Symbol: ?OnCompositionChanged@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameImpl__IEAAXXZ() {}

// Symbol: ?OnGetMinMaxInfo@CFrameImpl@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFrameImpl__IEAAXPEAUtagMINMAXINFO___Z(void* /*struct*/* p0) {}

// Symbol: ?OnLButtonDown@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnLButtonUp@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnMenuChar@CFrameImpl@@IEAAHI@Z
extern "C" int MS_ABI impl__OnMenuChar_CFrameImpl__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnMouseMove@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnNcActivate@CFrameImpl@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CFrameImpl__IEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnNcCalcSize@CFrameImpl@@IEAAHHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" int MS_ABI impl__OnNcCalcSize_CFrameImpl__IEAAHHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnNcHitTest@CFrameImpl@@IEAAIVCPoint@@@Z
extern "C" unsigned int MS_ABI impl__OnNcHitTest_CFrameImpl__IEAAIVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcMouseMove@CFrameImpl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CFrameImpl__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__OnNcPaint_CFrameImpl__IEAAHXZ() {
    return 0;
}

// Symbol: ?OnSetText@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnSetText_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?OnTrackCaptionButtons@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnWindowPosChanging@CFrameImpl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Symbol: ?ProcessKeyboard@CFrameImpl@@IEAAHHPEAH@Z
extern "C" int MS_ABI impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(int p0, int* p1) {
    return 0;
}

// Symbol: ?ProcessMouseClick@CFrameImpl@@IEAAHIUtagPOINT@@PEAUHWND__@@@Z
extern "C" int MS_ABI impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(unsigned int p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?ProcessMouseMove@CFrameImpl@@IEAAHUtagPOINT@@@Z
extern "C" int MS_ABI impl__ProcessMouseMove_CFrameImpl__IEAAHUtagPOINT___Z(void* /*struct*/ p0) {
    return 0;
}

// Symbol: ?ProcessMouseWheel@CFrameImpl@@IEAAH_K_J@Z
extern "C" int MS_ABI impl__ProcessMouseWheel_CFrameImpl__IEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?RedrawCaptionButton@CFrameImpl@@IEAAXPEAVCMFCCaptionButtonEx@@@Z
extern "C" void MS_ABI impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(void* /*class*/* p0) {}

// Symbol: ?RemoveFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(void* /*class*/* p0) {}

// Symbol: ?RemoveTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__RemoveTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* /*class*/* p0) {}

// Symbol: ?RestorePosition@CFrameImpl@@IEAAXAEAUtagCREATESTRUCTW@@@Z
extern "C" void MS_ABI impl__RestorePosition_CFrameImpl__IEAAXAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {}

// Symbol: ?SaveDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SaveDockState_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?SaveTearOffMenus@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveTearOffMenus_CFrameImpl__IEAAXH_Z(int p0) {}

// Symbol: ?SaveUserToolbars@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveUserToolbars_CFrameImpl__IEAAXH_Z(int p0) {}

// Symbol: ?SetDockState@CFrameImpl@@IEAAXAEBVCDockState@@@Z
extern "C" void MS_ABI impl__SetDockState_CFrameImpl__IEAAXAEBVCDockState___Z(const void* /*class*/* p0) {}

// Symbol: ?SetHighlightedSysButton@CFrameImpl@@IEAAXI@Z
extern "C" void MS_ABI impl__SetHighlightedSysButton_CFrameImpl__IEAAXI_Z(unsigned int p0) {}

// Symbol: ?SetNewUserToolBarRTC@CFrameImpl@@QEAAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SetNewUserToolBarRTC_CFrameImpl__QEAAXPEAUCRuntimeClass___Z(void* /*struct*/* p0) {}

// Symbol: ?SetupToolbarMenu@CFrameImpl@@IEAAXAEAVCMenu@@II@Z
extern "C" void MS_ABI impl__SetupToolbarMenu_CFrameImpl__IEAAXAEAVCMenu__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?ShowQuickCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__ShowQuickCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {}

// Symbol: ?StopCaptionButtonsTracking@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CFrameImpl__IEAAXXZ() {}

// Symbol: ?StoreWindowPlacement@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StoreWindowPlacement_CFrameImpl__IEAAXXZ() {}

// Symbol: ?UpdateCaption@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateCaption_CFrameImpl__IEAAXXZ() {}
