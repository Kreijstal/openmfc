// CMFCRibbonPanelMenuBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Object model (read before touching any body)
// ---------------------------------------------------------------------------
// OpenMFC has no C++ declaration of CMFCRibbonPanelMenuBar; the only trace of
// the class in include/openmfc/ is the CRuntimeClass descriptor
// (featurepack/menu/RuntimeClasses.cpp: MFC_TOOLBARS_DESC(CMFCRibbonPanelMenuBar,
// 5520, ...)), whose object size 5520 == 0x1590 is the retail size.  The bodies
// below therefore follow the standing convention of featurepack/menu/
// CMFCPopupMenu.cpp and CMFCPopupMenuBar.cpp: they address the RETAIL layout on
// the assumption that the caller's object is retail-sized (an MSVC client
// allocates sizeof(CMFCRibbonPanelMenuBar) == 0x1590 from the real headers and
// runs one of the five exported constructors below, which construct the
// CMFCPopupMenuBar subobject through its OpenMFC thunk and then initialise the
// own members through CommonInit).
//
// Own-member layout, in the declaration order of the on-host MSVC 14.51
// afxribbonpanelmenu.h, starting where the 0x1420-byte retail CMFCPopupMenuBar
// ends (featurepack/menu/CMFCPopupMenuBar.cpp pins that size).  Every offset
// was read out of the disassembled retail bodies (mfc140u.dll; the RVA is
// cited at each function):
//
//   +0x1420  BOOL   m_bTracked              (TrackMouseEvent armed)
//   +0x1424  BOOL   m_bSimpleMode           (AddButtons stores 1)
//   +0x1428  BOOL   m_bIsMenuMode
//   +0x142c  BOOL   m_bIsDefaultMenuLook
//   +0x1430  BOOL   m_bIsQATPopup
//   +0x1434  BOOL   m_bCustomizeMenu        (CommonInit stores 1)
//   +0x1438  BOOL   m_bIsFloaty
//   +0x143c  BOOL   m_bHasKeyTips
//   +0x1440  BOOL   m_bSetKeyTips
//   +0x1444  BOOL   m_bAutoCommandTimer
//   +0x1448  CSize  m_sizePrefered
//   +0x1450  CSize  m_sizeCategory
//   +0x1458  CRect  m_rectAutoCommand
//   +0x1468  CPoint m_ptStartMenu           (CommonInit stores (-1,-1))
//   +0x1470  CScrollBar m_wndScrollBarVert  (embedded, 0xe8 bytes; m_hWnd at +0x14b0)
//   +0x1558  CMFCRibbonBar*         m_pRibbonBar
//   +0x1560  CMFCRibbonPanel*       m_pPanel            (owned)
//   +0x1568  CMFCRibbonPanel*       m_pPanelOrigin      (the ctor argument)
//   +0x1570  CMFCRibbonButton*      m_pDelayedCloseButton
//   +0x1578  CMFCRibbonButton*      m_pDelayedButton
//   +0x1580  CMFCRibbonCategory*    m_pCategory         (owned)
//   +0x1588  CMFCRibbonBaseElement* m_pPressed
//   sizeof   0x1590 (5520)
//
// Inherited members the bodies use (all declared by OpenMFC at the retail
// offsets, pinned by the static_asserts below): CWnd::m_hWnd (+0x40),
// CBasePane::m_AccData (+0x150), CMFCToolBar::m_bInUpdateShadow (+0x111c),
// CMFCToolBar::m_iAccHotItem (+0x1154), CMFCToolBar::m_pToolTip (+0x12e8);
// and, past OpenMFC's 0x13b0-byte CMFCPopupMenuBar, the retail
// CMFCPopupMenuBar members m_bDisableSideBarInXPMode (+0x1350), m_iOffset
// (+0x1364) and m_bResizeTracking (+0x13a4) (offsets from CMFCPopupMenuBar.cpp).
//
// Timer ids: 0xec1b (delayed open of m_pDelayedButton), 0xec1c (delayed close
// of m_pDelayedCloseButton), 0xec1d (auto-repeat / auto-command), 0xec07
// (CMFCToolBar accessibility hot-item timer).
//
// Vtable slots named below were resolved against the mfc140u vftables: this
// class 0x30a748 (dumped in full), CMFCRibbonPanel 0x309390, CMFCRibbonCategory
// 0x3014c8, CMFCRibbonBaseElement 0x2dc5c0, CMFCRibbonBar 0x2fee78,
// CMFCVisualManager 0x31c128 (read out of ??0CMFCVisualManager@@QEAA@H@Z), and
// cross-checked against the declaration order of the on-host MSVC 14.51
// afxribbonpanel.h / afxribboncategory.h / afxbaseribbonelement.h /
// afxribbonbutton.h / afxribbonpalettegallery.h.  Own-class slots: +0x228
// PreTranslateMessage, +0x498 OnUpdateCmdUI, +0x4b8 OnSetAccData, +0x810 OnKey,
// +0x890 CloseDelayedSubMenu, +0x898 RestoreDelayedSubMenu, +0x8a0
// IsRibbonPanel, +0x8a8 IsRibbonPanelInRegularMode, +0x8b0 OnClickButton,
// +0x8b8 OnChangeHighlighted, +0x8c0 OnDrawMenuBorder.  The message-map
// handlers were located through the AFX_MSGMAP at 0x30a568 (mfc140u).
//
// What is NOT modeled, and therefore where the bodies deviate from retail
// (each deviation is also called out at the function):
//   * The clean-room CMFCRibbonPanel / CMFCRibbonCategory / CMFCRibbonBaseElement
//     are small side-table objects (afxmfc.h), so retail's direct reads and
//     writes of their members (panel m_bMenuMode/+0xb8, +0xbc, +0xc0, +0xc4,
//     m_bFloatyMode/+0xc0, m_nXMargin/+0xf0, +0xf4, +0xd4, m_nScrollOffset/+0xf8,
//     m_rect/+0x10c, m_pScrollBar/+0x150, m_pParent/+0x158, m_pParentMenuBar/
//     +0x160, m_pPaletteButton/+0x168, m_btnLaunch/+0x170, m_pHighlighted/
//     +0x3e8, m_btnDefault/+0x3f0, m_arWidths +0x6b0/+0x6b8 (data/count),
//     m_arElements +0x6d8/+0x6e0; category m_rect/+0xc0, m_pParentRibbonBar/
//     +0x760, m_pParentMenuBar/+0x768, m_arPanels count +0x7b0; element
//     m_rect/+0xc8, m_bCompactMode/+0x12c, m_bFloatyMode/+0x134,
//     m_bIsHighlighted/+0x13c, m_bIsPressed/+0x144, button m_rectMenu/+0x198,
//     m_bToBeClosed/+0x20c -- all offsets checked against the declaration
//     order of the on-host MSVC 14.51 afxribbonpanel.h / afxribboncategory.h /
//     afxbaseribbonelement.h / afxribbonbutton.h) are skipped.  Where retail
//     branches on such a field the field is treated as holding its
//     zero-initialised value, and the comment says so.  The panel->category
//     and category->bar links are recovered from the ribbon_state side tables
//     instead (PanelParentCategory / CategoryParentBar below, the same lookup
//     CMFCRibbonPanel.cpp uses).
//   * Retail dispatches through the retail vtables of those objects; OpenMFC
//     objects carry OpenMFC vtables, so every such call is made statically to
//     the exported thunk that the slot holds in the BASE class's vftable
//     (CMFCRibbonPanel 0x309390 / CMFCRibbonCategory 0x3014c8 /
//     CMFCRibbonBaseElement 0x2dc5c0).  DEVIATION: overrides in derived
//     classes are therefore not reached -- in particular CMFCRibbonMainPanel
//     (vftable 0x3079a8) overrides CopyFrom (+0x170), RecalcWidths (+0x1a0),
//     Add (+0x1a8), DoPaint (+0x1e0), Reposition (+0x1e8), OnDrawMenuBorder
//     (+0x228) and MouseButtonDown (+0x240), and the concrete element classes
//     override CopyFrom / ClosePopupMenu / FindByOriginal; only OnDrawMenuBorder
//     below selects the main-panel override (by IsKindOf), the other sites call
//     the CMFCRibbonPanel / CMFCRibbonBaseElement base thunks, exactly as
//     CMFCRibbonPanel.cpp does.  Inline virtuals that are not exported
//     (element IsAutoRepeatMode / OnAutoRepeat / OnLButtonDblClk / HasMenu /
//     IsMenuAreaHighlighted, panel IsMainPanel, gallery OnClickPaletteSubItem,
//     the base OnDrawMenuBorder pair, CMFCRibbonPanelMenu SetActive /
//     OnChangeMenuHighlight) cannot be reached; their base-class result is used
//     and noted.  Element IsHighlighted (slot +0x1b0) is `return
//     m_bIsHighlighted` (0xdee0: `mov 0x13c(%rcx),%eax`), an unmodeled read.
//   * The five constructors cannot create the owned panel: both
//     CMFCRibbonPanel ctor thunks in featurepack/ribbon/CMFCRibbonPanel.cpp are
//     misdeclared placeholders that ignore `this` and heap-allocate a fresh
//     object (reported as a header request), and CMFCRibbonPanel is
//     IMPLEMENT_DYNAMIC here so CRuntimeClass::CreateObject cannot clone one
//     either.  m_pPanel stays NULL there, and every body that retail runs
//     without a NULL check on m_pPanel checks it.  m_pCategory is only
//     non-NULL when a client-side DYNCREATE category class is cloned.
//   * Several element thunks this class needs are still argument-ignoring
//     placeholders (listed in the report's headerRequests).  Those whose
//     placeholders are harmless no-ops -- NotifyCommand, SetACCData,
//     GetTopLevelRibbonBar, CMFCRibbonGallery::GetMenuItems -- are declared
//     with their real signatures and called, so the bodies come alive when the
//     definitions are repaired; GetToolTipText / GetDescription (placeholders
//     that write through a mis-ordered parameter), GetMenuRowHeight and
//     CMFCRibbonPanelMenu::GetParentRibbonMenuBar (behind unreachable gates)
//     are not called.
//
// Every RVA below is an mfc140u.dll address resolved through the export table,
// the class vftable or the message map, as stated at each function.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"
#include "detail/RibbonState.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <mutex>

class CMFCRibbonPanelMenuBar;
class CMFCRibbonCmdUI;
class CMFCRibbonGallery;

namespace {
// Inherited members (OpenMFC declares them at the retail offsets).
constexpr int kOffHWnd                 = 0x40;    // CWnd::m_hWnd
constexpr int kOffAccData              = 0x150;   // CBasePane::m_AccData (CAccessibilityData, 88 bytes)
constexpr int kOffInUpdateShadow       = 0x111c;  // CMFCToolBar::m_bInUpdateShadow
constexpr int kOffAccHotItem           = 0x1154;  // CMFCToolBar::m_iAccHotItem
constexpr int kOffToolTip              = 0x12e8;  // CMFCToolBar::m_pToolTip (CMFCToolTipCtrl*)
constexpr int kOffDisableSideBarInXP   = 0x1350;  // CMFCPopupMenuBar::m_bDisableSideBarInXPMode
constexpr int kOffOffset               = 0x1364;  // CMFCPopupMenuBar::m_iOffset
constexpr int kOffResizeTracking       = 0x13a4;  // CMFCPopupMenuBar::m_bResizeTracking
constexpr int kOffPopupMenuBarOwnEnd   = 0x13b0;  // end of OpenMFC's clean-room CMFCPopupMenuBar
constexpr int kOffPopupMenuBarEnd      = 0x1420;  // end of the retail CMFCPopupMenuBar
// Own members (afxribbonpanelmenu.h order).
constexpr int kOffTracked              = 0x1420;
constexpr int kOffSimpleMode           = 0x1424;
constexpr int kOffIsMenuMode           = 0x1428;
constexpr int kOffIsDefaultMenuLook    = 0x142c;
constexpr int kOffIsQATPopup           = 0x1430;
constexpr int kOffCustomizeMenu        = 0x1434;
constexpr int kOffIsFloaty             = 0x1438;
constexpr int kOffHasKeyTips           = 0x143c;
constexpr int kOffSetKeyTips           = 0x1440;
constexpr int kOffAutoCommandTimer     = 0x1444;
constexpr int kOffSizePrefered         = 0x1448;
constexpr int kOffSizeCategory         = 0x1450;
constexpr int kOffRectAutoCommand      = 0x1458;
constexpr int kOffPtStartMenu          = 0x1468;
constexpr int kOffScrollBarVert        = 0x1470;
constexpr int kOffRibbonBar            = 0x1558;
constexpr int kOffPanel                = 0x1560;
constexpr int kOffPanelOrigin          = 0x1568;
constexpr int kOffDelayedCloseButton   = 0x1570;
constexpr int kOffDelayedButton        = 0x1578;
constexpr int kOffCategory             = 0x1580;
constexpr int kOffPressed              = 0x1588;
constexpr int kRetailSize              = 0x1590;

static_assert(offsetof(CWnd, m_hWnd) == kOffHWnd, "CWnd::m_hWnd");
static_assert(offsetof(CBasePane, m_AccData) == kOffAccData, "CBasePane::m_AccData");
static_assert(offsetof(CMFCToolBar, m_bInUpdateShadow) == kOffInUpdateShadow, "CMFCToolBar::m_bInUpdateShadow");
static_assert(offsetof(CMFCToolBar, m_iAccHotItem) == kOffAccHotItem, "CMFCToolBar::m_iAccHotItem");
static_assert(offsetof(CMFCToolBar, m_pToolTip) == kOffToolTip, "CMFCToolBar::m_pToolTip");
static_assert(sizeof(CMFCToolBar) == kOffDisableSideBarInXP, "CMFCPopupMenuBar's own members start at 0x1350");
static_assert(sizeof(CMFCPopupMenuBar) == kOffPopupMenuBarOwnEnd, "OpenMFC's CMFCPopupMenuBar is 0x13b0 bytes");
static_assert(sizeof(CWnd) == 0xe8 && kOffScrollBarVert + 0xe8 == kOffRibbonBar, "m_wndScrollBarVert is a 0xe8-byte CWnd");
static_assert(sizeof(CScrollBar) == sizeof(CWnd), "OpenMFC's CScrollBar adds no members to CWnd");
static_assert(kOffPressed + 8 == kRetailSize, "retail CMFCRibbonPanelMenuBar is 0x1590 bytes");
static_assert(sizeof(CAccessibilityData) == 88, "CAccessibilityData");
static_assert(offsetof(CDC, m_hDC) == 8, "CDC::m_hDC");
// CMFCRibbonBar is declared at retail size (afxmfc.h); the members read here:
static_assert(offsetof(CMFCRibbonBar, m_nKeyboardNavLevel) == 0x414, "CMFCRibbonBar::m_nKeyboardNavLevel");
static_assert(offsetof(CMFCRibbonBar, m_nTooltipWidthRegular) == 0x41c, "CMFCRibbonBar::m_nTooltipWidthRegular");
static_assert(offsetof(CMFCRibbonBar, m_nTooltipWidthLargeImage) == 0x420, "CMFCRibbonBar::m_nTooltipWidthLargeImage");
static_assert(offsetof(CMFCRibbonBar, m_bQuickAccessToolbarOnTop) == 0x434, "CMFCRibbonBar::m_bQuickAccessToolbarOnTop");
static_assert(offsetof(CMFCRibbonBar, m_bToolTip) == 0x450, "CMFCRibbonBar::m_bToolTip");
static_assert(offsetof(CMFCRibbonBar, m_bToolTipDescr) == 0x454, "CMFCRibbonBar::m_bToolTipDescr");
static_assert(offsetof(CMFCRibbonBar, m_bReplaceFrameCaption) == 0x468, "CMFCRibbonBar::m_bReplaceFrameCaption");
static_assert(offsetof(CMFCRibbonBar, m_pActiveCategory) == 0xb08, "CMFCRibbonBar::m_pActiveCategory");
static_assert(offsetof(CMFCRibbonBar, m_pKeyboardNavLevelCurrent) == 0xc10, "CMFCRibbonBar::m_pKeyboardNavLevelCurrent");

// Retail CMFCPopupMenu members (the offsets CMFCPopupMenu.cpp pins).
constexpr int kOffPopupParentRibbonElement = 0x19b8;  // CMFCRibbonBaseElement* m_pParentRibbonElement
constexpr int kOffPopupScrollable          = 0x16a4;  // BOOL written by CalcSize when the menu overflows
// Retail CMFCRibbonPanelMenu: CMFCPopupMenu (0x19c8) + m_wndRibbonBar (0x1590) then
constexpr int kOffPanelMenuForceClose      = 0x2f58;  // BOOL m_bForceClose (object size 0x2f60 == 12128, RuntimeClasses.cpp)
// Retail CArray<T*,T*> (a CObject): vfptr, m_pData +8, m_nSize +0x10.  OpenMFC's
// CArray (afx.h) has no vfptr, so a client-supplied array is read by offset.
constexpr int kOffMsArrayData = 0x08;
constexpr int kOffMsArraySize = 0x10;
// Retail CMFCRibbonCmdUI (a CCmdUI + m_pUpdated): 0x48 bytes, m_pOther at +0x20.
constexpr int kRibbonCmdUISize     = 0x48;
constexpr int kOffCmdUIOther       = 0x20;
// Retail CMFCToolTipCtrl members retail's OnNeedTipText writes (afxtooltipctrl.h
// order; documented at that function, not written here): m_pRibbonButton
// +0x138, m_ptLocation +0x198, m_nFixedWidthRegular +0x1a0, m_nFixedWidthWithImage +0x1a4.

constexpr UINT_PTR kTimerDelayedOpen  = 0xec1b;
constexpr UINT_PTR kTimerDelayedClose = 0xec1c;
constexpr UINT_PTR kTimerAutoCommand  = 0xec1d;
constexpr UINT_PTR kTimerAccHot       = 0xec07;
constexpr UINT     kAfxTooltipTypeRibbon = 0x200;   // AFX_TOOLTIP_TYPE_RIBBON
constexpr int      kTooltipMaxWidth      = 0x280;   // 640
constexpr UINT     kRdwInvalidateEraseUpdate = 0x105;   // RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW
constexpr UINT     kRdwInvalidateEraseUpdateAll = 0x185; // ... |RDW_ALLCHILDREN
constexpr int      kGlobalDataInitGate        = 0x000;
constexpr int      kGlobalDataEnableAccessibility = 0x278;  // written by ?EnableAccessibilitySupport@AFX_GLOBAL_DATA@@ (RVA 0x6bec0, mfc140u: `mov %edx,0x278(%rcx)`)
constexpr int      kGlobalDataMenuAnimation   = 0x26c;  // bMenuAnimation:  SPI_GETMENUANIMATION (0x1002) in ?OnSettingChange@AFX_GLOBAL_DATA@@ (RVA 0x6b0a0, mfc140u)
constexpr int      kGlobalDataMenuFadeEffect  = 0x270;  // bMenuFadeEffect: SPI_GETMENUFADE (0x1012), same function
constexpr std::uint32_t kSystemDefaultAnimation = 999;  // CMFCPopupMenu::SYSTEM_DEFAULT_ANIMATION
constexpr std::uint32_t kAnimationSlide = 2, kAnimationFade = 3;   // CMFCPopupMenu::SLIDE / FADE
constexpr UINT     kScrollBarStyle = 0x50000001;   // WS_CHILD|WS_VISIBLE|SBS_VERT
constexpr int      kAfxWmUpdateToolTipsMask = 0x200;  // AFX_TOOLTIP_TYPE_RIBBON bit tested by OnUpdateToolTips
constexpr int      kDefaultAutoRepeatDelay = 0x64;   // 100 ms, OnLButtonDown's default

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}
inline HWND HWndOf(const void* p) { return At<HWND>(p, kOffHWnd); }
inline CWnd* AsWnd(void* p) { return static_cast<CWnd*>(static_cast<CMFCPopupMenuBar*>(p)); }
inline CMFCPopupMenuBar* AsPopupMenuBar(void* p) { return static_cast<CMFCPopupMenuBar*>(p); }
inline CMFCToolBar* AsToolBar(void* p) { return static_cast<CMFCToolBar*>(static_cast<CMFCPopupMenuBar*>(p)); }
inline CMFCRibbonBar*& RibbonBarOf(void* p) { return At<CMFCRibbonBar*>(p, kOffRibbonBar); }
inline CMFCRibbonBar* RibbonBarOf(const void* p) { return At<CMFCRibbonBar*>(p, kOffRibbonBar); }
inline CMFCRibbonPanel*& PanelOf(void* p) { return At<CMFCRibbonPanel*>(p, kOffPanel); }
inline CMFCRibbonCategory*& CategoryOf(void* p) { return At<CMFCRibbonCategory*>(p, kOffCategory); }
inline CMFCRibbonPanel* PanelOf(const void* p) { return At<CMFCRibbonPanel*>(p, kOffPanel); }
inline CMFCRibbonCategory* CategoryOf(const void* p) { return At<CMFCRibbonCategory*>(p, kOffCategory); }
inline CMFCToolTipCtrl*& ToolTipOf(void* p) { return At<CMFCToolTipCtrl*>(p, kOffToolTip); }
inline CScrollBar* ScrollBarOf(void* p) { return reinterpret_cast<CScrollBar*>(static_cast<char*>(p) + kOffScrollBarVert); }
inline CMFCRibbonBaseElement*& DelayedCloseButtonOf(void* p) { return At<CMFCRibbonBaseElement*>(p, kOffDelayedCloseButton); }
inline CMFCRibbonBaseElement*& DelayedButtonOf(void* p) { return At<CMFCRibbonBaseElement*>(p, kOffDelayedButton); }
inline CMFCRibbonBaseElement*& PressedOf(void* p) { return At<CMFCRibbonBaseElement*>(p, kOffPressed); }
inline RECT& RectAutoCommandOf(void* p) { return At<RECT>(p, kOffRectAutoCommand); }
inline POINT& PtStartMenuOf(void* p) { return At<POINT>(p, kOffPtStartMenu); }
inline SIZE& SizePreferedOf(void* p) { return At<SIZE>(p, kOffSizePrefered); }

