#pragma once
// Shared internals of the former global_cmfcautohidebutton.cpp translation unit.
// Definitions live in detail/CMFCAutoHideButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcautohidebutton {} } }
using namespace openmfc::detail::cmfcautohidebutton;
// OpenMFC: CMFCAutoHideButton exports.
//
// All 10 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is taken
// as void*.  The retail body of nearly every method reads the auto-hide bar at
// +0x30 or the attached pane at +0x38 and dispatches through their
// vslot-0x5a0-family virtuals (OnDrawBorder / OnFillBackground / OnDraw route
// through the visual-manager vslots 0x368/0x370/0x378); none of that state is
// modeled.  The trivial pure-data methods (ShowButton -> +0xc, the byte flag
// work in SetAccelFlag) are transcribed faithfully through raw offsets; the
// rest terminates on the retail failure terminal.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleacc.h>

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cmfcautohidebutton {
struct CAutoHideButton {
    char    _pad000[0x0c];
    int     m_nShow;         // 0x0c: ShowButton / Create stores here
    char    _pad010[0x14 - 0x10];
    unsigned long m_dwStyle; // 0x14: button style flags (Create/Move read 0xa000)
    char    _pad018[0x1c - 0x18];
    RECT    m_rect;          // 0x1c: button rect (Move passes &+0x1c)
    char    _pad02c[0x30 - 0x2c];
    void*   m_pAutoHideBar;  // 0x30: CMFCAutoHideBar*
    void*   m_pPane;         // 0x38: CDockablePane* (attached window)
};
} } }



//-----------------------------------------------------------------------------
// CMFCAutoHideButton::Create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::CreateObject
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::Move
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::OnDraw
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::OnDrawBorder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::OnFillBackground
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::ReplacePane
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::ShowAttachedWindow
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::ShowButton
//-----------------------------------------------------------------------------

//=============================================================================
// CMFCAutoHideButton - class not declared in the OpenMFC headers (opaque).
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCAutoHideButton::UnSetAutoHideMode
//-----------------------------------------------------------------------------
