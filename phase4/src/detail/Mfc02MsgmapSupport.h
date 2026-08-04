#pragma once
// Shared internals of the former global_mfc_02_msgmap.cpp translation unit.
// Definitions live in detail/Mfc02MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfc02msgmap {} } }
using namespace openmfc::detail::mfc02msgmap;
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
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Mfc02Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCAcceleratorKeyAssignCtrl();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCAutoHideBar();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseTabCtrl();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseToolBar();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCButton();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCCaptionBar();
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCColorBar();
} } }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorButton();  // defined after maps (in-batch base CMFCButton)

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCAcceleratorKeyAssignCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCAutoHideBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCBaseTabCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCBaseToolBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCButton_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCCaptionBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCColorBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc02msgmap {
extern const AFX_MSGMAP classCMFCColorButton_msgmap;
} } }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorButton();

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

