#pragma once
// Shared internals of the former global_ole_09_msgmap.cpp translation unit.
// Definitions live in detail/Ole09MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ole09msgmap {} } }
using namespace openmfc::detail::ole09msgmap;
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
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Ole09Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP* AFXAPI gbm_COleCntrFrameWndEx();
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP* AFXAPI gbm_COleControl();
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP* AFXAPI gbm_COleDBRecordView();
} } }
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWnd();  // defined after maps (in-batch base COleIPFrameWnd)
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWndEx();  // defined after maps (in-batch base COleDocIPFrameWnd)
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWnd();
} } }
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWndEx();  // defined after maps (in-batch base COleIPFrameWnd)
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP* AFXAPI gbm_COlePropertyPage();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleCntrFrameWndEx_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleControl_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleDBRecordView_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleDocIPFrameWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleDocIPFrameWndEx_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleIPFrameWnd_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOleIPFrameWndEx_msgmap;
} } }
namespace openmfc { namespace detail { namespace ole09msgmap {
extern const AFX_MSGMAP classCOlePropertyPage_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWnd();
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWndEx();
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWndEx();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

