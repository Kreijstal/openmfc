// OLE/COM Implementation
// Provides MFC wrappers around the OLE32/OLEAUT32 COM APIs
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <algorithm>
#include <cstring>
#include <cwchar>
#include <cstdio>
#include <docobj.h>
#include <map>
#include <new>
#include <shlobj.h>
#include <vector>
#include "openmfc/afxdao.h"

// MinGW compat: Ambient property DISPIDs
#ifndef DISPID_AMBIENT_BACKCOLOR
#define DISPID_AMBIENT_BACKCOLOR    (-701)
#define DISPID_AMBIENT_DISPLAYNAME  (-702)
#define DISPID_AMBIENT_FONT         (-703)
#define DISPID_AMBIENT_FORECOLOR    (-704)
#define DISPID_AMBIENT_LOCALEID     (-705)
#define DISPID_AMBIENT_USERMODE     (-709)
#define DISPID_AMBIENT_UIDEAD       (-710)
#define DISPID_AMBIENT_SHOWGRABHANDLES (-711)
#define DISPID_AMBIENT_SHOWHATCHING (-712)
#define DISPID_AMBIENT_DISPLAYASDEFAULT (-713)
#define DISPID_AMBIENT_SUPPORTSMNEMONICS (-714)
#define DISPID_AMBIENT_APPEARANCE   (-716)
#define DISPID_AMBIENT_SCALEUNITS   (-807)
#endif
#ifndef DISPID_AMBIENT_TEXTALIGN
#define DISPID_AMBIENT_TEXTALIGN    (-708)
#endif
#ifndef VT_COLOR
#define VT_COLOR 0x0000000CL
#endif

// Stock control event DISPIDs (from olectl.h)
#ifndef DISPID_CLICK
#include <olectl.h>
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar);
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font);
extern "C" void* MS_ABI impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(void* self);
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(void* self);

//=============================================================================
// Base classes needed by OLE
//=============================================================================
IMPLEMENT_DYNAMIC(CControlBar, CWnd)
IMPLEMENT_DYNAMIC(CDocItem, CObject)

namespace {

struct CCmdUIShim {
    void* vftable;
    UINT m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};

static CMenu* CCmdUI_Menu(const CCmdUI* pThis) {
    auto* ui = reinterpret_cast<const CCmdUIShim*>(pThis);
    if (!ui) return nullptr;
    if (ui->m_pSubMenu != nullptr) {
        return ui->m_pSubMenu;
    }
    return ui->m_pMenu;
}

static bool CCmdUI_IsValidMenu(const CCmdUI* pThis) {
    auto* ui = reinterpret_cast<const CCmdUIShim*>(pThis);
    return ui != nullptr && CCmdUI_Menu(pThis) != nullptr && CCmdUI_Menu(pThis)->m_hMenu != nullptr;
}

static bool CCmdUI_IdValid(UINT id) {
    return id != 0U && id != static_cast<UINT>(-1);
}

static bool CCmdUI_HasByPositionTarget(const CCmdUI* pThis, UINT* itemID, UINT* flags) {
    auto* ui = reinterpret_cast<const CCmdUIShim*>(pThis);
    if (!ui) return false;
    if (ui->m_nIndex > 0) {
        *itemID = static_cast<UINT>(ui->m_nIndex);
        *flags = MF_BYPOSITION;
        return true;
    }
    if (!CCmdUI_IdValid(ui->m_nID)) {
        return false;
    }
    *itemID = ui->m_nID;
    *flags = MF_BYCOMMAND;
    return true;
}

static void CCmdUI_Delegated(CCmdUI* pThis, void (CCmdUI::*fn)(int), int arg) {
    if (pThis && pThis->m_pOther && pThis->m_pOther != pThis) {
        (pThis->m_pOther->*fn)(arg);
    }
}

static void CCmdUI_DelegatedText(CCmdUI* pThis, void (CCmdUI::*fn)(const wchar_t*), const wchar_t* text) {
    if (pThis && pThis->m_pOther && pThis->m_pOther != pThis) {
        (pThis->m_pOther->*fn)(text);
    }
}

static void CCmdUIEnableForMenu(CCmdUI* pThis, BOOL bOn) {
    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_Delegated(pThis, &CCmdUI::Enable, bOn);
        return;
    }

    CMenu* pMenu = CCmdUI_Menu(pThis);
    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;

    UINT state = bOn ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
    if (CCmdUI_IsValidMenu(pThis)) {
        ::EnableMenuItem(pMenu->m_hMenu, itemID, flags | state);
    }
}

static void CCmdUICheckForMenu(CCmdUI* pThis, int nCheck) {
    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_Delegated(pThis, &CCmdUI::SetCheck, nCheck);
        return;
    }

    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }
    CMenu* pMenu = CCmdUI_Menu(pThis);

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;
    ::CheckMenuItem(pMenu->m_hMenu, itemID, flags | (nCheck ? MF_CHECKED : MF_UNCHECKED));
}

static void CCmdUISetTextForMenu(CCmdUI* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    auto* ui = reinterpret_cast<CCmdUIShim*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_DelegatedText(pThis, &CCmdUI::SetText, lpszText);
        return;
    }

    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }
    CMenu* pMenu = CCmdUI_Menu(pThis);

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;

    MENUITEMINFOW mii = {};
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE | MIIM_STRING;
    mii.dwTypeData = const_cast<wchar_t*>(lpszText ? lpszText : L"");
    mii.cch = static_cast<UINT>(lpszText ? (wcslen(lpszText) + 1) : 0);
    ::SetMenuItemInfoW(pMenu->m_hMenu, itemID, flags == MF_BYPOSITION, &mii);
}
} // namespace

void CCmdUI::Enable(BOOL bOn) { CCmdUIEnableForMenu(this, bOn); }
void CCmdUI::SetCheck(int nCheck) { CCmdUICheckForMenu(this, nCheck); }
void CCmdUI::SetText(const wchar_t* lpszText) { CCmdUISetTextForMenu(this, lpszText); }

// Symbol: ?Enable@CCmdUI@@UEAAXH@Z
// Ordinal: 4188
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable) {
    if (pThis) pThis->Enable(enable != 0);
}

// Symbol: ?SetCheck@CCmdUI@@UEAAXH@Z
// Ordinal: 4192
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck) {
    if (pThis) pThis->SetCheck(nCheck);
}

// Symbol: ?SetText@CCmdUI@@UEAAXPEB_W@Z
// Ordinal: 4196
extern "C" void MS_ABI impl__SetText_CCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText) {
    if (pThis) pThis->SetText(lpszText);
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

//=============================================================================
// CControlBar exports (extern "C" MS_ABI impl_ functions)
//=============================================================================

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

CDocItem::CDocItem() : m_pDocument(nullptr) { memset(_docitem_padding, 0, sizeof(_docitem_padding)); }
CDocItem::~CDocItem() {}

// Symbol: ?CreateObject@CDocItem@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDocItem__SAPEAVCObject__XZ() {
    return new CDocItem();
}

// Symbol: ?IsBlank@CDocItem@@UEBAHXZ
extern "C" int MS_ABI impl__IsBlank_CDocItem__UEBAHXZ(const CDocItem* pThis) {
    return pThis == nullptr || pThis->m_pDocument == nullptr;
}

//=============================================================================
// OLE State
//=============================================================================
static int g_bOleInitialized = FALSE;
static int g_nOleLockCount = 0;
static BOOL g_bOleUserCtrl = FALSE;
static COleMessageFilter* g_pMessageFilter = nullptr;
static COleDataSource* g_pClipboardOwner = nullptr;
static std::map<CString, CString> g_oleFactoryLicenseKeys;
static BOOL g_userOleControlMode = TRUE;

namespace {

struct DataCacheEntry {
    FORMATETC format = {};
    STGMEDIUM medium = {};
    bool hasMedium = false;
    bool delayRender = false;
    bool delayRenderFile = false;
};

static std::map<void*, COleControl*> g_reflectorControls;

static FORMATETC MakeFormatEtc(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (lpFormatEtc) {
        FORMATETC fmt = *lpFormatEtc;
        fmt.cfFormat = cfFormat ? cfFormat : fmt.cfFormat;
        return fmt;
    }

    FORMATETC fmt = {};
    fmt.cfFormat = cfFormat;
    fmt.ptd = nullptr;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;
    return fmt;
}

static bool FormatMatches(const FORMATETC& cached, const FORMATETC& requested) {
    if (cached.cfFormat != requested.cfFormat) return false;
    if ((cached.tymed & requested.tymed) == 0) return false;
    if (requested.dwAspect != 0 && cached.dwAspect != requested.dwAspect) return false;
    if (requested.lindex != -1 && cached.lindex != requested.lindex) return false;
    return true;
}

static HGLOBAL DuplicateGlobalMemory(HGLOBAL source) {
    if (!source) return nullptr;

    SIZE_T size = GlobalSize(source);
    HGLOBAL copy = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!copy) return nullptr;

    void* src = GlobalLock(source);
    void* dst = GlobalLock(copy);
    if (!src || !dst) {
        if (src) GlobalUnlock(source);
        if (dst) GlobalUnlock(copy);
        GlobalFree(copy);
        return nullptr;
    }

    memcpy(dst, src, size);
    GlobalUnlock(copy);
    GlobalUnlock(source);
    return copy;
}

static bool CopyStorageMedium(const STGMEDIUM& source, STGMEDIUM* dest) {
    if (!dest) return false;
    memset(dest, 0, sizeof(*dest));
    dest->tymed = source.tymed;
    dest->pUnkForRelease = nullptr;

    switch (source.tymed) {
    case TYMED_HGLOBAL:
        dest->hGlobal = DuplicateGlobalMemory(source.hGlobal);
        return dest->hGlobal != nullptr;
    case TYMED_ISTREAM:
        dest->pstm = source.pstm;
        if (dest->pstm) dest->pstm->AddRef();
        return dest->pstm != nullptr;
    case TYMED_ISTORAGE:
        dest->pstg = source.pstg;
        if (dest->pstg) dest->pstg->AddRef();
        return dest->pstg != nullptr;
    case TYMED_GDI:
        dest->hBitmap = static_cast<HBITMAP>(CopyImage(source.hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
        return dest->hBitmap != nullptr;
    case TYMED_MFPICT:
        dest->hMetaFilePict = source.hMetaFilePict;
        return dest->hMetaFilePict != nullptr;
    case TYMED_ENHMF:
        dest->hEnhMetaFile = CopyEnhMetaFileW(source.hEnhMetaFile, nullptr);
        return dest->hEnhMetaFile != nullptr;
    case TYMED_FILE:
        dest->lpszFileName = source.lpszFileName ? static_cast<LPOLESTR>(CoTaskMemAlloc((wcslen(source.lpszFileName) + 1) * sizeof(OLECHAR))) : nullptr;
        if (source.lpszFileName && !dest->lpszFileName) return false;
        if (source.lpszFileName) wcscpy(dest->lpszFileName, source.lpszFileName);
        return true;
    default:
        dest->tymed = TYMED_NULL;
        return false;
    }
}

static LPOLESTR CopyOleString(const wchar_t* text) {
    if (!text) return nullptr;
    const size_t bytes = (wcslen(text) + 1) * sizeof(OLECHAR);
    LPOLESTR copy = static_cast<LPOLESTR>(CoTaskMemAlloc(bytes));
    if (copy) memcpy(copy, text, bytes);
    return copy;
}

static bool MakeRenderedFileMedium(COleDataSource* source, FORMATETC* format, STGMEDIUM* medium) {
    if (!source || !medium) return false;

    wchar_t tempPath[MAX_PATH] = {};
    wchar_t tempName[MAX_PATH] = {};
    if (!GetTempPathW(MAX_PATH, tempPath)) return false;
    if (!GetTempFileNameW(tempPath, L"omf", 0, tempName)) return false;

    bool ok = false;
    {
        CFile file(tempName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);
        ok = source->OnRenderFileData(format, &file) != 0;
    }

    if (!ok) {
        DeleteFileW(tempName);
        return false;
    }

    memset(medium, 0, sizeof(*medium));
    medium->tymed = TYMED_FILE;
    medium->lpszFileName = CopyOleString(tempName);
    if (!medium->lpszFileName) {
        DeleteFileW(tempName);
        return false;
    }
    return true;
}

class EmptyEnumConnectionPoints : public IEnumConnectionPoints {
public:
    EmptyEnumConnectionPoints() : m_refCount(1) {}
    virtual ~EmptyEnumConnectionPoints() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IEnumConnectionPoints) {
            *ppvObject = static_cast<IEnumConnectionPoints*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE Next(ULONG cConnections, IConnectionPoint** rgpcn, ULONG* pcFetched) override {
        if (pcFetched) *pcFetched = 0;
        if (cConnections == 0) return S_OK;
        if (cConnections > 0 && !rgpcn) return E_POINTER;
        if (cConnections > 1 && !pcFetched) return E_POINTER;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE Skip(ULONG) override { return S_FALSE; }
    HRESULT STDMETHODCALLTYPE Reset() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE Clone(IEnumConnectionPoints** ppEnum) override {
        if (!ppEnum) return E_POINTER;
        *ppEnum = new (std::nothrow) EmptyEnumConnectionPoints();
        return *ppEnum ? S_OK : E_OUTOFMEMORY;
    }

private:
    LONG m_refCount;
};

static void InitializeDocumentView(COleDocObjectItem* item, IOleDocumentView* view) {
    if (!item || !view) return;

    IOleClientSite* clientSite = nullptr;
    if (item->m_lpObject &&
        SUCCEEDED(item->m_lpObject->GetClientSite(&clientSite)) && clientSite) {
        IOleInPlaceSite* inPlaceSite = nullptr;
        if (SUCCEEDED(clientSite->QueryInterface(IID_IOleInPlaceSite,
                                                 reinterpret_cast<void**>(&inPlaceSite))) &&
            inPlaceSite) {
            view->SetInPlaceSite(inPlaceSite);
            inPlaceSite->Release();
        }
        clientSite->Release();
    }

    CRect itemRect;
    item->OnGetItemPosition(itemRect);
    RECT rect = { itemRect.left, itemRect.top, itemRect.right, itemRect.bottom };
    view->SetRect(&rect);
    view->UIActivate(TRUE);
}

static int CountDispatchParams(const BYTE* pbParamInfo) {
    if (!pbParamInfo) return 0;
    int count = 0;
    while (pbParamInfo[count] != 0) ++count;
    return count;
}

static HRESULT MakeDispatchVariant(VARTYPE vt, va_list* args, VARIANTARG* var) {
    if (!var) return E_POINTER;
    VariantInit(var);
    switch (vt) {
    case VT_I2:
        var->vt = VT_I2;
        var->iVal = static_cast<SHORT>(va_arg(*args, int));
        return S_OK;
    case VT_I4:
        var->vt = VT_I4;
        var->lVal = va_arg(*args, long);
        return S_OK;
    case VT_R4:
        var->vt = VT_R4;
        var->fltVal = static_cast<float>(va_arg(*args, double));
        return S_OK;
    case VT_R8:
        var->vt = VT_R8;
        var->dblVal = va_arg(*args, double);
        return S_OK;
    case VT_BOOL:
        var->vt = VT_BOOL;
        var->boolVal = va_arg(*args, int) ? VARIANT_TRUE : VARIANT_FALSE;
        return S_OK;
    case VT_BSTR:
        var->vt = VT_BSTR;
        var->bstrVal = SysAllocString(va_arg(*args, const wchar_t*));
        return var->bstrVal ? S_OK : E_OUTOFMEMORY;
    case VT_DISPATCH:
        var->vt = VT_DISPATCH;
        var->pdispVal = va_arg(*args, LPDISPATCH);
        return S_OK;
    case VT_UNKNOWN:
        var->vt = VT_UNKNOWN;
        var->punkVal = va_arg(*args, LPUNKNOWN);
        return S_OK;
    case VT_VARIANT: {
        VARIANT* src = va_arg(*args, VARIANT*);
        return src ? VariantCopy(var, src) : S_OK;
    }
    case VT_UI1:
        var->vt = VT_UI1;
        var->bVal = static_cast<BYTE>(va_arg(*args, int));
        return S_OK;
    case VT_UI2:
        var->vt = VT_UI2;
        var->uiVal = static_cast<USHORT>(va_arg(*args, int));
        return S_OK;
    case VT_UI4:
        var->vt = VT_UI4;
        var->ulVal = va_arg(*args, unsigned long);
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}

static HRESULT CopyDispatchResult(VARTYPE vt, void* pvRet, VARIANT* result) {
    if (vt == VT_EMPTY || !pvRet) return S_OK;
    switch (vt) {
    case VT_I2:
        *static_cast<short*>(pvRet) = result->iVal;
        return S_OK;
    case VT_I4:
        *static_cast<long*>(pvRet) = result->lVal;
        return S_OK;
    case VT_R4:
        *static_cast<float*>(pvRet) = result->fltVal;
        return S_OK;
    case VT_R8:
        *static_cast<double*>(pvRet) = result->dblVal;
        return S_OK;
    case VT_BOOL:
        *static_cast<BOOL*>(pvRet) = (result->boolVal == VARIANT_TRUE);
        return S_OK;
    case VT_BSTR:
        *static_cast<BSTR*>(pvRet) = result->bstrVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_DISPATCH:
        *static_cast<LPDISPATCH*>(pvRet) = result->pdispVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_UNKNOWN:
        *static_cast<LPUNKNOWN*>(pvRet) = result->punkVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_VARIANT:
        return VariantCopy(static_cast<VARIANT*>(pvRet), result);
    case VT_UI1:
        *static_cast<BYTE*>(pvRet) = result->bVal;
        return S_OK;
    case VT_UI2:
        *static_cast<USHORT*>(pvRet) = result->uiVal;
        return S_OK;
    case VT_UI4:
        *static_cast<ULONG*>(pvRet) = result->ulVal;
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}

static void ResetOleVariant(COleVariant* pThis) {
    if (!pThis) return;
    VARIANT* var = static_cast<VARIANT*>(pThis);
    VariantClear(var);
    VariantInit(var);
}

static void AssignI2Variant(COleVariant* pThis, short value, VARTYPE vt) {
    if (!pThis) return;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_I2 || vt == VT_EMPTY) {
        var->vt = VT_I2;
        var->iVal = value;
        return;
    }
    VARIANT src;
    VariantInit(&src);
    src.vt = VT_I2;
    src.iVal = value;
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
    }
}

static void AssignI4Variant(COleVariant* pThis, long value, VARTYPE vt) {
    if (!pThis) return;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_I4 || vt == VT_EMPTY) {
        var->vt = VT_I4;
        var->lVal = value;
        return;
    }
    VARIANT src;
    VariantInit(&src);
    src.vt = VT_I4;
    src.lVal = value;
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
    }
}

static void SetOleVariantString(COleVariant* pThis, const wchar_t* text, VARTYPE vt) {
    if (!pThis) return;
    const wchar_t* srcText = text ? text : L"";
    if (vt == VT_EMPTY) vt = VT_BSTR;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_BSTR) {
        var->vt = VT_BSTR;
        var->bstrVal = SysAllocString(srcText);
        return;
    }

    VARIANT src;
    VariantInit(&src);
    src.vt = VT_BSTR;
    src.bstrVal = SysAllocString(srcText);
    if (!src.bstrVal && srcText[0] != L'\0') {
        return;
    }
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
        return;
    }
    VariantClear(&src);
}

static const COleVariant* AssignOleVariant(COleVariant* pThis, const VARIANT* pSrc) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    if (pSrc) {
        VariantCopy(static_cast<VARIANT*>(pThis), const_cast<VARIANT*>(pSrc));
    }
    return pThis;
}

static COleClientItem* FindDocumentItemByOleObject(const COleDocument* doc, IOleObject* object) {
    if (!doc || !object) return nullptr;
    POSITION pos = reinterpret_cast<POSITION>(doc->GetStartPosition());
    while (pos) {
        COleClientItem* item = doc->GetNextClientItem(pos);
        if (item && item->GetObject() == object) return item;
    }
    return nullptr;
}

static void CopyVariantByteArray(COleVariant* pThis, CByteArray* bytes) {
    if (!bytes) return;
    bytes->RemoveAll();
    if (!pThis) return;

    VARIANT* var = static_cast<VARIANT*>(pThis);
    if ((var->vt & VT_ARRAY) == 0 || !var->parray) return;
    VARTYPE elementType = VT_EMPTY;
    if (FAILED(SafeArrayGetVartype(var->parray, &elementType))) {
        elementType = static_cast<VARTYPE>(var->vt & VT_TYPEMASK);
    }
    if (elementType != VT_UI1 && elementType != VT_I1) return;

    LONG lower = 0;
    LONG upper = -1;
    if (FAILED(SafeArrayGetLBound(var->parray, 1, &lower)) ||
        FAILED(SafeArrayGetUBound(var->parray, 1, &upper)) ||
        upper < lower) {
        return;
    }

    BYTE* data = nullptr;
    if (FAILED(SafeArrayAccessData(var->parray, reinterpret_cast<void**>(&data))) || !data) return;
    const LONG count = upper - lower + 1;
    bytes->SetSize(count);
    for (LONG i = 0; i < count; ++i) {
        bytes->SetAt(i, data[i]);
    }
    SafeArrayUnaccessData(var->parray);
}

struct OleDispatchDriverLayout {
    LPDISPATCH m_lpDispatch;
    BOOL m_bAutoRelease;
};

static OleDispatchDriverLayout* DispatchDriverLayout(COleDispatchDriver* pThis) {
    return reinterpret_cast<OleDispatchDriverLayout*>(pThis);
}

static const OleDispatchDriverLayout* DispatchDriverLayout(const COleDispatchDriver* pThis) {
    return reinterpret_cast<const OleDispatchDriverLayout*>(pThis);
}

// Symbol: ??0COleVariant@@QEAA@AEBUtagVARIANT@@@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignOleVariant(pThis, pSrc);
    return pThis;
}

// Symbol: ??0COleVariant@@QEAA@AEBV0@@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
) {
    return impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(
        pThis, reinterpret_cast<const VARIANT*>(pSrc)
    );
}

// Symbol: ??0COleVariant@@QEAA@FG@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_FG_Z(
    COleVariant* pThis, short value, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignI2Variant(pThis, value, vt);
    return pThis;
}

// Symbol: ??0COleVariant@@QEAA@JG@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_JG_Z(
    COleVariant* pThis, long value, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignI4Variant(pThis, value, vt);
    return pThis;
}

// Symbol: ??0COleVariant@@QEAA@PEB_WG@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    SetOleVariantString(pThis, text, vt);
    return pThis;
}

// Symbol: ??0COleVariant@@QEAA@PEBUtagVARIANT@@@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(pThis, pSrc);
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBV0@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
) {
    return AssignOleVariant(pThis, reinterpret_cast<const VARIANT*>(pSrc));
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBUtagVARIANT@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return AssignOleVariant(pThis, pSrc);
}

// Symbol: ??4COleVariant@@QEAAAEBV0@PEBUtagVARIANT@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return AssignOleVariant(pThis, pSrc);
}

