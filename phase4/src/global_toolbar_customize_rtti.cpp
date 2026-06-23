// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 8 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define TOOLBAR_CUSTOMIZE_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
TOOLBAR_CUSTOMIZE_DESC(CMFCTasksPane, 6936, 0x80000001, &CDockablePane::classCDockablePane);
TOOLBAR_CUSTOMIZE_DESC(CMFCTasksPaneFrameWnd, 608, 0x80000002, &CPaneFrameWnd::classCPaneFrameWnd);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsCommandsPropertyPage, 848, 0xFFFF, &CPropertyPage::classCPropertyPage);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsCustomizeDialog, 3512, 0xFFFF, &CPropertySheet::classCPropertySheet);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsKeyboardPropertyPage, 2552, 0xFFFF, &CPropertyPage::classCPropertyPage);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsListPropertyPage, 1816, 0xFFFF, &CPropertyPage::classCPropertyPage);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsMenuPropertyPage, 2752, 0xFFFF, &CPropertyPage::classCPropertyPage);
TOOLBAR_CUSTOMIZE_DESC(CMFCToolBarsOptionsPropertyPage, 1992, 0xFFFF, &CPropertyPage::classCPropertyPage);
#undef TOOLBAR_CUSTOMIZE_DESC
} // namespace

#define TOOLBAR_CUSTOMIZE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCTasksPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCTasksPane@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCTasksPane,
           impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCTasksPane__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCTasksPaneFrameWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCTasksPaneFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCTasksPaneFrameWnd,
           impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCTasksPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsCommandsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsCommandsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsCommandsPropertyPage,
           impl__GetThisClass_CMFCToolBarsCommandsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsCommandsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsCustomizeDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsCustomizeDialog@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsCustomizeDialog,
           impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsCustomizeDialog__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsKeyboardPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsKeyboardPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsKeyboardPropertyPage,
           impl__GetThisClass_CMFCToolBarsKeyboardPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsKeyboardPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsListPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsListPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsListPropertyPage,
           impl__GetThisClass_CMFCToolBarsListPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsListPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsMenuPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsMenuPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsMenuPropertyPage,
           impl__GetThisClass_CMFCToolBarsMenuPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsMenuPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarsOptionsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarsOptionsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_CUSTOMIZE_GETTERS(CMFCToolBarsOptionsPropertyPage,
           impl__GetThisClass_CMFCToolBarsOptionsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarsOptionsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_CUSTOMIZE_GETTERS
