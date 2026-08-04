#include "CSmartDockingInfoSupport.h"

static_assert(sizeof(S_Csmartdockinginfo) == 96, "CSmartDockingInfo must be size(96)");
static_assert(offsetof(S_Csmartdockinginfo, cy) == 12, "m_sizeTotal.cy at 12");
static_assert(offsetof(S_Csmartdockinginfo, m_clrTransparent) == 20, "m_clrTransparent at 20");
static_assert(offsetof(S_Csmartdockinginfo, m_uiMarkerBmpResID) == 40, "marker array at 40");
static_assert(offsetof(S_Csmartdockinginfo, m_bIsAlphaMarkers) == 88, "m_bIsAlphaMarkers at 88");
namespace openmfc { namespace detail { namespace csmartdockinginfo {
void* MS_ABI sd_GetRuntimeClass(void* pThis) {
    return impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(pThis);
}
void* MS_ABI sd_VectorDeletingDtor(void* p, unsigned int flags) {
    if (flags & 1) {
        ::operator delete(p);
    }
    return p;
}
void MS_ABI sd_Serialize(void* pThis, void* pAr) {
    if (!pThis || !pAr) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI sd_AssertValid(void* pThis) {
    if (!pThis) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI sd_Dump(void* pThis, void* /*dc*/) {
    if (!pThis) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::Dump();
}
} } }  // namespace openmfc::detail::csmartdockinginfo
