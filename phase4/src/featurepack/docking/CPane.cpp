// CPane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"


#include <cstddef>
#include <cstdint>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Retail decode notes for the bodies added below (2026-09).
//
// Every RVA cited is a FUNCTION ENTRY in retail mfc140.dll (image base
// 0x180000000) unless the comment says otherwise; where an individual
// instruction is meant that is stated explicitly.  mfc140.dll is the ANSI twin
// of the mfc140u.dll OpenMFC reimplements: the function BODIES are identical,
// so control flow, member offsets and constants read off it are correct, but
// its RVAs are NOT mfc140u RVAs.  Two bodies (GetPaneName, LoadState) exist
// only in the Unicode image and are cited as "(mfc140u)".
//
// An indirect `call *0x1802c5bd0` in the retail listings is the Control Flow
// Guard dispatch (__guard_dispatch_icall_fptr), i.e. an ordinary indirect call
// through RAX -- it is not a USER32 import.  Every USER32/CRT call named below
// was resolved through the import table, not guessed.
//
// CPane vtable slots referenced below.  These were read out of the retail CPane
// vftable in .rdata (mfc140 RVA 0x2f3a18, located by matching slot 0 against
// ?GetThisClass@CPane@@SAPEAUCRuntimeClass@@XZ at 0x8a00 and slot 93 against
// ?IsTabbed@CPane@@UEBAHXZ at 0xa2100).  Slots whose target is an unexported
// inline were identified by disassembling the target:
//   slot  91 (+0x2d8) 0x88e0  IsHorizontal()  -> GetCurrentAlignment()&0xA000
//   slot  92 (+0x2e0) 0xb300  IsDocked()
//   slot  93 (+0x2e8) 0xa2100 CPane::IsTabbed()
//   slot  94 (+0x2f0) 0x8780  IsFloating()    -> !IsDocked()
//   slot 105 (+0x348) 0xc770  CBasePane::GetDockSiteFrameWnd()
//   slot 106 (+0x350) 0x87e0  `return m_pParentDockBar;` (member at +0x128)
//   slot 112 (+0x380) 0xc820  CBasePane::GetDockingMode()
//   slot 114 (+0x390) 0x8840  GetPaneStyle()  -> `return m_dwStyle;` (+0x104)
//   slot 129 (+0x408) 0xa09f0 CPane::FloatPane()
//   slot 140 (+0x460) 0xb460  CBasePane::GetParentMiniFrame(BOOL bNoAssert)
//   slot 144 (+0x480) 0xb6a0  CBasePane::SetWindowPos()
//   slot 158 (+0x4f0) 0x8910  GetMinSize(CSize&) -> `size = m_sizeMin;`
//   slot 177 (+0x588) 0xa2080 CPane::OnProcessDblClk()
//   slot 192 (+0x600) 0xa0900 CPane::OnBeforeDock()
//   slot 193 (+0x608) 0x2820  CPane::OnAfterDock() -- an empty `ret`
//   slot 197 (+0x628) 0xa21a0 CPane::SetDragMode(BOOL)
//   slot 199 (+0x638) 0x9ff20 CPane::EnterDragMode(BOOL)
//   slot 200 (+0x640) 0xa2c30 CPane::OnRTLChanged(BOOL)
//   slot 201 (+0x648) 0x9fff0 CPane::StoreRecentDockSiteInfo()
// Slots added while decoding the 2026-09-07 batch, read out of the same
// vftable at mfc140 RVA 0x2f3a18:
//   slot 137 (+0x448) 0xa0f80 CPane::OnBeforeChangeParent()
//   slot 138 (+0x450) 0xa0fb0 CPane::OnAfterChangeParent()
//   slot 143 (+0x478) 0xb580  CBasePane::MoveWindow(const CRect&, BOOL, HDWP)
//   slot 148 (+0x4a0) 0xcf50  CBasePane::OnPaneContextMenu(CWnd*, CPoint)
//   slot 154 (+0x4d0) 0xacb0  CalcFixedLayout(BOOL, BOOL) -- the CBasePane body
//                             the linker folded onto CControlBar's
//   slot 156 (+0x4e0) 0xbdb0  CBasePane::AdjustDockingLayout(HDWP)
//   slot 160 (+0x500) 0xa1400 CPane::GetAvailableExpandSize()
//   slot 161 (+0x508) 0xa14f0 CPane::GetAvailableStretchSize()
//   slot 187 (+0x5d8) 0xa2cf0 CPane::OnShowControlBarMenu(CPoint)
//   slot 194 (+0x610) 0x2820  CPane::OnBeforeStretch(int&) -- empty `ret`
//   slot 195 (+0x618) 0x2820  CPane::OnAfterStretch(int)   -- empty `ret`
//   slot 196 (+0x620) 0xa07f0 CPane::RemoveFromMiniframe(CWnd*, AFX_DOCK_METHOD)
// Two OTHER classes' vftables are dispatched through by the bodies below.  Each
// was located the same way -- anchor slot 0 on that class's GetThisClass export,
// then confirm a second known export at the offset in question:
//   CDockSite   vftable mfc140 RVA 0x2e4388 (slot 0 = 0xa580)
//     slot 161 (+0x508) 0x53e10 CDockSite::RemovePane(CPane*, AFX_DOCK_METHOD)
//     slot 162 (+0x510) 0x53370 CDockSite::MovePane(CPane*, UINT, CPoint)
//   CBaseTabbedPane vftable mfc140 RVA 0x2db2a8 (slot 0 = 0x12f40)
//     slot 242 (+0x790) 0x13710 CBaseTabbedPane::RemovePane(CWnd*)
// Retail CPaneFrameWnd vtable slots used below (same method, vftable located
// from the CPaneFrameWnd constructor as documented in CPaneFrameWnd.cpp):
//   +0x308 0xaf680 CPaneFrameWnd::RemovePane(CBasePane*, BOOL, BOOL)
//   +0x458 0xb1b10 CPaneFrameWnd::CloseMiniFrame()
//
// Deliberate, shared deviations from retail, applied to every body below:
//  * a NULL check on `this` (and on out-pointer arguments).  Retail has none --
//    it would fault -- but the rest of this file already guards that way and a
//    generated stub can hand these thunks anything.
//  * where retail dispatches a virtual through the CPane vtable, OpenMFC's
//    CPane has no matching vtable, so the corresponding exported base
//    implementation is called through its impl__ thunk instead.  That loses
//    dispatch to a derived override; each site says so.  Several of those base
//    thunks (IsDocked, GetCurrentAlignment, GetDockingMode,
//    GetDockSiteFrameWnd, GetParentMiniFrame) are still generated stubs in
//    phase4/src/featurepack/docking/CBasePane.cpp as of this writing, so the
//    bodies below currently take the "not docked / no alignment / no dock site
//    / no mini frame" branch at run time.  They become correct as those are
//    implemented; nothing here needs to change.
// ---------------------------------------------------------------------------

// Sibling exports called below.  BRIEFING S1: the C++ methods do not exist
// inside this DLL, only these extern "C" thunks.  Some of them are still
// generated stubs whose parameter list omits the implicit `this`; the retail
// exports do take it in RCX and those stub bodies read no argument, so
// declaring the retail-correct list here is safe under the MS x64 ABI.
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int  MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ();
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString);
extern "C" int MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(const CBasePane* pThis);
extern "C" CWnd* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);
extern "C" void MS_ABI impl__CopyState_CBasePane__UEAAXPEAV1__Z(CBasePane* pThis, CBasePane* pOrgBar);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags, void* pExtra);
extern "C" void MS_ABI impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(void* pThis, int bClearInternalRects);
extern "C" void MS_ABI impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
    CBasePane* pBar, int bAdd);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* pHDWP);
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
    CBasePane* pThis, const RECT* lpRect, int bRepaint, void* hdwp);
extern "C" void MS_ABI impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(
    CBasePane* pThis, CWnd* pWnd, void* point);
extern "C" void MS_ABI impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(
    CDockBar* pThis, CPane* pPane, int dockMethod);
extern "C" int MS_ABI impl__MovePane_CDockSite__UEAAHPEAVCPane__IVCPoint___Z(
    CDockBar* pThis, CPane* pPane, unsigned int nFlags, void* ptOffset);
extern "C" int MS_ABI impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(CWnd* pThis, CWnd* pBar);
// atlmfc/include/afxdragframeimpl.h:37 declares
// `void MoveDragFrame(BOOL bForceMove = FALSE)`.
extern "C" void MS_ABI impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(void* pThis, int bForceMove);

// Exported DATA these bodies read.  ?m_pActivePopupMenu@CMFCPopupMenu@@1PEAV1@EA
// lives at mfc140 0x3b6fe8, ?m_bCustomizeMode@CMFCToolBar@@1HA at 0x3b70bc and
// ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA at 0x3b6f18 -- the three addresses
// CPane::OnContextMenu tests.  Defined in phase4/src/core/runtime/StaticData.cpp,
// phase4/src/featurepack/toolbar/StaticData.cpp and
// phase4/src/featurepack/CMFC_misc_stubs.cpp respectively.
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;

// Sibling thunks defined further down in THIS file.
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ(CPane* pThis);
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(CPane* pThis, void* ptOffset);
extern "C" int MS_ABI impl__OnShowControlBarMenu_CPane__UEAAHVCPoint___Z(CPane* pThis, void* point);
extern "C" void MS_ABI impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CWnd* pNewParent, int dockMethod);
extern "C" void MS_ABI impl__SetDragMode_CPane__MEAAXH_Z(CPane* pThis, int bOnOff);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPane__MEAAXXZ(CPane* pThis);
extern "C" void MS_ABI impl__OnCancelMode_CPane__IEAAXXZ(CPane* pThis);
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane* pThis, int bChangeHotPoint);
extern "C" void MS_ABI impl__OnRTLChanged_CPane__MEAAXH_Z(CPane* pThis, int bIsRTL);
extern "C" int MS_ABI impl__IsTabbed_CPane__UEBAHXZ(const CPane* pThis);
extern "C" int MS_ABI impl__OnBeforeDock_CPane__MEAAHPEAPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane** ppDockBar, const RECT* lpRect, int dockMethod);
extern "C" int MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
    CPane* pThis, const RECT* pRectFloat, int dockMethod, bool bShow);

// Sibling exports added for the 2026-09-08 batch (OnLButtonUp, SaveState).
// impl__SaveState_CBasePane__UEAAHPEB_WHI_Z is still a generated stub in
// phase4/src/featurepack/docking/CBasePane.cpp whose parameter list omits the
// implicit `this`; the retail export does take it in RCX and that stub body
// reads no argument, so the retail-correct list declared here is safe under
// the MS x64 ABI (the same reasoning as the block above).
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(
    void* pThis, bool bMoveBackToVirtualRect, CPane* pBarToExclude);
extern "C" int MS_ABI impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(
    CBasePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
// CString AFXGetRegPath(LPCTSTR lpszPostFix, LPCTSTR lpszProfileName): the
// by-value CStringT return is a hidden first parameter under the MS x64 ABI.
extern "C" CString* MS_ABI
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        CString* pRet, const wchar_t* lpszPostFix, const wchar_t* lpszProfileName);
// The settings store.  Create() lazily allocates the owned CSettingsStore with
// ::operator new and returns it; CSettingsStore is not a declared type in any
// OpenMFC header, so it is handled as an opaque void* here, exactly as
// phase4/src/core/app/CSettingsStoreSP.cpp does.
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
    void* pThis, int bAdmin, int bReadOnly);
extern "C" int MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* path);
extern "C" int MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* key);
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WH_Z(void* self, const wchar_t* name, int value);
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WK_Z(
    void* self, const wchar_t* name, unsigned long value);
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(
    void* self, const wchar_t* name, const CRect* rect);

