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

namespace {
// Shared empty message-map entries (terminator only): the class exposes no
// handlers we implement; routing falls through to the base via pfnGetBaseMap.
const AFX_MSGMAP_ENTRY g_emptyMsgEntries[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
} // namespace

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCAcceleratorKeyAssignCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCAutoHideBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseTabCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseToolBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCButton() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCCaptionBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorBar() { return impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorButton();  // defined after maps (in-batch base CMFCButton)

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCAcceleratorKeyAssignCtrl_msgmap = { gbm_CMFCAcceleratorKeyAssignCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCAutoHideBar_msgmap = { gbm_CMFCAutoHideBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCBaseTabCtrl_msgmap = { gbm_CMFCBaseTabCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCBaseToolBar_msgmap = { gbm_CMFCBaseToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCButton_msgmap = { gbm_CMFCButton, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCCaptionBar_msgmap = { gbm_CMFCCaptionBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCColorBar_msgmap = { gbm_CMFCColorBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCColorButton_msgmap = { gbm_CMFCColorButton, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CMFCColorButton() { return &classCMFCButton_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCAcceleratorKeyAssignCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCAcceleratorKeyAssignCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCAcceleratorKeyAssignCtrl,
       impl__GetMessageMap_CMFCAcceleratorKeyAssignCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCAcceleratorKeyAssignCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCAutoHideBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCAutoHideBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCAutoHideBar,
       impl__GetMessageMap_CMFCAutoHideBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCAutoHideBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCBaseTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCBaseTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCBaseTabCtrl,
       impl__GetMessageMap_CMFCBaseTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCBaseToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCBaseToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCBaseToolBar,
       impl__GetMessageMap_CMFCBaseToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCButton,
       impl__GetMessageMap_CMFCButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCCaptionBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCCaptionBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCCaptionBar,
       impl__GetMessageMap_CMFCCaptionBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCCaptionBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorBar,
       impl__GetMessageMap_CMFCColorBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorButton,
       impl__GetMessageMap_CMFCColorButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorButton__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
