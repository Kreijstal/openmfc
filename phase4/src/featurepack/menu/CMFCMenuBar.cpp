// CMFCMenuBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cwchar>
#include <new>

// ===========================================================================
// Retail CMFCMenuBar own-member layout (mfc140u.dll), read off the constructor
// ??0CMFCMenuBar@@QEAA@XZ (RVA 0x8ad80, mfc140u) in the declaration order of
// the on-host MSVC 14.51 afxmenubar.h.  The CMFCToolBar subobject ends at
// 0x1350 (pinned below); the retail object is 0x13c0 bytes, which is also the
// m_nObjectSize (5056) featurepack/menu/RuntimeClasses.cpp declares.  OpenMFC's
// clean-room class is 0x13b0 bytes (_mfcmenubar_padding[96]), so the last two
// members -- m_pMenuPage (0x13b0) and m_pMenuButtonRTC (0x13b8) -- lie OUTSIDE
// an object OpenMFC itself allocates (CreateObject does `new CMFCMenuBar`);
// growing the padding to 112 bytes is a header change this file cannot make
// (reported as a header request).  No body below reads or writes those two
// (SetMenuButtonRTC's store is compiled out until the header grows).
//
// Constructor stores (RVA 0x8ad80): +0x1350 <- 1 (8-byte store, so
// m_bClearHashOnClose = 0), +0x1360/+0x1368/+0x1370/+0x1378 <- 0, +0x1380 (8
// bytes) <- 0, +0x1388 (8 bytes) <- 0, +0x1390 <- 0, +0x1394 (8 bytes) <- 0,
// +0x139c <- 0, +0x13a0 (8 bytes) <- 0, +0x13a8 <- nil CString,
// +0x13b0 <- 0, +0x13b8 <- RUNTIME_CLASS(CMFCToolBarMenuButton) (0x1803b1748),
// and CPane::m_bExclusiveRow (+0x218) <- 1.  m_pParentWnd (+0x1358) is NOT
// written by the ctor; Create / CreateEx (RVAs 0x8d1c0 / 0x8d1d0) store their
// pParentWnd argument there before tail-calling the CMFCToolBar base.
// ===========================================================================
namespace {
constexpr int kOffHWnd                  = 0x40;    // CWnd::m_hWnd
constexpr int kOffAutoDocMenus          = 0x1350;  // BOOL  m_bAutoDocMenus        (ctor: TRUE)
constexpr int kOffClearHashOnClose      = 0x1354;  // BOOL  m_bClearHashOnClose    (ctor: FALSE)
constexpr int kOffParentWnd             = 0x1358;  // CWnd* m_pParentWnd           (Create/CreateEx argument)
constexpr int kOffHMenu                 = 0x1360;  // HMENU m_hMenu                (ctor: NULL)
constexpr int kOffHDefaultMenu          = 0x1368;  // HMENU m_hDefaultMenu         (ctor: NULL)
constexpr int kOffHSysMenu              = 0x1370;  // HMENU m_hSysMenu             (ctor: NULL)
constexpr int kOffHSysIcon              = 0x1378;  // HICON m_hSysIcon             (ctor: NULL)
constexpr int kOffDefMenuResId          = 0x1380;  // UINT  m_uiDefMenuResId       (ctor: 0)
constexpr int kOffHelpComboID           = 0x1384;  // UINT  m_nHelpComboID         (ctor: 0)
constexpr int kOffSystemButtonsNum      = 0x1388;  // int   m_nSystemButtonsNum    (ctor: 0)
constexpr int kOffSystemButtonsNumSaved = 0x138c;  // int   m_nSystemButtonsNumSaved (ctor: 0)
constexpr int kOffHelpComboWidth        = 0x1390;  // int   m_nHelpComboWidth      (ctor: 0)
constexpr int kOffMaximizeMode          = 0x1394;  // BOOL  m_bMaximizeMode        (ctor: FALSE)
constexpr int kOffHaveButtons           = 0x1398;  // BOOL  m_bHaveButtons         (ctor: FALSE)
constexpr int kOffForceDownArrows       = 0x139c;  // BOOL  m_bForceDownArrows     (ctor: FALSE)
constexpr int kOffSzSystemButton        = 0x13a0;  // CSize m_szSystemButton       (ctor: 0,0)
constexpr int kOffStrHelpComboPrompt    = 0x13a8;  // CString m_strHelpComboPrompt (ctor: nil string)
constexpr int kOffMenuPage              = 0x13b0;  // CMFCToolBarsMenuPropertyPage* m_pMenuPage (ctor: NULL) -- OUTSIDE OpenMFC's object
constexpr int kOffMenuButtonRTC         = 0x13b8;  // CRuntimeClass* m_pMenuButtonRTC (ctor: RUNTIME_CLASS(CMFCToolBarMenuButton)) -- OUTSIDE
constexpr int kRetailMenuBarSize        = 0x13c0;

static_assert(sizeof(CMFCToolBar) == kOffAutoDocMenus,
              "CMFCMenuBar's own members start where the CMFCToolBar subobject ends (0x1350)");
static_assert(kOffStrHelpComboPrompt + 8 == 0x13b0, "m_strHelpComboPrompt is the last member inside OpenMFC's 0x13b0-byte class");
// (OpenMFC's CMFCMenuBar is 0x13b0 bytes today; deliberately not pinned with
// `==` so that growing the padding to the retail size needs no edit here --
// SetMenuButtonRTC keys off sizeof(CMFCMenuBar).)
static_assert(sizeof(CMFCMenuBar) <= kRetailMenuBarSize, "OpenMFC's CMFCMenuBar must never be larger than the retail object");
static_assert(sizeof(CMFCMenuBar) >= kOffStrHelpComboPrompt + 8, "the ctor constructs m_strHelpComboPrompt inside the object");
static_assert(sizeof(CString) == 8, "m_strHelpComboPrompt is one CStringT pointer");
static_assert(offsetof(CPane, m_bExclusiveRow) == 0x218, "CPane::m_bExclusiveRow @0x218");
static_assert(offsetof(CBasePane, m_dwStyle) == 0x104, "CBasePane::m_dwStyle @0x104");
static_assert(offsetof(CMFCToolBar, m_iHighlighted) == 0x1138, "CMFCToolBar::m_iHighlighted @0x1138");
static_assert(offsetof(CMFCToolBar, m_bMenuMode) == 0x10ec, "CMFCToolBar::m_bMenuMode @0x10ec");
static_assert(offsetof(CMFCToolBar, m_bFloating) == 0x10d8, "CMFCToolBar::m_bFloating @0x10d8");
static_assert(offsetof(CMFCToolBar, m_bLocked) == 0x10b8, "CMFCToolBar::m_bLocked @0x10b8");
static_assert(offsetof(CMFCToolBar, m_bLargeIconsAreEnbaled) == 0x10bc, "CMFCToolBar::m_bLargeIconsAreEnbaled @0x10bc");
static_assert(offsetof(CMFCToolBar, m_sizeButtonLocked) == 0x1158, "CMFCToolBar::m_sizeButtonLocked @0x1158");
static_assert(offsetof(CMFCToolBar, m_sizeCurButtonLocked) == 0x1168, "CMFCToolBar::m_sizeCurButtonLocked @0x1168");
static_assert(offsetof(CMFCToolBar, m_Buttons) == 0x1188, "CMFCToolBar::m_Buttons @0x1188");
static_assert(offsetof(CMFCToolBar, m_OrigButtons) == 0x11c0, "CMFCToolBar::m_OrigButtons @0x11c0");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "CMFCToolBarButton::m_bUserButton @0x08");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "CMFCToolBarButton::m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_bWrap) == 0x14, "CMFCToolBarButton::m_bWrap @0x14");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "CMFCToolBarButton::m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "CMFCToolBarButton::m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "CMFCToolBarButton::m_strText @0x38");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48, "CMFCToolBarButton::m_iImage @0x48");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c, "CMFCToolBarButton::m_iUserImage @0x4c");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "CMFCToolBarButton::m_rect @0x68");
static_assert(offsetof(CWnd, m_hWnd) == kOffHWnd, "CWnd::m_hWnd @0x40");

// Retail CMFCToolBarMenuButton members (afxtoolbarmenubutton.h order: the
// 0x88-byte CMFCToolBarButton, CObList m_listCommands at 0x88, so):
constexpr int kOffMbPopupMenu           = 0xc0;    // CMFCPopupMenu* m_pPopupMenu (inside OpenMFC's _pad; see CMFCPopupMenuBar.cpp)
// Retail CMFCToolBarMenuButtonsButton derives from CMFCToolBarButton (on-host
// afxtoolbarmenubuttonsbutton.h:27, m_nObjectSize 0x90 in the descriptor at
// 0x1803188a0) and keeps UINT m_uiSystemCommand at +0x88 -- which is where
// every retail body below reads it.  OpenMFC's afxmfc.h derives the class from
// CMFCToolBarMenuButton instead and so places m_uiSystemCommand at 0x128; that
// contradiction is reported as a header request, and the bodies below use the
// RETAIL offset, because the only such buttons that reach them are the ones a
// retail-layout caller built (OpenMFC's SetMaximizeMode, which would build the
// wrong-layout ones, is left a stub for exactly this reason).
constexpr int kOffMbbSystemCommand      = 0x88;

// Retail CMultiDocTemplate keeps HMENU m_hMenuShared at +0xf0 and HACCEL
// m_hAccelTable at +0xf8 (its LoadTemplate, RVA 0x225740 mfc140u, stores the
// ::LoadMenuW / ::LoadAcceleratorsW results there), i.e. as its first own
// members right after the 0xf0-byte CDocTemplate.  OpenMFC's CDocTemplate is
// the same 0xf0 bytes and its CMultiDocTemplate the same 0x140 bytes (the
// retail descriptor at 0x329588 says m_nObjectSize 0x140), but both slots lie
// in the unnamed _multidoctemplate_padding, which no OpenMFC body ever writes
// AND no OpenMFC ctor zeroes (core/doc/CMultiDocTemplate.cpp forwards the
// ctor thunks to CDocTemplate's, which cannot reach past 0xf0; LoadTemplate
// forwards to CDocTemplate::LoadTemplate).  A read here therefore yields
// whatever the allocation held -- NOT reliably NULL -- and the one body below
// that reads it (ResetImages) validates the value with ::IsMenu before use.
constexpr int kOffMultiDocTemplateMenuShared = 0xf0;
static_assert(sizeof(CDocTemplate) == kOffMultiDocTemplateMenuShared, "CMultiDocTemplate's own members start at 0xf0");
static_assert(sizeof(CMultiDocTemplate) == 0x140, "CMultiDocTemplate is 0x140 bytes (retail m_nObjectSize)");

// afxGlobalData (a zeroed 720-byte block in OpenMFC, see CMFCToolBar.cpp):
//   +0x000 the one-time init gate every retail reader tests;
//   +0x278 the flag CMFCToolBar::AccNotifyObjectFocusEvent (RVA 0x15a560) and
//          CMFCMenuBar::OnChangeHot (RVA 0x8d580) gate accessibility events on;
//   +0x2a8 / +0x2ac the horizontal / vertical text heights (GetTextHeight).
constexpr int kGlobalDataInitGate       = 0x000;
constexpr int kGlobalDataAccessibility  = 0x278;
constexpr int kGlobalDataTextHeightHorz = 0x2a8;
constexpr int kGlobalDataTextHeightVert = 0x2ac;

// Timer id / private message / style bits the bodies use (afxres.h,
// afxtoolbarbutton.h, afxmenubar.cpp; none is spelled in OpenMFC's headers).
constexpr UINT_PTR kTimerShowAllCommands  = 0xec12;   // the 5000 ms "show all commands" timer
constexpr UINT     kWmSetMessageString    = 0x362;    // WM_SETMESSAGESTRING
constexpr UINT     kAfxIdsIdleMessage     = 0xe001;   // AFX_IDS_IDLEMESSAGE
constexpr UINT     kIdsMenuBarTitle       = 0x3e86;   // IDS_AFXBARRES_MENU_BAR_TITLE (afxribbonres.h: 16006)
constexpr unsigned kCbrsToolTips          = 0x8;      // CBRS_TOOLTIPS
constexpr unsigned kCbrsOrientHorz        = 0xa000;   // CBRS_ORIENT_HORZ = CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM
constexpr unsigned kTbbsSeparator         = 0x1;      // TBBS_SEPARATOR
constexpr unsigned kTbbsDisabled          = 0x40000;  // TBBS_DISABLED

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}
inline HWND HWndOf(const void* p) { return At<HWND>(p, kOffHWnd); }
inline CMFCToolBar* AsToolBar(CMFCMenuBar* p) { return static_cast<CMFCToolBar*>(p); }
inline const CMFCToolBar* AsToolBar(const CMFCMenuBar* p) { return static_cast<const CMFCToolBar*>(p); }
inline CWnd* AsWnd(CMFCMenuBar* p) { return static_cast<CWnd*>(p); }

// CPoint passed BY VALUE is an 8-byte aggregate in a register under the MS x64
// ABI: x in the low dword, y in the high dword.
inline CPoint UnpackPoint(long long v) {
    return CPoint(static_cast<int>(static_cast<std::uint32_t>(v & 0xffffffffu)),
                  static_cast<int>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) >> 32)));
}
} // namespace

