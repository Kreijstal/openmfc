// CBaseTabbedPane — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

static_assert(sizeof(CBaseTabbedPane) == 0x4d8,
              "preserve the existing compact CBaseTabbedPane allocation");

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

// Symbol: ??0CBaseTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CBaseTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CBaseTabbedPane(iStyle); }
// Symbol: ??1CBaseTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CBaseTabbedPane__UEAA_XZ(CBaseTabbedPane* pThis) { if (pThis) pThis->~CBaseTabbedPane(); }
CBaseTabbedPane::CBaseTabbedPane(int iStyle) {
    // Retail m_pTabWnd (+0x4e8) and m_arDefaultTabsOrder (+0x4f0) exceed this
    // local 0x4d8-byte object. Their semantic fields are held in PaneCoreState;
    // this constructor does not construct retail's in-object CArray storage.
    memset(_basetabbedpane_padding, 0, sizeof(_basetabbedpane_padding));
    (void)iStyle;
}
CBaseTabbedPane::~CBaseTabbedPane() {}

// STUB: retail RVA 0x131a0 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?AddTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__AddTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(
    CBaseTabbedPane* pThis, CWnd* pNewBar, int bVisible, int bSetActive, int bDetachable) {
    (void)pThis; (void)pNewBar; (void)bVisible; (void)bSetActive; (void)bDetachable;
    return 0;
}

// STUB: retail RVA 0x14060 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
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

// STUB: retail RVA 0x14cf0 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?ConvertToTabbedDocument@CBaseTabbedPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CBaseTabbedPane__UEAAXH_Z(
    CBaseTabbedPane* pThis, int bActiveTabOnly) {
    (void)pThis; (void)bActiveTabOnly;
}

// STUB: retail RVA 0x136d0 (mfc140u) dispatches through m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?DetachPane@CBaseTabbedPane@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__DetachPane_CBaseTabbedPane__UEAAHPEAVCWnd__H_Z(
    CBaseTabbedPane* pThis, CWnd* pBar, int bHide) {
    (void)pThis; (void)pBar; (void)bHide;
    return 0;
}

// STUB: retail RVA 0x147b0 (mfc140u) reads m_pTabWnd at +0x4e8 and performs tab-control virtual dispatches; the field layout is not modeled here.
// Symbol: ?Dock@CBaseTabbedPane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CBaseTabbedPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CBaseTabbedPane* pThis, CBasePane* pTargetBar, const RECT* pRect, int dockMethod) {
    (void)pThis; (void)pTargetBar; (void)pRect; (void)dockMethod;
    return 0;
}

// STUB: retail RVA 0x14b10 (mfc140u) reads m_pTabWnd at +0x4e8 and the CArray at +0x4f0. The fields exist in PaneCoreState, but safe synchronized lookup adds std::__throw_system_error rejected by checkfile.sh; the existing GetTabID thunk also returns -1 for valid indices, leaving no complete safe C-ABI path here.
// Symbol: ?FillDefaultTabsOrderArray@CBaseTabbedPane@@QEAAXXZ
extern "C" void MS_ABI impl__FillDefaultTabsOrderArray_CBaseTabbedPane__QEAAXXZ(CBaseTabbedPane* pThis) {
    (void)pThis;
}

// STUB: retail RVA 0x13610 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?FindBarByTabNumber@CBaseTabbedPane@@UEAAPEAVCWnd@@HH@Z
extern "C" CWnd* MS_ABI impl__FindBarByTabNumber_CBaseTabbedPane__UEAAPEAVCWnd__HH_Z(
    CBaseTabbedPane* pThis, int nTabNum, int bGetWrappedBar) {
    (void)pThis; (void)nTabNum; (void)bGetWrappedBar;
    return nullptr;
}

// STUB: retail RVA 0x13590 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?FindPaneByID@CBaseTabbedPane@@UEAAPEAVCWnd@@I@Z
extern "C" CWnd* MS_ABI impl__FindPaneByID_CBaseTabbedPane__UEAAPEAVCWnd__I_Z(
    CBaseTabbedPane* pThis, unsigned int uBarID) {
    (void)pThis; (void)uBarID;
    return nullptr;
}

// STUB: retail RVA 0x13c30 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?FloatPane@CBaseTabbedPane@@MEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
extern "C" int MS_ABI impl__FloatPane_CBaseTabbedPane__MEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
    CBaseTabbedPane* pThis, const CRect* rectFloat, int dockMethod, bool bShow) {
    (void)pThis; (void)rectFloat; (void)dockMethod; (void)bShow;
    return 0;
}

// STUB: retail RVA 0x13950 (mfc140u) reads m_pTabWnd at +0x4e8 and performs tab-control virtual dispatches; the field layout is not modeled here.
// Symbol: ?FloatTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__FloatTab_CBaseTabbedPane__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    CBaseTabbedPane* pThis, CWnd* pBar, int nTabID, int dockMethod, int bHide) {
    (void)pThis; (void)pBar; (void)nTabID; (void)dockMethod; (void)bHide;
    return 0;
}

// STUB: retail RVA 0x145d0 (mfc140u) reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?GetFirstVisibleTab@CBaseTabbedPane@@UEAAPEAVCWnd@@AEAH@Z
extern "C" CWnd* MS_ABI impl__GetFirstVisibleTab_CBaseTabbedPane__UEAAPEAVCWnd__AEAH_Z(
    CBaseTabbedPane* pThis, int* pTabNumber) {
    (void)pThis; (void)pTabNumber;
    return nullptr;
}

