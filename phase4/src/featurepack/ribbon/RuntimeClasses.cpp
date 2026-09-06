// RuntimeClasses — OpenMFC implementation.
// Sources: global_mfc_toolbars_rtti.cpp, global_popupmenu_button_rtti.cpp, global_propgrid_rtti.cpp, global_ribbon_elements_rtti.cpp, global_ribbon_panels_rtti.cpp, global_toolbar_buttons_rtti.cpp

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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define MFC_TOOLBARS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define POPUPMENU_BUTTON_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
POPUPMENU_BUTTON_DESC(CMFCPopupMenu, 6600, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd);
POPUPMENU_BUTTON_DESC(CMFCColorPopupMenu, 11992, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCDropDownListBox, 6640, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCRibbonPanelMenu, 12128, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCRibbonMiniToolBar, 12144, 0xFFFF, &classCMFCRibbonPanelMenu);
POPUPMENU_BUTTON_DESC(CMFCButton, 2848, 0xFFFF, &CButton::classCButton);
POPUPMENU_BUTTON_DESC(CMFCColorButton, 3016, 0xFFFF, &classCMFCButton);
POPUPMENU_BUTTON_DESC(CMFCLinkCtrl, 2880, 0xFFFF, &classCMFCButton);
POPUPMENU_BUTTON_DESC(CMFCMenuButton, 2888, 0xFFFF, &classCMFCButton);
#undef POPUPMENU_BUTTON_DESC

#define POPUPMENU_BUTTON_GETTERS(Cls, GetThisSym, GetRtSym) \
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

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define PROPGRID_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
PROPGRID_DESC(CMFCPropertyGridProperty, 352, 0xFFFF, &CObject::classCObject);
PROPGRID_DESC(CMFCPropertyGridColorProperty, 440, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyGridFileProperty, 384, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyGridFontProperty, 552, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyPage, 384, 0xFFFF, &CPropertyPage::classCPropertyPage);
PROPGRID_DESC(CMFCRibbonCustomizePropertyPage, 7376, 0xFFFF, &classCMFCPropertyPage);
PROPGRID_DESC(CMFCPropertySheet, 29232, 0xFFFF, &CPropertySheet::classCPropertySheet);
PROPGRID_DESC(CMFCRibbonCustomizeDialog, 29240, 0xFFFF, &classCMFCPropertySheet);
PROPGRID_DESC(CMFCPropertyGridCtrl, 1720, 0xFFFF, &CWnd::classCWnd);
#undef PROPGRID_DESC

#define PROPGRID_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 32 classes.
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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define RIBBON_ELEMENTS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
RIBBON_ELEMENTS_DESC(CMFCBaseAccessibleObject, 160, 0xFFFF, &CCmdTarget::classCCmdTarget);
RIBBON_ELEMENTS_DESC(CMFCRibbonBaseElement, 384, 0xFFFF, &classCMFCBaseAccessibleObject);
RIBBON_ELEMENTS_DESC(CMFCRibbonButton, 624, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonApplicationButton, 1440, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonButtonsGroup, 1648, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonCaptionButton, 632, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonCheckBox, 624, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonGallery, 1272, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonColorButton, 1496, 0xFFFF, &classCMFCRibbonGallery);
RIBBON_ELEMENTS_DESC(CMFCRibbonColorMenuButton, 640, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonEdit, 688, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonComboBox, 816, 0xFFFF, &classCMFCRibbonEdit);
RIBBON_ELEMENTS_DESC(CMFCRibbonContextCaption, 640, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonDefaultPanelButton, 640, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonFontComboBox, 840, 0xFFFF, &classCMFCRibbonComboBox);
RIBBON_ELEMENTS_DESC(CMFCRibbonGalleryIcon, 656, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonLabel, 624, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonLaunchButton, 632, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonLinkCtrl, 632, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonMainPanelButton, 624, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonProgressBar, 408, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonQuickAccessCustomizeButton, 672, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonQuickAccessToolBar, 1744, 0xFFFF, &classCMFCRibbonButtonsGroup);
RIBBON_ELEMENTS_DESC(CMFCRibbonRecentFilesList, 1648, 0xFFFF, &classCMFCRibbonButtonsGroup);
RIBBON_ELEMENTS_DESC(CMFCRibbonSeparator, 392, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonSlider, 480, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonStatusBarCustomizeButton, 624, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonStatusBarPane, 1072, 0xFFFF, &classCMFCRibbonButton);
RIBBON_ELEMENTS_DESC(CMFCRibbonTab, 400, 0xFFFF, &classCMFCRibbonBaseElement);
RIBBON_ELEMENTS_DESC(CMFCRibbonTabsGroup, 1648, 0xFFFF, &classCMFCRibbonButtonsGroup);
RIBBON_ELEMENTS_DESC(CMFCRibbonUndoButton, 1352, 0xFFFF, &classCMFCRibbonGallery);
RIBBON_ELEMENTS_DESC(CRibbonUndoLabel, 624, 0xFFFF, &classCMFCRibbonButton);
#undef RIBBON_ELEMENTS_DESC

