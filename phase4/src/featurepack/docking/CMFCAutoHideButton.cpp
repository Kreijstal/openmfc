// CMFCAutoHideButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfcautohidebutton.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCAutoHideButtonSupport.h"

extern "C" void MS_ABI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
    CDockablePane* pThis, void* pAutoHideBar, void* pAutoHideButton);

// CreateObject's retail allocation size (factory entry RVA 0x97f0, mfc140u).
static_assert(sizeof(CAutoHideButton) == 0x40,
              "CMFCAutoHideButton retail allocation size");

// Symbol: ??0CMFCAutoHideButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAutoHideButton__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons[pThis] = { FALSE, FALSE };
    return pThis;
}
// Symbol: ??1CMFCAutoHideButton@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCAutoHideButton__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons.erase(pThis);
    return pThis;
}
// Symbol: ?HighlightButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void MS_ABI impl__HighlightButton_CMFCAutoHideButton__UEAAXH_Z(void* pThis, int bHighlight) {
    if (!pThis) return;
    // Retail RVA 0xa470 (mfc140u) stores the raw argument at +0x10.
    *reinterpret_cast<int*>(static_cast<unsigned char*>(pThis) + 0x10) = bHighlight;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons[pThis].highlighted = bHighlight ? TRUE : FALSE;
}
// Retail RVA 0xa3e0 (mfc140u) tests the style bits at +0x14.
// Symbol: ?IsHorizontal@CMFCAutoHideButton@@QEBAHXZ
extern "C" int MS_ABI impl__IsHorizontal_CMFCAutoHideButton__QEBAHXZ(void* pThis) {
    if (!pThis) return FALSE;
    const CAutoHideButton* s = reinterpret_cast<const CAutoHideButton*>(pThis);
    return (s->m_dwStyle & 0xa000UL) != 0;
}
// Approximation only: retail GetSize at RVA 0x9940 (mfc140u) derives the
// result from visual-manager, style, pane and bar state; these constants are
// not a transcription of that calculation.
// Symbol: ?GetSize@CMFCAutoHideButton@@QEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetSize_CMFCAutoHideButton__QEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 96 : 80;
    pRet->cy = 22;
}
// Approximation only: retail GetTextSize at RVA 0xa230 (mfc140u) uses the
// attached pane/bar and text metrics; these constants are not a transcription.
// Symbol: ?GetTextSize@CMFCAutoHideButton@@UEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetTextSize_CMFCAutoHideButton__UEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 88 : 72;
    pRet->cy = 16;
}
// Retail RVA 0x98c0 (mfc140u) stores the bar/pane/style at +0x30/+0x38/+0x14,
// writes pane back-pointers at +0x480/+0x478, calls GetSize (RVA 0x9940),
// calls SetRect on this object's +0x1c RECT, then stores 1 at +0x0c. The IAT
// call here is SetRect, not window creation. OpenMFC redirects the pane links
// through SetAutoHideParents because those retail offsets exceed its pane
// storage; the size remains approximate because GetSize is approximate.
// Symbol: ?Create@CMFCAutoHideButton@@UEAAHPEAVCMFCAutoHideBar@@PEAVCDockablePane@@K@Z
extern "C" int MS_ABI impl__Create_CMFCAutoHideButton__UEAAHPEAVCMFCAutoHideBar__PEAVCDockablePane__K_Z(
    void* pThis, CMFCAutoHideBar* pAutoHideBar, CDockablePane* pPane,
    unsigned long dwControlBarStyle)
{
    if (!pThis || !pPane) return FALSE;
    CAutoHideButton* s = reinterpret_cast<CAutoHideButton*>(pThis);
    s->m_pAutoHideBar = pAutoHideBar;
    s->m_pPane = pPane;
    s->m_dwStyle = dwControlBarStyle;
    impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
        pPane, pAutoHideBar, pThis);

    CSize size;
    impl__GetSize_CMFCAutoHideButton__QEBA_AVCSize__XZ(&size, pThis);
    ::SetRect(&s->m_rect, 0, 0, size.cx, size.cy);
    s->m_nShow = TRUE;
    return TRUE;
}
// The retail factory at RVA 0x97f0 (mfc140u) allocates 0x40 bytes, returns
// NULL only when allocation fails, and otherwise tail-jumps to the constructor
// at RVA 0x9830 (mfc140u). That constructor installs a derived-class vftable.
// OpenMFC has no CMFCAutoHideButton C++ instance type or compatible instance
// vftable (its runtime-class descriptor is metadata only), and the local
// constructor thunk does not install one; raw storage cannot safely be returned.
// Symbol: ?CreateObject@CMFCAutoHideButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCAutoHideButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): leave stubbed until a usable CMFCAutoHideButton
    // vftable/constructor is modeled; returning raw storage would not create
    // a valid polymorphic object.
    return nullptr;
}
// Retail RVA 0x9920 (mfc140u) offsets the +0x1c RECT with ::OffsetRect:
// style bits 0xa000 select (nOffset, 0), otherwise it uses (0, nOffset).
// Symbol: ?Move@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void MS_ABI impl__Move_CMFCAutoHideButton__QEAAXH_Z(
    void* pThis, int nOffset)
{
    if (!pThis) return; // defensive; retail dereferences this unconditionally
    CAutoHideButton* s = reinterpret_cast<CAutoHideButton*>(pThis);
    if (s->m_dwStyle & 0xa000UL)
        ::OffsetRect(&s->m_rect, nOffset, 0);
    else
        ::OffsetRect(&s->m_rect, 0, nOffset);
}
// Retail RVA 0x9b90 (mfc140u) obtains the visual manager (RVA 0x9774), calls
// its vslots 0x368 and 0x380, reads style/rect/bar/pane state, and dispatches
// through this object's vslots 0x58 and 0x60. It does not write the +0x10
// highlight field. This body remains a stub because those dispatches and the
// instance vtable are not modeled.
// Symbol: ?OnDraw@CMFCAutoHideButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCAutoHideButton__UEAAXPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager and
    // button-instance virtual dispatches described above.
}
// Retail RVA 0xa1d0 (mfc140u) delegates to the visual manager's vslot 0x378,
// forwarding both RECT arguments. The visual-manager dispatch is not modeled.
// Symbol: ?OnDrawBorder@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@1@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect__1_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rectBorder*/, CRect /*rectBorder2*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager vslot-0x378
    // border routine; not modeled.
}
// Retail body at RVA 0xa170 (mfc140u) delegates to the visual manager's
// vslot 0x370, forwarding the RECT, CDC pointer and button pointer. This body
// remains a stub because the visual-manager dispatch is not modeled.
// Symbol: ?OnFillBackground@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager vslot-0x370
    // fill routine; not modeled.
}
// Retail body at RVA 0xa4b0 (mfc140u) sets this->+0x38 = pNewPane, then
// writes the bar/button back-pointers at pNewPane+0x480/+0x478. OpenMFC stores
// those links through CDockablePane's side-table helper instead of raw offsets.
// Symbol: ?ReplacePane@CMFCAutoHideButton@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__ReplacePane_CMFCAutoHideButton__QEAAXPEAVCDockablePane___Z(
    void* pThis, CDockablePane* pNewPane)
{
    if (!pThis) return;
    CAutoHideButton* s = reinterpret_cast<CAutoHideButton*>(pThis);
    s->m_pPane = pNewPane;
    if (pNewPane != nullptr) {
        impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
            pNewPane, s->m_pAutoHideBar, pThis);
    }
}
// Retail body at RVA 0xa3f0 (mfc140u) reads the attached pane at +0x38 and
// its HWND at +0x40, checks ::IsWindowVisible, conditionally calls the pane's
// vslot 0x698 with (bShow, TRUE), then tail-calls the bar's vslot 0x530 with
// bShow. The pane/bar pointers are in the layout mirror, but these virtual
// dispatches need a usable instance vtable and remain unmodeled.
// Symbol: ?ShowAttachedWindow@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowAttachedWindow_CMFCAutoHideButton__QEAAXH_Z(
    void* pThis, int /*bShow*/)
{
    if (!pThis) return;
    // TODO(clean-room): requires pane/bar instances with usable vtables for
    // the conditional pane call and final bar call.
}
// Retail RVA 0xa460 (mfc140u) stores bShow at +0x0c. The extra null check
// below is a defensive deviation from retail's unconditional store.
// Symbol: ?ShowButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void MS_ABI impl__ShowButton_CMFCAutoHideButton__UEAAXH_Z(
    void* pThis, int bShow)
{
    if (!pThis) return;
    CAutoHideButton* s = reinterpret_cast<CAutoHideButton*>(pThis);
    s->m_nShow = bShow;
}
// Retail body at RVA 0xa480 (mfc140u) loads the attached pane from +0x38 and,
// when non-NULL, calls its vslot 0x6d0. The pane field is in the layout mirror,
// but the instance vtable dispatch cannot be reproduced safely here.
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideButton@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CMFCAutoHideButton__UEAAXPEAVCDockablePane___Z(
    void* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return;
    // TODO(clean-room): retail calls the attached pane's vslot 0x6d0; the
    // instance vtable dispatch is not modeled.
}
