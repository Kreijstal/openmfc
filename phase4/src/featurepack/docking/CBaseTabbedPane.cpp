// CBaseTabbedPane — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MultiPaneFrameSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/Pane15MsgmapSupport.h"
#include <utility>

static_assert(sizeof(CBaseTabbedPane) == 0x518);

// The existing thunk calls the virtual CanFloat method on this object.
extern "C" int MS_ABI impl__CanFloat_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CPane__UEAAXXZ(CPane* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CPane__IEAAXXZ(CPane* pThis);
extern "C" void MS_ABI impl__OnSetFocus_CDockablePane__IEAAXPEAVCWnd___Z(
    CDockablePane* pThis, CWnd* pOldWnd);
extern "C" void MS_ABI impl__OnSize_CBasePane__IEAAXIHH_Z(
    CBasePane* pThis, unsigned int nType, int cx, int cy);
extern "C" void MS_ABI impl__Serialize_CDockablePane__UEAAXAEAVCArchive___Z(
    CDockablePane* pThis, CArchive& archive);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis);
extern "C" int MS_ABI impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(const CMFCBaseTabCtrl*, HWND);
extern "C" int MS_ABI impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(const CMFCBaseTabCtrl*, int);

namespace {
void RecordTabMembership(CWnd* window, CWnd* parent, BOOL tabbed) {
    if (!window->IsKindOf(RUNTIME_CLASS(CDockablePane))) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto& state = g_paneCoreState_Mfccore[window];
    state.tabbed = tabbed;
    state.tabWindow = tabbed && parent ? parent->m_hWnd : nullptr;
    state.parent = parent;
}
}

void openmfc::detail::FinishTabInsertion(CBaseTabbedPane& group, CWnd& pane, HWND previous) {
    if (previous != ::GetParent(group.m_hWnd) && pane.IsKindOf(RUNTIME_CLASS(CPane)))
        static_cast<CPane&>(pane).ReleaseFloatingFrame();
    RecordTabMembership(&pane, group.GetTabs(), TRUE);
}

// Symbol: ??0CBaseTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CBaseTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CBaseTabbedPane(iStyle); }
// Symbol: ??1CBaseTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CBaseTabbedPane__UEAA_XZ(CBaseTabbedPane* pThis) { if (pThis) pThis->~CBaseTabbedPane(); }
CBaseTabbedPane::CBaseTabbedPane(int) {
    static_assert(offsetof(CBaseTabbedPane, m_bAutoDestroy) == 0x4e0);
    static_assert(offsetof(CBaseTabbedPane, m_pTabWnd) == 0x4e8);
}

CBaseTabbedPane::~CBaseTabbedPane() {
    m_bAutoDestroy = FALSE;
    if (m_hWnd) DestroyWindow();
    delete m_pTabWnd;
}

const AFX_MSGMAP* AFXAPI CBaseTabbedPane::GetThisMessageMap() {
    return &openmfc::detail::pane15msgmap::classCBaseTabbedPane_msgmap;
}

const AFX_MSGMAP* CBaseTabbedPane::GetMessageMap() const {
    return GetThisMessageMap();
}

void CBaseTabbedPane::PostNcDestroy() {
    CDockablePane::PostNcDestroy();
    delete std::exchange(m_pTabWnd, nullptr);
    if (m_bAutoDestroy) delete this;
}

void CBaseTabbedPane::OnSize(UINT, int, int) {
    Default();
    RecalcLayout();
}