// Class statics with storage in featurepack/menu/StaticData.cpp,
// featurepack/toolbar/StaticData.cpp, featurepack/toolbar/CMFCToolBar.cpp and
// featurepack/CMFC_misc_stubs.cpp.  The addresses are the mfc140u ones the
// retail bodies read.
extern "C" std::int32_t  impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;               // 0x1803b1b08
extern "C" std::int32_t  impl__m_bShowAllCommands_CMFCMenuBar__1HA;                 // 0x1803be254
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;                   // 0x1803be35c
extern "C" unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80];   // 0x1803b1fd0
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];             // 0x1803c1620
struct MenuBarStaticCSize { long cx; long cy; };   // the shape CMFCToolBar.cpp defines these with
extern "C" MenuBarStaticCSize impl__m_sizeButton_CMFCToolBar__1VCSize__A;            // 0x1803b1e38
extern "C" MenuBarStaticCSize impl__m_sizeCurButton_CMFCToolBar__1VCSize__A;         // 0x1803b1ee8
extern "C" MenuBarStaticCSize impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A;        // 0x1803b1d08
extern "C" unsigned char impl__afxMenuHash__3VCMenuHash__A[128];                     // 0x1803b1d10 (72-byte CMenuHash; storage is the upper bound, see CMFC_misc_stubs.cpp)

// Cross-file thunks.  Each was grepped to its definition; the parameter lists
// are derived from the mangled names (several definitions still carry
// auto-generated placeholder lists -- noted where it matters; harmless across
// translation units for extern "C" symbols).
//   core/runtime/CObject.cpp                : IsKindOf
//   core/runtime/AFX_GLOBAL_DATA.cpp        : Initialize (empty stub, placeholder list), SetMenuFont (stub returning 0)
//   detail/MfcExceptionsSupport.cpp         : AfxThrowInvalidArgException
//   core/window/CWnd.cpp                    : FromHandle, FromHandlePermanent, SetWindowTextW
//   core/window/Thunks.cpp                  : Default, GetExStyle
//   core/window/CMenu.cpp                   : CMenu::FromHandle
//   detail/CMenuSupport.cpp                 : CMenu::GetMenuStringW(CString&)
//   core/collections/CObList.cpp            : FindIndex
//   core/collections/CStringT.cpp           : CStringT::LoadStringW(HINSTANCE, UINT)
//   featurepack/CMFC_misc_stubs.cpp         : AfxFindStringResourceHandle
//   core/frame/Thunks.cpp                   : CMDIFrameWnd::MDIGetActive
//   core/frame/CMDIFrameWnd.cpp / featurepack/toolbar/RuntimeClasses.cpp : GetThisClass getters
//   featurepack/docking/CBasePane.cpp       : GetCurrentAlignment, PreTranslateMessage@CBasePane (stub, placeholder list)
//   featurepack/toolbar/Thunks.cpp          : GetButton, GetCount
//   featurepack/toolbar/CMFCToolBar.cpp     : the CMFCToolBar base handlers and helpers named below, GetAllToolbars
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp : OnClick, OnCancelMode (both stubs with auto-generated parameter lists)
//   featurepack/customize/CMFCCmdUsageCount.cpp   : HasEnoughInformation
//   core/window/CMenuHash.cpp               : LoadMenuBar, SaveMenuBar (both map hMenu -> bar in a side table and return TRUE)
//   detail/RegcoreSupport.cpp               : AfxGetApp
//   core/app/CWinApp.cpp                    : GetFirstDocTemplatePosition, GetNextDocTemplate
//   core/doc/CMultiDocTemplate.cpp          : GetThisClass_CMultiDocTemplate
//   core/frame/Thunks.cpp                   : CFrameWnd::RecalcLayout
//   core/window/Thunks.cpp                  : CWnd::GetParentFrame
//   featurepack/toolbar/CMFCToolBar.cpp     : ResetImages (empty stub whose definition carries no `this` parameter; declared here with the shape the mangled name gives)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__SetMenuFont_AFX_GLOBAL_DATA__QEAAHPEAUtagLOGFONTW__H_Z(void* pThis, LOGFONTW* lpLogFont, int bHorz);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);
extern "C" int MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
    const CMenu* pThis, unsigned int nIDItem, CString& rString, unsigned int nFlags);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" const CObList* MS_ABI impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, unsigned int nID);
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(const CMDIFrameWnd* pThis, int* pbMaximized);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(CBasePane* pThis, MSG* pMsg);
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCToolBar* pThis, int iIndex);
extern "C" int MS_ABI impl__GetCount_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);
extern "C" int MS_ABI impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(const CMFCToolBar* pThis, unsigned int nIDFind, int iIndexFirst);
extern "C" CMFCToolBarButton* MS_ABI impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(CMFCToolBar* pThis, int iButton);
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, CPoint point);
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(const CMFCToolBar* pThis, int* pIndex);
extern "C" int MS_ABI impl__DropDownMenu_CMFCToolBar__IEAAHPEAVCMFCToolBarButton___Z(CMFCToolBar* pThis, CMFCToolBarButton* pButton);
extern "C" void MS_ABI impl__Deactivate_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RestoreFocus_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" int MS_ABI impl__TranslateChar_CMFCToolBar__UEAAHI_Z(CMFCToolBar* pThis, unsigned int nChar);
extern "C" void MS_ABI impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int iButton);
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int iHot);
extern "C" void MS_ABI impl__OnSetFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(CMFCToolBar* pThis, CWnd* pOldWnd);
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(const CMFCToolBar* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(CMFCToolBar* pThis, CMFCToolBarButton* pButton);
extern "C" int MS_ABI impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(const CMFCToolBar* pThis, CPoint point, CRect* pRect);
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(CMFCToolBar* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCToolBar* pThis, LPCREATESTRUCTW lpCreateStruct);
extern "C" void MS_ABI impl__OnSettingChange_CMFCToolBar__IEAAXIPEB_W_Z(CMFCToolBar* pThis, unsigned int uFlags, const wchar_t* lpszSection);
extern "C" void MS_ABI impl__AdjustLayout_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" int MS_ABI impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(CMFCToolBarMenuButton* pThis, CWnd* pWnd, int bDelay);
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(void* pButton);
extern "C" int MS_ABI impl__HasEnoughInformation_CMFCCmdUsageCount__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU hMenu, CMFCToolBar* pBar);
extern "C" int MS_ABI impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU hMenu, CMFCToolBar* pBar);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(const CWinApp* pThis);
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(const CWinApp* pThis, void** pos);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__ResetImages_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);

// Exports of this class defined later in this file and reached by earlier
// bodies (retail reaches GetRowHeight through vtable slot 0x6b8; see the call sites).
extern "C" int MS_ABI impl__GetRowHeight_CMFCMenuBar__UEBAHXZ(const CMFCMenuBar* pThis);
extern "C" void* MS_ABI impl__GetSystemMenu_CMFCMenuBar__QEBAPEAVCMFCToolBarSystemMenuButton__XZ(const CMFCMenuBar* pThis);
extern "C" int MS_ABI impl__FindMenuItemText_CMFCMenuBar__KAHPEAUHMENU____IAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    HMENU hMenu, unsigned int nItemID, CString* pStrText);
extern "C" void MS_ABI impl__CalcSysButtonSize_CMFCMenuBar__IEAAXXZ(CMFCMenuBar* pThis);

namespace {
inline int IsKindOfRT(const CObject* p, CRuntimeClass* pClass) {
    return p != nullptr && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, pClass);
}
inline int GlobalDataInt(int off) {
    int v; std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v); return v;
}
// The retail one-time gate: `if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }`.
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}
// GetOwner() as retail inlines it: `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`
// (CWnd + 0xa0).  DEVIATION, shared with CMFCToolBar.cpp / CMFCPopupMenuBar.cpp:
// OpenMFC's CWnd does not name m_hWndOwner (that byte range is anonymous
// padding nothing ever writes), so the ::GetParent branch is taken unconditionally.
inline HWND OwnerHwnd(const void* pThis) { return ::GetParent(HWndOf(pThis)); }

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h), the node
// a CObList::POSITION points at; used to walk GetAllToolbars() from this
// translation unit (the pattern featurepack/visualmanager/CMFCVisualManager.cpp uses).
struct MenuBarObNode {
    MenuBarObNode* pNext;
    MenuBarObNode* pPrev;
    CObject*       data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline const MenuBarObNode* ObNodeFromPos(CObList::POSITION pos) {
    const MenuBarObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}
} // namespace