// CPoint / CSize passed BY VALUE are 8-byte aggregates in a register under the
// MS x64 ABI: x/cx in the low dword, y/cy in the high dword.
inline POINT UnpackPoint(long long v) {
    POINT pt;
    pt.x = static_cast<int>(static_cast<std::uint32_t>(v & 0xffffffffu));
    pt.y = static_cast<int>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) >> 32));
    return pt;
}
inline long long PackPoint(POINT pt) {
    return static_cast<long long>((static_cast<std::uint64_t>(static_cast<std::uint32_t>(pt.y)) << 32) |
                                  static_cast<std::uint32_t>(pt.x));
}
inline CPoint ToCPoint(POINT pt) { return CPoint(pt.x, pt.y); }
} // namespace

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition; parameter lists are
// derived from the mangled names.
//   core/runtime/CObject.cpp / CRuntimeClass.cpp : IsKindOf, CRuntimeClass::CreateObject
//   core/window/CWnd.cpp / Thunks.cpp / CtorDtorPlacement.cpp :
//       FromHandle, ShowWindow, DestroyWindow, GetDlgCtrlID, UpdateDialogControls,
//       SendChildNotifyLastMsg, Default, GetTopLevelFrame, ClientToScreen(RECT*),
//       ??0CWnd (placement)
//   core/gdi/CClientDC.cpp / CGdiObject.cpp / CDC.cpp :
//       ??0CClientDC / ??1CClientDC, CGdiObject::FromHandle, CDC::SelectObject(CFont*),
//       CDC::SetBkMode, CDC::GetClipBox
//   core/controls/RuntimeClasses.cpp     : ??1CScrollBar
//   detail/MfcExceptionsSupport.cpp      : AfxThrowInvalidArgException
//   detail/CMemDCSupport.cpp             : ??0CMemDC(CDC&,CWnd*) / ??1CMemDC (declared by the header)
//   core/runtime/Globals.cpp / AFX_GLOBAL_DATA.cpp : AfxGetApp, afxGlobalData, AFX_GLOBAL_DATA::Initialize
//   featurepack/propertygrid/CMFCPropertyGridCtrl.cpp / core/runtime/CCmdTarget.cpp : CCmdTarget::BeginWaitCursor / EndWaitCursor
//   featurepack/CMFC_misc_stubs.cpp      : AFXGetParentFrame, g_pTopLevelFrame
//   featurepack/menu/RuntimeClasses.cpp / ribbon/RuntimeClasses.cpp : GetThisClass getters
//   featurepack/menu/CMFCPopupMenu.cpp   : GetParentPopupMenu, ActivatePopupMenu, UpdateAllShadows,
//       FromHandlePermanent, m_pActivePopupMenu, CMFCRibbonBaseElement::ClosePopupMenu
//   featurepack/menu/CMFCPopupMenuBar.cpp: the CMFCPopupMenuBar base handlers
//   featurepack/menu/StaticData.cpp      : m_AnimationType
//   featurepack/toolbar/CMFCToolBar.cpp  : OnSize, PreTranslateMessage
//   featurepack/customize/CTooltipManager.cpp : CreateToolTip, DeleteToolTip
//   featurepack/ribbon/CMFCRibbonBar.cpp : CToolTipCtrl::AddTool / SetToolRect (defined there),
//       DeactivateKeyboardFocus, ShowKeyTips, ProcessKey, SetKeyboardNavigationLevel,
//       OnShowRibbonContextMenu
//   featurepack/ribbon/CMFCRibbonPanel.cpp : the panel exports named below
//   featurepack/ribbon/CMFCRibbonCategory.cpp : the category exports named below
//   featurepack/ribbon/CMFCRibbonBaseElement.cpp : CopyFrom, SetOriginal, SetParentMenu, FindByOriginal
//   featurepack/ribbon/CMFCRibbonCmdUI.cpp : ??0CMFCRibbonCmdUI
//   featurepack/visualmanager/CMFCVisualManager.cpp / Thunks.cpp : m_pVisManager,
//       OnFillRibbonQuickAccessToolBarPopup
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CScrollBar__UEAA_XZ(CScrollBar* pThis);
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];      // 0x1803c1620 (mfc140u)
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;                   // 0x1803be1b8 (mfc140u)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanel__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonRichEditCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(const void* pThis);
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu);
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(RECT* lprectScreen);
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;             // 0x1803be288 (mfc140u)
extern "C" std::uint32_t impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A; // 0x1803be290 (mfc140u)
extern "C" void MS_ABI impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(CMFCRibbonBaseElement* pElem);
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCPopupMenuBar* pThis, LPCREATESTRUCTW lpCreateStruct);
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenuBar__IEAAXXZ(CMFCPopupMenuBar* pThis);
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long point);
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long point);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long point);
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long point);
extern "C" void* MS_ABI impl___0CMFCPopupMenuBar__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCPopupMenuBar__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnSize_CMFCToolBar__IEAAXIHH_Z(CMFCToolBar* pThis, unsigned int nType, int cx, int cy);
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(CMFCToolBar* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(CToolTipCtrl** ppToolTip, CWnd* pWndParent, unsigned int nType);
extern "C" void MS_ABI impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(CToolTipCtrl** ppToolTip);
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, const wchar_t* lpszText, const RECT* lpRectTool, unsigned __int64 nIDTool);
extern "C" void MS_ABI impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
    CToolTipCtrl* pThis, CWnd* pWnd, unsigned __int64 nIDTool, const RECT* lpRect);
extern "C" void MS_ABI impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(CMFCRibbonBar* pThis, int bSetFocus);
extern "C" void MS_ABI impl__ShowKeyTips_CMFCRibbonBar__QEAAXH_Z(CMFCRibbonBar* pThis, int bRepos);
extern "C" int MS_ABI impl__ProcessKey_CMFCRibbonBar__IEAAHH_Z(CMFCRibbonBar* pThis, int nKey);
extern "C" void MS_ABI impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(CMFCRibbonBar* pThis, CObject* pLevel, int bSetFocus);
extern "C" int MS_ABI impl__OnShowRibbonContextMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(const CMFCRibbonPanel* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(const CMFCRibbonPanel* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetPressed_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(const CMFCRibbonPanel* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonPanel__UEAAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(
    CMFCRibbonPanel* pThis, long long point, int bCheckPanelCaption);
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanel__UEBAHVCPoint___Z(const CMFCRibbonPanel* pThis, long long point);
extern "C" void MS_ABI impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(CMFCRibbonPanel* pThis, int bHighlight, long long point);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__MouseButtonDown_CMFCRibbonPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    CMFCRibbonPanel* pThis, long long point);
extern "C" void MS_ABI impl__MouseButtonUp_CMFCRibbonPanel__MEAAXVCPoint___Z(CMFCRibbonPanel* pThis, long long point);
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanel__UEAAHI_Z(CMFCRibbonPanel* pThis, unsigned int nChar);
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
    CMFCRibbonPanel* pThis, CMFCRibbonCmdUI* pCmdUI, CFrameWnd* pTarget, int bDisableIfNoHndler);
extern "C" void MS_ABI impl__Reposition_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(CMFCRibbonPanel* pThis, CDC* pDC, const CRect& rect);
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonPanel__MEAAXPEAVCDC___Z(CMFCRibbonPanel* pThis, CDC* pDC);
extern "C" void MS_ABI impl__RecalcWidths_CMFCRibbonPanel__UEAAXPEAVCDC__H_Z(CMFCRibbonPanel* pThis, CDC* pDC, int nHeight);
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonPanel__MEAAXPEAVCDC___Z(CMFCRibbonPanel* pThis, CDC* pDC);
extern "C" void MS_ABI impl__RemoveAll_CMFCRibbonPanel__QEAAXXZ(CMFCRibbonPanel* pThis);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonPanel__MEAAXAEAV1__Z(CMFCRibbonPanel* pThis, CMFCRibbonPanel& src);
extern "C" void MS_ABI impl__Add_CMFCRibbonPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElem);
extern "C" void MS_ABI impl__GetElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonPanel* pThis, CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(CMFCRibbonCategory* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(CMFCRibbonCategory* pThis);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(
    const CMFCRibbonCategory* pThis, CPoint point, int bCheckPanelCaption);
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonCategory__QEBAHVCPoint___Z(const CMFCRibbonCategory* pThis, CPoint point);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__OnLButtonDown_CMFCRibbonCategory__UEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    CMFCRibbonCategory* pThis, CPoint point);
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonCategory__UEAAXVCPoint___Z(CMFCRibbonCategory* pThis, CPoint point);
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonCategory__UEAAXVCPoint___Z(CMFCRibbonCategory* pThis, CPoint point);
extern "C" int MS_ABI impl__OnKey_CMFCRibbonCategory__UEAAHI_Z(CMFCRibbonCategory* pThis, unsigned int nChar);
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonCategory__UEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
    CMFCRibbonCategory* pThis, void* pCmdUI, CFrameWnd* pTarget, int bDisableIfNoHndler);
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonCategory__UEAAXPEAVCDC___Z(CMFCRibbonCategory* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCategory__UEAAXPEAVCDC___Z(CMFCRibbonCategory* pThis, CDC* pDC);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonCategory__UEAAXAEAV1__Z(CMFCRibbonCategory* pThis, CMFCRibbonCategory& src);
extern "C" CMFCRibbonPanel* MS_ABI impl__GetPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__H_Z(CMFCRibbonCategory* pThis, int nIndex);
extern "C" int MS_ABI impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(const CMFCRibbonCategory* pThis);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement& src);
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonBaseElement__UEAAXPEAV1__Z(CMFCRibbonBaseElement* pThis, CMFCRibbonBaseElement* pOriginal);
extern "C" void MS_ABI impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonPanelMenuBar* pMenuBar);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByOriginal_CMFCRibbonBaseElement__UEAAPEAV1_PEAV1__Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonBaseElement* pOriginal);
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonMainPanel__MEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar___Z(
    CMFCRibbonPanel* pThis, CDC* pDC, CMFCRibbonPanelMenuBar* pMenuBar);
// Four thunks whose definitions (featurepack/ribbon/CMFCRibbonBaseElement.cpp,
// CMFCRibbonGallery.cpp) are still auto-generated placeholders that ignore
// their arguments and return 0 / do nothing.  They are declared here with the
// signatures their mangled names specify and CALLED, so that the bodies below
// start behaving as retail the moment those definitions are repaired (see the
// report's headerRequests); today each call is a harmless no-op.  (This is
// safe only because the placeholders never touch their arguments -- the
// GetToolTipText / GetDescription thunks, whose placeholders WRITE through a
// mis-ordered parameter, are deliberately not called; see OnNeedTipText.)
extern "C" int MS_ABI impl__NotifyCommand_CMFCRibbonBaseElement__QEAAHH_Z(CMFCRibbonBaseElement* pThis, int bWithDelay);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonBaseElement* pThis, CWnd* pParent, CAccessibilityData& data);
extern "C" CMFCRibbonBar* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(const CMFCRibbonBaseElement* pThis);
extern "C" void MS_ABI impl__GetMenuItems_CMFCRibbonGallery__IEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonGallery* pThis, CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arButtons);
extern "C" void* MS_ABI impl___0CMFCRibbonCmdUI__QEAA_XZ(void* pThis);
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;             // 0x1803be3c0 (mfc140u)
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pMenuBar, CRect rect);

// Exports of this class reached by other bodies below (retail reaches most of
// them through vtable slots of this class; see each call site).
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AddButtons_CMFCRibbonPanelMenuBar__IEAAXPEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, CMFCRibbonBar* pRibbonBar, const void* pArButtons, int bFloatyMode);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(const void* pThis, long long point);
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanelMenuBar__QEBAHVCPoint___Z(const void* pThis, long long point);
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__XZ(const void* pThis);
extern "C" CMFCRibbonButton* MS_ABI impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(const void* pThis);
extern "C" CMFCRibbonBar* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBar__XZ(const void* pThis);
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point);
extern "C" void MS_ABI impl__OnContextMenu_CMFCRibbonPanelMenuBar__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* pWnd, long long point);
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanelMenuBar__UEAAHI_Z(void* pThis, unsigned int nChar);
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonPanelMenuBar__UEAAHJ_Z(void* pThis, long lVal);
extern "C" void MS_ABI impl__PopTooltip_CMFCRibbonPanelMenuBar__QEAAXXZ(void* pThis);

namespace {
// pObj->GetRuntimeClass() through vtable slot 0 (the first virtual of CObject
// in both the MSVC and the Itanium layout, as afx.h notes).  Written as a raw
// slot call so that the compiler cannot speculatively devirtualise it to the
// inline DECLARE_DYNAMIC body, which would pull the class descriptors in as
// undefined C++ symbols.
inline CRuntimeClass* RuntimeClassOf(const CObject* pObj) {
    using GetRtcFn = CRuntimeClass* (MS_ABI*)(const CObject*);
    void** vtbl = *reinterpret_cast<void** const*>(pObj);
    return reinterpret_cast<GetRtcFn>(vtbl[0])(pObj);
}
// `delete pObj` as retail's destructor performs it: vtable slot 1 -- the MSVC
// scalar deleting destructor, `(this, flags)` with bit 0 = free -- with flag 1.
// Every object stored in m_pPanel / m_pCategory by this file came out of
// CRuntimeClass::CreateObject, i.e. from a client DYNCREATE factory whose
// object carries the MSVC layout, for which this is exactly right.  (An object
// built by this DLL's own C++ classes would carry the Itanium layout, where
// slot 1 is the non-deleting destructor: it would be destroyed but its memory
// not freed.  Nothing in OpenMFC produces such an object for these members.)
// The exported ~CMFCRibbonPanel / ~CMFCRibbonCategory thunks are NOT used:
// they run the C++ virtual destructor, which on an MSVC-layout object would
// itself land in the deleting destructor and free the object once already.
inline void DeleteThroughVtable(CObject* pObj) {
    using DeletingDtorFn = void* (MS_ABI*)(void*, unsigned int);
    void** vtbl = *reinterpret_cast<void***>(pObj);
    reinterpret_cast<DeletingDtorFn>(vtbl[1])(pObj, 1u);
}
// RibbonMutex() through try_lock (noexcept) instead of lock(): the throwing
// path of std::mutex::lock is a C++ runtime symbol this file's audit forbids.
struct SideTableLock {
    SideTableLock() { while (!openmfc::ribbon_state::RibbonMutex().try_lock()) { ::Sleep(0); } }
    ~SideTableLock() { openmfc::ribbon_state::RibbonMutex().unlock(); }
    SideTableLock(const SideTableLock&) = delete;
    SideTableLock& operator=(const SideTableLock&) = delete;
};
inline bool IsKindOfRt(const CObject* p, CRuntimeClass* pRtc) {
    return p != nullptr && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, pRtc) != 0;
}
// `CWnd::FromHandle(::GetParent(m_hWnd))`, as every retail body inlines it.
inline CWnd* ParentWndOf(const void* pThis) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
}
// The parent kept only if it IsKindOf the given class, else NULL.
inline CWnd* ParentWndOfKind(const void* pThis, CRuntimeClass* pRtc) {
    CWnd* p = ParentWndOf(pThis);
    return IsKindOfRt(p, pRtc) ? p : nullptr;
}
inline CWnd* ParentRibbonPanelMenu(const void* pThis) {
    return ParentWndOfKind(pThis, impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ());
}
inline CWnd* ParentPopupMenu(const void* pThis) {
    return ParentWndOfKind(pThis, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ());
}
inline bool IsRibbonButton(const CMFCRibbonBaseElement* p) {
    return IsKindOfRt(p, impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ());
}

// The afxGlobalData init gate, exactly as every retail reader inlines it:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); afxGlobalData.<+0> = 1; }
inline void EnsureGlobalDataInitialized() {
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline int GlobalDataInt(int off) {
    int v = 0;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}

// CMFCPopupMenu::GetAnimationType(FALSE), the unexported helper at RVA 0x2e600
// (mfc140u) that the CMFCDisableMenuAnimation guard in OnKey saves:
//     if (m_AnimationType != SYSTEM_DEFAULT_ANIMATION) return m_AnimationType;
//     <afxGlobalData init gate>
//     if (!afxGlobalData.bMenuAnimation) return NO_ANIMATION;         // +0x26c
//     return afxGlobalData.bMenuFadeEffect ? FADE : SLIDE;            // +0x270 -> 3 : 2
inline std::uint32_t ResolvedAnimationType() {
    const std::uint32_t raw = impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A;
    if (raw != kSystemDefaultAnimation) return raw;
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalDataMenuAnimation) == 0) return 0;
    return GlobalDataInt(kGlobalDataMenuFadeEffect) != 0 ? kAnimationFade : kAnimationSlide;
}

// The ribbon_state side-table reverse lookups CMFCRibbonPanel.cpp uses for the
// panel->category (retail panel+0x158) and category->bar (retail category+0x760)
// links.  Both take RibbonMutex().
inline CMFCRibbonCategory* SideTableParentCategory(const CMFCRibbonPanel* pPanel) {
    if (pPanel == nullptr) return nullptr;
    SideTableLock lock;
    for (const auto& entry : openmfc::ribbon_state::RibbonCategoryStates()) {
        const auto& panels = entry.second.panels;
        if (std::find(panels.begin(), panels.end(), pPanel) != panels.end()) {
            return const_cast<CMFCRibbonCategory*>(entry.first);
        }
    }
    return nullptr;
}
inline CMFCRibbonBar* SideTableParentBar(const CMFCRibbonCategory* pCategory) {
    if (pCategory == nullptr) return nullptr;
    SideTableLock lock;
    for (const auto& entry : openmfc::ribbon_state::RibbonBarStates()) {
        const auto& categories = entry.second.categories;
        if (std::find(categories.begin(), categories.end(), pCategory) != categories.end()) {
            return const_cast<CMFCRibbonBar*>(entry.first);
        }
    }
    return nullptr;
}

