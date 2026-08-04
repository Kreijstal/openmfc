#pragma once
// Shared internals of the former global_mfc-feature-7_impl.cpp translation unit.
// Definitions live in detail/MfcFeature7ImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcfeature7impl {} } }
using namespace openmfc::detail::mfcfeature7impl;
// CMFCRibbonBar exports implemented for the wave2_mfc-feature-7 batch.
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump) rather than inferred, and member offsets referenced below are the
// ones declared in include/openmfc/afxmfc.h (pinned by static_asserts in
// phase4/src/mfccore.cpp):
//
//   m_dwHideFlags                  1144 (0x478)
//   m_pMainButton                  1152 (0x480)
//   m_pHighlighted                 1160 (0x488)
//   m_pPressed                     1168 (0x490)
//   m_TabElements                  1176 (0x498)  opaque CMFCRibbonButtonsGroup
//   m_pActiveCategory              2824 (0xb08)
//   m_arCategories                 2896 (0xb50)  opaque CArray blob
//   m_arKeyElements                2936 (0xb80)  opaque CArray blob
//   m_sizeMainButton               3064 (0xbf8)
//   m_pKeyboardNavLevelParent      3080 (0xc08)
//   m_pKeyboardNavLevelCurrent     3088 (0xc10)
//   m_CaptionButtons               3096 (0xc18)  opaque group blob
//   m_QAToolbar                    4992 (0x1380) opaque group blob
//   m_bSingleLevelAccessibilityMode 6736
//   m_Tabs                         6744 (0x1a58) opaque CMFCRibbonTabsGroup
//
// Most of these handlers walk the four opaque embedded group blobs
// (m_TabElements / m_CaptionButtons / m_QAToolbar / m_Tabs), the undeclared
// tab-group counter at +0x410 and the tooltip/size machinery behind the
// private messages 0x407/0x418/0x41c. Those functions get type-correct
// conservative bodies with a TODO(clean-room) note, exactly like the earlier
// ribbon batches. The ones that only need the ribbon_state.h side tables or
// declared members (category management, PreCreateWindow, OnSize's hide-flag
// logic, SetApplicationButton, SetMaximizeMode tail) are implemented against
// that state so they behave instead of reading uninitialized retail-shaped
// blobs.
//
// Retail addresses for the folded CWnd default handlers: 0x18028ac80 is the
// COMDAT-folded "CWnd::OnXxx() { Default(); }" used as the base of
// OnShowWindow/OnSysCommand/OnTimer/OnSizing; ::DefWindowProcW is its exact
// semantic equivalent and is used below instead of those internal addresses.
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
using openmfc::ribbon_state::RibbonBarState;

//---------------------------------------------------------------------------
// Cross-translation-unit helpers.
//---------------------------------------------------------------------------

// CMFCRibbonBar::ForceRecalcLayout -- defined in global_mfc-feature-6_impl.cpp
// (wave-1 batch); used by OnSettingChange below.
extern "C" void MS_ABI impl__ForceRecalcLayout_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis);

// CWnd::PreCreateWindow -- defined in wincore.cpp; base of PreCreateWindow.
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs);

// CMFCRibbonCategory::~CMFCRibbonCategory -- defined in cbarcore.cpp; removes
// the category from the ribbon side tables and runs the real destructor.
extern "C" void MS_ABI impl___1CMFCRibbonCategory__UEAA_XZ(void* pThis);

// CMFCRibbonBar::RecalcLayout (this file) -- the vtable+0x430 dispatch used by
// OnSize / SetMaximizeMode resolves to this override in retail.
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(
    CMFCRibbonBar* pThis);

// CMFCRibbonBar::SetKeyboardNavigationLevel (this file) -- timer 0xec1a fires
// this as a delayed key-tip navigation reset.
extern "C" void MS_ABI impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
    CMFCRibbonBar* pThis, CObject* pObj, int nLevel);

//=============================================================================
// Window messages
//=============================================================================
















//=============================================================================
// Window creation
//=============================================================================


//=============================================================================
// Keyboard navigation
//=============================================================================



//=============================================================================
// Layout
//=============================================================================


//=============================================================================
// Category management
//=============================================================================



//=============================================================================
// Tabs
//=============================================================================



//=============================================================================
// Key tips
//=============================================================================



//=============================================================================
// State persistence
//=============================================================================




//=============================================================================
// Application button / maximize mode
//=============================================================================


