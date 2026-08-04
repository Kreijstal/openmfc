// CTabbedPane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?m_arTabsAutoColors@CTabbedPane@@1V?$CArray@KK@@A
extern "C" __attribute__((used)) void* impl__m_arTabsAutoColors_CTabbedPane__1V__CArray_KK__A = nullptr;
// Symbol: ?m_bIsTabsAutoColor@CTabbedPane@@1HA
extern "C" __attribute__((used)) int impl__m_bIsTabsAutoColor_CTabbedPane__1HA = FALSE;
// Symbol: ?m_bTabsAlwaysTop@CTabbedPane@@2HA
extern "C" __attribute__((used)) int impl__m_bTabsAlwaysTop_CTabbedPane__2HA = FALSE;
// Symbol: ?m_lstTabbedControlBars@CTabbedPane@@1V?$CList@PEAUHWND__@@PEAU1@@@A
extern "C" __attribute__((used)) void* impl__m_lstTabbedControlBars_CTabbedPane__1V__CList_PEAUHWND____PEAU1___A = nullptr;
// Symbol: ?m_StyleTabWnd@CTabbedPane@@2W4Style@CMFCTabCtrl@@A
extern "C" __attribute__((used)) int impl__m_StyleTabWnd_CTabbedPane__2W4Style_CMFCTabCtrl__A = 0;
// Symbol: ?CheckTabbedBarAlignment@CTabbedPane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckTabbedBarAlignment_CTabbedPane__MEAAHXZ(void*) {
    return TRUE;
}
// Symbol: ?CreateObject@CTabbedPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTabbedPane__SAPEAVCObject__XZ() {
    return new GenericFeaturePackObject();
}
// Symbol: ?DetachPane@CTabbedPane@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__DetachPane_CTabbedPane__UEAAHPEAVCWnd__H_Z(void* pThis, void* pane, int) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    auto& tabs = g_tabbedPanes[pThis].tabs;
    tabs.erase(std::remove(tabs.begin(), tabs.end(), pane), tabs.end());
    return TRUE;
}
// Symbol: ?EnableTabAutoColor@CTabbedPane@@SAXH@Z
extern "C" void MS_ABI impl__EnableTabAutoColor_CTabbedPane__SAXH_Z(int enable) {
    impl__m_bIsTabsAutoColor_CTabbedPane__1HA = enable;
}
// Symbol: ?FloatTab@CTabbedPane@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    void* pThis, void* pane, int, int, int) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    auto& tabs = g_tabbedPanes[pThis].tabs;
    tabs.erase(std::remove(tabs.begin(), tabs.end(), pane), tabs.end());
    return TRUE;
}
// Symbol: ?GetMessageMap@CTabbedPane@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTabbedPane__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CTabbedPane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabbedPane__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_classCTabbedPane;
}
// Symbol: ?GetTabArea@CTabbedPane@@UEBAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__GetTabArea_CTabbedPane__UEBAXAEAVCRect__0_Z(void* pThis, CRect* tabArea, CRect* paneArea) {
    CRect bounds(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    CWnd* wnd = reinterpret_cast<CWnd*>(pThis);
    if (wnd && wnd->GetSafeHwnd()) {
        RECT rc = {};
        if (::GetClientRect(wnd->GetSafeHwnd(), &rc)) bounds = CRect(rc);
    }
    const int tabHeight = 24;
    if (impl__m_bTabsAlwaysTop_CTabbedPane__2HA) {
        if (tabArea) *tabArea = CRect(bounds.left, bounds.top, bounds.right, bounds.top + tabHeight);
        if (paneArea) *paneArea = CRect(bounds.left, bounds.top + tabHeight, bounds.right, bounds.bottom);
    } else {
        if (tabArea) *tabArea = CRect(bounds.left, bounds.bottom - tabHeight, bounds.right, bounds.bottom);
        if (paneArea) *paneArea = CRect(bounds.left, bounds.top, bounds.right, bounds.bottom - tabHeight);
    }
}
// Symbol: ?GetThisClass@CTabbedPane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabbedPane__SAPEAUCRuntimeClass__XZ() {
    return &g_classCTabbedPane;
}
// Symbol: ?GetThisMessageMap@CTabbedPane@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTabbedPane__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?IsTabLocationBottom@CTabbedPane@@UEBAHXZ
extern "C" int MS_ABI impl__IsTabLocationBottom_CTabbedPane__UEBAHXZ(void*) {
    return !impl__m_bTabsAlwaysTop_CTabbedPane__2HA;
}
// Symbol: ?OnCreate@CTabbedPane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CTabbedPane__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW*) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    g_tabbedPanes[pThis] = {};
    return 0;
}
// Symbol: ?OnDestroy@CTabbedPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTabbedPane__IEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    g_tabbedPanes.erase(pThis);
}
// Symbol: ?OnPressCloseButton@CTabbedPane@@MEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CTabbedPane__MEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    auto& tabs = g_tabbedPanes[pThis].tabs;
    if (!tabs.empty()) tabs.pop_back();
}
// Symbol: ?ResetTabs@CTabbedPane@@SAXXZ
extern "C" void MS_ABI impl__ResetTabs_CTabbedPane__SAXXZ() {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    g_tabbedPanes.clear();
}
// Symbol: ?SetTabAutoColors@CTabbedPane@@SAXAEBV?$CArray@KK@@@Z
extern "C" void MS_ABI impl__SetTabAutoColors_CTabbedPane__SAXAEBV__CArray_KK___Z(const void* colors) {
    impl__m_arTabsAutoColors_CTabbedPane__1V__CArray_KK__A = const_cast<void*>(colors);
    impl__m_bIsTabsAutoColor_CTabbedPane__1HA = TRUE;
}
// Symbol: ??0CTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CTabbedPane(iStyle); }
// Symbol: ??1CTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CTabbedPane__UEAA_XZ(CTabbedPane* pThis) { if (pThis) pThis->~CTabbedPane(); }
CTabbedPane::CTabbedPane(int iStyle) : CBaseTabbedPane(iStyle) { memset(_tabbedpane_padding, 0, sizeof(_tabbedpane_padding)); }
CTabbedPane::~CTabbedPane() {}
