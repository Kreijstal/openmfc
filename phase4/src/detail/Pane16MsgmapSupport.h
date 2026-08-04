#pragma once
// Shared internals of the former global_pane_16_msgmap.cpp translation unit.
// Definitions live in detail/Pane16MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace pane16msgmap {} } }
using namespace openmfc::detail::pane16msgmap;
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
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane16Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMiniDockFrameWnd();
} } }
const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd();  // defined after maps (in-batch base CPaneFrameWnd)
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CPane();
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CPaneDialog();
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CPaneDivider();
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CPaneFrameWnd();
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CPaneTrackingWnd();
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingGroupGuidesWnd();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCMiniDockFrameWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCMultiPaneFrameWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCPane_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCPaneDialog_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCPaneDivider_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCPaneFrameWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCPaneTrackingWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace pane16msgmap {
extern const AFX_MSGMAP classCSmartDockingGroupGuidesWnd_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

