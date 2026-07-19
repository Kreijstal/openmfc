// CMFCRibbonBar methods that the transcribed retail member layout makes
// implementable for real. Each one was decoded from the retail mfc140u export
// (ordinal -> RVA -> objdump) rather than inferred, and the member offsets the
// shipping code uses are exactly the ones in include/openmfc/afxmfc.h (see the
// static_asserts in mfccore.cpp).
//
// Retail offsets referenced below:
//   0x450 = 1104 m_bToolTip
//   0x454 = 1108 m_bToolTipDescr
//   0x458 = 1112 m_bKeyTips
//
// Only two CMFCRibbonBar exports are implemented here. GetCategoryCount is
// deliberately NOT among them: retail reads m_arCategories.m_nSize at 0xb60,
// but OpenMFC's categories live in a side table that AddCategory populates and
// nothing writes that member, so a retail-exact read would report 0 for every
// ribbon this library actually builds. Trading working behaviour for byte
// fidelity is the wrong trade; the existing side-table implementation in
// cbarcore.cpp stays until the CArray members are maintained for real. The
// rest reach through CMFCRibbonCategory / CMFCRibbonBaseElement / the panel
// classes, which OpenMFC does not model; ForceRecalcLayout, for instance, sets
// two flags this file could set and then iterates every category invoking a
// virtual on it. Implementing only the flag half would look like progress
// while silently doing a fraction of the work, so those are left as stubs
// until the element classes exist.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMFCRibbonBar::EnableKeyTips(BOOL) — retail is a single store:
//     mov DWORD PTR [rcx+0x458],edx ; ret
// It stores the argument verbatim, with no normalization to TRUE/FALSE and no
// repaint or layout side effect.
// Symbol: ?EnableKeyTips@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableKeyTips_CMFCRibbonBar__QEAAXH_Z(
    CMFCRibbonBar* pThis, int bEnable)
{
    if (!pThis) return;
    pThis->m_bKeyTips = bEnable;
}

// CMFCRibbonBar::EnableToolTips(BOOL, BOOL) — retail stores both arguments and
// returns:
//     mov DWORD PTR [rcx+0x450],edx    ; m_bToolTip
//     mov DWORD PTR [rcx+0x454],r8d    ; m_bToolTipDescr
//     ret
// Note the second flag is stored unconditionally -- it is not gated on the
// first -- so a caller can disable tooltips while leaving the description flag
// set, and re-enabling restores the previous description behaviour.
// Symbol: ?EnableToolTips@CMFCRibbonBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableToolTips_CMFCRibbonBar__QEAAXHH_Z(
    CMFCRibbonBar* pThis, int bEnable, int bEnableDescr)
{
    if (!pThis) return;
    pThis->m_bToolTip      = bEnable;
    pThis->m_bToolTipDescr = bEnableDescr;
}