// STUB: retail RVA 0x14ba0 (mfc140u) conditionally reads m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?GetMinSize@CBaseTabbedPane@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CBaseTabbedPane__UEBAXAEAVCSize___Z(
    const CBaseTabbedPane* pThis, CSize* pSize) {
    (void)pThis; (void)pSize;
}

// STUB: at retail RVA 0x14600 (mfc140u), passes the inherited HWND at +0x40 to an indirect helper and tests its result; the null-result path then reads m_pTabWnd at +0x4e8, which is not modeled here.
// Symbol: ?GetPaneIcon@CBaseTabbedPane@@UEAAPEAUHICON__@@H@Z
extern "C" HICON MS_ABI impl__GetPaneIcon_CBaseTabbedPane__UEAAPEAUHICON____H_Z(
    CBaseTabbedPane* pThis, int bBigIcon) {
    (void)pThis; (void)bBigIcon;
    return nullptr;
}

// STUB: retail RVA 0x14c30 (mfc140u) enumerates tab contents through m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?GetPaneList@CBaseTabbedPane@@UEAAXAEAVCObList@@PEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__GetPaneList_CBaseTabbedPane__UEAAXAEAVCObList__PEAUCRuntimeClass___Z(
    CBaseTabbedPane* pThis, CObList* pList, CRuntimeClass* pRTCFilter) {
    (void)pThis; (void)pList; (void)pRTCFilter;
}

// STUB: retail RVA 0x13d10 (mfc140u) parses CArchive's internal buffer and mutates the CList through a private helper; no verified typed archive/list path is available in this file.
// Symbol: ?LoadSiblingPaneIDs@CBaseTabbedPane@@SAXAEAVCArchive@@AEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__LoadSiblingPaneIDs_CBaseTabbedPane__SAXAEAVCArchive__AEAV__CList_II___Z(
    CArchive& archive, CList<UINT, UINT>& barIDs) {
    (void)archive; (void)barIDs;
}

// STUB: retail RVA 0x13ee0 (mfc140u) builds tab order and iterates m_pTabWnd at +0x4e8; this declaration has no pinned member there.
// Symbol: ?LoadState@CBaseTabbedPane@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CBaseTabbedPane__MEAAHPEB_WHI_Z(
    CBaseTabbedPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return 0;
}

// STUB: retail RVA 0x14680 (mfc140u) uses m_pTabWnd at +0x4e8 and other tabbed-pane state not represented by the public declaration.
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

// OpenMFC approximation from the mfc140.dll ANSI-twin entry RVA 0x13110:
// preserve CPane cleanup. Retail first destroys m_pTabWnd, then calls
// CPane::OnNcDestroy, and dispatches scalar-deleting-destructor slot 1 when
// m_bAutoDestroy (+0x4e0) is set. The tab pointer is side-table state and the
// auto-destroy flag is not modeled, so those two retail actions are omitted.
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

// OpenMFC approximation from mfc140.dll ANSI-twin entry RVA 0x13060:
// retain CBasePane::OnSize. Retail then calls private vslot +0x750 to get the
// tab control and sizes it from the pane's client rectangle; that accessor is
// not modeled in OpenMFC.
// Symbol: ?OnSize@CBaseTabbedPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CBaseTabbedPane__IEAAXIHH_Z(
    CBaseTabbedPane* pThis, unsigned int nType, int cx, int cy) {
    if (pThis) {
        impl__OnSize_CBasePane__IEAAXIHH_Z(
            static_cast<CBasePane*>(pThis), nType, cx, cy);
    }
}

// OpenMFC approximation from the mfc140.dll ANSI-twin entry RVA 0x14060:
// retain the CPane::RecalcLayout call. Retail then dispatches CMFCTabCtrl
// vslot +0x318 when m_pTabWnd (+0x4e8) is non-null; that side-table state
// cannot be read here without adding a C++ runtime symbol rejected by the
// per-file link audit, so the tab-control branch is omitted.
// Symbol: ?RecalcLayout@CBaseTabbedPane@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CBaseTabbedPane__UEAAXXZ(CBaseTabbedPane* pThis) {
    if (pThis) impl__RecalcLayout_CPane__UEAAXXZ(static_cast<CPane*>(pThis));
}

// STUB: the mfc140.dll ANSI-twin entry RVA 0x13710 reads m_pTabWnd (+0x4e8),
// removes a tab by virtual dispatch, then performs further pane/tab-control
// virtual calls. The side-table tab pointer and required operations have no
// safe C-ABI access path in this file.
// Symbol: ?RemovePane@CBaseTabbedPane@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(
    CBaseTabbedPane* pThis, CWnd* pBar) {
    (void)pThis; (void)pBar;
    return 0;
}

// STUB: the mfc140.dll ANSI-twin entry RVA 0x13d90 writes a tab count and
// per-tab IDs through CArchive's internal buffer, using m_pTabWnd (+0x4e8).
// Neither the tab list nor a verified typed archive path is available here.
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

// STUB: the mfc140.dll ANSI-twin entry RVA 0x13850 uses m_pTabWnd (+0x4e8)
// for tab-index lookup, visibility/activation updates, and pane notifications.
// Its required tab-control state and virtual operations are unavailable here.
// Symbol: ?ShowTab@CBaseTabbedPane@@UEAAHPEAVCWnd@@HHH@Z
extern "C" int MS_ABI impl__ShowTab_CBaseTabbedPane__UEAAHPEAVCWnd__HHH_Z(
    CBaseTabbedPane* pThis, CWnd* pBar, int bShow, int bDelay, int bActivate) {
    (void)pThis; (void)pBar; (void)bShow; (void)bDelay; (void)bActivate;
    return 0;
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
