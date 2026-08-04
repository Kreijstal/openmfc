#pragma once
// Shared internals of the former global_other-6_impl.cpp translation unit.
// Definitions live in detail/CPaneFrameWndSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cpaneframewnd {} } }
using namespace openmfc::detail::cpaneframewnd;
// CPaneFrameWnd message handlers and helpers, wave "other-6".
//
// Every function here was decoded from the retail mfc140u export (ordinal ->
// RVA -> objdump, see /tmp/openmfc_remaining/dis_out/*.txt) rather than
// inferred. The retail member offsets used below (0x140 exit-size-move focus
// hwnd, 0x148 pane hwnd, 0x120/0x124 timer ids, 0x12c roll-up flags, 0x10d
// dragging flag, 0xec idle-active flag) are NOT named in the OpenMFC headers
// (CPaneFrameWnd is modeled as a thin CMiniFrameWnd subclass), so object state
// is kept in a private side table - the same pattern cbarcore.cpp uses for the
// sibling CPaneFrameWnd exports. Only CWnd members declared in afxwin.h are
// accessed directly (m_hWnd @ 64 and the inline wrapper methods).
//
// Where a retail body depends on internal virtual slots (drag-frame impl,
// docking-manager / caption-button internals) or unexported globals, a
// type-correct conservative version is emitted (safe defaults, no state
// corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Side-table state for CPaneFrameWnd instances (retail members not modeled in
// include/openmfc/afxmfc.h). This mirrors the g_framePanes pattern from
// cbarcore.cpp; the two tables are independent, so entries default to the safe
// "nothing active" values below.
// ---------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace cpaneframewnd {
struct PaneFrameState {
    HWND exitSizeMoveHwnd = nullptr;  // retail 0x140: hwnd to re-focus after sizing
    HWND paneHwnd = nullptr;          // retail 0x148: hwnd of the hosted pane
    int  dockTimerID = 0;             // retail 0x120
    int  rollupTimerID = 0;           // retail 0x124
    int  rollupFlags = 0;             // retail 0x12c
    int  idleActive = 0;              // retail 0xec (bool-ish)
    bool dragging = false;            // retail 0x10d (byte)
};
} } }

namespace openmfc { namespace detail { namespace cpaneframewnd {
extern std::mutex g_other6Mutex;
} } }
namespace openmfc { namespace detail { namespace cpaneframewnd {
extern std::unordered_map<void*, PaneFrameState> g_other6State;
} } }

namespace openmfc { namespace detail { namespace cpaneframewnd {
CWnd* AsCwnd(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpaneframewnd {
HWND HwndOf(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpaneframewnd {
PaneFrameState& GetState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpaneframewnd {
PaneFrameState* FindStatePtr(const void* pThis);
} } }

// WM_NCACTIVATE / WM_IDLEUPDATECMDUI
#ifndef WM_NCACTIVATE
#define WM_NCACTIVATE 0x0086
#endif
#ifndef WM_IDLEUPDATECMDUI
#define WM_IDLEUPDATECMDUI 0x0363
#endif


// ---------------------------------------------------------------------------
// Sibling impl_ exports (defined in other phase4/src/*.cpp)
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void   MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" CWnd*  MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd*  MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int    MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                        const CObject* pThis, const CRuntimeClass* pClass);
extern "C" int    MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
                        CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd,
                        unsigned int nFlags);
extern "C" void   MS_ABI impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(
                        void* pThis, int bMoveToFront);

// Global customize-mode flag (CMFCToolBar::m_bCustomizeMode) + RTTI getter.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();

// ---------------------------------------------------------------------------
// Exports
// ---------------------------------------------------------------------------






