// A CClientDC built through the exported placement ctor / dtor thunks (retail:
// ??0CClientDC@@QEAA@PEAVCWnd@@@Z at 0x2a3b20 / ??1CClientDC@@UEAA@XZ at
// 0x2a3be0, mfc140u).  Only m_hDC is ever read through it, so the missing vptr
// is harmless.
struct ClientDCScope {
    alignas(8) unsigned char storage[sizeof(CClientDC)];
    explicit ClientDCScope(CWnd* pWnd) {
        std::memset(storage, 0, sizeof storage);
        impl___0CClientDC__QEAA_PEAVCWnd___Z(dc(), pWnd);
    }
    ~ClientDCScope() { impl___1CClientDC__UEAA_XZ(dc()); }
    CClientDC* dc() { return reinterpret_cast<CClientDC*>(storage); }
    ClientDCScope(const ClientDCScope&) = delete;
    ClientDCScope& operator=(const ClientDCScope&) = delete;
};

// `CFont* pOld = pDC->SelectObject(CFont::FromHandle((HFONT)m_pRibbonBar->
// SendMessage(WM_GETFONT)))` as retail inlines it (SendMessageW slot 0x2c7120,
// ?FromHandle@CGdiObject@@ 0x2a3ea0 -- CGdiObject's, not CFont's -- and the
// CDC::SelectObject body at 0x2a2730, which is the COMDAT-folded CFont/CBrush
// overload; OpenMFC's CFont overload thunk is called).  Retail does not
// NULL-check m_pRibbonBar (it dereferences bar->m_hWnd); a NULL bar here sends
// WM_GETFONT to a NULL HWND, which yields no font and lands on the same
// AfxThrowInvalidArgException retail raises when SelectObject returns NULL.
inline CFont* SelectRibbonFont(const void* pThis, CDC* pDC) {
    const CMFCRibbonBar* pBar = RibbonBarOf(pThis);
    HWND hWndBar = pBar != nullptr ? HWndOf(pBar) : nullptr;
    HGDIOBJ hFont = reinterpret_cast<HGDIOBJ>(::SendMessage(hWndBar, WM_GETFONT, 0, 0));
    CGdiObject* pFont = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(hFont);
    return impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, static_cast<CFont*>(pFont));
}

// CMFCVisualManager::GetInstance() as this tree reads it (see the note in
// CMFCRibbonBar.cpp: the C++ GetInstance is not an impl__ thunk, so the exported
// m_pVisManager static is read; retail's inlined getter would CREATE the default
// manager when none exists yet, OpenMFC skips the drawing in that case).
inline CMFCVisualManager* VisualManager() {
    return static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
}

// IsRibbonPanelInRegularMode() (own vtable slot +0x8a8, RVA 0x11dba0 mfc140u):
//     if (m_bIsMenuMode != 0) return FALSE;
//     if (m_pPanel != NULL && m_pPanel->IsMainPanel()) return FALSE;   // panel slot +0x178
//     return TRUE;
// IsMainPanel is an unexported per-class constant (CMFCRibbonPanel's slot is the
// `xor eax,eax; ret` stub at 0x71e0; CMFCRibbonMainPanel's, vftable 0x3079a8
// +0x178, is the `mov eax,1; ret` stub at 0x3a60), so it is folded to
// IsKindOf(CMFCRibbonMainPanel).
inline bool IsRibbonPanelInRegularMode(const void* pThis) {
    if (At<int>(pThis, kOffIsMenuMode) != 0) return false;
    const CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel != nullptr &&
        IsKindOfRt(pPanel, impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ())) {
        return false;
    }
    return true;
}

// The per-object part every constructor shares before CommonInit (transcribed
// from ??0CMFCRibbonPanelMenuBar@@IEAA@XZ, RVA 0x11e2b0 mfc140u; the other four
// constructors carry the identical prologue):
//     CMFCPopupMenuBar::CMFCPopupMenuBar();          // 0xbbf80
//     vfptr = &CMFCRibbonPanelMenuBar::`vftable'     // 0x30a748 -- OpenMFC keeps the
//                                                    // CMFCPopupMenuBar vtable its base ctor installed
//     m_sizePrefered = m_sizeCategory = CSize(0,0); m_rectAutoCommand = CRect(0,0,0,0);
//     m_ptStartMenu = CPoint(0,0);                   // the five movq $0 at +0x1448..+0x1468
//     CWnd::CWnd(&m_wndScrollBarVert); vfptr = CScrollBar vftable (0x2f82c8)
// DEVIATION: the embedded scroll bar is built with the CWnd placement thunk and
// keeps the CWnd vtable (constructing a C++ CScrollBar here would drag its
// vtable and every inherited CWnd virtual into this object as undefined C++
// symbols, which the link audit forbids); OpenMFC's CScrollBar adds no data
// members, so the layout is identical.
inline void ConstructPrologue(void* pThis) {
    impl___0CMFCPopupMenuBar__QEAA_XZ(pThis);
    SizePreferedOf(pThis) = SIZE{0, 0};
    At<SIZE>(pThis, kOffSizeCategory) = SIZE{0, 0};
    RectAutoCommandOf(pThis) = RECT{0, 0, 0, 0};
    PtStartMenuOf(pThis) = POINT{0, 0};
    impl___0CWnd__QEAA_XZ(ScrollBarOf(pThis));
}
} // namespace

// CommonInit(): retail RVA 0x11e370 (mfc140u), transcribed in full:
//     if (m_pPanel != NULL) m_pPanel->m_pParentMenuBar = this;     // panel +0x160
//     m_pCategory = NULL; m_sizeCategory = CSize(0,0); m_pDelayedCloseButton = NULL;
//     m_pDelayedButton = NULL; m_pPressed = NULL;
//     m_rectAutoCommand.SetRectEmpty();                             // IAT SetRectEmpty
//     m_bSimpleMode = m_bIsMenuMode = 0;        // one 8-byte store at +0x1424
//     m_bDisableSideBarInXPMode = 1;            // CMFCPopupMenuBar +0x1350
//     m_bCustomizeMenu = 1; m_bIsFloaty = 0;    // one 8-byte store at +0x1434
//     m_ptStartMenu = CPoint(-1, -1);
//     m_bIsDefaultMenuLook = m_bIsQATPopup = 0; // 8-byte store at +0x142c
//     m_pPanelOrigin = NULL; m_pRibbonBar = NULL; m_bTracked = 0;
//     m_pToolTip = NULL;                        // CMFCToolBar +0x12e8
//     m_sizePrefered = CSize(0,0);
//     m_bHasKeyTips = m_bSetKeyTips = 0;        // 8-byte store at +0x143c
//     m_bAutoCommandTimer = 0;
// DEVIATION: the store into the panel's m_pParentMenuBar (+0x160) is not made --
// the clean-room CMFCRibbonPanel has no such member (see the file header).
// Symbol: ?CommonInit@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    // (panel->m_pParentMenuBar = this is skipped: unmodeled, see above)
    CategoryOf(pThis) = nullptr;
    At<SIZE>(pThis, kOffSizeCategory) = SIZE{0, 0};
    DelayedCloseButtonOf(pThis) = nullptr;
    DelayedButtonOf(pThis) = nullptr;
    PressedOf(pThis) = nullptr;
    ::SetRectEmpty(&RectAutoCommandOf(pThis));
    At<int>(pThis, kOffSimpleMode) = 0;
    At<int>(pThis, kOffIsMenuMode) = 0;
    At<int>(pThis, kOffDisableSideBarInXP) = 1;
    At<int>(pThis, kOffCustomizeMenu) = 1;
    At<int>(pThis, kOffIsFloaty) = 0;
    PtStartMenuOf(pThis) = POINT{-1, -1};
    At<int>(pThis, kOffIsDefaultMenuLook) = 0;
    At<int>(pThis, kOffIsQATPopup) = 0;
    At<CMFCRibbonPanel*>(pThis, kOffPanelOrigin) = nullptr;
    RibbonBarOf(pThis) = nullptr;
    At<int>(pThis, kOffTracked) = 0;
    ToolTipOf(pThis) = nullptr;
    SizePreferedOf(pThis) = SIZE{0, 0};
    At<int>(pThis, kOffHasKeyTips) = 0;
    At<int>(pThis, kOffSetKeyTips) = 0;
    At<int>(pThis, kOffAutoCommandTimer) = 0;
}

// CMFCRibbonPanelMenuBar(): retail RVA 0x11e2b0 (mfc140u).  After the shared
// prologue (ConstructPrologue above):
//     m_pPanel = new CMFCRibbonPanel(NULL, NULL);   // ??2@YAPEAX_K@Z(0x6f8) + ??0CMFCRibbonPanel@@IEAA@PEB_WPEAUHICON__@@@Z (0x115d70)
//     CommonInit();
// DEVIATION: m_pPanel is left NULL.  The only route to a CMFCRibbonPanel from
// this file, ??0CMFCRibbonPanel@@IEAA@PEB_WPEAUHICON__@@@Z in
// featurepack/ribbon/CMFCRibbonPanel.cpp, is a misdeclared placeholder that
// reads `this` as the name string and heap-allocates an unrelated object
// (reported as a header request); calling it would be undefined behaviour.
// Symbol: ??0CMFCRibbonPanelMenuBar@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__IEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    ConstructPrologue(pThis);
    PanelOf(pThis) = nullptr;   // retail: the freshly constructed 0x6f8-byte panel
    impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(pThis);
    return pThis;
}

// CMFCRibbonPanelMenuBar(CMFCRibbonBar* pRibbonBar, const CArray<...>& arButtons,
// BOOL bIsFloatyMode): retail RVA 0x11ddd0 (mfc140u).  After the shared prologue:
//     m_pPanel = new CMFCRibbonPanel(NULL, NULL);
//     CommonInit();
//     AddButtons(pRibbonBar, arButtons, bIsFloatyMode);     // 0x11e170
// DEVIATION: m_pPanel stays NULL (see the XZ constructor), so AddButtons has no
// panel to fill; it still records pRibbonBar and the simple-mode flag.
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, CMFCRibbonBar* pRibbonBar, const void* pArButtons, int bIsFloatyMode) {
    if (pThis == nullptr) return nullptr;
    ConstructPrologue(pThis);
    PanelOf(pThis) = nullptr;   // retail: the freshly constructed 0x6f8-byte panel
    impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(pThis);
    impl__AddButtons_CMFCRibbonPanelMenuBar__IEAAXPEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
        pThis, pRibbonBar, pArButtons, bIsFloatyMode);
    return pThis;
}

// CMFCRibbonPanelMenuBar(CMFCRibbonGallery* pPaletteButton): retail RVA 0x11deb0
// (mfc140u).  After the shared prologue:
//     m_pPanel = new CMFCRibbonPanel(pPaletteButton);       // ??0CMFCRibbonPanel@@IEAA@PEAVCMFCRibbonGallery@@@Z (0x115f10)
//     CommonInit();
//     CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
//     pPaletteButton->GetMenuItems(arButtons);              // 0x114880
//     AddButtons(pPaletteButton->GetTopLevelRibbonBar(), arButtons, FALSE);   // 0x10140, 0x11e170
// DEVIATION: m_pPanel stays NULL (the gallery-flavoured panel ctor thunk is the
// same kind of placeholder as the named one).  GetMenuItems and
// GetTopLevelRibbonBar are called through their exported thunks with the
// retail arguments; both definitions are still argument-ignoring placeholders
// in this tree (header requests), so today the array stays empty and the bar
// is NULL, and AddButtons only records the simple-mode flag.
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonGallery@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonGallery___Z(
    void* pThis, CMFCRibbonGallery* pPaletteButton) {
    if (pThis == nullptr) return nullptr;
    ConstructPrologue(pThis);
    PanelOf(pThis) = nullptr;   // retail: new CMFCRibbonPanel(pPaletteButton)
    impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(pThis);
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
    CMFCRibbonBar* pBar = nullptr;
    if (pPaletteButton != nullptr) {   // retail dereferences it unconditionally
        impl__GetMenuItems_CMFCRibbonGallery__IEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(pPaletteButton, arButtons);
        // CMFCRibbonGallery -> CMFCRibbonButton -> CMFCRibbonBaseElement is a
        // single-inheritance chain (retail passes the same pointer to 0x10140).
        pBar = impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(
            reinterpret_cast<const CMFCRibbonBaseElement*>(pPaletteButton));
    }
    // The retail array is an MSVC CArray (vfptr, m_pData +8, m_nSize +0x10);
    // AddButtons reads those two fields, so hand it a retail-shaped view.
    struct { void* vfptr; CMFCRibbonBaseElement** pData; INT_PTR nSize; } msArray = {nullptr, arButtons.GetData(), arButtons.GetSize()};
    impl__AddButtons_CMFCRibbonPanelMenuBar__IEAAXPEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
        pThis, pBar, &msArray, FALSE);
    return pThis;
}

// CMFCRibbonPanelMenuBar(CMFCRibbonPanel* pPanel): retail RVA 0x11dbf0 (mfc140u).
// After the shared prologue:
//     CObject* pClone = pPanel->GetRuntimeClass()->CreateObject();      // vslot 0, 0x234d60
//     if (pClone == NULL || !pClone->IsKindOf(RUNTIME_CLASS(CMFCRibbonPanel))) pClone = NULL;
//     m_pPanel = (CMFCRibbonPanel*)pClone;
//     m_pPanel->CopyFrom(*pPanel);                    // panel slot +0x170 -- NO null check in retail
//     CommonInit();
//     m_pPanelOrigin = pPanel;
//     m_pPanel->m_pParentMenuBar = this;              // +0x160
//     m_pPanel->m_btnLaunch(+0x170).SetParentMenu(this);           // element slot +0x2e8 (the CMFCRibbonLaunchButton, 0x278 bytes)
//     for (i = 0; i < m_pPanel->m_arElements.GetSize(); i++)       // +0x6d8 / +0x6e0
//         m_pPanel->m_arElements[i]->SetParentMenu(this);          // element slot +0x2e8
//     m_pRibbonBar = m_pPanel->m_pParent->m_pParentRibbonBar;      // panel +0x158, category +0x760
// (a negative or out-of-range index throws AfxThrowInvalidArgException; it
// cannot happen in the loop as written.)
// DEVIATIONS: (1) the clone is attempted exactly as retail does, but OpenMFC's
// CMFCRibbonPanel is IMPLEMENT_DYNAMIC, so CreateObject yields NULL unless the
// client's panel class is DYNCREATE; a NULL clone is tolerated (retail would
// fault) and leaves m_pPanel NULL.  (2) The element SetParentMenu calls go
// through the exported thunk, over the element list the ribbon_state side table
// holds for the clone (the embedded launch button at +0x170 is not modeled).
// (3) m_pRibbonBar is recovered from the side tables of the SOURCE panel
// (PanelParentCategory -> CategoryParentBar), which is the value retail's
// CopyFrom would have propagated into the clone's +0x158.
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, CMFCRibbonPanel* pPanel) {
    if (pThis == nullptr) return nullptr;
    ConstructPrologue(pThis);

    CMFCRibbonPanel* pClone = nullptr;
    if (pPanel != nullptr) {
        CObject* pObj = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(RuntimeClassOf(pPanel));
        if (pObj != nullptr && IsKindOfRt(pObj, impl__GetThisClass_CMFCRibbonPanel__SAPEAUCRuntimeClass__XZ())) {
            pClone = static_cast<CMFCRibbonPanel*>(pObj);
        }
    }
    PanelOf(pThis) = pClone;
    if (pClone != nullptr) {
        impl__CopyFrom_CMFCRibbonPanel__MEAAXAEAV1__Z(pClone, *pPanel);
    }
    impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(pThis);
    At<CMFCRibbonPanel*>(pThis, kOffPanelOrigin) = pPanel;
    if (pClone != nullptr) {
        // (pClone->m_pParentMenuBar = this and the default button's SetParentMenu are skipped: unmodeled)
        CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arElements;
        impl__GetElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(pClone, arElements);
        for (int i = 0; i < arElements.GetSize(); i++) {
            impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
                arElements[i], reinterpret_cast<CMFCRibbonPanelMenuBar*>(pThis));
        }
    }
    RibbonBarOf(pThis) = SideTableParentBar(SideTableParentCategory(pPanel));
    return pThis;
}

// CMFCRibbonPanelMenuBar(CMFCRibbonCategory* pCategory, CSize size): retail RVA
// 0x11dfc0 (mfc140u).  CSize arrives by value in R8.  After the shared prologue:
//     m_pPanel = NULL;
//     CommonInit();
//     m_pCategory = (CMFCRibbonCategory*)pCategory->GetRuntimeClass()->CreateObject();  // no NULL / IsKindOf check
//     m_pCategory->CopyFrom(*pCategory);                       // category slot +0x1b8
//     m_pCategory->m_pParentMenuBar = this;                    // +0x768
//     for (i = 0; i < m_pCategory->m_arPanels.GetSize(); i++) {   // +0x7b0
//         CMFCRibbonPanel* pPanel = m_pCategory->GetPanel(i);  // 0xeda20
//         pPanel->m_pParentMenuBar = this;                     // +0x160
//         pPanel->m_btnLaunch(+0x170).SetParentMenu(this);     // element slot +0x2e8 (CMFCRibbonLaunchButton, 0x278 bytes)
//         pPanel->m_btnDefault(+0x3f0).SetParentMenu(this);    // element slot +0x2e8 (CMFCRibbonDefaultPanelButton, 0x280 bytes)
//         for (j ...) pPanel->m_arElements[j]->SetParentMenu(this);   // +0x6d8/+0x6e0, slot +0x2e8
//     }
//     m_sizeCategory = size;
//     m_pRibbonBar = m_pCategory->m_pParentRibbonBar;          // +0x760
// DEVIATIONS: a NULL clone (OpenMFC's CMFCRibbonCategory is IMPLEMENT_DYNAMIC;
// only a client DYNCREATE category clones) is tolerated where retail would
// fault; the +0x768 / +0x160 back-links and the two embedded buttons are not
// modeled; the per-element SetParentMenu goes through the exported thunk over
// the side-table element lists; m_pRibbonBar comes from the SOURCE category's
// side-table entry (the value retail's CopyFrom propagates into +0x760).
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonCategory@@VCSize@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonCategory__VCSize___Z(
    void* pThis, CMFCRibbonCategory* pCategory, long long size) {
    if (pThis == nullptr) return nullptr;
    ConstructPrologue(pThis);
    PanelOf(pThis) = nullptr;
    impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(pThis);

    CMFCRibbonCategory* pClone = nullptr;
    if (pCategory != nullptr) {
        pClone = static_cast<CMFCRibbonCategory*>(
            impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(RuntimeClassOf(pCategory)));
    }
    CategoryOf(pThis) = pClone;
    if (pClone != nullptr) {
        impl__CopyFrom_CMFCRibbonCategory__UEAAXAEAV1__Z(pClone, *pCategory);
        // (pClone->m_pParentMenuBar = this is skipped: unmodeled)
        const int nPanels = impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(pClone);
        for (int i = 0; i < nPanels; i++) {
            CMFCRibbonPanel* pPanel = impl__GetPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__H_Z(pClone, i);
            if (pPanel == nullptr) continue;   // retail dereferences it unconditionally
            // (pPanel->m_pParentMenuBar = this and the two embedded buttons are skipped: unmodeled)
            CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arElements;
            impl__GetElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(pPanel, arElements);
            for (int j = 0; j < arElements.GetSize(); j++) {
                impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
                    arElements[j], reinterpret_cast<CMFCRibbonPanelMenuBar*>(pThis));
            }
        }
    }
    const POINT sz = UnpackPoint(size);
    At<SIZE>(pThis, kOffSizeCategory) = SIZE{sz.x, sz.y};
    RibbonBarOf(pThis) = SideTableParentBar(pCategory);
    return pThis;
}

