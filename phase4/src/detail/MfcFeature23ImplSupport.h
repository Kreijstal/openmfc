#pragma once
// Shared internals of the former global_mfc-feature-23_impl.cpp translation unit.
// Definitions live in detail/MfcFeature23ImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcfeature23impl {} } }
using namespace openmfc::detail::mfcfeature23impl;
// CMFCRibbonBaseElement / CMFCRibbonPanel / CMFCRibbonSeparator exports
// implemented for the wave2_mfc-feature-23 batch (30 symbols: 12 base element
// + 13 panel + 5 separator).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump).  The retail layouts of all three classes are far larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonBaseElement retail members touched by this batch: the tooltip /
//   key CStrings (+0xa8/+0xb0), the parent pointers (+0xd8/+0xe0/+0xe8/+0xf8),
//   the rect (+0xc8) and its edges (+0xcc/+0xd0/+0xd4), m_nID (+0x118), the
//   text-always-on-right flag (+0x124), the stretch flags (+0x12c/+0x130), the
//   update-tooltip flag (+0x174) and the accessibility-mode flag (+0x17c).
//   OpenMFC declares only m_nID plus m_strText and opaque padding, so
//   everything except the ID is unreachable.  The small ID-based queries were
//   already implemented in global_mfc-feature-22_impl.cpp; the remaining
//   bodies here are type-correct conservative defaults with TODO notes.
//
//   CMFCRibbonSeparator retail members used here are the base element plus a
//   single 4-byte flag at +0x180 (drawn-size mode).  OpenMFC's separator is a
//   32-byte pad on top of the base, so the flag is unreachable.  The one fully
//   transcribed body is GetRegularSize (always CSize(4,4), no state needed);
//   CopyFrom delegates to the base CopyFrom; the draw/list functions are
//   conservative no-ops.
//
//   CMFCRibbonPanel retail members used here are the name CString (+0x140),
//   the layout members (+0xa0..+0x118, +0x140..+0x158, +0x288, +0x328), the
//   pressed element (+0x3e8), the launch button (+0x3f0), the buttons-group
//   (+0x170) and the two CArrays (+0x6a8/+0x6d8).  OpenMFC declares only
//   m_strName (protected, unreachable from a free function) plus m_hIcon and
//   opaque padding; panel membership lives in the ribbon_state side table (see
//   cbarcore.cpp / mfccore.cpp), so the two ID/state queries that can use it
//   (FindByID, GetDroppedDown) iterate the side table and everything else is
//   a conservative default with a TODO note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "detail/RibbonState.h"

#include <cstdint>
#include <mutex>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Classes referenced by the exported signatures but not declared in the
// clean-room headers.  Only pointers are used, so a forward declaration is
// sufficient.
class CMFCRibbonPanelMenuBar;
class CMFCRibbonCommandsListBox_MfcFeature23Impl;

// Accessibility return codes from wtypes.h / winerror.h; keep the numeric
// spellings so the file compiles even when <oleauto.h> is not pulled in.
#ifndef S_OK
  #define S_OK 0L
#endif
#ifndef S_FALSE
  #define S_FALSE 1L
#endif
#ifndef E_INVALIDARG
  #define E_INVALIDARG 0x80070057L
#endif

namespace openmfc { namespace detail { namespace mfcfeature23impl {
using openmfc::ribbon_state::RibbonPanelStates;
} } }
namespace openmfc { namespace detail { namespace mfcfeature23impl {
using openmfc::ribbon_state::RibbonMutex;
} } }

// Defined in global_mfc-feature-22_impl.cpp.
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(
    CMFCRibbonBaseElement* pThis, unsigned int nID);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(
    CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement& src);

//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================
// Only m_nID (GetID/SetID) is modeled, so none of the member writes below can
// be transcribed; the bodies are NULL-checked, type-correct conservative
// defaults (no-op / safe return) matching what the sibling
// global_mfc-feature-22_impl.cpp wave established for this class.








//=============================================================================
// CMFCRibbonBaseElement -- IAccessible methods
//=============================================================================
// The MSAA entry points below mostly gate on the accessibility-mode flag at
// +0x17c and on the parent bar/menu state (+0xd8/+0xf0), none of which is
// modeled.  The NULL/error-code contracts are transcribed faithfully; the
// flag-off results (S_FALSE / untouched output variants) are what retail
// returns for a freshly constructed element, which is the only state OpenMFC
// can produce.






//=============================================================================
// CMFCRibbonSeparator
//=============================================================================
// The separator's only retail member beyond the base element is a 4-byte flag
// at +0x180 (drawn-size mode), which the clean-room declaration does not
// model.  GetRegularSize needs no state and is transcribed faithfully;
// everything else is conservative.






//=============================================================================
// CMFCRibbonPanel
//=============================================================================
// Panel membership lives in the ribbon_state side table (see CMFCRibbonPanel
// ctor/Add/AddSeparator in mfccore.cpp / cbarcore.cpp), so the queries that
// are pure element searches (FindByID) are implemented over it.  Everything
// else touches retail members (name CString at +0x140, layout members, the
// launch button at +0x3f0, the buttons-group at +0x170, the element CArray at
// +0x6d8/+0x6e0) that the clean-room declaration does not model, so those
// bodies are type-correct conservative defaults with TODO notes.













