// CControlBar — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?IsDockBar@CControlBar@@UEBAHXZ
extern "C" int MS_ABI impl__IsDockBar_CControlBar__UEBAHXZ(const CControlBar* pThis) {
    (void)pThis;
    return FALSE;  // CControlBar is not a dock bar; CDockBar overrides to return TRUE
}
// Symbol: ?IsFloating@CControlBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsFloating_CControlBar__QEBAHXZ(const CControlBar* pThis) {
    return (pThis->m_dwStyle & CBRS_FLOATING) ? TRUE : FALSE;
}
// Symbol: ?IsVisible@CControlBar@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* pThis) {
    return (pThis->m_hWnd && ::IsWindowVisible(pThis->m_hWnd)) ? TRUE : FALSE;
}
// Symbol: ?EnableDocking@CControlBar@@QEAAXK@Z
extern "C" void MS_ABI impl__EnableDocking_CControlBar__QEAAXK_Z(CControlBar* pThis, DWORD dwDockStyle) {
    pThis->m_dwDockStyle = dwDockStyle;
}
// Symbol: ?SetBarStyle@CControlBar@@QEAAXK@Z
extern "C" void MS_ABI impl__SetBarStyle_CControlBar__QEAAXK_Z(CControlBar* pThis, DWORD dwStyle) {
    pThis->m_dwStyle = dwStyle;
}
// Symbol: ?GetDockingFrame@CControlBar@@QEBAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetDockingFrame_CControlBar__QEBAPEAVCFrameWnd__XZ(const CControlBar* pThis) {
    return static_cast<CFrameWnd*>(pThis->m_pDockSite);
}
// Symbol: ?SetBorders@CControlBar@@QEAAXHHHH@Z
extern "C" void MS_ABI impl__SetBorders_CControlBar__QEAAXHHHH_Z(CControlBar* pThis, int cxLeft, int cxRight, int cyTop, int cyBottom) {
    pThis->m_cxLeftBorder = cxLeft;
    pThis->m_cxRightBorder = cxRight;
    pThis->m_cyTopBorder = cyTop;
    pThis->m_cyBottomBorder = cyBottom;
}
// Symbol: ?SetInPlaceOwner@CControlBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetInPlaceOwner_CControlBar__QEAAXPEAVCWnd___Z(CControlBar* pThis, CWnd* pOwner) {
    pThis->m_pInPlaceOwner = pOwner;
}
// Symbol: ?EraseNonClient@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis) {
    (void)pThis;
    // Default no-op: non-client erase is handled by default window proc.
}
// Symbol: ?OnBarStyleChange@CControlBar@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnBarStyleChange_CControlBar__UEAAXKK_Z(CControlBar* pThis, DWORD dwOldStyle, DWORD dwNewStyle) {
    (void)pThis;
    (void)dwOldStyle;
    (void)dwNewStyle;
    // Default no-op: subclasses may override.
}
// Symbol: ?AllocElements@CControlBar@@QEAAHHH@Z
extern "C" int MS_ABI impl__AllocElements_CControlBar__QEAAHHH_Z(CControlBar* pThis, int nCount, int nSize) {
    if (nCount <= 0 || nSize <= 0) return FALSE;
    if (pThis->m_pData) free(pThis->m_pData);
    pThis->m_nCount = nCount;
    pThis->m_pData = (UINT*)calloc(nCount, nSize);
    return pThis->m_pData ? TRUE : FALSE;
}
// Symbol: ?CalcInsideRect@CControlBar@@UEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(const CControlBar* pThis, CRect& rect, int bHorz) {
    (void)bHorz;
    rect.left += pThis->m_cxLeftBorder;
    rect.right -= pThis->m_cxRightBorder;
    rect.top += pThis->m_cyTopBorder;
    rect.bottom -= pThis->m_cyBottomBorder;
}
// Symbol: ?DelayShow@CControlBar@@UEAAXH@Z
extern "C" void MS_ABI impl__DelayShow_CControlBar__UEAAXH_Z(CControlBar* pThis, int bShow) {
    if (pThis->m_hWnd) {
        ::ShowWindow(pThis->m_hWnd, bShow ? SW_SHOWNA : SW_HIDE);
    }
}
// Symbol: ?DestroyWindow@CControlBar@@UEAAHXZ
extern "C" int MS_ABI impl__DestroyWindow_CControlBar__UEAAHXZ(CControlBar* pThis) {
    if (!pThis->m_hWnd) return FALSE;
    BOOL result = ::DestroyWindow(pThis->m_hWnd);
    if (result) pThis->m_hWnd = nullptr;
    return result;
}
// Symbol: ?DoPaint@CControlBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CControlBar__UEAAXPEAVCDC___Z(CControlBar* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    // Default no-op: subclasses override to paint their content.
}
// Symbol: ?DrawBorders@CControlBar@@UEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawBorders_CControlBar__UEAAXPEAVCDC__AEAVCRect___Z(CControlBar* pThis, CDC* pDC, CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    // Default no-op: subclasses override to draw borders.
}
// Symbol: ?SetStatusText@CControlBar@@UEAAHH@Z
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAHH_Z(CControlBar* pThis, int nPane) {
    (void)pThis;
    (void)nPane;
    return -1;  // Base class: not implemented; frame should handle.
}
// Symbol: ?SetStatusText@CControlBar@@UEAAH_J@Z
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAH_J_Z(CControlBar* pThis, __int64 nPane) {
    (void)pThis;
    (void)nPane;
    return -1;  // Base class: not implemented; frame should handle.
}
// Symbol: ?WindowProc@CControlBar@@UEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CControlBar__UEAA_JI_K_J_Z(CControlBar* pThis, unsigned int msg, UINT64 wParam, __int64 lParam) {
    if (!pThis->m_hWnd) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, msg, (WPARAM)wParam, (LPARAM)lParam);
}
// Symbol: ?CalcFixedLayout@CControlBar@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CControlBar__UEAA_AVCSize__HH_Z(void* pRet, CControlBar* pThis, int bStretch, int bHorz) {
    (void)bStretch;
    (void)bHorz;
    if (pThis && pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        HWND hParent = ::GetParent(pThis->m_hWnd);
        ::ScreenToClient(hParent, (POINT*)&rc.left);
        ::ScreenToClient(hParent, (POINT*)&rc.right);
        new(pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
    } else {
        new(pRet) CSize(0, 0);
    }
    return pRet;
}
// Symbol: ?CalcDynamicLayout@CControlBar@@UEAA?AVCSize@@HK@Z
extern "C" void* MS_ABI impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(void* pRet, CControlBar* pThis, int nLength, DWORD dwMode) {
    (void)nLength;
    (void)dwMode;
    if (pThis && pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        new(pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
    } else {
        new(pRet) CSize(0, 0);
    }
    return pRet;
}
// Symbol: ?OnHelpHitTest@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return -1;  // No help context available
}
// Symbol: ?OnIdleUpdateCmdUI@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;  // Idle update handled by frame
}
// Symbol: ?OnInitialUpdate@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CControlBar__QEAAXXZ(CControlBar* pThis) {
    (void)pThis;
    // Default no-op: subclasses override as needed.
}
// Symbol: ?OnThemeChanged@CControlBar@@QEAA_JXZ
extern "C" __int64 MS_ABI impl__OnThemeChanged_CControlBar__QEAA_JXZ(CControlBar* pThis) {
    (void)pThis;
    return 0;  // Default: theme change handled by frame or subclass
}
// Symbol: ?OnTimer@CControlBar@@QEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CControlBar__QEAAX_K_Z(CControlBar* pThis, UINT64 nIDEvent) {
    (void)pThis;
    (void)nIDEvent;
    // Default no-op: subclasses override to handle timers.
}
// Symbol: ?PreTranslateMessage@CControlBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CControlBar__UEAAHPEAUtagMSG___Z(CControlBar* pThis, MSG* pMsg) {
    (void)pThis;
    (void)pMsg;
    return FALSE;  // Default: no message translation needed
}
// Symbol: ?OnMouseActivate@CControlBar@@QEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CControlBar__QEAAHPEAVCWnd__II_Z(CControlBar* pThis, CWnd* pDesktopWnd, UINT nHitTest, UINT message) {
    (void)pThis;
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    return MA_ACTIVATE;  // Default: activate on mouse click
}
// Symbol: ?OnCtlColor@CControlBar@@QEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CControlBar__QEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(CControlBar* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    (void)pThis;
    (void)pDC;
    (void)pWnd;
    (void)nCtlColor;
    return nullptr;  // Default: use system colors (no custom brush)
}
// Symbol: ?OnDestroy@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CControlBar__QEAAXXZ(CControlBar* pThis) {
    (void)pThis;
    // Default no-op: CWnd handles window destruction cleanup.
}
// Symbol: ?OnWindowPosChanging@CControlBar@@QEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(CControlBar* pThis, WINDOWPOS* pWndPos) {
    (void)pThis;
    (void)pWndPos;
    // Default no-op: subclasses override to adjust window positioning.
}
// Symbol: ?PostNcDestroy@CControlBar@@UEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CControlBar__UEAAXXZ(CControlBar* pThis) {
    if (pThis->m_bAutoDelete) {
        delete pThis;
    }
}
// Symbol: ?PreCreateWindow@CControlBar@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CControlBar__UEAAHAEAUtagCREATESTRUCTW___Z(CControlBar* pThis, CREATESTRUCTW& cs) {
    (void)pThis;
    // Default CControlBar::PreCreateWindow: ensure WS_CLIPCHILDREN
    cs.style |= WS_CLIPCHILDREN;
    return TRUE;
}
// Symbol: ?ResetTimer@CControlBar@@QEAAX_KI@Z
extern "C" void MS_ABI impl__ResetTimer_CControlBar__QEAAX_KI_Z(CControlBar* pThis, UINT64 nIDEvent, UINT uElapse) {
    if (!pThis->m_hWnd) return;
    ::KillTimer(pThis->m_hWnd, (UINT_PTR)nIDEvent);
    if (uElapse != 0) {
        ::SetTimer(pThis->m_hWnd, (UINT_PTR)nIDEvent, uElapse, nullptr);
    }
}
// Symbol: ?OnCreate@CControlBar@@QEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CControlBar__QEAAHPEAUtagCREATESTRUCTW___Z(CControlBar* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return 0;  // Success; subclasses perform additional initialization.
}
// Symbol: ?OnPaint@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CControlBar__QEAAXXZ(CControlBar* pThis) {
    (void)pThis;
    // Default no-op: subclasses (CToolBar, CStatusBar, etc.) override to paint.
}
// Symbol: ?OnLButtonDown@CControlBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CControlBar__QEAAXIVCPoint___Z(CControlBar* pThis, UINT nFlags, CPoint point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
    // Default: docking start is handled by CDockContext when set on the bar.
    // Base CControlBar defers to CWnd::OnLButtonDown.
}
// Symbol: ?OnLButtonDblClk@CControlBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CControlBar__QEAAXIVCPoint___Z(CControlBar* pThis, UINT nFlags, CPoint point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
    // Default: subclass override to toggle docked/floating state.
}
// Symbol: ?OnSizeParent@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSizeParent_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;  // Default: no action needed; subclasses reposition.
}
// Symbol: ?DrawGripper@CControlBar@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    // Default: subclasses override to draw the docking gripper.
}
// Symbol: ?DrawNCGripper@CControlBar@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawNCGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    // Default: no non-client gripper drawn by base class.
}
// Symbol: ?DrawNonThemedGripper@CControlBar@@UEAAHPEAVCDC@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__DrawNonThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    return FALSE;  // Non-themed gripper not drawn by base class.
}
// Symbol: ?DrawThemedGripper@CControlBar@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__DrawThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect__H_Z(CControlBar* pThis, CDC* pDC, const CRect& rect, int bHorz) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    (void)bHorz;
    return FALSE;  // Themed gripper not drawn; no theme support.
}
// Symbol: ?GetBarInfo@CControlBar@@QEAAXPEAVCControlBarInfo@@@Z
extern "C" void MS_ABI impl__GetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo___Z(CControlBar* pThis, void* pInfo) {
    // CControlBarInfo layout (from global_ccontrolbarinfo.cpp):
    // offset 0: m_nBarID, offset 4: m_bVisible, offset 8: m_bFloating,
    // offset 12: m_bHorz, offset 16: m_bDockBar, offset 20: m_pointPos (x,y),
    // offset 28: m_nMRUWidth, offset 32: m_bDocking, offset 40: m_rectMRUDockPos,
    // offset 56: m_dwMRUFloatStyle, offset 60: m_ptMRUFloatPos, offset 72: m_arrBarID
    if (!pInfo) return;
    unsigned char* p = (unsigned char*)pInfo;
    *(UINT*)(p + 0)  = pThis->m_hWnd ? (UINT)::GetWindowLongPtrW(pThis->m_hWnd, GWLP_ID) : 0;
    *(int*)(p + 4)   = (pThis->m_hWnd && ::IsWindowVisible(pThis->m_hWnd));
    *(int*)(p + 8)   = ((pThis->m_dwStyle & CBRS_FLOATING) != 0);
    *(int*)(p + 12)  = ((pThis->m_dwStyle & (CBRS_TOP | CBRS_BOTTOM)) == 0);  // bHorz
    *(int*)(p + 16)  = FALSE;  // CControlBar is not a dock bar
    // m_pointPos: use current window position
    if (pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        *(LONG*)(p + 20) = rc.left;
        *(LONG*)(p + 24) = rc.top;
    }
    *(UINT*)(p + 28)  = pThis->m_nMRUWidth;
    *(int*)(p + 32)   = (pThis->m_dwDockStyle != 0);
    *(DWORD*)(p + 56) = pThis->m_dwDockStyle;
}
// Symbol: ?SetBarInfo@CControlBar@@QEAAXPEAVCControlBarInfo@@PEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__SetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(CControlBar* pThis, void* pInfo, CFrameWnd* pFrame) {
    if (!pInfo) return;
    unsigned char* p = (unsigned char*)pInfo;
    int bVisible = *(int*)(p + 4);
    DWORD dwDockStyle = *(DWORD*)(p + 56);
    pThis->m_dwDockStyle = dwDockStyle;
    pThis->m_nMRUWidth = *(UINT*)(p + 28);
    if (pFrame) {
        pThis->m_pDockSite = pFrame;
    }
    if (pThis->m_hWnd && bVisible) {
        ::ShowWindow(pThis->m_hWnd, SW_SHOW);
    }
}
// Symbol: ?RecalcDelayShow@CControlBar@@UEAAKPEAUAFX_SIZEPARENTPARAMS@@@Z
extern "C" DWORD MS_ABI impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(CControlBar* pThis, void* lpLayout) {
    (void)pThis;
    (void)lpLayout;
    return 0;  // Default: no delayed-show regions.
}
CControlBar::CControlBar()
    : m_pInPlaceOwner(nullptr), m_bAutoDelete(FALSE),
      m_cxLeftBorder(0), m_cxRightBorder(0), m_cyTopBorder(0), m_cyBottomBorder(0),
      m_cxDefaultGap(0), m_nMRUWidth(0), m_nCount(0), m_pData(nullptr),
      m_hReBarTheme(nullptr), m_nStateFlags(0), m_dwStyle(0), m_dwDockStyle(0),
      m_pDockSite(nullptr), m_pDockBar(nullptr), m_pDockContext(nullptr) {
}
CControlBar::~CControlBar() {}
BOOL CControlBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;
    DWORD winStyle = dwStyle & 0xFFFF;
    if ((winStyle & WS_CHILD) == 0) winStyle |= WS_CHILD;

    m_hWnd = ::CreateWindowExW(0, L"STATIC", nullptr, winStyle,
                               0, 0, 0, 0,
                               pParentWnd->GetSafeHwnd(),
                               (HMENU)(UINT_PTR)nID,
                               AfxGetInstanceHandle(), nullptr);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);
    if (dwStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }
    return TRUE;
}
