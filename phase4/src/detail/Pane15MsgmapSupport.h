#pragma once
// Shared internals of the former global_pane_15_msgmap.cpp translation unit.
// Definitions live in detail/Pane15MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace pane15msgmap {} } }
using namespace openmfc::detail::pane15msgmap;
// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 8 classes. Each gets a base-chained AFX_MSGMAP with an empty
// terminator entry array (real handler entries are mfc140u code addresses we
// cannot replicate). pfnGetBaseMap delegates to the base's exported
// GetThisMessageMap (real-MFC _AFXDLL base resolution); base harvested by
// pointer identity from mfc140u.dll. GetMessageMap is virtual-const (takes a
// this ptr), GetThisMessageMap is static; both return the static map. The
// // Symbol: markers drive build_phase4.sh's weak-stub auto-exclude.
// Named global_*_msgmap.cpp so the shard glob compiles it.

#include "openmfc/afxwin.h"   // AFX_MSGMAP, AFX_MSGMAP_ENTRY, AfxSig_end, AFXAPI

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Shared empty message-map entries (terminator only): the class exposes no
// handlers we implement; routing falls through to the base via pfnGetBaseMap.
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane15Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CAutoHideDockSite();  // defined after maps (in-batch base CDockSite)
namespace openmfc { namespace detail { namespace pane15msgmap {
const AFX_MSGMAP* AFXAPI gbm_CBasePane();
} } }
const AFX_MSGMAP* AFXAPI gbm_CBaseTabbedPane();  // defined after maps (in-batch base CDockablePane)
namespace openmfc { namespace detail { namespace pane15msgmap {
const AFX_MSGMAP* AFXAPI gbm_CDockBar();
} } }
const AFX_MSGMAP* AFXAPI gbm_CDockSite();  // defined after maps (in-batch base CBasePane)
namespace openmfc { namespace detail { namespace pane15msgmap {
const AFX_MSGMAP* AFXAPI gbm_CDockablePane();
} } }
const AFX_MSGMAP* AFXAPI gbm_CDockablePaneAdapter();  // defined after maps (in-batch base CDockablePane)
const AFX_MSGMAP* AFXAPI gbm_CDummyDockablePane();  // defined after maps (in-batch base CDockablePane)

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCAutoHideDockSite_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCBasePane_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCBaseTabbedPane_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCDockBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCDockSite_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCDockablePane_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCDockablePaneAdapter_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane15msgmap {
extern const AFX_MSGMAP classCDummyDockablePane_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_CAutoHideDockSite();
const AFX_MSGMAP* AFXAPI gbm_CBaseTabbedPane();
const AFX_MSGMAP* AFXAPI gbm_CDockSite();
const AFX_MSGMAP* AFXAPI gbm_CDockablePaneAdapter();
const AFX_MSGMAP* AFXAPI gbm_CDummyDockablePane();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

