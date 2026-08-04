// RuntimeClasses — OpenMFC implementation.
// Sources: global_mfc_toolbars_rtti.cpp, global_toolbar_buttons_rtti.cpp, global_toolbar_customize_rtti.cpp

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 11 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define MFC_TOOLBARS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
MFC_TOOLBARS_DESC(CMFCBaseToolBar, 1016, 0xFFFF, &CPane::classCPane);
MFC_TOOLBARS_DESC(CMFCToolBar, 4944, 0x80000001, &classCMFCBaseToolBar);
MFC_TOOLBARS_DESC(CMFCPopupMenuBar, 5152, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCColorBar, 5384, 0x00000001, &classCMFCPopupMenuBar);
MFC_TOOLBARS_DESC(CMFCDropDownToolBar, 4944, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCMenuBar, 5056, 0x80000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCOutlookBarPane, 11112, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCOutlookBarToolBar, 5008, 0xFFFF, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCPrintPreviewToolBar, 4944, 0xFFFF, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCRibbonPanelMenuBar, 5520, 0xFFFF, &classCMFCPopupMenuBar);
MFC_TOOLBARS_DESC(CMFCTasksPaneToolBar, 4960, 0x00000001, &classCMFCToolBar);
#undef MFC_TOOLBARS_DESC

#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
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

#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCTasksPaneToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCTasksPaneToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCTasksPaneToolBar,
           impl__GetThisClass_CMFCTasksPaneToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCTasksPaneToolBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
#undef MFC_TOOLBARS_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CTasksPaneHistoryButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTasksPaneHistoryButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CTasksPaneHistoryButton,
           impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTasksPaneHistoryButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CTasksPaneMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTasksPaneMenuButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CTasksPaneMenuButton,
           impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTasksPaneMenuButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CTasksPaneNavigateButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CTasksPaneNavigateButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CTasksPaneNavigateButton,
           impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CTasksPaneNavigateButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
#undef TOOLBAR_BUTTONS_GETTERS
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
#undef TOOLBAR_CUSTOMIZE_GETTERS
