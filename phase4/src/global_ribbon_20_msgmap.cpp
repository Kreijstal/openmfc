// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 3 classes. Each gets a base-chained AFX_MSGMAP with an empty
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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonRichEditCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonSpinButtonCtrl() { return impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCRibbonStatusBar() { return impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCRibbonRichEditCtrl_msgmap = { gbm_CMFCRibbonRichEditCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonSpinButtonCtrl_msgmap = { gbm_CMFCRibbonSpinButtonCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCRibbonStatusBar_msgmap = { gbm_CMFCRibbonStatusBar, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCRibbonRichEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonRichEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonRichEditCtrl,
       impl__GetMessageMap_CMFCRibbonRichEditCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonRichEditCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonSpinButtonCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonSpinButtonCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonSpinButtonCtrl,
       impl__GetMessageMap_CMFCRibbonSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonStatusBar,
       impl__GetMessageMap_CMFCRibbonStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonStatusBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
