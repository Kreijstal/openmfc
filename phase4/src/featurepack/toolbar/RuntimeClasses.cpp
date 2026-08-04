// RuntimeClasses — OpenMFC implementation.
// Sources: global_mfc_toolbars_rtti.cpp, global_toolbar_buttons_rtti.cpp

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

#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCBaseToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCBaseToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCBaseToolBar,
           impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCBaseToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCToolBar,
           impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCDropDownToolBar,
           impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPrintPreviewToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPrintPreviewToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCPrintPreviewToolBar,
           impl__GetThisClass_CMFCPrintPreviewToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPrintPreviewToolBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCToolBarButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarButton,
           impl__GetThisClass_CMFCToolBarButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarComboBoxButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarComboBoxButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarComboBoxButton,
           impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarComboBoxButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CHelpComboBoxButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CHelpComboBoxButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CHelpComboBoxButton,
           impl__GetThisClass_CHelpComboBoxButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CHelpComboBoxButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarMenuButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarMenuButton,
           impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarMenuButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownToolbarButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownToolbarButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCDropDownToolbarButton,
           impl__GetThisClass_CMFCDropDownToolbarButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownToolbarButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarColorButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarColorButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarColorButton,
           impl__GetThisClass_CMFCToolBarColorButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarColorButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarDateTimeCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarDateTimeCtrl@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarDateTimeCtrl,
           impl__GetThisClass_CMFCToolBarDateTimeCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarDateTimeCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarEditBoxButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarEditBoxButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarEditBoxButton,
           impl__GetThisClass_CMFCToolBarEditBoxButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarEditBoxButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarFontComboBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarFontComboBox@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarFontComboBox,
           impl__GetThisClass_CMFCToolBarFontComboBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarFontComboBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarFontSizeComboBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarFontSizeComboBox@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarFontSizeComboBox,
           impl__GetThisClass_CMFCToolBarFontSizeComboBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarFontSizeComboBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarMenuButtonsButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarMenuButtonsButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarMenuButtonsButton,
           impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarMenuButtonsButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarSpinEditBoxButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarSpinEditBoxButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarSpinEditBoxButton,
           impl__GetThisClass_CMFCToolBarSpinEditBoxButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarSpinEditBoxButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBarSystemMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBarSystemMenuButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCToolBarSystemMenuButton,
           impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBarSystemMenuButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
