// RuntimeClasses — OpenMFC implementation.
// Sources: global_deferred_rtti.cpp, global_docktab_rtti.cpp, global_frames_docs_preview_rtti.cpp, global_pane2_rtti.cpp, global_tabctrl_renderer_list_rtti.cpp

#include "detail/DeferredRttiSupport.h"

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

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define FRAMES_DOCS_PREVIEW_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
FRAMES_DOCS_PREVIEW_DESC(CMultiPaneFrameWnd, 768, 0x80000002, &CPaneFrameWnd::classCPaneFrameWnd);
FRAMES_DOCS_PREVIEW_DESC(COleCntrFrameWndEx, 1352, 0xFFFF, &CFrameWnd::classCFrameWnd);
FRAMES_DOCS_PREVIEW_DESC(COleDBRecordView, 360, 0xFFFF, &CFormView::classCFormView);
FRAMES_DOCS_PREVIEW_DESC(COlePropertyPage, 512, 0xFFFF, &CDialog::classCDialog);
FRAMES_DOCS_PREVIEW_DESC(CPreviewDC, 88, 0xFFFF, &CDC::classCDC);
FRAMES_DOCS_PREVIEW_DESC(CRichEditCntrItem, 256, 0x00000000, &COleClientItem::classCOleClientItem);
FRAMES_DOCS_PREVIEW_DESC(CRichEditDoc, 672, 0xFFFF, &COleServerDoc::classCOleServerDoc);
FRAMES_DOCS_PREVIEW_DESC(CSmartDockingGroupGuidesManager, 6680, 0xFFFF, &CObject::classCObject);
FRAMES_DOCS_PREVIEW_DESC(CSmartDockingStandaloneGuide, 1128, 0xFFFF, &CObject::classCObject);
#undef FRAMES_DOCS_PREVIEW_DESC

#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define PANE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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
#define TABCTRL_RENDERER_LIST_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
TABCTRL_RENDERER_LIST_DESC(CMFCBaseTabCtrl, 960, 0xFFFF, &CWnd::classCWnd);
TABCTRL_RENDERER_LIST_DESC(CMFCOutlookBarTabCtrl, 11768, 0xFFFF, &classCMFCBaseTabCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCTabCtrl, 15720, 0xFFFF, &classCMFCBaseTabCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCControlRenderer, 512, 0xFFFF, &CObject::classCObject);
TABCTRL_RENDERER_LIST_DESC(CMFCShadowRenderer, 512, 0xFFFF, &classCMFCControlRenderer);
TABCTRL_RENDERER_LIST_DESC(CMFCListCtrl, 576, 0xFFFF, &CListCtrl::classCListCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCShellListCtrl, 632, 0xFFFF, &classCMFCListCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCAutoHideBar, 1112, 0xFFFF, &CPane::classCPane);
TABCTRL_RENDERER_LIST_DESC(CMFCAutoHideButton, 64, 0xFFFF, &CObject::classCObject);
#undef TABCTRL_RENDERER_LIST_DESC

#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

#define DEF_GETTERS(Cls, GT, GRC) \
    extern "C" CRuntimeClass* MS_ABI GT() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GRC(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CAutoHideDockSite@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CAutoHideDockSite@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CAutoHideDockSite,
            impl__GetThisClass_CAutoHideDockSite__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CAutoHideDockSite__UEBAPEAUCRuntimeClass__XZ)
#undef DEF_GETTERS
#define DT_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
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
#undef DT_GETTERS
#define FRAMES_DOCS_PREVIEW_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMultiPaneFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMultiPaneFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CMultiPaneFrameWnd,
           impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMultiPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CSmartDockingGroupGuidesManager@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSmartDockingGroupGuidesManager@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CSmartDockingGroupGuidesManager,
           impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSmartDockingGroupGuidesManager__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CSmartDockingStandaloneGuide@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CSmartDockingStandaloneGuide@@UEBAPEAUCRuntimeClass@@XZ
FRAMES_DOCS_PREVIEW_GETTERS(CSmartDockingStandaloneGuide,
           impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CSmartDockingStandaloneGuide__UEBAPEAUCRuntimeClass__XZ)
#undef FRAMES_DOCS_PREVIEW_GETTERS
#undef FRAMES_DOCS_PREVIEW_GETTERS
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
#undef PANE_GETTERS
#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCAutoHideBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCAutoHideBar@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCAutoHideBar,
           impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCAutoHideBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCAutoHideButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCAutoHideButton@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCAutoHideButton,
           impl__GetThisClass_CMFCAutoHideButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCAutoHideButton__UEBAPEAUCRuntimeClass__XZ)
#undef TABCTRL_RENDERER_LIST_GETTERS
#undef TABCTRL_RENDERER_LIST_GETTERS