// Symbol: ??4COleVariant@@QEAAAEBV0@F@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_F_Z(
    COleVariant* pThis, short value
) {
    AssignI2Variant(pThis, value, VT_I2);
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@J@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_J_Z(
    COleVariant* pThis, long value
) {
    AssignI4Variant(pThis, value, VT_I4);
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@M@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_M_Z(
    COleVariant* pThis, float value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_R4;
    pThis->fltVal = value;
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@N@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_N_Z(
    COleVariant* pThis, double value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_R8;
    pThis->dblVal = value;
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@QEB_W@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_QEB_W_Z(
    COleVariant* pThis, const wchar_t* text
) {
    SetOleVariantString(pThis, text, VT_BSTR);
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@_J@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__J_Z(
    COleVariant* pThis, __int64 value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_I8;
    pThis->llVal = value;
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@_K@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__K_Z(
    COleVariant* pThis, unsigned __int64 value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_UI8;
    pThis->ullVal = value;
    return pThis;
}

// Symbol: ?SetString@COleVariant@@QEAAXPEB_WG@Z
extern "C" void MS_ABI impl__SetString_COleVariant__QEAAXPEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
) {
    SetOleVariantString(pThis, text, vt);
}

// Symbol: ?Attach@COleVariant@@QEAAXAEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__Attach_COleVariant__QEAAXAEAUtagVARIANT___Z(
    COleVariant* pThis, VARIANT* pSrc
) {
    if (!pThis || !pSrc) return;
    ResetOleVariant(pThis);
    *static_cast<VARIANT*>(pThis) = *pSrc;
    VariantInit(pSrc);
}

// Symbol: ?Detach@COleVariant@@QEAA?AUtagVARIANT@@XZ
extern "C" VARIANT* MS_ABI impl__Detach_COleVariant__QEAA_AUtagVARIANT__XZ(
    VARIANT* pRet, COleVariant* pThis
) {
    if (!pRet) return nullptr;
    VariantInit(pRet);
    if (!pThis) return pRet;
    *pRet = *static_cast<VARIANT*>(pThis);
    VariantInit(static_cast<VARIANT*>(pThis));
    return pRet;
}

// Symbol: ?ChangeType@COleVariant@@QEAAXGPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__ChangeType_COleVariant__QEAAXGPEAUtagVARIANT___Z(
    COleVariant* pThis, VARTYPE vtNew, VARIANT* pSrc
) {
    if (!pThis) return;
    VARIANT converted;
    VariantInit(&converted);
    VARIANT* source = pSrc ? pSrc : static_cast<VARIANT*>(pThis);
    if (FAILED(VariantChangeType(&converted, source, 0, vtNew))) {
        return;
    }
    ResetOleVariant(pThis);
    *static_cast<VARIANT*>(pThis) = converted;
}

//----------------------------------------------------------------------------
// COleVariant – remaining operators, ctor, archive support
//----------------------------------------------------------------------------

namespace {

// Minimal IStream on top of CArchive for IPersistStream Save/Load.
// Real MFC uses CArchiveStream; this provides the same contract.
struct CArchiveStream : IStream {
    CArchive* m_ar;
    LONG m_ref;

    explicit CArchiveStream(CArchive* ar) : m_ar(ar), m_ref(1) {}

    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
        if (!ppv) return E_POINTER;
        if (riid == IID_IUnknown) { *ppv = static_cast<IUnknown*>(this); AddRef(); return S_OK; }
        *ppv = nullptr; return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&m_ref); }
    STDMETHOD_(ULONG, Release)() {
        LONG r = InterlockedDecrement(&m_ref);
        if (r == 0) { delete this; return 0; }
        return r;
    }
    STDMETHOD(Read)(void* pv, ULONG cb, ULONG* pcbRead) {
        ULONG n = m_ar->Read(pv, cb);
        if (pcbRead) *pcbRead = n;
        return S_OK;
    }
    STDMETHOD(Write)(const void* pv, ULONG cb, ULONG* pcbWritten) {
        m_ar->Write(pv, cb);
        if (pcbWritten) *pcbWritten = cb;
        return S_OK;
    }
    STDMETHOD(Seek)(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        DWORD origin = STREAM_SEEK_SET;
        if (dwOrigin == STREAM_SEEK_SET) {
            origin = CFile::begin;
        } else if (dwOrigin == STREAM_SEEK_CUR) {
            origin = CFile::current;
        } else if (dwOrigin == STREAM_SEEK_END) {
            origin = CFile::end;
        } else {
            return STG_E_INVALIDFUNCTION;
        }
        ULONGLONG pos = m_ar->GetFile()->Seek(dlibMove.QuadPart, origin);
        if (plibNewPosition) plibNewPosition->QuadPart = pos;
        return S_OK;
    }
    STDMETHOD(SetSize)(ULARGE_INTEGER libNewSize) {
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        m_ar->GetFile()->SetLength(libNewSize.QuadPart);
        return S_OK;
    }
    STDMETHOD(CopyTo)(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
        if (!pstm) return STG_E_INVALIDPOINTER;
        BYTE buffer[4096];
        ULONGLONG remaining = cb.QuadPart;
        ULONGLONG totalRead = 0, totalWritten = 0;
        while (remaining > 0) {
            ULONG toRead = static_cast<ULONG>(std::min<ULONGLONG>(remaining, sizeof(buffer)));
            ULONG readNow = 0;
            ULONG wroteNow = 0;
            HRESULT hr = Read(buffer, toRead, &readNow);
            if (FAILED(hr)) return hr;
            if (readNow == 0) break;
            totalRead += readNow;
            hr = pstm->Write(buffer, readNow, &wroteNow);
            if (FAILED(hr)) return hr;
            totalWritten += wroteNow;
            if (wroteNow != readNow) return STG_E_WRITEFAULT;
            remaining -= readNow;
        }
        if (pcbRead) pcbRead->QuadPart = totalRead;
        if (pcbWritten) pcbWritten->QuadPart = totalWritten;
        return S_OK;
    }
    STDMETHOD(Commit)(DWORD) { return S_OK; }
    STDMETHOD(Revert)() { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(LockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(UnlockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(Stat)(STATSTG* pstatstg, DWORD grfStatFlag) {
        if (!pstatstg) return STG_E_INVALIDPOINTER;
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        ZeroMemory(pstatstg, sizeof(*pstatstg));
        pstatstg->type = STGTY_STREAM;
        pstatstg->cbSize.QuadPart = m_ar->GetFile()->GetLength();
        pstatstg->grfMode = 0;
        pstatstg->grfLocksSupported = 0;
        pstatstg->grfStateBits = 0;
        if ((grfStatFlag & STATFLAG_NONAME) == 0) {
            pstatstg->pwcsName = nullptr;
        }
        return S_OK;
    }
    STDMETHOD(Clone)(IStream** ppstm) {
        if (!ppstm) return STG_E_INVALIDPOINTER;
        AddRef();
        *ppstm = this;
        return S_OK;
    }
};

// _AfxCompareSafeArrays — adapted from MFC olevar.cpp.
static BOOL _AfxCompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2)
{
    if (!parray1 || !parray2) return parray1 == parray2;
    DWORD dwDim1 = SafeArrayGetDim(parray1);
    DWORD dwDim2 = SafeArrayGetDim(parray2);
    if (dwDim1 != dwDim2) return FALSE;
    if (dwDim1 == 0) return TRUE;
    DWORD dwSize1 = SafeArrayGetElemsize(parray1);
    DWORD dwSize2 = SafeArrayGetElemsize(parray2);
    if (dwSize1 != dwSize2) return FALSE;

    BOOL bCompare = FALSE;
    long* pLBound1 = new (std::nothrow) long[dwDim1];
    long* pLBound2 = new (std::nothrow) long[dwDim2];
    long* pUBound1 = new (std::nothrow) long[dwDim1];
    long* pUBound2 = new (std::nothrow) long[dwDim2];
    void* pData1 = nullptr;
    void* pData2 = nullptr;

    if (!pLBound1 || !pLBound2 || !pUBound1 || !pUBound2) goto cleanup;

    {
        size_t nTotalElements = 1;
        for (DWORD i = 0; i < dwDim1; ++i) {
            if (FAILED(SafeArrayGetLBound(parray1, i+1, &pLBound1[i])) ||
                FAILED(SafeArrayGetLBound(parray2, i+1, &pLBound2[i])) ||
                FAILED(SafeArrayGetUBound(parray1, i+1, &pUBound1[i])) ||
                FAILED(SafeArrayGetUBound(parray2, i+1, &pUBound2[i])))
                goto cleanup;
            if (pUBound1[i] - pLBound1[i] != pUBound2[i] - pLBound2[i])
                goto cleanup;
            nTotalElements *= (size_t)(pUBound1[i] - pLBound1[i] + 1);
        }
        if (FAILED(SafeArrayAccessData(parray1, &pData1)) || !pData1) goto cleanup;
        if (FAILED(SafeArrayAccessData(parray2, &pData2)) || !pData2) goto cleanup;
        size_t nSize = nTotalElements * dwSize1;
        bCompare = (memcmp(pData1, pData2, nSize) == 0);
        SafeArrayUnaccessData(parray1);
        SafeArrayUnaccessData(parray2);
    }

cleanup:
    if (pData1) SafeArrayUnaccessData(parray1);
    if (pData2) SafeArrayUnaccessData(parray2);
    delete[] pLBound1;
    delete[] pLBound2;
    delete[] pUBound1;
    delete[] pUBound2;
    return bCompare;
}

// _AfxCreateOneDimArray — from MFC olevar.cpp.
static void _AfxCreateOneDimArray(COleVariant* pThis, DWORD dwSize)
{
    VARIANT* varSrc = static_cast<VARIANT*>(pThis);
    if (varSrc->vt != (VT_UI1 | VT_ARRAY) || SafeArrayGetDim(varSrc->parray) != 1) {
        VariantClear(varSrc);
        varSrc->vt = VT_UI1 | VT_ARRAY;
        SAFEARRAYBOUND bound = { dwSize, 0 };
        varSrc->parray = SafeArrayCreate(VT_UI1, 1, &bound);
        if (!varSrc->parray) AfxThrowMemoryException();
    } else {
        long lLower = 0, lUpper = 0;
        SafeArrayGetLBound(varSrc->parray, 1, &lLower);
        SafeArrayGetUBound(varSrc->parray, 1, &lUpper);
        long lSize = lUpper - lLower;
        if (lSize < 0) lSize = 0;
        if ((DWORD)lSize != dwSize) {
            SAFEARRAYBOUND bound = { dwSize, lLower };
            SafeArrayRedim(varSrc->parray, &bound);
        }
    }
}

// _AfxCopyBinaryData — from MFC olevar.cpp.
static void _AfxCopyBinaryData(COleVariant* pThis, const void* pvSrc, DWORD dwSize)
{
    SAFEARRAY* parray = static_cast<VARIANT*>(pThis)->parray;
    if (!parray) return;
    void* pDest = nullptr;
    if (SUCCEEDED(SafeArrayAccessData(parray, &pDest)) && pDest) {
        memcpy(pDest, pvSrc, dwSize);
        SafeArrayUnaccessData(parray);
    }
}

} // anonymous namespace

// Symbol: ??0COleVariant@@QEAA@PEFBU_ITEMIDLIST@@@Z
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEFBU_ITEMIDLIST___Z(
    COleVariant* pThis, const ITEMIDLIST* pidl)
{
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    if (pidl) {
        UINT cbTotal = 0;
        LPCITEMIDLIST pidlWalker = pidl;
        while (pidlWalker->mkid.cb) {
            cbTotal += pidlWalker->mkid.cb;
            pidlWalker = reinterpret_cast<LPCITEMIDLIST>(
                reinterpret_cast<const BYTE*>(pidlWalker) + pidlWalker->mkid.cb);
        }
        cbTotal += sizeof(ITEMIDLIST);
        SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, cbTotal);
        if (psa) {
            memcpy(psa->pvData, pidl, cbTotal);
            static_cast<VARIANT*>(pThis)->vt = VT_ARRAY | VT_UI1;
            static_cast<VARIANT*>(pThis)->parray = psa;
        }
    }
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleVariant* pThis, const CString* strSrc)
{
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    var->vt = VT_BSTR;
    var->bstrVal = SysAllocString(strSrc ? strSrc->GetString() : L"");
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCByteArray@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCByteArray___Z(
    COleVariant* pThis, const CByteArray* arrSrc)
{
    if (!pThis || !arrSrc) return pThis;
    INT_PTR nSize = arrSrc->GetSize();
    if (nSize > LONG_MAX) AfxThrowMemoryException();
    _AfxCreateOneDimArray(pThis, static_cast<DWORD>(nSize));
    _AfxCopyBinaryData(pThis, arrSrc->GetData(), static_cast<DWORD>(nSize));
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCLongBinary@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCLongBinary___Z(
    COleVariant* pThis, const CLongBinary* lbSrc)
{
    if (!pThis || !lbSrc) return pThis;
    if (lbSrc->m_dwDataLength > LONG_MAX) AfxThrowMemoryException();
    _AfxCreateOneDimArray(pThis, lbSrc->m_dwDataLength);
    BYTE* pData = static_cast<BYTE*>(GlobalLock(lbSrc->m_hData));
    if (pData) {
        _AfxCopyBinaryData(pThis, pData, lbSrc->m_dwDataLength);
        GlobalUnlock(lbSrc->m_hData);
    }
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCOleCurrency@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleCurrency___Z(
    COleVariant* pThis, const COleCurrency* curSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_CY) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_CY;
    }
    static_cast<VARIANT*>(pThis)->cyVal = curSrc->m_cur;
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCOleDateTime@ATL@@@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleDateTime_ATL___Z(
    COleVariant* pThis, const COleDateTime* dateSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_DATE) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_DATE;
    }
    static_cast<VARIANT*>(pThis)->date = dateSrc->m_dt;
    return pThis;
}

// Symbol: ??4COleVariant@@QEAAAEBV0@E@Z
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_E_Z(
    COleVariant* pThis, unsigned char nSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_UI1) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_UI1;
    }
    static_cast<VARIANT*>(pThis)->bVal = nSrc;
    return pThis;
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCOleVariant@@@Z
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleVariant___Z(
    CArchive* ar, COleVariant* varSrc)
{
    if (!ar || !varSrc) return ar;
    LPVARIANT pSrc = static_cast<VARIANT*>(varSrc);
    if (pSrc->vt != VT_EMPTY)
        VariantClear(pSrc);
    *ar >> pSrc->vt;
    if (pSrc->vt & VT_BYREF || pSrc->vt & VT_ARRAY)
        return ar;
    switch (pSrc->vt) {
    case VT_BOOL: { WORD v = 0; *ar >> v; V_BOOL(pSrc) = v; return ar; }
    case VT_I1:   *ar >> pSrc->cVal; return ar;
    case VT_UI1:  *ar >> pSrc->bVal; return ar;
    case VT_I2:   *ar >> pSrc->iVal; return ar;
    case VT_UI2:  *ar >> pSrc->uiVal; return ar;
    case VT_I4:   *ar >> pSrc->lVal; return ar;
    case VT_UI4:  *ar >> pSrc->ulVal; return ar;
    case VT_I8:   ar->Read(&pSrc->llVal, sizeof(LONGLONG)); return ar;
    case VT_UI8:  ar->Read(&pSrc->ullVal, sizeof(ULONGLONG)); return ar;
    case VT_CY:   *ar >> pSrc->cyVal.Lo; *ar >> pSrc->cyVal.Hi; return ar;
    case VT_R4:   *ar >> pSrc->fltVal; return ar;
    case VT_R8:   *ar >> pSrc->dblVal; return ar;
    case VT_DATE: *ar >> pSrc->date; return ar;
    case VT_BSTR: {
        DWORD nLen = 0;
        *ar >> nLen;
        if (nLen > 0) {
            pSrc->bstrVal = SysAllocStringByteLen(nullptr, nLen);
            if (!pSrc->bstrVal) AfxThrowMemoryException();
            ar->Read(pSrc->bstrVal, nLen * sizeof(BYTE));
        } else {
            pSrc->bstrVal = nullptr;
        }
        return ar;
    }
    case VT_ERROR: *ar >> pSrc->scode; return ar;
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        CLSID clsid = {};
        *ar >> clsid.Data1;
        *ar >> clsid.Data2;
        *ar >> clsid.Data3;
        ar->Read(&clsid.Data4[0], sizeof(clsid.Data4));
        HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL | CLSCTX_REMOTE_SERVER,
            pSrc->vt == VT_UNKNOWN ? IID_IUnknown : IID_IDispatch,
            reinterpret_cast<void**>(&pSrc->punkVal));
        if (hr == E_INVALIDARG)
            hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL & ~CLSCTX_REMOTE_SERVER,
                pSrc->vt == VT_UNKNOWN ? IID_IUnknown : IID_IDispatch,
                reinterpret_cast<void**>(&pSrc->punkVal));
        if (FAILED(hr)) AfxThrowOleException(hr);
        IPersistStream* pPS = nullptr;
        hr = pSrc->punkVal->QueryInterface(IID_IPersistStream, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr))
            hr = pSrc->punkVal->QueryInterface(IID_IPersistStreamInit, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr)) { pSrc->punkVal->Release(); AfxThrowOleException(hr); }
        CArchiveStream stm(ar);
        hr = pPS->Load(&stm);
        pPS->Release();
        if (FAILED(hr)) { pSrc->punkVal->Release(); AfxThrowOleException(hr); }
        return ar;
    }
    case VT_EMPTY:
    case VT_NULL:
    default:
        return ar;
    }
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCOleVariant@@@Z
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleVariant___Z(
    CArchive* ar, const VARIANT* varSrc)
{
    if (!ar || !varSrc) return ar;
    *ar << varSrc->vt;
    if (varSrc->vt & VT_BYREF || varSrc->vt & VT_ARRAY)
        return ar;
    switch (varSrc->vt) {
    case VT_BOOL: *ar << static_cast<WORD>(V_BOOL(varSrc)); return ar;
    case VT_I1:   *ar << varSrc->cVal; return ar;
    case VT_UI1:  *ar << varSrc->bVal; return ar;
    case VT_I2:   *ar << varSrc->iVal; return ar;
    case VT_UI2:  *ar << varSrc->uiVal; return ar;
    case VT_I4:   *ar << varSrc->lVal; return ar;
    case VT_UI4:  *ar << varSrc->ulVal; return ar;
    case VT_I8:   ar->Write(&varSrc->llVal, sizeof(LONGLONG)); return ar;
    case VT_UI8:  ar->Write(&varSrc->ullVal, sizeof(ULONGLONG)); return ar;
    case VT_CY:   *ar << varSrc->cyVal.Lo; *ar << varSrc->cyVal.Hi; return ar;
    case VT_R4:   *ar << varSrc->fltVal; return ar;
    case VT_R8:   *ar << varSrc->dblVal; return ar;
    case VT_DATE: *ar << varSrc->date; return ar;
    case VT_BSTR: {
        DWORD nLen = SysStringByteLen(varSrc->bstrVal);
        *ar << nLen;
        if (nLen > 0)
            ar->Write(varSrc->bstrVal, nLen * sizeof(BYTE));
        return ar;
    }
    case VT_ERROR: *ar << varSrc->scode; return ar;
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        IPersistStream* pPS = nullptr;
        HRESULT hr = varSrc->punkVal->QueryInterface(IID_IPersistStream, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr))
            hr = varSrc->punkVal->QueryInterface(IID_IPersistStreamInit, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr)) AfxThrowOleException(hr);
        CLSID clsid;
        hr = pPS->GetClassID(&clsid);
        if (FAILED(hr)) { pPS->Release(); AfxThrowOleException(hr); }
        *ar << clsid.Data1;
        *ar << clsid.Data2;
        *ar << clsid.Data3;
        ar->Write(&clsid.Data4[0], sizeof(clsid.Data4));
        CArchiveStream stm(ar);
        hr = pPS->Save(&stm, TRUE);
        pPS->Release();
        if (FAILED(hr)) AfxThrowOleException(hr);
        return ar;
    }
    case VT_EMPTY:
    case VT_NULL:
    default:
        return ar;
    }
}

// Symbol: ??8COleVariant@@QEBAHAEBUtagVARIANT@@@Z
extern "C" int MS_ABI impl___8COleVariant__QEBAHAEBUtagVARIANT___Z(
    const COleVariant* pThis, const VARIANT* var)
{
    if (!pThis || !var) return FALSE;
    if (static_cast<const VARIANT*>(pThis) == var) return TRUE;
    if (var->vt != static_cast<const VARIANT*>(pThis)->vt) return FALSE;
    switch (var->vt) {
    case VT_EMPTY:
    case VT_NULL:
        return TRUE;
    case VT_BOOL:  return V_BOOL(var) == V_BOOL(pThis);
    case VT_I1:    return var->cVal == static_cast<const VARIANT*>(pThis)->cVal;
    case VT_UI1:   return var->bVal == static_cast<const VARIANT*>(pThis)->bVal;
    case VT_I2:    return var->iVal == static_cast<const VARIANT*>(pThis)->iVal;
    case VT_UI2:   return var->uiVal == static_cast<const VARIANT*>(pThis)->uiVal;
    case VT_I4:    return var->lVal == static_cast<const VARIANT*>(pThis)->lVal;
    case VT_UI4:   return var->ulVal == static_cast<const VARIANT*>(pThis)->ulVal;
    case VT_I8:    return var->llVal == static_cast<const VARIANT*>(pThis)->llVal;
    case VT_UI8:   return var->ullVal == static_cast<const VARIANT*>(pThis)->ullVal;
    case VT_CY:
        return var->cyVal.Hi == static_cast<const VARIANT*>(pThis)->cyVal.Hi &&
               var->cyVal.Lo == static_cast<const VARIANT*>(pThis)->cyVal.Lo;
    case VT_R4:    return var->fltVal == static_cast<const VARIANT*>(pThis)->fltVal;
    case VT_R8:    return var->dblVal == static_cast<const VARIANT*>(pThis)->dblVal;
    case VT_DATE:  return var->date == static_cast<const VARIANT*>(pThis)->date;
    case VT_BSTR:
        return SysStringByteLen(var->bstrVal) == SysStringByteLen(static_cast<const VARIANT*>(pThis)->bstrVal) &&
               memcmp(var->bstrVal, static_cast<const VARIANT*>(pThis)->bstrVal,
                      SysStringByteLen(static_cast<const VARIANT*>(pThis)->bstrVal)) == 0;
    case VT_ERROR:
        return var->scode == static_cast<const VARIANT*>(pThis)->scode;
    case VT_DISPATCH:
    case VT_UNKNOWN:
        return var->punkVal == static_cast<const VARIANT*>(pThis)->punkVal;
    default:
        if ((var->vt & VT_ARRAY) && !(var->vt & VT_BYREF))
            return _AfxCompareSafeArrays(var->parray, static_cast<const VARIANT*>(pThis)->parray);
        return FALSE;
    }
}

// Symbol: ??0COleDispatchDriver@@QEAA@XZ
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_XZ(
    COleDispatchDriver* pThis
) {
    if (!pThis) return nullptr;
    new (pThis) COleDispatchDriver();
    return pThis;
}

// Symbol: ??0COleDispatchDriver@@QEAA@PEAUIDispatch@@H@Z
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_PEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
) {
    if (!pThis) return nullptr;
    new (pThis) COleDispatchDriver(lpDispatch, bAutoRelease);
    return pThis;
}

// Symbol: ??0COleDispatchDriver@@QEAA@AEBV0@@Z
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
) {
    if (!pThis) return nullptr;
    if (!pSrc) return impl___0COleDispatchDriver__QEAA_XZ(pThis);
    const OleDispatchDriverLayout* src = DispatchDriverLayout(pSrc);
    new (pThis) COleDispatchDriver(src->m_lpDispatch, src->m_bAutoRelease);
    OleDispatchDriverLayout* dst = DispatchDriverLayout(pThis);
    if (dst->m_lpDispatch && dst->m_bAutoRelease) {
        dst->m_lpDispatch->AddRef();
    }
    return pThis;
}

// Symbol: ??4COleDispatchDriver@@QEAAAEBV0@AEBV0@@Z
extern "C" const COleDispatchDriver* MS_ABI impl___4COleDispatchDriver__QEAAAEBV0_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
) {
    if (!pThis || !pSrc || pThis == pSrc) return pThis;
    const OleDispatchDriverLayout* src = DispatchDriverLayout(pSrc);
    LPDISPATCH srcDispatch = src->m_lpDispatch;
    if (srcDispatch && src->m_bAutoRelease) {
        srcDispatch->AddRef();
    }
    pThis->ReleaseDispatch();
    OleDispatchDriverLayout* dst = DispatchDriverLayout(pThis);
    dst->m_lpDispatch = srcDispatch;
    dst->m_bAutoRelease = src->m_bAutoRelease;
    return pThis;
}

// Symbol: ?AttachDispatch@COleDispatchDriver@@QEAAXPEAUIDispatch@@H@Z
extern "C" void MS_ABI impl__AttachDispatch_COleDispatchDriver__QEAAXPEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
) {
    if (!pThis) return;
    pThis->AttachDispatch(lpDispatch, bAutoRelease);
}

// Symbol: ?DetachDispatch@COleDispatchDriver@@QEAAPEAUIDispatch@@XZ
extern "C" LPDISPATCH MS_ABI impl__DetachDispatch_COleDispatchDriver__QEAAPEAUIDispatch__XZ(
    COleDispatchDriver* pThis
) {
    return pThis ? pThis->DetachDispatch() : nullptr;
}

// Symbol: ?ReleaseDispatch@COleDispatchDriver@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseDispatch_COleDispatchDriver__QEAAXXZ(
    COleDispatchDriver* pThis
) {
    if (!pThis) return;
    pThis->ReleaseDispatch();
}

// Symbol: ?CreateDispatch@COleDispatchDriver@@QEAAHAEBU_GUID@@PEAVCOleException@@@Z
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHAEBU_GUID__PEAVCOleException___Z(
    COleDispatchDriver* pThis, REFCLSID clsid, COleException* pError
) {
    return pThis ? pThis->CreateDispatch(clsid, pError) : FALSE;
}

// Symbol: ?CreateDispatch@COleDispatchDriver@@QEAAHPEB_WPEAVCOleException@@@Z
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHPEB_WPEAVCOleException___Z(
    COleDispatchDriver* pThis, const wchar_t* progId, COleException* pError
) {
    return pThis ? pThis->CreateDispatch(progId, pError) : FALSE;
}

// Symbol: ?InvokeHelper@COleDispatchDriver@@QEAAXJGGPEAXPEBEZZ
extern "C" void MS_ABI impl__InvokeHelper_COleDispatchDriver__QEAAXJGGPEAXPEBEZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, ...
) {
    if (!pThis) return;
    va_list args;
    va_start(args, pbParamInfo);
    pThis->InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
    va_end(args);
}

// Symbol: ?InvokeHelperV@COleDispatchDriver@@QEAAXJGGPEAXPEBEPEAD@Z
extern "C" void MS_ABI impl__InvokeHelperV_COleDispatchDriver__QEAAXJGGPEAXPEBEPEAD_Z(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, va_list args
) {
    if (!pThis) return;
    pThis->InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
}

// Symbol: ?SetProperty@COleDispatchDriver@@QEAAXJGZZ
extern "C" void MS_ABI impl__SetProperty_COleDispatchDriver__QEAAXJGZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, ...
) {
    if (!pThis) return;
    BYTE params[2] = { static_cast<BYTE>(vtProp), 0 };
    va_list args;
    va_start(args, vtProp);
    pThis->InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, params, args);
    va_end(args);
}

// Symbol: ?GetProperty@COleDispatchDriver@@QEBAXJGPEAX@Z
extern "C" void MS_ABI impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(
    const COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, void* pvProp
) {
    if (!pThis) return;
    const_cast<COleDispatchDriver*>(pThis)->InvokeHelper(
        dwDispID, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr
    );
}

// Symbol: ?QueryInterface@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(COleConnPtContainer* pThis);
extern "C" HRESULT MS_ABI impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z(
    COleConnPtContainer* pThis, REFIID riid, void** ppv
) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IConnectionPointContainer)) {
        *ppv = pThis;
        impl__AddRef_COleConnPtContainer__UEAAKXZ(pThis);
        return S_OK;
    }
    return E_NOINTERFACE;
}

// Symbol: ?AddRef@COleConnPtContainer@@UEAAKXZ
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
) {
    (void)pThis;
    return 1;
}

// Symbol: ?Release@COleConnPtContainer@@UEAAKXZ
extern "C" ULONG MS_ABI impl__Release_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
) {
    (void)pThis;
    return 1;
}

// Symbol: ?EnumConnectionPoints@COleConnPtContainer@@UEAAJPEAPEAUIEnumConnectionPoints@@@Z
extern "C" HRESULT MS_ABI impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z(
    COleConnPtContainer* pThis, IEnumConnectionPoints** ppEnum
) {
    (void)pThis;
    if (!ppEnum) return E_POINTER;
    *ppEnum = new EmptyEnumConnectionPoints();
    return *ppEnum ? S_OK : E_OUTOFMEMORY;
}

// Symbol: ?FindConnectionPoint@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAUIConnectionPoint@@@Z
extern "C" HRESULT MS_ABI impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z(
    COleConnPtContainer* pThis, REFIID riid, IConnectionPoint** ppCP
) {
    (void)pThis;
    (void)riid;
    if (!ppCP) return E_POINTER;
    *ppCP = nullptr;
    return OLE_E_NOCONNECTION;
}

struct DataCacheState;

class DataSourceDataObject : public IDataObject {
public:
    explicit DataSourceDataObject(DataCacheState* state) : m_refCount(1), m_state(state) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        return ref;
    }

    HRESULT STDMETHODCALLTYPE GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) override;
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*, STGMEDIUM*) override { return DATA_E_FORMATETC; }
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* pformatetc) override;
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*, FORMATETC* pformatetcOut) override {
        if (!pformatetcOut) return E_POINTER;
        pformatetcOut->ptd = nullptr;
        return DATA_S_SAMEFORMATETC;
    }
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) override;
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) override;
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override { return OLE_E_ADVISENOTSUPPORTED; }

private:
    LONG m_refCount;
    DataCacheState* m_state;
};

struct DataCacheState {
    explicit DataCacheState(COleDataSource* source) : owner(source), dataObject(this) {}
    ~DataCacheState() {
        for (DataCacheEntry& entry : entries) {
            if (entry.hasMedium) ReleaseStgMedium(&entry.medium);
        }
    }

    COleDataSource* owner;
    DataSourceDataObject dataObject;
    std::vector<DataCacheEntry> entries;
};