void CBaseTabbedPane::RecalcLayout() {
    if (!m_pTabWnd || !m_pTabWnd->m_hWnd) return;
    RECT client{};
    ::GetClientRect(m_hWnd, &client);
    ::SetWindowPos(m_pTabWnd->m_hWnd, nullptr, 0, 0, client.right, client.bottom,
        SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CBaseTabbedPane::AddTab(CWnd* window, BOOL visible, BOOL activate, BOOL detachable) {
    return InsertTab(window, -1, visible, activate, detachable);
}

BOOL CBaseTabbedPane::InsertTab(CWnd* window, int index, BOOL visible, BOOL activate, BOOL detachable) {
    if (!m_pTabWnd || !m_pTabWnd->m_hWnd || !window || window == this || !window->m_hWnd) return FALSE;
    const int existing = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(m_pTabWnd, window->m_hWnd);
    if (existing >= 0) return ShowTab(window, visible, activate);
    const HWND oldParent = ::GetParent(window->m_hWnd);
    if (oldParent != m_pTabWnd->m_hWnd) {
        ::SetLastError(ERROR_SUCCESS);
        if (!::SetParent(window->m_hWnd, m_pTabWnd->m_hWnd) && ::GetLastError() != ERROR_SUCCESS) return FALSE;
    }
    if (index < 0 || index > m_pTabWnd->GetTabsCount()) index = m_pTabWnd->GetTabsCount();
    if (!m_pTabWnd->InsertTab(window, nullptr, index, static_cast<UINT>(-1), detachable)) {
        if (oldParent != m_pTabWnd->m_hWnd) ::SetParent(window->m_hWnd, oldParent);
        return FALSE;
    }
    openmfc::detail::FinishTabInsertion(*this, *window, oldParent);
    m_pTabWnd->ShowTab(index, visible, TRUE, FALSE);
    if (visible && activate) m_pTabWnd->ActivateTab(index);
    return TRUE;
}

BOOL CBaseTabbedPane::RemovePane(CWnd* window) {
    if (!m_pTabWnd || !window || !window->m_hWnd) return FALSE;
    const int index = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(m_pTabWnd, window->m_hWnd);
    if (!m_pTabWnd->RemoveTab(index)) return FALSE;
    if (::GetParent(window->m_hWnd) == m_pTabWnd->m_hWnd) {
        const HWND parent = ::GetParent(m_hWnd);
        RECT bounds{};
        ::GetWindowRect(window->m_hWnd, &bounds);
        ::MapWindowPoints(nullptr, parent, reinterpret_cast<POINT*>(&bounds), 2);
        ::SetParent(window->m_hWnd, parent);
        ::SetWindowPos(window->m_hWnd, nullptr, bounds.left, bounds.top, bounds.right - bounds.left,
            bounds.bottom - bounds.top, SWP_NOACTIVATE | SWP_NOZORDER);
        RecordTabMembership(window, static_cast<CWnd*>(CWnd::FromHandlePermanent(parent)), FALSE);
    }
    return TRUE;
}

BOOL CBaseTabbedPane::ShowTab(CWnd* window, BOOL show, BOOL activate) {
    if (!m_pTabWnd || !window || !window->m_hWnd) return FALSE;
    const int index = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(m_pTabWnd, window->m_hWnd);
    if (!m_pTabWnd->ShowTab(index, show, TRUE, FALSE)) return FALSE;
    if (show && activate) m_pTabWnd->ActivateTab(index);
    return TRUE;
}

// Symbol: ?AddTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__AddTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(
    CBaseTabbedPane* pThis, CWnd* pNewBar, int bVisible, int bSetActive, int bDetachable) {
    return pThis && pThis->AddTab(pNewBar, bVisible, bSetActive, bDetachable);
}

// STUB: retail RVA 0x14060.
// Symbol: ?ApplyRestoredTabInfo@CBaseTabbedPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(
    CBaseTabbedPane* pThis, int bUseTabIndexes) {
    (void)pThis; (void)bUseTabIndexes;
}

// OpenMFC approximation: retail RVA 0x140c0 (mfc140u) tail-jumps to the full CBasePane::CanFloat body at RVA 0xc7c0 (mfc140u). The existing base thunk dispatches to CPane::CanFloat here (TRUE), so this preserves the local base-class behavior but not retail's docking checks.
// Symbol: ?CanFloat@CBaseTabbedPane@@UEBAHXZ
extern "C" int MS_ABI impl__CanFloat_CBaseTabbedPane__UEBAHXZ(const CBaseTabbedPane* pThis) {
    return impl__CanFloat_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis));
}

// STUB: retail RVA 0x14cf0.
// Symbol: ?ConvertToTabbedDocument@CBaseTabbedPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CBaseTabbedPane__UEAAXH_Z(
    CBaseTabbedPane* pThis, int bActiveTabOnly) {
    (void)pThis; (void)bActiveTabOnly;
}

