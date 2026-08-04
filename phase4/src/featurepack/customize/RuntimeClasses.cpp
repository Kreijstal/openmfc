// RuntimeClasses — OpenMFC implementation.
// Sources: global_mfc_misc_ctrls_rtti.cpp, global_oleresidual_rtti.cpp, global_toolbar_buttons_rtti.cpp, global_toolbar_customize_rtti.cpp

#include "detail/COleControlModuleSupport.h"

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 7 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define MFC_MISC_CTRLS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
MFC_MISC_CTRLS_DESC(CMFCMousePropertyPage, 1072, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_MISC_CTRLS_DESC(CMFCPreviewCtrlImpl, 264, 0xFFFF, &CWnd::classCWnd);
MFC_MISC_CTRLS_DESC(CMFCReBar, 1040, 0xFFFF, &CPane::classCPane);
MFC_MISC_CTRLS_DESC(CMFCShellTreeCtrl, 256, 0xFFFF, &CTreeCtrl::classCTreeCtrl);
MFC_MISC_CTRLS_DESC(CMFCStandardColorsPropertyPage, 1048, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_MISC_CTRLS_DESC(CMFCStatusBar, 1056, 0xFFFF, &CPane::classCPane);
MFC_MISC_CTRLS_DESC(CMFCVisualManagerVS2008, 824, 0xFFFF, &CMFCVisualManagerVS2005::classCMFCVisualManagerVS2005);
#undef MFC_MISC_CTRLS_DESC

#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 23 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define TOOLBAR_BUTTONS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
TOOLBAR_BUTTONS_DESC(CMFCToolBarButton, 136, 0x80000001, &CObject::classCObject);
TOOLBAR_BUTTONS_DESC(CMFCToolBarComboBoxButton, 336, 0x00000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CHelpComboBoxButton, 344, 0x80000001, &classCMFCToolBarComboBoxButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarMenuButton, 296, 0x80000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCColorMenuButton, 416, 0x80000001, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CMFCCustomizeButton, 408, 0x80000001, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CMFCCustomizeMenuButton, 328, 0xFFFF, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CMFCDropDownToolbarButton, 176, 0x80000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCOutlookBarPaneButton, 160, 0x00000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCRibbonGalleryMenuButton, 1568, 0xFFFF, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CMFCShowAllButton, 296, 0xFFFF, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarColorButton, 176, 0x00000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarDateTimeCtrl, 168, 0x00000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarEditBoxButton, 176, 0x00000001, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarFontComboBox, 352, 0x00000001, &classCMFCToolBarComboBoxButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarFontSizeComboBox, 344, 0x00000001, &classCMFCToolBarComboBoxButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarMenuButtonsButton, 144, 0xFFFF, &classCMFCToolBarButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarSpinEditBoxButton, 440, 0x00000001, &classCMFCToolBarEditBoxButton);
TOOLBAR_BUTTONS_DESC(CMFCToolBarSystemMenuButton, 312, 0x80000001, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(COutlookCustomizeButton, 408, 0xFFFF, &classCMFCCustomizeButton);
TOOLBAR_BUTTONS_DESC(CTasksPaneHistoryButton, 360, 0x00000001, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CTasksPaneMenuButton, 304, 0x00000001, &classCMFCToolBarMenuButton);
TOOLBAR_BUTTONS_DESC(CTasksPaneNavigateButton, 136, 0x00000001, &classCMFCToolBarButton);
#undef TOOLBAR_BUTTONS_DESC

#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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

#define TOOLBAR_CUSTOMIZE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCMousePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMousePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCMousePropertyPage,
           impl__GetThisClass_CMFCMousePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMousePropertyPage__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_MISC_CTRLS_GETTERS
#define OR_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMouseManager@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMouseManager@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CMouseManager,
           impl__GetThisClass_CMouseManager__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CUserTool@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CUserTool@@UEBAPEAUCRuntimeClass@@XZ
OR_GETTERS(CUserTool,
           impl__GetThisClass_CUserTool__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CUserTool__UEBAPEAUCRuntimeClass__XZ)

#undef OR_GETTERS

//=============================================================================
// COleControlModule lifecycle methods
// Both delegate to CWinApp (non-virtual qualified call to avoid vtable recursion).
//=============================================================================

BOOL COleControlModule::InitInstance() { return CWinApp::InitInstance(); }
#undef OR_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCCustomizeButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCCustomizeButton,
           impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCCustomizeButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
#define TOOLBAR_CUSTOMIZE_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
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
#undef TOOLBAR_CUSTOMIZE_GETTERS