class DropTargetAdapter : public IDropTarget {
public:
    explicit DropTargetAdapter(COleDropTarget* target) : m_refCount(1), m_target(target) {}
    virtual ~DropTargetAdapter() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropTarget) {
            *ppvObject = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragOver(DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragLeave() override;
    HRESULT STDMETHODCALLTYPE Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;

private:
    CPoint MakePoint(POINTL point) const { return CPoint(static_cast<int>(point.x), static_cast<int>(point.y)); }

    LONG m_refCount;
    COleDropTarget* m_target;
    COleDataObject m_dataObject;
};

struct DropTargetState {
    COleDropTarget* target = nullptr;
    DropTargetAdapter* adapter = nullptr;
};

struct DocumentState {
    COleDocument* document = nullptr;
    std::vector<COleClientItem*> items;
};

struct ClientItemState {
    COleClientItem* item = nullptr;
    LONG activeVerb = OLEIVERB_PRIMARY;
    BOOL modified = FALSE;
    CString hostName;
    CString hostObjectName;
    IUnknown* attachedDataObject = nullptr;
    HGLOBAL iconicMetafile = nullptr;
    HGLOBAL contentMetafile = nullptr;
    ClientItemState() = default;
    ClientItemState(const ClientItemState&) = delete;
    ClientItemState& operator=(const ClientItemState&) = delete;
    ClientItemState(ClientItemState&& other) noexcept
        : item(other.item), activeVerb(other.activeVerb), modified(other.modified),
          hostName(std::move(other.hostName)),
          hostObjectName(std::move(other.hostObjectName)),
          attachedDataObject(other.attachedDataObject),
          iconicMetafile(other.iconicMetafile), contentMetafile(other.contentMetafile) {
        other.item = nullptr;
        other.attachedDataObject = nullptr;
        other.iconicMetafile = nullptr;
        other.contentMetafile = nullptr;
        other.hostName.Empty();
        other.hostObjectName.Empty();
    }
    ClientItemState& operator=(ClientItemState&& other) noexcept {
        if (this != &other) {
            if (attachedDataObject) attachedDataObject->Release();
            if (iconicMetafile) GlobalFree(iconicMetafile);
            if (contentMetafile) GlobalFree(contentMetafile);
            item = other.item;
            activeVerb = other.activeVerb;
            modified = other.modified;
            hostName = std::move(other.hostName);
            hostObjectName = std::move(other.hostObjectName);
            attachedDataObject = other.attachedDataObject;
            iconicMetafile = other.iconicMetafile;
            contentMetafile = other.contentMetafile;
            other.item = nullptr;
            other.attachedDataObject = nullptr;
            other.hostName.Empty();
            other.hostObjectName.Empty();
            other.iconicMetafile = nullptr;
            other.contentMetafile = nullptr;
        }
        return *this;
    }
    ~ClientItemState() {
        if (attachedDataObject) attachedDataObject->Release();
        if (iconicMetafile) GlobalFree(iconicMetafile);
        if (contentMetafile) GlobalFree(contentMetafile);
    }
};

struct ServerDocState {
    COleServerDoc* document = nullptr;
    std::vector<COleServerItem*> items;
    CString hostName;
    CString hostObjectName;
};

struct ServerItemState {
    COleServerItem* item = nullptr;
    COleDataSource* dataSource = nullptr;
    SIZE contentExtent = {0, 0};
    BOOL hasExtent = FALSE;
    ~ServerItemState() { delete dataSource; }
};

struct OleControlEventSink {
    IID iid = IID_NULL;
    DWORD cookie = 0;
    IUnknown* sink = nullptr;
};

struct OleControlDataSourceCacheEntry {
    FORMATETC format = {};
    STGMEDIUM medium = {};
    bool hasMedium = false;
};

struct OleControlState {
    COleControl* control = nullptr;
    // OpenMFC-only associations. Retail COleControl has no member for either:
    // it reaches its container through m_pClientSite, and its m_pControlSite is
    // an IOleControlSite*, not OpenMFC's COleControlSite. Keeping them here
    // leaves the 912-byte retail member layout intact.
    COleControlContainer* container = nullptr;
    COleControlSite* mfcSite = nullptr;
    COLORREF backColor = RGB(255, 255, 255);
    COLORREF foreColor = RGB(0, 0, 0);
    BOOL enabled = TRUE;
    short appearance = 0;
    short borderStyle = 0;
    CString licenseKey;
    CString text;
    long readyState = 4;
    std::vector<OleControlEventSink> eventSinks;
    std::vector<IPropertyNotifySink*> propSinks;
    std::vector<IID> enabledConnectionPoints;
    RECT posRect = {0, 0, 0, 0};
    RECT clipRect = {0, 0, 0, 0};
    BOOL hasObjectRects = FALSE;
    std::vector<OleControlDataSourceCacheEntry> dataSourceEntries;
};

static std::vector<DropTargetState> g_dropTargetStates;
static std::vector<DocumentState> g_documentStates;
static std::vector<ClientItemState> g_clientItemStates;
static std::vector<ServerDocState> g_serverDocStates;
static std::vector<ServerItemState> g_serverItemStates;
static std::vector<OleControlState> g_oleControlStates;
static std::vector<COleObjectFactory*> g_oleObjectFactories;

static CString ClsidKey(REFCLSID clsid) {
    wchar_t clsidText[64] = {};
    if (StringFromGUID2(clsid, clsidText, 64) == 0) return CString();
    return CString(clsidText);
}

static CString FindFactoryLicense(REFCLSID clsid, const CString& fallbackProgId = CString()) {
    if (!fallbackProgId.IsEmpty()) {
        auto it = g_oleFactoryLicenseKeys.find(fallbackProgId);
        if (it != g_oleFactoryLicenseKeys.end()) return it->second;
    }

    CString clsidText = ClsidKey(clsid);
    if (!clsidText.IsEmpty()) {
        auto it = g_oleFactoryLicenseKeys.find(clsidText);
        if (it != g_oleFactoryLicenseKeys.end()) return it->second;
    }
    return CString();
}

static BSTR AllocateLicenseBstr(const CString& value) {
    return value.IsEmpty() ? nullptr : ::SysAllocString(value.GetString());
}

static DropTargetState* GetDropTargetState(COleDropTarget* target, bool create) {
    if (!target) return nullptr;
    for (DropTargetState& state : g_dropTargetStates) {
        if (state.target == target) return &state;
    }
    if (!create) return nullptr;
    g_dropTargetStates.push_back(DropTargetState{target, nullptr});
    return &g_dropTargetStates.back();
}

static void RemoveDropTargetState(COleDropTarget* target) {
    g_dropTargetStates.erase(
        std::remove_if(g_dropTargetStates.begin(), g_dropTargetStates.end(),
                       [target](const DropTargetState& state) { return state.target == target; }),
        g_dropTargetStates.end());
}

static DocumentState* GetDocumentState(COleDocument* document, bool create) {
    if (!document) return nullptr;
    for (DocumentState& state : g_documentStates) {
        if (state.document == document) return &state;
    }
    if (!create) return nullptr;
    g_documentStates.push_back(DocumentState());
    g_documentStates.back().document = document;
    return &g_documentStates.back();
}

static void RemoveDocumentState(COleDocument* document) {
    g_documentStates.erase(
        std::remove_if(g_documentStates.begin(), g_documentStates.end(),
                       [document](const DocumentState& state) { return state.document == document; }),
        g_documentStates.end());
}

class OleItemContainerAdapter : public IOleItemContainer {
public:
    explicit OleItemContainerAdapter(COleDocument* document) : m_refCount(1), m_document(document) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IOleContainer || riid == IID_IOleItemContainer) {
            *ppvObject = static_cast<IOleItemContainer*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE ParseDisplayName(IBindCtx*, LPOLESTR, ULONG* pchEaten, IMoniker** ppmkOut) override {
        if (pchEaten) *pchEaten = 0;
        if (ppmkOut) *ppmkOut = nullptr;
        return E_FAIL;
    }
    HRESULT STDMETHODCALLTYPE EnumObjects(DWORD, IEnumUnknown** ppenum) override {
        if (ppenum) *ppenum = nullptr;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE LockContainer(BOOL) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetObject(LPOLESTR pszItem, DWORD, IBindCtx*, REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        *ppvObject = nullptr;
        COleClientItem* item = m_document ? m_document->OnGetLinkedItem(pszItem) : nullptr;
        if (!item || !item->GetObject()) return MK_E_NOOBJECT;
        return item->GetObject()->QueryInterface(riid, ppvObject);
    }
    HRESULT STDMETHODCALLTYPE GetObjectStorage(LPOLESTR, IBindCtx*, REFIID, void** ppvStorage) override {
        if (!ppvStorage) return E_POINTER;
        *ppvStorage = nullptr;
        return E_FAIL;
    }
    HRESULT STDMETHODCALLTYPE IsRunning(LPOLESTR pszItem) override {
        COleClientItem* item = m_document ? m_document->OnGetLinkedItem(pszItem) : nullptr;
        return (item && item->IsRunning()) ? S_OK : S_FALSE;
    }

private:
    LONG m_refCount;
    COleDocument* m_document;
};

static ClientItemState* GetClientItemState(COleClientItem* item, bool create) {
    if (!item) return nullptr;
    for (ClientItemState& state : g_clientItemStates) {
        if (state.item == item) return &state;
    }
    if (!create) return nullptr;
    g_clientItemStates.push_back(ClientItemState());
    g_clientItemStates.back().item = item;
    return &g_clientItemStates.back();
}

static ClientItemState* FindClientItemState(const COleClientItem* item) {
    for (ClientItemState& state : g_clientItemStates) {
        if (state.item == item) return &state;
    }
    return nullptr;
}

static void RemoveClientItemState(COleClientItem* item) {
    g_clientItemStates.erase(
        std::remove_if(g_clientItemStates.begin(), g_clientItemStates.end(),
                       [item](const ClientItemState& state) { return state.item == item; }),
        g_clientItemStates.end());
}

static void AddOleObjectFactory(COleObjectFactory* factory) {
    if (!factory) return;
    if (std::find(g_oleObjectFactories.begin(), g_oleObjectFactories.end(), factory) == g_oleObjectFactories.end()) {
        g_oleObjectFactories.push_back(factory);
    }
}

static void RemoveOleObjectFactory(COleObjectFactory* factory) {
    g_oleObjectFactories.erase(std::remove(g_oleObjectFactories.begin(), g_oleObjectFactories.end(), factory),
                               g_oleObjectFactories.end());
}

static ServerDocState* GetServerDocState(COleServerDoc* document, bool create) {
    if (!document) return nullptr;
    for (ServerDocState& state : g_serverDocStates) {
        if (state.document == document) return &state;
    }
    if (!create) return nullptr;
    g_serverDocStates.push_back(ServerDocState());
    g_serverDocStates.back().document = document;
    return &g_serverDocStates.back();
}

static void RemoveServerDocState(COleServerDoc* document) {
    g_serverDocStates.erase(
        std::remove_if(g_serverDocStates.begin(), g_serverDocStates.end(),
                       [document](const ServerDocState& state) { return state.document == document; }),
        g_serverDocStates.end());
}

static ServerItemState* GetServerItemState(COleServerItem* item, bool create) {
    if (!item) return nullptr;
    for (ServerItemState& state : g_serverItemStates) {
        if (state.item == item) return &state;
    }
    if (!create) return nullptr;
    g_serverItemStates.push_back(ServerItemState());
    g_serverItemStates.back().item = item;
    return &g_serverItemStates.back();
}

static void RemoveServerItemState(COleServerItem* item) {
    g_serverItemStates.erase(
        std::remove_if(g_serverItemStates.begin(), g_serverItemStates.end(),
                       [item](const ServerItemState& state) { return state.item == item; }),
        g_serverItemStates.end());
}

static BOOL EnsureLinkingDocMoniker(COleLinkingDoc* document, const wchar_t* fileName, BOOL setModified) {
    if (!document) return FALSE;

    const wchar_t* monikerPath = fileName;
    if (!monikerPath || !*monikerPath) {
        monikerPath = document->GetPathName();
    }
    if (!monikerPath || !*monikerPath) return FALSE;

    if (document->m_lpMoniker) {
        document->m_lpMoniker->Release();
        document->m_lpMoniker = nullptr;
    }

    LPMONIKER moniker = nullptr;
    HRESULT hr = CreateFileMoniker(monikerPath, &moniker);
    if (FAILED(hr) || !moniker) {
        document->m_bRegistered = FALSE;
        return FALSE;
    }

    document->m_lpMoniker = moniker;
    document->m_bRegistered = TRUE;
    if (setModified) document->SetModifiedFlag(TRUE);
    return TRUE;
}

static OleControlState* GetOleControlState(COleControl* control, bool create) {
    if (!control) return nullptr;
    for (OleControlState& state : g_oleControlStates) {
        if (state.control == control) return &state;
    }
    if (!create) return nullptr;
    g_oleControlStates.push_back(OleControlState());
    g_oleControlStates.back().control = control;
    return &g_oleControlStates.back();
}

static COleControl* GetControlFromDataSource(const COleControl::CControlDataSource* pSource) {
    if (!pSource) return nullptr;
    for (OleControlState& state : g_oleControlStates) {
        if (state.control && state.control->m_pDataSource == pSource) return state.control;
    }
    return nullptr;
}

static OleControlDataSourceCacheEntry* FindControlDataSourceEntry(OleControlState* state,
                                                                const FORMATETC& format,
                                                                bool create) {
    if (!state) return nullptr;
    for (OleControlDataSourceCacheEntry& entry : state->dataSourceEntries) {
        if (FormatMatches(entry.format, format)) return &entry;
    }
    if (!create) return nullptr;
    state->dataSourceEntries.push_back(OleControlDataSourceCacheEntry{format});
    return &state->dataSourceEntries.back();
}

static OleControlDataSourceCacheEntry* FindControlDataSourceEntry(COleControl* control,
                                                                const FORMATETC& format,
                                                                bool create) {
    OleControlState* state = GetOleControlState(control, create);
    return FindControlDataSourceEntry(state, format, create);
}

static void ReleaseDataSourceEntryMedium(OleControlDataSourceCacheEntry& entry) {
    if (!entry.hasMedium) return;
    ReleaseStgMedium(&entry.medium);
    entry.hasMedium = false;
}

// OpenMFC's COleControlSite for a control, or null. Retail keeps no such
// pointer in the object (its m_pControlSite is an IOleControlSite*), so the
// association lives in the side table rather than in a member.
static COleControlSite* MfcSiteOf(const COleControl* control) {
    OleControlState* state = GetOleControlState(const_cast<COleControl*>(control), false);
    return state ? state->mfcSite : nullptr;
}

template <typename Interface>
static Interface* QueryOleControlInterface(COleControl* control, REFIID iid) {
    if (!control) return nullptr;
    Interface* iface = nullptr;
    OleControlState* state = GetOleControlState(control, false);
    if (state && state->mfcSite && state->mfcSite->m_lpObject &&
        SUCCEEDED(state->mfcSite->m_lpObject->QueryInterface(iid, reinterpret_cast<void**>(&iface)))) {
        return iface;
    }
    if (SUCCEEDED(control->InternalQueryInterface(iid, reinterpret_cast<void**>(&iface)))) {
        return iface;
    }
    return nullptr;
}

static void RemoveOleControlState(COleControl* control) {
    auto it = std::remove_if(g_oleControlStates.begin(), g_oleControlStates.end(),
                             [control](const OleControlState& state) { return state.control == control; });
    for (auto releaseIt = it; releaseIt != g_oleControlStates.end(); ++releaseIt) {
        for (auto& sink : releaseIt->eventSinks) {
            if (sink.sink) sink.sink->Release();
        }
        for (auto* sink : releaseIt->propSinks) {
            if (sink) sink->Release();
        }
        for (OleControlDataSourceCacheEntry& entry : releaseIt->dataSourceEntries) {
            ReleaseDataSourceEntryMedium(entry);
        }
    }
    g_oleControlStates.erase(it, g_oleControlStates.end());
}

static void AddDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, true);
    if (!state || !item) return;
    if (std::find(state->items.begin(), state->items.end(), item) == state->items.end()) {
        state->items.push_back(item);
    }
    item->m_pContainerDoc = document;
    item->m_pDocument = document;
}

static void RemoveDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, false);
    if (!state || !item) return;
    state->items.erase(std::remove(state->items.begin(), state->items.end(), item), state->items.end());
    if (item->m_pContainerDoc == document) item->m_pContainerDoc = nullptr;
    if (item->m_pDocument == document) item->m_pDocument = nullptr;
}

static void AddServerDocItem(COleServerDoc* document, COleServerItem* item) {
    ServerDocState* state = GetServerDocState(document, true);
    if (!state || !item) return;
    if (std::find(state->items.begin(), state->items.end(), item) == state->items.end()) {
        state->items.push_back(item);
    }
    item->m_pServerDoc = document;
    item->m_pDocument = document;
}

static void RemoveServerDocItem(COleServerDoc* document, COleServerItem* item) {
    ServerDocState* state = GetServerDocState(document, false);
    if (!state || !item) return;
    state->items.erase(std::remove(state->items.begin(), state->items.end(), item), state->items.end());
    if (item->m_pServerDoc == document) item->m_pServerDoc = nullptr;
    if (item->m_pDocument == document) item->m_pDocument = nullptr;
}

static size_t ParseLinkedItemIndex(const wchar_t* itemName) {
    if (!itemName || !*itemName) return 0;
    wchar_t* end = nullptr;
    unsigned long value = std::wcstoul(itemName, &end, 10);
    if (!end || *end != L'\0' || value == 0) return 0;
    return static_cast<size_t>(value);
}

static DataCacheState* GetDataCacheState(COleDataSource* source, bool create) {
    if (!source) return nullptr;
    DataCacheState* state = static_cast<DataCacheState*>(source->m_pDataCache);
    if (!state && create) {
        state = new DataCacheState(source);
        source->m_pDataCache = state;
    }
    return state;
}

static DataCacheEntry* FindCacheEntry(DataCacheState* state, const FORMATETC& format) {
    if (!state) return nullptr;
    for (DataCacheEntry& entry : state->entries) {
        if (FormatMatches(entry.format, format)) return &entry;
    }
    return nullptr;
}

HRESULT DataSourceDataObject::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (riid == IID_IUnknown || riid == IID_IDataObject) {
        *ppvObject = static_cast<IDataObject*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

HRESULT DataSourceDataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) {
    if (!pformatetcIn || !pmedium) return E_POINTER;
    DataCacheEntry* entry = FindCacheEntry(m_state, *pformatetcIn);
    if (!entry) return DATA_E_FORMATETC;

    if (!entry->hasMedium) {
        if (entry->delayRender && m_state->owner->OnRenderGlobalData(&entry->format, reinterpret_cast<void**>(&entry->medium.hGlobal))) {
            entry->medium.tymed = TYMED_HGLOBAL;
            entry->medium.pUnkForRelease = nullptr;
            entry->hasMedium = true;
        } else if (entry->delayRenderFile && MakeRenderedFileMedium(m_state->owner, &entry->format, &entry->medium)) {
            entry->hasMedium = true;
        } else {
            return DATA_E_FORMATETC;
        }
    }

    return CopyStorageMedium(entry->medium, pmedium) ? S_OK : STG_E_MEDIUMFULL;
}

HRESULT DataSourceDataObject::QueryGetData(FORMATETC* pformatetc) {
    if (!pformatetc) return E_POINTER;
    return FindCacheEntry(m_state, *pformatetc) ? S_OK : DATA_E_FORMATETC;
}

HRESULT DataSourceDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) {
    if (!pformatetc || !pmedium) return E_POINTER;

    DataCacheEntry* existing = FindCacheEntry(m_state, *pformatetc);
    if (!existing) {
        m_state->entries.push_back(DataCacheEntry());
        existing = &m_state->entries.back();
    } else if (existing->hasMedium) {
        ReleaseStgMedium(&existing->medium);
        existing->hasMedium = false;
    }

    existing->format = *pformatetc;
    existing->delayRender = false;
    existing->delayRenderFile = false;
    if (fRelease) {
        existing->medium = *pmedium;
        memset(pmedium, 0, sizeof(*pmedium));
        existing->hasMedium = true;
        return S_OK;
    }
    existing->hasMedium = CopyStorageMedium(*pmedium, &existing->medium);
    return existing->hasMedium ? S_OK : STG_E_MEDIUMFULL;
}

HRESULT DataSourceDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) {
    if (!ppenumFormatEtc) return E_POINTER;
    *ppenumFormatEtc = nullptr;
    if (dwDirection != DATADIR_GET) return S_FALSE;

    CEnumFormatEtc* enumerator = new CEnumFormatEtc();
    for (const DataCacheEntry& entry : m_state->entries) {
        enumerator->AddFormat(entry.format);
    }
    *ppenumFormatEtc = enumerator;
    return S_OK;
}

class DropSourceAdapter : public IDropSource {
public:
    explicit DropSourceAdapter(COleDropSource* source) : m_refCount(1), m_source(source) {}
    virtual ~DropSourceAdapter() {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropSource) {
            *ppvObject = static_cast<IDropSource*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) override {
        return m_source ? m_source->QueryContinueDrag(fEscapePressed, grfKeyState) : DRAGDROP_S_CANCEL;
    }
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect) override {
        return m_source ? m_source->GiveFeedback(dwEffect) : DRAGDROP_S_USEDEFAULTCURSORS;
    }

private:
    LONG m_refCount;
    COleDropSource* m_source;
};

class OleMessageFilterAdapter : public IMessageFilter {
public:
    explicit OleMessageFilterAdapter(COleMessageFilter* filter) : m_refCount(1), m_filter(filter) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IMessageFilter) {
            *ppvObject = static_cast<IMessageFilter*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    DWORD STDMETHODCALLTYPE HandleInComingCall(DWORD, HTASK, DWORD, LPINTERFACEINFO) override {
        return SERVERCALL_ISHANDLED;
    }
    DWORD STDMETHODCALLTYPE RetryRejectedCall(HTASK, DWORD, DWORD rejectType) override {
        if (!m_filter) return static_cast<DWORD>(-1);
        if (rejectType == SERVERCALL_RETRYLATER) {
            if (m_filter->m_nRetryReply != 0) return m_filter->m_nRetryReply;
            return m_filter->m_bEnableBusy ? 250 : static_cast<DWORD>(-1);
        }
        return static_cast<DWORD>(-1);
    }
    DWORD STDMETHODCALLTYPE MessagePending(HTASK, DWORD, DWORD) override {
        if (!m_filter) return PENDINGMSG_WAITDEFPROCESS;
        MSG msg = {};
        BOOL hasMsg = ::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE);
        return m_filter->OnMessagePending(hasMsg ? &msg : nullptr)
            ? PENDINGMSG_WAITDEFPROCESS
            : PENDINGMSG_CANCELCALL;
    }

private:
    LONG m_refCount;
    COleMessageFilter* m_filter;
};

static thread_local OleMessageFilterAdapter* g_messageFilterAdapter = nullptr;
static thread_local IMessageFilter* g_previousMessageFilter = nullptr;

HRESULT DropTargetAdapter::DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    m_dataObject.Release();
    if (dataObject) m_dataObject.Attach(dataObject, FALSE);
    *effect = m_target->OnDragEnter(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point));
    return S_OK;
}

HRESULT DropTargetAdapter::DragOver(DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = m_target ? m_target->OnDragOver(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point)) : DROPEFFECT_NONE;
    return S_OK;
}

HRESULT DropTargetAdapter::DragLeave() {
    if (m_target) m_target->OnDragLeave(m_target->m_pWnd);
    m_dataObject.Release();
    return S_OK;
}

HRESULT DropTargetAdapter::Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    DROPEFFECT requested = *effect;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    COleDataObject dropData;
    COleDataObject* data = &m_dataObject;
    if (dataObject) {
        dropData.Attach(dataObject, FALSE);
        data = &dropData;
    }

    if (m_target->OnDrop(m_target->m_pWnd, data, requested, MakePoint(point))) {
        *effect = requested;
    }
    m_dataObject.Release();
    (void)keyState;
    return S_OK;
}

} // namespace

//=============================================================================
// OLE Initialization / Termination
//=============================================================================
extern "C" {

MS_ABI void impl__AfxOleInit() {
    if (!g_bOleInitialized) {
        HRESULT hr = OleInitialize(nullptr);
        if (SUCCEEDED(hr)) {
            g_bOleInitialized = TRUE;
            // Create default message filter
            COleMessageFilter* pFilter = new COleMessageFilter();
            g_pMessageFilter = pFilter;
        }
    }
}

MS_ABI void impl__AfxOleTerm(int bExplicit) {
    if (g_bOleInitialized) {
        if (g_pMessageFilter) {
            delete g_pMessageFilter;
            g_pMessageFilter = nullptr;
        }
        if (bExplicit) {
            OleUninitialize();
        }
        g_bOleInitialized = FALSE;
    }
}

MS_ABI void impl__AfxOleLockApp() {
    g_nOleLockCount++;
    CoAddRefServerProcess();
}

MS_ABI void impl__AfxOleUnlockApp() {
    if (g_nOleLockCount > 0) {
        g_nOleLockCount--;
        CoReleaseServerProcess();
    }
}

MS_ABI int impl__AfxOleCanExitApp() {
    return (g_nOleLockCount == 0) ? TRUE : FALSE;
}

MS_ABI int impl__AfxOleGetMessageFilter() {
    return (g_pMessageFilter != nullptr) ? TRUE : FALSE;
}

} // extern "C"

//=============================================================================
// COleCurrency
//=============================================================================
COleCurrency::COleCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}

COleCurrency::COleCurrency(const CY& cySrc) {
    m_cur = cySrc;
    m_status = CY_VALID;
}

COleCurrency::COleCurrency(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
}

COleCurrency& COleCurrency::operator=(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
    return *this;
}

void COleCurrency::SetCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}

//=============================================================================
// COleDateTime
//=============================================================================
COleDateTime COleDateTime::GetCurrentTime() {
    COleDateTime dt;
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToVariantTime(&st, &dt.m_dt);
    dt.m_status = DT_VALID;
    return dt;
}

COleDateTime& COleDateTime::operator=(const COleDateTime& dtSrc) {
    m_dt = dtSrc.m_dt;
    m_status = dtSrc.m_status;
    return *this;
}

//=============================================================================
// COleSafeArray
//=============================================================================
// COleSafeArray : public tagVARIANT — the SAFEARRAY lives in `parray`, not
// inline. These operate on parray and keep vt / the cached dims in sync.
void COleSafeArray::Destroy() {
    if (parray) {
        SafeArrayDestroy(parray);
        parray = nullptr;
    }
    vt = VT_EMPTY;
    m_dwElementSize = 0;
    m_dwDims = 0;
}

void COleSafeArray::Clear() {
    VariantClear(this);   // frees parray and resets vt
    m_dwElementSize = 0;
    m_dwDims = 0;
}

void COleSafeArray::Create(VARTYPE vtype, DWORD cDims, SAFEARRAYBOUND* rgsabound) {
    Destroy();
    SAFEARRAY* psa = SafeArrayCreate(vtype, cDims, rgsabound);
    if (psa) {
        vt = VT_ARRAY | vtype;
        parray = psa;
        m_dwElementSize = psa->cbElements;
        m_dwDims = psa->cDims;
    }
}

void COleSafeArray::AccessData(void** ppvData) {
    SafeArrayAccessData(parray, ppvData);
}

void COleSafeArray::UnaccessData() {
    SafeArrayUnaccessData(parray);
}

void COleSafeArray::Attach(const SAFEARRAY& saSrc) {
    Destroy();
    // Deep-copy rather than alias the caller's descriptor: a const& may refer to
    // a stack/temporary SAFEARRAY, and Destroy() would later SafeArrayDestroy it.
    // (Element vartype isn't carried by a bare SAFEARRAY, so record only VT_ARRAY.)
    SAFEARRAY* psaNew = nullptr;
    if (SUCCEEDED(SafeArrayCopy(const_cast<SAFEARRAY*>(&saSrc), &psaNew)) && psaNew) {
        parray = psaNew;
        vt = VT_ARRAY;
        m_dwElementSize = psaNew->cbElements;
        m_dwDims = psaNew->cDims;
    }
}

SAFEARRAY* COleSafeArray::Detach() {
    SAFEARRAY* psa = parray;
    parray = nullptr;
    vt = VT_EMPTY;
    m_dwElementSize = 0;
    m_dwDims = 0;
    return psa;
}

void COleSafeArray::Copy(const COleSafeArray* psaSrc) {
    Destroy();
    if (psaSrc && psaSrc->parray) {
        SAFEARRAY* psaNew = nullptr;
        if (SUCCEEDED(SafeArrayCopy(psaSrc->parray, &psaNew)) && psaNew) {
            parray = psaNew;
            vt = psaSrc->vt;
            m_dwElementSize = psaNew->cbElements;
            m_dwDims = psaNew->cDims;
        }
    }
}

//=============================================================================
// COleDataObject
//=============================================================================
COleDataObject::COleDataObject()
    : m_lpDataObject(nullptr), m_lpEnumFmtEtc(nullptr), m_bAutoRelease(TRUE) {
    memset(_oledataobject_padding, 0, sizeof(_oledataobject_padding));
}

COleDataObject::~COleDataObject() {
    Release();
}

void COleDataObject::Attach(LPDATAOBJECT lpDataObject, BOOL bAutoRelease) {
    Release();
    m_lpDataObject = lpDataObject;
    m_bAutoRelease = bAutoRelease;
}

LPDATAOBJECT COleDataObject::Detach() {
    LPDATAOBJECT lp = m_lpDataObject;
    m_lpDataObject = nullptr;
    return lp;
}

void COleDataObject::Release() {
    if (m_lpEnumFmtEtc) {
        m_lpEnumFmtEtc->Release();
        m_lpEnumFmtEtc = nullptr;
    }
    if (m_lpDataObject && m_bAutoRelease) {
        m_lpDataObject->Release();
    }
    m_lpDataObject = nullptr;
}

void COleDataObject::BeginEnumFormats() {
    if (m_lpEnumFmtEtc) {
        m_lpEnumFmtEtc->Release();
        m_lpEnumFmtEtc = nullptr;
    }
    if (m_lpDataObject) {
        m_lpDataObject->EnumFormatEtc(DATADIR_GET, &m_lpEnumFmtEtc);
    }
}

BOOL COleDataObject::GetNextFormat(FORMATETC* lpFormatEtc) {
    if (!m_lpEnumFmtEtc) return FALSE;
    ULONG fetched = 0;
    return m_lpEnumFmtEtc->Next(1, lpFormatEtc, &fetched) == S_OK && fetched == 1;
}

BOOL COleDataObject::IsDataAvailable(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return FALSE;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    return m_lpDataObject->QueryGetData(lpFormatEtc) == S_OK;
}

HGLOBAL COleDataObject::GetGlobalData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return nullptr;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    STGMEDIUM stg = {};
    if (m_lpDataObject->GetData(lpFormatEtc, &stg) == S_OK) {
        return stg.hGlobal;
    }
    return nullptr;
}

BOOL COleDataObject::GetData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    if (!m_lpDataObject) return FALSE;
    FORMATETC fmt;
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;
        lpFormatEtc = &fmt;
    }
    return m_lpDataObject->GetData(lpFormatEtc, lpStorageMedium) == S_OK;
}

LPDATAOBJECT COleDataObject::GetIDataObject(BOOL bAddRef) const {
    if (m_lpDataObject && bAddRef) {
        m_lpDataObject->AddRef();
    }
    return m_lpDataObject;
}

BOOL COleDataObject::AttachClipboard() {
    EnsureClipboardObject();
    return m_lpDataObject != nullptr;
}

void COleDataObject::EnsureClipboardObject() {
    if (m_lpDataObject) {
        return;
    }

    LPDATAOBJECT lpDataObject = nullptr;
    if (SUCCEEDED(OleGetClipboard(&lpDataObject)) && lpDataObject) {
        Attach(lpDataObject, TRUE);
    }
}

CFile* COleDataObject::GetFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    EnsureClipboardObject();
    if (!m_lpDataObject) {
        return nullptr;
    }

    FORMATETC fmt = {};
    if (!lpFormatEtc) {
        fmt.cfFormat = cfFormat;
        fmt.ptd = nullptr;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_FILE;
        lpFormatEtc = &fmt;
    }

    STGMEDIUM stg = {};
    if (m_lpDataObject->GetData(lpFormatEtc, &stg) != S_OK) {
        return nullptr;
    }

    if (stg.tymed != TYMED_FILE || !stg.lpszFileName) {
        ReleaseStgMedium(&stg);
        return nullptr;
    }

    CString filePath = stg.lpszFileName;
    ReleaseStgMedium(&stg);

    if (filePath.IsEmpty()) {
        return nullptr;
    }

    auto* result = new CFile(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
    if (result == nullptr || result->m_hFile == INVALID_HANDLE_VALUE) {
        if (result) {
            result->Close();
            delete result;
        }
        return nullptr;
    }
    return result;
}

//=============================================================================
// COleDataSource
//=============================================================================
IMPLEMENT_DYNAMIC(COleDataSource, CCmdTarget)

COleDataSource::COleDataSource()
    : m_bClipboardOwner(FALSE), m_pDataCache(nullptr) {
    memset(_oledatasource_padding, 0, sizeof(_oledatasource_padding));
}

COleDataSource::~COleDataSource() {
    Empty();
}

void COleDataSource::CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal, FORMATETC* lpFormatEtc) {
    STGMEDIUM medium = {};
    medium.tymed = TYMED_HGLOBAL;
    medium.hGlobal = hGlobal;
    medium.pUnkForRelease = nullptr;
    CacheData(cfFormat, &medium, lpFormatEtc);
}

void COleDataSource::CacheData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    if (!lpStorageMedium) return;

    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;

    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }

    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = false;
    entry->hasMedium = CopyStorageMedium(*lpStorageMedium, &entry->medium);
}

void COleDataSource::DelayRenderData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = true;
    entry->delayRenderFile = false;
}

void COleDataSource::DelayRenderFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    format.tymed = TYMED_FILE;
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = true;
}

void COleDataSource::SetClipboard() {
    IDataObject* dataObject = GetInterface(TRUE);
    if (dataObject && SUCCEEDED(OleSetClipboard(dataObject))) {
        if (g_pClipboardOwner && g_pClipboardOwner != this) {
            g_pClipboardOwner->m_bClipboardOwner = FALSE;
        }
        g_pClipboardOwner = this;
        m_bClipboardOwner = TRUE;
    }
    if (dataObject) dataObject->Release();
}

COleDataSource* COleDataSource::GetClipboardOwner() {
    return g_pClipboardOwner;
}

DROPEFFECT COleDataSource::DoDragDrop(DWORD dwEffects, LPCRECT lpRectStartDrag,
                                       COleDropSource* pDropSource) {
    (void)lpRectStartDrag;
    IDataObject* dataObject = GetInterface(TRUE);
    if (!dataObject) return DROPEFFECT_NONE;

    COleDropSource defaultDropSource;
    COleDropSource* source = pDropSource ? pDropSource : &defaultDropSource;
    DropSourceAdapter* adapter = new DropSourceAdapter(source);
    DWORD effect = DROPEFFECT_NONE;
    HRESULT hr = ::DoDragDrop(dataObject, adapter, dwEffects, &effect);
    adapter->Release();
    dataObject->Release();
    return SUCCEEDED(hr) ? effect : DROPEFFECT_NONE;
}

int COleDataSource::OnRenderGlobalData(FORMATETC* lpFormatEtc, void** phGlobal) {
    if (phGlobal) *phGlobal = nullptr;
    if (!lpFormatEtc || !phGlobal) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL) return FALSE;
    *phGlobal = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *phGlobal != nullptr;
}

int COleDataSource::OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile) {
    if (!lpFormatEtc || !pFile) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium) return FALSE;

    if (entry->medium.tymed == TYMED_HGLOBAL && entry->medium.hGlobal) {
        SIZE_T size = GlobalSize(entry->medium.hGlobal);
        void* data = GlobalLock(entry->medium.hGlobal);
        if (!data) return FALSE;
        pFile->Write(data, static_cast<UINT>(size));
        GlobalUnlock(entry->medium.hGlobal);
        return TRUE;
    }
    if (entry->medium.tymed == TYMED_FILE && entry->medium.lpszFileName) {
        CFile source(entry->medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (source.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096];
        UINT read = 0;
        while ((read = source.Read(buffer, sizeof(buffer))) > 0) {
            pFile->Write(buffer, read);
        }
        return TRUE;
    }
    return FALSE;
}

void COleDataSource::Empty() {
    if (g_pClipboardOwner == this) g_pClipboardOwner = nullptr;
    delete static_cast<DataCacheState*>(m_pDataCache);
    m_pDataCache = nullptr;
    m_bClipboardOwner = FALSE;
}

LPDATAOBJECT COleDataSource::GetInterface(BOOL bAddRef) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return nullptr;
    if (bAddRef) state->dataObject.AddRef();
    return &state->dataObject;
}

//=============================================================================
// COleDropTarget
//=============================================================================
IMPLEMENT_DYNAMIC(COleDropTarget, CCmdTarget)

COleDropTarget::COleDropTarget()
    : m_lRefCount(0), m_pWnd(nullptr), m_bRegistered(FALSE) {
    memset(_oledroptarget_padding, 0, sizeof(_oledroptarget_padding));
    m_xDropTarget.m_pDropTarget = this;
}

COleDropTarget::~COleDropTarget() {
    Revoke();
}

BOOL COleDropTarget::Register(CWnd* pWnd) {
    if (m_bRegistered) return TRUE;
    if (!pWnd || !pWnd->GetSafeHwnd()) return FALSE;
    m_pWnd = pWnd;
    DropTargetState* state = GetDropTargetState(this, true);
    if (!state) return FALSE;
    state->adapter = new DropTargetAdapter(this);
    HRESULT hr = RegisterDragDrop(pWnd->GetSafeHwnd(), state->adapter);
    if (SUCCEEDED(hr)) {
        m_bRegistered = TRUE;
        return TRUE;
    }
    state->adapter->Release();
    state->adapter = nullptr;
    RemoveDropTargetState(this);
    m_pWnd = nullptr;
    return FALSE;
}

void COleDropTarget::Revoke() {
    DropTargetState* state = GetDropTargetState(this, false);
    if (m_bRegistered && m_pWnd) {
        RevokeDragDrop(m_pWnd->GetSafeHwnd());
        m_bRegistered = FALSE;
    }
    if (state && state->adapter) {
        state->adapter->Release();
        state->adapter = nullptr;
    }
    RemoveDropTargetState(this);
    m_pWnd = nullptr;
}

