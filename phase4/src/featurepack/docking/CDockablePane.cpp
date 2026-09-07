// CDockablePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int);
extern "C" int MS_ABI impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(CPane* pThis, CRect& rect, int);
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int);
extern "C" void MS_ABI impl__CopyState_CPane__UEAAXPEAV1__Z(CPane* pThis, CPane* pOrgBar);

// ---------------------------------------------------------------------------
// Retail CDockablePane instance layout (MSVC), derived from the shipping
// atlmfc header afxdockablepane.h and CONFIRMED against the retail
// mfc140.dll disassembly at the offsets marked (*) below. The CPane
// sub-object occupies 0..0x3F7 (sizeof(CPane) == 1016 == 0x3F8) and
// sizeof(CDockablePane) == 0x4E0 == 1248.
//
//   +0x3F8  CRect    m_rectCloseButton
//   +0x408  bool     m_bPrepareToFloat        (*) EnterDragMode 0x41e20 stores
//                                                 1; OnCancelMode 0x450d0 and
//                                                 OnContextMenu 0x46200 test it
//   +0x409  bool     m_bReadyToFloat
//   +0x40C  int      m_cyGripper              (*) EnableGripper 0x40160 writes,
//                                                 GetCaptionHeight 0x46520 reads
//   +0x410  int      m_nLastPercent           (*) CopyState 0x46410 copies
//   +0x414  int      m_nBorderSize
//   +0x418  BOOL     m_bHasGripper            (*) EnableGripper 0x40160 tests
//   +0x41C  BOOL     m_bActive
//   +0x420  BOOL     m_bEnableAutoHideAll     (*) IsAutohideAllEnabled thunk at
//                                                 RVA 0x12e10 is `mov 0x420(%rcx),%eax`
//   +0x424  DWORD    m_dwSCBStyle
//   +0x428  CRect    m_rectRedraw
//   +0x438  BOOL     m_bPinState              (*) IsAutoHideMode thunk at RVA
//                                                 0x12e00 is `mov 0x438(%rcx),%eax`
//   +0x440  UINT_PTR m_nAutoHideConditionTimerID
//   +0x448  UINT_PTR m_nSlideTimer
//   +0x450  int      m_nSlideStep
//   +0x454  int      m_nSlideDelta
//   +0x458  CRect    m_rectRestored           (*) CopyState 0x46410 copies 16 bytes
//   +0x468  BOOL     m_bIsSliding
//   +0x46C  BOOL     m_bIsResizing
//   +0x470  BOOL     m_bIsHiding
//   +0x474  UINT     m_ahSlideMode            (*) GetAHSlideMode thunk at RVA
//                                                 0x12e90 is `mov 0x474(%rcx),%eax`
//   +0x478  CMFCAutoHideButton* m_pAutoHideButton
//   +0x480  CMFCAutoHideBar*    m_pAutoHideBar (*) IsVisible 0x45330 reads it
//   +0x488  CObArray m_arrButtons             (*) FindButtonByHit 0x45210 reads
//                                                 m_pData at 0x490 and m_nSize at 0x498
//   +0x4B0  CRuntimeClass* m_pTabbedControlBarRTC
//   +0x4B8  CToolTipCtrl*  m_pToolTip
//   +0x4C0  HWND     m_hDefaultSlider         (*) GetDefaultPaneDivider 0x45cd0 reads
//   +0x4C8  HWND     m_hRestoredDefaultSlider
//   +0x4D0  UINT     m_nHot                   (*) OnLButtonDown 0x41150 writes
//   +0x4D4  UINT     m_nHit                   (*) OnLButtonDown 0x41150 writes
//   +0x4D8  BOOL     m_bCaptionButtonsCaptured (*) OnContextMenu 0x46200 tests
//
// OpenMFC's CDockablePane (include/openmfc/afxmfc.h) declares none of these by
// name: it carries a 128-byte `_dockablepane_padding` blob and sizeof() ==
// 1144. Measured with the phase4 mingw flags that blob starts at offset 1012
// (0x3F4) -- the Itanium ABI folds it into CPane's four bytes of tail padding --
// so the retail fields from +0x3F8 to +0x473 DO have storage inside the object
// and a few bodies below use them at their retail offsets through the
// DockablePaneField<> helper. Everything from +0x474 up does not: +0x474 is
// where CBaseTabbedPane's own padding blob begins (measured: offset 1140,
// sizeof(CBaseTabbedPane) == 1240) and +0x478 is one past the end of the
// object. So m_ahSlideMode, m_pAutoHideButton, m_pAutoHideBar, m_arrButtons,
// m_pTabbedControlBarRTC, m_pToolTip, m_hDefaultSlider, m_nHot, m_nHit and
// m_bCaptionButtonsCaptured are unreachable, which is what still blocks most of
// this file -- see `headerRequests`.
// ---------------------------------------------------------------------------

// Sibling thunks used below. BRIEFING S1: the C++ methods do not exist inside
// this DLL, only these exports. Several of these thunks are still generated
// stubs whose parameter list omits the implicit `this` (e.g.
// impl__OnCancelMode_CPane__IEAAXXZ is declared `()` at
// phase4/src/featurepack/docking/CPane.cpp:214 and has an empty body); the
// retail exports do take `this` in %rcx, and because those bodies read none of
// their arguments the extra register argument is harmless under the MS x64 ABI.
// The declarations here use the retail-correct parameter lists.
//
// One shared, deliberate deviation: the implemented bodies below start with a
// NULL check on `this` (and, in CanAcceptMiniFrame, on the mini-frame argument).
// Retail has no such check -- it would fault -- but the rest of this file
// already guards that way and a generated stub can hand these thunks anything.
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
    CBasePane* pBar, int bAdd);
extern "C" int MS_ABI impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(
    const void* pMiniFrame, const CDockablePane* pPane);
extern "C" void MS_ABI impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(CPane* pThis, CWnd* pWndOldParent);
extern "C" void MS_ABI impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(CPane* pThis, CWnd* pWndNewParent, int bDelay);
extern "C" void MS_ABI impl__OnCancelMode_CPane__IEAAXXZ(CPane* pThis);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);
// CPaintDC, defined at phase4/src/core/gdi/CPaintDC.cpp:12 and :30.
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);
// CDockingManager::m_bRestoringDockState, defined at
// phase4/src/featurepack/docking/StaticData.cpp:50.
extern "C" int impl__m_bRestoringDockState_CDockingManager__2HA;
// Sibling thunks defined further down in THIS file.
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis);
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis);

// --- Added for the batch of bodies transcribed below -------------------------
// Thunks in other translation units. Several of these are still generated stubs
// whose parameter list omits the implicit `this`; as noted above the retail
// exports do take it in %rcx and those bodies read no argument, so declaring the
// retail-correct list here is safe under the MS x64 ABI.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(RECT* lpRectExclude);
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* pRect);
extern "C" int MS_ABI impl__IsMDITabbed_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane* pThis, int bChangeHotPoint);
extern "C" void MS_ABI impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
    CPane* pThis, CWnd* pWnd, void* point);
extern "C" int MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
// CMFCPopupMenu::m_pActivePopupMenu, defined at
// phase4/src/core/runtime/StaticData.cpp:40.
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
// AFX_WM_ON_PRESS_CLOSE_BUTTON, a RegisterWindowMessage id defined at
// phase4/src/core/runtime/Globals.cpp:149.
extern "C" unsigned int impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA;
// More sibling thunks defined further down in THIS file.
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    CDockablePane* pThis, void* point, int bDetectCaption);
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ(const CDockablePane* pThis);
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(
    CDockablePane* pThis, void* point);

namespace {
// --- CDockablePane instance fields reached through the padding blob ----------
// OpenMFC's CDockablePane declares no members; it carries
// `char _dockablepane_padding[128]` (include/openmfc/afxmfc.h:476). Measured
// with the phase4 mingw flags (see checkfile.sh) that blob starts at offset
// 1012 == 0x3F4 -- the Itanium ABI folds it into CPane's four bytes of tail
// padding -- and sizeof(CDockablePane) is 1144. So every retail field from
// +0x3F8 (the first byte past the 1016-byte CPane sub-object) up to +0x473 has
// real storage inside the object and can be used at its retail offset.
//
// +0x474 and above cannot: sizeof(CBaseTabbedPane) measures 1240 with its own
// `_basetabbedpane_padding[96]` at offset 1140 == 0x474, so a write to +0x474
// on a CTabbedPane lands in the derived class's blob, and +0x478 is one past
// the end of the object. That is why m_ahSlideMode (+0x474), m_pAutoHideBar
// (+0x480), m_arrButtons (+0x488) and everything above them stay unreachable.
static_assert(sizeof(CPane) == 1016, "CPane layout changed; recheck the offsets below");
static_assert(sizeof(CDockablePane) == 1144, "CDockablePane size changed; recheck the offsets below");

const size_t kOffPrepareToFloat = 0x408;   // bool   m_bPrepareToFloat
const size_t kOffLastPercent    = 0x410;   // int    m_nLastPercent
const size_t kOffEnableAutoHideAll = 0x420; // BOOL  m_bEnableAutoHideAll
const size_t kOffSlideDelta     = 0x454;   // int    m_nSlideDelta
const size_t kOffRectRestored   = 0x458;   // CRect  m_rectRestored
const size_t kOffFirstUnusable  = 0x474;   // first retail offset with no storage here

static_assert(kOffPrepareToFloat >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffLastPercent >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffEnableAutoHideAll >= sizeof(CPane), "field would alias the CPane sub-object");
static_assert(kOffRectRestored + 16 <= kOffFirstUnusable, "field runs past the usable window");
static_assert(kOffFirstUnusable + 4 <= sizeof(CDockablePane), "usable window computed wrongly");

template <typename T>
inline T& DockablePaneField(CDockablePane* pThis, size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(pThis) + off);
}
template <typename T>
inline const T& DockablePaneField(const CDockablePane* pThis, size_t off) {
    return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(pThis) + off);
}

// CBRS_ALIGN_* from atlmfc/include/afxwin.h; the retail switches below compare
// GetCurrentAlignment() against exactly these four values.
const unsigned long kAlignLeft   = 0x1000UL;
const unsigned long kAlignTop    = 0x2000UL;
const unsigned long kAlignRight  = 0x4000UL;
const unsigned long kAlignBottom = 0x8000UL;
}  // namespace