namespace {
// Style bits from atlmfc/include/afxres.h (verified against the shipping SDK
// headers on this host):
//   CBRS_FLOATING 0x00000001  CBRS_BORDER_LEFT 0x0100  CBRS_BORDER_TOP 0x0200
//   CBRS_BORDER_RIGHT 0x0400  CBRS_BORDER_BOTTOM 0x0800
//   CBRS_ALIGN_LEFT 0x1000    CBRS_ALIGN_TOP  0x2000
//   CBRS_ALIGN_RIGHT 0x4000   CBRS_ALIGN_BOTTOM 0x8000
//   CBRS_GRIPPER 0x00400000   CBRS_ORIENT_HORZ = TOP|BOTTOM = 0xA000
const unsigned long kCbrsFloating     = 0x00000001UL;
const unsigned long kCbrsBorderLeft   = 0x00000100UL;
const unsigned long kCbrsBorderTop    = 0x00000200UL;
const unsigned long kCbrsBorderRight  = 0x00000400UL;
const unsigned long kCbrsBorderBottom = 0x00000800UL;
const unsigned long kCbrsOrientHorz   = 0x0000A000UL;
const unsigned long kCbrsBorderAny    = 0x00000F00UL;
const unsigned long kCbrsAlignAny     = 0x0000F000UL;
const unsigned long kCbrsGripper      = 0x00400000UL;
// WS_EX_LAYOUTRTL, the bit CalcInsideRect/OnStyleChanged test.
const unsigned long kExLayoutRtl      = 0x00400000UL;
// AFX_DOCK_METHOD (atlmfc/include/afxbasepane.h:42): DM_UNKNOWN, DM_MOUSE,
// DM_DBL_CLICK, DM_SHOW, DM_RECT, DM_STANDARD.
const int kDockMethodUnknown  = 0;
const int kDockMethodMouse    = 1;
const int kDockMethodDblClick = 2;
const int kDockMethodStandard = 5;
// AFX_DOCK_TYPE: DT_IMMEDIATE = 1, DT_STANDARD = 2 -- the two bits
// CPane::OnCancelMode and CPane::OnMouseMove test against GetDockingMode().
const int kDockTypeImmediate = 1;
const int kDockTypeStandard = 2;

// CBasePane::IsHorizontal is `_inline` in atlmfc/include/afxbasepane.h:313 --
// `return GetCurrentAlignment() & CBRS_ORIENT_HORZ;` -- so it is not exported;
// retail reaches it through CPane vtable slot 91, whose target 0x88e0 is
// literally `GetCurrentAlignment(); return eax & 0xA000;`.  Calling the
// GetCurrentAlignment thunk here reproduces the CBasePane behaviour but not a
// derived override of GetCurrentAlignment.
inline bool PaneIsHorizontal(const CPane* pThis) {
    return (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(
                reinterpret_cast<const CBasePane*>(pThis)) & kCbrsOrientHorz) != 0;
}
// CBasePane::IsFloating is CPane vtable slot 94, target 0x8780:
// `return IsDocked() == 0;`.
inline bool PaneIsFloating(const CPane* pThis) {
    return impl__IsDocked_CBasePane__UEBAHXZ(reinterpret_cast<const CBasePane*>(pThis)) == 0;
}

// CPane vtable slot 154 (+0x4d0) is CalcFixedLayout(BOOL bStretch, BOOL bHorz);
// for a plain CPane it resolves to 0xacb0, the CBasePane body the linker folded
// onto ?CalcFixedLayout@CControlBar@@UEAA?AVCSize@@HH@Z.  Disassembled, that
// body is nothing but
//     cx = (bStretch && bHorz)  ? 0x7fff : 0;
//     cy = (bStretch && !bHorz) ? 0x7fff : 0;
// It is reproduced inline because no ?CalcFixedLayout@CBasePane@@... export
// exists to call -- which also means a derived override (CDockablePane's
// returns the pane's own window size) is NOT reached from here.
inline SIZE PaneBaseCalcFixedLayout(int bStretch, int bHorz) {
    SIZE size;
    size.cx = (bStretch && bHorz) ? 32767 : 0;
    size.cy = (bStretch && !bHorz) ? 32767 : 0;
    return size;
}

// CPane vtable slot 174 (+0x570) resolves to mfc140 RVA 0x8970, which is
// exactly three instructions -- `*(int*)rdx = 0; return rcx;`.  That is
// CPane::DockPaneStandard(BOOL& bWasDocked), declared inline in the shipping
// SDK header atlmfc/include/afxpane.h:139 as literally
//     bWasDocked = FALSE; return this;
// Being inline it is not exported, so it is reproduced here; a derived
// override (CDockablePane declares one at afxdockablepane.h:137) is therefore
// NOT reached from here.
inline CPane* PaneBaseDockPaneStandard(CPane* pThis, int& bWasDocked) {
    bWasDocked = FALSE;
    return pThis;
}

// CMFCDragFrameImpl member offsets.  OpenMFC models the object as an opaque
// 128-byte blob (include/openmfc/afxmfc.h), so its fields are reached by byte
// offset.  The offsets are those of the class declared in the shipping SDK
// header atlmfc/include/afxdragframeimpl.h -- vfptr(0), m_ptHot(8),
// m_rectDrag(0x10), m_rectExpectedDocked(0x20), m_bDockToTab(0x30),
// m_bDragStarted(0x34), m_nInsertedTabID(0x38), m_pFinalTargetBar(0x40),
// m_pOldTargetBar(0x48), m_nOldThickness(0x50), m_pDraggedWnd(0x58),
// m_pDockManager(0x60), m_pTargetBar(0x68), m_pWndDummy(0x70),
// m_bFrameTabDrawn(0x78) -- which total exactly the retail sizeof 128 the
// header records, and whose m_ptHot at +8 is the offset the OnLButtonDown body
// above already uses.
const std::size_t kDragFrameRectDrag     = 0x10;  // CRect m_rectDrag
const std::size_t kDragFrameDragStarted  = 0x34;  // BOOL  m_bDragStarted
const std::size_t kDragFrameInsertedTab  = 0x38;  // int   m_nInsertedTabID

// CRecentDockSiteInfo member offsets, for the same reason (opaque 288-byte
// blob here).  Layout of the SDK class in atlmfc/include/afxrecentdocksiteinfo.h:
//   0x00 vfptr (it derives from CObject)
//   0x08 CRect m_rectRecentFloatingRect
//   0x18 DWORD m_dwRecentAlignmentToFrame
//   0x1c int   m_nRecentTabNumber
//   0x20 int   m_nRecentRowIndex          (+4 pad)
//   0x28 CDockSite*        m_pRecentDockBar
//   0x30 CDockingPanesRow* m_pRecentDockBarRow
//   0x38 HWND  m_hRecentDefaultSlider
//   0x40 HWND  m_hRecentMiniFrame
//   0x48 CRecentPaneContainerInfo m_recentSliderInfo     (104 bytes)
//   0xb0 CRecentPaneContainerInfo m_recentMiniFrameInfo  (104 bytes)
//   0x118 CPane* m_pBar
//   0x120 == 288, the retail sizeof this repo records for the blob.
// CRecentPaneContainerInfo's own 104-byte layout is the one already pinned by
// phase4/src/detail/CRecentPaneContainerInfoSupport.h (vfptr 0,
// m_rectDockedRect 8, ...), so m_recentSliderInfo.m_rectDockedRect lands at
// 0x48+8 == 0x50 -- which is the offset CPane::SaveState writes under the
// "RectRecentDocked" value name.  (An earlier revision of this comment added
// "and is what GetRecentDockedRect(FALSE) returns"; that is backwards.
// ?GetRecentDockedRect@CRecentDockSiteInfo@@QEAAAEAVCRect@@H@Z, mfc140 RVA
// 0xd22b0, is `return this + (bForSlider ? 0x50 : 0xb8)`, so +0x50 is the
// bForSlider == TRUE result and GetRecentDockedRect(FALSE) returns
// m_recentMiniFrameInfo.m_rectDockedRect at +0xb8 instead.)
const std::size_t kRecentFloatingRect = 0x08;  // CRect m_rectRecentFloatingRect
const std::size_t kRecentAlignment    = 0x18;  // DWORD m_dwRecentAlignmentToFrame
const std::size_t kRecentRowIndex     = 0x20;  // int   m_nRecentRowIndex
const std::size_t kRecentDockedRect   = 0x50;  // m_recentSliderInfo.m_rectDockedRect

inline char* RecentDockInfoBytes(CPane* pThis) {
    return reinterpret_cast<char*>(&pThis->m_recentDockInfo);
}

// MSVC x64 passes an 8-byte POD by value in one integer register, so a CPoint
// or CSize argument arrives in the register a `void*` parameter would use.
// These two helpers move between that packed form and a POINT.
inline void* PackPoint(int x, int y) {
    const unsigned long long v =
        (static_cast<unsigned long long>(static_cast<unsigned int>(y)) << 32) |
        static_cast<unsigned long long>(static_cast<unsigned int>(x));
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v));
}
inline POINT UnpackPoint(void* packed) {
    const unsigned long long v =
        static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(packed));
    POINT pt;
    pt.x = static_cast<int>(static_cast<unsigned int>(v & 0xffffffffULL));
    pt.y = static_cast<int>(static_cast<unsigned int>(v >> 32));
    return pt;
}
}  // namespace
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
// Retail mfc140 RVA 0xa1e80, transcribed:
//   if (nElements < 0 || cbElement < 0)         AfxThrowInvalidArgException();
//   if (m_pData == NULL && m_nCount != 0)       AfxThrowInvalidArgException();
//   pNew = NULL;
//   if (nElements > 0) {
//       if (cbElement <= 0)                     AfxThrowInvalidArgException();
//       pNew = calloc(nElements, cbElement);    // import slot 0x1802c5550
//       if (pNew == NULL) return 0;             // EAX is calloc's own 0
//   }
//   free(m_pData);                              // import slot 0x1802c5558
//   m_pData = pNew; m_nCount = nElements; return 1;
// The two throwing tests fall to `call ?AfxThrowInvalidArgException@@YAXXZ`
// (0x225b80) followed by int3, so retail never returns from them.
extern "C" int MS_ABI impl__AllocElements_CPane__QEAAHHH_Z(CPane* pThis, int nElements, int cbElement) {
    if (pThis == nullptr) return FALSE;
    if (nElements < 0 || cbElement < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (pThis->m_pData == nullptr && pThis->m_nCount != 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    void* pNew = nullptr;
    if (nElements > 0) {
        if (cbElement <= 0) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        pNew = std::calloc(static_cast<std::size_t>(nElements), static_cast<std::size_t>(cbElement));
        if (pNew == nullptr) return FALSE;
    }
    std::free(pThis->m_pData);
    pThis->m_pData = pNew;
    pThis->m_nCount = nElements;
    return TRUE;
}

// Symbol: ?CalcAvailableSize@CPane@@UEAA?AVCSize@@VCRect@@@Z
// Retail mfc140 RVA 0xa1590.  MSVC passes `this` in RCX, the hidden CSize
// return buffer in RDX and the by-value CRect (16 bytes, so by address) in R8.
// Transcribed:
//   CSize sizeMin(0, 0); GetMinSize(sizeMin);            // vtable slot 158
//   int l = rect.left, r = rect.right, t = rect.top, b = rect.bottom;
//   if (r - l < sizeMin.cx) r = l + sizeMin.cx;
//   if (b - t < sizeMin.cy) b = t + sizeMin.cy;
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);    // import 0x1802c5370
//   ret->cx = (r - l) - rectWnd.Width();
//   ret->cy = (b - t) - rectWnd.Height();
// GetMinSize is the unexported inline at 0x8910 (`size = m_sizeMin;`), so the
// member is read directly here; a derived override would be missed.
extern "C" void* MS_ABI impl__CalcAvailableSize_CPane__UEAA_AVCSize__VCRect___Z(
    CPane* pThis, SIZE* pRet, const RECT* pRectRequired) {
    if (pRet == nullptr) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr || pRectRequired == nullptr) return pRet;
    const int cxMin = pThis->m_sizeMin.cx;
    const int cyMin = pThis->m_sizeMin.cy;
    int left = pRectRequired->left, right = pRectRequired->right;
    int top = pRectRequired->top, bottom = pRectRequired->bottom;
    if (right - left < cxMin) right = left + cxMin;
    if (bottom - top < cyMin) bottom = top + cyMin;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    pRet->cx = (right - left) - (rectWnd.right - rectWnd.left);
    pRet->cy = (bottom - top) - (rectWnd.bottom - rectWnd.top);
    return pRet;
}

// Symbol: ?CalcInsideRect@CPane@@QEBAXAEAVCRect@@H@Z
// Retail mfc140 RVA 0xa1f10, transcribed:
//   DWORD dwStyle = GetPaneStyle();                  // slot 114 -> m_dwStyle
//   if (!IsFloating() && !IsTabbed()) {              // slots 94 and 93
//       if (dwStyle & CBRS_BORDER_LEFT)   rect.left++;
//       if (dwStyle & CBRS_BORDER_TOP)    rect.top++;
//       if (dwStyle & CBRS_BORDER_RIGHT)  rect.right--;
//       if (dwStyle & CBRS_BORDER_BOTTOM) rect.bottom--;
//   }
//   DWORD g = dwStyle & (CBRS_GRIPPER | CBRS_FLOATING);
//   if (bHorz) {
//       rect.left += m_cxLeftBorder;   rect.top    += m_cyTopBorder;
//       rect.right -= m_cxRightBorder; rect.bottom -= m_cyBottomBorder;
//       if (g == CBRS_GRIPPER) {
//           if (GetExStyle() & WS_EX_LAYOUTRTL) rect.right -= 7;  // addl $-7
//           else                                rect.left  += 7;
//       }
//   } else {                       // the transposed (vertical) assignment
//       rect.left += m_cyTopBorder;    rect.top    += m_cxLeftBorder;
//       rect.right -= m_cyBottomBorder; rect.bottom -= m_cxRightBorder;
//       if (g == CBRS_GRIPPER) rect.top += 7;
//   }
// GetPaneStyle is `return m_dwStyle;` (afxbasepane.h:121, and the vtable target
// 0x8840 is exactly that), so the member is read directly.
extern "C" void MS_ABI impl__CalcInsideRect_CPane__QEBAXAEAVCRect__H_Z(
    const CPane* pThis, RECT* pRect, int bHorz) {
    if (pThis == nullptr || pRect == nullptr) return;
    const unsigned long dwStyle = pThis->m_dwStyle;
    CPane* pMutable = const_cast<CPane*>(pThis);
    if (!PaneIsFloating(pThis) &&
        !impl__IsTabbed_CPane__UEBAHXZ(pThis)) {
        if (dwStyle & kCbrsBorderLeft)   pRect->left++;
        if (dwStyle & kCbrsBorderTop)    pRect->top++;
        if (dwStyle & kCbrsBorderRight)  pRect->right--;
        if (dwStyle & kCbrsBorderBottom) pRect->bottom--;
    }
    const bool bGripperOnly = (dwStyle & (kCbrsGripper | kCbrsFloating)) == kCbrsGripper;
    if (bHorz) {
        pRect->left   += pThis->m_cxLeftBorder;
        pRect->top    += pThis->m_cyTopBorder;
        pRect->right  -= pThis->m_cxRightBorder;
        pRect->bottom -= pThis->m_cyBottomBorder;
        if (bGripperOnly) {
            if (impl__GetExStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pMutable)) & kExLayoutRtl) {
                pRect->right -= 7;
            } else {
                pRect->left += 7;
            }
        }
    } else {
        pRect->left   += pThis->m_cyTopBorder;
        pRect->top    += pThis->m_cxLeftBorder;
        pRect->right  -= pThis->m_cyBottomBorder;
        pRect->bottom -= pThis->m_cxRightBorder;
        if (bGripperOnly) pRect->top += 7;
    }
}