// ~CMFCRibbonPanelMenuBar(): retail RVA 0x11e430 (mfc140u), transcribed:
//     vfptr = &CMFCRibbonPanelMenuBar::`vftable';
//     if (m_pPanel != NULL) {
//         if (m_pRibbonBar != NULL && m_pRibbonBar->m_pKeyboardNavLevelCurrent == m_pPanel)   // +0xc10
//             m_pRibbonBar->DeactivateKeyboardFocus(FALSE);                                    // 0xe1cd0
//         if (m_pPanel != NULL) delete m_pPanel;                    // vslot 1 (scalar deleting dtor), flag 1
//     }
//     if (m_pCategory != NULL) {
//         if (m_pRibbonBar != NULL && m_pRibbonBar->m_pKeyboardNavLevelCurrent == m_pCategory)
//             m_pRibbonBar->DeactivateKeyboardFocus(FALSE);
//         if (m_pCategory != NULL) delete m_pCategory;
//         if (m_pRibbonBar != NULL && m_pRibbonBar->m_pActiveCategory != NULL &&               // +0xb08
//             (!m_pRibbonBar->m_bQuickAccessToolbarOnTop || !m_pRibbonBar->m_bReplaceFrameCaption)) {   // +0x434, +0x468
//             CMFCRibbonCategory* pActive = m_pRibbonBar->m_pActiveCategory;
//             pActive->m_Tab.m_bIsDroppedDown = FALSE;               // +0x22c (m_Tab at +0xd8, element +0x154)
//             pActive->m_Tab.m_bIsHighlighted = FALSE;               // +0x214 (element +0x13c)
//             CRect rc = pActive->m_Tab.m_rect;                      // +0x1a0 (element +0xc8)
//             rc.bottom = m_pRibbonBar->m_QAToolbar.m_rect.bottom;   // bar +0x1454 (QAT element rect at +0x1380+0xc8)
//             rc.InflateRect(1, 1);
//             ::RedrawWindow(m_pRibbonBar->m_hWnd, &rc, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//         }
//     }
//     if (m_bHasKeyTips && CMFCPopupMenu::m_pActivePopupMenu != NULL &&        // 0x3be288
//         CWnd::FromHandlePermanent(m_pActivePopupMenu->m_hWnd) != NULL &&    // 0x28adc0
//         ::IsWindowVisible(m_pActivePopupMenu->m_hWnd))
//         CMFCPopupMenu::UpdateAllShadows(NULL);                              // 0xbacd0
//     m_wndScrollBarVert.~CScrollBar();                                        // 0x2943f0
//     CMFCPopupMenuBar::~CMFCPopupMenuBar();                                   // 0xbc0f0
// The owned objects are destroyed exactly as retail does it, through vtable
// slot 1 with the delete flag set (see DeleteThroughVtable): every object that
// can reach m_pPanel / m_pCategory here comes out of CRuntimeClass::CreateObject,
// i.e. from a client DYNCREATE factory with the MSVC layout, where slot 1 is the
// scalar deleting destructor.  DEVIATIONS: the active-category repaint block
// writes two m_Tab flags and reads the m_Tab rect of the retail category,
// which the clean-room category does not have, so that block is not performed
// (its condition is evaluated and documented only); m_pPanel / m_pCategory are
// additionally cleared after the delete (retail leaves them dangling); the
// embedded scroll bar is torn down with the exported ~CScrollBar thunk, as
// retail.
// Symbol: ??1CMFCRibbonPanelMenuBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonPanelMenuBar__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    CMFCRibbonBar* pBar = RibbonBarOf(pThis);

    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel != nullptr) {
        if (pBar != nullptr && pBar->m_pKeyboardNavLevelCurrent == static_cast<void*>(pPanel)) {
            impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(pBar, FALSE);
        }
        pPanel = PanelOf(pThis);
        if (pPanel != nullptr) {
            DeleteThroughVtable(pPanel);
            PanelOf(pThis) = nullptr;
        }
    }

    CMFCRibbonCategory* pCategory = CategoryOf(pThis);
    if (pCategory != nullptr) {
        if (pBar != nullptr && pBar->m_pKeyboardNavLevelCurrent == static_cast<void*>(pCategory)) {
            impl__DeactivateKeyboardFocus_CMFCRibbonBar__QEAAXH_Z(pBar, FALSE);
        }
        pCategory = CategoryOf(pThis);
        if (pCategory != nullptr) {
            DeleteThroughVtable(pCategory);
            CategoryOf(pThis) = nullptr;
        }
        pBar = RibbonBarOf(pThis);
        if (pBar != nullptr && pBar->m_pActiveCategory != nullptr &&
            (!pBar->m_bQuickAccessToolbarOnTop || !pBar->m_bReplaceFrameCaption)) {
            // Retail clears the active category's m_Tab.m_bIsDroppedDown /
            // m_Tab.m_bIsHighlighted and repaints m_Tab.m_rect extended down to
            // the QAT rect (see above); those members are not modeled, so
            // nothing is done here.
        }
    }

    if (At<int>(pThis, kOffHasKeyTips) != 0) {
        void* pActive = impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
        if (pActive != nullptr) {
            HWND hWndActive = HWndOf(pActive);
            if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hWndActive) != nullptr &&
                ::IsWindowVisible(hWndActive)) {
                impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(nullptr);
            }
        }
    }

    impl___1CScrollBar__UEAA_XZ(ScrollBarOf(pThis));
    impl___1CMFCPopupMenuBar__UEAA_XZ(pThis);
}

// AddButtons(CMFCRibbonBar* pRibbonBar, const CArray<...>& arButtons, BOOL
// bFloatyMode): retail RVA 0x11e170 (mfc140u), transcribed:
//     m_bSimpleMode = TRUE;
//     m_pRibbonBar = pRibbonBar;
//     m_pPanel->m_pParentMenuBar = this;          // +0x160 -- no NULL check in retail
//     m_pPanel->m_bFloatyMode = bFloatyMode;      // +0xc0
//     m_pPanel->m_nXMargin = m_pPanel->m_nYMargin = 2;   // +0xf0, +0xf4
//     m_pPanel->RemoveAll();                      // 0x116c10
//     for (i = 0; i < arButtons.GetSize(); i++) {          // MSVC CArray: m_pData +8, m_nSize +0x10
//         CMFCRibbonBaseElement* pSrc = arButtons[i];
//         CMFCRibbonBaseElement* pClone = (CMFCRibbonBaseElement*)pSrc->GetRuntimeClass()->CreateObject();  // no NULL check
//         pClone->CopyFrom(*pSrc);                // element slot +0x2e0
//         pClone->SetOriginal(pSrc);              // element slot +0x2f0
//         pClone->m_bCompactMode = TRUE;          // +0x12c
//         pClone->SetParentMenu(this);            // element slot +0x2e8
//         m_pPanel->Add(pClone);                  // panel slot +0x1a8
//     }
// DEVIATIONS: the four panel member stores are unmodeled and skipped; a NULL
// m_pPanel (the OpenMFC constructors, see the file header) skips RemoveAll and
// the loop -- with no panel there is nowhere to add the clones, and cloning
// them only to leak them would be worse than not cloning; a clone that
// CreateObject cannot produce (the element class is not DYNCREATE) is skipped
// where retail would fault; the +0x12c flag is unmodeled.
// Symbol: ?AddButtons@CMFCRibbonPanelMenuBar@@IEAAXPEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void MS_ABI impl__AddButtons_CMFCRibbonPanelMenuBar__IEAAXPEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, CMFCRibbonBar* pRibbonBar, const void* pArButtons, int bFloatyMode) {
    (void)bFloatyMode;   // only stored into the unmodeled panel +0xc0
    if (pThis == nullptr) return;
    At<int>(pThis, kOffSimpleMode) = 1;
    RibbonBarOf(pThis) = pRibbonBar;
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr || pArButtons == nullptr) return;   // retail: unconditional (see above)
    // (panel +0x160 / +0xc0 / +0xf0 / +0xf4 stores skipped: unmodeled)
    impl__RemoveAll_CMFCRibbonPanel__QEAAXXZ(pPanel);

    CMFCRibbonBaseElement* const* pData = At<CMFCRibbonBaseElement* const*>(pArButtons, kOffMsArrayData);
    const INT_PTR nSize = At<INT_PTR>(pArButtons, kOffMsArraySize);
    for (INT_PTR i = 0; i < nSize; i++) {
        CMFCRibbonBaseElement* pSrc = pData[i];
        if (pSrc == nullptr) continue;   // retail dereferences it unconditionally
        CObject* pObj = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(RuntimeClassOf(pSrc));
        if (pObj == nullptr) continue;   // retail dereferences it unconditionally
        CMFCRibbonBaseElement* pClone = static_cast<CMFCRibbonBaseElement*>(pObj);
        impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(pClone, *pSrc);
        impl__SetOriginal_CMFCRibbonBaseElement__UEAAXPEAV1__Z(pClone, pSrc);
        // (pClone->m_bCompactMode = TRUE skipped: unmodeled)
        impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
            pClone, reinterpret_cast<CMFCRibbonPanelMenuBar*>(pThis));
        impl__Add_CMFCRibbonPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(pPanel, pClone);
    }
}

// AdjustLocations(): retail RVA 0x11e5f0 (mfc140u), transcribed:
//     if (m_bInUpdateShadow) return;                       // CMFCToolBar +0x111c
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CClientDC dc(this);                                  // 0x2a3b20
//     CFont* pOldFont = dc.SelectObject(<ribbon bar font>);   // see SelectRibbonFont
//     if (pOldFont == NULL) AfxThrowInvalidArgException();    // 0x227720
//     if (m_pCategory != NULL) {
//         m_pCategory->m_rect = rectClient;                // category +0xc0
//         m_pCategory->RecalcLayout(&dc);                  // category slot +0x170
//     } else if (m_pPanel != NULL) {
//         m_pPanel->m_bSizeIsLocked = m_bResizeTracking;   // panel +0xd4 <- CMFCPopupMenuBar +0x13a4
//         m_pPanel->m_nScrollOffset = m_iOffset;           // panel +0xf8 <- CMFCPopupMenuBar +0x1364
//         m_pPanel->Reposition(&dc, rectClient);           // panel slot +0x1e8
//         m_pPanel->OnAfterChangeRect(&dc);                // panel slot +0x210
//         CMFCRibbonBar* pBar = GetTopLevelRibbonBar();    // 0x1202f0
//         if (pBar != NULL && pBar->m_pKeyboardNavLevelCurrent == m_pPanel)   // +0xc10
//             pBar->ShowKeyTips(TRUE);                     // 0xe32c0
//         m_pPanel->m_bSizeIsLocked = FALSE;               // panel +0xd4
//     }
//     dc.SelectObject(pOldFont);
// DEVIATIONS: the category rect (+0xc0) and the panel +0xd4/+0xf8 stores are
// unmodeled and skipped; the RecalcLayout / Reposition / OnAfterChangeRect
// calls go to the exported thunks statically.
// Symbol: ?AdjustLocations@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (At<int>(pThis, kOffInUpdateShadow) != 0) return;

    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rectClient);
    ClientDCScope dc(AsWnd(pThis));
    CFont* pOldFont = SelectRibbonFont(pThis, dc.dc());
    if (pOldFont == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    CMFCRibbonCategory* pCategory = CategoryOf(pThis);
    if (pCategory != nullptr) {
        // (pCategory->m_rect = rectClient skipped: unmodeled)
        impl__RecalcLayout_CMFCRibbonCategory__UEAAXPEAVCDC___Z(pCategory, dc.dc());
    } else if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
        // (panel +0xd4 / +0xf8 stores skipped: unmodeled)
        const CRect rc(rectClient);
        impl__Reposition_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(pPanel, dc.dc(), rc);
        impl__OnAfterChangeRect_CMFCRibbonPanel__MEAAXPEAVCDC___Z(pPanel, dc.dc());
        CMFCRibbonBar* pBar = impl__GetTopLevelRibbonBar_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBar__XZ(pThis);
        if (pBar != nullptr && pBar->m_pKeyboardNavLevelCurrent == static_cast<void*>(PanelOf(pThis))) {
            impl__ShowKeyTips_CMFCRibbonBar__QEAAXH_Z(pBar, TRUE);
        }
        // (panel +0xd4 = 0 skipped: unmodeled)
    }
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(dc.dc(), pOldFont);
}

// SetPreferedSize(CSize size): retail RVA 0x11e7a0 (mfc140u; absent from
// mfc140u_rva_symbols.json, resolved through the mfc140u export directory as
// ordinal 13478).  CSize arrives by value in RDX (cx low dword, cy high dword).
// Transcribed:
//     int nMinWidth = 0;
//     if (m_pPanel != NULL && m_pPanel->m_pPaletteButton != NULL)             // +0x168
//         nMinWidth = m_pPanel->GetPaltteMinSize().cx - ::GetSystemMetrics(SM_CXVSCROLL) - 2;   // 0x11c930
//     m_sizePrefered.cy = size.cy;
//     m_sizePrefered.cx = max(size.cx, nMinWidth);
// DEVIATION: panel +0x168 (m_pPaletteButton) is unmodeled and read as NULL,
// so the guard fails, GetPaltteMinSize is not called and nMinWidth stays 0.
// Symbol: ?SetPreferedSize@CMFCRibbonPanelMenuBar@@IEAAXVCSize@@@Z
extern "C" void MS_ABI impl__SetPreferedSize_CMFCRibbonPanelMenuBar__IEAAXVCSize___Z(void* pThis, long long size) {
    if (pThis == nullptr) return;
    const POINT sz = UnpackPoint(size);
    const int nMinWidth = 0;   // m_pPanel->m_pPaletteButton (+0x168) is unmodeled: NULL
    SizePreferedOf(pThis).cy = sz.y;
    SizePreferedOf(pThis).cx = std::max(static_cast<int>(sz.x), nMinWidth);
}

// CalcSize(BOOL bVertDock) -> CSize: retail RVA 0x11e820 (mfc140u).  MS x64 ABI:
// RCX = this, RDX = the hidden CSize return slot (returned in RAX), R8D =
// bVertDock (never read).  Transcribed:
//     if (m_pCategory != NULL) return m_sizeCategory;                 // 0x11e857
//     m_pPanel->m_bIsQATPopup = m_bIsQATPopup;                        // panel +0xc4 -- no NULL check
//     CClientDC dc(m_pRibbonBar);                                     // 0x2a3b20 on the BAR
//     CFont* pOldFont = dc.SelectObject(<ribbon bar font>); if (!pOldFont) AfxThrowInvalidArgException();
//     if (m_bIsMenuMode) {                                            // 0x11e8e7
//         m_pPanel->m_bMenuMode = TRUE;                               // +0xb8
//         m_pPanel->m_bIsDefaultMenuLook = m_bIsDefaultMenuLook;      // +0xbc
//         m_pPanel->Reposition(&dc, CRect(0, 0, m_sizePrefered.cx, m_sizePrefered.cy));   // slot +0x1e8
//         dc.SelectObject(pOldFont);
//         CSize size(m_pPanel->m_rect.Width(), m_pPanel->m_rect.Height());       // +0x10c..+0x118
//         if (m_sizePrefered != CSize(0,0)) {
//             size.cx = max(size.cx, m_sizePrefered.cx);
//             if (m_sizePrefered.cy > 0) {
//                 if (m_pPanel->m_pPaletteButton != NULL) size.cy = max(size.cy, m_sizePrefered.cy);   // +0x168
//                 else {
//                     if (size.cy > m_sizePrefered.cy) {
//                         CMFCPopupMenu* pParent = <parent if IsKindOf(CMFCPopupMenu)>;
//                         if (pParent != NULL) pParent->+0x16a4 = TRUE;   // scroll flag
//                     }
//                     size.cy = m_sizePrefered.cy;
//                 }
//             }
//         }
//         return size;
//     }
//     if (m_bSimpleMode && m_pPanel->m_arWidths.GetSize() == 0) {      // +0x6b8 (CArray<int,int> m_arWidths: data +0x6b0, count +0x6b8)
//         AfxGetModuleState()->m_pCurrentWinApp->BeginWaitCursor();   // 0x133930, +8, 0x1de7b0
//         m_pPanel->RecalcWidths(&dc, 0x7fff);                        // slot +0x1a0
//         AfxGetModuleState()->m_pCurrentWinApp->EndWaitCursor();     // the CWaitCursor dtor at 0x7687c: 0x133930, +8, 0x1de7e0
//     }
//     if (m_pPanel->m_arWidths.GetSize() == 0) { dc.SelectObject(pOldFont); return CSize(10, 10); }   // 0x11ea68
//     <three further layouts pick a width out of m_arWidths (+0xb0 / +0xc0 / m_bIsQATPopup
//      select which entry) and a height, call Reposition (slot +0x1e8), then either return
//      the panel rect size or, in simple/non-QAT mode, the union of the element rects (+0xc8)
//      plus twice the panel margins (+0xf0/+0xf4)>   // 0x11ea7f..0x11eca9
// DEVIATIONS: every panel member above is unmodeled and read as zero, so with a
// panel present the reachable paths are: menu mode -> the panel rect is empty
// and the result is m_sizePrefered clamped as above; regular mode -> the
// m_arWidths count (+0x6b8) is unmodeled and read as 0 both before and after
// RecalcWidths, so retail's own CSize(10, 10) terminal is returned.  A NULL m_pPanel (the
// OpenMFC constructors) takes the same CSize(10, 10) terminal instead of
// faulting.  The wait cursor is raised and lowered through the exported
// CCmdTarget::BeginWaitCursor / EndWaitCursor on the current CWinApp, as
// retail's inlined CWaitCursor does; the +0x16a4 scroll flag of the parent
// popup is written at its retail offset, as CMFCPopupMenu.cpp does.
// Symbol: ?CalcSize@CMFCRibbonPanelMenuBar@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__CalcSize_CMFCRibbonPanelMenuBar__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bVertDock) {
    (void)bVertDock;   // never read by retail
    if (pRet == nullptr) return nullptr;
    if (pThis == nullptr) { pRet->cx = 0; pRet->cy = 0; return pRet; }

    if (CategoryOf(pThis) != nullptr) {
        const SIZE sz = At<SIZE>(pThis, kOffSizeCategory);
        pRet->cx = sz.cx;
        pRet->cy = sz.cy;
        return pRet;
    }

    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    // (panel->m_bIsQATPopup = m_bIsQATPopup skipped: unmodeled)
    ClientDCScope dc(RibbonBarOf(pThis));
    CFont* pOldFont = SelectRibbonFont(pThis, dc.dc());
    if (pOldFont == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        pRet->cx = 0; pRet->cy = 0;
        return pRet;
    }
    const SIZE pref = SizePreferedOf(pThis);

    if (At<int>(pThis, kOffIsMenuMode) != 0) {
        // (panel +0xb8 / +0xbc stores skipped: unmodeled)
        if (pPanel != nullptr) {
            const CRect rc(0, 0, pref.cx, pref.cy);
            impl__Reposition_CMFCRibbonPanel__MEAAXPEAVCDC__AEBVCRect___Z(pPanel, dc.dc(), rc);
        }
        impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(dc.dc(), pOldFont);
        int cx = 0, cy = 0;   // the panel rect (+0x10c) is unmodeled: empty
        if (pref.cx != 0 || pref.cy != 0) {
            cx = std::max(cx, static_cast<int>(pref.cx));
            if (pref.cy > 0) {
                // panel +0x168 (m_pPaletteButton) unmodeled: NULL -> the non-palette branch
                if (cy > pref.cy) {
                    if (CWnd* pParent = ParentPopupMenu(pThis)) {
                        At<int>(pParent, kOffPopupScrollable) = 1;
                    }
                }
                cy = pref.cy;
            }
        }
        pRet->cx = cx;
        pRet->cy = cy;
        return pRet;
    }

    if (At<int>(pThis, kOffSimpleMode) != 0 && pPanel != nullptr /* && m_arWidths.GetSize() == 0: unmodeled, read as 0 */) {
        // AfxGetModuleState()->m_pCurrentWinApp is what AfxGetApp() inlines to;
        // OpenMFC's AFX_MODULE_STATE is not retail-shaped, so the exported
        // AfxGetApp is used for the same lookup.
        CCmdTarget* pApp = static_cast<CCmdTarget*>(impl__AfxGetApp__YAPEAVCWinApp__XZ());
        if (pApp != nullptr) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
        impl__RecalcWidths_CMFCRibbonPanel__UEAAXPEAVCDC__H_Z(pPanel, dc.dc(), 0x7fff);
        if (pApp != nullptr) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);   // ~CWaitCursor
    }
    // The panel's m_arWidths count (+0x6b8) is unmodeled and read as 0, which
    // is retail's CSize(10, 10) terminal at 0x11ea68; a NULL panel lands there too.
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(dc.dc(), pOldFont);
    pRet->cx = 10;
    pRet->cy = 10;
    return pRet;
}

