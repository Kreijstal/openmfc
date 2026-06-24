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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCOutlookBarPaneAdapter() { return impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCOutlookBarTabCtrl() { return impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCOutlookBarToolBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPopupMenu() { return impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPopupMenuBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPreviewCtrlImpl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPrintPreviewToolBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCReBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCOutlookBarPaneAdapter_msgmap = { gbm_CMFCOutlookBarPaneAdapter, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCOutlookBarTabCtrl_msgmap = { gbm_CMFCOutlookBarTabCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCOutlookBarToolBar_msgmap = { gbm_CMFCOutlookBarToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPopupMenu_msgmap = { gbm_CMFCPopupMenu, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPopupMenuBar_msgmap = { gbm_CMFCPopupMenuBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPreviewCtrlImpl_msgmap = { gbm_CMFCPreviewCtrlImpl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPrintPreviewToolBar_msgmap = { gbm_CMFCPrintPreviewToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCReBar_msgmap = { gbm_CMFCReBar, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCOutlookBarPaneAdapter@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPaneAdapter@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarPaneAdapter,
       impl__GetMessageMap_CMFCOutlookBarPaneAdapter__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarPaneAdapter__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarTabCtrl,
       impl__GetMessageMap_CMFCOutlookBarTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarTabCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarToolBar,
       impl__GetMessageMap_CMFCOutlookBarToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPopupMenu@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPopupMenu@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPopupMenu,
       impl__GetMessageMap_CMFCPopupMenu__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPopupMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPopupMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPopupMenuBar,
       impl__GetMessageMap_CMFCPopupMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPreviewCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPreviewCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPreviewCtrlImpl,
       impl__GetMessageMap_CMFCPreviewCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPreviewCtrlImpl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPrintPreviewToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPrintPreviewToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPrintPreviewToolBar,
       impl__GetMessageMap_CMFCPrintPreviewToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPrintPreviewToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCReBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCReBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCReBar,
       impl__GetMessageMap_CMFCReBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCReBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
