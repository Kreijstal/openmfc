// CMFCPropertySheetTabCtrl — OpenMFC implementation.
// Sources: global_cmfcpropertysheettabctrl.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCPropertySheetTabCtrlSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCPropertySheetTabCtrl@@AEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertySheetTabCtrl__AEAA_XZ(void* pThis) {
    S_Cmfcpropertysheettabctrl* self = static_cast<S_Cmfcpropertysheettabctrl*>(pThis);
    // Install the most-derived vtable, then apply this class' defaults.
    self->vfptr = (void*)&g_CMFCPropertySheetTabCtrl_vtbl[0];
    self->m_dwRef = 1;          // CCmdTarget starts its ref count at 1
    self->m_pParent = nullptr;  // owner is wired up by CMFCPropertySheet later
    return pThis;
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetActiveTab@CMFCPropertySheetTabCtrl@@EEAAHH@Z
extern "C" int MS_ABI impl__SetActivePage_CPropertySheet__QEAAHH_Z(void* pThis, int p0);

// CMFCPropertySheetTabCtrl's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
extern void* const g_CMFCPropertySheetTabCtrl_vtbl[10] = {
    (void*)&v_GetRuntimeClass,   // CMFCTabCtrl::GetRuntimeClass
    (void*)&v_vecdel_dtor,       // CMFCPropertySheetTabCtrl::{dtor}
    (void*)&v_Serialize,         // CMFCBaseTabCtrl::Serialize
    (void*)&v_AssertValid,       // CObject::AssertValid
    (void*)&v_Dump,              // CObject::Dump
    (void*)&v_OnCmdMsg,          // CCmdTarget::OnCmdMsg
    (void*)&v_OnFinalRelease,    // CWnd::OnFinalRelease
    (void*)&v_IsInvokeAllowed,   // CCmdTarget::IsInvokeAllowed
    (void*)&v_GetDispatchIID,    // CCmdTarget::GetDispatchIID
    (void*)&v_GetTypeInfoCount,  // CCmdTarget::GetTypeInfoCount
};
} } }