namespace {
// AFX_DOCK_TYPE::DT_IMMEDIATE, from atlmfc/include/afxglobals.h.
const int kDockTypeImmediate = 1;
}  // namespace


// Symbol: ?CreateObject@CDockablePane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockablePane__SAPEAVCObject__XZ() {
    return new CDockablePane();
}
// Symbol: ?GetRuntimeClass@CDockablePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockablePane__UEBAPEAUCRuntimeClass__XZ(const CDockablePane*) {
    return CDockablePane::GetThisClass();
}
// Symbol: ?GetThisClass@CDockablePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ() {
    return CDockablePane::GetThisClass();
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, 240, 140);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        state.canAutoHide = TRUE;
        state.hasGripper = bHasGripper ? TRUE : FALSE;
        state.tabbed = (dwTabbedStyle != 0) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 120;
        if (state.minSize.cy <= 0) state.minSize.cy = 80;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), L"STATIC", dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@VCSize@@HIKKK@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, CSize sizeDefault,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle, unsigned long dwControlBarStyle) {
    CRect rect(0, 0, sizeDefault.cx > 0 ? sizeDefault.cx : 240, sizeDefault.cy > 0 ? sizeDefault.cy : 140);
    int created = impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, *static_cast<const RECT*>(rect), bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, nullptr);
    if (created && pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        g_paneCoreState_Cbarcore[pThis].minSize = sizeDefault;
    }
    return created;
}
// Symbol: ?CreateEx@CDockablePane@@UEAAHKPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockablePane__UEAAHKPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, unsigned long, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long dwControlBarStyle, CCreateContext* pContext) {
    return impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);
}
// Symbol: ?Dock@CDockablePane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CDockablePane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), pTargetBar, lpRect, dockMethod);
}
// Symbol: ?OnBeforeFloat@CDockablePane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CDockablePane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CRect& rect, int dockMethod) {
    return impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), rect, dockMethod);
}
// Symbol: ?DockToFrameWindow@CDockablePane@@UEAAHKPEBUtagRECT@@KPEAVCBasePane@@HH@Z
extern "C" int MS_ABI impl__DockToFrameWindow_CDockablePane__UEAAHKPEBUtagRECT__KPEAVCBasePane__HH_Z(
    CDockablePane* pThis, unsigned long, const RECT* lpRect, unsigned long, CBasePane* pTargetBar, int, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
    state.parent = pTargetBar != nullptr ? static_cast<CWnd*>(pTargetBar) : state.parent;
    state.recentRect = NormalizeRect(lpRect, 240, 140);
    state.tabbed = FALSE;
    return TRUE;
}
// Symbol: ?ShowPane@CDockablePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate) {
    impl__ShowPane_CBasePane__UEAAXHHH_Z(reinterpret_cast<CBasePane*>(pThis), bShow, bDelay, bActivate);
}
CDockablePane::CDockablePane() {
    memset(_dockablepane_padding, 0, sizeof(_dockablepane_padding));
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = TRUE;
    state.canAutoHide = TRUE;
    state.hasGripper = TRUE;
}
CDockablePane::~CDockablePane() {}
BOOL CDockablePane::CanBeAttached() const { return TRUE; }
BOOL CDockablePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canAutoHide : TRUE;
}
void CDockablePane::EnableAutohideAll() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].canAutoHide = TRUE;
}
CMFCAutoHideBar* CDockablePane::SetAutoHideMode(int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar, int) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.autoHideMode = bAutoHideMode ? TRUE : FALSE;
    state.autoHideAlignment = bAutoHideMode ? dwAlignment : 0;
    state.autoHideBar = bAutoHideMode ? pAutoHideBar : nullptr;
    state.canAutoHide = TRUE;
    return reinterpret_cast<CMFCAutoHideBar*>(state.autoHideBar);
}
BOOL CDockablePane::IsAutoHideMode() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.autoHideMode : FALSE;
}
BOOL CDockablePane::IsTabbed() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.tabbed : FALSE;
}
// CDockablePane::m_sizeDragSensitivity -- the static CSize data export. Retail
// has no static initializer for it in .data; it is filled in by the dynamic
// initializer at RVA 0x12a0, whose whole body is
//     ecx = 0x45 ; call *GetSystemMetrics ; -> ebx
//     ecx = 0x44 ; call *GetSystemMetrics ; -> eax
//     [0x3ba2f8] = eax    ; m_sizeDragSensitivity.cx
//     [0x3ba2fc] = ebx    ; m_sizeDragSensitivity.cy
// (0x1802c4c50 resolves to USER32!GetSystemMetrics; 0x44 == SM_CXDRAG and
// 0x45 == SM_CYDRAG). Reproduced here with an equivalent dynamic initializer.
// Symbol: ?m_sizeDragSensitivity@CDockablePane@@1VCSize@@A
extern "C" SIZE impl__m_sizeDragSensitivity_CDockablePane__1VCSize__A = {
    ::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG)
};

// STUB. Retail entry RVA 0x44880. It asks the CPaneDivider argument for a rect
// (CPaneDivider::GetRootContainerRect, RVA 0xad530), bails if ::IsRectEmpty
// says it is empty, maps it through CWnd::GetParentFrame (RVA 0x28c680) and
// CWnd::ScreenToClient (RVA 0x2a11f0), then repositions itself. Blocked:
// OpenMFC models no CPaneDivider, so the source rect cannot be obtained.
// Symbol: ?AdjustPaneToPaneContainer@CDockablePane@@MEAAXPEAVCPaneDivider@@@Z
extern "C" void MS_ABI impl__AdjustPaneToPaneContainer_CDockablePane__MEAAXPEAVCPaneDivider___Z(void* /*class*/* p0) {}

// STUB. Retail entry RVA 0x43530 (~1088 bytes, the largest body in this class).
// Its body touches the CDockablePane fields at +0x450 (m_nSlideStep), +0x4b0
// (m_pTabbedControlBarRTC) and +0x4c0 (m_hDefaultSlider). Blocked: +0x4b0 and
// +0x4c0 are past the end of the 1144-byte object (+0x450 would be reachable,
// but on its own it is not enough to drive this body), and the function also
// needs the CTabbedPane / CPaneDivider machinery OpenMFC does not model.
// Symbol: ?AttachToTabWnd@CDockablePane@@UEAAPEAV1@PEAV1@W4AFX_DOCK_METHOD@@HPEAPEAV1@@Z
extern "C" void* MS_ABI impl__AttachToTabWnd_CDockablePane__UEAAPEAV1_PEAV1_W4AFX_DOCK_METHOD__HPEAPEAV1__Z(int p0, void* /*class*/** p1) {
    return nullptr;
}

// STUB. Retail entry RVA 0x42c10. Its body touches the CDockablePane field
// at +0x480 (m_pAutoHideBar). Blocked: that offset is past the end of OpenMFC's
// 1144-byte CDockablePane.
// Symbol: ?CalcCorrectOffset@CDockablePane@@IEAA?AVCPoint@@VCRect@@H@Z
extern "C" void* MS_ABI impl__CalcCorrectOffset_CDockablePane__IEAA_AVCPoint__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return nullptr;
}

// CDockablePane::CalcFixedLayout -- transcribed from retail. This export is
// absent from mfc140_rva_symbols.json only because that map is keyed by RVA and
// the linker ICF-folded this body onto another name; it is NOT missing from the
// DLL. In mfc140u.dll the export resolves to RVA 0x92b0 (shared with
// ?CalcFixedLayout@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z, whose independent
// transcription in phase4/src/featurepack/docking/CMFCAutoHideBar.cpp:222 reads
// the same way -- though note that sibling takes its two leading pointers in the
// opposite order, which the disassembly below does not support), and the whole
// body is:
//     mov 0x40(%rcx),%rcx        ; this->m_hWnd
//     xorps %xmm0,%xmm0 ; movdqu %xmm0,0x20(%rsp)   ; RECT rc = {0,0,0,0}
//     lea 0x20(%rsp),%rdx ; call *0x1802c6c08       ; ::GetWindowRect(m_hWnd,&rc)
//     mov 0x28(%rsp),%ecx ; sub 0x20(%rsp),%ecx ; mov %ecx,(%rbx)   ; cx = right-left
//     mov 0x2c(%rsp),%ecx ; sub 0x24(%rsp),%ecx ; mov %ecx,0x4(%rbx); cy = bottom-top
//     mov %rbx,%rax                                 ; return the hidden slot
// (IAT slot 0x1802c6c08 resolved with iatu.py == USER32!GetWindowRect. The
// result is NOT checked; the zeroed rect is used as-is on failure.) Both BOOL
// arguments are ignored.
// Signature corrected: the generated stub dropped `this` and the hidden CSize
// return slot. MSVC x64 passes `this` in %rcx and the hidden slot in %rdx and
// returns that same pointer in %rax -- confirmed here by `mov 0x40(%rcx),%rcx`
// (an HWND member read, so %rcx is `this`) and `mov %rbx,%rax` where %rbx was
// loaded from %rdx.
// Symbol: ?CalcFixedLayout@CDockablePane@@UEAA?AVCSize@@HH@Z
extern "C" SIZE* MS_ABI impl__CalcFixedLayout_CDockablePane__UEAA_AVCSize__HH_Z(
    CDockablePane* pThis, SIZE* pRet, int bStretch, int bHorz) {
    (void)bStretch;   // retail reads neither argument
    (void)bHorz;
    if (pRet == nullptr) return pRet;
    RECT rc = {0, 0, 0, 0};
    if (pThis != nullptr) {   // deviation: retail has no NULL check
        ::GetWindowRect(pThis->m_hWnd, &rc);
    }
    pRet->cx = rc.right - rc.left;
    pRet->cy = rc.bottom - rc.top;
    return pRet;
}

