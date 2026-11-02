#pragma once
// Shared internals of the former global_other-6_impl.cpp translation unit.
// Definitions live in detail/CPaneFrameWndSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cpaneframewnd {} } }
using namespace openmfc::detail::cpaneframewnd;
// CPaneFrameWnd message handlers and helpers, wave "other-6".
// Retail CPaneFrameWnd is 0x258 bytes and derives from CWnd; the OpenMFC public
// declaration remains a smaller CMiniFrameWnd-derived object. Consequently all
// retail fields below are pThis-keyed side-table state, never raw offsets into
// the OpenMFC allocation. The base constructor entry RVA 0xad110 (mfc140u)
// establishes the member stores, while AddButton entry RVA 0xb0f50 and
// ArrangeCaptionButtons entry RVA 0xb13d0 (mfc140u) establish the caption list
// representation and button fields. Existing CWnd storage (m_hWnd +0x40) is
// still accessed through the declared CWnd base only.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "detail/DockingState.h"

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
    bool blockMove = false;                 // m_bBlockMove +0x0e8
    HWND exitSizeMoveHwnd = nullptr;        // m_hLastFocusWnd +0x140
    HWND paneHwnd = nullptr;                // m_hEmbeddedBar +0x148
    int  dockTimerID = 0;                   // m_nDockTimerID +0x120
    int  rollupTimerID = 0;                 // m_nRollTimerID +0x124
    int  rollupFlags = 0;                   // m_dwCaptionButtons +0x12c
    int  idleActive = 0;                    // m_bActive +0xec
    bool moving = false;                    // m_bIsMoving +0xf0
    bool enablePreDockState = false;        // m_bEnablePreDockState +0xf4
    bool rolledUp = false;                  // m_bRolledUp +0xf8
    bool pinned = false;                    // m_bPinned +0xfc
    bool noDelayedDestroy = false;          // m_bNoDelayedDestroy +0x100
    bool hostsToolbar = false;               // m_bHostsToolbar +0x104
    bool delayShow = false;                 // m_bDelayShow +0x108
    bool tabDragRectDisplayed = false;       // m_bTabDragRectDisplayed +0x10c
    bool dragging = false;                  // m_bCaptured +0x10d
    UINT heightBeforeRollUp = 0;             // m_nHeightBeforeRollUp +0x110
    UINT captionHeight = 0;                  // m_nCaptionHeight +0x114

    UINT hitButtonID = 0;                   // m_nHit +0x118
    UINT hotButtonID = 0;                   // m_nHot +0x11c
    UINT restoredEmbeddedBarID = 0;         // m_nRestoredEmbeddedBarID +0x128
    HWND ownerHwnd = nullptr;               // m_hParentWnd +0x130
    HWND destroyOnReleaseHwnd = nullptr;    // m_hWndToDestroyOnRelease +0x138
    RECT recentFloatingRect{};              // m_rectRecentFloatingRect +0x150
    SIZE minSize{};                         // m_sizeMinSize +0x160
    RECT redrawRect{};                      // m_rectRedraw +0x168
    POINT captionButtonHotPoint{};          // m_ptHot +0x178
    CMFCDragFrameImpl dragFrame{};           // m_dragFrameImpl +0x1c8
    openmfc::docking_state::CaptionButtonState* captionButtons = nullptr; // side data for m_lstCaptionButtons @+0x180
    std::size_t captionButtonCount = 0;      // retail CObList m_nCount +0x198
    std::size_t captionButtonCapacity = 0;   // side-table allocation capacity
    CToolTipCtrl* toolTip = nullptr;         // m_pToolTip +0x1b8
    CDockablePane* preDockPane = nullptr;    // m_pPreDockBar +0x1c0
    openmfc::docking_state::DockingTooltipState* toolTips = nullptr;
    std::size_t toolTipCount = 0;
    CDockingManager* dockManager = nullptr;  // m_pDockManager +0x248
    int preDockState = 0;                   // m_preDockStateCurr +0x250
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






























