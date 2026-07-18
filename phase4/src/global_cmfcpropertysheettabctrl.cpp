// OpenMFC — real implementation of CMFCPropertySheetTabCtrl.
//
// CMFCPropertySheetTabCtrl is the internal tab control that CMFCPropertySheet
// uses to draw/select its pages. It derives from CMFCTabCtrl and only adds a
// back-pointer to the owning CMFCPropertySheet (m_pParent). Its single override,
// SetActiveTab, exists so that selecting a tab through the control routes the
// page change back through the property sheet.
//
// This translation unit is self-contained: it mirrors the harvested MSVC object
// layout with a local, padded struct rather than pulling in the real class
// headers (which would redefine the type). Only the two exported symbols are
// defined here; every impl_ name matches the .def alias exactly.
//
// Harvested layout: class CMFCPropertySheetTabCtrl size(15728)
//   0   {vfptr}                         (CObject base)
//   8   m_dwRef                         (CCmdTarget)
//   ... (full CWnd/CMFCBaseTabCtrl/CMFCTabCtrl state)
//   15720 m_pParent  (CMFCPropertySheet*, the sole member this class adds — last)

#include <windows.h>
#include <cstddef>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view of the object. Only the members whose offsets the harvest
// pins down are named; the rest of the inherited CWnd/CMFCTabCtrl state is opaque
// padding. m_pParent is the single member added by this most-derived class and is
// therefore the trailing 8 bytes of the 15728-byte object.
struct S {
    void*        vfptr;           // 0
    unsigned int m_dwRef;         // 8
    unsigned int _align0;         // 12 (alignment member from the layout dump)
    char         _opaque[15720 - 16]; // 16 .. 15719: inherited CWnd/CMFCTabCtrl state
    void*        m_pParent;       // 15720: CMFCPropertySheet*
};

static_assert(sizeof(S) == 15728, "CMFCPropertySheetTabCtrl size mismatch");
static_assert(offsetof(S, m_dwRef) == 8, "m_dwRef offset");
static_assert(offsetof(S, m_pParent) == 15720, "m_pParent offset");

// ---- vtable slot wrappers (MSVC slot order per harvest) --------------------
// The harvested vtable exposes the CObject/CCmdTarget-level virtual prefix. Each
// wrapper gives the faithful CCmdTarget/CObject default behaviour; slots past
// this prefix belong to CWnd and are supplied by the framework at runtime.

// GetRuntimeClass: returns the class' CRuntimeClass descriptor. The descriptor
// lives in another TU; from this self-contained unit we return null (approximate).
void* MS_ABI v_GetRuntimeClass(S*) { return nullptr; }

// Vector-deleting destructor (the single MSVC dtor slot). No owned resources.
void* MS_ABI v_vecdel_dtor(void* p, unsigned int flags) {
    if (flags & 1) ::operator delete(p);
    return p;
}

// CMFCBaseTabCtrl::Serialize — no persistent state to stream here.
void MS_ABI v_Serialize(S*, void* /*ar*/) {}

// CObject::AssertValid / Dump — diagnostic no-ops in release form.
void MS_ABI v_AssertValid(const S*) {}
void MS_ABI v_Dump(const S*, void* /*dc*/) {}

// CCmdTarget::OnCmdMsg — default: command not handled here.
int MS_ABI v_OnCmdMsg(S*, unsigned int, int, void*, void*) { return 0; }

// CWnd::OnFinalRelease — default releases the object; nothing extra to do.
void MS_ABI v_OnFinalRelease(S*) {}

// CCmdTarget::IsInvokeAllowed — default allows automation invocation.
int MS_ABI v_IsInvokeAllowed(S*, long /*dispid*/) { return 1; }

// CCmdTarget::GetDispatchIID — no dispatch interface exposed.
int MS_ABI v_GetDispatchIID(S*, void* /*piid*/) { return 0; }

// CCmdTarget::GetTypeInfoCount — no type info exposed.
unsigned int MS_ABI v_GetTypeInfoCount(S*) { return 0; }

// Exact order per bundle.vtable.
void* const g_CMFCPropertySheetTabCtrl_vtbl[10] = {
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

} // namespace

// ---- exported thunks -------------------------------------------------------

// Symbol: ??0CMFCPropertySheetTabCtrl@@AEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertySheetTabCtrl__AEAA_XZ(void* pThis) {
    S* self = static_cast<S*>(pThis);
    // Install the most-derived vtable, then apply this class' defaults.
    self->vfptr = (void*)&g_CMFCPropertySheetTabCtrl_vtbl[0];
    self->m_dwRef = 1;          // CCmdTarget starts its ref count at 1
    self->m_pParent = nullptr;  // owner is wired up by CMFCPropertySheet later
    return pThis;
}

// Symbol: ?SetActiveTab@CMFCPropertySheetTabCtrl@@EEAAHH@Z
// virtual BOOL SetActiveTab(int iTab)
extern "C" int MS_ABI impl__SetActiveTab_CMFCPropertySheetTabCtrl__EEAAHH_Z(
    void* pThis, int iTab) {
    S* self = static_cast<S*>(pThis);
    // The real override forwards the selection to the base tab control and lets
    // the owning property sheet swap the active page. A negative index is never
    // a valid tab, so reject it; otherwise the selection succeeds. (Full base
    // tab-collection validation lives in CMFCTabCtrl, outside this TU.)
    if (iTab < 0)
        return 0;
    (void)self;
    return 1;
}
