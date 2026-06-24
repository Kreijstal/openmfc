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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonCommandsListBox() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonCustomizePropertyPage() { return impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonKeyTip() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonKeyboardCustomizeDialog() { return impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonMiniToolBar();  // defined after maps (in-batch base CMFCRibbonPanelMenu)
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonPanelMenu() { return impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonPanelMenuBar() { return impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCRibbonBar_msgmap = { gbm_CMFCRibbonBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonCommandsListBox_msgmap = { gbm_CMFCRibbonCommandsListBox, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonCustomizePropertyPage_msgmap = { gbm_CMFCRibbonCustomizePropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonKeyTip_msgmap = { gbm_CMFCRibbonKeyTip, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonKeyboardCustomizeDialog_msgmap = { gbm_CMFCRibbonKeyboardCustomizeDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonMiniToolBar_msgmap = { gbm_CMFCRibbonMiniToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonPanelMenu_msgmap = { gbm_CMFCRibbonPanelMenu, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonPanelMenuBar_msgmap = { gbm_CMFCRibbonPanelMenuBar, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonMiniToolBar() { return &classCMFCRibbonPanelMenu_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCRibbonBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonBar,
       impl__GetMessageMap_CMFCRibbonBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonCommandsListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonCommandsListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonCommandsListBox,
       impl__GetMessageMap_CMFCRibbonCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonCommandsListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonCustomizePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonCustomizePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonCustomizePropertyPage,
       impl__GetMessageMap_CMFCRibbonCustomizePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonCustomizePropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonKeyTip@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonKeyTip@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonKeyTip,
       impl__GetMessageMap_CMFCRibbonKeyTip__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonKeyTip__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonKeyboardCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonKeyboardCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonKeyboardCustomizeDialog,
       impl__GetMessageMap_CMFCRibbonKeyboardCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonKeyboardCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonMiniToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonMiniToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonMiniToolBar,
       impl__GetMessageMap_CMFCRibbonMiniToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonMiniToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonPanelMenu@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonPanelMenu@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonPanelMenu,
       impl__GetMessageMap_CMFCRibbonPanelMenu__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonPanelMenu__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonPanelMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonPanelMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonPanelMenuBar,
       impl__GetMessageMap_CMFCRibbonPanelMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonPanelMenuBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