#define RIBBON_ELEMENTS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 4 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define RIBBON_PANELS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
RIBBON_PANELS_DESC(CMFCRibbonCustomizeCategory, 72, 0xFFFF, &CObject::classCObject);
RIBBON_PANELS_DESC(CMFCRibbonMainPanel, 1832, 0xFFFF, &CMFCRibbonPanel::classCMFCRibbonPanel);
RIBBON_PANELS_DESC(CMFCRibbonRichEditCtrl, 264, 0xFFFF, &CRichEditCtrl::classCRichEditCtrl);
RIBBON_PANELS_DESC(CMFCRibbonStatusBar, 8728, 0xFFFF, &CMFCRibbonBar::classCMFCRibbonBar);
#undef RIBBON_PANELS_DESC

#define RIBBON_PANELS_GETTERS(Cls, GetThisSym, GetRtSym) \
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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define TOOLBAR_BUTTONS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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

#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonPanelMenuBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonPanelMenuBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCRibbonPanelMenuBar,
           impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonPanelMenuBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
#define POPUPMENU_BUTTON_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonPanelMenu@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonPanelMenu@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCRibbonPanelMenu,
           impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonPanelMenu__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonMiniToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonMiniToolBar@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCRibbonMiniToolBar,
           impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonMiniToolBar__UEBAPEAUCRuntimeClass__XZ)
#undef POPUPMENU_BUTTON_GETTERS
#define PROPGRID_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonCustomizePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCRibbonCustomizePropertyPage,
           impl__GetThisClass_CMFCRibbonCustomizePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizePropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonCustomizeDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeDialog@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCRibbonCustomizeDialog,
           impl__GetThisClass_CMFCRibbonCustomizeDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizeDialog__UEBAPEAUCRuntimeClass__XZ)
