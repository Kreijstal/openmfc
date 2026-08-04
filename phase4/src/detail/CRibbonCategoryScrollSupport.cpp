#include "CRibbonCategoryScrollSupport.h"

static_assert(sizeof(S_Cribboncategoryscroll) == 632, "CRibbonCategoryScroll must be 632 bytes");
static_assert(offsetof(S_Cribboncategoryscroll, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S_Cribboncategoryscroll, m_bIsLeft) == 624, "m_bIsLeft at 624");

namespace openmfc { namespace detail { namespace cribboncategoryscroll {
OpenMfcRuntimeClass_Cribboncategoryscroll g_CRibbonCategoryScroll_rtti = {
    "CRibbonCategoryScroll",
    sizeof(S_Cribboncategoryscroll),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
OpenMfcRuntimeClass_Cribboncategoryscroll* GetBaseClass_Cribboncategoryscroll() {
    static OpenMfcRuntimeClass_Cribboncategoryscroll* base = reinterpret_cast<OpenMfcRuntimeClass_Cribboncategoryscroll*>(
        impl__GetRuntimeClass_CMFCRibbonButton__UEBAPEAUCRuntimeClass__XZ(nullptr));
    return base ? base : nullptr;
}
void* MS_ABI slot_GetRuntimeClass_Cribboncategoryscroll(const void* /*pThis*/) {
    if (!g_CRibbonCategoryScroll_rtti.m_pBaseClass) {
        g_CRibbonCategoryScroll_rtti.m_pBaseClass = GetBaseClass_Cribboncategoryscroll();
    }
    return &g_CRibbonCategoryScroll_rtti;
}
void MS_ABI slot_Serialize_Cribboncategoryscroll(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI slot_AssertValid_Cribboncategoryscroll(const void* pThis) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI slot_Dump_Cribboncategoryscroll(const void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
int MS_ABI slot_OnCmdMsg(void* pThis, unsigned nID, int nCode,
                         void* pExtra, void* pHandlerInfo) {
    (void)nID;
    (void)nCode;
    (void)pExtra;
    (void)pHandlerInfo;
    return pThis ? TRUE : FALSE;
}
void MS_ABI slot_OnFinalRelease(void* pThis) {
    if (!pThis) return;
    // Intentionally no-op.
    return;
}
int MS_ABI slot_IsInvokeAllowed(void* pThis, unsigned /*dispid*/) {
    return pThis ? 1 : 0;
}
int MS_ABI slot_GetDispatchIID(void* pThis, void* /*pIID*/) {
    if (!pThis) return 0;
    return 0;
}
unsigned MS_ABI slot_GetTypeInfoCount(void* pThis) {
    if (!pThis) return 0;
    return 0;
}
void* MS_ABI slot_vecDelDtor_Cribboncategoryscroll(void* pThis, unsigned flags) {
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}
} } }  // namespace openmfc::detail::cribboncategoryscroll
