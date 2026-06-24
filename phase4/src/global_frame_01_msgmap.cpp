// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 7 classes. Each gets a base-chained AFX_MSGMAP with an empty
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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIChildWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CControlFrameWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CFrameWndEx() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMDIChildWndEx() { return impl__GetThisMessageMap_CMDIChildWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMDIClientAreaWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMDIFrameWndEx() { return impl__GetThisMessageMap_CMDIFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMDITabProxyWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMiniFrameWnd() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCControlFrameWnd_msgmap = { gbm_CControlFrameWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCFrameWndEx_msgmap = { gbm_CFrameWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCMDIChildWndEx_msgmap = { gbm_CMDIChildWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCMDIClientAreaWnd_msgmap = { gbm_CMDIClientAreaWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMDIFrameWndEx_msgmap = { gbm_CMDIFrameWndEx, g_emptyMsgEntries };
const AFX_MSGMAP classCMDITabProxyWnd_msgmap = { gbm_CMDITabProxyWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMiniFrameWnd_msgmap = { gbm_CMiniFrameWnd, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CControlFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CControlFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CControlFrameWnd,
       impl__GetMessageMap_CControlFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CControlFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CFrameWndEx,
       impl__GetMessageMap_CFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIChildWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIChildWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIChildWndEx,
       impl__GetMessageMap_CMDIChildWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIChildWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIClientAreaWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIClientAreaWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIClientAreaWnd,
       impl__GetMessageMap_CMDIClientAreaWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIClientAreaWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIFrameWndEx,
       impl__GetMessageMap_CMDIFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDITabProxyWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDITabProxyWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDITabProxyWnd,
       impl__GetMessageMap_CMDITabProxyWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDITabProxyWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMiniFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMiniFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMiniFrameWnd,
       impl__GetMessageMap_CMiniFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
