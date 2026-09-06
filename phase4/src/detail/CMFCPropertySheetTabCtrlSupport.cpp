#include "CMFCPropertySheetTabCtrlSupport.h"

static_assert(sizeof(S_Cmfcpropertysheettabctrl) == 15728, "CMFCPropertySheetTabCtrl size mismatch");
static_assert(offsetof(S_Cmfcpropertysheettabctrl, m_dwRef) == 8, "m_dwRef offset");
static_assert(offsetof(S_Cmfcpropertysheettabctrl, m_pParent) == 15720, "m_pParent offset");
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
// Retail reads the base link at +0x18 as a FUNCTION pointer and calls it, so this
// descriptor must publish a thunk there rather than a resolved CRuntimeClass*.
static OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* MS_ABI _gb_thunk_cmfcpropertysheettabctrl();
OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* GetBaseClass_Cmfcpropertysheettabctrl() {
    static OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* base = reinterpret_cast<OpenMfcRuntimeClass_Cmfcpropertysheettabctrl*>(
        impl__GetRuntimeClass_CMFCTabCtrl__UEBAPEAUCRuntimeClass__XZ(nullptr));
    if (!base) {
        base = reinterpret_cast<OpenMfcRuntimeClass_Cmfcpropertysheettabctrl*>(
            impl__GetRuntimeClass_CWnd__UEBAPEAUCRuntimeClass__XZ(nullptr));
    }
    return base;
}
OpenMfcRuntimeClass_Cmfcpropertysheettabctrl g_classCMFCPropertySheetTabCtrl = {
    "CMFCPropertySheetTabCtrl",
    sizeof(S_Cmfcpropertysheettabctrl),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
void* MS_ABI v_GetRuntimeClass(S_Cmfcpropertysheettabctrl*) {
    if (!g_classCMFCPropertySheetTabCtrl.m_pfnGetBaseClass) {
        g_classCMFCPropertySheetTabCtrl.m_pfnGetBaseClass =
            reinterpret_cast<void*>(&_gb_thunk_cmfcpropertysheettabctrl);
    }
    return &g_classCMFCPropertySheetTabCtrl;
}
void* MS_ABI v_vecdel_dtor(void* p, unsigned int flags) {
    if (flags & 1) ::operator delete(p);
    return p;
}
void MS_ABI v_Serialize(S_Cmfcpropertysheettabctrl* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    reinterpret_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI v_AssertValid(const S_Cmfcpropertysheettabctrl* pThis) {
    if (!pThis) return;
    reinterpret_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI v_Dump(const S_Cmfcpropertysheettabctrl* pThis, void* /*dc*/) {
    if (!pThis) return;
    reinterpret_cast<const CObject*>(pThis)->CObject::Dump();
}
int MS_ABI v_OnCmdMsg(S_Cmfcpropertysheettabctrl* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    if (!pThis || !pThis->m_pParent) return FALSE;
    return impl__OnCmdMsg_CPropertySheet__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis->m_pParent, nID, nCode, pExtra, static_cast<AFX_CMDHANDLERINFO*>(pHandlerInfo));
}
void MS_ABI v_OnFinalRelease(S_Cmfcpropertysheettabctrl* pThis) {
    if (pThis) {
        impl__OnFinalRelease_CWnd__UEAAXXZ(pThis);
    }
}
int MS_ABI v_IsInvokeAllowed(S_Cmfcpropertysheettabctrl* pThis, long /*dispid*/) { return pThis ? TRUE : FALSE; }
int MS_ABI v_GetDispatchIID(S_Cmfcpropertysheettabctrl* pThis, void* /*piid*/) {
    if (!pThis) return 0;
    return 0;
}
unsigned int MS_ABI v_GetTypeInfoCount(S_Cmfcpropertysheettabctrl* pThis) { return pThis ? 0u : 0u; }
static OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* MS_ABI _gb_thunk_cmfcpropertysheettabctrl() { return GetBaseClass_Cmfcpropertysheettabctrl(); }
} } }  // namespace openmfc::detail::cmfcpropertysheettabctrl
