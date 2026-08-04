#pragma once
// Shared internals of the former global_other_11_msgmap.cpp translation unit.
// Definitions live in detail/Other11MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace other11msgmap {} } }
using namespace openmfc::detail::other11msgmap;
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
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Other11Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP* AFXAPI gbm_CCommonDialog();
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP* AFXAPI gbm_CControlBar();
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP* AFXAPI gbm_CCtrlView();
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP* AFXAPI gbm_CDHtmlDialog();
} } }
const AFX_MSGMAP* AFXAPI gbm_CDialogBar();  // defined after maps (in-batch base CControlBar)
const AFX_MSGMAP* AFXAPI gbm_CDocObjectServer();  // defined after maps (in-batch base CCmdTarget)
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP* AFXAPI gbm_CHtmlEditView();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCCmdTarget_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCCommonDialog_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCControlBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCCtrlView_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCDHtmlDialog_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCDialogBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCDocObjectServer_msgmap;
} } }
namespace openmfc { namespace detail { namespace other11msgmap {
extern const AFX_MSGMAP classCHtmlEditView_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_CDialogBar();
const AFX_MSGMAP* AFXAPI gbm_CDocObjectServer();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