DROPEFFECT COleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                        DWORD dwKeyState, CPoint point) {
    (void)pWnd; (void)point;
    if (!pDataObject || !pDataObject->GetIDataObject(FALSE)) return DROPEFFECT_NONE;
    return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

DROPEFFECT COleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                       DWORD dwKeyState, CPoint point) {
    return OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}

void COleDropTarget::OnDragLeave(CWnd* pWnd) {
    (void)pWnd;
}

BOOL COleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                             DROPEFFECT dropEffect, CPoint point) {
    (void)pWnd; (void)point;
    return pDataObject && pDataObject->GetIDataObject(FALSE) && dropEffect != DROPEFFECT_NONE;
}

DROPEFFECT COleDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
                                     DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point) {
    return OnDrop(pWnd, pDataObject, dropDefault, point);
}

DROPEFFECT COleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point) {
    return DROPEFFECT_SCROLL;
}

//=============================================================================
// COleDropTarget::XDropTarget - nested COM IDropTarget
//=============================================================================

STDMETHODIMP COleDropTarget::XDropTarget::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}

STDMETHODIMP COleDropTarget::XDropTarget::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    *pdwEffect = DROPEFFECT_NONE;
    if (!m_pDropTarget) return S_OK;
    COleDataObject data;
    if (pDataObj) data.Attach(pDataObj, FALSE);
    *pdwEffect = m_pDropTarget->OnDragEnter(m_pDropTarget->m_pWnd, &data, grfKeyState,
                                            CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y)));
    return S_OK;
}

STDMETHODIMP COleDropTarget::XDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    *pdwEffect = m_pDropTarget ? m_pDropTarget->OnDragOver(m_pDropTarget->m_pWnd, nullptr,
                  grfKeyState, CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y))) : DROPEFFECT_NONE;
    return S_OK;
}

STDMETHODIMP COleDropTarget::XDropTarget::DragLeave() {
    if (m_pDropTarget) m_pDropTarget->OnDragLeave(m_pDropTarget->m_pWnd);
    return S_OK;
}

STDMETHODIMP COleDropTarget::XDropTarget::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    DROPEFFECT requested = *pdwEffect;
    *pdwEffect = DROPEFFECT_NONE;
    if (!m_pDropTarget) return S_OK;
    COleDataObject data;
    if (pDataObj) data.Attach(pDataObj, FALSE);
    if (m_pDropTarget->OnDrop(m_pDropTarget->m_pWnd, &data, requested,
                              CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y)))) {
        *pdwEffect = requested;
    }
    (void)grfKeyState;
    return S_OK;
}

// Symbol: ?DragEnter@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__DragEnter_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, IDataObject* pDataObj, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->DragEnter(pDataObj, grfKeyState, pt, pdwEffect);
}

// Symbol: ?DragLeave@XDropTarget@COleDropTarget@@UEAAJXZ
extern "C" long MS_ABI impl__DragLeave_XDropTarget_COleDropTarget__UEAAJXZ(
    COleDropTarget::XDropTarget* pThis) {
    return pThis->DragLeave();
}

// Symbol: ?DragOver@XDropTarget@COleDropTarget@@UEAAJKU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__DragOver_XDropTarget_COleDropTarget__UEAAJKU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->DragOver(grfKeyState, pt, pdwEffect);
}

// Symbol: ?Drop@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__Drop_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, IDataObject* pDataObj, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->Drop(pDataObj, grfKeyState, pt, pdwEffect);
}

// Symbol: ?nScrollDelay@COleDropTarget@@1IA
extern "C" unsigned int impl__nScrollDelay_COleDropTarget__1IA = 200;

// Symbol: ?nScrollInset@COleDropTarget@@1HA
extern "C" int impl__nScrollInset_COleDropTarget__1HA = 1;

// Symbol: ?nScrollInterval@COleDropTarget@@1IA
extern "C" unsigned int impl__nScrollInterval_COleDropTarget__1IA = 50;

//=============================================================================
// COleDropSource
//=============================================================================
IMPLEMENT_DYNAMIC(COleDropSource, CCmdTarget)

COleDropSource::COleDropSource()
    : m_lRefCount(0) {
    memset(_oledropsource_padding, 0, sizeof(_oledropsource_padding));
    m_xDropSource.m_pDropSource = this;
}

COleDropSource::~COleDropSource() {
    m_lRefCount = 0;
}

SCODE COleDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState) {
    if (bEscapePressed) return DRAGDROP_S_CANCEL;
    if (!(dwKeyState & (MK_LBUTTON | MK_RBUTTON))) return DRAGDROP_S_DROP;
    return S_OK;
}

SCODE COleDropSource::GiveFeedback(DROPEFFECT dropEffect) {
    (void)dropEffect;
    return DRAGDROP_S_USEDEFAULTCURSORS;
}

BOOL COleDropSource::OnBeginDrag(CWnd* pWnd) {
    // Real MFC default: capture mouse if not already captured
    if (pWnd != nullptr && ::GetCapture() != pWnd->m_hWnd)
        ::SetCapture(pWnd->m_hWnd);
    return TRUE;
}

//=============================================================================
// COleDropSource::XDropSource - nested COM IDropSource
//=============================================================================

STDMETHODIMP COleDropSource::XDropSource::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) COleDropSource::XDropSource::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) COleDropSource::XDropSource::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}

STDMETHODIMP COleDropSource::XDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) {
    return m_pDropSource ? m_pDropSource->QueryContinueDrag(fEscapePressed, grfKeyState) : DRAGDROP_S_CANCEL;
}

STDMETHODIMP COleDropSource::XDropSource::GiveFeedback(DWORD dwEffect) {
    return m_pDropSource ? m_pDropSource->GiveFeedback((DROPEFFECT)dwEffect) : DRAGDROP_S_USEDEFAULTCURSORS;
}

// Symbol: ?GiveFeedback@XDropSource@COleDropSource@@UEAAJK@Z
extern "C" long MS_ABI impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z(
    COleDropSource::XDropSource* pThis, unsigned long p0) {
    return pThis->GiveFeedback(p0);
}

// Symbol: ?QueryContinueDrag@XDropSource@COleDropSource@@UEAAJHK@Z
extern "C" long MS_ABI impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z(
    COleDropSource::XDropSource* pThis, int p0, unsigned long p1) {
    return pThis->QueryContinueDrag(p0, p1);
}

// Symbol: ?OnBeginDrag@COleDropSource@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(
    COleDropSource* pThis, CWnd* p0) {
    return pThis->OnBeginDrag(p0);
}

// Symbol: ?nDragDelay@COleDropSource@@1IA
extern "C" unsigned int impl__nDragDelay_COleDropSource__1IA = 0;

// Symbol: ?nDragMinDist@COleDropSource@@1IA
extern "C" unsigned int impl__nDragMinDist_COleDropSource__1IA = 0;

//=============================================================================
// COleMessageFilter
//=============================================================================
IMPLEMENT_DYNAMIC(COleMessageFilter, CCmdTarget)

COleMessageFilter::COleMessageFilter()
    : m_nBusyCount(0), m_bEnableBusy(TRUE), m_bEnableNotResponding(TRUE),
      m_nBusyReply(SERVERCALL_RETRYLATER), m_nRetryReply(0), m_nTimeout(5000) {
    memset(_olemessagefilter_padding, 0, sizeof(_olemessagefilter_padding));
    m_xMessageFilter.m_pMessageFilter = this;
}

COleMessageFilter::~COleMessageFilter() {
    if (g_pMessageFilter == this) {
        COleMessageFilter::Revoke();
        g_pMessageFilter = nullptr;
    } else {
        Revoke();
    }
}

int COleMessageFilter::Register() {
    if (g_messageFilterAdapter) {
        CoRegisterMessageFilter(g_previousMessageFilter, nullptr);
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
        if (g_previousMessageFilter) {
            g_previousMessageFilter->Release();
            g_previousMessageFilter = nullptr;
        }
    }

    g_messageFilterAdapter = new(std::nothrow) OleMessageFilterAdapter(this);
    if (!g_messageFilterAdapter) return FALSE;
    IMessageFilter* oldFilter = nullptr;
    HRESULT hr = CoRegisterMessageFilter(g_messageFilterAdapter, &oldFilter);
    if (FAILED(hr)) {
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
        return FALSE;
    }
    g_previousMessageFilter = oldFilter;
    return TRUE;
}

void COleMessageFilter::Revoke() {
    IMessageFilter* oldFilter = nullptr;
    CoRegisterMessageFilter(g_previousMessageFilter, &oldFilter);
    if (oldFilter) oldFilter->Release();
    if (g_previousMessageFilter) {
        g_previousMessageFilter->Release();
        g_previousMessageFilter = nullptr;
    }
    if (g_messageFilterAdapter) {
        g_messageFilterAdapter->Release();
        g_messageFilterAdapter = nullptr;
    }
}

BOOL COleMessageFilter::EnableBusyDialog(BOOL bEnableBusy) {
    BOOL bOld = m_bEnableBusy;
    m_bEnableBusy = bEnableBusy;
    return bOld;
}

BOOL COleMessageFilter::EnableNotRespondingDialog(BOOL bEnableNotResponding) {
    BOOL bOld = m_bEnableNotResponding;
    m_bEnableNotResponding = bEnableNotResponding;
    return bOld;
}

void COleMessageFilter::SetBusyReply(SERVERCALL nBusyReply) {
    m_nBusyReply = nBusyReply;
}

void COleMessageFilter::SetRetryReply(DWORD nRetryReply) {
    m_nRetryReply = nRetryReply;
}

void COleMessageFilter::SetMessagePendingDelay(DWORD nTimeout) {
    m_nTimeout = (nTimeout >= 0) ? nTimeout : 5000;
}

void COleMessageFilter::BeginBusyState() {
    m_nBusyCount++;
}

void COleMessageFilter::EndBusyState() {
    if (m_nBusyCount > 0) m_nBusyCount--;
}

BOOL COleMessageFilter::IsBusy() const {
    return m_nBusyCount > 0;
}

BOOL COleMessageFilter::OnMessagePending(const MSG* pMsg) {
    return FALSE;
}

int COleMessageFilter::OnBusyDialog(HTASK hTaskBusy) {
    return -1;
}

BOOL COleMessageFilter::IsSignificantMessage(MSG* pMsg) {
    if (!pMsg) return FALSE;
    return pMsg->message == WM_TIMER || pMsg->message == WM_PAINT ||
           pMsg->message == WM_MOUSEMOVE;
}

int COleMessageFilter::OnNotRespondingDialog(HTASK hTaskBusy) {
    (void)hTaskBusy;
    if (!m_bEnableNotResponding) return 250;
    return m_nRetryReply != 0 ? (int)m_nRetryReply : 250;
}

//=============================================================================
// COleMessageFilter::XMessageFilter - nested COM IMessageFilter
//=============================================================================

STDMETHODIMP COleMessageFilter::XMessageFilter::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IMessageFilter)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::HandleInComingCall(
    DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo) {
    (void)dwCallType; (void)htaskCaller; (void)dwTickCount; (void)lpInterfaceInfo;
    return SERVERCALL_ISHANDLED;
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::RetryRejectedCall(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType) {
    (void)htaskCallee; (void)dwTickCount;
    if (!m_pMessageFilter || !m_pMessageFilter->m_bEnableBusy) return (ULONG)-1;
    if (m_pMessageFilter->m_nRetryReply != 0) return m_pMessageFilter->m_nRetryReply;
    return dwRejectType == SERVERCALL_RETRYLATER ? 250 : (ULONG)-1;
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::MessagePending(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwType) {
    (void)htaskCallee; (void)dwTickCount; (void)dwType;
    MSG msg;
    if (m_pMessageFilter && ::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (m_pMessageFilter->IsSignificantMessage(&msg)) return PENDINGMSG_WAITDEFPROCESS;
        if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) return PENDINGMSG_CANCELCALL;
        ::DispatchMessageW(&msg);
    }
    return PENDINGMSG_WAITDEFPROCESS;
}

// Symbol: ?HandleInComingCall@XMessageFilter@COleMessageFilter@@UEAAKKPEAUHTASK__@@KPEAUtagINTERFACEINFO@@@Z
// Ordinal: 7564
extern "C" unsigned long MS_ABI impl__HandleInComingCall_XMessageFilter_COleMessageFilter__UEAAKKPEAUHTASK____KPEAUtagINTERFACEINFO___Z(
    COleMessageFilter::XMessageFilter* pThis, unsigned long dwCallType,
    HTASK__* htaskCaller, unsigned long dwTickCount,
    tagINTERFACEINFO* lpInterfaceInfo) {
    return pThis->HandleInComingCall(dwCallType, htaskCaller, dwTickCount, lpInterfaceInfo);
}

// Symbol: ?IsSignificantMessage@COleMessageFilter@@UEAAHPEAUtagMSG@@@Z
// Ordinal: 8006
extern "C" int MS_ABI impl__IsSignificantMessage_COleMessageFilter__UEAAHPEAUtagMSG___Z(
    COleMessageFilter* pThis, tagMSG* pMsg) {
    return (int)pThis->IsSignificantMessage(pMsg);
}

// Symbol: ?MessagePending@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
// Ordinal: 8450
extern "C" unsigned long MS_ABI impl__MessagePending_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    COleMessageFilter::XMessageFilter* pThis, HTASK__* htaskCallee,
    unsigned long dwTickCount, unsigned long dwType) {
    return pThis->MessagePending(htaskCallee, dwTickCount, dwType);
}

// Symbol: ?OnNotRespondingDialog@COleMessageFilter@@UEAAHPEAUHTASK__@@@Z
// Ordinal: 10692
extern "C" int MS_ABI impl__OnNotRespondingDialog_COleMessageFilter__UEAAHPEAUHTASK_____Z(
    COleMessageFilter* pThis, HTASK__* hTaskBusy) {
    return (int)pThis->OnNotRespondingDialog(hTaskBusy);
}

// Symbol: ?RetryRejectedCall@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
// Ordinal: 12562
extern "C" unsigned long MS_ABI impl__RetryRejectedCall_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    COleMessageFilter::XMessageFilter* pThis, HTASK__* htaskCallee,
    unsigned long dwTickCount, unsigned long dwRejectType) {
    return pThis->RetryRejectedCall(htaskCallee, dwTickCount, dwRejectType);
}

//=============================================================================
// COleCmdUI
//=============================================================================
COleCmdUI::COleCmdUI(OLECMD* rgCmds, ULONG cCmds, const GUID* pGuid)
    : m_pCmd(nullptr), m_cCmds(cCmds), m_pGuid(pGuid), m_rgCmds(rgCmds) {
    memset(_olecmdui_padding, 0, sizeof(_olecmdui_padding));
}

COleCmdUI::~COleCmdUI() {
    m_pCmd = nullptr;
    m_cCmds = 0;
    m_pGuid = nullptr;
    m_rgCmds = nullptr;
}

void COleCmdUI::Enable(BOOL bOn) {
    if (m_rgCmds && m_cCmds > 0) {
        m_rgCmds[0].cmdf = bOn ? OLECMDF_ENABLED : OLECMDF_SUPPORTED;
    }
}

void COleCmdUI::SetCheck(int nCheck) {
    if (m_rgCmds && m_cCmds > 0) {
        if (nCheck == 1)
            m_rgCmds[0].cmdf |= OLECMDF_LATCHED;
        else
            m_rgCmds[0].cmdf &= ~OLECMDF_LATCHED;
    }
}

void COleCmdUI::SetText(const wchar_t* lpszText) {
    (void)lpszText;
}

int COleCmdUI::DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHandler) {
    // Minimal fallback implementation: expose command as supported/enabled
    // without invoking full MFC command-routing.
    (void)pTarget;
    if (!m_rgCmds || m_cCmds == 0) {
        return FALSE;
    }

    m_rgCmds[0].cmdf |= OLECMDF_SUPPORTED;
    if (bDisableIfNoHandler) {
        Enable(FALSE);
    }
    return TRUE;
}

//=============================================================================
// COleFrameHook
//=============================================================================
IMPLEMENT_DYNAMIC(COleFrameHook, CCmdTarget)

COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
    : m_pFrameWnd(pFrameWnd), m_pActiveItem(pItem), m_lpActiveUIWindow(nullptr) {
    memset(_oleframehook_padding, 0, sizeof(_oleframehook_padding));
}

COleFrameHook::~COleFrameHook() {
    m_pFrameWnd = nullptr;
    m_pActiveItem = nullptr;
    m_lpActiveUIWindow = nullptr;
}

BOOL COleFrameHook::OnDocActivate(BOOL bActivate) {
    return TRUE;
}

BOOL COleFrameHook::OnContextHelp() {
    return FALSE;
}

BOOL COleFrameHook::OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow) {
    return FALSE;
}

BOOL COleFrameHook::OnGetClipboardData(COleDataSource* pDataSource) {
    return FALSE;
}

//=============================================================================
// COleResizeBar
//=============================================================================
IMPLEMENT_DYNAMIC(COleResizeBar, CControlBar)

COleResizeBar::COleResizeBar()
    : m_pClientItem(nullptr), m_nHandleSize(4) {
    memset(_oleresizebar_padding, 0, sizeof(_oleresizebar_padding));
}

COleResizeBar::~COleResizeBar() {
    if (m_hWnd) {
        DestroyWindow();
    }
    m_pClientItem = nullptr;
}

BOOL COleResizeBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    (void)dwStyle;
    if (!pParentWnd) return FALSE;
    m_hWnd = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
                           0, 0, 0, 0, pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                           AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

//=============================================================================
// COleStreamFile
//=============================================================================
// Symbol: ??0COleStreamFile@@QEAA@PEAUIStream@@@Z
extern "C" COleStreamFile* MS_ABI impl___0COleStreamFile__QEAA_PEAUIStream___Z(COleStreamFile* pThis, LPSTREAM lpStream) {
    if (!pThis) return nullptr;
    return new(pThis) COleStreamFile(lpStream);
}

COleStreamFile::COleStreamFile(LPSTREAM lpStream)
    : CFile(), m_lpStream(lpStream) {
    memset(_olestreamfile_padding, 0, sizeof(_olestreamfile_padding));
}

COleStreamFile::~COleStreamFile() {
    if (m_lpStream) {
        m_lpStream->Release();
        m_lpStream = nullptr;
    }
}

LPSTREAM COleStreamFile::GetStream() const {
    return m_lpStream;
}

BOOL COleStreamFile::OpenStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags) {
    if (!lpStorage || !lpszStreamName) return FALSE;
    if (m_lpStream) m_lpStream->Release();
    HRESULT hr = lpStorage->OpenStream(lpszStreamName, nullptr, nOpenFlags, 0, &m_lpStream);
    return SUCCEEDED(hr);
}

BOOL COleStreamFile::CreateStream(LPSTORAGE lpStorage, const wchar_t* lpszStreamName, DWORD nOpenFlags) {
    if (!lpStorage || !lpszStreamName) return FALSE;
    if (m_lpStream) m_lpStream->Release();
    HRESULT hr = lpStorage->CreateStream(lpszStreamName, nOpenFlags | STGM_CREATE, 0, 0, &m_lpStream);
    return SUCCEEDED(hr);
}

void COleStreamFile::Attach(LPSTREAM lpStream) {
    if (m_lpStream) m_lpStream->Release();
    m_lpStream = lpStream;
}

LPSTREAM COleStreamFile::Detach() {
    LPSTREAM lp = m_lpStream;
    m_lpStream = nullptr;
    return lp;
}

UINT COleStreamFile::Read(void* lpBuf, UINT nCount) {
    if (!m_lpStream) return 0;
    ULONG cbRead = 0;
    HRESULT hr = m_lpStream->Read(lpBuf, nCount, &cbRead);
    if (FAILED(hr)) return 0;
    return cbRead;
}

void COleStreamFile::Write(const void* lpBuf, UINT nCount) {
    if (!m_lpStream) return;
    ULONG cbWritten = 0;
    m_lpStream->Write(lpBuf, nCount, &cbWritten);
}

ULONGLONG COleStreamFile::Seek(LONGLONG lOff, UINT nFrom) {
    if (!m_lpStream) return (ULONGLONG)-1;
    LARGE_INTEGER li;
    li.QuadPart = lOff;
    ULARGE_INTEGER uli;
    HRESULT hr = m_lpStream->Seek(li, nFrom, &uli);
    if (FAILED(hr)) return (ULONGLONG)-1;
    return uli.QuadPart;
}

void COleStreamFile::LockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    if (!m_lpStream) return;
    ULARGE_INTEGER uliPos;
    ULARGE_INTEGER uliCount;
    uliPos.QuadPart = dwPos;
    uliCount.QuadPart = dwCount;
    m_lpStream->LockRegion(uliPos, uliCount, LOCK_EXCLUSIVE);
}

void COleStreamFile::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    if (!m_lpStream) return;
    ULARGE_INTEGER uliPos;
    ULARGE_INTEGER uliCount;
    uliPos.QuadPart = dwPos;
    uliCount.QuadPart = dwCount;
    m_lpStream->UnlockRegion(uliPos, uliCount, LOCK_EXCLUSIVE);
}

//=============================================================================
// COlePropertyPage
//=============================================================================
IMPLEMENT_DYNAMIC(COlePropertyPage, CDialog)

COlePropertyPage::COlePropertyPage()
    : CDialog(), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}

COlePropertyPage::COlePropertyPage(UINT nIDTemplate, UINT nIDCaption)
    : CDialog(nIDTemplate), m_pPageSite(nullptr), m_bModified(FALSE),
      m_pszPageName(nullptr), m_pszHelpFile(nullptr),
      m_dwHelpContext(0), m_pszDocString(nullptr) {
    memset(_olepropertypage_padding, 0, sizeof(_olepropertypage_padding));
}

COlePropertyPage::~COlePropertyPage() {
}

void COlePropertyPage::SetPageName(const wchar_t* pszName) {
    m_pszPageName = (wchar_t*)pszName;
}

void COlePropertyPage::SetHelpInfo(const wchar_t* lpszDocString, const wchar_t* lpszHelpFile, DWORD dwHelpContext) {
    m_pszDocString = (wchar_t*)lpszDocString;
    m_pszHelpFile = (wchar_t*)lpszHelpFile;
    m_dwHelpContext = dwHelpContext;
}

BOOL COlePropertyPage::OnSetPageSite() {
    return TRUE;
}

void COlePropertyPage::OnObjectsChanged() {
    SetModifiedFlag(TRUE);
}

BOOL COlePropertyPage::OnApply() {
    return TRUE;
}

BOOL COlePropertyPage::OnEditProperty(DISPID dispid) {
    (void)dispid;
    return FALSE;
}

void COlePropertyPage::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified ? TRUE : FALSE;
    if (m_pPageSite) {
        m_pPageSite->OnStatusChange(bModified ? PROPPAGESTATUS_DIRTY : 0);
    }
}

//=============================================================================
// COleDocument
//=============================================================================
IMPLEMENT_DYNAMIC(COleDocument, CDocument)

COleDocument::COleDocument()
    : CDocument(), m_bCompoundFile(FALSE), m_bRemember(TRUE) {
    memset(_coledocument_padding, 0, sizeof(_coledocument_padding));
}

COleDocument::~COleDocument() {
    DocumentState* state = GetDocumentState(this, false);
    if (state) {
        for (COleClientItem* item : state->items) {
            if (item && item->m_pContainerDoc == this) item->m_pContainerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    RemoveDocumentState(this);
}

void COleDocument::AddItem(COleClientItem* pItem) {
    if (pItem) AddDocumentItem(this, pItem);
}

void COleDocument::RemoveItem(COleClientItem* pItem) {
    RemoveDocumentItem(this, pItem);
}

COleClientItem* COleDocument::GetStartPosition() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state || state->items.empty()) return nullptr;
    return reinterpret_cast<COleClientItem*>(static_cast<uintptr_t>(1));
}

COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    uintptr_t index = reinterpret_cast<uintptr_t>(pos);
    if (!state || index == 0 || index > state->items.size()) {
        pos = nullptr;
        return nullptr;
    }
    COleClientItem* item = state->items[index - 1];
    pos = (index < state->items.size())
        ? reinterpret_cast<POSITION>(index + 1)
        : nullptr;
    return item;
}

COleClientItem* COleDocument::GetPrimarySelectedItem(CView* pView) const {
    (void)pView;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}

int COleDocument::GetItemCount() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    return state ? static_cast<int>(state->items.size()) : 0;
}

void COleDocument::EnableCompoundFile(BOOL bEnable) {
    m_bCompoundFile = bEnable;
}

BOOL COleDocument::HasBlankItems() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->GetItemState() == OLE_EMPTY) return TRUE;
    }
    return FALSE;
}

BOOL COleDocument::IsInPlaceActive() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->IsInPlaceActive()) return TRUE;
    }
    return FALSE;
}

void COleDocument::OnShowViews(BOOL bVisible) {
    POSITION pos = GetFirstViewPosition();
    while (pos) {
        CWnd* pView = static_cast<CWnd*>(GetNextView(pos));
        if (pView && pView->GetSafeHwnd()) {
            ::ShowWindow(pView->GetSafeHwnd(), bVisible ? SW_SHOW : SW_HIDE);
        }
    }
}

COleClientItem* COleDocument::OnGetLinkedItem(const wchar_t* lpszItemName) {
    DocumentState* state = GetDocumentState(this, false);
    if (!state || state->items.empty()) return nullptr;
    if (!lpszItemName || !*lpszItemName) return state->items.front();

    size_t index = ParseLinkedItemIndex(lpszItemName);
    if (index != 0 && index <= state->items.size()) {
        return state->items[index - 1];
    }

    for (COleClientItem* item : state->items) {
        if (!item) continue;
        CLSID clsid = CLSID_NULL;
        item->GetClassID(&clsid);
        LPOLEOBJECT object = item->GetObject();
        if (object) {
            LPOLESTR userType = nullptr;
            if (SUCCEEDED(object->GetUserType(USERCLASSTYPE_FULL, &userType)) && userType) {
                bool match = wcscmp(userType, lpszItemName) == 0;
                CoTaskMemFree(userType);
                if (match) return item;
            }
        }
    }
    return state->items.size() == 1 ? state->items.front() : nullptr;
}

COleClientItem* COleDocument::OnGetEmbeddedItem() {
    return GetPrimarySelectedItem(nullptr);
}

void COleDocument::OnEditChangeIcon(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    COleChangeIconDialog dlg(target);
    if (dlg.DoChangeIcon(target) == TRUE) {
        SetModifiedFlag(TRUE);
    }
}