// Symbol: ?DetachPane@CBaseTabbedPane@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__DetachPane_CBaseTabbedPane__UEAAHPEAVCWnd__H_Z(
    CBaseTabbedPane* pThis, CWnd* pBar, int bHide) {
    if (!pThis || !pThis->RemovePane(pBar)) return FALSE;
    ::ShowWindow(pBar->m_hWnd, bHide ? SW_HIDE : SW_SHOWNOACTIVATE);
    return TRUE;
}

// STUB: retail RVA 0x147b0.
// Symbol: ?Dock@CBaseTabbedPane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CBaseTabbedPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CBaseTabbedPane* pThis, CBasePane* pTargetBar, const RECT* pRect, int dockMethod) {
    (void)pThis; (void)pTargetBar; (void)pRect; (void)dockMethod;
    return 0;
}

// STUB: retail RVA 0x14b10 stores tab IDs in m_arDefaultTabsOrder.
// Symbol: ?FillDefaultTabsOrderArray@CBaseTabbedPane@@QEAAXXZ
extern "C" void MS_ABI impl__FillDefaultTabsOrderArray_CBaseTabbedPane__QEAAXXZ(CBaseTabbedPane* pThis) {
    (void)pThis;
}

// Symbol: ?FindBarByTabNumber@CBaseTabbedPane@@UEAAPEAVCWnd@@HH@Z
extern "C" CWnd* MS_ABI impl__FindBarByTabNumber_CBaseTabbedPane__UEAAPEAVCWnd__HH_Z(
    CBaseTabbedPane* pThis, int nTabNum, int bGetWrappedBar) {
    (void)bGetWrappedBar;
    return pThis && pThis->GetTabs() ? pThis->GetTabs()->GetTabWnd(nTabNum) : nullptr;
}

// Symbol: ?FindPaneByID@CBaseTabbedPane@@UEAAPEAVCWnd@@I@Z
extern "C" CWnd* MS_ABI impl__FindPaneByID_CBaseTabbedPane__UEAAPEAVCWnd__I_Z(
    CBaseTabbedPane* pThis, unsigned int uBarID) {
    auto* tabs = pThis ? pThis->GetTabs() : nullptr;
    if (!tabs) return nullptr;
    for (int i = 0; i < tabs->GetTabsCount(); ++i) {
        auto* pane = tabs->GetTabWnd(i);
        if (pane && pane->m_hWnd && static_cast<UINT>(::GetDlgCtrlID(pane->m_hWnd)) == uBarID)
            return pane;
    }
    return nullptr;
}

// Symbol: ?FloatPane@CBaseTabbedPane@@MEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
extern "C" int MS_ABI impl__FloatPane_CBaseTabbedPane__MEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
    CBaseTabbedPane* group, const CRect* bounds, int method, bool show) {
    return group && bounds && group->FloatPaneWindow(*static_cast<const RECT*>(*bounds), method, show);
}

// Symbol: ?FloatTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CBaseTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    CBaseTabbedPane* group, CWnd* window, int index, int method, int hide) {
    auto* tabs = group ? group->GetTabs() : nullptr;
    if (!tabs || !window || !window->IsKindOf(RUNTIME_CLASS(CPane))) return FALSE;
    if (index < 0) index = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(tabs, window->m_hWnd);
    if (tabs->GetTabWnd(index) != window || !tabs->IsTabDetachable(index)) return FALSE;
    auto* pane = static_cast<CPane*>(window);
    return pane->FloatPaneWindow(pane->m_recentDockInfo.m_layout.m_rectRecentFloatingRect, method, !hide);
}

// Symbol: ?GetFirstVisibleTab@CBaseTabbedPane@@UEAAPEAVCWnd@@AEAH@Z
extern "C" CWnd* MS_ABI impl__GetFirstVisibleTab_CBaseTabbedPane__UEAAPEAVCWnd__AEAH_Z(
    CBaseTabbedPane* pThis, int* pTabNumber) {
    if (pTabNumber) *pTabNumber = -1;
    auto* tabs = pThis ? pThis->GetTabs() : nullptr;
    if (!tabs) return nullptr;
    for (int i = 0; i < tabs->GetTabsCount(); ++i) {
        if (impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(tabs, i)) {
            if (pTabNumber) *pTabNumber = i;
            return tabs->GetTabWnd(i);
        }
    }
    return nullptr;
}

