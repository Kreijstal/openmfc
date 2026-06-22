// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of docking
// pane wrappers (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). Each chains to a real base
// descriptor the DLL already defines — CObject::classCObject (afx.h),
// CWnd::classCWnd (wincore.cpp), CBasePane::classCBasePane and
// CDockablePane::classCDockablePane (mfccore.cpp) — so IsKindOf/IsDerivedFrom
// walk the correct RUNTIME_CLASS graph across modules. m_nObjectSize is the real
// MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout. GetRuntimeClass
// returns the static descriptor directly (never pThis->GetRuntimeClass(), which
// would recurse through this very export in a drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"   // CBasePane/CPane/CDockablePane + CWnd/CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
//
// m_wSchema matches the real mfc140u.dll descriptor (read by calling the exported
// getters under Wine): DECLARE_DYNAMIC/DECLARE_DYNCREATE use 0xFFFF, DECLARE_SERIAL
// carries a real version number (0x80000000 = VERSIONABLE_SCHEMA flag). m_pfnCreateObject
// stays null even for DYNCREATE/SERIAL classes — OpenMFC has no class body for these,
// so it cannot manufacture instances, and null honestly signals "not constructible".
#define PANE_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }

PANE_DESC(CPaneContainer,        248,  0xFFFF,     &CObject::classCObject);          // DECLARE_DYNAMIC
PANE_DESC(CPaneContainerManager, 160,  0xFFFF,     &CObject::classCObject);          // DECLARE_DYNAMIC
PANE_DESC(CPaneDivider,          536,  0xFFFF,     &CBasePane::classCBasePane);       // DECLARE_DYNCREATE
PANE_DESC(CPaneDialog,           1264, 0x80000001, &CDockablePane::classCDockablePane); // DECLARE_SERIAL
PANE_DESC(CPaneFrameWnd,         600,  0x80000002, &CWnd::classCWnd);                 // DECLARE_SERIAL
#undef PANE_DESC
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define PANE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CPaneContainer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPaneContainer@@UEBAPEAUCRuntimeClass@@XZ
PANE_GETTERS(CPaneContainer,
             impl__GetThisClass_CPaneContainer__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CPaneContainer__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CPaneContainerManager@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPaneContainerManager@@UEBAPEAUCRuntimeClass@@XZ
PANE_GETTERS(CPaneContainerManager,
             impl__GetThisClass_CPaneContainerManager__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CPaneContainerManager__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CPaneDivider@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPaneDivider@@UEBAPEAUCRuntimeClass@@XZ
PANE_GETTERS(CPaneDivider,
             impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CPaneDivider__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CPaneDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPaneDialog@@UEBAPEAUCRuntimeClass@@XZ
PANE_GETTERS(CPaneDialog,
             impl__GetThisClass_CPaneDialog__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CPaneDialog__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CPaneFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CPaneFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
PANE_GETTERS(CPaneFrameWnd,
             impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ)

#undef PANE_GETTERS
