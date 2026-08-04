#pragma once
// Shared internals of the former global_cmfccontrolbarimpl.cpp translation unit.
// Definitions live in detail/CMFCControlBarImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {} } }
using namespace openmfc::detail::cmfccontrolbarimpl;
// OpenMFC real implementation of CMFCControlBarImpl.
//
// Layout (harvested via cl.exe /d1reportSingleClassLayout):
//   class CMFCControlBarImpl  size(16):
//     0  | {vfptr}
//     8  | m_pBar        (CPane*)
//
// Polymorphic: single vtable slot = virtual destructor (vector deleting dtor).
//
// CMFCControlBarImpl is the helper that CMFCToolBar / CMFCReBar delegate to for
// non-client (gripper) sizing and painting. All observable behaviour flows
// through its owning CPane (m_pBar), whose HWND lives at CWnd::m_hWnd (offset 64).

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#include <cstddef>


// Layout-faithful view of CMFCControlBarImpl.
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
struct S_Cmfccontrolbarimpl {
    void* vfptr;   // 0
    void* m_pBar;  // 8  -> CPane*
};
} } }

// CWnd::m_hWnd lives at offset 64 inside any CWnd-derived object (CPane derives
// from CBasePane -> CPane -> ... -> CWnd). We read it to reach the real window.
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
HWND BarHwnd(void* pBar);
} } }

// Default gripper thickness used by the MFC docking/rebar non-client frame.
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
static const int kGripperSize = 12;
} } }




// -------------------------------------------------------------------------
// vtable  (slot 0 = vector deleting destructor)
// -------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
void* MS_ABI vdtor_Cmfccontrolbarimpl(void* p, unsigned flags);
} } }
namespace openmfc { namespace detail { namespace cmfccontrolbarimpl {
extern void* const g_CMFCControlBarImpl_vtbl[1];
} } }