// STUB: retail RVA 0x14ba0.
// Symbol: ?GetMinSize@CBaseTabbedPane@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CBaseTabbedPane__UEBAXAEAVCSize___Z(
    const CBaseTabbedPane* pThis, CSize* pSize) {
    (void)pThis; (void)pSize;
}

// STUB: at retail RVA 0x14600.
// Symbol: ?GetPaneIcon@CBaseTabbedPane@@UEAAPEAUHICON__@@H@Z
extern "C" HICON MS_ABI impl__GetPaneIcon_CBaseTabbedPane__UEAAPEAUHICON____H_Z(
    CBaseTabbedPane* pThis, int bBigIcon) {
    (void)pThis; (void)bBigIcon;
    return nullptr;
}

// Symbol: ?GetPaneList@CBaseTabbedPane@@UEAAXAEAVCObList@@PEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__GetPaneList_CBaseTabbedPane__UEAAXAEAVCObList__PEAUCRuntimeClass___Z(
    CBaseTabbedPane* pThis, CObList* pList, CRuntimeClass* pRTCFilter) {
    auto* tabs = pThis ? pThis->GetTabs() : nullptr;
    if (!tabs || !pList) return;
    for (int i = 0; i < tabs->GetTabsCount(); ++i) {
        auto* pane = tabs->GetTabWnd(i);
        if (pane && (!pRTCFilter || pane->IsKindOf(pRTCFilter))) pList->AddTail(pane);
    }
}

// STUB: retail RVA 0x13d10 (mfc140u) parses CArchive's internal buffer and mutates the CList through a private helper; no verified typed archive/list path is available in this file.
// Symbol: ?LoadSiblingPaneIDs@CBaseTabbedPane@@SAXAEAVCArchive@@AEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__LoadSiblingPaneIDs_CBaseTabbedPane__SAXAEAVCArchive__AEAV__CList_II___Z(
    CArchive& archive, CList<UINT, UINT>& barIDs) {
    (void)archive; (void)barIDs;
}

// STUB: retail RVA 0x13ee0.
// Symbol: ?LoadState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CBaseTabbedPane__MEAAHPEB_WHI_Z(
    CBaseTabbedPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return 0;
}

// STUB: retail RVA 0x14680.
// Symbol: ?OnChangeActiveTab@CBaseTabbedPane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeActiveTab_CBaseTabbedPane__IEAA_J_K_J_Z(
    CBaseTabbedPane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// STUB: no RVA is resolved for this handler in the retail symbol map.
// Symbol: ?OnEraseBkgnd@CBaseTabbedPane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CBaseTabbedPane__IEAAHPEAVCDC___Z(
    CBaseTabbedPane* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
    return 0;
}

// CWnd::OnNcDestroy invokes PostNcDestroy after detaching the HWND. The
// tab control and any window-owned pane are released there.
// Symbol: ?OnNcDestroy@CBaseTabbedPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CBaseTabbedPane__IEAAXXZ(CBaseTabbedPane* pThis) {
    if (pThis) impl__OnNcDestroy_CPane__IEAAXXZ(static_cast<CPane*>(pThis));
}

// OpenMFC approximation from mfc140.dll ANSI-twin entry RVA 0x140a0:
// call CDockablePane::OnSetFocus (entry RVA 0x46260, mfc140). Retail then
// asks private vslot +0x750 for this pane's tab control and calls SetFocus on
// it; OpenMFC has no safe accessor for that unexported virtual, so that step
// is omitted.
// Symbol: ?OnSetFocus@CBaseTabbedPane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CBaseTabbedPane__IEAAXPEAVCWnd___Z(
    CBaseTabbedPane* pThis, CWnd* pOldWnd) {
    if (pThis) {
        impl__OnSetFocus_CDockablePane__IEAAXPEAVCWnd___Z(
            static_cast<CDockablePane*>(pThis), pOldWnd);
    }
}

// Symbol: ?OnSize@CBaseTabbedPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CBaseTabbedPane__IEAAXIHH_Z(
    CBaseTabbedPane* pThis, unsigned int nType, int cx, int cy) {
    if (pThis) pThis->OnSize(nType, cx, cy);
}

