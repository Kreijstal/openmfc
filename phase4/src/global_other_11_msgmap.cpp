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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CCommonDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CControlBar() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CCtrlView() { return impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDHtmlDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDialogBar();  // defined after maps (in-batch base CControlBar)
const AFX_MSGMAP* AFXAPI gbm_CDocObjectServer();  // defined after maps (in-batch base CCmdTarget)
const AFX_MSGMAP* AFXAPI gbm_CHtmlEditView() { return impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCCmdTarget_msgmap = { nullptr, g_emptyMsgEntries };
const AFX_MSGMAP classCCommonDialog_msgmap = { gbm_CCommonDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCControlBar_msgmap = { gbm_CControlBar, g_emptyMsgEntries };
const AFX_MSGMAP classCCtrlView_msgmap = { gbm_CCtrlView, g_emptyMsgEntries };
const AFX_MSGMAP classCDHtmlDialog_msgmap = { gbm_CDHtmlDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCDialogBar_msgmap = { gbm_CDialogBar, g_emptyMsgEntries };
const AFX_MSGMAP classCDocObjectServer_msgmap = { gbm_CDocObjectServer, g_emptyMsgEntries };
const AFX_MSGMAP classCHtmlEditView_msgmap = { gbm_CHtmlEditView, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CDialogBar() { return &classCControlBar_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDocObjectServer() { return &classCCmdTarget_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CCmdTarget@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCmdTarget@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCmdTarget,
       impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CCommonDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCommonDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCommonDialog,
       impl__GetMessageMap_CCommonDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CControlBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CControlBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CControlBar,
       impl__GetMessageMap_CControlBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CCtrlView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCtrlView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCtrlView,
       impl__GetMessageMap_CCtrlView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDHtmlDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDHtmlDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDHtmlDialog,
       impl__GetMessageMap_CDHtmlDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDHtmlDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDialogBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDialogBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDialogBar,
       impl__GetMessageMap_CDialogBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDialogBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDocObjectServer@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDocObjectServer@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDocObjectServer,
       impl__GetMessageMap_CDocObjectServer__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDocObjectServer__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CHtmlEditView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CHtmlEditView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CHtmlEditView,
       impl__GetMessageMap_CHtmlEditView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CHtmlEditView__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
