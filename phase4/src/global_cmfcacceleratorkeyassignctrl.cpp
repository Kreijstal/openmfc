// OpenMFC: CMFCAcceleratorKeyAssignCtrl exports.
//
// All 4 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is
// taken as void*.  Only the +0xec flag byte and the +0xe8 enable flags touched
// by ResetKey / SetAccelFlag are mirrored through raw offsets; the +0x108
// kill-focus marker and the PreTranslateMessage state machine (+0xec..+0x108,
// the +0xf8 CString) are unmodeled, so those bodies keep their NULL checks and
// return the retail terminal value with a `// TODO(clean-room)` marker.
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

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
    CWnd* pThis, MSG* pMsg);

namespace {

struct CAccelKeyAssignCtrl {
    char          _pad000[0x40];
    HWND          m_hWnd;          // 0x040: CWnd::m_hWnd
    char          _pad048[0xe8 - 0x48];
    int           m_bAccelFlags;   // 0x0e8: enable flags (ResetKey clears)
    unsigned char m_flags;         // 0x0ec: tracked modifier/key flags
    char          _pad0ed[0x108 - 0xed];
    int           m_bKillFocus;    // 0x108: kill-focus marker (OnKillFocus)
};

static_assert(offsetof(CAccelKeyAssignCtrl, m_hWnd) == 0x40, "hwnd @0x40");
static_assert(offsetof(CAccelKeyAssignCtrl, m_bAccelFlags) == 0xe8, "flags @0xe8");
static_assert(offsetof(CAccelKeyAssignCtrl, m_flags) == 0xec, "flags byte @0xec");
static_assert(offsetof(CAccelKeyAssignCtrl, m_bKillFocus) == 0x108, "kill-focus @0x108");

} // namespace

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::OnKillFocus
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::PreTranslateMessage
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::ResetKey
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::SetAccelFlag
//-----------------------------------------------------------------------------
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
