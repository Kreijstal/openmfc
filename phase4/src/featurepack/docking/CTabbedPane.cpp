// CTabbedPane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/Pane17MsgmapSupport.h"

extern "C" CRuntimeClass* impl__m_pTabWndRTC_CTabbedPane__2PEAUCRuntimeClass__EA;
extern "C" int MS_ABI impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(CDockablePane*, CREATESTRUCTW*);
extern "C" int MS_ABI impl__DetachPane_CBaseTabbedPane__UEAAHPEAVCWnd__H_Z(CBaseTabbedPane*, CWnd*, BOOL);

// Symbol: ?m_arTabsAutoColors@CTabbedPane@@1V?$CArray@KK@@A
extern "C" void* impl__m_arTabsAutoColors_CTabbedPane__1V__CArray_KK__A = nullptr;
// Symbol: ?m_bIsTabsAutoColor@CTabbedPane@@1HA
extern "C" int impl__m_bIsTabsAutoColor_CTabbedPane__1HA = FALSE;
// Symbol: ?m_bTabsAlwaysTop@CTabbedPane@@2HA
extern "C" int impl__m_bTabsAlwaysTop_CTabbedPane__2HA = FALSE;
// Symbol: ?m_lstTabbedControlBars@CTabbedPane@@1V?$CList@PEAUHWND__@@PEAU1@@@A
extern "C" void* impl__m_lstTabbedControlBars_CTabbedPane__1V__CList_PEAUHWND____PEAU1___A = nullptr;
// Symbol: ?m_StyleTabWnd@CTabbedPane@@2W4Style@CMFCTabCtrl@@A
extern "C" int impl__m_StyleTabWnd_CTabbedPane__2W4Style_CMFCTabCtrl__A = 0;
// Neither the mfc140.dll nor mfc140u.dll lookup resolves this export to an RVA;
// keep the behavior as a placeholder rather than infer it without the function body.
// Symbol: ?CheckTabbedBarAlignment@CTabbedPane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckTabbedBarAlignment_CTabbedPane__MEAAHXZ(void*) {
    return TRUE;
}
// Symbol: ?CreateObject@CTabbedPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTabbedPane__SAPEAVCObject__XZ() {
    return CTabbedPane::CreateObject();
}
// Symbol: ?DetachPane@CTabbedPane@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__DetachPane_CTabbedPane__UEAAHPEAVCWnd__H_Z(CTabbedPane* group, CWnd* pane, BOOL hide) {
    return impl__DetachPane_CBaseTabbedPane__UEAAHPEAVCWnd__H_Z(group, pane, hide);
}
// Symbol: ?EnableTabAutoColor@CTabbedPane@@SAXH@Z
extern "C" void MS_ABI impl__EnableTabAutoColor_CTabbedPane__SAXH_Z(int enable) {
    impl__m_bIsTabsAutoColor_CTabbedPane__1HA = enable;
}
extern "C" int MS_ABI impl__FloatTab_CBaseTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    CBaseTabbedPane*, CWnd*, int, int, int);

// Symbol: ?FloatTab@CTabbedPane@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    CTabbedPane* group, CWnd* pane, int index, int method, int hide) {
    if (!group || !group->GetTabs() || group->GetTabs()->GetTabsCount() <= 1) return FALSE;
    return impl__FloatTab_CBaseTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(group, pane, index, method, hide);
}
// GetMessageMap/GetThisMessageMap/GetRuntimeClass/GetThisClass for CTabbedPane live in
// featurepack/docking/MessageMaps.cpp and featurepack/docking/RuntimeClasses.cpp, whose
// descriptors match retail: the message map at 0x18030f6c0 in mfc140.dll chains to
// ?GetThisMessageMap@CBaseTabbedPane@@KAPEBUAFX_MSGMAP@@XZ, and the CRuntimeClass at
// 0x1803b14a8 in mfc140u.dll (0x1803aa4a8 in mfc140.dll) is "CTabbedPane", size 1304,
// schema 0x80000002, base CBaseTabbedPane. The copies that used to be here returned
// CWnd's message map and g_classCTabbedPane, an 8-byte descriptor based on CWnd.
// Symbol: ?GetTabArea@CTabbedPane@@UEBAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__GetTabArea_CTabbedPane__UEBAXAEAVCRect__0_Z(void* pThis, CRect* tabArea, CRect* paneArea) {
    const auto* group = static_cast<const CTabbedPane*>(pThis);
    const auto* state = group && group->GetTabs() ? FindTabCtrlState(group->GetTabs()) : nullptr;
    if (tabArea) *tabArea = state ? state->strip : CRect(0, 0, 0, 0);
    if (paneArea) *paneArea = state ? state->page : CRect(0, 0, 0, 0);
}
// Symbol: ?IsTabLocationBottom@CTabbedPane@@UEBAHXZ
extern "C" int MS_ABI impl__IsTabLocationBottom_CTabbedPane__UEBAHXZ(void*) {
    return !impl__m_bTabsAlwaysTop_CTabbedPane__2HA;
}
// Symbol: ?OnCreate@CTabbedPane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CTabbedPane__IEAAHPEAUtagCREATESTRUCTW___Z(CTabbedPane* pane, CREATESTRUCTW* create) {
    return pane ? pane->OnCreate(create) : -1;
}

int CTabbedPane::OnCreate(CREATESTRUCTW* create) {
    if (impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(this, create) == -1) return -1;
    auto* type = impl__m_pTabWndRTC_CTabbedPane__2PEAUCRuntimeClass__EA;
    if (!type) type = CMFCTabCtrl::GetThisClass();
    if (!type->IsDerivedFrom(RUNTIME_CLASS(CMFCTabCtrl))) return -1;
    std::unique_ptr<CMFCTabCtrl> tabs(static_cast<CMFCTabCtrl*>(type->CreateObject()));
    RECT client{};
    ::GetClientRect(m_hWnd, &client);
    if (!tabs || !tabs->CreateTabs(impl__m_StyleTabWnd_CTabbedPane__2W4Style_CMFCTabCtrl__A,
            client, this, 1, impl__m_bTabsAlwaysTop_CTabbedPane__2HA ? 1 : 0, FALSE)) return -1;
    m_pTabWnd = tabs.release();
    return 0;
}
// Symbol: ?OnDestroy@CTabbedPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTabbedPane__IEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_tabbedPaneMutex);
    g_tabbedPanes.erase(pThis);
}
// Symbol: ?OnPressCloseButton@CTabbedPane@@MEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CTabbedPane__MEAAXXZ(CTabbedPane* pane) {
    if (auto* tabs = pane ? pane->GetTabs() : nullptr)
        pane->ShowTab(tabs->GetTabWnd(tabs->GetActiveTab()), FALSE, FALSE);
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
static_assert(sizeof(CTabbedPane) == 0x518);

CTabbedPane::CTabbedPane(int style) : CBaseTabbedPane(style) {}
CTabbedPane::~CTabbedPane() = default;

const AFX_MSGMAP* AFXAPI CTabbedPane::GetThisMessageMap() {
    return &openmfc::detail::pane17msgmap::classCTabbedPane_msgmap;
}
const AFX_MSGMAP* CTabbedPane::GetMessageMap() const {
    return GetThisMessageMap();
}
