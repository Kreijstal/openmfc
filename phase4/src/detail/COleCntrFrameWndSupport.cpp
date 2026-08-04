#include "COleCntrFrameWndSupport.h"

static_assert(sizeof(S_Colecntrframewnd) == 480, "COleCntrFrameWnd must be size 480");
static_assert(offsetof(S_Colecntrframewnd, m_pInPlaceFrame) == 472, "m_pInPlaceFrame @472");
static_assert(offsetof(S_Colecntrframewnd, vfptr) == 0, "vfptr @0");
namespace openmfc { namespace detail { namespace colecntrframewnd {
void* MS_ABI vt_GetRuntimeClass_Colecntrframewnd(void* pThis) {
    return impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(pThis);
}
void* MS_ABI vt_vdtor_Colecntrframewnd(void* p, unsigned int flags) {
    impl___1COleCntrFrameWnd__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}
void MS_ABI vt_Serialize_Colecntrframewnd(void* pThis, void* pAr) {
    if (!pThis || !pAr) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI vt_AssertValid_Colecntrframewnd(void* pThis) {
    if (!pThis) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI vt_Dump_Colecntrframewnd(void* pThis, void* /*dc*/) {
    if (!pThis) {
        return;
    }
    static_cast<CObject*>(pThis)->CObject::Dump();
}
int MS_ABI vt_OnCmdMsg(void* pThis, unsigned int nID, int nCode,
                       void* pExtra, void* pHandlerInfo) {
    return impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
}
void MS_ABI vt_OnFinalRelease(void* pThis) {
    impl__OnFinalRelease_CWnd__UEAAXXZ(pThis);
}
int MS_ABI vt_IsInvokeAllowed(void* /*pThis*/, unsigned int /*dispid*/) {
    return TRUE;
}
int MS_ABI vt_GetDispatchIID(void* /*pThis*/, void* /*pIID*/) {
    return FALSE;
}
unsigned int MS_ABI vt_GetTypeInfoCount(void* /*pThis*/) {
    return 0;
}
} } }  // namespace openmfc::detail::colecntrframewnd
