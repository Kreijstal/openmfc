// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of docking
// state / tabbed-pane wrappers (12 exports, 6 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). Each chains to a descriptor that is
// consistent with what the base class's *exported* GetThisClass returns:
// CObject::classCObject (afx.h); CDockablePane::classCDockablePane and
// CMiniFrameWnd::classCMiniFrameWnd, whose exported getters in mfccore.cpp return
// those same members; and the in-file CBaseTabbedPane descriptor (CTabbedPane's
// base). So IsKindOf/IsDerivedFrom walk an unbroken graph in a drop-in client.
// m_nObjectSize is the real MSVC sizeof harvested with cl.exe
// /d1reportSingleClassLayout. GetRuntimeClass returns the static descriptor
// directly (never pThis->GetRuntimeClass(), which would recurse through this very
// export in a drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"   // CDockablePane/CMiniFrameWnd/CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define DT_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// CBaseTabbedPane before CTabbedPane so the latter can take its address.
DT_DESC(CDockState,           96,   &CObject::classCObject);
DT_DESC(CDockingPanesRow,     112,  &CObject::classCObject);
DT_DESC(CDockablePaneAdapter, 1280, &CDockablePane::classCDockablePane);
DT_DESC(CBaseTabbedPane,      1304, &CDockablePane::classCDockablePane);
DT_DESC(CTabbedPane,          1304, &classCBaseTabbedPane);
DT_DESC(CMiniDockFrameWnd,    896,  &CMiniFrameWnd::classCMiniFrameWnd);
#undef DT_DESC
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define DT_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CDockState@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockState@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CDockState,
           impl__GetThisClass_CDockState__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockState__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDockingPanesRow@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockingPanesRow@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CDockingPanesRow,
           impl__GetThisClass_CDockingPanesRow__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockingPanesRow__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDockablePaneAdapter@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockablePaneAdapter@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CDockablePaneAdapter,
           impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockablePaneAdapter__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CBaseTabbedPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CBaseTabbedPane@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CBaseTabbedPane,
           impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CBaseTabbedPane__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CTabbedPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTabbedPane@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CTabbedPane,
           impl__GetThisClass_CTabbedPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTabbedPane__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CMiniDockFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMiniDockFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CMiniDockFrameWnd,
           impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMiniDockFrameWnd__UEBAPEAUCRuntimeClass__XZ)

#undef DT_GETTERS