// CDockablePane::CalcRectForSliding -- transcribed from retail entry RVA
// 0x42a70. It grows or shrinks `rect` along the auto-hide edge by
// m_nSlideDelta (+0x454), clamps the result to the extent recorded in
// m_rectRestored (+0x458..+0x467), then slides `rectResult` so that its own
// (unchanged) width/height sits against the same clamped edge:
//     if (!IsAutoHideMode()) return;                  // vtable slot 0x3c8
//     int d = m_nSlideDelta;
//     switch (GetCurrentAlignment()) {                // vtable slot 0x338
//     case CBRS_ALIGN_LEFT:                           // 0x42bb6
//         if (bGrow) { rect.right += d;
//                      if (rect.Width() > m_rectRestored.Width())
//                          rect.right = rect.left + m_rectRestored.Width(); }
//         else       { rect.right -= d;
//                      if (rect.right < rect.left) rect.right = rect.left; }
//         rectResult.right = rectResult.Width() + rect.right;
//         rectResult.left  = rect.right;              break;
//     case CBRS_ALIGN_TOP:                            // 0x42b66, same on the y axis
//         ... rectResult.bottom = rectResult.Height() + rect.bottom;
//             rectResult.top    = rect.bottom;        break;
//     case CBRS_ALIGN_RIGHT:                          // 0x42b24
//         if (bGrow) { rect.left -= d;
//                      if (rect.Width() > m_rectRestored.Width())
//                          rect.left = rect.right - m_rectRestored.Width(); }
//         else       { rect.left += d;
//                      if (rect.left > rect.right) rect.left = rect.right; }
//         rectResult.right = rect.left;
//         rectResult.left  = rect.left - rectResult.Width();   break;
//     case CBRS_ALIGN_BOTTOM:                         // 0x42ad8, same on the y axis
//         ... rectResult.bottom = rect.top;
//             rectResult.top    = rect.top - rectResult.Height();  break;
//     default: return;
//     }
// (In each arm the width/height of rectResult is read BEFORE the assignment
// that overwrites one of its edges -- e.g. 0x42bf4 `mov 0x8(%rdi),%eax ; sub
// (%rdi),%eax ; add %ecx,%eax ; mov %eax,0x8(%rdi) ; mov %ecx,(%rdi)`.)
// Both fields are inside the padding blob (see the offset note near the top) and
// are read at their retail offsets. As with OffsetRectForSliding above, the
// sibling CBasePane::GetCurrentAlignment thunk is still a stub returning 0, so
// today no case matches and the caller's rects are left alone -- retail's own
// behaviour for an alignment of 0.
// Signature corrected to carry `this`.
// Symbol: ?CalcRectForSliding@CDockablePane@@IEAAXAEAVCRect@@0H@Z
extern "C" void MS_ABI impl__CalcRectForSliding_CDockablePane__IEAAXAEAVCRect__0H_Z(
    CDockablePane* pThis, CRect& rect, CRect& rectResult, int bGrow) {
    if (pThis == nullptr) return;
    if (!pThis->IsAutoHideMode()) return;

    const int nDelta = DockablePaneField<int>(pThis, kOffSlideDelta);
    const RECT& rectRestored = DockablePaneField<RECT>(pThis, kOffRectRestored);

    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis))) {
    case kAlignLeft: {
        if (bGrow) {
            rect.right += nDelta;
            const int nMax = rectRestored.right - rectRestored.left;
            if (rect.right - rect.left > nMax) rect.right = rect.left + nMax;
        } else {
            rect.right -= nDelta;
            if (rect.right < rect.left) rect.right = rect.left;
        }
        const int nWidth = rectResult.right - rectResult.left;
        rectResult.right = rect.right + nWidth;
        rectResult.left = rect.right;
        break;
    }
    case kAlignTop: {
        if (bGrow) {
            rect.bottom += nDelta;
            const int nMax = rectRestored.bottom - rectRestored.top;
            if (rect.bottom - rect.top > nMax) rect.bottom = rect.top + nMax;
        } else {
            rect.bottom -= nDelta;
            if (rect.bottom < rect.top) rect.bottom = rect.top;
        }
        const int nHeight = rectResult.bottom - rectResult.top;
        rectResult.bottom = rect.bottom + nHeight;
        rectResult.top = rect.bottom;
        break;
    }
    case kAlignRight: {
        if (bGrow) {
            rect.left -= nDelta;
            const int nMax = rectRestored.right - rectRestored.left;
            if (rect.right - rect.left > nMax) rect.left = rect.right - nMax;
        } else {
            rect.left += nDelta;
            if (rect.left > rect.right) rect.left = rect.right;
        }
        const int nWidth = rectResult.right - rectResult.left;
        rectResult.right = rect.left;
        rectResult.left = rect.left - nWidth;
        break;
    }
    case kAlignBottom: {
        if (bGrow) {
            rect.top -= nDelta;
            const int nMax = rectRestored.bottom - rectRestored.top;
            if (rect.bottom - rect.top > nMax) rect.top = rect.bottom - nMax;
        } else {
            rect.top += nDelta;
            if (rect.top > rect.bottom) rect.top = rect.bottom;
        }
        const int nHeight = rectResult.bottom - rectResult.top;
        rectResult.bottom = rect.top;
        rectResult.top = rect.top - nHeight;
        break;
    }
    default:
        return;
    }
}

// CDockablePane::CanAcceptMiniFrame -- transcribed from retail entry RVA
// 0x45ed0. The whole body swaps the two arguments and tail-jumps through the
// mini frame's vtable at byte offset 0x2d8:
//     mov (%rdx),%rax ; mov %rdx,%r8 ; mov %rcx,%rdx ; mov %r8,%rcx
//     mov 0x2d8(%rax),%rax ; rex.W jmp *...
// i.e. `return pMiniFrame->{slot 0x2d8}(this);`. Slot 0x2d8 of the
// CPaneFrameWnd vftable (RVA 0x2f5ad8) holds
// ?CanBeDockedToPane@CPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z (RVA 0xb3770),
// so the call is `pMiniFrame->CanBeDockedToPane(this)`.
// Deviation: OpenMFC models no CPaneFrameWnd vtable, so this dispatches
// statically to the CPaneFrameWnd export instead of virtually; a
// CMultiPaneFrameWnd argument would run its override (RVA 0x94be0) in retail.
// Symbol: ?CanAcceptMiniFrame@CDockablePane@@UEBAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__CanAcceptMiniFrame_CDockablePane__UEBAHPEAVCPaneFrameWnd___Z(
    const CDockablePane* pThis, void* pMiniFrame) {
    if (pMiniFrame == nullptr) return FALSE;
    return impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(pMiniFrame, pThis);
}

// STUB. Retail entry RVA 0x423c0. Its body touches CDockablePane fields at +0x41c
// (m_bActive), +0x46c (m_bIsResizing), +0x478 (m_pAutoHideButton), +0x480
// (m_pAutoHideBar), +0x4b8 (m_pToolTip) and +0x4c0 (m_hDefaultSlider).
// Blocked: everything from +0x478 up is past the end of the object. (+0x41c and
// +0x46c are inside the reachable window, but the pointers that gate the body
// are not.)
// Symbol: ?CheckAutoHideCondition@CDockablePane@@MEAAHXZ
extern "C" int MS_ABI impl__CheckAutoHideCondition_CDockablePane__MEAAHXZ() {
    return 0;
}

// STUB. Retail entry RVA 0x42570. Its body touches CDockablePane fields at +0x458..+0x464
// (m_rectRestored), +0x474 (m_ahSlideMode) and +0x480 (m_pAutoHideBar).
// Blocked: m_rectRestored is reachable, but m_ahSlideMode (+0x474) and
// m_pAutoHideBar (+0x480) are not -- +0x474 is where a derived
// CBaseTabbedPane's own padding blob starts here and +0x478 is one past the end
// of the object.
// Symbol: ?CheckStopSlideCondition@CDockablePane@@MEAAHH@Z
extern "C" int MS_ABI impl__CheckStopSlideCondition_CDockablePane__MEAAHH_Z(int p0) {
    return 0;
}

// STUB. Retail entry RVA 0x46590:
//     if (IsAutoHideMode()) return;                       // vtable slot 0x3c8
//     CWnd* pSite = GetDockSiteFrameWnd();                // vtable slot 0x348
//     if (pSite == NULL ||
//         !pSite->IsKindOf(<RTC at RVA 0x2ec3c8, "CMDIFrameWndEx">)) return;
//     if (IsTabbed()) {                                   // vtable slot 0x2e8
//         CBaseTabbedPane* p = GetParentTabbedPane();     // RVA 0xc9b0
//         if (p != NULL && p->IsKindOf(<RTC at RVA 0x3aa4a8, "CTabbedPane">))
//             p->{slot 0x5d0}(bActiveTabOnly);            // ConvertToTabbedDocument
//     } else {
//         pSite->{slot 0x3b8}(this);
//     }
// Blocked: none of those virtuals or the tabbed-pane lookup exist in OpenMFC.
// Symbol: ?ConvertToTabbedDocument@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CDockablePane__UEAAXH_Z(int p0) {}

// CDockablePane::CopyState -- transcribed from retail entry RVA 0x46410, whose
// whole body is:
//     CPane::CopyState(pOrgBar);                          // RVA 0xa3610
//     m_rectRestored  = pOrgBar->m_rectRestored;          // 16 bytes at +0x458
//     m_ahSlideMode   = pOrgBar->GetAHSlideMode();        // vtable slot 0x6f0
//     m_nLastPercent  = pOrgBar->m_nLastPercent;          // +0x410
//     m_bEnableAutoHideAll = pOrgBar->IsAutohideAllEnabled();  // vtable slot 0x690
// Slots 0x6f0 and 0x690 are the one-instruction getters at RVA 0x12e90
// (`mov 0x474(%rcx),%eax`) and 0x12e10 (`mov 0x420(%rcx),%eax`), verified with
// vt.py on the CDockablePane vftable at RVA 0x2e3068, so both reduce to a plain
// field read on the source pane.
// PARTIAL, and the one deviation is deliberate: m_ahSlideMode lives at +0x474,
// which is exactly where a derived CBaseTabbedPane's own padding blob starts
// here (see the offset note near the top), so that field has no storage and is
// NOT copied. The other three are inside the reachable window and are copied at
// their retail offsets. m_rectRestored is not dead weight -- CalcRectForSliding
// and (indirectly) OffsetRectForSliding below read it back.
// The base call goes to the sibling CPane::CopyState thunk
// (phase4/src/featurepack/docking/CPane.cpp:131), which is itself still an empty
// stub, so today it contributes nothing.
// Signature corrected to carry `this`.
// Symbol: ?CopyState@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CDockablePane__UEAAXPEAV1__Z(
    CDockablePane* pThis, CDockablePane* pOrgBar) {
    if (pThis == nullptr || pOrgBar == nullptr) return;  // deviation: retail has no NULL check
    impl__CopyState_CPane__UEAAXPEAV1__Z(static_cast<CPane*>(pThis), static_cast<CPane*>(pOrgBar));
    DockablePaneField<RECT>(pThis, kOffRectRestored) =
        DockablePaneField<RECT>(pOrgBar, kOffRectRestored);
    // m_ahSlideMode (+0x474) is skipped -- no storage here.
    DockablePaneField<int>(pThis, kOffLastPercent) =
        DockablePaneField<int>(pOrgBar, kOffLastPercent);
    DockablePaneField<int>(pThis, kOffEnableAutoHideAll) =
        DockablePaneField<int>(pOrgBar, kOffEnableAutoHideAll);
}

