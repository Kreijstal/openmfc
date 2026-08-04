// CMFCAcceleratorKeyAssignCtrl — OpenMFC implementation.
// Sources: global_cmfcacceleratorkeyassignctrl.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCAcceleratorKeyAssignCtrlSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail (0x180003770): `movl $0x0,0x108(%rcx); jmp CWnd::Default`
// (0x18028ac80).  The +0x108 marker is unmodeled; the Default tail is kept.
// Symbol: ?OnKillFocus@CMFCAcceleratorKeyAssignCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCAcceleratorKeyAssignCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* /*pNewWnd*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x108 kill-focus marker
    // is not modeled; the CWnd::Default tail is kept.
    impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// Retail (0x180003530): a state machine over +0xe8/+0xec/+0xee (virtual-key
// tracking), +0xf8 (a CString, formatted via CMFCAcceleratorKey::Format
// 0x1800028f0) and +0x108.  For the WM_KEYDOWN/WM_SYSKEYDOWN paths it arms
// SetFocus / SetWindowTextW and returns 1; everything else falls through to
// CWnd::PreTranslateMessage (0x18028bc00).  The tracked state and the CString
// are unmodeled, so the fall-through is the only reproducible behavior.
// Symbol: ?PreTranslateMessage@CMFCAcceleratorKeyAssignCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCAcceleratorKeyAssignCtrl__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- the +0xe8/+0xec/+0xee/+0xf8/
    // +0x108 key-tracking state machine and its SetFocus/SetWindowTextW
    // side effects are not modeled; the CWnd::PreTranslateMessage fall-through
    // is kept.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
        reinterpret_cast<CWnd*>(pThis), pMsg);
}
// Retail (0x180003700): clears the 6 flag bytes +0xec..+0xf1 (memset helper
// IAT 0x1802c7418), stores 0 into +0xe8 and, when the window exists (+0x40),
// calls SetWindowTextW(hwnd, L"") (0x1802a9790).  All three effects are kept
// through the layout mirror.
// Symbol: ?ResetKey@CMFCAcceleratorKeyAssignCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    CAccelKeyAssignCtrl* s = reinterpret_cast<CAccelKeyAssignCtrl*>(pThis);
    s->m_bAccelFlags = 0;                              // +0xe8
    std::memset(&s->m_flags, 0, 6);                    // +0xec..+0xf1
    if (s->m_hWnd) {
        ::SetWindowTextW(s->m_hWnd, L"");
    }
}
// Retail (0x180003750): when bOn is set, `+0xec |= flag`; otherwise
// `+0xec &= ~flag`.  Pure byte-flag operation; transcribed faithfully.
// Symbol: ?SetAccelFlag@CMFCAcceleratorKeyAssignCtrl@@IEAAXEH@Z
extern "C" void MS_ABI impl__SetAccelFlag_CMFCAcceleratorKeyAssignCtrl__IEAAXEH_Z(
    void* pThis, unsigned char flag, int bOn)
{
    if (!pThis) return;
    CAccelKeyAssignCtrl* s = reinterpret_cast<CAccelKeyAssignCtrl*>(pThis);
    if (bOn) {
        s->m_flags = static_cast<unsigned char>(s->m_flags | flag);
    } else {
        s->m_flags = static_cast<unsigned char>(s->m_flags & ~flag);
    }
}
// Symbol: ??0CMFCAcceleratorKeyAssignCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAcceleratorKeyAssignCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCAcceleratorKeyAssignCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCAcceleratorKeyAssignCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}
