#include "CMFCTasksPaneTaskGroupSupport.h"

static_assert(sizeof(S_Cmfctaskspanetaskgroup) == 152, "CMFCTasksPaneTaskGroup must be size 152");
static_assert(offsetof(S_Cmfctaskspanetaskgroup, m_pPage)      == 8,   "m_pPage @8");
static_assert(offsetof(S_Cmfctaskspanetaskgroup, m_lstTasks)   == 24,  "m_lstTasks @24");
static_assert(offsetof(S_Cmfctaskspanetaskgroup, m_bIsBottom)  == 80,  "m_bIsBottom @80");
static_assert(offsetof(S_Cmfctaskspanetaskgroup, m_hIcon)      == 136, "m_hIcon @136");
static_assert(offsetof(S_Cmfctaskspanetaskgroup, m_clrTextHot) == 148, "m_clrTextHot @148");
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
OpenMfcRuntimeClass_Cmfctaskspanetaskgroup g_CMFCTasksPaneTaskGroup_rtti = {
    "CMFCTasksPaneTaskGroup",
    sizeof(S_Cmfctaskspanetaskgroup),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
OpenMfcRuntimeClass_Cmfctaskspanetaskgroup* GetBaseClass_Cmfctaskspanetaskgroup() {
    static OpenMfcRuntimeClass_Cmfctaskspanetaskgroup* base = reinterpret_cast<OpenMfcRuntimeClass_Cmfctaskspanetaskgroup*>(
        impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(nullptr));
    return base ? base : nullptr;
}
const void* MS_ABI vslot_GetRuntimeClass_Cmfctaskspanetaskgroup(void* /*p*/) {
    if (!g_CMFCTasksPaneTaskGroup_rtti.m_pBaseClass) {
        g_CMFCTasksPaneTaskGroup_rtti.m_pBaseClass = GetBaseClass_Cmfctaskspanetaskgroup();
    }
    return &g_CMFCTasksPaneTaskGroup_rtti;
}
void MS_ABI vslot_Serialize_Cmfctaskspanetaskgroup(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI vslot_AssertValid_Cmfctaskspanetaskgroup(void* pThis) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vslot_Dump_Cmfctaskspanetaskgroup(void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<CObject*>(pThis)->CObject::Dump();
}
void* MS_ABI vdtor_Cmfctaskspanetaskgroup(void* p, unsigned flags) {
    S_Cmfctaskspanetaskgroup* self = (S_Cmfctaskspanetaskgroup*)p;
    self->m_pPage = nullptr;
    if (flags & 1)
        ::operator delete(p);
    return p;
}
} } }  // namespace openmfc::detail::cmfctaskspanetaskgroup