// STUB. Retail entry RVA 0x43110 (a static, so no `this`). It instantiates the
// caller's CRuntimeClass (CRuntimeClass::CreateObject RVA 0x233380), rejects
// the result unless it IsKindOf the descriptor at RVA 0x2f4b58 ("CPaneDivider"),
// sets the new divider's field at +0x1c4 to 1, or -- when pSliderRTC is NULL --
// uses the default RTC held at RVA 0x3aab10 and calls RVA 0xab720 on it; then
// calls the divider's CreateEx (vtable slot 0x560) and deletes it on failure.
// Blocked: OpenMFC models no CPaneDivider class or vtable.
// Symbol: ?CreateDefaultPaneDivider@CDockablePane@@SAPEAVCPaneDivider@@KPEAVCWnd@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__CreateDefaultPaneDivider_CDockablePane__SAPEAVCPaneDivider__KPEAVCWnd__PEAUCRuntimeClass___Z(unsigned long p0, void* /*class*/* p1, void* /*struct*/* p2) {
    return nullptr;
}

// STUB. Retail entry RVA 0x43a30. Its body touches the CDockablePane field
// at +0x4b0 (m_pTabbedControlBarRTC), the CRuntimeClass it instantiates.
// Blocked: that offset is past the end of OpenMFC's 1144-byte CDockablePane.
// Symbol: ?CreateTabbedPane@CDockablePane@@UEAAPEAVCTabbedPane@@XZ
extern "C" void* MS_ABI impl__CreateTabbedPane_CDockablePane__UEAAPEAVCTabbedPane__XZ() {
    return nullptr;
}

// STUB. Retail entry RVA 0x44390. It works against the
// CPaneContainerManager reference it is handed and touches the CDockablePane
// field at +0x4c0 (m_hDefaultSlider). Blocked: OpenMFC models no
// CPaneContainerManager, and +0x4c0 is past the end of the object.
// Symbol: ?DockPaneContainer@CDockablePane@@UEAAHAEAVCPaneContainerManager@@KW4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockPaneContainer_CDockablePane__UEAAHAEAVCPaneContainerManager__KW4AFX_DOCK_METHOD___Z(void* /*class*/* p0, unsigned long p1, int /*enum*/ p2, short* p3, int p4, char p5, void* p6, void* p7, unsigned long p8, int p9, float p10, unsigned char p11, void* p12, int p13, void* p14, char p15) {
    return 0;
}

// STUB. Retail entry RVA 0x45ae0. It reads GetDockingMode() (vtable slot 0x380),
// then computes `nSensitivity = (mode & DT_SMART) ? -1 : <global at RVA
// 0x3aaacc>` (DT_SMART == 0x80) and calls IsChangeState (vtable slot 0x578)
// with it and a CBasePane* out-parameter; the remainder was not transcribed.
// Blocked: OpenMFC has no docking-mode global and no working IsChangeState.
// Symbol: ?DockPaneStandard@CDockablePane@@UEAAPEAVCPane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CDockablePane__UEAAPEAVCPane__AEAH_Z(int* p0) {
    return nullptr;
}

// STUB. Retail entry RVA 0x44220. Reads the CDockablePane field at +0x4c0
// (m_hDefaultSlider). Blocked: that offset is past the end of OpenMFC's
// 1144-byte CDockablePane.
// Symbol: ?DockToWindow@CDockablePane@@UEAAHPEAV1@KPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DockToWindow_CDockablePane__UEAAHPEAV1_KPEBUtagRECT___Z(void* /*class*/* p0) {
    return 0;
}

// STUB. Retail entry RVA 0x444f0. Its body touches the CDockablePane fields
// at +0x41c (m_bActive) and +0x490/+0x498 (m_arrButtons.m_pData / m_nSize).
// Blocked: OpenMFC's CDockablePane has no m_arrButtons, and +0x490/+0x498 are
// past the end of the object.
// Symbol: ?DrawCaption@CDockablePane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawCaption_CDockablePane__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// STUB. Retail entry RVA 0x45260, fully decoded:
//     CMFCCaptionButton* p = FindButtonByHit(nHit);   // RVA 0x45210
//     if (p != NULL) p->{+0x14} = bEnable;            // CMFCCaptionButton field
// Blocked: FindButtonByHit walks m_arrButtons at this+0x488, which OpenMFC's
// CDockablePane does not have.
// Symbol: ?EnableButton@CDockablePane@@IEAAXIH@Z
extern "C" void MS_ABI impl__EnableButton_CDockablePane__IEAAXIH_Z(unsigned int p0, int p1) {}

// STUB. Retail entry RVA 0x40160, fully decoded:
//     m_cyGripper = (bEnable && m_bHasGripper)                 // +0x40c, +0x418
//         ? afxGlobalData<+0x2a8 at RVA 0x3ba628> + 5 : 0;     // after a lazy
//           AFX_GLOBAL_DATA::Initialize (RVA 0x6a5c0) guarded by the flag at
//           RVA 0x3ba380
//     SetWindowPos(NULL, 0,0,0,0, 0x37);   // vtable slot 0x480; 0x37 ==
//           SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED
// Left a stub. m_cyGripper (+0x40c) and m_bHasGripper (+0x418) are both inside
// the reachable window, but the value the gripper height is derived from is not:
// 0x3ba628 is afxGlobalData + 0x2a8, and OpenMFC's afxGlobalData
// (phase4/src/featurepack/CMFC_misc_stubs.cpp:3652) is a 720-byte all-zero blob
// with no Initialize, so the transcription would store a fabricated 5. Retail
// also only ever sets m_bHasGripper from CDockablePane::Create, and this file's
// Create keeps that flag in its side table instead, so the flag would read 0
// forever and the "enable" branch could never be taken.
// Symbol: ?EnableGripper@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(int p0) {}

// CDockablePane::EnterDragMode -- transcribed from retail entry RVA 0x41e20,
// whose whole body is
//     movb $0x1,0x408(%rcx)      ; m_bPrepareToFloat = true
//     jmp  0x9ff20               ; tail call CPane::EnterDragMode(bChangeHotPoint)
// (0x9ff20 is ?EnterDragMode@CPane@@MEAAXH@Z.)
// m_bPrepareToFloat is a single byte at +0x408, inside the padding blob (see
// the offset note near the top), so it is stored at its retail offset here.
// OnNcMouseMove and OnContextMenu below read it back.
// Signature corrected to carry `this`.
// Symbol: ?EnterDragMode@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__EnterDragMode_CDockablePane__MEAAXH_Z(
    CDockablePane* pThis, int bChangeHotPoint) {
    if (pThis == nullptr) return;
    DockablePaneField<bool>(pThis, kOffPrepareToFloat) = true;
    impl__EnterDragMode_CPane__MEAAXH_Z(static_cast<CPane*>(pThis), bChangeHotPoint);
}

// STUB. Retail entry RVA 0x45110. It scans m_arrButtons (m_pData at
// this+0x490, m_nSize at this+0x498) to find the button under `point`.
// Blocked: OpenMFC's CDockablePane has no m_arrButtons.
// Symbol: ?FindButton@CDockablePane@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindButton_CDockablePane__IEBAPEAVCMFCCaptionButton__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// STUB. Retail entry RVA 0x45210, fully decoded:
//     for (i = 0; i < m_arrButtons.m_nSize; i++)          // +0x498
//         if (m_arrButtons.m_pData[i]->{+0x20} == nHit)   // +0x490
//             return m_arrButtons.m_pData[i];
//     return NULL;
// (out-of-range indices call AfxThrowInvalidArgException, RVA 0x225b80.)
// Blocked: OpenMFC's CDockablePane has no m_arrButtons.
// Symbol: ?FindButtonByHit@CDockablePane@@IEBAPEAVCMFCCaptionButton@@I@Z
extern "C" void* MS_ABI impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(unsigned int p0) {
    return nullptr;
}

// STUB. Retail entry RVA 0x46520:
//     if ({slot 0x2f0}()) return 0;      // body at RVA 0x8780 is `!IsDocked()`
//     if (IsMDITabbed()) return 0;       // vtable slot 0x2f8
//     if (m_cyGripper == 0) return 0;    // +0x40c
//     return m_cyGripper + <object at RVA 0x97f4>->{vtable slot 0x1a0}();
// Left a stub. m_cyGripper (+0x40c) is inside the reachable window, but nothing
// in this DLL writes it (see EnableGripper above), so the transcription would
// return 0 on the m_cyGripper == 0 test anyway; and the singleton at RVA 0x97f4
// (no name in the RVA map -- its body lazily builds an object cached at RVA
// 0x3b7120) supplies the extra height and is not modelled.
// Signature corrected to carry `this` (HitTest below calls it).
// Symbol: ?GetCaptionHeight@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    (void)pThis;
    return 0;
}

// STUB. Retail entry RVA 0x45cd0, fully decoded:
//     CWnd* p = CWnd::FromHandlePermanent(m_hDefaultSlider);   // +0x4c0, RVA 0x2891d0
//     return (p != NULL && p->IsKindOf(<RTC at RVA 0x2f4b58, "CPaneDivider">))
//            ? (CPaneDivider*)p : NULL;
// Blocked: +0x4c0 is past the end of OpenMFC's 1144-byte CDockablePane, and no
// CPaneDivider runtime class exists here.
// Symbol: ?GetDefaultPaneDivider@CDockablePane@@QEBAPEAVCPaneDivider@@XZ
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ() {
    return nullptr;
}

// STUB. Retail entry RVA 0x45d10. It reads GetDockingMode() (vtable slot 0x380),
// resolves the dock site (vtable slot 0x348) and hands the point to the
// CDockingManager obtained from afxGlobalUtils (CGlobalUtils::GetDockingManager,
// RVA 0x6ccc0). Blocked: OpenMFC has no CDockingManager implementation.
// Symbol: ?GetDockingStatus@CDockablePane@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@H@Z
extern "C" void* MS_ABI impl__GetDockingStatus_CDockablePane__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, void* /*class*/ p10, int p11) {
    return nullptr;
}

