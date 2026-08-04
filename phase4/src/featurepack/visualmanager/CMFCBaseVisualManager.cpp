// CMFCBaseVisualManager — OpenMFC implementation.
// Sources: global_cmfcbasevisualmanager.cpp, manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCBaseVisualManager exports.
//
// All 9 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is taken
// as void*.  The retail bodies draw through the theme handles stored at
// +0x08/+0x18/+0x20/+0x30 (m_hTheme...), clean up the theme brushes at
// +0x08..+0x78 and probe the active visual style; none of that state is
// modeled, so every body terminates on the retail NULL-theme default (draw
// methods return 0, mutators become no-ops).
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleacc.h>

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CMFCBaseVisualManager - not declared in the OpenMFC headers (opaque), so
// pThis is taken as void*. The retail bodies read the theme handles stored at
// +0x08/+0x18/+0x20/+0x30 (m_hTheme...) and the theme brushes at +0x08..+0x78
// (see the constructor, manual_small_stub_implementations.cpp); none of that
// state is modeled, so every body terminates on the retail NULL-theme default
// and the mutators become no-ops.
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
// Retail (0x18018f900): deletes the per-theme brush handles at +0x08..+0x78
// (DestroyObject-style call at 0x1802c7390) when non-NULL. No CMFCBaseVisualManager
// members are modeled, so there is nothing to clean up.
// Symbol: ?CleanUpThemes@CMFCBaseVisualManager@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpThemes_CMFCBaseVisualManager__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- retail deletes the theme brushes at
    // +0x08..+0x78 (CloseThemeData/DeleteObject at 0x1802c7390); the members
    // are not modeled.
}
// Retail (0x18018f470): resolves the active visual manager (0x180133a20), the
// current frame windows and the theme brush colors, storing the created
// brushes at +0x08..+0x78 (brush-creation call at 0x1802c7388). No
// CMFCBaseVisualManager members are modeled, so the stores are skipped.
// Symbol: ?UpdateSystemColors@CMFCBaseVisualManager@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateSystemColors_CMFCBaseVisualManager__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- retail stores the resolved theme
    // brushes at +0x08..+0x78 (GetThemeColor / brush creation at 0x1802c7388);
    // the members are not modeled.
}
// Retail (0x180190180): probes the active visual style (OpenThemeData /
// GetThemeString at 0x1802c7398 / 0x1802c7788) and maps the recognized
// "Aero"/"Luna" class names to the WinXpTheme enum, returning -1/0 when no
// standard theme matches. The theme state is unmodeled, so WinXpThemeNone (0)
// -- the retail "no standard theme" terminal -- is returned.
// Symbol: ?GetStandardWindowsTheme@CMFCBaseVisualManager@@UEAA?AW4WinXpTheme@1@XZ
extern "C" int MS_ABI impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(
    void* pThis)
{
    if (!pThis) return 0;
    // TODO(clean-room): not transcribed -- retail probes the active visual
    // style with OpenThemeData/GetThemeString and maps the "Aero"/"Luna" class
    // names onto the WinXpTheme enum; the theme state is not modeled.
    return 0;
}
// Retail (0x180190010): returns 0 when the checkbox theme handle at +0x20 is
// NULL, otherwise computes the BP_CHECKBOX state from the bDisabled/bChecked/
// bHot/bPressed flags and draws it with DrawThemeBackground (0x1802c7378),
// returning 1. The theme handle is unmodeled (NULL), so the retail
// NULL-theme terminal 0 is returned.
// Symbol: ?DrawCheckBox@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
extern "C" int MS_ABI impl__DrawCheckBox_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/, int /*bChecked*/,
    int /*bHot*/, int /*bPressed*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- retail draws through the
    // m_hThemeCheckBox handle at +0x20 (DrawThemeBackground at 0x1802c7378);
    // the theme-handle member is not modeled.
    return FALSE;
}
// Retail (0x18018fd90): returns 0 when the combo theme handle at +0x08 is
// NULL; otherwise, when bIsDropped or bIsHighlighted is set, it draws the
// combo border with the theme and returns 1. The theme handle is unmodeled
// (NULL), so the retail NULL-theme terminal 0 is returned.
// Symbol: ?DrawComboBorder@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHH@Z
extern "C" int MS_ABI impl__DrawComboBorder_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/,
    int /*bIsDropped*/, int /*bIsHighlighted*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- retail draws through the combo theme
    // handle at +0x08 (DrawThemeBackground / fill at 0x1802a5bc0); the
    // theme-handle member is not modeled.
    return FALSE;
}
// Retail (0x18018fd20): returns 0 when the combo theme handle at +0x30 is
// NULL, otherwise computes the CBXS state from the bDisabled/bIsDropped/
// bIsHighlighted flags and draws it with DrawThemeBackground (0x1802c7378),
// returning 1. The theme handle is unmodeled (NULL), so the retail
// NULL-theme terminal 0 is returned.
// Symbol: ?DrawComboDropButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHH@Z
extern "C" int MS_ABI impl__DrawComboDropButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/,
    int /*bIsDropped*/, int /*bIsHighlighted*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- retail draws through the combo
    // theme handle at +0x30 (DrawThemeBackground at 0x1802c7378); the
    // theme-handle member is not modeled.
    return FALSE;
}
// Retail (0x18018fa40): returns 0 when the button theme handle at +0x20 is
// NULL; otherwise it computes the BP_PUSHBUTTON state from the CMFCButton
// flags / focus and draws it with DrawThemeBackground (0x1802c7378),
// returning 1. The theme handle is unmodeled (NULL), so the retail
// NULL-theme terminal 0 is returned.
// Symbol: ?DrawPushButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCButton@@I@Z
extern "C" int MS_ABI impl__DrawPushButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__PEAVCMFCButton__I_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/,
    unsigned int /*uiState*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- retail draws through the push-button
    // theme handle at +0x20 (DrawThemeBackground at 0x1802c7378) using the
    // button state; the theme-handle member is not modeled.
    return FALSE;
}
// Retail (0x1801900e0): returns 0 when the radio theme handle at +0x20 is
// NULL, otherwise computes the BP_RADIOBUTTON state from the bDisabled/
// bChecked/bHot/bPressed flags and draws it with DrawThemeBackground
// (0x1802c7378), returning 1. The theme handle is unmodeled (NULL), so the
// retail NULL-theme terminal 0 is returned.
// Symbol: ?DrawRadioButton@CMFCBaseVisualManager@@UEAAHPEAVCDC@@VCRect@@HHHH@Z
extern "C" int MS_ABI impl__DrawRadioButton_CMFCBaseVisualManager__UEAAHPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bDisabled*/, int /*bChecked*/,
    int /*bHot*/, int /*bPressed*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- retail draws through the radio
    // theme handle at +0x20 (DrawThemeBackground at 0x1802c7378); the
    // theme-handle member is not modeled.
    return FALSE;
}
// Retail (0x18018fe40): fills rectClient on pDC with the rebar theme at +0x18
// when present, otherwise with the global clrBarFace color (0x1803c1740), and
// then expands the rect from the pane's window rect. The theme handle, the
// global color and the pane geometry path are unmodeled.
// Symbol: ?FillReBarPane@CMFCBaseVisualManager@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@@Z
extern "C" void MS_ABI impl__FillReBarPane_CMFCBaseVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CBasePane* /*pBar*/, CRect /*rectClient*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- retail fills through the rebar theme
    // handle at +0x18 or the global clrBarFace color (0x1803c1740) and expands
    // the rect from the pane's window rect; none are modeled.
}
// Symbol: ??0CMFCBaseVisualManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseVisualManager__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCBaseVisualManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseVisualManager__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?DrawStatusBarProgress@CMFCBaseVisualManager@@UEAAHPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" int MS_ABI impl__DrawStatusBarProgress_CMFCBaseVisualManager__UEAAHPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    void* /*CMFCBaseVisualManager**/ pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    (void)pThis;
    // Delegate to the same drawing logic as CMFCVisualManager::OnDrawStatusBarProgress.
    // CMFCBaseVisualManager is the abstract root; the real impl is in CMFCVisualManager.
    if (CMFCVisualManager* mgr = CMFCVisualManager::GetInstance()) {
        mgr->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                     clrBar, clrProgressBarDest, clrProgressText, (BOOL)bProgressText);
    }
    return 1;  // handled
}