// Symbol: ?Create@CMFCMenuBar@@UEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCMenuBar__UEAAHPEAVCWnd__KI_Z(CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID) {
    return pThis ? pThis->Create(pParentWnd, dwStyle, nID) : FALSE;
}
// Symbol: ?CreateEx@CMFCMenuBar@@UEAAHPEAVCWnd@@KKVCRect@@I@Z
extern "C" int MS_ABI impl__CreateEx_CMFCMenuBar__UEAAHPEAVCWnd__KKVCRect__I_Z(
    CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, CRect rcBorders, unsigned int nID) {
    return pThis ? pThis->CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID) : FALSE;
}
// Symbol: ?CreateFromMenu@CMFCMenuBar@@UEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(CMFCMenuBar* pThis, HMENU hMenu, int bDefaultMenu, int bForceUpdate) {
    if (pThis) pThis->CreateFromMenu(hMenu, bDefaultMenu, bForceUpdate);
}
// Symbol: ?GetMenuItem@CMFCMenuBar@@QEBAPEAVCMFCToolBarButton@@H@Z
extern "C" CMFCToolBarButton* MS_ABI impl__GetMenuItem_CMFCMenuBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCMenuBar* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}
// Symbol: ?CalcFixedLayout@CMFCMenuBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CMFCMenuBar__UEAA_AVCSize__HH_Z(CSize* pRet, CMFCMenuBar* pThis, int bStretch, int bHorz) {
    (void)bStretch;
    (void)bHorz;
    new (pRet) CSize(CalcMenuBarFixedLayout(pThis));
}
// Symbol: ?CalcLayout@CMFCMenuBar@@UEAA?AVCSize@@KH@Z
extern "C" void MS_ABI impl__CalcLayout_CMFCMenuBar__UEAA_AVCSize__KH_Z(CSize* pRet, CMFCMenuBar* pThis, unsigned long dwMode, int nLength) {
    new (pRet) CSize(pThis ? pThis->CalcLayout(dwMode, nLength) : CSize());
}
// Symbol: ?AdjustLocations@CMFCMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCMenuBar__UEAAXXZ(CMFCMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}
// Symbol: ?GetMenuFont@CMFCMenuBar@@SAAEBVCFont@@H@Z
extern "C" CFont* MS_ABI impl__GetMenuFont_CMFCMenuBar__SAAEBVCFont__H_Z(int bHorz) {
    return &CMFCMenuBar::GetMenuFont(bHorz);
}
// The 96-byte padding is the retail member block 0x1350..0x13b0 (see the layout
// note above).  After zeroing it, the retail initial values that differ from
// zero are applied: m_bAutoDocMenus = TRUE (the ctor's 8-byte store of 1 at
// +0x1350), CPane::m_bExclusiveRow = TRUE (+0x218), and m_strHelpComboPrompt
// (+0x13a8) is constructed as a real (nil) CString so that EnableHelpCombobox
// can assign to it; the destructor tears it down.  m_pMenuPage /
// m_pMenuButtonRTC (+0x13b0 / +0x13b8) lie outside this object and are not
// touched.
CMFCMenuBar::CMFCMenuBar() {
    memset(_mfcmenubar_padding, 0, sizeof(_mfcmenubar_padding));
    At<int>(this, kOffAutoDocMenus) = TRUE;
    m_bExclusiveRow = TRUE;
    new (&At<CString>(this, kOffStrHelpComboPrompt)) CString();
}
CMFCMenuBar::~CMFCMenuBar() {
    At<CString>(this, kOffStrHelpComboPrompt).~CString();
    ClearMenuBarState(this);
}
// Retail Create (RVA 0x8d1c0, mfc140u) is `m_pParentWnd = pParentWnd;` (+0x1358)
// followed by a tail call to CMFCToolBar::Create; CreateEx (RVA 0x8d1d0) stores
// the same member and calls CMFCToolBar::CreateEx.  The store is reproduced
// here next to the pre-existing side-table bookkeeping.
BOOL CMFCMenuBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    g_menuBarStates[this].parent = pParentWnd;
    At<CWnd*>(this, kOffParentWnd) = pParentWnd;
    return CMFCToolBar::Create(pParentWnd, dwStyle, nID);
}
BOOL CMFCMenuBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    At<CWnd*>(this, kOffParentWnd) = pParentWnd;
    return Create(pParentWnd, dwStyle, nID);
}
// Clean-room rebuild over the mfccore side table.  Of the retail body (RVA
// 0x8af20, mfc140u) only two of its member stores are reproduced here: on the
// rebuild path -- taken when `m_hMenu != hMenu || CMFCToolBar::m_bCustomizeMode
// || bForceUpdate` (the tests at 0x8af96..0x8afa6; the other branch only
// re-applies the maximize-mode buttons through SetMaximizeMode (0x8b6f0) and
// InvalidateButton (0x14fe50) and jumps to the shared tail at 0x8b41e without
// touching these members) -- retail stores `m_hMenu = hMenu` (+0x1360, at
// 0x8b050) and, if bDefaultMenu, `m_hDefaultMenu = hMenu` (+0x1368, at
// 0x8b05c).  ResetImages and OnSetDefaultButtonText read those members, so
// they are kept in step here.
// NOT reproduced from the same path: the entry guard (m_pMenuButtonRTC
// (+0x13b8) == NULL -> AfxThrowInvalidArgException, and a non-exported
// CFrameWnd helper at 0x2a9b60 called on GetParentFrame() when
// CWnd::FromHandle(::GetFocus()) is this bar), afxMenuHash.SaveMenuBar(old m_hMenu)
// at 0x8b038, the `m_bMaximizeMode = FALSE` store at 0x8b049 (retail saves the
// old value and, at 0x8b3bc, re-enters maximize mode through SetMaximizeMode
// when it was on and m_pParentWnd is a CMDIFrameWnd; SetMaximizeMode is a stub
// here and nothing in OpenMFC ever sets m_bMaximizeMode), the
// CMenuTearOffManager / LoadMenuBar / per-item button build, the help-combo
// insertion, and the tail's AdjustLayout / AdjustSizeImmediate.
// DEVIATION: the side-table rebuild below runs unconditionally, where retail
// skips it on the non-rebuild branch.
void CMFCMenuBar::CreateFromMenu(HMENU hMenu, BOOL bDefaultMenu, BOOL bForceUpdate) {
    if (At<HMENU>(this, kOffHMenu) != hMenu || impl__m_bCustomizeMode_CMFCToolBar__1HA || bForceUpdate) {
        At<HMENU>(this, kOffHMenu) = hMenu;
        if (bDefaultMenu) At<HMENU>(this, kOffHDefaultMenu) = hMenu;
    }
    MenuBarState& state = g_menuBarStates[this];
    for (CMFCToolBarButton* item : state.ownedItems) {
        delete item;
    }
    state.items.clear();
    state.ownedItems.clear();
    state.menu = hMenu;

    if (!hMenu) return;
    int count = ::GetMenuItemCount(hMenu);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarMenuButton* button = CreateMenuButtonFromMenu(hMenu, i);
        state.items.push_back(button);
        state.ownedItems.insert(button);
    }
}
CMFCToolBarButton* CMFCMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_menuBarStates.find(this);
    if (it == g_menuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
CSize CMFCMenuBar::CalcLayout(DWORD, int nLength) {
    CSize size = CalcMenuBarFixedLayout(this);
    if (nLength > 0) size.cx = nLength;
    return size;
}
void CMFCMenuBar::AdjustLocations() {
    MenuBarState& state = g_menuBarStates[this];
    const int count = static_cast<int>(state.items.size());
    const int height = 22;
    const int width = count > 0 ? std::max(23, (count * 80)) : 23;
    if (GetSafeHwnd()) {
        ::SendMessageW(GetSafeHwnd(), TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].recentRect = CRect(0, 0, width, height);
}
CFont& CMFCMenuBar::GetMenuFont(BOOL) {
    return EnsureMenuFont_Mfccore();
}

// ===========================================================================
// The bodies below are transcribed from the retail mfc140u.dll exports (entry
// RVA cited at each function; every address is an mfc140u address resolved
// through the export table).  Retail walks the CMFCToolBar::m_Buttons CObList
// through its nodes (head +0x1190, tail +0x1198, count +0x11a0); OpenMFC keeps
// a toolbar's buttons in the mfccore side table, so every such walk runs over
// the exported GetCount()/GetButton() -- the same substitution
// featurepack/toolbar/CMFCToolBar.cpp makes.  "vslot 0xNNN" names a virtual
// call through the CMFCMenuBar vftable at 0x1802eef18 (mfc140u); where the
// slot's retail body is this class's own export, that export is called
// directly (DEVIATION for a client-derived override, which is not reached).
// ===========================================================================

// Retail (RVA 0x8dd20, mfc140u), decoded:
//     while (m_OrigButtons.GetCount() != 0) {                          // +0x11d8
//         CMFCToolBarButton* p = m_OrigButtons.RemoveHead();           // 0x231f40
//         if (p) delete p;                                             // vslot 8, flag 1
//     }
//     CWinAppEx* pApp = AfxGetModuleState()->m_pCurrentWinApp;         // 0x133930, +0x8
//     if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)) ||        // 0x3207c8
//         !pApp->[+0x1bc])                                             // afxwinappex.h: m_bResourceSmartUpdate
//         return FALSE;
//     CMenu menu;
//     if (!menu.Attach(::LoadMenuW(AfxFindResourceHandle(MAKEINTRESOURCE(uiMenuResID), RT_MENU), MAKEINTRESOURCE(uiMenuResID))))
//         return FALSE;
//     int nCount = ::GetMenuItemCount(menu);
//     for (int i = 0; i < nCount; i++) {
//         UINT uiCmd = ::GetMenuItemID(menu, i);
//         CString strText;                                             // nil
//         if (uiCmd == 0) {                                            // separator
//             CMFCToolBarButton* p = new CMFCToolBarButton;            // 0x88 bytes, ctor 0x15bec0
//             if (!p) AfxThrowInvalidArgException();
//             p->m_nStyle = TBBS_SEPARATOR;                            // +0x28
//             m_OrigButtons.AddTail(p);                                // 0x231e70
//         } else if (uiCmd == (UINT)-1) {                              // popup
//             CMenu* pSub = CMenu::FromHandle(::GetSubMenu(menu, i));  // 0x2a80a0
//             if (!pSub) AfxThrowInvalidArgException();
//             CMFCToolBarMenuButton* p = (CMFCToolBarMenuButton*)m_pMenuButtonRTC->CreateObject();   // +0x13b8, 0x234d60
//             p->Initialize(0, pSub->m_hMenu, -1, strText, FALSE);     // 0x1729b0
//             m_OrigButtons.AddTail(p);
//         } else {
//             m_OrigButtons.AddTail(new CMFCToolBarButton(uiCmd, -1, strText, FALSE, FALSE));   // ctor 0x15bf40
//         }
//     }
//     menu.DestroyMenu();                                              // 0x2a8190
//     return TRUE;
// Left a stub.  Two of its inputs do not exist on this side: CWinAppEx's
// m_bResourceSmartUpdate (+0x1bc) is not a member of OpenMFC's CWinAppEx (the
// ctor argument is discarded, core/app/CWinAppEx.cpp:283, and the byte range
// is anonymous padding), and m_pMenuButtonRTC (+0x13b8) lies outside OpenMFC's
// 0x13b0-byte object and is never initialised by its ctor, so reading it from
// a retail-sized caller yields whatever the caller's memory held.  The
// remaining plumbing (CMFCToolBarButton ctors, CMFCToolBarMenuButton::Initialize,
// CObList::AddTail) is available; this becomes implementable once the header
// grows the padding to 112 bytes and initialises +0x13b8.  Retail's
// m_OrigButtons clearing is not reproduced either, so the list is left as it
// was; the return value FALSE is what retail returns whenever the app is not
// a resource-smart-update CWinAppEx, which today is always.
// Symbol: ?BuildOrigItems@CMFCMenuBar@@MEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCMenuBar__MEAAHI_Z(CMFCMenuBar* pThis, unsigned int uiMenuResID) {
    (void)pThis; (void)uiMenuResID;
    return FALSE;
}

// Retail (RVA 0x8dc70, mfc140u), fully transcribed:
//     m_bHaveButtons = FALSE;                                          // +0x1398
//     for (node = m_Buttons.head; node; node = node->next) {
//         CMFCToolBarButton* p = node->data;
//         if (p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton))) continue;   // 0x3188a0
//         if (p->IsKindOf(RUNTIME_CLASS(CMFCToolBarSystemMenuButton))) continue;    // 0x3b17a8
//         if (!p->m_bImage) continue;                                  // +0x10
//         if (p->GetImage() >= 0) { m_bHaveButtons = TRUE; break; }    // m_bUserButton ? m_iUserImage : m_iImage
//     }
//     return GetRowHeight();                                           // vslot 0x6b8 (tail jump)
// Symbol: ?CalcMaxButtonHeight@CMFCMenuBar@@UEAAHXZ
extern "C" int MS_ABI impl__CalcMaxButtonHeight_CMFCMenuBar__UEAAHXZ(CMFCMenuBar* pThis) {
    if (!pThis) return 0;
    At<int>(pThis, kOffHaveButtons) = FALSE;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (p == nullptr) continue;   // retail dereferences the node data unconditionally
        if (IsKindOfRT(p, impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ())) continue;
        if (IsKindOfRT(p, impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ())) continue;
        if (!p->m_bImage) continue;
        if (p->GetImage() >= 0) {
            At<int>(pThis, kOffHaveButtons) = TRUE;
            break;
        }
    }
    return impl__GetRowHeight_CMFCMenuBar__UEBAHXZ(pThis);   // vslot 0x6b8 = this class's GetRowHeight
}

// Retail (RVA 0x8daa0, mfc140u), fully transcribed:
//     CWindowDC dc(NULL);                                              // 0x2a3c20: ::GetWindowDC(NULL)
//     CDC dcMem;  dcMem.Attach(::CreateCompatibleDC(NULL));            // note: NULL, not dc
//     int cx = ::GetSystemMetrics(SM_CXMENUSIZE);                      // 0x36
//     int cy = ::GetSystemMetrics(SM_CXMENUSIZE);                      // 0x36 again (not SM_CYMENUSIZE)
//     CBitmap bmp;  bmp.Attach(::CreateCompatibleBitmap(dc.m_hDC, cx, cy));
//     CBitmap* pOld = dcMem.SelectObject(&bmp);                        // ::SelectObject + CGdiObject::FromHandle
//     CRect rect(0, 0, cx, cy);
//     ::DrawFrameControl(dcMem.m_hDC, &rect, DFC_CAPTION /*1*/, DFCS_ADJUSTRECT /*0x2000*/);
//     m_szSystemButton.cx = rect.right - rect.left;                    // +0x13a0
//     m_szSystemButton.cy = rect.bottom - rect.top;                    // +0x13a4
//     dcMem.SelectObject(pOld);
//     (bmp, dcMem and dc are destroyed: DeleteObject / DeleteDC / ReleaseDC)
// The MFC wrappers only carry the handles used here, so the body is written
// against the handles directly; the sequence of USER32/GDI32 calls and their
// arguments are the retail ones.  (The import slots resolve to
// CreateCompatibleDC, GetSystemMetrics, CreateCompatibleBitmap, SelectObject,
// DrawFrameControl and DeleteDC; ??0CWindowDC (0x2a3c20) is ::GetWindowDC on
// the NULL HWND plus CDC::Attach, and the CBitmap teardown at 0x1c6f0 is
// CGdiObject::DeleteObject.)  DEVIATION: the CWindowDC ctor raises
// AfxThrowResourceException when ::GetWindowDC fails; here the NULL DC simply
// flows into the GDI calls, which fail and leave m_szSystemButton at cx x cy.
// Symbol: ?CalcSysButtonSize@CMFCMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__CalcSysButtonSize_CMFCMenuBar__IEAAXXZ(CMFCMenuBar* pThis) {
    if (!pThis) return;
    HDC hScreenDC = ::GetWindowDC(nullptr);
    HDC hMemDC = ::CreateCompatibleDC(nullptr);
    const int cx = ::GetSystemMetrics(SM_CXMENUSIZE);
    const int cy = ::GetSystemMetrics(SM_CXMENUSIZE);
    HBITMAP hBmp = ::CreateCompatibleBitmap(hScreenDC, cx, cy);
    HGDIOBJ hOld = ::SelectObject(hMemDC, hBmp);

    RECT rect = { 0, 0, cx, cy };
    ::DrawFrameControl(hMemDC, &rect, DFC_CAPTION, DFCS_ADJUSTRECT);
    At<SIZE>(pThis, kOffSzSystemButton).cx = rect.right - rect.left;
    At<SIZE>(pThis, kOffSzSystemButton).cy = rect.bottom - rect.top;

    ::SelectObject(hMemDC, hOld);
    if (hBmp) ::DeleteObject(hBmp);
    if (hMemDC) ::DeleteDC(hMemDC);
    if (hScreenDC) ::ReleaseDC(nullptr, hScreenDC);
}

