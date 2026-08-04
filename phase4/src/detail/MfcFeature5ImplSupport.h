#pragma once
// Shared internals of the former global_mfc-feature-5_impl.cpp translation unit.
// Definitions live in detail/MfcFeature5ImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcfeature5impl {} } }
using namespace openmfc::detail::mfcfeature5impl;
// CMFCToolBar Feature Pack exports, wave 5 ("mfc-feature-5").
//
// 20 methods + 10 static data exports transcribed from the retail mfc140u
// export (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350).
//
// Where the retail body depends on unmodeled state (visual-manager virtuals,
// the AFX_GLOBAL_DATA singleton, CMFCToolBarImages::OnSysColorChange is an
// auto-stub, CObList internals live in a side table) the implementation keeps
// the observable side effects (member writes, region creation, WM_COMMAND
// posts, VARIANT results) and marks the gaps with `// TODO(clean-room)`.
//
// The ten DATA exports (CMFCToolBar's static image lists plus the drop source
// and the default-command map) are defined at the top of this file with
// `extern "C"` storage; the `// Symbol:` comments exclude them from the
// auto-generated stubs.  The eight CMFCToolBarImages statics use the modeled
// 408-byte type; m_DefaultImages (retail CMap<UINT,UINT,int,int>, 56 bytes)
// and m_DropSource (retail CMFCToolBarDropSource, 144 bytes) are opaque local
// structs sized to the harvested retail layout because those classes are not
// declared in the clean-room headers.  UpdateImagesColor drives
// OnSysColorChange over 7 of the 8 statics (m_DisabledImages is skipped in
// retail) plus all 8 *_Locked lists.

#include "openmfc/afxmfc.h"

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// VT_EMPTY / VT_I4 and S_OK / S_FALSE / E_INVALIDARG from wtypes.h (via
// windows.h) are used below; keep the numeric spellings for the HRESULTs so
// the file compiles even when <oleauto.h> is not pulled in.
#ifndef S_OK
  #define S_OK 0L
#endif
#ifndef S_FALSE
  #define S_FALSE 1L
#endif
#ifndef E_INVALIDARG
  #define E_INVALIDARG 0x80070057L
#endif

// ---------------------------------------------------------------------------
// Cross-file impl_ helpers.
// ---------------------------------------------------------------------------

// CMFCToolBarImages::OnSysColorChange (auto-stub until a real impl lands).
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);

//=============================================================================
// Static data exports
//=============================================================================
// Retail sizes harvested from the .data section of mfc140u.dll:
//   m_DefaultImages (CMap<UINT,UINT,int,int>) = 56 bytes  (0x3b1f90)
//   m_DropSource     (CMFCToolBarDropSource)  = 144 bytes (0x3c2510)
//   eight CMFCToolBarImages (408 bytes each)  = 0x3c25a0..0x3c3100
// The OpenMFC CMap<UINT,UINT,int,int> template is 40 bytes (no CObject base),
// so m_DefaultImages uses an opaque 56-byte blob to keep the exported object
// at retail size; CMFCToolBarDropSource is not declared in the clean-room
// headers, so m_DropSource uses a local 144-byte blob.

struct alignas(8) CMap_IIHH_56Bytes {
    unsigned char data[56];
};

struct alignas(8) CToolBarDropSource_144Bytes {
    unsigned char data[144];
};











//=============================================================================
// CMFCToolBar::SetRoundedRgn
//=============================================================================

//=============================================================================
// CMFCToolBar::SetSiblingToolBar
//=============================================================================

//=============================================================================
// CMFCToolBar::SetToolBarBtnText
//=============================================================================

//=============================================================================
// CMFCToolBar::SetTwoRowsWithSibling
//=============================================================================

//=============================================================================
// CMFCToolBar::SetUserImages (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::ShowCommandMessageString
//=============================================================================

//=============================================================================
// CMFCToolBar::SizeToolBar
//=============================================================================

//=============================================================================
// CMFCToolBar::SmartUpdate
//=============================================================================

//=============================================================================
// CMFCToolBar::StretchPane
//=============================================================================

//=============================================================================
// CMFCToolBar::ToolBarMouseHookProc (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::TranslateChar
//=============================================================================

//=============================================================================
// CMFCToolBar::UpdateButton
//=============================================================================

//=============================================================================
// CMFCToolBar::UpdateImagesColor
//=============================================================================

//=============================================================================
// CMFCToolBar::UpdateTooltips
//=============================================================================

//=============================================================================
// CMFCToolBar::WrapToolBar
//=============================================================================

//=============================================================================
// CMFCToolBar::accDoDefaultAction (IAccessible)
//=============================================================================

//=============================================================================
// CMFCToolBar::accHitTest (IAccessible)
//=============================================================================

//=============================================================================
// CMFCToolBar::accNavigate (IAccessible)
//=============================================================================

//=============================================================================
// CMFCToolBar::get_accChild (IAccessible)
//=============================================================================

//=============================================================================
// CMFCToolBar::get_accChildCount (IAccessible)
//=============================================================================
