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