// Retail (RVA 0x8e0e0, mfc140u), fully transcribed:
//     m_nHelpComboID = uiID;  m_nHelpComboWidth = nComboBoxWidth;      // +0x1384, +0x1390
//     m_strHelpComboPrompt.SetString(lpszPrompt ? lpszPrompt : L"", wcslen(...));   // +0x13a8, 0x2e30
//     if (m_hWnd) { AdjustLayout();                                   // vslot 0x428
//                   ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW /*0x105*/); }
// (0x33d19c is the empty wide string.)  AdjustLayout goes to the exported
// CMFCToolBar body (DEVIATION for a derived override).
// Symbol: ?EnableHelpCombobox@CMFCMenuBar@@QEAAXIPEB_WH@Z
extern "C" void MS_ABI impl__EnableHelpCombobox_CMFCMenuBar__QEAAXIPEB_WH_Z(
    CMFCMenuBar* pThis, unsigned int uiID, const wchar_t* lpszPrompt, int nComboBoxWidth) {
    if (!pThis) return;
    At<unsigned int>(pThis, kOffHelpComboID) = uiID;
    At<int>(pThis, kOffHelpComboWidth) = nComboBoxWidth;
    At<CString>(pThis, kOffStrHelpComboPrompt) = (lpszPrompt != nullptr ? lpszPrompt : L"");
    if (HWndOf(pThis) != nullptr) {
        impl__AdjustLayout_CMFCToolBar__UEAAXXZ(AsToolBar(pThis));
        ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
}

// Retail (RVA 0x8d4c0, mfc140u), fully transcribed:
//     int iIndex = CMFCToolBar::FindDropIndex(point, rectDrag);       // 0x152a10
//     if (m_bMaximizeMode && iIndex >= 0) {                            // +0x1394
//         if (iIndex == 0 && m_hSysMenu != NULL) return -1;            // +0x1370
//         int nCount = m_Buttons.GetCount() - m_nSystemButtonsNum;     // +0x11a0, +0x1388
//         if (iIndex > nCount) {
//             iIndex = nCount;
//             if (m_nSystemButtonsNum > 0) {
//                 CMFCToolBarButton* p = GetButton(nCount - 1);        // 0x14fe00
//                 CPoint pt(p->m_rect.right, p->m_rect.top + (p->m_rect.bottom - p->m_rect.top) / 2);   // +0x68
//                 CMFCToolBar::FindDropIndex(pt, rectDrag);            // result discarded (only rectDrag is rewritten)
//             }
//         }
//     }
//     if (m_nHelpComboID != 0) {                                       // +0x1384
//         int iHelp = CommandToIndex(m_nHelpComboID, 0);               // 0x14eed0
//         if (iHelp >= 0 && iIndex > iHelp) iIndex = iHelp;
//     }
//     return iIndex;
// The base FindDropIndex is still a stub in featurepack/toolbar/CMFCToolBar.cpp
// (returns -1 and leaves the rect alone), so today the m_bMaximizeMode arm is
// never entered.  The `/ 2` is the signed cltd/sub/sar idiom.
// Symbol: ?FindDropIndex@CMFCMenuBar@@MEBAHVCPoint@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__FindDropIndex_CMFCMenuBar__MEBAHVCPoint__AEAVCRect___Z(const CMFCMenuBar* pThis, long long point, CRect* pRectDrag) {
    if (!pThis) return -1;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    int iIndex = impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(pBar, UnpackPoint(point), pRectDrag);
    if (At<int>(pThis, kOffMaximizeMode) != 0 && iIndex >= 0) {
        if (iIndex == 0 && At<HMENU>(pThis, kOffHSysMenu) != nullptr) return -1;
        const int nSys = At<int>(pThis, kOffSystemButtonsNum);
        const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar) - nSys;
        if (iIndex > nCount) {
            iIndex = nCount;
            if (nSys > 0) {
                CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nCount - 1);
                if (p != nullptr) {   // retail reads p->m_rect unconditionally
                    const CRect& r = p->m_rect;
                    const CPoint pt(r.right, r.top + (r.bottom - r.top) / 2);
                    (void)impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(pBar, pt, pRectDrag);
                }
            }
        }
    }
    const unsigned int nHelpID = At<unsigned int>(pThis, kOffHelpComboID);
    if (nHelpID != 0) {
        const int iHelp = impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(pBar, nHelpID, 0);
        if (iHelp >= 0 && iIndex > iHelp) iIndex = iHelp;
    }
    return iIndex;
}

// Retail (RVA 0x8d410, mfc140u), fully transcribed (static):
//     if (hMenu == NULL || nItemID == 0 || nItemID == (UINT)-1) return FALSE;   // `nItemID - 1 > 0xfffffffd`
//     CMenu* pMenu = CMenu::FromHandle(hMenu);  if (!pMenu) return FALSE;      // 0x2a80a0
//     int nCount = ::GetMenuItemCount(pMenu->m_hMenu);
//     for (int i = 0; i < nCount; i++) {
//         UINT uiID = ::GetMenuItemID(pMenu->m_hMenu, i);
//         if (uiID == nItemID) { pMenu->GetMenuString(i, strText, MF_BYPOSITION); return TRUE; }   // 0x2a81c0
//         if (uiID == (UINT)-1) {
//             CMenu* pSub = CMenu::FromHandle(::GetSubMenu(pMenu->m_hMenu, i));
//             if (!pSub) AfxThrowInvalidArgException();
//             if (FindMenuItemText(pSub->m_hMenu, nItemID, strText)) return TRUE;
//         }
//     }
//     return FALSE;
// The throw is kept as the exported AfxThrowInvalidArgException followed by an
// early return (the OpenMFC thunk does not unwind).
// Symbol: ?FindMenuItemText@CMFCMenuBar@@KAHPEAUHMENU__@@IAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__FindMenuItemText_CMFCMenuBar__KAHPEAUHMENU____IAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    HMENU hMenu, unsigned int nItemID, CString* pStrText) {
    if (hMenu == nullptr || pStrText == nullptr) return FALSE;
    if (nItemID - 1u > 0xfffffffdu) return FALSE;   // nItemID == 0 || nItemID == (UINT)-1
    CMenu* pMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(hMenu);
    if (pMenu == nullptr) return FALSE;
    const int nCount = ::GetMenuItemCount(pMenu->m_hMenu);
    for (int i = 0; i < nCount; ++i) {
        const unsigned int uiID = ::GetMenuItemID(pMenu->m_hMenu, i);
        if (uiID == nItemID) {
            impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
                pMenu, static_cast<unsigned int>(i), *pStrText, MF_BYPOSITION);
            return TRUE;
        }
        if (uiID == 0xffffffffu) {
            CMenu* pSub = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(::GetSubMenu(pMenu->m_hMenu, i));
            if (pSub == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return FALSE;
            }
            if (impl__FindMenuItemText_CMFCMenuBar__KAHPEAUHMENU____IAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                    pSub->m_hMenu, nItemID, pStrText)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// Retail (RVA 0x8e1e0, mfc140u), fully transcribed:
//     if (m_Buttons.Find(pButton) == NULL) return -1;                 // node walk from +0x1190
//     if (!IsFloating()) return -1;                                    // vslot 0x2f0 = `return m_bFloating` (+0x10d8, RVA 0x23a50)
//     // any wrapped button at all?  (walk; first p->m_bWrap != 0 wins) // +0x14
//     if (none) return -1;
//     int nRow = 0, nColumn = 0;
//     for (node = m_Buttons.head; node; node = node->next) {
//         CMFCToolBarButton* p = node->data;
//         if (p == pButton) {
//             if (p->m_bWrap) return 3;
//             if (nColumn == 0) return 4;
//             return nRow != 0 ? 1 : 2;                                // `neg; sbb; add 2`
//         }
//         if (p->m_bWrap) { nRow++; nColumn = 0; } else nColumn++;
//     }
//     return -1;
// IsFloating is read as the member the CMFCToolBar override returns.
// Symbol: ?GetFloatPopupDirection@CMFCMenuBar@@QEAAHPEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__GetFloatPopupDirection_CMFCMenuBar__QEAAHPEAVCMFCToolBarMenuButton___Z(CMFCMenuBar* pThis, CMFCToolBarMenuButton* pButton) {
    if (!pThis) return -1;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);

    bool bFound = false;
    for (int i = 0; i < nCount; ++i) {
        if (impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i) == pButton) { bFound = true; break; }
    }
    if (!bFound) return -1;
    if (pBar->m_bFloating == 0) return -1;

    bool bAnyWrap = false;
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (p != nullptr && p->m_bWrap != 0) { bAnyWrap = true; break; }
    }
    if (!bAnyWrap) return -1;

    int nRow = 0;
    int nColumn = 0;
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (p == pButton) {
            if (p->m_bWrap != 0) return 3;
            if (nColumn == 0) return 4;
            return nRow != 0 ? 1 : 2;
        }
        if (p != nullptr && p->m_bWrap != 0) {
            ++nRow;
            nColumn = 0;
        } else {
            ++nColumn;
        }
    }
    return -1;
}

// Retail (RVA 0x8e160, mfc140u), fully transcribed:
//     if (m_nHelpComboID == 0) return NULL;                            // +0x1384
//     int iIndex = CommandToIndex(m_nHelpComboID, 0);                  // 0x14eed0
//     if (iIndex <= 0) return NULL;                                    // `jle` -- index 0 is rejected too
//     CMFCToolBarButton* p = GetButton(iIndex);                        // 0x14fe00
//     return (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarComboBoxButton))) ? p : NULL;   // 0x3b1658
// Symbol: ?GetHelpCombobox@CMFCMenuBar@@QEAAPEAVCMFCToolBarComboBoxButton@@XZ
extern "C" void* MS_ABI impl__GetHelpCombobox_CMFCMenuBar__QEAAPEAVCMFCToolBarComboBoxButton__XZ(CMFCMenuBar* pThis) {
    if (!pThis) return nullptr;
    const unsigned int nHelpID = At<unsigned int>(pThis, kOffHelpComboID);
    if (nHelpID == 0) return nullptr;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    const int iIndex = impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(pBar, nHelpID, 0);
    if (iIndex <= 0) return nullptr;
    CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iIndex);
    if (!IsKindOfRT(p, impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ())) return nullptr;
    return p;
}

// Retail (RVA 0x8d260, mfc140u), fully transcribed:
//     int cy;
//     if (m_bHaveButtons)                                              // +0x1398
//         cy = GetButtonSize().cy;                                     // inline 0x239b4, see below
//     else
//         cy = (m_sizeMenuButton.cy > 0 ? m_sizeMenuButton.cy : m_sizeButton.cy) - 2;   // 0x3b1d0c / 0x3b1e3c
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }
//     int cyText = (GetCurrentAlignment() & CBRS_ORIENT_HORZ /*0xa000*/)   // vslot 0x338, 0xcdb0
//                      ? afxGlobalData.<+0x2a8> : afxGlobalData.<+0x2ac>;
//     if (cyText > cy) return cyText;    // retail recomputes both operands after the compare, same values
//     return cy;
// The afxtoolbar.h GetButtonSize inline (0x239b4) selects
//     m_bLocked ? (m_bLargeIconsAreEnbaled ? m_sizeCurButtonLocked : m_sizeButtonLocked)   // +0x10b8, +0x10bc, +0x1168, +0x1158
//               : (m_bLargeIconsAreEnbaled ? m_sizeCurButton : m_sizeButton)               // 0x3b1ee8, 0x3b1e38
// and adds CMFCVisualManager::GetButtonExtraBorder() only when
// IsButtonExtraSizeAvailable() (vslot 0x6c8) -- which in the CMFCMenuBar
// vftable is 0x71e0, `return 0` (afxmenubar.h: `return FALSE`), so for this
// class the inline reduces to the selection above and is reproduced exactly.
// Symbol: ?GetRowHeight@CMFCMenuBar@@UEBAHXZ
extern "C" int MS_ABI impl__GetRowHeight_CMFCMenuBar__UEBAHXZ(const CMFCMenuBar* pThis) {
    if (!pThis) return 0;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    int cy;
    if (At<int>(pThis, kOffHaveButtons) != 0) {
        if (pBar->m_bLocked) {
            cy = static_cast<int>(pBar->m_bLargeIconsAreEnbaled ? pBar->m_sizeCurButtonLocked.cy : pBar->m_sizeButtonLocked.cy);
        } else {
            cy = static_cast<int>(pBar->m_bLargeIconsAreEnbaled ? impl__m_sizeCurButton_CMFCToolBar__1VCSize__A.cy
                                                                : impl__m_sizeButton_CMFCToolBar__1VCSize__A.cy);
        }
    } else {
        const long cyMenu = impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cy;
        cy = static_cast<int>(cyMenu > 0 ? cyMenu : impl__m_sizeButton_CMFCToolBar__1VCSize__A.cy) - 2;
    }
    EnsureGlobalDataInitialized();
    const bool bHorz = (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pBar) & kCbrsOrientHorz) != 0;
    const int cyText = GlobalDataInt(bHorz ? kGlobalDataTextHeightHorz : kGlobalDataTextHeightVert);
    return cyText > cy ? cyText : cy;
}

// Retail (RVA 0x8d6b0, mfc140u), fully transcribed:
//     if (!m_bMaximizeMode) return NULL;                               // +0x1394
//     if (bByCommand) {
//         for (node = m_Buttons.tail; node; node = node->prev) {       // +0x1198
//             CMFCToolBarButton* p = node->data;
//             if (!p || !p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton))) return NULL;   // 0x3188a0
//             if (p->m_nID == uiBtn) return p;                         // +0x24
//         }
//         return NULL;
//     }
//     if ((int)uiBtn < 0 || (int)uiBtn >= m_nSystemButtonsNum) return NULL;   // +0x1388
//     CMFCToolBarButton* p = GetButton(m_Buttons.GetCount() - m_nSystemButtonsNum + uiBtn);   // +0x11a0, 0x14fe00
//     return (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton))) ? p : NULL;
// Symbol: ?GetSystemButton@CMFCMenuBar@@QEBAPEAVCMFCToolBarMenuButtonsButton@@IH@Z
extern "C" void* MS_ABI impl__GetSystemButton_CMFCMenuBar__QEBAPEAVCMFCToolBarMenuButtonsButton__IH_Z(const CMFCMenuBar* pThis, unsigned int uiBtn, int bByCommand) {
    if (!pThis) return nullptr;
    if (At<int>(pThis, kOffMaximizeMode) == 0) return nullptr;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    CRuntimeClass* pMbbClass = impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ();
    if (bByCommand != 0) {
        for (int i = nCount - 1; i >= 0; --i) {
            CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
            if (!IsKindOfRT(p, pMbbClass)) return nullptr;
            if (p->m_nID == uiBtn) return p;
        }
        return nullptr;
    }
    const int nSys = At<int>(pThis, kOffSystemButtonsNum);
    if (static_cast<int>(uiBtn) < 0 || static_cast<int>(uiBtn) >= nSys) return nullptr;
    CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nCount - nSys + static_cast<int>(uiBtn));
    if (!IsKindOfRT(p, pMbbClass)) return nullptr;
    return p;
}

