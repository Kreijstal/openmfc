// CMFCBaseVisualManager — OpenMFC implementation.
// Sources: global_cmfcbasevisualmanager.cpp, manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCBaseVisualManager exports. The class itself is opaque in this
// translation unit and has no corresponding shadow-layout header here. Several
// bodies below are intentionally conservative placeholders; their constant
// returns/no-ops are not evidence that retail's theme handles are null or that
// the retail theme-dependent behavior has been reproduced.
//
// Export definitions use explicit `this` parameters and the MSVC x64 ABI. The
// `// Symbol:` markers exclude generated weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <uxtheme.h>
#include <oleacc.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// Retail CMFCBaseVisualManager's 19 HTHEME slots are contiguous after its
// vfptr. Their order below matches the individual accesses in CleanUpThemes
// at RVA 0x18f900 (mfc140u); OpenMFC stores the same byte range in
// CMFCVisualManager::_visualmanager_padding.
constexpr std::size_t kThemeHandleOffsets[] = {
    0x08, 0x10, 0x18, 0x28, 0x20, 0x30, 0x38, 0x40, 0x48, 0x50,
    0x58, 0x60, 0x68, 0x70, 0x78, 0x88, 0x80, 0x90, 0x98,
};
constexpr std::size_t kThemeHandleBankSize =
    sizeof(kThemeHandleOffsets) / sizeof(kThemeHandleOffsets[0]) * sizeof(HTHEME);
static_assert(sizeof(HTHEME) == 8, "theme handles are 64-bit in this x64 ABI");
static_assert(sizeof(CObject) == 8, "CMFCBaseVisualManager vfptr prefix is 8 bytes");
static_assert(kThemeHandleOffsets[0] == sizeof(void*) &&
                  kThemeHandleOffsets[sizeof(kThemeHandleOffsets) / sizeof(kThemeHandleOffsets[0]) - 1]
                          + sizeof(HTHEME) == sizeof(void*) + kThemeHandleBankSize,
              "retail HTHEME slots span the expected contiguous byte range");
static_assert(sizeof(CMFCVisualManager) >= 0xa0,
              "OpenMFC visual-manager storage covers the retail theme-handle bank");
}