// CDockablePane::HitTest -- transcribed from retail entry RVA 0x40e20:
//     CRect rectWindow(0,0,0,0);
//     ::GetWindowRect(m_hWnd, &rectWindow);                   // 0x1802c5370
//     if (!::PtInRect(&rectWindow, point)) return HTNOWHERE;  // 0x1802c5320
//     CDockingManager* pMgr =
//         afxGlobalUtils.GetDockingManager(GetDockSiteFrameWnd());  // slot 0x348,
//                                                             // RVA 0x6ccc0
//     if (pMgr != NULL && pMgr->{+0xc} == 0)                  // else skip the scan
//         for (i = 0; i < m_arrButtons.m_nSize; i++) {        // +0x498 / +0x490
//             CRect rc; m_arrButtons[i]->{vtable +0x28}(&rc);
//             ::OffsetRect(&rc, rectWindow.left, rectWindow.top);
//             if (::PtInRect(&rc, point)) return m_arrButtons[i]->{+0x20};
//         }
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);  // 0x1802c5358
//     ClientToScreen(&rectClient);                             // RVA 0x2a1250
//     if (::PtInRect(&rectClient, point)) return HTCLIENT;
//     if (IsDocked()) {                                        // vtable slot 0x2e0
//         CRect rectCaption;
//         ::SetRect(&rectCaption, rectWindow.left, rectWindow.top + 1,
//                   rectWindow.right, rectWindow.top + 1 + GetCaptionHeight());
//                                                              // slot 0x358, 0x1802c5398
//         if (::PtInRect(&rectCaption, point))
//             return bDetectCaption ? HTCAPTION : HTCLIENT;
//         // (`neg %r14d ; sbb %eax,%eax ; neg %eax ; inc %eax` == (bDetectCaption
//         //  != 0) + 1, and HTCLIENT == 1 / HTCAPTION == 2.)
//     }
//     return HTERROR;
// The IAT slots above were resolved with iat.py; 0x1802c5bd0 is the CFG
// indirect-call trampoline every virtual call in this DLL goes through (iat.py
// reports it is not an import slot), not a call target of its own.
//
// Deviations, both forced: the caption-button scan is dropped -- m_arrButtons
// lives at +0x488, past the usable window (see the offset note near the top) --
// and the docking-manager lookup that gates that scan goes with it. So this
// never reports a caption-button hit code. The remaining four outcomes
// (HTNOWHERE / HTCLIENT / HTCAPTION / HTERROR) are retail's. Note the caption
// strip is empty in practice today because the sibling GetCaptionHeight thunk
// is still a stub returning 0.
// Signature corrected: the generated stub dropped `this` and folded the CPoint
// into the first slot. The by-value CPoint occupies one register (retail passes
// it straight on to ::PtInRect as a POINT), so it is taken as an opaque 8-byte
// value and copied into a POINT.
// Symbol: ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    CDockablePane* pThis, void* point, int bDetectCaption) {
    if (pThis == nullptr) return HTERROR;  // deviation: retail has no NULL check
    POINT pt;
    memcpy(&pt, &point, sizeof(pt));

    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    if (!::PtInRect(&rectWindow, pt)) return HTNOWHERE;

    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rectClient);
    if (::PtInRect(&rectClient, pt)) return HTCLIENT;

    if (impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) {
        const int cyCaption = impl__GetCaptionHeight_CDockablePane__UEBAHXZ(pThis);
        RECT rectCaption = {0, 0, 0, 0};
        ::SetRect(&rectCaption, rectWindow.left, rectWindow.top + 1,
                  rectWindow.right, rectWindow.top + 1 + cyCaption);
        if (::PtInRect(&rectCaption, pt)) return bDetectCaption ? HTCAPTION : HTCLIENT;
    }
    return HTERROR;
}

// STUB. Retail entry RVA 0x45280, fully decoded:
//     CMFCCaptionButton* p = FindButtonByHit(nHit);   // RVA 0x45210
//     return (p != NULL) ? p->{+0x14} : 0;
// Blocked for the same reason as FindButtonByHit: no m_arrButtons.
// Symbol: ?IsButtonEnabled@CDockablePane@@IEBAHI@Z
extern "C" int MS_ABI impl__IsButtonEnabled_CDockablePane__IEBAHI_Z(unsigned int p0) {
    return 0;
}

// STUB. Retail entry RVA 0x41050. It reads the cursor
// position (USER32!GetCursorPos), takes ::GetParent(m_hWnd) and wraps it with
// CWnd::FromHandle (RVA 0x289180); when that parent IsKindOf the descriptor at
// RVA 0x3aa418 ("CPaneFrameWnd") it re-resolves through
// CWnd::FromHandlePermanent (RVA 0x2891d0); then it asks afxGlobalUtils for the
// CDockingManager (CGlobalUtils::GetDockingManager, RVA 0x6ccc0) and returns
// the result of a virtual on it. Blocked: OpenMFC has no CDockingManager.
// Symbol: ?IsChangeState@CDockablePane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
extern "C" void* MS_ABI impl__IsChangeState_CDockablePane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(int /*enum*/* p0, short* p1, int p2, void* p3, void* p4, int p5, void* p6, void* p7, void** p8, void* /*struct*/ p9, int p10, void* /*class*/** p11) {
    return nullptr;
}

// CDockablePane::IsDocked -- transcribed from retail entry RVA 0xb300:
//     CPaneFrameWnd* p = GetParentMiniFrame(FALSE);   // vtable slot 0x460
//     if (p == NULL) return TRUE;
//     return p->{slot 0x350}() != 1;                  // pane count
// (`xor %ecx,%ecx ; cmp $0x1,%eax ; setne %cl` -- note it is != 1, not > 1.)
// Deviation: the pane-count virtual on CPaneFrameWnd is not modelled in this
// DLL (no GetPaneCount export exists here), so the "inside a mini frame" branch
// returns FALSE, which is what retail returns for the common single-pane mini
// frame. A mini frame holding several panes would be reported wrongly.
// Symbol: ?IsDocked@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsDocked_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMiniFrame == nullptr) return TRUE;
    return FALSE;
}

// CDockablePane::IsInFloatingMultiPaneFrameWnd -- transcribed from retail entry
// RVA 0x45ef0:
//     CWnd* p = GetParentMiniFrame(FALSE);            // vtable slot 0x460
//     if (p == NULL) return FALSE;
//     return p->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd));
// (tail-jump to ?IsKindOf@CObject@@ at RVA 0x233310 with %rdx = the descriptor
// at RVA 0x3aa2f8, whose m_lpszClassName reads "CMultiPaneFrameWnd" and whose
// m_nObjectSize is 768.)
// Symbol: ?IsInFloatingMultiPaneFrameWnd@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(const CDockablePane* pThis) {
    if (pThis == nullptr) return FALSE;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMiniFrame == nullptr) return FALSE;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        static_cast<const CObject*>(pMiniFrame),
        impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ());
}

// STUB. Retail entry RVA 0x45330, fully decoded:
//     if (!IsAutoHideMode())        return CBasePane::IsVisible();
//     if (!IsHideInAutoHideMode())  return FALSE;
//     return m_pAutoHideBar->IsVisible();
// IsAutoHideMode is vtable slot 0x3c8 (its body at RVA 0x12e00 is
// `mov 0x438(%rcx),%eax`), IsHideInAutoHideMode is slot 0x6e8, the fall-through
// tail-jumps to ?IsVisible@CBasePane@@ at RVA 0xb390, and m_pAutoHideBar is the
// field at +0x480.
// Blocked: +0x480 is past the end of OpenMFC's 1144-byte CDockablePane.
// Symbol: ?IsVisible@CDockablePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CDockablePane__UEBAHXZ() {
    return 0;
}

// CDockablePane::LoadState -- transcribed from retail. The RVA map has no entry
// for the UNICODE mangling, but the disassembler reads mfc140.dll (the ANSI
// twin), where the same export is ?LoadState@CDockablePane@@UEAAHPEBDHI@Z at
// RVA 0x45640 and its ENTIRE body is
//     jmp 0x1800a2260            ; ?LoadState@CPane@@UEAAHPEBDHI@Z
// -- an unconditional tail jump with no argument shuffling, i.e. CDockablePane
// adds nothing to CPane::LoadState. Reproduced as a straight forward.
// Note the sibling CPane::LoadState thunk
// (phase4/src/featurepack/docking/CPane.cpp:188) is itself still a stub
// returning 0, so this returns FALSE today.
// Signature corrected to carry `this`.
// Symbol: ?LoadState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CDockablePane__UEAAHPEB_WHI_Z(
    CDockablePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    return impl__LoadState_CPane__UEAAHPEB_WHI_Z(
        static_cast<CPane*>(pThis), lpszProfileName, nIndex, uiID);
}

// CDockablePane::OffsetRectForSliding -- transcribed from retail entry RVA
// 0x429a0. The whole body is:
//     if (!IsAutoHideMode()) return;                  // vtable slot 0x3c8
//     int d = m_nSlideDelta;                          // +0x454
//     switch (GetCurrentAlignment()) {                // vtable slot 0x338
//     case CBRS_ALIGN_LEFT:   ::OffsetRect(&rect, (bParam2 != bParam3) ? d : -d, 0); break;
//     case CBRS_ALIGN_RIGHT:  ::OffsetRect(&rect, (bParam2 == bParam3) ? d : -d, 0); break;
//     case CBRS_ALIGN_TOP:    ::OffsetRect(&rect, 0, bParam2 ? d : -d); break;
//     case CBRS_ALIGN_BOTTOM: ::OffsetRect(&rect, 0, bParam2 ? -d : d); break;
//     default: return;                                // no ::OffsetRect call
//     }
// The four sign tables were read off the jump chain at 0x4299c..0x42a5c
// (0x1802c5318 resolves to USER32!OffsetRect); the two BOOL arguments arrive in
// %r8d and %r9d and MFC does not export names for them, so they are named
// positionally here. m_nSlideDelta lives inside the padding blob (see the
// offset note near the top) and is read at its retail offset.
// Note: the sibling CBasePane::GetCurrentAlignment thunk
// (phase4/src/featurepack/docking/CBasePane.cpp:262) is still a stub returning
// 0, so today no case matches and this returns without offsetting anything --
// which is exactly what retail does for an alignment of 0. Nothing in this DLL
// writes m_nSlideDelta yet either (retail writes it from Slide/OnSlide, both
// still stubs here), so it reads back as the 0 the constructor left.
// Signature corrected to carry `this`.
// Symbol: ?OffsetRectForSliding@CDockablePane@@IEAAXAEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OffsetRectForSliding_CDockablePane__IEAAXAEAVCRect__HH_Z(
    CDockablePane* pThis, CRect& rect, int bParam2, int bParam3) {
    if (pThis == nullptr) return;
    if (!pThis->IsAutoHideMode()) return;

    const int nDelta = DockablePaneField<int>(pThis, kOffSlideDelta);
    int dx = 0;
    int dy = 0;
    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis))) {
    case kAlignLeft:   dx = ((bParam2 != 0) != (bParam3 != 0)) ?  nDelta : -nDelta; break;
    case kAlignRight:  dx = ((bParam2 != 0) == (bParam3 != 0)) ?  nDelta : -nDelta; break;
    case kAlignTop:    dy = (bParam2 != 0) ?  nDelta : -nDelta; break;
    case kAlignBottom: dy = (bParam2 != 0) ? -nDelta :  nDelta; break;
    default: return;
    }
    ::OffsetRect(reinterpret_cast<RECT*>(&rect), dx, dy);
}

