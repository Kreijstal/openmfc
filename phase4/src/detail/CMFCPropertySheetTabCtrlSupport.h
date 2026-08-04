#pragma once
// Shared internals of the former global_cmfcpropertysheettabctrl.cpp translation unit.
// Definitions live in detail/CMFCPropertySheetTabCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {} } }
using namespace openmfc::detail::cmfcpropertysheettabctrl;
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


// Layout-faithful view of the object. Only the members whose offsets the harvest
// pins down are named; the rest of the inherited CWnd/CMFCTabCtrl state is opaque
// padding. m_pParent is the single member added by this most-derived class and is
// therefore the trailing 8 bytes of the 15728-byte object.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
struct S_Cmfcpropertysheettabctrl {
    void*        vfptr;           // 0
    unsigned int m_dwRef;         // 8
    unsigned int _align0;         // 12 (alignment member from the layout dump)
    char         _opaque[15720 - 16]; // 16 .. 15719: inherited CWnd/CMFCTabCtrl state
    void*        m_pParent;       // 15720: CMFCPropertySheet*
};
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
struct OpenMfcRuntimeClass_Cmfcpropertysheettabctrl {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* m_pBaseClass;
    void* m_pClassContext;
};
} } }


namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
OpenMfcRuntimeClass_Cmfcpropertysheettabctrl* GetBaseClass_Cmfcpropertysheettabctrl();
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
extern OpenMfcRuntimeClass_Cmfcpropertysheettabctrl g_classCMFCPropertySheetTabCtrl;
} } }

// ---- vtable slot wrappers (MSVC slot order per harvest) --------------------
// The harvested vtable exposes the CObject/CCmdTarget-level virtual prefix. Each
// wrapper gives the faithful CCmdTarget/CObject default behaviour; slots past
// this prefix belong to CWnd and are supplied by the framework at runtime.

// GetRuntimeClass: returns the class' CRuntimeClass descriptor. The descriptor
// lives in another TU; we provide a local derived descriptor for the class and
// resolve the base pointer to CMFCTabCtrl at runtime when available.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void* MS_ABI v_GetRuntimeClass(S_Cmfcpropertysheettabctrl*);
} } }

// Vector-deleting destructor (the single MSVC dtor slot). No owned resources.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void* MS_ABI v_vecdel_dtor(void* p, unsigned int flags);
} } }

// CMFCBaseTabCtrl::Serialize — no persistent state to stream here.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void MS_ABI v_Serialize(S_Cmfcpropertysheettabctrl* pThis, void* pAr);
} } }

// CObject::AssertValid / Dump — diagnostic no-ops in release form.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void MS_ABI v_AssertValid(const S_Cmfcpropertysheettabctrl* pThis);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void MS_ABI v_Dump(const S_Cmfcpropertysheettabctrl* pThis, void* /*dc*/);
} } }

// CCmdTarget::OnCmdMsg — default: command not handled here.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
int MS_ABI v_OnCmdMsg(S_Cmfcpropertysheettabctrl* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
} } }

// CWnd::OnFinalRelease — default releases the object; nothing extra to do.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
void MS_ABI v_OnFinalRelease(S_Cmfcpropertysheettabctrl* pThis);
} } }

// CCmdTarget::IsInvokeAllowed — default allows automation invocation.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
int MS_ABI v_IsInvokeAllowed(S_Cmfcpropertysheettabctrl* pThis, long /*dispid*/);
} } }

// CCmdTarget::GetDispatchIID — no dispatch interface exposed.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
int MS_ABI v_GetDispatchIID(S_Cmfcpropertysheettabctrl* pThis, void* /*piid*/);
} } }

// CCmdTarget::GetTypeInfoCount — no type info exposed.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
unsigned int MS_ABI v_GetTypeInfoCount(S_Cmfcpropertysheettabctrl* pThis);
} } }

// Exact order per bundle.vtable.
namespace openmfc { namespace detail { namespace cmfcpropertysheettabctrl {
extern void* const g_CMFCPropertySheetTabCtrl_vtbl[10];
} } }


// ---- exported thunks -------------------------------------------------------