// Symbol: ?CanBeTabbedDocument@CPane@@UEBAHXZ
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa36c0 is:
//   if (this->virt(+0x3c8)()) return FALSE;                 // CPane's own
//                                                           // target is 0x7260
//                                                           // == `xor eax,eax`
//   CWnd* pFrame = GetDockSiteFrameWnd();                   // slot 105
//   if (pFrame == NULL) return FALSE;
//   if (!pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return FALSE;
//   return ((CMDIFrameWndEx*)pFrame)->virt(+0x3c8)() != 0;
// The last step is CMDIFrameWndEx vtable slot 121, whose target 0x880c0 is the
// unexported inline `return *(int*)(this + 0x208);` -- an AreMDITabs-style flag
// that OpenMFC's CMDIFrameWndEx does not model (no shadow struct, no member at
// +0x208).  Writing the first three steps and then hard-coding FALSE would
// give the wrong answer exactly on the path the function exists for, so this
// stays a stub.
extern "C" int MS_ABI impl__CanBeTabbedDocument_CPane__UEBAHXZ(const CPane* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?ConvertToTabbedDocument@CPane@@UEAAXH@Z
// NOT IMPLEMENTED.  This export has no RVA in either image's symbol map
// (checked mfc140 and mfc140u), so there is no retail body to transcribe here;
// atlmfc/include/afxpane.h declares it `virtual void
// ConvertToTabbedDocument(BOOL bActiveTabOnly = TRUE)` but gives no inline.
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CPane__UEAAXH_Z(CPane* pThis, int bActiveTabOnly) {
    (void)pThis;
    (void)bActiveTabOnly;
}

// Symbol: ?CopyState@CPane@@UEAAXPEAV1@@Z
// Retail mfc140 RVA 0xa3610, transcribed:
//   CBasePane::CopyState(pOrgBar);                        // call 0xce40
//   m_bFirstInGroup  = pOrgBar->m_bFirstInGroup;          // +0x1ac
//   m_bLastInGroup   = pOrgBar->m_bLastInGroup;           // +0x1b0
//   m_bActiveInGroup = pOrgBar->m_bActiveInGroup;         // +0x1b4
//   pOrgBar->GetMinSize(this->m_sizeMin);                 // slot 158, RCX is
//                                                         // pOrgBar, RDX is
//                                                         // this+0x200
//   m_recentDockInfo = pOrgBar->m_recentDockInfo;         // call 0xd2440 ==
//                                        ??4CRecentDockSiteInfo@@QEAAAEAV0@AEAV0@@Z
//   m_rectSavedDockedRect  = pOrgBar->m_rectSavedDockedRect;   // 16 bytes
//   m_bRecentFloatingState = pOrgBar->m_bRecentFloatingState;  // +0x3e8
// GetMinSize is the unexported inline `size = m_sizeMin;` (0x8910), so the
// member is copied directly.  CRecentDockSiteInfo is modelled in
// include/openmfc/afxmfc.h as an opaque 288-byte block with no members and no
// ownership, so its retail operator= is a plain byte copy here; that must
// become a real assignment if the struct ever gains members.
extern "C" void MS_ABI impl__CopyState_CPane__UEAAXPEAV1__Z(CPane* pThis, CPane* pOrgBar) {
    if (pThis == nullptr || pOrgBar == nullptr) return;
    impl__CopyState_CBasePane__UEAAXPEAV1__Z(static_cast<CBasePane*>(pThis),
                                             static_cast<CBasePane*>(pOrgBar));
    pThis->m_bFirstInGroup  = pOrgBar->m_bFirstInGroup;
    pThis->m_bLastInGroup   = pOrgBar->m_bLastInGroup;
    pThis->m_bActiveInGroup = pOrgBar->m_bActiveInGroup;
    pThis->m_sizeMin = pOrgBar->m_sizeMin;
    std::memcpy(&pThis->m_recentDockInfo, &pOrgBar->m_recentDockInfo, sizeof(CRecentDockSiteInfo));
    pThis->m_rectSavedDockedRect  = pOrgBar->m_rectSavedDockedRect;
    pThis->m_bRecentFloatingState = pOrgBar->m_bRecentFloatingState;
}

// Symbol: ?CreateDefaultMiniframe@CPane@@UEAAPEAVCPaneFrameWnd@@VCRect@@@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa1a00 does
//   CObject* p = m_pMiniFrameRTC->CreateObject();     // 0x233380, +0x230
//   if (p == NULL) return NULL;
//   ... two helper calls (0x6bb10, 0x6ccc0) whose results are written into the
//   new mini frame at +0x248 (its CDockingManager*) ...
//   if (!p->virt(+0x418)(NULL, 0x80000000, &rectInitial, <helper>, NULL))
//       { p->virt(+0x8)(1); return NULL; }             // deleting destructor
//   return p;
// The +0x248 store and the +0x418 Create dispatch are both against the retail
// CPaneFrameWnd layout/vtable, which OpenMFC does not reproduce (its
// CPaneFrameWnd is a CMiniFrameWnd subclass with a 128-byte padding blob -- see
// the note at the top of CPaneFrameWnd.cpp).
extern "C" void* MS_ABI impl__CreateDefaultMiniframe_CPane__UEAAPEAVCPaneFrameWnd__VCRect___Z(
    CPane* pThis, const RECT* pRectInitial) {
    (void)pThis;
    (void)pRectInitial;
    return nullptr;
}

// Symbol: ?DockByMouse@CPane@@UEAAHPEAVCBasePane@@@Z
// Retail mfc140 RVA 0xa0420, transcribed:
//   if (!OnBeforeDock(&pDockBar, NULL, DM_MOUSE)) return FALSE;   // slot 192
//   if (!Dock(pDockBar, NULL, DM_MOUSE))          return FALSE;   // slot 198
//   OnAfterDock(pDockBar, NULL, DM_MOUSE);                        // slot 193
//   return TRUE;
// `&pDockBar` is the argument's own home slot, so OnBeforeDock may replace it;
// the second and third calls read it back.  OnAfterDock has no CPane export and
// its vtable target (0x2820) is a bare `ret` -- CPane's override is empty
// (afxpane.h declares it inline and empty) -- so omitting it is behaviour
// preserving for a CPane, though it does skip a derived override.
extern "C" int MS_ABI impl__DockByMouse_CPane__UEAAHPEAVCBasePane___Z(CPane* pThis, CBasePane* pDockBar) {
    if (pThis == nullptr) return FALSE;
    if (!impl__OnBeforeDock_CPane__MEAAHPEAPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
            pThis, &pDockBar, nullptr, kDockMethodMouse)) {
        return FALSE;
    }
    if (!impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
            pThis, pDockBar, nullptr, kDockMethodMouse)) {
        return FALSE;
    }
    return TRUE;
}

// Symbol: ?EnterDragMode@CPane@@MEAAXH@Z
// Retail mfc140 RVA 0x9ff20, transcribed:
//   CPoint pt(0, 0); ::GetCursorPos(&pt);              // import 0x1802c5348
//   UpdateVirtualRect();                               // call 0xa1ae0
//   if (bChangeHotPoint) {
//       m_ptClientHotSpot = pt;                        // +0x208
//       ::ScreenToClient(m_hWnd, &m_ptClientHotSpot);  // import 0x1802c5340
//   }
//   if (!m_bCaptured && IsDocked()) {                  // +0x210, slot 92
//       CWnd::FromHandle(::SetCapture(m_hWnd));        // import 0x1802c5278
//       m_dragFrameImpl.m_ptHot = pt;                  // +0x240
//       m_bCaptured = TRUE;
//       SetDragMode(TRUE);                             // slot 197
//       ::GetWindowRect(m_hWnd, &m_rectDragImmediate); // +0x1f0
//   }
// +0x240 is m_dragFrameImpl (+0x238) plus 8; CMFCDragFrameImpl has a virtual
// destructor (atlmfc/include/afxdragframeimpl.h:34) so its vptr occupies +0
// and m_ptHot sits at +8.  OpenMFC models CMFCDragFrameImpl as an opaque
// 128-byte block, so the store below lands at the same byte offset but under no
// member name, and nothing in this repo reads it back yet.
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane* pThis, int bChangeHotPoint) {
    if (pThis == nullptr) return;
    POINT pt = {0, 0};
    ::GetCursorPos(&pt);
    impl__UpdateVirtualRect_CPane__QEAAXXZ(pThis);
    if (bChangeHotPoint) {
        pThis->m_ptClientHotSpot.x = pt.x;
        pThis->m_ptClientHotSpot.y = pt.y;
        ::ScreenToClient(pThis->m_hWnd, reinterpret_cast<POINT*>(&pThis->m_ptClientHotSpot));
    }
    if (!pThis->m_bCaptured &&
        impl__IsDocked_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pThis))) {
        (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(pThis->m_hWnd));
        *reinterpret_cast<POINT*>(reinterpret_cast<char*>(&pThis->m_dragFrameImpl) + 8) = pt;
        pThis->m_bCaptured = TRUE;
        impl__SetDragMode_CPane__MEAAXH_Z(pThis, TRUE);
        ::GetWindowRect(pThis->m_hWnd, reinterpret_cast<RECT*>(&pThis->m_rectDragImmediate));
    }
}

// Symbol: ?FloatPane@CPane@@UEAAHVCRect@@W4AFX_DOCK_METHOD@@_N@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa09f0 is a ~340 instruction body.  It
// reaches CreateDefaultMiniframe (CPane vtable slot 135) and GetParentMiniFrame
// (slot 140), calls CGlobalUtils::GetDockingManager (0x6ccc0) and then makes
// roughly twenty-five further virtual calls, most of them on the resulting mini
// frame and docking manager.  OpenMFC reproduces neither object's layout nor
// its vtable (its CPaneFrameWnd is a CMiniFrameWnd subclass with a padding blob
// -- see the note at the top of CPaneFrameWnd.cpp), and CreateDefaultMiniframe
// itself is stubbed above for the same reason, so there is nothing faithful to
// transcribe.
extern "C" int MS_ABI impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
    CPane* pThis, const RECT* pRectFloat, int dockMethod, bool bShow) {
    (void)pThis;
    (void)pRectFloat;
    (void)dockMethod;
    (void)bShow;
    return FALSE;
}

// Symbol: ?GetAvailableExpandSize@CPane@@UEBAHXZ
// Retail mfc140 RVA 0xa1400, transcribed:
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);      // import 0x1802c5370
//   if (IsHorizontal()) {                                  // slot 91
//       if (rectWnd.Width() >= m_rectVirtual.Width()) return 0;
//   } else {
//       if (rectWnd.Height() >= m_rectVirtual.Height()) return 0;
//   }
//   return IsHorizontal() ? m_rectVirtual.Width()  - rectWnd.Width()
//                         : m_rectVirtual.Height() - rectWnd.Height();
// Retail evaluates IsHorizontal() three times; it is evaluated once here.
extern "C" int MS_ABI impl__GetAvailableExpandSize_CPane__UEBAHXZ(const CPane* pThis) {
    if (pThis == nullptr) return 0;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    const int cxWnd = rectWnd.right - rectWnd.left;
    const int cyWnd = rectWnd.bottom - rectWnd.top;
    const int cxVirtual = pThis->m_rectVirtual.right - pThis->m_rectVirtual.left;
    const int cyVirtual = pThis->m_rectVirtual.bottom - pThis->m_rectVirtual.top;
    if (PaneIsHorizontal(pThis)) {
        if (cxWnd >= cxVirtual) return 0;
        return cxVirtual - cxWnd;
    }
    if (cyWnd >= cyVirtual) return 0;
    return cyVirtual - cyWnd;
}

// Symbol: ?GetAvailableStretchSize@CPane@@UEBAHXZ
// Retail mfc140 RVA 0xa14f0, transcribed:
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   CSize sizeMin(0, 0); GetMinSize(sizeMin);              // slot 158
//   return IsHorizontal() ? rectWnd.Width()  - sizeMin.cx
//                         : rectWnd.Height() - sizeMin.cy;
extern "C" int MS_ABI impl__GetAvailableStretchSize_CPane__UEBAHXZ(const CPane* pThis) {
    if (pThis == nullptr) return 0;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    if (PaneIsHorizontal(pThis)) {
        return (rectWnd.right - rectWnd.left) - pThis->m_sizeMin.cx;
    }
    return (rectWnd.bottom - rectWnd.top) - pThis->m_sizeMin.cy;
}

// Symbol: ?GetBorders@CPane@@QEBA?AVCRect@@XZ
// Retail mfc140 RVA 0x9fda0.  MSVC passes `this` in RCX and the hidden CRect
// return buffer in RDX, and the whole body is four moves plus `mov %rdx,%rax`:
//   ret->left  = m_cxLeftBorder;  ret->top    = m_cyTopBorder;
//   ret->right = m_cxRightBorder; ret->bottom = m_cyBottomBorder;
extern "C" void* MS_ABI impl__GetBorders_CPane__QEBA_AVCRect__XZ(const CPane* pThis, RECT* pRet) {
    if (pRet == nullptr) return pRet;
    if (pThis == nullptr) {
        pRet->left = pRet->top = pRet->right = pRet->bottom = 0;
        return pRet;
    }
    pRet->left   = pThis->m_cxLeftBorder;
    pRet->top    = pThis->m_cyTopBorder;
    pRet->right  = pThis->m_cxRightBorder;
    pRet->bottom = pThis->m_cyBottomBorder;
    return pRet;
}

// Symbol: ?GetPaneName@CPane@@UEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// This overload exists only in the Unicode image.  Retail mfc140u RVA 0xa30e0,
// transcribed in full (it is six instructions):
//   if (this != NULL && m_hWnd != NULL) CWnd::GetWindowTextW(strName);
// i.e. the string is left untouched when the pane has no window -- retail does
// NOT clear it on that path.
extern "C" void MS_ABI
impl__GetPaneName_CPane__UEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CPane* pThis, CString* strName) {
    if (strName == nullptr) return;
    if (pThis == nullptr || pThis->m_hWnd == nullptr) return;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        static_cast<const CWnd*>(pThis), strName);
}

// Symbol: ?GetVirtualRect@CPane@@QEBAXAEAVCRect@@@Z
// Retail mfc140 RVA 0xa1ca0, transcribed:
//   rectVirtual = m_rectVirtual;                             // 16-byte copy
//   CWnd::FromHandle(::GetParent(m_hWnd))->ClientToScreen(&rectVirtual);
// (::GetParent is import slot 0x1802c5300, CWnd::FromHandle is 0x289180 and the
// tail jump goes to ?ClientToScreen@CWnd@@QEBAXPEAUtagRECT@@@Z at 0x2a1250.)
// Retail does not null-check the FromHandle result; this does.
extern "C" void MS_ABI impl__GetVirtualRect_CPane__QEBAXAEAVCRect___Z(
    const CPane* pThis, RECT* pRectVirtual) {
    if (pThis == nullptr || pRectVirtual == nullptr) return;
    pRectVirtual->left   = pThis->m_rectVirtual.left;
    pRectVirtual->top    = pThis->m_rectVirtual.top;
    pRectVirtual->right  = pThis->m_rectVirtual.right;
    pRectVirtual->bottom = pThis->m_rectVirtual.bottom;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent != nullptr) {
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pParent, pRectVirtual);
    }
}

// Symbol: ?IsChangeState@CPane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa1710 is a ~270 instruction body that
// walks the owning CDockingPanesRow's pane list to decide between CS_NOTHING,
// CS_DOCK_IMMEDIATELY, CS_DELAY_DOCK and CS_DELAY_DOCK_TO_TAB.  OpenMFC models
// neither CDockingPanesRow's layout nor its pane list, so there is nothing to
// walk.
extern "C" int MS_ABI impl__IsChangeState_CPane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(
    const CPane* pThis, int nOffset, CBasePane** ppTargetBar) {
    (void)pThis;
    (void)nOffset;
    if (ppTargetBar != nullptr) *ppTargetBar = nullptr;
    return 0;  // CS_NOTHING
}

// Symbol: ?IsLeftOf@CPane@@QEBA_NVCRect@@_N@Z
// Retail mfc140 RVA 0xa1660, transcribed:
//   if (m_pParentDockBar == NULL) return true;                  // +0x128
//   CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//   if (!bWindowRect) m_pParentDockBar->ScreenToClient(&rectWnd);
//   return m_pParentDockBar->IsHorizontal() ? rect.left < rectWnd.left
//                                           : rect.top  < rectWnd.top;
// The `rect` argument is a by-value CRect, so MSVC passes its address in RDX.
// IsHorizontal is dispatched on m_pParentDockBar in retail; OpenMFC has no
// vtable for that object, so the CBasePane GetCurrentAlignment thunk is used
// instead -- a derived override of it would be missed.
extern "C" int MS_ABI impl__IsLeftOf_CPane__QEBA_NVCRect___N_Z(
    const CPane* pThis, const RECT* pRect, bool bWindowRect) {
    if (pThis == nullptr || pRect == nullptr) return 1;
    CDockBar* pParentDockBar = pThis->m_pParentDockBar;
    if (pParentDockBar == nullptr) return 1;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    const CWnd* pDockBarWnd = reinterpret_cast<const CWnd*>(pParentDockBar);
    if (!bWindowRect) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockBarWnd, &rectWnd);
    }
    const bool bHorz =
        (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(
             reinterpret_cast<const CBasePane*>(pParentDockBar)) & kCbrsOrientHorz) != 0;
    return bHorz ? (pRect->left < rectWnd.left) : (pRect->top < rectWnd.top);
}

