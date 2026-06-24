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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CHtmlView() { return impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COutlookOptionsDlg() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPreviewView() { return impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPreviewViewEx();  // defined after maps (in-batch base CPreviewView)
const AFX_MSGMAP* AFXAPI gbm_CPrintDialogEx() { return impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CReBar() { return impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CRichEditView() { return impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CScreenWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCHtmlView_msgmap = { gbm_CHtmlView, g_emptyMsgEntries };
const AFX_MSGMAP classCOutlookOptionsDlg_msgmap = { gbm_COutlookOptionsDlg, g_emptyMsgEntries };
const AFX_MSGMAP classCPreviewView_msgmap = { gbm_CPreviewView, g_emptyMsgEntries };
const AFX_MSGMAP classCPreviewViewEx_msgmap = { gbm_CPreviewViewEx, g_emptyMsgEntries };
const AFX_MSGMAP classCPrintDialogEx_msgmap = { gbm_CPrintDialogEx, g_emptyMsgEntries };
const AFX_MSGMAP classCReBar_msgmap = { gbm_CReBar, g_emptyMsgEntries };
const AFX_MSGMAP classCRichEditView_msgmap = { gbm_CRichEditView, g_emptyMsgEntries };
const AFX_MSGMAP classCScreenWnd_msgmap = { gbm_CScreenWnd, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_CPreviewViewEx() { return &classCPreviewView_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CHtmlView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CHtmlView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CHtmlView,
       impl__GetMessageMap_CHtmlView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CHtmlView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COutlookOptionsDlg@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COutlookOptionsDlg@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COutlookOptionsDlg,
       impl__GetMessageMap_COutlookOptionsDlg__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COutlookOptionsDlg__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPreviewView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPreviewView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPreviewView,
       impl__GetMessageMap_CPreviewView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPreviewView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPreviewViewEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPreviewViewEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPreviewViewEx,
       impl__GetMessageMap_CPreviewViewEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPreviewViewEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPrintDialogEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPrintDialogEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPrintDialogEx,
       impl__GetMessageMap_CPrintDialogEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPrintDialogEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CReBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CReBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CReBar,
       impl__GetMessageMap_CReBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CReBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CRichEditView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CRichEditView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CRichEditView,
       impl__GetMessageMap_CRichEditView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CRichEditView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CScreenWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CScreenWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CScreenWnd,
       impl__GetMessageMap_CScreenWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CScreenWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
