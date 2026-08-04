#pragma once
// Shared internals of the former global_mfc-feature-22_impl.cpp translation unit.
// Definitions live in detail/CMFCRibbonBaseElementSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribbonbaseelement {} } }
using namespace openmfc::detail::cmfcribbonbaseelement;
// CMFCRibbonBaseElement / CMFCRibbonButton exports implemented for the
// wave2_mfc-feature-22 batch (30 symbols: 13 button + 17 base element).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump).  The retail layout of both classes is much larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonBaseElement retail members used here include m_dwData (+0x110),
//   m_nID (+0x118), the tooltip/description strings (+0xa0/+0xa8/+0xb0), the
//   parent pointers (+0xe0/+0xf0), the "update tooltip info" flag (+0x174),
//   the tooltip-info-shortcut flag (+0x178), the popup pointer (+0x108) and
//   m_pOriginal (+0xe8).  OpenMFC declares only m_nID plus m_strText and
//   opaque padding, so everything except the ID is unreachable.  The small
//   ID-only functions (Find / FindByID / GetElementsByID / GetItemIDsList /
//   CanBeAddedToQuickAccessToolBar) are therefore transcribed faithfully
//   against GetID(), and everything else is a type-correct conservative
//   default with a TODO(clean-room) note.
//
//   CMFCRibbonButton retail members used here are entirely unmodeled: the
//   sub-item CArray (+0x248/+0x250/+0x258), the menu HMENU (+0x1d0) and menu
//   flags (+0x1f0/+0x21c/+0x1c4), the parent category/bar (+0xe0/+0xd8), the
//   m_pOriginal link (+0xe8), the geometry rects (+0x198/+0x1a8) and all the
//   mode flags (+0x144/+0x148/+0x13c/+0x154/+0x1f8/+0x1fc).  The clean-room
//   CMFCRibbonButton is a placeholder with only a 64-byte pad, so every
//   button mutator gets a type-correct no-op (or a NULL-checked safe default)
//   with a TODO(clean-room) note.  SetDescription is the exception: it only
//   forwards to the base element implementation (which OpenMFC keeps in the
//   ribbon_state side tables, see cbarcore.cpp), so it is implemented.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Classes referenced by the exported signatures but not declared in the
// clean-room headers.  Only pointers/array references are used, so a forward
// declaration is sufficient.
class CMFCRibbonKeyTip;
class CMFCRibbonCommandsListBox_MfcFeature22Impl;

// Defined in cbarcore.cpp (the ribbon_state side-table implementation of
// CMFCRibbonBaseElement::SetDescription); called by the button override below.
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
    CMFCRibbonBaseElement* pThis, const wchar_t* lpszText);

//=============================================================================
// CMFCRibbonButton
//=============================================================================
// The button's retail members (sub-item CArray, menu handle/flags, parent
// links, geometry rects and mode flags) are not modeled by the clean-room
// declaration, so these bodies are conservative NULL-checked no-ops.  They
// keep the exports present and ABI-correct without corrupting state.














//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================
// The base element's only modeled state is the command ID (GetID/SetID), so
// the ID-based queries below are transcribed faithfully; everything else is a
// type-correct conservative default.

















