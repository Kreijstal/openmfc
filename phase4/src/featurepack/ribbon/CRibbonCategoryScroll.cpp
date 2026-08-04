// CRibbonCategoryScroll — OpenMFC implementation.
// Sources: global_cribboncategoryscroll.cpp

#include "detail/CRibbonCategoryScrollSupport.h"

// Symbol: ??0CRibbonCategoryScroll@@AEAA@XZ
// Private default constructor.  Installs the vfptr and clears the direction
// flag (CMFCRibbonCategory sets m_bIsLeft afterwards).
extern "C" void* MS_ABI
impl___0CRibbonCategoryScroll__AEAA_XZ(void* pThis) {
    S_Cribboncategoryscroll* self = (S_Cribboncategoryscroll*)pThis;
    std::memset(self, 0, sizeof(S_Cribboncategoryscroll));           // zero-init the full object (base ctors do this)
    self->vfptr    = (void*)&g_CRibbonCategoryScroll_vtbl[0];
    InstallBaseDefaults(self);                 // non-zero base defaults (harvested, byte-verified)
    self->m_bIsLeft = FALSE;
    return pThis;
}
// Symbol: ?CopyFrom@CRibbonCategoryScroll@@EEAAXAEBVCMFCRibbonBaseElement@@@Z
// virtual void CopyFrom(const CMFCRibbonBaseElement& src)
// Copies the scroll-direction flag from the source element (which, when it is
// actually a CRibbonCategoryScroll, carries m_bIsLeft at the same offset).
extern "C" void MS_ABI
impl__CopyFrom_CRibbonCategoryScroll__EEAAXAEBVCMFCRibbonBaseElement___Z(
        void* pThis, const void* src) {
    S_Cribboncategoryscroll* self = (S_Cribboncategoryscroll*)pThis;
    if (src) {
        const S_Cribboncategoryscroll* other = (const S_Cribboncategoryscroll*)src;
        self->m_bIsLeft = other->m_bIsLeft;
    }
}
// Symbol: ?OnAutoRepeat@CRibbonCategoryScroll@@EEAAHXZ
// virtual BOOL OnAutoRepeat()
// Fired by the auto-repeat timer while the scroll button is pressed; the real
// implementation asks the owning category to scroll one step and returns TRUE
// to keep repeating.  The category back-pointer is internal ribbon state not
// modeled here, so this returns TRUE (continue repeating) without scrolling.
extern "C" int MS_ABI
impl__OnAutoRepeat_CRibbonCategoryScroll__EEAAHXZ(void* /*pThis*/) {
    return TRUE;
}
// Symbol: ?OnDraw@CRibbonCategoryScroll@@EEAAXPEAVCDC@@@Z
// virtual void OnDraw(CDC* pDC)
// Paints the left/right scroll arrow via the active visual manager.  Rendering
// depends on the ribbon visual-manager and this element's cached rect, which
// are internal ribbon state not modeled here; no-op when unavailable.
extern "C" void MS_ABI
impl__OnDraw_CRibbonCategoryScroll__EEAAXPEAVCDC___Z(
        void* /*pThis*/, void* /*pDC*/) {
    // No-op in this compatibility layer: full visual behavior requires the
    // owning CMFCRibbonCategory and visual-manager state, which is outside the
    // modeled object model.
    return;
}
// Symbol: ?OnMouseMove@CRibbonCategoryScroll@@EEAAXVCPoint@@@Z
// virtual void OnMouseMove(CPoint point)
// Hover tracking for the scroll button.  CPoint is passed by value as a packed
// 8-byte value (x in low 32 bits, y in high 32 bits).  Hover state updates the
// ribbon's internal highlight tracking which is not modeled here; no-op.
extern "C" void MS_ABI
impl__OnMouseMove_CRibbonCategoryScroll__EEAAXVCPoint___Z(
        void* /*pThis*/, unsigned long long /*point*/) {
    // No-op in this layer: hover state is owned by the ribbon category container.
    return;
}

// CRibbonCategoryScroll's MSVC-layout vtable: defined here, next to the
// implementations it points at.
extern void* const g_CRibbonCategoryScroll_vtbl[10] = {
    (void*)&slot_GetRuntimeClass_Cribboncategoryscroll,    // 0 CMFCRibbonButton::GetRuntimeClass
    (void*)&slot_vecDelDtor_Cribboncategoryscroll,         // 1 {dtor}
    (void*)&slot_Serialize_Cribboncategoryscroll,          // 2 CObject::Serialize
    (void*)&slot_AssertValid_Cribboncategoryscroll,        // 3 CObject::AssertValid
    (void*)&slot_Dump_Cribboncategoryscroll,               // 4 CObject::Dump
    (void*)&slot_OnCmdMsg,           // 5 CCmdTarget::OnCmdMsg
    (void*)&slot_OnFinalRelease,     // 6 CCmdTarget::OnFinalRelease
    (void*)&slot_IsInvokeAllowed,    // 7 CCmdTarget::IsInvokeAllowed
    (void*)&slot_GetDispatchIID,     // 8 CCmdTarget::GetDispatchIID
    (void*)&slot_GetTypeInfoCount,   // 9 CCmdTarget::GetTypeInfoCount
};
