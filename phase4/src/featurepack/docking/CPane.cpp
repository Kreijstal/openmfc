// CPane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPane__SAPEAVCObject__XZ() {
    return new CPane();
}
// Symbol: ?GetRuntimeClass@CPane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPane__UEBAPEAUCRuntimeClass__XZ(const CPane*) {
    return CPane::GetThisClass();
}
// Symbol: ?GetThisClass@CPane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ() {
    return CPane::GetThisClass();
}
// Symbol: ?Create@CPane@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CPane* pThis, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect,
    CWnd* pParentWnd, unsigned int nID, unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        if (state.minSize.cx <= 0) state.minSize.cx = 64;
        if (state.minSize.cy <= 0) state.minSize.cy = 64;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?CreateEx@CPane@@UEAAHKPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CPane__UEAAHKPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CPane* pThis, unsigned long, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect,
    CWnd* pParentWnd, unsigned int nID, unsigned long cbStyle, CCreateContext* pContext) {
    return impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
        pThis, lpszClassName, dwStyle, rect, pParentWnd, nID, cbStyle, pContext);
}
// Symbol: ?Dock@CPane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int) {
    if (pThis == nullptr || pTargetBar == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    state.parent = static_cast<CWnd*>(pTargetBar);
    state.recentRect = NormalizeRect(lpRect);
    state.tabbed = FALSE;
    return TRUE;
}
// Symbol: ?DockPane@CPane@@UEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockPane_CPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        pThis, pTargetBar, lpRect, dockMethod);
}
// Symbol: ?OnBeforeFloat@CPane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(CPane* pThis, CRect& rect, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    g_paneCoreState_Cbarcore[pThis].recentRect = rect;
    return TRUE;
}
// Symbol: ?OnAfterFloat@CPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnAfterFloat_CPane__UEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    g_paneCoreState_Cbarcore[pThis].visible = TRUE;
}
// Symbol: ?CalcRecentDockedRect@CPane@@QEAAXXZ
extern "C" void MS_ABI impl__CalcRecentDockedRect_CPane__QEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, 200, 120);
    }
}
// Symbol: ?AdjustSizeImmediate@CPane@@UEAAXH@Z
extern "C" void MS_ABI impl__AdjustSizeImmediate_CPane__UEAAXH_Z(CPane* pThis, int nLength) {
    if (pThis == nullptr || nLength <= 0) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    if (state.minSize.cx < nLength) state.minSize.cx = nLength;
    if (state.minSize.cy < nLength) state.minSize.cy = nLength;
}
CPane::CPane() {
    std::memset(reinterpret_cast<char*>(this) + sizeof(CBasePane), 0,
                sizeof(CPane) - sizeof(CBasePane));
    m_cxLeftBorder = m_cxRightBorder = 6;
    m_cyTopBorder = m_cyBottomBorder = 6;
    m_cxDefaultGap = 1;
    m_nMRUWidth = 32767;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].canFloat = TRUE;
}
CPane::~CPane() {}
BOOL CPane::CanBeDocked() const { return TRUE; }
BOOL CPane::CanFloat() const { return TRUE; }
void CPane::RecalcLayout() {
    CBasePane::RecalcLayout();
}
// Symbol: ?AllocElements@CPane@@QEAAHHH@Z
extern "C" int MS_ABI impl__AllocElements_CPane__QEAAHHH_Z(int p0, int p1) {
    return 0;
}

// Symbol: ?CalcAvailableSize@CPane@@UEAA?AVCSize@@VCRect@@@Z
extern "C" void* MS_ABI impl__CalcAvailableSize_CPane__UEAA_AVCSize__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return nullptr;
}

// Symbol: ?CalcInsideRect@CPane@@QEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__CalcInsideRect_CPane__QEBAXAEAVCRect__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?CanBeTabbedDocument@CPane@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeTabbedDocument_CPane__UEBAHXZ() {
    return 0;
}

// Symbol: ?ConvertToTabbedDocument@CPane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CPane__UEAAXH_Z(int p0) {}

// Symbol: ?CopyState@CPane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CPane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?CreateDefaultMiniframe@CPane@@UEAAPEAVCPaneFrameWnd@@VCRect@@@Z
extern "C" void* MS_ABI impl__CreateDefaultMiniframe_CPane__UEAAPEAVCPaneFrameWnd__VCRect___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?DockByMouse@CPane@@UEAAHPEAVCBasePane@@@Z
extern "C" int MS_ABI impl__DockByMouse_CPane__UEAAHPEAVCBasePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?EnterDragMode@CPane@@MEAAXH@Z
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(int p0) {}

// Symbol: ?FloatPane@CPane@@UEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
extern "C" int MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(void* /*class*/ p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14, bool p15) {
    return 0;
}

// Symbol: ?GetAvailableExpandSize@CPane@@UEBAHXZ
extern "C" int MS_ABI impl__GetAvailableExpandSize_CPane__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetAvailableStretchSize@CPane@@UEBAHXZ
extern "C" int MS_ABI impl__GetAvailableStretchSize_CPane__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetBorders@CPane@@QEBA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetBorders_CPane__QEBA_AVCRect__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetPaneName@CPane@@UEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneName_CPane__UEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, void** p1, void* p2) {}