void COleDocument::OnEditConvert(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    CLSID oldClass = {};
    target->GetClassID(&oldClass);
    COleConvertDialog dlg(target);
    if (dlg.DoModal() != IDOK) return;

    OLEUICONVERTW* cv = dlg.GetOleUIConvert();
    if (!cv) return;

    CLSID newClass = cv->clsidNew;
    if (IsEqualGUID(newClass, CLSID_NULL) || IsEqualGUID(oldClass, newClass)) {
        return;
    }

    if (dlg.IsConvertTo()) {
        if (target->ConvertTo(newClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    } else {
        if (target->ActivateAs(newClass, oldClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    }
}

void COleDocument::OnEditLinks() {
    COleLinksDialog dlg(this);
    if (dlg.DoModal() == IDOK) {
        SetModifiedFlag(TRUE);
    }
}

LPOLEITEMCONTAINER COleDocument::GetItemContainer() {
    return new(std::nothrow) OleItemContainerAdapter(this);
}

//=============================================================================
// COleLinkingDoc
//=============================================================================
IMPLEMENT_DYNAMIC(COleLinkingDoc, COleDocument)

COleLinkingDoc::COleLinkingDoc()
    : COleDocument(), m_bRegistered(FALSE), m_lpMoniker(nullptr) {
    memset(_colelinkingdoc_padding, 0, sizeof(_colelinkingdoc_padding));
}

COleLinkingDoc::~COleLinkingDoc() {
    Revoke();
}

LPMONIKER COleLinkingDoc::GetMoniker(OLEGETMONIKER nAssign) {
    if (!m_lpMoniker && nAssign != OLEGETMONIKER_ONLYIFTHERE) {
        EnsureLinkingDocMoniker(this, nullptr, FALSE);
    }
    return m_lpMoniker;
}

LPMONIKER COleLinkingDoc::GetFileMoniker() {
    return m_lpMoniker;
}

BOOL COleLinkingDoc::RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified) {
    return EnsureLinkingDocMoniker(this, lpszFileName, bSetModified);
}

void COleLinkingDoc::Revoke() {
    if (m_lpMoniker) {
        m_lpMoniker->Release();
        m_lpMoniker = nullptr;
    }
    m_bRegistered = FALSE;
}

BOOL COleLinkingDoc::IsRegistered() const {
    return m_bRegistered;
}

void COleLinkingDoc::OnShowDocument(BOOL bShow) {
    if (!bShow) Revoke();
}

//=============================================================================
// COleServerDoc
//=============================================================================
IMPLEMENT_DYNAMIC(COleServerDoc, COleLinkingDoc)

COleServerDoc::COleServerDoc()
    : COleLinkingDoc(), m_bEmbedded(FALSE) {
    memset(_coleserverdoc_padding, 0, sizeof(_coleserverdoc_padding));
    GetServerDocState(this, true);
}

COleServerDoc::~COleServerDoc() {
    ServerDocState* state = GetServerDocState(this, false);
    if (state) {
        for (COleServerItem* item : state->items) {
            if (item && item->m_pServerDoc == this) item->m_pServerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    RemoveServerDocState(this);
}

void COleServerDoc::NotifyChanged() {
    SetModifiedFlag(TRUE);
    UpdateAllViews(nullptr, 0, nullptr);
}

void COleServerDoc::NotifyClosed() {
    if (m_bRegistered || m_lpMoniker) Revoke();
}

void COleServerDoc::NotifyRename(const wchar_t* lpszNewName) {
    const wchar_t* newName = lpszNewName ? lpszNewName : L"";
    SetTitle(newName);
    if (*newName) {
        SetPathName(newName, FALSE);
        RegisterIfServerAttached(newName, FALSE);
    }
}

void COleServerDoc::NotifySaved() {
    SetModifiedFlag(FALSE);
}

void COleServerDoc::SaveEmbedding() {
    const wchar_t* path = GetPathName();
    if (path && *path) {
        OnSaveDocument(path);
    } else {
        SetModifiedFlag(FALSE);
    }
}

COleClientItem* COleServerDoc::GetEmbeddedItem() {
    return OnGetEmbeddedItem();
}

COleServerItem* COleServerDoc::GetEmbeddedServerItem() {
    ServerDocState* state = GetServerDocState(this, false);
    if (!state || state->items.empty()) return nullptr;
    return state->items.front();
}

COleServerItem* COleServerDoc::GetLinkedServerItem(const wchar_t* lpszItemName) {
    ServerDocState* state = GetServerDocState(this, false);
    if (!state || state->items.empty()) return nullptr;
    if (!lpszItemName || !*lpszItemName) return state->items.front();
    size_t index = ParseLinkedItemIndex(lpszItemName);
    if (index != 0 && index <= state->items.size()) return state->items[index - 1];
    for (COleServerItem* item : state->items) {
        if (!item) continue;
        COleServerDoc* itemDoc = item->GetDocument();
        const wchar_t* title = itemDoc ? itemDoc->GetTitle() : nullptr;
        if (title && wcscmp(title, lpszItemName) == 0) return item;
    }
    return state->items.size() == 1 ? state->items.front() : nullptr;
}

BOOL COleServerDoc::OnSetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) {
    ServerDocState* state = GetServerDocState(this, true);
    if (state) {
        state->hostName = lpszHost ? lpszHost : L"";
        state->hostObjectName = lpszHostObj ? lpszHostObj : L"";
    }
    return TRUE;
}

//=============================================================================
// COleClientItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleClientItem, CDocItem)

COleClientItem::COleClientItem(COleDocument* pContainerDoc)
    : m_pContainerDoc(pContainerDoc), m_lpObject(nullptr), m_lpLink(nullptr),
      m_lpInPlaceObject(nullptr), m_nStatus(OLE_EMPTY), m_nDrawAspect(DVASPECT_CONTENT),
      m_pControlSite(nullptr), m_lpFrame(nullptr), m_lpDocFrame(nullptr),
      m_bInPlaceActive(FALSE) {
    memset(_oleclientitem_padding, 0, sizeof(_oleclientitem_padding));
    m_pDocument = pContainerDoc;
    GetClientItemState(this, true);
    if (pContainerDoc) AddDocumentItem(pContainerDoc, this);
}

COleClientItem::~COleClientItem() {
    if (m_pContainerDoc) RemoveDocumentItem(m_pContainerDoc, this);
    Close(OLECLOSE_NOSAVE);
    RemoveClientItemState(this);
}

BOOL COleClientItem::CreateFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;
    COleDataObject wrapper;
    wrapper.Attach(dataObject, TRUE);
    return CreateFromData(&wrapper, render, cfFormat, lpFormatEtc);
}

BOOL COleClientItem::CreateNewItem(REFCLSID clsid, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)render; (void)cfFormat; (void)lpFormatEtc;
    Close(OLECLOSE_NOSAVE);
    HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL, IID_IOleObject,
                                  reinterpret_cast<void**>(&m_lpObject));
    if (SUCCEEDED(hr) && m_lpObject) {
        OleSetContainedObject(m_lpObject, TRUE);
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

BOOL COleClientItem::CreateStaticFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return CreateFromClipboard(render, cfFormat, lpFormatEtc);
}

BOOL COleClientItem::CreateFromData(COleDataObject* pDataObject, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!pDataObject) return FALSE;
    IDataObject* dataObject = pDataObject->GetIDataObject(TRUE);
    if (!dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    HRESULT hr = OleCreateFromData(dataObject, IID_IOleObject, render,
                                   cfFormat || lpFormatEtc ? &format : nullptr,
                                   nullptr, nullptr,
                                   reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

BOOL COleClientItem::CreateLinkFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    hr = OleCreateLinkFromData(dataObject, IID_IOleObject, render,
                               cfFormat || lpFormatEtc ? &format : nullptr,
                               nullptr, nullptr,
                               reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&m_lpLink));
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}

void COleClientItem::Activate(LONG nVerb, CView* pView, HWND hwndParent, LPCRECT lpRect, LPCRECT lpClipRect, BOOL bSplit) {
    (void)lpClipRect; (void)bSplit;
    if (!m_lpObject) return;
    HWND parent = hwndParent;
    if (!parent && pView) parent = pView->GetSafeHwnd();
    RECT rect = {};
    LPCRECT rectToUse = lpRect;
    if (!rectToUse) {
        CRect itemRect;
        OnGetItemPosition(itemRect);
        rect.left = itemRect.left;
        rect.top = itemRect.top;
        rect.right = itemRect.right;
        rect.bottom = itemRect.bottom;
        rectToUse = &rect;
    }
    HRESULT hr = m_lpObject->DoVerb(nVerb, nullptr, nullptr, 0, parent, rectToUse);
    if (SUCCEEDED(hr)) {
        ClientItemState* state = GetClientItemState(this, true);
        if (state) state->activeVerb = nVerb;
        if (m_lpInPlaceObject) {
            m_lpInPlaceObject->Release();
            m_lpInPlaceObject = nullptr;
        }
        if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
            m_bInPlaceActive = TRUE;
        }
        m_nStatus = OLE_RUNNING;
        OnActivate();
    }
}

void COleClientItem::Deactivate() {
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}

int COleClientItem::DoVerb(LONG nVerb, CView* pView, MSG* lpMsg) {
    if (!m_lpObject) return -1;
    HWND parent = pView ? pView->GetSafeHwnd() : nullptr;
    CRect rect;
    OnGetItemPosition(rect);
    HRESULT hr = m_lpObject->DoVerb(nVerb, lpMsg, nullptr, 0, parent, rect);
    if (FAILED(hr)) return static_cast<int>(hr);
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_RUNNING;
    return 0;
}

BOOL COleClientItem::IsInPlaceActive() const {
    return m_bInPlaceActive;
}

OLE_STATUS COleClientItem::GetItemState() const {
    return m_nStatus;
}

void COleClientItem::SetItemState(OLE_STATUS nNewState) {
    m_nStatus = nNewState;
}

void COleClientItem::Close(OLE_CLOSE dwCloseOption) {
    if (m_lpObject) {
        m_lpObject->Close(dwCloseOption);
        m_lpObject->Release();
        m_lpObject = nullptr;
    }
    if (m_lpLink) {
        m_lpLink->Release();
        m_lpLink = nullptr;
    }
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
    m_bInPlaceActive = FALSE;
    m_nStatus = OLE_EMPTY;
}

BOOL COleClientItem::Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect) {
    if (!m_lpObject || !pDC || !pDC->GetSafeHdc() || !lpBounds) return FALSE;
    return SUCCEEDED(OleDraw(m_lpObject, nDrawAspect, pDC->GetSafeHdc(), lpBounds));
}

BOOL COleClientItem::GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect) {
    if (!m_lpObject || !lpSize) return FALSE;
    SIZEL sizel = {};
    HRESULT hr = m_lpObject->GetExtent(nDrawAspect, &sizel);
    if (FAILED(hr)) return FALSE;
    lpSize->cx = sizel.cx;
    lpSize->cy = sizel.cy;
    return TRUE;
}

BOOL COleClientItem::SetExtent(const CSize& size, DVASPECT nDrawAspect) {
    if (!m_lpObject) return FALSE;
    SIZEL sizel = { size.cx, size.cy };
    return SUCCEEDED(m_lpObject->SetExtent(nDrawAspect, &sizel));
}

void COleClientItem::GetClassID(CLSID* pClassID) {
    if (!pClassID) return;
    *pClassID = CLSID_NULL;
    if (m_lpObject) m_lpObject->GetUserClassID(pClassID);
}

void COleClientItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    if (!m_lpObject) return;
    IDataObject* dataObject = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&dataObject)))) {
        OleSetClipboard(dataObject);
        dataObject->Release();
    }
}

void COleClientItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam) {
    (void)dwParam;
    if (nCode == OLE_CHANGED) {
        SetModifiedFlag(TRUE);
        if (m_pContainerDoc) {
            m_pContainerDoc->SetModifiedFlag(TRUE);
            m_pContainerDoc->UpdateAllViews(nullptr, 0, nullptr);
        }
    }
}

void COleClientItem::OnActivate() {
    m_bInPlaceActive = TRUE;
    if (m_nStatus == OLE_EMPTY && m_lpObject) {
        m_nStatus = OLE_LOADED;
    }
    if (m_nStatus == OLE_LOADED || m_nStatus == OLE_OPEN) {
        m_nStatus = OLE_RUNNING;
    }
    if (m_pControlSite) {
        m_pControlSite->Activate(TRUE);
    }
}

void COleClientItem::OnDeactivate() {
    m_bInPlaceActive = FALSE;
    if (m_nStatus == OLE_RUNNING) {
        m_nStatus = OLE_OPEN;
    }
    if (m_pControlSite) {
        m_pControlSite->Activate(FALSE);
    }
}

void COleClientItem::OnGetItemPosition(CRect& rPosition) {
    if (m_pControlSite) {
        CWnd* siteWindow = m_pControlSite->GetWindow();
        HWND hwnd = siteWindow ? siteWindow->GetSafeHwnd() : nullptr;
        if (hwnd) {
            RECT rc = {};
            if (::GetWindowRect(hwnd, &rc)) {
                HWND parent = ::GetParent(hwnd);
                if (parent) {
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.left));
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.right));
                }
                rPosition = rc;
                return;
            }
        }
    }

    if (m_lpInPlaceObject) {
        HWND hwnd = nullptr;
        if (SUCCEEDED(m_lpInPlaceObject->GetWindow(&hwnd)) && hwnd) {
            RECT rc = {};
            if (::GetWindowRect(hwnd, &rc)) {
                HWND parent = ::GetParent(hwnd);
                if (parent) {
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.left));
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.right));
                }
                rPosition = rc;
                return;
            }
        }
    }

    if (m_pContainerDoc) {
        POSITION pos = m_pContainerDoc->GetFirstViewPosition();
        if (pos) {
            CWnd* pView = static_cast<CWnd*>(m_pContainerDoc->GetNextView(pos));
            if (pView && pView->GetSafeHwnd()) {
                RECT rc = {};
                if (::GetClientRect(pView->GetSafeHwnd(), &rc)) {
                    rPosition = rc;
                    return;
                }
            }
        }
    }

    rPosition = CRect(0, 0, 100, 100);
}

BOOL COleClientItem::OnChangeItemPosition(const CRect& rectPos) {
    if (!m_lpInPlaceObject) return FALSE;
    RECT rect = { rectPos.left, rectPos.top, rectPos.right, rectPos.bottom };
    return SUCCEEDED(m_lpInPlaceObject->SetObjectRects(&rect, &rect));
}

void COleClientItem::OnDiscardUndoState() {
    SetModifiedFlag(FALSE);
    if (m_nStatus == OLE_CHANGED) {
        m_nStatus = OLE_OPEN;
    }
    ClientItemState* state = FindClientItemState(this);
    if (state) {
        if (state->iconicMetafile) {
            GlobalFree(state->iconicMetafile);
            state->iconicMetafile = nullptr;
        }
        if (state->contentMetafile) {
            GlobalFree(state->contentMetafile);
            state->contentMetafile = nullptr;
        }
    }
}

void COleClientItem::SetControlSite(COleControlSite* pSite) {
    m_pControlSite = pSite;
}

COleControlSite* COleClientItem::GetControlSite() const {
    return m_pControlSite;
}

//=============================================================================
// COleServerItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleServerItem, CDocItem)

COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
    : m_pServerDoc(pServerDoc), m_bAutoDelete(bAutoDelete) {
    memset(_oleserveritem_padding, 0, sizeof(_oleserveritem_padding));
    m_pDocument = pServerDoc;
    GetServerItemState(this, true);
    if (pServerDoc) AddServerDocItem(pServerDoc, this);
}

COleServerItem::~COleServerItem() {
    if (m_pServerDoc) RemoveServerDocItem(m_pServerDoc, this);
    RemoveServerItemState(this);
}

BOOL COleServerItem::OnDraw(CDC* pDC, CSize& rSize) {
    return OnDrawEx(pDC, DVASPECT_CONTENT, rSize);
}

BOOL COleServerItem::OnDrawEx(CDC* pDC, DVASPECT nDrawAspect, CSize& rSize) {
    (void)pDC; // Lifecycle-only fallback: use stored extent when no rendering backend is available.
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    return OnGetExtent(nDrawAspect, rSize);
}

BOOL COleServerItem::OnGetExtent(DVASPECT nDrawAspect, CSize& rSize) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, false);
    if (!state || !state->hasExtent) {
        rSize.cx = 0;
        rSize.cy = 0;
        return TRUE;
    }
    rSize.cx = state->contentExtent.cx;
    rSize.cy = state->contentExtent.cy;
    return TRUE;
}

BOOL COleServerItem::OnSetExtent(DVASPECT nDrawAspect, const CSize& size) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return FALSE;
    state->contentExtent.cx = size.cx;
    state->contentExtent.cy = size.cy;
    state->hasExtent = TRUE;
    return TRUE;
}

void COleServerItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
}

COleDataSource* COleServerItem::GetDataSource() {
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return nullptr;
    if (!state->dataSource) state->dataSource = new COleDataSource();
    return state->dataSource;
}

void COleServerItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    COleDataSource* dataSource = GetDataSource();
    if (dataSource) dataSource->SetClipboard();
}

void COleServerItem::NotifyChanged() {
    if (m_pServerDoc) m_pServerDoc->NotifyChanged();
}

BOOL COleServerItem::IsConnected() const {
    return m_pServerDoc && m_pServerDoc->IsRegistered();
}

COleServerDoc* COleServerItem::GetDocument() const {
    return m_pServerDoc;
}

//=============================================================================
// COleObjectFactory
//=============================================================================
IMPLEMENT_DYNAMIC(COleObjectFactory, CCmdTarget)

COleObjectFactory::COleObjectFactory()
    : m_pRuntimeClass(nullptr), m_bMultiInstance(FALSE), m_strProgID(L""),
      m_dwRegister(0), m_pTemplate(nullptr),
      m_xClassFactory(this) {
    memset(&m_clsid, 0, sizeof(m_clsid));
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
    AddOleObjectFactory(this);
}

COleObjectFactory::COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                      BOOL bMultiInstance, const wchar_t* lpszProgID)
    : m_clsid(clsid), m_pRuntimeClass(pRuntimeClass),
      m_bMultiInstance(bMultiInstance), m_strProgID(lpszProgID ? lpszProgID : L""),
      m_dwRegister(0), m_pTemplate(nullptr),
      m_xClassFactory(this) {
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
    AddOleObjectFactory(this);
}

COleObjectFactory::COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                      BOOL bMultiInstance, BOOL bFreeOnRelease,
                                      const wchar_t* lpszProgID)
    : COleObjectFactory() {
    CommonConstruct(clsid, pRuntimeClass, bMultiInstance, bFreeOnRelease, lpszProgID);
}

COleObjectFactory::~COleObjectFactory() {
    Revoke();
    RemoveOleObjectFactory(this);
}

// Export wrappers (placement-new the C++ ctors above; matches .def aliases).
// Symbol: ??0COleObjectFactory@@QEAA@AEBU_GUID@@PEAUCRuntimeClass@@HPEB_W@Z
// Ordinal: 850
extern "C" COleObjectFactory* MS_ABI impl___0COleObjectFactory__QEAA_AEBU_GUID__PEAUCRuntimeClass__HPEB_W_Z(
    COleObjectFactory* pThis, const GUID* pClsid, CRuntimeClass* pRuntimeClass,
    int bMultiInstance, const wchar_t* lpszProgID) {
    return new(pThis) COleObjectFactory(*pClsid, pRuntimeClass, bMultiInstance, lpszProgID);
}

// Symbol: ??0COleObjectFactory@@QEAA@AEBU_GUID@@PEAUCRuntimeClass@@HHPEB_W@Z
// Ordinal: 849
extern "C" COleObjectFactory* MS_ABI impl___0COleObjectFactory__QEAA_AEBU_GUID__PEAUCRuntimeClass__HHPEB_W_Z(
    COleObjectFactory* pThis, const GUID* pClsid, CRuntimeClass* pRuntimeClass,
    int bMultiInstance, int bFreeOnRelease, const wchar_t* lpszProgID) {
    return new(pThis) COleObjectFactory(*pClsid, pRuntimeClass, bMultiInstance,
                                         bFreeOnRelease, lpszProgID);
}

void COleObjectFactory::CommonConstruct(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                         BOOL bMultiInstance, BOOL bFreeOnRelease,
                                         const wchar_t* lpszProgID) {
    (void)bFreeOnRelease;
    m_clsid = clsid;
    m_pRuntimeClass = pRuntimeClass;
    m_bMultiInstance = bMultiInstance;
    m_strProgID = lpszProgID ? lpszProgID : L"";
    m_dwRegister = 0;
    m_pTemplate = nullptr;
    AddOleObjectFactory(this);
}

BOOL COleObjectFactory::Register() {
    AfxOleInit();
    HRESULT hr = CoRegisterClassObject(m_clsid, &m_xClassFactory,
                                        CLSCTX_LOCAL_SERVER,
                                        REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED,
                                        &m_dwRegister);
    return SUCCEEDED(hr);
}

void COleObjectFactory::Revoke() {
    if (m_dwRegister) {
        CoRevokeClassObject(m_dwRegister);
        m_dwRegister = 0;
    }
}

BOOL COleObjectFactory::IsRegistered() const {
    return m_dwRegister != 0;
}

BOOL COleObjectFactory::IsLicenseValid() {
    return VerifyUserLicense();
}

int COleObjectFactory::RegisterAll() {
    int count = 0;
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (factory && factory->Register()) ++count;
    }
    return count;
}

BOOL COleObjectFactory::UpdateRegistryAll(BOOL bRegister) {
    BOOL ok = TRUE;
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (!factory) continue;
        factory->UpdateRegistry(bRegister);
    }
    return ok;
}

void COleObjectFactory::RevokeAll() {
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (factory) factory->Revoke();
    }
}

BOOL COleObjectFactory::Unregister() {
    Revoke();
    return TRUE;
}

BOOL COleObjectFactory::UnregisterAll() {
    RevokeAll();
    return TRUE;
}

CCmdTarget* COleObjectFactory::OnCreateObject() {
    if (!m_pRuntimeClass) {
        return nullptr;
    }
    CObject* pObject = m_pRuntimeClass->CreateObject();
    return pObject ? static_cast<CCmdTarget*>(pObject) : nullptr;
}

BOOL COleObjectFactory::OnVerifyFile(LPCTSTR lpszFileName) {
    return lpszFileName && *lpszFileName;
}

BOOL COleObjectFactory::UpdateRegistry(BOOL bRegister) {
    if (m_strProgID.IsEmpty()) return FALSE;

    wchar_t clsidText[64] = {};
    if (StringFromGUID2(m_clsid, clsidText, 64) == 0) return FALSE;

    CString clsidKey = CString(L"CLSID\\") + clsidText;
    if (!bRegister) {
        LONG progResult = RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(m_strProgID));
        LONG clsidResult = RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey));
        return (progResult == ERROR_SUCCESS || progResult == ERROR_FILE_NOT_FOUND) &&
               (clsidResult == ERROR_SUCCESS || clsidResult == ERROR_FILE_NOT_FOUND);
    }

    BOOL success = TRUE;
    HKEY hKey = nullptr;
    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(m_strProgID), 0, nullptr,
                        REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        HKEY hClsid = nullptr;
        if (RegCreateKeyExW(hKey, L"CLSID", 0, nullptr, REG_OPTION_NON_VOLATILE,
                            KEY_SET_VALUE, nullptr, &hClsid, nullptr) == ERROR_SUCCESS) {
            RegSetValueExW(hClsid, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(clsidText),
                           static_cast<DWORD>((wcslen(clsidText) + 1) * sizeof(wchar_t)));
            RegCloseKey(hClsid);
        } else {
            success = FALSE;
        }
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey), 0, nullptr,
                        REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        const wchar_t* progID = static_cast<const wchar_t*>(m_strProgID);
        RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                       reinterpret_cast<const BYTE*>(progID),
                       static_cast<DWORD>((wcslen(progID) + 1) * sizeof(wchar_t)));
        HKEY hProgID = nullptr;
        if (RegCreateKeyExW(hKey, L"ProgID", 0, nullptr, REG_OPTION_NON_VOLATILE,
                            KEY_SET_VALUE, nullptr, &hProgID, nullptr) == ERROR_SUCCESS) {
            RegSetValueExW(hProgID, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(progID),
                           static_cast<DWORD>((wcslen(progID) + 1) * sizeof(wchar_t)));
            RegCloseKey(hProgID);
        }
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }
    return success;
}

void COleObjectFactory::ConnectTemplate(COleTemplateServer* pTemplate) {
    m_pTemplate = pTemplate;
}

BOOL COleObjectFactory::VerifyUserLicense() {
    return TRUE;
}

BOOL COleObjectFactory::VerifyLicenseKey(BSTR bstrKey) {
    (void)bstrKey;
    return TRUE;
}

BOOL COleObjectFactory::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved;
    if (pbstrKey) {
        *pbstrKey = nullptr;
    }
    return FALSE;
}

//=============================================================================
// COleObjectFactory::XClassFactory - nested COM IClassFactory2
//=============================================================================

COleObjectFactory::XClassFactory::XClassFactory(COleObjectFactory* pOuter)
    : m_pOuter(pOuter), m_refCount(0), m_lockCount(0) {
}

STDMETHODIMP COleObjectFactory::XClassFactory::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory) ||
        IsEqualIID(riid, IID_IClassFactory2)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) {
        // In real MFC, factory objects are typically global and never deleted.
        // We reset the ref count to prevent re-deletion.
        m_refCount = 1;
    }
    return ref;
}

STDMETHODIMP COleObjectFactory::XClassFactory::CreateInstance(
    IUnknown* pUnkOuter, REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;
    if (!m_pOuter) return E_UNEXPECTED;

    CCmdTarget* pObject = m_pOuter->OnCreateObject();
    if (!pObject) return E_OUTOFMEMORY;

    // CCmdTarget-derived objects in this implementation do not yet expose
    // COM interfaces through their interface map. Return E_NOINTERFACE.
    // Real MFC would walk the object's GetInterfaceMap() to find the
    // matching COM identity (e.g. nested X* class) and QI from it.
    (void)riid;
    delete pObject;
    return E_NOINTERFACE;
}

STDMETHODIMP COleObjectFactory::XClassFactory::LockServer(BOOL fLock) {
    if (fLock)
        InterlockedIncrement(&m_lockCount);
    else
        InterlockedDecrement(&m_lockCount);
    return S_OK;
}

STDMETHODIMP COleObjectFactory::XClassFactory::GetLicInfo(LICINFO* pLicInfo) {
    if (!pLicInfo) return E_POINTER;
    memset(pLicInfo, 0, sizeof(*pLicInfo));
    if (!m_pOuter) return E_UNEXPECTED;
    if (m_pOuter->VerifyUserLicense()) {
        pLicInfo->fLicVerified = TRUE;
        pLicInfo->fRuntimeKeyAvail = m_pOuter->GetLicenseKey(0, nullptr) ? TRUE : FALSE;
    }
    return S_OK;
}

STDMETHODIMP COleObjectFactory::XClassFactory::RequestLicKey(
    DWORD dwReserved, BSTR* pbstrKey) {
    if (!pbstrKey) return E_POINTER;
    if (!m_pOuter) return E_UNEXPECTED;
    return m_pOuter->GetLicenseKey(dwReserved, pbstrKey) ? S_OK : E_FAIL;
}

STDMETHODIMP COleObjectFactory::XClassFactory::CreateInstanceLic(
    IUnknown* pUnkOuter, IUnknown* pUnkReserved, REFIID riid,
    BSTR bstrKey, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;
    if (!m_pOuter) return E_UNEXPECTED;
    (void)pUnkReserved;

    // Verify license key
    if (!m_pOuter->VerifyLicenseKey(bstrKey))
        return CLASS_E_NOTLICENSED;

    return CreateInstance(nullptr, riid, ppv);
}

// Export wrappers for XClassFactory methods.

// Symbol: ?CreateInstance@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@AEBU_GUID@@PEAPEAX@Z
// Ordinal: 3312
extern "C" HRESULT MS_ABI impl__CreateInstance_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__AEBU_GUID__PEAPEAX_Z(
    COleObjectFactory::XClassFactory* pThis, IUnknown* pUnkOuter,
    const GUID* priid, void** ppv) {
    return pThis->CreateInstance(pUnkOuter, *priid, ppv);
}

// Symbol: ?CreateInstanceLic@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@0AEBU_GUID@@PEA_WPEAPEAX@Z
// Ordinal: 3313
extern "C" HRESULT MS_ABI impl__CreateInstanceLic_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__0AEBU_GUID__PEA_WPEAPEAX_Z(
    COleObjectFactory::XClassFactory* pThis, IUnknown* pUnkOuter,
    IUnknown* pUnkReserved, const GUID* priid, wchar_t* bstrKey, void** ppv) {
    return pThis->CreateInstanceLic(pUnkOuter, pUnkReserved, *priid, bstrKey, ppv);
}

// Symbol: ?GetLicInfo@XClassFactory@COleObjectFactory@@UEAAJPEAUtagLICINFO@@@Z
// Ordinal: 5685
extern "C" HRESULT MS_ABI impl__GetLicInfo_XClassFactory_COleObjectFactory__UEAAJPEAUtagLICINFO___Z(
    COleObjectFactory::XClassFactory* pThis, LICINFO* pLicInfo) {
    return pThis->GetLicInfo(pLicInfo);
}

// Symbol: ?LockServer@XClassFactory@COleObjectFactory@@UEAAJH@Z
// Ordinal: 8199
extern "C" HRESULT MS_ABI impl__LockServer_XClassFactory_COleObjectFactory__UEAAJH_Z(
    COleObjectFactory::XClassFactory* pThis, int fLock) {
    return pThis->LockServer(fLock);
}

// Symbol: ?RequestLicKey@XClassFactory@COleObjectFactory@@UEAAJKPEAPEA_W@Z
// Ordinal: 12488
extern "C" HRESULT MS_ABI impl__RequestLicKey_XClassFactory_COleObjectFactory__UEAAJKPEAPEA_W_Z(
    COleObjectFactory::XClassFactory* pThis, unsigned long dwReserved,
    wchar_t** pbstrKey) {
    return pThis->RequestLicKey(dwReserved, pbstrKey);
}

//=============================================================================
// COleTemplateServer
//=============================================================================
IMPLEMENT_DYNAMIC(COleTemplateServer, COleObjectFactory)

COleTemplateServer::COleTemplateServer()
    : m_pDocTemplate(nullptr) {
}

COleTemplateServer::~COleTemplateServer() {
}

void COleTemplateServer::ConnectTemplate(REFCLSID clsid, CDocTemplate* pDocTemplate, BOOL bMultiInstance) {
    m_clsid = clsid;
    m_pRuntimeClass = nullptr;
    m_bMultiInstance = bMultiInstance;
    m_pDocTemplate = pDocTemplate;
    COleObjectFactory::ConnectTemplate(this);
}

BOOL COleTemplateServer::Register() {
    return COleObjectFactory::Register();
}

BOOL COleTemplateServer::Unregister() {
    return COleObjectFactory::Unregister();
}

