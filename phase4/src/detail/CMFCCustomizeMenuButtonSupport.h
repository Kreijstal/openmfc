#pragma once
// Shared internals of the former global_cmfccustomizemenubutton.cpp translation unit.
// Definitions live in detail/CMFCCustomizeMenuButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccustomizemenubutton {} } }
using namespace openmfc::detail::cmfccustomizemenubutton;
// OpenMFC: CMFCCustomizeMenuButton exports.
//
// All 6 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  It derives from
// CMFCToolBarMenuButton (296 bytes, declared in afxmfc.h) with its own members
// starting at +0x128; the whole object is 0x148 bytes.  The bodies that need
// the module-global customize maps or the CMFCPopupMenu / visual-manager
// machinery behind InvokeCommand / OnClickMenuItem / OnDraw / OnCalculateSize
// are kept as conservative type-correct versions with a `// TODO(clean-room)`
// marker.
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

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc);


namespace openmfc { namespace detail { namespace cmfccustomizemenubutton {
struct CZMenuButton : public CMFCToolBarMenuButton {
    UINT    m_nCommandID;      // 0x128
    int     m_b12c;            // 0x12c
    int     m_b130;            // 0x130
    int     m_b134;            // 0x134
    int     m_n138;            // 0x138
    int     m_b13c;            // 0x13c
    void*   m_pToolbar;        // 0x140
};
} } }








