// RuntimeClasses — OpenMFC implementation.
// Sources: global_deferred_rtti.cpp, global_mfc_dialogs_ctrls_rtti.cpp, global_mfc_misc_ctrls_rtti.cpp, global_mfc_toolbars_rtti.cpp, global_popupmenu_button_rtti.cpp, global_ribbon_elements_rtti.cpp, global_tabctrl_renderer_list_rtti.cpp, global_toolbar_buttons_rtti.cpp, global_toolbar_vslistbox_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DeferredRttiSupport.h"

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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define MFC_DIALOGS_CTRLS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
MFC_DIALOGS_CTRLS_DESC(CMFCCaptionBar, 1720, 0xFFFF, &CPane::classCPane);
MFC_DIALOGS_CTRLS_DESC(CMFCColorPropertySheet, 408, 0xFFFF, &CPropertySheet::classCPropertySheet);
MFC_DIALOGS_CTRLS_DESC(CMFCCustomColorsPropertyPage, 1072, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_DIALOGS_CTRLS_DESC(CMFCDesktopAlertDialog, 3776, 0xFFFF, &CDialogEx::classCDialogEx);
MFC_DIALOGS_CTRLS_DESC(CMFCDesktopAlertWnd, 6160, 0xFFFF, &CWnd::classCWnd);
MFC_DIALOGS_CTRLS_DESC(CMFCDropDownFrame, 5488, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd);
MFC_DIALOGS_CTRLS_DESC(CMFCDynamicLayout, 80, 0xFFFF, &CObject::classCObject);
MFC_DIALOGS_CTRLS_DESC(CMFCEditBrowseCtrl, 336, 0xFFFF, &CEdit::classCEdit);
MFC_DIALOGS_CTRLS_DESC(CMFCMaskedEdit, 296, 0xFFFF, &CEdit::classCEdit);
#undef MFC_DIALOGS_CTRLS_DESC

#define MFC_DIALOGS_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define MFC_MISC_CTRLS_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define TABCTRL_RENDERER_LIST_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








#define DEF_GETTERS(Cls, GT, GRC) \
    extern "C" CRuntimeClass* MS_ABI GT() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GRC(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCHeaderCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCHeaderCtrl@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCHeaderCtrl,
            impl__GetThisClass_CMFCHeaderCtrl__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCHeaderCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCToolTipCtrl,
            impl__GetThisClass_CMFCToolTipCtrl__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCToolTipCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef DEF_GETTERS
#undef DEF_GETTERS
#define MFC_DIALOGS_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCCaptionBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCCaptionBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCCaptionBar,
           impl__GetThisClass_CMFCCaptionBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCCaptionBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorPropertySheet@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorPropertySheet@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCColorPropertySheet,
           impl__GetThisClass_CMFCColorPropertySheet__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorPropertySheet__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCCustomColorsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCCustomColorsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCCustomColorsPropertyPage,
           impl__GetThisClass_CMFCCustomColorsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCCustomColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDesktopAlertDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDesktopAlertDialog@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDesktopAlertDialog,
           impl__GetThisClass_CMFCDesktopAlertDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDesktopAlertDialog__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDesktopAlertWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDesktopAlertWnd@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDesktopAlertWnd,
           impl__GetThisClass_CMFCDesktopAlertWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDesktopAlertWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownFrame@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownFrame@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDropDownFrame,
           impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownFrame__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDynamicLayout@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDynamicLayout@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDynamicLayout,
           impl__GetThisClass_CMFCDynamicLayout__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDynamicLayout__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCEditBrowseCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCEditBrowseCtrl@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCEditBrowseCtrl,
           impl__GetThisClass_CMFCEditBrowseCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCEditBrowseCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCMaskedEdit@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMaskedEdit@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCMaskedEdit,
           impl__GetThisClass_CMFCMaskedEdit__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMaskedEdit__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_DIALOGS_CTRLS_GETTERS
#undef MFC_DIALOGS_CTRLS_GETTERS
#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCPreviewCtrlImpl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPreviewCtrlImpl@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCPreviewCtrlImpl,
           impl__GetThisClass_CMFCPreviewCtrlImpl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPreviewCtrlImpl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCReBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCReBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCReBar,
           impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCReBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCShellTreeCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShellTreeCtrl@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCShellTreeCtrl,
           impl__GetThisClass_CMFCShellTreeCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShellTreeCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCStandardColorsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCStandardColorsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCStandardColorsPropertyPage,
           impl__GetThisClass_CMFCStandardColorsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCStandardColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCStatusBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCStatusBar,
           impl__GetThisClass_CMFCStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCStatusBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_MISC_CTRLS_GETTERS
#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCColorBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCColorBar,
           impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
#define POPUPMENU_BUTTON_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCColorPopupMenu@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorPopupMenu@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCColorPopupMenu,
           impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorPopupMenu__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownListBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownListBox@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCDropDownListBox,
           impl__GetThisClass_CMFCDropDownListBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownListBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCButton@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCButton,
           impl__GetThisClass_CMFCButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorButton@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCColorButton,
           impl__GetThisClass_CMFCColorButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCLinkCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCLinkCtrl,
           impl__GetThisClass_CMFCLinkCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCLinkCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef POPUPMENU_BUTTON_GETTERS
#define RIBBON_ELEMENTS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCBaseAccessibleObject@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCBaseAccessibleObject@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCBaseAccessibleObject,
           impl__GetThisClass_CMFCBaseAccessibleObject__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCBaseAccessibleObject__UEBAPEAUCRuntimeClass__XZ)
#undef RIBBON_ELEMENTS_GETTERS
#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCListCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCListCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCListCtrl,
           impl__GetThisClass_CMFCListCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCListCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCShellListCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShellListCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCShellListCtrl,
           impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShellListCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef TABCTRL_RENDERER_LIST_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCShowAllButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShowAllButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCShowAllButton,
           impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShowAllButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CVSListBox@@SAPEAUCRuntimeClass@@XZ
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CVSListBox@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CVSListBoxBase@@SAPEAUCRuntimeClass@@XZ
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CVSListBoxBase@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CVSListBoxEditCtrl@@SAPEAUCRuntimeClass@@XZ
// Implemented in dlgcommon.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CVSListBoxEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