void COleTemplateServer::UpdateRegistry(OLE_APPTYPE nAppType,
                                         const wchar_t** rglpszRegister,
                                         const wchar_t** rglpszOverwrite,
                                         BOOL bRegister) {
    (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite;
    COleObjectFactory::UpdateRegistry(bRegister);
}

CCmdTarget* COleTemplateServer::OnCreateObject() {
    if (!m_pDocTemplate) {
        return nullptr;
    }
    CDocument* pDoc = m_pDocTemplate->CreateNewDocument();
    return pDoc ? static_cast<CCmdTarget*>(pDoc) : nullptr;
}

BOOL COleTemplateServer::OnCmdMsg(UINT nID, int nCode, void* pExtra,
                                  AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (m_pDocTemplate && m_pDocTemplate->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }
    return COleObjectFactory::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//=============================================================================
// COleControlSite
//=============================================================================
IMPLEMENT_DYNAMIC(COleControlSite, CCmdTarget)

namespace {
static HWND GetSiteParentWindow(const COleControlSite* pSite) {
    if (!pSite) {
        return nullptr;
    }
    if (pSite->m_pCtrlCont && pSite->m_pCtrlCont->GetWnd()) {
        HWND hwnd = pSite->m_pCtrlCont->GetWnd()->GetSafeHwnd();
        if (hwnd) {
            return hwnd;
        }
    }
    if (pSite->m_hWnd) {
        HWND hwndParent = ::GetParent(pSite->m_hWnd);
        return hwndParent ? hwndParent : pSite->m_hWnd;
    }
    return nullptr;
}

static RECT GetSitePositionRect(const COleControlSite* pSite) {
    RECT rc = {};
    HWND hwndParent = GetSiteParentWindow(pSite);
    if (pSite && pSite->m_hWnd) {
        ::GetWindowRect(pSite->m_hWnd, &rc);
        if (hwndParent) {
            ::MapWindowPoints(nullptr, hwndParent, reinterpret_cast<POINT*>(&rc), 2);
        }
        return rc;
    }
    if (hwndParent) {
        ::GetClientRect(hwndParent, &rc);
    }
    return rc;
}

static void SetVariantBool(VARIANT* pVar, BOOL value) {
    VariantInit(pVar);
    pVar->vt = VT_BOOL;
    pVar->boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
}

static BOOL TryGetAmbientOverride(COleControlSite* pSite, DISPID dispid, VARIANT* pVarResult) {
    if (!pSite || !pVarResult || !pSite->m_pCtrlCont || !pSite->m_pCtrlCont->GetWnd()) {
        return FALSE;
    }
    VariantInit(pVarResult);
    CWnd* pWnd = pSite->m_pCtrlCont->GetWnd();
    if (impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
            pWnd, pSite, dispid, pVarResult)) {
        return TRUE;
    }
    VariantClear(pVarResult);
    return FALSE;
}

class ControlSiteAdapter : public IOleClientSite,
                           public IOleInPlaceSite,
                           public IOleControlSite,
                           public IOleInPlaceFrame,
                           public IDispatch {
public:
    explicit ControlSiteAdapter(COleControlSite* pSite) : m_refs(1), m_site(pSite) {}
    virtual ~ControlSiteAdapter() = default;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) {
            return E_POINTER;
        }
        *ppvObject = nullptr;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite)) {
            *ppvObject = static_cast<IOleClientSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleWindow) || IsEqualIID(riid, IID_IOleInPlaceSite)) {
            *ppvObject = static_cast<IOleInPlaceSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleControlSite)) {
            *ppvObject = static_cast<IOleControlSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleInPlaceFrame) ||
                   IsEqualIID(riid, IID_IOleInPlaceUIWindow)) {
            *ppvObject = static_cast<IOleInPlaceFrame*>(this);
        } else if (IsEqualIID(riid, IID_IDispatch)) {
            *ppvObject = static_cast<IDispatch*>(this);
        } else {
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef() override {
        return static_cast<ULONG>(InterlockedIncrement(&m_refs));
    }

    ULONG STDMETHODCALLTYPE Release() override {
        LONG refs = InterlockedDecrement(&m_refs);
        if (refs == 0) {
            delete this;
        }
        return static_cast<ULONG>(refs);
    }

    void DetachSite() { m_site = nullptr; }

    HRESULT STDMETHODCALLTYPE SaveObject() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker,
                                         IMoniker** ppmk) override {
        (void)dwAssign; (void)dwWhichMoniker;
        if (ppmk) {
            *ppmk = nullptr;
        }
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer** ppContainer) override {
        if (!ppContainer) {
            return E_POINTER;
        }
        *ppContainer = nullptr;
        return m_site ? m_site->GetContainer(ppContainer) : E_FAIL;
    }

    HRESULT STDMETHODCALLTYPE ShowObject() override {
        if (m_site && m_site->m_lpInPlaceObject) {
            HWND hwnd = nullptr;
            if (SUCCEEDED(m_site->m_lpInPlaceObject->GetWindow(&hwnd))) {
                m_site->m_hWnd = hwnd;
            }
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow) override {
        (void)fShow;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetWindow(HWND* phwnd) override {
        if (!phwnd) {
            return E_POINTER;
        }
        *phwnd = GetSiteParentWindow(m_site);
        return *phwnd ? S_OK : E_FAIL;
    }

    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode) override {
        (void)fEnterMode;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CanInPlaceActivate() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE OnInPlaceActivate() override {
        if (m_site) {
            m_site->m_bInPlaceActive = TRUE;
            if (m_site->m_lpInPlaceObject) {
                HWND hwnd = nullptr;
                if (SUCCEEDED(m_site->m_lpInPlaceObject->GetWindow(&hwnd))) {
                    m_site->m_hWnd = hwnd;
                }
            }
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnUIActivate() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame** ppFrame,
                                               IOleInPlaceUIWindow** ppDoc,
                                               LPRECT lprcPosRect,
                                               LPRECT lprcClipRect,
                                               LPOLEINPLACEFRAMEINFO lpFrameInfo) override {
        if (ppFrame) {
            *ppFrame = static_cast<IOleInPlaceFrame*>(this);
            AddRef();
        }
        if (ppDoc) {
            *ppDoc = nullptr;
        }
        RECT rc = GetSitePositionRect(m_site);
        if (lprcPosRect) {
            *lprcPosRect = rc;
        }
        if (lprcClipRect) {
            HWND hwndParent = GetSiteParentWindow(m_site);
            if (hwndParent) {
                ::GetClientRect(hwndParent, lprcClipRect);
            } else {
                *lprcClipRect = rc;
            }
        }
        if (lpFrameInfo) {
            memset(lpFrameInfo, 0, sizeof(*lpFrameInfo));
            lpFrameInfo->cb = sizeof(*lpFrameInfo);
            lpFrameInfo->hwndFrame = GetSiteParentWindow(m_site);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder) override {
        if (!lprectBorder) {
            return E_POINTER;
        }
        HWND hwndParent = GetSiteParentWindow(m_site);
        if (hwndParent) {
            ::GetClientRect(hwndParent, lprectBorder);
        } else {
            *lprectBorder = GetSitePositionRect(m_site);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths) override {
        (void)pborderwidths;
        return INPLACE_E_NOTOOLSPACE;
    }

    HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths) override {
        (void)pborderwidths;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject* pActiveObject,
                                              LPCOLESTR pszObjName) override {
        (void)pActiveObject; (void)pszObjName;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared,
                                          LPOLEMENUGROUPWIDTHS lpMenuWidths) override {
        (void)hmenuShared;
        if (lpMenuWidths) {
            memset(lpMenuWidths, 0, sizeof(*lpMenuWidths));
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu,
                                      HWND hwndActiveObject) override {
        (void)hmenuShared; (void)holemenu; (void)hwndActiveObject;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared) override {
        (void)hmenuShared;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText) override {
        (void)pszStatusText;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable) override {
        (void)fEnable;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID) override {
        (void)lpmsg; (void)wID;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant) override {
        (void)scrollExtant;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable) override {
        (void)fUndoable;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() override {
        if (m_site) {
            m_site->m_bInPlaceActive = FALSE;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE DiscardUndoState() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE DeactivateAndUndo() override {
        if (m_site) {
            m_site->Deactivate();
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect) override {
        if (!lprcPosRect) {
            return E_POINTER;
        }
        if (m_site && m_site->m_lpInPlaceObject) {
            return m_site->m_lpInPlaceObject->SetObjectRects(lprcPosRect, lprcPosRect);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnControlInfoChanged() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE LockInPlaceActive(BOOL fLock) override {
        (void)fLock;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetExtendedControl(IDispatch** ppDisp) override {
        if (!ppDisp) {
            return E_POINTER;
        }
        *ppDisp = nullptr;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer,
                                               DWORD dwFlags) override {
        if (!pPtlHimetric || !pPtfContainer) {
            return E_POINTER;
        }
        HWND hwndParent = GetSiteParentWindow(m_site);
        HDC hdc = ::GetDC(hwndParent);
        int dpiX = hdc ? ::GetDeviceCaps(hdc, LOGPIXELSX) : 96;
        int dpiY = hdc ? ::GetDeviceCaps(hdc, LOGPIXELSY) : 96;
        if (hdc) {
            ::ReleaseDC(hwndParent, hdc);
        }
        if (dpiX <= 0) {
            dpiX = 96;
        }
        if (dpiY <= 0) {
            dpiY = 96;
        }

        if (dwFlags & XFORMCOORDS_HIMETRICTOCONTAINER) {
            pPtfContainer->x = static_cast<float>((pPtlHimetric->x * dpiX) / 2540.0);
            pPtfContainer->y = static_cast<float>((pPtlHimetric->y * dpiY) / 2540.0);
            return S_OK;
        }
        if (dwFlags & XFORMCOORDS_CONTAINERTOHIMETRIC) {
            pPtlHimetric->x = static_cast<LONG>((pPtfContainer->x * 2540.0f) / dpiX);
            pPtlHimetric->y = static_cast<LONG>((pPtfContainer->y * 2540.0f) / dpiY);
            return S_OK;
        }
        return E_INVALIDARG;
    }

    HRESULT STDMETHODCALLTYPE TranslateAccelerator(MSG* pMsg, DWORD grfModifiers) override {
        (void)pMsg; (void)grfModifiers;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE OnFocus(BOOL fGotFocus) override {
        (void)fGotFocus;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE ShowPropertyFrame() override {
        if (m_site) {
            m_site->ShowPropertyFrame();
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo) override {
        if (!pctinfo) {
            return E_POINTER;
        }
        *pctinfo = 0;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override {
        (void)iTInfo; (void)lcid;
        if (ppTInfo) {
            *ppTInfo = nullptr;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,
                                            UINT cNames, LCID lcid, DISPID* rgDispId) override {
        (void)riid; (void)rgszNames; (void)cNames; (void)lcid;
        if (rgDispId && cNames) {
            for (UINT i = 0; i < cNames; ++i) {
                rgDispId[i] = DISPID_UNKNOWN;
            }
        }
        return DISP_E_UNKNOWNNAME;
    }

    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
                                     WORD wFlags, DISPPARAMS* pDispParams,
                                     VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
                                     UINT* puArgErr) override {
        (void)riid; (void)lcid; (void)pDispParams; (void)pExcepInfo; (void)puArgErr;
        if (!(wFlags & DISPATCH_PROPERTYGET)) {
            return DISP_E_MEMBERNOTFOUND;
        }
        if (!pVarResult) {
            return E_POINTER;
        }
        if (TryGetAmbientOverride(m_site, dispIdMember, pVarResult)) {
            return S_OK;
        }

        switch (dispIdMember) {
        case DISPID_AMBIENT_USERMODE:
        case DISPID_AMBIENT_SUPPORTSMNEMONICS:
            SetVariantBool(pVarResult, TRUE);
            return S_OK;
        case DISPID_AMBIENT_UIDEAD:
        case DISPID_AMBIENT_DISPLAYASDEFAULT:
        case DISPID_AMBIENT_SHOWGRABHANDLES:
        case DISPID_AMBIENT_SHOWHATCHING:
            SetVariantBool(pVarResult, FALSE);
            return S_OK;
        case DISPID_AMBIENT_BACKCOLOR:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetSysColor(COLOR_WINDOW));
            return S_OK;
        case DISPID_AMBIENT_FORECOLOR:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetSysColor(COLOR_WINDOWTEXT));
            return S_OK;
        case DISPID_AMBIENT_LOCALEID:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetUserDefaultLCID());
            return S_OK;
        case DISPID_AMBIENT_DISPLAYNAME:
        case DISPID_AMBIENT_SCALEUNITS:
            VariantInit(pVarResult);
            pVarResult->vt = VT_BSTR;
            pVarResult->bstrVal = SysAllocString(L"");
            return pVarResult->bstrVal ? S_OK : E_OUTOFMEMORY;
        case DISPID_AMBIENT_APPEARANCE:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I2;
            pVarResult->iVal = 0;
            return S_OK;
        default:
            return DISP_E_MEMBERNOTFOUND;
        }
    }

private:
    volatile LONG m_refs;
    COleControlSite* m_site;
};

struct OleControlSiteState {
    UINT controlId = 0;
    ControlSiteAdapter* adapter = nullptr;
};

static std::map<const COleControlSite*, OleControlSiteState> g_oleControlSiteState;

static void SetControlSiteId(const COleControlSite* pSite, UINT controlId) {
    if (!pSite) {
        return;
    }
    g_oleControlSiteState[pSite].controlId = controlId;
}

static void RemoveControlSiteState(const COleControlSite* pSite) {
    auto it = g_oleControlSiteState.find(pSite);
    if (it != g_oleControlSiteState.end()) {
        if (it->second.adapter) {
            it->second.adapter->DetachSite();
            it->second.adapter->Release();
        }
        g_oleControlSiteState.erase(it);
    }
}

static BOOL TryGetControlSiteId(const COleControlSite* pSite, UINT* pControlId) {
    if (!pSite || !pControlId) {
        return FALSE;
    }
    auto it = g_oleControlSiteState.find(pSite);
    if (it == g_oleControlSiteState.end()) {
        return FALSE;
    }
    *pControlId = it->second.controlId;
    return TRUE;
}

static ControlSiteAdapter* GetControlSiteAdapter(COleControlSite* pSite, bool create) {
    if (!pSite) {
        return nullptr;
    }
    auto it = g_oleControlSiteState.find(pSite);
    if (it == g_oleControlSiteState.end()) {
        if (!create) {
            return nullptr;
        }
        it = g_oleControlSiteState.emplace(pSite, OleControlSiteState{}).first;
    }
    if (!it->second.adapter && create) {
        it->second.adapter = new(std::nothrow) ControlSiteAdapter(pSite);
    }
    return it->second.adapter;
}
} // namespace

COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
    : m_pCtrlCont(pCtrlCont), m_pControl(nullptr),
      m_lpObject(nullptr), m_lpInPlaceObject(nullptr), m_lpDispatch(nullptr),
      m_hWnd(nullptr), m_bInPlaceActive(FALSE), m_dwStyle(0) {
    memset(_olecontrolsite_padding, 0, sizeof(_olecontrolsite_padding));
}

COleControlSite::~COleControlSite() {
    DestroyControl();
    RemoveControlSiteState(this);
}

BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, REFCLSID clsid, const wchar_t* lpszWindowName,
                                     DWORD dwStyle, const RECT& rect, UINT nID, CFile* pPersist,
                                     BOOL bStorage, BSTR bstrLicKey) {
    (void)lpszWindowName; (void)pPersist; (void)bStorage;
    m_dwStyle = dwStyle;
    SetControlSiteId(this, nID);

    // Use CoCreateInstance (with optional license via IClassFactory2)
    IUnknown* pUnk = nullptr;
    HRESULT hr;
    if (bstrLicKey) {
        IClassFactory2* pCF2 = nullptr;
        hr = CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, nullptr, IID_IClassFactory2,
                              reinterpret_cast<void**>(&pCF2));
        if (SUCCEEDED(hr) && pCF2) {
            hr = pCF2->CreateInstanceLic(nullptr, nullptr, IID_IUnknown, bstrLicKey,
                                          reinterpret_cast<void**>(&pUnk));
            pCF2->Release();
        } else {
            hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown,
                                  reinterpret_cast<void**>(&pUnk));
        }
    } else {
        hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown,
                              reinterpret_cast<void**>(&pUnk));
    }
    if (FAILED(hr) || !pUnk) return FALSE;

    // Get IOleObject
    hr = pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&m_lpObject));
    if (FAILED(hr)) { pUnk->Release(); return FALSE; }

    // Get IDispatch
    pUnk->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&m_lpDispatch));

    // Get IOleInPlaceObject
    pUnk->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject));

    pUnk->Release();

    ControlSiteAdapter* pAdapter = GetControlSiteAdapter(this, true);
    if (!pAdapter) {
        DestroyControl();
        return FALSE;
    }
    hr = m_lpObject->SetClientSite(static_cast<IOleClientSite*>(pAdapter));
    if (FAILED(hr)) {
        DestroyControl();
        return FALSE;
    }
    OleSetContainedObject(m_lpObject, TRUE);

    // Set host names
    m_lpObject->SetHostNames(L"OpenMFC", lpszWindowName ? lpszWindowName : L"");

    // Activate in-place if we have a parent window
    HWND hParent = pWndCtrl ? pWndCtrl->GetSafeHwnd() : nullptr;
    if (!hParent) {
        hParent = GetSiteParentWindow(this);
    }
    if (hParent) {
        hr = m_lpObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr,
                                static_cast<IOleClientSite*>(pAdapter), 0, hParent, &rect);
        if (SUCCEEDED(hr)) {
            m_bInPlaceActive = TRUE;
            // Retrieve the in-place window handle
            if (m_lpInPlaceObject) {
                m_lpInPlaceObject->GetWindow(&m_hWnd);
                m_lpInPlaceObject->SetObjectRects(&rect, &rect);
            }
        }
    }

    return TRUE;
}

BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, const wchar_t* lpszProgID,
                                     DWORD dwStyle, const RECT& rect, UINT nID) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(pWndCtrl, clsid, L"", dwStyle, rect, nID);
}

BOOL COleControlSite::DestroyControl() {
    if (m_lpObject) { m_lpObject->SetClientSite(nullptr); }
    if (m_lpObject) { m_lpObject->Release(); m_lpObject = nullptr; }
    if (m_lpInPlaceObject) { m_lpInPlaceObject->Release(); m_lpInPlaceObject = nullptr; }
    if (m_lpDispatch) { m_lpDispatch->Release(); m_lpDispatch = nullptr; }
    m_pControl = nullptr;
    m_hWnd = nullptr;
    m_bInPlaceActive = FALSE;
    return TRUE;
}

void COleControlSite::Activate(BOOL bActivate) {
    if (bActivate) {
        if (!m_bInPlaceActive && m_lpObject) {
            HWND hParent = m_hWnd ? ::GetParent(m_hWnd) : nullptr;
            if (!hParent && m_pCtrlCont && m_pCtrlCont->GetWnd())
                hParent = m_pCtrlCont->GetWnd()->GetSafeHwnd();
            if (hParent) {
                RECT rc = {};
                if (m_hWnd) {
                    rc = GetSitePositionRect(this);
                } else {
                    ::GetClientRect(hParent, &rc);
                }
                ControlSiteAdapter* pAdapter = GetControlSiteAdapter(this, true);
                if (!pAdapter) {
                    return;
                }
                HRESULT hr = m_lpObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr,
                                                static_cast<IOleClientSite*>(pAdapter), 0, hParent, &rc);
                if (SUCCEEDED(hr)) {
                    m_bInPlaceActive = TRUE;
                    if (m_lpInPlaceObject && !m_hWnd)
                        m_lpInPlaceObject->GetWindow(&m_hWnd);
                    if (m_lpInPlaceObject)
                        m_lpInPlaceObject->SetObjectRects(&rc, &rc);
                }
            }
        }
    } else {
        Deactivate();
    }
}

void COleControlSite::Deactivate() {
    if (m_bInPlaceActive && m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}

BOOL COleControlSite::IsInPlaceActive() const {
    return m_bInPlaceActive;
}

void COleControlSite::SetProperty(DISPID dwDispID, VARTYPE vtProp, ...) {
    BYTE params[2] = { static_cast<BYTE>(vtProp), 0 };
    va_list args;
    va_start(args, vtProp);
    InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr,
                  params, args);
    va_end(args);
}

void COleControlSite::GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) {
    InvokeHelper(dwDispID, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr);
}

HRESULT COleControlSite::InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                       void* pvRet, const BYTE* pbParamInfo, ...) {
    va_list args;
    va_start(args, pbParamInfo);
    HRESULT hr = InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
    va_end(args);
    return hr;
}

HRESULT COleControlSite::InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                        void* pvRet, const BYTE* pbParamInfo, va_list argList) {
    if (!m_lpDispatch) return E_POINTER;

    int cParams = CountDispatchParams(pbParamInfo);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        HRESULT hr = MakeDispatchVariant(static_cast<VARTYPE>(pbParamInfo[i]), &argList, &params[static_cast<size_t>(i)]);
        if (FAILED(hr)) {
            for (int j = 0; j < i; ++j) VariantClear(&params[static_cast<size_t>(j)]);
            return hr;
        }
    }

    std::vector<VARIANTARG> reversed(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dispParams = {};
    dispParams.cArgs = cParams;
    dispParams.rgvarg = cParams ? reversed.data() : nullptr;
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) {
        dispParams.cNamedArgs = 1;
        dispParams.rgdispidNamedArgs = &dispidNamed;
    }

    VARIANT result;
    VariantInit(&result);
    EXCEPINFO excep = {};
    UINT argErr = 0;
    HRESULT hr = m_lpDispatch->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
                                      &dispParams, &result, &excep, &argErr);

    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }

    if (FAILED(hr)) {
        if (excep.bstrDescription) SysFreeString(excep.bstrDescription);
        if (excep.bstrSource) SysFreeString(excep.bstrSource);
        if (excep.bstrHelpFile) SysFreeString(excep.bstrHelpFile);
        VariantClear(&result);
        return hr;
    }

    hr = CopyDispatchResult(vtRet, pvRet, &result);
    VariantClear(&result);
    return hr;
}

BOOL COleControlSite::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    if (!pvProp) {
        return FALSE;
    }

    if (m_lpDispatch &&
        SUCCEEDED(InvokeHelper(dwDispid, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr))) {
        return TRUE;
    }

    switch (dwDispid) {
    case DISPID_AMBIENT_USERMODE:
        if (vtProp == VT_BOOL) {
            *static_cast<VARIANT_BOOL*>(pvProp) = VARIANT_TRUE;
            return TRUE;
        }
        break;
    case DISPID_AMBIENT_BACKCOLOR:
        if (vtProp == VT_COLOR || vtProp == VT_I4) {
            *static_cast<long*>(pvProp) = static_cast<long>(GetSysColor(COLOR_WINDOW));
            return TRUE;
        }
        break;
    case DISPID_AMBIENT_FORECOLOR:
        if (vtProp == VT_COLOR || vtProp == VT_I4) {
            *static_cast<long*>(pvProp) = static_cast<long>(GetSysColor(COLOR_WINDOWTEXT));
            return TRUE;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

long COleControlSite::GetWindow(HWND__** phWnd) {
    if (!phWnd) return E_POINTER;
    *phWnd = m_hWnd;
    return m_hWnd ? S_OK : E_FAIL;
}

CWnd* COleControlSite::GetWindow() const {
    return CWnd::FromHandle(m_hWnd);
}

long COleControlSite::GetContainer(IOleContainer** ppContainer) {
    if (!ppContainer) return E_POINTER;
    *ppContainer = nullptr;
    return E_NOINTERFACE;
}

void COleControlSite::ShowPropertyFrame() {
    if (m_pControl) {
        m_pControl->ShowPropertyPages();
        return;
    }
    if (!m_lpObject) {
        return;
    }

    ISpecifyPropertyPages* pSPP = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_ISpecifyPropertyPages,
                                          reinterpret_cast<void**>(&pSPP))) || !pSPP) {
        return;
    }

    CAUUID pages = {};
    if (SUCCEEDED(pSPP->GetPages(&pages)) && pages.cElems > 0 && pages.pElems) {
        IUnknown* pUnk = nullptr;
        m_lpObject->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
        HWND hwndOwner = m_hWnd ? m_hWnd : GetSiteParentWindow(this);
        OleCreatePropertyFrame(hwndOwner, 0, 0, nullptr,
                               pUnk ? 1u : 0u, pUnk ? &pUnk : nullptr,
                               pages.cElems, pages.pElems,
                               LOCALE_USER_DEFAULT, 0, nullptr);
        if (pUnk) {
            pUnk->Release();
        }
    }
    CoTaskMemFree(pages.pElems);
    pSPP->Release();
}

void COleControlSite::EnableWindow(BOOL bEnable) {
    if (m_hWnd) {
        ::EnableWindow(m_hWnd, bEnable);
    }
}

COleControlContainer* COleControlSite::GetContainer() const {
    return m_pCtrlCont;
}

COleControl* COleControlSite::GetControl() const {
    return m_pControl;
}

LPOLEOBJECT COleControlSite::GetOleObject() {
    return m_lpObject;
}

LPOLEINPLACEOBJECT COleControlSite::GetInPlaceObject() {
    return m_lpInPlaceObject;
}

LPDISPATCH COleControlSite::GetDispatch() {
    return m_lpDispatch;
}

//=============================================================================
// COleControlContainer
//=============================================================================
IMPLEMENT_DYNAMIC(COleControlContainer, CCmdTarget)

COleControlContainer::COleControlContainer(CWnd* pWnd)
    : m_pWnd(pWnd) {
    memset(_olecontrolcontainer_padding, 0, sizeof(_olecontrolcontainer_padding));
}

COleControlContainer::~COleControlContainer() {
    // Cleanup sites without using POSITION
    while (!m_listSites.IsEmpty()) {
        COleControlSite* pSite = (COleControlSite*)m_listSites.RemoveHead();
        delete pSite;
    }
}

COleControlSite* COleControlContainer::CreateSite(COleControlContainer* pCtrlCont) {
    COleControlSite* pSite = new COleControlSite(pCtrlCont);
    m_listSites.AddTail(pSite);
    return pSite;
}

BOOL COleControlContainer::DeleteSite(COleControlSite* pSite) {
    // Look for pSite in the list and remove it
    CPtrList::POSITION pos = m_listSites.Find(pSite);
    if (pos != CPtrList::POSITION(nullptr)) {
        m_listSites.RemoveAt(pos);
        delete pSite;
        return TRUE;
    }
    return FALSE;
}

COleControlSite* COleControlContainer::FindItem(UINT nID) const {
    CPtrList::POSITION pos = m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(m_listSites.GetNext(pos));
        if (!pSite) {
            continue;
        }

        UINT siteId = 0;
        if (TryGetControlSiteId(pSite, &siteId) && siteId == nID) {
            return pSite;
        }

        HWND hWnd = pSite->m_hWnd;
        if (hWnd && ::IsWindow(hWnd) && static_cast<UINT>(::GetDlgCtrlID(hWnd)) == nID) {
            return pSite;
        }
    }
    return nullptr;
}

//=============================================================================
// COleIPFrameWnd
//=============================================================================
IMPLEMENT_DYNAMIC(COleIPFrameWnd, CFrameWnd)

COleIPFrameWnd::COleIPFrameWnd()
    : CFrameWnd(), m_pResizeBar(nullptr) {
    memset(_oleipframewnd_padding, 0, sizeof(_oleipframewnd_padding));
}

COleIPFrameWnd::~COleIPFrameWnd() {
}

BOOL COleIPFrameWnd::OnCreateAggregates() {
    return TRUE;
}

BOOL COleIPFrameWnd::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc) {
    return TRUE;
}

void COleIPFrameWnd::OnRequestPositionChange(LPCRECT lpRect) {
    if (!m_hWnd || !lpRect) return;

    const int width = lpRect->right - lpRect->left;
    const int height = lpRect->bottom - lpRect->top;
    ::SetWindowPos(m_hWnd, nullptr, lpRect->left, lpRect->top,
                   width > 0 ? width : 0, height > 0 ? height : 0,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    if (m_pResizeBar && m_pResizeBar->GetSafeHwnd()) {
        m_pResizeBar->MoveWindow(lpRect->left, lpRect->top,
                                 width > 0 ? width : 0, height > 0 ? height : 0, TRUE);
    }
}

//=============================================================================
// COleConnPtContainer
//=============================================================================
COleConnPtContainer::COleConnPtContainer() {
    memset(_oleconnptcontainer_padding, 0, sizeof(_oleconnptcontainer_padding));
}

COleConnPtContainer::~COleConnPtContainer() {
}

//=============================================================================
// CEnumOleVerb
//=============================================================================
CEnumOleVerb::CEnumOleVerb() : m_verbs(nullptr), m_count(0), m_position(0) {
}

CEnumOleVerb::~CEnumOleVerb() {
}

void CEnumOleVerb::SetVerbs(OLEVERB* pVerbs, unsigned long nCount) {
    m_verbs = pVerbs;
    m_count = nCount;
    m_position = 0;
}

// Fetches the verb at the cursor and advances. Retail (mfc140u
// ?OnNext@CEnumOleVerb@@) runs the CEnumArray element copy first and, when that
// succeeds, replaces OLEVERB::lpszVerbName with a caller-owned duplicate --
// allocated with the COM task allocator, since the caller is expected to
// CoTaskMemFree it -- and raises a memory exception if that allocation fails.
int CEnumOleVerb::OnNext(void* pv) {
    if (pv == nullptr) return FALSE;
    if (m_verbs == nullptr || m_position >= m_count) return FALSE;

    // Build the result from the verb at the cursor WITHOUT consuming it, so a
    // failed allocation leaves the enumerator exactly where it was. Advancing
    // first would silently swallow a verb on OOM: the caller gets FALSE, and
    // a retry resumes at the following element.
    const OLEVERB& src = m_verbs[m_position];
    OLEVERB result = src;

    if (src.lpszVerbName != nullptr) {
        const size_t cb = (wcslen(src.lpszVerbName) + 1) * sizeof(wchar_t);
        LPOLESTR copy = static_cast<LPOLESTR>(::CoTaskMemAlloc(cb));
        if (copy == nullptr) {
            return FALSE;   // cursor untouched; caller may retry this verb
        }
        memcpy(copy, src.lpszVerbName, cb);
        result.lpszVerbName = copy;
    }

    *static_cast<OLEVERB*>(pv) = result;
    ++m_position;
    return TRUE;
}

// Symbol: ?OnNext@CEnumOleVerb@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumOleVerb__MEAAHPEAX_Z(CEnumOleVerb* pThis, void* pv) {
    return pThis ? pThis->OnNext(pv) : FALSE;
}

HWND COleControlSiteOrWnd::GetSafeHwnd() const {
    if (m_pSite) {
        return m_pSite->m_hWnd;
    }
    return m_pWnd ? m_pWnd->GetSafeHwnd() : nullptr;
}

DWORD COleControlSiteOrWnd::GetStyle() const {
    if (m_pSite) {
        return m_pSite->m_dwStyle;
    }
    HWND hWnd = GetSafeHwnd();
    if (!hWnd) {
        return 0;
    }
    return static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE));
}

DWORD COleControlSiteOrWnd::GetExStyle() const {
    HWND hWnd = GetSafeHwnd();
    if (!hWnd) {
        return 0;
    }
    return static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_EXSTYLE));
}

//=============================================================================
// COleDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleDialog, CDialog)

COleDialog::COleDialog(UINT nIDTemplate, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd) {
    memset(_olediag_padding, 0, sizeof(_olediag_padding));
}

COleDialog::COleDialog(CWnd* pParentWnd)
    : CDialog(static_cast<UINT>(0), pParentWnd) {
    memset(_olediag_padding, 0, sizeof(_olediag_padding));
}

COleDialog::~COleDialog() {
}

int COleDialog::MapResult(UINT nResult) {
    switch (nResult) {
    case OLEUI_OK:
        return IDOK;
    case OLEUI_CANCEL:
        return IDCANCEL;
    default:
        return static_cast<int>(nResult);
    }
}

//=============================================================================
// COleBusyDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleBusyDialog, COleDialog)

COleBusyDialog::COleBusyDialog(HTASK hTaskBusy, BOOL bNotResponding,
                                DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_bz, 0, sizeof(m_bz));
    m_bz.cbStruct = sizeof(OLEUIBUSYW);
    m_bz.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_bz.dwFlags = dwFlags;
    m_bz.hTask = hTaskBusy;
}

COleBusyDialog::~COleBusyDialog() {
}

intptr_t COleBusyDialog::DoModal() {
    UINT result = OleUIBusyW(&m_bz);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

//=============================================================================
// COleChangeIconDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleChangeIconDialog, COleDialog)

COleChangeIconDialog::COleChangeIconDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd) {
    memset(&m_ci, 0, sizeof(m_ci));
    m_ci.cbStruct = sizeof(OLEUICHANGEICONW);
    m_ci.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ci.dwFlags = dwFlags;
    // Seed the dialog from the item (no separate member; state lives in m_ci).
    if (pItem != nullptr) {
        pItem->GetClassID(&m_ci.clsid);
        m_ci.hMetaPict = pItem->GetIconicMetafile();
    }
}

COleChangeIconDialog::~COleChangeIconDialog() {
}