//=============================================================================
// CMFCBaseVisualManager - opaque in OpenMFC; retail has theme-dependent state
// and behavior that the placeholder bodies below do not reproduce.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::CleanUpThemes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::UpdateSystemColors
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::GetStandardWindowsTheme
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::DrawCheckBox
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::DrawComboBorder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::DrawComboDropButton
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::DrawPushButton
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::DrawRadioButton
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseVisualManager::FillReBarPane
//-----------------------------------------------------------------------------
// Retail RVA 0x18f900 (mfc140u) checks 19 theme-handle fields in the order
// below and closes each non-null HTHEME. OpenMFC's CMFCVisualManager reserves
// this retail byte range in its zeroed padding; the storage bounds are pinned
// above so cleanup can release handles without inventing a vftable address.
// Symbol: ?CleanUpThemes@CMFCBaseVisualManager@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpThemes_CMFCBaseVisualManager__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    auto* bytes = static_cast<unsigned char*>(pThis);
    for (std::size_t offset : kThemeHandleOffsets) {
        HTHEME theme = nullptr;
        std::memcpy(&theme, bytes + offset, sizeof(theme));
        if (theme != nullptr) {
            ::CloseThemeData(theme);
        }
    }
}
// The retail body at RVA 0x18f470 (mfc140u) opens theme resources and stores
// their handles in this object's theme-state fields. The OpenMFC layout and
// setup path are not modeled here, so this placeholder performs no updates.
// Symbol: ?UpdateSystemColors@CMFCBaseVisualManager@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateSystemColors_CMFCBaseVisualManager__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // Deliberate placeholder: no theme handles are opened or stored here.
}
// Retail RVA 0x190180 (mfc140u) queries visual-style/theme properties and can
// return -1 or a mapped WinXpTheme value. This placeholder returns 0
// unconditionally; that is not a transcription of the retail query path.
// Symbol: ?GetStandardWindowsTheme@CMFCBaseVisualManager@@UEAA?AW4WinXpTheme@1@XZ
extern "C" int MS_ABI impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(
    void* pThis)
{
    if (!pThis) return 0;
    // Placeholder: the retail visual-style query and theme-name mapping are
    // not implemented.
    return 0;
}
// Retail RVA 0x190010 (mfc140u) returns 0 for a null checkbox theme handle at
// +0x20; otherwise it selects a state and draws. This placeholder always
// returns FALSE and does not establish that the handle is null.
// Symbol: ?DrawCheckBox@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
extern "C" int MS_ABI impl__DrawCheckBox_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/, int /*bChecked*/,
    int /*bHot*/, int /*bPressed*/)
{
    if (!pThis) return FALSE;
    // The theme-dependent retail path is not implemented.
    return FALSE;
}
// Retail RVA 0x18fd90 (mfc140u) returns 0 when the combo theme handle at +0x08
// is null. With a non-null handle it returns 1; it draws for dropped or
// highlighted states, but also has a handled-without-drawing branch when both
// flags are clear. This placeholder always returns FALSE.
// Symbol: ?DrawComboBorder@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHH@Z
extern "C" int MS_ABI impl__DrawComboBorder_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/,
    int /*bIsDropped*/, int /*bIsHighlighted*/)
{
    if (!pThis) return FALSE;
    // The themed and handled-without-drawing retail paths are not implemented.
    return FALSE;
}
// Retail RVA 0x18fd20 (mfc140u) returns 0 for a null combo-drop theme handle at
// +0x30; otherwise it selects a state, draws, and returns 1. This placeholder
// always returns FALSE and does not establish that the handle is null.
// Symbol: ?DrawComboDropButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHH@Z
extern "C" int MS_ABI impl__DrawComboDropButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/,
    int /*bIsDropped*/, int /*bIsHighlighted*/)
{
    if (!pThis) return FALSE;
    // The theme-dependent retail path is not implemented.
    return FALSE;
}
// Retail RVA 0x18fa40 (mfc140u) returns 0 for a null button theme handle at
// +0x20; otherwise it inspects button state and draws, returning 1. This
// placeholder always returns FALSE and does not establish that the handle is
// null.
// Symbol: ?DrawPushButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCButton@@I@Z
extern "C" int MS_ABI impl__DrawPushButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__PEAVCMFCButton__I_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/,
    unsigned int /*uiState*/)
{
    if (!pThis) return FALSE;
    // The theme-dependent retail path is not implemented.
    return FALSE;
}
// Retail RVA 0x1900e0 (mfc140u) returns 0 for a null radio theme handle at
// +0x20; otherwise it selects a state, draws, and returns 1. This placeholder
// always returns FALSE and does not establish that the handle is null.
// Symbol: ?DrawRadioButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
extern "C" int MS_ABI impl__DrawRadioButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/, int /*bChecked*/,
    int /*bHot*/, int /*bPressed*/)
{
    if (!pThis) return FALSE;
    // The theme-dependent retail path is not implemented.
    return FALSE;
}
// Retail RVA 0x18fe40 (mfc140u) branches on the rebar theme handle at +0x18:
// the null-handle path fills with a global color, while the themed path derives
// and clips geometry using pane/theme data before drawing. Those inputs and
// layout are not modeled by this placeholder.
// Symbol: ?FillReBarPane@CMFCBaseVisualManager@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@@Z
extern "C" void MS_ABI impl__FillReBarPane_CMFCBaseVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CBasePane* /*pBar*/, CRect /*rectClient*/)
{
    if (!pThis) return;
    // The themed drawing and global-color fallback are not implemented.
}
// Retail RVA 0x18f390 (mfc140u) installs its base vftable, clears these 19
// handles, and calls UpdateSystemColors. OpenMFC has no matching base vftable;
// this thunk safely initializes the known handle storage but cannot reproduce
// the vftable store or the unavailable theme-opening path.
// Symbol: ??0CMFCBaseVisualManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseVisualManager__QEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        std::memset(static_cast<unsigned char*>(pThis) + sizeof(void*), 0,
                    kThemeHandleBankSize);
    }
    return pThis;
}
// Retail RVA 0x18f460 (mfc140u) stores the base vftable at [this] and
// tail-jumps to CleanUpThemes at RVA 0x18f900 (mfc140u). The OpenMFC base
// vftable is unavailable, so this preserves the cleanup behavior without
// fabricating or embedding a retail-image address; only the vfptr reset is
// omitted.
// Symbol: ??1CMFCBaseVisualManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseVisualManager__UEAA_XZ(void* pThis) {
    impl__CleanUpThemes_CMFCBaseVisualManager__IEAAXXZ(pThis);
}
// Retail RVA 0x18fb40 (mfc140u) returns handled without drawing when the
// progress theme handle at +0x38 is null. Otherwise it draws the themed track;
// it draws the progress portion only when the total is nonzero and the text
// only when requested. This pre-existing OpenMFC body delegates to
// CMFCVisualManager instead, a known deviation from that retail path.
// Symbol: ?DrawStatusBarProgress@CMFCBaseVisualManager@@UEAAHPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" int MS_ABI impl__DrawStatusBarProgress_CMFCBaseVisualManager__UEAAHPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    void* /*CMFCBaseVisualManager**/ pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    (void)pThis;
    // This delegation is an OpenMFC approximation, not the retail base theme path.
    if (CMFCVisualManager* mgr = CMFCVisualManager::GetInstance()) {
        mgr->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                     clrBar, clrProgressBarDest, clrProgressText, (BOOL)bProgressText);
    }
    return 1;  // handled
}