// Symbol: ?IsTabbed@CPane@@UEBAHXZ
// Retail mfc140 RVA 0xa2100, transcribed:
//   CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//   if (pParent == NULL) return FALSE;
//   CWnd* pGrand  = CWnd::FromHandle(::GetParent(pParent->m_hWnd));
//   if (pGrand == NULL) return FALSE;
//   if (pParent->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl)) &&
//       pGrand ->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)))      return TRUE;
//   if (pParent->IsKindOf(RUNTIME_CLASS(CDockablePaneAdapter)) &&
//       pGrand ->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl)))      return TRUE;
//   return FALSE;
// The three CRuntimeClass descriptors were identified by reading the class name
// pointer out of each: 0x1802dba98 -> CMFCBaseTabCtrl, 0x1802db168 ->
// CBaseTabbedPane, 0x1803aa1a8 -> CDockablePaneAdapter (mfc140 addresses).
extern "C" int MS_ABI impl__IsTabbed_CPane__UEBAHXZ(const CPane* pThis) {
    if (pThis == nullptr) return FALSE;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent == nullptr) return FALSE;
    CWnd* pGrandParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParent->m_hWnd));
    if (pGrandParent == nullptr) return FALSE;
    const CObject* pParentObj = static_cast<const CObject*>(pParent);
    const CObject* pGrandObj = static_cast<const CObject*>(pGrandParent);
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ()) &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pGrandObj, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ())) {
        return TRUE;
    }
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentObj, impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ()) &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pGrandObj, impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?LoadState@CPane@@UEAAHPEB_WHI@Z
// NOT IMPLEMENTED.  Retail mfc140u RVA 0xa1ca0 (this overload exists only in
// the Unicode image; the ANSI twin has ?LoadState@CPane@@UEAAHPEBDHI@Z at
// mfc140 0xa2260 instead).  Transcribed as far as it was decoded: it formats a
// registry section name from "Panes" (0x18033f100) plus "%TsPane-%d"
// (0x18033fff8) or "%TsPane-%d%x" (0x180340010) depending on whether uiID is
// -1, opens a settings-store object through that object's own vtable, and on
// success reads eight NAMED values -- not one blob:
//   "ID"                   -> +0x220 m_nID
//   "RectRecentFloat"      -> +0x2c0 (inside m_recentDockInfo)
//   "RectRecentDocked"     -> +0x3d8 m_rectSavedDockedRect, then copied
//                             verbatim to +0x308 (inside m_recentDockInfo)
//   "RecentFrameAlignment" -> +0x2d0 (inside m_recentDockInfo)
//   "RecentRowIndex"       -> +0x2d8 (inside m_recentDockInfo)
//   "IsFloating"           -> +0x3e8 m_bRecentFloatingState
//   "MRUWidth"             -> +0x1dc m_nMRUWidth
//   "PinState"             -> +0x21c m_bPinState
// then tail-calls 0x18000c410 with the same four arguments.  Four of the eight
// destinations are inside CRecentDockSiteInfo, which OpenMFC models as an
// opaque 288-byte block with no members.  Left a stub.  (Correcting an earlier
// revision of this comment, which added "and the settings-store object it reads
// through is not modelled here at all": that is false.  OpenMFC does model the
// store -- phase4/src/core/app/CSettingsStore.cpp exports the whole Read/Write
// family and CSettingsStoreSP.cpp exports Create -- and CPane::SaveState below
// now writes these eight values through it.  What still blocks LoadState is
// that 0x18000c410, the CBasePane::LoadState it tail-calls, is a stub here, so
// the half of the state that lives in the base class would not come back.)
extern "C" int MS_ABI impl__LoadState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)nIndex;
    (void)uiID;
    return FALSE;
}

// Symbol: ?MoveByAlignment@CPane@@QEAAHKH@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa1000 does GetWindowRect ->
// parent ScreenToClient -> ::OffsetRect by nOffset along the axis chosen by
// dwAlignment -> UpdateVirtualRect(CPoint) -> CBasePane::SetWindowPos(&wndTop,
// ..., SWP_NOSIZE|SWP_NOACTIVATE|...).  The final SetWindowPos passes
// `CWnd::wndTop` (a static CWnd) as pWndInsertAfter; OpenMFC's
// SetWindowPos thunk in featurepack/docking/Thunks.cpp is a stub, and
// UpdateVirtualRect(CPoint) in this file is a stub too, so transcribing this
// would produce a body whose two effects both go nowhere.
extern "C" int MS_ABI impl__MoveByAlignment_CPane__QEAAHKH_Z(
    CPane* pThis, unsigned long dwAlignment, int nOffset) {
    (void)pThis;
    (void)dwAlignment;
    (void)nOffset;
    return FALSE;
}

// Symbol: ?MovePane@CPane@@UEAA?AVCSize@@VCRect@@HAEAPEAX@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa1140 moves the pane inside its
// CDockingPanesRow, redistributing the delta across the sibling panes in the
// row through the row object, then defers the window move with
// ::DeferWindowPos.  OpenMFC does not model CDockingPanesRow or its pane list.
extern "C" void* MS_ABI impl__MovePane_CPane__UEAA_AVCSize__VCRect__HAEAPEAX_Z(
    CPane* pThis, SIZE* pRet, const RECT* pRectNew, int bForceMove, void** phdwp) {
    (void)pThis;
    (void)pRectNew;
    (void)bForceMove;
    (void)phdwp;
    if (pRet != nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
    }
    return pRet;
}

// Symbol: ?OnAfterChangeParent@CPane@@UEAAXPEAVCWnd@@@Z
// Retail mfc140 RVA 0xa0fb0, transcribed:
//   UpdateVirtualRect();                                    // call 0xa1ae0
//   CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//   if (!pParent->IsKindOf(RUNTIME_CLASS(CDockSite)))       // rtc 0x1802e4258
//       { m_pParentDockBar = NULL; m_pDockBarRow = NULL; }  // +0x128, +0x130
// pWndOldParent is not read anywhere in the retail body.
extern "C" void MS_ABI impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(
    CPane* pThis, CWnd* pWndOldParent) {
    (void)pWndOldParent;
    if (pThis == nullptr) return;
    impl__UpdateVirtualRect_CPane__QEAAXXZ(pThis);
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParent),
            impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ())) {
        pThis->m_pParentDockBar = nullptr;
        pThis->m_pDockBarRow = nullptr;
    }
}

// Symbol: ?OnBeforeChangeParent@CPane@@UEAAXPEAVCWnd@@H@Z
// Retail mfc140 RVA 0xa0f80, transcribed in full (it is nine instructions):
//   if (m_pParentDockBar != NULL)                          // member +0x128
//       m_pParentDockBar->RemovePane(this, DM_UNKNOWN);    // its vtable +0x508
// Neither pWndNewParent nor bDelay is read.  The +0x508 slot is slot 161 of the
// CDockSite vftable (anchored on ?GetThisClass@CDockSite@@ at 0xa580), which
// holds 0x53e10 == ?RemovePane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z
// -- an ordinary export, so it is reachable after all.  R8D is zero at the call
// site, i.e. AFX_DOCK_METHOD DM_UNKNOWN (an earlier note in this file read that
// argument as FALSE; it is the dock method, not a flag).
// Calling the thunk loses dispatch to a CDockSite override, and that thunk is
// itself still a generated stub, so this currently has no run-time effect.
extern "C" void MS_ABI impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(
    CPane* pThis, CWnd* pWndNewParent, int bDelay) {
    (void)pWndNewParent;
    (void)bDelay;
    if (pThis == nullptr) return;
    if (pThis->m_pParentDockBar == nullptr) return;
    impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(
        pThis->m_pParentDockBar, pThis, kDockMethodUnknown);
}

// Symbol: ?OnBeforeDock@CPane@@MEAAHPEAPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
// Retail mfc140 RVA 0xa0900 is exactly:
//   CPaneFrameWnd* p = GetParentMiniFrame(FALSE);      // slot 140, EDX = 0
//   if (p != NULL) m_bPinState = *(int*)((char*)p + 0xfc);   // +0x21c
//   return TRUE;
// (ppDockBar, lpRect and dockMethod are not read).  The +0xfc read is against
// the retail CPaneFrameWnd layout, which OpenMFC does not reproduce, so only
// the unconditional TRUE is faithful here -- the m_bPinState copy is omitted.
extern "C" int MS_ABI impl__OnBeforeDock_CPane__MEAAHPEAPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane** ppDockBar, const RECT* lpRect, int dockMethod) {
    (void)pThis;
    (void)ppDockBar;
    (void)lpRect;
    (void)dockMethod;
    return TRUE;
}

// Symbol: ?OnCancelMode@CPane@@IEAAXXZ
// Retail mfc140 RVA 0xa2930, transcribed:
//   Default();                                              // 0x289090
//   if (m_bCaptured) {                                      // +0x210
//       if (GetDockingMode() & DT_STANDARD)                 // slot 112, bit 1
//           m_dragFrameImpl.EndDrawDragFrame(TRUE);         // 0x55cb0, +0x238
//       ::ReleaseCapture();                                 // import 0x1802c5280
//       m_bCaptured = FALSE;
//       SetDragMode(FALSE);                                 // slot 197
//       if (m_hwndMiniFrameToBeClosed != NULL &&            // +0x228
//           ::IsWindow(m_hwndMiniFrameToBeClosed))          // import 0x1802c5390
//           ::DestroyWindow(m_hwndMiniFrameToBeClosed);     // import 0x1802c4da8
//       m_hwndMiniFrameToBeClosed = NULL;
//   }
// Note the ordering retail actually uses: m_bCaptured is cleared BEFORE the
// SetDragMode(FALSE) dispatch, and m_hwndMiniFrameToBeClosed is nulled on both
// paths out of the inner `if`, not only after a successful DestroyWindow.
extern "C" void MS_ABI impl__OnCancelMode_CPane__IEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (!pThis->m_bCaptured) return;
    if (impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(
            static_cast<const CBasePane*>(pThis)) & kDockTypeStandard) {
        impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(&pThis->m_dragFrameImpl, TRUE);
    }
    ::ReleaseCapture();
    pThis->m_bCaptured = FALSE;
    impl__SetDragMode_CPane__MEAAXH_Z(pThis, FALSE);
    if (pThis->m_hwndMiniFrameToBeClosed != nullptr &&
        ::IsWindow(pThis->m_hwndMiniFrameToBeClosed)) {
        ::DestroyWindow(pThis->m_hwndMiniFrameToBeClosed);
    }
    pThis->m_hwndMiniFrameToBeClosed = nullptr;
}

// Symbol: ?OnChar@CPane@@IEAAXIII@Z
// Retail mfc140 RVA 0xa29d0, transcribed in full (it is seven instructions):
//   if (nChar == VK_ESCAPE) OnCancelMode();     // direct call to 0xa2930
//   Default();                                  // tail jump to 0x289090
// nRepCnt and nFlags are not read.
extern "C" void MS_ABI impl__OnChar_CPane__IEAAXIII_Z(
    CPane* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt;
    (void)nFlags;
    if (pThis == nullptr) return;
    if (nChar == VK_ESCAPE) impl__OnCancelMode_CPane__IEAAXXZ(pThis);
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// Symbol: ?OnContextMenu@CPane@@IEAAXPEAVCWnd@@VCPoint@@@Z
// Retail mfc140 RVA 0xa21b0, transcribed in full:
//   if (CMFCPopupMenu::m_pActivePopupMenu != NULL) return;   // 0x3b6fe8
//   if (CMFCToolBar::m_bCustomizeMode != 0)        return;   // 0x3b70bc
//   if (OnShowControlBarMenu(point))               return;   // vtable slot 187
//   CFrameWnd* pFrame = m_pDockSite;                         // member +0x110
//   if (pFrame == NULL || !pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) {
//       pFrame = g_pTopLevelFrame;                           // 0x3b6f18
//       if (pFrame == NULL) {
//           pFrame = GetTopLevelFrame();                     // 0x28c910
//           if (pFrame == NULL) return;
//       }
//   }
//   OnPaneContextMenu(pFrame, point);                        // vtable slot 148
// The three .data addresses were resolved through mfc140_rva_symbols.json --
// 0x3b6fe8 is ?m_pActivePopupMenu@CMFCPopupMenu@@1PEAV1@EA, 0x3b70bc is
// ?m_bCustomizeMode@CMFCToolBar@@1HA and 0x3b6f18 is
// ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA -- and OpenMFC defines all three, so
// they are read here rather than assumed.  0x180338990 is the CFrameWnd
// CRuntimeClass (its name pointer spells "CFrameWnd"), and CPane vtable slot
// 148 (+0x4a0) is 0xcf50 ==
// ?OnPaneContextMenu@CBasePane@@UEAAXPEAVCWnd@@VCPoint@@@Z.
// The `pWnd` parameter of the message handler is never read by retail.
// Both OnShowControlBarMenu (below) and the OnPaneContextMenu thunk are still
// stubs, so today this reliably reaches the final call and that call does
// nothing; the routing itself is what is transcribed here.
extern "C" void MS_ABI impl__OnContextMenu_CPane__IEAAXPEAVCWnd__VCPoint___Z(
    CPane* pThis, CWnd* pWnd, void* point) {
    (void)pWnd;
    if (pThis == nullptr) return;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA != nullptr) return;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (impl__OnShowControlBarMenu_CPane__UEAAHVCPoint___Z(pThis, point)) return;
    CWnd* pFrame = reinterpret_cast<CWnd*>(pThis->m_pDockSite);
    if (pFrame == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pFrame),
            impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        pFrame = reinterpret_cast<CWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
        if (pFrame == nullptr) {
            pFrame = reinterpret_cast<CWnd*>(
                impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis)));
            if (pFrame == nullptr) return;
        }
    }
    impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(
        static_cast<CBasePane*>(pThis), pFrame, point);
}

// Symbol: ?OnContinueMoving@CPane@@MEAAXXZ
// Retail mfc140 RVA 0xa09d0 is four instructions: an unconditional tail jump to
// vtable slot 199 with EDX = 0, i.e. `EnterDragMode(FALSE);`.
extern "C" void MS_ABI impl__OnContinueMoving_CPane__MEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    impl__EnterDragMode_CPane__MEAAXH_Z(pThis, FALSE);
}

