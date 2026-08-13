// CDockablePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int);
extern "C" int MS_ABI impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(CPane* pThis, CRect& rect, int);
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int);

// Symbol: ?CreateObject@CDockablePane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockablePane__SAPEAVCObject__XZ() {
    return new CDockablePane();
}
// Symbol: ?GetRuntimeClass@CDockablePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockablePane__UEBAPEAUCRuntimeClass__XZ(const CDockablePane*) {
    return CDockablePane::GetThisClass();
}
// Symbol: ?GetThisClass@CDockablePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ() {
    return CDockablePane::GetThisClass();
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, 240, 140);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        state.canAutoHide = TRUE;
        state.hasGripper = bHasGripper ? TRUE : FALSE;
        state.tabbed = (dwTabbedStyle != 0) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 120;
        if (state.minSize.cy <= 0) state.minSize.cy = 80;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), L"STATIC", dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@VCSize@@HIKKK@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, CSize sizeDefault,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle, unsigned long dwControlBarStyle) {
    CRect rect(0, 0, sizeDefault.cx > 0 ? sizeDefault.cx : 240, sizeDefault.cy > 0 ? sizeDefault.cy : 140);
    int created = impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, *static_cast<const RECT*>(rect), bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, nullptr);
    if (created && pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        g_paneCoreState_Cbarcore[pThis].minSize = sizeDefault;
    }
    return created;
}
// Symbol: ?CreateEx@CDockablePane@@UEAAHKPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockablePane__UEAAHKPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, unsigned long, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long dwControlBarStyle, CCreateContext* pContext) {
    return impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);
}
// Symbol: ?Dock@CDockablePane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CDockablePane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), pTargetBar, lpRect, dockMethod);
}
// Symbol: ?OnBeforeFloat@CDockablePane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CDockablePane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CRect& rect, int dockMethod) {
    return impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), rect, dockMethod);
}
// Symbol: ?DockToFrameWindow@CDockablePane@@UEAAHKPEBUtagRECT@@KPEAVCBasePane@@HH@Z
extern "C" int MS_ABI impl__DockToFrameWindow_CDockablePane__UEAAHKPEBUtagRECT__KPEAVCBasePane__HH_Z(
    CDockablePane* pThis, unsigned long, const RECT* lpRect, unsigned long, CBasePane* pTargetBar, int, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    state.parent = pTargetBar != nullptr ? static_cast<CWnd*>(pTargetBar) : state.parent;
    state.recentRect = NormalizeRect(lpRect, 240, 140);
    state.tabbed = FALSE;
    return TRUE;
}
// Symbol: ?ShowPane@CDockablePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate) {
    impl__ShowPane_CBasePane__UEAAXHHH_Z(reinterpret_cast<CBasePane*>(pThis), bShow, bDelay, bActivate);
}
CDockablePane::CDockablePane() {
    memset(_dockablepane_padding, 0, sizeof(_dockablepane_padding));
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = TRUE;
    state.canAutoHide = TRUE;
    state.hasGripper = TRUE;
}
CDockablePane::~CDockablePane() {}
BOOL CDockablePane::CanBeAttached() const { return TRUE; }
BOOL CDockablePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canAutoHide : TRUE;
}
void CDockablePane::EnableAutohideAll() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].canAutoHide = TRUE;
}
CMFCAutoHideBar* CDockablePane::SetAutoHideMode(int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar, int) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.autoHideMode = bAutoHideMode ? TRUE : FALSE;
    state.autoHideAlignment = bAutoHideMode ? dwAlignment : 0;
    state.autoHideBar = bAutoHideMode ? pAutoHideBar : nullptr;
    state.canAutoHide = TRUE;
    return reinterpret_cast<CMFCAutoHideBar*>(state.autoHideBar);
}
BOOL CDockablePane::IsAutoHideMode() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.autoHideMode : FALSE;
}
BOOL CDockablePane::IsTabbed() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.tabbed : FALSE;
}
// Symbol: ?m_sizeDragSensitivity@CDockablePane@@1VCSize@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_sizeDragSensitivity_CDockablePane__1VCSize__A = {};

// Symbol: ?AdjustPaneToPaneContainer@CDockablePane@@MEAAXPEAVCPaneDivider@@@Z
extern "C" void MS_ABI impl__AdjustPaneToPaneContainer_CDockablePane__MEAAXPEAVCPaneDivider___Z(void* /*class*/* p0) {}

