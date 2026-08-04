#pragma once
// Shared internals of the former global_ribbon_20_msgmap.cpp translation unit.
// Definitions live in detail/Ribbon20MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ribbon20msgmap {} } }
using namespace openmfc::detail::ribbon20msgmap;
// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 3 classes. Each gets a base-chained AFX_MSGMAP with an empty
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
namespace openmfc { namespace detail { namespace ribbon20msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Ribbon20Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace ribbon20msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonRichEditCtrl();
} } }
namespace openmfc { namespace detail { namespace ribbon20msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonSpinButtonCtrl();
} } }
namespace openmfc { namespace detail { namespace ribbon20msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonStatusBar();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace ribbon20msgmap {
extern const AFX_MSGMAP classCMFCRibbonRichEditCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace ribbon20msgmap {
extern const AFX_MSGMAP classCMFCRibbonSpinButtonCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace ribbon20msgmap {
extern const AFX_MSGMAP classCMFCRibbonStatusBar_msgmap;
} } }

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

