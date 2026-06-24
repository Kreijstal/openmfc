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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CListCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCLinkCtrl() { return impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCListCtrl() { return impl__GetThisMessageMap_CListCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCMaskedEdit() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCMenuBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCMenuButton() { return impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCMousePropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCOutlookBar() { return impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCOutlookBarPane() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCLinkCtrl_msgmap = { gbm_CMFCLinkCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCListCtrl_msgmap = { gbm_CMFCListCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCMaskedEdit_msgmap = { gbm_CMFCMaskedEdit, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCMenuBar_msgmap = { gbm_CMFCMenuBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCMenuButton_msgmap = { gbm_CMFCMenuButton, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCMousePropertyPage_msgmap = { gbm_CMFCMousePropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCOutlookBar_msgmap = { gbm_CMFCOutlookBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCOutlookBarPane_msgmap = { gbm_CMFCOutlookBarPane, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCLinkCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCLinkCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCLinkCtrl,
       impl__GetMessageMap_CMFCLinkCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCLinkCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCListCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCListCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCListCtrl,
       impl__GetMessageMap_CMFCListCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMaskedEdit@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMaskedEdit@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMaskedEdit,
       impl__GetMessageMap_CMFCMaskedEdit__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMaskedEdit__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMenuBar,
       impl__GetMessageMap_CMFCMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMenuBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMenuButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMenuButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMenuButton,
       impl__GetMessageMap_CMFCMenuButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMenuButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMousePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMousePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMousePropertyPage,
       impl__GetMessageMap_CMFCMousePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMousePropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBar,
       impl__GetMessageMap_CMFCOutlookBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarPane,
       impl__GetMessageMap_CMFCOutlookBarPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