// Symbol: ?AttachToTabWnd@CDockablePane@@UEAAPEAV1@PEAV1@W4AFX_DOCK_METHOD@@HPEAPEAV1@@Z
extern "C" void* MS_ABI impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(int p0, void* /*class*/** p1) {
    return nullptr;
}

// Symbol: ?CalcCorrectOffset@CDockablePane@@IEAA?AVCPoint@@VCRect@@H@Z
extern "C" void* MS_ABI impl__CalcCorrectOffset_CDockablePane__IEAA_AVCPoint__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return nullptr;
}

// Symbol: ?CalcFixedLayout@CDockablePane@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CDockablePane__UEAA_AVCSize__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?CalcRectForSliding@CDockablePane@@IEAAXAEAVCRect@@0H@Z
extern "C" void MS_ABI impl__CalcRectForSliding_CDockablePane__IEAAXAEAVCRect__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// Symbol: ?CanAcceptMiniFrame@CDockablePane@@UEBAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__CanAcceptMiniFrame_CDockablePane__UEBAHPEAVCPaneFrameWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?CheckAutoHideCondition@CDockablePane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ() {
    return 0;
}

// Symbol: ?CheckStopSlideCondition@CDockablePane@@MEAAHH@Z
extern "C" int MS_ABI impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?ConvertToTabbedDocument@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(int p0) {}

// Symbol: ?CopyState@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CDockablePane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?CreateDefaultPaneDivider@CDockablePane@@SAPEAVCPaneDivider@@KPEAVCWnd@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(unsigned long p0, void* /*class*/* p1, void* /*struct*/* p2) {
    return nullptr;
}

// Symbol: ?CreateTabbedPane@CDockablePane@@UEAAPEAVCTabbedPane@@XZ
extern "C" void* MS_ABI impl__CreateTabbedPane_CDockablePane__UEAAPEAVCTabbedPane__XZ() {
    return nullptr;
}

// Symbol: ?DockPaneContainer@CDockablePane@@UEAAHAEAVCPaneContainerManager@@KW4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockPaneContainer_CDockablePane__UEAAHAEAVCPaneContainerManager__KW4AFX_DOCK_METHOD___Z(void* /*class*/* p0, unsigned long p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15) {
    return 0;
}

// Symbol: ?DockPaneStandard@CDockablePane@@UEAAPEAVCPane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CDockablePane__UEAAPEAVCPane__AEAH_Z(int* p0) {
    return nullptr;
}

// Symbol: ?DockToWindow@CDockablePane@@UEAAHPEAV1@KPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DockToWindow_CDockablePane__UEAAHPEAV1_KPEBUtagRECT___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?DrawCaption@CDockablePane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawCaption_CDockablePane__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?EnableButton@CDockablePane@@IEAAXIH@Z
extern "C" void MS_ABI impl__EnableButton_CDockablePane__IEAAXIH_Z(unsigned int p0, int p1) {}

// Symbol: ?EnableGripper@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(int p0) {}

// Symbol: ?EnterDragMode@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__EnterDragMode_CDockablePane__MEAAXH_Z(int p0) {}

// Symbol: ?FindButton@CDockablePane@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindButton_CDockablePane__IEBAPEAVCMFCCaptionButton__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?FindButtonByHit@CDockablePane@@IEBAPEAVCMFCCaptionButton@@I@Z
extern "C" void* MS_ABI impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?GetCaptionHeight@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetDefaultPaneDivider@CDockablePane@@QEBAPEAVCPaneDivider@@XZ
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ() {
    return nullptr;
}

// Symbol: ?GetDockingStatus@CDockablePane@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@H@Z
extern "C" void* MS_ABI impl__GetDockingStatus_CDockablePane__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, void* /*class*/ p10, int p11) {
    return nullptr;
}

// Symbol: ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(void* /*class*/ p0, int p1) {
    return 0;
}

// Symbol: ?IsButtonEnabled@CDockablePane@@IEBAHI@Z
extern "C" int MS_ABI impl__IsButtonEnabled_CDockablePane__IEBAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?IsChangeState@CDockablePane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
extern "C" void* MS_ABI impl__IsChangeState_CDockablePane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, int p10, void* /*class*/** p11) {
    return nullptr;
}

// Symbol: ?IsDocked@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsInFloatingMultiPaneFrameWnd@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsVisible@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CDockablePane__UEBAHXZ() {
    return 0;
}

