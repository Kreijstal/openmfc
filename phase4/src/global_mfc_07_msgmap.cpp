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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCShadowWnd() { return impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCShellListCtrl() { return impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCShellTreeCtrl() { return impl__GetThisMessageMap_CTreeCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCSpinButtonCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCStandardColorsPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCStatusBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCTabCtrl() { return impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCTasksPane() { return impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCShadowWnd_msgmap = { gbm_CMFCShadowWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCShellListCtrl_msgmap = { gbm_CMFCShellListCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCShellTreeCtrl_msgmap = { gbm_CMFCShellTreeCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCSpinButtonCtrl_msgmap = { gbm_CMFCSpinButtonCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCStandardColorsPropertyPage_msgmap = { gbm_CMFCStandardColorsPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCStatusBar_msgmap = { gbm_CMFCStatusBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCTabCtrl_msgmap = { gbm_CMFCTabCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCTasksPane_msgmap = { gbm_CMFCTasksPane, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCShadowWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShadowWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShadowWnd,
       impl__GetMessageMap_CMFCShadowWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShadowWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCShellListCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShellListCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShellListCtrl,
       impl__GetMessageMap_CMFCShellListCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShellListCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCShellTreeCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShellTreeCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShellTreeCtrl,
       impl__GetMessageMap_CMFCShellTreeCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShellTreeCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCSpinButtonCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCSpinButtonCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCSpinButtonCtrl,
       impl__GetMessageMap_CMFCSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCStandardColorsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCStandardColorsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCStandardColorsPropertyPage,
       impl__GetMessageMap_CMFCStandardColorsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCStandardColorsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCStatusBar,
       impl__GetMessageMap_CMFCStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCStatusBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTabCtrl,
       impl__GetMessageMap_CMFCTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTabCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCTasksPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPane,
       impl__GetMessageMap_CMFCTasksPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
