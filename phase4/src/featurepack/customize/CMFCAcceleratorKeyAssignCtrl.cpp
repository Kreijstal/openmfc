// CMFCAcceleratorKeyAssignCtrl — OpenMFC implementation.
// Sources: global_cmfcacceleratorkeyassignctrl.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCAcceleratorKeyAssignCtrlSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// The class is opaque in include/openmfc. This local view records the retail
// destructor's embedded CMFCAcceleratorKey at +0xf8 and the enclosing size.
// The offsets are cross-checked against the retail constructor and destructor.
namespace {
struct S_AccelKeyAssignCtrlDtor {
    alignas(8) unsigned char m_edit[0xe8];
    int m_bIsDefined;
    ACCEL m_accel;
    unsigned char _pad0f2[0xf8 - 0xf2];
    struct { void* vfptr; LPACCEL m_lpAccel; } m_helper;
    int m_bIsFocused;
};
static_assert(sizeof(CWnd) == 0xe8, "retail CMFCAcceleratorKeyAssignCtrl CWnd base size");
static_assert(sizeof(CEdit) == 0xe8, "retail CMFCAcceleratorKeyAssignCtrl CEdit base size");
static_assert(offsetof(S_AccelKeyAssignCtrlDtor, m_bIsDefined) == 0xe8,
              "retail ctor RVA 0x3440 (mfc140u): defined flag");
static_assert(offsetof(S_AccelKeyAssignCtrlDtor, m_accel) == 0xec,
              "retail ctor RVA 0x3440 (mfc140u): ACCEL member");
static_assert(offsetof(S_AccelKeyAssignCtrlDtor, m_helper) == 0xf8,
              "retail dtor RVA 0x3500 (mfc140u): helper vftable store");
static_assert(offsetof(S_AccelKeyAssignCtrlDtor, m_bIsFocused) == 0x108,
              "retail ctor RVA 0x3440 (mfc140u): focused flag");
static_assert(sizeof(S_AccelKeyAssignCtrlDtor) == 0x110,
              "retail CMFCAcceleratorKeyAssignCtrl size");
}

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);
extern "C" void MS_ABI impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
    CWnd* pThis, const wchar_t* lpszString);

static_assert(offsetof(CAccelKeyAssignCtrl, m_bKillFocus) == 0x108,
              "retail OnKillFocus marker offset");

// Retail RVA 0x3770 (mfc140u) clears +0x108 then jumps to CWnd::Default at
// RVA 0x28ac80 (mfc140u); both effects are preserved for non-null this. The
// OpenMFC body adds a defensive null guard.
// Symbol: ?OnKillFocus@CMFCAcceleratorKeyAssignCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCAcceleratorKeyAssignCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* /*pNewWnd*/)
{
    if (!pThis) return;
    static_cast<CAccelKeyAssignCtrl*>(pThis)->m_bKillFocus = 0;
    impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// Retail RVA 0x3530 (mfc140u) processes key and character messages using the
// +0xe8/+0xec/+0xee tracking fields, formats the +0xf8 CString through
// CMFCAcceleratorKey::Format (RVA 0x28f0, mfc140u), and may update focus/text.
// This OpenMFC body omits that state machine and delegates every message to
// CWnd::PreTranslateMessage (RVA 0x28bc00, mfc140u); it is a fallback-only
// approximation, not a faithful transcription.
// Symbol: ?PreTranslateMessage@CMFCAcceleratorKeyAssignCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCAcceleratorKeyAssignCtrl__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (!pThis) return FALSE;
    // The retail key-tracking state machine and its focus/text side effects are
    // not modeled here; preserve the base-class fallback for all messages.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
        reinterpret_cast<CWnd*>(pThis), pMsg);
}
// Retail RVA 0x3700 (mfc140u): memset clears +0xec..+0xf1, +0xe8 is zeroed,
// and a non-null +0x40 HWND causes CWnd::SetWindowTextW(this, L"") (RVA
// 0x2a9790, mfc140u). The body below preserves these effects via the shadow
// layout and the CWnd thunk.
// Symbol: ?ResetKey@CMFCAcceleratorKeyAssignCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    CAccelKeyAssignCtrl* s = reinterpret_cast<CAccelKeyAssignCtrl*>(pThis);
    s->m_bAccelFlags = 0;                              // +0xe8
    std::memset(&s->m_flags, 0, 6);                    // +0xec..+0xf1
    if (s->m_hWnd) {
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
            reinterpret_cast<CWnd*>(pThis), L"");
    }
}
// Retail RVA 0x3750 (mfc140u): for non-null this, when bOn is set
// `+0xec |= flag`, otherwise `+0xec &= ~flag`. OpenMFC adds a null guard.
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
// Retail RVA 0x3440 (mfc140u) calls CWnd's default constructor, initializes
// the helper's ACCEL pointer, clears the defined/focused fields, then calls
// ResetKey (RVA 0x3700, mfc140u). OpenMFC has no class-specific vftable here;
// the helper and base construction plus data initialization are retained.
// Symbol: ??0CMFCAcceleratorKeyAssignCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAcceleratorKeyAssignCtrl__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    auto* s = reinterpret_cast<S_AccelKeyAssignCtrlDtor*>(pThis);
    impl___0CWnd__QEAA_XZ(pThis);
    s->m_bIsDefined = 0;
    impl___0CMFCAcceleratorKey__QEAA_XZ(&s->m_helper);
    s->m_helper.m_lpAccel = &s->m_accel;
    s->m_bIsFocused = 0;
    impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(pThis);
    return pThis;
}
// Retail RVA 0x3500 (mfc140u): install this class's vfptr, install the
// CMFCAcceleratorKey vfptr at +0xf8, then tail-jump to CEdit::~CEdit (RVA
// 0x294370, mfc140u). No class-specific vftable definition is present in the
// OpenMFC sources, so the first store is omitted; helper/base destruction is
// retained. OpenMFC also keeps a defensive null guard absent from retail.
// Symbol: ??1CMFCAcceleratorKeyAssignCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCAcceleratorKeyAssignCtrl__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    auto* s = reinterpret_cast<S_AccelKeyAssignCtrlDtor*>(pThis);
    impl___1CMFCAcceleratorKey__UEAA_XZ(&s->m_helper);
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(pThis));
}
