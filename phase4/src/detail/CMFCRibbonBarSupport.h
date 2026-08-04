#pragma once
// Shared internals of the former global_mfc-feature-6_impl.cpp translation unit.
// Definitions live in detail/CMFCRibbonBarSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribbonbar {} } }
using namespace openmfc::detail::cmfcribbonbar;
// CMFCRibbonBar exports implemented for the wave1_mfc-feature-6 batch.
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump) rather than inferred, and member offsets referenced below are the
// ones declared in include/openmfc/afxmfc.h (pinned by static_asserts in
// phase4/src/mfccore.cpp):
//
//   m_bRecalcCategoryHeight        1060 (0x424)
//   m_bRecalcCategoryWidth         1064 (0x428)
//   m_bTracked                     1068 (0x42c)
//   m_bForceRedraw                 1080 (0x438)
//   m_bAutoCommandTimer            1088 (0x440)
//   m_dwHideFlags                  1144 (0x478)
//   m_pMainButton                  1152 (0x480)
//   m_pHighlighted                 1160 (0x488)
//   m_pPressed                     1168 (0x490)
//   m_arContextCaptions            2856 (0xb28)  opaque CArray blob
//   m_arCategories                 2896 (0xb50)  opaque CArray blob
//   m_arKeyElements                2936 (0xb80)  opaque CArray blob
//   m_CaptionButtons               3096 (0xc18)  opaque group blob
//   m_QAToolbar                    4992 (0x1380) opaque group blob
//
// A lot of the retail bodies walk ribbon objects OpenMFC does not model:
// CMFCRibbonButtonsGroup / CMFCRibbonQuickAccessToolBar / the category
// internal panel groups (retail reads element rects at +0xc8, category
// context IDs at +0xac and visibility at +0xa8, key tips, accessibility
// objects). Those functions get type-correct conservative bodies with a
// TODO(clean-room) note, exactly like the earlier ribbon batches. The ones
// that only need the ribbon_state.h side tables (categories/panels/elements)
// are implemented against that state so they return working results instead
// of reading uninitialized retail-shaped blobs.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "detail/RibbonState.h"

#include <mutex>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc::ribbon_state::RibbonMutex;
using openmfc::ribbon_state::RibbonBarStates;
using openmfc::ribbon_state::RibbonCategoryStates;
using openmfc::ribbon_state::RibbonPanelStates;

// CKeyboardManager::TranslateCharToUpper(unsigned int) -- defined in
// mfccore.cpp; used by CMFCRibbonBar::OnBeforeProcessKey.
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int ch);

//=============================================================================
// Category lookup
//=============================================================================




//=============================================================================
// Element collection
//=============================================================================





//=============================================================================
// Context captions
//=============================================================================






//=============================================================================
// Layout
//=============================================================================



//=============================================================================
// Hit testing / keyboard navigation
//=============================================================================









//=============================================================================
// Context categories
//=============================================================================


//=============================================================================
// Resource / state loading
//=============================================================================





//=============================================================================
// Misc window behaviour
//=============================================================================



