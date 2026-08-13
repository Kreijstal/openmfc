// CBasePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?CreateEx@CBasePane@@UEAAHKPEB_W0KAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CBasePane__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CBasePane* pThis, unsigned long dwStyleEx, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID, unsigned long, CCreateContext* pContext) {
    (void)dwStyleEx;
    (void)lpszWindowName;
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 32;
        if (state.minSize.cy <= 0) state.minSize.cy = 32;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?ShowPane@CBasePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        g_paneCoreState_Cbarcore[pThis].visible = bShow ? TRUE : FALSE;
    }
    if (pThis->GetSafeHwnd() != nullptr) {
        pThis->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
    }
}
// Symbol: ?GetRuntimeClass@CBasePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBasePane__UEBAPEAUCRuntimeClass__XZ(const CBasePane*) {
    return CBasePane::GetThisClass();
}
// Symbol: ?GetThisClass@CBasePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ() {
    return CBasePane::GetThisClass();
}
CBasePane::CBasePane() {
    // Zero the retail member block (CWnd subobject is constructed already),
    // then apply the non-zero retail defaults.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(CBasePane) - sizeof(CWnd));
    new (&m_AccData) CAccessibilityData();
    m_bEnableIDChecking = TRUE;
    m_dwEnabledAlignment = CBRS_ALIGN_ANY;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = FALSE;
    state.canAutoHide = FALSE;
    state.hasGripper = FALSE;
}
CBasePane::~CBasePane() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore.erase(this);
}
BOOL CBasePane::CanAcceptPane(const CBasePane*) const { return TRUE; }
BOOL CBasePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canAutoHide : FALSE;
}
BOOL CBasePane::CanBeAttached() const { return TRUE; }
BOOL CBasePane::CanBeClosed() const { return TRUE; }
BOOL CBasePane::CanBeDocked() const { return TRUE; }
BOOL CBasePane::CanBeFloating() const { return CanFloat(); }
BOOL CBasePane::CanBeResized() const { return TRUE; }
BOOL CBasePane::CanFloat() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canFloat : FALSE;
}
BOOL CBasePane::DoesAllowSiblingBars() const { return TRUE; }
BOOL CBasePane::HasGripper() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.hasGripper : FALSE;
}
BOOL CBasePane::IsAccessibilityCompatible() { return TRUE; }
void CBasePane::SetPaneAlignment(DWORD dwAlignment) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].alignment = dwAlignment;
}
DWORD CBasePane::GetPaneAlignment() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.alignment : 0;
}
void* CBasePane::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra) {
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        PaneCoreState& state = g_paneCoreState_Mfccore[this];
        CRect rect = state.recentRect;
        if ((nFlags & SWP_NOMOVE) == 0) {
            rect.left = x;
            rect.top = y;
        }
        if ((nFlags & SWP_NOSIZE) == 0) {
            rect.right = rect.left + std::max(0, cx);
            rect.bottom = rect.top + std::max(0, cy);
        }
        state.recentRect = rect;
    }
    HWND hwnd = GetSafeHwnd();
    if (hwnd != nullptr) {
        HWND hwndInsertAfter = pWndInsertAfter != nullptr ? pWndInsertAfter->GetSafeHwnd() : nullptr;
        ::SetWindowPos(hwnd, hwndInsertAfter, x, y, cx, cy, nFlags);
    }
    return pExtra;
}
void CBasePane::CalcFixedLayout(BOOL bStretch, BOOL bHorz) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];

    if (state.minSize.cx <= 0) state.minSize.cx = bHorz ? 120 : 64;
    if (state.minSize.cy <= 0) state.minSize.cy = bHorz ? 64 : 120;

    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, state.minSize.cx, state.minSize.cy);
    }

    if (bStretch) {
        if (bHorz) {
            if (state.recentRect.Height() < state.minSize.cy) {
                state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
            }
            if (state.recentRect.Width() < state.minSize.cx) {
                state.recentRect.right = state.recentRect.left + state.minSize.cx;
            }
        } else {
            if (state.recentRect.Width() < state.minSize.cx) {
                state.recentRect.right = state.recentRect.left + state.minSize.cx;
            }
            if (state.recentRect.Height() < state.minSize.cy) {
                state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
            }
        }
    } else {
        state.recentRect.right = state.recentRect.left + state.minSize.cx;
        state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
    }

    m_rectBar = state.recentRect;
    state.visible = (m_hWnd != nullptr) ? TRUE : FALSE;
}
void CBasePane::RecalcLayout() {
    HWND hwnd = GetSafeHwnd();
    if (hwnd) {
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
    }
}
// Symbol: ?accHitTest@CBasePane@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CBasePane__UEAAJJJPEAUtagVARIANT___Z(long p0, long p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?accLocation@CBasePane@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CBasePane__UEAAJPEAJ000UtagVARIANT___Z(long* p0, long* p1, long* p2, long* p3, void* /*struct*/ p4) {
    return 0;
}

