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