// Retail (RVA 0x8d660, mfc140u), fully transcribed:
//     if (!m_bMaximizeMode || m_Buttons.GetCount() == 0) return NULL;   // +0x1394, +0x11a0
//     CMFCToolBarButton* p = m_Buttons.GetHead();                      // +0x1190 -> node->data
//     return (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarSystemMenuButton))) ? p : NULL;   // 0x3b17a8
// Symbol: ?GetSystemMenu@CMFCMenuBar@@QEBAPEAVCMFCToolBarSystemMenuButton@@XZ
extern "C" void* MS_ABI impl__GetSystemMenu_CMFCMenuBar__QEBAPEAVCMFCToolBarSystemMenuButton__XZ(const CMFCMenuBar* pThis) {
    if (!pThis) return nullptr;
    if (At<int>(pThis, kOffMaximizeMode) == 0) return nullptr;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    if (impl__GetCount_CMFCToolBar__QEBAHXZ(pBar) == 0) return nullptr;
    CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, 0);
    if (!IsKindOfRT(p, impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ())) return nullptr;
    return p;
}

// Retail (RVA 0x8e0b0, mfc140u), fully transcribed:
//     if (m_bMenuMode) return TRUE;                                    // +0x10ec
//     return pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton));  // 0x3b1748
// Symbol: ?IsPureMenuButton@CMFCMenuBar@@MEBAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__IsPureMenuButton_CMFCMenuBar__MEBAHPEAVCMFCToolBarButton___Z(const CMFCMenuBar* pThis, CMFCToolBarButton* pButton) {
    if (!pThis) return FALSE;
    if (AsToolBar(pThis)->m_bMenuMode != 0) return TRUE;
    return IsKindOfRT(pButton, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ()) ? TRUE : FALSE;
}

// Retail (RVA 0x8c1f0..0x8c5f4, mfc140u, ~250 instructions), decoded, NOT
// implemented:
//     CString strProfile = AFXGetRegPath(L"MFCMenuBar-..." /*profile name*/, lpszProfileName);   // 0xd2070
//     SetMaximizeMode(FALSE, NULL, TRUE);                              // 0x8b6f0
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;           // 0x133930, +0x8
//     if (pApp->m_pDocManager /*+0xb8*/) {
//         for each CDocTemplate (CDocManager vslots 0x30 / 0x38) that IsKindOf(CMultiDocTemplate) (0x329588)
//             with a non-NULL m_hMenuShared (+0xf0):
//             if (m_bAutoDocMenus /*+0x1350*/ && !afxMenuHash.LoadMenuBar(hMenuShared, this)) {   // 0x8ee80
//                 CMFCToolBar::LoadState(strProfile, nIndex, uiID);    // 0x1539c0
//                 afxMenuHash.SaveMenuBar(hMenuShared, this);          // 0x8eb40
//             } else {
//                 the owner (m_hWndOwner ? : ::GetParent(m_hWnd)) is asked WM_IDLEUPDATECMDUI-style
//                 for a CMDIFrameWnd active child ... and the shared menu re-associated
//             }
//     }
//     the same LoadMenuBar / LoadState / SaveMenuBar sequence for m_hDefaultMenu (+0x1368),
//     then afxMenuHash.LoadMenuBar(m_hMenu /*+0x1360*/, this); RestoreMaximizeMode(TRUE) (0x8baa0)
//     when it was on; GetParentFrame()->vslot 0x300 (RecalcLayout), InvalidateRect, UpdateWindow;
//     m_bResourceWasChanged /*+0x10d0*/ handling via 0x1563b0; return TRUE.
// Left a stub: the per-document-template shared menus (CMultiDocTemplate::
// m_hMenuShared, +0xf0 -- on this side an unnamed, never-written, never-zeroed
// slot of _multidoctemplate_padding, see kOffMultiDocTemplateMenuShared), the
// CDocManager vtable walk, and the afxMenuHash save/load of a
// whole button list (OpenMFC's CMenuHash is a HMENU->bar map that stores no
// buttons) are all infrastructure this repo does not have; a partial
// transcription would restore nothing and could clear a live bar.
// Symbol: ?LoadState@CMFCMenuBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCMenuBar__UEAAHPEB_WHI_Z(CMFCMenuBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return FALSE;
}

// Retail (RVA 0x8d580, mfc140u), fully transcribed:
//     CMFCToolBar::OnChangeHot(iHot);                                  // 0x1576f0
//     ::KillTimer(m_hWnd, 0xec12);
//     if (GetDroppedDownMenu(NULL) == NULL)                            // 0x157f30
//         m_bShowAllCommands = FALSE;                                  // 0x3be254
//     else
//         ::SetTimer(m_hWnd, 0xec12, 5000, NULL);
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }
//     if (afxGlobalData.<+0x278>) AccNotifyObjectFocusEvent(m_iHighlighted);   // vslot 0x4c8, +0x1138
// (The import slots resolve to KillTimer and SetTimer.)  AccNotifyObjectFocusEvent
// goes to the exported CMFCToolBar body; OpenMFC's afxGlobalData is a zeroed
// block, so the +0x278 gate is closed today exactly as it would be in retail
// with no accessibility client.
// Symbol: ?OnChangeHot@CMFCMenuBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(CMFCMenuBar* pThis, int iHot) {
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pBar, iHot);
    ::KillTimer(HWndOf(pThis), kTimerShowAllCommands);
    if (impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pBar, nullptr) == nullptr) {
        impl__m_bShowAllCommands_CMFCMenuBar__1HA = FALSE;
    } else {
        ::SetTimer(HWndOf(pThis), kTimerShowAllCommands, 5000, nullptr);
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalDataAccessibility) != 0) {
        impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(pBar, pBar->m_iHighlighted);
    }
}

// Retail (RVA 0x8c080, mfc140u), fully decoded:
//     if (CMFCToolBar::OnCreate(lpCreateStruct) == -1) return -1;      // 0x151ff0
//     CFrameImpl* pImpl = NULL;
//     if (m_pParentWnd && m_pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))       pImpl = m_pParentWnd + 0x590;   // 0x2ee478
//     else if (m_pParentWnd && m_pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))     pImpl = m_pParentWnd + 0x1f0;   // 0x2e9210
//     else { CFrameWnd* p = GetParentFrame(); if (p && p->IsKindOf(RUNTIME_CLASS(COleIPFrameWndEx))) pImpl = p + 0x298; }   // 0x2f1780
//     if (pImpl) pImpl->SetMenuBar(this);                              // 0x63650 = ?SetMenuBar@CFrameImpl@@
//     CString str;
//     if (!str.LoadString(AfxFindStringResourceHandle(IDS_AFXBARRES_MENU_BAR_TITLE), IDS_AFXBARRES_MENU_BAR_TITLE))   // 0x3e86
//         AfxThrowInvalidArgException();
//     SetWindowText(str);                                              // 0x2a9790
//     SetPaneStyle(GetPaneStyle() | CBRS_TOOLTIPS);                    // vslots 0x390 / 0x3d8 = m_dwStyle (+0x104) get/set
//     CalcSysButtonSize();                                             // 0x8daa0
//     return 0;
// PARTIAL: the CFrameImpl::SetMenuBar delegation is dropped.  Retail reaches
// the CFrameImpl EMBEDDED in the frame at +0x590 / +0x1f0 / +0x298; OpenMFC's
// CMDIFrameWndEx / CFrameWndEx / COleIPFrameWndEx objects do not embed one
// (core/frame/CMDIFrameWndEx.cpp keeps a heap-built m_Impl in a per-frame
// record that this file cannot reach), so there is no object to hand to the
// exported ?SetMenuBar@CFrameImpl@@ thunk.  Everything else is transcribed; the
// LoadString failure keeps retail's throw as the exported thunk + `return -1`.
// Symbol: ?OnCreate@CMFCMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCMenuBar* pThis, LPCREATESTRUCTW lpCreateStruct) {
    if (!pThis) return -1;
    if (impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(AsToolBar(pThis), lpCreateStruct) == -1) return -1;

    // TODO(clean-room): retail calls CFrameImpl::SetMenuBar(this) on the
    // parent frame's embedded CFrameImpl here (see the note above).

    CString str;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kIdsMenuBarTitle));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, kIdsMenuBarTitle)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(AsWnd(pThis), static_cast<const wchar_t*>(str));
    pThis->m_dwStyle |= kCbrsToolTips;
    impl__CalcSysButtonSize_CMFCMenuBar__IEAAXXZ(pThis);
    return 0;
}

// Retail (RVA 0x8d920, mfc140u), fully transcribed:
//     int iHit = HitTest(point);                                       // vslot 0x730, 0x150390
//     BOOL bSysMenu = FALSE;
//     if (iHit >= 0) { p = GetButton(iHit); bSysMenu = p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarSystemMenuButton)); }   // 0x3b17a8
//     CMFCToolBar::OnLButtonDblClk(nFlags, point);                     // 0x1560c0
//     if (bSysMenu) return;
//     if (m_bShowAllCommands || !m_bRecentlyUsedMenus ||               // 0x3be254, 0x3b1b08
//         !m_UsageCount.HasEnoughInformation() ||                      // inline: 0x3b2010 (m_nTotalCount) < m_nStartCount (0x3be1ac), unsigned
//         m_bCustomizeMode) return;                                    // 0x3be35c
//     iHit = HitTest(point);  if (iHit < 0) return;
//     CMFCToolBarButton* pBtn = GetButton(iHit);
//     if (!pBtn || !pBtn->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return;   // 0x3b1748
//     if (pBtn->IsKindOf(RUNTIME_CLASS(CMFCToolBarSystemMenuButton))) return;
//     m_bShowAllCommands = TRUE;
//     pBtn->OnCancelMode();                                            // button vslot 0xc0
//     if (pBtn->m_nStyle & TBBS_DISABLED) return;                      // +0x28, 0x40000
//     if (!pBtn->OnClick(this, FALSE)) return;                         // button vslot 0x50
//     OnChangeHot(iHit);                                               // vslot 0x770
//     InvalidateButton(iHit);                                          // 0x14fe50
//     ::UpdateWindow(m_hWnd);
// The two button virtuals go to the exported CMFCToolBarMenuButton bodies --
// the object was just verified to be one -- (DEVIATION for a client-derived
// override), both of which are still stubs with auto-generated parameter
// lists in featurepack/toolbar/CMFCToolBarMenuButton.cpp, so OnClick returns
// 0 and the tail (OnChangeHot / InvalidateButton / UpdateWindow) is not
// reached today.  HitTest and OnChangeHot are this class's exports.
// Symbol: ?OnLButtonDblClk@CMFCMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCMenuBar__IEAAXIVCPoint___Z(CMFCMenuBar* pThis, unsigned int nFlags, long long point) {
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    const CPoint pt = UnpackPoint(point);

    int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, pt);
    bool bSysMenu = false;
    if (iHit >= 0) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
        bSysMenu = IsKindOfRT(p, impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ()) != 0;
    }
    impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(pBar, nFlags, pt);
    if (bSysMenu) return;
    if (impl__m_bShowAllCommands_CMFCMenuBar__1HA != 0) return;
    if (impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA == 0) return;
    if (!impl__HasEnoughInformation_CMFCCmdUsageCount__QEBAHXZ(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A)) return;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;

    iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, pt);
    if (iHit < 0) return;
    CMFCToolBarButton* pBtn = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
    if (!IsKindOfRT(pBtn, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ())) return;
    if (IsKindOfRT(pBtn, impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ())) return;

    impl__m_bShowAllCommands_CMFCMenuBar__1HA = TRUE;
    CMFCToolBarMenuButton* pMenuBtn = static_cast<CMFCToolBarMenuButton*>(pBtn);
    impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(pMenuBtn);                     // button vslot 0xc0
    if (pBtn->m_nStyle & kTbbsDisabled) return;
    if (!impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(pMenuBtn, AsWnd(pThis), FALSE)) return;   // button vslot 0x50
    impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(pThis, iHit);                            // vslot 0x770
    impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
    ::UpdateWindow(HWndOf(pThis));
}

// Retail (RVA 0x8c9d0, mfc140u), fully transcribed:
//     if (HitTest(point) < 0) GetDroppedDownMenu(NULL);               // vslot 0x730; the result is discarded
//     CMFCToolBar::OnLButtonDown(nFlags, point);                       // 0x150410 (tail jump)
// The base handler is still a documented stub in featurepack/toolbar/CMFCToolBar.cpp.
// Symbol: ?OnLButtonDown@CMFCMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCMenuBar__IEAAXIVCPoint___Z(CMFCMenuBar* pThis, unsigned int nFlags, long long point) {
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    const CPoint pt = UnpackPoint(point);
    if (impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, pt) < 0) {
        (void)impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pBar, nullptr);
    }
    impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(pBar, nFlags, pt);
}