// Symbol: ?OnDestroy@CPane@@IEAAXXZ
// Retail mfc140 RVA 0xa1d40, transcribed:
//   if (IsTabbed()) {                                       // vtable slot 93
//       CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//       if (p->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl)))    // rtc 0x1802dba98
//           p = CWnd::FromHandle(::GetParent(p->m_hWnd));
//       if (p->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) {  // rtc 0x1802db168
//           HWND hWndSave = m_hWnd;
//           ((CBaseTabbedPane*)p)->RemovePane(this);        // its vtable +0x790
//           if (!::IsWindow(hWndSave)) return;   // skips the base call entirely
//       }
//   }
//   CWnd::OnDestroy();                                      // 0x289cc0
// The +0x790 dispatch is now named: anchoring the CBaseTabbedPane vftable on
// ?GetThisClass@CBaseTabbedPane@@ (0x12f40) puts its base at mfc140 RVA
// 0x2db2a8, and slot 242 (+0x790) there holds 0x13710 ==
// ?RemovePane@CBaseTabbedPane@@UEAAHPEAVCWnd@@@Z, an ordinary export.  Only RDX
// is set at the call site (R8/R9 are untouched), which matches that
// one-argument signature.
// Retail follows the second IsKindOf with the usual MFC checked-downcast pair
// (a NULL test and a repeat of the same IsKindOf, both branching to the throw
// helper at 0x225b80); with the first test already passed both are dead, so
// they are not reproduced.
// The two RUNTIME_CLASS descriptors are reached through GetThisClass thunks
// here, the same way CPane::IsTabbed above does it.
extern "C" void MS_ABI impl__OnDestroy_CPane__IEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    if (impl__IsTabbed_CPane__UEBAHXZ(pThis)) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
        // Deviation: retail calls IsKindOf on the FromHandle result unchecked.
        if (pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
            pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParent->m_hWnd));
        }
        if (pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ())) {
            const HWND hWndSave = pThis->m_hWnd;
            impl__RemovePane_CBaseTabbedPane__UEAAHPEAVCWnd___Z(
                pParent, static_cast<CWnd*>(pThis));
            if (!::IsWindow(hWndSave)) return;
        }
    }
    impl__OnDestroy_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
}

// Symbol: ?OnLButtonDblClk@CPane@@IEAAXIVCPoint@@@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa2010 is:
//   OnProcessDblClk();                                      // slot 177
//   if (CanFloat()) {                                       // slot 117
//       CRect rect = *(CRect*)(this + 0x2c0);   // m_recentDockInfo + 8
//       FloatPane(rect, DM_DBL_CLICK, true);                // slot 129
//       Default();
//   }
// The rectangle comes from inside CRecentDockSiteInfo, which OpenMFC models as
// an opaque 288-byte block with no members, and FloatPane is itself unavailable
// (see above), so both halves of the `if` body are unreachable here.
extern "C" void MS_ABI impl__OnLButtonDblClk_CPane__IEAAXIVCPoint___Z(
    CPane* pThis, unsigned int nFlags, void* point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// Symbol: ?OnLButtonUp@CPane@@IEAAXIVCPoint@@@Z
// Retail mfc140 RVA 0xa0040, transcribed:
//   CPaneFrameWnd* pMiniFrame = GetParentMiniFrame(FALSE);   // slot 140, run
//                                                            // unconditionally
//   if (m_bCaptured) {                                       // byte at +0x210
//       ::ReleaseCapture();                                  // USER32 import
//       m_bCaptured = FALSE;
//       if (nFlags != 0xFFFF) {
//           if (m_hwndMiniFrameToBeClosed != NULL &&         // +0x228
//               ::IsWindow(m_hwndMiniFrameToBeClosed))
//               ::DestroyWindow(m_hwndMiniFrameToBeClosed);
//           m_hwndMiniFrameToBeClosed = NULL;
//       }
//       SetDragMode(FALSE);                                  // slot 197
//       CDockingManager* pDM = afxGlobalUtils.GetDockingManager(   // 0x6ccc0
//           CWnd::FromHandle(::GetParent(m_hWnd)));          // 0x289180
//       if (pDM != NULL) {
//           void* p = *(void**)((char*)pDM + 0x308);   // a CSmartDockingManager*
//           if (p != NULL) CSmartDockingManager::Stop(p);     // 0x132940
//       }   // (the +0x308 field's NAME was not established; its type follows
//           //  from the callee)
//       if (GetDockingMode() & DT_STANDARD) {                // slot 112
//           if (!m_dragFrameImpl.m_bDragStarted &&
//               m_dragFrameImpl.m_nInsertedTabID < 0) goto default_path;
//           CRect rect = m_dragFrameImpl.m_rectDrag;
//           if (m_dragFrameImpl.m_bDragStarted &&
//               (GetDockingMode() & DT_STANDARD))
//               m_dragFrameImpl.EndDrawDragFrame(TRUE);      // 0x55cb0
//           BOOL bWasDocked = FALSE;
//           StoreRecentDockSiteInfo();                       // slot 201
//           CPane* pDocked = DockPaneStandard(bWasDocked);       // slot 174
//           if (!bWasDocked && !::IsRectEmpty(&rect) && pDocked != this)
//               FloatPane(rect, DM_STANDARD /*5*/, true);    // slot 129
//           return;                                          // no Default()
//       }
//       goto default_path;
//   }
//   if (pMiniFrame == NULL || m_bDblClick /*+0x212*/ ||
//       !::IsWindowVisible(pMiniFrame->m_hWnd)) goto default_path;
//   ::MapWindowPoints(m_hWnd, pMiniFrame->m_hWnd, (LPPOINT)&point, 1);
//   ::SendMessage(pMiniFrame->m_hWnd, WM_LBUTTONUP, nFlags,
//                 MAKELPARAM(point.x, point.y));
//   return;                                                  // no Default()
// default_path:
//   m_bDblClick = FALSE;
//   if (m_pDockBarRow != NULL)                               // +0x130
//       m_pDockBarRow->FixupVirtualRects(FALSE, NULL);       // 0x51170
//   Default();                                               // 0x289090
// Every USER32 name above was resolved through the import table
// (ReleaseCapture 0x1802c5280, IsWindow 0x1802c5390, DestroyWindow 0x1802c4da8,
// GetParent 0x1802c5300, IsWindowVisible 0x1802c5350, MapWindowPoints
// 0x1802c5250, SendMessageA 0x1802c5378 -- SendMessageW in mfc140u --
// IsRectEmpty 0x1802c52c8).
//
// Two deliberate deviations, both because OpenMFC does not model the object
// involved:
//  * the CSmartDockingManager stop is DROPPED.  CDockingManager is modelled in
//    include/openmfc/afxmfc.h as a ~144-byte object with no member at +0x308,
//    so the pointer retail loads there does not exist; reading that offset
//    would be an out-of-bounds read of a foreign object.  The whole
//    GetDockingManager call is therefore skipped too, since its only use here
//    is to reach that pointer.
//  * DockPaneStandard is the inlined CPane body (see PaneBaseDockPaneStandard
//    above) rather than a virtual dispatch.  Because that body returns `this`,
//    the `pDocked != this` test below is always false and the FloatPane call
//    cannot be reached for a plain CPane; it is written out anyway so the
//    control flow matches retail once a real override exists.
// As elsewhere in this file the virtuals retail dispatches through the CPane
// vtable are reached through the exported base implementation, which loses
// dispatch to a derived override.
extern "C" void MS_ABI impl__OnLButtonUp_CPane__IEAAXIVCPoint___Z(
    CPane* pThis, unsigned int nFlags, void* point) {
    if (pThis == nullptr) return;

    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);

    bool bDefault = false;
    if (pThis->m_bCaptured) {
        ::ReleaseCapture();
        pThis->m_bCaptured = FALSE;
        if (nFlags != 0xFFFFu) {
            if (pThis->m_hwndMiniFrameToBeClosed != nullptr &&
                ::IsWindow(pThis->m_hwndMiniFrameToBeClosed)) {
                ::DestroyWindow(pThis->m_hwndMiniFrameToBeClosed);
            }
            pThis->m_hwndMiniFrameToBeClosed = nullptr;
        }
        impl__SetDragMode_CPane__MEAAXH_Z(pThis, FALSE);
        // (retail stops smart docking here -- see the note above)
        if ((impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(
                 static_cast<const CBasePane*>(pThis)) & kDockTypeStandard) == 0) {
            bDefault = true;
        } else {
            char* pDrag = reinterpret_cast<char*>(&pThis->m_dragFrameImpl);
            const int bDragStarted =
                *reinterpret_cast<const int*>(pDrag + kDragFrameDragStarted);
            const int nInsertedTabID =
                *reinterpret_cast<const int*>(pDrag + kDragFrameInsertedTab);
            if (bDragStarted == 0 && nInsertedTabID < 0) {
                bDefault = true;
            } else {
                RECT rectDrag = *reinterpret_cast<const RECT*>(pDrag + kDragFrameRectDrag);
                if (bDragStarted != 0 &&
                    (impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(
                         static_cast<const CBasePane*>(pThis)) & kDockTypeStandard) != 0) {
                    impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(pDrag, TRUE);
                }
                int bWasDocked = FALSE;
                impl__StoreRecentDockSiteInfo_CPane__MEAAXXZ(pThis);
                CPane* pDocked = PaneBaseDockPaneStandard(pThis, bWasDocked);
                if (bWasDocked == 0 && !::IsRectEmpty(&rectDrag) && pDocked != pThis) {
                    impl__FloatPane_CPane__UEAAHVCRect__W4AFX_DOCK_METHOD___N_Z(
                        pThis, &rectDrag, kDockMethodStandard, true);
                }
            }
        }
    } else if (pMiniFrame == nullptr || pThis->m_bDblClick ||
               !::IsWindowVisible(reinterpret_cast<CWnd*>(pMiniFrame)->m_hWnd)) {
        bDefault = true;
    } else {
        const HWND hWndMini = reinterpret_cast<CWnd*>(pMiniFrame)->m_hWnd;
        POINT pt = UnpackPoint(point);
        ::MapWindowPoints(pThis->m_hWnd, hWndMini, &pt, 1);
        ::SendMessage(hWndMini, WM_LBUTTONUP, static_cast<WPARAM>(nFlags),
                      MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)));
    }

    if (bDefault) {
        pThis->m_bDblClick = FALSE;
        if (pThis->m_pDockBarRow != nullptr) {
            impl__FixupVirtualRects_CDockingPanesRow__QEAAX_NPEAVCPane___Z(
                static_cast<void*>(pThis->m_pDockBarRow), false, nullptr);
        }
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// Symbol: ?OnMouseMove@CPane@@IEAAXIVCPoint@@@Z
// Retail mfc140 RVA 0xa02a0, transcribed:
//   if (!m_bCaptured) { Default(); return; }              // +0x210, 0x289090
//   DWORD dt = GetDockingMode();                          // vtable slot 112
//   if (dt & DT_IMMEDIATE) {
//       CPoint ptCursor(0,0); ::GetCursorPos(&ptCursor);  // import 0x1802c5348
//       CPoint ptOffset(ptCursor.x - m_dragFrameImpl.m_ptHot.x,
//                       ptCursor.y - m_dragFrameImpl.m_ptHot.y);   // +0x240
//       ::OffsetRect(&m_rectDragImmediate,                // +0x1f0,
//                    ptOffset.x, ptOffset.y);             // import 0x1802c5318
//       UpdateVirtualRect(ptOffset);                      // direct call 0xa1ba0
//       if (m_pParentDockBar != NULL) {                   // +0x128
//           m_pParentDockBar->MovePane(this, nFlags, ptOffset);  // vtable +0x510
//           ::RedrawWindow(m_hWnd, NULL, NULL,
//                          RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW /*0x105*/);
//       }
//       m_dragFrameImpl.m_ptHot = ptCursor;   // runs whether or not
//                                             // m_pParentDockBar was NULL
//   } else if (dt & DT_STANDARD) {
//       m_dragFrameImpl.MoveDragFrame(FALSE);   // 0x55450 ==
//                     // ?MoveDragFrame@CMFCDragFrameImpl@@QEAAXH@Z, EDX = 0
//   }
// Note what retail does NOT do: Default() runs only on the !m_bCaptured path,
// and neither docking-mode branch falls through to it.
// The CDockSite +0x510 slot was identified by anchoring the CDockSite vftable
// on ?GetThisClass@CDockSite@@ (0xa580); slot 162 there is 0x53370 ==
// ?MovePane@CDockSite@@UEAAHPEAVCPane@@IVCPoint@@@Z, matching the (pane,
// nFlags, packed CPoint) register set at the call site.  Import slot 0x1802c5388
// resolved to USER32!RedrawWindow with iat.py; 0x105 is
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW.
// m_dragFrameImpl is an opaque 128-byte block in OpenMFC, but the hot point is
// at a fixed offset inside it: CMFCDragFrameImpl has a virtual destructor
// (atlmfc/include/afxdragframeimpl.h:34) and m_ptHot is its first data member
// (line 44), so the vptr occupies +0 and m_ptHot sits at +8 -- which is the
// +0x240 retail reads, m_dragFrameImpl being at +0x238.  CPane::EnterDragMode
// above already writes that same byte offset.
extern "C" void MS_ABI impl__OnMouseMove_CPane__IEAAXIVCPoint___Z(
    CPane* pThis, unsigned int nFlags, void* point) {
    (void)point;   // retail does not read the message point
    if (pThis == nullptr) return;
    if (!pThis->m_bCaptured) {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
        return;
    }
    const int dockingMode = impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(
        static_cast<const CBasePane*>(pThis));
    POINT* pHot = reinterpret_cast<POINT*>(
        reinterpret_cast<char*>(&pThis->m_dragFrameImpl) + 8);
    if (dockingMode & kDockTypeImmediate) {
        POINT ptCursor = {0, 0};
        ::GetCursorPos(&ptCursor);
        const POINT ptOffset = {ptCursor.x - pHot->x, ptCursor.y - pHot->y};
        ::OffsetRect(reinterpret_cast<RECT*>(&pThis->m_rectDragImmediate),
                     ptOffset.x, ptOffset.y);
        impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(pThis, PackPoint(ptOffset.x, ptOffset.y));
        if (pThis->m_pParentDockBar != nullptr) {
            impl__MovePane_CDockSite__UEAAHPEAVCPane__IVCPoint___Z(
                pThis->m_pParentDockBar, pThis, nFlags, PackPoint(ptOffset.x, ptOffset.y));
            ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr,
                           RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        }
        *pHot = ptCursor;
    } else if (dockingMode & kDockTypeStandard) {
        impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(&pThis->m_dragFrameImpl, FALSE);
    }
}

// Symbol: ?OnNcDestroy@CPane@@IEAAXXZ
// Retail mfc140 RVA 0xa1e20, transcribed in full:
//   CPaneFrameWnd::AddRemovePaneFromGlobalList(this, FALSE);   // 0xaf740
//   CPaneFrameWnd* p = GetParentMiniFrame(TRUE);               // slot 140
//   if (p != NULL) p->RemovePane(this, FALSE, FALSE);          // +0x308
//   CWnd::OnNcDestroy();                                       // tail 0x289d80
extern "C" void MS_ABI impl__OnNcDestroy_CPane__IEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
        static_cast<CBasePane*>(pThis), FALSE);
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), TRUE);
    if (pMiniFrame != nullptr) {
        impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(
            pMiniFrame, static_cast<CBasePane*>(pThis), FALSE, FALSE);
    }
    impl__OnNcDestroy_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
}

