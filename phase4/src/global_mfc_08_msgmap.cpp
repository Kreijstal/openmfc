// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 4 classes. Each gets a base-chained AFX_MSGMAP with an empty
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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolTipCtrl__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCTasksPaneFrameWnd() { return impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCTasksPaneToolBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCToolTipCtrl() { return impl__GetThisMessageMap_CToolTipCtrl__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCWindowsManagerDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCTasksPaneFrameWnd_msgmap = { gbm_CMFCTasksPaneFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCTasksPaneToolBar_msgmap = { gbm_CMFCTasksPaneToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCToolTipCtrl_msgmap = { gbm_CMFCToolTipCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCWindowsManagerDialog_msgmap = { gbm_CMFCWindowsManagerDialog, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCTasksPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPaneFrameWnd,
       impl__GetMessageMap_CMFCTasksPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCTasksPaneToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPaneToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPaneToolBar,
       impl__GetMessageMap_CMFCTasksPaneToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPaneToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolTipCtrl,
       impl__GetMessageMap_CMFCToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolTipCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCWindowsManagerDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCWindowsManagerDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCWindowsManagerDialog,
       impl__GetMessageMap_CMFCWindowsManagerDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCWindowsManagerDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
