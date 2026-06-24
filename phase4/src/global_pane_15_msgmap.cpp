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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CAutoHideDockSite();  // defined after maps (in-batch base CDockSite)
const AFX_MSGMAP* AFXAPI gbm_CBasePane() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CBaseTabbedPane();  // defined after maps (in-batch base CDockablePane)
const AFX_MSGMAP* AFXAPI gbm_CDockBar() { return impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDockSite();  // defined after maps (in-batch base CBasePane)
const AFX_MSGMAP* AFXAPI gbm_CDockablePane() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDockablePaneAdapter();  // defined after maps (in-batch base CDockablePane)
const AFX_MSGMAP* AFXAPI gbm_CDummyDockablePane();  // defined after maps (in-batch base CDockablePane)

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCAutoHideDockSite_msgmap = { gbm_CAutoHideDockSite, g_emptyMsgEntries };
const AFX_MSGMAP classCBasePane_msgmap = { gbm_CBasePane, g_emptyMsgEntries };
const AFX_MSGMAP classCBaseTabbedPane_msgmap = { gbm_CBaseTabbedPane, g_emptyMsgEntries };
const AFX_MSGMAP classCDockBar_msgmap = { gbm_CDockBar, g_emptyMsgEntries };
const AFX_MSGMAP classCDockSite_msgmap = { gbm_CDockSite, g_emptyMsgEntries };
const AFX_MSGMAP classCDockablePane_msgmap = { gbm_CDockablePane, g_emptyMsgEntries };
const AFX_MSGMAP classCDockablePaneAdapter_msgmap = { gbm_CDockablePaneAdapter, g_emptyMsgEntries };
const AFX_MSGMAP classCDummyDockablePane_msgmap = { gbm_CDummyDockablePane, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CAutoHideDockSite() { return &classCDockSite_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CBaseTabbedPane() { return &classCDockablePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDockSite() { return &classCBasePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDockablePaneAdapter() { return &classCDockablePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDummyDockablePane() { return &classCDockablePane_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CAutoHideDockSite@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CAutoHideDockSite@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CAutoHideDockSite,
       impl__GetMessageMap_CAutoHideDockSite__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CAutoHideDockSite__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CBasePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CBasePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CBasePane,
       impl__GetMessageMap_CBasePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CBaseTabbedPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CBaseTabbedPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CBaseTabbedPane,
       impl__GetMessageMap_CBaseTabbedPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockBar,
       impl__GetMessageMap_CDockBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockSite@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockSite@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockSite,
       impl__GetMessageMap_CDockSite__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockSite__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockablePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockablePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockablePane,
       impl__GetMessageMap_CDockablePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockablePaneAdapter@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockablePaneAdapter@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockablePaneAdapter,
       impl__GetMessageMap_CDockablePaneAdapter__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDummyDockablePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDummyDockablePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDummyDockablePane,
       impl__GetMessageMap_CDummyDockablePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDummyDockablePane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
