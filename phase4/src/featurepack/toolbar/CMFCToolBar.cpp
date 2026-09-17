// CMFCToolBar — OpenMFC implementation.
// Sources: global_mfc-feature-3_impl.cpp, global_mfc-feature-4_impl.cpp, global_mfc-feature-5_impl.cpp, global_mfctoolbar_batch1.cpp, global_mfctoolbar_batch2.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCToolBarSupport.h"
#include "detail/MfcFeature5ImplSupport.h"
#include "detail/MfctoolbarBatch2Support.h"
#include "detail/MfccoreSupport.h"

// CMFCToolBar Feature Pack exports, wave 4 ("mfc-feature-4").
//
// Each function is transcribed from the retail mfc140u export
// (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350).
// Where the retail body needs internal helpers, unmodeled globals or virtual
// slots that OpenMFC does not model, the implementation keeps the visible side
// effects (member writes, window invalidation, message posting) and returns
// the safe default, with a `// TODO(clean-room)` marker.
//
// Cross-file impl helpers referenced here:
//   thunks.cpp   : CWnd::Default / CWnd::OnSetFocus / CWnd::UpdateDialogControls
//   wincore.cpp  : CWnd::PreTranslateMessage
//   mfccore.cpp  : CTooltipManager::CreateToolTip
//   feature-3    : CMFCToolBar::OnCancelMode / OnChangeHot / OnAfterButtonDelete
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Statics with storage in feature_static_data.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;
extern "C" void*        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA;
extern "C" void*        impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA;

// Forward declarations of exported helpers defined in sibling shards.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI   impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, CWnd* pOldWnd);
extern "C" void MS_ABI   impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
                             CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHandler);
extern "C" int MS_ABI    impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
extern "C" int MS_ABI    impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
                             CToolTipCtrl** ppToolTip, CWnd* pParentWnd, unsigned int nType);
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot);
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis);

// Functions implemented in this file that are referenced by earlier functions.
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBar__UEAAHH_Z(CMFCToolBar* pThis, int iIndex);
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);

// Exported helpers from sibling shards used by the accessibility / image /
// usage-count exports at the end of this file.
//   featurepack/toolbar/CMFCToolBarImages.cpp : Clear / CleanUp / CopyTo / GrayImages
//   featurepack/customize/CMFCCmdUsageCount.cpp : AddCmd
extern "C" void MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" void MS_ABI impl__CleanUp_CMFCToolBarImages__SAXXZ();
extern "C" int  MS_ABI impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(
                           CMFCToolBarImages* pThis, CMFCToolBarImages* pDst);
extern "C" int  MS_ABI impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(
                           CMFCToolBarImages* pThis, int nLuminancePercentage);
extern "C" void MS_ABI impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(void* pThis, unsigned int cmd);

// Class statics with storage in featurepack/toolbar/StaticData.cpp, written by
// AutoGrayInactiveImages (retail globals 0x1803be358 / 0x1803be34c).
extern "C" std::int32_t impl__m_bAutoGrayInactiveImages_CMFCToolBar__1HA;
extern "C" std::int32_t impl__m_nGrayImagePercentage_CMFCToolBar__1HA;

// CalcFixedLayout forwards to CalcLayout, which is defined after it.
extern "C" CSize* MS_ABI impl__CalcLayout_CMFCToolBar__MEAA_AVCSize__KH_Z(
                             CMFCToolBar* pThis, CSize* pRet, unsigned long dwMode, int nLength);

// ---------------------------------------------------------------------------
// Additional cross-reference declarations used by the exports at the end of
// this file.
//
// Class statics with storage in featurepack/toolbar/StaticData.cpp.
//   ?m_clrTextHot@CMFCToolBar@@1KA   is the retail global at 0x3aab54,
//   ?m_sizeMenuButton@ / ?m_sizeButton@ are defined further down in THIS file.
extern "C" std::uint32_t impl__m_clrTextHot_CMFCToolBar__1KA;

// ResetAll / Deactivate re-enter CMFCToolBar exports defined later in this file.
extern "C" int  MS_ABI impl__CanBeRestored_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis);
extern "C" int  MS_ABI impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RestoreFocus_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);

// CObList exports used by GetAllToolbars (definitions in
// core/collections/CObList.cpp).  The C++ methods CObList::AddTail/RemoveAll
// exist in the DLL only as those thunks from this translation unit's point of
// view, so they are called through the thunks -- see AGENTS.md §"impl__".
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);

// Cross-file thunks used by the InsertButton / drag-over / focus / row-height
// bodies further down.  Each one was grepped to a definition in the file named;
// the parameter lists here are derived from the mangled names.  Several of the
// definitions still carry auto-generated placeholder parameter lists (noted per
// symbol) -- that mismatch is harmless across translation units for extern "C"
// symbols, and the callees concerned ignore their arguments anyway.
//   featurepack/toolbar/CMFCToolBarButton.cpp : OnChangeParentWnd (placeholder
//                                               list, empty stub), CreateFromOleData
//                                               (placeholder list, returns NULL)
//   featurepack/toolbar/RuntimeClasses.cpp    : GetThisClass@CMFCToolBarSystemMenuButton
//   featurepack/menu/CMFCPopupMenu.cpp        : GetParentToolBar (returns NULL today)
//   featurepack/docking/CBasePane.cpp         : IsDocked, GetParentMiniFrame
//   featurepack/docking/CPane.cpp             : OnLButtonDblClk (stub)
//   featurepack/docking/CPaneFrameWnd.cpp     : OnDockToRecentPos (no-op)
//   featurepack/visualmanager/Thunks.cpp      : OnDrawSeparator@CMFCVisualManager
//   core/frame/CFrameImpl.cpp                 : IsUserDefinedToolbar
//   core/window/Thunks.cpp                    : GetTopLevelFrame@CWnd
//   core/runtime/AFX_GLOBAL_DATA.cpp          : Initialize@AFX_GLOBAL_DATA (empty
//                                               stub with a placeholder list)
//   featurepack/CMFC_misc_stubs.cpp           : ?afxGlobalData@@ storage, a zeroed
//                                               720-byte block
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(
                           CMFCToolBarButton* pThis, CWnd* pWndParent);
extern "C" CMFCToolBarButton* MS_ABI
    impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(COleDataObject* pDataObject);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CMFCToolBar* MS_ABI impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(const void* pThis);
extern "C" int  MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
                            const CBasePane* pThis, int bNoAssert);
extern "C" void MS_ABI impl__OnLButtonDblClk_CPane__IEAAXIVCPoint___Z(
                           CPane* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
                           CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz);
extern "C" int  MS_ABI impl__IsUserDefinedToolbar_CFrameImpl__IEBAHPEBVCMFCToolBar___Z(
                           const void* pImpl, const void* pToolBar);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
//   core/frame/CFrameWndEx.cpp / CMDIFrameWndEx.cpp : GetThisClass (RTTI descriptors)
//   core/runtime (CObject)                    : IsKindOf@CObject
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                          const CObject* pThis, const CRuntimeClass* pClass);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];

// Exports defined later in this file that the bodies above them call.
extern "C" int  MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, CPoint point);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(
                           CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" int  MS_ABI impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(
                           const CMFCToolBar* pThis, CPoint point, CRect* pRect);
extern "C" int  MS_ABI impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(
                           CMFCToolBar* pThis, CMFCToolBarButton* pButton, int iInsertAt);
extern "C" int  MS_ABI impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(
                           const CMFCToolBar* pThis, unsigned int nIDFind, int iIndexFirst);
extern "C" void MS_ABI impl__GetItemRect_CMFCToolBar__UEBAXHPEAUtagRECT___Z(
                           const CMFCToolBar* pThis, int nIndex, RECT* lpRect);
extern "C" void MS_ABI impl__Deactivate_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);

// AFX_GLOBAL_DATA is not declared in OpenMFC's headers; ?afxGlobalData@@ is a
// 720-byte block.  The four fields below are read at the byte offsets the retail
// bodies in this file use, so that they start returning real values the moment
// AFX_GLOBAL_DATA::Initialize / UpdateTextMetrics are implemented:
//   +0x000 int    : one-time init gate -- every retail reader does
//                   `if (!*(int*)&afxGlobalData) { Initialize(); *(int*) = 1; }`
//                   before touching the object (see GetMenuImageSize below)
//   +0x2a8 int    : text height for a horizontal bar; +0x2ac the vertical one.
//                   ?UpdateTextMetrics@AFX_GLOBAL_DATA@@IEAAXXZ (RVA 0x6b7a0,
//                   mfc140u) stores them at 0x6b818 / 0x6b851 as
//                   tmHeight + (tmHeight < 15 ? 2 : 5) of the fonts at +0x1a8
//                   and +0x208 respectively.
//   +0x2b8 double, +0x2c0 int : an image scale and its enable flag, read as
//                   `flag ? scale : 1.0` (GetMenuImageSize below).
namespace {
constexpr int kGlobalDataInitGate       = 0x000;
constexpr int kGlobalDataTextHeightHorz = 0x2a8;
constexpr int kGlobalDataTextHeightVert = 0x2ac;
constexpr int kGlobalDataImageScale     = 0x2b8;
constexpr int kGlobalDataImageScaleOn   = 0x2c0;

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline double GlobalDataDouble(int off) { double v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }

// The retail one-time gate: `if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }`.
inline void EnsureGlobalDataInitialized()
{
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
} // namespace

// Further cross-file thunks used by the message handlers and insert bodies at
// the end of this file.  Each was grepped to its definition in the file named.
//   featurepack/toolbar/RuntimeClasses.cpp : GetThisClass@CMFCToolBar /
//                                            @CMFCToolBarMenuButtonsButton
//   featurepack/menu/RuntimeClasses.cpp    : GetThisClass@CMFCPopupMenu
//   featurepack/tabs/RuntimeClasses.cpp    : GetThisClass@CMFCTabCtrl
//   featurepack/docking/CPane.cpp          : GetThisClass@CPane
//   featurepack/docking/CMFCControlBarImpl.cpp : CalcNcSize / DrawNcArea /
//                                            GetGripperRect
//   featurepack/toolbar/CMFCToolBarButton.cpp : CopyFrom (real body),
//                                            SetACCData (stub, placeholder list)
//   core/runtime/CRuntimeClass.cpp         : CreateObject@CRuntimeClass
//   core/window/CWnd.cpp                   : FromHandle@CWnd, GetWindowTextW@CWnd
//   featurepack/CMFC_misc_stubs.cpp        : AfxLoadString (wide)
//   core/collections/Globals.cpp           : AfxExtractSubString, AfxMessageBox
//   core/runtime/StaticData.cpp            : ?m_pActivePopupMenu@CMFCPopupMenu@@,
//                                            ?m_hookMouseHelp@CMFCToolBar@@
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__CalcNcSize_CMFCControlBarImpl__IEAAXPEAUtagNCCALCSIZE_PARAMS___Z(
                           void* pImpl, NCCALCSIZE_PARAMS* lpncsp);
extern "C" void MS_ABI impl__DrawNcArea_CMFCControlBarImpl__IEAAXXZ(void* pImpl);
extern "C" void MS_ABI impl__GetGripperRect_CMFCControlBarImpl__QEAAXAEAVCRect__H_Z(
                           void* pImpl, RECT* pRect, int bClientCoord);
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(
                           void* pThis, const CMFCToolBarButton* pSrc);
extern "C" int  MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
                           CMFCToolBarButton* pThis, CWnd* pParent, CAccessibilityData* pData);
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pClass);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                           const CWnd* pThis, CString* pString);
extern "C" int  MS_ABI impl__AfxLoadString__YAHIPEA_WI_Z(unsigned int nID, wchar_t* lpszBuf, unsigned int nMaxBuf);
extern "C" int  MS_ABI impl__AfxExtractSubString__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WH_W_Z(
                           CString* pString, const wchar_t* lpszFullString, int iSubString, wchar_t chSep);
extern "C" int  MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, unsigned int nType, unsigned int nIDHelp);
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
extern "C" void* impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA;
// CObList::InsertBefore as this DLL defines it (core/collections/CObList.cpp):
// note that definition takes the POSITION *by address* and dereferences it,
// so it is called with &pos here.
extern "C" CObList::POSITION MS_ABI
    impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
        CObList* pThis, CObList::POSITION* pPos, CObject* value);

//   featurepack/toolbar/Thunks.cpp         : ??0CMFCToolBarButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_XZ(void* pThis);

//   core/window/Thunks.cpp                 : SetFocus@CWnd (runs the C++ CWnd::SetFocus)
//   featurepack/docking/Thunks.cpp         : RecalcLayout@CPane (virtual dispatch on
//                                            the C++ object)
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" void  MS_ABI impl__RecalcLayout_CPane__UEAAXXZ(CPane* pThis);

// Exports defined later in this file that the handlers below call.
extern "C" void MS_ABI impl__SetHelpMode_CMFCToolBar__SAXH_Z(int bOn);
extern "C" CToolBarDropSource_144Bytes impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A;

// Two members of the static ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
// (mfc140u 0x3c2510, a 144-byte blob here -- CMFCToolBarDropSource is not
// declared in OpenMFC's headers).  The dynamic initialiser at RVA 0x1d60
// (mfc140u) runs the COleDropSource ctor (0x25a350) on it, installs the
// CMFCToolBarDropSource vftable (0x3178d8) and then stores a QWORD 1 at +0x68
// and zeroes +0x70, +0x78 and +0x80..+0x8f, i.e. BOOL +0x68 = TRUE,
// BOOL +0x6c = FALSE, BOOL +0x70 = FALSE.  OnDragEnter (0x152250) clears +0x68 and OnDragLeave
// (0x152290) sets it; OnLButtonDown (0x150410) clears +0x70 before
// COleDataSource::DoDragDrop and tests +0x70 and +0x6c afterwards.  That is the
// afxtoolbar.h declaration order m_bDeleteOnDrop (TRUE by default),
// m_bEscapePressed, m_bDragStarted; the names are taken from that header, the
// offsets from the retail bodies.
namespace {
constexpr int kDropSourceDeleteOnDrop = 0x68;
static_assert(kDropSourceDeleteOnDrop + 4 <= 144, "m_DropSource blob too small");
inline void SetDropSourceDeleteOnDrop(int v)
{
    std::memcpy(impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A.data +
                    kDropSourceDeleteOnDrop, &v, sizeof v);
}

// An unnamed CMFCToolBar* file-static of retail afxtoolbar.cpp (mfc140u
// .bss 0x3c3728).  Written only by OnMouseMove (0x150ce0, at 0x150f9b when it
// arms TrackMouseEvent) and compared against `this` only by OnMouseLeave
// (0x1567a0) -- those are the only two references in the whole image.  The
// name is OpenMFC's.  OnMouseMove is still a stub here, so it is never set.
CMFCToolBar* g_pTrackedToolBar = nullptr;

// afxGlobalData slots read by the handlers below (see the block above for the
// convention): +0x158 and +0x170 (0x3c1778 / 0x3c1790 in mfc140u) are handed
// to USER32!SetCursor by OnSetCursor, +0x1b0 (0x3c17d0) is the WM_SETFONT
// wParam OnNeedTipText sends to the tooltip.  The constant names follow the
// afxglobals.h order (m_hcurStretch, m_hcurStretchVert, m_hcurHand,
// m_hcurSizeAll; fontTooltip), which is consistent with those uses but is not
// itself verified here.  AFX_GLOBAL_DATA::Initialize is an empty stub in
// OpenMFC, so all three read back NULL today; callers treat NULL as "not
// available" rather than hand it to USER32.
constexpr int kGlobalDataCursorStretch = 0x158;
constexpr int kGlobalDataCursorSizeAll = 0x170;
constexpr int kGlobalDataFontTooltip   = 0x1b0;
inline void* GlobalDataPtr(int off) { void* v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }

// The inlined AFX_GLOBAL_DATA::GetTextHeight(bHorz) that GetRowHeight and
// CalcMaxButtonHeight read: `bHorz ? +0x2a8 : +0x2ac`.
inline int GlobalDataTextHeight(bool bHorz)
{
    return GlobalDataInt(bHorz ? kGlobalDataTextHeightHorz : kGlobalDataTextHeightVert);
}

// CMFCToolBarButton::OnChangeParentWnd, base body (RVA 0x15e570, mfc140u):
//     m_bExtraSize = FALSE;  m_pWndParent = pWndParent;          // +0x5c, +0x80
//     if (pWndParent && pWndParent->IsKindOf(RUNTIME_CLASS(CMFCToolBar)) &&  // 0x3b15f8
//         ((CMFCToolBar*)pWndParent)->IsButtonExtraSizeAvailable())        // vslot 0x6c8
//         m_bExtraSize = TRUE;
// Used by InsertButton because the button vslot (0x60) cannot be dispatched
// and the exported base thunk is an empty stub with a placeholder parameter
// list.  Slot 0x6c8 of the CMFCToolBar vftable (0x1803157c8) is 0x3a60, the
// header inline `return TRUE`; the parent passed here is always a CMFCToolBar,
// so the IsKindOf test is folded away.  A derived toolbar's override of
// IsButtonExtraSizeAvailable (retail dispatches it) is not honoured.
inline void ButtonChangeParentWnd(CMFCToolBarButton* pButton, CMFCToolBar* pParent)
{
    pButton->m_bExtraSize = FALSE;
    pButton->m_pWndParent = pParent;
    if (pParent) pButton->m_bExtraSize = TRUE;   // TODO(clean-room): vslot 0x6c8
}
} // namespace

// ---------------------------------------------------------------------------
// Cross-file thunks and statics used by the Set* / Save* / Load* / Reset* /
// Restore* bodies at the end of this file.  Each was grepped to its definition
// in the file named; the parameter lists are derived from the mangled names.
//   featurepack/docking/CBasePane.cpp         : GetThisClass@CBasePane, GetDockSiteFrameWnd
//   featurepack/docking/CPane.cpp             : AdjustSizeImmediate (partial), UpdateVirtualRect
//   featurepack/docking/CDockingPanesRow.cpp  : ArrangePanes(CPane*) (real body)
//   featurepack/toolbar/CMFCToolBarButton.cpp : SetImage (real body), ResetImageToDefault
//                                               (an EMPTY thunk there; NOT retail's
//                                               behaviour -- see ResetImages below)
//   featurepack/CMFC_misc_stubs.cpp           : AFXGetParentFrame, AFXGetRegPath
//   core/app/CSettingsStoreSP.cpp             : Create@CSettingsStoreSP
//   core/app/CSettingsStore.cpp               : Open / CreateKey / Read(int&) /
//                                               Read(CObject&) / Write(int) / Write(CObject&)
//   core/collections/CPlex.cpp                : Create@CPlex, FreeDataChain@CPlex
//   core/frame/CFrameWnd.cpp                  : GetThisClass@CFrameWnd
//   featurepack/toolbar/StaticData.cpp        : m_dblLargeImageRatio, m_bShowTooltips,
//                                               m_bShowShortcutKeys
//   featurepack/menu/StaticData.cpp           : m_AnimationType@CMFCPopupMenu,
//                                               m_bRecentlyUsedMenus / m_bMenuShadows /
//                                               m_bShowAllMenusDelay@CMFCMenuBar
//   featurepack/customize/StaticData.cpp      : m_nStartCount / m_nMinUsagePercentage
//                                               @CMFCCmdUsageCount
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis);
extern "C" void  MS_ABI impl__AdjustSizeImmediate_CPane__UEAAXH_Z(CPane* pThis, int bRecalcLayout);
extern "C" void  MS_ABI impl__UpdateVirtualRect_CPane__QEAAXXZ(CPane* pThis);
extern "C" void  MS_ABI impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(void* pRow, CPane* pInitialBar);
extern "C" void  MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage);
extern "C" void  MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void* pThis);
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString* pRet, const wchar_t* lpszPostFix, const wchar_t* lpszProfileName);
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(void* pThis, int bAdmin, int bReadOnly);
extern "C" int   MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);
extern "C" int   MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);
extern "C" int   MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(void* pStore, const wchar_t* lpszValueName, int* pValue);
extern "C" int   MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* pStore, const wchar_t* lpszValueName, CObject* pObj);
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WH_Z(void* pStore, const wchar_t* lpszValueName, int nValue);
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* pStore, const wchar_t* lpszValueName, CObject* pObj);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void  MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* pHead);
extern "C" double        impl__m_dblLargeImageRatio_CMFCToolBar__2NA;
extern "C" std::int32_t  impl__m_bShowTooltips_CMFCToolBar__1HA;
extern "C" std::int32_t  impl__m_bShowShortcutKeys_CMFCToolBar__1HA;
extern "C" std::uint32_t impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A;
extern "C" std::int32_t  impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;
extern "C" std::int32_t  impl__m_bMenuShadows_CMFCMenuBar__1HA;
extern "C" std::int32_t  impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA;
extern "C" std::uint32_t impl__m_nStartCount_CMFCCmdUsageCount__1IA;
extern "C" std::uint32_t impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA;
// Defined later in this file; LoadParameters calls it the way retail does.
extern "C" void MS_ABI impl__SetLargeIcons_CMFCToolBar__SAXH_Z(int bLargeIcons);
// ?m_UsageCount@CMFCToolBar@@ storage, defined later in this file (80 bytes).
extern "C" unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80];

// ---------------------------------------------------------------------------
// Cross-file thunks and statics used by the Save*/Set*/Restore*/OnCopyImage
// bodies further down.  Each was grepped to its definition in the file named;
// the parameter lists are derived from the mangled names.
//   core/collections/CObList.cpp            : AddTail(CObList*)  (the list overload)
//   featurepack/docking/CDockingPanesRow.cpp: RemovePane (real body); AddPane is an
//                                             EMPTY STUB with an auto-generated
//                                             placeholder parameter list -- the
//                                             declaration below is the one the
//                                             mangled name describes (see the
//                                             headerRequest filed for that file)
//   core/app/CSettingsStore.cpp             : Write(LPCTSTR,LPCTSTR), Write(LPCTSTR,LPBYTE,UINT)
//   core/window/Thunks.cpp                  : GetDlgCtrlID@CWnd
//   core/file/Thunks.cpp                    : ??0CMemFile(UINT), ??1CMemFile, Detach, GetLength
//   core/runtime/CArchive.cpp / Thunks.cpp  : ??0CArchive, Flush, ??1CArchive
//   detail/RegcoreSupport.cpp               : AfxGetApp
//   core/app/CWinAppEx.cpp                  : GetThisClass@CWinAppEx
//   featurepack/docking/CPane.cpp           : SaveState@CPane (real body)
//   core/frame/Thunks.cpp                   : RecalcLayout@CFrameWnd (virtual dispatch)
//   featurepack/docking/Thunks.cpp          : SetWindowPos@CBasePane
//   core/runtime/CCmdTarget.cpp             : BeginWaitCursor / EndWaitCursor
//   featurepack/toolbar/CMFCToolBarImages.cpp : CopyImageToClipboard (partial body)
//   this file (defined later)               : ?m_pUserImages@ / ?m_Images@ storage,
//                                             Serialize (stub, see its comment)
extern "C" void  MS_ABI impl__AddTail_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList);
extern "C" void  MS_ABI impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(void* pRow, CPane* pPane);
extern "C" void  MS_ABI impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(
                            void* pRow, CPane* pPane, int dockMethod, const RECT* lpRect, int bAddLast);
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_W0_Z(void* pStore, const wchar_t* lpszValueName, const wchar_t* lpszValue);
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(void* pStore, const wchar_t* lpszValueName, const unsigned char* pData, unsigned int nBytes);
extern "C" int   MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void* MS_ABI impl___0CMemFile__QEAA_I_Z(void* pThis, unsigned int nGrowBytes);
extern "C" void  MS_ABI impl___1CMemFile__UEAA_XZ(void* pThis);
extern "C" unsigned char* MS_ABI impl__Detach_CMemFile__QEAAPEAEXZ(CMemFile* pThis);
extern "C" unsigned __int64 MS_ABI impl__GetLength_CMemFile__UEBA_KXZ(const CMemFile* pThis);
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(void* pThis, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf);
extern "C" void  MS_ABI impl__Flush_CArchive__QEAAXXZ(CArchive* pThis);
extern "C" void  MS_ABI impl___1CArchive__QEAA_XZ(void* pThis);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
extern "C" int   MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" void  MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                            CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra);
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" int   MS_ABI impl__CopyImageToClipboard_CMFCToolBarImages__QEAAHH_Z(CMFCToolBarImages* pThis, int iImageIndex);
extern "C" void* impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA;
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
//   featurepack/CMFC_misc_stubs.cpp         : ?afxUserToolsManager@@ (a NULL pointer nothing in this tree assigns)
extern "C" void* impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
extern "C" void  MS_ABI impl__Serialize_CMFCToolBar__UEAAXAEAVCArchive___Z(CMFCToolBar* pThis, CArchive* ar);
extern "C" void  MS_ABI impl__SaveOriginalState_CMFCToolBar__MEAAXAEAVCSettingsStore___Z(CMFCToolBar* pThis, CSettingsStore& store);
extern "C" void  MS_ABI impl__SaveResetOriginalState_CMFCToolBar__IEAAXAEAVCSettingsStore___Z(CMFCToolBar* pThis, CSettingsStore& store);

namespace {
// Retail keeps every live CMFCToolBar in a private static CObList (0x3b2090,
// mfc140u; head at 0x3b2098, count at 0x3b20a8) that OnCreate/OnDestroy
// maintain.  OpenMFC has no such list; the substitution made throughout this
// file (AutoGrayInactiveImages, ResetAll, RedrawUnderlines, ...) is the
// mfccore.cpp toolbar side table.  The keys are snapshotted first because the
// per-bar work below (AdjustLayout, OnCustomizeMode, OnCancelMode) touches the
// same table.
inline std::vector<CMFCToolBar*> SnapshotToolBars()
{
    std::vector<CMFCToolBar*> bars;
    bars.reserve(g_toolBarStates.size());
    for (auto& entry : g_toolBarStates) {
        if (entry.first) bars.push_back(const_cast<CMFCToolBar*>(entry.first));
    }
    return bars;
}

// Writer counterpart of GlobalDataInt above (RestoreFocus clears a flag).
inline void GlobalDataSetInt(int off, int v) { std::memcpy(GlobalDataBytes() + off, &v, sizeof v); }

// Two more afxGlobalData slots: +0x27c and +0x280 (0x3c189c / 0x3c18a0 in
// mfc140u).  ?OnSettingChange@AFX_GLOBAL_DATA@@QEAAXXZ (RVA 0x6b0a0, mfc140u)
// fills +0x280 with SystemParametersInfoW(SPI_GETKEYBOARDCUES /*0x100a*/) at
// 0x6b194 and then copies it into +0x27c at 0x6b1a6, i.e. the system
// keyboard-cues setting and the working "underline shortcuts" flag that
// RestoreFocus below clears -- the afxglobals.h pair
// m_bUnderlineKeyboardShortcuts (+0x27c) / m_bSysUnderlineKeyboardShortcuts
// (+0x280), declared in that order at afxglobals.h lines 171-172 of the
// 14.51 SDK on this host.
constexpr int kGlobalDataUnderlineShortcuts    = 0x27c;
constexpr int kGlobalDataSysUnderlineShortcuts = 0x280;

// The 16-byte CSettingsStoreSP owner object (m_pRegistry, m_dwUserData) that
// retail's Load*/Save* bodies build on the stack, hand-rolled the way
// featurepack/menu/CMFCPopupMenuBar.cpp::OnCreate and
// featurepack/docking/CPane.cpp::SaveState do it: Create() stores the
// CSettingsStore it allocates in slot 0, and the destructor releases it
// through CObject's virtual destructor (retail: the deleting destructor at
// vslot 0x08 with flag 1, e.g. at 0x1585f4 inside LoadLargeIconsState).
struct SettingsStoreSP {
    void* slots[2] = { nullptr, nullptr };
    void* Create(int bAdmin, int bReadOnly)
    {
        return impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(slots, bAdmin, bReadOnly);
    }
    ~SettingsStoreSP()
    {
        if (slots[0]) delete static_cast<CObject*>(slots[0]);
    }
};

// AFXGetRegPath(L"MFCToolBars", lpszProfileName) followed by
// Format(L"%TsMFCToolBarParameters", strPath) -- the key that LoadParameters,
// LoadLargeIconsState and SaveParameters all open (literals 0x342a60 /
// 0x342ad8 in mfc140u).
inline CString ToolBarParametersKey(const wchar_t* lpszProfileName)
{
    CString strPath;
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        &strPath, L"MFCToolBars", lpszProfileName);
    CString strKey;
    strKey.Format(L"%sMFCToolBarParameters", strPath.GetString());
    return strKey;
}

// CMFCToolBarImages::m_sizeImage is protected (include/openmfc/afxmfc.h pins
// it at +0x68); SetMenuSizes and SetLockedSizes store straight into it the way
// the retail bodies do (`mov %rdx,0x68(<images>)`).
constexpr int kImagesSizeImage = 0x68;
inline void SetImagesSizeImage(CMFCToolBarImages* pImages, long cx, long cy)
{
    long* p = reinterpret_cast<long*>(reinterpret_cast<char*>(pImages) + kImagesSizeImage);
    p[0] = cx;
    p[1] = cy;
}

// (int)(v * m_dblLargeImageRatio + 0.5) with cvttsd2si truncation -- the
// large-icon scaling every retail Set*Sizes / SetLargeIcons body inlines (the
// 0.5 is the .rdata double at 0x34fef0, mfc140u; the ratio is
// ?m_dblLargeImageRatio@CMFCToolBar@@2NA, 2.0 in retail .data 0x3b1b58).
inline int ScaleLarge(long v)
{
    return static_cast<int>(static_cast<double>(v) * impl__m_dblLargeImageRatio_CMFCToolBar__2NA + 0.5);
}

// ---------------------------------------------------------------------------
// CMFCToolBar::m_AccelKeys, in retail layout.
//
// afxmfc.h models the member as an opaque 56-byte block; retail's is an
// afxtempl.h CMap<UINT, UINT, CMFCToolBarButton*, CMFCToolBarButton*&>.  Its
// layout is harvested from the CMFCToolBar constructor (RVA 0x14d2b0,
// mfc140u; the seven stores at 0x14d476..0x14d4a3):
//   +0x00 vfptr (0x315220)   +0x08 m_pHashTable = NULL   +0x10 m_nHashTableSize = 17
//   +0x18 m_nCount = 0       +0x20 m_pFreeList = NULL    +0x28 m_pBlocks = NULL
//   +0x30 m_nBlockSize = 10
// and its CAssoc from the SetAt instantiation RebuildAccelerationKeys calls
// (RVA 0x15bbe8, mfc140u): key +0, value +8, pNext +0x10, nHashValue +0x18,
// sizeof 0x20 (the CPlex::Create call there passes 0x20).  The hash is the
// inlined GetAssocAt at RVA 0xbfe24 (mfc140u):
//     ldiv(key, 127773); h = 16807*rem - 2836*quot; if (h < 0) h += 0x7fffffff;
//     bucket = h % m_nHashTableSize; walk the chain comparing nHashValue, then key
// This is the same shape phase4/src/featurepack/menu/CMFCPopupMenuBar.cpp
// decoded for its OnKey accelerator lookup, which reads this block by these
// offsets -- so it is populated in exactly this layout.  The vfptr is left
// NULL (OpenMFC has no CMap vftable); nothing in this DLL dispatches through it.
struct AccelAssoc {
    UINT               key;         // +0x00
    CMFCToolBarButton* value;       // +0x08
    AccelAssoc*        pNext;       // +0x10
    UINT               nHashValue;  // +0x18 (+4 pad)
};
struct AccelMap {
    void*        vfptr;             // +0x00
    AccelAssoc** m_pHashTable;      // +0x08
    UINT         m_nHashTableSize;  // +0x10 (+4 pad)
    INT_PTR      m_nCount;          // +0x18
    AccelAssoc*  m_pFreeList;       // +0x20
    CPlex*       m_pBlocks;         // +0x28
    INT_PTR      m_nBlockSize;      // +0x30
};
static_assert(sizeof(AccelAssoc) == 0x20, "retail CAssoc is 32 bytes");
static_assert(offsetof(AccelAssoc, value) == 0x08 && offsetof(AccelAssoc, pNext) == 0x10 &&
              offsetof(AccelAssoc, nHashValue) == 0x18, "retail CAssoc layout");
static_assert(sizeof(AccelMap) == 0x38, "retail CMap is 56 bytes");
static_assert(offsetof(AccelMap, m_pHashTable) == 0x08 && offsetof(AccelMap, m_nHashTableSize) == 0x10 &&
              offsetof(AccelMap, m_nCount) == 0x18 && offsetof(AccelMap, m_pFreeList) == 0x20 &&
              offsetof(AccelMap, m_pBlocks) == 0x28 && offsetof(AccelMap, m_nBlockSize) == 0x30,
              "retail CMap layout");
static_assert(offsetof(CMFCToolBar, m_AccelKeys) == 0x1318, "m_AccelKeys at 0x1318");
static_assert(sizeof(CMFCToolBar::m_AccelKeys) == sizeof(AccelMap), "m_AccelKeys block is one CMap");
constexpr UINT    kAccelHashTableSize = 17;
constexpr INT_PTR kAccelBlockSize     = 10;

inline AccelMap* AccelKeysOf(CMFCToolBar* pThis)
{
    return reinterpret_cast<AccelMap*>(pThis->m_AccelKeys);
}

// The retail constructor state (see above); called from CMFCToolBar's ctor
// after the member block is zeroed.
inline void AccelMapConstruct(AccelMap* m)
{
    m->vfptr = nullptr;
    m->m_pHashTable = nullptr;
    m->m_nHashTableSize = kAccelHashTableSize;
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    m->m_pBlocks = nullptr;
    m->m_nBlockSize = kAccelBlockSize;
}

// The Park-Miller hash from RVA 0xbfe24 (mfc140u), see above.
inline UINT AccelHashKey(UINT key)
{
    const std::int32_t k    = static_cast<std::int32_t>(key);
    const std::int32_t quot = k / 127773;
    const std::int32_t rem  = k % 127773;
    std::int32_t h = static_cast<std::int32_t>(16807u * static_cast<std::uint32_t>(rem)) -
                     static_cast<std::int32_t>(2836u * static_cast<std::uint32_t>(quot));
    if (h < 0) h += 0x7fffffff;
    return static_cast<UINT>(h);
}

// GetAssocAt (RVA 0xbfe24, mfc140u).  Retail divides by m_nHashTableSize
// before testing m_pHashTable; a zero size is guarded here instead.
inline AccelAssoc* AccelGetAssocAt(const AccelMap* m, UINT key, UINT& nHash, UINT& nBucket)
{
    nHash = AccelHashKey(key);
    if (m->m_nHashTableSize == 0) { nBucket = 0; return nullptr; }
    nBucket = nHash % m->m_nHashTableSize;
    if (!m->m_pHashTable) return nullptr;
    for (AccelAssoc* p = m->m_pHashTable[nBucket]; p; p = p->pNext) {
        if (p->nHashValue == nHash && p->key == key) return p;
    }
    return nullptr;
}

// CMap::RemoveAll, the ICF-shared body at RVA 0x1ba40 (mfc140u, exported
// under the name ?RemoveAll@CMapPtrToPtr@@QEAAXXZ), which the constructor's
// counterpart in ~CMFCToolBar (0x14d6b0, at 0x14d759) calls on this block:
//     if (m_pHashTable) { free(m_pHashTable); m_pHashTable = NULL; }
//     m_nCount = 0;  m_pFreeList = NULL;
//     for (p = m_pBlocks; p; p = next) free(p);       // CPlex::FreeDataChain
//     m_pBlocks = NULL;
// The blocks come from the exported CPlex::Create, so they are released with
// the matching exported FreeDataChain (this tree's CPlex pairs new[]/delete[]).
inline void AccelMapRemoveAll(AccelMap* m)
{
    if (m->m_pHashTable) {
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    if (m->m_pBlocks) {
        impl__FreeDataChain_CPlex__QEAAXXZ(m->m_pBlocks);
        m->m_pBlocks = nullptr;
    }
}

// CMap::SetAt (RVA 0x15bbe8, mfc140u), fully transcribed:
//     if ((pAssoc = GetAssocAt(key, nHash, nBucket)) == NULL) {
//         if (m_pHashTable == NULL) {                       // InitHashTable(m_nHashTableSize)
//             m_pHashTable = new CAssoc*[m_nHashTableSize]; memset(..., 0);
//         }
//         if (m_pFreeList == NULL) {                        // NewAssoc(): refill from a CPlex block
//             CPlex* p = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CAssoc) /*0x20*/);
//             CAssoc* a = (CAssoc*)p->data() + m_nBlockSize - 1;
//             for (i = m_nBlockSize - 1; i >= 0; i--, a--) { a->pNext = m_pFreeList; m_pFreeList = a; }
//         }
//         pAssoc = m_pFreeList;  next = pAssoc->pNext;  memset(pAssoc, 0, 0x20);  pAssoc->pNext = next;
//         m_nCount++;  m_pFreeList = m_pFreeList->pNext;
//         pAssoc->key = key;  pAssoc->nHashValue = nHash;
//         pAssoc->pNext = m_pHashTable[nBucket];  m_pHashTable[nBucket] = pAssoc;
//     }
//     pAssoc->value = value;
// (Allocation failures throw AfxThrowInvalidArgException in retail; here they
// leave the map unchanged.)
inline void AccelMapSetAt(AccelMap* m, UINT key, CMFCToolBarButton* value)
{
    UINT nHash = 0, nBucket = 0;
    AccelAssoc* pAssoc = AccelGetAssocAt(m, key, nHash, nBucket);
    if (!pAssoc) {
        if (m->m_nHashTableSize == 0) return;
        if (!m->m_pHashTable) {
            m->m_pHashTable = static_cast<AccelAssoc**>(std::calloc(m->m_nHashTableSize, sizeof(AccelAssoc*)));
            if (!m->m_pHashTable) return;
        }
        if (!m->m_pFreeList) {
            const INT_PTR nBlock = m->m_nBlockSize > 0 ? m->m_nBlockSize : kAccelBlockSize;
            CPlex* pPlex = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
                &m->m_pBlocks, static_cast<unsigned long long>(nBlock), sizeof(AccelAssoc));
            if (!pPlex) return;
            // CPlex::data() is this + 1, i.e. the 8-byte header (detail/CPlexSupport.h).
            AccelAssoc* a = reinterpret_cast<AccelAssoc*>(reinterpret_cast<char*>(pPlex) + 8) + (nBlock - 1);
            for (INT_PTR i = nBlock - 1; i >= 0; --i, --a) {
                a->pNext = m->m_pFreeList;
                m->m_pFreeList = a;
            }
        }
        pAssoc = m->m_pFreeList;
        AccelAssoc* pNextFree = pAssoc->pNext;
        std::memset(pAssoc, 0, sizeof(AccelAssoc));
        m->m_pFreeList = pNextFree;
        ++m->m_nCount;
        pAssoc->key = key;
        pAssoc->nHashValue = nHash;
        pAssoc->pNext = m->m_pHashTable[nBucket];
        m->m_pHashTable[nBucket] = pAssoc;
    }
    pAssoc->value = value;
}

// ---------------------------------------------------------------------------
// The two static CList<UINT,UINT> command lists, in retail layout.
//
// ?m_lstBasicCommands@ (0x3b2020, mfc140u) and ?m_lstUnpermittedCommands@
// (0x3b2058) are afxtempl.h CList<UINT,UINT> objects: the .data image bytes
// hold the CList vfptr at +0 and m_nBlockSize = 10 at +0x30, everything else
// zero -- the same {vfptr, m_pNodeHead, m_pNodeTail, m_nCount, m_pNodeFree,
// m_pBlocks, m_nBlockSize} shape include/openmfc/afx.h pins for CObList.  The
// node comes from the NewNode instantiation at RVA 0x121f8 (mfc140u), which
// passes 0x18 to CPlex::Create: pNext +0, pPrev +8, data +0x10.
// The retail writers are all in this file (SetBasicCommands /
// SetNonPermittedCommands / AddBasicCommand), but the lists are READ by
// other retail classes through inlined CList code -- a scan of mfc140u for
// references to 0x3b2020..0x3b2088 finds, besides this file's Serialize
// (0x3b2060 = the m_pNodeHead of ?m_lstUnpermittedCommands@) and
// InsertButton (0x14eadf), sites inside CKeyboardManager::LoadAcceleratorState
// (0x73993), CMFCPopupMenuBar::ImportFromMenu (0xbd497),
// CMFCToolBarsCustomizeDialog::AddButton (0x17781f), two non-exported bodies
// (entries 0x88620 and 0x17eca0; the latter reads the m_nCount at 0x3b2038
// right after CDialog::OnInitDialog, 0x208d10) and the two atexit destructor
// thunks at 0x2c5710 / 0x2c5730 (vfptr store + RemoveAll) -- and an MSVC client
// that receives the GetBasicCommands reference walks it with its own inlined
// CList code too, so the layout is load-bearing and is kept exactly.  The
// vfptr stays NULL.
struct UIntListNode {
    UIntListNode* pNext;   // +0x00
    UIntListNode* pPrev;   // +0x08
    UINT          data;    // +0x10 (+4 pad)
};
struct UIntList {
    void*         vfptr;         // +0x00
    UIntListNode* m_pNodeHead;   // +0x08
    UIntListNode* m_pNodeTail;   // +0x10
    INT_PTR       m_nCount;      // +0x18
    UIntListNode* m_pNodeFree;   // +0x20
    CPlex*        m_pBlocks;     // +0x28
    INT_PTR       m_nBlockSize;  // +0x30
};
static_assert(sizeof(UIntListNode) == 0x18, "retail CList<UINT,UINT>::CNode is 24 bytes");
static_assert(sizeof(UIntList) == 56, "retail CList<UINT,UINT> is 56 bytes");
static_assert(offsetof(UIntList, m_pNodeHead) == 0x08 && offsetof(UIntList, m_pNodeTail) == 0x10 &&
              offsetof(UIntList, m_nCount) == 0x18 && offsetof(UIntList, m_pNodeFree) == 0x20 &&
              offsetof(UIntList, m_pBlocks) == 0x28 && offsetof(UIntList, m_nBlockSize) == 0x30,
              "retail CList layout");
constexpr INT_PTR kUIntListBlockSize = 10;

// CList<UINT,UINT>::NewNode (RVA 0x121f8, mfc140u), fully transcribed:
//     if (m_pNodeFree == NULL) {
//         CPlex* p = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CNode) /*0x18*/);
//         CNode* n = (CNode*)p->data() + m_nBlockSize - 1;
//         for (i = m_nBlockSize - 1; i >= 0; i--, n--) { n->pNext = m_pNodeFree; m_pNodeFree = n; }
//     }
//     CNode* n = m_pNodeFree;  if (!n) AfxThrowInvalidArgException();
//     m_pNodeFree = n->pNext;  n->pPrev = pPrev;  n->pNext = pNext;  m_nCount++;
//     return n;
inline UIntListNode* UIntListNewNode(UIntList* l, UIntListNode* pPrev, UIntListNode* pNext)
{
    if (!l->m_pNodeFree) {
        const INT_PTR nBlock = l->m_nBlockSize > 0 ? l->m_nBlockSize : kUIntListBlockSize;
        CPlex* pPlex = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
            &l->m_pBlocks, static_cast<unsigned long long>(nBlock), sizeof(UIntListNode));
        if (!pPlex) return nullptr;
        UIntListNode* n = reinterpret_cast<UIntListNode*>(reinterpret_cast<char*>(pPlex) + 8) + (nBlock - 1);
        for (INT_PTR i = nBlock - 1; i >= 0; --i, --n) {
            n->pNext = l->m_pNodeFree;
            l->m_pNodeFree = n;
        }
    }
    UIntListNode* n = l->m_pNodeFree;
    if (!n) return nullptr;
    l->m_pNodeFree = n->pNext;
    n->pPrev = pPrev;
    n->pNext = pNext;
    ++l->m_nCount;
    return n;
}

// CList<UINT,UINT>::AddTail (RVA 0x12284, mfc140u), fully transcribed:
//     CNode* n = NewNode(m_pNodeTail, NULL);  n->data = v;
//     if (m_pNodeTail) m_pNodeTail->pNext = n; else m_pNodeHead = n;
//     m_pNodeTail = n;
inline void UIntListAddTail(UIntList* l, UINT v)
{
    UIntListNode* n = UIntListNewNode(l, l->m_pNodeTail, nullptr);
    if (!n) return;
    n->data = v;
    if (l->m_pNodeTail) l->m_pNodeTail->pNext = n; else l->m_pNodeHead = n;
    l->m_pNodeTail = n;
}

// CList<UINT,UINT>::Find (RVA 0x11f1c, mfc140u): a head-to-tail scan
// comparing data (+0x10); returns the node or NULL.
inline UIntListNode* UIntListFind(const UIntList* l, UINT v)
{
    for (UIntListNode* n = l->m_pNodeHead; n; n = n->pNext) {
        if (n->data == v) return n;
    }
    return nullptr;
}

// CList::RemoveAll, the ICF-shared body at RVA 0x8350 (mfc140u, exported as
// ?RemoveAll@CObList@@QEAAXXZ):
//     m_nCount = 0;  m_pNodeFree = m_pNodeTail = m_pNodeHead = NULL;
//     for (p = m_pBlocks; p; p = next) free(p);   m_pBlocks = NULL;
// Blocks are released through the exported CPlex::FreeDataChain, which pairs
// with this tree's CPlex::Create allocation.
inline void UIntListRemoveAll(UIntList* l)
{
    l->m_nCount = 0;
    l->m_pNodeFree = nullptr;
    l->m_pNodeTail = nullptr;
    l->m_pNodeHead = nullptr;
    if (l->m_pBlocks) {
        impl__FreeDataChain_CPlex__QEAAXXZ(l->m_pBlocks);
        l->m_pBlocks = nullptr;
    }
}

// SetBasicCommands / SetNonPermittedCommands (RVA 0x156f50 / 0x156f00,
// mfc140u) share one body shape, fully transcribed:
//     lst.RemoveAll();                                          // 0x8350
//     if (&lstCommands == NULL) AfxThrowInvalidArgException();  // after the RemoveAll
//     for (node = lstCommands.m_pNodeHead; node; node = node->pNext)
//         lst.AddTail(node->data);                              // 0x12284
// The source is the caller's CList<UINT,UINT>, read by the retail node layout
// above (head at +8, data at +0x10, pNext at +0).  The NULL-reference throw
// becomes an early return, after the RemoveAll exactly as in retail.
inline void UIntListAssignFrom(UIntList* lst, const void* pSrcList)
{
    UIntListRemoveAll(lst);
    if (!pSrcList) return;
    const UIntList* src = static_cast<const UIntList*>(pSrcList);
    for (const UIntListNode* n = src->m_pNodeHead; n; n = n->pNext) {
        UIntListAddTail(lst, n->data);
    }
}
} // namespace

//=============================================================================
// CMFCToolBar::OnSetFocus
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSettingChange
//=============================================================================

//=============================================================================
// CMFCToolBar::OnShowWindow
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSize
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSysColorChange
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolHitTest
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarAppearance
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarDelete
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarImage
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarImageAndText
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarNewMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarReset
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarStartGroup
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarText
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUpdateCmdUI
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUpdateToolTips
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUserToolTip
//=============================================================================

//=============================================================================
// CMFCToolBar::OnWindowPosChanged
//=============================================================================

//=============================================================================
// CMFCToolBar::OnWindowPosChanging
//=============================================================================

//=============================================================================
// CMFCToolBar::PreTranslateMessage
//=============================================================================

//=============================================================================
// CMFCToolBar::PrevMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::ProcessCommand
//=============================================================================

//=============================================================================
// CMFCToolBar::RebuildAccelerationKeys
//=============================================================================

//=============================================================================
// CMFCToolBar::RedrawCustomizeButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RedrawUnderlines (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveAllButtons
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveResetStateButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveStateFromRegistry
//=============================================================================

//=============================================================================
// CMFCToolBar::ResetAll (static)
//=============================================================================
// CMFCToolBar methods enabled by the corrected retail layout (see the layout
// chain rebuild: CBasePane 424 / CPane 1016 / CMFCToolBar 4944). Each function
// below was decoded from the retail mfc140u export (ordinal -> RVA -> objdump),
// and the member offsets the shipping code touches are exactly the ones
// transcribed into include/openmfc/afxmfc.h:
//
//   [rcx+0x10b8] 4280 m_bLocked          [rcx+0x1134] 4404 m_iButtonCapture
//   [rcx+0x10bc] 4284 m_bLargeIconsAreEnbaled
//   [rcx+0x10c8] 4296 m_bTextLabels      [rcx+0x1138] 4408 m_iHighlighted
//   [rcx+0x10ec] 4332 m_bMenuMode        [rcx+0x12f8] 4856 m_pBrotherToolBar
//   [rcx+0x0130]  304 m_pDockBarRow (inherited from CBasePane)
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMFCToolBar::m_bCustomizeMode is a protected static; it already has storage
// in feature_static_data.cpp under its exported name.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;






// Implementations this unit calls that are defined with their own class.
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;
extern "C" void* impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA;

// Retail (0x18014fe50):
//     if (iButton < 0 || iButton >= [0x11a0] /* m_Buttons.m_nCount */) return 0;
//     rect = 0;
//     vcall slot 0x6f0 (this, &rect, iButton);        // GetButtonRect(iButton,&rect)
//     pButton = GetButton(iButton);
//     if (pButton == m_pCustomizeBtn) { rect.right += 10; rect.bottom += 10; }
//     InvalidateRect(m_hWnd, &rect, TRUE);
//     if (pButton == m_pCustomizeBtn) { extra text-size invalidate from the
//       customize button's extended members at 0x12c/0x150 -- not modeled }
//     return pButton;
// Slot 0x6f0 and the extended CMFCToolBarCustomizeButton members are not modeled
// in the OpenMFC headers, so the button's own m_rect stands in for the rect.
// Symbol: ?InvalidateButton@CMFCToolBar@@QEAAPEAVCMFCToolBarButton@@H@Z
extern "C" CMFCToolBarButton* MS_ABI
impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
    CMFCToolBar* pThis, int iButton)
{
    if (!pThis) return nullptr;
    if (iButton < 0 || iButton >= pThis->GetCount()) return nullptr;

    CMFCToolBarButton* pButton = pThis->GetButton(iButton);

    CRect rect(0, 0, 0, 0);
    if (pButton) {
        rect = pButton->m_rect;
        if (pButton == pThis->m_pCustomizeBtn) {
            rect.right += 10;
            rect.bottom += 10;
        }
    }
    pThis->InvalidateRect((const struct tagRECT*)&rect, TRUE);
    return pButton;
}
// Retail (0x180158c60): returns TRUE only for commands inside three hard-coded
// ID windows AND not present in two internal command lists (0x1803b2020 /
// 0x1803b1fd0). The lists are internal globals OpenMFC does not model; with them
// empty, FALSE is the honest answer for every command, so only the
// customize-mode gate is kept.
// Symbol: ?IsCommandRarelyUsed@CMFCToolBar@@SAHI@Z
extern "C" int MS_ABI impl__IsCommandRarelyUsed_CMFCToolBar__SAHI_Z(unsigned int nCmdID)
{
    (void)nCmdID;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return FALSE;
    // TODO(clean-room): partially transcribed -- internal command lists skipped.
    return FALSE;
}
// Retail (0x180156960): pButton->vslot0x80 -> IsWindow() check, then compares
// the button against the global last-command info (0x18028ac10). Not modeled.
// Symbol: ?IsLastCommandFromButton@CMFCToolBar@@SAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__IsLastCommandFromButton_CMFCToolBar__SAHPEAVCMFCToolBarButton___Z(
    CMFCToolBarButton* pButton)
{
    if (!pButton) return FALSE;
    // TODO(clean-room): not transcribed -- needs the global last-command state.
    return FALSE;
}
// Retail (0x1801596a0), transcribed:
//     pFrame = *(CFrameWnd**)0x1803be1b8;           // cached top-level frame
//     if (pFrame == NULL) { pFrame = GetTopLevelFrame();   // 0x18028e490
//                           if (pFrame == NULL) return FALSE; }
//     if      (pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))  pImpl = pFrame + 0x590;
//     else if (pFrame->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))     pImpl = pFrame + 0x1f0;
//     else return FALSE;
//     return CFrameImpl::IsUserDefinedToolbar(pImpl, this);      // 0x180062840
// (RTTI descriptors 0x1802ee478 / 0x1802e9210 read back as "CMDIFrameWndEx" /
// "CFrameWndEx"; 0x180062840 is
// ?IsUserDefinedToolbar@CFrameImpl@@IEBAHPEBVCMFCToolBar@@@Z.)
// (Re-read on mfc140u at RVA 0x1596a0: the global is 0x3be1b8, GetTopLevelFrame
// is 0x28e490, the descriptors are 0x2ee478 / 0x2e9210 -- both read back with
// those names and object sizes 0x4710 / 0x6f0 -- and the callee is 0x62840.)
//
// Implemented with two deviations:
//  * the cached-frame global at 0x3be1b8 is not modelled, so GetTopLevelFrame()
//    is always asked.  Retail only consults the cache first; when the cache is
//    empty it does exactly this, so the difference is confined to sessions
//    where the cache and the real top-level frame disagree;
//  * OpenMFC's CFrameWndEx / CMDIFrameWndEx do not declare the CFrameImpl
//    member, so its address is formed from the retail offsets (+0x1f0 / +0x590)
//    the way an MSVC client compiled against the shipping afxframewndex.h /
//    afxmdiframewndex.h forms it when it calls CFrameImpl::InitUserToolbars
//    through the inline CFrameWndEx::InitUserToolbars.  The address is only
//    used as a key: phase4/src/core/frame/CFrameImpl.cpp keeps the
//    InitUserToolbars range in a side table keyed by the CFrameImpl address and
//    never dereferences it, so the frame object itself is not touched.
// Symbol: ?IsUserDefined@CMFCToolBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;

    // TODO(clean-room): the cached top-level-frame global (0x3be1b8) is not
    // modelled; GetTopLevelFrame() is the retail fallback for an empty cache.
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (!pFrame) return FALSE;

    const char* pImpl = nullptr;
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        pImpl = reinterpret_cast<const char*>(pFrame) + 0x590;   // CMDIFrameWndEx::m_Impl
    } else if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        pImpl = reinterpret_cast<const char*>(pFrame) + 0x1f0;   // CFrameWndEx::m_Impl
    } else {
        return FALSE;
    }
    return impl__IsUserDefinedToolbar_CFrameImpl__IEBAHPEBVCMFCToolBar___Z(pImpl, pThis);
}
// Retail (0x18014e360):
//     m_bLocked = bLocked;
//     locked path (bLocked != 0) maps the eight CMFCToolBarInfo resource ids
//     onto the toolbar's own member image collections:
//         m_ImagesLocked            <- info.m_uiHotResID          (always loaded)
//         m_ColdImagesLocked        <- info.m_uiColdResID         (or copied from
//                                       m_ImagesLocked when 0 and the global
//                                       image-count flag is set)
//         m_DisabledImagesLocked    <- info.m_uiDisabledResID
//         m_LargeImagesLocked       <- info.m_uiLargeColdResID
//         m_LargeColdImagesLocked   <- info.m_uiLargeHotResID
//         m_LargeDisabledImagesLocked <- info.m_uiLargeDisabledResID
//         m_MenuImagesLocked        <- info.m_uiMenuResID
//         m_MenuImagesLocked        <- info.m_uiMenuResID again when the menu
//                                       disabled id is set (retail quirk)
//     any failed Load returns 0. The unlocked path (bLocked == 0) targets the
//     static image objects 0x1803c25a0..0x1803c3100 which are not modeled, so it
//     reports success without loading.
// Symbol: ?LoadBitmapEx@CMFCToolBar@@UEAAHAEAVCMFCToolBarInfo@@H@Z
extern "C" int MS_ABI impl__LoadBitmapEx_CMFCToolBar__UEAAHAEAVCMFCToolBarInfo__H_Z(
    CMFCToolBar* pThis, CMFCToolBarInfo& info, int bLocked)
{
    if (!pThis) return 0;
    pThis->m_bLocked = bLocked;

    if (!bLocked) {
        // TODO(clean-room): partially transcribed -- the unlocked path uses the
        // static image objects 0x1803c25a0.. which are not modeled.
        return TRUE;
    }

    if (!pThis->m_ImagesLocked.Load(info.m_uiHotResID, nullptr, TRUE)) return 0;

    if (info.m_uiColdResID != 0) {
        if (!pThis->m_ColdImagesLocked.Load(info.m_uiColdResID, nullptr, TRUE)) return 0;
    } else {
        // TODO(clean-room): retail copies m_ImagesLocked -> m_ColdImagesLocked
        // and sets its image size when the global default count is nonzero;
        // the globals 0x1803be358/0x1803be34c are not modeled.
    }

    if (info.m_uiDisabledResID != 0 &&
        !pThis->m_DisabledImagesLocked.Load(info.m_uiDisabledResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeColdResID != 0 &&
        !pThis->m_LargeImagesLocked.Load(info.m_uiLargeColdResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeHotResID != 0 &&
        !pThis->m_LargeColdImagesLocked.Load(info.m_uiLargeHotResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeDisabledResID != 0 &&
        !pThis->m_LargeDisabledImagesLocked.Load(info.m_uiLargeDisabledResID, nullptr, TRUE)) return 0;

    if (info.m_uiMenuResID != 0 &&
        !pThis->m_MenuImagesLocked.Load(info.m_uiMenuResID, nullptr, TRUE)) return 0;

    if (info.m_uiMenuDisabledResID != 0 &&
        !pThis->m_MenuImagesLocked.Load(info.m_uiMenuResID, nullptr, TRUE)) return 0;

    return TRUE;
}
// Retail (0x18014e2b0): packs the six arguments into a stack CMFCToolBarInfo
//     cold = p1  hot = p0  disabled = p4  large* = 0  menu = p2  menuDisabled = p5
// and tail-calls virtual LoadBitmapEx(this, &info, bLocked = p3).
// Symbol: ?LoadBitmapW@CMFCToolBar@@UEAAHIIIHII@Z
extern "C" int MS_ABI impl__LoadBitmapW_CMFCToolBar__UEAAHIIIHII_Z(
    CMFCToolBar* pThis, unsigned int p0, unsigned int p1, unsigned int p2,
    int p3, unsigned int p4, unsigned int p5)
{
    if (!pThis) return 0;
    CMFCToolBarInfo info = {};
    info.m_uiColdResID          = p1;
    info.m_uiHotResID           = p0;
    info.m_uiDisabledResID      = p4;
    info.m_uiMenuResID          = p2;
    info.m_uiMenuDisabledResID  = p5;
    return impl__LoadBitmapEx_CMFCToolBar__UEAAHAEAVCMFCToolBarInfo__H_Z(pThis, info, p3);
}
// Retail (0x180158520), transcribed:
//     strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);   // 0x1800d2070
//     strKey.Format(L"%TsMFCToolBarParameters", strPath);         // 0x18000da00
//     CSettingsStoreSP sp; CSettingsStore& reg = sp.Create(FALSE, TRUE); // 0x18012a550
//     if (!reg.Open(strKey)) return FALSE;                        // vslot 0x30
//     return reg.Read(L"LargeIcons", CMFCToolBar::m_bLargeIcons); // vslot 0xb8
// (The three wide literals are the ones at 0x180342a60 / 0x180342ad8 /
// 0x180342b40; the destination is ?m_bLargeIcons@CMFCToolBar@@1HA at
// 0x1803be368.)
// (Re-read on mfc140u at RVA 0x158520: same shape; Create is 0x12a550, the
// CSettingsStoreSP owner is the two QWORDs at rsp+0x20/0x28 and its store is
// released through the deleting destructor at vslot 0x08 on the way out.
// Slots 0x30 / 0xb8 of the CSettingsStore vftable at 0x30e610 (mfc140u) are
// ?Open@ and ?Read@CSettingsStore@@UEAAHPEB_WAEAH@Z.)
// Implemented through the exported CSettingsStore / CSettingsStoreSP thunks
// with the hand-rolled owner object (SettingsStoreSP near the top of this
// file; the same pattern as featurepack/menu/CMFCPopupMenuBar.cpp).
// Deviation: OpenMFC's CSettingsStore is an in-memory map, so the value is
// only found if SaveParameters ran earlier in this process.
// Symbol: ?LoadLargeIconsState@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    const CString strKey = ToolBarParametersKey(lpszProfileName);

    SettingsStoreSP sp;
    void* pStore = sp.Create(FALSE, TRUE);
    if (!pStore) return FALSE;
    if (!impl__Open_CSettingsStore__UEAAHPEB_W_Z(pStore, strKey.GetString())) return FALSE;

    // Read(LPCTSTR, int&) straight into the static, as retail does.
    return impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(
        pStore, L"LargeIcons", &impl__m_bLargeIcons_CMFCToolBar__1HA);
}
// Retail (0x18015a660), transcribed:
//     CObList lst;
//     BOOL bRet = FALSE;
//     if (store.Read(L"OriginalItems", lst))      // vslot 0x90, literal 0x180342c08
//         bRet = SmartUpdate(lst);                // vslot 0x870
//     while (lst not empty) delete lst.RemoveHead();
//     return bRet;
// (Slot 0x870 of the retail mfc140u CMFCToolBar vtable at 0x1803157c8 is
// ?SmartUpdate@CMFCToolBar@@MEAAHAEBVCObList@@@Z; the read value name is
// "OriginalItems", not the class name.)
// Not implemented.  Correcting an earlier revision of this comment: OpenMFC
// DOES model CSettingsStore -- phase4/src/core/app/CSettingsStore.cpp exports
// the whole Read family, including
// ?Read@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z, as impl__ thunks, and the
// class is forward-declared for this very signature in
// phase4/src/detail/CMFCToolBarSupport.h.  The real blockers are that that
// CObject& overload only looks a pointer up in an in-memory map and never
// deserialises anything into the reference (see its body, which discards what
// it reads), that nothing in OpenMFC ever writes an "OriginalItems" value, and
// that CMFCToolBar::SmartUpdate is itself a stub -- so the transcription would
// hand an empty CObList to a no-op.
// Symbol: ?LoadLastOriginalState@CMFCToolBar@@MEAAHAEAVCSettingsStore@@@Z
extern "C" int MS_ABI impl__LoadLastOriginalState_CMFCToolBar__MEAAHAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    (void)pThis; (void)store;
    // TODO(clean-room): not transcribed -- CSettingsStore + serialized buttons.
    return 0;
}
// Retail (RVA 0x1582b0, mfc140u), fully transcribed:
//     CString strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);        // 0xd2070
//     CString strKey;  strKey.Format(L"%TsMFCToolBarParameters", strPath);     // 0xda00
//     CSettingsStoreSP sp;  CSettingsStore& reg = sp.Create(FALSE, TRUE);      // 0x12a550
//     if (!reg.Open(strKey)) return FALSE;                                     // vslot 0x30
//     int nAnim = 0;
//     BOOL bRet = reg.Read(L"Tooltips",               m_bShowTooltips)        // vslot 0xb8, 0x3b1b64
//              && reg.Read(L"ShortcutKeys",           m_bShowShortcutKeys)    // 0x3b1b60
//              && reg.Read(L"LargeIcons",             m_bLargeIcons)          // 0x3be368
//              && reg.Read(L"MenuAnimation",          nAnim)
//              && reg.Read(L"RecentlyUsedMenus",      CMFCMenuBar::m_bRecentlyUsedMenus)   // 0x3b1b08
//              && reg.Read(L"MenuShadows",            CMFCMenuBar::m_bMenuShadows)         // 0x3b1b00
//              && reg.Read(L"ShowAllMenusAfterDelay", CMFCMenuBar::m_bShowAllMenusDelay)   // 0x3b1b04
//              && reg.Read(L"CommandsUsage",          m_UsageCount);          // vslot 0x90 (CObject&), 0x3b1fd0
//     CMFCPopupMenu::m_AnimationType = nAnim;   // 0x3be290 -- unconditional once Open succeeded
//     SetLargeIcons(m_bLargeIcons);             // 0x158990 -- likewise unconditional
//     return bRet;                              // then the owner's deleting-dtor release
// (Slots 0x30 / 0xb8 / 0x90 of the CSettingsStore vftable at 0x30e610 are Open,
// Read(LPCTSTR,int&) and Read(LPCTSTR,CObject&); the reads write the statics
// in place, so a failure part-way leaves the earlier ones updated.)
// Deviations: the CSettingsStoreSP owner is hand-rolled (SettingsStoreSP near
// the top of this file); OpenMFC's CSettingsStore is an in-memory map, so the
// values exist only if SaveParameters ran in this process, and its
// Read(CObject&) overload only reports whether a value is stored -- it
// deserialises nothing into m_UsageCount.
// Symbol: ?LoadParameters@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadParameters_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    const CString strKey = ToolBarParametersKey(lpszProfileName);

    SettingsStoreSP sp;
    void* pStore = sp.Create(FALSE, TRUE);
    if (!pStore) return FALSE;
    if (!impl__Open_CSettingsStore__UEAAHPEB_W_Z(pStore, strKey.GetString())) return FALSE;

    // Each Read(LPCTSTR, int&) writes its target only on success, exactly as
    // retail's by-reference reads do, so the statics are passed directly.
    int nAnim = 0;
    const int bRet =
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"Tooltips", &impl__m_bShowTooltips_CMFCToolBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"ShortcutKeys", &impl__m_bShowShortcutKeys_CMFCToolBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"LargeIcons", &impl__m_bLargeIcons_CMFCToolBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"MenuAnimation", &nAnim) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"RecentlyUsedMenus", &impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"MenuShadows", &impl__m_bMenuShadows_CMFCMenuBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(pStore, L"ShowAllMenusAfterDelay", &impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA) &&
        impl__Read_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(
            pStore, L"CommandsUsage",
            reinterpret_cast<CObject*>(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A));
    impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A = static_cast<std::uint32_t>(nAnim);
    impl__SetLargeIcons_CMFCToolBar__SAXH_Z(impl__m_bLargeIcons_CMFCToolBar__1HA);
    return bRet ? TRUE : FALSE;
}
// Retail (0x18015af10), transcribed.  Note the direction: it REPLACES
// m_OrigResetButtons with what it read, it does not filter the existing list.
//     CObList lst;
//     if (store.Read(L"OrigResetItems", lst) && lst.GetCount() > 0) {  // vslot 0x90
//         while (m_OrigResetButtons.GetCount() != 0)                   // 0x11f8/0x1210
//             delete m_OrigResetButtons.RemoveHead();
//         for (p in lst)
//             if (p->IsKindOf(RUNTIME_CLASS(CMFCToolBarButton)))       // 0x1803b1628
//                 m_OrigResetButtons.AddTail(p);
//     }
//     lst.RemoveAll();
//     return TRUE;                       // unconditional -- even when Read failed
// (0x1803b1628 reads back as "CMFCToolBarButton"; 0x180231f40/0x180231e70/
// 0x180008350 are CObList::RemoveHead/AddTail/RemoveAll.)
// Not implemented.  Not because CSettingsStore is missing -- it is modelled,
// see the correction on LoadLastOriginalState above -- but because its
// ?Read@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z thunk never deserialises
// anything into the CObList it is handed, and nothing in OpenMFC writes an
// "OrigResetItems" value, so the read list would always be empty.  Retail's
// unconditional TRUE is deliberately not reproduced from an empty body: it
// would claim a load that did not happen, so FALSE is kept.
// Symbol: ?LoadResetOriginalState@CMFCToolBar@@IEAAHAEAVCSettingsStore@@@Z
extern "C" int MS_ABI impl__LoadResetOriginalState_CMFCToolBar__IEAAHAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    (void)pThis; (void)store;
    // TODO(clean-room): not transcribed.
    return 0;
}
// Retail (0x1801539c0), transcribed down to the key construction:
//     strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);
//     if (nIndex == -1) nIndex = GetDlgCtrlID();                 // 0x1802a99a0
//     strKey.Format(uiID == -1 ? L"%TsMFCToolBar-%d"
//                              : L"%TsMFCToolBar-%d%x", strPath, nIndex, uiID);
//     CSettingsStoreSP sp; CSettingsStore& reg = sp.Create(FALSE, TRUE);
//     if (!reg.Open(strKey)) return FALSE;                       // vslot 0x30
//     ... reg.Read(L"Buttons", <CObList>) and the remaining layout values ...
// (Literals at 0x180342a60 / 0x180342a78 / 0x180342aa0 / 0x180342ac8.)
// Not implemented.  CSettingsStore and CSettingsStoreSP are modelled (see the
// correction on LoadLargeIconsState above); what is missing is the serialised
// button list -- OpenMFC's CObject Read overload deserialises nothing -- and a
// SaveState that would have written the key in the first place.
// Symbol: ?LoadState@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID)
{
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    // TODO(clean-room): not transcribed -- registry state restoration.
    return 0;
}
// Retail (0x18014e650) locates the RT_TOOLBAR resource, parses the button array
// (FindResourceW/LoadResource/LockResource + the 0xf1/RT_TOOLBAR layout struct)
// and forwards to the image loader. OpenMFC's LoadToolBar stub accepts resources
// without parsing; the resource layout is not modeled here, but the original
// resource id is remembered (m_uiOriginalResID) so CanBeRestored/LoadState see
// the toolbar as loaded, and success is reported like the LoadToolBar stub does.
// Symbol: ?LoadToolBarEx@CMFCToolBar@@UEAAHIAEAVCMFCToolBarInfo@@H@Z
extern "C" int MS_ABI impl__LoadToolBarEx_CMFCToolBar__UEAAHIAEAVCMFCToolBarInfo__H_Z(
    CMFCToolBar* pThis, unsigned int uiResID, CMFCToolBarInfo& info, int bLocked)
{
    if (!pThis) return 0;
    if (uiResID == 0) return 0;
    (void)info; (void)bLocked;
    pThis->m_uiOriginalResID = uiResID;
    // TODO(clean-room): partially transcribed -- RT_TOOLBAR resource parsing and
    // button/bitmap setup not modeled.
    return TRUE;
}
// Retail (0x180157c30): after internal checks, scans forward from
// m_iSelected + 1 (wrapping at the m_Buttons count @0x11a0) for a menu button
// that IsKindOf(CMFCToolBarMenuButton) and whose m_nStyle does not carry the
// 0x40000 bit; then calls virtual slot 0x770 (OnChangeHot) with the found index
// and, when the new hot button is a dropped menu, posts WM_KEYDOWN(VK_MENU) to
// its parent. Returns 1 unless the internal highlight helper fails.
// Symbol: ?NextMenu@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__NextMenu_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return 0;

    const int iSelected = pThis->m_iSelected;
    if (iSelected < 0) return 0;

    const int nCount = pThis->GetCount();
    if (nCount == 0) return 1;

    int iFound = -1;
    int i = iSelected + 1;
    if (i >= nCount) i = 0;

    for (int scanned = 0; scanned < nCount; ++scanned) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&
            (pButton->m_nStyle & 0x40000) == 0) {
            iFound = i;
            break;
        }
        if (++i >= nCount) i = 0;
    }

    if (iFound == -1) return 1;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iFound);

    // TODO(clean-room): retail also posts WM_KEYDOWN(VK_MENU) to the hot menu
    // button's parent via extended CMFCToolBarMenuButton members; not modeled.
    return 1;
}
// Retail (0x180154b80):
//     if (nNotifyCmd == 0 && pButton->vslot0x90(nCommand)) return FALSE;
//     hwndParent = [this+0xa0] ? [this+0xa0]->m_hWnd : GetParent(m_hWnd);
//     PostMessageW(hwndParent, WM_COMMAND,
//                  LOWORD(pButton->m_nID) | (LOWORD(nCommand) << 16), lParam);
//     return TRUE;
// Slot 0x90 and the owner member at [this+0xa0] are not modeled; the button
// early-out is dropped and the parent is resolved with CWnd::GetParent().
// Symbol: ?NotifyControlCommand@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@HH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__HH_K_J_Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, int nNotifyCmd, int nCommand,
    unsigned long long wParam, long long lParam)
{
    (void)wParam; (void)nNotifyCmd;
    if (!pThis || !pButton) return 0;

    CWnd* pParent = pThis->GetParent();
    if (!pParent) return 0;

    ::PostMessageW(pParent->GetSafeHwnd(), WM_COMMAND,
                   MAKEWPARAM(LOWORD(pButton->m_nID), LOWORD(nCommand)),
                   static_cast<LPARAM>(lParam));
    return TRUE;
}
// Retail (RVA 0x154260, mfc140u), fully transcribed:
//     if (IsFloating()) AdjustLayout();            // vslot 0x2f0, then vslot 0x428
//     else AdjustSizeImmediate(TRUE);              // vslot 0x5b8 with edx = 1
// Slot 0x2f0 of the CMFCToolBar vftable (0x3157c8, mfc140u) is RVA 0x23a50,
// `mov 0x10d8(%rcx),%eax; ret` -- the inline CMFCToolBar::IsFloating reading
// m_bFloating -- and slot 0x5b8 is ?AdjustSizeImmediate@CPane@@UEAAXH@Z
// (0xa24a0).  An earlier revision of this comment called them GetButtonCount
// and SetDrawTextLabels and the body relayed out on a non-empty button list;
// that was not what retail does.
// Deviation: AdjustSizeImmediate is reached through the exported CPane thunk
// (a partial body in featurepack/docking/CPane.cpp) rather than virtual
// dispatch; IsFloating is inlined as the m_bFloating read.
// Symbol: ?OnAfterButtonDelete@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (pThis->m_bFloating) {
        pThis->AdjustLayout();
    } else {
        impl__AdjustSizeImmediate_CPane__UEAAXH_Z(pThis, TRUE);
    }
}
// Retail (0x18015a0a0):
//     m_bFloating = FALSE;
//     hwndParent = GetParent(m_hWnd);
//     pParentWnd = CWnd::FromHandlePermanent(hwndParent);
//     if (pParentWnd) {
//         GetWindowRect(m_hWnd, &rc);
//         pParentWnd->ScreenToClient(&rc);
//         RedrawWindow(pParentWnd->m_hWnd, &rc, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
//     }
// Symbol: ?OnAfterDock@CMFCToolBar@@MEAAXPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__OnAfterDock_CMFCToolBar__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CMFCToolBar* pThis, CBasePane* /*pDockBar*/, const RECT* /*lpRect*/, int /*dockMethod*/)
{
    if (!pThis) return;
    pThis->m_bFloating = FALSE;

    CWnd* pParent = pThis->GetParent();
    if (!pParent) return;

    RECT rc;
    ::GetWindowRect(pThis->GetSafeHwnd(), &rc);
    pParent->ScreenToClient(&rc);
    pParent->RedrawWindow(&rc, nullptr, 0x105);
}
// Retail (0x18015a030): calls base CBasePane::OnAfterFloat, runs two window-pos
// virtuals (slots 0x420/0x460, then 0x378 on the result) and finally stores
// m_bFloating = TRUE. The base call and the virtual chain are not modeled; the
// observable member write is reproduced.
// Symbol: ?OnAfterFloat@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterFloat_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    pThis->m_bFloating = TRUE;
    // TODO(clean-room): partially transcribed -- base CBasePane::OnAfterFloat and
    // the window-position virtual chain are not modeled.
}
// Retail (0x18015a140):
//     CBasePane::OnBeforeChangeParent(pWndNewParent, bDelay);
//     m_bFloating = (pWndNewParent != NULL &&
//                    pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)));
// (RTTI string "CPaneFrameWnd" is at 0x1803b1418 in the retail image.)
// Symbol: ?OnBeforeChangeParent@CMFCToolBar@@MEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CMFCToolBar__MEAAXPEAVCWnd__H_Z(
    CMFCToolBar* pThis, CWnd* pWndNewParent, int /*bDelay*/)
{
    if (!pThis) return;
    // TODO(clean-room): base CBasePane::OnBeforeChangeParent not modeled.
    pThis->m_bFloating =
        (pWndNewParent != nullptr && pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
            ? TRUE : FALSE;
}
// Retail (0x18015ac70):
//     GetClientRect(m_hWnd, &rcClient);
//     *pRect = button->m_rect;
//     if (button->m_bWrap && bHorz) {
//         pRect->left   = rcClient.left;
//         pRect->right  = rcClient.right;
//         pRect->top    = button->m_rect.bottom;
//         pRect->bottom = button->m_rect.bottom + 5;
//     }
// (The member reads are m_bWrap@0x14 and m_rect@0x68 in CMFCToolBarButton.)
// Symbol: ?OnCalcSeparatorRect@CMFCToolBar@@MEAAXPEAVCMFCToolBarButton@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnCalcSeparatorRect_CMFCToolBar__MEAAXPEAVCMFCToolBarButton__AEAVCRect__H_Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, CRect& rect, int bHorz)
{
    if (!pThis || !pButton) return;

    CRect rcClient;
    pThis->GetClientRect((struct tagRECT*)&rcClient);

    rect = pButton->m_rect;

    if (pButton->m_bWrap && bHorz) {
        rect.left   = rcClient.left;
        rect.right  = rcClient.right;
        rect.top    = pButton->m_rect.bottom;
        rect.bottom = pButton->m_rect.bottom + 5;
    }
}
// Retail (0x1801516b0):
//     CBasePane::OnCancelMode();
//     if (m_bStretchButton) {
//         m_pDragButton = NULL; m_bStretchButton = FALSE;
//         SetRectEmpty(&m_rectTrack); ReleaseCapture();
//         if (m_pWndLastCapture) { SetCapture(m_pWndLastCapture->m_hWnd);
//                                  m_pWndLastCapture = NULL; }
//     }
//     if (m_iButtonCapture >= 0) {
//         pButton = GetButton(m_iButtonCapture);
//         if (pButton) {
//             style = pButton->m_nStyle & ~(1<<17);   // btr $0x11
//             if (GetCapture() == m_hWnd) {
//                 ReleaseCapture();
//                 if (m_pWndLastCapture) { SetCapture(...); m_pWndLastCapture = NULL; }
//             }
//             vslot0x6f8 (this, m_iButtonCapture, style);  // SetButtonStyle
//         }
//     }
//     m_iButtonCapture = -1; m_iHighlighted = -1;
//     vslot0x770 (this);                 // OnChangeHot(-1)
//     for (pButton : m_Buttons) pButton->vslot0xc0();      // OnCancelMode()
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_FRAME); // 0x505
// Base call, SetButtonStyle and per-button virtuals are not modeled.
// Symbol: ?OnCancelMode@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    if (pThis->m_bStretchButton) {
        pThis->m_bStretchButton = FALSE;
        pThis->m_pDragButton = nullptr;
        pThis->m_rectTrack.SetRectEmpty();
        if (pThis->GetSafeHwnd()) ::ReleaseCapture();
        pThis->m_pWndLastCapture = nullptr;
    }

    if (pThis->m_iButtonCapture >= 0) {
        CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iButtonCapture);
        if (pButton) {
            pButton->m_nStyle &= ~(1 << 17);
            if (::GetCapture() == pThis->GetSafeHwnd()) {
                ::ReleaseCapture();
                pThis->m_pWndLastCapture = nullptr;
            }
            // TODO(clean-room): vslot0x6f8 (SetButtonStyle) not modeled.
        }
    }

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    // TODO(clean-room): per-button vslot0xc0 (OnCancelMode) not modeled.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x1801576f0), first part -- the m_iHot toggle:
//     if (m_iHot != nHot) m_iHot = nHot;
//     else if (m_iHot >= 0) m_iHot = -1;      // toggling back clears the hot
//     else m_iHot = nHot;
// The menu-mode / NotifyWinEvent(0x8005, hwnd, OBJID_CLIENT, ...) tail depends
// on CMFCMenuBar RTTI plus focus state and is not modeled.
// Symbol: ?OnChangeHot@CMFCToolBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot)
{
    if (!pThis) return;
    const int iOld = pThis->m_iHot;
    if (iOld == nHot) {
        pThis->m_iHot = (iOld < 0) ? nHot : -1;
    } else {
        pThis->m_iHot = nHot;
    }
    // TODO(clean-room): menu-mode NotifyWinEvent path not modeled.
}
// Retail (0x18015b6d0):
//     m_bRoundShape = pVisManager->GetToolBarButtonRoundShape(this);  // slot 0x188
//     if (m_bRoundShape) { round-shape handling } else SetWindowRgn(m_hWnd, 0, 0);
//     if (!m_bLocked && !vslot0x2e8()) vslot0x5b8 (this, 1);
//     adjust internal image lists + redraw (tail 0x180151890).
// GetToolBarButtonRoundShape and the round-shape helper are not modeled; the
// non-round branch (SetWindowRgn + layout) and the final redraw are kept.
// Symbol: ?OnChangeVisualManager@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    // TODO(clean-room): retail asks CMFCVisualManager for the round-shape flag
    // (vslot0x188 GetToolBarButtonRoundShape); not modeled, keep FALSE.
    pThis->m_bRoundShape = FALSE;

    if (pThis->m_bRoundShape) {
        // TODO(clean-room): round-shape handling (0x18015b760) not modeled.
    } else if (pThis->GetSafeHwnd()) {
        ::SetWindowRgn(pThis->GetSafeHwnd(), nullptr, FALSE);
    }

    if (!pThis->m_bLocked) {
        pThis->AdjustLayout();
    }

    // retail tail (0x180151890): per-image adjustments + RedrawWindow.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x180154a60):
//     if (m_bCustomizeMode && !m_bLocked) return call 0x18028e040();
//     if (lParam == 0) {
//         if (wParam == 2) { vslot0x6d8 (this); return 1; }   // OnCustomize
//         if (wParam == 1) { focus-based button handling }
//     } else { iterate m_Buttons doing IsChild checks }
//     return 0;
// Only the customize-mode gate and the IDCANCEL-style early return are kept.
// Symbol: ?OnCommand@CMFCToolBar@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCToolBar__MEAAH_K_J_Z(
    CMFCToolBar* pThis, unsigned long long wParam, long long lParam)
{
    if (!pThis) return 0;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        // TODO(clean-room): retail dispatches to an internal customizer helper.
        return 0;
    }

    if (lParam == 0 && wParam == 2) {
        // retail calls vslot0x6d8 (OnCustomize) then returns TRUE.
        return 1;
    }

    // TODO(clean-room): partially transcribed -- focus/IsChild button handling
    // not modeled.
    return 0;
}
// Retail (0x180153db0):
//     if (m_bLocked && m_bCustomizeMode) { MessageBeep(-1); return; }
//     if (vslot0x4e8 (this)) return;
//     if (m_bStretchButton) return;
//     vslot0x770 (this, -1);                          // OnChangeHot(-1)
//     if (!m_bCustomizeMode) { base CBasePane::OnContextMenu(...); return; }
//     ::ScreenToClient(m_hWnd, &pt);
//     iHit = vslot0x730 (this, pt);                   // HitTest
//     old = m_iSelected; m_iSelected = iHit;
//     if (old != -1) InvalidateButton(old);
//     ...
// The base call and the post-hit-test menu setup are not modeled; the hit test
// is reimplemented as a button-rect scan.
// Symbol: ?OnContextMenu@CMFCToolBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCToolBar__IEAAXPEAVCWnd__VCPoint___Z(
    CMFCToolBar* pThis, CWnd* pWnd, CPoint point)
{
    if (!pThis) return;

    if (pThis->m_bLocked && impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    if (pThis->m_bStretchButton) return;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        // TODO(clean-room): base CBasePane::OnContextMenu not modeled.
        (void)pWnd;
        return;
    }

    CPoint pt = point;
    ::ScreenToClient(pThis->GetSafeHwnd(), (LPPOINT)&pt);

    int iHit = -1;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->m_rect.PtInRect(pt)) {
            iHit = i;
            break;
        }
    }

    const int iOld = pThis->m_iSelected;
    pThis->m_iSelected = iHit;
    if (iOld != -1) {
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iOld);
    }

    // TODO(clean-room): partially transcribed -- post-hit-test menu handling not
    // modeled.
}
// Retail (RVA 0x157230, mfc140u), fully transcribed:
//     CMFCToolBarButton* p = GetButton(m_iSelected);          // 0x14fe00, 0x113c
//     if (p == NULL) AfxThrowInvalidArgException();           // 0x227720
//     CUserToolsManager* pMgr = afxUserToolsManager;          // 0x3be3b0
//     if (pMgr && !p->m_bUserButton &&                        // +0x08
//         p->m_nID >= pMgr-><+0x44> && p->m_nID <= pMgr-><+0x48>) {   // user-tool id range
//         for (node = pMgr-><+0x10 list head>; node; node = node->pNext) {
//             CUserTool* pTool = node->data;                  // node + 0x10
//             if (pTool-><+0x20> == p->m_nID) {               // the tool's command id
//                 if (pTool == NULL) break;                   // 0x1572db: a NULL element falls through to the image path
//                 pTool->CopyIconToClipboard();               // 0x182ff0
//                 return;
//             }
//         }
//     }
//     CMFCToolBarImages* pImages = p->m_bUserButton ? CMFCToolBar::m_pUserImages   // 0x3be370
//                                                   : &CMFCToolBar::m_Images;      // 0x3c25a0
//     if (pImages == NULL) AfxThrowInvalidArgException();
//     CWaitCursor wait;                                       // AfxGetModuleState()->m_pCurrentWinApp (0x133930, +8) -> BeginWaitCursor (0x1de7b0)
//     pImages->CopyImageToClipboard(p->GetImage());           // 0x16ef50; +0x48/+0x4c via the neg/sbb/and-4 idiom
//                                                             // ~CWaitCursor (0x7687c) -> EndWaitCursor (0x1de7e0)
// (All addresses mfc140u; the two CUserToolsManager / CUserTool member offsets
// are what the code reads, their names are not evidenced here.)
// Deviations: (1) the user-tool branch is NOT reproduced -- the exported
// ?afxUserToolsManager@@ pointer is never assigned anywhere in this tree
// (featurepack/CMFC_misc_stubs.cpp defines it NULL and nothing writes it;
// the unrelated internal C++ global of the same name in
// detail/MfccoreSupport.cpp points at a private CUserToolsManager and is
// what CWinAppEx::GetUserToolsManager hands out) and the CUserToolsManager
// list-head / id-range members retail reads are not modelled, so the branch
// is unreachable here and is skipped with the gate on that pointer kept.  (2) The two AfxThrowInvalidArgException paths become
// plain returns.  (3) CMFCToolBarImages::CopyImageToClipboard is itself a
// partial body in featurepack/toolbar/CMFCToolBarImages.cpp.
// Symbol: ?OnCopyImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCopyImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    // TODO(clean-room): retail's `afxUserToolsManager && !m_bUserButton &&
    // <id in range>` user-tool branch (deviation 1 above) is not reproduced;
    // the exported pointer is NULL throughout this tree, so the images path
    // below is the one retail would take here as well.
    (void)impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;

    CMFCToolBarImages* pImages = pButton->m_bUserButton
        ? static_cast<CMFCToolBarImages*>(impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA)
        : &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
    if (!pImages) return;

    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    impl__CopyImageToClipboard_CMFCToolBarImages__QEAAHH_Z(pImages, pButton->GetImage());
    if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
}
// Retail (0x180151ff0).  The first call is CWnd::Default (0x18028ac80), not a
// base-class OnCreate -- an earlier revision of this comment said otherwise.
//     if (CWnd::Default() == -1) return -1;
//     afxGlobalData.Initialize() if not yet initialised;         // 0x18006a790
//     lazily LoadCursorW's 0x7904, 0x7905 and 0x7f86 into three afxGlobalData
//         cursor slots (0x1803c1778 / 0x1803c1780 / 0x1803c1790);
//     if (AFXGetParentFrame(this) != NULL)                       // 0x18006bd00
//         cache (frame->GetStyle() & 0x400000) in the global at 0x1803be390;
//     if (!m_bNoDropTarget && <module flag>) m_DropTarget.Register(this); // 0x1230
//     create the drag pen into m_penDrag (0x12c0) and the tooltip control into
//         m_pToolTip (0x12e8) with nType 2;
//     m_bRoundShape = GetVisualManager()->IsToolBarRoundShape(this); // 0x1118
//     if (m_bRoundShape) SetRoundedRgn(); else ::SetWindowRgn(m_hWnd, NULL, 0);
//     add this toolbar to the global toolbar list at 0x1803b2090;
//     return 0;
// Not implemented: the afxGlobalData cursor cache, the visual manager and the
// global toolbar list are all unmodelled, and a partial body would create a
// tooltip and a drop target without the state the rest of the flow expects.
// Symbol: ?OnCreate@CMFCToolBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCToolBar* pThis, LPCREATESTRUCTW lpCreateStruct)
{
    (void)pThis; (void)lpCreateStruct;
    // TODO(clean-room): not transcribed -- base OnCreate + cursor/tooltip init.
    return 0;
}
// Retail (0x1801546e0): brush = base CWnd::OnCtlColor(...); when
// m_bCustomizeMode && !m_bLocked it then walks m_Buttons asking each button for
// its own brush via virtuals (slots 0x150/0xa0). Those button virtuals are not
// modeled; the base brush is returned.
// Symbol: ?OnCtlColor@CMFCToolBar@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBar__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CMFCToolBar* pThis, CDC* pDC, CWnd* pWnd, unsigned int nCtlColor)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): partially transcribed -- per-button brush virtuals skipped.
    return pThis->CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}
// Retail (0x180156490):
//     m_iButtonCapture = m_iHighlighted = m_iSelected = -1;
//     for (pButton : m_Buttons) {
//         pButton->vslot0xc0 (pButton);                // OnCancelMode()
//         if ((pButton->m_nStyle & 0x40000) == 0)
//             pButton->vslot0x140 (pButton, bCustomize == 0);
//     }
// The per-button virtuals are not modeled; the state reset is kept.
// Symbol: ?OnCustomizeMode@CMFCToolBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int bCustomize)
{
    if (!pThis) return;

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    pThis->m_iSelected = -1;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        if ((pButton->m_nStyle & 0x40000) == 0) {
            // TODO(clean-room): vslot0xc0 / vslot0x140 per-button customize-mode
            // calls not modeled; bCustomize is deliberately unused.
            (void)bCustomize;
        }
    }
}
// Retail (0x180152fd0): destroys m_penDrag (0x12c0), deletes m_pToolTip
// (0x12e8), calls base CBasePane::OnDestroy, clears the global
// m_pLastHookedToolbar when it points at this toolbar, and unlinks itself from
// the global toolbar list (0x1803b2098). Only the m_pLastHookedToolbar side
// effect and the tooltip pointer reset are reproduced; m_penDrag is a CPen and
// m_pToolTip a CMFCToolTipCtrl* whose full type is not available here.
// Symbol: ?OnDestroy@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- CPen dtor, tooltip delete, base
    // OnDestroy and the global toolbar-list unlink are not modeled.
    pThis->m_pToolTip = nullptr;
    if (impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA == pThis) {
        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
    }
}
// Retail (0x180152250):
//     if (m_bDisableCustomize) return 0;
//     m_iDragIndex = -1;
//     global drag flag (0x1803c2578) = 0;
//     return vslot0x7c0 (this, pDataObject, dwKeyState, point);   // OnDragOver
// Symbol: ?OnDragEnter@CMFCToolBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point)
{
    (void)pDataObject; (void)dwKeyState; (void)point;
    if (!pThis) return 0;

    if (pThis->m_bDisableCustomize) return 0;

    pThis->m_iDragIndex = -1;
    // TODO(clean-room): the global drag flag and the OnDragOver tail call
    // (vslot0x7c0) are not modeled; DROPEFFECT_NONE is the safe default.
    return DROPEFFECT_NONE;
}
// Retail (0x180158660):
//     CWnd::OnSetFocus(pOldWnd);
//     if (m_bLeaveFocus && pOldWnd && IsWindow(pOldWnd->m_hWnd) &&
//         !pOldWnd->IsKindOf(CMFCToolBar) &&
//         pOldWnd's parent is not a CMFCToolBar &&
//         this's parent is not a CMFCBaseTabCtrl)
//         m_hwndLastFocus = pOldWnd->m_hWnd;
// Symbol: ?OnSetFocus@CMFCToolBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(
    CMFCToolBar* pThis, CWnd* pOldWnd)
{
    if (!pThis) return;

    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(pThis, pOldWnd);

    if (!pThis->m_bLeaveFocus) return;
    if (!pOldWnd || !::IsWindow(pOldWnd->GetSafeHwnd())) return;
    if (pOldWnd->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return;

    CWnd* pParentOld = CWnd::FromHandle(::GetParent(pOldWnd->GetSafeHwnd()));
    if (pParentOld && pParentOld->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return;

    CWnd* pParentThis = CWnd::FromHandle(::GetParent(pThis->GetSafeHwnd()));
    if (pParentThis && pParentThis->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl))) return;

    pThis->m_hwndLastFocus = pOldWnd->GetSafeHwnd();
}
// Retail (0x180159620):
//     CBasePane::OnSettingChange(wParam, lpszSection);
//     if (wParam <= 0x2f && bit wParam of 0x840400000000 is set) {
//         gdata init + AFX_GLOBAL_DATA::UpdateFonts();
//         AdjustLayout();
//     }
// The base call and the AFX_GLOBAL_DATA object are not modeled.
// Symbol: ?OnSettingChange@CMFCToolBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCToolBar__IEAAXIPEB_W_Z(
    CMFCToolBar* pThis, unsigned int wParam, const wchar_t* lpszSection)
{
    if (!pThis) return;
    (void)lpszSection;
    // TODO(clean-room): retail starts with CBasePane::OnSettingChange (0x18000ce90).
    const unsigned long long mask = 0x840400000000ULL;   // SPI_SETNONCLIENTMETRICS etc.
    if (wParam <= 0x2f && ((mask >> wParam) & 1ULL)) {
        // TODO(clean-room): retail inits AFX_GLOBAL_DATA and calls UpdateFonts
        // (0x18006a9e0); the global data object is not modeled.
        pThis->AdjustLayout();
    }
}
// Retail (0x18015ad00):
//     CWnd::Default();
//     if (m_bCustomizeMode && customize-manager singleton && !m_bLocked) {
//         manager->ShowToolBar(this, bShow);           // 0x18017c600
//         if (this == selected-toolbar global) {
//             selected-toolbar global = NULL;
//             m_iSelected = -1;
//         }
//     }
// The manager singleton (0x1803be3a0), its ShowToolBar helper and the
// selected-toolbar global are unmodeled; the m_pSelToolbar static stands in
// for the latter.
// Symbol: ?OnShowWindow@CMFCToolBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCToolBar__IEAAXHI_Z(
    CMFCToolBar* pThis, int bShow, unsigned int nStatus)
{
    (void)bShow; (void)nStatus;
    if (!pThis) return;

    impl__Default_CWnd__IEAA_JXZ(pThis);

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        // TODO(clean-room): retail gates on the customize-manager singleton
        // (0x1803be3a0) and calls its ShowToolBar helper (0x18017c600) with
        // (this, bShow); neither is modeled.
        if (impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA == pThis) {
            impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
            pThis->m_iSelected = -1;
        }
    }
}
// Retail (0x180155860):
//     SetRoundedRgn();
//     CBasePane::OnSize(nType, cx, cy);
//     if (m_bCustomizeMode && !m_bLocked) OnCancelMode();
//     if (!m_bInUpdateShadow) vslot0x7d8();
//     if (GetButtonCount() > 0) {
//         parent frame (AFXGetParentFrame) IsKindOf(CMiniFrameWnd):
//             adjust the frame's system menu (GetSystemMenu/DeleteMenu/
//             EnableMenuItem on ids 0xf120/0xf020/0xf030/0xf060).
//     }
// SetRoundedRgn, the base OnSize and the frame-menu walk are not modeled.
// Symbol: ?OnSize@CMFCToolBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBar__IEAAXIHH_Z(
    CMFCToolBar* pThis, unsigned int nType, int cx, int cy)
{
    (void)nType; (void)cx; (void)cy;
    if (!pThis) return;

    // TODO(clean-room): retail first calls SetRoundedRgn (0x18015b760) and then
    // CBasePane::OnSize (0x18000c2a0); neither is modeled.

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pThis);
    }

    if (!pThis->m_bInUpdateShadow) {
        // TODO(clean-room): retail calls vslot 0x7d8; not modeled.
    }

    if (pThis->GetCount() > 0) {
        // TODO(clean-room): retail walks the parent CMiniFrameWnd and edits its
        // system menu via AFXGetParentFrame/GetSystemMenu/DeleteMenu/
        // EnableMenuItem; the frame chain is not modeled.
    }
}
// Retail (0x180151820):
//     gdata init + AFX_GLOBAL_DATA::UpdateSysColors();
//     global-data helper (0x180009774) + its vslot 0x70;
//     UpdateImagesColor();       // per-image CMFCToolBarImages::OnSysColorChange
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
// Only the final redraw survives (the AFX_GLOBAL_DATA object and the image
// collection helpers are not modeled).
// Symbol: ?OnSysColorChange@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): gdata init/UpdateSysColors and the per-image
    // CMFCToolBarImages::OnSysColorChange loop (0x180151890) not modeled.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18014ff80) is complex: it gates on m_bShowTooltips /
// GetCapture() == this, hit-tests via vslot 0x730, and fills pTI->lpszText
// through CTooltipManager::SetTooltipText (0x180182560). The conservative
// version below keeps the button-rect hit test and the minimal TOOLINFOW
// fields, which is the observable part without the tooltip text plumbing.
// Symbol: ?OnToolHitTest@CMFCToolBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CMFCToolBar* pThis, CPoint point, TOOLINFOW* pTI)
{
    if (!pThis) return -1;
    // TODO(clean-room): partially transcribed -- the m_bShowTooltips/GetCapture
    // gate and CTooltipManager::SetTooltipText are not modeled.

    const int nCount = pThis->GetCount();
    int iHit = -1;
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->m_rect.PtInRect(point)) {
            iHit = i;
            break;
        }
    }
    if (iHit == -1) return -1;

    CMFCToolBarButton* pButton = pThis->GetButton(iHit);
    if (!pButton) return -1;

    // A NULL pTI is legal (OnHelpHitTest passes one): retail (0x14ff80,
    // mfc140u) branches to 0x150350 after the GetButton and returns
    // `m_nID == (UINT)-1 ? 0 : m_nID` without touching the TOOLINFO.  This
    // used to be an early `return -1`, which made every help hit-test miss.
    if (!pTI) {
        return pButton->m_nID == static_cast<unsigned int>(-1) ? 0 : (__int64)pButton->m_nID;
    }

    std::memset(pTI, 0, sizeof(*pTI));
    pTI->cbSize = sizeof(*pTI);
    pTI->hwnd = pThis->GetSafeHwnd();
    pTI->uId = pButton->m_nID;
    pTI->rect.left   = pButton->m_rect.left;
    pTI->rect.top    = pButton->m_rect.top;
    pTI->rect.right  = pButton->m_rect.right;
    pTI->rect.bottom = pButton->m_rect.bottom;
    return (__int64)pButton->m_nID;
}
// Retail (0x180154100):
//     pButton = GetButton(m_iSelected); if NULL return;
//     if (button text empty) vslot0x750 (OnSetDefaultButtonText);
//     run CMFCToolBarButtonCustomizeDialog(pButton, ..., this);
//     if (IDOK) { AdjustLayout(); SetDrawTextLabels(TRUE); RedrawWindow(0x505); }
// The dialog and the text virtual are not modeled.
// Symbol: ?OnToolbarAppearance@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarAppearance_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;
    // TODO(clean-room): retail runs the CMFCToolBarButtonCustomizeDialog
    // (ctor 0x18015eae0 / DoModal 0x1802088b0 / dtor 0x18015ee10) and, on IDOK,
    // calls AdjustLayout, SetDrawTextLabels(TRUE) and RedrawWindow(0x505); the
    // dialog is not modeled.
    (void)pButton;
}
// Retail (0x180154210):
//     vslot0x6a8 (RemoveButton) with m_iSelected;
//     m_iSelected = -1;
//     tail-call vslot0x858 (OnAfterButtonDelete).
// Symbol: ?OnToolbarDelete@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarDelete_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    impl__RemoveButton_CMFCToolBar__UEAAHH_Z(pThis, pThis->m_iSelected);
    pThis->m_iSelected = -1;
    impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(pThis);
}
// Retail (0x1801542b0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     save m_bText/m_bImage; set m_bText = FALSE, m_bImage = TRUE;
//     if image index < 0 run the customize dialog and restore the flags on
//     cancel; on OK / valid image:
//         AdjustLayout(); SetDrawTextLabels(TRUE);
// The dialog is not modeled; the success path is kept.
// Symbol: ?OnToolbarImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = FALSE;
    pButton->m_bImage = TRUE;

    // TODO(clean-room): retail shows the CMFCToolBarButtonCustomizeDialog for
    // image-less buttons and restores m_bText/m_bImage on cancel; not modeled.
    pThis->AdjustLayout();
    pThis->m_bDrawTextLabels = TRUE;    // retail vslot 0x5b8 SetDrawTextLabels(TRUE)
}
// Retail (0x1801543c0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     set m_bText = m_bImage = TRUE;
//     dialog path (skipped); then:
//     if (button text empty) vslot0x750 (OnSetDefaultButtonText);
//     if (text still empty) { MessageBeep(-1); m_bText = FALSE; m_bImage = TRUE; }
//     AdjustLayout();
// Symbol: ?OnToolbarImageAndText@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImageAndText_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = TRUE;
    pButton->m_bImage = TRUE;

    // TODO(clean-room): retail runs the CMFCToolBarButtonCustomizeDialog first
    // for image-less buttons; not modeled.
    if (pButton->m_strText.IsEmpty()) {
        // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
        // not modeled.
        ::MessageBeep(static_cast<UINT>(-1));
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
    }

    pThis->AdjustLayout();
}
// Retail (RVA 0x1587e0, mfc140u), decoded but NOT implemented:
//     CMFCToolBarMenuButton* p = new CMFCToolBarMenuButton();      // 0x128 bytes, ctor 0x172790
//     p->m_bText = TRUE;  p->m_bImage = FALSE;                     // one QWORD store at +0xc
//     BOOL bMenuMode = IsPureMenuButton(p);                        // vslot 0x828 = RVA 0x23a70, `mov 0x10ec(%rcx),%eax`
//                                                                  //   (the afxtoolbar.h inline returning m_bMenuMode)
//     CMFCToolBarButtonCustomizeDialog dlg(p, m_pUserImages, this, 0, bMenuMode);   // 0x15eae0; the vslot
//                                                                  //   result is the 6th ctor arg at 0x158843
//     if (dlg.DoModal() != IDOK) { delete p; }                     // deleting dtor, vslot 0x08
//     else {
//         m_iSelected = InsertButton(p, m_iSelected);              // vslot 0x690
//         AdjustLayout();                                          // vslot 0x428
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x505);
//         p->OnClick(this, FALSE);                                 // button vslot 0x50 (slot 10): for the
//     }                                                            //   CMFCToolBarMenuButton just created that is
//                                                                  //   ?OnClick@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@H@Z
//                                                                  //   (0x173990, slot 10 of vftable 0x3184e8), i.e.
//                                                                  //   the new menu button is dropped down
// The customize dialog (a 0x970-byte CDialog run modally) is not modelled, so
// none of this can be reproduced faithfully.  (Earlier revisions of this
// comment called the final vslot 0x50 "OnChangeParentWnd" -- that is slot
// 0x60 -- and then "the 0x316048 slot 10 `xor eax,eax` stub" -- that is the
// CMFCToolBarButton BASE table; the object here carries the menu-button
// table, whose slot 10 is the real OnClick.  Both were wrong.)
// Symbol: ?OnToolbarNewMenu@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarNewMenu_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    // TODO(clean-room): not transcribed -- needs the customize dialog and the
    // InsertButton virtual (vslot 0x690); neither is modeled.
    (void)pThis;
}
// Retail (0x180156ab0) resets the selected button to its CCommandManager
// default (image lookup in 0x1803b1f90 + SetImage), forces text on/off from
// m_bMenuMode/m_bImage, then SetButtonText, AdjustLayout, usage count update
// and RedrawWindow(0x505). The CCommandManager is not modeled.
// Symbol: ?OnToolbarReset@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarReset_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;
    // TODO(clean-room): the CCommandManager image lookup (0x1803b1f90) and the
    // button-state reset are not modeled; only the layout/redraw is kept.
    (void)pButton;
    pThis->AdjustLayout();
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x180154500) walks the real m_Buttons CObList for the button at
// m_iSelected and toggles a group separator (InsertSeparator via vslot 0x6a0
// or RemoveButton of the previous button), then AdjustLayout. The walk uses
// the real CObList which OpenMFC keeps empty (live buttons are side-table
// backed in mfccore.cpp), so only the relayout survives.
// Symbol: ?OnToolbarStartGroup@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarStartGroup_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the separator toggle walks the
    // real m_Buttons list and uses the InsertSeparator virtual; not modeled.
    pThis->AdjustLayout();
}
// Retail (0x1801545a0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     m_bText = TRUE; m_bImage = FALSE;
//     if (text empty) vslot0x750 (OnSetDefaultButtonText);
//     if (text still empty) { MessageBeep(-1); m_bText = FALSE; m_bImage = TRUE; }
//     AdjustLayout(); SetDrawTextLabels(TRUE);
// Symbol: ?OnToolbarText@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarText_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = TRUE;
    pButton->m_bImage = FALSE;

    if (pButton->m_strText.IsEmpty()) {
        // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
        // not modeled.
        ::MessageBeep(static_cast<UINT>(-1));
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
    }

    pThis->AdjustLayout();
    pThis->m_bDrawTextLabels = TRUE;    // retail vslot 0x5b8 SetDrawTextLabels(TRUE)
}
// Retail (0x180151c80) builds a stack CMFCToolBarCmdUI and routes every
// non-separator, non-AFX-id button through pTarget->OnCmdMsg (frame vslot
// 0x28); it ends with CWnd::UpdateDialogControls(this, pTarget,
// bDisableIfNoHandler && m_bDisableControlsIfNoHandler). The CmdUI object and
// the per-button routing are not modeled; the skip conditions and the final
// UpdateDialogControls call are kept.
// Symbol: ?OnUpdateCmdUI@CMFCToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
    CMFCToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHandler)
{
    if (!pThis) return;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        if (pButton->m_nStyle & 0x1) continue;          // separator/checkbox style
        const UINT nID = pButton->m_nID;
        if (nID == 0) continue;
        if ((nID - 0xf000) <= 0x1ef) continue;          // AFX control-bar ids
        if (nID >= 0xff00) continue;                    // AFX prompt ids
        if (nID == 0xffff) continue;
        // TODO(clean-room): retail routes the button through a stack
        // CMFCToolBarCmdUI + pTarget->OnCmdMsg; not modeled.
    }

    if (!pTarget) return;

    const int bDisable = (bDisableIfNoHandler != 0 &&
                          pThis->m_bDisableControlsIfNoHandler != 0) ? 1 : 0;
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisable);
}
// Retail (0x18015b930):
//     if (wParam & 2) {
//         CTooltipManager::CreateToolTip(&m_pToolTip, this, 2);
//         UpdateTooltips();
//     }
//     return 0;
// UpdateTooltips (0x180159ce0) is not modeled.
// Symbol: ?OnUpdateToolTips@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned long long wParam, long long lParam)
{
    (void)lParam;
    if (!pThis) return 0;

    if (wParam & 2) {
        impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
            reinterpret_cast<CToolTipCtrl**>(&pThis->m_pToolTip), pThis, 2);
        // TODO(clean-room): retail then calls UpdateTooltips (0x180159ce0);
        // not modeled.
    }
    return 0;
}
// Retail (0x180159370), transcribed:
//     pFrame = AFXGetParentFrame(this);  if (!pFrame) return FALSE;
//     if (pFrame->IsKindOf(RUNTIME_CLASS(CMFCDropDownFrame)))     // 0x1803b1208
//         { pFrame = AFXGetParentFrame(pFrame); if (!pFrame) return FALSE; }
//     if      (pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))  // 0x1802ee478
//          return pFrame->vslot0x400(pButton, strText);
//     else if (pFrame->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))     // 0x1802e9210
//          return pFrame->vslot0x3d8(pButton, strText);
//     else if (pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)))  // 0x1802ed888
//          return pFrame->vslot0x3b8(pButton, strText);
//     return FALSE;
// The frame that is skipped is a CMFCDropDownFrame, not a CMDIChildWndEx -- an
// earlier revision of this comment had that wrong; all four names were read back
// out of the CRuntimeClass descriptors.  Those OnGetToolTipText virtuals are not
// declared in OpenMFC, so nothing is transcribed.
// Symbol: ?OnUserToolTip@CMFCToolBar@@UEBAHPEAVCMFCToolBarButton@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUserToolTip_CMFCToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCToolBar* pThis, CMFCToolBarButton* pButton, CString& strText)
{
    (void)pThis; (void)pButton; (void)strText;
    // TODO(clean-room): not transcribed -- the frame OnGetToolTipText virtuals
    // (0x400/0x3d8/0x3b8) are not modeled.
    return FALSE;
}
// Retail (0x180154640):
//     bResized = (m_sizeLast.cx < lpwndpos->cx) || (m_sizeLast.cy < lpwndpos->cy);
//     m_sizeLast = { cx, cy };
//     CWnd::Default();
//     if (bResized || parent IsKindOf(CMFCReBar)) RedrawWindow(NULL,NULL,0x585);
//     tail-call RedrawCustomizeButton();
// CMFCReBar is not declared in OpenMFC headers, so only the bResized gate is
// kept.
// Symbol: ?OnWindowPosChanged@CMFCToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCToolBar__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCToolBar* pThis, WINDOWPOS* lpwndpos)
{
    if (!pThis || !lpwndpos) return;

    const int cx = lpwndpos->cx;
    const int cy = lpwndpos->cy;
    const BOOL bResized = (pThis->m_sizeLast.cx < cx) ||
                          (pThis->m_sizeLast.cy < cy);

    pThis->m_sizeLast.cx = cx;
    pThis->m_sizeLast.cy = cy;

    impl__Default_CWnd__IEAA_JXZ(pThis);

    if (bResized) {
        // TODO(clean-room): retail also redraws when the parent
        // IsKindOf(CMFCReBar) (RTTI 0x1802fd8b8); CMFCReBar is not declared in
        // OpenMFC headers.
        ::RedrawWindow(pThis->GetSafeHwnd(), nullptr, nullptr, 0x585);
    }

    impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(pThis);
}
// Retail (0x180159020):
//     CWnd::Default();
//     if (parent IsKindOf(CMFCReBar)) AdjustLayout();
// Symbol: ?OnWindowPosChanging@CMFCToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMFCToolBar__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCToolBar* pThis, WINDOWPOS* lpwndpos)
{
    (void)lpwndpos;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): retail relayouts when the parent IsKindOf(CMFCReBar)
    // (RTTI 0x1802fd8b8); CMFCReBar is not declared in OpenMFC headers.
}
// Retail (0x180154940):
//     if (WM_KEYDOWN && wParam == VK_ESCAPE) {
//         if (m_bStretchButton) { OnCancelMode(); return TRUE; }
//         if (!GetDroppedDownMenu(this, NULL)) { vslot0x6d0(); vslot0x6d8(); }
//         return TRUE;
//     }
//     if (message == registered customize message) { OnPromptReset(...); return TRUE; }
//     if (message is a keyboard/mouse navigation message)
//         SendMessageW(m_pToolTip->m_hWnd, 0x407, 0, (LPARAM)pMsg);
//     return CBasePane::PreTranslateMessage(pMsg);
// GetDroppedDownMenu, vslots 0x6d0/0x6d8, the registered message and the
// CBasePane base are not modeled; the ESC/stretch path, the tooltip forward
// and the CWnd base are kept.
// Symbol: ?PreTranslateMessage@CMFCToolBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(
    CMFCToolBar* pThis, MSG* pMsg)
{
    if (!pThis || !pMsg) return 0;

    const UINT nMsg = pMsg->message;

    if (nMsg == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        if (pThis->m_bStretchButton) {
            impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pThis);
            return 1;
        }
        // TODO(clean-room): retail checks GetDroppedDownMenu(); when none is
        // open it calls vslots 0x6d0/0x6d8 and returns TRUE. Not modeled.
        return 1;
    }

    // TODO(clean-room): retail handles a registered customize message
    // (global 0x1803c2500) via OnPromptReset; not modeled.

    // Forward mouse/keyboard activity to the tooltip so it can dismiss itself.
    static const UINT s_tooltipFwd[] = {
        0xa1, 0xa2, 0xa4, 0xa5, 0xa7, 0xa8,   // non-client mouse buttons
        0x104,                                 // WM_SYSKEYDOWN
        0x200, 0x201, 0x202, 0x204, 0x205, 0x207,  // client mouse buttons
    };
    bool bForward = false;
    for (UINT m : s_tooltipFwd) {
        if (nMsg == m) { bForward = true; break; }
    }
    if (bForward && pThis->m_pToolTip) {
        // CMFCToolTipCtrl is only forward-declared; its CWnd base carries
        // m_hWnd at offset 0x40.
        HWND hwndTip = *reinterpret_cast<HWND*>(
            reinterpret_cast<char*>(pThis->m_pToolTip) + 0x40);
        if (hwndTip) {
            ::SendMessageW(hwndTip, 0x407, 0, reinterpret_cast<LPARAM>(pMsg));
        }
    }

    // Retail tail-calls CBasePane::PreTranslateMessage (0x18000cb30); the CWnd
    // implementation is the closest modeled base.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Retail (0x180157ae0) first asks GetDroppedDownMenu(); when the dropped menu
// is closed it scans backward from the dropped index (wrapping) for a
// CMFCToolBarMenuButton whose m_nStyle does not carry 0x40000 and calls
// OnChangeHot on it. GetDroppedDownMenu is not modeled, so the scan anchors on
// m_iSelected instead.
// Symbol: ?PrevMenu@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__PrevMenu_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return 0;

    const int nCount = pThis->GetCount();
    if (nCount == 0) return 0;

    const int iSelected = pThis->m_iSelected;

    int iFound = -1;
    int i = iSelected - 1;
    if (i < 0) i = nCount - 1;

    for (int scanned = 0; scanned < nCount; ++scanned) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&
            (pButton->m_nStyle & 0x40000) == 0) {
            iFound = i;
            break;
        }
        if (--i < 0) i = nCount - 1;
    }

    if (iFound == -1) return 1;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iFound);

    // TODO(clean-room): retail posts WM_KEYDOWN(VK_MENU) to the hot button's
    // parent when the newly selected button is a dropped-down menu; not modeled.
    return 1;
}
// Retail (0x180157eb0):
//     if (m_nID == 0 || m_nID == 0xffffffff) return 0;
//     AFXPlaySystemSound(1);
//     CMFCCmdUsageCount::AddCmd(m_nID);       // global map 0x1803b1fd0
//     hwndParent = owner handle at [this+0xa0] or GetParent(m_hWnd);
//     pParent = CWnd::FromHandle(hwndParent);
//     PostMessageW(pParent->m_hWnd, WM_COMMAND, m_nID, 0);
//     return 1;
// The sound, the usage-count map and the owner-handle member are not modeled.
// Symbol: ?ProcessCommand@CMFCToolBar@@QEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__ProcessCommand_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton)
{
    if (!pThis || !pButton) return 0;

    const UINT nID = pButton->m_nID;
    if (nID == 0 || nID == 0xffffffff) return 0;

    // TODO(clean-room): retail plays AFXPlaySystemSound(1) (0x180132460) and
    // records the command in CMFCCmdUsageCount::AddCmd; not modeled.

    CWnd* pParent = CWnd::FromHandle(::GetParent(pThis->GetSafeHwnd()));
    if (!pParent) return 0;

    ::PostMessageW(pParent->GetSafeHwnd(), WM_COMMAND, nID, 0);
    return 1;
}
// Retail (0x1801563b0), transcribed:
//     m_AccelKeys.RemoveAll();                       // 0x1318, CMap::RemoveAll
//     for (pButton in m_Buttons) {
//         if (pButton->m_nStyle & TBBS_SEPARATOR)  continue;
//         if (!pButton->m_bText)                   continue;
//         if (pButton->m_strText.GetLength() <= 0) continue;
//         p = ::wcschr(pButton->m_strText, L'&');  if (!p) continue;
//         i = p - pButton->m_strText;
//         if (i < 0 || i >= GetLength() - 1) continue;   // no char after the '&'
//         wch = pButton->m_strText[i + 1];
//         ::CharUpperW(&wch);
//         m_AccelKeys.SetAt(wch, pButton);
//     }
// (Re-read on mfc140u at RVA 0x1563b0: RemoveAll is the ICF-shared 0x1ba40,
// wcschr / CharUpperW are the IAT slots 0x2c7400 / 0x2c6d88, SetAt is the
// instantiation at 0x15bbe8; the `i + 1` bounds check before the character
// read is CSimpleStringT::operator[]'s and can only pass, so it is folded.)
// m_AccelKeys is populated in retail layout through the AccelMap helpers near
// the top of this file, so the CMFCPopupMenuBar::OnKey reader that walks the
// block by those offsets now finds the entries.  Deviation: the buttons come
// from the mfccore.cpp side table (GetCount / GetButton) rather than the
// m_Buttons CObList, which OpenMFC keeps empty.
// Symbol: ?RebuildAccelerationKeys@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    AccelMap* pMap = AccelKeysOf(pThis);
    AccelMapRemoveAll(pMap);

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) break;                            // retail leaves the loop (0x1563e2 -> the ret at 0x156478) on a NULL element
        if (pButton->m_nStyle & 1u) continue;           // TBBS_SEPARATOR
        if (!pButton->m_bText) continue;
        const int nLen = pButton->m_strText.GetLength();
        if (nLen <= 0) continue;
        const wchar_t* pszText = pButton->m_strText.GetString();
        const wchar_t* pAmp = ::wcschr(pszText, L'&');
        if (!pAmp) continue;
        const int iAmp = static_cast<int>(pAmp - pszText);
        if (iAmp < 0 || iAmp >= nLen - 1) continue;     // no character after the '&'
        wchar_t wch[2] = { pszText[iAmp + 1], L'\0' };
        ::CharUpperW(wch);
        AccelMapSetAt(pMap, static_cast<UINT>(wch[0]), pButton);
    }
}
// Retail (0x18015b870):
//     if (!m_hWnd || !m_pCustomizeBtn) return;
//     rect = m_pCustomizeBtn->GetInvalidateRect();   // vslot 0x120
//     InflateRect(&rect, dx, dy);                    // extended members 0x12c/0x150
//     rect.right += 10; rect.bottom += 10;
//     RedrawWindow(m_hWnd, &rect, NULL, 0x505);
// The GetInvalidateRect virtual and the extended customize-button members are
// not modeled; m_rect stands in for the invalidate rect.
// Symbol: ?RedrawCustomizeButton@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (!pThis->GetSafeHwnd()) return;
    CMFCToolBarButton* pBtn = pThis->m_pCustomizeBtn;
    if (!pBtn) return;

    // TODO(clean-room): retail uses vslot 0x120 (GetInvalidateRect) and the
    // extended customize-button members at 0x12c/0x150; m_rect stands in.
    CRect rect = pBtn->m_rect;
    rect.right += 10;
    rect.bottom += 10;
    ::RedrawWindow(pThis->GetSafeHwnd(), (const struct tagRECT*)&rect, nullptr, 0x505);
}
// Retail (0x18015b970), transcribed:
//     for (pPane in the global pane list at 0x1803b2098) {
//         if (pPane == NULL) break;
//         if (!pPane->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) continue;   // 0x1803b15f8
//         if (CWnd::FromHandlePermanent(pPane->m_hWnd) == NULL) continue;
//         bAny = FALSE;
//         for (node = pBar->m_Buttons.head; node; node = node->next) {
//             pButton = node->data;  if (pButton == NULL) break;
//             if (pButton->m_nStyle & TBBS_SEPARATOR)  continue;  // 0x28
//             if (!pButton->m_bText)                   continue;  // 0x0c
//             if (pButton->m_strText.GetLength() <= 0) continue;  // 0x38, len at -0x10
//             if (::wcschr(pButton->m_strText, L'&') == NULL) continue;
//             ::InvalidateRect(pBar->m_hWnd, &pButton->m_rect, TRUE);  // 0x68
//             bAny = TRUE;
//         }
//         if (bAny) ::UpdateWindow(pBar->m_hWnd);
//     }
// The three IAT slots resolve to USER32!InvalidateRect (0x1802c7128),
// USER32!UpdateWindow (0x1802c7300) and VCRUNTIME140!wcschr (0x1802c7400);
// 0x26 is L'&'.  Retail also computes the '&' offset in wchar units and skips
// a negative one, which is unreachable, so it is not reproduced.
//
// Deviations: OpenMFC has no equivalent of the retail global pane list, so the
// outer loop runs over the mfccore.cpp toolbar side table -- which is already
// CMFCToolBar-only, making the IsKindOf filter implicit -- and the buttons come
// from that same side table rather than from the m_Buttons CObList (empty
// here).  CWnd::FromHandlePermanent, i.e. "this pane object is attached to a
// live window", becomes a GetSafeHwnd() test, because CMFCToolBar::Create does
// not register its HWND in the permanent map.
// Symbol: ?RedrawUnderlines@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__RedrawUnderlines_CMFCToolBar__SAXXZ()
{
    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (!pBar) continue;
        const HWND hwnd = pBar->GetSafeHwnd();
        if (!hwnd) continue;

        BOOL bAny = FALSE;
        const int nCount = pBar->GetCount();
        for (int i = 0; i < nCount; ++i) {
            CMFCToolBarButton* pButton = pBar->GetButton(i);
            if (!pButton) break;                       // retail breaks on a NULL element
            if (pButton->m_nStyle & 1u) continue;      // TBBS_SEPARATOR
            if (!pButton->m_bText) continue;
            if (pButton->m_strText.IsEmpty()) continue;
            if (::wcschr(pButton->m_strText.GetString(), L'&') == nullptr) continue;

            ::InvalidateRect(hwnd, reinterpret_cast<const RECT*>(&pButton->m_rect), TRUE);
            bAny = TRUE;
        }

        if (bAny) {
            ::UpdateWindow(hwnd);
        }
    }
}
// Retail (0x18014ec50):
//     m_iButtonCapture = m_iHighlighted = m_iSelected = -1;
//     while (m_Buttons not empty) {
//         pButton = m_Buttons.RemoveHead();
//         pButton->vslot0xc0();            // OnCancelMode
//         delete pButton;                  // vslot 0x8
//     }
//     m_pCustomizeBtn = NULL;
// OpenMFC keeps the live buttons in a per-toolbar side table (mfccore.cpp
// g_toolBarStates) that is not reachable from this file; the real m_Buttons
// CObList is a compatibility member and is normally empty, so the loop below
// is a no-op and the side-table buttons are left untouched (documented
// divergence).
// Symbol: ?RemoveAllButtons@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveAllButtons_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    pThis->m_iSelected = -1;

    // TODO(clean-room): OpenMFC's live buttons are side-table backed and are
    // not cleared here (mfccore.cpp g_toolBarStates is not reachable).
    while (pThis->m_Buttons.GetCount() > 0) {
        CMFCToolBarButton* pButton =
            static_cast<CMFCToolBarButton*>(pThis->m_Buttons.RemoveHead());
        if (pButton) {
            // TODO(clean-room): retail calls vslot 0xc0 (OnCancelMode) first.
            delete pButton;
        }
    }

    pThis->m_pCustomizeBtn = nullptr;
}
// Retail (0x18014ecf0):
//     if (iIndex < 0 || iIndex >= m_Buttons.GetCount()) return FALSE;
//     if (iIndex == count-1 && m_pCustomizeBtn) return FALSE;
//     pButton = m_Buttons[iIndex]; m_Buttons.RemoveAt(pos);
//     pButton->vslot0xc0(); delete pButton;
//     adjust m_iSelected / m_iButtonCapture / m_iHighlighted;
//     OnChangeHot(-1);
//     remove trailing separator buttons;
//     RebuildAccelerationKeys();
//     return TRUE;
// As with RemoveAllButtons, OpenMFC's live buttons are side-table backed, so
// this operates on the (normally empty) compatibility CObList.
// Symbol: ?RemoveButton@CMFCToolBar@@UEAAHH@Z
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBar__UEAAHH_Z(CMFCToolBar* pThis, int iIndex)
{
    if (!pThis) return FALSE;

    const INT_PTR nCount = pThis->m_Buttons.GetCount();
    if (iIndex < 0 || iIndex >= nCount) return FALSE;

    // The last button cannot be removed while a customize button is present.
    if (iIndex == nCount - 1 && pThis->m_pCustomizeBtn != nullptr) return FALSE;

    CObList::POSITION pos = pThis->m_Buttons.FindIndex(iIndex);
    if (pos == CObList::POSITION(nullptr)) return FALSE;

    CMFCToolBarButton* pButton =
        static_cast<CMFCToolBarButton*>(pThis->m_Buttons.GetAt(pos));
    if (!pButton) return FALSE;

    pThis->m_Buttons.RemoveAt(pos);
    // TODO(clean-room): retail calls pButton->vslot 0xc0 (OnCancelMode) first.
    delete pButton;

    if (pThis->m_iSelected == iIndex) {
        pThis->m_iSelected = -1;
    } else if (pThis->m_iSelected > iIndex && pThis->m_iSelected >= 0) {
        pThis->m_iSelected--;
    }
    if (pThis->m_iButtonCapture == iIndex) {
        pThis->m_iButtonCapture = -1;
    } else if (pThis->m_iButtonCapture > iIndex && pThis->m_iButtonCapture >= 0) {
        pThis->m_iButtonCapture--;
    }
    if (pThis->m_iHighlighted == iIndex) {
        pThis->m_iHighlighted = -1;
    } else if (pThis->m_iHighlighted > iIndex && pThis->m_iHighlighted >= 0) {
        pThis->m_iHighlighted--;
    }

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    // Remove trailing separator buttons.
    while (pThis->m_Buttons.GetCount() > 0) {
        CMFCToolBarButton* pTail =
            static_cast<CMFCToolBarButton*>(pThis->m_Buttons.GetTail());
        if (pTail && (pTail->m_nStyle & 0x1) != 0) {
            pThis->m_Buttons.RemoveTail();
            delete pTail;
        } else {
            break;
        }
    }

    impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(pThis);

    // TODO(clean-room): side-table divergence (see RemoveAllButtons) -- when
    // m_Buttons is empty this returns FALSE without touching the side table.
    return TRUE;
}
// Retail (0x18015b480): scans m_OrigResetButtons (0x11f8) for the button whose
// m_nID == uiID and removes it; returns FALSE when not found.
// Symbol: ?RemoveResetStateButton@CMFCToolBar@@IEAAHI@Z
extern "C" int MS_ABI impl__RemoveResetStateButton_CMFCToolBar__IEAAHI_Z(
    CMFCToolBar* pThis, unsigned int uiID)
{
    if (!pThis) return FALSE;

    INT_PTR nIndex = 0;
    CObList::POSITION pos = pThis->m_OrigResetButtons.GetHeadPosition();
    while (pos != CObList::POSITION(nullptr)) {
        CMFCToolBarButton* pButton =
            static_cast<CMFCToolBarButton*>(pThis->m_OrigResetButtons.GetNext(pos));
        if (pButton && pButton->m_nID == uiID) {
            CObList::POSITION removePos = pThis->m_OrigResetButtons.FindIndex(nIndex);
            if (removePos == CObList::POSITION(nullptr)) return FALSE;
            pThis->m_OrigResetButtons.RemoveAt(removePos);
            return TRUE;
        }
        ++nIndex;
    }
    return FALSE;
}
// Retail (0x180153880), transcribed:
//     strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);   // 0x1800d2070
//     if (nIndex == -1) nIndex = GetDlgCtrlID();                  // 0x1802a99a0
//     strKey.Format(uiID == -1 ? L"%TsMFCToolBar-%d"              // 0x18000da00
//                              : L"%TsMFCToolBar-%d%x", strPath, nIndex, uiID);
//     CSettingsStoreSP sp; CSettingsStore& reg = sp.Create(FALSE, FALSE); // 0x18012a550
//     return reg.DeleteKey(strKey, FALSE);                        // vslot 0x48
// Not implemented.  Correction to an earlier revision of this comment:
// CSettingsStore IS modelled -- phase4/src/core/app/CSettingsStore.cpp exports
// ?DeleteKey@CSettingsStore@@UEAAHPEB_WH@Z and
// phase4/src/core/app/CSettingsStoreSP.cpp exports Create.  The transcription
// is still not written because that store is an in-memory map that nothing in
// OpenMFC ever writes an MFCToolBar-* key into, so the DeleteKey would always
// miss and return FALSE, and because CSettingsStoreSP has no header
// declaration and no destructor thunk (its owner object would have to be
// hand-rolled and the store it allocates would leak).
// Symbol: ?RemoveStateFromRegistry@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__RemoveStateFromRegistry_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID)
{
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    // TODO(clean-room): not transcribed -- registry access via CSettingsStore.
    return FALSE;
}
// Retail (0x1801594b0), transcribed:
//     CCommandManager::ClearAllCmdImages(&g_CmdManager);          // 0x18002f010
//     for (each entry of the CMap at 0x1803b1fa8)
//         CCommandManager::SetCmdImage(&g_CmdManager, uiCmd, iImage, FALSE); // 0x18002ef30
//     for (pBar in the global toolbar list at 0x1803b2098)
//         if (pBar->CanBeRestored())        // vslot 0x718
//             pBar->RestoreOriginalState(); // vslot 0x720
//
// Re-checked against mfc140.dll (the ANSI twin the disassembler reads), where
// ResetAll is at RVA 0x157b10.  Instruction for instruction it is:
//     CCommandManager::ClearAllCmdImages(&afxCommandManager);   // 0x2f0d0
//     for ((uiCmd, iImage) in CMFCToolBar::m_DefaultImages)     // CMap 0x3aaf90
//         afxCommandManager.SetCmdImage(uiCmd, iImage, FALSE);  // 0x2eff0
//     for (node = <all-toolbars CObList>.m_pNodeHead; node; node = node->pNext) {
//         CMFCToolBar* pBar = node->data;                       // node + 0x10
//         if (pBar == NULL) AfxThrowInvalidArgException();      // 0x225b80
//         if (pBar->CanBeRestored())        pBar->RestoreOriginalState();
//     }
// The two vtable byte offsets are 0x718 and 0x720; in mfc140.dll the
// CMFCToolBar vftable is at 0x180313618 (its slot 0 is
// ?GetThisClass@CMFCToolBar@@SAPEAUCRuntimeClass@@XZ), where 0x718 ->
// ?CanBeRestored@CMFCToolBar@@UEBAHXZ and 0x720 ->
// ?RestoreOriginalState@CMFCToolBar@@UEAAHXZ.
//
// Read the two vtable addresses used in this file carefully: they name the
// SAME class in two different binaries, and an earlier revision of this comment
// wrongly "corrected" one into the other.  0x1803157c8, quoted by the older
// comments here, is the CMFCToolBar vftable in mfc140u.dll (the Unicode DLL
// OpenMFC reimplements, and the one those comments take their 0x180xxxxxx RVAs
// from): its +0x710/+0x718/+0x720 hold 0x180153880/0x180156950/0x180156520,
// which are exactly the mfc140u RemoveStateFromRegistry / CanBeRestored /
// RestoreOriginalState RVAs cited elsewhere in this file.  0x180313618 is the
// same vftable in mfc140.dll, the ANSI twin that scratchpad/wf2/disas.py reads,
// and it is the right one to pair with an "mfc140.dll RVA" citation.  Neither
// address is wrong; they simply belong to different images.
//
// Deviations, both deliberate:
//  * the CCommandManager half is skipped -- CMFCToolBar::m_DefaultImages is
//    inert storage here (a 56-byte blob, not a CMap), and
//    CCommandManager::ClearAllCmdImages / SetCmdImage are empty stubs in
//    phase4/src/featurepack/customize/CCommandManager.cpp, so there is nothing
//    to replay.
//  * the pane walk runs over the mfccore.cpp toolbar side table (every live
//    CMFCToolBar on this thread) instead of retail's private CObList of all
//    toolbars, which OpenMFC has no equivalent of.  This is the same
//    substitution AutoGrayInactiveImages above makes.
// CMFCToolBar::RestoreOriginalState is itself still a stub in this file, so
// today the loop has no visible effect; the control flow is reproduced so that
// it starts working the moment that stub is filled in.  Warning for whoever
// fills it in: this walks g_toolBarStates by reference, so a RestoreOriginalState
// that inserts or erases a g_toolBarStates entry would invalidate the iterator.
// Snapshot the keys into a local vector first if that becomes possible.
// Symbol: ?ResetAll@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__ResetAll_CMFCToolBar__SAXXZ()
{
    // TODO(clean-room): partially transcribed -- the CCommandManager /
    // m_DefaultImages replay is skipped (neither is modelled).
    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (!pBar) continue;
        if (impl__CanBeRestored_CMFCToolBar__UEBAHXZ(pBar)) {
            impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(pBar);
        }
    }
}
// ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A is an afxtempl.h
// CMap<UINT, UINT, int, int> -- the same 56-byte {vfptr, m_pHashTable,
// m_nHashTableSize, m_nCount, m_pFreeList, m_pBlocks, m_nBlockSize} shape
// decoded for m_AccelKeys near the top of this file.  Its retail .data image
// at 0x3b1f90 (mfc140u) reads, QWORD by QWORD:
//     +0x00 0x1802e2e88 (the CMap vftable)   +0x08 0   +0x10 0x11   +0x18 0
//     +0x20 0           +0x28 0               +0x30 0x0a
// i.e. the CMap constructor state m_nHashTableSize = 17, m_nBlockSize = 10,
// everything else NULL/0.  That state is reproduced below in the opaque
// 56-byte blob the Support header models the object as (the type is not
// this file's to change); the vfptr has no OpenMFC equivalent and stays NULL.
// In retail it is populated by LoadToolBarEx (entry 0x14e650; the SetAt call
// at 0x14e8ce inside it) and walked by ResetAll (entry 0x1594b0, which reads
// m_nCount at 0x3b1fa8 before the SetCmdImage replay); neither side is
// modelled here yet, so nothing in this tree populates or walks it.
namespace {
constexpr CMap_IIHH_56Bytes MakeDefaultImagesInitialState()
{
    CMap_IIHH_56Bytes m = {};
    m.data[0x10] = 17;   // m_nHashTableSize (little-endian low byte of the UINT at +0x10)
    m.data[0x30] = 10;   // m_nBlockSize     (low byte of the INT_PTR at +0x30)
    return m;
}
} // namespace
// Symbol: ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A =
    MakeDefaultImagesInitialState();
// The retail dynamic initialiser for this object (RVA 0x1d60, mfc140u) runs
// the COleDropSource constructor (0x25a350) on it, installs the
// CMFCToolBarDropSource vftable (0x3178d8), stores a QWORD 1 at +0x68 and
// zeroes +0x70..+0x8f (a DWORD, a QWORD and a 16-byte store; the object ends
// at +0x90, where ?m_Images@ begins) -- i.e. m_bDeleteOnDrop (+0x68) starts TRUE and
// m_bEscapePressed (+0x6c) / m_bDragStarted (+0x70) start FALSE (see the
// notes near the top of this file for how those offsets are read by
// OnDragEnter / OnDragLeave / OnLButtonDown).  The constant initialiser below
// reproduces that member state; the COleDropSource base (its vfptr and
// refcount) is not reproduced because CMFCToolBarDropSource is not declared in
// OpenMFC's headers and nothing in this tree hands the object to
// DoDragDrop.
namespace {
constexpr CToolBarDropSource_144Bytes MakeDropSourceInitialState()
{
    CToolBarDropSource_144Bytes s = {};
    s.data[kDropSourceDeleteOnDrop] = 1;   // BOOL m_bDeleteOnDrop = TRUE (little-endian low byte)
    return s;
}
} // namespace
// Symbol: ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
extern "C" CToolBarDropSource_144Bytes impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A =
    MakeDropSourceInitialState();
// The eight CMFCToolBarImages statics below are shared with other CMFCToolBar
// code through their exported symbols, so they keep plain (non-static)
// linkage; `extern "C"` plus an initializer in one declaration matches the
// house pattern in feature_static_data.cpp and keeps the object at the
// modeled 408-byte retail size.
// Symbol: ?m_Images@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_ColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_DisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_DisabledMenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeDisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_MenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Retail (0x18015b760): if m_hWnd:
//     if !m_bRoundShape (or GetVisualManager()==NULL): SetWindowRgn(hwnd,NULL,FALSE);
//     else: GetWindowRect -> CreateRoundRectRgn(0,0,w+1,h+1,4,4)
//           -> SetWindowRgn(hwnd, rgn, FALSE).
// The visual-manager gate (vslot 0x350) is not modeled, so the create path is
// taken whenever m_bRoundShape is set.
// Symbol: ?SetRoundedRgn@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__SetRoundedRgn_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    const HWND hwnd = pThis->GetSafeHwnd();
    if (!hwnd) return;

    // TODO(clean-room): retail gates the create path on GetVisualManager()
    // (vslot 0x350) being non-NULL; not modeled, so this assumes the manager
    // exists whenever m_bRoundShape is set.
    if (!pThis->m_bRoundShape) {
        // Retail clears the region when the shape flag is off.
        ::SetWindowRgn(hwnd, nullptr, FALSE);
        return;
    }

    RECT rc;
    ::GetWindowRect(hwnd, &rc);
    const int cx = rc.right - rc.left + 1;
    const int cy = rc.bottom - rc.top + 1;
    HRGN hRgn = ::CreateRoundRectRgn(0, 0, cx, cy, 4, 4);
    // Retail passes the (possibly NULL) region straight to SetWindowRgn;
    // SetWindowRgn with NULL clears the region, matching retail on failure.
    ::SetWindowRgn(hwnd, hRgn, FALSE);
}
// Retail (0x18015b010) is exactly six stores; fully transcribed.
// Symbol: ?SetSiblingToolBar@CMFCToolBar@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetSiblingToolBar_CMFCToolBar__QEAAXPEAV1__Z(
    CMFCToolBar* pThis, CMFCToolBar* pSibling)
{
    if (!pThis || !pSibling) return;

    pThis->m_pBrotherToolBar = pSibling;   // 0x12F8
    pThis->m_bHasBrother = 1;              // 0x110C
    pSibling->m_bHasBrother = 1;
    pSibling->m_pBrotherToolBar = pThis;
    pThis->m_bElderBrother = 1;            // 0x1110
    pSibling->m_bElderBrother = 0;
}
// Retail (0x180158920):
//     pButton = GetButton(nIndex); if NULL return;
//     if (bShowText) {
//         if (szText == NULL) vslot0x750(OnSetDefaultButtonText, pButton);
//         else                SetButtonText(nIndex, szText);   // sets button text
//     }
//     pButton->m_bImage = bShowImage;   // 0x10
//     pButton->m_bText  = bShowText;    // 0x0C
// The two text virtuals are not modeled; the string write is done directly.
// Symbol: ?SetToolBarBtnText@CMFCToolBar@@QEAAXIPEB_WHH@Z
extern "C" void MS_ABI impl__SetToolBarBtnText_CMFCToolBar__QEAAXIPEB_WHH_Z(
    CMFCToolBar* pThis, unsigned int nIndex, const wchar_t* szText, int bShowText, int bShowImage)
{
    if (!pThis) return;

    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nIndex));
    if (!pButton) return;

    if (bShowText) {
        if (szText) {
            // Retail delegates to CMFCToolBar::SetButtonText (0x18014f1c0),
            // which assigns the string to the button; reproduced inline.
            pButton->m_strText = szText;
        } else {
            // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
            // not modeled.
        }
    }
    pButton->m_bImage = bShowImage;   // @16 (retail stores the raw int)
    pButton->m_bText  = bShowText;    // @12
}
// Retail (0x18015b110) re-lays both toolbars into two rows using CMFCToolBar
// virtual layout slots (0x350, 0x4d0, 0x58) and CMFCControlBarImpl helpers,
// all unmodeled.
// Symbol: ?SetTwoRowsWithSibling@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__SetTwoRowsWithSibling_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the sibling two-row layout
    // (0x18015b110) walks virtual layout slots and CObList internals that
    // OpenMFC does not model.
}
// Retail (0x18014de40): if pImages && m_hbmImageWell && size matches the
// default button size globals -> store pImages to static m_pUserImages, return
// TRUE.  The size globals (0x1803b1cb8/0x1803b1cbc) and the m_pUserImages
// static (ordinal 8387, a different wave) are not modeled here.
// Symbol: ?SetUserImages@CMFCToolBar@@SAHPEAVCMFCToolBarImages@@@Z
extern "C" int MS_ABI impl__SetUserImages_CMFCToolBar__SAHPEAVCMFCToolBarImages___Z(
    CMFCToolBarImages* pImages)
{
    if (!pImages) return 0;
    // m_hbmImageWell (0x0A0) is protected in the header; read via the
    // documented offset.
    const void* pHbm = *reinterpret_cast<void* const*>(
        reinterpret_cast<const char*>(pImages) + 0xA0);
    if (!pHbm) return 0;
    // TODO(clean-room): retail compares m_sizeImage (0x68) against the default
    // button-size globals and stores pImages into the CMFCToolBar::m_pUserImages
    // static; neither is modeled in this wave.
    return 1;
}
// Retail (0x1801566f0) maps the command ID to status-bar text via the
// status-bar manager singleton (0x1803be360) and AFX_GLOBAL_DATA, then posts
// WM_SETMESSAGESTRING to the frame; all unmodeled.
// Symbol: ?ShowCommandMessageString@CMFCToolBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(
    CMFCToolBar* pThis, unsigned int nID)
{
    (void)nID;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the command -> string table
    // lookup and WM_SETMESSAGESTRING post depend on AFX_GLOBAL_DATA and the
    // status-bar manager singleton; not modeled.
}
// Retail (0x1801555b0) computes the toolbar size through the visual manager
// (vslot 0x338/0x3b20), CalcLayout and CMFCToolBarImages locking; unmodeled.
// Symbol: ?SizeToolBar@CMFCToolBar@@IEAAXHH@Z
extern "C" void MS_ABI impl__SizeToolBar_CMFCToolBar__IEAAXHH_Z(
    CMFCToolBar* pThis, int cx, int cy)
{
    (void)cx; (void)cy;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- layout math needs the visual
    // manager and CalcLayout; not modeled.
}
// Retail (0x180159950) walks the CObList comparing against m_OrigButtons and
// refreshes m_bResourceWasChanged / button styles; CObList contents live in a
// side table and the comparison helpers are unmodeled.
// Symbol: ?SmartUpdate@CMFCToolBar@@MEAAHAEBVCObList@@@Z
extern "C" int MS_ABI impl__SmartUpdate_CMFCToolBar__MEAAHAEBVCObList___Z(
    CMFCToolBar* pThis, const CObList& lstButtons)
{
    (void)lstButtons;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail list comparison
    // (0x180159950) is not modeled.
    return 0;
}
// Retail (0x180154c40) computes the size via SizeToolBar and CanBeResized.
// CSize is returned through the hidden return pointer used by MSVC for the
// exported ?AVCSize@@ methods (see cbarcore.cpp).
// Symbol: ?StretchPane@CMFCToolBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__StretchPane_CMFCToolBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CMFCToolBar* pThis, int cx, int cy)
{
    if (!pRet) return;
    new(pRet) CSize(0, 0);
    if (!pThis) return;
    (void)cx; (void)cy;
    // TODO(clean-room): transcribed partially -- retail sizes the pane through
    // SizeToolBar and CanBeResized (vslot 0x2d8); not modeled.
}
// Retail (RVA 0x156cb0, mfc140u) is the WH_MOUSE hook procedure SetHelpMode
// installs.  Its first branch is transcribed:
//     if (nCode != 0) return ::CallNextHookEx(m_hookMouseHelp, nCode, wParam, lParam);   // 0x3be360, IAT 0x2c6c68
// For HC_ACTION it then (not transcribed -- OnMouseMove@CMFCToolBar is a stub
// in this file) does, for wParam == WM_MOUSEMOVE only:
//     pBar = CWnd::FromHandle(::WindowFromPoint(pMouse->pt));       // 0x28ad70, IAT 0x2c7240
//     if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) {     // 0x3b15f8
//         ::ScreenToClient(pBar->m_hWnd, &pt);  pBar->OnMouseMove(0, pt);   // 0x150ce0
//     } else pBar = NULL;
//     if (m_pLastHookedToolbar && m_pLastHookedToolbar != pBar) {   // 0x3be340
//         old->m_bTracked = FALSE;  old->m_ptLastMouse = (-1,-1);   // +0x10e8, +0x12d8
//         if (old->m_iHighlighted >= 0) {                           // +0x1138
//             iOld = old->m_iHighlighted;  old->m_iHighlighted = -1;
//             if (!pBar || CMFCPopupMenu::GetParentToolBar(<CMFCPopupMenu parent of pBar>) != old)   // 0xb75f0
//                 { old->OnChangeHot(old->m_iHighlighted /* now -1 */);  old->InvalidateButton(iOld);  ::UpdateWindow(old->m_hWnd); }
//         }
//     }
//     m_pLastHookedToolbar = pBar;  return 0;      // retail does not chain for HC_ACTION either
// (An earlier revision of this comment said "CMFCPopupMenu::GetParentToolBar
// returns NULL" in OpenMFC; it does not -- featurepack/menu/CMFCPopupMenu.cpp
// gives it a real body.  The blocker is OnMouseMove.)
// Symbol: ?ToolBarMouseHookProc@CMFCToolBar@@KA_JH_K_J@Z
extern "C" __int64 MS_ABI impl__ToolBarMouseHookProc_CMFCToolBar__KA_JH_K_J_Z(
    int nCode, unsigned __int64 wParam, __int64 lParam)
{
    if (nCode != 0) {
        return ::CallNextHookEx(static_cast<HHOOK>(impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA),
                                nCode, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
    }
    // TODO(clean-room): the HC_ACTION mouse-move forwarding is not transcribed
    // (see above).
    return 0;
}
// Retail (0x180159570) looks the character up in the m_AccelKeys map and runs
// the matching command; the CMap internals (0x1318) are not modeled.
// Symbol: ?TranslateChar@CMFCToolBar@@UEAAHI@Z
extern "C" int MS_ABI impl__TranslateChar_CMFCToolBar__UEAAHI_Z(
    CMFCToolBar* pThis, unsigned int nChar)
{
    (void)nChar;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- m_AccelKeys map lookup not
    // modeled.
    return 0;
}
// Retail (0x180151e40) validates the button style range and refreshes the
// button window; internal button-state helpers are unmodeled.
// Symbol: ?UpdateButton@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateButton_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int iIndex)
{
    (void)iIndex;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180151e40) redraws
    // the button through unmodeled state helpers.
}
// Retail (0x180151890) calls CMFCToolBarImages::OnSysColorChange for seven
// static image lists (m_DisabledImages is skipped) and the eight embedded
// *_Locked lists, in this exact order.  Transcribed 1:1 against the static
// data exports defined at the top of this file.
// Symbol: ?UpdateImagesColor@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateImagesColor_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    // Static image lists (retail .data 0x3c25a0..0x3c3100).
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);

    // Embedded locked lists.
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ImagesLocked);              // 0x3F8
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ColdImagesLocked);          // 0x590

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_MenuImagesLocked);          // 0xD88
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledImagesLocked);      // 0x728
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledMenuImagesLocked);  // 0xF20

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeImagesLocked);         // 0x8C0
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeColdImagesLocked);     // 0xA58
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeDisabledImagesLocked); // 0xBF0
    // NB: retail does NOT touch m_DisabledImages here -- 7 static lists + 8
    // locked lists = 15 calls at 0x180151890.
}
// Retail (0x180159ce0) posts tooltip-add messages to m_pToolTip for every
// button with text; the tooltip manager and button walk are unmodeled.
// Symbol: ?UpdateTooltips@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180159ce0) walks
    // m_Buttons and drives m_pToolTip via TTM_ADDTOOL; not modeled.
}
// Retail (0x180155270) wraps buttons across rows using CalcLayout and a
// "wrap" font; the layout machinery is unmodeled.
// Symbol: ?WrapToolBar@CMFCToolBar@@QEAAHHHPEAVCDC@@HH@Z
extern "C" int MS_ABI impl__WrapToolBar_CMFCToolBar__QEAAHHHPEAVCDC__HH_Z(
    CMFCToolBar* pThis, int cx, int cy, CDC* pDC, int nMaxRows, int nMaxCols)
{
    (void)cx; (void)cy; (void)pDC; (void)nMaxRows; (void)nMaxCols;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail wrap layout (0x180155270)
    // is not modeled.
    return 0;
}
// Retail (0x18015a4a0): validate vt==VT_I4, resolve the button by child id,
// then for m_nID outside {0, 0xFFFFFFFF} run the button's OnClick virtual and
// post WM_COMMAND to the frame when it returns 0; for the two special ids call
// vslot 0x50.
// The child-id resolution (AccGetButtonByChildId, 0x18015ba90) numbers
// buttons 1-based among those passing the button vslot 0x188 gate; with that
// gate unmodeled the child id maps to GetButton(nChild - 1).
// Symbol: ?accDoDefaultAction@CMFCToolBar@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCToolBar__MEAAJUtagVARIANT___Z(
    CMFCToolBar* pThis, VARIANT varChild)
{
    if (!pThis) return S_OK;
    if (varChild.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const LONG nChild = varChild.lVal;
    if (nChild == 0) return S_OK;

    // Retail resolves the button through AccGetButtonByChildId (0x18015ba90);
    // child ids are 1-based, so index = nChild - 1.
    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nChild - 1));
    if (!pButton) return S_OK;

    // Retail (0x18015a4db..): `dec m_nID; cmp $0xFFFFFFFD; ja` — the vslot-0x50
    // path is taken only for m_nID == 0 or m_nID == 0xFFFFFFFF; every other id
    // runs the OnClick + WM_COMMAND path.
    if (pButton->m_nID != 0 && pButton->m_nID != 0xFFFFFFFFu) {
        // TODO(clean-room): retail invokes the button OnClick virtual (vslot
        // 0x58) and posts WM_COMMAND (0x111) only when it returns 0; the
        // virtual is not modeled.  Retail resolves the owner as
        // m_pWndParent@0xA0, falling back to GetParent(m_hWnd); m_pWndParent
        // is not modeled either, so the fallback is used here.
        HWND hOwner = ::GetParent(pThis->GetSafeHwnd());
        if (hOwner) {
            ::SendMessageW(hOwner, WM_COMMAND, pButton->m_nID, 0);
        }
    } else {
        // TODO(clean-room): retail calls button vslot 0x50; not modeled.
    }
    return S_OK;
}
// Retail (0x18015a250): null pVarOut -> E_INVALIDARG; ScreenToClient the point,
// walk the buttons, and on the first m_rect hit store the 1-based child id.
// Symbol: ?accHitTest@CMFCToolBar@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCToolBar__MEAAJJJPEAUtagVARIANT___Z(
    CMFCToolBar* pThis, long x, long y, VARIANT* pVarOut)
{
    if (!pThis) return S_OK;
    if (!pVarOut) return E_INVALIDARG;

    pVarOut->vt = 3;   // VT_I4
    pVarOut->lVal = 0;

    POINT pt;
    pt.x = x;
    pt.y = y;
    if (pThis->GetSafeHwnd()) {
        ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    }

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        // TODO(clean-room): retail first calls the button's vslot 0x188
        // (rect/visibility check) and only tests m_rect when it returns > 0.
        if (pButton->m_rect.PtInRect(CPoint(pt))) {
            pVarOut->lVal = i + 1;   // 1-based child id
            // TODO(clean-room): retail then calls the button's vslot 0x180 to
            // fill accessibility data; not modeled.
            return S_OK;
        }
    }
    return S_OK;
}
// Retail (0x18015a3e0), fully transcribed.  NAVDIR ids: LEFT=3, RIGHT=4,
// NEXT=5, PREVIOUS=6, FIRSTCHILD=7, LASTCHILD=8 (oleacc.h).  LEFT shares a
// body with PREVIOUS, RIGHT with NEXT; out-of-range navigation clears vt to
// VT_EMPTY and returns S_FALSE; FIRSTCHILD/LASTCHILD reject a non-zero start
// child with S_FALSE.
// Symbol: ?accNavigate@CMFCToolBar@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCToolBar__MEAAJJUtagVARIANT__PEAU2__Z(
    CMFCToolBar* pThis, long navDir, VARIANT varStart, VARIANT* pvarEnd)
{
    if (!pvarEnd) return E_INVALIDARG;

    pvarEnd->vt = 0;   // VT_EMPTY
    if (varStart.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const long nCount = pThis ? static_cast<long>(pThis->GetCount()) : 0;

    switch (navDir) {
    case 3:  // NAVDIR_LEFT
    case 6:  // NAVDIR_PREVIOUS
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n - 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next > 0) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 4:  // NAVDIR_RIGHT
    case 5:  // NAVDIR_NEXT
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n + 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next <= nCount) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 7:  // NAVDIR_FIRSTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = 1;
        return S_OK;

    case 8:  // NAVDIR_LASTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = nCount;
        return S_OK;

    default:
        // Retail (0x18015a488) returns S_FALSE for any other navDir, leaving
        // pvarEnd->vt at VT_EMPTY (set at entry).
        return S_FALSE;
    }
}
// Retail (0x18015a350): null ppDispChild -> E_INVALIDARG; otherwise zero the
// out-param and return S_FALSE unless the child resolves through the RTTI and
// IAccessibleFromWindow plumbing, which is unmodeled.
// Symbol: ?get_accChild@CMFCToolBar@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCToolBar__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCToolBar* pThis, VARIANT varChild, IDispatch** ppDispChild)
{
    if (!pThis) return S_FALSE;
    if (!ppDispChild) return E_INVALIDARG;
    *ppDispChild = nullptr;

    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;
    if (varChild.lVal == 0) return S_FALSE;

    // TODO(clean-room): retail resolves the button, checks IsKindOf(
    // CMFCToolBarButton) and calls IAccessibleFromWindow on its host window
    // (0x1803e9148); not modeled, so children are reported as not accessible.
    return S_FALSE;
}
// Retail (0x18015a320), fully transcribed: *pCountChildren = GetCount().
// Symbol: ?get_accChildCount@CMFCToolBar@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCToolBar__MEAAJPEAJ_Z(
    CMFCToolBar* pThis, long* pCountChildren)
{
    if (!pThis) return S_OK;
    if (!pCountChildren) return E_INVALIDARG;
    *pCountChildren = static_cast<long>(pThis->GetCount());
    return S_OK;
}
// CMFCToolBar::EnableLargeIcons(BOOL) — retail is a single store:
//     mov [rcx+0x10bc],edx ; ret
// It deliberately does not relayout; callers follow up with AdjustLayout.
// Symbol: ?EnableLargeIcons@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableLargeIcons_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int bEnable)
{
    if (!pThis) return;
    pThis->m_bLargeIconsAreEnbaled = bEnable;
}
// CMFCToolBar::EnableTextLabels(BOOL) — retail:
//     if (m_bMenuMode) return;        // cmp [rcx+0x10ec],0 ; jne done
//     m_bTextLabels = bEnable;        // mov [rcx+0x10c8],edx
//     AdjustLayout();                 // virtual, vtable slot +0x428
// The early-out matters: a menu bar silently ignores the request rather than
// relaying out.
// Symbol: ?EnableTextLabels@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTextLabels_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int bEnable)
{
    if (!pThis) return;
    if (pThis->m_bMenuMode) return;
    pThis->m_bTextLabels = bEnable;
    pThis->AdjustLayout();
}
// CMFCToolBar::GetHighlightedButton() — retail:
//     mov edx,[rcx+0x1138]      ; m_iHighlighted
//     test edx,edx ; jns take   ; negative -> no highlight
//     xor eax,eax ; ret
//     jmp GetButton             ; tail call with the index still in edx
// Symbol: ?GetHighlightedButton@CMFCToolBar@@QEBAPEAVCMFCToolBarButton@@XZ
extern "C" CMFCToolBarButton* MS_ABI
impl__GetHighlightedButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__XZ(const CMFCToolBar* pThis)
{
    if (!pThis) return nullptr;
    const int iHighlighted = pThis->m_iHighlighted;
    if (iHighlighted < 0) return nullptr;
    return pThis->GetButton(iHighlighted);
}
// CMFCToolBar::IsButtonHighlighted(int) — retail, in order:
//     if (m_bCustomizeMode && !m_bLocked) return FALSE;
//     if (m_bMenuMode) return iButton == m_iHighlighted;
//     if (iButton != m_iHighlighted && iButton != m_iButtonCapture) return FALSE;
//     if (m_iButtonCapture == -1) return TRUE;
//     return iButton == m_iButtonCapture;
// The customize-mode gate reads the class static, not an instance member.
// Symbol: ?IsButtonHighlighted@CMFCToolBar@@QEBAHH@Z
extern "C" int MS_ABI impl__IsButtonHighlighted_CMFCToolBar__QEBAHH_Z(
    const CMFCToolBar* pThis, int iButton)
{
    if (!pThis) return FALSE;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && pThis->m_bLocked == 0)
        return FALSE;

    const int iHighlighted   = pThis->m_iHighlighted;
    const int iButtonCapture = pThis->m_iButtonCapture;

    if (pThis->m_bMenuMode != 0)
        return iButton == iHighlighted ? TRUE : FALSE;

    if (iButton != iHighlighted && iButton != iButtonCapture)
        return FALSE;
    if (iButtonCapture == -1)
        return TRUE;
    return iButton == iButtonCapture ? TRUE : FALSE;
}
// CMFCToolBar::IsOneRowWithSibling() — retail:
//     mov rdx,[rcx+0x12f8]      ; m_pBrotherToolBar
//     mov rdx,[rdx+0x130]       ; brother->m_pDockBarRow
//     cmp [rcx+0x130],rdx       ; this->m_pDockBarRow
//     sete al
// Retail dereferences m_pBrotherToolBar without a null check (the method is
// only reached when a sibling exists). OpenMFC guards instead of faulting:
// a null-deref is not behaviour worth reproducing bit-for-bit, and FALSE is
// the answer consistent with "not sharing a row with a sibling".
// Symbol: ?IsOneRowWithSibling@CMFCToolBar@@QEAAHXZ
extern "C" int MS_ABI impl__IsOneRowWithSibling_CMFCToolBar__QEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;
    const CMFCToolBar* pBrother = pThis->m_pBrotherToolBar;
    if (!pBrother) return FALSE;
    return pThis->m_pDockBarRow == pBrother->m_pDockBarRow ? TRUE : FALSE;
}
// CMFCToolBar::CanBeRestored() const — retail:
//     xor eax,eax ; cmp DWORD PTR [rcx+0x1308],eax ; setne al ; ret
// A toolbar can be restored precisely when it remembers the resource id it was
// originally loaded from. Note the result is normalized to 0/1 by setne, so it
// is a genuine BOOL rather than the raw id.
// Symbol: ?CanBeRestored@CMFCToolBar@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeRestored_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;
    return pThis->m_uiOriginalResID != 0 ? TRUE : FALSE;
}
// Symbol: ?LoadToolBar@CMFCToolBar@@UEAAHIIIHIII@Z
extern "C" int MS_ABI impl__LoadToolBar_CMFCToolBar__UEAAHIIIHIII_Z(
    CMFCToolBar* pThis, unsigned int p0, unsigned int p1, unsigned int p2,
    int p3, unsigned int p4, unsigned int p5, unsigned int p6) {
    return (int)pThis->LoadToolBar(p0, p1, p2, p3, p4, p5, p6);
}
// Symbol: ?SetButtons@CMFCToolBar@@UEAAHPEBIHH@Z
extern "C" int MS_ABI impl__SetButtons_CMFCToolBar__UEAAHPEBIHH_Z(
    CMFCToolBar* pThis, const unsigned int* p0, int p1, int p2) {
    return (int)pThis->SetButtons(p0, p1, p2);
}
// Symbol: ?SetSizes@CMFCToolBar@@SAXUtagSIZE@@0@Z
extern "C" void MS_ABI impl__SetSizes_CMFCToolBar__SAXUtagSIZE__0_Z(SIZE p0, SIZE p1) {
    CMFCToolBar::SetSizes(p0, p1);
}
// Symbol: ?EnableDocking@CMFCToolBar@@UEAAXK@Z
extern "C" void MS_ABI impl__EnableDocking_CMFCToolBar__UEAAXK_Z(CMFCToolBar* pThis, unsigned long p0) {
    pThis->EnableDocking(p0);
}
// Symbol: ?AdjustLayout@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustLayout();
}
// Symbol: ?AdjustSize@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__AdjustSize_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustSize();
}
// Symbol: ?GetButtonText@CMFCToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CMFCToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}
// Symbol: ?GetButtonText@CMFCToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}
CMFCToolBar::CMFCToolBar() {
    // The eight CMFCToolBarImages members precede m_bLocked and keep their
    // constructed state. The span zeroed here is NOT purely POD, though: the
    // three CObLists (4488/4544/4600) and m_penDrag (4800) all sit after
    // m_bLocked, so they are re-constructed below. Zeroing already gives an
    // empty list and a null pen handle; what the memset destroys and placement
    // new restores is their vfptrs.
    char* const base = reinterpret_cast<char*>(this);
    std::memset(base + offsetof(CMFCToolBar, m_bLocked), 0,
                sizeof(CMFCToolBar) - offsetof(CMFCToolBar, m_bLocked));
    new (&m_Buttons) CObList();
    new (&m_OrigButtons) CObList();
    new (&m_OrigResetButtons) CObList();
    new (&m_penDrag) CPen();
    // m_AccelKeys: the retail CMap constructor state (hash table size 17,
    // block size 10; see the AccelMap notes near the top of this file, taken
    // from the retail ctor at RVA 0x14d2b0, mfc140u).  Without this the
    // zeroed block has a zero hash-table size and RebuildAccelerationKeys
    // could never populate it.
    AccelMapConstruct(AccelKeysOf(this));
    m_iButtonCapture = -1;
    m_iHighlighted   = -1;
    m_iSelected      = -1;
    m_iHot           = -1;
    m_iDragIndex     = -1;
    m_iAccHotItem    = -1;
    m_bGrayDisabledButtons = TRUE;
    m_bRouteCommandsViaFrame = TRUE;
    m_bShowHotBorder = TRUE;
}
CMFCToolBar::~CMFCToolBar() {
    // Retail (~CMFCToolBar, RVA 0x14d6b0 mfc140u) ends with
    // m_AccelKeys.RemoveAll() at 0x14d759; release the map's blocks the same
    // way.  (Retail also deletes the m_OrigButtons / m_OrigResetButtons items
    // and calls RemoveAllButtons first; those are not reproduced here.)
    AccelMapRemoveAll(AccelKeysOf(this));
    ClearToolBarState(this);
}
BOOL CMFCToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    g_toolBarStates[this].parent = pParentWnd;
    m_hWnd = ::CreateWindowExW(0, L"ToolbarWindow32", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    } else {
        ClearToolBarState(this);
    }
    return m_hWnd != nullptr;
}
BOOL CMFCToolBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}
BOOL CMFCToolBar::LoadToolBar(UINT, UINT, UINT, BOOL, UINT, UINT, UINT) { return TRUE; }
BOOL CMFCToolBar::LoadBitmap(UINT) { return TRUE; }
BOOL CMFCToolBar::SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bImages) {
    if (nIDCount < 0 || (nIDCount > 0 && lpIDArray == nullptr)) return FALSE;

    ToolBarState& state = g_toolBarStates[this];
    ClearToolBarButtons(state);

    for (int i = 0; i < nIDCount; ++i) {
        UINT id = lpIDArray[i];
        auto* button = new (std::nothrow) CMFCToolBarButton(id, bImages ? i : -1);
        if (!button) {
            ClearToolBarButtons(state);
            return FALSE;
        }
        if (id == 0) {
            button->m_nStyle = TBSTYLE_SEP;
            button->m_iImage = -1;
            button->m_bImage = FALSE;
        }
        state.buttons.push_back(button);
        state.ownedButtons.insert(button);
    }

    if (m_hWnd) {
        int existing = static_cast<int>(::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0));
        while (existing-- > 0) {
            ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
        }

        for (CMFCToolBarButton* button : state.buttons) {
            TBBUTTON tb = {};
            tb.iBitmap = button->m_iImage;
            tb.idCommand = static_cast<int>(button->m_nID);
            tb.fsState = TBSTATE_ENABLED;
            tb.fsStyle = button->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, reinterpret_cast<LPARAM>(&tb));
        }
    }

    return TRUE;
}
BOOL CMFCToolBar::ReplaceButton(UINT nID, const CMFCToolBarButton& button, BOOL) {
    ToolBarState& state = g_toolBarStates[this];
    for (size_t i = 0; i < state.buttons.size(); ++i) {
        CMFCToolBarButton* current = state.buttons[i];
        if (!current || current->m_nID != nID) continue;

        CMFCToolBarButton* replacement = CloneToolBarButton(button);
        if (!replacement) return FALSE;

        if (state.ownedButtons.erase(current) != 0) {
            delete current;
        }
        state.buttons[i] = replacement;
        state.ownedButtons.insert(replacement);

        if (m_hWnd) {
            TBBUTTONINFO tbi = {};
            tbi.cbSize = sizeof(TBBUTTONINFO);
            tbi.dwMask = TBIF_COMMAND | TBIF_IMAGE | TBIF_STATE | TBIF_STYLE;
            tbi.idCommand = static_cast<int>(replacement->m_nID);
            tbi.iImage = replacement->m_iImage;
            tbi.fsState = TBSTATE_ENABLED;
            tbi.fsStyle = replacement->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_SETBUTTONINFO, nID, reinterpret_cast<LPARAM>(&tbi));
        }
        return TRUE;
    }
    return FALSE;
}
int CMFCToolBar::GetCount() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? 0 : static_cast<int>(it->second.buttons.size());
}
CMFCToolBarButton* CMFCToolBar::GetButton(int nIndex) const {
    auto it = g_toolBarStates.find(this);
    if (it == g_toolBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.buttons.size())) return nullptr;
    return it->second.buttons[nIndex];
}
void CMFCToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    for (auto& entry : g_toolBarStates) {
        entry.second.buttonSize = CSize(sizeButton);
        entry.second.imageSize = CSize(sizeImage);
        if (entry.first && entry.first->GetSafeHwnd()) {
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        }
    }
}
CSize CMFCToolBar::GetButtonSize() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? CSize(23, 22) : it->second.buttonSize;
}
void CMFCToolBar::EnableDocking(DWORD dwDockStyle) {
    g_toolBarStates[this].dockStyle = dwDockStyle;
    // Retail's override is exactly two stores:
    //     mov DWORD PTR [rcx+0x108],0x1   ; m_dwControlBarStyle = 1
    //     mov DWORD PTR [rcx+0x100],edx   ; m_dwEnabledAlignment = dwDockStyle
    // The side table above is what OpenMFC's own docking code reads, but these
    // members are what a real client sees, and previously they were left at
    // zero -- so an MSVC caller could not tell a dockable toolbar from a fixed
    // one. Note the literal is assigned, not OR'd: retail discards any style
    // bits already present, and that is reproduced rather than "corrected".
    m_dwControlBarStyle  = 1;
    m_dwEnabledAlignment = dwDockStyle;
}
void CMFCToolBar::AdjustLayout() {
    ToolBarState& state = g_toolBarStates[this];
    const int count = static_cast<int>(state.buttons.size());
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(state.buttonSize.cx, state.buttonSize.cy));
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(state.imageSize.cx, state.imageSize.cy));
        ::SendMessageW(m_hWnd, TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
    }

    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& paneState = g_paneCoreState_Mfccore[this];
    if (paneState.recentRect.IsRectEmpty()) {
        paneState.recentRect = CRect(0, 0, std::max(1, count) * state.buttonSize.cx, state.buttonSize.cy);
    } else if (count > 0) {
        paneState.recentRect.right = paneState.recentRect.left + count * state.buttonSize.cx;
        paneState.recentRect.bottom = paneState.recentRect.top + state.buttonSize.cy;
    }
}
void CMFCToolBar::AdjustSize() {
    AdjustLayout();
}
CString CMFCToolBar::GetButtonText(int nIndex) const {
    CMFCToolBarButton* button = GetButton(nIndex);
    return button ? button->m_strText : CString();
}
void CMFCToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}
// The class statics below were generated as 8-byte `void*` blobs.  Sizes and
// initial values are taken from the retail image: the .data bytes at each
// symbol's RVA, and the distance to the next static in declaration order.
//
//   ?m_sizeButton@       0x3b1e38  17 00 00 00 16 00 00 00   ->  CSize(23, 22)
//   ?m_sizeCurButton@    0x3b1ee8  17 00 00 00 16 00 00 00   ->  CSize(23, 22)
//   ?m_sizeImage@        0x3b1cb8  10 00 00 00 0f 00 00 00   ->  CSize(16, 15)
//   ?m_sizeCurImage@     0x3b1c78  10 00 00 00 0f 00 00 00   ->  CSize(16, 15)
//   ?m_sizeMenuButton@   0x3b1d08  ff..ff ff..ff             ->  CSize(-1, -1)
//   ?m_sizeMenuImage@    0x3b1c38  ff..ff ff..ff             ->  CSize(-1, -1)
//
// (m_sizeImage's pair is confirmed a second time by the retail body at
// 0x180159280, which resets 0x3b1cb8/0x3b1cbc to 0x10/0xf after clearing the
// eight static image wells.)
//
// A plain two-LONG POD is used rather than CSize so the initialisation is a
// constant one -- these are read from other translation units and must not
// depend on a dynamic initialiser running first.  The 8-byte footprint is
// unchanged, so nothing that already links against them moves.
namespace {
struct StaticCSize { long cx; long cy; };
}

// ?m_lstBasicCommands@ 0x3b2020 and ?m_lstUnpermittedCommands@ 0x3b2058 are
// 0x38 = 56 bytes apart, which is sizeof(CList<UINT,UINT>) (CObject vfptr plus
// m_pNodeHead/m_pNodeTail/m_nCount/m_pNodeFree/m_pBlocks/m_nBlockSize).  Both
// are defined with the retail layout (UIntList, near the top of this file) and
// the retail .data image state: every field zero except m_nBlockSize = 10 at
// +0x30 (mfc140u .data bytes at 0x3b2050 / 0x3b2088 read 0a 00 00 00 ...).
// The vfptr retail keeps at +0 has no OpenMFC equivalent and stays NULL.
// Symbol: ?m_lstBasicCommands@CMFCToolBar@@1V?$CList@II@@A
extern "C" UIntList impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A =
    { nullptr, nullptr, nullptr, 0, nullptr, nullptr, kUIntListBlockSize };

// Symbol: ?m_lstUnpermittedCommands@CMFCToolBar@@1V?$CList@II@@A
extern "C" UIntList impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A =
    { nullptr, nullptr, nullptr, 0, nullptr, nullptr, kUIntListBlockSize };

// A pointer; 0x3b70d0/0x3be370 lives in .bss in the retail image, i.e. it
// starts NULL.  Already correct at 8 zero bytes.
// Symbol: ?m_pUserImages@CMFCToolBar@@1PEAVCMFCToolBarImages@@EA
extern "C" void* impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA = nullptr;

// Symbol: ?m_sizeButton@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeButton_CMFCToolBar__1VCSize__A = { 23, 22 };

// Symbol: ?m_sizeCurButton@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeCurButton_CMFCToolBar__1VCSize__A = { 23, 22 };

// Symbol: ?m_sizeCurImage@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeCurImage_CMFCToolBar__1VCSize__A = { 16, 15 };

// Symbol: ?m_sizeImage@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeImage_CMFCToolBar__1VCSize__A = { 16, 15 };

// Symbol: ?m_sizeMenuButton@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A = { -1, -1 };

// Symbol: ?m_sizeMenuImage@CMFCToolBar@@1VCSize@@A
extern "C" StaticCSize impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A = { -1, -1 };

// ?m_UsageCount@ sits at 0x3b1fd0 and the next static (?m_lstBasicCommands@)
// at 0x3b2020, i.e. 0x50 further on; the shadow struct in
// detail/CMFCCmdUsageCountSupport.h models the object as 0x48 bytes, so the
// remaining 8 are 16-byte alignment padding.  0x50 is used here as the safe
// upper bound.  The size matters: AddCommandUsage below hands this object to
// CMFCCmdUsageCount::AddCmd, which writes m_nTotalCount at +0x40 -- against the
// previous 8-byte blob that was a straight overrun.
// The retail .data image at 0x3b1fd0 (mfc140u) reads, QWORD by QWORD:
//     +0x00 0x1802dfee8 (CMFCCmdUsageCount vftable)   +0x08 0x1802dfeb8 (the CMap vftable)
//     +0x10 0   +0x18 0x11   +0x20 0   +0x28 0   +0x30 0   +0x38 0x0a   +0x40 0   +0x48 0
// i.e. the CMap<UINT,UINT,UINT,UINT> at +8 (the CU::m_map block of the shadow
// struct) in its constructor state -- m_nHashTableSize = 17 at +0x18,
// m_nBlockSize = 10 at +0x38 -- and m_nTotalCount = 0 at +0x40.  That state is
// reproduced below (one byte per line-group, little-endian); the two vfptrs
// have no OpenMFC equivalent and stay NULL.
// Symbol: ?m_UsageCount@CMFCToolBar@@1VCMFCCmdUsageCount@@A
extern "C" alignas(8) unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80] = {
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x00 CMFCCmdUsageCount vfptr (NULL here)
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x08 m_map vfptr (NULL here)
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x10 m_map.m_pHashTable
    17, 0, 0, 0, 0, 0, 0, 0,   // +0x18 m_map.m_nHashTableSize = 17
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x20 m_map.m_nCount
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x28 m_map.m_pFreeList
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x30 m_map.m_pBlocks
    10, 0, 0, 0, 0, 0, 0, 0,   // +0x38 m_map.m_nBlockSize = 10
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x40 m_nTotalCount = 0 (+4 pad)
    0,  0, 0, 0, 0, 0, 0, 0,   // +0x48 alignment tail
};

// ---------------------------------------------------------------------------
// Accessibility helper shared by the three Acc* exports below.
//
// Retail dispatches CMFCToolBarButton::GetAccCount through vtable slot 0x188.
// mfc140u exports exactly one GetAccCount body (?GetAccCount@CMFCToolBarButton@@UEAAHXZ,
// RVA 0x15eab0) and no derived button class overrides it, so inlining that one
// body here is equivalent to the virtual dispatch.  Fully transcribed:
//     if (m_bVisible == 0)              return 0;   // 0x64
//     if (::IsRectEmpty(&m_rect))       return 0;   // 0x68
//     return (~m_nStyle) & 1;                       // 0x28 -- 0 for a separator
// The three offsets match include/openmfc/afxmfc.h (m_nStyle @40, m_bVisible
// @100, m_rect @104).
namespace {
inline int ToolBarButtonAccCount(const CMFCToolBarButton* pButton)
{
    if (!pButton) return 0;
    if (!pButton->m_bVisible) return 0;
    if (::IsRectEmpty(reinterpret_cast<const RECT*>(&pButton->m_rect))) return 0;
    return static_cast<int>((~pButton->m_nStyle) & 1u);
}
} // namespace

// Retail (0x18015ba90) walks m_Buttons (0x1190) with a 1-based counter that is
// advanced only for buttons whose GetAccCount() is > 0, and returns the button
// whose counter equals lVal (NULL when the walk runs out):
//     nId = 1;
//     for (node = m_Buttons.head; node != NULL; node = node->next) {
//         pButton = node->data;                 // NULL -> AfxThrowInvalidArgException
//         if (pButton->GetAccCount() <= 0) continue;
//         if (nId == lVal) return pButton;
//         nId++;
//     }
//     return NULL;
// Deviation: OpenMFC keeps the live buttons in the mfccore.cpp side table, so
// the walk runs over GetCount()/GetButton() instead of the m_Buttons CObList
// (which stays empty here).  The retail NULL-element throw has no counterpart;
// a NULL entry is skipped by ToolBarButtonAccCount returning 0.
// Symbol: ?AccGetButtonByChildId@CMFCToolBar@@IEAAPEAVCMFCToolBarButton@@J@Z
extern "C" CMFCToolBarButton* MS_ABI impl__AccGetButtonByChildId_CMFCToolBar__IEAAPEAVCMFCToolBarButton__J_Z(
    CMFCToolBar* pThis, long lVal)
{
    if (!pThis) return nullptr;

    long nId = 1;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (ToolBarButtonAccCount(pButton) <= 0) continue;
        if (nId == lVal) return pButton;
        ++nId;
    }
    return nullptr;
}

// Retail (0x18015ba50), fully transcribed: the sum of GetAccCount() over every
// button in m_Buttons (0x1190).  Same side-table deviation as above.
// Symbol: ?AccGetButtonsCount@CMFCToolBar@@MEAAHXZ
extern "C" int MS_ABI impl__AccGetButtonsCount_CMFCToolBar__MEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return 0;

    int nTotal = 0;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        nTotal += ToolBarButtonAccCount(pThis->GetButton(i));
    }
    return nTotal;
}

// Retail (0x18015baf0) is the inverse of AccGetButtonByChildId: it maps a
// button index to the 1-based accessibility child id.
//     if (nButtonIndex < 0 || nButtonIndex >= m_Buttons.GetCount()) return 0;
//     nId = 1; iButton = 0;
//     for (node = m_Buttons.head; node != NULL; node = node->next) {
//         if (node->data->GetAccCount() > 0) {
//             if (iButton == nButtonIndex) return nId;
//             nId++;
//         }
//         iButton++;
//     }
//     return 0;
// Note the asymmetry that is deliberately reproduced: the running index
// iButton advances for every button, but nId only for accessible ones, so a
// hidden/separator button consumes an index without consuming a child id.
// Same side-table deviation as above.
// Symbol: ?AccGetChildIdByButtonIndex@CMFCToolBar@@IEAAHH@Z
extern "C" int MS_ABI impl__AccGetChildIdByButtonIndex_CMFCToolBar__IEAAHH_Z(
    CMFCToolBar* pThis, int nButtonIndex)
{
    if (!pThis) return 0;

    const int nCount = pThis->GetCount();
    if (nButtonIndex < 0 || nButtonIndex >= nCount) return 0;

    int nId = 1;
    for (int i = 0; i < nCount; ++i) {
        if (ToolBarButtonAccCount(pThis->GetButton(i)) > 0) {
            if (i == nButtonIndex) return nId;
            ++nId;
        }
    }
    return 0;
}

// Retail (0x18015a560):
//     if (afxGlobalData not yet initialised) afxGlobalData.Initialize();
//     if (*(int*)(&afxGlobalData + 0x278) == 0) return;   // accessibility gate
//     pButton = GetButton(iButton); if (pButton == NULL) return;
//     pButton->SetACCData(this, this->m_AccData);          // vslot 0x180, this+0x150
//     nChildId = AccGetChildIdByButtonIndex(iButton);
//     if (nChildId <= 0) return;
//     ::NotifyWinEvent(EVENT_OBJECT_FOCUS, m_hWnd, OBJID_CLIENT, nChildId);
// (0x1802c7210 resolves to USER32!NotifyWinEvent; 0x8005 is EVENT_OBJECT_FOCUS
// and 0xfffffffc is OBJID_CLIENT.)
//
// Two deviations, both because OpenMFC models neither side:
//   * the afxGlobalData flag at +0x278 is not modelled, so the gate is dropped
//     and the event is always sent.  NotifyWinEvent is inert when no
//     accessibility client is hooked, which is exactly when retail's flag is
//     clear, so the observable difference is confined to sessions that already
//     have a client attached;
//   * SetACCData (CMFCToolBarButton vslot 0x180) cannot be dispatched (no
//     button vtable) and the exported base thunk is a stub, so that call is
//     skipped.  (The CAccessibilityData itself IS declared -- CBasePane::m_AccData
//     at 0x150 in afxmfc.h; an earlier revision of this comment said it was not.)
// Symbol: ?AccNotifyObjectFocusEvent@CMFCToolBar@@MEAAXH@Z
extern "C" void MS_ABI impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(
    CMFCToolBar* pThis, int iButton)
{
    if (!pThis) return;

    // TODO(clean-room): retail gates on afxGlobalData+0x278 and calls
    // pButton->SetACCData(this, m_AccData) first; neither is modelled.
    CMFCToolBarButton* pButton = pThis->GetButton(iButton);
    if (!pButton) return;

    const int nChildId = impl__AccGetChildIdByButtonIndex_CMFCToolBar__IEAAHH_Z(pThis, iButton);
    if (nChildId <= 0) return;

    ::NotifyWinEvent(EVENT_OBJECT_FOCUS, pThis->GetSafeHwnd(), OBJID_CLIENT, nChildId);
}

// Retail (RVA 0x156fa0, mfc140u), fully transcribed:
//     if (m_lstBasicCommands.Find(uiCmd) == NULL)      // 0x11f1c on 0x3b2020
//         m_lstBasicCommands.AddTail(uiCmd);           // 0x12284
// Runs on the retail-layout list through the UIntList helpers near the top of
// this file.
// Symbol: ?AddBasicCommand@CMFCToolBar@@SAXI@Z
extern "C" void MS_ABI impl__AddBasicCommand_CMFCToolBar__SAXI_Z(unsigned int uiCmd) {
    if (UIntListFind(&impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A, uiCmd) == nullptr) {
        UIntListAddTail(&impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A, uiCmd);
    }
}

// Retail (0x18015a780) is a single tail jump, fully transcribed:
//     jmp CMFCCmdUsageCount::AddCmd(&CMFCToolBar::m_UsageCount, uiCmd)
// (rcx = 0x1803b1fd0 = ?m_UsageCount@CMFCToolBar@@1VCMFCCmdUsageCount@@A,
//  target 0x1800232e0 = ?AddCmd@CMFCCmdUsageCount@@QEAAXI@Z).
// AddCmd itself is only partly modelled -- see the note on its definition in
// featurepack/customize/CMFCCmdUsageCount.cpp -- but the command-range gate and
// the total-count bump it does implement now run against real storage, which is
// why m_UsageCount above had to grow from 8 bytes to the retail 80.
// Symbol: ?AddCommandUsage@CMFCToolBar@@SAXI@Z
extern "C" void MS_ABI impl__AddCommandUsage_CMFCToolBar__SAXI_Z(unsigned int uiCmd) {
    impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(
        impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A, uiCmd);
}

// Retail (0x180155fd0), transcribed but NOT implemented -- see below.
//     iIndex = ButtonToIndex(pButton);
//     if (iIndex <= 0) return;                       // nothing before button 0
//     if (GetCurrentAlignment() & (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM))  // 0xa000
//          nDelta = ptDrop.x - ptStart.x;            // horizontal bar
//     else nDelta = ptDrop.y - ptStart.y;
//     if (::abs(nDelta) < 6) return;
//     pPrev = GetButton(iIndex - 1);
//     if (nDelta > 0) {
//         if (pPrev->m_nStyle & TBBS_SEPARATOR) return;      // already there
//         InsertSeparator(iIndex);                           // vslot 0x6a0
//     } else {
//         if (!(pPrev->m_nStyle & TBBS_SEPARATOR)) return;
//         if (pPrev->m_bVisible) RemoveButton(iIndex - 1);    // vslot 0x6a8
//     }
//     AdjustLayout();                                        // vslot 0x428
//     m_iSelected = -1;
//     ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|
//                                        RDW_ALLCHILDREN|RDW_FRAME);  // 0x505
// Vtable slots named from the retail CMFCToolBar vtable at 0x1803157c8
// (0x338 -> CBasePane::GetCurrentAlignment, 0x6a0 -> InsertSeparator,
//  0x6a8 -> RemoveButton, 0x428 -> AdjustLayout).
//
// Left a stub deliberately: InsertSeparator (now implemented below) inserts
// into the mfccore.cpp side table that GetCount()/GetButton() read, while
// RemoveButton above still operates on the m_Buttons CObList, so a
// transcription would create separators during a customize drag that the
// remove branch could never find again -- an asymmetry that is worse than
// doing nothing until the two storages are unified.
// Symbol: ?AddRemoveSeparator@CMFCToolBar@@MEAAXPEBVCMFCToolBarButton@@AEBVCPoint@@1@Z
extern "C" void MS_ABI impl__AddRemoveSeparator_CMFCToolBar__MEAAXPEBVCMFCToolBarButton__AEBVCPoint__1_Z(
    CMFCToolBar* pThis, const CMFCToolBarButton* pButton,
    const CPoint* ptStart, const CPoint* ptDrop)
{
    (void)pThis; (void)pButton; (void)ptStart; (void)ptDrop;
    // TODO(clean-room): not implemented -- needs a working InsertSeparator.
}

// Retail (0x1801569b0), fully transcribed: it builds a throwaway CMFCToolBar on
// the stack (ctor 0x18014d2b0, dtor 0x18014d6b0) purely so the image-loading
// path runs, and returns what LoadToolBar (0x18014e300) returns:
//     CMFCToolBar bar;
//     return bar.LoadToolBar(uiResID, uiColdResID, uiMenuResID,
//                            /*bLocked*/ FALSE, uiDisabledResID,
//                            uiMenuDisabledResID, /*uiHotResID*/ uiBmpResID);
// The argument shuffle is worth spelling out because it is not the identity:
// the collection's uiBmpResID lands in LoadToolBar's *uiHotResID* slot (arg 8),
// bLocked is hard-wired to 0 (arg 5), and the remaining four pass straight
// through.
// Caveat: OpenMFC's CMFCToolBar::LoadToolBar accepts everything without parsing
// the RT_TOOLBAR resource, so this reproduces retail's control flow and return
// value but loads no images.
// Symbol: ?AddToolBarForImageCollection@CMFCToolBar@@SAHIIIIII@Z
extern "C" int MS_ABI impl__AddToolBarForImageCollection_CMFCToolBar__SAHIIIIII_Z(
    unsigned int uiResID, unsigned int uiBmpResID, unsigned int uiColdResID,
    unsigned int uiMenuResID, unsigned int uiDisabledResID,
    unsigned int uiMenuDisabledResID)
{
    CMFCToolBar bar;
    return bar.LoadToolBar(uiResID, uiColdResID, uiMenuResID, FALSE,
                           uiDisabledResID, uiMenuDisabledResID, uiBmpResID) ? 1 : 0;
}

// Retail (0x180155990) lays every button out row by row: it bails when
// m_Buttons is empty (0x11a0) or m_hWnd is NULL (0x40), reads
// GetCurrentAlignment() (vslot 0x338) to pick the horizontal or vertical pass,
// takes a CClientDC (0x1802a3b20) and then drives each button's
// OnCalculateSize/OnMove through its vtable while tracking wrap points.
// The blocker is the per-button OnCalculateSize/OnMove virtuals: OpenMFC has no
// dispatchable CMFCToolBarButton vtable and both exist only as empty exported
// stubs, so every measured size would come back zero.  (CClientDC itself is NOT
// the obstacle -- an earlier revision of this comment said the "CClientDC
// plumbing" was unmodelled, but include/openmfc/afxwin.h declares CClientDC and
// phase4/src/core/gdi/CClientDC.cpp implements it.)
// Re-read on mfc140u at RVA 0x155990 (entry): the shape above holds.  Beyond
// the per-button virtuals (OnCalculateSize is button vslot 0x48; the base
// export is an empty stub with a placeholder parameter list) the body also
// needs the font it selects -- DEFAULT_GUI_FONT via CDC vslot 0x58 when
// m_bIsDlgControl, else afxGlobalData.fontRegular / fontVert (+0x1a8 /
// +0x208), which OpenMFC's zero afxGlobalData does not hold, and retail
// throws when that SelectObject returns NULL -- and the customize button's
// extended members (its CObList at +0x160 is emptied and refilled with the
// buttons that did not fit, +0x148 / +0x128 / +0x178 are read), which are
// not modelled.  The tail is UpdateTooltips (0x159ce0) and
// RedrawCustomizeButton (0x15b870) after the font is restored.  Left a stub.
// Symbol: ?AdjustLocations@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis) {
    (void)pThis;
    // TODO(clean-room): not transcribed -- needs the button layout virtuals.
}

// Retail (0x18015b3a0), transcribed:
//     m_bAutoGrayInactiveImages = bEnable;            // 0x1803be358
//     m_nGrayImagePercentage    = nGrayImagePercentage; // 0x1803be34c
//     if (bEnable) { m_Images.CopyTo(m_ColdImages);
//                    m_ColdImages.GrayImages(m_nGrayImagePercentage); }
//     else         { m_ColdImages.Clear(); }
//     for (pBar in the global toolbar list at 0x1803b2098) {
//         if (CWnd::FromHandlePermanent(pBar->m_hWnd) == NULL) continue;
//         if (pBar->m_bLocked) {                       // 0x10b8
//             if (m_bAutoGrayInactiveImages) {
//                 pBar->m_ImagesLocked.CopyTo(pBar->m_ColdImagesLocked);
//                 pBar->m_ColdImagesLocked.GrayImages(m_nGrayImagePercentage);
//             } else pBar->m_ColdImagesLocked.Clear();
//         }
//         if (bRedrawAllToolbars)
//             ::RedrawWindow(pBar->m_hWnd, NULL, NULL, 0x585);
//     }
// (0x18016f2f0 = CMFCToolBarImages::CopyTo, 0x180170110 =
//  CMFCToolBarImages::GrayImages, 0x18016f690 = CMFCToolBarImages::Clear,
//  0x1802c7130 = USER32!RedrawWindow.  Note the second CopyTo/GrayImages pair
//  re-reads the class static rather than the parameter, which is reproduced.)
//
// Deviations: OpenMFC has no equivalent of the retail global pane list, so the
// loop runs over the mfccore.cpp toolbar side table (every live CMFCToolBar on
// this thread), and the CWnd::FromHandlePermanent test -- "this pane object is
// attached to a live window" -- becomes a GetSafeHwnd() test, because
// CMFCToolBar::Create does not put its HWND in the permanent map.
// CMFCToolBarImages::CopyTo and ::GrayImages are themselves partial in this
// repo (the pixel work is not modelled); only their state writes take effect.
// Symbol: ?AutoGrayInactiveImages@CMFCToolBar@@SAXHHH@Z
extern "C" void MS_ABI impl__AutoGrayInactiveImages_CMFCToolBar__SAXHHH_Z(
    int bEnable, int nGrayImagePercentage, int bRedrawAllToolbars)
{
    impl__m_bAutoGrayInactiveImages_CMFCToolBar__1HA = bEnable;
    impl__m_nGrayImagePercentage_CMFCToolBar__1HA    = nGrayImagePercentage;

    if (bEnable) {
        impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(
            &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A,
            &impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
        impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(
            &impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A,
            impl__m_nGrayImagePercentage_CMFCToolBar__1HA);
    } else {
        impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    }

    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (!pBar) continue;
        const HWND hwnd = pBar->GetSafeHwnd();
        if (!hwnd) continue;

        if (pBar->m_bLocked) {
            if (impl__m_bAutoGrayInactiveImages_CMFCToolBar__1HA) {
                impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(
                    &pBar->m_ImagesLocked, &pBar->m_ColdImagesLocked);
                impl__GrayImages_CMFCToolBarImages__QEAAHH_Z(
                    &pBar->m_ColdImagesLocked,
                    impl__m_nGrayImagePercentage_CMFCToolBar__1HA);
            } else {
                impl__Clear_CMFCToolBarImages__QEAAXXZ(&pBar->m_ColdImagesLocked);
            }
        }

        if (bRedrawAllToolbars) {
            ::RedrawWindow(hwnd, nullptr, nullptr, 0x585);
        }
    }
}

// Retail (0x18014f000), fully transcribed: a linear scan of m_Buttons (0x1190)
// returning the 0-based position of pButton, or -1.
//     iIndex = 0;
//     for (node = m_Buttons.head; node != NULL; node = node->next) {
//         if (node->data == NULL) AfxThrowInvalidArgException();  // 0x180227720
//         if (node->data == pButton) return iIndex;
//         iIndex++;
//     }
//     return -1;
// Deviation: the scan runs over the mfccore.cpp side table via
// GetCount()/GetButton(), which is where OpenMFC keeps the live buttons; the
// retail NULL-element throw therefore has no counterpart (a NULL slot simply
// does not match).
// Symbol: ?ButtonToIndex@CMFCToolBar@@QEBAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(
    const CMFCToolBar* pThis, const CMFCToolBarButton* pButton)
{
    if (!pThis) return -1;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        if (pThis->GetButton(i) == pButton) return i;
    }
    return -1;
}

// Retail (0x18014f0b0), fully transcribed: it packs the two BOOLs into
// CalcLayout's mode word and tail-calls CalcLayout with an unconstrained
// length.
//     dwMode = (bStretch ? LM_STRETCH : 0) | (bHorz ? LM_HORZ : 0);   // 1 | 2
//     return CalcLayout(dwMode, -1);                                  // vslot 0x780
// (The vtable slot is named from the retail CMFCToolBar vtable at 0x1803157c8:
// 0x780 -> ?CalcLayout@CMFCToolBar@@MEAA?AVCSize@@KH@Z.)
//
// Deviation: OpenMFC's DLL-internal vtable does not carry that slot, so the
// CalcLayout export below is called directly rather than virtually -- a derived
// class overriding CalcLayout would not be re-entered here.  CalcLayout is
// itself a stub, so the returned size is (0, 0) rather than a real layout.
//
// The generated stub this replaces also had the struct return wrong: the hidden
// CSize slot arrives in rdx and must be returned in rax, and it returned
// nullptr.  Both are fixed here and in CalcLayout/CalcSize below.
// Symbol: ?CalcFixedLayout@CMFCToolBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCToolBar__UEAA_AVCSize__HH_Z(
    CMFCToolBar* pThis, CSize* pRet, int bStretch, int bHorz)
{
    const unsigned long dwMode =
        static_cast<unsigned long>((bStretch ? 1 : 0) | (bHorz ? 2 : 0));
    return impl__CalcLayout_CMFCToolBar__MEAA_AVCSize__KH_Z(pThis, pRet, dwMode, -1);
}

// Retail (0x180154d70) computes the docked size: it clears a flag on
// m_pCustomizeBtn (0x1300), caches CalcMaxButtonHeight() (vslot 0x848) into
// m_nMaxBtnHeight (0x1130), works out an available length from m_dwStyle
// (0x104) and the LM_* bits, calls WrapToolBar (0x1801555b0) and CalcSize
// (vslot 0x558), then adds the pane borders (0x1800a1950) and clamps against
// 0x7fff before calling RebuildAccelerationKeys (0x1801563b0).  Almost every
// step goes through a virtual or an internal helper OpenMFC does not model, so
// only the ABI is honoured here: the hidden CSize slot is zeroed and returned.
// Symbol: ?CalcLayout@CMFCToolBar@@MEAA?AVCSize@@KH@Z
extern "C" CSize* MS_ABI impl__CalcLayout_CMFCToolBar__MEAA_AVCSize__KH_Z(
    CMFCToolBar* pThis, CSize* pRet, unsigned long dwMode, int nLength)
{
    (void)pThis; (void)dwMode; (void)nLength;
    // TODO(clean-room): not transcribed -- wrapping, CalcSize and the border
    // helpers are unmodelled.  Retail also stores CalcMaxButtonHeight() into
    // m_nMaxBtnHeight here; that is skipped because CalcMaxButtonHeight is a
    // stub and would write a bogus 0.
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}

// Retail (0x1801590b0) measures the tallest button: it clears m_bDrawTextLabels
// (0x10cc), returns 0 immediately unless m_bTextLabels (0x10c8) is set AND the
// bar is horizontally aligned, then takes a CClientDC (0x1802a3b20), selects
// either the DEFAULT_GUI_FONT stock font (dialog-control bars) or
// afxGlobalData's regular font, walks m_Buttons calling OnSetDefaultButtonText
// (vslot 0x750) for empty labels and OnCalculateSize (button vslot 0x48) for
// each button whose text is drawn below its image, keeps the maximum height
// and finally sets m_bDrawTextLabels from the comparison against the default
// button height.
// The blockers are the fonts (afxGlobalData is unmodelled) and the two virtuals
// -- CMFCToolBarButton has no dispatchable vtable here and OnCalculateSize is
// an empty stub, so every height would measure zero.  CClientDC is not a
// blocker: an earlier revision of this comment listed "the DC" among the
// unmodelled pieces, but phase4/src/core/gdi/CClientDC.cpp implements it.
// Re-read on mfc140u at RVA 0x1590b0 (entry), instruction for instruction:
//     bHorz = (GetCurrentAlignment() & 0xa000) != 0;              // vslot 0x338
//     m_bDrawTextLabels = FALSE;                                  // +0x10cc
//     if (!m_bTextLabels || !bHorz) return 0;                     // +0x10c8
//     CClientDC dc(this);                                         // 0x2a3b20
//     CFont* pOld = m_bIsDlgControl /*+0xf4*/ ? dc.SelectStockObject(DEFAULT_GUI_FONT /*0x11*/)   // CDC vslot 0x58
//                                             : dc.SelectObject(&afxGlobalData.fontRegular /*+0x1a8*/);   // CDC vslot 0x60
//     if (pOld == NULL) AfxThrowInvalidArgException();
//     int nMax = 0;
//     for (node = m_Buttons.head; node; node = node->next) {
//         p = node->data;  if (!p) AfxThrowInvalidArgException();
//         if (!p->m_bTextBelow /*+0x1c*/) continue;
//         if (p->m_strText.IsEmpty()) OnSetDefaultButtonText(p);     // vslot 0x750
//         SIZE sz = p->OnCalculateSize(&dc, GetButtonSize(), bHorz);  // button vslot 0x48; 0x239b4 is the
//         nMax = max(nMax, sz.cy);                                    //   inlined GetButtonSize
//     }
//     m_bDrawTextLabels = nMax > GetButtonSize().cy;
//     dc.SelectObject(pOld);                                      // 0x2a2730
//     return nMax;
// The early-return side effect (m_bDrawTextLabels cleared) IS reproduced
// below; the measurement is not, for the reasons above: OpenMFC's
// afxGlobalData holds no CFont at +0x1a8 (SelectObject would fail, which is
// retail's throw path), and the base OnCalculateSize export is an empty stub
// with a placeholder parameter list, so nothing could be measured.
// Symbol: ?CalcMaxButtonHeight@CMFCToolBar@@MEAAHXZ
extern "C" int MS_ABI impl__CalcMaxButtonHeight_CMFCToolBar__MEAAHXZ(CMFCToolBar* pThis) {
    if (!pThis) return 0;
    pThis->m_bDrawTextLabels = FALSE;
    // TODO(clean-room): not transcribed past the gate -- needs the button size
    // virtual and the afxGlobalData fonts (see above).
    return 0;
}

// Retail (0x180154f90) returns CSize(0,0) at once when m_Buttons is empty
// (0x11a0), and otherwise runs the full row/column measurement: a CClientDC,
// the menu or global font, GetRowHeight (vslot 0x6b8) and GetColumnWidth
// (vslot 0x6c0), then a walk of m_Buttons accumulating wrapped extents.  None
// of that is modelled here, so only the ABI is honoured: the hidden CSize slot
// is zeroed and returned -- which does coincide with retail's empty-toolbar
// answer.
// Symbol: ?CalcSize@CMFCToolBar@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__CalcSize_CMFCToolBar__UEAA_AVCSize__H_Z(
    CMFCToolBar* pThis, CSize* pRet, int bVertDock)
{
    (void)pThis; (void)bVertDock;
    // TODO(clean-room): not transcribed -- needs CClientDC and the row/column
    // virtuals.
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}

// Retail (0x18015b2f0), fully transcribed:
//     if (!m_bHasBrother)        return FALSE;   // 0x110c
//     if (!m_pBrotherToolBar)    return FALSE;   // 0x12f8
//     dwThis    = GetCurrentAlignment();                    // vslot 0x338
//     dwBrother = m_pBrotherToolBar->GetCurrentAlignment();
//     if (dwBrother == 0)        return FALSE;
//     return dwThis == dwBrother;
// GetCurrentAlignment is not overridden by CMFCToolBar -- slot 0x338 of the
// retail CMFCToolBar vtable (0x1803157c8) points at
// ?GetCurrentAlignment@CBasePane@@UEBAKXZ (RVA 0xcdb0), whose whole body is
// `return m_dwStyle & 0xF000;` -- so it is inlined here rather than dispatched.
// m_dwStyle sits at 0x104, which include/openmfc/afxmfc.h already pins.
// Symbol: ?CanHandleSiblings@CMFCToolBar@@QEAAHXZ
extern "C" int MS_ABI impl__CanHandleSiblings_CMFCToolBar__QEAAHXZ(CMFCToolBar* pThis) {
    if (!pThis) return FALSE;
    if (!pThis->m_bHasBrother) return FALSE;

    CMFCToolBar* pBrother = pThis->m_pBrotherToolBar;
    if (!pBrother) return FALSE;

    const DWORD dwThis    = pThis->m_dwStyle & 0xF000u;
    const DWORD dwBrother = pBrother->m_dwStyle & 0xF000u;
    if (dwBrother == 0) return FALSE;
    return dwThis == dwBrother ? TRUE : FALSE;
}

// Retail (0x180159720), transcribed:
//     m_Images.Clear(); m_ColdImages.Clear(); m_MenuImages.Clear();
//     m_DisabledImages.Clear(); m_DisabledMenuImages.Clear();
//     m_LargeImages.Clear(); m_LargeColdImages.Clear();
//     m_LargeDisabledImages.Clear();
//     m_DefaultImages.RemoveAll();          // 0x1803b1f90
//     m_UsageCount.<map at +8>.RemoveAll(); // 0x1803b1fd8
//     m_UsageCount.m_nTotalCount = 0;       // 0x1803b2010 == m_UsageCount+0x40
//     CMFCToolBarImages::CleanUp();         // tail jump, 0x18016b080
// The eight statics are the ones defined above in this file; the retail
// addresses 0x1803c25a0/0x3c2740/0x3c28e0/0x3c2a80/0x3c2c20/0x3c2dc0/0x3c2f60/
// 0x3c3100 map to m_Images / m_ColdImages / m_MenuImages / m_DisabledImages /
// m_DisabledMenuImages / m_LargeImages / m_LargeColdImages /
// m_LargeDisabledImages in that order.  The one omission is the CMap::RemoveAll
// on m_DefaultImages and on the usage-count map: OpenMFC models neither CMap,
// though the total-count reset next to it is done because m_UsageCount is now
// real storage.
// Symbol: ?CleanUpImages@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__CleanUpImages_CMFCToolBar__SAXXZ() {
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);

    // TODO(clean-room): retail also empties the m_DefaultImages CMap and the
    // per-command CMap inside m_UsageCount; neither CMap is modelled.
    // m_nTotalCount lives at +0x40 of the CMFCCmdUsageCount object (see
    // detail/CMFCCmdUsageCountSupport.h).
    std::memset(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A + 0x40, 0,
                sizeof(unsigned int));

    impl__CleanUp_CMFCToolBarImages__SAXXZ();
}

// Retail (0x1801597b0), fully transcribed:
//     if (!m_bLocked) return;                       // 0x10b8
//     m_ImagesLocked.Clear();              // 0x3f8
//     m_ColdImagesLocked.Clear();          // 0x590
//     m_DisabledImagesLocked.Clear();      // 0x728
//     m_LargeImagesLocked.Clear();         // 0x8c0
//     m_LargeColdImagesLocked.Clear();     // 0xa58
//     m_LargeDisabledImagesLocked.Clear(); // 0xbf0
//     m_MenuImagesLocked.Clear();          // 0xd88
//     m_DisabledMenuImagesLocked.Clear();  // 0xf20
// The eight offsets are exactly the ones include/openmfc/afxmfc.h documents for
// the locked image block, in the same order.
// Symbol: ?CleanUpLockedImages@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUpLockedImages_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis) {
    if (!pThis) return;
    if (!pThis->m_bLocked) return;

    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_ImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_ColdImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeColdImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeDisabledImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_MenuImagesLocked);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledMenuImagesLocked);
}

// Retail (0x18014eed0), fully transcribed: the index of the first button at or
// after iIndexFirst whose m_nID (0x24) is nIDFind, or -1.
//     iIndex = 0;
//     for (node = m_Buttons.head; node != NULL; node = node->next) {
//         if (node->data == NULL) AfxThrowInvalidArgException();   // 0x180227720
//         if (iIndex >= iIndexFirst && node->data->m_nID == nIDFind) return iIndex;
//         iIndex++;
//     }
//     return -1;
// Note the guard is `iIndex >= iIndexFirst`, i.e. iIndexFirst is inclusive.
// Same side-table deviation as ButtonToIndex above; m_nID @36 in
// include/openmfc/afxmfc.h matches the retail 0x24.
// Symbol: ?CommandToIndex@CMFCToolBar@@QEBAHIH@Z
extern "C" int MS_ABI impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(
    const CMFCToolBar* pThis, unsigned int nIDFind, int iIndexFirst)
{
    if (!pThis) return -1;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        if (i < iIndexFirst) continue;
        const CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->m_nID == nIDFind) return i;
    }
    return -1;
}
// Retail (mfc140.dll RVA 0x154850): it calls
// ?CreateFromOleData@CMFCToolBarButton@@SAPEAV1@PEAVCOleDataObject@@@Z
// (0x15adf0) to rebuild a button from the drag-and-drop payload, then, if that
// button carries text, splits m_strText at the first TAB and keeps only the
// part before it as the label.
// Not implemented: OpenMFC models neither COleDataObject clipboard extraction
// nor CMFCToolBarButton::CreateFromOleData (itself a stub), so there is nothing
// to build a button from.
// Symbol: ?CreateDroppedButton@CMFCToolBar@@MEAAPEAVCMFCToolBarButton@@PEAVCOleDataObject@@@Z
extern "C" CMFCToolBarButton* MS_ABI impl__CreateDroppedButton_CMFCToolBar__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject) {
    (void)pThis; (void)pDataObject;
    return nullptr;
}

// Retail (mfc140.dll RVA 0x156620), transcribed instruction for instruction:
//     int iHighlighted = m_iHighlighted;                 // 0x1138
//     if (iHighlighted >= 0 && iHighlighted < m_Buttons.m_nCount) {  // 0x11a0
//         m_iHot         = -1;                           // 0x1140
//         m_iHighlighted = -1;
//         InvalidateButton(iHighlighted);                // 0x14e4c0
//         ::UpdateWindow(m_hWnd);                        // USER32 import
//         HWND h = m_hWndOwner;                          // CWnd + 0xa0
//         if (h == NULL) h = ::GetParent(m_hWnd);
//         CWnd* p = CWnd::FromHandle(h);                 // 0x289180
//         ::SendMessage(p->m_hWnd, 0x362 /*WM_SETMESSAGESTRING*/,
//                       0xE001 /*AFX_IDS_IDLEMESSAGE*/, 0);
//     }
//     RestoreFocus();          // tail jump through vtable +0x6d8
// Notes on the evidence: the old index is read into %rdx BEFORE m_iHighlighted
// is overwritten, and %rdx is still what InvalidateButton receives.  When the
// index is out of range the entire body is skipped, but RestoreFocus still
// runs, because the vtable load and tail jump sit after the join at 0x156691.
// CWnd::m_hWndOwner is at +0xa0: ?SetOwner@CToolBar@@QEAAXPEAVCWnd@@@Z
// (RVA 0x1d93a0) ends with `mov %rax,0xa0(%rdi)` where rax is the new owner's
// m_hWnd.  Slot 0x6d8 of the CMFCToolBar vtable (0x180313618) is
// ?RestoreFocus@CMFCToolBar@@UEAAXXZ.
//
// Deviation: OpenMFC's CWnd does not name m_hWndOwner (that byte range is
// anonymous padding, so reading it would read uninitialised memory), so the
// ::GetParent(m_hWnd) branch is taken unconditionally.  For a toolbar whose
// owner was never overridden with SetOwner that is the same window retail
// picks.  The CWnd::FromHandle round trip is skipped as well: retail sends to
// the returned wrapper's m_hWnd, which is by construction the handle it was
// given.  A null-handle guard is added around the window calls (retail has
// none), and the range test uses GetCount(), which reads the mfccore.cpp side
// table where OpenMFC keeps the live buttons, rather than m_Buttons.m_nCount.
// RestoreFocus is still a stub here, so the focus half is a no-op.
// Symbol: ?Deactivate@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__Deactivate_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    const int iHighlighted = pThis->m_iHighlighted;
    if (iHighlighted >= 0 && iHighlighted < pThis->GetCount()) {
        pThis->m_iHot         = -1;
        pThis->m_iHighlighted = -1;

        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iHighlighted);

        const HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd != nullptr) {
            ::UpdateWindow(hWnd);
            // TODO(clean-room): retail prefers CWnd::m_hWndOwner (CWnd + 0xa0)
            // and only falls back to ::GetParent; OpenMFC does not model it.
            const HWND hOwner = ::GetParent(hWnd);
            if (hOwner != nullptr) {
                // 0x362 == WM_SETMESSAGESTRING, 0xE001 == AFX_IDS_IDLEMESSAGE;
                // neither is spelled in the OpenMFC public headers.
                ::SendMessage(hOwner, 0x362, 0xE001, 0);
            }
        }
    }

    impl__RestoreFocus_CMFCToolBar__UEAAXXZ(pThis);
}

// Retail (mfc140.dll RVA 0x14d900) is the whole toolbar paint: it takes the
// DC clip box (CDC vtable +0xb0), reads GetCurrentAlignment() (CMFCToolBar
// vtable 0x180313618 +0x338) to decide horizontal vs vertical, then walks
// m_Buttons drawing separators and calling DrawButton for every visible
// button, with the image collection picked by GetImageList.
// Not implemented: the CDC clip/box virtuals and the per-button draw virtuals
// are not modelled -- see the note on DrawButton below, which is the same
// blocker one level down.
// Symbol: ?DoPaint@CMFCToolBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCToolBar__MEAAXPEAVCDC___Z(CMFCToolBar* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// Retail (mfc140.dll RVA 0x14e370), fully decoded but NOT implemented:
//     if (!pButton->m_bVisible)  return TRUE;                  // +0x64
//     if ( pButton->m_bIsHidden) return TRUE;                  // +0x54
//     if (!pDC->RectVisible(pButton->m_rect)) return TRUE;     // CDC vtable +0xc0
//     BOOL bHorz = (GetCurrentAlignment() & 0xA000) != 0;      // vtable +0x338
//     BOOL bGray = m_bGrayDisabledButtons && !bDrawDisabledImages;   // +0x1100
//     BOOL bCust = CMFCToolBar::m_bCustomizeMode &&            // 0x3b70bc
//                  !CMFCToolBar::m_bAltCustomizeMode &&        // 0x3b70cc
//                  !m_bLocked;                                 // +0x10b8
//     pButton->OnDraw(pDC, pButton->m_rect, pImages, bHorz, bCust,
//                     bHighlighted, m_bShowHotBorder /*+0x10fc*/, bGray);
//     return TRUE;
// (OnDraw is slot 8 of the button vtable -- +0x40 of both 0x180313e98 and
// 0x180316338 resolves to ?OnDraw@CMFCToolBarButton@@ / @CMFCToolBarMenuButton@@.
// The bGray expression is the `neg/sbb/and` idiom at 0x14e3e2; note it is the
// NEGATION of the bDrawDisabledImages argument, not a copy of it.)
// Left a stub deliberately: nothing in phase4/src defines a CDC::RectVisible
// entry point at all (neither the C++ method nor an impl__ thunk),
// and CMFCToolBarButton::OnDraw exists only as an empty exported stub with no
// virtual dispatch behind it, so nothing would be drawn -- the flags above
// would be computed and thrown away.
// Symbol: ?DrawButton@CMFCToolBar@@MEAAHPEAVCDC@@PEAVCMFCToolBarButton@@PEAVCMFCToolBarImages@@HH@Z
extern "C" int MS_ABI impl__DrawButton_CMFCToolBar__MEAAHPEAVCDC__PEAVCMFCToolBarButton__PEAVCMFCToolBarImages__HH_Z(
    CMFCToolBar* pThis, CDC* pDC, CMFCToolBarButton* pButton, CMFCToolBarImages* pImages,
    int bHighlighted, int bDrawDisabledImages) {
    (void)pThis; (void)pDC; (void)pButton; (void)pImages; (void)bHighlighted; (void)bDrawDisabledImages;
    return 0;
}

// Retail (mfc140.dll RVA 0x151410) draws the customize-drag insertion marker:
// it reads GetCurrentAlignment() (vtable +0x338) to pick the horizontal or
// vertical form, selects m_penDrag (+0x12c0) into the DC
// (?SelectObject@CDC@@UEAAPEAVCFont@@PEAV2@@Z, 0x2a0670) and strokes a caret
// around m_rectDrag (+0x12a0).
// Not implemented: OpenMFC models neither CDC pen selection nor the drawing
// primitives this needs.
// Symbol: ?DrawDragCursor@CMFCToolBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawDragCursor_CMFCToolBar__MEAAXPEAVCDC___Z(CMFCToolBar* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// Retail (RVA 0x1561a0, mfc140u) is a single forward, fully transcribed:
//     CMFCVisualManager* pVM = CMFCVisualManager::GetInstance();   // 0x9774, see below
//     pVM->OnDrawSeparator(pDC, this, rect, bHorz);                // vftable +0xa0
// The CRect argument is copied into the outgoing frame ([rsp+0x30]) and passed
// by address in r9, bHorz goes in the [rsp+0x20] stack slot.  Slot 0xa0 (20) of
// the mfc140u CMFCVisualManager vftable at 0x18031c128 is
// ?OnDrawSeparator@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
// (RVA 0x185140).  0x9774 is not an export: it is the inlined
// CMFCVisualManager::GetInstance() --
//     if (m_pVisManager == NULL) {                              // 0x3be3c0
//         m_pVisManager = m_pRTIDefault ? m_pRTIDefault->CreateObject()   // 0x3be3b8
//                                       : new CMFCVisualManager();        // 0x108 bytes
//         m_pVisManager->m_bAutoDestroy(+0x104) = TRUE;
//         m_pVisManager->OnUpdateSystemColors();                // vslot 0x70
//     }
//     return m_pVisManager;
//
// OpenMFC: the current manager is read from the exported static
// ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA, which
// phase4/src/detail/MfccoreSupport.cpp keeps in step with its C++ singleton, and
// the draw goes through the exported OnDrawSeparator thunk
// (featurepack/visualmanager/Thunks.cpp), which dispatches virtually on the C++
// object -- so a derived visual manager's override IS reached.
// Deviation: the lazy construction of a default manager is not reproduced --
// there is no impl__ thunk for GetInstance (it is inline in afxvisualmanager.h,
// hence 0x9774 is a private helper) and the C++ CMFCVisualManager::GetInstance()
// cannot be called from this translation unit (AGENTS.md link rule) -- so with
// no manager created yet nothing is drawn, where retail would have created the
// default one first.
// Symbol: ?DrawSeparator@CMFCToolBar@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCToolBar__MEAAXPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBar* pThis, CDC* pDC, const CRect* pRect, int bHorz)
{
    if (!pThis || !pRect) return;
    CMFCVisualManager* pVM =
        static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (!pVM) {
        // TODO(clean-room): retail's GetInstance() would create the default
        // manager here; see the deviation note above.
        return;
    }
    CRect rect(*pRect);
    impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
        pVM, pDC, pThis, rect, bHorz);
}

// Retail (mfc140.dll RVA 0x156470), fully decoded but NOT implemented:
//     if (pButton == NULL) return FALSE;
//     if (!pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return FALSE;
//     if (!pButton->OnClick(this, TRUE)) return FALSE;      // button vtable +0x50
//     if (pButton->IsDroppedDown())                         // button vtable +0xf0
//         ::SendMessage(pButton->m_pPopupMenu->m_hWnd, WM_KEYDOWN, VK_HOME, 0);
//     SetHot(pButton);                                      // 0x1563e0
//     return TRUE;
// (The runtime-class descriptor the IsKindOf test uses is at 0x3aa748; its
// name string reads back as "CMFCToolBarMenuButton" and its object size as
// 296.  Slot +0xf0 of that class' vtable, 0x180316338, is the two-instruction
// body at 0x2a580 -- `return this->[0xc0] != NULL`, i.e. m_pPopupMenu.  The
// SendMessage import slot 0x2c5378 resolves to USER32!SendMessageA in this
// ANSI twin, i.e. SendMessageW in mfc140u.)
// Left a stub: CMFCToolBarMenuButton::m_pPopupMenu (+0xC0) is inside the
// anonymous padding of the OpenMFC class, so the dropped-down test cannot be
// evaluated, and CMFCToolBarButton::OnClick has no dispatchable virtual here.
// Symbol: ?DropDownMenu@CMFCToolBar@@IEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__DropDownMenu_CMFCToolBar__IEAAHPEAVCMFCToolBarButton___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton) {
    (void)pThis; (void)pButton;
    return 0;
}

// Retail (mfc140.dll RVA 0x155bb0) greys the entries of the customize context
// menu according to the button under the cursor: it asks the button a virtual
// question (button vtable +0xe0) and then drives ::EnableMenuItem /
// ::CheckMenuItem over the command ids 0x420E..0x4214, using
// m_dwStyle & 0xA000 (+0x104) to tell a horizontal bar from a vertical one and
// the button's image index (m_bUserButton ? m_iUserImage : m_iImage) to decide
// whether the image commands apply.
//
// Correcting an earlier revision of this comment, which said "neither the
// button virtual nor CMenu::EnableMenuItem is reachable from here": both are.
// The calls at 0x155c19 / 0x155c76 go through the import slots 0x1802c4d58 and
// 0x1802c4e30, which iat.py resolves to USER32!EnableMenuItem and
// USER32!CheckMenuItem -- plain Win32, not CMenu methods -- and they are passed
// pMenu->m_hMenu read straight from CMenu + 8.  (OpenMFC does also export
// ?EnableMenuItem@CMenu@@QEAAIIH@Z, from phase4/src/detail/CMenuSupport.cpp.)
// The button virtual at +0xe0 folds to RVA 0x7260, `xor eax,eax ; ret`, in both
// the CMFCToolBarButton (0x180313e98) and CMFCToolBarMenuButton (0x180316338)
// vftables, and the CMFCToolBar virtual at +0x828 is RVA 0x23b30,
// `return m_bMenuMode` (+0x10ec) -- so neither dispatch is a real obstacle
// either.
// Still left a stub, deliberately and for a different reason: the body is a
// ~100-instruction chain of greying decisions over the AFX_ID_* customize-menu
// ids and OpenMFC builds no such menu (CMFCToolBarsCustomizeDialog is a stub).
// Transcribing it half-checked would be a confidently wrong body in an entry
// point real apps can call.  (The parameter list below is now the real one; the
// generated one was missing `this`.)
// Symbol: ?EnableContextMenuItems@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@PEAVCMenu@@@Z
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, CMenu* pMenu) {
    (void)pThis; (void)pButton; (void)pMenu;
    return 0;
}

// Retail (mfc140.dll RVA 0x157490 -- the ANSI twin of this Unicode export):
//     if (bEnable) {
//         if (m_pCustomizeBtn != NULL) {          // +0x1300
//             m_pCustomizeBtn->[+0x128] = iCustomizeCmd;
//             m_pCustomizeBtn->m_strText = strCustomizeText;   // +0x38
//         } else {
//             CMFCCustomizeButton btn(iCustomizeCmd, strCustomizeText); // 0x35000
//             if (InsertButton(btn, -1) < 0) return;   // vtable +0x698
//             CObject* p = m_Buttons.m_pNodeTail->data;         // +0x1198
//             m_pCustomizeBtn = (p && p->IsKindOf(RUNTIME_CLASS(CMFCCustomizeButton)))
//                             ? p : NULL;                       // RTC 0x3aa148
//         }
//         m_bQuickCustomize = bQuickCustomize;                  // +0x1108
//     } else if (m_pCustomizeBtn != NULL) {
//         m_Buttons.RemoveTail();                               // 0x2305d0
//         delete m_pCustomizeBtn;
//         m_pCustomizeBtn = NULL;
//     }
//     UpdateVirtualRect();                                      // 0xa1ae0
// Not implemented.  CMFCToolBar::InsertButton (the CMFCToolBarButton& overload,
// vtable slot 211) is real further down in this file now, but
// CMFCCustomizeButton is not declared in any OpenMFC header and its
// (int, const CString&) constructor is an empty stub with a placeholder
// parameter list in phase4/src/featurepack/customize/CMFCCustomizeButton.cpp,
// so there is no object to insert; the `+0x128` command-id member written on
// the "already have one" path is not modelled either.
// Symbol: ?EnableCustomizeButton@CMFCToolBar@@QEAAXHHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__EnableCustomizeButton_CMFCToolBar__QEAAXHHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CMFCToolBar* pThis, int bEnable, int iCustomizeCmd, const CString* pStrCustomizeText, int bQuickCustomize) {
    (void)pThis; (void)bEnable; (void)iCustomizeCmd; (void)pStrCustomizeText; (void)bQuickCustomize;
    // TODO(clean-room): not transcribed -- needs CMFCCustomizeButton.
}

// Retail (mfc140.dll RVA 0x1575d0), fully decoded: it is only a string-resource
// wrapper around the overload above.
//     CString str;
//     HINSTANCE h = AfxFindStringResourceHandle(uiTextResID);   // 0x2accf0
//     if (h == NULL) AfxThrowInvalidArgException();
//     if (!str.LoadString(h, uiTextResID)) AfxThrowInvalidArgException();
//     EnableCustomizeButton(bEnable, iCustomizeCmd, str, bQuickCustomize); // 0x157490
// Not implemented: the target overload is itself a stub (see just above), so
// loading the string would accomplish nothing.
// Symbol: ?EnableCustomizeButton@CMFCToolBar@@QEAAXHHIH@Z
extern "C" void MS_ABI impl__EnableCustomizeButton_CMFCToolBar__QEAAXHHIH_Z(
    CMFCToolBar* pThis, int bEnable, int iCustomizeCmd, unsigned int uiCustomizeTextResId, int bQuickCustomize) {
    (void)pThis; (void)bEnable; (void)iCustomizeCmd; (void)uiCustomizeTextResId; (void)bQuickCustomize;
    // TODO(clean-room): not transcribed -- the target overload is a stub.
}

// Retail (mfc140.dll RVA 0x151070) maps a screen point onto an insertion index
// during a customize drag: it converts the incoming rect with a USER32 call
// (import slot 0x2c5368), reads GetCurrentAlignment() (vtable +0x338) for the
// horizontal/vertical split, then walks m_Buttons comparing the point against
// each button rect and fills the reference rect with the drop marker.
// Not implemented: the walk depends on laid-out button rectangles, which
// OpenMFC does not produce (AdjustLocations and CalcLayout are stubs), so any
// index returned here would be meaningless.
// Symbol: ?FindDropIndex@CMFCToolBar@@MEBAHVCPoint@@AEAVCRect@@@Z
// NB: this stub returns 0 and leaves the caller's rect untouched; retail
// returns -1 for "no drop position" and always rewrites the rect.  OnDragOver
// below calls it, so once FindDropIndex is transcribed its -1 must be kept.
extern "C" int MS_ABI impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(
    const CMFCToolBar* pThis, CPoint point, CRect* pRect) {
    (void)pThis; (void)point; (void)pRect;
    // TODO(clean-room): not transcribed.  Returns -1 ("no drop index"), which
    // is retail's not-found value, rather than the generated 0 -- 0 would tell
    // OnDragOver that a drop before button 0 is possible.
    return -1;
}

// Retail (mfc140.dll RVA 0x153270), fully transcribed: a linear scan of the
// private "all toolbars" CObList (object at 0x3ab090, its m_pNodeHead read at
// 0x3ab098) for the bar whose window handle matches.
//     for (node = lstAllToolbars.m_pNodeHead; node; node = node->pNext) {
//         CMFCToolBar* pBar = node->data;          // node + 0x10
//         HWND h = (pBar == NULL) ? NULL : pBar->m_hWnd;   // CWnd + 0x40
//         if (h == hWnd) return pBar;
//     }
//     return NULL;
// (0x3ab090 is not exported; it is bracketed by
// ?m_lstUnpermittedCommands@CMFCToolBar@@1V?$CList@II@@A at 0x3ab058 and
// ?m_lstProtectedCommands@CMFCToolBarButton@@1V?$CList@II@@A at 0x3ab0c8, i.e.
// exactly one 56-byte CObList wide, and it is the same object
// ?GetAllToolbars@CMFCToolBar@@SAAEBVCObList@@XZ returns.)
//
// Deviation: OpenMFC has no equivalent of that list, so the scan runs over the
// mfccore.cpp toolbar side table -- every live CMFCToolBar on this thread --
// which is the same substitution AutoGrayInactiveImages and ResetAll make.  A
// NULL hWnd never matches, because a detached bar has GetSafeHwnd() == NULL and
// retail would compare NULL == NULL; the explicit guard keeps that from
// returning an arbitrary detached toolbar.
// Symbol: ?FromHandlePermanent@CMFCToolBar@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CMFCToolBar* MS_ABI impl__FromHandlePermanent_CMFCToolBar__SAPEAV1_PEAUHWND_____Z(
    HWND hWnd)
{
    if (hWnd == nullptr) return nullptr;

    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (pBar && pBar->GetSafeHwnd() == hWnd) return pBar;
    }
    return nullptr;
}

// Retail (mfc140.dll RVA 0x157c70) is two instructions:
//     lea rax, [rip + <0x3ab090>] ; ret
// i.e. it hands back a reference to the private static CObList of every live
// CMFCToolBar (see the note on FromHandlePermanent above for how that address
// is identified).  Retail maintains that list from the window handlers, not
// from the constructor: ?OnCreate@CMFCToolBar@@ (RVA 0x150650) takes its
// address at 0x150890 to append, and ?OnDestroy@CMFCToolBar@@ (0x151630) at
// 0x1516af to remove -- so a bar only appears once its window exists.
//
// Deviation: OpenMFC has no such list.  A file-scope CObList stands in and is
// rebuilt from the mfccore.cpp toolbar side table on every call, so callers see
// the live toolbars of the calling thread.  Two consequences worth knowing:
// the returned list is rebuilt (so a POSITION held across two calls is stale),
// and the order is the side table's, not creation order.  The important part
// versus the previous stub is that the reference is now valid at all -- the
// generated body returned a null reference, which faults in the caller.
namespace {
CObList& OpenMFC_AllToolbars()
{
    static CObList s_lstAllToolbars;
    impl__RemoveAll_CObList__QEAAXXZ(&s_lstAllToolbars);
    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (pBar) {
            impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
                &s_lstAllToolbars, static_cast<CObject*>(pBar));
        }
    }
    return s_lstAllToolbars;
}
}  // namespace
// Symbol: ?GetAllToolbars@CMFCToolBar@@SAAEBVCObList@@XZ
extern "C" const CObList* MS_ABI impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ() {
    return &OpenMFC_AllToolbars();
}

// Retail (mfc140.dll RVA 0x14d760), fully transcribed:
//     CMFCToolBarButton* p = GetButton(nIndex);        // 0x14e470
//     if (p == NULL) { nID = 0; nStyle = 0; iImage = -1; return; }
//     nID    = p->m_nID;                               // +0x24
//     nStyle = p->m_nStyle;                            // +0x28
//     iImage = p->m_bUserButton ? p->m_iUserImage : p->m_iImage;  // +0x08, +0x4c/+0x48
// The image selection is the `neg/sbb/and $4` idiom at 0x14d790: it turns
// m_bUserButton into a 0 or 4 byte displacement added to +0x48, i.e. exactly
// CMFCToolBarButton::GetImage().  All four offsets match the layout pinned in
// include/openmfc/afxmfc.h (m_nID @36, m_nStyle @40, m_bUserButton @8,
// m_iImage @72, m_iUserImage @76).  Note that iImage is written on BOTH paths,
// including the not-found one -- the -1 is set up before the join.
// Symbol: ?GetButtonInfo@CMFCToolBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetButtonInfo_CMFCToolBar__QEBAXHAEAI0AEAH_Z(
    const CMFCToolBar* pThis, int nIndex, unsigned int* pnID,
    unsigned int* pnStyle, int* piImage)
{
    if (!pThis) return;

    const CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    if (!pButton) {
        if (pnID)    *pnID = 0;
        if (pnStyle) *pnStyle = 0;
        if (piImage) *piImage = -1;
        return;
    }

    if (pnID)    *pnID    = pButton->m_nID;
    if (pnStyle) *pnStyle = pButton->m_nStyle;
    if (piImage) *piImage = pButton->m_bUserButton ? pButton->m_iUserImage
                                                   : pButton->m_iImage;
}

// Retail (mfc140.dll RVA 0x14d650), fully transcribed:
//     CMFCToolBarButton* p = GetButton(nIndex);   // 0x14e470
//     return p ? p->m_nStyle : 0;                 // +0x28
// (On the NULL path retail simply leaves eax as GetButton returned it, i.e. 0.)
// Symbol: ?GetButtonStyle@CMFCToolBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetButtonStyle_CMFCToolBar__QEBAIH_Z(
    const CMFCToolBar* pThis, int nIndex)
{
    if (!pThis) return 0;
    const CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    return pButton ? pButton->m_nStyle : 0u;
}

// Retail (mfc140.dll RVA 0x150ff0), fully transcribed:
//     lstButtons.RemoveAll();                              // 0x83d0
//     if (uiCmd == 0) return 0;                            // before the walk
//     for (node = <all-toolbars CObList>.m_pNodeHead; node; node = node->pNext) {
//         CMFCToolBar* pBar = node->data;
//         if (pBar == NULL) AfxThrowInvalidArgException(); // 0x225b80
//         if (CWnd::FromHandlePermanent(pBar->m_hWnd) == NULL) continue;  // 0x2891d0
//         for (b = pBar->m_Buttons.m_pNodeHead; b; b = b->pNext) {
//             CMFCToolBarButton* pButton = b->data;
//             if (pButton == NULL) break;                  // moves to the next bar
//             if (pButton->m_nID == uiCmd)                 // +0x24
//                 lstButtons.AddTail(pButton);             // 0x230490
//         }
//     }
//     return lstButtons.m_nCount;                          // CObList + 0x18
// (0x83d0/0x230490 disassemble as CPtrList::RemoveAll/AddTail -- the identical
// CObList bodies are folded onto them by ICF; the parameter is AEAVCObList@@.
// The zero-command early return happens AFTER the list has already been
// emptied, which is why the RemoveAll is unconditional here too.)
//
// Deviations, the same two the other whole-toolbar walks in this file make:
// the scan runs over the mfccore.cpp side table instead of retail's private
// all-toolbars CObList, and the "this pane object owns a live window" test
// becomes a GetSafeHwnd() test because CMFCToolBar::Create does not put its
// HWND into CWnd's permanent map.  The count is accumulated while adding
// rather than read back from the list.
// Symbol: ?GetCommandButtons@CMFCToolBar@@SAHIAEAVCObList@@@Z
extern "C" int MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(
    unsigned int uiCmd, CObList* pLstButtons)
{
    if (!pLstButtons) return 0;
    impl__RemoveAll_CObList__QEAAXXZ(pLstButtons);
    if (uiCmd == 0) return 0;

    int nCount = 0;
    for (auto& entry : g_toolBarStates) {
        CMFCToolBar* pBar = const_cast<CMFCToolBar*>(entry.first);
        if (!pBar || pBar->GetSafeHwnd() == nullptr) continue;

        const int nButtons = pBar->GetCount();
        for (int i = 0; i < nButtons; ++i) {
            CMFCToolBarButton* pButton = pBar->GetButton(i);
            if (!pButton) break;
            if (pButton->m_nID == uiCmd) {
                impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
                    pLstButtons, static_cast<CObject*>(pButton));
                ++nCount;
            }
        }
    }
    return nCount;
}

// Retail (mfc140.dll RVA 0x156590), fully decoded but NOT implemented:
//     if (m_Buttons.m_nCount == 0) return NULL;   // note: *pIndex NOT written
//     int i = 0;
//     for (node = m_Buttons.m_pNodeHead; node; node = node->pNext, ++i) {
//         CMFCToolBarButton* p = node->data;
//         if (p != NULL &&
//             p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&  // RTC 0x3aa748
//             p->IsDroppedDown()) {                                 // vtable +0xf0
//             if (pIndex) *pIndex = i;
//             return p;
//         }
//     }
//     if (pIndex) *pIndex = -1;
//     return NULL;
// Left a stub for the same reason as DropDownMenu above: the dropped-down test
// is `m_pPopupMenu != NULL` at button offset 0xC0, and OpenMFC keeps no popup
// pointer there -- reading it would read uninitialised padding and could hand
// the caller an arbitrary button.
// Symbol: ?GetDroppedDownMenu@CMFCToolBar@@QEBAPEAVCMFCToolBarMenuButton@@PEAH@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(
    const CMFCToolBar* pThis, int* pIndex) {
    (void)pThis; (void)pIndex;
    // TODO(clean-room): not transcribed -- see above.  *pIndex is deliberately
    // left alone, as on retail's empty-toolbar path.
    return nullptr;
}

// Retail (mfc140.dll RVA 0x1550d0), fully transcribed:
//     if (CMFCToolBar::m_clrTextHot != (COLORREF)-1)   // 0x3aab54
//         return CMFCToolBar::m_clrTextHot;
//     if (afxGlobalData.<first DWORD> == 0) {          // 0x3ba380
//         afxGlobalData.Initialize();                  // 0x6a5c0
//         afxGlobalData.<first DWORD> = 1;
//     }
//     return *(COLORREF*)(&afxGlobalData + 0x34);
// afxGlobalData + 0x34 is the cached button-text colour:
// ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ (RVA 0x6afd0) fills it at 0x6b0bb
// with ::GetSysColor(0x12), and 0x12 is COLOR_BTNTEXT.  (Its neighbours check
// out the same way: +0x28 <- COLOR_BTNFACE(15), +0x2c <- COLOR_BTNSHADOW(16),
// +0x30 <- COLOR_BTNHIGHLIGHT(20).)
//
// Deviations: OpenMFC models neither afxGlobalData nor its lazy Initialize, so
// the fallback reads the colour live from the system instead of from the
// cache.  Also note that retail's m_clrTextHot static is initialised to
// 0xFFFFFFFF (verified in the retail image at 0x3aab54) whereas OpenMFC's
// storage in featurepack/toolbar/StaticData.cpp starts at 0, so today the
// sentinel branch is never taken and this returns black; see the header
// request filed alongside this change.
// Symbol: ?GetHotTextColor@CMFCToolBar@@SAKXZ
extern "C" unsigned long MS_ABI impl__GetHotTextColor_CMFCToolBar__SAKXZ() {
    if (impl__m_clrTextHot_CMFCToolBar__1KA != 0xFFFFFFFFu) {
        return impl__m_clrTextHot_CMFCToolBar__1KA;
    }
    return static_cast<unsigned long>(::GetSysColor(COLOR_BTNTEXT));
}

// Retail (mfc140.dll RVA 0x158df0), fully transcribed.  It is a four-way pick
// between the caller's two "regular" collections and its two "large" ones.
// With the parameters numbered by register (rdx, r8, r9, [rsp+0x28]):
//     BOOL bMenuMode = m_bMenuMode;                       // 0x10ec
//     if (m_bLocked) {                                    // 0x10b8
//         if (bMenuMode)                       return p2;
//         if (CMFCToolBar::m_bLargeIcons == 0) return p2; // 0x3b70c8
//         if (p4->m_iCount > 0)                return p4; // CMFCToolBarImages +8
//         return p2;
//     }
//     if (bMenuMode)                       return p1;
//     if (CMFCToolBar::m_bLargeIcons == 0) return p1;
//     if (p3->m_iCount > 0)                return p3;
//     return p1;
// So the odd parameters serve the unlocked bar and the even ones the locked
// bar, and the large pair is only taken when large icons are on AND that
// collection actually holds images.  m_iCount at CMFCToolBarImages + 8 is the
// offset pinned in include/openmfc/afxmfc.h; retail's GetCount() is inline
// (mfc140 exports no ?GetCount@CMFCToolBarImages@@ at all), which is why the
// count appears here as a bare member load.
//
// Deviation, and it matters: the count is read straight out of that member,
// exactly as retail does, using the same raw-offset idiom the sibling
// featurepack/toolbar/CMFCToolBarImages.cpp uses on its own members.  OpenMFC
// keeps the live image count in a side table and writes the member only in
// CopyTemp, so until m_iCount is maintained the large-image branches will not
// fire and this returns the regular collection -- which is also what retail
// returns whenever large icons are off.  The control flow is otherwise exact.
// Symbol: ?GetImageList@CMFCToolBar@@IEBAPEAVCMFCToolBarImages@@AEAV2@000@Z
extern "C" CMFCToolBarImages* MS_ABI
impl__GetImageList_CMFCToolBar__IEBAPEAVCMFCToolBarImages__AEAV2_000_Z(
    const CMFCToolBar* pThis,
    CMFCToolBarImages* pRegularUnlocked,   // p1: chosen when !m_bLocked
    CMFCToolBarImages* pRegularLocked,     // p2: chosen when  m_bLocked
    CMFCToolBarImages* pLargeUnlocked,     // p3
    CMFCToolBarImages* pLargeLocked)       // p4
// (The parameters are named for the roles the disassembly proves, not for the
// names the shipping afxtoolbar.h gives them, which are not evidenced here.)
{
    if (!pThis) return pRegularUnlocked;

    // CMFCToolBarImages::m_iCount lives at +8 (see the layout comment in
    // include/openmfc/afxmfc.h); it is protected, so it is read by offset the
    // way featurepack/toolbar/CMFCToolBarImages.cpp reads its own members.
    const auto imageCount = [](const CMFCToolBarImages* p) -> int {
        return p ? *reinterpret_cast<const int*>(
                        reinterpret_cast<const char*>(p) + 8) : 0;
    };

    const bool bLargeAvailable =
        pThis->m_bMenuMode == 0 && impl__m_bLargeIcons_CMFCToolBar__1HA != 0;

    if (pThis->m_bLocked) {
        if (bLargeAvailable && imageCount(pLargeLocked) > 0) {
            return pLargeLocked;
        }
        return pRegularLocked;
    }

    if (bLargeAvailable && imageCount(pLargeUnlocked) > 0) {
        return pLargeUnlocked;
    }
    return pRegularUnlocked;
}

// Retail (mfc140.dll RVA 0x14d5e0), fully transcribed:
//     if (lpRect == NULL) AfxThrowInvalidArgException();   // 0x225b80
//     CMFCToolBarButton* p = GetButton(nIndex);            // 0x14e470
//     *lpRect = (p == NULL) ? CRect(0,0,0,0)
//                           : p->GetInvalidateRect();      // button vtable +0x120
//
// Deviation: OpenMFC has no DLL-internal vtable for CMFCToolBarButton, so the
// virtual cannot be dispatched.  Both the base CMFCToolBarButton vtable
// (0x180313e98) and the CMFCToolBarMenuButton one (0x180316338) put the same
// body at +0x120 (RVA 0x23890), whose whole text is
// `movups 0x68(%rcx),%xmm0 ; mov %rdx,%rax ; movdqu %xmm0,(%rdx) ; ret`
// -- i.e. `return m_rect` into the hidden CRect slot.
// That body is inlined here, which is exact for those two classes; a derived
// button that overrides GetInvalidateRect to report a larger repaint area (the
// combo/edit box buttons do) will not be re-entered.
// The null-lpRect throw becomes a plain early return: OpenMFC does not raise
// MFC exceptions from these thunks.
// Symbol: ?GetInvalidateItemRect@CMFCToolBar@@UEBAXHPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetInvalidateItemRect_CMFCToolBar__UEBAXHPEAUtagRECT___Z(
    const CMFCToolBar* pThis, int nIndex, RECT* lpRect)
{
    if (!lpRect) return;
    ::SetRectEmpty(lpRect);
    if (!pThis) return;

    if (const CMFCToolBarButton* pButton = pThis->GetButton(nIndex)) {
        const CRect& rc = pButton->m_rect;
        lpRect->left   = rc.left;
        lpRect->top    = rc.top;
        lpRect->right  = rc.right;
        lpRect->bottom = rc.bottom;
    }
}

// Retail (mfc140.dll RVA 0x14d580), fully transcribed:
//     CMFCToolBarButton* p = GetButton(nIndex);   // 0x14e470
//     return p ? p->m_nID : 0;                    // +0x24
// (As in GetButtonStyle, the NULL path just leaves GetButton's zero in eax.)
// Symbol: ?GetItemID@CMFCToolBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetItemID_CMFCToolBar__QEBAIH_Z(
    const CMFCToolBar* pThis, int nIndex)
{
    if (!pThis) return 0;
    const CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    return pButton ? pButton->m_nID : 0u;
}

// Retail (mfc140.dll RVA 0x14d5a0), fully transcribed:
//     if (lpRect == NULL) AfxThrowInvalidArgException();   // 0x225b80
//     CMFCToolBarButton* p = GetButton(nIndex);            // 0x14e470
//     *lpRect = (p == NULL) ? CRect(0,0,0,0) : p->m_rect;  // +0x68
// (the copy is a single 16-byte movups/movdqu pair, so all four LONGs move).
// m_rect @104 in include/openmfc/afxmfc.h is the same 0x68.  The only
// deviation is the null-lpRect throw, which becomes an early return.
// Symbol: ?GetItemRect@CMFCToolBar@@UEBAXHPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemRect_CMFCToolBar__UEBAXHPEAUtagRECT___Z(
    const CMFCToolBar* pThis, int nIndex, RECT* lpRect)
{
    if (!lpRect) return;
    ::SetRectEmpty(lpRect);
    if (!pThis) return;

    if (const CMFCToolBarButton* pButton = pThis->GetButton(nIndex)) {
        const CRect& rc = pButton->m_rect;
        lpRect->left   = rc.left;
        lpRect->top    = rc.top;
        lpRect->right  = rc.right;
        lpRect->bottom = rc.bottom;
    }
}

// Retail (mfc140.dll RVA 0x155b90), fully transcribed:
//     CSize sz = CMFCToolBar::m_sizeMenuButton;       // 0x3aaee8, loaded as one qword
//     if (sz.cx == -1) sz = CMFCToolBar::m_sizeButton; // 0x3aaf88 (cmove on the qword)
//     *pRet = sz; return pRet;
// Only cx is tested; the whole 8-byte CSize is then swapped.  Both statics are
// defined in this file and already carry retail's initial values, verified
// against the retail image: m_sizeMenuButton = (-1,-1), m_sizeButton = (23,22).
//
// The generated stub this replaces also had the struct return wrong -- the
// hidden CSize slot arrives in rcx for a static (no `this`) and must come back
// in rax; it returned nullptr.  Both are fixed here.
// Symbol: ?GetMenuButtonSize@CMFCToolBar@@SA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetMenuButtonSize_CMFCToolBar__SA_AVCSize__XZ(CSize* pRet) {
    if (!pRet) return pRet;
    if (impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cx == -1) {
        pRet->cx = impl__m_sizeButton_CMFCToolBar__1VCSize__A.cx;
        pRet->cy = impl__m_sizeButton_CMFCToolBar__1VCSize__A.cy;
    } else {
        pRet->cx = impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cx;
        pRet->cy = impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cy;
    }
    return pRet;
}

// Retail (RVA 0x157400, mfc140u), fully transcribed:
//     CSize size = (m_sizeMenuImage.cx == -1) ? m_sizeImage : m_sizeMenuImage;
//                                   // 0x3b1c38 cx test, cmove of 0x3b1cb8
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }      // 0x3c1620
//     double scale = afxGlobalData.<+0x2c0> ? afxGlobalData.<+0x2b8> : 1.0;
//     if (scale != 1.0) {              // ucomisd against the 1.0 at 0x34ff68
//         size.cy = (int)(size.cy * scale + 0.5);   // 0.5 is 0x34fef0
//         size.cx = (int)(size.cx * scale + 0.5);   // the gate + scale are
//     }                                             // re-read before each use
//     return size;
// The hidden CSize slot arrives in rcx (static, no `this`) and is returned in
// rax.  The generated stub took a `void**` and returned nullptr, which is
// wrong on both counts.
// Symbol: ?GetMenuImageSize@CMFCToolBar@@SA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(CSize* pRet) {
    if (!pRet) return pRet;
    if (impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx == -1) {
        pRet->cx = impl__m_sizeImage_CMFCToolBar__1VCSize__A.cx;
        pRet->cy = impl__m_sizeImage_CMFCToolBar__1VCSize__A.cy;
    } else {
        pRet->cx = impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx;
        pRet->cy = impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cy;
    }
    EnsureGlobalDataInitialized();
    const double scale = GlobalDataInt(kGlobalDataImageScaleOn) != 0
                             ? GlobalDataDouble(kGlobalDataImageScale) : 1.0;
    if (scale != 1.0) {
        pRet->cy = static_cast<int>(pRet->cy * scale + 0.5);
        pRet->cx = static_cast<int>(pRet->cx * scale + 0.5);
    }
    return pRet;
}

// Retail (RVA 0x158d10, mfc140u), fully transcribed:
//     if (m_bDrawTextLabels) return m_nMaxBtnHeight;              // 0x10cc, 0x1130
//     int cy = m_bMenuMode                                         // 0x10ec
//         ? (m_sizeMenuButton.cy > 0 ? m_sizeMenuButton.cy : m_sizeButton.cy)
//                                                // 0x3b1d0c / 0x3b1e3c
//         : GetButtonSize().cy;                  // 0x239b4, the inlined
//                                                // afxtoolbar.h GetButtonSize
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }
//     int cyText = (m_dwStyle & CBRS_ORIENT_HORZ /*0xa000*/)      // 0x104
//                      ? afxGlobalData.<+0x2a8> : afxGlobalData.<+0x2ac>;
//     if (cyText > cy) return cyText;      // both re-read after the gate
//     return cy;                           // recomputed the same way
// Note the menu-mode select tests `m_sizeMenuButton.cy > 0`, not the `cx == -1`
// that GetMenuButtonSize uses.
// Deviation: GetButtonSize() is the side-table CMFCToolBar::GetButtonSize
// defined in this file (the size registered by SetSizes, default 23x22).  The
// retail inline at 0x239b4 instead selects
//     m_bLocked ? (m_bLargeIconsAreEnbaled ? m_sizeCurButtonLocked /*0x1168*/
//                                          : m_sizeButtonLocked    /*0x1158*/)
//               : (m_bLargeIconsAreEnbaled ? m_sizeCurButton /*0x3b1ee8*/
//                                          : m_sizeButton    /*0x3b1e38*/)
// and then adds CMFCVisualManager::GetInstance()->GetButtonExtraBorder()
// (vslot 0x290) when IsButtonExtraSizeAvailable() (vslot 0x6c8).  OpenMFC's
// SetSizes writes only the side table, not the exported m_sizeButton static,
// so the side table is the value that tracks SetSizes here; the locked /
// large-icon selection and the extra border are not reproduced.
// Symbol: ?GetRowHeight@CMFCToolBar@@UEBAHXZ
extern "C" int MS_ABI impl__GetRowHeight_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis) {
    if (!pThis) return 0;
    if (pThis->m_bDrawTextLabels) return pThis->m_nMaxBtnHeight;

    int cy;
    if (pThis->m_bMenuMode) {
        const long cyMenu = impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cy;
        cy = static_cast<int>(cyMenu > 0 ? cyMenu : impl__m_sizeButton_CMFCToolBar__1VCSize__A.cy);
    } else {
        cy = pThis->GetButtonSize().cy;
    }

    EnsureGlobalDataInitialized();
    const int cyText = GlobalDataTextHeight((pThis->m_dwStyle & 0xa000) != 0);
    return cyText > cy ? cyText : cy;
}

// Retail (RVA 0x150390, mfc140u), fully transcribed:
//     int i = 0;
//     for (node = m_Buttons.head (0x1190); node; node = node->next, i++) {
//         pButton = node->data;  if (!pButton) AfxThrowInvalidArgException();
//         if (::PtInRect(&pButton->m_rect /*+0x68*/, point) &&
//             !pButton->m_bIsHidden /*+0x54*/)
//             return (pButton->m_nStyle & TBBS_SEPARATOR /*+0x28 bit 0*/) ? -1 : i;
//     }
//     return -1;
// (0x1802c72f8 resolves to USER32!PtInRect.)  Same side-table deviation as
// CommandToIndex: the walk runs over GetCount()/GetButton().
// Symbol: ?HitTest@CMFCToolBar@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, CPoint point) {
    if (!pThis) return -1;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        const CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        RECT rc = { pButton->m_rect.left, pButton->m_rect.top,
                    pButton->m_rect.right, pButton->m_rect.bottom };
        POINT pt = { point.x, point.y };
        if (::PtInRect(&rc, pt) && !pButton->m_bIsHidden) {
            return (pButton->m_nStyle & 0x1) ? -1 : i;
        }
    }
    return -1;
}

// Retail (RVA 0x14eac0, mfc140u), fully decoded:
//     if (!pButton) AfxThrowInvalidArgException();
//     if (m_lstUnpermittedCommands.Find(pButton->m_nID)) return -1;   // 0x11f1c on 0x3b2058
//     if (iInsertAt != -1) {
//         if (iInsertAt < 0 || iInsertAt > m_Buttons.GetCount()) return -1;
//         if (iInsertAt < m_Buttons.GetCount()) {
//             m_Buttons.InsertBefore(m_Buttons.FindIndex(iInsertAt), pButton); // 0x232010
//             pButton->OnChangeParentWnd(this);                     // vslot 0x60
//             return iInsertAt;
//         }
//     }
//     if (m_pCustomizeBtn == NULL) {                                // 0x1300
//         m_Buttons.AddTail(pButton);                               // 0x231e70
//         pButton->OnChangeParentWnd(this);
//         return m_Buttons.GetCount() - 1;
//     }
//     iInsertAt = m_Buttons.GetCount() - 1;          // keep the customize
//     m_Buttons.InsertBefore(FindIndex(iInsertAt), pButton);   // button last
//     pButton->OnChangeParentWnd(this);
//     return iInsertAt;
// (FindIndex is inlined as a head-walk that throws on a bad index.)
// Deviations:
//  * the button goes into the side table that GetCount()/GetButton()/
//    CommandToIndex/HitTest read (mfccore.cpp g_toolBarStates), not the
//    m_Buttons CObList, and is marked owned so the toolbar's cleanup deletes it
//    as retail's RemoveAllButtons would.  RemoveButton/RemoveAllButtons above
//    still operate on the CObList, so a button inserted here is not removable
//    through them -- a pre-existing split this change does not widen: those two
//    already could not see SetButtons' buttons either;
//  * the m_lstUnpermittedCommands test is dropped: the static is an opaque
//    zeroed block and SetNonPermittedCommands is a stub, so the list is always
//    empty and Find would return NULL anyway;
//  * OnChangeParentWnd cannot be dispatched (no button vtable, see
//    CMFCVisualManagerOfficeXP.cpp) and the exported base thunk is an empty
//    stub, so the retail base body (0x15e570) is reproduced by the
//    ButtonChangeParentWnd helper at the top of this file.
// Symbol: ?InsertButton@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@H@Z
extern "C" int MS_ABI impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, int iInsertAt) {
    if (!pThis || !pButton) return -1;

    ToolBarState& state = g_toolBarStates[pThis];
    const int nCount = static_cast<int>(state.buttons.size());

    // TODO(clean-room): m_lstUnpermittedCommands.Find is not modelled (always
    // empty here).

    if (iInsertAt != -1) {
        if (iInsertAt < 0 || iInsertAt > nCount) return -1;
        if (iInsertAt < nCount) {
            state.buttons.insert(state.buttons.begin() + iInsertAt, pButton);
            state.ownedButtons.insert(pButton);
            ButtonChangeParentWnd(pButton, pThis);
            return iInsertAt;
        }
    }

    if (pThis->m_pCustomizeBtn == nullptr) {
        state.buttons.push_back(pButton);
        state.ownedButtons.insert(pButton);
        ButtonChangeParentWnd(pButton, pThis);
        return nCount;
    }

    // A customize button is present: insert in front of it (it stays last).
    iInsertAt = nCount - 1;
    if (iInsertAt < 0) return -1;   // retail's FindIndex(-1) throws here
    state.buttons.insert(state.buttons.begin() + iInsertAt, pButton);
    state.ownedButtons.insert(pButton);
    ButtonChangeParentWnd(pButton, pThis);
    return iInsertAt;
}

// Retail (RVA 0x14e940, mfc140u), fully transcribed:
//     CRuntimeClass* pRTC = button.GetRuntimeClass();   // vslot 0
//     if (!pRTC) AfxThrowInvalidArgException();
//     CMFCToolBarButton* pButton = (CMFCToolBarButton*)pRTC->CreateObject();  // 0x234d60
//     if (!pButton) AfxThrowInvalidArgException();
//     pButton->CopyFrom(button);                        // vslot 0x38
//     int iIndex = InsertButton(pButton, (int)iInsertAt);   // vslot 0x690
//     if (iIndex < 0) delete pButton;                   // vslot 8, flag 1
//     return iIndex;
// GetRuntimeClass is slot 0 under both vtable ABIs, so the C++ virtual call is
// safe on MSVC-built and mingw-built buttons alike.  Deviations: CopyFrom
// goes through the exported base body (a derived CopyFrom override is not
// reached -- no dispatchable button vtable); InsertButton is this file's
// export rather than vslot 0x690; and the throws are early returns.
// Symbol: ?InsertButton@CMFCToolBar@@UEAAHAEBVCMFCToolBarButton@@_J@Z
extern "C" int MS_ABI impl__InsertButton_CMFCToolBar__UEAAHAEBVCMFCToolBarButton___J_Z(
    CMFCToolBar* pThis, const CMFCToolBarButton* pSrc, __int64 iInsertAt) {
    if (!pThis || !pSrc) return -1;

    CRuntimeClass* pRTC = pSrc->GetRuntimeClass();
    if (!pRTC) return -1;
    CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(
        impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pRTC));
    if (!pButton) return -1;

    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pButton, pSrc);

    const int iIndex = impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(
        pThis, pButton, static_cast<int>(iInsertAt));
    if (iIndex < 0) {
        delete pButton;
    }
    return iIndex;
}

// Retail (RVA 0x15b500, mfc140u), fully transcribed:
//     if (iInsertAt != -1 && (iInsertAt < 0 || iInsertAt > m_OrigResetButtons.GetCount()))
//         return -1;                                             // 0x1210
//     pRTC = button.GetRuntimeClass(); if (!pRTC) throw;
//     pButton = pRTC->CreateObject(); if (!pButton) throw;        // 0x234d60
//     pButton->CopyFrom(button);                                  // vslot 0x38
//     if (iInsertAt == -1 || iInsertAt == m_OrigResetButtons.GetCount()) {
//         m_OrigResetButtons.AddTail(pButton);                    // 0x231e70
//         return m_OrigResetButtons.GetCount() - 1;
//     }
//     m_OrigResetButtons.InsertBefore(FindIndex(iInsertAt), pButton);  // 0x232010
//     return iInsertAt;
// m_OrigResetButtons (0x11f8) is the real CObList here, as in
// RemoveResetStateButton above.  Same CopyFrom / throw deviations as the
// InsertButton overload above.
// Symbol: ?InsertResetStateButton@CMFCToolBar@@IEAAHAEBVCMFCToolBarButton@@H@Z
extern "C" int MS_ABI impl__InsertResetStateButton_CMFCToolBar__IEAAHAEBVCMFCToolBarButton__H_Z(
    CMFCToolBar* pThis, const CMFCToolBarButton* pSrc, int iInsertAt) {
    if (!pThis || !pSrc) return -1;

    const INT_PTR nCount = pThis->m_OrigResetButtons.GetCount();
    if (iInsertAt != -1 && (iInsertAt < 0 || iInsertAt > nCount)) return -1;

    CRuntimeClass* pRTC = pSrc->GetRuntimeClass();
    if (!pRTC) return -1;
    CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(
        impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pRTC));
    if (!pButton) return -1;

    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pButton, pSrc);

    if (iInsertAt == -1 || iInsertAt == nCount) {
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(&pThis->m_OrigResetButtons, pButton);
        return static_cast<int>(pThis->m_OrigResetButtons.GetCount() - 1);
    }

    CObList::POSITION pos = pThis->m_OrigResetButtons.FindIndex(iInsertAt);
    if (pos == CObList::POSITION(nullptr)) {
        delete pButton;   // retail throws before allocating; keep no leak
        return -1;
    }
    impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(
        &pThis->m_OrigResetButtons, &pos, pButton);
    return iInsertAt;
}

// Retail (RVA 0x14ebb0, mfc140u), fully transcribed:
//     if (m_Buttons.GetCount() == 0 || iInsertAt == 0) return -1;   // 0x11a0
//     CMFCToolBarButton* pSep = new CMFCToolBarButton();   // 0x88 bytes, ctor 0x15bec0
//     pSep->m_nStyle = TBBS_SEPARATOR;                     // +0x28 = 1
//     int iIndex = InsertButton(pSep, (int)iInsertAt);     // vslot 0x690
//     if (iIndex == -1) delete pSep;                       // vslot 8, flag 1
//     return iIndex;
// Deviations: the count comes from the side table (GetCount()), and
// InsertButton is this file's export rather than vslot 0x690.
// Symbol: ?InsertSeparator@CMFCToolBar@@UEAAH_J@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCToolBar__UEAAH_J_Z(CMFCToolBar* pThis, __int64 iInsertAt) {
    if (!pThis) return -1;
    if (pThis->GetCount() == 0 || iInsertAt == 0) return -1;

    // `new CMFCToolBarButton()` as retail does it: raw allocation of the
    // 0x88-byte object plus the exported default ctor (featurepack/toolbar/
    // Thunks.cpp), since the C++ ctor is not linkable from this unit.
    void* pMem = ::operator new(sizeof(CMFCToolBarButton), std::nothrow);
    if (!pMem) return -1;
    CMFCToolBarButton* pSep = static_cast<CMFCToolBarButton*>(impl___0CMFCToolBarButton__QEAA_XZ(pMem));
    pSep->m_nStyle = 0x1;   // TBBS_SEPARATOR

    const int iIndex = impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(
        pThis, pSep, static_cast<int>(iInsertAt));
    if (iIndex == -1) {
        delete pSep;
    }
    return iIndex;
}

// Retail (RVA 0x152290, mfc140u), fully transcribed:
//     if (m_bDisableCustomize) return;                        // 0x10f8
//     m_iDragIndex = -1;                                      // 0x1148
//     CRect rect = m_rectDrag; ::InflateRect(&rect, 2, 2);    // 0x12a0
//     ::InvalidateRect(m_hWnd, &rect, TRUE);
//     ::UpdateWindow(m_hWnd);
//     ::SetRectEmpty(&m_rectDrag);
//     m_iDragIndex = -1;                                      // written twice
//     m_DropSource.m_bDeleteOnDrop = TRUE;                    // 0x3c2578, see top
// (0x1802c72e8/0x2c7128/0x2c7300/0x2c7348 resolve to USER32!InflateRect /
// InvalidateRect / UpdateWindow / SetRectEmpty.)
// Symbol: ?OnDragLeave@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnDragLeave_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis) {
    if (!pThis) return;
    if (pThis->m_bDisableCustomize) return;

    pThis->m_iDragIndex = -1;
    RECT rect = { pThis->m_rectDrag.left, pThis->m_rectDrag.top,
                  pThis->m_rectDrag.right, pThis->m_rectDrag.bottom };
    ::InflateRect(&rect, 2, 2);
    ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
    ::UpdateWindow(pThis->m_hWnd);
    pThis->m_rectDrag.left = pThis->m_rectDrag.top = 0;
    pThis->m_rectDrag.right = pThis->m_rectDrag.bottom = 0;
    pThis->m_iDragIndex = -1;
    SetDropSourceDeleteOnDrop(TRUE);
}

// Retail (RVA 0x152340, mfc140u), fully transcribed:
//     if (m_bLocked || m_bDisableCustomize) return DROPEFFECT_NONE;  // 0x10b8, 0x10f8
//     CMFCToolBarButton* p = CMFCToolBarButton::CreateFromOleData(pDataObject); // 0x15c790
//     if (!p) return DROPEFFECT_NONE;
//     BOOL bAllow = p->CanBeDropped(this);    // vslot 0x30; the base body is
//     delete p;                               // `mov eax,1; ret` (0x3a60)
//     if (!bAllow) return DROPEFFECT_NONE;
//     m_bIsDragCopy = dwKeyState & MK_CONTROL;                  // 0x10e0 (the
//                                                               // masked word, 8, not 1)
//     if (m_pDragButton == NULL) {                              // 0x12f0
//         int iSel = m_iSelected; m_iSelected = -1;             // 0x113c
//         if (iSel != -1) { InvalidateButton(iSel); ::UpdateWindow(m_hWnd); }  // 0x14fe50
//     }
//     CRect rectOld = m_rectDrag;                               // 0x12a0
//     int iDrag = FindDropIndex(point, m_rectDrag);             // vslot 0x7d0, by reference
//     if (!::EqualRect(&rectOld, &m_rectDrag)) {
//         m_iDragIndex = iDrag;                                 // 0x1148
//         ::InflateRect(&rectOld, 2, 2); ::InvalidateRect(m_hWnd, &rectOld, TRUE);
//         CRect r2 = m_rectDrag; ::InflateRect(&r2, 2, 2);
//         ::InvalidateRect(m_hWnd, &m_rectDrag, TRUE);   // NB: the member, not
//         ::UpdateWindow(m_hWnd);                        // the inflated copy
//     }
//     int iHot = HitTest(point);                                // vslot 0x730
//     int iPrev = m_iHighlighted; m_iHighlighted = iHot;        // 0x1138
//     if (iPrev != iHot) OnChangeHot(iHot);                     // vslot 0x770
//     if (iDrag == -1) return DROPEFFECT_NONE;
//     return m_bIsDragCopy ? DROPEFFECT_COPY : DROPEFFECT_MOVE; // neg/sbb +2
// The second InflateRect really is on a copy that is then never used -- the
// invalidate at 0x15248b passes rdi (&m_rectDrag).  Reproduced as-is.
// Deviations: CanBeDropped is not dispatched (base TRUE is assumed -- a
// derived override returning FALSE is not honoured), and FindDropIndex /
// HitTest / OnChangeHot are this file's exports.  CreateFromOleData is a stub
// that returns NULL in OpenMFC, so today this always takes the second early
// return; the rest is in place for when it is implemented.
// Symbol: ?OnDragOver@CMFCToolBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point) {
    if (!pThis) return DROPEFFECT_NONE;
    if (pThis->m_bLocked || pThis->m_bDisableCustomize) return DROPEFFECT_NONE;

    CMFCToolBarButton* pProbe =
        impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(pDataObject);
    if (!pProbe) return DROPEFFECT_NONE;
    // TODO(clean-room): pProbe->CanBeDropped(this) (vslot 0x30) is not
    // dispatched; the base body returns TRUE.
    const int bAllowDrop = TRUE;
    delete pProbe;
    if (!bAllowDrop) return DROPEFFECT_NONE;

    // Retail stores the masked word itself (`and $0x8,%ebp; mov %ebp,0x10e0`),
    // so a client reading m_bIsDragCopy sees 8, not 1.
    pThis->m_bIsDragCopy = static_cast<int>(dwKeyState & MK_CONTROL);

    if (pThis->m_pDragButton == nullptr) {
        const int iSel = pThis->m_iSelected;
        pThis->m_iSelected = -1;
        if (iSel != -1) {
            impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iSel);
            ::UpdateWindow(pThis->m_hWnd);
        }
    }

    RECT rectOld = { pThis->m_rectDrag.left, pThis->m_rectDrag.top,
                     pThis->m_rectDrag.right, pThis->m_rectDrag.bottom };
    const int iDragIndex = impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(
        pThis, point, &pThis->m_rectDrag);
    RECT rectNew = { pThis->m_rectDrag.left, pThis->m_rectDrag.top,
                     pThis->m_rectDrag.right, pThis->m_rectDrag.bottom };
    if (!::EqualRect(&rectOld, &rectNew)) {
        pThis->m_iDragIndex = iDragIndex;
        ::InflateRect(&rectOld, 2, 2);
        ::InvalidateRect(pThis->m_hWnd, &rectOld, TRUE);
        RECT rectInflated = rectNew;
        ::InflateRect(&rectInflated, 2, 2);     // computed and unused, as retail
        ::InvalidateRect(pThis->m_hWnd, &rectNew, TRUE);
        ::UpdateWindow(pThis->m_hWnd);
    }

    const int iHot = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, point);
    const int iPrevHot = pThis->m_iHighlighted;
    pThis->m_iHighlighted = iHot;
    if (iPrevHot != iHot) {
        impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iHot);
    }

    if (iDragIndex == -1) return DROPEFFECT_NONE;
    return pThis->m_bIsDragCopy ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

// Retail (RVA 0x152520, mfc140u), fully decoded:
//     if (m_bDisableCustomize) return FALSE;                     // 0x10f8
//     int iDragIndex = m_iDragIndex; if (iDragIndex < 0) return FALSE;   // 0x1148
//     CMFCToolBarButton* pDragButton = m_pDragButton; m_pDragButton = NULL; // 0x12f0
//     OnDragLeave();                                             // vslot 0x7b8
//     int iHit = HitTest(point);                                 // vslot 0x730
//     if (iHit >= 0 && GetButton(iHit) == pDragButton) return FALSE;  // 0x14fe00
//     CMFCToolBarButton* pButton = CreateDroppedButton(pDataObject);   // vslot 0x808
//     if (!pButton) return FALSE;
//     if (!pButton->OnBeforeDrop(this)) { delete pButton; return TRUE; }  // vslot 0x100
//     pButton->m_bDragFromCollection = FALSE;                    // +0x20
//     if (pDragButton && dropEffect != DROPEFFECT_COPY) {
//         int iIndex = ButtonToIndex(pDragButton);               // 0x14f000
//         if (iDragIndex == iIndex || iDragIndex == iIndex + 1) {
//             AddRemoveSeparator(pDragButton, m_ptStartDrag, point);   // vslot 0x838
//             delete pButton; return TRUE;
//         }
//         RemoveButton(iIndex);                                  // vslot 0x6a8
//         if (iDragIndex > iIndex) iDragIndex--;
//         iDragIndex = min(iDragIndex, m_Buttons.GetCount());    // 0x11a0
//     }
//     if (InsertButton(pButton, iDragIndex) == -1) { delete pButton; return FALSE; }  // vslot 0x690
//     AdjustLayout();                                            // vslot 0x428
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));     // 0x28ad70
//     if (pParent->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl))) {       // 0x312be0
//         CWnd* pGrand = CWnd::FromHandle(::GetParent(pParent->m_hWnd));
//         if (pGrand && pGrand->IsKindOf(RUNTIME_CLASS(CMFCToolBar)))   // 0x3b15f8
//             pGrand->RecalcLayout();                            // vslot 0x430
//     }
//     if (m_bAltCustomizeMode) pButton->SaveBarState();          // 0x3be36c, vslot 0x110
//     m_iSelected = -1;
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x505);
//     pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu)))   // 0x3b1448
//         ::RedrawWindow(pParent->m_hWnd, NULL, NULL, 0x505);
//     return TRUE;
// (0x1802c72d8 = USER32!GetParent, 0x2c7130 = RedrawWindow; the retail
// vtable at 0x1803157c8 names 0x7b8 OnDragLeave, 0x808 CreateDroppedButton,
// 0x838 AddRemoveSeparator, 0x430 CPane::RecalcLayout, 0x428 AdjustLayout.)
// Implemented up to CreateDroppedButton with the retail control flow; the
// button vslots 0x100 (OnBeforeDrop, base returns TRUE -- 0x3a60) and 0x110
// (SaveBarState, base is a bare `ret` -- 0x27d0) are assumed to be the base
// bodies; AddRemoveSeparator is this file's export (a stub today) and the
// grandparent's RecalcLayout goes through ?RecalcLayout@CPane@@UEAAXXZ in
// featurepack/docking/Thunks.cpp, which dispatches virtually on the C++
// object (retail slot 0x430 of the CMFCToolBar vftable is CPane::RecalcLayout
// itself, 0x9fde0 -- CMFCToolBar does not override it).  CreateDroppedButton
// is a stub that returns NULL in OpenMFC, so
// today every drop ends at that `return FALSE`; the remainder is in place for
// when it is implemented.  Note the pre-existing split described under
// InsertButton: RemoveButton works on the m_Buttons CObList while
// InsertButton / GetCount work on the side table, so the move branch's
// RemoveButton would not find a side-table button.
// Symbol: ?OnDrop@CMFCToolBar@@MEAAHPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" int MS_ABI impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dropEffect, CPoint point) {
    if (!pThis) return FALSE;
    if (pThis->m_bDisableCustomize) return FALSE;

    int iDragIndex = pThis->m_iDragIndex;
    if (iDragIndex < 0) return FALSE;

    CMFCToolBarButton* pDragButton = pThis->m_pDragButton;
    pThis->m_pDragButton = nullptr;

    impl__OnDragLeave_CMFCToolBar__MEAAXXZ(pThis);

    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, point);
    if (iHit >= 0 && pThis->GetButton(iHit) == pDragButton) return FALSE;

    CMFCToolBarButton* pButton =
        impl__CreateDroppedButton_CMFCToolBar__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(
            pThis, pDataObject);
    if (!pButton) return FALSE;

    // TODO(clean-room): pButton->OnBeforeDrop(this) (vslot 0x100) is not
    // dispatched; the base body returns TRUE.
    pButton->m_bDragFromCollection = FALSE;

    if (pDragButton != nullptr && dropEffect != DROPEFFECT_COPY) {
        const int iIndex = impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(pThis, pDragButton);
        if (iDragIndex == iIndex || iDragIndex == iIndex + 1) {
            CPoint ptStart = pThis->m_ptStartDrag;
            impl__AddRemoveSeparator_CMFCToolBar__MEAAXPEBVCMFCToolBarButton__AEBVCPoint__1_Z(
                pThis, pDragButton, &ptStart, &point);
            delete pButton;
            return TRUE;
        }
        impl__RemoveButton_CMFCToolBar__UEAAHH_Z(pThis, iIndex);
        if (iDragIndex > iIndex) iDragIndex--;
        const int nCount = pThis->GetCount();
        if (iDragIndex > nCount) iDragIndex = nCount;
    }

    if (impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(pThis, pButton, iDragIndex) == -1) {
        delete pButton;
        return FALSE;
    }

    impl__AdjustLayout_CMFCToolBar__UEAAXXZ(pThis);

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                       pParent, impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ())) {
        CWnd* pGrand = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParent->m_hWnd));
        if (pGrand && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                          pGrand, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
            impl__RecalcLayout_CPane__UEAAXXZ(static_cast<CPane*>(pGrand));   // vslot 0x430
        }
    }

    // TODO(clean-room): if (m_bAltCustomizeMode) pButton->SaveBarState()
    // (vslot 0x110) -- not dispatched; the base body is a bare `ret`.

    pThis->m_iSelected = -1;
    ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, 0x505);

    pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                       pParent, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ())) {
        ::RedrawWindow(pParent->m_hWnd, nullptr, nullptr, 0x505);
    }
    return TRUE;
}

// Retail: the WM_ERASEBKGND entry of the CMFCToolBar message map (0x315270,
// mfc140u) points at 0x3a60, a COMDAT-folded `mov eax,1; ret` shared with
// OnNcHitTest below and several unrelated exports.  The background is left
// to the paint path.
// Symbol: ?OnEraseBkgnd@CMFCToolBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolBar__IEAAHPEAVCDC___Z(CMFCToolBar* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Retail (RVA 0x159860, mfc140u; the WM_USER+23 (0x417) entry of the message
// map), fully transcribed.  lParam is a TBBUTTON*:
//     CMFCToolBarButton* p = GetButton((int)wParam);            // 0x14fe00
//     UINT style = p ? p->m_nStyle : 0;                         // +0x28
//     pTB->fsStyle = (BYTE)style;                               // +9
//     pTB->fsState = (BYTE)(style >> 16);                       // +8
//     p = GetButton((int)wParam);                               // called again
//     pTB->idCommand = p ? p->m_nID : 0;                        // +4, +0x24
//     pTB->iBitmap = 0; pTB->dwData = 0; pTB->iString = 0;      // +0, +0x10, +0x18
//     return TRUE;
// (When GetButton returns NULL retail just uses the zero left in eax.)
// Symbol: ?OnGetButton@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButton_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    TBBUTTON* pTB = reinterpret_cast<TBBUTTON*>(lParam);
    if (!pThis || !pTB) return 0;   // retail would fault on a NULL lParam
    const CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(wParam));
    const unsigned int nStyle = pButton ? pButton->m_nStyle : 0u;
    pTB->fsStyle = static_cast<BYTE>(nStyle);
    pTB->fsState = static_cast<BYTE>(nStyle >> 16);
    pButton = pThis->GetButton(static_cast<int>(wParam));
    pTB->idCommand = pButton ? static_cast<int>(pButton->m_nID) : 0;
    pTB->iBitmap = 0;
    pTB->dwData = 0;
    pTB->iString = 0;
    return TRUE;
}

// Retail (RVA 0x159830, mfc140u; the WM_USER+24 (0x418) entry of the message
// map) is `movslq 0x11a0(%rcx),%rax; ret` -- m_Buttons.GetCount().  Side-table
// deviation as everywhere in this file.
// Symbol: ?OnGetButtonCount@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButtonCount_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam; (void)lParam;
    if (!pThis) return 0;
    return pThis->GetCount();
}

// Retail (RVA 0x1598d0, mfc140u; the WM_USER+75 (0x44b) entry of the message
// map), fully transcribed:
//     int iIndex = CommandToIndex((UINT)wParam, 0);            // 0x14eed0
//     CString str = GetButtonText(iIndex);                     // 0x14f210
//     if (lParam) ::lstrcpyW((LPWSTR)lParam, str);             // 0x1802c6618
//     return str.GetLength();                                  // CStringData -0x10
// Symbol: ?OnGetButtonText@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButtonText_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    const int iIndex = impl__CommandToIndex_CMFCToolBar__QEBAHIH_Z(
        pThis, static_cast<unsigned int>(wParam), 0);
    const CString strText = pThis->GetButtonText(iIndex);
    if (lParam) {
        ::lstrcpyW(reinterpret_cast<wchar_t*>(lParam), strText.GetString());
    }
    return strText.GetLength();
}

// Retail (RVA 0x159840, mfc140u; the WM_USER+29 (0x41d) entry of the message
// map): `GetItemRect((int)wParam, (LPRECT)lParam)` through vslot 0x6e8 (the
// retail vtable at 0x1803157c8 names that slot GetItemRect), then return TRUE.
// Deviation: this file's GetItemRect export is called directly, so a
// derived-class override is not reached.
// Symbol: ?OnGetItemRect@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetItemRect_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    impl__GetItemRect_CMFCToolBar__UEBAXHPEAUtagRECT___Z(
        pThis, static_cast<int>(wParam), reinterpret_cast<RECT*>(lParam));
    return TRUE;
}

// Retail (RVA 0x15b360, mfc140u; slot 236 of the vtable at 0x1803157c8),
// fully transcribed:
//     for (node = m_Buttons.head (0x1190); node; node = node->next) {
//         pButton = node->data;  if (!pButton) AfxThrowInvalidArgException();
//         pButton->OnGlobalFontsChanged();                    // vslot 0xc8
//     }
// Slot 0xc8 of the retail CMFCToolBarButton vftable (0x316048) is 0x27d0, a
// bare `ret`, so for a plain button the loop does nothing; only derived
// buttons override it.  Deviation: with no dispatchable button vtable the
// override cannot be reached, so the walk is kept (side table) and the call
// is not made.
// Symbol: ?OnGlobalFontsChanged@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis) {
    if (!pThis) return;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        // TODO(clean-room): pButton->OnGlobalFontsChanged() (vslot 0xc8) is
        // not dispatched; the base body is a bare `ret`.
    }
}

// Retail (RVA 0x156bc0, mfc140u; the WM_HELPHITTEST (0x366) entry of the
// message map), fully transcribed:
//     OnCancelMode();                                          // 0x1516b0
//     CPoint pt((short)LOWORD(lParam), (short)HIWORD(lParam));
//     int iHit = HitTest(pt);                                  // vslot 0x730
//     if (iHit < 0) { ::MessageBeep((UINT)-1); return -1; }    // 0x1802c7230
//     CMFCToolBarButton* p = GetButton(iHit); if (!p) return -1;   // 0x14fe00
//     if (p->OnContextHelp(this)) return -1;                   // vslot 0xd8; base
//                                                              // is `xor eax,eax` (0x71e0)
//     LRESULT lRes = CWnd::OnHelpHitTest(wParam, lParam);      // 0xc9b0, see below
//     UINT id = p->m_nID;                                      // +0x24
//     if (id - 0xF000 <= 0x1EF)                                // an SC_* id
//         lRes = 0x1EF00 + ((id - 0xF000) >> 4);   // HID_BASE_COMMAND + ID_COMMAND_FROM_SC(id)
//     if (p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton)))   // 0x3188a0
//         lRes = 0x1EF00 + ((p-><+0x88> - 0xF000) >> 4);       // its system-command id
//     if (lRes > 0) SetHelpMode(FALSE);                        // 0x156e50
//     return lRes;
// CWnd::OnHelpHitTest (0xc9b0, not exported) is:
//     INT_PTR nID = OnToolHitTest(pt, NULL);                   // vslot 0xf8
//     if (nID != -1) return HID_BASE_COMMAND /*0x10000*/ + nID;
//     nID = ::GetDlgCtrlID(m_hWnd);                            // 0x1802c72a8
//     return nID ? HID_BASE_CONTROL /*0x50000*/ + nID : 0;
// Deviations: OnContextHelp is not dispatched (base FALSE assumed), the
// CMFCToolBarMenuButtonsButton member at +0x88 is read by offset (the class
// is not declared in OpenMFC; sizeof(CMFCToolBarButton) == 0x88 so it is the
// derived class's first member), OnToolHitTest is this file's export rather
// than vslot 0xf8, and SetHelpMode is still a stub in this file.
// Symbol: ?OnHelpHitTest@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis) return -1;

    impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pThis);

    CPoint pt(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)));
    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, pt);
    if (iHit < 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return -1;
    }
    const CMFCToolBarButton* pButton = pThis->GetButton(iHit);
    if (!pButton) return -1;
    // TODO(clean-room): pButton->OnContextHelp(this) (vslot 0xd8) is not
    // dispatched; the base body returns FALSE.

    // CWnd::OnHelpHitTest, inlined here (see the comment above).
    __int64 lRes;
    const __int64 nToolId = impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
        pThis, pt, nullptr);
    if (nToolId != -1) {
        lRes = 0x10000 + nToolId;
    } else {
        const int nCtrlId = ::GetDlgCtrlID(pThis->m_hWnd);
        lRes = nCtrlId ? 0x50000 + static_cast<unsigned int>(nCtrlId) : 0;
    }

    const unsigned int nID = pButton->m_nID;
    if (nID - 0xF000u <= 0x1EFu) {
        lRes = 0x1EF00 + ((nID - 0xF000u) >> 4);
    }
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pButton, impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ())) {
        unsigned int nSysCmd;
        std::memcpy(&nSysCmd, reinterpret_cast<const char*>(pButton) + 0x88, sizeof nSysCmd);
        lRes = 0x1EF00 + ((nSysCmd - 0xF000u) >> 4);
    }
    if (lRes > 0) {
        impl__SetHelpMode_CMFCToolBar__SAXH_Z(FALSE);
    }
    return lRes;
}

// Retail (RVA 0x159440, mfc140u; the WM_KILLFOCUS entry of the message map),
// fully transcribed:
//     CWnd::Default();                                            // 0x28ac80
//     if (CMFCToolBar::m_bCustomizeMode) return;                  // 0x3be35c
//     if (pNewWnd && pNewWnd->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu)) &&   // 0x3b1448
//         ((CMFCPopupMenu*)pNewWnd)->GetParentToolBar() == this)  // 0xb75f0
//         return;
//     Deactivate();                                               // vslot 0x6d0
// Deviation: Deactivate is this file's export rather than vslot 0x6d0.
// Symbol: ?OnKillFocus@CMFCToolBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(CMFCToolBar* pThis, CWnd* pNewWnd) {
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (pNewWnd &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pNewWnd, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ()) &&
        impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(pNewWnd) == pThis) {
        return;
    }
    impl__Deactivate_CMFCToolBar__UEAAXXZ(pThis);
}

// Retail (RVA 0x1560c0, mfc140u; the WM_LBUTTONDBLCLK entry of the message
// map), fully transcribed:
//     int iHit = HitTest(point);                                  // vslot 0x730
//     if (iHit >= 0) {
//         CMFCToolBarButton* p = GetButton(iHit); if (!p) return;  // 0x14fe00
//         BOOL bSysMenu = p->IsKindOf(RUNTIME_CLASS(CMFCToolBarSystemMenuButton)); // 0x3b17a8
//         p->OnDblClk(this);                                       // vslot 0xa8
//         if (!bSysMenu) OnLButtonDown(nFlags, point);             // 0x150410
//         return;
//     }
//     if (IsDocked()) { CPane::OnLButtonDblClk(nFlags, point); return; }  // vslot 0x2e0, 0xa1a50
//     CPaneFrameWnd* pMini = GetParentMiniFrame(FALSE);            // vslot 0x460
//     if (pMini) pMini->OnDockToRecentPos();                       // vslot 0x340
// (Vtable slots named from the retail vtables: CMFCToolBar 0x1803157c8 --
// 0x2e0 CBasePane::IsDocked, 0x460 CBasePane::GetParentMiniFrame; CPaneFrameWnd
// 0x1802f7b88 -- 0x340 OnDockToRecentPos.)
// Deviations: OnDblClk is not dispatched (slot 21 of the button vftable at
// 0x316048 is 0x27d0, a bare `ret`, so the base does nothing; a derived
// override is not reached), and CPane::OnLButtonDblClk / OnDockToRecentPos /
// OnLButtonDown are stubs in their files, so only the hit-test / IsKindOf /
// GetParentMiniFrame plumbing is live.
// Symbol: ?OnLButtonDblClk@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(
    CMFCToolBar* pThis, unsigned int nFlags, CPoint point) {
    if (!pThis) return;

    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, point);
    if (iHit >= 0) {
        CMFCToolBarButton* pButton = pThis->GetButton(iHit);
        if (!pButton) return;
        const int bSysMenuButton = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pButton, impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ());
        // TODO(clean-room): pButton->OnDblClk(this) (vslot 0xa8) is not
        // dispatched; the base body is a bare `ret`.
        if (!bSysMenuButton) {
            impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(pThis, nFlags, point);
        }
        return;
    }

    if (impl__IsDocked_CBasePane__UEBAHXZ(pThis)) {
        impl__OnLButtonDblClk_CPane__IEAAXIVCPoint___Z(pThis, nFlags, point);
        return;
    }
    void* pMiniFrame = impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pThis, FALSE);
    if (pMiniFrame) {
        impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(pMiniFrame);
    }
}

// Retail (RVA 0x150410, mfc140u; the WM_LBUTTONDOWN entry of the message map)
// is ~330 instructions.  Decoded structure, NOT implemented:
//     int iHit = HitTest(point);                                   // vslot 0x730
//     if (m_pSelToolbar != this && m_bCustomizeMode) {             // 0x3be338 / 0x3be35c
//         swap m_pSelToolbar to this, clearing the old one's m_iSelected
//         via InvalidateButton (0x14fe50);
//     }
//     if (!m_bCustomizeMode && parent IsKindOf(CPane) (0x2f5918)) parent->SetFocus();  // 0x2a9b60, see OnRButtonDown
//     if (iHit < 0) {                    // background click
//         m_iButtonCapture = -1; drop the customize selection; OnChangeHot(-1);
//         if (vslot 0x3a8) ::SetCursor(afxGlobalData+0x170);
//         CPane::OnLButtonDown(nFlags, point) (0x9f810); return;
//     }
//     pButton = GetButton(iHit); vslot 0x4c8 (this, iHit);   // AccNotifyObjectFocusEvent
//     m_bAltCustomizeMode = FALSE;
//     if (m_bAltCustomization && vslot 0x850 && !m_bCustomizeMode &&
//         ::GetAsyncKeyState(VK_MENU) < 0) { enter alt-customize: m_iSelected = iHit,
//         m_iHighlighted = -1, m_pSelToolbar = this; }
//     if ((m_bCustomizeMode || m_bAltCustomizeMode) && !m_bLocked && !m_bDisableCustomize) {
//         // customize drag: m_pDragButton = pButton, vslots 0xd0 / 0x88 / 0xb0 / 0xf8
//         // on the button, stretch-handle test (labs(x - rect.right) <= 6),
//         // COleDataSource (0x252f90) + pButton->PrepareDrag (vslot 0x28) +
//         // ShowCommandMessageString + COleDataSource::DoDragDrop (0x25a690)
//         // with &m_DropSource, then RemoveButton / AdjustLocations /
//         // RedrawWindow / vslot 0x5b8 / parent CMFCTabCtrl RecalcLayout,
//         // finally OnChangeHot and the m_DropSource flags at +0x6c/+0x70.
//     } else {
//         m_iButtonCapture = iHit; 0x151e40(this, iHit);
//         if ((pButton->m_nStyle & 0x40000) && !IsKindOf(0x3b11d8)) { m_iButtonCapture = -1; return; }
//         pButton->m_nStyle |= 0x20000;  InvalidateButton; UpdateWindow;
//         ShowCommandMessageString(pButton->m_nID);               // vslot 0x840
//         BOOL bWasDropped = pButton->vslot 0xf0;
//         if (pButton->OnClick(this, FALSE)) {                    // vslot 0x50
//             (if the button is still in m_Buttons) pButton->m_nStyle &= ~0x20000;
//             m_iButtonCapture = m_iHighlighted = -1; OnChangeHot(-1);
//             InvalidateButton; UpdateWindow;
//             if (dropped state changed) NotifyWinEvent(EVENT_OBJECT_STATECHANGE / EVENT_OBJECT_FOCUS ...)
//         } else m_pWndLastCapture = CWnd::FromHandle(::SetCapture(m_hWnd));
//     }
//     if (m_bAltCustomizeMode) { m_bAltCustomizeMode = FALSE; m_pSelToolbar = NULL;
//                                0x1527e0(0); RedrawWindow(0x505); }
// Left a stub: every live branch ends in a CMFCToolBarButton virtual
// (OnClick, PrepareDrag, CanBeStretched, IsDroppedDown ...) that OpenMFC
// cannot dispatch (no button vtable), or in COleDataSource::DoDragDrop; a
// half transcription would swallow the click without ever firing the command.
// Symbol: ?OnLButtonDown@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(
    CMFCToolBar* pThis, unsigned int nFlags, CPoint point) {
    (void)pThis; (void)nFlags; (void)point;
    // TODO(clean-room): not transcribed -- see the decoding above.
}

// Retail (RVA 0x151260, mfc140u; the WM_LBUTTONUP entry of the message map)
// is ~200 instructions.  Decoded structure, NOT implemented: it releases the
// capture (m_pWndLastCapture), calls HitTest (vslot 0x730) up to three times,
// finishes a stretch drag (m_bStretchButton: SetButtonStyle vslot 0x6f8,
// AdjustLayout 0x428, vslot 0x5b8) or a customize drag, and otherwise
// clears the 0x20000 pressed bit on m_iButtonCapture's button, invalidates
// it (0x14fe50) and dispatches pButton->OnClickUp() (vslot 0x58) followed by
// the command through 0x183ee0 / vslot 0x7e0 (NotifyControlCommand) and
// CPane::OnLButtonUp (0x9fa80); the help-mode hook (0x3be360),
// m_bCustomizeMode (0x3be35c) and m_UsageCount (0x3b1fd0) are consulted.
// Left a stub for the same reason as OnLButtonDown: the button virtuals
// (OnClickUp, IsDroppedDown, CanBeStretched) cannot be dispatched.
// Symbol: ?OnLButtonUp@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(
    CMFCToolBar* pThis, unsigned int nFlags, CPoint point) {
    (void)pThis; (void)nFlags; (void)point;
    // TODO(clean-room): not transcribed -- see the decoding above.
}

// Retail (RVA 0x1567a0, mfc140u; the WM_MOUSELEAVE entry of the message
// map), fully transcribed:
//     if (m_hookMouseHelp != NULL) return;                        // 0x3be360
//     if (m_bMenuMode && !m_bCustomizeMode && GetDroppedDownMenu(NULL)) return;  // 0x157f30
//     m_bTracked = FALSE;  m_ptLastMouse = CPoint(-1, -1);        // 0x10e8, 0x12d8
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//     BOOL bIsFocus = (pFocus == this);
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl))) {  // 0x312be0
//         bIsFocus = (pParent == pFocus);
//         if (!bIsFocus) bIsFocus = (CWnd::FromHandle(::GetParent(pParent->m_hWnd)) == pFocus);
//     }
//     if (m_iHighlighted < 0) {                                    // 0x1138
//         if (<tracked toolbar, 0x3c3728> == this) ShowCommandMessageString(-1);  // vslot 0x840
//         return;                    // NB: falls to the end either way
//     }
//     if (bIsFocus) return;
//     if (vslot 0x7f8 (this)) return;   // slot 255 of 0x1803157c8 -> 0x71e0, `xor eax,eax`
//     int iPrev = m_iHighlighted; m_iHighlighted = -1; OnChangeHot(-1);   // vslot 0x770
//     CMFCToolBarButton* p = InvalidateButton(iPrev);              // 0x14fe50
//     ::UpdateWindow(m_hWnd);
//     if (p && p->vslot 0xf0 (p)) return;    // button slot 30, IsDroppedDown
//     if (<tracked toolbar> != this) return;
//     HWND hOwner = m_hWndOwner /*+0xa0*/ ? m_hWndOwner : ::GetParent(m_hWnd);   // GetOwner()
//     ::SendMessage(CWnd::FromHandle(hOwner)->m_hWnd, WM_SETMESSAGESTRING /*0x362*/,
//                   AFX_IDS_IDLEMESSAGE /*0xe001*/, 0);                       // 0x1802c7120
// Deviations: vslot 0x7f8 is a `return FALSE` in CMFCToolBar and is assumed
// so (a derived override is not honoured); the button's IsDroppedDown
// (vslot 0xf0) cannot be dispatched -- slot 30 of the button vftable at
// 0x316048 is 0x71e0, `xor eax,eax`, so the base FALSE is assumed and a
// dropped-down menu button (derived override) would still get the idle
// message; the tracked-toolbar static is never set because OnMouseMove is a
// stub, so the two branches that compare against it are dead today; and the
// GetOwner() lookup takes the ::GetParent branch unconditionally (same
// deviation as Deactivate).
// Symbol: ?OnMouseLeave@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis) {
    if (!pThis) return;
    if (impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA != nullptr) return;
    if (pThis->m_bMenuMode && impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 &&
        impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pThis, nullptr) != nullptr) {
        return;
    }

    pThis->m_bTracked = FALSE;
    pThis->m_ptLastMouse = CPoint(-1, -1);

    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    bool bIsFocus = (pFocus == pThis);
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                       pParent, impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ())) {
        bIsFocus = (pParent == pFocus);
        if (!bIsFocus) {
            CWnd* pGrand = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pParent->m_hWnd));
            bIsFocus = (pGrand == pFocus);
        }
    }

    if (pThis->m_iHighlighted < 0) {
        if (g_pTrackedToolBar == pThis) {
            impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(pThis, static_cast<unsigned int>(-1));
        }
        return;
    }
    if (bIsFocus) return;
    // TODO(clean-room): vslot 0x7f8 (a `return FALSE` in CMFCToolBar) is not
    // dispatched.

    const int iPrevHighlighted = pThis->m_iHighlighted;
    pThis->m_iHighlighted = -1;
    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);
    CMFCToolBarButton* pButton =
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iPrevHighlighted);
    ::UpdateWindow(pThis->m_hWnd);
    (void)pButton;   // TODO(clean-room): pButton->IsDroppedDown() (vslot 0xf0) is not dispatched.

    if (g_pTrackedToolBar != pThis) return;
    // CWnd::GetOwner() inlined: m_hWndOwner (+0xa0) if set, else the parent.
    // TODO(clean-room): as in Deactivate above, OpenMFC's CWnd does not name
    // m_hWndOwner (anonymous padding), so the ::GetParent branch is taken
    // unconditionally.
    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pOwner) {
        ::SendMessage(pOwner->m_hWnd, 0x362 /*WM_SETMESSAGESTRING*/, 0xE001 /*AFX_IDS_IDLEMESSAGE*/, 0);
    }
}

// Retail (RVA 0x150ce0, mfc140u; the WM_MOUSEMOVE entry of the message map)
// is ~330 instructions.  Decoded structure, NOT implemented: it bails while
// the help-mode hook (0x3be360) is set, forwards to the dropped-down menu
// (0x157f30) in menu mode, arms TrackMouseEvent (0x1802c7310) the first time
// (setting m_bTracked and the tracked-toolbar static at 0x3c3728), runs the
// customize-mode stretch drag (m_bStretchButton: SetButtonStyle vslot 0x6f8,
// AdjustLayout, CClientDC 0x2a3b20 + DrawDragCursor), otherwise hit-tests
// (vslot 0x730 x3) to move m_iHighlighted / m_iAccHotItem, calls OnChangeHot
// (vslot 0x770), ShowCommandMessageString (vslot 0x840), the button's
// vslot 0x78 / 0x7e8, SetTimer(0xec07, 500ms) for the accessibility hot item,
// afxGlobalData+0x278 and +0x158, and finally CPane::OnMouseMove (0x9fce0).
// Left a stub: the customize drag and the per-button virtuals cannot be
// modelled, and the hot-tracking half alone would leave m_iHighlighted
// moving without the visual feedback that DrawButton (a stub) provides.
// Symbol: ?OnMouseMove@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCToolBar__IEAAXIVCPoint___Z(
    CMFCToolBar* pThis, unsigned int nFlags, CPoint point) {
    (void)pThis; (void)nFlags; (void)point;
    // TODO(clean-room): not transcribed -- see the decoding above.
}

// Retail (RVA 0x156fd0, mfc140u; the WM_NCCALCSIZE entry of the message map)
// is a tail jump: `m_Impl.CalcNcSize(lpncsp)` with rcx = this+0x1120 (m_Impl)
// and rdx = lpncsp, to ?CalcNcSize@CMFCControlBarImpl@@IEAAXPEAUtagNCCALCSIZE_PARAMS@@@Z
// (0x31910).  bCalcValidRects is not consulted.
// Symbol: ?OnNcCalcSize@CMFCToolBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCToolBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CMFCToolBar* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (!pThis) return;
    impl__CalcNcSize_CMFCControlBarImpl__IEAAXPEAUtagNCCALCSIZE_PARAMS___Z(&pThis->m_Impl, lpncsp);
}

// Retail: the WM_NCHITTEST entry of the message map (0x315270, mfc140u)
// points at 0x3a60, `mov eax,1; ret` -- HTCLIENT, shared by COMDAT folding
// with OnEraseBkgnd above.
// Symbol: ?OnNcHitTest@CMFCToolBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCToolBar__IEAA_JVCPoint___Z(CMFCToolBar* pThis, CPoint point) {
    (void)pThis; (void)point;
    return HTCLIENT;
}

// Retail (RVA 0x156fe0, mfc140u; the WM_NCPAINT entry of the message map) is
// a tail jump: `m_Impl.DrawNcArea()` with rcx = this+0x1120, to
// ?DrawNcArea@CMFCControlBarImpl@@IEAAXXZ (0x31620).
// Symbol: ?OnNcPaint@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis) {
    if (!pThis) return;
    impl__DrawNcArea_CMFCControlBarImpl__IEAAXXZ(&pThis->m_Impl);
}

// Retail (RVA 0x159ec0, mfc140u; the ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0,
// 0xffff) entry of the message map), fully transcribed:
//     if (!m_pToolTip || !m_pToolTip->m_hWnd ||                    // 0x12e8, +0x40
//         pNMH->hwndFrom != m_pToolTip->m_hWnd) return FALSE;
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     TOOLINFOW ti; memset(&ti, 0, 0x48); ti.cbSize = 0x40;   // TTTOOLINFOW_V2_SIZE
//     if (OnToolHitTest(pt, &ti) < 0) return FALSE;              // vslot 0xf8
//     LPWSTR psz = ti.lpszText;
//     if (psz == NULL || psz == LPSTR_TEXTCALLBACK) return FALSE;
//     static CString strTipText;                                 // 0x3c3298
//     strTipText.SetString(psz, psz ? wcslen(psz) : 0);          // 0x2e30
//     free(psz);                                                 // 0x1802c74e8
//     ((NMTTDISPINFOW*)pNMH)->lpszText = (LPWSTR)(LPCWSTR)strTipText;   // +0x18
//     if (!afxGlobalData.<+0>) { Initialize(); <+0> = 1; }
//     ::SendMessage(m_pToolTip->m_hWnd, WM_SETFONT, (WPARAM)afxGlobalData.<+0x1b0>, 0);
//     return TRUE;
// (pResult is never written.)  Deviations: OnToolHitTest is this file's
// export rather than vslot 0xf8 -- and it never fills lpszText, so today the
// function returns FALSE at the NULL test; the font slot is skipped when it
// reads NULL (see the note on afxGlobalData at the top).
// Symbol: ?OnNeedTipText@CMFCToolBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCToolBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMFCToolBar* pThis, unsigned int id, NMHDR* pNMH, __int64* pResult) {
    (void)id; (void)pResult;
    if (!pThis || !pNMH) return FALSE;
    CWnd* pToolTip = reinterpret_cast<CWnd*>(pThis->m_pToolTip);
    if (!pToolTip || !pToolTip->m_hWnd || pNMH->hwndFrom != pToolTip->m_hWnd) return FALSE;

    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(pThis->m_hWnd, &pt);

    TOOLINFOW ti;
    std::memset(&ti, 0, sizeof ti);
    ti.cbSize = 0x40;
    if (impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
            pThis, CPoint(pt.x, pt.y), &ti) < 0) {
        return FALSE;
    }
    wchar_t* pszText = ti.lpszText;
    if (pszText == nullptr || pszText == LPSTR_TEXTCALLBACKW) return FALSE;

    static CString strTipText;
    strTipText = pszText;
    std::free(pszText);
    reinterpret_cast<NMTTDISPINFOW*>(pNMH)->lpszText = const_cast<wchar_t*>(strTipText.GetString());

    EnsureGlobalDataInitialized();
    if (void* hFont = GlobalDataPtr(kGlobalDataFontTooltip)) {
        ::SendMessage(pToolTip->m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 0);
    }
    return TRUE;
}

// Retail (RVA 0x15ad80, mfc140u; the first ON_REGISTERED_MESSAGE entry of the
// message map, whose UINT lives at 0x3c1180 -- the other, 0x3c1184 ->
// 0x15b930, is OnUpdateToolTips), fully transcribed:
//     CString strName; GetWindowText(strName);                    // 0x28be00
//     strName.TrimLeft(); strName.TrimRight();                    // 0x127f0 / 0x12750
//     if (strName.IsEmpty() &&
//         !strName.LoadString(AfxFindStringResourceHandle(0x3ee8), 0x3ee8))  // 0x2aee00, 0xdb70
//         AfxThrowInvalidArgException();
//     CString strPrompt; strPrompt.Format(0x3eef, (LPCTSTR)strName);   // 0x66db0 = CStringT::Format(UINT, ...)
//     if (AfxMessageBox(strPrompt, MB_OKCANCEL | MB_ICONWARNING /*0x31*/, 0) == IDOK)  // 0x1cec10
//         RestoreOriginalState();                                  // vslot 0x720
//     return 0;
// (0x127f0 / 0x12750 are the two iswspace-driven CStringT trims; 0x3ee8 /
// 0x3eef are string ids in the AFXBARRES range.)
// Deviations: both string loads go through the AfxLoadString export into a
// 256-char buffer (OpenMFC's CString has no Format(UINT, ...)); the throw is
// an early return; RestoreOriginalState is this file's export (a stub today)
// rather than vslot 0x720.
// Symbol: ?OnPromptReset@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPromptReset_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam; (void)lParam;
    if (!pThis) return 0;

    CString strName;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pThis, &strName);
    strName.TrimLeft();
    strName.TrimRight();
    if (strName.IsEmpty()) {
        wchar_t szUntitled[256] = { 0 };
        if (!impl__AfxLoadString__YAHIPEA_WI_Z(0x3ee8, szUntitled, 256)) {
            return 0;   // retail throws here
        }
        strName = szUntitled;
    }

    wchar_t szFormat[256] = { 0 };
    if (!impl__AfxLoadString__YAHIPEA_WI_Z(0x3eef, szFormat, 256)) {
        return 0;   // CStringT::Format(UINT) asserts on a missing resource
    }
    CString strPrompt;
    strPrompt.Format(szFormat, strName.GetString());
    if (impl__AfxMessageBox__YAHPEB_WII_Z(strPrompt.GetString(), MB_OKCANCEL | MB_ICONWARNING, 0) == IDOK) {
        impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(pThis);
    }
    return 0;
}

// Retail (RVA 0x15b660, mfc140u; the WM_RBUTTONDOWN entry of the message
// map), fully transcribed:
//     CWnd::Default();                                            // 0x28ac80
//     if (CMFCToolBar::m_bCustomizeMode) return;                  // 0x3be35c
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));      // 0x28ad70
//     if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CPane))) {   // 0x2f5918
//         pParent = CWnd::FromHandle(::GetParent(m_hWnd));        // re-fetched
//         pParent->SetFocus();                                    // 0x2a9b60
//     }
// 0x2a9b60 is not in the RVA map but its body is CWnd::SetFocus exactly:
//     if (m_pCtrlSite /*+0xd0*/) return m_pCtrlSite->vslot 0x170 ();
//     CWnd* p = GetParent(); if (p && p->m_pCtrlCont /*+0xc8*/) p->m_pCtrlCont-><+0xd8> = NULL;
//     return CWnd::FromHandle(::SetFocus(m_hWnd));               // 0x1802c6da8 = USER32!SetFocus
// (OnLButtonDown calls the same routine on a CPane parent.)  It is called
// here through the exported ?SetFocus@CWnd@@QEAAPEAV1@XZ thunk
// (core/window/Thunks.cpp, which runs OpenMFC's C++ CWnd::SetFocus).  nFlags
// and point are not read by retail.
// Symbol: ?OnRButtonDown@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(
    CMFCToolBar* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags; (void)point;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                       pParent, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ())) {
        pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
        if (pParent) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pParent);
    }
}

// Retail (RVA 0x15a190, mfc140u; slot 151 of the vtable at 0x1803157c8),
// fully transcribed:
//     m_AccData.Clear();       // this+0x150: zero +0x30..+0x53, then Empty() on the
//                              // CStrings at +0, +8, +0x10, +0x18, +0x20, +0 (again), +0x28
//     CMFCToolBarButton* p = AccGetButtonByChildId(lVal);        // 0x15ba90
//     if (!p) return FALSE;
//     p->SetACCData(this, m_AccData);                            // vslot 0x180
//     return TRUE;
// m_AccData is the CAccessibilityData at CBasePane+0x150 (afxmfc.h).
// Deviation: SetACCData is not dispatched; the exported base thunk is called
// instead, and that thunk is an argument-ignoring stub today, so the cleared
// block stays empty.
// Symbol: ?OnSetAccData@CMFCToolBar@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCToolBar__UEAAHJ_Z(CMFCToolBar* pThis, long lVal) {
    if (!pThis) return FALSE;

    CAccessibilityData& acc = pThis->m_AccData;
    acc.m_nAccRole = 0;
    acc.m_bAccState = 0;
    acc.m_nAccHit = 0;
    acc.m_rectAccLocation.left = acc.m_rectAccLocation.top = 0;
    acc.m_rectAccLocation.right = acc.m_rectAccLocation.bottom = 0;
    acc.m_ptAccHit.x = acc.m_ptAccHit.y = 0;
    acc.m_strAccName.Empty();
    acc.m_strAccValue.Empty();
    acc.m_strDescription.Empty();
    acc.m_strAccKeys.Empty();
    acc.m_strAccHelp.Empty();
    acc.m_strAccName.Empty();
    acc.m_strAccDefAction.Empty();

    CMFCToolBarButton* pButton =
        impl__AccGetButtonByChildId_CMFCToolBar__IEAAPEAVCMFCToolBarButton__J_Z(pThis, lVal);
    if (!pButton) return FALSE;
    // TODO(clean-room): vslot 0x180 (SetACCData) is not dispatched; the base
    // export below is a stub.
    impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(pButton, pThis, &acc);
    return TRUE;
}

// Retail (RVA 0x1547b0, mfc140u; the WM_SETCURSOR entry of the message map),
// fully transcribed:
//     CPoint pt(0, 0); ::GetCursorPos(&pt);                       // 0x1802c7320
//     if (m_bStretchButton) {                                     // 0x10e4
//         gate afxGlobalData; ::SetCursor(afxGlobalData.<+0x158>); return TRUE;
//     }
//     if (m_bCustomizeMode && m_iSelected != -1 && !m_bLocked) {  // 0x3be35c, 0x113c, 0x10b8
//         ::ScreenToClient(m_hWnd, &pt);           // NB: converts pt in place
//         if (HitTest(pt) == m_iSelected) {                        // vslot 0x730
//             p = GetButton(m_iSelected);                          // 0x14fe00
//             if (p->CanBeStretched() &&                           // vslot 0x88
//                 labs(pt.x - p->m_rect.right /*+0x70*/) <= 6) {   // 0x1802c7860
//                 gate afxGlobalData; ::SetCursor(<+0x158>); return TRUE;
//             }
//         }
//     }
//     CPoint pt2 = pt; ::ScreenToClient(m_hWnd, &pt2);   // a second conversion
//                                        // on the customize path above -- retail
//     CRect rectGripper(0,0,0,0);        // quirk, reproduced
//     m_Impl.GetGripperRect(rectGripper, TRUE);                    // 0x31a40, this+0x1120
//     if (::PtInRect(&rectGripper, pt2) && CMFCPopupMenu::m_pActivePopupMenu == NULL) {  // 0x3be288
//         gate afxGlobalData; ::SetCursor(afxGlobalData.<+0x170>); return TRUE;
//     }
//     return (BOOL)CWnd::Default();                                // 0x28ac80
// pWnd / nHitTest / message are not read.  Deviations: CanBeStretched is not
// dispatched -- slot 17 of the button vftable at 0x316048 is 0x71e0,
// `xor eax,eax`, so the base FALSE is assumed and the stretch-handle test is
// skipped (a derived button that can be stretched does not get the stretch
// cursor here; the customize path keeps its ScreenToClient/HitTest side
// effects); the two cursor
// slots read NULL until AFX_GLOBAL_DATA::Initialize is real, and a NULL is
// not handed to ::SetCursor (that would hide the cursor) -- the function
// falls through to Default() instead.
// Symbol: ?OnSetCursor@CMFCToolBar@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCToolBar__IEAAHPEAVCWnd__II_Z(
    CMFCToolBar* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd; (void)nHitTest; (void)message;
    if (!pThis) return FALSE;

    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);

    if (pThis->m_bStretchButton) {
        EnsureGlobalDataInitialized();
        if (void* hCur = GlobalDataPtr(kGlobalDataCursorStretch)) {
            ::SetCursor(static_cast<HCURSOR>(hCur));
            return TRUE;
        }
        return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    }

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && pThis->m_iSelected != -1 && !pThis->m_bLocked) {
        ::ScreenToClient(pThis->m_hWnd, &pt);
        const int iSel = pThis->m_iSelected;
        if (impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, CPoint(pt.x, pt.y)) == iSel) {
            CMFCToolBarButton* pButton = pThis->GetButton(iSel);
            (void)pButton;
            // TODO(clean-room): pButton->CanBeStretched() (vslot 0x88) is not
            // dispatched; the base body returns FALSE, so the
            // `labs(pt.x - m_rect.right) <= 6` stretch cursor is never
            // selected here.
        }
    }

    POINT pt2 = pt;
    ::ScreenToClient(pThis->m_hWnd, &pt2);
    RECT rectGripper = { 0, 0, 0, 0 };
    impl__GetGripperRect_CMFCControlBarImpl__QEAAXAEAVCRect__H_Z(&pThis->m_Impl, &rectGripper, TRUE);
    if (::PtInRect(&rectGripper, pt2) && impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == nullptr) {
        EnsureGlobalDataInitialized();
        if (void* hCur = GlobalDataPtr(kGlobalDataCursorSizeAll)) {
            ::SetCursor(static_cast<HCURSOR>(hCur));
            return TRUE;
        }
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// Retail (RVA 0x1572f0, mfc140u; slot 234 of the vtable at 0x1803157c8),
// fully transcribed:
//     UINT id = pButton->m_nID;                                    // +0x24
//     if (id - 1 > 0xFFFFFFFD) return FALSE;    // id == 0 || id == (UINT)-1
//     CString strText;  WCHAR szFullText[256];
//     BOOL bRet = TRUE;
//     if (AfxLoadString(id, szFullText, 256) &&                    // 0x2af0b0
//         AfxExtractSubString(strText, szFullText, 1, L'\n'))      // 0x2ae130
//         pButton->m_strText = strText;                            // +0x38, 0xde30
//     else bRet = FALSE;
//     return bRet;
// i.e. the tooltip half ("prompt\ntip") of the command's string resource
// becomes the button label.
// Symbol: ?OnSetDefaultButtonText@CMFCToolBar@@UEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton) {
    (void)pThis;
    if (!pButton) return FALSE;
    const unsigned int nID = pButton->m_nID;
    if (nID - 1u > 0xFFFFFFFDu) return FALSE;

    wchar_t szFullText[256] = { 0 };
    CString strText;
    if (impl__AfxLoadString__YAHIPEA_WI_Z(nID, szFullText, 256) &&
        impl__AfxExtractSubString__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WH_W_Z(
            &strText, szFullText, 1, L'\n')) {
        pButton->m_strText = strText;
        return TRUE;
    }
    return FALSE;
}

// Retail (RVA 0x159280, mfc140u), fully transcribed -- eight calls to
// ?Clear@CMFCToolBarImages@@QEAAXXZ (0x16f690) on the class-static wells, in
// this order, then the image-size static reset:
//     m_Images.Clear();  m_ColdImages.Clear();  m_DisabledImages.Clear();
//     m_MenuImages.Clear();  m_DisabledMenuImages.Clear();
//     m_LargeImages.Clear();  m_LargeColdImages.Clear();  m_LargeDisabledImages.Clear();
//     m_sizeImage = CSize(16, 15);          // two DWORD stores at 0x3b1cb8 / 0x3b1cbc
// The per-toolbar *Locked wells are not touched.
// Symbol: ?ResetAllImages@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__ResetAllImages_CMFCToolBar__SAXXZ()
{
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__Clear_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__m_sizeImage_CMFCToolBar__1VCSize__A.cx = 16;
    impl__m_sizeImage_CMFCToolBar__1VCSize__A.cy = 15;
}

// Retail (RVA 0x159300, mfc140u), fully transcribed:
//     if (m_bLocked) return;                                        // +0x10b8
//     for (node = m_Buttons.head /*+0x1190*/; node; node = node->next)
//         node->data->ResetImageToDefault();                        // button vslot 0x130 (slot 38)
//     if (IsFloating()) AdjustLayout();                             // vslot 0x2f0 (`mov 0x10d8(%rcx),%eax`), vslot 0x428
// Deviations: the buttons come from the mfccore.cpp side table (GetCount /
// GetButton), and ResetImageToDefault is reached through the exported base
// thunk instead of vslot 0x130, so CMFCToolBarMenuButton's override (RVA
// 0x176000, slot 38 of its vftable 0x3184e8) is not dispatched.  More
// importantly, that exported thunk is an EMPTY body in
// featurepack/toolbar/CMFCToolBarButton.cpp, whose comment calls the retail
// base "a no-op (harvested)".  It is not: ?ResetImageToDefault@CMFCToolBarButton@@
// (RVA 0x15e3b0, mfc140u) does, for a non-user button with m_nID > 0 that is
// not a user tool, `if (m_DefaultImages.Lookup(m_nID, iImage) && iImage >= 0)
// SetImage(iImage) /*vslot 0x190*/; else if (m_bImage) { m_bText = TRUE;
// m_bImage = FALSE; if (m_strText.IsEmpty()) m_strText = <resource string for
// m_nID, cut at '\n'> }` (0x2f610 is the CMap<UINT,UINT,int,int>::Lookup on
// 0x3b1f90, 0x2aee00 AfxFindStringResourceHandle).  A probe on a default
// button (m_nID == 0) observes nothing, which is how the earlier "no-op"
// claim arose.  So today this loop resets nothing per button; only the
// AdjustLayout tail is effective.  (headerRequest filed for
// CMFCToolBarButton.cpp.)
// Symbol: ?ResetImages@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImages_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (pThis->m_bLocked) return;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(pButton);   // TODO(clean-room): vslot 0x130 not dispatched
    }

    if (pThis->m_bFloating) {
        pThis->AdjustLayout();
    }
}

// Retail (RVA 0x158720, mfc140u), fully transcribed:
//     if (::IsWindow(m_hwndLastFocus)) ::SetFocus(m_hwndLastFocus);   // +0x1310; IAT 0x2c7138 / 0x2c6da8
//     m_hwndLastFocus = NULL;
//     <the afxGlobalData one-time Initialize gate, repeated before each read>
//     if (afxGlobalData.bUnderlineKeyboardShortcuts        /* +0x27c, 0x3c189c */ &&
//         !afxGlobalData.bSysUnderlineKeyboardShortcuts    /* +0x280, 0x3c18a0 */ &&
//         !m_bCustomizeMode) {                             // 0x3be35c
//         afxGlobalData.bUnderlineKeyboardShortcuts = FALSE;
//         RedrawUnderlines();                              // 0x15b970
//     }
// The two afxGlobalData slots are the ones named near the top of this file
// (kGlobalDataUnderlineShortcuts / kGlobalDataSysUnderlineShortcuts, with the
// OnSettingChange evidence for the names).  AFX_GLOBAL_DATA::Initialize is an
// empty stub in OpenMFC, so both read back 0 today and the tail is never taken.
// Symbol: ?RestoreFocus@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreFocus_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (::IsWindow(pThis->m_hwndLastFocus)) {
        ::SetFocus(pThis->m_hwndLastFocus);
    }
    pThis->m_hwndLastFocus = nullptr;

    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalDataUnderlineShortcuts) != 0 &&
        GlobalDataInt(kGlobalDataSysUnderlineShortcuts) == 0 &&
        impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        GlobalDataSetInt(kGlobalDataUnderlineShortcuts, 0);
        impl__RedrawUnderlines_CMFCToolBar__SAXXZ();
    }
}

// Retail (RVA 0x156520, mfc140u), fully transcribed:
//     if (m_uiOriginalResID == 0) return FALSE;                              // +0x1308
//     BOOL bRet = LoadToolBar(m_uiOriginalResID, 0, 0, FALSE, 0, 0, 0);       // vslot 0x670
//     AdjustLayout();                                                        // vslot 0x428
//     if (IsFloating()) {                                                    // vslot 0x2f0
//         RecalcLayout();                                                    // vslot 0x430 (CPane)
//     } else if (m_pParentDockBar != NULL) {                                 // +0x128
//         CSize size = CalcFixedLayout(FALSE, IsHorizontal());               // vslot 0x4d0; vslot 0x2d8 is
//                                                                            //   RVA 0x8860 = GetCurrentAlignment() & 0xa000
//         CRect rect;  ::GetWindowRect(m_hWnd, &rect);
//         if (rect.Width() != size.cx || rect.Height() != size.cy) {
//             SetWindowPos(NULL, 0, 0, size.cx, size.cy,
//                          SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /*0x16*/, NULL);   // vslot 0x480 (CBasePane)
//             UpdateVirtualRect();                                           // 0xa1520
//         }
//         m_pDockBarRow->ArrangePanes(this);                                 // +0x130, row vslot 0x60
//         AFXGetParentFrame(this)->RecalcLayout(TRUE);                       // 0x6bd00, frame vslot 0x300
//     }
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x505);
//     return bRet;
// Deviations: (1) OpenMFC's CalcLayout (which CalcFixedLayout forwards to) is
// still a stub returning (0, 0), so a faithful SetWindowPos would collapse the
// window; the resize + UpdateVirtualRect pair is therefore applied only when
// the computed size is non-empty, which is the only case retail can produce
// for a real toolbar.  (2) LoadToolBar / AdjustLayout are the C++ bodies in
// this file, RecalcLayout / ArrangePanes / SetWindowPos the exported thunks,
// so derived overrides are not re-entered -- and CMFCToolBar::LoadToolBar
// here is `{ return TRUE; }` (see the member definitions below), so NOTHING
// is reloaded from m_uiOriginalResID: the button set is left as it is, bRet
// is always TRUE, and only the relayout / redraw tail is effective.  (3) Retail
// does not NULL-check the dock row or the parent frame; both are guarded here.
// Symbol: ?RestoreOriginalState@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;
    if (pThis->m_uiOriginalResID == 0) return FALSE;

    const int bRet = pThis->LoadToolBar(pThis->m_uiOriginalResID, 0, 0, FALSE, 0, 0, 0) ? TRUE : FALSE;
    pThis->AdjustLayout();

    if (pThis->m_bFloating) {
        impl__RecalcLayout_CPane__UEAAXXZ(pThis);
    } else if (pThis->m_pParentDockBar != nullptr) {
        const int bHorz = (pThis->m_dwStyle & 0xa000) != 0 ? TRUE : FALSE;
        CSize size(0, 0);
        impl__CalcFixedLayout_CMFCToolBar__UEAA_AVCSize__HH_Z(pThis, &size, FALSE, bHorz);

        RECT rect = { 0, 0, 0, 0 };
        ::GetWindowRect(pThis->m_hWnd, &rect);
        const bool bSizeDiffers = (rect.right - rect.left) != size.cx || (rect.bottom - rect.top) != size.cy;
        if (bSizeDiffers && size.cx > 0 && size.cy > 0) {   // TODO(clean-room): the size gate is the deviation noted above
            impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
                pThis, nullptr, 0, 0, size.cx, size.cy,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
            impl__UpdateVirtualRect_CPane__QEAAXXZ(pThis);
        }

        if (pThis->m_pDockBarRow != nullptr) {
            impl__ArrangePanes_CDockingPanesRow__UEAAXPEAVCPane___Z(pThis->m_pDockBarRow, pThis);
        }
        CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pThis);
        if (pFrame) {
            impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrame, TRUE);
        }
    }

    ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, 0x505);
    return bRet;
}

// Retail (RVA 0x15a620, mfc140u), fully transcribed:
//     if (m_OrigButtons.GetCount() != 0)                    // +0x11c0, count at +0x11d8
//         store.Write(L"OriginalItems", m_OrigButtons);    // vslot 0x58 = Write(LPCTSTR, CObject&), literal 0x342c08
// The CObject& overload of OpenMFC's CSettingsStore records the object
// pointer in its in-memory map; nothing is serialised.
// Symbol: ?SaveOriginalState@CMFCToolBar@@MEAAXAEAVCSettingsStore@@@Z
extern "C" void MS_ABI impl__SaveOriginalState_CMFCToolBar__MEAAXAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    if (!pThis) return;
    if (pThis->m_OrigButtons.GetCount() != 0) {
        impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(
            &store, L"OriginalItems", &pThis->m_OrigButtons);
    }
}

// Retail (RVA 0x158050, mfc140u), fully transcribed:
//     CString strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);       // 0xd2070
//     CString strKey;  strKey.Format(L"%TsMFCToolBarParameters", strPath);    // 0xda00
//     CSettingsStoreSP sp;  CSettingsStore& reg = sp.Create(FALSE, FALSE);    // 0x12a550
//     BOOL bRet = FALSE;
//     if (reg.CreateKey(strKey))                                              // vslot 0x28
//         bRet = reg.Write(L"Tooltips",               m_bShowTooltips)        // vslot 0x80 = Write(LPCTSTR,int), 0x3b1b64
//             && reg.Write(L"ShortcutKeys",           m_bShowShortcutKeys)    // 0x3b1b60
//             && reg.Write(L"LargeIcons",             m_bLargeIcons)          // 0x3be368
//             && reg.Write(L"MenuAnimation",          CMFCPopupMenu::m_AnimationType)      // 0x3be290
//             && reg.Write(L"RecentlyUsedMenus",      CMFCMenuBar::m_bRecentlyUsedMenus)   // 0x3b1b08
//             && reg.Write(L"MenuShadows",            CMFCMenuBar::m_bMenuShadows)         // 0x3b1b00
//             && reg.Write(L"ShowAllMenusAfterDelay", CMFCMenuBar::m_bShowAllMenusDelay)   // 0x3b1b04
//             && reg.Write(L"CommandsUsage",          m_UsageCount);          // vslot 0x58 = Write(LPCTSTR,CObject&), 0x3b1fd0
//     return bRet;                       // then the owner's deleting-dtor release of the store
// This is the exact mirror of LoadParameters above (same key, same eight
// value names, literals 0x342b08..0x342be8).  Deviations: the CSettingsStoreSP
// owner is the hand-rolled SettingsStoreSP; "%Ts" is written as "%s" because
// OpenMFC's CString::Format has no %Ts (see ToolBarParametersKey); the store
// is OpenMFC's in-memory map, and its CObject& Write records only the pointer
// to m_UsageCount.
// Symbol: ?SaveParameters@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__SaveParameters_CMFCToolBar__SAHPEB_W_Z(const wchar_t* lpszProfileName)
{
    const CString strKey = ToolBarParametersKey(lpszProfileName);

    SettingsStoreSP sp;
    void* pStore = sp.Create(FALSE, FALSE);
    if (!pStore) return FALSE;

    int bRet = FALSE;
    if (impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pStore, strKey.GetString())) {
        bRet =
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"Tooltips", impl__m_bShowTooltips_CMFCToolBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"ShortcutKeys", impl__m_bShowShortcutKeys_CMFCToolBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"LargeIcons", impl__m_bLargeIcons_CMFCToolBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"MenuAnimation",
                                                       static_cast<int>(impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A)) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"RecentlyUsedMenus", impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"MenuShadows", impl__m_bMenuShadows_CMFCMenuBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WH_Z(pStore, L"ShowAllMenusAfterDelay", impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA) &&
            impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(
                pStore, L"CommandsUsage",
                reinterpret_cast<CObject*>(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A));
    }
    return bRet ? TRUE : FALSE;
}

// Retail (RVA 0x15aed0, mfc140u), fully transcribed -- the same shape as
// SaveOriginalState on the other list:
//     if (m_OrigResetButtons.GetCount() != 0)                  // +0x11f8, count at +0x1210
//         store.Write(L"OrigResetItems", m_OrigResetButtons);  // vslot 0x58, literal 0x342c28
// Symbol: ?SaveResetOriginalState@CMFCToolBar@@IEAAXAEAVCSettingsStore@@@Z
extern "C" void MS_ABI impl__SaveResetOriginalState_CMFCToolBar__IEAAXAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    if (!pThis) return;
    if (pThis->m_OrigResetButtons.GetCount() != 0) {
        impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(
            &store, L"OrigResetItems", &pThis->m_OrigResetButtons);
    }
}

// Retail (RVA 0x153510, mfc140u), fully transcribed:
//     CString strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);     // 0xd2070
//     if (nIndex == -1) nIndex = GetDlgCtrlID();                            // 0x2a99a0
//     CString strKey;
//     strKey.Format(uiID == (UINT)-1 ? L"%TsMFCToolBar-%d"                  // 0x342a78
//                                    : L"%TsMFCToolBar-%d%x", strPath, nIndex, uiID);   // 0x342aa0
//     {
//         CMemFile file(0x400);                                             // 0x22ae70
//         {
//             CArchive ar(&file, CArchive::store /*0*/, 0x1000, NULL);      // 0x1d1550
//             Serialize(ar);                                                // vslot 0x10
//             ar.Flush();                                                   // 0x1d1be0
//         }                                                                 // ~CArchive 0x1d1700
//         UINT uiDataSize = (UINT)file.GetLength();                         // inlined: m_nFileSize
//         LPBYTE lpbData = file.Detach();                                   // inlined: m_lpBuffer, then zero
//                                                                           //   m_lpBuffer/m_nFileSize/m_nBufferSize/m_nPosition
//         if (lpbData != NULL) {
//             CSettingsStoreSP sp;  CSettingsStore& reg = sp.Create(FALSE, FALSE);   // 0x12a550
//             if (reg.CreateKey(strKey)) {                                  // vslot 0x28
//                 if (::IsWindow(m_hWnd)) {
//                     CString strName;  GetWindowText(strName);             // 0x28be00
//                     reg.Write(L"Name", strName);                          // vslot 0x70 = Write(LPCTSTR,LPCTSTR), 0x33f4d8
//                 }
//                 BOOL bResult = reg.Write(L"Buttons", lpbData, uiDataSize);   // vslot 0x60 = Write(LPCTSTR,LPBYTE,UINT), 0x342ac8
//                 CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp());   // AfxGetModuleState()->m_pCurrentWinApp (+8), RTC 0x3207c8
//                 if (bResult && pApp && pApp->IsResourceSmartUpdate() /* +0x1bc */)
//                     SaveOriginalState(reg);                               // vslot 0x860
//                 SaveResetOriginalState(reg);                              // 0x15aed0 (direct call)
//             }
//             free(lpbData);                                                // IAT 0x2c74e8
//         }                                                                 // ~CMemFile 0x22afa0
//     }
//     return CPane::SaveState(lpszProfileName, nIndex, uiID);               // 0xa1f20
// Deviations: (1) CMFCToolBar::Serialize below is still a stub, so the
// CMemFile stays empty, Detach() hands back NULL and the "Name"/"Buttons"
// values are never written -- the flow is kept so it starts working the moment
// Serialize is filled in.  (2) Serialize is reached through this file's export
// thunk rather than vslot 0x10, so a derived override is not re-entered (the
// only one mfc140u exports is ?Serialize@CMFCColorBar@@MEAAXAEAVCArchive@@@Z;
// CMFCMenuBar has none).  (3) CWinAppEx::m_bResourceSmartUpdate (+0x1bc) is not a member
// of OpenMFC's CWinAppEx (anonymous padding; its ctor argument is discarded --
// see the same note in featurepack/menu/CMFCMenuBar.cpp), so the
// IsResourceSmartUpdate gate is taken as FALSE and SaveOriginalState is not
// called; headerRequest filed.  (4) "%Ts" is written as "%s", the
// CSettingsStoreSP owner is the hand-rolled SettingsStoreSP, and the CMemFile /
// CArchive objects are built in place through their exported ctor/dtor thunks
// because the C++ classes' members are not linkable from this unit.
// Symbol: ?SaveState@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID)
{
    if (!pThis) return FALSE;

    CString strPath;
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        &strPath, L"MFCToolBars", lpszProfileName);
    if (nIndex == -1) {
        nIndex = impl__GetDlgCtrlID_CWnd__QEBAHXZ(pThis);
    }
    CString strKey;
    if (uiID == static_cast<unsigned int>(-1)) {
        strKey.Format(L"%sMFCToolBar-%d", strPath.GetString(), nIndex);
    } else {
        strKey.Format(L"%sMFCToolBar-%d%x", strPath.GetString(), nIndex, uiID);
    }

    {
        alignas(16) unsigned char fileStorage[sizeof(CMemFile)];
        CMemFile* pFile = static_cast<CMemFile*>(impl___0CMemFile__QEAA_I_Z(fileStorage, 0x400));
        {
            alignas(16) unsigned char arStorage[sizeof(CArchive)];
            CArchive* pAr = static_cast<CArchive*>(impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
                arStorage, pFile, 0 /* CArchive::store */, 0x1000, nullptr));
            impl__Serialize_CMFCToolBar__UEAAXAEAVCArchive___Z(pThis, pAr);   // TODO(clean-room): vslot 0x10 not dispatched
            impl__Flush_CArchive__QEAAXXZ(pAr);
            impl___1CArchive__QEAA_XZ(pAr);
        }
        const unsigned int uiDataSize = static_cast<unsigned int>(impl__GetLength_CMemFile__UEBA_KXZ(pFile));
        unsigned char* lpbData = impl__Detach_CMemFile__QEAAPEAEXZ(pFile);
        if (lpbData != nullptr) {
            SettingsStoreSP sp;
            void* pStore = sp.Create(FALSE, FALSE);
            if (pStore && impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pStore, strKey.GetString())) {
                if (::IsWindow(pThis->m_hWnd)) {
                    CString strName;
                    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                        pThis, &strName);
                    impl__Write_CSettingsStore__UEAAHPEB_W0_Z(pStore, L"Name", strName.GetString());
                }
                const int bResult = impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(pStore, L"Buttons", lpbData, uiDataSize);
                // TODO(clean-room): `bResult && pApp->IsResourceSmartUpdate()` -- the
                // CWinAppEx member at +0x1bc is not modelled (deviation 3 above), so
                // the retail SaveOriginalState(reg) call is not made.
                (void)bResult;
                impl__SaveResetOriginalState_CMFCToolBar__IEAAXAEAVCSettingsStore___Z(
                    pThis, *reinterpret_cast<CSettingsStore*>(pStore));
            }
            std::free(lpbData);
        }
        impl___1CMemFile__UEAA_XZ(pFile);
    }

    return impl__SaveState_CPane__UEAAHPEB_WHI_Z(pThis, lpszProfileName, nIndex, uiID);
}

// Retail (RVA 0x153070, mfc140u), decoded but NOT implemented:
//     CBasePane::Serialize(ar);                                              // 0xc2e0
//     if (m_bLocked) return;                                                 // +0x10b8
//     if (ar.IsLoading()) {                                                  // ar.m_nMode & 1 (+0x20)
//         CMFCToolBarButton* pCustomize = NULL;
//         if (m_pCustomizeBtn) {                                             // +0x1300
//             pCustomize = m_pCustomizeBtn->GetRuntimeClass()->CreateObject();   // vslot 0, 0x234d60
//             if (!pCustomize || !pCustomize->IsKindOf(RUNTIME_CLASS(CMFCCustomizeButton))) pCustomize = NULL;   // 0x3b1148
//             pCustomize->CopyFrom(*m_pCustomizeBtn);                        // button vslot 0x38 (no NULL check in retail)
//         }
//         RemoveAllButtons();                                                // vslot 0x6b0
//         m_Buttons.Serialize(ar);                                           // CObList vslot 0x10 -> CArchive object I/O
//         for (node = m_Buttons.head; node; node = next) {
//             if (node->data == NULL) {                                      // an unresolvable element: give up
//                 m_Buttons.RemoveAll();                                     // 0x8350
//                 if (CanBeRestored()) RestoreOriginalState();               // vslots 0x718 / 0x720
//                 AdjustLocations();                                         // vslot 0x7d8
//                 return;
//             }
//             node->data->m_nStyle &= ~(TBBS_PRESSED | TBBS_CHECKED);       // +0x28 &= 0xfffcffff
//             node->data->OnChangeParentWnd(this);                           // button vslot 0x60
//         }
//         ar >> m_bTextLabels (via FillBuffer/+0x38 buffer read);           // gated by vslot 0x700 (0x3a60 -> TRUE)
//         if (pCustomize) { m_iSelected = InsertButton(pCustomize, -1);  m_pCustomizeBtn = pCustomize; }   // vslot 0x690
//         AdjustLocations();                                                 // vslot 0x7d8
//         ar >> strName (0x1b5e4);  if (::IsWindow(m_hWnd)) SetWindowText(strName);   // 0x2a9790
//         for (node = m_lstUnpermittedCommands.head; node; node = node->next)   // 0x3b2060 = ?m_lstUnpermittedCommands@ (0x3b2058) + 8,
//             { i = CommandToIndex(node->data); if (i >= 0) RemoveButton(i); }  //   i.e. its m_pNodeHead; 0x14eed0, vslot 0x6a8
//         ar >> m_nMRUWidth;                                                 // +0x1dc (CPane)
//     } else {
//         CObList lst;                                                       // vftable 0x32a6f8
//         for (node = m_Buttons.head; node; node = next)
//             if (node->data && node->data->IsKindOf(RUNTIME_CLASS(CMFCToolBarButton)) &&   // 0x3b1628
//                 node->data->CanBeStored())                                 // button vslot 0xb0
//                 lst.AddTail(node->data);
//         lst.Serialize(ar);                                                 // 0x232170
//         ar << m_bTextLabels;  ar << (IsWindow ? GetWindowText() : L"") (0x1b818);  ar << m_nMRUWidth;
//         lst.RemoveAll();
//     }
// Left a stub: the whole body is CArchive object serialisation of the
// m_Buttons CObList (CObList::Serialize -> CArchive::ReadObject/WriteObject
// with CRuntimeClass lookup and per-button Serialize overrides), and OpenMFC's
// live buttons are not in m_Buttons at all but in the mfccore.cpp side table,
// so neither direction can be reproduced faithfully today.  The loading path
// additionally needs InsertButton/AdjustLocations (the latter a stub here).
// Symbol: ?Serialize@CMFCToolBar@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBar__UEAAXAEAVCArchive___Z(CMFCToolBar* pThis, CArchive* ar)
{
    (void)pThis; (void)ar;
    // TODO(clean-room): not transcribed -- see above.
}

// Retail (RVA 0x156f50, mfc140u), fully transcribed -- see the UIntListAssignFrom
// helper near the top of this file for the instruction-level decoding:
//     m_lstBasicCommands.RemoveAll();                                   // 0x8350 on 0x3b2020
//     if (&lstCommands == NULL) AfxThrowInvalidArgException();
//     for (node = lstCommands.head; node; node = node->next) m_lstBasicCommands.AddTail(node->data);   // 0x12284
// Symbol: ?SetBasicCommands@CMFCToolBar@@SAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetBasicCommands_CMFCToolBar__SAXAEAV__CList_II___Z(const void* pListCommands)
{
    UIntListAssignFrom(&impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A, pListCommands);
}

// Retail (RVA 0x14f150, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = GetButton(nIndex);                    // 0x14fe00
//     if (pButton == NULL) return;                                        // (retail: ASSERT + return)
//     pButton->m_nID = nID;  pButton->m_nStyle = nStyle;                  // +0x24, +0x28
//     pButton->SetImage(iImage);                                          // button vslot 0x190 (slot 50)
//     if ((nStyle & TBBS_SEPARATOR) && iImage > 0) AdjustLayout();        // vslot 0x428
//     InvalidateButton(nIndex);                                           // 0x14fe50
// Deviation: SetImage is reached through the exported base thunk (the real
// body), not dispatched -- a derived button's override is not seen.
// Symbol: ?SetButtonInfo@CMFCToolBar@@QEAAXHIIH@Z
extern "C" void MS_ABI impl__SetButtonInfo_CMFCToolBar__QEAAXHIIH_Z(
    CMFCToolBar* pThis, int nIndex, unsigned int nID, unsigned int nStyle, int iImage)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    if (!pButton) return;

    pButton->m_nID = nID;
    pButton->m_nStyle = nStyle;
    impl__SetImage_CMFCToolBarButton__UEAAXH_Z(pButton, iImage);   // TODO(clean-room): vslot 0x190 not dispatched

    if ((nStyle & 1u) != 0 && iImage > 0) {   // TBBS_SEPARATOR: a separator's "image" is its width
        pThis->AdjustLayout();
    }
    impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, nIndex);
}

// Retail (RVA 0x14f040, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = GetButton(nIndex);                    // 0x14fe00
//     if (pButton == NULL) return;
//     UINT nOldStyle = pButton->m_nStyle;                                 // +0x28
//     if (nOldStyle == nStyle) return;
//     if (nStyle & TBBS_DISABLED /*0x40000, bt 18*/)
//         nStyle &= ~TBBS_PRESSED;                                        // 0x20000, btr 17 + cmovae
//     pButton->SetStyle(nStyle);                                          // button vslot 0x128 (slot 37) = `m_nStyle = nStyle`
//     if (((nStyle & nOldStyle) & TBBS_PRESSED) == 0)                     // and %r14d,%ebx ; bt $0x11
//         InvalidateButton(nIndex);                                       // 0x14fe50
// SetStyle is the header inline (RVA 0x237e0 is `mov %edx,0x28(%rcx); ret`),
// so the member store is made directly.
// Symbol: ?SetButtonStyle@CMFCToolBar@@UEAAXHI@Z
extern "C" void MS_ABI impl__SetButtonStyle_CMFCToolBar__UEAAXHI_Z(
    CMFCToolBar* pThis, int nIndex, unsigned int nStyle)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    if (!pButton) return;

    const unsigned int nOldStyle = pButton->m_nStyle;
    if (nOldStyle == nStyle) return;

    if (nStyle & 0x40000u) {      // TBBS_DISABLED
        nStyle &= ~0x20000u;      // TBBS_PRESSED
    }
    pButton->m_nStyle = nStyle;   // CMFCToolBarButton::SetStyle, vslot 0x128 inline

    if (((nStyle & nOldStyle) & 0x20000u) == 0) {
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, nIndex);
    }
}

// Retail (RVA 0x14f1c0, mfc140u), fully transcribed:
//     if (lpszText == NULL) AfxThrowInvalidArgException();               // 0x227720
//     CMFCToolBarButton* pButton = GetButton(nIndex);                    // 0x14fe00
//     if (pButton == NULL) return FALSE;
//     pButton->m_strText.SetString(lpszText, wcslen(lpszText));          // +0x38; IAT 0x2c7748 wcslen, 0x2e30 SetString
//     return TRUE;
// The NULL-text throw becomes a FALSE return.
// Symbol: ?SetButtonText@CMFCToolBar@@QEAAHHPEB_W@Z
extern "C" int MS_ABI impl__SetButtonText_CMFCToolBar__QEAAHHPEB_W_Z(
    CMFCToolBar* pThis, int nIndex, const wchar_t* lpszText)
{
    if (!pThis || !lpszText) return FALSE;
    CMFCToolBarButton* pButton = pThis->GetButton(nIndex);
    if (!pButton) return FALSE;
    pButton->m_strText = lpszText;
    return TRUE;
}

// Retail (RVA 0x23450, mfc140u; a low-address ICF-shared body), fully transcribed:
//     if (nMinUsagePercentage >= 100) return FALSE;                      // unsigned compare, jb
//     CMFCCmdUsageCount::m_nStartCount = nStartCount;                    // 0x3be1ac
//     CMFCCmdUsageCount::m_nMinUsagePercentage = nMinUsagePercentage;    // 0x3b1ab8
//     return TRUE;
// Symbol: ?SetCommandUsageOptions@CMFCToolBar@@SAHII@Z
extern "C" int MS_ABI impl__SetCommandUsageOptions_CMFCToolBar__SAHII_Z(
    unsigned int nStartCount, unsigned int nMinUsagePercentage)
{
    if (nMinUsagePercentage >= 100u) return FALSE;
    impl__m_nStartCount_CMFCCmdUsageCount__1IA = nStartCount;
    impl__m_nMinUsagePercentage_CMFCCmdUsageCount__1IA = nMinUsagePercentage;
    return TRUE;
}

// Retail (RVA 0x1527e0, mfc140u), fully transcribed:
//     if (m_bCustomizeMode == bSet) return FALSE;                              // 0x3be35c
//     do {                                                                     // pass 1
//         bRestart = FALSE;
//         node = <all-toolbars list>.head (0x3b2098);  nCount = its count (0x3b20a8);
//         for (; node && !bRestart; node = node->next) {
//             pBar = node->data;  if (!pBar) AfxThrowInvalidArgException();
//             if (CWnd::FromHandlePermanent(pBar->m_hWnd)) {                  // 0x28adc0
//                 pBar->OnCustomizeMode(bSet);                                // vslot 0x818
//                 if (<list count> != nCount) bRestart = TRUE;                // a bar came or went: start over
//             }
//         }
//     } while (bRestart);
//     m_bCustomizeMode = bSet;
//     for (pBar in the list)                                                   // pass 2
//         if (FromHandlePermanent(pBar->m_hWnd) && !pBar->m_bLocked)           // +0x10b8
//             pBar->AdjustLayout();                                            // vslot 0x428
//     BOOL bFrameDone = FALSE;
//     for (pBar in the list) {                                                 // pass 3
//         if (!(pBar->GetStyle() & WS_VISIBLE)) continue;                      // 0x2a9690, bt 28
//         CWnd* pSite = pBar->GetDockSiteFrameWnd();                           // vslot 0x348
//         if (pSite && pSite->IsKindOf(RUNTIME_CLASS(CFrameWnd)) && !bFrameDone) {   // 0x33aef0
//             ((CFrameWnd*)pSite)->RecalcLayout(TRUE);                         // frame vslot 0x300
//             bFrameDone = TRUE;
//         }
//         if (FromHandlePermanent(pBar->m_hWnd))
//             ::RedrawWindow(pBar->m_hWnd, NULL, NULL, 0x585);                 // IAT 0x2c7130
//     }
//     if (!bSet) m_pSelToolbar = NULL;                                         // 0x3be338
//     return TRUE;
// Deviations: the three passes run over the mfccore.cpp toolbar side table
// (SnapshotToolBars) instead of retail's private CObList, the
// FromHandlePermanent test becomes a GetSafeHwnd() test (see RedrawUnderlines),
// the restart condition compares the side table's size, OnCustomizeMode is
// reached through this file's export thunk rather than vslot 0x818, and
// GetStyle is the inlined ::GetWindowLongW(GWL_STYLE).
// Symbol: ?SetCustomizeMode@CMFCToolBar@@SAHH@Z
extern "C" int MS_ABI impl__SetCustomizeMode_CMFCToolBar__SAHH_Z(int bSet)
{
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == bSet) return FALSE;

    bool bRestart;
    do {
        bRestart = false;
        const std::vector<CMFCToolBar*> bars = SnapshotToolBars();
        const std::size_t nCount = g_toolBarStates.size();
        for (CMFCToolBar* pBar : bars) {
            if (bRestart) break;
            if (!pBar->GetSafeHwnd()) continue;
            impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(pBar, bSet);   // TODO(clean-room): vslot 0x818 not dispatched
            if (g_toolBarStates.size() != nCount) bRestart = true;
        }
    } while (bRestart);

    impl__m_bCustomizeMode_CMFCToolBar__1HA = bSet;

    const std::vector<CMFCToolBar*> bars = SnapshotToolBars();
    for (CMFCToolBar* pBar : bars) {
        if (!pBar->GetSafeHwnd()) continue;
        if (pBar->m_bLocked) continue;
        pBar->AdjustLayout();
    }

    bool bFrameDone = false;
    for (CMFCToolBar* pBar : bars) {
        const HWND hwnd = pBar->GetSafeHwnd();
        const LONG style = hwnd ? ::GetWindowLongW(hwnd, GWL_STYLE) : 0;
        if ((style & WS_VISIBLE) == 0) continue;

        CWnd* pSite = static_cast<CWnd*>(impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(pBar));
        if (pSite && !bFrameDone &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pSite, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            impl__RecalcLayout_CFrameWnd__UEAAXH_Z(static_cast<CFrameWnd*>(pSite), TRUE);
            bFrameDone = true;
        }
        if (hwnd) {
            ::RedrawWindow(hwnd, nullptr, nullptr, 0x585);
        }
    }

    if (!bSet) {
        impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
    }
    return TRUE;
}

// Retail (RVA 0x14dd90, mfc140u), fully transcribed:
//     int cyDelta = cyHeight;
//     if (m_dwStyle & CBRS_BORDER_TOP    /*0x200, bt 9*/)  cyDelta -= afxData.cyBorder2;   // 0x3c32d4 = afxData+0x14
//     if (m_dwStyle & CBRS_BORDER_BOTTOM /*0x800, bt 11*/) cyDelta -= afxData.cyBorder2;
//     m_cyBottomBorder = (cyDelta - GetRowHeight()) / 2;                    // +0x1d4 (CPane); vslot 0x6b8
//     m_cyTopBorder    = cyDelta - m_cyBottomBorder - GetRowHeight();       // +0x1d0; GetRowHeight called again
//     if (m_cyTopBorder < 0) { m_cyBottomBorder += m_cyTopBorder;  m_cyTopBorder = 0; }
//     if (m_hWnd) ::RedrawWindow(m_hWnd, NULL, NULL, 0x505);               // IAT 0x2c7130
// (The two stores are to +0x1d4 first and +0x1d0 second; the names follow
// include/openmfc/afxmfc.h, which pins m_cyTopBorder at 0x1d0 and
// m_cyBottomBorder at 0x1d4.)  afxData.cyBorder2 is the constant 2 here, the
// value the retail AUX_DATA initialiser stores -- OpenMFC's exported afxData
// is a zero blob (same choice as featurepack/menu/CMFCPopupMenuBar.cpp and
// core/frame/CSplitterWnd.cpp).  GetRowHeight is this file's export, not
// dispatched.
// Symbol: ?SetHeight@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetHeight_CMFCToolBar__QEAAXH_Z(CMFCToolBar* pThis, int cyHeight)
{
    if (!pThis) return;
    constexpr int kAuxDataCyBorder2 = 2;

    int cyDelta = cyHeight;
    if (pThis->m_dwStyle & 0x200u) cyDelta -= kAuxDataCyBorder2;   // CBRS_BORDER_TOP
    if (pThis->m_dwStyle & 0x800u) cyDelta -= kAuxDataCyBorder2;   // CBRS_BORDER_BOTTOM

    pThis->m_cyBottomBorder = (cyDelta - impl__GetRowHeight_CMFCToolBar__UEBAHXZ(pThis)) / 2;
    pThis->m_cyTopBorder = cyDelta - pThis->m_cyBottomBorder - impl__GetRowHeight_CMFCToolBar__UEBAHXZ(pThis);
    if (pThis->m_cyTopBorder < 0) {
        pThis->m_cyBottomBorder += pThis->m_cyTopBorder;
        pThis->m_cyTopBorder = 0;
    }

    if (pThis->m_hWnd) {
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, 0x505);
    }
}

// Retail (RVA 0x156e50, mfc140u), fully transcribed:
//     if (bOn) {
//         if (m_hookMouseHelp == NULL)                                          // 0x3be360
//             m_hookMouseHelp = ::SetWindowsHookExW(WH_MOUSE /*7*/, ToolBarMouseHookProc /*0x156cb0*/,
//                                                   NULL, ::GetCurrentThreadId());   // IAT 0x2c6c70 / 0x2c6638
//         return;
//     }
//     if (m_hookMouseHelp == NULL) return;
//     ::UnhookWindowsHookEx(m_hookMouseHelp);                                   // IAT 0x2c6c78
//     node = <all-toolbars list>.head;                                          // 0x3b2098, read before the clears
//     m_hookMouseHelp = NULL;  m_pLastHookedToolbar = NULL;                     // 0x3be340
//     for (; node; node = node->next) {
//         pBar = node->data;  if (!pBar) AfxThrowInvalidArgException();
//         if (CWnd::FromHandlePermanent(pBar->m_hWnd)) pBar->OnCancelMode();   // 0x28adc0, 0x1516b0 (direct call)
//     }
// Deviations: the walk runs over the mfccore.cpp side table with the
// GetSafeHwnd() substitution for FromHandlePermanent (see RedrawUnderlines).
// Symbol: ?SetHelpMode@CMFCToolBar@@SAXH@Z
extern "C" void MS_ABI impl__SetHelpMode_CMFCToolBar__SAXH_Z(int bOn)
{
    if (bOn) {
        if (impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA == nullptr) {
            impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA = ::SetWindowsHookExW(
                WH_MOUSE, reinterpret_cast<HOOKPROC>(&impl__ToolBarMouseHookProc_CMFCToolBar__KA_JH_K_J_Z),
                nullptr, ::GetCurrentThreadId());
        }
        return;
    }

    if (impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA == nullptr) return;
    ::UnhookWindowsHookEx(static_cast<HHOOK>(impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA));
    const std::vector<CMFCToolBar*> bars = SnapshotToolBars();
    impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA = nullptr;
    impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
    for (CMFCToolBar* pBar : bars) {
        if (!pBar->GetSafeHwnd()) continue;
        impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pBar);
    }
}

// Retail (RVA 0x157d80, mfc140u), fully transcribed:
//     CMFCToolBarMenuButton* pMenu = GetDroppedDownMenu(NULL);                // 0x157f30
//     if (pMenu && pMenu->IsExclusive()) return TRUE;                          // menu-button vslot 0x1e8 (slot 61)
//     if (pButton == NULL) { m_iHot = -1;  return TRUE; }                      // +0x1140
//     int i = 0;
//     for (node = m_Buttons.head /*+0x1190*/; node; node = node->next, i++) {
//         if (node->data == pButton) {
//             if (m_iHot != i) OnChangeHot(i);                                 // vslot 0x770
//             return TRUE;
//         }
//     }
//     return FALSE;                                                            // eax is the exhausted node pointer, 0
// Slot 61 of the CMFCToolBarMenuButton vftable (0x3184e8, mfc140u) is RVA
// 0x71e0, `xor eax,eax; ret` -- the header inline `IsExclusive() { return
// FALSE; }` -- and GetDroppedDownMenu is a stub returning NULL here anyway, so
// the first test is folded to "not exclusive"; a derived button's override is
// not honoured.  The button walk runs over the side table; OnChangeHot is
// this file's export, not dispatched.
// Symbol: ?SetHot@CMFCToolBar@@QEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton)
{
    if (!pThis) return FALSE;

    CMFCToolBarMenuButton* pMenu =
        impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(pThis, nullptr);
    (void)pMenu;   // TODO(clean-room): pMenu->IsExclusive() (vslot 0x1e8) folded to FALSE, see above

    if (!pButton) {
        pThis->m_iHot = -1;
        return TRUE;
    }

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        if (pThis->GetButton(i) == pButton) {
            if (pThis->m_iHot != i) {
                impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, i);
            }
            return TRUE;
        }
    }
    return FALSE;
}

// Retail (RVA 0x156a60, mfc140u), fully transcribed: a single store,
//     m_clrTextHot = clrText;                    // 0x3b1b54
// Symbol: ?SetHotTextColor@CMFCToolBar@@SAXK@Z
extern "C" void MS_ABI impl__SetHotTextColor_CMFCToolBar__SAXK_Z(unsigned long clrText)
{
    impl__m_clrTextHot_CMFCToolBar__1KA = static_cast<std::uint32_t>(clrText);
}

// Retail (RVA 0x158990, mfc140u), fully transcribed:
//     m_bLargeIcons = bLargeIcons;                                             // 0x3be368
//     if (bLargeIcons) {
//         m_sizeCurButton = Scale(m_sizeButton);  m_sizeCurImage = Scale(m_sizeImage);   // 0x3b1ee8 / 0x3b1c78 <- 0x3b1e38 / 0x3b1cb8
//     } else {
//         m_sizeCurButton = m_sizeButton;  m_sizeCurImage = m_sizeImage;      // two QWORD copies
//     }
//     for (pBar in the all-toolbars list (0x3b2098)) {
//         if (!CWnd::FromHandlePermanent(pBar->m_hWnd)) continue;             // 0x28adc0
//         if (pBar->m_bLocked) {                                              // +0x10b8
//             if (m_bLargeIcons) {
//                 if (pBar->m_sizeCurButtonLocked == Scale(pBar->m_sizeButtonLocked) &&    // +0x1168 vs +0x1158
//                     pBar->m_sizeCurImageLocked  == Scale(pBar->m_sizeImageLocked))       // +0x1170 vs +0x1160
//                     continue;                                               // nothing changed: no relayout
//                 pBar->m_sizeCurButtonLocked = Scale(pBar->m_sizeButtonLocked);
//                 pBar->m_sizeCurImageLocked  = Scale(pBar->m_sizeImageLocked);
//             } else {
//                 if (pBar->m_sizeCurButtonLocked == pBar->m_sizeButtonLocked &&
//                     pBar->m_sizeCurImageLocked  == pBar->m_sizeImageLocked)
//                     continue;
//                 pBar->m_sizeCurImageLocked  = pBar->m_sizeImageLocked;
//                 pBar->m_sizeCurButtonLocked = pBar->m_sizeButtonLocked;
//             }
//         }
//         pBar->AdjustLayout();                                               // vslot 0x428
//         if (pBar->m_bLocked) {
//             CWnd* pParent = CWnd::FromHandle(::GetParent(pBar->m_hWnd));    // IAT 0x2c72d8, 0x28ad70
//             if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CBasePane)))     // 0x2dc540
//                 ((CBasePane*)pParent)->AdjustLayout();                      // vslot 0x428
//         }
//     }
// Scale(v) is the (int)(v * m_dblLargeImageRatio + 0.5) idiom (ScaleLarge
// near the top of this file); the comparisons are made component-wise on the
// freshly scaled values exactly as retail does.
// Deviations: the walk runs over the mfccore.cpp side table with the
// GetSafeHwnd() substitution (see RedrawUnderlines).  CBasePane::AdjustLayout
// is a header inline no-op that OpenMFC neither exports nor declares, so the
// parent's vslot 0x428 cannot be dispatched here: it is called only when the
// parent is itself a CMFCToolBar (its exported AdjustLayout); any other
// CBasePane-derived parent (a dock site, a pane divider, ...) is skipped.
// Symbol: ?SetLargeIcons@CMFCToolBar@@SAXH@Z
extern "C" void MS_ABI impl__SetLargeIcons_CMFCToolBar__SAXH_Z(int bLargeIcons)
{
    impl__m_bLargeIcons_CMFCToolBar__1HA = bLargeIcons;

    if (bLargeIcons) {
        impl__m_sizeCurButton_CMFCToolBar__1VCSize__A.cx = ScaleLarge(impl__m_sizeButton_CMFCToolBar__1VCSize__A.cx);
        impl__m_sizeCurButton_CMFCToolBar__1VCSize__A.cy = ScaleLarge(impl__m_sizeButton_CMFCToolBar__1VCSize__A.cy);
        impl__m_sizeCurImage_CMFCToolBar__1VCSize__A.cx  = ScaleLarge(impl__m_sizeImage_CMFCToolBar__1VCSize__A.cx);
        impl__m_sizeCurImage_CMFCToolBar__1VCSize__A.cy  = ScaleLarge(impl__m_sizeImage_CMFCToolBar__1VCSize__A.cy);
    } else {
        impl__m_sizeCurButton_CMFCToolBar__1VCSize__A = impl__m_sizeButton_CMFCToolBar__1VCSize__A;
        impl__m_sizeCurImage_CMFCToolBar__1VCSize__A  = impl__m_sizeImage_CMFCToolBar__1VCSize__A;
    }

    const std::vector<CMFCToolBar*> bars = SnapshotToolBars();
    for (CMFCToolBar* pBar : bars) {
        const HWND hwnd = pBar->GetSafeHwnd();
        if (!hwnd) continue;

        if (pBar->m_bLocked) {
            if (impl__m_bLargeIcons_CMFCToolBar__1HA) {
                const int cxButton = ScaleLarge(pBar->m_sizeButtonLocked.cx);
                const int cyButton = ScaleLarge(pBar->m_sizeButtonLocked.cy);
                const int cxImage  = ScaleLarge(pBar->m_sizeImageLocked.cx);
                const int cyImage  = ScaleLarge(pBar->m_sizeImageLocked.cy);
                if (pBar->m_sizeCurButtonLocked.cx == cxButton && pBar->m_sizeCurButtonLocked.cy == cyButton &&
                    pBar->m_sizeCurImageLocked.cx == cxImage && pBar->m_sizeCurImageLocked.cy == cyImage) {
                    continue;
                }
                pBar->m_sizeCurButtonLocked = CSize(cxButton, cyButton);
                pBar->m_sizeCurImageLocked  = CSize(cxImage, cyImage);
            } else {
                if (pBar->m_sizeCurButtonLocked.cx == pBar->m_sizeButtonLocked.cx &&
                    pBar->m_sizeCurButtonLocked.cy == pBar->m_sizeButtonLocked.cy &&
                    pBar->m_sizeCurImageLocked.cx == pBar->m_sizeImageLocked.cx &&
                    pBar->m_sizeCurImageLocked.cy == pBar->m_sizeImageLocked.cy) {
                    continue;
                }
                pBar->m_sizeCurImageLocked  = pBar->m_sizeImageLocked;
                pBar->m_sizeCurButtonLocked = pBar->m_sizeButtonLocked;
            }
        }

        pBar->AdjustLayout();

        if (pBar->m_bLocked) {
            CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hwnd));
            if (pParent && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                               pParent, impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ())) {
                if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                        pParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
                    impl__AdjustLayout_CMFCToolBar__UEAAXXZ(static_cast<CMFCToolBar*>(pParent));
                }
                // TODO(clean-room): vslot 0x428 on a non-toolbar CBasePane parent is not dispatched.
            }
        }
    }
}

// Retail (RVA 0x14dc40, mfc140u), fully transcribed:
//     m_sizeImageLocked     = sizeImage;                     // +0x1160
//     m_bDontScaleLocked    = bDontScale;                    // +0x1178 (stored, not consulted here)
//     m_sizeButtonLocked    = sizeButton;                    // +0x1158
//     m_sizeCurButtonLocked = sizeButton;                    // +0x1168
//     m_sizeCurImageLocked  = sizeImage;                     // +0x1170
//     m_ImagesLocked.SetImageSize(sizeImage);                // +0x460  (= 0x3f8 + 0x68)
//     m_MenuImagesLocked.SetImageSize(sizeImage);            // +0xdf0  (= 0xd88 + 0x68)
//     m_ColdImagesLocked.SetImageSize(sizeImage);            // +0x5f8  (= 0x590 + 0x68)
//     m_DisabledImagesLocked.SetImageSize(sizeImage);        // +0x790  (= 0x728 + 0x68)
//     m_DisabledMenuImagesLocked.SetImageSize(sizeImage);    // +0xf88  (= 0xf20 + 0x68)
//     CSize sizeLarge = Scale(m_sizeImageLocked);            // the m_dblLargeImageRatio idiom, 0x3b1b58
//     m_LargeImagesLocked.SetImageSize(sizeLarge);           // +0x928  (= 0x8c0 + 0x68)
//     m_LargeColdImagesLocked.SetImageSize(sizeLarge);       // +0xac0  (= 0xa58 + 0x68)
//     m_LargeDisabledImagesLocked.SetImageSize(sizeLarge);   // +0xc58  (= 0xbf0 + 0x68)
//     if (m_bLargeIcons) {                                   // 0x3be368
//         m_sizeCurButtonLocked = Scale(m_sizeCurButtonLocked);
//         m_sizeCurImageLocked  = Scale(m_sizeCurImageLocked);
//     }
// SetImageSize is the inlined store into CMFCToolBarImages::m_sizeImage
// (SetImagesSizeImage near the top of this file).
// Symbol: ?SetLockedSizes@CMFCToolBar@@QEAAXUtagSIZE@@0H@Z
extern "C" void MS_ABI impl__SetLockedSizes_CMFCToolBar__QEAAXUtagSIZE__0H_Z(
    CMFCToolBar* pThis, SIZE sizeButton, SIZE sizeImage, int bDontScale)
{
    if (!pThis) return;

    pThis->m_sizeImageLocked     = CSize(sizeImage);
    pThis->m_bDontScaleLocked    = bDontScale;
    pThis->m_sizeButtonLocked    = CSize(sizeButton);
    pThis->m_sizeCurButtonLocked = CSize(sizeButton);
    pThis->m_sizeCurImageLocked  = CSize(sizeImage);

    SetImagesSizeImage(&pThis->m_ImagesLocked,             sizeImage.cx, sizeImage.cy);
    SetImagesSizeImage(&pThis->m_MenuImagesLocked,         sizeImage.cx, sizeImage.cy);
    SetImagesSizeImage(&pThis->m_ColdImagesLocked,         sizeImage.cx, sizeImage.cy);
    SetImagesSizeImage(&pThis->m_DisabledImagesLocked,     sizeImage.cx, sizeImage.cy);
    SetImagesSizeImage(&pThis->m_DisabledMenuImagesLocked, sizeImage.cx, sizeImage.cy);

    const int cxLarge = ScaleLarge(pThis->m_sizeImageLocked.cx);
    const int cyLarge = ScaleLarge(pThis->m_sizeImageLocked.cy);
    SetImagesSizeImage(&pThis->m_LargeImagesLocked,         cxLarge, cyLarge);
    SetImagesSizeImage(&pThis->m_LargeColdImagesLocked,     cxLarge, cyLarge);
    SetImagesSizeImage(&pThis->m_LargeDisabledImagesLocked, cxLarge, cyLarge);

    if (impl__m_bLargeIcons_CMFCToolBar__1HA) {
        pThis->m_sizeCurButtonLocked.cx = ScaleLarge(pThis->m_sizeCurButtonLocked.cx);
        pThis->m_sizeCurButtonLocked.cy = ScaleLarge(pThis->m_sizeCurButtonLocked.cy);
        pThis->m_sizeCurImageLocked.cx  = ScaleLarge(pThis->m_sizeCurImageLocked.cx);
        pThis->m_sizeCurImageLocked.cy  = ScaleLarge(pThis->m_sizeCurImageLocked.cy);
    }
}

// Retail (RVA 0x1573e0, mfc140u), fully transcribed -- four QWORD stores:
//     m_sizeMenuButton = sizeButton;                       // 0x3b1d08
//     m_sizeMenuImage  = sizeImage;                        // 0x3b1c38
//     m_MenuImages.SetImageSize(sizeImage);                // 0x3c2948 = ?m_MenuImages@ (0x3c28e0) + 0x68
//     m_DisabledMenuImages.SetImageSize(sizeImage);        // 0x3c2c88 = ?m_DisabledMenuImages@ (0x3c2c20) + 0x68
// Symbol: ?SetMenuSizes@CMFCToolBar@@SAXUtagSIZE@@0@Z
extern "C" void MS_ABI impl__SetMenuSizes_CMFCToolBar__SAXUtagSIZE__0_Z(SIZE sizeButton, SIZE sizeImage)
{
    impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cx = sizeButton.cx;
    impl__m_sizeMenuButton_CMFCToolBar__1VCSize__A.cy = sizeButton.cy;
    impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx  = sizeImage.cx;
    impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cy  = sizeImage.cy;
    SetImagesSizeImage(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A,         sizeImage.cx, sizeImage.cy);
    SetImagesSizeImage(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A, sizeImage.cx, sizeImage.cy);
}

// Retail (RVA 0x156f00, mfc140u), fully transcribed -- the SetBasicCommands
// shape (see UIntListAssignFrom) on ?m_lstUnpermittedCommands@ (0x3b2058).
// Symbol: ?SetNonPermittedCommands@CMFCToolBar@@SAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetNonPermittedCommands_CMFCToolBar__SAXAEAV__CList_II___Z(const void* pListCommands)
{
    UIntListAssignFrom(&impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A, pListCommands);
}

// Retail (RVA 0x15b060, mfc140u), fully transcribed:
//     if (!m_bHasBrother) return;                                            // +0x110c
//     CMFCToolBar* pBrother = m_pBrotherToolBar;  if (!pBrother) return;    // +0x12f8
//     CDockingPanesRow* pRow = m_pDockBarRow;                                // +0x130
//     CDockingPanesRow* pBrotherRow = pBrother->m_pDockBarRow;
//     if (pRow == pBrotherRow) return;
//     if (m_bElderBrother) {                                                 // +0x1110
//         pBrotherRow->RemovePane(pBrother);                                 // row vslot 0x58
//         pRow->AddPane(pBrother, DM_STANDARD /*5*/, NULL, FALSE);           // row vslot 0x48
//     } else {
//         pRow->RemovePane(this);
//         pBrotherRow->AddPane(this, DM_STANDARD, NULL, FALSE);
//     }
// Slots 0x58 / 0x48 of the CDockingPanesRow vftable (0x2e61f8, mfc140u) are
// ?RemovePane@ (0x4fba0) and ?AddPane@CDockingPanesRow@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@H@Z
// (0x4f6c0); 5 is DM_STANDARD in the afxbasepane.h enum.  Retail does not
// NULL-check either row; both are guarded here.  Deviation: AddPane is an
// empty stub in featurepack/docking/CDockingPanesRow.cpp (with a placeholder
// parameter list -- see the headerRequest), so today the pane is removed from
// its row but not re-added to the sibling's.
// Symbol: ?SetOneRowWithSibling@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__SetOneRowWithSibling_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (!pThis->m_bHasBrother) return;
    CMFCToolBar* pBrother = pThis->m_pBrotherToolBar;
    if (!pBrother) return;

    void* pRow = pThis->m_pDockBarRow;
    void* pBrotherRow = pBrother->m_pDockBarRow;
    if (pRow == pBrotherRow) return;
    if (!pRow || !pBrotherRow) return;

    constexpr int kDmStandard = 5;   // AFX_DOCK_METHOD::DM_STANDARD
    if (pThis->m_bElderBrother) {
        impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(pBrotherRow, pBrother);
        impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(
            pRow, pBrother, kDmStandard, nullptr, FALSE);
    } else {
        impl__RemovePane_CDockingPanesRow__UEAAXPEAVCPane___Z(pRow, pThis);
        impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(
            pBrotherRow, pThis, kDmStandard, nullptr, FALSE);
    }
}

// Retail (RVA 0x15b5e0, mfc140u), fully transcribed:
//     while (m_OrigButtons.GetCount() != 0) {                 // +0x11c0, count at +0x11d8
//         CObject* p = m_OrigButtons.RemoveHead();            // 0x231f40
//         if (p) delete p;                                    // vslot 0x08, flag 1 (deleting dtor)
//     }
//     m_OrigButtons.AddTail((CObList*)&lstOrigButtons);       // tail-jump to ?AddTail@CObList@@QEAAXPEAV1@@Z (0x231f00):
//                                                             //   the pointers are copied, the buttons are NOT cloned
// The delete goes through the buttons' virtual destructor as in retail.
// Deviation: OpenMFC's CObList keeps its elements in a side table keyed by
// the list object (include/openmfc/afx.h; detail/FilecoreSupport.h
// OPENMFC_DEFINE_LIST_METHODS) and never maintains the retail m_pNodeHead /
// m_nCount members.  A source list an MSVC client fills through the exported
// AddTail/AddHead/InsertAfter (those are DLL calls, not inlines) IS seen here;
// what a client cannot do is read m_OrigButtons back through its own inlined
// CObList::GetCount()/GetHeadPosition() (afxcoll.inl), which read the raw
// members and see an empty list.  That is a property of the CObList model,
// not of this body.
// Symbol: ?SetOrigButtons@CMFCToolBar@@QEAAXAEBVCObList@@@Z
extern "C" void MS_ABI impl__SetOrigButtons_CMFCToolBar__QEAAXAEBVCObList___Z(
    CMFCToolBar* pThis, const CObList* pListOrigButtons)
{
    if (!pThis) return;
    while (pThis->m_OrigButtons.GetCount() != 0) {
        CObject* p = pThis->m_OrigButtons.RemoveHead();
        if (p) delete p;
    }
    impl__AddTail_CObList__QEAAXPEAV1__Z(&pThis->m_OrigButtons, const_cast<CObList*>(pListOrigButtons));
}
