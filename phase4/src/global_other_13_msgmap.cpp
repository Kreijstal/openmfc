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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CSplitButton() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSplitterWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSplitterWndEx();  // defined after maps (in-batch base CSplitterWnd)
const AFX_MSGMAP* AFXAPI gbm_CStatusBar() { return impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CTabView() { return impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CToolBar() { return impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CToolBarCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CVSListBox() { return impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCSplitButton_msgmap = { gbm_CSplitButton, g_emptyMsgEntries };
const AFX_MSGMAP classCSplitterWnd_msgmap = { gbm_CSplitterWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCSplitterWndEx_msgmap = { gbm_CSplitterWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCStatusBar_msgmap = { gbm_CStatusBar, g_emptyMsgEntries };
const AFX_MSGMAP classCTabView_msgmap = { gbm_CTabView, g_emptyMsgEntries };
const AFX_MSGMAP classCToolBar_msgmap = { gbm_CToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCToolBarCtrl_msgmap = { gbm_CToolBarCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCVSListBox_msgmap = { gbm_CVSListBox, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CSplitterWndEx() { return &classCSplitterWnd_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CSplitButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitButton,
       impl__GetMessageMap_CSplitButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSplitterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitterWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitterWnd,
       impl__GetMessageMap_CSplitterWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitterWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSplitterWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitterWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitterWndEx,
       impl__GetMessageMap_CSplitterWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitterWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CStatusBar,
       impl__GetMessageMap_CStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CStatusBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CTabView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CTabView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CTabView,
       impl__GetMessageMap_CTabView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CTabView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CToolBar,
       impl__GetMessageMap_CToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CToolBarCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CToolBarCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CToolBarCtrl,
       impl__GetMessageMap_CToolBarCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CToolBarCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CVSListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CVSListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CVSListBox,
       impl__GetMessageMap_CVSListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CVSListBox__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
