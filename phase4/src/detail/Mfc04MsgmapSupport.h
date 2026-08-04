#pragma once
// Shared internals of the former global_mfc_04_msgmap.cpp translation unit.
// Definitions live in detail/Mfc04MsgmapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfc04msgmap {} } }
using namespace openmfc::detail::mfc04msgmap;
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
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Mfc04Msgmap[];
} } }

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCDropDownListBox();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCDropDownToolBar();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCEditBrowseCtrl();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCFontComboBox();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCHeaderCtrl();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCImageEditorDialog();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCImagePaintArea();
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
const AFX_MSGMAP* AFXAPI gbm_CMFCKeyMapDialog();
} } }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCDropDownListBox_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCDropDownToolBar_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCEditBrowseCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCFontComboBox_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCHeaderCtrl_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCImageEditorDialog_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCImagePaintArea_msgmap;
} } }
namespace openmfc { namespace detail { namespace mfc04msgmap {
extern const AFX_MSGMAP classCMFCKeyMapDialog_msgmap;
} } }

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