// Retail (RVA 0x8bf50, mfc140u), fully transcribed:
//     if (!pButton || !pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton))) return FALSE;   // 0x3188a0
//     UINT uiCmd = pButton->m_uiSystemCommand;                         // +0x88
//     if (((uiCmd - SC_MINIMIZE) & ~0x140) == 0 && uiCmd != 0xf160)    // i.e. SC_MINIMIZE, SC_CLOSE or SC_RESTORE
//     {
//         if (m_pParentWnd && m_pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))) {   // +0x1358, 0x33c180
//             CMDIChildWnd* pChild = ((CMDIFrameWnd*)m_pParentWnd)->MDIGetActive(NULL);  // 0x2a6ad0
//             ::SendMessage(pChild->m_hWnd, WM_SYSCOMMAND, uiCmd, 0);   // no NULL test in retail
//         } else {
//             ::MessageBeep((UINT)-1);
//         }
//     }
//     return TRUE;
// (The import slots resolve to SendMessageW and MessageBeep.)  DEVIATION: a
// NULL active child is skipped instead of faulting.  m_uiSystemCommand is read
// at the retail +0x88 (see kOffMbbSystemCommand).
// Symbol: ?OnSendCommand@CMFCMenuBar@@UEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCMenuBar__UEAAHPEBVCMFCToolBarButton___Z(CMFCMenuBar* pThis, const CMFCToolBarButton* pButton) {
    if (!pThis) return FALSE;
    if (!IsKindOfRT(pButton, impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ())) return FALSE;
    const unsigned int uiCmd = At<unsigned int>(pButton, kOffMbbSystemCommand);
    if (((uiCmd - SC_MINIMIZE) & 0xfffffebfu) == 0 && uiCmd != 0xf160u) {
        CWnd* pParent = At<CWnd*>(pThis, kOffParentWnd);
        if (IsKindOfRT(pParent, impl__GetThisClass_CMDIFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            CMDIChildWnd* pChild = impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(static_cast<CMDIFrameWnd*>(pParent), nullptr);
            if (pChild != nullptr) {
                ::SendMessage(HWndOf(pChild), WM_SYSCOMMAND, uiCmd, 0);
            }
        } else {
            ::MessageBeep(static_cast<UINT>(-1));
        }
    }
    return TRUE;
}

// Retail (RVA 0x8d370, mfc140u), fully transcribed:
//     CString strText;
//     if (FindMenuItemText(m_hMenu, pButton->m_nID, strText)) {        // +0x1360, +0x24, 0x8d410
//         pButton->m_strText = strText;                                // +0x38, CSimpleStringT::operator=
//         return TRUE;
//     }
//     return CMFCToolBar::OnSetDefaultButtonText(pButton);             // 0x1572f0
// Symbol: ?OnSetDefaultButtonText@CMFCMenuBar@@UEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCMenuBar__UEAAHPEAVCMFCToolBarButton___Z(CMFCMenuBar* pThis, CMFCToolBarButton* pButton) {
    if (!pThis || !pButton) return FALSE;
    CString strText;
    if (impl__FindMenuItemText_CMFCMenuBar__KAHPEAUHMENU____IAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            At<HMENU>(pThis, kOffHMenu), pButton->m_nID, &strText)) {
        pButton->m_strText = strText;
        return TRUE;
    }
    return impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(AsToolBar(pThis), pButton);
}

// Retail (RVA 0x8d130, mfc140u), fully transcribed:
//     CMFCToolBar::OnSetFocus(pOldWnd);                                // 0x158660
//     if (GetDroppedDownMenu(NULL) != NULL) return;                    // 0x157f30
//     CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd));   // +0xa0, 0x28ad70
//     ::SendMessage(pOwner->m_hWnd, WM_SETMESSAGESTRING /*0x362*/, AFX_IDS_IDLEMESSAGE /*0xe001*/, 0);
//     int iFirst = (GetSystemMenu() != NULL) ? 1 : 0;                  // 0x8d660
//     if (m_iHighlighted < 0 && iFirst < m_Buttons.GetCount()) {       // +0x1138, +0x11a0
//         m_iHighlighted = iFirst;
//         InvalidateButton(iFirst);                                    // 0x14fe50
//     }
// (The import slots resolve to GetParent and SendMessageW.)  A NULL FromHandle
// result is skipped rather than dereferenced (retail's FromHandle never
// returns NULL for a valid HWND); OwnerHwnd carries the m_hWndOwner deviation.
// Symbol: ?OnSetFocus@CMFCMenuBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCMenuBar__IEAAXPEAVCWnd___Z(CMFCMenuBar* pThis, CWnd* pOldWnd) {
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    impl__OnSetFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(pBar, pOldWnd);
    if (impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pBar, nullptr) != nullptr) return;

    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
    if (pOwner != nullptr) {
        ::SendMessage(HWndOf(pOwner), kWmSetMessageString, kAfxIdsIdleMessage, 0);
    }
    const int iFirst = (impl__GetSystemMenu_CMFCMenuBar__QEBAPEAVCMFCToolBarSystemMenuButton__XZ(pThis) != nullptr) ? 1 : 0;
    if (pBar->m_iHighlighted < 0 && iFirst < impl__GetCount_CMFCToolBar__QEBAHXZ(pBar)) {
        pBar->m_iHighlighted = iFirst;
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, iFirst);
    }
}

// Retail (RVA 0x8dc30, mfc140u), fully transcribed:
//     CMFCToolBar::OnSettingChange(uFlags, lpszSection);               // 0x159620
//     CalcSysButtonSize();                                             // 0x8daa0
//     ::InvalidateRect(m_hWnd, NULL, TRUE);
//     ::UpdateWindow(m_hWnd);                                          // tail jump
// Symbol: ?OnSettingChange@CMFCMenuBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCMenuBar__IEAAXIPEB_W_Z(CMFCMenuBar* pThis, unsigned int uFlags, const wchar_t* lpszSection) {
    if (!pThis) return;
    impl__OnSettingChange_CMFCToolBar__IEAAXIPEB_W_Z(AsToolBar(pThis), uFlags, lpszSection);
    impl__CalcSysButtonSize_CMFCMenuBar__IEAAXXZ(pThis);
    ::InvalidateRect(HWndOf(pThis), nullptr, TRUE);
    ::UpdateWindow(HWndOf(pThis));
}

// Retail (RVA 0x8d860, mfc140u), fully decoded:
//     if (nIDEvent == 0xec12) {
//         CPoint pt;  ::GetCursorPos(&pt);  ::ScreenToClient(m_hWnd, &pt);
//         CMFCToolBarMenuButton* pMenu = GetDroppedDownMenu(NULL);     // 0x157f30
//         if (pMenu && pMenu->m_pPopupMenu /*+0xc0*/ && ::PtInRect(&pMenu->m_rect /*+0x68*/, pt)) {
//             CMFCPopupMenu* pPopup = pMenu->m_pPopupMenu;
//             if (!pPopup->GetMenuBar()->m_bAreAllCommandsShown)       // vslot 0x3a0 = the bar embedded at +0x230; its +0x1380
//                 pPopup->ShowAllCommands();                           // 0xb8150
//         }
//         ::KillTimer(m_hWnd, 0xec12);
//     }
//     CWnd::Default();                                                 // 0x28ac80 (tail jump)
// PARTIAL: the timer's payload -- the "show all commands after hovering the
// open menu for 5 s" step -- is dropped, and only the KillTimer and the
// CWnd::Default forward are kept.  Retail decides it by reading
// m_bAreAllCommandsShown of the CMFCPopupMenuBar EMBEDDED in the popup at
// +0x230; OpenMFC's CMFCPopupMenu is 0x238 bytes and never constructs that
// bar (see the object-model note in CMFCPopupMenu.cpp), so the flag cannot be
// read, and calling ShowAllCommands without the test would re-hide/re-show and
// re-sound a menu that already shows everything.
// Symbol: ?OnTimer@CMFCMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCMenuBar__IEAAX_K_Z(CMFCMenuBar* pThis, unsigned __int64 nIDEvent) {
    if (!pThis) return;
    if (nIDEvent == kTimerShowAllCommands) {
        // TODO(clean-room): retail's ShowAllCommands step is not reproducible
        // here (see the note above).
        ::KillTimer(HWndOf(pThis), kTimerShowAllCommands);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail (RVA 0x8c000, mfc140u), fully transcribed:
//     int iHit = HitTest(point);                                       // vslot 0x730
//     if (iHit != -1) {
//         CMFCToolBarButton* p = GetButton(iHit);                      // 0x14fe00
//         if (p == NULL || p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return -1;   // 0x3b1748
//     }
//     return CMFCToolBar::OnToolHitTest(point, pTI);                   // 0x14ff80
// (menu buttons never get a tooltip.)
// Symbol: ?OnToolHitTest@CMFCMenuBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCMenuBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(const CMFCMenuBar* pThis, long long point, TOOLINFOW* pTI) {
    if (!pThis) return -1;
    const CMFCToolBar* pBar = AsToolBar(pThis);
    const CPoint pt = UnpackPoint(point);
    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(const_cast<CMFCToolBar*>(pBar), pt);
    if (iHit != -1) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
        if (p == nullptr || IsKindOfRT(p, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ())) return -1;
    }
    return impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pBar, pt, pTI);
}

// Retail (RVA 0x8cf20, mfc140u), fully transcribed:
//     if (pMsg->message == WM_KEYDOWN) {
//         if (GetDroppedDownMenu(NULL)) return CBasePane::PreTranslateMessage(pMsg);   // 0x157f30, 0xcb30
//         int nCount = m_Buttons.GetCount();                           // +0x11a0
//         if (m_bMaximizeMode) nCount -= m_nSystemButtonsNum;          // +0x1394, +0x1388
//         int iHot = m_iHighlighted;                                   // +0x1138
//         if (iHot >= 0 && iHot < nCount) {
//             UINT nChar = (UINT)pMsg->wParam;
//             if (nChar == VK_TAB) nChar = (::GetKeyState(VK_SHIFT) & 0x80) ? VK_LEFT : VK_RIGHT;
//             BOOL bRTL = GetExStyle() & WS_EX_LAYOUTRTL;              // 0x2a96c0, bit 22
//             if (bRTL && nChar == VK_LEFT)  goto right;               // the two arrows swap under RTL
//             if (bRTL && nChar == VK_RIGHT) goto left;
//             switch (nChar) {
//             case VK_RETURN: if (!DropDownMenu(GetButton(iHot)))      // 0x157e10
//                                 ProcessCommand(GetButton(iHot));     // 0x157eb0 (not exported)
//                             return TRUE;
//             case VK_ESCAPE: Deactivate(); RestoreFocus();            // vslots 0x6d0 / 0x6d8
//                             m_bShowAllCommands = FALSE; break;       // 0x3be254
//             case VK_LEFT:  left:  if (--m_iHighlighted < 0) m_iHighlighted = nCount - 1;  goto redraw;
//             case VK_RIGHT: right: if (++m_iHighlighted >= nCount) m_iHighlighted = 0;
//                            redraw: InvalidateButton(iHot); InvalidateButton(m_iHighlighted);   // 0x14fe50
//                                    ::UpdateWindow(m_hWnd);
//                                    AccNotifyObjectFocusEvent(m_iHighlighted);   // vslot 0x4c8
//                                    break;
//             case VK_DOWN:  DropDownMenu(GetButton(iHot)); return TRUE;
//             default:       if (TranslateChar(nChar)) return TRUE;    // vslot 0x738
//             }
//         }
//     }
//     return CMFCToolBar::PreTranslateMessage(pMsg);                   // 0x154940
// (The import slot resolves to GetKeyState.)  ProcessCommand has no export and
// no OpenMFC body, so the VK_RETURN fallback after a failed DropDownMenu is
// dropped (DEVIATION); DropDownMenu itself is still a stub in
// featurepack/toolbar/CMFCToolBar.cpp, and CBasePane::PreTranslateMessage is a
// stub returning 0.  The other virtuals go to the exported CMFCToolBar bodies.
// Symbol: ?PreTranslateMessage@CMFCMenuBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCMenuBar__UEAAHPEAUtagMSG___Z(CMFCMenuBar* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;
    CMFCToolBar* pBar = AsToolBar(pThis);
    if (pMsg->message == WM_KEYDOWN) {
        if (impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pBar, nullptr) != nullptr) {
            return impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(static_cast<CBasePane*>(pBar), pMsg);
        }
        int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
        if (At<int>(pThis, kOffMaximizeMode) != 0) nCount -= At<int>(pThis, kOffSystemButtonsNum);
        const int iHot = pBar->m_iHighlighted;
        if (iHot >= 0 && iHot < nCount) {
            unsigned int nChar = static_cast<unsigned int>(pMsg->wParam);
            if (nChar == VK_TAB) {
                nChar = (::GetKeyState(VK_SHIFT) & 0x80) ? VK_LEFT : VK_RIGHT;
            }
            const bool bRTL = (impl__GetExStyle_CWnd__QEBAKXZ(AsWnd(pThis)) & WS_EX_LAYOUTRTL) != 0;
            bool bMoveLeft = false;
            bool bMoveRight = false;
            if (bRTL && nChar == VK_LEFT)       bMoveRight = true;
            else if (bRTL && nChar == VK_RIGHT) bMoveLeft = true;
            else if (nChar == VK_LEFT)          bMoveLeft = true;
            else if (nChar == VK_RIGHT)         bMoveRight = true;

            if (bMoveLeft || bMoveRight) {
                if (bMoveLeft) {
                    if (--pBar->m_iHighlighted < 0) pBar->m_iHighlighted = nCount - 1;
                } else {
                    if (++pBar->m_iHighlighted >= nCount) pBar->m_iHighlighted = 0;
                }
                impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, iHot);
                impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, pBar->m_iHighlighted);
                ::UpdateWindow(HWndOf(pThis));
                impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(pBar, pBar->m_iHighlighted);   // vslot 0x4c8
            } else if (nChar == VK_RETURN) {
                if (!impl__DropDownMenu_CMFCToolBar__IEAAHPEAVCMFCToolBarButton___Z(
                        pBar, impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHot))) {
                    // TODO(clean-room): retail calls CMFCToolBar::ProcessCommand(GetButton(iHot))
                    // here (RVA 0x157eb0, not exported); no OpenMFC body exists for it.
                }
                return TRUE;
            } else if (nChar == VK_ESCAPE) {
                impl__Deactivate_CMFCToolBar__UEAAXXZ(pBar);      // vslot 0x6d0
                impl__RestoreFocus_CMFCToolBar__UEAAXXZ(pBar);    // vslot 0x6d8
                impl__m_bShowAllCommands_CMFCMenuBar__1HA = FALSE;
            } else if (nChar == VK_DOWN) {
                impl__DropDownMenu_CMFCToolBar__IEAAHPEAVCMFCToolBarButton___Z(
                    pBar, impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHot));
                return TRUE;
            } else if (impl__TranslateChar_CMFCToolBar__UEAAHI_Z(pBar, nChar)) {   // vslot 0x738
                return TRUE;
            }
        }
    }
    return impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(pBar, pMsg);
}

