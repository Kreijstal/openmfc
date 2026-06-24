// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 5 classes. Each gets a base-chained AFX_MSGMAP with an empty
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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCPropertyGridCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPropertyGridToolTipCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPropertySheet() { return impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCPropertySheetListBox() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCPropertyGridCtrl_msgmap = { gbm_CMFCPropertyGridCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPropertyGridToolTipCtrl_msgmap = { gbm_CMFCPropertyGridToolTipCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPropertyPage_msgmap = { gbm_CMFCPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPropertySheet_msgmap = { gbm_CMFCPropertySheet, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCPropertySheetListBox_msgmap = { gbm_CMFCPropertySheetListBox, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCPropertyGridCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyGridCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyGridCtrl,
       impl__GetMessageMap_CMFCPropertyGridCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyGridCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertyGridToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyGridToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyGridToolTipCtrl,
       impl__GetMessageMap_CMFCPropertyGridToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyGridToolTipCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyPage,
       impl__GetMessageMap_CMFCPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertySheet@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertySheet@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertySheet,
       impl__GetMessageMap_CMFCPropertySheet__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertySheet__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertySheetListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertySheetListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertySheetListBox,
       impl__GetMessageMap_CMFCPropertySheetListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertySheetListBox__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
