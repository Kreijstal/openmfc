#pragma once
// Shared internals of the former global_ole-5_impl.cpp translation unit.
// Definitions live in detail/COleControlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colecontrol {} } }
using namespace openmfc::detail::colecontrol;
// COleControl methods, wave 5 (ole-5). Every function here was decoded from the
// retail mfc140u export (ordinal -> RVA -> objdump, see /tmp/ole5_dis/all.log)
// rather than inferred, and the member offsets used are exactly the ones
// transcribed into include/openmfc/afxole.h.
//
// Retail bitfield word at this+0x160; bit positions referenced below are the
// ones the shipping code actually tests:
//   bit 10 = m_bInPlaceActive, bit 22 = m_bInPlaceSiteWndless,
//   bits 4-6 = m_iButtonState, bits 7-9 = m_iDblClkState.
//
// Where a retail body depends on virtual slots or internal unexported helpers
// that the OpenMFC headers do not model, a type-correct conservative version is
// emitted (safe defaults, no state corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"
#include "openmfc/afxstr.h"

#include <new>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#ifndef DISPID_ERROREVENT
#define DISPID_ERROREVENT (-608)
#endif

// Layout anchors used by the code below (offsets come from afxole.h comments
// and are asserted there too; these just catch accidental header regressions).

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" void* MS_ABI impl___0CRectTracker__QEAA_PEBUtagRECT__I_Z(
    void* pThis, const RECT* lpSrcRect, unsigned int nStyle);
extern "C" void MS_ABI impl___1CRectTracker__UEAA_XZ(void* pThis);

// ---- forward declarations within this file ----
extern "C" void MS_ABI impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(
    const COleControl* pThis, long* pDx, long* pDy);
extern "C" void MS_ABI impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(
    COleControl* pThis);
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(
    COleControl* pThis, int nSize, int bEnableResize, const RECT* lpRect);

// CPoint arrives by value as one 8-byte integer register under MS x64; unpack
// it the same way global_crecttracker.cpp does.
namespace openmfc { namespace detail { namespace colecontrol {
long PtX(unsigned long long pt);
} } }
namespace openmfc { namespace detail { namespace colecontrol {
long PtY(unsigned long long pt);
} } }

//=============================================================================
// Ambient / event helpers
//=============================================================================




//=============================================================================
// Button helpers
//=============================================================================




//=============================================================================
// Coordinate helpers
//=============================================================================





//=============================================================================
// Capture
//=============================================================================


//=============================================================================
// Window / frame creation
//=============================================================================







//=============================================================================
// Drawing
//=============================================================================



//=============================================================================
// Persistence / property exchange
//=============================================================================



//=============================================================================
// In-place UI state
//=============================================================================





//=============================================================================
// Tracker
//=============================================================================




//=============================================================================
// Connection points
//=============================================================================


