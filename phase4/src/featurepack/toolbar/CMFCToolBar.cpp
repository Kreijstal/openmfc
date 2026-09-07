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
#include <cstring>
#include <cwchar>

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
// Not implemented: neither the cached-frame global, the CFrameImpl subobject at
// those two offsets, nor IsUserDefinedToolbar is modelled by OpenMFC.
// Symbol: ?IsUserDefined@CMFCToolBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed.
    return FALSE;
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
// Not implemented -- but not for the reason an earlier revision of this comment
// gave.  That revision said "OpenMFC models neither CSettingsStore nor
// CSettingsStoreSP"; that is false.  Both are modelled:
// phase4/src/core/app/CSettingsStore.cpp exports Open/CreateKey/DeleteKey and
// the whole Read/Write family as impl__ thunks, and
// phase4/src/core/app/CSettingsStoreSP.cpp exports
// ?Create@CSettingsStoreSP@@QEAAAEAVCSettingsStore@@HH@Z.  The real blockers
// are: (a) that store is an in-memory map, not the registry, and nothing in
// OpenMFC ever writes the MFCToolBar* keys (every Save* entry point in this
// file is still a stub), so a faithful transcription would open an empty store,
// read nothing and return FALSE -- which is what this stub already returns;
// and (b) CSettingsStoreSP is not declared in any OpenMFC header and has no
// destructor thunk, so the transcription would have to hand-roll its 16-byte
// owner object and would leak the CSettingsStore that Create allocates.
// Nothing is read and m_bLargeIcons is left alone.
// Symbol: ?LoadLargeIconsState@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    (void)lpszProfileName;
    // TODO(clean-room): not transcribed -- registry read skipped.
    return 0;
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
// Retail (0x1801582b0), transcribed:
//     strPath = AFXGetRegPath(L"MFCToolBars", lpszProfileName);
//     strKey.Format(L"%TsMFCToolBarParameters", strPath);
//     CSettingsStoreSP sp; CSettingsStore& reg = sp.Create(FALSE, TRUE);
//     if (!reg.Open(strKey)) return FALSE;                     // vslot 0x30
//     reg.Read(L"Tooltips",     m_bShowTooltips)     &&        // vslot 0xb8
//     reg.Read(L"ShortcutKeys", m_bShowShortcutKeys) && ...    // and further values
// The two statics are named by the export map:
// ?m_bShowTooltips@CMFCToolBar@@1HA at 0x1803b1b64 and
// ?m_bShowShortcutKeys@CMFCToolBar@@1HA at 0x1803b1b60 -- an earlier revision of
// this comment called the first one m_bDontScaleImages, which it is not.
// Not implemented.  A further correction: CSettingsStore/CSettingsStoreSP ARE
// modelled (phase4/src/core/app/CSettingsStore.cpp and CSettingsStoreSP.cpp
// export Open/Read/Create as impl__ thunks) -- the claim that they are not,
// which this comment used to make, is false.  What blocks the transcription is
// that the modelled store is an in-memory map rather than the registry and
// nothing ever writes the MFCToolBarParameters key (SaveParameters below is a
// stub), so every Read would miss and the function would still return FALSE;
// and that CSettingsStoreSP has no header declaration and no destructor thunk,
// so its owner object would have to be hand-rolled and the store it allocates
// would leak.
// Symbol: ?LoadParameters@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadParameters_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    (void)lpszProfileName;
    // TODO(clean-room): not transcribed -- registry read skipped.
    return 0;
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
// Retail (0x180154260):
//     if (GetButtonCount() > 0) AdjustLayout();
//     else SetDrawTextLabels(TRUE);
// (GetButtonCount = vtable slot 0x2f0, AdjustLayout = slot 0x428,
//  SetDrawTextLabels = slot 0x5b8 with edx=1.)
// Symbol: ?OnAfterButtonDelete@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (pThis->GetCount() > 0) {
        pThis->AdjustLayout();
    } else {
        pThis->m_bDrawTextLabels = TRUE;
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
// Retail (0x180157230): copies the image of the selected button (m_iSelected)
// into the user-images store via several internal image-list helpers and global
// static CMFCToolBarImages objects. Not modeled.
// Symbol: ?OnCopyImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCopyImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed -- internal image-copy helpers skipped.
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
    if (!pTI) return -1;

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
// Retail (0x1801587e0) allocates a CMFCToolBarMenuButton (0x128 bytes), sets
// text flags, runs the customize dialog and on IDOK inserts the button via
// vslot 0x690 (InsertButton) at m_iSelected, then AdjustLayout, RedrawWindow
// and the button's vslot 0x50 (OnChangeParentWnd). None of the dialog, the
// insertion virtual or the delete path can be reproduced faithfully.
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
// m_AccelKeys is modelled only as an opaque 56-byte block
// (include/openmfc/afxmfc.h), so the map cannot be populated and nothing is
// transcribed.
// Symbol: ?RebuildAccelerationKeys@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    // TODO(clean-room): not transcribed -- needs the m_AccelKeys CMap internals.
    (void)pThis;
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
// Symbol: ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A = {};
// Symbol: ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
extern "C" CToolBarDropSource_144Bytes impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A = {};
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
// Retail (0x180156cb0) is a WH_MOUSE hook proc that forwards WM_MOUSEMOVE to
// the currently hooked toolbar (global 0x1803be360) and installs/removes the
// hook; the hook-global and the mouse handling are unmodeled.
// Symbol: ?ToolBarMouseHookProc@CMFCToolBar@@KA_JH_K_J@Z
extern "C" __int64 MS_ABI impl__ToolBarMouseHookProc_CMFCToolBar__KA_JH_K_J_Z(
    int nCode, unsigned __int64 wParam, __int64 lParam)
{
    (void)wParam; (void)lParam;
    if (nCode < 0) return 0;
    // TODO(clean-room): transcribed partially -- the global hook handle and
    // toolbar pointer (0x1803be360) are not modeled.
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
// m_pNodeHead/m_pNodeTail/m_nCount/m_pNodeFree/m_pBlocks/m_nBlockSize).  The
// list internals are not modelled, so the storage is right-sized and zeroed;
// retail's image bytes hold only the CList vfptr at +0, which OpenMFC has no
// equivalent for.
// Symbol: ?m_lstBasicCommands@CMFCToolBar@@1V?$CList@II@@A
extern "C" alignas(8) unsigned char impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A[56] = {};

// Symbol: ?m_lstUnpermittedCommands@CMFCToolBar@@1V?$CList@II@@A
extern "C" alignas(8) unsigned char impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A[56] = {};

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
// previous 8-byte blob that was a straight overrun.  Zero-initialised: OpenMFC
// models neither the CMap at +8 nor the two vfptrs the retail image starts
// with (0x1802dfee8 at +0 and 0x1802dfeb8 at +8).
// Symbol: ?m_UsageCount@CMFCToolBar@@1VCMFCCmdUsageCount@@A
extern "C" alignas(8) unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80] = {};

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
//   * SetACCData (CMFCToolBarButton vslot 0x180) and the CAccessibilityData at
//     this+0x150 are not declared in OpenMFC, so that call is skipped.
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

// Retail (0x180156fa0):
//     if (m_lstBasicCommands.Find(uiCmd, NULL) == NULL)
//         m_lstBasicCommands.AddTail(uiCmd);
// (the two calls are 0x180011f1c / 0x180012284 on the static CList<UINT,UINT>
// at 0x1803b2020, which the map names ?m_lstBasicCommands@CMFCToolBar@@1V?$CList@II@@A).
// OpenMFC has no CList<UINT,UINT> implementation and the static above is
// right-sized but inert storage, so nothing is recorded.
// Symbol: ?AddBasicCommand@CMFCToolBar@@SAXI@Z
extern "C" void MS_ABI impl__AddBasicCommand_CMFCToolBar__SAXI_Z(unsigned int uiCmd) {
    (void)uiCmd;
    // TODO(clean-room): not transcribed -- CList<UINT,UINT> Find/AddTail.
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
// Left a stub deliberately: RemoveButton and AdjustLayout are available here,
// but CMFCToolBar::InsertSeparator is itself an unimplemented stub, so a
// half-transcription would delete separators during a customize drag and never
// create them -- an asymmetry that is worse than doing nothing.
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
// either the menu font or afxGlobalData's regular font, walks m_Buttons calling
// OnSetDefaultButtonText (vslot 0x750) for empty labels and OnCalculateSize
// (button vslot 0x48) for each, keeps the maximum height and finally sets
// m_bDrawTextLabels from the comparison against the default button height.
// The blockers are the fonts (afxGlobalData is unmodelled) and the two virtuals
// -- CMFCToolBarButton has no dispatchable vtable here and OnCalculateSize is
// an empty stub, so every height would measure zero.  CClientDC is not a
// blocker: an earlier revision of this comment listed "the DC" among the
// unmodelled pieces, but phase4/src/core/gdi/CClientDC.cpp implements it.
// Note that the retail early return also leaves m_bDrawTextLabels cleared,
// which is the one visible side effect being skipped.
// Symbol: ?CalcMaxButtonHeight@CMFCToolBar@@MEAAHXZ
extern "C" int MS_ABI impl__CalcMaxButtonHeight_CMFCToolBar__MEAAHXZ(CMFCToolBar* pThis) {
    (void)pThis;
    // TODO(clean-room): not transcribed -- needs the button size virtuals and
    // the afxGlobalData fonts.
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
extern "C" void* MS_ABI impl__CreateDroppedButton_CMFCToolBar__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(void* /*class*/* p0) {
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
extern "C" void MS_ABI impl__DoPaint_CMFCToolBar__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

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
extern "C" int MS_ABI impl__DrawButton_CMFCToolBar__MEAAHPEAVCDC__PEAVCMFCToolBarButton__PEAVCMFCToolBarImages__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {
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
extern "C" void MS_ABI impl__DrawDragCursor_CMFCToolBar__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Retail (mfc140.dll RVA 0x154800) is a single forward, fully transcribed:
//     CMFCVisualManager* pVM = CMFCVisualManager::GetInstance();   // 0x97f4
//     pVM->OnDrawSeparator(pDC, this, rect, bHorz);                // vtable +0xa0
// (the CRect argument is copied by value into the outgoing frame; slot 20 of
// the visual-manager vtable is OnDrawSeparator).
// Not implemented here: CMFCVisualManager::GetInstance exists in this tree only
// as a C++ method (phase4/src/featurepack/visualmanager/CMFCVisualManager.cpp),
// not as an impl__ export, so it cannot be called from this translation unit
// without tripping the link rule in AGENTS.md; and the base
// CMFCVisualManager::OnDrawSeparator is itself a stub, so the forward would
// draw nothing.  See the header request filed with this change.
// Symbol: ?DrawSeparator@CMFCToolBar@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCToolBar__MEAAXPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {}

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
extern "C" int MS_ABI impl__DropDownMenu_CMFCToolBar__IEAAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
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
// ids, OpenMFC builds no such menu (CMFCToolBarsCustomizeDialog and
// InsertSeparator/InsertButton are stubs), and the generated signature below is
// also missing its `this`.  Transcribing it half-checked would be a confidently
// wrong body in an entry point real apps can call.
// Symbol: ?EnableContextMenuItems@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@PEAVCMenu@@@Z
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(void* /*class*/* p0, void* /*class*/* p1) {
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
// Not implemented: CMFCCustomizeButton construction and CMFCToolBar::InsertButton
// (the CMFCToolBarButton& overload, vtable slot 211) are both unavailable here.
// Symbol: ?EnableCustomizeButton@CMFCToolBar@@QEAAXHHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__EnableCustomizeButton_CMFCToolBar__QEAAXHHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(int p0, int p1, const void* /*class*/* p2, void** p3, void* p4, int p5) {}

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
extern "C" void MS_ABI impl__EnableCustomizeButton_CMFCToolBar__QEAAXHHIH_Z(int p0, int p1, unsigned int p2, int p3) {}

// Retail (mfc140.dll RVA 0x151070) maps a screen point onto an insertion index
// during a customize drag: it converts the incoming rect with a USER32 call
// (import slot 0x2c5368), reads GetCurrentAlignment() (vtable +0x338) for the
// horizontal/vertical split, then walks m_Buttons comparing the point against
// each button rect and fills the reference rect with the drop marker.
// Not implemented: the walk depends on laid-out button rectangles, which
// OpenMFC does not produce (AdjustLocations and CalcLayout are stubs), so any
// index returned here would be meaningless.
// Symbol: ?FindDropIndex@CMFCToolBar@@MEBAHVCPoint@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__FindDropIndex_CMFCToolBar__MEBAHVCPoint__AEAVCRect___Z(void* /*class*/ p0, void* /*class*/* p1) {
    return 0;
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
extern "C" void* MS_ABI impl__GetDroppedDownMenu_CMFCToolBar__QEBAPEAVCMFCToolBarMenuButton__PEAH_Z(int* p0) {
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

// Symbol: ?GetMenuImageSize@CMFCToolBar@@SA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetRowHeight@CMFCToolBar@@UEBAHXZ
extern "C" int MS_ABI impl__GetRowHeight_CMFCToolBar__UEBAHXZ() {
    return 0;
}

// Symbol: ?HitTest@CMFCToolBar@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?InsertButton@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@H@Z
extern "C" int MS_ABI impl__InsertButton_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?InsertButton@CMFCToolBar@@UEAAHAEBVCMFCToolBarButton@@_J@Z
extern "C" int MS_ABI impl__InsertButton_CMFCToolBar__UEAAHAEBVCMFCToolBarButton___J_Z(const void* /*class*/* p0, __int64 p1) {
    return 0;
}

// Symbol: ?InsertResetStateButton@CMFCToolBar@@IEAAHAEBVCMFCToolBarButton@@H@Z
extern "C" int MS_ABI impl__InsertResetStateButton_CMFCToolBar__IEAAHAEBVCMFCToolBarButton__H_Z(const void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?InsertSeparator@CMFCToolBar@@UEAAH_J@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCToolBar__UEAAH_J_Z(__int64 p0) {
    return 0;
}

// Symbol: ?OnDragLeave@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnDragLeave_CMFCToolBar__MEAAXXZ() {}

// Symbol: ?OnDragOver@CMFCToolBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(void* /*class*/* p0, unsigned long p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnDrop@CMFCToolBar@@MEAAHPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" int MS_ABI impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(void* /*class*/* p0, unsigned long p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnEraseBkgnd@CMFCToolBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolBar__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnGetButton@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButton_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnGetButtonCount@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButtonCount_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnGetButtonText@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetButtonText_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnGetItemRect@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetItemRect_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnGlobalFontsChanged@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__OnGlobalFontsChanged_CMFCToolBar__UEAAXXZ() {}

// Symbol: ?OnHelpHitTest@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnKillFocus@CMFCToolBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnLButtonDblClk@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonDown@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseLeave@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCToolBar__IEAAXXZ() {}

// Symbol: ?OnMouseMove@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCToolBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcCalcSize@CMFCToolBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCToolBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcHitTest@CMFCToolBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCToolBar__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcPaint@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCToolBar__IEAAXXZ() {}

// Symbol: ?OnNeedTipText@CMFCToolBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCToolBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(unsigned int p0, void* /*struct*/* p1, __int64* p2) {
    return 0;
}

// Symbol: ?OnPromptReset@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPromptReset_CMFCToolBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnRButtonDown@CMFCToolBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnSetAccData@CMFCToolBar@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCToolBar__UEAAHJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnSetCursor@CMFCToolBar@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCToolBar__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnSetDefaultButtonText@CMFCToolBar@@UEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ResetAllImages@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__ResetAllImages_CMFCToolBar__SAXXZ() {}

// Symbol: ?ResetImages@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImages_CMFCToolBar__UEAAXXZ() {}

// Still a stub, but the generated signature was missing the `this` argument;
// Deactivate below tail-calls this the way retail does, so it needs the real
// one-argument shape.
// Symbol: ?RestoreFocus@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreFocus_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis) {
    (void)pThis;
    // TODO(clean-room): not transcribed.
}

// Still a stub; signature corrected to take `this` because ResetAll above calls
// it for every live toolbar, exactly as retail does.
// Symbol: ?RestoreOriginalState@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis) {
    (void)pThis;
    // TODO(clean-room): not transcribed.
    return 0;
}

// Symbol: ?SaveOriginalState@CMFCToolBar@@MEAAXAEAVCSettingsStore@@@Z
extern "C" void MS_ABI impl__SaveOriginalState_CMFCToolBar__MEAAXAEAVCSettingsStore___Z(void* /*class*/* p0) {}

// Symbol: ?SaveParameters@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__SaveParameters_CMFCToolBar__SAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?SaveResetOriginalState@CMFCToolBar@@IEAAXAEAVCSettingsStore@@@Z
extern "C" void MS_ABI impl__SaveResetOriginalState_CMFCToolBar__IEAAXAEAVCSettingsStore___Z(void* /*class*/* p0) {}

// Symbol: ?SaveState@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CMFCToolBar@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBar__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetBasicCommands@CMFCToolBar@@SAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetBasicCommands_CMFCToolBar__SAXAEAV__CList_II___Z(void* /*class*/* p0) {}

// Symbol: ?SetButtonInfo@CMFCToolBar@@QEAAXHIIH@Z
extern "C" void MS_ABI impl__SetButtonInfo_CMFCToolBar__QEAAXHIIH_Z(int p0, unsigned int p1, unsigned int p2, int p3) {}

// Symbol: ?SetButtonStyle@CMFCToolBar@@UEAAXHI@Z
extern "C" void MS_ABI impl__SetButtonStyle_CMFCToolBar__UEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?SetButtonText@CMFCToolBar@@QEAAHHPEB_W@Z
extern "C" int MS_ABI impl__SetButtonText_CMFCToolBar__QEAAHHPEB_W_Z(int p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetCommandUsageOptions@CMFCToolBar@@SAHII@Z
extern "C" int MS_ABI impl__SetCommandUsageOptions_CMFCToolBar__SAHII_Z(unsigned int p0, unsigned int p1) {
    return 0;
}

// Symbol: ?SetCustomizeMode@CMFCToolBar@@SAHH@Z
extern "C" int MS_ABI impl__SetCustomizeMode_CMFCToolBar__SAHH_Z(int p0) {
    return 0;
}

// Symbol: ?SetHeight@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetHeight_CMFCToolBar__QEAAXH_Z(int p0) {}

// Symbol: ?SetHelpMode@CMFCToolBar@@SAXH@Z
extern "C" void MS_ABI impl__SetHelpMode_CMFCToolBar__SAXH_Z(int p0) {}

// Symbol: ?SetHot@CMFCToolBar@@QEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?SetHotTextColor@CMFCToolBar@@SAXK@Z
extern "C" void MS_ABI impl__SetHotTextColor_CMFCToolBar__SAXK_Z(unsigned long p0) {}

// Symbol: ?SetLargeIcons@CMFCToolBar@@SAXH@Z
extern "C" void MS_ABI impl__SetLargeIcons_CMFCToolBar__SAXH_Z(int p0) {}

// Symbol: ?SetLockedSizes@CMFCToolBar@@QEAAXUtagSIZE@@0H@Z
extern "C" void MS_ABI impl__SetLockedSizes_CMFCToolBar__QEAAXUtagSIZE__0H_Z(void* /*struct*/ p0, void* /*struct*/ p1, int p2) {}

// Symbol: ?SetMenuSizes@CMFCToolBar@@SAXUtagSIZE@@0@Z
extern "C" void MS_ABI impl__SetMenuSizes_CMFCToolBar__SAXUtagSIZE__0_Z(void* /*struct*/ p0, void* /*struct*/ p1) {}

// Symbol: ?SetNonPermittedCommands@CMFCToolBar@@SAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetNonPermittedCommands_CMFCToolBar__SAXAEAV__CList_II___Z(void* /*class*/* p0) {}

// Symbol: ?SetOneRowWithSibling@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__SetOneRowWithSibling_CMFCToolBar__QEAAXXZ() {}

// Symbol: ?SetOrigButtons@CMFCToolBar@@QEAAXAEBVCObList@@@Z
extern "C" void MS_ABI impl__SetOrigButtons_CMFCToolBar__QEAAXAEBVCObList___Z(const void* /*class*/* p0) {}
