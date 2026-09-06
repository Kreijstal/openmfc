// RuntimeClasses — OpenMFC implementation.
// Sources: global_deferred_rtti.cpp, global_mfc_toolbars_rtti.cpp, global_tabctrl_renderer_list_rtti.cpp, global_toolbar_buttons_rtti.cpp

#include "detail/DeferredRttiSupport.h"

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

#define DEF_GETTERS(Cls, GT, GRC) \
    extern "C" CRuntimeClass* MS_ABI GT() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GRC(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCOutlookBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBar@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCOutlookBar,
            impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCOutlookBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarPaneAdapter@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarPaneAdapter@@UEBAPEAUCRuntimeClass@@XZ
DEF_GETTERS(CMFCOutlookBarPaneAdapter,
            impl__GetThisClass_CMFCOutlookBarPaneAdapter__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CMFCOutlookBarPaneAdapter__UEBAPEAUCRuntimeClass__XZ)
#undef DEF_GETTERS
#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCOutlookBarPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarPane@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCOutlookBarPane,
           impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarPane__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCOutlookBarToolBar,
           impl__GetThisClass_CMFCOutlookBarToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarToolBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCOutlookBarTabCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCOutlookBarTabCtrl,
           impl__GetThisClass_CMFCOutlookBarTabCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarTabCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef TABCTRL_RENDERER_LIST_GETTERS
#define TOOLBAR_BUTTONS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCOutlookBarPaneButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarPaneButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(CMFCOutlookBarPaneButton,
           impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarPaneButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@COutlookCustomizeButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COutlookCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
TOOLBAR_BUTTONS_GETTERS(COutlookCustomizeButton,
           impl__GetThisClass_COutlookCustomizeButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_COutlookCustomizeButton__UEBAPEAUCRuntimeClass__XZ)
#undef TOOLBAR_BUTTONS_GETTERS