// Symbol: ?OnPressCloseButton@CPane@@UEAAXXZ
// Retail mfc140 RVA 0xa35d0 is thirteen instructions:
//   CPaneFrameWnd* p = GetParentMiniFrame(FALSE);   // slot 140, EDX = 0
//   if (p != NULL) p->CloseMiniFrame();             // its vtable +0x458
extern "C" void MS_ABI impl__OnPressCloseButton_CPane__UEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    if (pMiniFrame != nullptr) {
        impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(pMiniFrame);
    }
}

// Symbol: ?OnProcessDblClk@CPane@@UEAAXXZ
// Retail mfc140 RVA 0xa2080, transcribed:
//   m_bDblClick = TRUE;                                     // +0x212
//   StoreRecentDockSiteInfo();                              // slot 201
//   if (m_bCaptured) {                                      // +0x210
//       ::ReleaseCapture();
//       m_bCaptured = FALSE;
//       SetDragMode(FALSE);                                 // slot 197
//       if (m_hwndMiniFrameToBeClosed != NULL &&
//           ::IsWindow(m_hwndMiniFrameToBeClosed))
//           ::DestroyWindow(m_hwndMiniFrameToBeClosed);
//       m_hwndMiniFrameToBeClosed = NULL;
//   }
// Unlike OnCancelMode this path does NOT call Default() and does NOT test the
// docking mode / end the drag frame.
extern "C" void MS_ABI impl__OnProcessDblClk_CPane__UEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    pThis->m_bDblClick = TRUE;
    impl__StoreRecentDockSiteInfo_CPane__MEAAXXZ(pThis);
    if (!pThis->m_bCaptured) return;
    ::ReleaseCapture();
    pThis->m_bCaptured = FALSE;
    impl__SetDragMode_CPane__MEAAXH_Z(pThis, FALSE);
    if (pThis->m_hwndMiniFrameToBeClosed != nullptr &&
        ::IsWindow(pThis->m_hwndMiniFrameToBeClosed)) {
        ::DestroyWindow(pThis->m_hwndMiniFrameToBeClosed);
    }
    pThis->m_hwndMiniFrameToBeClosed = nullptr;
}

// Symbol: ?OnRTLChanged@CPane@@MEAAXH@Z
// Retail mfc140 RVA 0xa2c30, transcribed:
//   <lazily run AFX_GLOBAL_DATA::Initialize (0x6a5c0) once, then store bIsRTL
//    into a field of that process-global object>
//   if (m_pParentDockBar != NULL) {                     // slot 106 -> +0x128
//       if (IsHorizontal()) {                           // slot 91
//           SetWindowPos(NULL, m_rectVirtual.left, m_rectVirtual.top,
//                        m_rectVirtual.Width(), m_rectVirtual.Height(),
//                        SWP_NOZORDER, NULL);           // slot 144
//       }
//   }
// The AFX_GLOBAL_DATA store is NOT reproduced: OpenMFC has no AFX_GLOBAL_DATA
// object, so there is nowhere to put the flag.  The layout half is transcribed.
extern "C" void MS_ABI impl__OnRTLChanged_CPane__MEAAXH_Z(CPane* pThis, int bIsRTL) {
    (void)bIsRTL;
    if (pThis == nullptr) return;
    if (pThis->m_pParentDockBar == nullptr) return;
    if (!PaneIsHorizontal(pThis)) return;
    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pThis), nullptr,
        pThis->m_rectVirtual.left, pThis->m_rectVirtual.top,
        pThis->m_rectVirtual.right - pThis->m_rectVirtual.left,
        pThis->m_rectVirtual.bottom - pThis->m_rectVirtual.top,
        SWP_NOZORDER, nullptr);
}

// Symbol: ?OnShowControlBarMenu@CPane@@UEAAHVCPoint@@@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa2cf0 is a ~430 instruction body.
// Correcting an earlier revision of this comment, which said the menu is built
// "from the frame's docking manager pane list": it is not.  What the body
// actually does, as far as it was decoded:
//   if (<process-global context-menu manager pointer at mfc140 .data
//        0x1803b6f10> == NULL) return FALSE;
//   if ((GetEnabledAlignment() & CBRS_ALIGN_ANY) == 0 && !CanFloat())
//       return FALSE;                       // slots 104 (0x87d0, `return
//                                           // m_dwEnabledAlignment;`) and 117
//   CMenu menu; menu.Attach(::CreatePopupMenu());          // 0x2a6020
//   // five FIXED items, each built as
//   //   HINSTANCE h = AfxFindStringResourceHandle(nIDS);  // 0x2accf0
//   //   str.LoadString(h, nIDS);                          // 0xdc00
//   //   ::AppendMenu(hMenu, MF_STRING, nCmd, str);        // import 0x1802c4d38
//   //   nIDS 0x428F -> nCmd -102   0x4290 -> -103   0x42C0 -> -106
//   //   nIDS 0x4291 -> -104        0x4292 -> -105
//   // then ::EnableMenuItem / ::CheckMenuItem (imports 0x1802c4d58 /
//   // 0x1802c4e30) on those five commands, driven by the pane's own state:
//   // GetParentMiniFrame, CanFloat, IsMDITabbed, CanBeTabbedDocument,
//   // GetDockSiteFrameWnd, IsDocked, IsTabbed, CanAutoHide (slot 119 ==
//   // 0x8880, `m_dwControlBarStyle & 2`), CanBeClosed (slot 116 == 0x8860,
//   // `m_dwControlBarStyle & 8`) and GetEnabledAlignment.
//   if (!<slot 188>(menu)) { destroy the menu; return ...; }   // a hook the
//                                           // menu must pass to be shown
//   int nCmd = <context menu manager>->vtable+0x38(hMenu, point.x, point.y,
//                                                  this, FALSE);
//   if (::IsWindow(hWndSave) && <slot 189>(nCmd)) { ...destroy menu... }
//   else switch (nCmd) { case -102: ... case -106: ... }   // float / dock the
//                                           // parent mini frame / tabbed
//                                           // document / auto-hide / hide,
//                                           // each through further virtuals
//   ::DestroyMenu(...)                                     // import 0x1802c4c58
// It is left a stub because two pieces it needs are not modelled here.  First,
// the pointer it tests and tracks through is a process-global that retail does
// not even export (mfc140 .data 0x1803b6f10 sits unnamed between
// ?m_nStartCount@CMFCCmdUsageCount@@1IA at 0x3b6f0c and ?g_pTopLevelFrame@@...
// at 0x3b6f18); phase4/src/featurepack/menu/CContextMenuManager.cpp models the
// class but nothing in OpenMFC publishes such a pointer or the TrackPopupMenu
// its vtable slot +0x38 is.  Second, every branch of the command dispatch ends
// in a docking operation -- FloatPane, the mini frame's dock, the
// tabbed-document switch -- that is itself unavailable here, so a partial body
// would pop up a menu whose commands do nothing.
// (An earlier revision of this comment also listed CMenu::Attach and
// CString::LoadString as blockers.  That is false and has been removed: both
// have working thunks in this tree -- impl__Attach_CMenu__QEAAHPEAUHMENU_____Z
// at phase4/src/core/window/CMenu.cpp:32 and
// impl__LoadStringW___CStringT...QEAAHPEAUHINSTANCE____I_Z at
// phase4/src/core/collections/CStringT.cpp:283 -- and calling a sibling through
// its thunk is exactly how the implemented bodies in this file work.)
extern "C" int MS_ABI impl__OnShowControlBarMenu_CPane__UEAAHVCPoint___Z(CPane* pThis, void* point) {
    (void)pThis;
    (void)point;
    return FALSE;
}

// Symbol: ?OnStyleChanged@CPane@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
// Retail mfc140 RVA 0xa2bc0, transcribed:
//   Default();                                              // 0x289090
//   if (nStyleType == GWL_EXSTYLE /* -20 */) {
//       DWORD dwNew = lpStyleStruct->styleNew & WS_EX_LAYOUTRTL;
//       BOOL  bOld  = (lpStyleStruct->styleOld & WS_EX_LAYOUTRTL) != 0;
//       if (bOld ? (dwNew == 0) : (dwNew != 0))             // i.e. it changed
//           OnRTLChanged((lpStyleStruct->styleNew >> 22) & 1);   // slot 200
//   }
// The shift-by-22 is just the WS_EX_LAYOUTRTL bit normalised to 0/1.
extern "C" void MS_ABI impl__OnStyleChanged_CPane__IEAAXHPEAUtagSTYLESTRUCT___Z(
    CPane* pThis, int nStyleType, STYLESTRUCT* lpStyleStruct) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (nStyleType != GWL_EXSTYLE || lpStyleStruct == nullptr) return;
    const bool bNewRtl = (lpStyleStruct->styleNew & kExLayoutRtl) != 0;
    const bool bOldRtl = (lpStyleStruct->styleOld & kExLayoutRtl) != 0;
    if (bNewRtl == bOldRtl) return;
    impl__OnRTLChanged_CPane__MEAAXH_Z(pThis, bNewRtl ? 1 : 0);
}

// Symbol: ?PrepareToDock@CPane@@UEAAXPEAVCDockSite@@W4AFX_DOCK_METHOD@@@Z
// Retail mfc140 RVA 0xa0710, transcribed:
//   if (pDockBar == NULL) return;
//   m_pParentDockBar = pDockBar->IsKindOf(RUNTIME_CLASS(CDockSite))
//                          ? pDockBar : NULL;                  // +0x128
//   RemoveFromMiniframe(pDockBar, dockMethod);                 // vtable slot 196
//   DWORD s = GetPaneStyle();                                  // vtable slot 114
//   SetPaneStyle(((((s ^ m_dwStyle) & CBRS_ALIGN_ANY) ^ s)
//                 & ~CBRS_FLOATING) | CBRS_BORDER_ANY);        // vtable slot 123
//   SetPaneAlignment(pDockBar->GetCurrentAlignment());         // slot 122 / 103
// The conditional store is the `neg %eax; sbb %r9,%r9; and %rdi,%r9` idiom at
// 0xa0740, i.e. a mask of pDockBar by 0 or ~0; 0x1802e4258 is the CDockSite
// CRuntimeClass (its name pointer spells "CDockSite").
// The three CPane style accessors are unexported inlines and are reproduced
// directly against m_dwStyle (+0x104), which is exactly what their targets do:
// 0x8840 GetPaneStyle `return m_dwStyle;`, 0x88b0 SetPaneStyle
// `m_dwStyle = arg;`, 0x8890 SetPaneAlignment
// `m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | arg;`.  GetCurrentAlignment is
// dispatched on pDockBar in retail (its vtable slot 103); the CBasePane thunk
// is used here instead, so a CDockSite override of it is missed.
// Because GetPaneStyle is that inline, `s` equals m_dwStyle and the
// `(s ^ m_dwStyle) & CBRS_ALIGN_ANY` term is zero -- the expression is written
// out in full anyway so it matches the retail text.
extern "C" void MS_ABI impl__PrepareToDock_CPane__UEAAXPEAVCDockSite__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, void* pDockBar, int dockMethod) {
    if (pThis == nullptr || pDockBar == nullptr) return;
    CObject* pDockBarObj = reinterpret_cast<CObject*>(pDockBar);
    const bool bIsDockSite = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        pDockBarObj, impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ()) != 0;
    pThis->m_pParentDockBar = bIsDockSite ? reinterpret_cast<CDockBar*>(pDockBar) : nullptr;
    impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
        pThis, reinterpret_cast<CWnd*>(pDockBar), dockMethod);
    const unsigned long dwStyle = pThis->m_dwStyle;   // GetPaneStyle()
    pThis->m_dwStyle = ((((dwStyle ^ pThis->m_dwStyle) & kCbrsAlignAny) ^ dwStyle) &
                        ~kCbrsFloating) | kCbrsBorderAny;
    const unsigned long dwAlignment = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(
        reinterpret_cast<const CBasePane*>(pDockBar));
    pThis->m_dwStyle = (pThis->m_dwStyle & ~kCbrsAlignAny) | dwAlignment;
}

// Symbol: ?RemoveFromMiniframe@CPane@@MEAAXPEAVCWnd@@W4AFX_DOCK_METHOD@@@Z
// Retail mfc140 RVA 0xa07f0, transcribed:
//   CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);      // vtable slot 140
//   CWnd* pOldParent = CWnd::FromHandle(::GetParent(m_hWnd));
//   OnBeforeChangeParent(pNewParent, FALSE);               // vtable slot 137
//   if (pMini != NULL) {
//       BOOL bDelay = (dockMethod == DM_MOUSE &&
//                      m_hwndMiniFrameToBeClosed == NULL); // +0x228
//       pMini->RemovePane(this, FALSE, bDelay);            // its vtable +0x308
//       if (dockMethod == DM_MOUSE) {
//           if (m_hwndMiniFrameToBeClosed == NULL)
//               m_hwndMiniFrameToBeClosed = pMini->m_hWnd;
//           ::SendMessage(pMini->m_hWnd, WM_LBUTTONUP, 0, 0);
//       }
//   }
//   if (pNewParent != NULL)
//       CWnd::FromHandle(::SetParent(m_hWnd, pNewParent->m_hWnd));
//   OnAfterChangeParent(pOldParent);                       // tail, slot 138
// Slot 137 (+0x448) is 0xa0f80 == CPane::OnBeforeChangeParent and slot 138
// (+0x450) is 0xa0fb0 == CPane::OnAfterChangeParent, both read straight out of
// the CPane vftable; both are defined above in this file, so they are called
// directly rather than through a vtable.  ::SetParent is import slot
// 0x1802c52b0 and the message send is import slot 0x1802c5378, which resolves
// to USER32!SendMessageA in this ANSI image -- i.e. SendMessageW in mfc140u --
// so ::SendMessage is written here and UNICODE picks.  The FromHandle wrapping
// of the ::SetParent result is retail's way of interning the old parent HWND;
// its return value is discarded.
extern "C" void MS_ABI impl__RemoveFromMiniframe_CPane__MEAAXPEAVCWnd__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CWnd* pNewParent, int dockMethod) {
    if (pThis == nullptr) return;
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
        static_cast<const CBasePane*>(pThis), FALSE);
    CWnd* pOldParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    impl__OnBeforeChangeParent_CPane__UEAAXPEAVCWnd__H_Z(pThis, pNewParent, FALSE);
    if (pMiniFrame != nullptr) {
        // Retail writes DM_MOUSE's own register here (`mov %r15d,%r9d`), which
        // on that path is 1; TRUE is the same value.
        const int bDelay = (dockMethod == kDockMethodMouse &&
                            pThis->m_hwndMiniFrameToBeClosed == nullptr) ? TRUE : FALSE;
        impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(
            pMiniFrame, static_cast<CBasePane*>(pThis), FALSE, bDelay);
        if (dockMethod == kDockMethodMouse) {
            const HWND hWndMini = reinterpret_cast<CWnd*>(pMiniFrame)->m_hWnd;
            if (pThis->m_hwndMiniFrameToBeClosed == nullptr) {
                pThis->m_hwndMiniFrameToBeClosed = hWndMini;
            }
            ::SendMessage(hWndMini, WM_LBUTTONUP, 0, 0);
        }
    }
    if (pNewParent != nullptr) {
        (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::SetParent(pThis->m_hWnd, pNewParent->m_hWnd));
    }
    impl__OnAfterChangeParent_CPane__UEAAXPEAVCWnd___Z(pThis, pOldParent);
}

