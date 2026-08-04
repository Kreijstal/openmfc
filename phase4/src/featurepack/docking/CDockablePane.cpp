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
