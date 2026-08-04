#pragma once
// Shared internals of the former global_mfc-feature-17_impl.cpp translation unit.
// Definitions live in detail/CMFCRibbonCategorySupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribboncategory {} } }
using namespace openmfc::detail::cmfcribboncategory;
// CMFCRibbonCategory / CMFCRibbonGallery exports implemented for the
// wave2_mfc-feature-17 batch (30 symbols).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump). The retail layouts of both classes are much larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonCategory retail members used here include the panel CArray at
//   +0x7a8/+0x7b0, the hidden-element CArray at +0xb00/+0xb08/+0xb10, image
//   sizes at +0x838/+0x9d0, the scroll-button chain (+0x768) and the size
//   cache members (+0xb0/+0x770). OpenMFC declares only m_strName plus opaque
//   padding, and keeps the panel list in ribbon_state.h instead, so the
//   category functions are implemented against that side table where they
//   can return working results (GetElements / GetElementsByID /
//   GetItemIDsList / FindByID / FindPanelWithElem / GetPanelIndex /
//   first/last-visible-element) and conservatively otherwise.
//
//   CMFCRibbonGallery retail members used in these bodies (m_Items at +0x480,
//   m_arGroups at +0x2a0/+0x2a8, m_Images at +0x2e8, m_nNotifyParentID at
//   +0x4d0, m_bBuilt at +0x4e0 ...) are entirely unmodeled: the clean-room
//   class is a placeholder with only a 32-byte pad, so every gallery mutator
//   gets a type-correct no-op with a TODO(clean-room) note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "detail/RibbonState.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc::ribbon_state::RibbonMutex;
using openmfc::ribbon_state::RibbonCategoryStates;
using openmfc::ribbon_state::RibbonPanelStates;

//=============================================================================
// Static data export
//=============================================================================
// CMFCRibbonGallery::m_mapSelectedItems is a CMap<UINT,UINT,int,int> static.
// The retail object is 56 bytes (same template as CMFCToolBar::m_DefaultImages
// in global_mfc-feature-5_impl.cpp, which documents the harvested size), while
// the clean-room CMap template in afx.h is only 40 bytes, so the export uses
// an opaque 56-byte blob to keep the exported object at retail size.
struct alignas(8) CMap_IIHH_56Bytes {
    unsigned char data[56];
};


//=============================================================================
// CMFCRibbonGallery
//=============================================================================
// All eight gallery functions touch member blocks the clean-room class does
// not declare (items at +0x480, groups at +0x2a0, the image list at +0x2e8,
// the notify-parent id at +0x4d0, the "built" flag at +0x4e0 ...).  Nothing
// can be transcribed against declared members, so each body is a type-correct
// no-op that neither reads nor corrupts state.








//=============================================================================
// CMFCRibbonCategory
//=============================================================================
// The category keeps its panel list in ribbon_state.h (see AddPanel in
// cbarcore.cpp / mfccore.cpp), so the search/collection methods below are
// implemented against that side table.  The retail hidden-element CArray
// (+0xb00) is not modeled, so hidden elements never participate.






















