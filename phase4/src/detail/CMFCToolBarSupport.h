#pragma once
// Shared internals of the former global_mfc-feature-3_impl.cpp translation unit.
// Definitions live in detail/CMFCToolBarSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctoolbar {} } }
using namespace openmfc::detail::cmfctoolbar;
// CMFCToolBar Feature Pack exports, wave 3 ("mfc-feature-3").
//
// Each function is transcribed from the retail mfc140u export
// (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350, i.e.
// m_ImagesLocked .. m_AccelKeys). Where the retail body needs internal helpers
// or virtual slots that OpenMFC does not model, the implementation keeps the
// visible side effects (member writes, window invalidation) and returns the
// safe default, with a `// TODO(clean-room)` marker.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Statics with storage in feature_static_data.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void*        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA;

// Forward declaration: NextMenu, OnCancelMode and OnContextMenu call
// OnChangeHot, which is defined later in this file.
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot);

// CMFCToolBarInfo is not declared in OpenMFC headers. The layout-faithful mirror
// below (8 x UINT, size 32) is identical to the one used by the exported
// CMFCToolBarInfo ctor in global_cmfctoolbarinfo.cpp, so the by-reference
// parameter matches the MSVC-mangled AEAVCMFCToolBarInfo@@ type exactly.
struct CMFCToolBarInfo {
    UINT m_uiColdResID;          // +0  Toolbar regular images
    UINT m_uiHotResID;           // +4  Toolbar "hot" images
    UINT m_uiDisabledResID;      // +8  Toolbar disabled images
    UINT m_uiLargeColdResID;     // +12 Toolbar large regular images
    UINT m_uiLargeHotResID;      // +16 Toolbar large "hot" images
    UINT m_uiLargeDisabledResID; // +20 Toolbar large disabled images
    UINT m_uiMenuResID;          // +24 Menu images
    UINT m_uiMenuDisabledResID;  // +28 Menu disabled images
};

// CSettingsStore is not declared in OpenMFC headers either. Only a forward
// declaration is used so the by-reference parameter matches AEAVCSettingsStore@@;
// the two functions that take it never dereference the object.
class CSettingsStore;

//=============================================================================
// CMFCToolBar::InvalidateButton
//=============================================================================

//=============================================================================
// CMFCToolBar::IsCommandRarelyUsed (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::IsLastCommandFromButton (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::IsUserDefined
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadBitmapEx
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadBitmapW
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadLargeIconsState (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadLastOriginalState
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadParameters (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadResetOriginalState
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadState
//=============================================================================

//=============================================================================
// CMFCToolBar::LoadToolBarEx
//=============================================================================

//=============================================================================
// CMFCToolBar::NextMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::NotifyControlCommand
//=============================================================================

//=============================================================================
// CMFCToolBar::OnAfterButtonDelete
//=============================================================================

//=============================================================================
// CMFCToolBar::OnAfterDock
//=============================================================================

//=============================================================================
// CMFCToolBar::OnAfterFloat
//=============================================================================

//=============================================================================
// CMFCToolBar::OnBeforeChangeParent
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCalcSeparatorRect
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCancelMode
//=============================================================================

//=============================================================================
// CMFCToolBar::OnChangeHot
//=============================================================================

//=============================================================================
// CMFCToolBar::OnChangeVisualManager
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCommand
//=============================================================================

//=============================================================================
// CMFCToolBar::OnContextMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCopyImage
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCreate
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCtlColor
//=============================================================================

//=============================================================================
// CMFCToolBar::OnCustomizeMode
//=============================================================================

//=============================================================================
// CMFCToolBar::OnDestroy
//=============================================================================

//=============================================================================
// CMFCToolBar::OnDragEnter
//=============================================================================
