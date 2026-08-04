#pragma once
// Shared internals of the former global_colecntrframewnd.cpp translation unit.
// Definitions live in detail/COleCntrFrameWndSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colecntrframewnd {} } }
using namespace openmfc::detail::colecntrframewnd;
// OpenMFC — real implementation of COleCntrFrameWnd
//
// COleCntrFrameWnd is the container-side frame window MFC creates around a
// server object that is being edited in place. It exists solely to forward
// layout recalculation and command routing to the *in-place* frame window
// (COleIPFrameWnd) supplied at construction, and to suppress the automatic
// "delete this" that CFrameWnd::PostNcDestroy performs (the container frame is
// an embedded member, not a heap object).
//
//   class COleCntrFrameWnd  size(480):
//     0   | +--- (base CFrameWnd : CWnd : CCmdTarget : CObject)
//     0   | | {vfptr}
//     8   | | m_dwRef ... (CFrameWnd body)
//   472   | m_pInPlaceFrame  (COleIPFrameWnd*)
//
#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of the object. Only the vfptr (offset 0) and the single
// derived member m_pInPlaceFrame (offset 472) are addressed here; the CFrameWnd
// body in between is opaque padding.
namespace openmfc { namespace detail { namespace colecntrframewnd {
struct S_Colecntrframewnd {
    void* vfptr;               // 0
    char  _cframewnd[472 - 8]; // 8 .. 472  (CFrameWnd / CWnd / CCmdTarget body)
    void* m_pInPlaceFrame;     // 472  COleIPFrameWnd*
};
} } }


// OnCmdMsg lives at vtable slot 5 for every CFrameWnd-derived type (see the
// harvested vtable), so we can route through the in-place frame's *actual*
// runtime type by dispatching that slot directly.
namespace openmfc { namespace detail { namespace colecntrframewnd {
typedef int (MS_ABI *OnCmdMsgFn)(void*, unsigned int, int, void*, void*);
} } }
namespace openmfc { namespace detail { namespace colecntrframewnd {
enum { VT_ONCMDMSG_SLOT = 5 };
} } }


// --- external impl_ thunks we delegate to (defined in sibling TUs) ----------
extern "C" int  MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(void* pThis);
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(void* pThis, int bNotify);

// --- forward decls of our own exports referenced by the vtable --------------
extern "C" void MS_ABI impl___1COleCntrFrameWnd__UEAA_XZ(void* pThis);
extern "C" int  MS_ABI impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__PostNcDestroy_COleCntrFrameWnd__MEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_COleCntrFrameWnd__UEAAXH_Z(void* pThis, int bNotify);


// --- vtable slot wrappers ---------------------------------------------------

// slot 0: CFrameWnd::GetRuntimeClass — COleCntrFrameWnd inherits the frame's
// runtime-class getter; delegate to it so RTTI queries resolve.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void* MS_ABI vt_GetRuntimeClass_Colecntrframewnd(void* pThis);
} } }

// slot 1: COleCntrFrameWnd::{dtor} — vector-deleting-destructor wrapper. Runs
// the real destructor, then frees storage when the delete flag (bit 0) is set.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void* MS_ABI vt_vdtor_Colecntrframewnd(void* p, unsigned int flags);
} } }

// slot 2: CObject::Serialize — delegate to base CObject implementation.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void MS_ABI vt_Serialize_Colecntrframewnd(void* pThis, void* pAr);
} } }

// slot 3: CObject::AssertValid — base implementation.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void MS_ABI vt_AssertValid_Colecntrframewnd(void* pThis);
} } }

// slot 4: CObject::Dump — base implementation.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void MS_ABI vt_Dump_Colecntrframewnd(void* pThis, void* /*dc*/);
} } }

// slot 5: COleCntrFrameWnd::OnCmdMsg — dispatch to our exported override.
namespace openmfc { namespace detail { namespace colecntrframewnd {
int MS_ABI vt_OnCmdMsg(void* pThis, unsigned int nID, int nCode,
                       void* pExtra, void* pHandlerInfo);
} } }

// slot 6: CWnd::OnFinalRelease — inherited; delegate to CWnd's implementation.
namespace openmfc { namespace detail { namespace colecntrframewnd {
void MS_ABI vt_OnFinalRelease(void* pThis);
} } }

// slot 7: CCmdTarget::IsInvokeAllowed — the base default permits every DISPID.
namespace openmfc { namespace detail { namespace colecntrframewnd {
int MS_ABI vt_IsInvokeAllowed(void* /*pThis*/, unsigned int /*dispid*/);
} } }

// slot 8: CCmdTarget::GetDispatchIID — no automation IID; base returns FALSE
// and leaves the caller's IID untouched.
namespace openmfc { namespace detail { namespace colecntrframewnd {
int MS_ABI vt_GetDispatchIID(void* /*pThis*/, void* /*pIID*/);
} } }

// slot 9: CCmdTarget::GetTypeInfoCount — no type information exposed: returns 0.
namespace openmfc { namespace detail { namespace colecntrframewnd {
unsigned int MS_ABI vt_GetTypeInfoCount(void* /*pThis*/);
} } }

namespace openmfc { namespace detail { namespace colecntrframewnd {
extern void* const g_COleCntrFrameWnd_vtbl[10];
} } }







