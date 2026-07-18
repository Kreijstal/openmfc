// OpenMFC: CMFCAcceleratorKey  (public CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(16):
//   0  {vfptr}     (CObject vfptr)
//   8  m_lpAccel   (LPACCEL) -- borrowed pointer to a caller-owned ACCEL
//
// A thin wrapper around a single ACCEL entry that formats it into a human
// readable accelerator string such as "Ctrl+Shift+A".  The object does NOT own
// the ACCEL it points at, so the destructor frees nothing.  This class is not
// DECLARE_DYNAMIC: its vtable slot 0 (GetRuntimeClass), plus Serialize /
// AssertValid / Dump, are the inherited CObject implementations.

#include <windows.h>
#include <cstddef>
#include "openmfc/afxstr.h"   // ABI-compatible CStringW (single m_pszData pointer)

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

struct S {
    void*   vfptr;      // 0
    LPACCEL m_lpAccel;  // 8
};

static_assert(sizeof(S) == 16, "CMFCAcceleratorKey must be 16 bytes");
static_assert(offsetof(S, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S, m_lpAccel) == 8, "m_lpAccel at 8");

// ---- vtable slot helpers (non-exported virtuals) ----------------------------

// Slot 0: CObject::GetRuntimeClass -- delegate to the real CObject impl so the
// object reports RuntimeClass "CObject" (this class is not DECLARE_DYNAMIC).
extern "C" void* MS_ABI
impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

static void* MS_ABI slot_GetRuntimeClass(const void* pThis) {
    return impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(pThis);
}

// Slot 2: CObject::Serialize -- base implementation is a no-op.
static void MS_ABI slot_Serialize(void* /*pThis*/, void* /*ar*/) {}

// Slot 3: CObject::AssertValid -- const, no-op in release.
static void MS_ABI slot_AssertValid(const void* /*pThis*/) {}

// Slot 4: CObject::Dump -- const, no-op in release.
static void MS_ABI slot_Dump(const void* /*pThis*/, void* /*dc*/) {}

// ---- shared formatting helper -----------------------------------------------
//
// Appends the display name of one virtual key to `str`.  Mirrors the real
// CMFCAcceleratorKey::AddVirtKeyStr: derive the hardware scan code from the
// virtual key, set the extended-key bit for the navigation/edit cluster, ask
// the OS for the localized key name via GetKeyNameText, and append a trailing
// "+" separator unless this is the final (base) key.
static void AddVirtKeyStrImpl(CString& str, UINT uiVirtKey, BOOL bLast) {
    UINT uiScanCode = MapVirtualKeyW(uiVirtKey, MAPVK_VK_TO_VSC);
    LONG lParam = (LONG)(uiScanCode << 16);

    // Extended keys need bit 24 set so GetKeyNameText resolves them correctly
    // (e.g. the right-hand Alt/Ctrl and the grey navigation cluster).
    switch (uiVirtKey) {
    case VK_INSERT: case VK_DELETE:
    case VK_HOME:   case VK_END:
    case VK_PRIOR:  case VK_NEXT:
    case VK_LEFT:   case VK_RIGHT:
    case VK_UP:     case VK_DOWN:
    case VK_NUMLOCK: case VK_DIVIDE:
    case VK_RCONTROL: case VK_RMENU:
        lParam |= (1L << 24);
        break;
    default:
        break;
    }

    wchar_t szKey[64];
    szKey[0] = L'\0';
    if (GetKeyNameTextW(lParam, szKey, 64) != 0) {
        str += szKey;
    }

    if (!bLast) {
        str += L"+";
    }
}

} // namespace

// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace { extern void* const g_CMFCAcceleratorKey_vtbl[5]; }

// Symbol: ??0CMFCAcceleratorKey@@QEAA@PEAUtagACCEL@@@Z
extern "C" void* MS_ABI
impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(void* pThis, LPACCEL lpAccel) {
    S* self = (S*)pThis;
    self->vfptr    = (void*)&g_CMFCAcceleratorKey_vtbl[0];
    self->m_lpAccel = lpAccel;
    return pThis;
}

// Symbol: ??0CMFCAcceleratorKey@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCAcceleratorKey__QEAA_XZ(void* pThis) {
    S* self = (S*)pThis;
    self->vfptr    = (void*)&g_CMFCAcceleratorKey_vtbl[0];
    self->m_lpAccel = NULL;
    return pThis;
}

// Symbol: ??1CMFCAcceleratorKey@@UEAA@XZ
extern "C" void MS_ABI
impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis) {
    // The ACCEL is borrowed, not owned -- nothing to release.
    (void)pThis;
}

// Symbol: ?AddVirtKeyStr@CMFCAcceleratorKey@@IEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IH@Z
extern "C" void MS_ABI
impl__AddVirtKeyStr_CMFCAcceleratorKey__IEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IH_Z(
        const void* /*pThis*/, CString* str, UINT uiVirtKey, BOOL bLast) {
    if (!str) return;
    AddVirtKeyStrImpl(*str, uiVirtKey, bLast);
}

// Symbol: ?Format@CMFCAcceleratorKey@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI
impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        const void* pThis, CString* str) {
    if (!str) return;
    const S* self = (const S*)pThis;

    str->Empty();

    if (self->m_lpAccel == NULL) {
        return;  // real MFC ASSERTs; degrade to an empty string in release.
    }

    const BYTE fVirt = self->m_lpAccel->fVirt;

    if (fVirt & FCONTROL) {
        AddVirtKeyStrImpl(*str, VK_CONTROL, FALSE);
    }
    if (fVirt & FALT) {
        AddVirtKeyStrImpl(*str, VK_MENU, FALSE);
    }
    if (fVirt & FSHIFT) {
        AddVirtKeyStrImpl(*str, VK_SHIFT, FALSE);
    }

    // The base key: a virtual key when FVIRTKEY is set, otherwise an ASCII char.
    if (fVirt & FVIRTKEY) {
        AddVirtKeyStrImpl(*str, self->m_lpAccel->key, TRUE);
    } else {
        wchar_t ch[2] = { (wchar_t)self->m_lpAccel->key, L'\0' };
        *str += ch;
    }
}

// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------

namespace {

// Slot 1: CMFCAcceleratorKey::{vector deleting destructor}.
void* MS_ABI slot_vecDelDtor(void* pThis, unsigned flags) {
    // Destructor body is a no-op (borrowed ACCEL); honour the delete flag.
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}

void* const g_CMFCAcceleratorKey_vtbl[5] = {
    (void*)&slot_GetRuntimeClass,   // 0 CObject::GetRuntimeClass
    (void*)&slot_vecDelDtor,        // 1 {dtor}
    (void*)&slot_Serialize,         // 2 CObject::Serialize
    (void*)&slot_AssertValid,       // 3 CObject::AssertValid
    (void*)&slot_Dump,              // 4 CObject::Dump
};

} // namespace
