#include "CMFCCaptionButtonExSupport.h"

static_assert(sizeof(S_Cmfccaptionbuttonex) == 72, "CMFCCaptionButtonEx must be size 72");
static_assert(offsetof(S_Cmfccaptionbuttonex, m_bPushed) == 8,  "m_bPushed @8");
static_assert(offsetof(S_Cmfccaptionbuttonex, m_nHit) == 32,    "m_nHit @32");
static_assert(offsetof(S_Cmfccaptionbuttonex, m_ptOrg_x) == 40, "m_ptOrg @40");
static_assert(offsetof(S_Cmfccaptionbuttonex, m_rect) == 56,    "m_rect @56");
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
OpenMfcRuntimeClass_Cmfccaptionbuttonex g_CMFCCaptionButtonEx_rtti = {
    "CMFCCaptionButtonEx",
    sizeof(S_Cmfccaptionbuttonex),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
// Retail reads the base link at +0x18 as a FUNCTION pointer and calls it, so this
// descriptor must publish a thunk there rather than a resolved CRuntimeClass*.
static OpenMfcRuntimeClass_Cmfccaptionbuttonex* MS_ABI _gb_thunk_cmfccaptionbuttonex();
OpenMfcRuntimeClass_Cmfccaptionbuttonex* GetBaseClass_Cmfccaptionbuttonex() {
    static OpenMfcRuntimeClass_Cmfccaptionbuttonex* base = reinterpret_cast<OpenMfcRuntimeClass_Cmfccaptionbuttonex*>(
        impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(nullptr));
    return base ? base : nullptr;
}
void* MS_ABI vt_GetRuntimeClass_Cmfccaptionbuttonex(void* /*pThis*/) {
    if (!g_CMFCCaptionButtonEx_rtti.m_pfnGetBaseClass) {
        g_CMFCCaptionButtonEx_rtti.m_pfnGetBaseClass =
            reinterpret_cast<void*>(&_gb_thunk_cmfccaptionbuttonex);
    }
    return &g_CMFCCaptionButtonEx_rtti;
}
void* MS_ABI vt_vdtor_Cmfccaptionbuttonex(void* p, unsigned flags) {
    impl___1CMFCCaptionButtonEx__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}
void MS_ABI vt_Serialize_Cmfccaptionbuttonex(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI vt_AssertValid_Cmfccaptionbuttonex(void* pThis) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vt_Dump_Cmfccaptionbuttonex(void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::Dump();
}
void* MS_ABI vt_GetRect(void* pThis, void* retbuf) {
    S_Cmfccaptionbuttonex* self = (S_Cmfccaptionbuttonex*)pThis;
    *(RECT*)retbuf = self->m_rect;
    return retbuf;
}
void MS_ABI vt_OnDraw(void* /*pThis*/, void* /*pDC*/, unsigned char /*bActive*/,
                      unsigned char /*bDarkBackground*/) {
    // No-op drawing: this TU models only the object shell of CMFCCaptionButtonEx.
    // The real button rendering is owned by CMFCToolBar internals and visual
    // manager state that is not represented in this ABI compatibility layer.
    return;
}
UINT MS_ABI vt_GetIconID(void* pThis, unsigned char /*bHorz*/) {
    S_Cmfccaptionbuttonex* self = (S_Cmfccaptionbuttonex*)pThis;
    return self->m_nHit;
}
static OpenMfcRuntimeClass_Cmfccaptionbuttonex* MS_ABI _gb_thunk_cmfccaptionbuttonex() { return GetBaseClass_Cmfccaptionbuttonex(); }
} } }  // namespace openmfc::detail::cmfccaptionbuttonex