// DoPaint(CDC* pDC): retail RVA 0x11ecc0 (mfc140u), transcribed:
//     CMemDC memDC(*pDC, this);                            // 0x69f50
//     CDC* pPaint = memDC.m_bMemDC ? &memDC.m_dcMem : &memDC.m_dc;   // GetDC()
//     CRect rectClip(0,0,0,0); pDC->GetClipBox(&rectClip); // CDC vslot +0xb0
//     CRgn rgnClip;                                        // vftable 0x2e29e0
//     if (!rectClip.IsRectEmpty()) { rgnClip.Attach(::CreateRectRgnIndirect(&rectClip)); pPaint->SelectClipRgn(&rgnClip); }
//     CFont* pOldFont = pPaint->SelectObject(<ribbon bar font>);      // CDC vslot +0x60
//     if (pOldFont == NULL) AfxThrowInvalidArgException();
//     pPaint->SetBkMode(TRANSPARENT);                      // 0x2a2860
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CRect rect = rectClient; rect.InflateRect(3, 3);
//     if (m_pCategory != NULL) {
//         CMFCVisualManager::GetInstance()->OnDrawRibbonCategory(pPaint, m_pCategory, rect);   // VM slot +0x438
//         m_pCategory->OnDraw(pPaint);                     // category slot +0x178
//     } else {
//         CMFCRibbonCategory* pCategory = m_pPanel->m_pParent;        // +0x158 -- no NULL check on m_pPanel
//         if (pCategory != NULL) {
//             CMFCRibbonPanelMenuBar* pSaved = pCategory->m_pParentMenuBar;   // +0x768
//             pCategory->m_pParentMenuBar = this;
//             CMFCVisualManager::GetInstance()->OnDrawRibbonCategory(pPaint, pCategory, rect);
//             pCategory->m_pParentMenuBar = pSaved;
//         } else if (m_bIsQATPopup) {
//             CMFCVisualManager::GetInstance()->OnFillRibbonQuickAccessToolBarPopup(pPaint, this, rectClient);   // VM slot +0x558
//         } else {
//             CMFCVisualManager::GetInstance()->OnFillBarBackground(pPaint, this, rectClient, rectClient, FALSE); // VM slot +0x78
//         }
//         m_pPanel->DoPaint(pPaint);                       // panel slot +0x1e0
//     }
//     pPaint->SelectObject(pOldFont);                      // CDC vslot +0x60
//     pPaint->SelectClipRgn(NULL);
//     <~CRgn (0x1c6f0), ~CMemDC (0x6a380)>
// DEVIATIONS: the clip region is handled with raw GDI on the paint DC's m_hDC
// (OpenMFC's CDC::SelectClipRgn does exactly ::SelectClipRgn(m_hDC, hRgn)); the
// panel->category link comes from the ribbon_state side table and the +0x768
// swap is not performed (unmodeled); the visual manager is the exported
// m_pVisManager static (see VisualManager()); OnFillRibbonQuickAccessToolBarPopup
// is reached through its exported base thunk because OpenMFC's C++ manager has
// no such virtual; nothing is painted when no visual manager exists yet.
// Symbol: ?DoPaint@CMFCRibbonPanelMenuBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonPanelMenuBar__UEAAXPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) return;

    alignas(8) unsigned char memDCStorage[sizeof(S_Cmemdc)];
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(memDCStorage, pDC, AsWnd(pThis));
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memDCStorage);
    CDC* pPaint = pMemDC->m_bMemDC ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem) : static_cast<CDC*>(pMemDC->m_dc);

    RECT rectClip = {0, 0, 0, 0};
    impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(pDC, &rectClip);
    HRGN hRgnClip = nullptr;
    if (!::IsRectEmpty(&rectClip)) {
        hRgnClip = ::CreateRectRgnIndirect(&rectClip);
        ::SelectClipRgn(pPaint->m_hDC, hRgnClip);
    }

    CFont* pOldFont = SelectRibbonFont(pThis, pPaint);
    if (pOldFont == nullptr) {
        if (hRgnClip != nullptr) { ::SelectClipRgn(pPaint->m_hDC, nullptr); ::DeleteObject(hRgnClip); }
        impl___1CMemDC__UEAA_XZ(memDCStorage);
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__SetBkMode_CDC__QEAAHH_Z(pPaint, TRANSPARENT);

    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rectClient);
    RECT rect = rectClient;
    ::InflateRect(&rect, 3, 3);

    CMFCVisualManager* pVM = VisualManager();
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        if (pVM != nullptr) pVM->OnDrawRibbonCategory(pPaint, pCategory, CRect(rect));
        impl__OnDraw_CMFCRibbonCategory__UEAAXPEAVCDC___Z(pCategory, pPaint);
    } else {
        CMFCRibbonPanel* pPanel = PanelOf(pThis);   // retail dereferences it unconditionally
        CMFCRibbonCategory* pParentCategory = SideTableParentCategory(pPanel);
        if (pParentCategory != nullptr) {
            // (the +0x768 m_pParentMenuBar swap is skipped: unmodeled)
            if (pVM != nullptr) pVM->OnDrawRibbonCategory(pPaint, pParentCategory, CRect(rect));
        } else if (At<int>(pThis, kOffIsQATPopup) != 0) {
            if (pVM != nullptr) {
                impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
                    pVM, pPaint, pThis, CRect(rectClient));
            }
        } else {
            if (pVM != nullptr) {
                pVM->OnFillBarBackground(pPaint, static_cast<CBasePane*>(AsToolBar(pThis)), CRect(rectClient), CRect(rectClient), FALSE);
            }
        }
        if (pPanel != nullptr) impl__DoPaint_CMFCRibbonPanel__MEAAXPEAVCDC___Z(pPanel, pPaint);
    }

    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pPaint, pOldFont);
    ::SelectClipRgn(pPaint->m_hDC, nullptr);
    if (hRgnClip != nullptr) ::DeleteObject(hRgnClip);
    impl___1CMemDC__UEAA_XZ(memDCStorage);
}

// OnDrawMenuBorder(CDC* pDC): retail RVA 0x11fb80 (mfc140u), a pure forwarder:
//     if (m_pCategory != NULL) m_pCategory->OnDrawMenuBorder(pDC, this);   // category slot +0x1c0
//     else                     m_pPanel->OnDrawMenuBorder(pDC, this);      // panel slot +0x228 -- no NULL check
// In the base vftables both slots hold the inline `{}` of afxribboncategory.h
// / afxribbonpanel.h (the shared `ret` stub at 0x27d0: CMFCRibbonCategory
// 0x3014c8 +0x1c0, CMFCRibbonPanel 0x309390 +0x228).  CMFCRibbonMainPanel DOES
// override it: its vftable (0x3079a8) has +0x228 = the exported
// ?OnDrawMenuBorder@CMFCRibbonMainPanel@@ (0x1109f0), which is what the
// application-menu popup reaches.  As everywhere in this file the dispatch is
// made statically (IsKindOf(CMFCRibbonMainPanel) selects the override thunk);
// a category, a plain panel and a NULL panel (retail: fault) do nothing.
// Symbol: ?OnDrawMenuBorder@CMFCRibbonPanelMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonPanelMenuBar__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return;
    if (CategoryOf(pThis) != nullptr) return;   // CMFCRibbonCategory::OnDrawMenuBorder is `{}`
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel != nullptr &&
        IsKindOfRt(pPanel, impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ())) {
        impl__OnDrawMenuBorder_CMFCRibbonMainPanel__MEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar___Z(
            pPanel, pDC, reinterpret_cast<CMFCRibbonPanelMenuBar*>(pThis));
    }
    // CMFCRibbonPanel::OnDrawMenuBorder is `{}`.
}

// GetTopLevelRibbonBar() const: retail RVA 0x1202f0 (mfc140u), transcribed:
//     if (m_pRibbonBar != NULL) return m_pRibbonBar;
//     return m_pPanelOrigin->m_pParent->m_pParentRibbonBar;   // +0x1568 -> +0x158 -> +0x760, no NULL checks
// DEVIATION: the two retail links are unmodeled; the ribbon_state side tables
// give the same panel -> category -> bar chain for a source panel that sits in
// a category of a bar, and NULL otherwise (where retail would fault).
// Symbol: ?GetTopLevelRibbonBar@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBar@@XZ
extern "C" CMFCRibbonBar* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBar__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    if (CMFCRibbonBar* pBar = RibbonBarOf(pThis)) return pBar;
    return SideTableParentBar(SideTableParentCategory(At<CMFCRibbonPanel*>(pThis, kOffPanelOrigin)));
}

// HitTest(CPoint point) const: retail RVA 0x120df0 (mfc140u).  CPoint by value
// in RDX.  Transcribed:
//     if (m_pCategory != NULL) return m_pCategory->HitTest(point, TRUE);   // 0xee510
//     return m_pPanel->HitTest(point, FALSE);                              // panel slot +0x180, no NULL check
// A NULL m_pPanel yields NULL instead of a fault.
// Symbol: ?HitTest@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    const void* pThis, long long point) {
    if (pThis == nullptr) return nullptr;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        return impl__HitTest_CMFCRibbonCategory__QEBAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(pCategory, ToCPoint(UnpackPoint(point)), TRUE);
    }
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr) return nullptr;
    return impl__HitTest_CMFCRibbonPanel__UEAAPEAVCMFCRibbonBaseElement__VCPoint__H_Z(pPanel, point, FALSE);
}

// HitTestEx(CPoint point) const: retail RVA 0x120e30 (mfc140u), transcribed:
//     if (m_pCategory != NULL) return m_pCategory->HitTestEx(point);   // 0xee600
//     return m_pPanel->HitTestEx(point);                               // panel slot +0x188, no NULL check
// A NULL m_pPanel yields -1 (the miss value both callees return) instead of a fault.
// Symbol: ?HitTestEx@CMFCRibbonPanelMenuBar@@QEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanelMenuBar__QEBAHVCPoint___Z(const void* pThis, long long point) {
    if (pThis == nullptr) return -1;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        return impl__HitTestEx_CMFCRibbonCategory__QEBAHVCPoint___Z(pCategory, ToCPoint(UnpackPoint(point)));
    }
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr) return -1;
    return impl__HitTestEx_CMFCRibbonPanel__UEBAHVCPoint___Z(pPanel, point);
}

// GetFocused() const: retail RVA 0x120e60 (mfc140u), transcribed:
//     if (m_pCategory != NULL) return m_pCategory->GetFocused();   // tail call 0xeff30
//     return m_pPanel->GetFocused();                               // tail call 0x11cee0, no NULL check
// Symbol: ?GetFocused@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        return impl__GetFocused_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(pCategory);
    }
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr) return nullptr;
    return impl__GetFocused_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(pPanel);
}

// GetDroppedDown() const: retail RVA 0x120da0 (mfc140u), transcribed:
//     CMFCRibbonBaseElement* p = m_pCategory != NULL ? m_pCategory->GetDroppedDown()   // 0xef200
//                                                    : m_pPanel->GetDroppedDown();      // 0x11b960, no NULL check
//     if (p != NULL && !p->IsKindOf(RUNTIME_CLASS(CMFCRibbonButton))) p = NULL;         // 0x300a80
//     return (CMFCRibbonButton*)p;
// Symbol: ?GetDroppedDown@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonButton@@XZ
extern "C" CMFCRibbonButton* MS_ABI impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    CMFCRibbonBaseElement* p = nullptr;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        p = impl__GetDroppedDown_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(pCategory);
    } else if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
        p = impl__GetDroppedDown_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(pPanel);
    }
    if (p != nullptr && !IsRibbonButton(p)) p = nullptr;
    return static_cast<CMFCRibbonButton*>(p);
}

// FindByOrigin(CMFCRibbonBaseElement* pOrigin) const: retail RVA 0x120220
// (mfc140u), transcribed:
//     if (m_pPanel == NULL) return NULL;
//     CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> ar;
//     m_pPanel->GetElements(ar);                                    // 0x11b6d0
//     for (i = 0; i < ar.GetSize(); i++)                            // out-of-range -> AfxThrowInvalidArgException (unreachable)
//         if (CMFCRibbonBaseElement* p = ar[i]->FindByOriginal(pOrigin)) return p;   // element slot +0x360
//     return NULL;
// The element call goes to the exported base FindByOriginal statically.
// Symbol: ?FindByOrigin@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@PEAV2@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByOrigin_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__PEAV2__Z(
    const void* pThis, CMFCRibbonBaseElement* pOrigin) {
    if (pThis == nullptr) return nullptr;
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr) return nullptr;
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> ar;
    impl__GetElements_CMFCRibbonPanel__QEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(pPanel, ar);
    for (int i = 0; i < ar.GetSize(); i++) {
        if (ar[i] == nullptr) continue;
        if (CMFCRibbonBaseElement* p = impl__FindByOriginal_CMFCRibbonBaseElement__UEAAPEAV1_PEAV1__Z(ar[i], pOrigin)) {
            return p;
        }
    }
    return nullptr;
}

// PopTooltip(): retail RVA 0x120190 (mfc140u; absent from mfc140u_rva_symbols.json,
// resolved through the mfc140u export directory as ordinal 11750), transcribed:
//     if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL)
//         ::SendMessage(m_pToolTip->m_hWnd, TTM_POP, 0, 0);          // 0x41c
// Symbol: ?PopTooltip@CMFCRibbonPanelMenuBar@@QEAAXXZ
extern "C" void MS_ABI impl__PopTooltip_CMFCRibbonPanelMenuBar__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CMFCToolTipCtrl* pTip = ToolTipOf(pThis);
    if (pTip == nullptr) return;
    HWND hWndTip = HWndOf(pTip);
    if (hWndTip == nullptr) return;
    ::SendMessage(hWndTip, TTM_POP, 0, 0);
}

// SetActive(BOOL bIsActive): retail RVA 0x1201c0 (mfc140u; absent from
// mfc140u_rva_symbols.json, resolved through the mfc140u export directory as
// ordinal 12888), transcribed:
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CMFCRibbonPanelMenu)))   // 0x30b010
//         ((CMFCRibbonPanelMenu*)pParent)->SetActive(bIsActive);   // CMFCRibbonPanelMenu vslot +0x438
// CMFCRibbonPanelMenu::SetActive is the inline `{}` of afxribbonpanelmenu.h and
// is not exported, so after the parent lookup nothing remains to be done.
// Symbol: ?SetActive@CMFCRibbonPanelMenuBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetActive_CMFCRibbonPanelMenuBar__QEAAXH_Z(void* pThis, int bIsActive) {
    (void)bIsActive;
    if (pThis == nullptr) return;
    CWnd* pParent = ParentRibbonPanelMenu(pThis);
    (void)pParent;   // pParent->SetActive(bIsActive) is the inline empty body
}

// CloseDelayedSubMenu(): retail RVA 0x120670 (mfc140u), transcribed:
//     if (m_pDelayedCloseButton != NULL) {
//         ::KillTimer(m_hWnd, 0xec1c);
//         m_pDelayedCloseButton->ClosePopupMenu();     // element slot +0x340
//         m_pDelayedCloseButton = NULL;
//     }
// Symbol: ?CloseDelayedSubMenu@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CMFCRibbonBaseElement* pButton = DelayedCloseButtonOf(pThis);
    if (pButton == nullptr) return;
    ::KillTimer(HWndOf(pThis), kTimerDelayedClose);
    impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(DelayedCloseButtonOf(pThis));
    DelayedCloseButtonOf(pThis) = nullptr;
}

// RestoreDelayedSubMenu(): retail RVA 0x1206c0 (mfc140u), transcribed:
//     if (m_pDelayedCloseButton == NULL || m_pPanel == NULL) return;
//     m_pDelayedCloseButton->m_bToBeClosed = FALSE;                    // button +0x20c
//     CMFCRibbonBaseElement* pOld = m_pPanel->m_pHighlighted;          // +0x3e8
//     m_pPanel->Highlight(TRUE, m_pDelayedCloseButton->m_rect.TopLeft());   // panel slot +0x238, the 8 bytes at +0xc8 as a CPoint
//     BOOL bChanged = FALSE;
//     if (m_pDelayedCloseButton != pOld) {
//         if (m_pDelayedCloseButton != NULL) ::InvalidateRect(m_hWnd, &m_pDelayedCloseButton->m_rect, TRUE);
//         if (pOld != NULL)                  ::InvalidateRect(m_hWnd, &pOld->m_rect, TRUE);
//         bChanged = TRUE;
//     }
//     m_pDelayedCloseButton = NULL; ::KillTimer(m_hWnd, 0xec1c);
//     if (bChanged) ::UpdateWindow(m_hWnd);
// DEVIATIONS: the button +0x20c flag, the panel's m_pHighlighted (+0x3e8) and the
// element rects (+0xc8) are unmodeled.  The highlight is re-issued through the
// exported Highlight thunk with point (0,0) in place of the unreadable rect
// corner; pOld is read as NULL, so the "changed" branch is taken whenever a
// delayed button exists, without the two InvalidateRect calls (no rects) --
// the timer is killed, the member cleared and UpdateWindow run as retail.
// Symbol: ?RestoreDelayedSubMenu@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CMFCRibbonBaseElement* pDelayed = DelayedCloseButtonOf(pThis);
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pDelayed == nullptr || pPanel == nullptr) return;
    // (pDelayed->+0x20c = FALSE skipped: unmodeled)
    CMFCRibbonBaseElement* pOld = nullptr;   // panel +0x3e8 unmodeled: NULL
    impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(pPanel, TRUE, PackPoint(POINT{0, 0}));
    bool bChanged = false;
    if (DelayedCloseButtonOf(pThis) != pOld) {
        // (InvalidateRect of the two element rects skipped: unmodeled)
        bChanged = true;
    }
    DelayedCloseButtonOf(pThis) = nullptr;
    ::KillTimer(HWndOf(pThis), kTimerDelayedClose);
    if (bChanged) ::UpdateWindow(HWndOf(pThis));
}