// CDockablePane::OnAfterChangeParent -- transcribed from retail entry RVA
// 0x40400, which is exactly:
//     CPane::OnAfterChangeParent(pWndOldParent);          // RVA 0xa0fb0
//     if (GetParentMiniFrame(FALSE) != NULL)              // vtable slot 0x460
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(this, TRUE);  // RVA 0xaf740
// (`mov $0x1,%edx` before the static call is the bAdd argument.)
// Symbol: ?OnAfterChangeParent@CDockablePane@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CDockablePane__UEAAXPEAVCWnd___Z(
    CDockablePane* pThis, CWnd* pWndOldParent) {
    if (pThis == nullptr) return;
    impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(static_cast<CPane*>(pThis), pWndOldParent);
    if (impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            static_cast<const CBasePane*>(pThis), FALSE) != nullptr) {
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
            static_cast<CBasePane*>(pThis), TRUE);
    }
}

// CDockablePane::OnAfterDock -- transcribed from retail entry RVA 0x402b0. All
// three arguments are ignored; the body reads only `this`:
//     if (!CDockingManager::m_bRestoringDockState)   // DWORD at RVA 0x3b6f50
//         SetFocus();                                // RVA 0x2a7a70
//     if (GetDockingMode() == DT_IMMEDIATE) {        // vtable slot 0x380
//         ::GetCursorPos(&m_ptClientHotSpot);        // this+0x208
//         ::ScreenToClient(m_hWnd, &m_ptClientHotSpot);
//     }
//     if (GetDlgCtrlID() != -1 &&                    // RVA 0x2a78b0
//         GetParentMiniFrame(FALSE) == NULL)         // vtable slot 0x460
//         CPaneFrameWnd::AddRemovePaneFromGlobalList(this, FALSE);  // RVA 0xaf740
// (0x1802c5348 and 0x1802c5340 resolve to USER32!GetCursorPos and
// USER32!ScreenToClient; m_ptClientHotSpot is the CPane member this repo
// already models at +0x208.)
// Deviation: CBasePane::GetDockingMode (RVA 0xc820) is
// `m_dockMode != 0 ? m_dockMode : <CGlobalUtils default at RVA 0x3aaac8>`;
// OpenMFC has no such global, so m_dockMode is read directly and a zero
// m_dockMode is treated as "not DT_IMMEDIATE" -- correct whenever the
// application default is DT_STANDARD, wrong if it was set to DT_IMMEDIATE.
// Symbol: ?OnAfterDock@CDockablePane@@MEAAXPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__OnAfterDock_CDockablePane__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pBar, const RECT* lpRect, int dockMethod) {
    (void)pBar;
    (void)lpRect;
    (void)dockMethod;
    if (pThis == nullptr) return;
    if (!impl__m_bRestoringDockState_CDockingManager__2HA) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
    }
    if (pThis->m_dockMode == kDockTypeImmediate) {
        // Retail passes &m_ptClientHotSpot straight to both APIs; OpenMFC's
        // CPoint is not derived from POINT, so a temporary is used and copied
        // back. Same net effect.
        POINT ptHotSpot = {0, 0};
        ::GetCursorPos(&ptHotSpot);
        ::ScreenToClient(pThis->m_hWnd, &ptHotSpot);
        pThis->m_ptClientHotSpot.x = ptHotSpot.x;
        pThis->m_ptClientHotSpot.y = ptHotSpot.y;
    }
    if (impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis)) != -1 &&
        impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
            static_cast<const CBasePane*>(pThis), FALSE) == nullptr) {
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
            static_cast<CBasePane*>(pThis), FALSE);
    }
}

// CDockablePane::OnBeforeChangeParent -- transcribed from retail entry RVA
// 0x40340:
//     if (pWndNewParent == NULL) return;                 // `test %rdx,%rdx; je ret`
//     BOOL bTabbed = pWndNewParent->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane));
//     if (bTabbed) StoreRecentDockSiteInfo();            // vtable slot 0x648
//     if (pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)) ||
//         pWndNewParent->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) || bTabbed)
//         UndockPane(bDelay);                             // vtable slot 0x490
//     CPane::OnBeforeChangeParent(pWndNewParent, FALSE);  // RVA 0xa0f80
// The three descriptors are the ones at RVA 0x2db168 ("CBaseTabbedPane"),
// 0x3aa418 ("CPaneFrameWnd") and 0x310b30 ("CMFCTabCtrl"), read out of their
// m_lpszClassName fields. Note the final call passes a hard FALSE
// (`xor %r8d,%r8d`), NOT the caller's bDelay.
// Slot 0x648 of the CDockablePane vftable (RVA 0x2e3068) holds
// ?StoreRecentDockSiteInfo@CDockablePane@@MEAAXXZ (RVA 0x41260) and slot 0x490
// holds ?UndockPane@CDockablePane@@UEAAXH@Z (RVA 0x44d20); this file has both
// exports, so the two virtual calls are reproduced as direct calls to its own
// thunks (both of which are still stubs -- see `stubbed`).
// Symbol: ?OnBeforeChangeParent@CDockablePane@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CDockablePane__UEAAXPEAVCWnd__H_Z(
    CDockablePane* pThis, CWnd* pWndNewParent, int bDelay) {
    if (pThis == nullptr || pWndNewParent == nullptr) return;
    CObject* pParentObj = static_cast<CObject*>(pWndNewParent);
    int bTabbed = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        pParentObj, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ());
    if (bTabbed) {
        impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(pThis);
    }
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ()) ||
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ()) ||
        bTabbed) {
        impl__UndockPane_CDockablePane__UEAAXH_Z(pThis, bDelay);
    }
    impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(
        static_cast<CPane*>(pThis), pWndNewParent, FALSE);
}

// CDockablePane::OnCancelMode -- transcribed from retail entry RVA 0x450d0,
// whose whole body is:
//     StopCaptionButtonsTracking();       // vtable slot 0x728
//     if (m_bPrepareToFloat) m_bPrepareToFloat = false;   // byte at this+0x408
//     CPane::OnCancelMode();              // tail jump, RVA 0xa2930
// (`cmpb $0x0,0x408(%rbx) ; je ; movb $0x0,0x408(%rbx)` at 0x450e9.)
// All three steps are reproduced. The m_bPrepareToFloat clear MUST be here:
// +0x408 is inside the reachable padding window (see the offset note near the
// top), EnterDragMode above sets that byte, and OnNcMouseMove and OnContextMenu
// below both branch on it -- leaving it set would wedge those two handlers on
// the "in a float drag" path forever after the first drag.
// Symbol: ?OnCancelMode@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;
    impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(pThis);
    DockablePaneField<bool>(pThis, kOffPrepareToFloat) = false;
    impl__OnCancelMode_CPane__IEAAXXZ(static_cast<CPane*>(pThis));
}

// CDockablePane::OnClose -- transcribed from retail entry RVA 0x43510. The
// entire body is `mov (%rcx),%rax ; mov 0xd0(%rax),%rax ; rex.W jmp *...`, a
// tail-jump through vtable byte offset 0xd0, which in the CDockablePane vftable
// (RVA 0x2e3068) holds ?DestroyWindow@CWnd@@UEAAHXZ (RVA 0x289f70). So the
// handler is simply `DestroyWindow();`.
// Symbol: ?OnClose@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;
    impl__DestroyWindow_CWnd__UEAAHXZ(static_cast<CWnd*>(pThis));
}

// CDockablePane::OnContextMenu -- transcribed from retail entry RVA 0x46200,
// whose whole body is:
//     if (m_bPrepareToFloat) return;                        // byte at +0x408
//     if (m_bCaptionButtonsCaptured)                        // +0x4d8
//         StopCaptionButtonsTracking();                     // vtable slot 0x728
//     CPane::OnContextMenu(pWnd, point);                    // RVA 0xa21b0
// m_bPrepareToFloat is inside the padding blob and is honoured here;
// EnterDragMode above sets it.
// Deviation: the m_bCaptionButtonsCaptured branch is dropped -- +0x4d8 is past
// the end of the 1144-byte object (see the offset note near the top). That
// costs nothing observable today because this file's
// StopCaptionButtonsTracking is itself a stub, but a real one would not run.
// Signature corrected to carry `this`; the by-value CPoint occupies one
// register and is forwarded to CPane::OnContextMenu unexamined, as in retail.
// Symbol: ?OnContextMenu@CDockablePane@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CDockablePane__IEAAXPEAVCWnd__VCPoint___Z(
    CDockablePane* pThis, CWnd* pWnd, void* point) {
    if (pThis == nullptr) return;
    if (DockablePaneField<bool>(pThis, kOffPrepareToFloat)) return;
    impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
        static_cast<CPane*>(pThis), pWnd, point);
}