// Symbol: ?accSelect@CBasePane@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_CBasePane__UEAAJJUtagVARIANT___Z(long p0, void* /*struct*/ p1) {
    return 0;
}

// Symbol: ?AddPane@CBasePane@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddPane_CBasePane__QEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?AdjustDockingLayout@CBasePane@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(void* p0) {}

// Symbol: ?CopyState@CBasePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CBasePane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?DockPaneUsingRTTI@CBasePane@@QEAAXH@Z
extern "C" void MS_ABI impl__DockPaneUsingRTTI_CBasePane__QEAAXH_Z(int p0) {}

// Symbol: ?DoPaint@CBasePane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CBasePane__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?get_accChild@CBasePane@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CBasePane__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* /*struct*/ p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?get_accChildCount@CBasePane@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CBasePane__UEAAJPEAJ_Z(long* p0) {
    return 0;
}

// Symbol: ?get_accDefaultAction@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accDescription@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accFocus@CBasePane@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_CBasePane__UEAAJPEAUtagVARIANT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?get_accHelp@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accHelpTopic@CBasePane@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_CBasePane__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(wchar_t** p0, void* /*struct*/ p1, long* p2) {
    return 0;
}

// Symbol: ?get_accKeyboardShortcut@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accName@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accParent@CBasePane@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CBasePane__UEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?get_accRole@CBasePane@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?get_accSelection@CBasePane@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_CBasePane__UEAAJPEAUtagVARIANT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?get_accState@CBasePane@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?get_accValue@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?GetCurrentAlignment@CBasePane@@UEBAKXZ
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ() {
    return 0;
}

// Symbol: ?GetDockingMode@CBasePane@@UEBA?AW4AFX_DOCK_TYPE@@XZ
extern "C" void* MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(int /*enum*/* p0, short* p1, int p2, char p3, void* p4, void* p5, unsigned long p6, int p7, void* p8, void* p9, unsigned char* p10) {
    return nullptr;
}

// Symbol: ?GetDockSiteFrameWnd@CBasePane@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ() {
    return nullptr;
}

// Symbol: ?GetParentMiniFrame@CBasePane@@UEBAPEAVCPaneFrameWnd@@H@Z
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(int p0) {
    return nullptr;
}

// Symbol: ?GetParentTabbedPane@CBasePane@@QEBAPEAVCBaseTabbedPane@@XZ
extern "C" void* MS_ABI impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ() {
    return nullptr;
}

// Symbol: ?GetParentTabWnd@CBasePane@@QEBAPEAVCMFCBaseTabCtrl@@AEAPEAUHWND__@@@Z
extern "C" void* MS_ABI impl__GetParentTabWnd_CBasePane__QEBAPEAVCMFCBaseTabCtrl__AEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return nullptr;
}

// Symbol: ?HandleInitDialog@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__HandleInitDialog_CBasePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?InsertPane@CBasePane@@QEAAHPEAV1@0H@Z
extern "C" int MS_ABI impl__InsertPane_CBasePane__QEAAHPEAV1_0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?IsDocked@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsDocked_CBasePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsMDITabbed@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsMDITabbed_CBasePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsPointNearDockSite@CBasePane@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CBasePane__QEBAHVCPoint__AEAKAEAH_Z(void* /*class*/ p0, unsigned long* p1, int* p2) {
    return 0;
}

// Symbol: ?IsTabbed@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsTabbed_CBasePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsVisible@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CBasePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?LoadState@CBasePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CBasePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?MoveWindow@CBasePane@@UEAAPEAXAEBVCRect@@HPEAX@Z
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(const void* /*class*/* p0, int p1, void* p2) {
    return nullptr;
}

// Symbol: ?OnEraseBkgnd@CBasePane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CBasePane__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnGetObject@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetObject_CBasePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnHelpHitTest@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CBasePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnIdleUpdateCmdUI@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CBasePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnPaint@CBasePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CBasePane__IEAAXXZ() {}

// Symbol: ?OnPaneContextMenu@CBasePane@@UEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnPrintClient@CBasePane@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CBasePane__IEAA_JPEAVCDC__I_Z(void* /*class*/* p0, unsigned int p1) {
    return 0;
}

// Symbol: ?OnSetAccData@CBasePane@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CBasePane__UEAAHJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnSetIcon@CBasePane@@IEAAPEAUHICON__@@HPEAU2@@Z
extern "C" void* MS_ABI impl__OnSetIcon_CBasePane__IEAAPEAUHICON____HPEAU2__Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?OnSettingChange@CBasePane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CBasePane__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnSize@CBasePane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CBasePane__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?PaneFromPoint@CBasePane@@QEBAPEAV1@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CBasePane__QEBAPEAV1_VCPoint__H_NPEAUCRuntimeClass___Z(int p0, bool p1, void* /*struct*/* p2) {
    return nullptr;
}

// Symbol: ?PreTranslateMessage@CBasePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RemovePaneFromDockManager@CBasePane@@QEAAXPEAV1@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?SaveState@CBasePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CBasePane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?WindowProc@CBasePane@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CBasePane__MEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}