// OnClickButton(CMFCRibbonButton* pButton, CPoint point): retail RVA 0x11f550
// (mfc140u; vslot +0x8b0).  `point` (R8) is never read.  Transcribed:
//     CRect rect = pButton->m_rect;                     // +0xc8 -- no NULL check on pButton
//     pButton->m_bIsPressed = FALSE;                    // +0x144
//     pButton->m_bIsHighlighted = FALSE;                // +0x13c
//     ::RedrawWindow(m_hWnd, &rect, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//     if (m_pPanel != NULL && m_pPanel->m_pPaletteButton != NULL &&                   // +0x168
//         m_pPanel->m_pPaletteButton->OnClickPaletteSubItem(pButton, this)) return;   // gallery slot +0x540
//     BOOL bNotified = pButton->NotifyCommand(TRUE);    // 0x10ab0
//     if (m_bIsFloaty) {
//         CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//         if (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CMFCRibbonMiniToolBar))) return;   // 0x3085a8
//     }
//     if (bNotified) {
//         CWnd* pParent = <parent if IsKindOf(CMFCRibbonPanelMenu)>;                   // 0x30b010
//         if (pParent != NULL) {
//             CMFCPopupMenu* pGrand = ((CMFCPopupMenu*)pParent)->GetParentPopupMenu();   // 0xb7550
//             if (pGrand != NULL && pGrand->IsKindOf(RUNTIME_CLASS(CMFCRibbonPanelMenu)))
//                 ((CMFCRibbonPanelMenu*)pGrand)->m_bForceClose = TRUE;                // +0x2f58
//         }
//     }
//     AFXGetParentFrame(this)->DestroyWindow();         // 0x6bd00; CFrameWnd vslot +0xd0 is ?DestroyWindow@CWnd@@ (0x28baf0) -- no NULL check
// DEVIATIONS: the button's rect and two flags are unmodeled, so the redraw
// covers the whole client area (the only rect this file can name) and the
// flag stores are skipped; panel +0x168 is unmodeled (read as NULL, the
// gallery hook is skipped); NotifyCommand(TRUE) is called through its exported
// thunk, whose definition is still an argument-ignoring `return 0` placeholder
// in this tree (header request), so today bNotified reads FALSE and the
// m_bForceClose branch cannot fire; the parent frame's DestroyWindow (which is
// what closes the popup) goes through the exported CWnd thunk statically, and
// a NULL frame is skipped instead of faulting.
// Symbol: ?OnClickButton@CMFCRibbonPanelMenuBar@@UEAAXPEAVCMFCRibbonButton@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCRibbonPanelMenuBar__UEAAXPEAVCMFCRibbonButton__VCPoint___Z(
    void* pThis, CMFCRibbonButton* pButton, long long point) {
    (void)point;   // never read by retail
    if (pThis == nullptr || pButton == nullptr) return;   // retail dereferences pButton unconditionally
    // (pButton->m_bIsPressed / m_bIsHighlighted = FALSE skipped: unmodeled)
    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rect);   // retail: the button's own rect (+0xc8), unmodeled
    ::RedrawWindow(HWndOf(pThis), &rect, nullptr, kRdwInvalidateEraseUpdate);
    // (m_pPanel->m_pPaletteButton->OnClickPaletteSubItem skipped: +0x168 unmodeled)
    const int bNotified = impl__NotifyCommand_CMFCRibbonBaseElement__QEAAHH_Z(
        reinterpret_cast<CMFCRibbonBaseElement*>(pButton), TRUE);   // 0x10ab0; placeholder today (see above)
    if (At<int>(pThis, kOffIsFloaty) != 0) {
        if (ParentWndOfKind(pThis, impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ()) != nullptr) {
            return;
        }
    }
    if (bNotified) {
        if (CWnd* pParent = ParentRibbonPanelMenu(pThis)) {
            void* pGrand = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pParent);
            if (IsKindOfRt(static_cast<CObject*>(static_cast<CWnd*>(pGrand)),
                           impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ())) {
                At<int>(pGrand, kOffPanelMenuForceClose) = 1;
            }
        }
    }
    if (CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(AsWnd(pThis))) {
        impl__DestroyWindow_CWnd__UEAAHXZ(pFrame);
    }
}

// OnChangeHighlighted(CMFCRibbonBaseElement* pHot): retail RVA 0x11f6c0 (mfc140u;
// vslot +0x8b8).  The whole body sits behind one gate:
//     if (m_pPanel == NULL || !m_pPanel->m_bMenuMode) return;   // +0xb8
// and then (transcribed for the record):
//     CMFCRibbonButton* pDropped = <m_pPanel->GetDroppedDown() if IsKindOf(CMFCRibbonButton)>;   // 0x11b960
//     CMFCRibbonButton* pHotButton = <pHot if IsKindOf(CMFCRibbonButton)>;
//     if (pDropped != NULL && pHot == NULL) return;
//     BOOL bChanged = pDropped != pHot;
//     if (pHotButton == NULL || pDropped != pHotButton || pHotButton->m_rectMenu.IsRectEmpty() ||   // +0x198
//         !pHotButton->IsMenuAreaHighlighted()) {                                           // button slot +0x4c0
//         if (bChanged) {
//             CWnd* pParentMenu = <parent if IsKindOf(CMFCRibbonPanelMenu)>;
//             if (pDropped != NULL) {
//                 const MSG* pMsg = CWnd::GetCurrentMessage();                              // 0x28ac10
//                 if (!CMFCToolBar::m_bCustomizeMode && (pMsg == NULL || pMsg->message != WM_KEYDOWN)) {
//                     m_pDelayedCloseButton = pDropped; pDropped->m_bToBeClosed = TRUE;     // +0x20c
//                     ::SetTimer(m_hWnd, 0xec1c, CMFCPopupMenuBar::m_uiPopupTimerDelay - 1, NULL);
//                     pDropped->Redraw();                                                   // element slot +0x380
//                 } else {
//                     ::KillTimer(m_hWnd, 0xec1c); m_pDelayedCloseButton = NULL;
//                     pDropped->ClosePopupMenu();                                           // element slot +0x340
//                     if (pParentMenu != NULL)
//                         CMFCPopupMenu::ActivatePopupMenu(g_pTopLevelFrame ? g_pTopLevelFrame : GetTopLevelFrame(), pParentMenu);
//                 }
//             }
//             if (pHotButton != NULL && pHotButton->HasMenu()) {                            // element slot +0x280
//                 if (m_pDelayedButton != NULL) ::KillTimer(m_hWnd, 0xec1b);
//                 m_pDelayedButton = pHotButton;
//                 if (pHotButton == m_pDelayedCloseButton) {
//                     if (pHotButton->m_rectMenu.IsRectEmpty() || <cursor inside m_rectMenu>) {
//                         RestoreDelayedSubMenu();                                          // own slot +0x898
//                         m_pDelayedButton = NULL;
//                     }
//                 } else ::SetTimer(m_hWnd, 0xec1b, CMFCPopupMenuBar::m_uiPopupTimerDelay, NULL);
//             }
//             if (pParentMenu != NULL) {
//                 CMFCRibbonPanelMenuBar* pParentBar = pParentMenu->GetParentRibbonMenuBar();   // 0x121230
//                 if (pParentBar != NULL && pParentBar->m_pDelayedCloseButton == pParentMenu->m_pParentRibbonElement)
//                     pParentBar->RestoreDelayedSubMenu();
//                 if (pParentMenu->m_pParentRibbonElement != NULL)                             // +0x19b8
//                     pParentMenu->m_pParentRibbonElement->OnChangeMenuHighlight(this, pHotButton);   // element slot +0x408
//             }
//         }
//     } else if (pHotButton != NULL && pHotButton == m_pDelayedCloseButton) {
//         m_pDelayedCloseButton->m_bToBeClosed = FALSE; m_pDelayedCloseButton = NULL; ::KillTimer(m_hWnd, 0xec1c);
//     }
//     if (pHot == NULL) { <parent CMFCRibbonPanelMenu>->m_pParentRibbonElement->OnChangeMenuHighlight(this, NULL); }
// DEVIATION: panel +0xb8 (m_bMenuMode) is unmodeled and read as 0, so the gate
// returns at once for every panel; the rest of the body additionally needs the
// button members +0x198/+0x20c, the unexported element virtuals (HasMenu,
// IsMenuAreaHighlighted, Redraw, OnChangeMenuHighlight) and the placeholder
// ?GetParentRibbonMenuBar@CMFCRibbonPanelMenu@@ (header request), so it is not
// transcribed beyond the gate.
// Symbol: ?OnChangeHighlighted@CMFCRibbonPanelMenuBar@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__OnChangeHighlighted_CMFCRibbonPanelMenuBar__UEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, CMFCRibbonBaseElement* pHot) {
    (void)pHot;
    if (pThis == nullptr) return;
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel == nullptr) return;
    // pPanel->m_bMenuMode (+0xb8) is unmodeled and read as 0: retail returns here.
    return;
}

// OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler): retail RVA 0x11fa70
// (mfc140u; vslot +0x498), transcribed:
//     CMFCRibbonCmdUI state;                    // vftable 0x300020; zeroed, m_pOther (+0x20) = this, m_pUpdated (+0x40) = NULL
//     if (m_pCategory != NULL) m_pCategory->OnUpdateCmdUI(&state, pTarget, bDisableIfNoHndler);   // category slot +0x1a0
//     else                     m_pPanel->OnUpdateCmdUI(&state, pTarget, bDisableIfNoHndler);      // panel slot +0x200 -- no NULL check
//     CWnd::UpdateDialogControls(pTarget, bDisableIfNoHndler);              // 0x291460
//     if (bDisableIfNoHndler && m_bSetKeyTips) {
//         if (m_pPanel != NULL) {
//             if (m_pPanel->GetDroppedDown() == NULL)                       // 0x11b960
//                 m_pRibbonBar->SetKeyboardNavigationLevel(m_pPanel, FALSE);   // 0xe1db0 -- no NULL check on the bar
//         } else if (m_pCategory != NULL) {
//             m_pRibbonBar->SetKeyboardNavigationLevel(m_pCategory, FALSE);
//         }
//         m_bSetKeyTips = FALSE;
//         CMFCPopupMenu::UpdateAllShadows(NULL);                             // 0xbacd0
//     }
// The CMFCRibbonCmdUI is built with its exported constructor and m_pOther is
// stored at the retail offset; a NULL m_pPanel with no category skips the
// update call instead of faulting, and a NULL m_pRibbonBar skips the
// SetKeyboardNavigationLevel call.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonPanelMenuBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonPanelMenuBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;
    alignas(8) unsigned char cmdUI[kRibbonCmdUISize];
    std::memset(cmdUI, 0, sizeof cmdUI);
    impl___0CMFCRibbonCmdUI__QEAA_XZ(cmdUI);
    At<void*>(cmdUI, kOffCmdUIOther) = pThis;

    CMFCRibbonCategory* pCategory = CategoryOf(pThis);
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pCategory != nullptr) {
        impl__OnUpdateCmdUI_CMFCRibbonCategory__UEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(pCategory, cmdUI, pTarget, bDisableIfNoHndler);
    } else if (pPanel != nullptr) {
        impl__OnUpdateCmdUI_CMFCRibbonPanel__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
            pPanel, reinterpret_cast<CMFCRibbonCmdUI*>(cmdUI), pTarget, bDisableIfNoHndler);
    }
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(AsWnd(pThis), pTarget, bDisableIfNoHndler);

    if (bDisableIfNoHndler != 0 && At<int>(pThis, kOffSetKeyTips) != 0) {
        CMFCRibbonBar* pBar = RibbonBarOf(pThis);
        pPanel = PanelOf(pThis);
        if (pPanel != nullptr) {
            if (impl__GetDroppedDown_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(pPanel) == nullptr && pBar != nullptr) {
                impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(pBar, PanelOf(pThis), FALSE);
            }
        } else if (CMFCRibbonCategory* pCat = CategoryOf(pThis)) {
            if (pBar != nullptr) impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(pBar, pCat, FALSE);
        }
        At<int>(pThis, kOffSetKeyTips) = 0;
        impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(nullptr);
    }
}

// OnKey(UINT nChar): retail RVA 0x1207e0 (mfc140u; vslot +0x810), transcribed:
//     if ((nChar == VK_F10 && ::GetKeyState(VK_SHIFT) < 0) || nChar == VK_APPS) {
//         OnContextMenu(this, CPoint(-1, -1));                        // 0x1209b0
//         return TRUE;
//     }
//     if (m_pRibbonBar->ProcessKey(nChar)) return TRUE;               // 0xe2f30 -- no NULL check
//     if (m_pPanel != NULL) {
//         CMFCDisableMenuAnimation guard;   // stack object, vftable 0x300048:
//             // ctor: m_animType = CMFCPopupMenu::GetAnimationType(FALSE);   // the helper at 0x2e600
//             //       CMFCPopupMenu::m_AnimationType = NO_ANIMATION (0);     // 0x3be290
//         m_pPanel->OnKey(nChar);                                     // panel slot +0x1c8; result discarded
//             // dtor: CMFCPopupMenu::m_AnimationType = m_animType;
//         return TRUE;
//     }
//     if (m_pCategory != NULL) { <same guard>; m_pCategory->OnKey(nChar); return TRUE; }   // category slot +0x1e0
//     return FALSE;
// NOTE the saved value is GetAnimationType(FALSE), NOT the raw static: the
// helper at 0x2e600 returns m_AnimationType unless it is
// SYSTEM_DEFAULT_ANIMATION (999), which it resolves through afxGlobalData
// (init gate, then +0x26c bMenuAnimation == 0 -> NO_ANIMATION (0), else
// +0x270 bMenuFadeEffect ? FADE (3) : SLIDE (2)).  So after a key handled by
// the panel/category, retail leaves the static holding the RESOLVED value
// when it held 999 before; ResolvedAnimationType() above reproduces that.
// A NULL m_pRibbonBar skips ProcessKey.
// Symbol: ?OnKey@CMFCRibbonPanelMenuBar@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanelMenuBar__UEAAHI_Z(void* pThis, unsigned int nChar) {
    if (pThis == nullptr) return FALSE;
    if ((nChar == VK_F10 && ::GetKeyState(VK_SHIFT) < 0) || nChar == VK_APPS) {
        impl__OnContextMenu_CMFCRibbonPanelMenuBar__IEAAXPEAVCWnd__VCPoint___Z(pThis, AsWnd(pThis), PackPoint(POINT{-1, -1}));
        return TRUE;
    }
    if (CMFCRibbonBar* pBar = RibbonBarOf(pThis)) {
        if (impl__ProcessKey_CMFCRibbonBar__IEAAHH_Z(pBar, static_cast<int>(nChar)) != 0) return TRUE;
    }
    if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
        const std::uint32_t savedAnimation = ResolvedAnimationType();   // GetAnimationType(FALSE)
        impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A = 0;
        impl__OnKey_CMFCRibbonPanel__UEAAHI_Z(pPanel, nChar);
        impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A = savedAnimation;
        return TRUE;
    }
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        const std::uint32_t savedAnimation = ResolvedAnimationType();   // GetAnimationType(FALSE)
        impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A = 0;
        impl__OnKey_CMFCRibbonCategory__UEAAHI_Z(pCategory, nChar);
        impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A = savedAnimation;
        return TRUE;
    }
    return FALSE;
}

// OnSetAccData(long lVal): retail RVA 0x1208e0 (mfc140u; vslot +0x4b8), transcribed:
//     CPoint pt(LOWORD(lVal), HIWORD(lVal));         // zero-extended words
//     ::ScreenToClient(m_hWnd, &pt);
//     CMFCRibbonBaseElement* pHit = HitTest(pt);     // 0x120df0
//     if (pHit == NULL) return FALSE;
//     m_AccData.Clear();                             // inlined: role/state/hit/rect/point zeroed, the six CStrings Empty()'d
//     return pHit->SetACCData(this, m_AccData);      // element slot +0x168
// The final SetACCData goes through the exported base thunk statically; its
// definition is still an argument-ignoring `return 0` placeholder in this tree
// (header request), so today the hit path returns FALSE after clearing the
// block.
// Symbol: ?OnSetAccData@CMFCRibbonPanelMenuBar@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonPanelMenuBar__UEAAHJ_Z(void* pThis, long lVal) {
    if (pThis == nullptr) return FALSE;
    POINT pt;
    pt.x = static_cast<int>(static_cast<unsigned long>(lVal) & 0xffffu);
    pt.y = static_cast<int>((static_cast<unsigned long>(lVal) >> 16) & 0xffffu);
    ::ScreenToClient(HWndOf(pThis), &pt);
    CMFCRibbonBaseElement* pHit = impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(pThis, PackPoint(pt));
    if (pHit == nullptr) return FALSE;
    CAccessibilityData& data = At<CAccessibilityData>(pThis, kOffAccData);
    data.m_nAccRole = 0;
    data.m_bAccState = 0;
    data.m_nAccHit = 0;
    data.m_rectAccLocation = CRect(0, 0, 0, 0);
    data.m_ptAccHit = CPoint(0, 0);
    data.m_strAccName.Empty();
    data.m_strAccValue.Empty();
    data.m_strDescription.Empty();
    data.m_strAccKeys.Empty();
    data.m_strAccHelp.Empty();
    data.m_strAccDefAction.Empty();
    return impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(pHit, AsWnd(pThis), data);   // 0x11200
}

// PreTranslateMessage(MSG* pMsg): retail RVA 0x120e80 (mfc140u; vslot +0x228), transcribed:
//     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && OnKey(VK_TAB))   // own slot +0x810
//         return TRUE;
//     if (pMsg->message == WM_LBUTTONDOWN) {
//         CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//         if (pFocus != NULL && pFocus->IsKindOf(RUNTIME_CLASS(CMFCRibbonRichEditCtrl))) {   // 0x305a38
//             CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//             pFocus->m_edit (+0xe8: the CMFCRibbonEdit& right after the 0xe8-byte CWnd)
//                 ->PreLMouseDown(pt);   // CMFCRibbonEdit vftable 0x3051a8 slot +0x510 = ?PreLMouseDown@CMFCRibbonEdit@@UEAAHVCPoint@@@Z (0xfc710); result discarded
//         }
//     }
//     return CMFCToolBar::PreTranslateMessage(pMsg);              // 0x154940
// DEVIATION: OpenMFC has no CMFCRibbonRichEditCtrl class and its exported ctor
// thunk (featurepack/ribbon/CMFCRibbonRichEditCtrl.cpp) never stores the
// CMFCRibbonEdit& it is given, so the +0xe8 owner link cannot be read; the
// PreLMouseDown notification is therefore not forwarded (its OpenMFC thunk is a
// `return 0` placeholder anyway).  The focus lookup and the fall-through are as
// retail.
// Symbol: ?PreTranslateMessage@CMFCRibbonPanelMenuBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCRibbonPanelMenuBar__MEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB) {
        if (impl__OnKey_CMFCRibbonPanelMenuBar__UEAAHI_Z(pThis, VK_TAB) != 0) return TRUE;
    }
    if (pMsg->message == WM_LBUTTONDOWN) {
        CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
        if (IsKindOfRt(pFocus, impl__GetThisClass_CMFCRibbonRichEditCtrl__SAPEAUCRuntimeClass__XZ())) {
            POINT pt = {0, 0};
            ::GetCursorPos(&pt);
            ::ScreenToClient(HWndOf(pThis), &pt);
            // (pFocus->m_edit.PreLMouseDown(pt) skipped: the +0xe8 link is never written in OpenMFC, see above)
        }
    }
    return impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(AsToolBar(pThis), pMsg);
}

