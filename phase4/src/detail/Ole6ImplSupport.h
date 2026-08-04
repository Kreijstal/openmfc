#pragma once
// Shared internals of the former global_ole-6_impl.cpp translation unit.
// Definitions live in detail/Ole6ImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ole6impl {} } }
using namespace openmfc::detail::ole6impl;
// COleControl methods, wave 6 (ole-6). Every function here was decoded from
// the retail mfc140u export (ordinal -> RVA -> objdump, see /tmp/ole6_dis/)
// rather than inferred, and the member offsets used are exactly the ones
// transcribed into include/openmfc/afxole.h.
//
// The retail aliases every stock-property change notification
// (OnAppearanceChanged, OnBackColorChanged, OnBorderStyleChanged,
// OnEnabledChanged, OnForeColorChanged) to the same body:
//     xor edx,edx; lea r8d,0x1(rdx); jmp <InvalidateControl>
// i.e. they are all `InvalidateControl(NULL, TRUE)` and fire no per-property
// event on their own (the property setters do that). They are therefore
// spelled here as direct calls to the OpenMFC C++ member.
//
// OnEventAdvise and OnFreezeEvents export the same bare `ret $0x0` stub in
// retail (RVA 0x27d0): the base control takes no action when a container
// connects an event sink or freezes events.
//
// OnDoVerb and OnEnumVerbs tail-jump into the OLE verb-map machinery; a base
// COleControl declares no verb map, so both report FALSE / a null enumerator.
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

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout anchors used by the code below (offsets come from afxole.h comments
// and are asserted there too; these just catch accidental header regressions).
// COleControl is not standard-layout (virtuals), so GCC flags offsetof here;
// the sibling global_ole-5_impl.cpp relies on the same asserts. The benign
// diagnostic is suppressed for this block only.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" void MS_ABI impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" void MS_ABI impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" void MS_ABI impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(
    void* self);

// Retail computes the keyboard shift state with three GetKeyState calls
// (VK_SHIFT/VK_CONTROL/VK_MENU) folded into MK_SHIFT|MK_CONTROL|MK_ALT.
namespace openmfc { namespace detail { namespace ole6impl {
short GetShiftState();
} } }

//=============================================================================
// Keyboard / mouse events
//=============================================================================









//=============================================================================
// Stock-property change notifications
//=============================================================================







//=============================================================================
// OLE verbs
//=============================================================================




//=============================================================================
// Drawing
//=============================================================================



//=============================================================================
// Window lifecycle
//=============================================================================







//=============================================================================
// Event advise / freeze
//=============================================================================



//=============================================================================
// Persistence
//=============================================================================