// STUB. The RVA map has no entry for the UNICODE mangling, but the ANSI twin
// the disassembler reads has ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTA@@@Z
// at RVA 0x401f0:
//     if (Default() == -1) return -1;                      // 0x180289090
//     SetCaptionButtons();                                 // vtable slot 0x730
//     if (CTooltipManager::CreateToolTip(m_pToolTip, this, 16))   // +0x4b8,
//                                                          // RVA 0x1808e0
//         for (int i = 1; i < 4; i++) {
//             CRect rc; ::SetRectEmpty(&rc);               // 0x1802c5368
//             m_pToolTip->AddTool(this, (LPCTSTR)-1, &rc, i);
//         }                                                // RVA 0x273c00
//     return 0;
// (Argument order read off the call at 0x18004027c: %rcx = m_pToolTip,
// %rdx = this, %r8 = -1 (LPSTR_TEXTCALLBACK, the lpszText slot), %r9 = &rc and
// [%rsp+0x20] = i, i.e. AddTool(CWnd*, LPCTSTR, LPCRECT, UINT_PTR).)
// Blocked: m_pToolTip lives at +0x4b8, past the end of the 1144-byte object,
// and the two tooltip helpers above are not implemented here either.
// Symbol: ?OnCreate@CDockablePane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CDockablePane__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// STUB. Retail entry RVA 0x44e00. Its body touches the CDockablePane fields
// at +0x490 (m_arrButtons.m_pData) and +0x4b8 (m_pToolTip) while tearing the
// pane down. Blocked: OpenMFC's CDockablePane owns neither, and both offsets
// are past the end of the 1144-byte object.
// Symbol: ?OnDestroy@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDockablePane__IEAAXXZ() {}

// CDockablePane::OnDrawDragRect -- transcribed from retail entry RVA 0x40d40.
// `this` is never read (%rcx is overwritten at 0x40d61 without being saved), so
// the missing CDockablePane layout does not block this one. Retail does:
//     CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));  // 0x2a1b60 / 0x289180,
//                                                            // slot 0x1802c4c00 =
//                                                            // USER32!GetDesktopWindow
//     dc.DrawDragRect(lprectNew, CSize(1,1), lprectOld, CSize(1,1), NULL, NULL);
// (RVA 0x2a3660. The {1,1} qword at rsp+0x98 is passed both in %r8 as `size`
// and at rsp+0x20 as `sizeLast`; rsp+0x28 and rsp+0x30 are the two zeroed
// CBrush* arguments. Unlike CMDIClientAreaWnd::DrawNewGroupRect, the caller's
// pointers are forwarded unchanged -- there is no NULL-to-empty-rect fixup.)
// Symbol: ?OnDrawDragRect@CDockablePane@@MEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__OnDrawDragRect_CDockablePane__MEAAXPEBUtagRECT__0_Z(
    CDockablePane* pThis, const RECT* lprectNew, const RECT* lprectOld) {
    (void)pThis;  // retail never reads `this` here

    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());

    // CWindowDC exists in this DLL only as its impl__ thunks, so the object is
    // built in raw storage and torn down through the matching thunk.
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pDesktop);

    SIZE size;
    size.cx = 1;
    size.cy = 1;
    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        pDC, lprectNew, size, lprectOld, size, nullptr, nullptr);

    impl___1CWindowDC__UEAA_XZ(pDC);
}

// STUB. Retail entry RVA 0x43250. Its body touches the CDockablePane field
// at +0x4c0 (m_hDefaultSlider). Blocked: that offset is past the end of
// OpenMFC's 1144-byte CDockablePane.
// Symbol: ?OnLButtonDblClk@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// STUB. Retail entry RVA 0x41150. Its body touches the CDockablePane fields
// at +0x408 (m_bPrepareToFloat), +0x4d0 (m_nHot) and +0x4d4 (m_nHit). Blocked:
// m_bPrepareToFloat is reachable, but m_nHot and m_nHit are past the end of the
// object and the body's whole purpose is to latch them.
// Symbol: ?OnLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// STUB. Retail entry RVA 0x41840 (~1296 bytes). Its body touches the
// CDockablePane fields at +0x408 (m_bPrepareToFloat), +0x438 (m_bPinState),
// +0x480 (m_pAutoHideBar), +0x4d0 (m_nHot) and +0x4d4 (m_nHit). Blocked: the
// first two are reachable, but m_pAutoHideBar, m_nHot and m_nHit are all past
// the end of the 1144-byte object.
// Symbol: ?OnLButtonUp@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// STUB. Retail entry RVA 0x41480 (~950 bytes), not transcribed. It branches on
// GetDockingMode() (vtable slot 0x380): the DT_IMMEDIATE arm drags the pane
// itself (CPane::UpdateVirtualRect RVA 0xa1ba0, then either
// CBaseTabbedPane::{slot 0x350} or FloatPane via slot 0x408 followed by
// CGlobalUtils::GetDockingManager/ForceAdjustLayout, RVA 0x6ccc0 / 0x6d800),
// the DT_STANDARD arm moves the drag frame (CMFCDragFrameImpl::MoveDragFrame,
// RVA 0x55450). Blocked: it reads and writes m_bPrepareToFloat (+0x408),
// m_bReadyToFloat (+0x409) and m_bCaptionButtonsCaptured (+0x4d8) -- the last
// is past the end of the object -- plus a drag anchor point at +0x240, which
// falls inside the 128-byte m_dragFrameImpl blob this repo models opaquely.
// Symbol: ?OnMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CDockablePane__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// STUB. Retail entry RVA 0x40570:
//     Default();                                       // 0x180289090
//     if ({slot 0x2f0}())          // body at RVA 0x8780 is `!IsDocked()`
//         for (i = 0; i < m_arrButtons.m_nSize; i++)   // +0x498 / +0x490
//             m_arrButtons[i]->{+0x10} = i + 1;
//     else
//         // reserves the caption strip: GetCaptionHeight() (slot 0x358),
//         // ::CopyRect of lpncsp->rgrc[0] (0x1802c5240) and m_nBorderSize
//         // (+0x414); the rest of that arm was not transcribed.
// Blocked: m_arrButtons is at +0x488, past the end of the 1144-byte object.
// Symbol: ?OnNcCalcSize@CDockablePane@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CDockablePane__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// CDockablePane::OnNcHitTest -- transcribed from retail entry RVA 0x40de0,
// whose whole body is:
//     int nHit = HitTest(point, FALSE);      // vtable slot 0x678 == HitTest
//                                            // (`xor %r8d,%r8d` is bDetectCaption)
//     if (nHit != HTERROR) return (unsigned)nHit;   // `mov %eax,%eax`
//     return Default();                      // tail jmp 0x180289090
// Slot 0x678 of the CDockablePane vftable (RVA 0x2e3068) holds
// ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z (RVA 0x40e20), which this file
// exports, so the virtual call is reproduced as a direct call to its own thunk.
// 0x180289090 is ?Default@CWnd@@IEAA_JXZ.
// Signature corrected to carry `this`.
// Symbol: ?OnNcHitTest@CDockablePane@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CDockablePane__IEAA_JVCPoint___Z(
    CDockablePane* pThis, void* point) {
    if (pThis == nullptr) return HTERROR;
    const int nHit = impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(pThis, point, FALSE);
    if (nHit != HTERROR) return static_cast<__int64>(static_cast<unsigned int>(nHit));
    return impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// CDockablePane::OnNcLButtonDown -- transcribed from retail entry RVA 0x434e0,
// whose whole body is:
//     if (!IsDocked()) Default();     // vtable slot 0x2e0, then 0x180289090
// Neither argument is read. Slot 0x2e0 of the CDockablePane vftable holds
// ?IsDocked@CDockablePane@@UEBAHXZ (RVA 0xb300), which this file exports, so
// the virtual call is a direct call to its own thunk.
// Signature corrected to carry `this`.
// Symbol: ?OnNcLButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nHitTest, void* point) {
    (void)nHitTest;
    (void)point;
    if (pThis == nullptr) return;
    if (!impl__IsDocked_CDockablePane__UEBAHXZ(pThis)) {
        (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// CDockablePane::OnNcMouseMove -- transcribed from retail entry RVA 0x44ff0,
// whose whole body is:
//     if (!m_bPrepareToFloat)                 // `cmpb $0x0,0x408(%rcx)`
//         OnTrackCaptionButtons(point);       // vtable slot 0x720, `mov %r8,%rdx`
//     Default();                              // tail jmp 0x180289090
// nHitTest is never read. Slot 0x720 holds
// ?OnTrackCaptionButtons@CDockablePane@@MEAAXVCPoint@@@Z (RVA 0x44f00), which
// this file exports (still a stub -- see `stubbed`), so the virtual call is a
// direct call to its own thunk. m_bPrepareToFloat is inside the padding blob;
// EnterDragMode above sets it.
// Signature corrected to carry `this`.
// Symbol: ?OnNcMouseMove@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nHitTest, void* point) {
    (void)nHitTest;
    if (pThis == nullptr) return;
    if (!DockablePaneField<bool>(pThis, kOffPrepareToFloat)) {
        impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(pThis, point);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// STUB. Retail entry RVA 0x408f0, not transcribed. Its opening is
//     if (CBasePane::m_bMultiThreaded)                  // RVA 0x3b6f00
//         ::EnterCriticalSection(<the section at RVA 0x3aabd0>);
//     CWindowDC dc(this);                               // RVA 0x2a1b60
//     ::GetUpdateRect(m_hWnd, &rectUpdate, FALSE);      // 0x1802c4d00
//     ::GetClientRect(m_hWnd, &rectClient);  ClientToScreen(&rectClient);
//     ::GetWindowRect(m_hWnd, &rectWindow);
//     ::OffsetRect(&rectClient, -rectWindow.left, -rectWindow.top);
//     ::OffsetRect(&rectWindow, -rectWindow.left, -rectWindow.top);
// after which it paints the caption and the caption buttons through the visual
// manager. Blocked: the paint itself needs m_arrButtons (+0x488) and the
// visual-manager singleton, neither of which exists here.
// Symbol: ?OnNcPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CDockablePane__IEAAXXZ() {}

// STUB. Retail entry RVA 0x46120, fully decoded:
//     if (pNMHDR == NULL || m_pToolTip == NULL ||       // m_pToolTip at +0x4b8
//         m_pToolTip->m_hWnd == NULL || pNMHDR->hwndFrom != m_pToolTip->m_hWnd)
//         return FALSE;
//     switch (pNMHDR->idFrom) {
//     case 1:  nID = 0x3ea0; break;                     // close button
//     case 2:  nID = 0x3ea1;                            // pin button
//              if (::GetAsyncKeyState(VK_CONTROL) < 0)  // 0x1802c4c08, key 0x11
//                  nID += IsAutohideAllEnabled() ? 3 : 0;   // vtable slot 0x690
//              break;                                    // (`neg %eax ; sbb %ecx,%ecx
//                                                        //   ; and $0x3,%ecx`)
//     case 3:  nID = 0x428e; break;
//     default: return FALSE;
//     }
//     HINSTANCE h = AfxFindStringResourceHandle(nID);   // RVA 0x2accf0
//     if (h == NULL || !<the module-level CString at RVA 0x3ba308>.LoadString(h, nID))
//         return FALSE;                                 // RVA 0xdc00
//     ((TOOLTIPTEXT*)pNMHDR)->lpszText = that string;   // pNMHDR+0x18
//     return TRUE;
// Blocked: m_pToolTip is at +0x4b8, past the end of the 1144-byte object, so
// the guard that gates the whole body cannot be evaluated.
// Symbol: ?OnNeedTipText@CDockablePane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CDockablePane__IEAAHIPEAUtagNMHDR__PEA_J_Z(unsigned int p0, void* /*struct*/* p1, __int64* p2) {
    return 0;
}

// CDockablePane::OnPaint -- transcribed from retail. This export is absent from
// mfc140_rva_symbols.json only because that map is keyed by RVA and the linker
// ICF-folded this body onto another name; it is NOT missing from the DLL. In
// mfc140.dll it shares RVA 0x41000 with ?OnPaint@CDockSite@@IEAAXXZ and in
// mfc140u.dll RVA 0x411b0 with the same twin, and the whole body (bar the
// /GS cookie prologue and epilogue) is:
//     mov %rcx,%rdx ; lea 0x20(%rsp),%rcx ; call 0x1802a1c60  ; CPaintDC dc(this)
//     lea 0x20(%rsp),%rcx ; call 0x1802a1d10                  ; dc.~CPaintDC()
// (0x2a1c60 == ??0CPaintDC@@QEAA@PEAVCWnd@@@Z and 0x2a1d10 == ??1CPaintDC@@UEAA@XZ
// in the mfc140 RVA map; the mfc140u pair is 0x2a3d20 / 0x2a3dd0.) So the whole
// handler is a bare BeginPaint/EndPaint pair that validates the update region
// and paints nothing of its own.
// Signature corrected to carry `this` (the generated stub dropped it).
// As with OnDrawDragRect above, CPaintDC exists in this DLL only as its impl__
// thunks, so the object is built in raw storage and torn down through the
// matching thunk. Neither thunk dispatches a virtual, so the unset vfptr in
// that storage is never used.
// Symbol: ?OnPaint@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDockablePane__IEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// CDockablePane::OnPressCloseButton -- transcribed from retail entry RVA
// 0x41d50, whose whole body is:
//     CFrameWnd* pFrame = AFXGetParentFrame(this);          // RVA 0x6bb10
//     if (pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) &&
//         ::SendMessage(pFrame->m_hWnd, AFX_WM_ON_PRESS_CLOSE_BUTTON,
//                       0, (LPARAM)this) != 0)
//         return;                                           // the pane was handled
//     if (IsAutoHideMode())                                 // vtable slot 0x3c8
//         SetAutoHideMode(FALSE, GetCurrentAlignment(), NULL, TRUE);
//                                    // slot 0x6e0; slot 0x338; `movl $1,0x20(%rsp)`
//     ShowPane(FALSE, FALSE, FALSE);                        // slot 0x458
//     AdjustDockingLayout(NULL);                            // slot 0x4e0
// The descriptor at RVA 0x338990 reads "CFrameWnd" through its
// m_lpszClassName; the message id is the ?AFX_WM_ON_PRESS_CLOSE_BUTTON@@3IA
// export at RVA 0x3ba300. The IAT slot 0x1802c5378 resolves to
// USER32!SendMessageA in the ANSI twin the disassembler reads, i.e.
// ::SendMessage in this UNICODE build.
// Deviation: OpenMFC does not model this class's vftable, so the three
// self-virtuals dispatch statically -- IsAutoHideMode/SetAutoHideMode through
// the C++ definitions in this file and ShowPane/AdjustDockingLayout through the
// matching exports. A derived override would not run.
// Signature corrected to carry `this`.
// Symbol: ?OnPressCloseButton@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__OnPressCloseButton_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    if (pThis == nullptr) return;

    CFrameWnd* pParentFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(
        static_cast<const CWnd*>(pThis));
    if (pParentFrame != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParentFrame),
            impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ()) &&
        ::SendMessage(pParentFrame->m_hWnd, impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA,
                      0, reinterpret_cast<LPARAM>(pThis)) != 0) {
        return;
    }

    if (pThis->IsAutoHideMode()) {
        pThis->SetAutoHideMode(
            FALSE,
            impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pThis)),
            nullptr, TRUE);
    }
    impl__ShowPane_CDockablePane__UEAAXHHH_Z(pThis, FALSE, FALSE, FALSE);
    impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(static_cast<CBasePane*>(pThis), nullptr);
}