#undef PROPGRID_GETTERS
#define RIBBON_ELEMENTS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonBaseElement@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonBaseElement@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonBaseElement,
           impl__GetThisClass_CMFCRibbonBaseElement__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonBaseElement__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonButton,
           impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonApplicationButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonApplicationButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonApplicationButton,
           impl__GetThisClass_CMFCRibbonApplicationButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonApplicationButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonButtonsGroup@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonButtonsGroup@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonButtonsGroup,
           impl__GetThisClass_CMFCRibbonButtonsGroup__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonButtonsGroup__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonCaptionButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCaptionButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonCaptionButton,
           impl__GetThisClass_CMFCRibbonCaptionButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCaptionButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonCheckBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCheckBox@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonCheckBox,
           impl__GetThisClass_CMFCRibbonCheckBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCheckBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonGallery@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonGallery@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonGallery,
           impl__GetThisClass_CMFCRibbonGallery__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonGallery__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonColorButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonColorButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonColorButton,
           impl__GetThisClass_CMFCRibbonColorButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonColorButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonColorMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonColorMenuButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonColorMenuButton,
           impl__GetThisClass_CMFCRibbonColorMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonColorMenuButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonEdit@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonEdit@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonEdit,
           impl__GetThisClass_CMFCRibbonEdit__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonEdit__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonComboBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonComboBox@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonComboBox,
           impl__GetThisClass_CMFCRibbonComboBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonComboBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonContextCaption@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonContextCaption@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonContextCaption,
           impl__GetThisClass_CMFCRibbonContextCaption__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonContextCaption__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonDefaultPanelButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonDefaultPanelButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonDefaultPanelButton,
           impl__GetThisClass_CMFCRibbonDefaultPanelButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonDefaultPanelButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonFontComboBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonFontComboBox@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonFontComboBox,
           impl__GetThisClass_CMFCRibbonFontComboBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonFontComboBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonGalleryIcon@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonGalleryIcon@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonGalleryIcon,
           impl__GetThisClass_CMFCRibbonGalleryIcon__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonGalleryIcon__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonLabel@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonLabel@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonLabel,
           impl__GetThisClass_CMFCRibbonLabel__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonLabel__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonLaunchButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonLaunchButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonLaunchButton,
           impl__GetThisClass_CMFCRibbonLaunchButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonLaunchButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonLinkCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonLinkCtrl,
           impl__GetThisClass_CMFCRibbonLinkCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonLinkCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonMainPanelButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonMainPanelButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonMainPanelButton,
           impl__GetThisClass_CMFCRibbonMainPanelButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonMainPanelButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonProgressBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonProgressBar@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonProgressBar,
           impl__GetThisClass_CMFCRibbonProgressBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonProgressBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonQuickAccessCustomizeButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonQuickAccessCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonQuickAccessCustomizeButton,
           impl__GetThisClass_CMFCRibbonQuickAccessCustomizeButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonQuickAccessCustomizeButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonQuickAccessToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonQuickAccessToolBar@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonQuickAccessToolBar,
           impl__GetThisClass_CMFCRibbonQuickAccessToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonQuickAccessToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonRecentFilesList@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonRecentFilesList@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonRecentFilesList,
           impl__GetThisClass_CMFCRibbonRecentFilesList__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonRecentFilesList__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonSeparator@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonSeparator@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonSeparator,
           impl__GetThisClass_CMFCRibbonSeparator__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonSeparator__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonSlider@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonSlider@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonSlider,
           impl__GetThisClass_CMFCRibbonSlider__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonSlider__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonStatusBarCustomizeButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonStatusBarCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonStatusBarCustomizeButton,
           impl__GetThisClass_CMFCRibbonStatusBarCustomizeButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonStatusBarCustomizeButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonStatusBarPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonStatusBarPane@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonStatusBarPane,
           impl__GetThisClass_CMFCRibbonStatusBarPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonStatusBarPane__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonTab@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonTab@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonTab,
           impl__GetThisClass_CMFCRibbonTab__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonTab__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonTabsGroup@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonTabsGroup@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonTabsGroup,
           impl__GetThisClass_CMFCRibbonTabsGroup__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonTabsGroup__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonUndoButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonUndoButton@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCRibbonUndoButton,
           impl__GetThisClass_CMFCRibbonUndoButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonUndoButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CRibbonUndoLabel@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CRibbonUndoLabel@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CRibbonUndoLabel,
           impl__GetThisClass_CRibbonUndoLabel__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CRibbonUndoLabel__UEBAPEAUCRuntimeClass__XZ)
#undef RIBBON_ELEMENTS_GETTERS
#undef RIBBON_ELEMENTS_GETTERS
#define RIBBON_PANELS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonCustomizeCategory@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeCategory@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonCustomizeCategory,
           impl__GetThisClass_CMFCRibbonCustomizeCategory__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizeCategory__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonMainPanel@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonMainPanel@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonMainPanel,
           impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonMainPanel__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonRichEditCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonRichEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonRichEditCtrl,
           impl__GetThisClass_CMFCRibbonRichEditCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonRichEditCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonStatusBar@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonStatusBar,
           impl__GetThisClass_CMFCRibbonStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonStatusBar__UEBAPEAUCRuntimeClass__XZ)
#undef RIBBON_PANELS_GETTERS
#undef RIBBON_PANELS_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCRibbonGalleryMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonGalleryMenuButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCRibbonGalleryMenuButton,
           impl__GetThisClass_CMFCRibbonGalleryMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonGalleryMenuButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