// Symbol: ?LoadState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CDockablePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OffsetRectForSliding@CDockablePane@@IEAAXAEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OffsetRectForSliding_CDockablePane__IEAAXAEAVCRect__HH_Z(void* /*class*/* p0, int p1, int p2) {}

// Symbol: ?OnAfterChangeParent@CDockablePane@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CDockablePane__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnAfterDock@CDockablePane@@MEAAXPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__OnAfterDock_CDockablePane__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, const void* /*struct*/* p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15) {}

// Symbol: ?OnBeforeChangeParent@CDockablePane@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CDockablePane__UEAAXPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnCancelMode@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CDockablePane__IEAAXXZ() {}

// Symbol: ?OnClose@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CDockablePane__IEAAXXZ() {}

// Symbol: ?OnContextMenu@CDockablePane@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CDockablePane__IEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDockablePane__IEAAXXZ() {}

// Symbol: ?OnDrawDragRect@CDockablePane@@MEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__OnDrawDragRect_CDockablePane__MEAAXPEBUtagRECT__0_Z(const void* /*struct*/* p0, const void* /*struct*/* p1) {}

// Symbol: ?OnLButtonDblClk@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcCalcSize@CDockablePane@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CDockablePane__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcHitTest@CDockablePane@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CDockablePane__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CDockablePane__IEAAXXZ() {}

// Symbol: ?OnNeedTipText@CDockablePane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CDockablePane__IEAAHIPEAUtagNMHDR__PEA_J_Z(unsigned int p0, void* /*struct*/* p1, __int64* p2) {
    return 0;
}

// Symbol: ?OnPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDockablePane__IEAAXXZ() {}

// Symbol: ?OnPressCloseButton@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CDockablePane__MEAAXXZ() {}

// Symbol: ?OnRButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnSetFocus@CDockablePane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CDockablePane__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetText@CDockablePane@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CDockablePane__IEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSettingChange@CDockablePane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CDockablePane__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnSlide@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__OnSlide_CDockablePane__MEAAXH_Z(int p0) {}

// Symbol: ?OnTimer@CDockablePane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CDockablePane__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnTrackCaptionButtons@CDockablePane@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnUpdateCmdUI@CDockablePane@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDockablePane__UEAAXPEAVCFrameWnd__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnUpdateToolTips@CDockablePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CDockablePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?PreTranslateMessage@CDockablePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDockablePane__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RedrawButton@CDockablePane@@IEAAXPEBVCMFCCaptionButton@@@Z
extern "C" void MS_ABI impl__RedrawButton_CDockablePane__IEAAXPEBVCMFCCaptionButton___Z(const void* /*class*/* p0) {}

// Symbol: ?RemoveCaptionButtons@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ() {}

// Symbol: ?RemoveFromDefaultPaneDivider@CDockablePane@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveFromDefaultPaneDivider_CDockablePane__QEAAXXZ() {}

// Symbol: ?ReplacePane@CDockablePane@@QEAAHPEAV1@W4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__ReplacePane_CDockablePane__QEAAHPEAV1_W4AFX_DOCK_METHOD__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?SaveState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CDockablePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CDockablePane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDockablePane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetAutoHideParents@CDockablePane@@QEAAXPEAVCMFCAutoHideBar@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?SetCaptionButtons@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtons_CDockablePane__MEAAXXZ() {}

// Symbol: ?SetDefaultPaneDivider@CDockablePane@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(void* /*struct*/* p0) {}

// Symbol: ?SetResizeMode@CDockablePane@@QEAAXH@Z
extern "C" void MS_ABI impl__SetResizeMode_CDockablePane__QEAAXH_Z(int p0) {}

// Symbol: ?Slide@CDockablePane@@UEAAXHH@Z
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(int p0, int p1) {}

// Symbol: ?StopCaptionButtonsTracking@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ() {}

// Symbol: ?StoreRecentDockSiteInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ() {}

// Symbol: ?StoreRecentTabRelatedInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CDockablePane__MEAAXXZ() {}

// Symbol: ?ToggleAutoHide@CDockablePane@@UEAAXXZ
extern "C" void MS_ABI impl__ToggleAutoHide_CDockablePane__UEAAXXZ() {}

// Symbol: ?UndockPane@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(int p0) {}

// Symbol: ?UnSetAutoHideMode@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?UpdateTooltips@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CDockablePane__IEAAXXZ() {}