intptr_t COleChangeIconDialog::DoModal() {
    UINT result = OleUIChangeIconW(&m_ci);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

int COleChangeIconDialog::DoChangeIcon(COleClientItem* pItem) {
    if (DoModal() != IDOK)
        return FALSE;
    // Apply the chosen icon back to the item.
    if (pItem != nullptr)
        pItem->SetIconicMetafile(m_ci.hMetaPict);
    return TRUE;
}

//=============================================================================
// COleChangeSourceDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleChangeSourceDialog, COleDialog)

COleChangeSourceDialog::COleChangeSourceDialog(COleClientItem* pItem, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pItem(pItem) {
    memset(&m_cs, 0, sizeof(m_cs));
    m_cs.cbStruct = sizeof(OLEUICHANGESOURCEW);
    m_cs.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cs.dwFlags = dwFlags;
    memset(_olechangesourcedialog_padding, 0, sizeof(_olechangesourcedialog_padding));
}

COleChangeSourceDialog::~COleChangeSourceDialog() {
}

intptr_t COleChangeSourceDialog::DoModal() {
    UINT result = OleUIChangeSourceW(&m_cs);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CString COleChangeSourceDialog::GetDisplayName() {
    return m_cs.lpszDisplayName ? m_cs.lpszDisplayName : L"";
}

CString COleChangeSourceDialog::GetFrom() {
    return m_cs.lpszFrom ? m_cs.lpszFrom : L"";
}

CString COleChangeSourceDialog::GetTo() {
    return m_cs.lpszTo ? m_cs.lpszTo : L"";
}

void COleChangeSourceDialog::PreInitDialog() {
    static const wchar_t kEmpty[] = L"";
    m_cs.lpszDisplayName = const_cast<wchar_t*>(kEmpty);
    m_cs.lpszFrom = const_cast<wchar_t*>(kEmpty);
    m_cs.lpszTo = const_cast<wchar_t*>(kEmpty);
}

//=============================================================================
// COleConvertDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleConvertDialog, COleDialog)

COleConvertDialog::COleConvertDialog(COleClientItem* pItem, DWORD dwFlags,
                                      CLSID* pClassID, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd) {
    memset(&m_cv, 0, sizeof(m_cv));
    m_cv.cbStruct = sizeof(OLEUICONVERTW);
    m_cv.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_cv.dwFlags = dwFlags;
    // Class id: explicit argument wins, else seed from the item.
    if (pClassID)
        m_cv.clsid = *pClassID;
    else if (pItem != nullptr)
        pItem->GetClassID(&m_cv.clsid);
}

COleConvertDialog::~COleConvertDialog() {
}

intptr_t COleConvertDialog::DoModal() {
    UINT result = OleUIConvertW(&m_cv);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COleConvertDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}

BOOL COleConvertDialog::IsConvertTo() const {
    return (m_cv.clsidConvertDefault == m_cv.clsidNew);
}

BOOL COleConvertDialog::IsActivateAs() const {
    return FALSE;
}

CString COleConvertDialog::GetDisplayIcon() const {
    return L"";
}

int COleConvertDialog::DoConvert(COleClientItem* pItem) {
    if (DoModal() != IDOK)
        return FALSE;
    // Apply the chosen conversion/activation to the item.
    if (pItem != nullptr) {
        if (IsConvertTo()) {
            pItem->ConvertTo(GetClassID());
        } else {
            CLSID clsidOld;
            pItem->GetClassID(&clsidOld);
            pItem->ActivateAs(GetClassID(), clsidOld);
        }
    }
    return TRUE;
}

UINT COleConvertDialog::GetSelectionType() const {
    if (m_cv.dwFlags & CF_SELECTACTIVATEAS) {
        return CF_SELECTACTIVATEAS;
    }
    if (m_cv.dwFlags & CF_SELECTCONVERTTO) {
        return CF_SELECTCONVERTTO;
    }
    return 0;
}

//=============================================================================
// COleInsertDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleInsertDialog, COleDialog)

COleInsertDialog::COleInsertDialog(DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags) {
    memset(&m_io, 0, sizeof(m_io));
    m_io.cbStruct = sizeof(OLEUIINSERTOBJECTW);
    m_io.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_io.dwFlags = dwFlags;
    memset(_oleinsertdialog_padding, 0, sizeof(_oleinsertdialog_padding));
}

COleInsertDialog::~COleInsertDialog() {
}

__int64 COleInsertDialog::DoModal() {
    return DoModal(0UL);
}

__int64 COleInsertDialog::DoModal(unsigned long) {
    UINT result = OleUIInsertObjectW(&m_io);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COleInsertDialog::GetClassID() const {
    return m_io.clsid;
}

CString COleInsertDialog::GetPathName() const {
    return m_io.lpszFile ? m_io.lpszFile : L"";
}

BOOL COleInsertDialog::IsCreateNew() const {
    return m_io.dwFlags & IOF_SELECTCREATENEW || !(m_io.dwFlags & IOF_SELECTCREATEFROMFILE);
}

BOOL COleInsertDialog::IsDisplayAsIcon() const {
    return m_io.dwFlags & IOF_CHECKDISPLAYASICON;
}

int COleInsertDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
}

COleClientItem* COleInsertDialog::CreateItem(COleDocument* pDoc) {
    (void)pDoc;
    return nullptr;
}

void COleInsertDialog::GetIconMetafile(HGLOBAL* phMetaPict) {
    if (phMetaPict) *phMetaPict = m_io.hMetaPict;
}

UINT COleInsertDialog::GetSelectionType() const {
    if (m_io.dwFlags & IOF_SELECTCREATECONTROL) {
        return IOF_SELECTCREATECONTROL;
    }
    if (m_io.dwFlags & IOF_SELECTCREATEFROMFILE) {
        return IOF_SELECTCREATEFROMFILE;
    }
    if (m_io.dwFlags & IOF_SELECTCREATENEW) {
        return IOF_SELECTCREATENEW;
    }
    return 0;
}

void COleInsertDialog::AddClassIDToList(CLSID*& rgclsid, int& nCount, int& nAlloc, CLSID* pClassID) {
    if (!pClassID) {
        return;
    }
    if (nCount >= nAlloc) {
        int nNewAlloc = nAlloc > 0 ? nAlloc * 2 : 8;
        CLSID* pNewList = static_cast<CLSID*>(std::realloc(rgclsid, sizeof(CLSID) * nNewAlloc));
        if (!pNewList) {
            return;
        }
        rgclsid = pNewList;
        nAlloc = nNewAlloc;
    }
    rgclsid[nCount++] = *pClassID;
}

//=============================================================================
// COleLinksDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleLinksDialog, COleDialog)

COleLinksDialog::COleLinksDialog(COleDocument* pDoc, CView* pView, DWORD dwFlags, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDoc(pDoc) {
    memset(&m_el, 0, sizeof(m_el));
    m_el.cbStruct = sizeof(OLEUIEDITLINKSW);
    m_el.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_el.dwFlags = dwFlags;
    memset(_olelinksdialog_padding, 0, sizeof(_olelinksdialog_padding));
}

COleLinksDialog::~COleLinksDialog() {
}

intptr_t COleLinksDialog::DoModal() {
    UINT result = OleUIEditLinksW(&m_el);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

//=============================================================================
// COlePasteSpecialDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COlePasteSpecialDialog, COleDialog)

COlePasteSpecialDialog::COlePasteSpecialDialog(DWORD dwFlags, COleDataObject* pDataObject, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_dwFlags(dwFlags), m_pDataObject(pDataObject) {
    memset(&m_ps, 0, sizeof(m_ps));
    m_ps.cbStruct = sizeof(OLEUIPASTESPECIALW);
    m_ps.hWndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_ps.dwFlags = dwFlags;
    memset(_olepastepecialdialog_padding, 0, sizeof(_olepastepecialdialog_padding));
}

COlePasteSpecialDialog::~COlePasteSpecialDialog() {
}

intptr_t COlePasteSpecialDialog::DoModal() {
    UINT result = OleUIPasteSpecialW(&m_ps);
    return (result == OLEUI_OK) ? IDOK : IDCANCEL;
}

CLSID COlePasteSpecialDialog::GetClassID() const {
    CLSID clsid = {};
    return clsid;
}

BOOL COlePasteSpecialDialog::IsPasteLink() const {
    return (m_ps.dwFlags & PSF_SELECTPASTELINK) != 0;
}

int COlePasteSpecialDialog::CreateItem(COleClientItem* pItem) {
    (void)pItem;
    return 0;
}

COleClientItem* COlePasteSpecialDialog::CreateItem(COleDocument* pDoc) {
    return nullptr;
}

UINT COlePasteSpecialDialog::GetSelectionType() const {
    if (m_ps.dwFlags & PSF_SELECTPASTELINK) {
        return PSF_SELECTPASTELINK;
    }
    if (m_ps.dwFlags & PSF_SELECTPASTE) {
        return PSF_SELECTPASTE;
    }
    return 0;
}

void COlePasteSpecialDialog::AddFormat(const FORMATETC& formatEtc, wchar_t* lpszFormat,
                                       wchar_t* lpszResult, DWORD flags) {
    int nNewCount = m_ps.cPasteEntries + 1;
    OLEUIPASTEENTRYW* pEntries = static_cast<OLEUIPASTEENTRYW*>(
        std::realloc(m_ps.arrPasteEntries, sizeof(OLEUIPASTEENTRYW) * nNewCount));
    if (!pEntries) {
        return;
    }
    m_ps.arrPasteEntries = pEntries;

    OLEUIPASTEENTRYW& entry = m_ps.arrPasteEntries[m_ps.cPasteEntries++];
    memset(&entry, 0, sizeof(entry));
    entry.fmtetc = formatEtc;
    entry.lpstrFormatName = lpszFormat;
    entry.lpstrResultText = lpszResult;
    entry.dwFlags = flags;
}

void COlePasteSpecialDialog::AddFormat(UINT cfFormat, TYMED tymed, UINT nFormatID,
                                       BOOL bEnableIcon, BOOL bLink) {
    (void)nFormatID;
    static wchar_t szFormat[] = L"Format";
    static wchar_t szPasteResult[] = L"Paste";
    static wchar_t szLinkResult[] = L"Link";

    FORMATETC formatEtc = {};
    formatEtc.cfFormat = static_cast<CLIPFORMAT>(cfFormat);
    formatEtc.dwAspect = DVASPECT_CONTENT;
    formatEtc.lindex = -1;
    formatEtc.tymed = tymed;

    DWORD flags = bLink ? OLEUIPASTE_LINKANYTYPE : OLEUIPASTE_PASTE;
    if (bEnableIcon) {
        flags |= OLEUIPASTE_ENABLEICON;
    }
    AddFormat(formatEtc, szFormat, bLink ? szLinkResult : szPasteResult, flags);
}

OLEUIPASTEFLAG COlePasteSpecialDialog::AddLinkEntry(UINT nFormatID) {
    static const OLEUIPASTEFLAG linkFlags[] = {
        OLEUIPASTE_LINKTYPE1, OLEUIPASTE_LINKTYPE2, OLEUIPASTE_LINKTYPE3, OLEUIPASTE_LINKTYPE4,
        OLEUIPASTE_LINKTYPE5, OLEUIPASTE_LINKTYPE6, OLEUIPASTE_LINKTYPE7, OLEUIPASTE_LINKTYPE8
    };
    if (nFormatID == 0 || nFormatID > sizeof(linkFlags) / sizeof(linkFlags[0])) {
        return OLEUIPASTE_LINKANYTYPE;
    }
    return linkFlags[nFormatID - 1];
}

void COlePasteSpecialDialog::AddStandardFormats(BOOL bEnableLink) {
    AddFormat(CF_UNICODETEXT, TYMED_HGLOBAL, CF_UNICODETEXT, FALSE, FALSE);
    AddFormat(CF_TEXT, TYMED_HGLOBAL, CF_TEXT, FALSE, FALSE);
    if (bEnableLink) {
        AddFormat(CF_UNICODETEXT, TYMED_HGLOBAL, CF_UNICODETEXT, FALSE, TRUE);
    }
}

//=============================================================================
// COlePropertiesDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COlePropertiesDialog, COleDialog)

COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pItem(pItem) {
    (void)nScaleMin; (void)nScaleMax;
    memset(_olepropertiesdialog_padding, 0, sizeof(_olepropertiesdialog_padding));
}

COlePropertiesDialog::~COlePropertiesDialog() {
}

intptr_t COlePropertiesDialog::DoModal() {
    return IDOK;  // OLEUIOBJECTPROPSW is not fully defined in MinGW
}

//=============================================================================
// COleUpdateDialog
//=============================================================================
IMPLEMENT_DYNAMIC(COleUpdateDialog, COleDialog)

COleUpdateDialog::COleUpdateDialog(COleDocument* pDoc, BOOL bUpdateLinks,
                                    BOOL bUpdateEmbeddings, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pDoc(pDoc),
      m_bUpdateLinks(bUpdateLinks), m_bUpdateEmbeddings(bUpdateEmbeddings) {
    memset(_oleupdatedialog_padding, 0, sizeof(_oleupdatedialog_padding));
}

COleUpdateDialog::~COleUpdateDialog() {
}

intptr_t COleUpdateDialog::DoModal() {
    return IDOK;
}

//=============================================================================
// COleControl
//=============================================================================
IMPLEMENT_DYNAMIC(COleControl, CWnd)

COleControl::CControlDataSource::CControlDataSource(COleControl* pCtrl) {
    if (pCtrl) pCtrl->m_pDataSource = this;
}

// sizeof/offset guards for the retail-transcribed layout. If any of these fire,
// a member edit has silently broken binary compatibility with mfc140u.
static_assert(sizeof(COleControl) == 912, "COleControl must match retail sizeof");
static_assert(sizeof(CFontHolder) == 24, "CFontHolder must match retail sizeof");
static_assert(offsetof(COleControl, m_piidPrimary) == 232, "m_piidPrimary @232");
static_assert(offsetof(COleControl, m_ambientDispDriver) == 256, "m_ambientDispDriver @256");
static_assert(offsetof(COleControl, m_rcPos) == 288, "m_rcPos @288");
static_assert(offsetof(COleControl, m_cxExtent) == 328, "m_cxExtent @328");
static_assert(offsetof(COleControl, m_clrBackColor) == 356, "m_clrBackColor @356");
static_assert(offsetof(COleControl, m_strText) == 368, "m_strText @368");
static_assert(offsetof(COleControl, m_font) == 376, "m_font @376");
static_assert(offsetof(COleControl, m_pClientSite) == 472, "m_pClientSite @472");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_frameInfo) == 528, "m_frameInfo @528");
static_assert(offsetof(COleControl, m_xPersistStorage) == 584, "m_xPersistStorage @584");
static_assert(offsetof(COleControl, m_xEventConnPt) == 720, "m_xEventConnPt @720");
static_assert(offsetof(COleControl, m_xPropConnPt) == 816, "m_xPropConnPt @816");

COleControl::COleControl() {
    // Zero the whole retail member block, then set the members whose retail
    // initial values are not zero. Members live at fixed offsets now, so this
    // clears exactly the region a real client would inspect.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(COleControl) - sizeof(CWnd));
    m_bAutoMenuEnable = 1;
    m_bAutoClip = 1;
    m_bCountOnAmbients = 1;
    m_sAppearance = 0;
    m_bEnabled = TRUE;
    m_lReadyState = 4;              // READYSTATE_COMPLETE
    m_clrBackColor = 0x80000005;    // COLOR_WINDOW  | 0x80000000
    m_clrForeColor = 0x80000008;    // COLOR_WINDOWTEXT | 0x80000000
    new (&m_strText) CString();
    new (&m_font) CFontHolder();
    new (&m_ambientDispDriver) COleDispatchDriver();
    GetOleControlState(this, true);
}

COleControl::~COleControl() {
    if (m_pDataSource) {
        auto* source = static_cast<COleControl::CControlDataSource*>(m_pDataSource);
        m_pDataSource = nullptr;
        delete source;
    }
    RemoveOleControlState(this);
}

// Symbol: ?GetDataSource@COleControl@@IEAAPEAVCControlDataSource@1@XZ
extern "C" void* MS_ABI impl__GetDataSource_COleControl__IEAAPEAVCControlDataSource_1_XZ(
    void* p0, void* /*p1*/, void* /*p2*/, void* /*p3*/, void* /*p4*/, void* /*p5*/, void* /*p6*/,
    char /*p7*/, void* /*p8*/, void* /*p9*/, void* /*p10*/, void* /*p11*/, void* /*p12*/, void* /*p13*/,
    void* /*p14*/, void* /*p15*/, void* /*p16*/, char /*p17*/) {
    auto* control = static_cast<COleControl*>(p0);
    if (!control) return nullptr;

    if (!control->m_pDataSource) {
        return new COleControl::CControlDataSource(control);
    }

    return control->m_pDataSource;
}

// Symbol: ?OnRenderData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    FORMATETC request = MakeFormatEtc(0, p0Format);
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, request, false);
    if (!entry || !entry->hasMedium) return FALSE;

    return CopyStorageMedium(entry->medium, p1) ? TRUE : FALSE;
}

// Symbol: ?OnRenderFileData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAVCFile___Z(
    void* p0, FORMATETC* p0Format, CFile* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium) return FALSE;

    const STGMEDIUM& medium = entry->medium;
    if (medium.tymed == TYMED_HGLOBAL && medium.hGlobal) {
        SIZE_T size = GlobalSize(medium.hGlobal);
        void* data = GlobalLock(medium.hGlobal);
        if (!data) return FALSE;
        p1->Write(data, static_cast<UINT>(size));
        GlobalUnlock(medium.hGlobal);
        return TRUE;
    }

    if (medium.tymed == TYMED_FILE && medium.lpszFileName) {
        CFile sourceFile(medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (sourceFile.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096] = {};
        UINT read = 0;
        while ((read = sourceFile.Read(buffer, sizeof(buffer))) > 0) {
            p1->Write(buffer, read);
        }
        return TRUE;
    }

    return FALSE;
}

// Symbol: ?OnRenderGlobalData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAPEAX_Z(
    void* p0, FORMATETC* p0Format, void** p0Medium) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    if (!p0Format) return FALSE;
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL || !entry->medium.hGlobal) {
        *p0Medium = nullptr;
        return FALSE;
    }

    *p0Medium = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *p0Medium != nullptr;
}

// Symbol: ?OnSetData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1Medium, int /*p2*/) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, true);
    if (!entry) return FALSE;

    ReleaseDataSourceEntryMedium(*entry);
    entry->format = MakeFormatEtc(0, p0Format);
    entry->hasMedium = CopyStorageMedium(*p1Medium, &entry->medium);
    return entry->hasMedium ? TRUE : FALSE;
}

BOOL COleControl::CreateControl(REFCLSID clsid, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!pParentWnd) return FALSE;

    COleControlContainer* container = pParentWnd->GetControlContainer();
    if (!container) {
        if (!pParentWnd->CreateControlContainer(&container) || !container) return FALSE;
    }

    COleControlSite* site = container->CreateSite(container);
    if (!site) return FALSE;
    site->m_pControl = this;

    if (!site->CreateControl(this, clsid, lpszWindowName, dwStyle, rect, nID, pPersist, bStorage, bstrLicKey)) {
        site->m_pControl = nullptr;
        container->DeleteSite(site);
        return FALSE;
    }

    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = container;
        state->mfcSite = site;
    }
    m_bInitialized = 1;
    return TRUE;
}

BOOL COleControl::CreateControl(const wchar_t* lpszProgID, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(clsid, lpszWindowName, dwStyle, rect, pParentWnd, nID, pPersist, bStorage, bstrLicKey);
}

BOOL COleControl::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved;
    if (pbstrKey) {
        *pbstrKey = nullptr;
    }

    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return FALSE;
    }

    if (pbstrKey) {
        *pbstrKey = AllocateLicenseBstr(state->licenseKey);
    }
    return TRUE;
}

BSTR COleControl::GetLicenseKey(REFCLSID clsid) {
    return AllocateLicenseBstr(FindFactoryLicense(clsid));
}

void COleControl::Serialize(CArchive& ar) {
    CWnd::Serialize(ar);
}

BOOL COleControl::DoPropExchange(CPropExchange* pPX) {
    return pPX != nullptr;
}

// COleControl::GetAmbientDispatchDriver — real implementation lives in
// global_olecontrol_batch1.cpp (decoded from retail); it lazily binds the
// embedded driver to the container's IDispatch via m_pClientSite.
extern "C" COleDispatchDriver* MS_ABI
impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(COleControl* pThis);

BOOL COleControl::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    if (!pvProp) return FALSE;

    // OpenMFC's own CreateControl path records a COleControlSite in the side
    // table and that site answers ambients directly, so it stays the preferred
    // source. It is not the only way to be hosted, though: an ordinary COM
    // container calls SetClientSite and never touches the side table, and for
    // those controls every ambient lookup used to fail outright. Fall back to
    // the container's IDispatch, which is the path retail always takes.
    if (COleControlSite* pSite = MfcSiteOf(this))
        return pSite->GetAmbientProperty(dwDispid, vtProp, pvProp);

    COleDispatchDriver* pDriver =
        impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(this);
    if (pDriver == nullptr || pDriver->GetIDispatch(FALSE) == nullptr)
        return FALSE;

    // DISPATCH_PROPERTYGET == 2. A container that does not implement the
    // ambient reports failure through the driver rather than by throwing here.
    pDriver->InvokeHelper(dwDispid, 2, vtProp, pvProp, nullptr);
    return TRUE;
}

void COleControl::FireEvent(DISPID dispId, BYTE* pbParams, ...) {
    va_list args;
    va_start(args, pbParams);
    FireEventV(dispId, pbParams, args);
    va_end(args);
}

void COleControl::FireEventV(DISPID dispId, BYTE* pbParams, va_list argList) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->eventSinks.empty()) return;

    // Build DISPPARAMS from the VT-encoded parameter list
    int cParams = CountDispatchParams(pbParams);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    va_list argCopy;
    // We need a copy of argList per each sink invocation
    // Build params once; they are read-only for each Invoke call
    va_copy(argCopy, argList);
    for (int i = 0; i < cParams; ++i) {
        MakeDispatchVariant(static_cast<VARTYPE>(pbParams[i]), &argCopy, &params[static_cast<size_t>(i)]);
    }
    va_end(argCopy);

    // Reverse for DISPPARAMS (COM convention: last arg first)
    std::vector<VARIANTARG> revParams(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        revParams[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dp = {};
    dp.cArgs = static_cast<UINT>(cParams);
    dp.rgvarg = cParams ? revParams.data() : nullptr;

    // Fire to all registered sinks
    for (auto& sink : state->eventSinks) {
        if (!sink.sink) continue;
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(sink.sink->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDisp))) && pDisp) {
            pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                          DISPATCH_METHOD, &dp, nullptr, nullptr, nullptr);
            pDisp->Release();
        }
    }

    // Clean up variants (don't use VariantClear on the reversed view; clear originals)
    for (int i = 0; i < cParams; ++i) {
        params[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }
}

BOOL COleControl::IsOptimizedDraw() const {
    return m_bOptimizedDraw;
}

void COleControl::SetInitialSize(int cx, int cy) {
    SetControlSize(cx, cy);
}

void COleControl::OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid) {
    (void)rcInvalid;
    if (!pDC || !pDC->GetSafeHdc()) return;

    RECT rc = *rcBounds;
    const COLORREF backColor = m_bEnabled ? AmbientBackColor() : RGB(240, 240, 240);
    HBRUSH hBrush = ::CreateSolidBrush(backColor);
    if (hBrush) {
        ::FillRect(pDC->GetSafeHdc(), &rc, hBrush);
        ::DeleteObject(hBrush);
    } else {
        ::FillRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    }
    ::FrameRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));

    const wchar_t* text = m_strText.IsEmpty() ? L"ActiveX Control" : m_strText.GetString();
    int oldMode = ::SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);
    ::SetTextColor(pDC->GetSafeHdc(), AmbientForeColor());
    ::DrawTextW(pDC->GetSafeHdc(), text, -1, &rc,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    ::SetBkMode(pDC->GetSafeHdc(), oldMode);
}

//=============================================================================
// OLE Helper Functions (extern "C" exports)
//=============================================================================
extern "C" {

MS_ABI int impl__AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName) {
    if (!lpszFileName) return FALSE;
    ITypeLib* pTypeLib = nullptr;
    HRESULT hr = LoadTypeLib(lpszFileName, &pTypeLib);
    if (FAILED(hr)) return FALSE;
    hr = RegisterTypeLib(pTypeLib, (OLECHAR*)lpszFileName, nullptr);
    pTypeLib->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

MS_ABI int impl__AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                            const wchar_t* lpszShortTypeName,
                                            const wchar_t* lpszLongTypeName,
                                            int nAppType, const wchar_t** rglpszRegister,
                                            const wchar_t** rglpszOverwrite) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite;
    return FALSE;
}

MS_ABI int impl__AfxOleRegisterControlClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                             const wchar_t* lpszShortTypeName,
                                             const wchar_t* lpszLongTypeName,
                                             DWORD dwMiscStatus, DWORD dwFlags, DWORD dwVersion) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)dwMiscStatus; (void)dwFlags; (void)dwVersion;
    return FALSE;
}

MS_ABI int impl__AfxOleUnregisterClass(REFCLSID clsid, const wchar_t* lpszClassName) {
    (void)clsid; (void)lpszClassName;
    return TRUE;
}

MS_ABI int impl__AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                                    UINT nIDVerbMin, UINT nIDVerbMax, UINT nIDConvert) {
    (void)pItem; (void)pMenu; (void)iMenuItem; (void)nIDVerbMin; (void)nIDVerbMax; (void)nIDConvert;
    return FALSE;
}

MS_ABI int impl__AfxOleSetUserCtrl(BOOL bUserCtrl) {
    (void)bUserCtrl;
    return FALSE;
}

MS_ABI COLORREF impl__AfxOleTranslateColor(OLE_COLOR clrColor, HPALETTE hpal) {
    (void)hpal;
    // OLE_COLOR is a COLORREF for standard colors
    // ARGB -> ABGR conversion for OLE_COLOR type
    if (clrColor & 0x80000000) {
        // Palette entry, extract from hpal
        return clrColor;
    }
    return clrColor;
}

MS_ABI int impl__AfxOleUnregisterTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid) {
    HRESULT hr = UnRegisterTypeLib(guid, wVerMajor, wVerMinor, lcid, SYS_WIN32);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

} // extern "C"

//=============================================================================
// COleClientItem - additional methods
//=============================================================================
void COleClientItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
    if (ar.IsStoring()) {
        ar << m_nStatus << m_nDrawAspect << m_bInPlaceActive;
        ClientItemState* state = FindClientItemState(this);
        const BOOL modified = state ? state->modified : FALSE;
        const LONG activeVerb = state ? state->activeVerb : OLEIVERB_PRIMARY;
        ar << modified << activeVerb;
        return;
    }

    ar >> m_nStatus >> m_nDrawAspect >> m_bInPlaceActive;
    ClientItemState* state = GetClientItemState(this, true);
    if (!state) return;
    ar >> state->modified >> state->activeVerb;
}
void COleClientItem::AssertValid() const {
    ClientItemState* state = FindClientItemState(this);
    if (!state) {
        state = GetClientItemState(const_cast<COleClientItem*>(this), true);
    }
    if (state) state->item = const_cast<COleClientItem*>(this);

    if (m_nStatus != OLE_EMPTY && !m_lpObject && !m_lpLink) {
        const_cast<COleClientItem*>(this)->m_nStatus = OLE_EMPTY;
    }
}
void COleClientItem::OnShowItem() {
    if (m_nStatus == OLE_EMPTY) {
        m_nStatus = OLE_LOADED;
    }
    m_bInPlaceActive = TRUE;
}
void COleClientItem::OnOpen() {
    if (m_nStatus < OLE_OPEN) {
        m_nStatus = OLE_OPEN;
    }
    m_bInPlaceActive = TRUE;
}
void COleClientItem::OnClose() {
    if (m_nStatus == OLE_OPEN || m_nStatus == OLE_RUNNING) {
        m_nStatus = OLE_LOADED;
    }
    m_bInPlaceActive = FALSE;
}
BOOL COleClientItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow) {
    if (!pFrameWnd) return TRUE;
    pFrameWnd->RedrawWindow(nullptr, nullptr, bShow ? RDW_UPDATENOW : 0);
    return TRUE;
}
HGLOBAL COleClientItem::GetIconicMetafile() {
    ClientItemState* state = FindClientItemState(this);
    if (state && state->iconicMetafile) return DuplicateGlobalMemory(state->iconicMetafile);
    if (!m_lpObject) return nullptr;
    FORMATETC fmt = {};
    fmt.cfFormat = CF_METAFILEPICT;
    fmt.dwAspect = DVASPECT_ICON;
    fmt.lindex = -1;
    fmt.tymed = TYMED_MFPICT;
    STGMEDIUM medium = {};
    IDataObject* data = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&data))) || !data) return nullptr;
    HRESULT hr = data->GetData(&fmt, &medium);
    data->Release();
    if (FAILED(hr) || medium.tymed != TYMED_MFPICT) {
        ReleaseStgMedium(&medium);
        return nullptr;
    }
    HGLOBAL copy = DuplicateGlobalMemory(medium.hMetaFilePict);
    ReleaseStgMedium(&medium);
    return copy;
}
BOOL COleClientItem::SetIconicMetafile(HGLOBAL hMetaPict) {
    ClientItemState* state = GetClientItemState(this, true);
    if (!state) return FALSE;
    HGLOBAL copy = hMetaPict ? DuplicateGlobalMemory(hMetaPict) : nullptr;
    if (hMetaPict && !copy) return FALSE;
    if (state->iconicMetafile) GlobalFree(state->iconicMetafile);
    state->iconicMetafile = copy;
    m_nDrawAspect = hMetaPict ? DVASPECT_ICON : DVASPECT_CONTENT;
    SetModifiedFlag(TRUE);
    return TRUE;
}
HGLOBAL COleClientItem::GetMetaFile() {
    ClientItemState* state = FindClientItemState(this);
    if (state && state->contentMetafile) return DuplicateGlobalMemory(state->contentMetafile);
    if (!m_lpObject) return nullptr;
    IDataObject* data = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&data))) || !data) return nullptr;
    FORMATETC fmt = {};
    fmt.cfFormat = CF_METAFILEPICT;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_MFPICT;
    STGMEDIUM medium = {};
    HRESULT hr = data->GetData(&fmt, &medium);
    data->Release();
    if (FAILED(hr) || medium.tymed != TYMED_MFPICT) {
        ReleaseStgMedium(&medium);
        return nullptr;
    }
    HGLOBAL copy = DuplicateGlobalMemory(medium.hMetaFilePict);
    ReleaseStgMedium(&medium);
    return copy;
}
void COleClientItem::SetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) {
        state->hostName = lpszHost ? lpszHost : L"";
        state->hostObjectName = lpszHostObj ? lpszHostObj : L"";
    }

    if (m_lpObject) {
        m_lpObject->SetHostNames(lpszHost ? lpszHost : L"",
                                 lpszHostObj ? lpszHostObj : L"");
    }
}
BOOL COleClientItem::ConvertTo(REFCLSID clsidNew) {
    if (!m_lpObject) return FALSE;
    CLSID clsidOld = CLSID_NULL;
    if (FAILED(m_lpObject->GetUserClassID(&clsidOld))) return FALSE;
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::ActivateAs(REFCLSID clsidNew, REFCLSID clsidOld) {
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::Reload() {
    IPersistStorage* persist = nullptr;
    if (!m_lpObject || FAILED(m_lpObject->QueryInterface(IID_IPersistStorage, reinterpret_cast<void**>(&persist)))) {
        return FALSE;
    }
    HRESULT hr = persist->HandsOffStorage();
    persist->Release();
    return SUCCEEDED(hr);
}
BOOL COleClientItem::UpdateLink() {
    return m_lpLink && SUCCEEDED(m_lpLink->Update(nullptr));
}
BOOL COleClientItem::IsLinkUpToDate() const {
    if (!m_lpObject) return TRUE;
    return m_lpObject->IsUpToDate() == S_OK;
}
BOOL COleClientItem::CanActivate() { return m_lpObject != nullptr; }
BOOL COleClientItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
BOOL COleClientItem::IsRunning() const { return m_nStatus == OLE_RUNNING; }
HRESULT COleClientItem::EnumVerbs(IEnumOLEVERB** ppEnumOleVerb) {
    if (!ppEnumOleVerb) return E_POINTER;
    *ppEnumOleVerb = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;
    CLSID clsid = CLSID_NULL;
    HRESULT hr = m_lpObject->GetUserClassID(&clsid);
    if (FAILED(hr)) return hr;
    return OleRegEnumVerbs(clsid, ppEnumOleVerb);
}
LONG COleClientItem::GetActiveVerb() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->activeVerb : OLEIVERB_PRIMARY;
}

void COleClientItem::SetActiveVerb(LONG nVerb) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
}

