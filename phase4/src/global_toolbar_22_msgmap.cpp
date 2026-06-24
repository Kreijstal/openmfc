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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCheckListBox__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarNameDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsCommandsListBox() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsCommandsPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsCustomizeDialog() { return impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsKeyboardPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsListCheckBox() { return impl__GetThisMessageMap_CCheckListBox__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsListPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolBarsMenuPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCToolBarNameDialog_msgmap = { gbm_CMFCToolBarNameDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsCommandsListBox_msgmap = { gbm_CMFCToolBarsCommandsListBox, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsCommandsPropertyPage_msgmap = { gbm_CMFCToolBarsCommandsPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsCustomizeDialog_msgmap = { gbm_CMFCToolBarsCustomizeDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsKeyboardPropertyPage_msgmap = { gbm_CMFCToolBarsKeyboardPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsListCheckBox_msgmap = { gbm_CMFCToolBarsListCheckBox, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsListPropertyPage_msgmap = { gbm_CMFCToolBarsListPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolBarsMenuPropertyPage_msgmap = { gbm_CMFCToolBarsMenuPropertyPage, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCToolBarNameDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarNameDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarNameDialog,
       impl__GetMessageMap_CMFCToolBarNameDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarNameDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsCommandsListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCommandsListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCommandsListBox,
       impl__GetMessageMap_CMFCToolBarsCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCommandsListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsCommandsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCommandsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCommandsPropertyPage,
       impl__GetMessageMap_CMFCToolBarsCommandsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCommandsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsCustomizeDialog,
       impl__GetMessageMap_CMFCToolBarsCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsKeyboardPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsKeyboardPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsKeyboardPropertyPage,
       impl__GetMessageMap_CMFCToolBarsKeyboardPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsKeyboardPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsListCheckBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsListCheckBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsListCheckBox,
       impl__GetMessageMap_CMFCToolBarsListCheckBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsListCheckBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsListPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsListPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsListPropertyPage,
       impl__GetMessageMap_CMFCToolBarsListPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsListPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarsMenuPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarsMenuPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarsMenuPropertyPage,
       impl__GetMessageMap_CMFCToolBarsMenuPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarsMenuPropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