// Symbol: ?GetVirtualRect@CPane@@QEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?IsChangeState@CPane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
extern "C" void* MS_ABI impl__IsChangeState_CPane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, int p10, void* /*class*/** p11) {
    return nullptr;
}

// Symbol: ?IsLeftOf@CPane@@QEBA_NVCRect@@_N@Z
extern "C" int MS_ABI impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(void* /*class*/ p0, bool p1) {
    return 0;  // false
}

// Symbol: ?IsTabbed@CPane@@UEBAHXZ
extern "C" int MS_ABI impl__IsTabbed_CPane__UEBAHXZ() {
    return 0;
}

// Symbol: ?LoadState@CPane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?MoveByAlignment@CPane@@QEAAHKH@Z
extern "C" int MS_ABI impl__MoveByAlignment_CPane__QEAAHKH_Z(unsigned long p0, int p1) {
    return 0;
}

// Symbol: ?MovePane@CPane@@UEAA?AVCSize@@VCRect@@HAEAPEAX@Z
extern "C" void* MS_ABI impl__MovePane_CPane__UEAA_AVCSize__VCRect__HAEAPEAX_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void** p3) {
    return nullptr;
}

// Symbol: ?OnAfterChangeParent@CPane@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnBeforeChangeParent@CPane@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnBeforeDock@CPane@@MEAAHPEAPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeDock_CPane__MEAAHPEAPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(void* /*class*/** p0, const void* /*struct*/* p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15) {
    return 0;
}

// Symbol: ?OnCancelMode@CPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CPane__IEAAXXZ() {}

// Symbol: ?OnChar@CPane@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnChar_CPane__IEAAXIII_Z(unsigned int p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?OnContextMenu@CPane@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnContinueMoving@CPane@@MEAAXXZ
extern "C" void MS_ABI impl__OnContinueMoving_CPane__MEAAXXZ() {}

// Symbol: ?OnDestroy@CPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CPane__IEAAXXZ() {}

// Symbol: ?OnLButtonDblClk@CPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CPane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CPane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseMove@CPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CPane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcDestroy@CPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CPane__IEAAXXZ() {}

// Symbol: ?OnPressCloseButton@CPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CPane__UEAAXXZ() {}

// Symbol: ?OnProcessDblClk@CPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnProcessDblClk_CPane__UEAAXXZ() {}

// Symbol: ?OnRTLChanged@CPane@@MEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CPane__MEAAXH_Z(int p0) {}

// Symbol: ?OnShowControlBarMenu@CPane@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__OnShowControlBarMenu_CPane__UEAAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnStyleChanged@CPane@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CPane__IEAAXHPEAUtagSTYLESTRUCT___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?PrepareToDock@CPane@@UEAAXPEAVCDockSite@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__PrepareToDock_CPane__UEAAXPEAVCDockSite__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14) {}

// Symbol: ?RemoveFromMiniframe@CPane@@MEAAXPEAVCWnd@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14) {}

// Symbol: ?SaveState@CPane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?SetActiveInGroup@CPane@@UEAAXH@Z
extern "C" void MS_ABI impl__SetActiveInGroup_CPane__UEAAXH_Z(int p0) {}

// Symbol: ?SetBorders@CPane@@QEAAXHHHH@Z
extern "C" void MS_ABI impl__SetBorders_CPane__QEAAXHHHH_Z(int p0, int p1, int p2, int p3) {}

// Symbol: ?SetBorders@CPane@@QEAAXPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__SetBorders_CPane__QEAAXPEBUtagRECT___Z(const void* /*struct*/* p0) {}

// Symbol: ?SetDockState@CPane@@UEAAXPEAVCDockingManager@@@Z
extern "C" void MS_ABI impl__SetDockState_CPane__UEAAXPEAVCDockingManager___Z(void* /*class*/* p0) {}

// Symbol: ?SetDragMode@CPane@@MEAAXH@Z
extern "C" void MS_ABI impl__SetDragMode_CPane__MEAAXH_Z(int p0) {}

// Symbol: ?SetVirtualRect@CPane@@QEAAXAEBVCRect@@H@Z
extern "C" void MS_ABI impl__SetVirtualRect_CPane__QEAAXAEBVCRect__H_Z(const void* /*class*/* p0, int p1) {}

// Symbol: ?StoreRecentDockSiteInfo@CPane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPane__MEAAXXZ() {}

// Symbol: ?StretchPaneDeferWndPos@CPane@@UEAAHHAEAPEAX@Z
extern "C" int MS_ABI impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(int p0, void** p1) {
    return 0;
}

// Symbol: ?UndockPane@CPane@@UEAAXH@Z
extern "C" void MS_ABI impl__UndockPane_CPane__UEAAXH_Z(int p0) {}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCSize___Z(void* /*class*/ p0) {}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ() {}