// Symbol: ?SaveState@CPane@@UEAAHPEB_WHI@Z
// This exact (Unicode) symbol has no resolved RVA in either image's map, but
// its ANSI twin ?SaveState@CPane@@UEAAHPEBDHI@Z does -- mfc140 RVA 0xa24e0,
// which is also what CPane vftable slot 142 points at.  Transcribed from it:
//   CString strProfileName = ::AFXGetRegPath(_T("Panes"), lpszProfileName);
//                                                         // 0xd2540
//   if (nIndex == -1) nIndex = GetDlgCtrlID();            // 0x2a78b0
//   CString strSection;
//   if (uiID == -1) strSection.Format(_T("%TsPane-%d"),   // 0xda80
//                                     strProfileName, nIndex);
//   else            strSection.Format(_T("%TsPane-%d%x"),
//                                     strProfileName, nIndex, uiID);
//   CSettingsStoreSP regSP;
//   CSettingsStore& reg = regSP.Create(FALSE, FALSE);     // 0x12b320
//   if (reg.CreateKey(strSection)) {                      // its vtable +0x28
//       BOOL bIsFloating = IsFloating();                  // slot 94
//       if (bIsFloating) {
//           CPaneFrameWnd* pMiniFrame = GetParentMiniFrame(FALSE);  // slot 140
//           if (pMiniFrame != NULL)
//               ::GetWindowRect(pMiniFrame->m_hWnd,       // import 0x1802c5370
//                   &m_recentDockInfo.m_rectRecentFloatingRect);    // this+0x2c0
//       } else {
//           CalcRecentDockedRect();                       // 0xa27e0
//           if (m_pParentDockBar != NULL) {               // +0x128
//               m_recentDockInfo.m_dwRecentAlignmentToFrame =       // this+0x2d0
//                   m_pParentDockBar->GetCurrentAlignment();  // its vtable +0x338
//               // then: walk m_pParentDockBar's CObList at +0x1e8 (CNode:
//               // pNext@0, data@0x10) counting nodes until data ==
//               // m_pDockBarRow, and store that index (0 if not found) in
//               // m_recentDockInfo.m_nRecentRowIndex          // this+0x2d8
//           }
//       }
//       reg.Write(_T("ID"),                   m_nID);             // +0x80, +0x220
//       reg.Write(_T("RectRecentFloat"),      *(CRect*)(this+0x2c0));  // +0x68
//       reg.Write(_T("RectRecentDocked"),     *(CRect*)(this+0x308));  // +0x68
//       reg.Write(_T("RecentFrameAlignment"), *(DWORD*)(this+0x2d0));  // +0x78
//       reg.Write(_T("RecentRowIndex"),       *(int*)(this+0x2d8));    // +0x80
//       reg.Write(_T("IsFloating"),           bIsFloating);            // +0x80
//       reg.Write(_T("MRUWidth"),             m_nMRUWidth);   // +0x80, +0x1dc
//       reg.Write(_T("PinState"),             m_bPinState);   // +0x80, +0x21c
//   }
//   return CBasePane::SaveState(lpszProfileName, nIndex, uiID);   // 0xc600
// (the eight value names and the two format strings are the literals at
// 0x18033c6fc, 0x18033cea0, 0x18033ceb0, 0x18033cec8, 0x18033cee0, 0x18033cef0,
// 0x18033cf00, 0x18033cf10, 0x18033ce80 and 0x18033ce90; "Panes" is at
// 0x18033c498.  They are the same eight values CPane::LoadState above reads
// back.  The Write vtable slots were read off the call sites: +0x68 takes a
// CRect, +0x78 a DWORD, +0x80 an int.)
//
// The four m_recentDockInfo destinations are named through the offsets pinned
// in the anonymous namespace above; they are inside the opaque 288-byte blob
// but at offsets derived from the SDK class declaration, not invented.
//
// Deliberate deviations, each because OpenMFC does not model the object:
//  * the row-index scan is NOT reproduced.  It walks a CObList living at
//    +0x1e8 of the parent dock site; OpenMFC only forward-declares CDockSite
//    and models no such list, so there is no list to walk.  Note that retail
//    always STORES a value into m_nRecentRowIndex on this path -- the
//    instructions at 0xa2624..0xa2654 inside SaveState (entry mfc140 0xa24e0)
//    start with ecx = 0 and store ecx whether m_pDockBarRow is NULL, the node
//    is found, or the walk falls off the end -- so leaving the field alone,
//    as the code below does, differs from retail even in the trivial cases.
//    The value written under "RecentRowIndex" is therefore whatever
//    m_nRecentRowIndex already held, not a freshly computed index.
//  * Format uses L"%sPane-%d" / L"%sPane-%d%x".  Retail's literal is
//    "%TsPane-%d"; %Ts is ATL's type-neutral string specifier, which in a
//    Unicode build formats a wide string.  OpenMFC's CString::Format
//    (include/openmfc/afxstr.h) is a plain vswprintf and does not implement
//    %Ts, so the already-expanded form is used.
//  * retail builds the CSettingsStoreSP on the stack and destroys the owned
//    store on the way out -- the instructions at 0xa2768..0xa277d inside
//    SaveState (entry mfc140 0xa24e0) reload the owner's first quadword and
//    call the store's vtable slot 1 with the flag 1, i.e. the scalar
//    deleting destructor.  OpenMFC cannot reproduce that here: the
//    store comes from CSettingsStore::CreateObject, which allocates with
//    ::operator new (phase4/src/detail/DyncreateFactoriesSupport.h:54), so
//    releasing it needs ::operator delete -- and checkfile.sh's link audit
//    rejects the new `_ZdlPv` reference that would introduce into this
//    object.  The 16-byte owner object is therefore hand-rolled as a
//    function-local static (retail zeroes both quadwords of its stack one,
//    which is the static's initial state) and one store is created and reused
//    instead of one per call.
//    That reuse is NOT transparent, and the code below compensates for it.
//    OpenMFC's CSettingsStore::CreateKey (phase4/src/core/app/CSettingsStore.cpp:33)
//    does `state.path = NormalizeSettingPath(state.path, key)` -- it APPENDS
//    the key to whatever path the store already carries, it does not replace
//    it.  On a fresh store the path is empty and CreateKey yields
//    "Software\OpenMFC\<section>"; on the reused static the second and every
//    later SaveState would nest one section inside the previous one and write
//    the pane's values somewhere no reader looks.  Open(_T("")) resets the
//    path to "Software\OpenMFC" (NormalizeSettingPath's empty-base default),
//    which is byte-for-byte the state a freshly constructed store is in, so
//    the CreateKey below produces the same path a per-call store would.  What
//    the sharing still costs is that two threads saving state at once share
//    one store; OpenMFC's settings store is a process-wide std::map with no
//    locking of its own, so that is not a new hazard.
//  * IsFloating and GetCurrentAlignment are reached through the exported
//    CBasePane implementations rather than by virtual dispatch, so a derived
//    override is not seen.  For GetCurrentAlignment that is not a behavioural
//    change for a plain dock site: slot 103 of the retail CDockSite vftable
//    (mfc140 0x2e4388) is ?GetCurrentAlignment@CBasePane@@UEBAKXZ itself.
//
// Note that OpenMFC's CSettingsStore (phase4/src/core/app/CSettingsStore.cpp)
// is an in-memory map keyed by path, not the Windows registry, so what this
// writes is only visible to CSettingsStore readers in the same process.
extern "C" int MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    if (pThis == nullptr) return FALSE;

    CString strProfileName;
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        &strProfileName, L"Panes", lpszProfileName);

    if (nIndex == -1) {
        nIndex = impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis));
    }

    CString strSection;
    if (uiID == 0xFFFFFFFFu) {
        strSection.Format(L"%sPane-%d", strProfileName.GetString(), nIndex);
    } else {
        strSection.Format(L"%sPane-%d%x", strProfileName.GetString(), nIndex, uiID);
    }

    static void* s_regSP[2] = { nullptr, nullptr };   // the hand-rolled CSettingsStoreSP
    void* pReg = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(
        static_cast<void*>(s_regSP), FALSE, FALSE);
    if (pReg != nullptr) {
        // Put the reused store back into the state a fresh one would be in --
        // CreateKey appends to the stored path rather than replacing it.  See
        // the note above.
        (void)impl__Open_CSettingsStore__UEAAHPEB_W_Z(pReg, L"");
    }

    if (pReg != nullptr &&
        impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pReg, strSection.GetString())) {
        char* pInfo = RecentDockInfoBytes(pThis);
        const int bIsFloating = PaneIsFloating(pThis) ? TRUE : FALSE;
        if (bIsFloating) {
            void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
                static_cast<const CBasePane*>(pThis), FALSE);
            if (pMiniFrame != nullptr) {
                ::GetWindowRect(reinterpret_cast<CWnd*>(pMiniFrame)->m_hWnd,
                                reinterpret_cast<RECT*>(pInfo + kRecentFloatingRect));
            }
        } else {
            impl__CalcRecentDockedRect_CPane__QEAAXXZ(pThis);
            if (pThis->m_pParentDockBar != nullptr) {
                *reinterpret_cast<unsigned long*>(pInfo + kRecentAlignment) =
                    impl__GetCurrentAlignment_CBasePane__UEBAKXZ(
                        reinterpret_cast<const CBasePane*>(pThis->m_pParentDockBar));
                // (retail recomputes m_nRecentRowIndex here -- see above)
            }
        }

        impl__Write_CSettingsStore__UEAAHPEB_WH_Z(
            pReg, L"ID", static_cast<int>(pThis->m_nID));
        impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(
            pReg, L"RectRecentFloat",
            reinterpret_cast<const CRect*>(pInfo + kRecentFloatingRect));
        impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(
            pReg, L"RectRecentDocked",
            reinterpret_cast<const CRect*>(pInfo + kRecentDockedRect));
        impl__Write_CSettingsStore__UEAAHPEB_WK_Z(
            pReg, L"RecentFrameAlignment",
            *reinterpret_cast<const unsigned long*>(pInfo + kRecentAlignment));
        impl__Write_CSettingsStore__UEAAHPEB_WH_Z(
            pReg, L"RecentRowIndex",
            *reinterpret_cast<const int*>(pInfo + kRecentRowIndex));
        impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pReg, L"IsFloating", bIsFloating);
        impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pReg, L"MRUWidth", pThis->m_nMRUWidth);
        impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pReg, L"PinState", pThis->m_bPinState);
    }

    const int bRes = impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(
        static_cast<CBasePane*>(pThis), lpszProfileName, nIndex, uiID);

    return bRes;
}

// Symbol: ?SetActiveInGroup@CPane@@UEAAXH@Z
// Retail mfc140 RVA 0xa29f0 is two instructions: `m_bActiveInGroup = bActive;`
// (member at +0x1b4), then ret.
extern "C" void MS_ABI impl__SetActiveInGroup_CPane__UEAAXH_Z(CPane* pThis, int bActive) {
    if (pThis == nullptr) return;
    pThis->m_bActiveInGroup = bActive;
}

// Symbol: ?SetBorders@CPane@@QEAAXHHHH@Z
// Retail mfc140 RVA 0x9fd50, five instructions.  Argument order confirmed from
// the register assignment (EDX -> +0x1c8, R8D -> +0x1d0, R9D -> +0x1cc,
// [RSP+0x28] -> +0x1d4), which matches the declaration in
// atlmfc/include/afxpane.h: SetBorders(cxLeft, cyTop, cxRight, cyBottom).
extern "C" void MS_ABI impl__SetBorders_CPane__QEAAXHHHH_Z(
    CPane* pThis, int cxLeft, int cyTop, int cxRight, int cyBottom) {
    if (pThis == nullptr) return;
    pThis->m_cxLeftBorder   = cxLeft;
    pThis->m_cyTopBorder    = cyTop;
    pThis->m_cxRightBorder  = cxRight;
    pThis->m_cyBottomBorder = cyBottom;
}

// Symbol: ?SetBorders@CPane@@QEAAXPEBUtagRECT@@@Z
// Retail mfc140 RVA 0x9fd70, eight instructions:
//   m_cxLeftBorder   = lpRect->left;    m_cxRightBorder  = lpRect->right;
//   m_cyTopBorder    = lpRect->top;     m_cyBottomBorder = lpRect->bottom;
extern "C" void MS_ABI impl__SetBorders_CPane__QEAAXPEBUtagRECT___Z(CPane* pThis, const RECT* lpRect) {
    if (pThis == nullptr || lpRect == nullptr) return;
    pThis->m_cxLeftBorder   = lpRect->left;
    pThis->m_cxRightBorder  = lpRect->right;
    pThis->m_cyTopBorder    = lpRect->top;
    pThis->m_cyBottomBorder = lpRect->bottom;
}

