// RuntimeClasses — OpenMFC implementation.
// Sources: global_controlbar_keyframe_rtti.cpp, global_docitem_edit_rtti.cpp, global_docktab_rtti.cpp, global_toolbar_vslistbox_rtti.cpp

#define OPENMFC_APPCORE_IMPL

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CONTROLBAR_KEYFRAME_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
CONTROLBAR_KEYFRAME_DESC(CControlBar, 328, 0xFFFF, &CWnd::classCWnd);
CONTROLBAR_KEYFRAME_DESC(CDockBar, 400, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(COleResizeBar, 408, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CReBar, 336, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CStatusBar, 336, 0xFFFF, &classCControlBar);
CONTROLBAR_KEYFRAME_DESC(CBaseKeyFrame, 24, 0xFFFF, &CObject::classCObject);
CONTROLBAR_KEYFRAME_DESC(CKeyFrame, 48, 0xFFFF, &classCBaseKeyFrame);
CONTROLBAR_KEYFRAME_DESC(CPreviewView, 536, 0xFFFF, &CScrollView::classCScrollView);
CONTROLBAR_KEYFRAME_DESC(CPreviewViewEx, 5536, 0xFFFF, &classCPreviewView);
#undef CONTROLBAR_KEYFRAME_DESC

#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// other_mfc shard 03/08 real exports.
//
// Implements the unambiguous RTTI accessors (GetThisClass / GetRuntimeClass)
// for classes whose CRuntimeClass is already defined via IMPLEMENT_DYNAMIC in
// the repo, plus a couple of trivial virtuals with well-defined defaults.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








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

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
//
// m_wSchema matches the real mfc140u.dll descriptor (read by calling the exported
// getters under Wine), which depends on the class's DECLARE macro: DECLARE_DYNAMIC
// and DECLARE_DYNCREATE use 0xFFFF, while DECLARE_SERIAL carries a real version
// number (0x80000000 = VERSIONABLE_SCHEMA flag, or plain 0). m_pfnCreateObject is
// left null even for DYNCREATE/SERIAL classes: OpenMFC has no class body for these
// so it cannot manufacture instances, and null honestly signals "not constructible".
#define DT_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }

// CBaseTabbedPane before CTabbedPane so the latter can take its address.
DT_DESC(CDockState,           96,   0x00000000, &CObject::classCObject);          // DECLARE_SERIAL, schema 0
DT_DESC(CDockingPanesRow,     112,  0xFFFF,     &CObject::classCObject);          // DECLARE_DYNAMIC
DT_DESC(CDockablePaneAdapter, 1280, 0x80000002, &CDockablePane::classCDockablePane); // DECLARE_SERIAL
DT_DESC(CBaseTabbedPane,      1304, 0xFFFF,     &CDockablePane::classCDockablePane); // DECLARE_DYNAMIC
DT_DESC(CTabbedPane,          1304, 0x80000002, &classCBaseTabbedPane);           // DECLARE_SERIAL
DT_DESC(CMiniDockFrameWnd,    896,  0xFFFF,     &CMiniFrameWnd::classCMiniFrameWnd); // DECLARE_DYNCREATE
#undef DT_DESC

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define DT_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








#define CONTROLBAR_KEYFRAME_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CControlBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CControlBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CControlBar,
           impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CControlBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CDockBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CDockBar,
           impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COleResizeBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleResizeBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(COleResizeBar,
           impl__GetThisClass_COleResizeBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COleResizeBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CReBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CReBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CReBar,
           impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CReBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CStatusBar@@UEBAPEAUCRuntimeClass@@XZ
CONTROLBAR_KEYFRAME_GETTERS(CStatusBar,
           impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CStatusBar__UEBAPEAUCRuntimeClass__XZ)
#undef CONTROLBAR_KEYFRAME_GETTERS
// Symbol: ?GetRuntimeClass@CDialogBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialogBar__UEBAPEAUCRuntimeClass__XZ(const CDialogBar*) {
    return CDialogBar::GetThisClass();
}
// Symbol: ?GetThisClass@CDialogBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialogBar__SAPEAUCRuntimeClass__XZ() {
    return CDialogBar::GetThisClass();
}
#define DT_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CDockState@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDockState@@UEBAPEAUCRuntimeClass@@XZ
DT_GETTERS(CDockState,
           impl__GetThisClass_CDockState__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CDockState__UEBAPEAUCRuntimeClass__XZ)

#undef DT_GETTERS
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CToolBar@@SAPEAUCRuntimeClass@@XZ
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CToolBar@@UEBAPEAUCRuntimeClass@@XZ