// Retail (RVA 0x8c860, mfc140u), transcribed:
//     if (m_hDefaultMenu == NULL) AfxThrowInvalidArgException();      // +0x1368, 0x227720
//     afxMenuHash.SaveMenuBar(m_hMenu, this);                          // 0x3b1d10, 0x8eb40, +0x1360
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;           // 0x133930, +0x8
//     if (pApp->m_pDocManager /*+0xb8*/) {
//         for (POSITION pos = pDocManager->GetFirstDocTemplatePosition(); pos; ) {   // vslot 0x30 of the CDocManager vftable (0x3293b8)
//             CDocTemplate* pTemplate = pDocManager->GetNextDocTemplate(pos);        // vslot 0x38
//             if (pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)) /*0x329588*/ && ((CMultiDocTemplate*)pTemplate)->m_hMenuShared /*+0xf0*/
//                 && afxMenuHash.LoadMenuBar(hMenuShared, this)) {     // 0x8ee80
//                 CMFCToolBar::ResetImages();                          // direct call, 0x159300
//                 afxMenuHash.SaveMenuBar(hMenuShared, this);
//             }
//         }
//     }
//     if (afxMenuHash.LoadMenuBar(m_hDefaultMenu, this)) { CMFCToolBar::ResetImages(); afxMenuHash.SaveMenuBar(m_hDefaultMenu, this); }
//     if (m_hMenu != NULL && afxMenuHash.LoadMenuBar(m_hMenu, this)) {
//         GetParentFrame()->RecalcLayout(TRUE);                        // 0x28e200, then vslot 0x300 of the CFrameWnd vftable (0x33a528) = ?RecalcLayout@CFrameWnd@@
//         ::InvalidateRect(m_hWnd, NULL, TRUE);  ::UpdateWindow(m_hWnd);   // import slots 0x2c7128 / 0x2c7300
//     }
// Deviations, all forced by what OpenMFC models:
//   * pApp->m_pDocManager and its two vslots are replaced by the exported
//     CWinApp::GetFirstDocTemplatePosition / GetNextDocTemplate (retail RVAs
//     0x1cf780 / 0x1cf7a0, mfc140u: both forward to the same m_pDocManager
//     vslots 0x30 / 0x38, the first returning NULL and the second throwing
//     when m_pDocManager is NULL); a NULL AfxGetApp() skips the walk instead
//     of faulting.
//   * OpenMFC's CMenuHash::LoadMenuBar / SaveMenuBar only record the
//     (hMenu -> bar) pair and return TRUE -- no button list is swapped in --
//     so the walk resets the bar's one live button set rather than each
//     saved set.
//   * m_hMenuShared (+0xf0, see kOffMultiDocTemplateMenuShared) is never
//     written NOR zeroed on this side, so the slot holds whatever the
//     allocation held; retail only tests it for NULL, this body additionally
//     requires ::IsMenu() so that an indeterminate value cannot be recorded
//     in afxMenuHash.  (For a retail-valid value -- NULL or a live HMENU --
//     the two tests agree.)  Until the header names and initialises the
//     member, the per-template branch is therefore effectively dead.
//   * CMFCToolBar::ResetImages is still an empty stub in CMFCToolBar.cpp; the
//     call is kept so the images reset once it is implemented.
//   * GetParentFrame() is null-checked (retail dereferences it) and reaches
//     the exported CFrameWnd::RecalcLayout rather than vslot 0x300.
//   * retail calls IsKindOf on the template pointer unchecked; IsKindOfRT
//     treats a NULL GetNextDocTemplate result as "not a CMultiDocTemplate".
// m_hDefaultMenu / m_hMenu are set by CreateFromMenu above exactly where
// retail sets them, so the entry check throws in the same situations.
// Symbol: ?ResetImages@CMFCMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__ResetImages_CMFCMenuBar__MEAAXXZ(CMFCMenuBar* pThis) {
    if (!pThis) return;
    if (At<HMENU>(pThis, kOffHDefaultMenu) == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    CMenuHash* pHash = reinterpret_cast<CMenuHash*>(impl__afxMenuHash__3VCMenuHash__A);
    CMFCToolBar* pBar = AsToolBar(pThis);
    impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, At<HMENU>(pThis, kOffHMenu), pBar);

    if (CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        void* pos = impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(pApp);
        while (pos != nullptr) {
            CDocTemplate* pTemplate = impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(pApp, &pos);
            if (!IsKindOfRT(pTemplate, impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ())) continue;
            HMENU hMenuShared = At<HMENU>(pTemplate, kOffMultiDocTemplateMenuShared);
            if (hMenuShared == nullptr || !::IsMenu(hMenuShared)) continue;   // DEVIATION: ::IsMenu, see above
            if (impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, hMenuShared, pBar)) {
                impl__ResetImages_CMFCToolBar__UEAAXXZ(pBar);
                impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, hMenuShared, pBar);
            }
        }
    }

    const HMENU hDefault = At<HMENU>(pThis, kOffHDefaultMenu);
    if (impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, hDefault, pBar)) {
        impl__ResetImages_CMFCToolBar__UEAAXXZ(pBar);
        impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, hDefault, pBar);
    }

    const HMENU hMenu = At<HMENU>(pThis, kOffHMenu);
    if (hMenu != nullptr && impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(pHash, hMenu, pBar)) {
        if (CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis))) {
            impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrame, TRUE);
        }
        ::InvalidateRect(HWndOf(pThis), nullptr, TRUE);
        ::UpdateWindow(HWndOf(pThis));
    }
}

// Retail (RVA 0x8baa0, mfc140u), fully decoded, NOT implemented:
//     if (!m_bMaximizeMode) {                                          // +0x1394
//         if (m_hSysMenu /*+0x1370*/ && m_pParentWnd /*+0x1358*/ IsKindOf(CMDIFrameWnd) (0x33c180)
//             && MDIGetActive(NULL) != NULL) {
//             DWORD dwStyle = ::GetWindowLong(pActiveChild->m_hWnd, GWL_STYLE);
//             InsertButton(CMFCToolBarSystemMenuButton(m_hSysMenu, m_hSysIcon), 0);   // vslot 0x698, ctor 0x17f730
//             if (dwStyle & WS_MINIMIZEBOX) InsertButton(CMFCToolBarMenuButtonsButton(SC_MINIMIZE), -1);   // ctor 0x176380
//             if (dwStyle & WS_MAXIMIZEBOX) InsertButton(CMFCToolBarMenuButtonsButton(SC_RESTORE), -1);
//             CMFCToolBarMenuButtonsButton btnClose(SC_CLOSE);
//             MENUITEMINFO mii = { sizeof, MIIM_STATE };
//             if (!::GetMenuItemInfo(m_hSysMenu, SC_CLOSE, FALSE, &mii) || (mii.fState & MFS_GRAYED))
//                 btnClose.m_nStyle |= TBBS_DISABLED;                  // bit 18
//             InsertButton(btnClose, -1);
//         }
//         m_bMaximizeMode = TRUE;
//         m_nSystemButtonsNum = m_nSystemButtonsNumSaved;              // +0x1388 <- +0x138c
//         if (bRecalcLayout) { GetParentFrame()->RecalcLayout(TRUE); ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd); }   // vslot 0x300
//     }
// Left a stub: CMFCToolBarSystemMenuButton is not declared in OpenMFC's
// headers and its exported ctor (featurepack/toolbar/CMFCToolBarSystemMenuButton.cpp)
// is an empty stub, and OpenMFC's CMFCToolBarMenuButtonsButton has a layout
// that contradicts retail (see kOffMbbSystemCommand), so the four buttons
// cannot be built; inserting nothing but flipping m_bMaximizeMode would leave
// GetSystemMenu / GetSystemButton reading ordinary menu buttons as system ones.
// Symbol: ?RestoreMaximizeMode@CMFCMenuBar@@IEAAXH@Z
extern "C" void MS_ABI impl__RestoreMaximizeMode_CMFCMenuBar__IEAAXH_Z(CMFCMenuBar* pThis, int bRecalcLayout) {
    (void)pThis; (void)bRecalcLayout;
}

// Retail (RVA 0x8ca30..0x8cf02, mfc140u, ~290 instructions), decoded, NOT
// implemented:
//     if (m_hMenu) afxMenuHash.SaveMenuBar(m_hMenu, this);             // +0x1360, 0x8eb40
//     CMFCToolBarButton* pCustomize = NULL;
//     if (m_pCustomizeBtn /*+0x1300*/) { pCustomize = m_pCustomizeBtn->GetRuntimeClass()->CreateObject();
//                                        if (!IsKindOf(CMFCCustomizeButton) (0x3b1148)) pCustomize = NULL;
//                                        pCustomize->CopyFrom(*m_pCustomizeBtn); }   // vslot 0x38
//     CFrameWndEx-derived parent test (0x2ee478 = CMDIFrameWndEx) -> pFrameEx;
//     CMenuTearOffManager (0x3be260) ->Reset(NULL) (0x8fd20) if present;
//     BOOL bMax = m_bMaximizeMode;  SetMaximizeMode(FALSE, NULL, TRUE);   // 0x8b6f0
//     for each CMultiDocTemplate (CDocManager vslots 0x30 / 0x38) with m_hMenuShared (+0xf0):
//         reload it with ::LoadMenuW(AfxFindResourceHandle(m_nIDResource /*+0x98*/, RT_MENU)),
//         CreateFromMenu(hMenu, FALSE, FALSE) (vslot 0x878), afxMenuHash.SaveMenuBar,
//         UpdateMDIChildrenMenus(pTemplate) (0x8dfc0), afxMenuHash.RemoveMenu (0x8ef70) + ::DestroyMenu of the old one;
//     if (m_uiDefMenuResId /*+0x1380*/) { m_hDefaultMenu = ::LoadMenuW(...); OnDefaultMenuLoaded (vslot 0x888);
//         CreateFromMenu(m_hDefaultMenu, TRUE, FALSE); afxMenuHash.SaveMenuBar(m_hDefaultMenu, this);
//         pFrameEx->m_hMenuDefault (+0xf0) / +0x5d8 updated; a CFrameWnd parent's +0xf0 too;
//         old default menu removed from the hash and destroyed; }
//     if (afxMenuHash.LoadMenuBar(hMenu, this)) { m_hMenu = hMenu; if (!bMax) { GetParentFrame()->RecalcLayout(TRUE); InvalidateRect; UpdateWindow; } }
//     if (pFrameEx) pFrameEx->OnUpdateFrameMenu(m_hMenu);   // vslot 0x360 of the CMDIFrameWndEx vftable (0x2ee9a8) = ?OnUpdateFrameMenu@CMDIFrameWndEx@@UEAAXPEAUHMENU__@@@Z (0x88d40)
//     if (bMax) RestoreMaximizeMode(TRUE);
//     if (m_pMenuPage /*+0x13b0*/) m_pMenuPage->... (0x17e880) ;
//     if (pCustomize) { InsertButton(pCustomize, -1) (vslot 0x690); m_pCustomizeBtn = pCustomize; AdjustLayout (0x428); AdjustSizeImmediate(TRUE) (0x5b8); }
//     return TRUE;
// Left a stub: it rebuilds the bar from the document templates' shared menus
// and the CFrameWndEx default-menu members (+0xf0 / +0x5d8), none of which
// OpenMFC's CDocTemplate / CFrameWndEx model, through CMenuHash bookkeeping
// that here stores no buttons, and it reads m_pMenuPage (+0x13b0) outside
// OpenMFC's object.  A partial body would destroy the caller's menus without
// rebuilding the bar.
// Symbol: ?RestoreOriginalState@CMFCMenuBar@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreOriginalState_CMFCMenuBar__UEAAHXZ(CMFCMenuBar* pThis) {
    (void)pThis;
    return FALSE;
}