// OnCreate(LPCREATESTRUCT lpcs): retail RVA 0x11fbc0 (mfc140u; message map
// WM_CREATE), transcribed:
//     if (CMFCPopupMenuBar::OnCreate(lpcs) == -1) return -1;                // 0xbf180
//     if (!m_bIsFloaty || m_bIsQATPopup) {
//         CTooltipManager::CreateToolTip(m_pToolTip, this, AFX_TOOLTIP_TYPE_RIBBON);   // 0x1822c0, 0x200
//         if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL) {
//             CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//             ::SendMessage(m_pToolTip->m_hWnd, TTM_SETMAXTIPWIDTH (0x418), 0, 640);
//             m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK (-1), &rectClient, GetDlgCtrlID());   // 0x275060, 0x2a99a0
//         }
//     }
//     if (m_pPanel != NULL && m_pPanel->m_pPaletteButton != NULL) {          // +0x168
//         m_wndScrollBarVert.Create(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, 1);   // CScrollBar vslot +0x2d8
//         m_pPanel->m_pScrollBar = &m_wndScrollBarVert;                     // +0x150
//     }
//     if (m_pRibbonBar != NULL && m_pRibbonBar->m_nKeyboardNavLevel >= 0)    // +0x414
//         m_bSetKeyTips = m_bHasKeyTips = TRUE;
//     ::GetCursorPos(&m_ptStartMenu);
//     return 0;
// DEVIATION: panel +0x168 is unmodeled and read as NULL, so the scroll bar is
// not created (and +0x150 not written); the scroll-bar Create would have gone
// to the exported CScrollBar::Create thunk.
// Symbol: ?OnCreate@CMFCRibbonPanelMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCRibbonPanelMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, LPCREATESTRUCTW lpcs) {
    if (pThis == nullptr) return -1;
    if (impl__OnCreate_CMFCPopupMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(AsPopupMenuBar(pThis), lpcs) == -1) return -1;

    if (At<int>(pThis, kOffIsFloaty) == 0 || At<int>(pThis, kOffIsQATPopup) != 0) {
        CToolTipCtrl** ppTip = reinterpret_cast<CToolTipCtrl**>(&ToolTipOf(pThis));
        impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(ppTip, AsWnd(pThis), kAfxTooltipTypeRibbon);
        CToolTipCtrl* pTip = *ppTip;
        if (pTip != nullptr && HWndOf(pTip) != nullptr) {
            RECT rectClient = {0, 0, 0, 0};
            ::GetClientRect(HWndOf(pThis), &rectClient);
            ::SendMessage(HWndOf(pTip), TTM_SETMAXTIPWIDTH, 0, kTooltipMaxWidth);
            impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
                pTip, AsWnd(pThis), LPSTR_TEXTCALLBACKW, &rectClient,
                static_cast<unsigned __int64>(static_cast<long long>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(AsWnd(pThis)))));
        }
    }
    // (m_pPanel->m_pPaletteButton (+0x168) is unmodeled: read as NULL, no scroll bar)
    CMFCRibbonBar* pBar = RibbonBarOf(pThis);
    if (pBar != nullptr && pBar->m_nKeyboardNavLevel >= 0) {
        At<int>(pThis, kOffSetKeyTips) = 1;
        At<int>(pThis, kOffHasKeyTips) = 1;
    }
    ::GetCursorPos(&PtStartMenuOf(pThis));
    return 0;
}

// OnDestroy(): retail RVA 0x11fd30 (mfc140u; message map WM_DESTROY), transcribed:
//     if (m_pToolTip != NULL) CTooltipManager::DeleteToolTip(m_pToolTip);   // 0x1824a0
//     CMFCPopupMenuBar::OnDestroy();                                        // tail jump 0xbdd80
// Symbol: ?OnDestroy@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CToolTipCtrl** ppTip = reinterpret_cast<CToolTipCtrl**>(&ToolTipOf(pThis));
    if (*ppTip != nullptr) {
        impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(ppTip);
    }
    impl__OnDestroy_CMFCPopupMenuBar__IEAAXXZ(AsPopupMenuBar(pThis));
}

// OnSize(UINT nType, int cx, int cy): retail RVA 0x11fd60 (mfc140u; message map
// WM_SIZE), transcribed:
//     CMFCToolBar::OnSize(nType, cx, cy);                                   // 0x155860 -- CMFCToolBar's, not CMFCPopupMenuBar's
//     if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL)
//         m_pToolTip->SetToolRect(this, GetDlgCtrlID(), CRect(0, 0, cx, cy));   // 0x275480, 0x2a99a0
// Symbol: ?OnSize@CMFCRibbonPanelMenuBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCRibbonPanelMenuBar__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    if (pThis == nullptr) return;
    impl__OnSize_CMFCToolBar__IEAAXIHH_Z(AsToolBar(pThis), nType, cx, cy);
    CToolTipCtrl* pTip = reinterpret_cast<CToolTipCtrl*>(ToolTipOf(pThis));
    if (pTip != nullptr && HWndOf(pTip) != nullptr) {
        const RECT rect = {0, 0, cx, cy};
        impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
            pTip, AsWnd(pThis),
            static_cast<unsigned __int64>(static_cast<long long>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(AsWnd(pThis)))), &rect);
    }
}

// OnUpdateToolTips(WPARAM wp, LPARAM lp): retail RVA 0x1200d0 (mfc140u; message
// map entry for the registered AFX_WM_UPDATETOOLTIPS message), transcribed:
//     if ((wp & AFX_TOOLTIP_TYPE_RIBBON) && (!m_bIsFloaty || m_bIsQATPopup)) {
//         CTooltipManager::CreateToolTip(m_pToolTip, this, AFX_TOOLTIP_TYPE_RIBBON);   // 0x1822c0
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         ::SendMessage(m_pToolTip->m_hWnd, TTM_SETMAXTIPWIDTH, 0, 640);      // no NULL check on m_pToolTip
//         m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK, &rectClient, GetDlgCtrlID());
//     }
//     return 0;                                    // the single terminal at 0x120174
// A NULL tooltip after CreateToolTip is tolerated (retail would fault); lp is
// never read.
// Symbol: ?OnUpdateToolTips@CMFCRibbonPanelMenuBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCRibbonPanelMenuBar__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wp, __int64 lp) {
    (void)lp;
    if (pThis == nullptr) return 0;
    if ((wp & kAfxWmUpdateToolTipsMask) != 0 &&
        (At<int>(pThis, kOffIsFloaty) == 0 || At<int>(pThis, kOffIsQATPopup) != 0)) {
        CToolTipCtrl** ppTip = reinterpret_cast<CToolTipCtrl**>(&ToolTipOf(pThis));
        impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(ppTip, AsWnd(pThis), kAfxTooltipTypeRibbon);
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(HWndOf(pThis), &rectClient);
        CToolTipCtrl* pTip = *ppTip;
        if (pTip != nullptr) {
            ::SendMessage(HWndOf(pTip), TTM_SETMAXTIPWIDTH, 0, kTooltipMaxWidth);
            impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(
                pTip, AsWnd(pThis), LPSTR_TEXTCALLBACKW, &rectClient,
                static_cast<unsigned __int64>(static_cast<long long>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(AsWnd(pThis)))));
        }
    }
    return 0;
}

// OnNeedTipText(UINT id, NMHDR* pNMH, LRESULT* pResult): retail RVA 0x11fde0
// (mfc140u; message map TTN_GETDISPINFOW reflected for every id), transcribed:
//     if (m_pToolTip == NULL || m_pToolTip->m_hWnd == NULL || pNMH->hwndFrom != m_pToolTip->m_hWnd) return FALSE;
//     if (CMFCPopupMenu::m_pActivePopupMenu != NULL &&
//         CWnd::FromHandle(::GetParent(m_hWnd)) != CMFCPopupMenu::m_pActivePopupMenu) return FALSE;
//     CMFCRibbonBar* pBar = GetTopLevelRibbonBar();                          // 0x1202f0
//     if (pBar != NULL && !pBar->m_bToolTip) return TRUE;                    // +0x450
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     CMFCRibbonBaseElement* pHit = HitTest(pt);                             // 0x120df0
//     if (pHit == NULL) return FALSE;
//     if (pHit->HasMenu() && m_pPanel != NULL && m_pPanel->IsMainPanel()) return TRUE;   // element slot +0x280, panel slot +0x178
//     static CString strTipText; strTipText = pHit->GetToolTipText();       // element slot +0x190, the static at 0x3c24a0
//     if (strTipText.IsEmpty()) return FALSE;
//     if (m_pToolTip->IsKindOf(RUNTIME_CLASS(CMFCToolTipCtrl))) {            // 0x31bfa0
//         CMFCToolTipCtrl* pTip = (CMFCToolTipCtrl*)m_pToolTip;
//         if (pBar != NULL) {
//             pTip->m_nFixedWidthRegular = pBar->m_nTooltipWidthRegular;     // +0x1a0 <- +0x41c
//             pTip->m_nFixedWidthWithImage = pBar->m_nTooltipWidthLargeImage;   // +0x1a4 <- +0x420
//         }
//         if (pBar == NULL || pBar->m_bToolTipDescr) pTip->SetDescription(pHit->GetDescription());   // tip slot +0x2e8, element slot +0x198
//         pTip->m_pRibbonButton = pHit->IsKindOf(RUNTIME_CLASS(CMFCRibbonButton)) ? pHit : NULL;     // +0x138
//         if (!m_bIsMenuMode && !(m_pPanel != NULL && m_pPanel->IsMainPanel())) {
//             CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow);       // computed, then unused
//             CRect rectElem = pHit->m_rect; ClientToScreen(&rectElem);       // +0xc8
//             pTip->m_ptLocation = CPoint(rectElem.left, rectElem.bottom);   // +0x198
//         }
//     }
//     if (m_bHasKeyTips) m_pToolTip->SetWindowPos(&CWnd::wndTopMost, -1, -1, -1, -1, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER (0x13));
//     ((TOOLTIPTEXT*)pNMH)->lpszText = (LPWSTR)strTipText;                  // +0x18
//     return TRUE;
// DEVIATIONS: ?GetToolTipText@CMFCRibbonBaseElement@@ is exported here with its
// hidden-return-slot parameter FIRST (the retail order is this, slot) -- calling
// it with the correct order would write a CString over the element (header
// request) -- so from the hit onward the text cannot be fetched and the body
// returns FALSE there, exactly as retail does for an element whose tooltip text
// is empty; the CMFCToolTipCtrl block and the topmost re-order are therefore
// unreachable and not written.  HasMenu is an unexported inline (FALSE in the
// base), so that early TRUE is folded away as well.
// Symbol: ?OnNeedTipText@CMFCRibbonPanelMenuBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCRibbonPanelMenuBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int id, NMHDR* pNMH, __int64* pResult) {
    (void)id;
    (void)pResult;
    if (pThis == nullptr || pNMH == nullptr) return FALSE;
    CMFCToolTipCtrl* pTip = ToolTipOf(pThis);
    if (pTip == nullptr) return FALSE;
    HWND hWndTip = HWndOf(pTip);
    if (hWndTip == nullptr || pNMH->hwndFrom != hWndTip) return FALSE;
    void* pActive = impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
    if (pActive != nullptr && static_cast<void*>(ParentWndOf(pThis)) != pActive) return FALSE;

    CMFCRibbonBar* pBar = impl__GetTopLevelRibbonBar_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBar__XZ(pThis);
    if (pBar != nullptr && !pBar->m_bToolTip) return TRUE;

    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    ::ScreenToClient(HWndOf(pThis), &pt);
    CMFCRibbonBaseElement* pHit = impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(pThis, PackPoint(pt));
    if (pHit == nullptr) return FALSE;
    // pHit->GetToolTipText(): not callable through the current thunk (see above);
    // an empty text is retail's FALSE terminal at 0x11ff4c.
    return FALSE;
}

// OnMouseMove(UINT nFlags, CPoint point): retail RVA 0x11efb0 (mfc140u; message
// map WM_MOUSEMOVE), transcribed:
//     CMFCPopupMenuBar::OnMouseMove(nFlags, point);                          // 0xbeaf0
//     if (m_pPanel != NULL) {
//         <afxGlobalData init gate>;
//         if (afxGlobalData.m_bEnableAccessibility) {                        // +0x278
//             int nHit = m_pPanel->HitTestEx(point);                         // panel slot +0x188
//             if (nHit != -1 && nHit != m_iAccHotItem) { m_iAccHotItem = nHit; ::SetTimer(m_hWnd, 0xec07, 500, NULL); }
//         }
//     }
//     if (m_pPanel != NULL && !m_pPanel->m_bMenuMode && m_pPanel->GetDroppedDown() != NULL) return;   // +0xb8, 0x11b960
//     if (m_pCategory != NULL && m_pCategory->GetDroppedDown() != NULL) return;                       // 0xef200
//     if (m_ptStartMenu != CPoint(-1, -1)) {
//         CPoint ptCursor; ::GetCursorPos(&ptCursor);
//         if (abs(ptCursor.x - m_ptStartMenu.x) < 10 && abs(ptCursor.y - m_ptStartMenu.y) < 10) return;
//         m_ptStartMenu = CPoint(-1, -1);
//     }
//     if (point == CPoint(-1, -1)) m_bTracked = FALSE;
//     else if (!m_bTracked) {
//         TRACKMOUSEEVENT tme = { sizeof tme, TME_LEAVE, m_hWnd, 0 }; m_bTracked = TRUE; ::TrackMouseEvent(&tme);
//         if (m_pCategory == NULL && m_pPanel != NULL) {
//             CMFCRibbonBaseElement* pPressed = m_pPanel->GetPressed();      // 0x11b8f0
//             if (pPressed != NULL && !(nFlags & MK_LBUTTON)) pPressed->m_bIsPressed = FALSE;   // +0x144
//         }
//     }
//     if (m_pCategory != NULL) m_pCategory->OnMouseMove(point);              // category slot +0x198
//     else if (m_pPanel != NULL) {
//         BOOL bWasHighlighted = m_pPanel->m_bIsHighlighted;                 // +0xa8
//         m_pPanel->Highlight(TRUE, point);                                  // panel slot +0x238
//         if (!bWasHighlighted) ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ALLCHILDREN);
//     }
// DEVIATIONS: panel +0xb8 and +0xa8 are unmodeled and read as 0 (so the
// dropped-down test is made and the redraw follows every Highlight); the element
// +0x144 store is skipped.
// Symbol: ?OnMouseMove@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    impl__OnMouseMove_CMFCPopupMenuBar__IEAAXIVCPoint___Z(AsPopupMenuBar(pThis), nFlags, point);
    const POINT pt = UnpackPoint(point);

    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    if (pPanel != nullptr) {
        EnsureGlobalDataInitialized();
        if (GlobalDataInt(kGlobalDataEnableAccessibility) != 0) {
            const int nHit = impl__HitTestEx_CMFCRibbonPanel__UEBAHVCPoint___Z(pPanel, point);
            if (nHit != -1 && nHit != At<int>(pThis, kOffAccHotItem)) {
                At<int>(pThis, kOffAccHotItem) = nHit;
                ::SetTimer(HWndOf(pThis), kTimerAccHot, 500, nullptr);
            }
        }
    }
    pPanel = PanelOf(pThis);
    if (pPanel != nullptr /* && !pPanel->m_bMenuMode: unmodeled, read as 0 */) {
        if (impl__GetDroppedDown_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(pPanel) != nullptr) return;
    }
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        if (impl__GetDroppedDown_CMFCRibbonCategory__QEAAPEAVCMFCRibbonBaseElement__XZ(pCategory) != nullptr) return;
    }
    POINT& ptStart = PtStartMenuOf(pThis);
    if (ptStart.x != -1 || ptStart.y != -1) {
        POINT ptCursor = {0, 0};
        ::GetCursorPos(&ptCursor);
        if (std::labs(ptCursor.x - ptStart.x) < 10 && std::labs(ptCursor.y - ptStart.y) < 10) return;
        ptStart.x = -1;
        ptStart.y = -1;
    }
    if (pt.x == -1 && pt.y == -1) {
        At<int>(pThis, kOffTracked) = 0;
    } else if (At<int>(pThis, kOffTracked) == 0) {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof tme;
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = HWndOf(pThis);
        tme.dwHoverTime = 0;
        At<int>(pThis, kOffTracked) = 1;
        ::TrackMouseEvent(&tme);
        if (CategoryOf(pThis) == nullptr) {
            if (CMFCRibbonPanel* p = PanelOf(pThis)) {
                CMFCRibbonBaseElement* pPressed = impl__GetPressed_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__XZ(p);
                if (pPressed != nullptr && (nFlags & MK_LBUTTON) == 0) {
                    // (pPressed->m_bIsPressed = FALSE skipped: unmodeled)
                }
            }
        }
    }
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        impl__OnMouseMove_CMFCRibbonCategory__UEAAXVCPoint___Z(pCategory, ToCPoint(pt));
    } else if (CMFCRibbonPanel* p = PanelOf(pThis)) {
        const int bWasHighlighted = 0;   // panel +0xa8 unmodeled: read as 0
        impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(p, TRUE, point);
        if (!bWasHighlighted) ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwInvalidateEraseUpdateAll);
    }
}

// OnMouseLeave(): retail RVA 0x11f1e0 (mfc140u; message map WM_MOUSELEAVE), transcribed:
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     if (!rectClient.PtInRect(pt)) {
//         OnMouseMove(0, CPoint(-1, -1));                                    // 0x11efb0
//         m_bTracked = FALSE;
//         if (m_pPanel != NULL) m_pPanel->Highlight(FALSE, CPoint(-1, -1));  // panel slot +0x238
//         ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ALLCHILDREN);
//     }
//     m_bTracked = FALSE;
// Symbol: ?OnMouseLeave@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    ::ScreenToClient(HWndOf(pThis), &pt);
    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rectClient);
    if (!::PtInRect(&rectClient, pt)) {
        impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(pThis, 0, PackPoint(POINT{-1, -1}));
        At<int>(pThis, kOffTracked) = 0;
        if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
            impl__Highlight_CMFCRibbonPanel__MEAAXHVCPoint___Z(pPanel, FALSE, PackPoint(POINT{-1, -1}));
        }
        ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwInvalidateEraseUpdateAll);
    }
    At<int>(pThis, kOffTracked) = 0;
}

// OnLButtonDown(UINT nFlags, CPoint point): retail RVA 0x11f3b0 (mfc140u;
// message map WM_LBUTTONDOWN), transcribed:
//     CMFCPopupMenuBar::OnLButtonDown(nFlags, point);                        // 0xbe770
//     CMFCRibbonButton* pDropped = GetDroppedDown();                         // 0x120da0
//     if (pDropped != NULL) pDropped->ClosePopupMenu();                      // element slot +0x340
//     OnMouseMove(nFlags, point);                                            // 0x11efb0
//     m_pPressed = NULL; m_rectAutoCommand.SetRectEmpty();
//     HWND hWnd = m_hWnd;
//     CMFCRibbonBaseElement* pPressed = m_pCategory != NULL ? m_pCategory->OnLButtonDown(point)   // category slot +0x188
//                                                           : m_pPanel->MouseButtonDown(point);   // panel slot +0x240, no NULL check
//     if (!::IsWindow(hWnd)) return;
//     m_pPressed = pPressed;
//     if (pPressed == NULL) return;
//     int nDelay = 100;
//     if (pPressed->IsAutoRepeatMode(nDelay)) {                              // element slot +0x2a8 (unexported inline, FALSE in the base)
//         ::SetTimer(m_hWnd, 0xec1d, nDelay, NULL);
//         m_bAutoCommandTimer = TRUE;
//         m_rectAutoCommand = m_pPressed->m_rect;                            // +0xc8
//     }
// DEVIATION: IsAutoRepeatMode is not exported and no OpenMFC element overrides
// it, so the base FALSE is folded in and the timer block is unreachable; a
// NULL m_pPanel with no category yields no pressed element instead of a fault.
// Symbol: ?OnLButtonDown@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    impl__OnLButtonDown_CMFCPopupMenuBar__IEAAXIVCPoint___Z(AsPopupMenuBar(pThis), nFlags, point);
    if (CMFCRibbonButton* pDropped = impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(pThis)) {
        impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(pDropped);
    }
    impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(pThis, nFlags, point);
    PressedOf(pThis) = nullptr;
    ::SetRectEmpty(&RectAutoCommandOf(pThis));

    const HWND hWnd = HWndOf(pThis);
    CMFCRibbonBaseElement* pPressed = nullptr;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        pPressed = impl__OnLButtonDown_CMFCRibbonCategory__UEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(pCategory, ToCPoint(UnpackPoint(point)));
    } else if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
        pPressed = impl__MouseButtonDown_CMFCRibbonPanel__MEAAPEAVCMFCRibbonBaseElement__VCPoint___Z(pPanel, point);
    }
    if (!::IsWindow(hWnd)) return;
    PressedOf(pThis) = pPressed;
    if (pPressed == nullptr) return;
    // pPressed->IsAutoRepeatMode(nDelay) is the unexported base `return FALSE`: no auto-command timer.
    (void)kDefaultAutoRepeatDelay;
}

