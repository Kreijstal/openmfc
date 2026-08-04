#pragma once
// Shared internals of the former global_cmfcacceleratorkeyassignctrl.cpp translation unit.
// Definitions live in detail/CMFCAcceleratorKeyAssignCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcacceleratorkeyassignctrl {} } }
using namespace openmfc::detail::cmfcacceleratorkeyassignctrl;
// OpenMFC: CMFCAcceleratorKeyAssignCtrl exports.
//
// All 4 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (opaque), so `this` is
// taken as void*.  Only the +0xec flag byte and the +0xe8 enable flags touched
// by ResetKey / SetAccelFlag are mirrored through raw offsets; the +0x108
// kill-focus marker and the PreTranslateMessage state machine (+0xec..+0x108,
// the +0xf8 CString) are unmodeled, so those bodies keep their NULL checks and
// return the retail terminal value with a `// TODO(clean-room)` marker.
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

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
    CWnd* pThis, MSG* pMsg);


namespace openmfc { namespace detail { namespace cmfcacceleratorkeyassignctrl {
struct CAccelKeyAssignCtrl {
    char          _pad000[0x40];
    HWND          m_hWnd;          // 0x040: CWnd::m_hWnd
    char          _pad048[0xe8 - 0x48];
    int           m_bAccelFlags;   // 0x0e8: enable flags (ResetKey clears)
    unsigned char m_flags;         // 0x0ec: tracked modifier/key flags
    char          _pad0ed[0x108 - 0xed];
    int           m_bKillFocus;    // 0x108: kill-focus marker (OnKillFocus)
};
} } }



//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::OnKillFocus
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::PreTranslateMessage
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::ResetKey
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl::SetAccelFlag
//-----------------------------------------------------------------------------
