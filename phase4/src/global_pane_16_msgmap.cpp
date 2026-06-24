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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMiniDockFrameWnd() { return impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd();  // defined after maps (in-batch base CPaneFrameWnd)
const AFX_MSGMAP* AFXAPI gbm_CPane() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDialog() { return impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDivider() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneFrameWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneTrackingWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingGroupGuidesWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMiniDockFrameWnd_msgmap = { gbm_CMiniDockFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMultiPaneFrameWnd_msgmap = { gbm_CMultiPaneFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCPane_msgmap = { gbm_CPane, g_emptyMsgEntries };
const AFX_MSGMAP classCPaneDialog_msgmap = { gbm_CPaneDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCPaneDivider_msgmap = { gbm_CPaneDivider, g_emptyMsgEntries };
const AFX_MSGMAP classCPaneFrameWnd_msgmap = { gbm_CPaneFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCPaneTrackingWnd_msgmap = { gbm_CPaneTrackingWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCSmartDockingGroupGuidesWnd_msgmap = { gbm_CSmartDockingGroupGuidesWnd, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd() { return &classCPaneFrameWnd_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMiniDockFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMiniDockFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMiniDockFrameWnd,
       impl__GetMessageMap_CMiniDockFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMiniDockFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMultiPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMultiPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMultiPaneFrameWnd,
       impl__GetMessageMap_CMultiPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMultiPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPane,
       impl__GetMessageMap_CPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneDialog,
       impl__GetMessageMap_CPaneDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneDivider@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneDivider@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneDivider,
       impl__GetMessageMap_CPaneDivider__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneDivider__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneFrameWnd,
       impl__GetMessageMap_CPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneTrackingWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneTrackingWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneTrackingWnd,
       impl__GetMessageMap_CPaneTrackingWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneTrackingWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSmartDockingGroupGuidesWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingGroupGuidesWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingGroupGuidesWnd,
       impl__GetMessageMap_CSmartDockingGroupGuidesWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingGroupGuidesWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
