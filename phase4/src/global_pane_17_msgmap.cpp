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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingHighlighterWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingStandaloneGuideWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CTabbedPane() { return impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCSmartDockingHighlighterWnd_msgmap = { gbm_CSmartDockingHighlighterWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCSmartDockingStandaloneGuideWnd_msgmap = { gbm_CSmartDockingStandaloneGuideWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCTabbedPane_msgmap = { gbm_CTabbedPane, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CSmartDockingHighlighterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingHighlighterWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingHighlighterWnd,
       impl__GetMessageMap_CSmartDockingHighlighterWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingHighlighterWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSmartDockingStandaloneGuideWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingStandaloneGuideWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingStandaloneGuideWnd,
       impl__GetMessageMap_CSmartDockingStandaloneGuideWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingStandaloneGuideWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CTabbedPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CTabbedPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CTabbedPane,
       impl__GetMessageMap_CTabbedPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CTabbedPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
