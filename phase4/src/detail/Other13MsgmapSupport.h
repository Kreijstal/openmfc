#pragma once
// Shared internals of the former global_other_13_msgmap.cpp translation unit.
// Definitions live in detail/Other13MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace other13msgmap {} } }
using namespace openmfc::detail::other13msgmap;
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
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Other13Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CSplitButton();
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CSplitterWnd();
} } }
const AFX_MSGMAP* AFXAPI gbm_CSplitterWndEx();  // defined after maps (in-batch base CSplitterWnd)
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CStatusBar();
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CTabView();
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CToolBar();
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CToolBarCtrl();
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
const AFX_MSGMAP* AFXAPI gbm_CVSListBox();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCSplitButton_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCSplitterWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCSplitterWndEx_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCStatusBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCTabView_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCToolBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCToolBarCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace other13msgmap {
extern const AFX_MSGMAP classCVSListBox_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_CSplitterWndEx();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

