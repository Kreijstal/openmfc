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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCEditBrowseCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBar() { return impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarButtonCustomizeDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarButtonsListButton() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarComboBoxEdit() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarDateTimeCtrlImpl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarDropSource() { return impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarDropTarget() { return impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarEditCtrl() { return impl__GetThisMessageMap_CMFCEditBrowseCtrl__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCToolBar_msgmap = { gbm_CMFCToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarButtonCustomizeDialog_msgmap = { gbm_CMFCToolBarButtonCustomizeDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarButtonsListButton_msgmap = { gbm_CMFCToolBarButtonsListButton, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarComboBoxEdit_msgmap = { gbm_CMFCToolBarComboBoxEdit, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarDateTimeCtrlImpl_msgmap = { gbm_CMFCToolBarDateTimeCtrlImpl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarDropSource_msgmap = { gbm_CMFCToolBarDropSource, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarDropTarget_msgmap = { gbm_CMFCToolBarDropTarget, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarEditCtrl_msgmap = { gbm_CMFCToolBarEditCtrl, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBar,
       impl__GetMessageMap_CMFCToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarButtonCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarButtonCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarButtonCustomizeDialog,
       impl__GetMessageMap_CMFCToolBarButtonCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarButtonCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarButtonsListButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarButtonsListButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarButtonsListButton,
       impl__GetMessageMap_CMFCToolBarButtonsListButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarButtonsListButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarComboBoxEdit@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarComboBoxEdit@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarComboBoxEdit,
       impl__GetMessageMap_CMFCToolBarComboBoxEdit__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarComboBoxEdit__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDateTimeCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDateTimeCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDateTimeCtrlImpl,
       impl__GetMessageMap_CMFCToolBarDateTimeCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDateTimeCtrlImpl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDropSource@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDropSource@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDropSource,
       impl__GetMessageMap_CMFCToolBarDropSource__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDropSource__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDropTarget@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDropTarget@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDropTarget,
       impl__GetMessageMap_CMFCToolBarDropTarget__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDropTarget__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarEditCtrl,
       impl__GetMessageMap_CMFCToolBarEditCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarEditCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
