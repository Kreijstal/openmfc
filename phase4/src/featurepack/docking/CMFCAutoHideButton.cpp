// CMFCAutoHideButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfcautohidebutton.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCAutoHideButtonSupport.h"

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
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons[pThis].highlighted = bHighlight ? TRUE : FALSE;
}
// Symbol: ?IsHorizontal@CMFCAutoHideButton@@QEBAHXZ
extern "C" int MS_ABI impl__IsHorizontal_CMFCAutoHideButton__QEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_autoHideButtons[pThis].horizontal;
}
// Symbol: ?GetSize@CMFCAutoHideButton@@QEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetSize_CMFCAutoHideButton__QEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 96 : 80;
    pRet->cy = 22;
}
// Symbol: ?GetTextSize@CMFCAutoHideButton@@UEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetTextSize_CMFCAutoHideButton__UEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 88 : 72;
    pRet->cy = 16;
}
// Retail (0x1800098c0): stores the bar/pane/style into +0x30/+0x38/+0x14,
// records the back-pointers (pPane->+0x480 = bar, pPane->+0x478 = this), then
// sizes the button (GetSize 0x180009940) and creates the window via
// CreateWindowExW (IAT 0x1802c7140) and finally stores 1 into +0xc.  The
// pane back-pointers (+0x478/+0x480) and the window creation are driven by
// the unmodeled GetSize helper, so the retail success terminal 1 is returned
// without creating anything.
// Symbol: ?Create@CMFCAutoHideButton@@UEAAHPEAVCMFCAutoHideBar@@PEAVCDockablePane@@K@Z
extern "C" int MS_ABI impl__Create_CMFCAutoHideButton__UEAAHPEAVCMFCAutoHideBar__PEAVCDockablePane__K_Z(
    void* pThis, CMFCAutoHideBar* /*pAutoHideBar*/, CDockablePane* /*pPane*/,
    unsigned long /*dwControlBarStyle*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- the +0x30/+0x38/+0x14 stores,
    // the pane back-pointers (+0x478/+0x480) and the CreateWindowExW call
    // (driven by the unmodeled GetSize helper) are not modeled; the retail
    // success terminal 1 is returned.
    return TRUE;
}
// Retail (0x1800097f0) allocates a 0x40-byte object and runs the constructor
// (0x180009830) which installs the retail vftable (0x1802db808).  The class
// is opaque in OpenMFC -- no constructor or vftable is modeled -- so no valid
// object can be produced.
// Symbol: ?CreateObject@CMFCAutoHideButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCAutoHideButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x40 bytes
    // and runs the CMFCAutoHideButton constructor (RVA 0x180009830, retail
    // vftable at 0x1802db808); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}
// Retail (0x180009920): when the +0x14 style carries the 0xa000 bits, the
// button is pinned and the window is moved to x=0 (r8d=0); otherwise the
// caller's offset is passed through.  The move is a SetWindowPos-family IAT
// call (0x1802c72f0) with the rect at +0x1c.  The rect member and the call
// are unmodeled.
// Symbol: ?Move@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void MS_ABI impl__Move_CMFCAutoHideButton__QEAAXH_Z(
    void* pThis, int /*nOffset*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x14 style check and the
    // SetWindowPos-family move on the +0x1c rect are not modeled.
}
// Retail (0x180009b90): resolves the visual manager (0x180009774), checks the
// +0x14 style, paints the auto-hide button (text/bitmap/image list via the
// visual-manager vslots 0x368/0x370/0x378) and updates the +0x10 mouse-over
// flag.  The visual manager and the button style/members are unmodeled.
// Symbol: ?OnDraw@CMFCAutoHideButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCAutoHideButton__UEAAXPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager vslots
    // (0x368/0x370/0x378) and the +0x14/+0x10 button state; not modeled.
}
// Retail (0x18000a1d0): delegates to the visual manager's vslot-0x378 border
// routine (rects forwarded).  The visual manager is unmodeled.
// Symbol: ?OnDrawBorder@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@1@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect__1_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rectBorder*/, CRect /*rectBorder2*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager vslot-0x378
    // border routine; not modeled.
}
// Retail (0x18000a170): delegates to the visual manager's vslot-0x370 fill
// routine (rect and the button pointer forwarded).  The visual manager is
// unmodeled.
// Symbol: ?OnFillBackground@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the visual-manager vslot-0x370
    // fill routine; not modeled.
}
// Retail (0x18000a4b0): pure member surgery: +0x38 = pNewPane, and the pane
// back-pointers pNewPane->+0x480 = +0x30 and pNewPane->+0x478 = this.  The
// offsets are harvested, but the pane layout is not modeled in OpenMFC, so
// the stores are not performed.
// Symbol: ?ReplacePane@CMFCAutoHideButton@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__ReplacePane_CMFCAutoHideButton__QEAAXPEAVCDockablePane___Z(
    void* pThis, CDockablePane* /*pNewPane*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x38 store and the pane
    // back-pointers (+0x480 = bar, +0x478 = this) are not modeled.
}
// Retail (0x18000a3f0): shows/hides the attached pane window (+0x38 -> its
// hwnd at +0x40, IsWindowVisible-checked) and drives the pane's vslot-0x698
// and the bar's vslot-0x530 virtuals with bShow.  The pane/bar pointers and
// their virtuals are unmodeled.
// Symbol: ?ShowAttachedWindow@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowAttachedWindow_CMFCAutoHideButton__QEAAXH_Z(
    void* pThis, int /*bShow*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x38 pane, the +0x30 bar
    // and their vslot-0x698/0x530 virtuals; not modeled.
}
// Retail (0x18000a460): `mov %edx,0xc(%rcx); ret` -- stores bShow into +0xc.
// Pure member store; transcribed faithfully through the layout mirror.
// Symbol: ?ShowButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void MS_ABI impl__ShowButton_CMFCAutoHideButton__UEAAXH_Z(
    void* pThis, int bShow)
{
    if (!pThis) return;
    CAutoHideButton* s = reinterpret_cast<CAutoHideButton*>(pThis);
    s->m_nShow = bShow;
}
// Retail (0x18000a480): loads the auto-hide bar stored at +0x38 and, when it
// is non-NULL, invokes its vslot-0x6d0 virtual, forwarding the CDockablePane*
// argument (left in %rdx). The +0x38 member and the bar virtual are not
// modeled, so no call is made.
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideButton@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CMFCAutoHideButton__UEAAXPEAVCDockablePane___Z(
    void* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- retail loads the auto-hide bar
    // at +0x38 and calls its vslot-0x6d0 virtual (UnSetAutoHideMode); neither
    // the member nor the bar virtual is modeled.
}
