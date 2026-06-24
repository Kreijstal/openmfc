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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_COleCntrFrameWndEx() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleControl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleDBRecordView() { return impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWnd();  // defined after maps (in-batch base COleIPFrameWnd)
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWndEx();  // defined after maps (in-batch base COleDocIPFrameWnd)
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWnd() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWndEx();  // defined after maps (in-batch base COleIPFrameWnd)
const AFX_MSGMAP* AFXAPI gbm_COlePropertyPage() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCOleCntrFrameWndEx_msgmap = { gbm_COleCntrFrameWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCOleControl_msgmap = { gbm_COleControl, g_emptyMsgEntries };
const AFX_MSGMAP classCOleDBRecordView_msgmap = { gbm_COleDBRecordView, g_emptyMsgEntries };
const AFX_MSGMAP classCOleDocIPFrameWnd_msgmap = { gbm_COleDocIPFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCOleDocIPFrameWndEx_msgmap = { gbm_COleDocIPFrameWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCOleIPFrameWnd_msgmap = { gbm_COleIPFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCOleIPFrameWndEx_msgmap = { gbm_COleIPFrameWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCOlePropertyPage_msgmap = { gbm_COlePropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWnd() { return &classCOleIPFrameWnd_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWndEx() { return &classCOleDocIPFrameWnd_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWndEx() { return &classCOleIPFrameWnd_msgmap; }
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@COleCntrFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleCntrFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleCntrFrameWndEx,
       impl__GetMessageMap_COleCntrFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleCntrFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleControl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleControl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleControl,
       impl__GetMessageMap_COleControl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleControl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleDBRecordView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDBRecordView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDBRecordView,
       impl__GetMessageMap_COleDBRecordView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDBRecordView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleDocIPFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDocIPFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDocIPFrameWnd,
       impl__GetMessageMap_COleDocIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDocIPFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleDocIPFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDocIPFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDocIPFrameWndEx,
       impl__GetMessageMap_COleDocIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDocIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleIPFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleIPFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleIPFrameWnd,
       impl__GetMessageMap_COleIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleIPFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleIPFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleIPFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleIPFrameWndEx,
       impl__GetMessageMap_COleIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COlePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COlePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COlePropertyPage,
       impl__GetMessageMap_COlePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COlePropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