// Symbol: ?RecalcLayout@CBaseTabbedPane@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CBaseTabbedPane__UEAAXXZ(CBaseTabbedPane* pThis) {
    if (pThis) pThis->RecalcLayout();
}

// Symbol: ?RemovePane@CBaseTabbedPane@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(
    CBaseTabbedPane* pThis, CWnd* pBar) {
    return pThis && pThis->RemovePane(pBar);
}

// STUB: the mfc140.dll ANSI-twin entry RVA 0x13d90 writes a tab count and
// per-tab IDs through CArchive's internal buffer, using m_pTabWnd (+0x4e8).
// The archive format remains unimplemented.
// Symbol: ?SaveSiblingBarIDs@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SaveSiblingBarIDs_CBaseTabbedPane__UEAAXAEAVCArchive___Z(
    CBaseTabbedPane* pThis, CArchive& archive) {
    (void)pThis; (void)archive;
}

// STUB: the exact Unicode export ?SaveState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
// is unresolved in the mfc140u map. Its ANSI char-signature counterpart is at
// RVA 0x13f70 (mfc140) and traverses tab state; it is not the same export.
// Symbol: ?SaveState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CBaseTabbedPane__MEAAHPEB_WHI_Z(
    CBaseTabbedPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return 0;
}

// Retail mfc140.dll ANSI-twin entry RVA 0x13ca0 is a tail jump to
// CDockablePane::Serialize (entry RVA 0x45660, mfc140); delegate through its
// existing C-ABI thunk.
// Symbol: ?Serialize@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CBaseTabbedPane__UEAAXAEAVCArchive___Z(
    CBaseTabbedPane* pThis, CArchive& archive) {
    if (pThis) {
        impl__Serialize_CDockablePane__UEAAXAEAVCArchive___Z(
            static_cast<CDockablePane*>(pThis), archive);
    }
}

// STUB: the mfc140.dll ANSI-twin entry RVA 0x13cb0 checks m_pTabWnd (+0x4e8)
// and, when non-null, dispatches vslot +0x10 with the CArchive reference.
// The pointer is side-table state with no safe C-ABI accessor in this file.
// Symbol: ?SerializeTabWindow@CBaseTabbedPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeTabWindow_CBaseTabbedPane__UEAAXAEAVCArchive___Z(
    CBaseTabbedPane* pThis, CArchive& archive) {
    (void)pThis; (void)archive;
}

// STUB: the mfc140.dll ANSI-twin entry RVA 0x140e0 contains a substantial
// tab/auto-hide transition using m_pTabWnd (+0x4e8), multiple tab-control
// virtuals, and pane state not represented by a safe accessor in this file.
// Symbol: ?SetAutoHideMode@CBaseTabbedPane@@UEAAPEAVCMFCAutoHideBar@@HKPEAV2@H@Z
extern "C" CMFCAutoHideBar* MS_ABI impl__SetAutoHideMode_CBaseTabbedPane__UEAAPEAVCMFCAutoHideBar__HKPEAV2_H_Z(
    CBaseTabbedPane* pThis, int bMode, unsigned long dwAlignment,
    CMFCAutoHideBar* pCurrAutoHideBar, int bUseTimer) {
    (void)pThis; (void)bMode; (void)dwAlignment; (void)pCurrAutoHideBar; (void)bUseTimer;
    return nullptr;
}

// Symbol: ?ShowTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__ShowTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(
    CBaseTabbedPane* pThis, CWnd* pBar, int bShow, int bDelay, int bActivate) {
    (void)bDelay;
    return pThis && pThis->ShowTab(pBar, bShow, bActivate);
}

// OpenMFC approximation from the mfc140.dll ANSI-twin entry RVA 0x13b70:
// delegate to CDockablePane::StoreRecentDockSiteInfo. Retail tail-jumps to it
// after walking m_pTabWnd's tabs and calling StoreRecentDockSiteInfo on CPane
// tabs; the tab pointer is side-table state, and safe synchronized access adds
// a C++ runtime symbol rejected by the per-file link audit, so that loop is omitted.
// Symbol: ?StoreRecentDockSiteInfo@CBaseTabbedPane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CBaseTabbedPane__MEAAXXZ(CBaseTabbedPane* pThis) {
    if (pThis) {
        impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(
            static_cast<CDockablePane*>(pThis));
    }
}