BOOL COleClientItem::IsModified() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->modified : FALSE;
}

void COleClientItem::SetModifiedFlag(BOOL bModified) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->modified = bModified;
}
void COleClientItem::AttachDataObject(COleDataObject& dataObject) const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), true);
    if (!state) return;

    if (state->attachedDataObject) {
        state->attachedDataObject->Release();
        state->attachedDataObject = nullptr;
    }

    state->attachedDataObject = dataObject.GetIDataObject(TRUE);
}

//=============================================================================
// COleControl - additional methods
//=============================================================================
BOOL COleControl::VerifyUserLicense() { return TRUE; }
BOOL COleControl::VerifyLicenseKey(BSTR bstrKey) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return TRUE;
    }
    CString key = bstrKey ? bstrKey : L"";
    return key == state->licenseKey;
}
BOOL COleControl::SetLicenseKey(const wchar_t* lpszLicenseKey) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return FALSE;
    state->licenseKey = lpszLicenseKey ? lpszLicenseKey : L"";
    return TRUE;
}
void COleControl::DoDataExchange(void* pDX) { (void)pDX; }
void COleControl::OnResetState() {
    OleControlState* state = GetOleControlState(this, true);
    if (state) state->text.Empty();
    m_strText.Empty();
    // The properties below now live in the ABI-visible members, so the reset
    // has to clear those rather than the side-table shadow a client cannot
    // see. Values match what the retail constructor establishes.
    m_clrBackColor = 0x80000005;   // COLOR_WINDOW     | 0x80000000
    m_clrForeColor = 0x80000008;   // COLOR_WINDOWTEXT | 0x80000000
    m_bEnabled     = TRUE;
    m_sAppearance  = 0;
    m_sBorderStyle = 0;
    m_lReadyState  = 4;            // READYSTATE_COMPLETE
    // The side table is not the ABI-visible copy: IsModified() reads the
    // m_bModified bit at this+0x160, so the reset has to clear that too or a
    // freshly reset control still reports itself dirty to a real client.
    m_bModified = 0;
    m_cxExtent = 0;
    m_cyExtent = 0;
}
// COleControl::GetControlFlags() — retail is `mov eax,0x2 ; ret`: the base
// implementation returns clipPaintDC and nothing else, unconditionally.
// Derived controls override it to add fastBeginPaint / pointerInactive /
// windowlessActivate / canOptimizeDraw. The previous version here computed a
// flag word from m_bOptimizedDraw, m_hWnd and the control site, which meant a
// plain COleControl reported capabilities (and, with no window, omitted
// clipPaintDC) that the real base class never reports.
DWORD COleControl::GetControlFlags() {
    return 0x00000002;   // clipPaintDC
}
// m_cxExtent / m_cyExtent are the control's width and height in HIMETRIC, per
// the retail layout — one scalar each, not a pair of CSize.
BOOL COleControl::OnSetExtent(DVASPECT dwDrawAspect, const SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    m_cxExtent = size.cx;
    m_cyExtent = size.cy;
    return TRUE;
}
BOOL COleControl::OnGetExtent(DVASPECT dwDrawAspect, SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    size.cx = m_cxExtent;
    size.cy = m_cyExtent;
    return TRUE;
}
BOOL COleControl::OnMapPropertyToPage(DISPID dispid, CLSID* pclsid, BOOL* pbPageOptional) {
    if (!pclsid) return FALSE;
    *pclsid = CLSID_NULL;
    if (pbPageOptional) *pbPageOptional = FALSE;

    IPerPropertyBrowsing* browsing = QueryOleControlInterface<IPerPropertyBrowsing>(this, IID_IPerPropertyBrowsing);
    if (!browsing) return FALSE;

    CLSID clsid = CLSID_NULL;
    HRESULT hr = browsing->MapPropertyToPage(dispid, &clsid);
    browsing->Release();
    if (FAILED(hr) || IsEqualCLSID(clsid, CLSID_NULL)) return FALSE;

    *pclsid = clsid;
    return TRUE;
}

COLORREF COleControl::AmbientBackColor() { COLORREF cr = RGB(255,255,255); GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientForeColor() { COLORREF cr = RGB(0,0,0); GetAmbientProperty(DISPID_AMBIENT_FORECOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientAppearance() { short appearance = 0; GetAmbientProperty(DISPID_AMBIENT_APPEARANCE, VT_I2, &appearance); return static_cast<COLORREF>(appearance); }
OLE_COLOR COleControl::AmbientBackColorOle() { return (OLE_COLOR)AmbientBackColor(); }
OLE_COLOR COleControl::AmbientForeColorOle() { return (OLE_COLOR)AmbientForeColor(); }
IFontDisp* COleControl::AmbientFont() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return (IFontDisp*)p; }
IDispatch* COleControl::AmbientFontDisp() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return p; }
short COleControl::AmbientTextAlign() {
    short align = 0;
    GetAmbientProperty(DISPID_AMBIENT_TEXTALIGN, VT_I2, &align);
    return align;
}
BOOL COleControl::AmbientUserMode() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_USERMODE, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientUIDead() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_UIDEAD, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowGrabHandles() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowHatching() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, VT_BOOL, &b); return b; }
CString COleControl::AmbientDisplayName() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_DISPLAYNAME, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
BOOL COleControl::AmbientDisplayAsDefault() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_DISPLAYASDEFAULT, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientAutoClip() { return TRUE; }
BOOL COleControl::AmbientSupportsMnemonics() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SUPPORTSMNEMONICS, VT_BOOL, &b); return b; }
CString COleControl::AmbientScaleUnits() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
unsigned long COleControl::AmbientLocaleID() { unsigned long locale = static_cast<unsigned long>(::GetUserDefaultLCID()); GetAmbientProperty(DISPID_AMBIENT_LOCALEID, VT_I4, &locale); return locale; }

void COleControl::FireClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_CLICK, noParams);
}
void COleControl::FireDblClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_DBLCLICK, noParams);
}
void COleControl::FireKeyDown(USHORT* pnChar, short nShiftState) {
    // VT_PI2 (pointer to I2) = VT_BYREF|VT_I2 = 0x4002; MFC uses 0x4002 for USHORT*
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYDOWN, params, pnChar, nShiftState);
}
void COleControl::FireKeyPress(USHORT* pnChar) {
    BYTE params[2] = { static_cast<BYTE>(VT_BYREF | VT_I2), 0 };
    FireEvent(DISPID_KEYPRESS, params, pnChar);
}
void COleControl::FireKeyUp(USHORT* pnChar, short nShiftState) {
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYUP, params, pnChar, nShiftState);
}
void COleControl::FireMouseDown(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEDOWN, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseMove(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEMOVE, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseUp(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEUP, params, nButton, nShiftState, x, y);
}
// Fires the notification only. The ready state itself is owned by whoever
// transitions it (the constructor, or a derived control moving through
// LOADING/INTERACTIVE); stamping COMPLETE here would clobber an intermediate
// state the moment the container was told about it.
void COleControl::FireReadyStateChange() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_READYSTATECHANGE, noParams);
}

// Retail reads these straight out of the object: GetBackColor is
// `mov eax,[rcx+0x164]`. They used to be served from the OleControlState side
// table, which a real client cannot see -- so the constructor's
// m_clrBackColor (0x80000005) and the value this returned disagreed. Now the
// ABI-visible member is the single source of truth.
COLORREF COleControl::GetBackColor() const {
    return static_cast<COLORREF>(m_clrBackColor);
}
void COleControl::SetBackColor(COLORREF clr) {
    if (m_clrBackColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrBackColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
// Retail: `mov eax,[rcx+0x168]`.
COLORREF COleControl::GetForeColor() const {
    return static_cast<COLORREF>(m_clrForeColor);
}
void COleControl::SetForeColor(COLORREF clr) {
    if (m_clrForeColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrForeColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
// Retail: `mov eax,[rcx+0x19c]`.
BOOL COleControl::GetEnabled() const {
    return m_bEnabled;
}
void COleControl::SetEnabled(BOOL bEnabled) {
    if (m_bEnabled == bEnabled) return;
    m_bEnabled = bEnabled;
    if (m_hWnd) ::EnableWindow(m_hWnd, bEnabled);
    SetModifiedFlag(TRUE);
    // A windowless control never sees ::EnableWindow, so this is its only
    // trigger to repaint in the grayed state.
    InvalidateControl();
}
void COleControl::SetFont(LPFONTDISP pFontDisp) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font, pFontDisp);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
void COleControl::SetFont(CFont* pFont) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font,
                                                  pFont ? pFont->GetSafeHandle() : nullptr);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
// COleControl::GetHwnd() — retail gates the handle on two flag bits:
//     test DWORD PTR [rcx+0x160],0x2400 ; jne take_handle
//     xor eax,eax ; ret                 ; otherwise report no window
//   take_handle: mov rax,[rcx+0x40] ; ret
// 0x2400 is bit 10 (m_bInPlaceActive) | bit 13 (m_bOpen), both named in this
// header, so the test is spelled with the members rather than a raw word read.
// A control that has an HWND but is not activated reports 0 to its container,
// which is what an OLE host uses to decide whether the control can be talked
// to as a window at all. Returning the raw handle unconditionally, as this
// did, misreports an inactive control.
unsigned int COleControl::GetHwnd() {
    if (!m_bInPlaceActive && !m_bOpen) return 0;
    return (unsigned int)(uintptr_t)m_hWnd;
}
void COleControl::SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
OLE_COLOR COleControl::GetBackColorOle() const { return (OLE_COLOR)GetBackColor(); }
OLE_COLOR COleControl::GetForeColorOle() const { return (OLE_COLOR)GetForeColor(); }
void COleControl::SetBackColorOle(OLE_COLOR clr) { SetBackColor((COLORREF)clr); }
void COleControl::SetForeColorOle(OLE_COLOR clr) { SetForeColor((COLORREF)clr); }
// Retail: `movzx eax,WORD PTR [rcx+0x198]` -- a 16-bit read, so the stored
// value is returned unwidened.
short COleControl::GetAppearance() const {
    return m_sAppearance;
}
void COleControl::SetAppearance(short nAppearance) {
    if (m_sAppearance == nAppearance) return;
    m_sAppearance = nAppearance;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
// Retail: `movzx eax,WORD PTR [rcx+0x19a]`.
short COleControl::GetBorderStyle() const {
    return m_sBorderStyle;
}
void COleControl::SetBorderStyle(short nBorderStyle) {
    if (m_sBorderStyle == nBorderStyle) return;
    m_sBorderStyle = nBorderStyle;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
wchar_t* COleControl::GetText() {
    return const_cast<wchar_t*>(m_strText.GetString());
}
const CString& COleControl::InternalGetText() { return m_strText; }
void COleControl::SetText(const wchar_t* lpszText) {
    CString newText = lpszText ? lpszText : L"";
    if (m_strText == newText) return;
    m_strText = newText;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->text = newText;
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
void COleControl::GetText(CString& strText) const { strText = m_strText; }
// Retail: `mov eax,[rcx+0x1a0]`.
long COleControl::GetReadyState() const {
    return m_lReadyState;
}

BOOL COleControl::IsSubclassedControl() {
    return m_hWnd != nullptr && MfcSiteOf(this) == nullptr;
}
// Retail (?SetModifiedFlag@COleControl@@QEAAXH@Z):
//     mov   eax,[rcx+0x160]      ; movzx r8d,dl   ; shl r8d,2
//     xor   r8d,eax              ; and   r8d,4    ; xor r8d,eax
//     mov   [rcx+0x160],r8d      ; ret
// The MSVC idiom for a 1-bit bitfield store: bit 2 is set from bit 0 of the
// LOW BYTE of the argument. So the value is truncated, not normalized --
// SetModifiedFlag(2) clears the flag, and a plain assignment to the bitfield
// reproduces that exactly. Do not "fix" this into `bModified ? 1 : 0`.
//
// This used to write only the OleControlState side table, which no client can
// see, while IsModified() read the m_bModified bit -- so a control marked
// dirty by any property setter still reported itself clean. Same defect class
// as the accessor migration in ca63d6a, which corrected OnResetState but left
// the setter behind.
void COleControl::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified;
}
BOOL COleControl::GetModifiedFlag() const {
    return m_bModified;
}
ULONG COleControl::InternalAddRef() { return 1; }
ULONG COleControl::InternalRelease() { return 1; }
ULONG COleControl::InternalQueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        return MfcSiteOf(this)->m_lpObject->QueryInterface(riid, ppv);
    }
    return E_NOINTERFACE;
}
void COleControl::GetControlSize(int* pCX, int* pCY) {
    if (pCX) *pCX = m_cxExtent;
    if (pCY) *pCY = m_cyExtent;
}
BOOL COleControl::SetControlSize(int cx, int cy) {
    SIZE size = { cx, cy };
    if (OnSetExtent(DVASPECT_CONTENT, size)) {
        SetModifiedFlag(TRUE);
        return TRUE;
    }
    return FALSE;
}
void COleControl::OnSetClientSite() {
    COleControlSite* site = MfcSiteOf(this);
    m_bInitialized = (site != nullptr) ? 1u : 0u;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = site ? site->GetContainer() : nullptr;
    }
}
void COleControl::OnGetControlInfo(LPCONTROLINFO pControlInfo) {
    if (!pControlInfo) return;
    memset(pControlInfo, 0, sizeof(*pControlInfo));
    pControlInfo->cb = sizeof(*pControlInfo);

    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (control) {
        CONTROLINFO info = {};
        info.cb = sizeof(info);
        if (SUCCEEDED(control->GetControlInfo(&info))) {
            *pControlInfo = info;
        }
        control->Release();
    }
}

BOOL COleControl::OnMnemonic(LPMSG pMsg) {
    if (!pMsg) return FALSE;
    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (!control) return FALSE;
    HRESULT hr = control->OnMnemonic(pMsg);
    control->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnAmbientPropertyChange(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    if (dispid == DISPID_AMBIENT_BACKCOLOR || dispid == DISPID_UNKNOWN) {
        state->backColor = AmbientBackColor();
        InvalidateControl();
    }
    if (dispid == DISPID_AMBIENT_FORECOLOR || dispid == DISPID_UNKNOWN) {
        state->foreColor = AmbientForeColor();
        InvalidateControl();
    }
}
void COleControl::BoundPropertyChanged(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (state) {
        for (auto* sink : state->propSinks) {
            if (sink) sink->OnChanged(dispid);
        }
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
BOOL COleControl::BoundPropertyRequestEdit(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return TRUE;
    for (auto* sink : state->propSinks) {
        if (sink && FAILED(sink->OnRequestEdit(dispid))) return FALSE;
    }
    return TRUE;
}
void COleControl::InvalidateControl(LPCRECT lpRect, BOOL bErase) {
    if (m_hWnd) ::InvalidateRect(m_hWnd, lpRect, bErase);
}
int COleControl::OnProperties(MSG* pMsg, HWND hWnd, const RECT* lpRect) {
    (void)pMsg; (void)lpRect;
    if (COleControlSite* site = MfcSiteOf(this)) {
        site->ShowPropertyFrame();
        return TRUE;
    }
    HWND oldWnd = m_hWnd;
    if (!m_hWnd && hWnd) m_hWnd = hWnd;
    int count = GetPropertyPageCount();
    if (count > 0) ShowPropertyPages();
    if (!oldWnd && hWnd) m_hWnd = oldWnd;
    return count > 0 ? TRUE : FALSE;
}
void COleControl::ShowPropertyPages() {
    if (!m_hWnd) return;
    ISpecifyPropertyPages* pSPP = nullptr;
    if (FAILED(InternalQueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pSPP))) || !pSPP)
        return;
    CAUUID pages = {};
    if (SUCCEEDED(pSPP->GetPages(&pages)) && pages.cElems > 0) {
        IUnknown* pUnk = nullptr;
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
        OleCreatePropertyFrame(m_hWnd, 0, 0, nullptr,
                               pUnk ? 1u : 0u, pUnk ? &pUnk : nullptr,
                               pages.cElems, pages.pElems,
                               LOCALE_USER_DEFAULT, 0, nullptr);
        if (pUnk) pUnk->Release();
        CoTaskMemFree(pages.pElems);
    }
    pSPP->Release();
}
int COleControl::GetPropertyPageCount() const {
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        const_cast<COleControl*>(this)->InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return 0;

    ISpecifyPropertyPages* pages = nullptr;
    HRESULT hr = unknown->QueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pages));
    unknown->Release();
    if (FAILED(hr) || !pages) return 0;
    CAUUID cauuid = {};
    int count = 0;
    if (SUCCEEDED(pages->GetPages(&cauuid))) {
        count = static_cast<int>(cauuid.cElems);
        CoTaskMemFree(cauuid.pElems);
    }
    pages->Release();
    return count;
}
BOOL COleControl::IsPropertyPage(LPUNKNOWN lpUnk) {
    if (!lpUnk) return FALSE;
    IPropertyPage* page = nullptr;
    HRESULT hr = lpUnk->QueryInterface(IID_IPropertyPage, reinterpret_cast<void**>(&page));
    if (page) page->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
BOOL COleControl::CanCreateConnectionPoints() { return TRUE; }
void COleControl::EnableConnectionPoints() {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    IConnectionPointContainer* container = nullptr;
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return;
    HRESULT hr = unknown->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));
    unknown->Release();
    if (FAILED(hr) || !container) return;

    IEnumConnectionPoints* enumPoints = nullptr;
    if (SUCCEEDED(container->EnumConnectionPoints(&enumPoints)) && enumPoints) {
        IConnectionPoint* point = nullptr;
        ULONG fetched = 0;
        while (enumPoints->Next(1, &point, &fetched) == S_OK && fetched == 1) {
            IID iid = IID_NULL;
            if (SUCCEEDED(point->GetConnectionInterface(&iid))) {
                bool exists = std::any_of(state->enabledConnectionPoints.begin(), state->enabledConnectionPoints.end(),
                    [&iid](const IID& value) { return IsEqualIID(value, iid); });
                if (!exists) state->enabledConnectionPoints.push_back(iid);
            }
            point->Release();
        }
        enumPoints->Release();
    }
    container->Release();
}
BOOL COleControl::IsConnectionPointEnabled(REFIID riid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return FALSE;
    for (const IID& iid : state->enabledConnectionPoints) {
        if (IsEqualIID(iid, riid)) return TRUE;
    }
    for (auto& sink : state->eventSinks) {
        if (IsEqualIID(sink.iid, riid)) return TRUE;
    }
    return FALSE;
}
void COleControl::FirePropChanged(DISPID dispid) {
    BoundPropertyChanged(dispid);
}
BOOL COleControl::PreTranslateMessage(MSG* pMsg) {
    if (!pMsg) return FALSE;
    IOleInPlaceActiveObject* activeObject = QueryOleControlInterface<IOleInPlaceActiveObject>(this, IID_IOleInPlaceActiveObject);
    if (!activeObject) return FALSE;
    HRESULT hr = activeObject->TranslateAccelerator(pMsg);
    activeObject->Release();
    return hr == S_OK ? TRUE : FALSE;
}

LONG COleControl::OnPosRectChange(LPCRECT lprcPosRect) {
    if (!lprcPosRect) return E_POINTER;
    RECT clipRect = *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state && state->hasObjectRects) {
        clipRect = state->clipRect;
    }
    return OnSetObjectRects(lprcPosRect, &clipRect) ? S_OK : E_FAIL;
}

BOOL COleControl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect) {
    if (!lprcPosRect) return FALSE;
    RECT clipRect = lprcClipRect ? *lprcClipRect : *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state) {
        state->posRect = *lprcPosRect;
        state->clipRect = clipRect;
        state->hasObjectRects = TRUE;
    }
    COleControlSite* ipSite = MfcSiteOf(this);
    if (ipSite && ipSite->m_lpInPlaceObject &&
        SUCCEEDED(ipSite->m_lpInPlaceObject->SetObjectRects(lprcPosRect, &clipRect))) {
        return TRUE;
    }
    IOleInPlaceObject* inPlace = QueryOleControlInterface<IOleInPlaceObject>(this, IID_IOleInPlaceObject);
    if (!inPlace) return FALSE;
    HRESULT hr = inPlace->SetObjectRects(lprcPosRect, &clipRect);
    inPlace->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnClose(DWORD dwSaveOption) {
    m_bOpen = FALSE;
    m_bInPlaceActive = FALSE;
    if (dwSaveOption == OLECLOSE_NOSAVE) {
        SetModifiedFlag(FALSE);
    }
}
CWnd* COleControl::SetCapture() {
    HWND previous = m_hWnd ? ::SetCapture(m_hWnd) : nullptr;
    return previous ? CWnd::FromHandle(previous) : nullptr;
}
BOOL COleControl::ReleaseCapture() { return ::ReleaseCapture(); }
void COleControl::BringWindowToTop() { if(m_hWnd) ::BringWindowToTop(m_hWnd); }
void COleControl::MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { if(m_hWnd) ::MoveWindow(m_hWnd, X, Y, nWidth, nHeight, bRepaint); }
void COleControl::MoveWindow(LPCRECT lpRect, BOOL bRepaint) { if(m_hWnd && lpRect) ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint); }

//=============================================================================
// COleDocObjectItem
//=============================================================================
IMPLEMENT_DYNAMIC(COleDocObjectItem, COleClientItem)

COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc) : COleClientItem(pContainerDoc) { memset(_coledocobjectitem_padding, 0, sizeof(_coledocobjectitem_padding)); }
COleDocObjectItem::~COleDocObjectItem() {}
BOOL COleDocObjectItem::IsDocObject() const {
    if (!m_lpObject) return FALSE;
    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (document) document->Release();
    return SUCCEEDED(hr);
}
BOOL COleDocObjectItem::IsActive() const { return m_bInPlaceActive && IsOpen(); }
IOleDocumentView* COleDocObjectItem::GetActiveView() const {
    if (!m_lpObject) return nullptr;
    IOleDocumentView* view = nullptr;
    if (m_lpInPlaceObject &&
        SUCCEEDED(m_lpInPlaceObject->QueryInterface(IID_IOleDocumentView, reinterpret_cast<void**>(&view)))) {
        return view;
    }

    IOleDocument* document = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document))) && document) {
        if (SUCCEEDED(document->CreateView(nullptr, nullptr, 0, &view)) && view) {
            InitializeDocumentView(const_cast<COleDocObjectItem*>(this), view);
        }
        document->Release();
    }
    return view;
}
HRESULT COleDocObjectItem::GetDocument(IUnknown** ppDocument) {
    if (!ppDocument) return E_POINTER;
    *ppDocument = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (SUCCEEDED(hr) && document) {
        *ppDocument = static_cast<IUnknown*>(document);
        return S_OK;
    }
    return hr;
}
void COleDocObjectItem::ActivateAndShow() {
    if (!m_lpObject) return;
    Activate(OLEIVERB_SHOW);
    IOleDocumentView* view = GetActiveView();
    if (view) {
        view->Show(TRUE);
        view->Release();
    }
}
BOOL COleDocObjectItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
void COleDocObjectItem::OnActivateView() {
    if (!m_lpObject) return;

    if (!IsOpen()) {
        OnOpen();
        if (m_nStatus == OLE_OPEN) {
            Activate(OLEIVERB_OPEN, nullptr, nullptr, nullptr, nullptr, FALSE);
        }
    }

    IOleDocumentView* view = GetActiveView();
    if (!view) return;
    view->UIActivate(TRUE);
    if (m_lpInPlaceObject && !m_bInPlaceActive) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_OPEN;
    view->Show(TRUE);
    view->Release();
}
BOOL COleDocObjectItem::OnPreparePrinting(void* pInfo) { (void)pInfo; return TRUE; }
void COleDocObjectItem::OnBeginPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnPrint(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnEndPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
HRESULT COleDocObjectItem::ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt, const GUID* pguidCmdGroup) {
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleCommandTarget* commandTarget = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleCommandTarget, reinterpret_cast<void**>(&commandTarget));
    if (FAILED(hr) || !commandTarget) return hr == E_NOINTERFACE ? OLECMDERR_E_NOTSUPPORTED : hr;

    hr = commandTarget->Exec(pguidCmdGroup, nCmdID, nCmdExecOpt, nullptr, nullptr);
    commandTarget->Release();
    return hr;
}

//=============================================================================
// CEnumFormatEtc
//=============================================================================
CEnumFormatEtc::CEnumFormatEtc() : m_refCount(1), m_formats(nullptr), m_count(0), m_capacity(0), m_position(0) { memset(_cenumformatetc_padding, 0, sizeof(_cenumformatetc_padding)); }
CEnumFormatEtc::~CEnumFormatEtc() { if (m_formats) free(m_formats); }

STDMETHODIMP CEnumFormatEtc::QueryInterface(REFIID riid, void** ppv) {
    if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) { *ppv = this; AddRef(); return S_OK; }
    *ppv = nullptr; return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef() { return ++m_refCount; }
STDMETHODIMP_(ULONG) CEnumFormatEtc::Release() {
    ULONG ref = --m_refCount;
    if (ref == 0) delete this;
    return ref;
}
STDMETHODIMP CEnumFormatEtc::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) {
    ULONG fetched = 0;
    while (m_position < m_count && fetched < celt) {
        rgelt[fetched] = m_formats[m_position];
        if (rgelt[fetched].ptd) rgelt[fetched].ptd = nullptr; // Don't copy DVTARGETDEVICE
        m_position++;
        fetched++;
    }
    if (pceltFetched) *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
}
STDMETHODIMP CEnumFormatEtc::Skip(ULONG celt) {
    if (m_position + celt > m_count) { m_position = m_count; return S_FALSE; }
    m_position += celt;
    return S_OK;
}
STDMETHODIMP CEnumFormatEtc::Reset() { m_position = 0; return S_OK; }
STDMETHODIMP CEnumFormatEtc::Clone(IEnumFORMATETC** ppEnum) {
    if (!ppEnum) return E_POINTER;
    CEnumFormatEtc* clone = new CEnumFormatEtc();
    for (ULONG i = 0; i < m_count; ++i) {
        clone->AddFormat(m_formats[i]);
    }
    clone->m_position = m_position;
    *ppEnum = clone;
    return S_OK;
}

void CEnumFormatEtc::AddFormat(const FORMATETC& formatEtc) {
    if (m_count >= m_capacity) {
        m_capacity = m_capacity ? m_capacity * 2 : 8;
        m_formats = (FORMATETC*)realloc(m_formats, m_capacity * sizeof(FORMATETC));
    }
    if (m_formats) m_formats[m_count++] = formatEtc;
}

//=============================================================================
// COleDocument OnUpdate* command-UI handlers
//=============================================================================

// Symbol: ?OnUpdateEditChangeIcon@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditChangeIcon_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateEditLinksMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditLinksMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateObjectVerbMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateObjectVerbPopup@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbPopup_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdatePasteLinkMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteLinkMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdatePasteMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

//=============================================================================
// COleServerDoc OnUpdate* command-UI handlers
//=============================================================================

// Symbol: ?OnUpdateFileExit@COleServerDoc@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileExit_COleServerDoc__IEAAXPEAVCCmdUI___Z(COleServerDoc*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateFileUpdate@COleServerDoc@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileUpdate_COleServerDoc__IEAAXPEAVCCmdUI___Z(COleServerDoc*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?UpdateUsingHostObj@COleServerDoc@@IEAAXIPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__UpdateUsingHostObj_COleServerDoc__IEAAXIPEAVCCmdUI___Z(COleServerDoc*, unsigned int nID, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
    (void)nID;
}

//=============================================================================
// COleIPFrameWnd OnUpdate* command-UI handler
//=============================================================================

// Symbol: ?OnUpdateControlBarMenu@COleIPFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateControlBarMenu_COleIPFrameWnd__IEAAXPEAVCCmdUI___Z(COleIPFrameWnd*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

//=============================================================================
// COleDBRecordView OnUpdate* handlers (no header class yet)
//=============================================================================

// Symbol: ?OnUpdateRecordFirst@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateRecordLast@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateRecordNext@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdateRecordPrev@COleDBRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_COleDBRecordView__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

//=============================================================================
// Frame Window Ex OnUpdate* handlers (no header class yet)
//=============================================================================

// Symbol: ?OnUpdatePaneMenu@COleDocIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleDocIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

// Symbol: ?OnUpdatePaneMenu@COleIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}

//=============================================================================
// CArray<COleVariant> template helpers
//=============================================================================

// Symbol: ??$CopyElements@VCOleVariant@@@@YAXPEAVCOleVariant@@PEBV0@_J@Z
extern "C" void MS_ABI impl____CopyElements_VCOleVariant____YAXPEAVCOleVariant__PEBV0__J_Z(
    COleVariant* pDest, const COleVariant* pSrc, __int64 nCount) {
    while (nCount--) {
        impl___4COleVariant__QEAAAEBV0_AEBV0__Z(pDest, pSrc);
        ++pDest; ++pSrc;
    }
}

// Symbol: ??$SerializeElements@VCOleVariant@@@@YAXAEAVCArchive@@PEAVCOleVariant@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_VCOleVariant____YAXAEAVCArchive__PEAVCOleVariant___J_Z(
    CArchive* ar, COleVariant* pElements, __int64 nCount) {
    if (!ar || !pElements) return;
    if (ar->IsStoring())
        for (__int64 i = 0; i < nCount; ++i)
            impl___6_YAAEAVCArchive__AEAV0_VCOleVariant___Z(ar, reinterpret_cast<const VARIANT*>(&pElements[i]));
    else
        for (__int64 i = 0; i < nCount; ++i)
            impl___5_YAAEAVCArchive__AEAV0_AEAVCOleVariant___Z(ar, &pElements[i]);
}
