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
#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

struct CRuntimeClass;
struct AFX_CMDHANDLERINFO;

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCTabCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWnd__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" int MS_ABI impl__SetActivePage_CPropertySheet__QEAAHH_Z(void* pThis, int p0);
extern "C" int MS_ABI impl__OnCmdMsg_CPropertySheet__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(void* pThis);

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

struct OpenMfcRuntimeClass {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass* m_pBaseClass;
    void* m_pClassContext;
};

static_assert(sizeof(S) == 15728, "CMFCPropertySheetTabCtrl size mismatch");
static_assert(offsetof(S, m_dwRef) == 8, "m_dwRef offset");
static_assert(offsetof(S, m_pParent) == 15720, "m_pParent offset");

static OpenMfcRuntimeClass* GetBaseClass() {
    static OpenMfcRuntimeClass* base = reinterpret_cast<OpenMfcRuntimeClass*>(
        impl__GetRuntimeClass_CMFCTabCtrl__UEBAPEAUCRuntimeClass__XZ(nullptr));
    if (!base) {
        base = reinterpret_cast<OpenMfcRuntimeClass*>(
            impl__GetRuntimeClass_CWnd__UEBAPEAUCRuntimeClass__XZ(nullptr));
    }
    return base;
}

static OpenMfcRuntimeClass g_classCMFCPropertySheetTabCtrl = {
    "CMFCPropertySheetTabCtrl",
    sizeof(S),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

// ---- vtable slot wrappers (MSVC slot order per harvest) --------------------
// The harvested vtable exposes the CObject/CCmdTarget-level virtual prefix. Each
// wrapper gives the faithful CCmdTarget/CObject default behaviour; slots past
// this prefix belong to CWnd and are supplied by the framework at runtime.

// GetRuntimeClass: returns the class' CRuntimeClass descriptor. The descriptor
// lives in another TU; we provide a local derived descriptor for the class and
// resolve the base pointer to CMFCTabCtrl at runtime when available.
void* MS_ABI v_GetRuntimeClass(S*) {
    if (!g_classCMFCPropertySheetTabCtrl.m_pBaseClass) {
        g_classCMFCPropertySheetTabCtrl.m_pBaseClass = GetBaseClass();
    }
    return &g_classCMFCPropertySheetTabCtrl;
}

// Vector-deleting destructor (the single MSVC dtor slot). No owned resources.
void* MS_ABI v_vecdel_dtor(void* p, unsigned int flags) {
    if (flags & 1) ::operator delete(p);
    return p;
}

// CMFCBaseTabCtrl::Serialize — no persistent state to stream here.
void MS_ABI v_Serialize(S* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    reinterpret_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}

// CObject::AssertValid / Dump — diagnostic no-ops in release form.
void MS_ABI v_AssertValid(const S* pThis) {
    if (!pThis) return;
    reinterpret_cast<const CObject*>(pThis)->CObject::AssertValid();
}

void MS_ABI v_Dump(const S* pThis, void* /*dc*/) {
    if (!pThis) return;
    reinterpret_cast<const CObject*>(pThis)->CObject::Dump();
}

// CCmdTarget::OnCmdMsg — default: command not handled here.
int MS_ABI v_OnCmdMsg(S* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    if (!pThis || !pThis->m_pParent) return FALSE;
    return impl__OnCmdMsg_CPropertySheet__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis->m_pParent, nID, nCode, pExtra, static_cast<AFX_CMDHANDLERINFO*>(pHandlerInfo));
}

// CWnd::OnFinalRelease — default releases the object; nothing extra to do.
void MS_ABI v_OnFinalRelease(S* pThis) {
    if (pThis) {
        impl__OnFinalRelease_CWnd__UEAAXXZ(pThis);
    }
}

// CCmdTarget::IsInvokeAllowed — default allows automation invocation.
int MS_ABI v_IsInvokeAllowed(S* pThis, long /*dispid*/) { return pThis ? TRUE : FALSE; }

// CCmdTarget::GetDispatchIID — no dispatch interface exposed.
int MS_ABI v_GetDispatchIID(S* pThis, void* /*piid*/) {
    if (!pThis) return 0;
    return 0;
}

// CCmdTarget::GetTypeInfoCount — no type info exposed.
unsigned int MS_ABI v_GetTypeInfoCount(S* pThis) { return pThis ? 0u : 0u; }

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
    // The real override forwards the selection to the owning property sheet.
    // This class only stores the parent pointer; base tab validation remains in
    // CMFCPropertySheet/CPropertySheet.
    if (iTab < 0 || !self->m_pParent) {
        return 0;
    }
    (void)self;
    return impl__SetActivePage_CPropertySheet__QEAAHH_Z(self->m_pParent, iTab);
}
