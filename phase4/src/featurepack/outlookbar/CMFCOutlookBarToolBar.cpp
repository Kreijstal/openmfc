// CMFCOutlookBarToolBar — OpenMFC implementation.
// Sources: global_cmfcoutlookbartoolbar.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// OpenMFC: CMFCOutlookBarToolBar exports.
//
// Bodies below are based on retail mfc140u.dll (14.51.36231, x64) where the
// export resolves; unresolved or unmodeled behavior is identified at its body.
//
// The class is not declared in the public headers, so `this` is taken as void*.
// CMFCOutlookBarToolBar derives from CMFCToolBar and is 5008 bytes (0x1390);
// the CMFCToolBar base occupies 0..0x134f, and the derived region starts at
// 0x1350. Retail offsets below are described inline; this file has no shadow
// struct for the derived class. Implementations and documented stubs are
// distinguished at their definitions.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * bodies that dereference pThis check it; no-op stubs may ignore it;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// The build's AUTO_EXCLUDES pass scans `// Symbol:` comments and drops the
// generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMFCToolBar::OnCustomizeMode (global_mfc-feature-3_impl.cpp).
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int bCustomize);
// CWnd::EnableWindow (wincore.cpp).
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
// CMFCToolBar::UpdateTooltips (global_mfc-feature-5_impl.cpp).
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
// Existing toolbar/global-data thunks used by OnSetCursor.
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, long long point);
extern "C" int MS_ABI impl__OnSetCursor_CMFCToolBar__IEAAHPEAVCWnd__II_Z(
    CMFCToolBar* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__GetHandCursor_AFX_GLOBAL_DATA__QEAAPEAUHICON____XZ(void* pThis);

//=============================================================================
// CMFCOutlookBarToolBar
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::AdjustLocations()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnCustomizeMode(BOOL bCustomize)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnNcCalcSize(BOOL bCalcValidRects,
//                                          NCCALCSIZE_PARAMS* lpncsp)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnNcPaint()
//=============================================================================

//=============================================================================
// int CMFCOutlookBarToolBar::OnSendCommand(const CMFCToolBarButton* pButton)
//=============================================================================

//=============================================================================
// int CMFCOutlookBarToolBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisable)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarToolBar::OnUserToolTip(CMFCToolBarButton* pButton,
//                                           CString& rString)
//=============================================================================
// Retail entry RVA 0x9b750 (mfc140u): scales toolbar images, lays out the
// buttons, handles the customize-button case, and finishes with a tooltip
// update. This body omits the layout work and only calls the tooltip-update
// thunk; it is a partial implementation, not a faithful transcription.
// Symbol: ?AdjustLocations@CMFCOutlookBarToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCOutlookBarToolBar__UEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): retail image scaling, button layout, and the
    // m_pCustomizeBtn special case are omitted; only the tooltip update runs.
    impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(reinterpret_cast<CMFCToolBar*>(pThis));
}
// Retail entry RVA 0x9b9c0 (mfc140u):
//     CMFCToolBar::OnCustomizeMode(this, bCustomize);
//     return CWnd::EnableWindow(this, bCustomize == 0);   // tail call
// Symbol: ?OnCustomizeMode@CMFCOutlookBarToolBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCOutlookBarToolBar__UEAAXH_Z(
    void* pThis, int bCustomize)
{
    if (!pThis) return;
    impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), bCustomize);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(pThis), bCustomize == 0);
}
// Stub: the exact symbol is absent from both retail RVA maps, so its body is
// unverified. Keep this no-op rather than attributing behavior to an unrelated
// RVA.
// Symbol: ?OnNcCalcSize@CMFCOutlookBarToolBar@@QEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCOutlookBarToolBar__QEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    (void)pThis; (void)bCalcValidRects; (void)lpncsp;
}
// Stub: disas.py reports NOT FOUND for this exact symbol in both retail RVA
// maps, so its body could not be verified.
// Symbol: ?OnNcPaint@CMFCOutlookBarToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCOutlookBarToolBar__QEAAXXZ(void* pThis)
{
    (void)pThis;
}
// The ANSI twin's function entry is RVA 0x9bad0 (mfc140). It resolves the
// button index, looks up per-button info at +0x1350, compares against the
// internal toolbar at +0x1388, and has a conditional parent-window message
// dispatch. The internal array/callback representation is not modeled here;
// this function is therefore a stub that returns 0 for every non-null object.
// Symbol: ?OnSendCommand@CMFCOutlookBarToolBar@@UEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCOutlookBarToolBar__UEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton* pButton)
{
    (void)pButton;
    if (!pThis) return 0;
    // TODO(clean-room): the +0x1350 info-array lookup, hot-toolbar comparison,
    // and conditional parent-window message branch are not modeled.
    return 0;
}
// Retail ANSI-twin entry RVA 0x9bc40 (mfc140): GetCursorPos and
// ScreenToClient(this->m_hWnd), hit-test through vtable byte offset 0x730, set
// AFX_GLOBAL_DATA::GetHandCursor and return 1 for a hit; otherwise call the
// base CMFCToolBar::OnSetCursor entry at RVA 0x152e10 (mfc140). OpenMFC uses
// the existing HitTest thunk instead of dispatching through the derived object's
// vtable.
// Symbol: ?OnSetCursor@CMFCOutlookBarToolBar@@QEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCOutlookBarToolBar__QEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message)
{
    if (!pThis) return FALSE;

    auto* pBar = reinterpret_cast<CMFCToolBar*>(pThis);
    POINT point = { 0, 0 };
    ::GetCursorPos(&point);
    ::ScreenToClient(reinterpret_cast<CWnd*>(pThis)->m_hWnd, &point);

    const std::uint64_t packedPoint =
        static_cast<std::uint32_t>(point.x) |
        (static_cast<std::uint64_t>(static_cast<std::uint32_t>(point.y)) << 32);
    if (impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, static_cast<long long>(packedPoint)) >= 0) {
        std::uint32_t initialized = 0;
        std::memcpy(&initialized, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof(initialized));
        if (initialized == 0) {
            impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
            initialized = 1;
            std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &initialized, sizeof(initialized));
        }
        ::SetCursor(static_cast<HCURSOR>(impl__GetHandCursor_AFX_GLOBAL_DATA__QEAAPEAUHICON____XZ(
            impl__afxGlobalData__3UAFX_GLOBAL_DATA__A)));
        return TRUE;
    }

    return impl__OnSetCursor_CMFCToolBar__IEAAHPEAVCWnd__II_Z(
        pBar, pWnd, nHitTest, message);
}
// The ANSI twin's function entry is RVA 0x9bb90 (mfc140). It iterates the
// button count at +0x11a0, looks up per-button info through the internal array
// at +0x1350, compares against the internal toolbar at +0x1388, then invokes
// vtable byte offset 0x6f8 to update styles. The array/callback representation
// is not modeled, so this remains a documented no-op stub.
// Symbol: ?OnUpdateCmdUI@CMFCOutlookBarToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCOutlookBarToolBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler)
{
    (void)pTarget; (void)bDisableIfNoHndler;
    if (!pThis) return;
    // TODO(clean-room): the +0x1350 info-array lookup, hot-toolbar comparison,
    // and vtable-offset-0x6f8 style update are not modeled.
}
// Clean-room approximation: copies the button's text to the output string and
// returns TRUE. The exact symbol is unresolved in the retail RVA maps, so this
// behavior is not claimed as a verified retail transcription.
// Symbol: ?OnUserToolTip@CMFCOutlookBarToolBar@@UEBAHPEAVCMFCToolBarButton@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUserToolTip_CMFCOutlookBarToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CMFCToolBarButton* pButton, CString* pString)
{
    (void)pThis;
    if (!pButton || !pString) return FALSE;
    *pString = pButton->m_strText;
    return TRUE;
}
// Stub. The retail constructor (entry RVA 0x9b3f0, mfc140u) calls the
// CMFCToolBar constructor, installs a derived vftable, and initializes an
// internal array and Outlook-bar fields. This file has no compatible derived
// or array vftable representation to install, so this body must not be mistaken
// for a working constructor; the object remains uninitialized.
// Symbol: ??0CMFCOutlookBarToolBar@@QEAA@PEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" void* MS_ABI impl___0CMFCOutlookBarToolBar__QEAA_PEAVCMFCOutlookBarTabCtrl___Z(void* pThis, void* pTabCtrl) {
    (void)pTabCtrl;
    return pThis;
}