// OnLButtonUp(UINT nFlags, CPoint point): retail RVA 0x11f2d0 (mfc140u; message
// map WM_LBUTTONUP), transcribed:
//     if (m_bAutoCommandTimer) {
//         ::KillTimer(m_hWnd, 0xec1d); m_bAutoCommandTimer = FALSE; m_pPressed = NULL; m_rectAutoCommand.SetRectEmpty();
//     }
//     HWND hWnd = m_hWnd;
//     CMFCPopupMenuBar::OnLButtonUp(nFlags, point);                          // 0xbe860
//     if (!::IsWindow(hWnd)) return;
//     if (m_pCategory != NULL) m_pCategory->OnLButtonUp(point);              // category slot +0x190
//     else                     m_pPanel->MouseButtonUp(point);               // panel slot +0x248, no NULL check
//     if (!::IsWindow(hWnd)) return;
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(hWnd, &pt);
//     OnMouseMove(nFlags, pt);                                               // 0x11efb0
// Symbol: ?OnLButtonUp@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    if (At<int>(pThis, kOffAutoCommandTimer) != 0) {
        ::KillTimer(HWndOf(pThis), kTimerAutoCommand);
        At<int>(pThis, kOffAutoCommandTimer) = 0;
        PressedOf(pThis) = nullptr;
        ::SetRectEmpty(&RectAutoCommandOf(pThis));
    }
    const HWND hWnd = HWndOf(pThis);
    impl__OnLButtonUp_CMFCPopupMenuBar__IEAAXIVCPoint___Z(AsPopupMenuBar(pThis), nFlags, point);
    if (!::IsWindow(hWnd)) return;
    if (CMFCRibbonCategory* pCategory = CategoryOf(pThis)) {
        impl__OnLButtonUp_CMFCRibbonCategory__UEAAXVCPoint___Z(pCategory, ToCPoint(UnpackPoint(point)));
    } else if (CMFCRibbonPanel* pPanel = PanelOf(pThis)) {
        impl__MouseButtonUp_CMFCRibbonPanel__MEAAXVCPoint___Z(pPanel, point);
    }
    if (!::IsWindow(hWnd)) return;
    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    ::ScreenToClient(hWnd, &pt);
    impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(pThis, nFlags, PackPoint(pt));
}

// OnLButtonDblClk(UINT nFlags, CPoint point): retail RVA 0x11f4c0 (mfc140u;
// message map WM_LBUTTONDBLCLK), transcribed:
//     CMFCPopupMenuBar::OnLButtonDblClk(nFlags, point);                      // 0xbf810
//     if (IsRibbonPanelInRegularMode()) {                                    // own slot +0x8a8
//         CMFCRibbonButton* pDropped = GetDroppedDown();                     // 0x120da0
//         if (pDropped != NULL) pDropped->ClosePopupMenu();                  // element slot +0x340
//     }
//     CMFCRibbonBaseElement* pHit = HitTest(point);                          // 0x120df0
//     if (pHit != NULL) pHit->OnLButtonDblClk(point);                        // element slot +0x448 (unexported inline `{}` in the base)
// Symbol: ?OnLButtonDblClk@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    impl__OnLButtonDblClk_CMFCPopupMenuBar__IEAAXIVCPoint___Z(AsPopupMenuBar(pThis), nFlags, point);
    if (IsRibbonPanelInRegularMode(pThis)) {
        if (CMFCRibbonButton* pDropped = impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(pThis)) {
            impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(pDropped);
        }
    }
    CMFCRibbonBaseElement* pHit = impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(pThis, point);
    (void)pHit;   // pHit->OnLButtonDblClk(point) is the unexported inline empty body
}

// OnContextMenu(CWnd* pWnd, CPoint point): retail RVA 0x1209b0 (mfc140u;
// message map WM_CONTEXTMENU).  pWnd is never read; point is in screen
// coordinates.  Transcribed:
//     CWnd* pParent = <parent if IsKindOf(CMFCPopupMenu)>;
//     if (pParent != NULL && pParent->m_pParentRibbonElement != NULL &&      // +0x19b8
//         pParent->m_pParentRibbonElement->m_bFloatyMode) return;            // element +0x134
//     if (m_bAutoCommandTimer) { ::KillTimer(m_hWnd, 0xec1d); m_bAutoCommandTimer = FALSE; m_pPressed = NULL; m_rectAutoCommand.SetRectEmpty(); }
//     if (!IsRibbonPanel()) return;                                          // own slot +0x8a0, TRUE for this class
//     if (!m_bCustomizeMenu) return;
//     if (m_bIsFloaty && !m_bIsQATPopup) return;
//     if (::GetAsyncKeyState(::GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) < 0) return;
//     CPoint ptClient = point; ::ScreenToClient(m_hWnd, &ptClient);
//     CMFCRibbonButton* pDropped = GetDroppedDown(); if (pDropped != NULL) pDropped->ClosePopupMenu();   // slot +0x340
//     if (m_pDelayedButton != NULL) ::KillTimer(m_hWnd, 0xec1b);
//     if (point == CPoint(-1, -1)) {
//         CMFCRibbonBaseElement* pFocused = m_pCategory != NULL ? m_pCategory->GetFocused() : m_pPanel->GetFocused();   // 0xeff30 / 0x11cee0, no NULL check
//         if (pFocused != NULL) {
//             CRect rect = pFocused->m_rect; ClientToScreen(&rect);           // +0xc8, 0x2a3310
//             m_pRibbonBar->OnShowRibbonContextMenu(this, rect.left, rect.top, pFocused);   // bar slot +0x6d8, no NULL check
//         }
//     } else {
//         m_pRibbonBar->OnShowRibbonContextMenu(this, point.x, point.y, HitTest(ptClient));
//     }
// DEVIATIONS: the element m_bFloatyMode (+0x134) is unmodeled (read as 0: no early return);
// IsRibbonPanel() is this class's own TRUE; the focused element's rect is
// unmodeled, so the keyboard-invoked branch passes the bar's context-menu hook
// the client origin mapped to screen instead of the element's top-left; a
// NULL m_pRibbonBar skips the hook instead of faulting.
// Symbol: ?OnContextMenu@CMFCRibbonPanelMenuBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCRibbonPanelMenuBar__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* pWnd, long long point) {
    (void)pWnd;   // never read by retail
    if (pThis == nullptr) return;
    if (CWnd* pParent = ParentPopupMenu(pThis)) {
        void* pParentElement = At<void*>(pParent, kOffPopupParentRibbonElement);
        (void)pParentElement;   // its m_bFloatyMode (+0x134) is unmodeled: read as 0, no early return
    }
    if (At<int>(pThis, kOffAutoCommandTimer) != 0) {
        ::KillTimer(HWndOf(pThis), kTimerAutoCommand);
        At<int>(pThis, kOffAutoCommandTimer) = 0;
        PressedOf(pThis) = nullptr;
        ::SetRectEmpty(&RectAutoCommandOf(pThis));
    }
    // IsRibbonPanel() (own slot +0x8a0) is TRUE for this class.
    if (At<int>(pThis, kOffCustomizeMenu) == 0) return;
    if (At<int>(pThis, kOffIsFloaty) != 0 && At<int>(pThis, kOffIsQATPopup) == 0) return;
    const int vkButton = ::GetSystemMetrics(SM_SWAPBUTTON) != 0 ? VK_RBUTTON : VK_LBUTTON;
    if (::GetAsyncKeyState(vkButton) < 0) return;

    const POINT ptScreen = UnpackPoint(point);
    POINT ptClient = ptScreen;
    ::ScreenToClient(HWndOf(pThis), &ptClient);
    if (CMFCRibbonButton* pDropped = impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(pThis)) {
        impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(pDropped);
    }
    if (DelayedButtonOf(pThis) != nullptr) ::KillTimer(HWndOf(pThis), kTimerDelayedOpen);

    CMFCRibbonBar* pBar = RibbonBarOf(pThis);
    if (ptScreen.x == -1 && ptScreen.y == -1) {
        CMFCRibbonBaseElement* pFocused = impl__GetFocused_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__XZ(pThis);
        if (pFocused != nullptr) {
            RECT rect = {0, 0, 0, 0};   // retail: pFocused->m_rect (+0xc8), unmodeled
            impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rect);
            if (pBar != nullptr) {
                impl__OnShowRibbonContextMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
                    pBar, AsWnd(pThis), rect.left, rect.top, pFocused);
            }
        }
    } else {
        CMFCRibbonBaseElement* pHit = impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(pThis, PackPoint(ptClient));
        if (pBar != nullptr) {
            impl__OnShowRibbonContextMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
                pBar, AsWnd(pThis), ptScreen.x, ptScreen.y, pHit);
        }
    }
}

// OnTimer(UINT_PTR nIDEvent): retail RVA 0x120320 (mfc140u; message map
// WM_TIMER).  GetCursorPos + ScreenToClient(m_hWnd) first, then:
//   0xec1b: ::KillTimer(m_hWnd, 0xec1b);
//           if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL) m_pToolTip->ShowWindow(SW_HIDE);   // 0x2a9ad0
//           if (m_pDelayedCloseButton != NULL && m_pDelayedCloseButton->m_rect.PtInRect(pt)) return;   // +0xc8
//           CloseDelayedSubMenu();                                           // own slot +0x890
//           CMFCRibbonButton* pDelayed = m_pDelayedButton; m_pDelayedButton = NULL;
//           if (pDelayed != NULL && pDelayed->IsHighlighted())               // element slot +0x1b0 = 0xdee0: `return m_bIsHighlighted` (+0x13c)
//               pDelayed->OnShowPopupMenu();                                 // element slot +0x288
//   0xec1c: ::KillTimer(m_hWnd, 0xec1c);
//           if (m_pDelayedCloseButton == NULL) return;
//           CWnd* pParent = <parent if IsKindOf(CMFCPopupMenu)>;
//           if (m_pDelayedCloseButton->m_rect.PtInRect(pt)) return;
//           m_pDelayedCloseButton->ClosePopupMenu();                         // element slot +0x340
//           m_pDelayedCloseButton = NULL;
//           if (pParent != NULL) CMFCPopupMenu::ActivatePopupMenu(g_pTopLevelFrame ? g_pTopLevelFrame : GetTopLevelFrame(), pParent);   // 0xb7920
//   0xec07: ::KillTimer(m_hWnd, 0xec07);
//           CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//           if (!rectClient.PtInRect(pt)) return;
//           int nHit = HitTestEx(pt);                                        // 0x120e30
//           if (nHit == -1 || nHit != m_iAccHotItem || m_iAccHotItem == -1) return;
//           ::GetCursorPos(&pt);
//           if (OnSetAccData(MAKELONG(pt.x, pt.y)))                          // own slot +0x4b8
//               ::NotifyWinEvent(EVENT_OBJECT_FOCUS (0x8005), m_hWnd, OBJID_CLIENT (-4), nHit + 1);
//   0xec1d: if (!m_rectAutoCommand.PtInRect(pt)) { m_pPressed = NULL; ::KillTimer(m_hWnd, 0xec1d); m_rectAutoCommand.SetRectEmpty(); return; }
//           if (m_pPressed != NULL && m_pPressed->m_rect.PtInRect(pt) && !m_pPressed->OnAutoRepeat())   // +0xc8, element slot +0x2b0 (unexported inline, FALSE)
//               ::KillTimer(m_hWnd, 0xec1d);
// DEVIATIONS: the element rects (+0xc8) are unmodeled and treated as empty
// (PtInRect FALSE), so the 0xec1b / 0xec1c cases never take their "cursor still
// inside the element" early returns and the 0xec1d case does nothing while the
// cursor stays inside m_rectAutoCommand; IsHighlighted reads the unmodeled
// element m_bIsHighlighted (+0x13c, taken as 0) and OnAutoRepeat is the
// unexported base `return FALSE` (0x71e0), so the 0xec1b case never opens the
// delayed button and OnShowPopupMenu (an argument-less placeholder here, header
// request) is not reached.
// Symbol: ?OnTimer@CMFCRibbonPanelMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCRibbonPanelMenuBar__IEAAX_K_Z(void* pThis, unsigned __int64 nIDEvent) {
    if (pThis == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    ::ScreenToClient(hWnd, &pt);

    if (nIDEvent == kTimerDelayedOpen) {
        ::KillTimer(hWnd, kTimerDelayedOpen);
        CMFCToolTipCtrl* pTip = ToolTipOf(pThis);
        if (pTip != nullptr && HWndOf(pTip) != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pTip), SW_HIDE);
        }
        // (m_pDelayedCloseButton->m_rect.PtInRect(pt): rect unmodeled, treated as a miss)
        impl__CloseDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(pThis);
        CMFCRibbonBaseElement* pDelayed = DelayedButtonOf(pThis);
        DelayedButtonOf(pThis) = nullptr;
        (void)pDelayed;   // pDelayed->IsHighlighted() is the unexported base FALSE: no OnShowPopupMenu
        return;
    }
    if (nIDEvent == kTimerDelayedClose) {
        ::KillTimer(hWnd, kTimerDelayedClose);
        if (DelayedCloseButtonOf(pThis) == nullptr) return;
        CWnd* pParent = ParentPopupMenu(pThis);
        // (m_pDelayedCloseButton->m_rect.PtInRect(pt): rect unmodeled, treated as a miss)
        impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(DelayedCloseButtonOf(pThis));
        DelayedCloseButtonOf(pThis) = nullptr;
        if (pParent != nullptr) {
            CFrameWnd* pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
            if (pFrame == nullptr) pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));
            impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(pFrame, static_cast<CMFCPopupMenu*>(pParent));
        }
        return;
    }
    if (nIDEvent == kTimerAccHot) {
        ::KillTimer(hWnd, kTimerAccHot);
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(hWnd, &rectClient);
        if (!::PtInRect(&rectClient, pt)) return;
        const int nHit = impl__HitTestEx_CMFCRibbonPanelMenuBar__QEBAHVCPoint___Z(pThis, PackPoint(pt));
        if (nHit == -1) return;
        const int nAccHot = At<int>(pThis, kOffAccHotItem);
        if (nAccHot != nHit || nAccHot == -1) return;
        ::GetCursorPos(&pt);
        const long lVal = static_cast<long>((static_cast<unsigned long>(pt.y & 0xffff) << 16) | static_cast<unsigned long>(pt.x & 0xffff));
        if (impl__OnSetAccData_CMFCRibbonPanelMenuBar__UEAAHJ_Z(pThis, lVal) != 0) {
            ::NotifyWinEvent(EVENT_OBJECT_FOCUS, hWnd, OBJID_CLIENT, nHit + 1);
        }
        return;
    }
    if (nIDEvent == kTimerAutoCommand) {
        RECT& rectAuto = RectAutoCommandOf(pThis);
        if (!::PtInRect(&rectAuto, pt)) {
            PressedOf(pThis) = nullptr;
            ::KillTimer(hWnd, kTimerAutoCommand);
            ::SetRectEmpty(&rectAuto);
            return;
        }
        if (PressedOf(pThis) != nullptr) {
            // (m_pPressed->m_rect.PtInRect(pt): rect unmodeled, treated as a miss -> nothing more)
        }
        return;
    }
}

// OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar): retail RVA
// 0x120bd0 (mfc140u; message map WM_VSCROLL), transcribed:
//     if (m_pPanel != NULL &&
//         (pScrollBar ? pScrollBar->m_hWnd : NULL) == m_wndScrollBarVert.m_hWnd &&
//         m_pPanel->m_pPaletteButton != NULL) {                                          // +0x168
//         SCROLLINFO si = { sizeof si, SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS (0x17) };
//         ::GetScrollInfo(m_wndScrollBarVert.m_hWnd, SB_CTL, &si);
//         int nMaxOffset = si.nMax - si.nPage;
//         if (nMaxOffset <= 1) return;
//         int nOffset = m_pPanel->m_nScrollOffset;                                       // +0xf8
//         int nRow = m_pPanel->m_pPaletteButton->GetMenuRowHeight();                     // 0x114960
//         switch (nSBCode) {
//         case SB_LINEUP:   nOffset -= nRow;   break;   case SB_LINEDOWN: nOffset += nRow;   break;
//         case SB_PAGEUP:   nOffset -= si.nPage; break; case SB_PAGEDOWN: nOffset += si.nPage; break;
//         case SB_THUMBPOSITION: case SB_THUMBTRACK: nOffset = nPos; break;
//         case SB_TOP: nOffset = 0; break;             case SB_BOTTOM: nOffset = si.nMax; break;
//         default: return;
//         }
//         nOffset = min(max(nOffset, 0), nMaxOffset);
//         if (nOffset == m_pPanel->m_nScrollOffset) return;
//         m_pPanel->ScrollPalette(nOffset, FALSE);                                       // 0x11c830
//         ::SetScrollPos(m_wndScrollBarVert.m_hWnd, SB_CTL, nOffset, TRUE);
//         ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//     } else {
//         <static re-entrancy flag at 0x3c3e68>: if set, return; set it;
//         if (pScrollBar == NULL || !pScrollBar->SendChildNotifyLastMsg(NULL)) Default();   // 0x28ef80, 0x28ac80
//         clear it;
//     }
// DEVIATION: panel +0x168 is unmodeled and read as NULL, so the palette branch
// is never taken and every WM_VSCROLL goes down the reflection path (which
// is also what retail does for a non-palette menu bar); the re-entrancy flag
// is kept as a file-static.
// Symbol: ?OnVScroll@CMFCRibbonPanelMenuBar@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CMFCRibbonPanelMenuBar__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    (void)nSBCode;
    (void)nPos;
    if (pThis == nullptr) return;
    static int s_bInVScroll = 0;   // retail: the static at 0x3c3e68 (mfc140u)
    CMFCRibbonPanel* pPanel = PanelOf(pThis);
    const HWND hWndScroll = pScrollBar != nullptr ? HWndOf(pScrollBar) : nullptr;
    const bool bOwnBar = hWndScroll == HWndOf(ScrollBarOf(pThis));
    if (pPanel != nullptr && bOwnBar /* && pPanel->m_pPaletteButton != NULL: unmodeled, read as NULL */) {
        // The palette-scroll branch is unreachable (see above).
    }
    if (s_bInVScroll != 0) return;
    s_bInVScroll = 1;
    int bHandled = 0;
    if (pScrollBar != nullptr) {
        bHandled = impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(pScrollBar, nullptr);
    }
    if (bHandled == 0) impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    s_bInVScroll = 0;
}
