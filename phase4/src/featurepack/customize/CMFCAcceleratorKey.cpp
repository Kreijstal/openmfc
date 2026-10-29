// CMFCAcceleratorKey — OpenMFC implementation.
// Sources: global_cmfcacceleratorkey.cpp

#include "detail/CMFCAcceleratorKeySupport.h"

// Layout used by the destructor body below (class is not declared in include/openmfc).
static_assert(sizeof(S_Cmfcacceleratorkey) == 16, "CMFCAcceleratorKey size");
static_assert(offsetof(S_Cmfcacceleratorkey, vfptr) == 0, "CMFCAcceleratorKey vfptr offset");
static_assert(offsetof(S_Cmfcacceleratorkey, m_lpAccel) == 8, "CMFCAcceleratorKey ACCEL offset");

// Symbol: ??0CMFCAcceleratorKey@@QEAA@PEAUtagACCEL@@@Z
extern "C" void* MS_ABI
impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(void* pThis, LPACCEL lpAccel) {
    S_Cmfcacceleratorkey* self = (S_Cmfcacceleratorkey*)pThis;
    self->vfptr    = (void*)&g_CMFCAcceleratorKey_vtbl[0];
    self->m_lpAccel = lpAccel;
    return pThis;
}
// Symbol: ??0CMFCAcceleratorKey@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCAcceleratorKey__QEAA_XZ(void* pThis) {
    S_Cmfcacceleratorkey* self = (S_Cmfcacceleratorkey*)pThis;
    self->vfptr    = (void*)&g_CMFCAcceleratorKey_vtbl[0];
    self->m_lpAccel = NULL;
    return pThis;
}
// Symbol: ??1CMFCAcceleratorKey@@UEAA@XZ
extern "C" void MS_ABI
impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis) {
    // Retail writes the CMFCAcceleratorKey vfptr here; its entry at RVA 0x28e0
    // (mfc140u) targets the same table as the default ctor at RVA 0x28c0.
    static_cast<S_Cmfcacceleratorkey*>(pThis)->vfptr =
        reinterpret_cast<void*>(const_cast<void**>(&g_CMFCAcceleratorKey_vtbl[0]));
}
// Symbol: ?AddVirtKeyStr@CMFCAcceleratorKey@@IEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IH@Z
extern "C" void MS_ABI
impl__AddVirtKeyStr_CMFCAcceleratorKey__IEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IH_Z(
        const void* /*pThis*/, CString* str, UINT uiVirtKey, BOOL bLast) {
    // Existing helper approximation: retail AddVirtKeyStr (RVA 0x29b0, mfc140u)
    // special-cases VK_PAUSE and sets the extended bit for 0x21..0x2f and VK_DIVIDE;
    // AddVirtKeyStrImpl uses a different key set. Keep the existing shared helper.
    if (!str) return;
    AddVirtKeyStrImpl(*str, uiVirtKey, bLast);
}
// Symbol: ?Format@CMFCAcceleratorKey@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI
impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        const void* pThis, CString* str) {
    if (!str) return;
    const S_Cmfcacceleratorkey* self = (const S_Cmfcacceleratorkey*)pThis;

    str->Empty();

    if (self->m_lpAccel == NULL) {
        return;  // Retail Format (RVA 0x28f0, mfc140u) returns after Empty().
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

    // Existing approximation appends the non-virtual key as wchar_t. Retail Format
    // (RVA 0x28f0, mfc140u) skips key 0x1b and otherwise passes its sign-extended
    // low byte to a char-append path.
    if (fVirt & FVIRTKEY) {
        AddVirtKeyStrImpl(*str, self->m_lpAccel->key, TRUE);
    } else {
        wchar_t ch[2] = { (wchar_t)self->m_lpAccel->key, L'\0' };
        *str += ch;
    }
}

// CMFCAcceleratorKey's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
extern void* const g_CMFCAcceleratorKey_vtbl[5] = {
    (void*)&slot_GetRuntimeClass_Cmfcacceleratorkey,   // 0 CObject::GetRuntimeClass
    (void*)&slot_vecDelDtor_Cmfcacceleratorkey,        // 1 {dtor}
    (void*)&slot_Serialize_Cmfcacceleratorkey,         // 2 CObject::Serialize
    (void*)&slot_AssertValid_Cmfcacceleratorkey,       // 3 CObject::AssertValid
    (void*)&slot_Dump_Cmfcacceleratorkey,              // 4 CObject::Dump
};
} } }