// CDockablePane::OnRButtonDown -- transcribed from retail entry RVA 0x41440,
// whose whole body is:
//     SetFocus();                                        // RVA 0x2a7a70
//     CMFCPopupMenu* p = CMFCPopupMenu::m_pActivePopupMenu;   // RVA 0x3b6fe8
//     if (p != NULL && CWnd::FromHandlePermanent(p->m_hWnd) != NULL)
//         CMFCPopupMenu::UpdateAllShadows(NULL);         // RVA 0xbb290, `xor %ecx,%ecx`
//     Default();                                         // tail jmp 0x180289090
// Neither argument is read. `mov 0x40(%rcx),%rcx` is CWnd::m_hWnd on the popup
// menu; the C++ member is used here rather than the literal offset because
// OpenMFC's CWnd is not laid out like retail's.
// Symbol: ?OnRButtonDown@CDockablePane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CDockablePane__IEAAXIVCPoint___Z(
    CDockablePane* pThis, unsigned int nFlags, void* point) {
    (void)nFlags;
    (void)point;
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));

    CWnd* pActivePopup = reinterpret_cast<CWnd*>(impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA);
    if (pActivePopup != nullptr &&
        impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pActivePopup->m_hWnd) != nullptr) {
        impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(nullptr);
    }
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// Symbol: ?OnSetFocus@CDockablePane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CDockablePane__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetText@CDockablePane@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CDockablePane__IEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSettingChange@CDockablePane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CDockablePane__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnSlide@CDockablePane@@MEAAXH@Z
extern "C" void MS_ABI impl__OnSlide_CDockablePane__MEAAXH_Z(int p0) {}

// Symbol: ?OnTimer@CDockablePane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CDockablePane__IEAAX_K_Z(unsigned __int64 p0) {}

// STUB. Retail entry RVA 0x44f00; not transcribed -- it tracks the caption
// buttons held in m_arrButtons (+0x488), which is past the usable window.
// Signature corrected to carry `this` (OnNcMouseMove above calls it).
// Symbol: ?OnTrackCaptionButtons@CDockablePane@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CDockablePane__MEAAXVCPoint___Z(
    CDockablePane* pThis, void* point) {
    (void)pThis;
    (void)point;
}

// Symbol: ?OnUpdateCmdUI@CDockablePane@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDockablePane__UEAAXPEAVCFrameWnd__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnUpdateToolTips@CDockablePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CDockablePane__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?PreTranslateMessage@CDockablePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDockablePane__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RedrawButton@CDockablePane@@IEAAXPEBVCMFCCaptionButton@@@Z
extern "C" void MS_ABI impl__RedrawButton_CDockablePane__IEAAXPEBVCMFCCaptionButton___Z(const void* /*class*/* p0) {}

// Symbol: ?RemoveCaptionButtons@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveCaptionButtons_CDockablePane__IEAAXXZ() {}

// Symbol: ?RemoveFromDefaultPaneDivider@CDockablePane@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveFromDefaultPaneDivider_CDockablePane__QEAAXXZ() {}

// Symbol: ?ReplacePane@CDockablePane@@QEAAHPEAV1@W4AFX_DOCK_METHOD@@H@Z
extern "C" int MS_ABI impl__ReplacePane_CDockablePane__QEAAHPEAV1_W4AFX_DOCK_METHOD__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?SaveState@CDockablePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CDockablePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CDockablePane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDockablePane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetAutoHideParents@CDockablePane@@QEAAXPEAVCMFCAutoHideBar@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?SetCaptionButtons@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtons_CDockablePane__MEAAXXZ() {}

// Symbol: ?SetDefaultPaneDivider@CDockablePane@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(void* /*struct*/* p0) {}

// Symbol: ?SetResizeMode@CDockablePane@@QEAAXH@Z
extern "C" void MS_ABI impl__SetResizeMode_CDockablePane__QEAAXH_Z(int p0) {}

// Symbol: ?Slide@CDockablePane@@UEAAXHH@Z
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(int p0, int p1) {}

// STUB. Retail entry RVA 0x45030. It walks m_arrButtons (this+0x488, m_pData
// at +0x490 / m_nSize at +0x498) clearing each caption button's pressed/hot
// state and repainting. OpenMFC's CDockablePane has no m_arrButtons -- offsets
// from +0x478 up are past the end of the 1144-byte object -- so there is
// nothing to walk. Signature corrected to carry `this` (it is called above).
// Symbol: ?StopCaptionButtonsTracking@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// STUB. Retail entry RVA 0x41260; not transcribed. Signature corrected to
// carry `this` (it is called from OnBeforeChangeParent above).
// Symbol: ?StoreRecentDockSiteInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CDockablePane__MEAAXXZ(CDockablePane* pThis) {
    (void)pThis;
}

// Symbol: ?StoreRecentTabRelatedInfo@CDockablePane@@MEAAXXZ
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CDockablePane__MEAAXXZ() {}

// Symbol: ?ToggleAutoHide@CDockablePane@@UEAAXXZ
extern "C" void MS_ABI impl__ToggleAutoHide_CDockablePane__UEAAXXZ() {}

// STUB. Retail entry RVA 0x44d20; not transcribed. Signature corrected to
// carry `this` (it is called from OnBeforeChangeParent above).
// Symbol: ?UndockPane@CDockablePane@@UEAAXH@Z
extern "C" void MS_ABI impl__UndockPane_CDockablePane__UEAAXH_Z(CDockablePane* pThis, int bDelay) {
    (void)pThis;
    (void)bDelay;
}

// Symbol: ?UnSetAutoHideMode@CDockablePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?UpdateTooltips@CDockablePane@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CDockablePane__IEAAXXZ() {}