// Retail (RVA 0x8c600..0x8c85e, mfc140u), decoded, NOT implemented:
//     CString strProfile = AFXGetRegPath(..., lpszProfileName);        // 0xd2070
//     afxMenuHash.SaveMenuBar(m_hMenu, this);                          // 0x8eb40
//     SetMaximizeMode(FALSE, NULL, TRUE);                              // 0x8b6f0
//     for each CMultiDocTemplate with m_hMenuShared (+0xf0):
//         if (m_bAutoDocMenus && afxMenuHash.LoadMenuBar(hMenuShared, this))   // +0x1350, 0x8ee80
//             CMFCToolBar::SaveState(strProfile, nIndex, uiID);        // 0x153510
//     if (afxMenuHash.LoadMenuBar(m_hDefaultMenu, this)) CMFCToolBar::SaveState(...);
//     afxMenuHash.LoadMenuBar(m_hMenu, this);  RestoreMaximizeMode(TRUE) when it was on;
//     GetParentFrame()->RecalcLayout(TRUE); ::InvalidateRect; ::UpdateWindow; return TRUE.
// Left a stub for the same reasons as LoadState (document-template shared
// menus and CMenuHash button lists are not modelled here).
// Symbol: ?SaveState@CMFCMenuBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCMenuBar__UEAAHPEB_WHI_Z(CMFCMenuBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return FALSE;
}

// Retail (RVA 0x8cf10, mfc140u), fully transcribed: `m_uiDefMenuResId = uiResId;` (+0x1380).
// Symbol: ?SetDefaultMenuResId@CMFCMenuBar@@QEAAXI@Z
extern "C" void MS_ABI impl__SetDefaultMenuResId_CMFCMenuBar__QEAAXI_Z(CMFCMenuBar* pThis, unsigned int uiResId) {
    if (!pThis) return;
    At<unsigned int>(pThis, kOffDefMenuResId) = uiResId;
}

// Retail (RVA 0x8b6f0..0x8ba90, mfc140u), fully decoded, NOT implemented:
//     if (m_bMaximizeMode == bMax) return;                             // +0x1394
//     if (bMax) {
//         if (pWnd == NULL) { bMax = FALSE; goto done; }
//         BOOL bIsMDIChild = pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) (0x33aef0) && pWnd->[+0x120] != NULL;
//         m_hSysMenu = NULL;                                           // +0x1370
//         CMenu* pSysMenu = CMenu::FromHandle(::GetSystemMenu(pWnd->m_hWnd, FALSE));
//         if (pSysMenu && ::IsMenu(pSysMenu->m_hMenu)) {
//             m_hSysMenu = pSysMenu->m_hMenu;
//             if (!::IsMenu(m_hSysMenu) || !((pWnd->GetStyle() & WS_SYSMENU) || bIsMDIChild)) m_hSysMenu = NULL;
//         }
//         if (m_hSysMenu) {
//             m_hSysIcon = (HICON)::SendMessage(pWnd->m_hWnd, WM_GETICON, ICON_SMALL, 0);   // +0x1378
//             if (!m_hSysIcon) m_hSysIcon = (HICON)::GetClassLongPtr(pWnd->m_hWnd, GCLP_HICONSM /*-34*/);
//             InsertButton(CMFCToolBarSystemMenuButton(m_hSysMenu, m_hSysIcon), 0);   // vslot 0x698, ctor 0x17f730
//         }
//         DWORD dwStyle = ::GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
//         m_nSystemButtonsNum = 0;                                     // +0x1388
//         if (m_hSysMenu) {
//             if (dwStyle & WS_MINIMIZEBOX) { InsertButton(CMFCToolBarMenuButtonsButton(SC_MINIMIZE), -1); m_nSystemButtonsNum++; }
//             if (dwStyle & WS_MAXIMIZEBOX) { InsertButton(CMFCToolBarMenuButtonsButton(SC_RESTORE), -1);  m_nSystemButtonsNum++; }
//             CMFCToolBarMenuButtonsButton btnClose(SC_CLOSE);
//             MENUITEMINFO mii = { sizeof, MIIM_STATE };
//             if (!::GetMenuItemInfo(m_hSysMenu, SC_CLOSE, FALSE, &mii) || (mii.fState & MFS_GRAYED)) btnClose.m_nStyle |= TBBS_DISABLED;
//             InsertButton(btnClose, -1); m_nSystemButtonsNum++;
//         }
//     } else {
//         m_nSystemButtonsNumSaved = m_nSystemButtonsNum;              // +0x138c
//         if (m_hSysMenu) RemoveButton(0);                             // vslot 0x6a8
//         int nLast = m_Buttons.GetCount() - (m_pCustomizeBtn ? 1 : 0);   // +0x11a0, +0x1300
//         for (int i = 0; i < m_nSystemButtonsNum; i++) RemoveButton(nLast - 1 - i);
//         m_nSystemButtonsNum = 0;
//     }
//   done:
//     m_bMaximizeMode = bMax;
//     if (bRecalcLayout) AdjustLayout();                               // vslot 0x428
//     if (!m_bExclusiveRow && bRecalcLayout) AdjustSizeImmediate(TRUE);   // +0x218, vslot 0x5b8
// Left a stub for the reasons given at RestoreMaximizeMode (no constructible
// CMFCToolBarSystemMenuButton, wrong-layout CMFCToolBarMenuButtonsButton).
// Symbol: ?SetMaximizeMode@CMFCMenuBar@@QEAAXHPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__SetMaximizeMode_CMFCMenuBar__QEAAXHPEAVCWnd__H_Z(CMFCMenuBar* pThis, int bMax, CWnd* pWnd, int bRecalcLayout) {
    (void)pThis; (void)bMax; (void)pWnd; (void)bRecalcLayout;
}

// Retail (RVA 0x8e1c0, mfc140u), fully decoded:
//     m_pMenuButtonRTC = pMenuButtonRTC ? pMenuButtonRTC : RUNTIME_CLASS(CMFCToolBarMenuButton);   // +0x13b8, 0x3b1748
// Left a stub: +0x13b8 lies outside OpenMFC's 0x13b0-byte CMFCMenuBar (see
// the layout note at the top of the file), and the objects OpenMFC itself
// allocates (CreateObject: `new CMFCMenuBar`) are that size, so the store
// would overrun them.  The body is written so that it activates by itself
// once the header grows the padding to 112 bytes.
// Symbol: ?SetMenuButtonRTC@CMFCMenuBar@@QEAAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SetMenuButtonRTC_CMFCMenuBar__QEAAXPEAUCRuntimeClass___Z(CMFCMenuBar* pThis, CRuntimeClass* pMenuButtonRTC) {
    if (!pThis) return;
    if constexpr (sizeof(CMFCMenuBar) >= kOffMenuButtonRTC + static_cast<int>(sizeof(CRuntimeClass*))) {
        At<CRuntimeClass*>(pThis, kOffMenuButtonRTC) =
            pMenuButtonRTC != nullptr ? pMenuButtonRTC : impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
    } else {
        (void)pMenuButtonRTC;
    }
}

// Retail (RVA 0x8d760, mfc140u), fully transcribed (static):
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }
//     if (!afxGlobalData.SetMenuFont(lpLogFont, bHorz)) return FALSE;  // 0x6b6b0
//     for (node = m_lstAllToolbars.head /*0x3b2098*/; node; node = node->next) {
//         CMFCToolBar* pBar = node->data;  if (!pBar) AfxThrowInvalidArgException();
//         if (CWnd::FromHandlePermanent(pBar->m_hWnd)) pBar->AdjustLayout();   // 0x28adc0, vslot 0x428
//     }
//     return TRUE;
// Deviations: OpenMFC has no m_lstAllToolbars; the walk runs over the list
// the exported ?GetAllToolbars@CMFCToolBar@@ returns (CMFCToolBar.cpp rebuilds
// it from the mfccore toolbar side table on every call), read through the
// exported FindIndex thunk and the CList<CObject*,CObject*>::CNode shape of
// include/openmfc/afx.h, exactly as featurepack/visualmanager/CMFCVisualManager.cpp
// walks it; the bars are snapshotted first because AdjustLayout may rebuild
// that list.  AdjustLayout goes to the exported CMFCToolBar body (a derived
// override is not reached).  AFX_GLOBAL_DATA::SetMenuFont is still a stub
// returning 0 in core/runtime/AFX_GLOBAL_DATA.cpp, so today this returns FALSE
// before the walk, exactly as retail does when the font cannot be set.
// Symbol: ?SetMenuFont@CMFCMenuBar@@SAHPEAUtagLOGFONTW@@H@Z
extern "C" int MS_ABI impl__SetMenuFont_CMFCMenuBar__SAHPEAUtagLOGFONTW__H_Z(LOGFONTW* lpLogFont, int bHorz) {
    EnsureGlobalDataInitialized();
    if (!impl__SetMenuFont_AFX_GLOBAL_DATA__QEAAHPEAUtagLOGFONTW__H_Z(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, lpLogFont, bHorz)) {
        return FALSE;
    }
    const CObList* pList = impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();
    if (pList == nullptr) return TRUE;
    std::size_t n = 0;
    for (const MenuBarObNode* p = ObNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0)); p != nullptr; p = p->pNext) ++n;
    if (n == 0) return TRUE;
    CMFCToolBar** bars = static_cast<CMFCToolBar**>(std::malloc(n * sizeof(CMFCToolBar*)));
    if (bars == nullptr) return TRUE;
    std::size_t count = 0;
    for (const MenuBarObNode* p = ObNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0)); p != nullptr && count < n; p = p->pNext) {
        if (p->data == nullptr) {   // retail: AfxThrowInvalidArgException()
            impl__AfxThrowInvalidArgException__YAXXZ();
            std::free(bars);
            return TRUE;
        }
        bars[count++] = static_cast<CMFCToolBar*>(p->data);
    }
    for (std::size_t i = 0; i < count; ++i) {
        if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWndOf(bars[i])) != nullptr) {
            impl__AdjustLayout_CMFCToolBar__UEAAXXZ(bars[i]);
        }
    }
    std::free(bars);
    return TRUE;
}

// Retail (RVA 0x8d630, mfc140u), fully transcribed: `m_bRecentlyUsedMenus = bOn;` (0x3b1b08).
// Symbol: ?SetRecentlyUsedMenus@CMFCMenuBar@@SAXH@Z
extern "C" void MS_ABI impl__SetRecentlyUsedMenus_CMFCMenuBar__SAXH_Z(int bOn) {
    impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA = bOn;
}

// Retail (RVA 0x8d620, mfc140u), fully transcribed: `m_bShowAllCommands = bShowAllCommands;` (0x3be254).
// Symbol: ?SetShowAllCommands@CMFCMenuBar@@SAXH@Z
extern "C" void MS_ABI impl__SetShowAllCommands_CMFCMenuBar__SAXH_Z(int bShowAllCommands) {
    impl__m_bShowAllCommands_CMFCMenuBar__1HA = bShowAllCommands;
}

// Retail (RVA 0x8dfc0, mfc140u), fully decoded, NOT implemented (static):
//     for (POSITION pos = pTemplate->GetFirstDocPosition(); pos; ) {   // vslot 0xb8
//         CDocument* pDoc = pTemplate->GetNextDoc(pos);                // vslot 0xc0
//         for (POSITION posView = pDoc->GetFirstViewPosition(); posView; ) {   // vslot 0xe0
//             CView* pView = pDoc->GetNextView(posView);               // vslot 0xe8
//             CFrameWnd* pFrame = pView->GetParentFrame();             // 0x28e200
//             if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {   // 0x33c150
//                 ((CMDIChildWnd*)pFrame)->m_hMenuShared = pTemplate->m_hMenuShared;   // +0x1d8 <- +0xf0
//                 pFrame->m_hAccelTable = pTemplate->m_hAccelTable;    // +0xf8 <- +0xf8
//             }
//         }
//     }
// (Every virtual call goes through the CFG dispatcher at 0x2c7b30; the
// CMDIChildWnd descriptor at 0x33c150 says m_nObjectSize 0x1e8; the only
// IsKindOf in the body is the one on the frame -- the template is trusted as
// a CMultiDocTemplate from its parameter type, and +0xf0 / +0xf8 are its own
// members, see kOffMultiDocTemplateMenuShared.)
// Left a stub: the template side of the copy is unusable in OpenMFC -- +0xf0 /
// +0xf8 lie in CMultiDocTemplate::_multidoctemplate_padding, which no OpenMFC
// body writes and no ctor zeroes (LoadTemplate forwards to
// CDocTemplate::LoadTemplate, which loads only the doc strings), so both reads
// yield indeterminate memory -- and the frame side would be harmful:
// OpenMFC's CFrameWnd::m_hAccelTable is a live member (LoadAccelTable writes
// it, PreTranslateMessage translates through it; it sits at +0xf0 here, not
// retail's +0xf8 as CFrameWnd::LoadAccelTable (RVA 0x29ce10) stores it), so
// the +0xf8 store would clobber every MDI child's accelerator table with that
// indeterminate value.  CMDIChildWnd::m_hMenuShared (+0x1d8) would land in
// _mdichild_padding (which starts at 0x1d4 here because mingw folds it into
// CFrameWnd's tail padding; sizeof(CFrameWnd) == 0x1d8, sizeof(CMDIChildWnd)
// == 0x1e8), which nothing reads.  Checked 2026-09-17 against afxwin.h and
// core/doc/CMultiDocTemplate.cpp / core/frame/CFrameWnd.cpp.
// Symbol: ?UpdateMDIChildrenMenus@CMFCMenuBar@@KAXPEAVCMultiDocTemplate@@@Z
extern "C" void MS_ABI impl__UpdateMDIChildrenMenus_CMFCMenuBar__KAXPEAVCMultiDocTemplate___Z(CMultiDocTemplate* pTemplate) {
    (void)pTemplate;
}
