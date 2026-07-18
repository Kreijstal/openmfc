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