// Symbol: ?SetDockState@CPane@@UEAAXPEAVCDockingManager@@@Z
// NOT IMPLEMENTED.  Retail mfc140 RVA 0xa2850:
//   if (m_bRecentFloatingState != 0) return;              // +0x3e8
//   CDockSite* p = pDockManager->virt(+0x60)(*(int*)(this+0x2d0), TRUE);
//   if (p != NULL) <0x483c0>(pDockManager, this, p->virt(+0x568)(),
//                            (CRect*)(this+0x308));
//   if (m_pParentDockBar == NULL) return;                 // +0x128
//   m_pParentDockBar->virt(+0x550)(this, this->virt(+0x368)(), TRUE, 0);
//   if (m_pDockBarRow != NULL) <0x4ffd0>(m_pDockBarRow);  // +0x130
// Two independent blockers: the +0x2d0 and +0x308 reads are inside
// m_recentDockInfo, which OpenMFC models as an opaque 288-byte block with no
// members; and the CDockingManager the state is looked up in is a CObject
// subclass with a padding blob and no state here, so there is nothing to read.
extern "C" void MS_ABI impl__SetDockState_CPane__UEAAXPEAVCDockingManager___Z(
    CPane* pThis, CDockingManager* pDockManager) {
    (void)pThis;
    (void)pDockManager;
}

// Symbol: ?SetDragMode@CPane@@MEAAXH@Z
// Still a stub.  Retail mfc140 RVA 0xa21a0 is CPane::SetDragMode(BOOL); only
// the parameter list is corrected here (the generated stub omitted the implicit
// `this`, which the retail export does take in RCX and which the call sites in
// this file and in CDockablePane.cpp already pass).
extern "C" void MS_ABI impl__SetDragMode_CPane__MEAAXH_Z(CPane* pThis, int bOnOff) {
    (void)pThis;
    (void)bOnOff;
}

// Symbol: ?SetVirtualRect@CPane@@QEAAXAEBVCRect@@H@Z
// Retail mfc140 RVA 0xa1ce0, transcribed in full (it is fifteen instructions):
//   m_rectVirtual = rect;                              // one 16-byte movups
//   if (bMapToParent) {
//       CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//       ::MapWindowPoints(m_hWnd, pParent ? pParent->m_hWnd : NULL,
//                         (LPPOINT)&m_rectVirtual, 2);
//   }
// atlmfc/include/afxpane.h:106 declares
// `void SetVirtualRect(const CRect& rect, BOOL bMapToParent = TRUE)`, which
// names the second argument and matches the mapping the bytes perform.
// (::GetParent is import slot 0x1802c5300, CWnd::FromHandle is 0x289180 and
// ::MapWindowPoints is import slot 0x1802c5250; both slots were resolved with
// iat.py, not guessed.)  Retail really does pass a NULL hWndTo when FromHandle
// returns NULL -- that is the `xor %edx,%edx` at 0x1800a1d17 -- so the NULL
// case is a mapping to screen coordinates, not an early exit.
// Signature corrected: the generated stub dropped `this` and the CRect
// argument; MSVC passes `this` in RCX, the `const CRect&` in RDX and
// bMapToParent in R8D, which is exactly what `lea 0x1e0(%rcx),%rdi` /
// `movups (%rdx),%xmm0` / `test %r8d,%r8d` show.
extern "C" void MS_ABI impl__SetVirtualRect_CPane__QEAAXAEBVCRect__H_Z(
    CPane* pThis, const RECT* pRect, int bMapToParent) {
    if (pThis == nullptr || pRect == nullptr) return;   // deviation: retail has no NULL check
    pThis->m_rectVirtual.left   = pRect->left;
    pThis->m_rectVirtual.top    = pRect->top;
    pThis->m_rectVirtual.right  = pRect->right;
    pThis->m_rectVirtual.bottom = pRect->bottom;
    if (!bMapToParent) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    ::MapWindowPoints(pThis->m_hWnd, pParent != nullptr ? pParent->m_hWnd : nullptr,
                      reinterpret_cast<POINT*>(&pThis->m_rectVirtual), 2);
}

// Symbol: ?StoreRecentDockSiteInfo@CPane@@MEAAXXZ
// NOT IMPLEMENTED.  Retail mfc140 RVA 0x9fff0, transcribed:
//   m_recentDockInfo.<+0x28> = m_pDockBarRow;             // this+0x2e8 <- +0x130
//   m_recentDockInfo.<+0x20> = m_pParentDockBar;          // this+0x2e0 <- +0x128
//   if (m_pParentDockBar != NULL) {
//       int nIndex = 0;
//       if (m_pDockBarRow != NULL) {
//           // walk the CObList at m_pParentDockBar+0x1e8 counting nodes until
//           // node->data (+0x10) == m_pDockBarRow; 0 if never found
//       }
//       m_recentDockInfo.<+0x18> = nIndex;                // this+0x2d8
//   }
//   CalcRecentDockedRect();                               // tail jump 0xa27e0
// Every destination is inside m_recentDockInfo (CPane +0x2b8), which OpenMFC
// models as an opaque 288-byte block with no members and no pinned interior
// offsets, and the walk needs the CDockSite row list at its +0x1e8, which
// OpenMFC does not model either.  Parameter list corrected to take the implicit
// `this`, which the call sites in this file pass.
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPane__MEAAXXZ(CPane* pThis) {
    (void)pThis;
}

// Symbol: ?StretchPaneDeferWndPos@CPane@@UEAAHHAEAPEAX@Z
// Retail mfc140 RVA 0xa12b0, transcribed:
//   int nExpand  = GetAvailableExpandSize();             // vtable slot 160
//   int nStretch = GetAvailableStretchSize();            // vtable slot 161
//   int nDelta;
//   if (nLength > 0) {
//       if (nExpand == 0) return 0;
//       nDelta = min(nExpand, nLength);                  // cmovg at 0xa1311
//   } else {
//       nDelta = nLength;
//       if (nStretch < abs(nLength)) nDelta = -nStretch; // cmovl at 0xa1328
//   }
//   CRect rect(0,0,0,0); ::GetWindowRect(m_hWnd, &rect); // import 0x1802c5370
//   if (IsHorizontal()) rect.right  += nDelta;           // vtable slot 91
//   else                rect.bottom += nDelta;
//   OnBeforeStretch(nDelta);                             // vtable slot 194
//   if (abs(nDelta) > 0) {                               // CRT abs, import
//                                                        // 0x1802c5910
//       CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rect);
//       MoveWindow(rect, TRUE, NULL);                    // vtable slot 143
//       OnAfterStretch(nDelta);                          // vtable slot 195
//   }
//   return nDelta;
// Two things worth flagging, both verified in the bytes:
//  * the `HDWP& hdwp` out-parameter is NEVER read or written.  R8 is not saved
//    in the prologue (its home slot is reused for RBX) and the slot-143 call at
//    0xa13bc explicitly passes hdwp = NULL (`xor %r9d,%r9d`), so despite the
//    name nothing is deferred.  This body reproduces that; `hdwp` is untouched.
//  * slots 194 (+0x610) and 195 (+0x618) both hold 0x2820 in the CPane vftable,
//    which is a bare `ret`.  They are CPane::OnBeforeStretch(int&) and
//    CPane::OnAfterStretch(int) -- atlmfc/include/afxpane.h:193-194 declares
//    both as empty inline bodies, and they sit in exactly that position in the
//    header's virtual-declaration order (OnBeforeDock, OnAfterDock,
//    OnBeforeStretch, OnAfterStretch, RemoveFromMiniframe, SetDragMode, ...
//    == slots 192..197).  Being no-ops for a CPane they are omitted here; that
//    skips a derived override, and it also means nDelta cannot be revised
//    between the two `abs` reads the way retail allows.
// Signature corrected: the generated stub dropped the implicit `this`.
extern "C" int MS_ABI impl__StretchPaneDeferWndPos_CPane__UEAAHHAEAPEAX_Z(
    CPane* pThis, int nLength, void** phdwp) {
    (void)phdwp;   // retail never touches it -- see above
    if (pThis == nullptr) return 0;
    const int nExpand = impl__GetAvailableExpandSize_CPane__UEBAHXZ(pThis);
    const int nStretch = impl__GetAvailableStretchSize_CPane__UEBAHXZ(pThis);
    int nDelta;
    if (nLength > 0) {
        if (nExpand == 0) return 0;
        nDelta = nExpand > nLength ? nLength : nExpand;
    } else {
        nDelta = nLength;
        const int nAbs = nLength < 0 ? -nLength : nLength;
        if (nStretch < nAbs) nDelta = -nStretch;
    }
    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rect);
    if (PaneIsHorizontal(pThis)) {
        rect.right += nDelta;
    } else {
        rect.bottom += nDelta;
    }
    if (nDelta == 0) return nDelta;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent != nullptr) {   // deviation: retail dereferences the result unchecked
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rect);
    }
    impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
        static_cast<CBasePane*>(pThis), &rect, TRUE, nullptr);
    return nDelta;
}

// Symbol: ?UndockPane@CPane@@UEAAXH@Z
// Retail mfc140 RVA 0xa2a00, transcribed in full (it is fourteen instructions):
//   if (m_pParentDockBar != NULL)                        // member at +0x128
//       m_pParentDockBar->RemovePane(this, DM_UNKNOWN);  // its vtable +0x508
//   if (!bDelay)
//       AdjustDockingLayout(NULL);                       // vtable slot 156
// The +0x508 dispatch was identified by locating the CDockSite vftable (mfc140
// RVA 0x2db... found by anchoring slot 0 on ?GetThisClass@CDockSite@@ at 0xa580
// and slot 161 on 0x53e10) and reading slot 161 out of it:
// ?RemovePane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z.  R8D is zero at
// the call site, i.e. AFX_DOCK_METHOD DM_UNKNOWN.  CPane vtable slot 156
// (+0x4e0) is 0xbdb0 == ?AdjustDockingLayout@CBasePane@@UEAAXPEAX@Z, called
// with EDX = 0.
// Both are called through their exported thunks, so a derived override of
// either is missed; both of those thunks are still generated stubs as of this
// writing, so this body currently has no observable effect at run time.
// Signature corrected: the generated stub dropped the implicit `this`.
extern "C" void MS_ABI impl__UndockPane_CPane__UEAAXH_Z(CPane* pThis, int bDelay) {
    if (pThis == nullptr) return;
    if (pThis->m_pParentDockBar != nullptr) {
        impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(
            pThis->m_pParentDockBar, pThis, kDockMethodUnknown);
    }
    if (!bDelay) {
        impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(static_cast<CBasePane*>(pThis), nullptr);
    }
}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXVCPoint@@@Z
// Retail mfc140 RVA 0xa1ba0, transcribed in full:
//   CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//   if (p->GetExStyle() & WS_EX_LAYOUTRTL)          // `bt $0x16,%eax`
//       if (IsHorizontal())                         // vtable slot 91
//           ptOffset.x = -ptOffset.x;               // `neg %ebx`
//   ::OffsetRect(&m_rectVirtual, ptOffset.x, ptOffset.y);   // tail jump,
//                                                   // import slot 0x1802c5318
// The x negation is nested inside the RTL test -- it happens only when BOTH
// hold.  ::GetParent is import 0x1802c5300, ::OffsetRect import 0x1802c5318 and
// 0x2a75d0 is ?GetExStyle@CWnd@@QEBAKXZ; all three were resolved, not guessed.
// Signature corrected: the generated stub dropped the implicit `this`.  MSVC
// passes the by-value CPoint packed into RDX, so it arrives here in a
// pointer-sized parameter (see UnpackPoint).
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCPoint___Z(
    CPane* pThis, void* ptOffsetPacked) {
    if (pThis == nullptr) return;
    POINT ptOffset = UnpackPoint(ptOffsetPacked);
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    // Deviation: retail calls GetExStyle on the FromHandle result unchecked.
    if (pParent != nullptr &&
        (impl__GetExStyle_CWnd__QEBAKXZ(pParent) & kExLayoutRtl) != 0 &&
        PaneIsHorizontal(pThis)) {
        ptOffset.x = -ptOffset.x;
    }
    ::OffsetRect(reinterpret_cast<RECT*>(&pThis->m_rectVirtual), ptOffset.x, ptOffset.y);
}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXVCSize@@@Z
// Retail mfc140 RVA 0xa1c10, transcribed in full:
//   ::GetWindowRect(m_hWnd, &m_rectVirtual);           // import 0x1802c5370
//   m_rectVirtual.right  = m_rectVirtual.left + sizeNew.cx;
//   m_rectVirtual.bottom = m_rectVirtual.top  + sizeNew.cy;
//   CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//   if (p != NULL) p->ScreenToClient(&m_rectVirtual);  // 0x2a11f0
// (Note the whole rect is first overwritten with the SCREEN rect, so left/top
// are the screen origin at the moment the two adds run; only then is the whole
// rect mapped back to the parent's client space.)
// Signature corrected: the generated stub dropped the implicit `this`.  The
// by-value CSize arrives packed in RDX, i.e. in a pointer-sized parameter.
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXVCSize___Z(
    CPane* pThis, void* sizeNewPacked) {
    if (pThis == nullptr) return;
    const POINT sizeNew = UnpackPoint(sizeNewPacked);   // .x == cx, .y == cy
    ::GetWindowRect(pThis->m_hWnd, reinterpret_cast<RECT*>(&pThis->m_rectVirtual));
    pThis->m_rectVirtual.right  = pThis->m_rectVirtual.left + sizeNew.x;
    pThis->m_rectVirtual.bottom = pThis->m_rectVirtual.top  + sizeNew.y;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
            pParent, reinterpret_cast<RECT*>(&pThis->m_rectVirtual));
    }
}

// Symbol: ?UpdateVirtualRect@CPane@@QEAAXXZ
// Retail mfc140 RVA 0xa1ae0, transcribed in full:
//   ::GetWindowRect(m_hWnd, &m_rectVirtual);            // import 0x1802c5370
//   CSize size = CalcFixedLayout(FALSE, IsHorizontal());// slots 154 and 91
//   m_rectVirtual.right  = m_rectVirtual.left + size.cx;
//   m_rectVirtual.bottom = m_rectVirtual.top  + size.cy;
//   CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//   if (p != NULL) p->ScreenToClient(&m_rectVirtual);   // 0x2a11f0
// This is the CSize overload above with the size taken from CalcFixedLayout
// instead of from an argument.  CalcFixedLayout is CPane vtable slot 154
// (+0x4d0 -> 0xacb0); its bStretch argument is FALSE here, and for that input
// the retail body returns CSize(0, 0) whatever bHorz is -- so a plain CPane
// really does collapse m_rectVirtual to zero width and height at this point.
// PaneBaseCalcFixedLayout reproduces exactly that body; see its comment for the
// override that is lost by not dispatching.
extern "C" void MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    ::GetWindowRect(pThis->m_hWnd, reinterpret_cast<RECT*>(&pThis->m_rectVirtual));
    const SIZE size = PaneBaseCalcFixedLayout(FALSE, PaneIsHorizontal(pThis) ? 1 : 0);
    pThis->m_rectVirtual.right  = pThis->m_rectVirtual.left + size.cx;
    pThis->m_rectVirtual.bottom = pThis->m_rectVirtual.top  + size.cy;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
            pParent, reinterpret_cast<RECT*>(&pThis->m_rectVirtual));
    }
}
