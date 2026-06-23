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

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define RIBBON_ELEMENTS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
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
} // namespace

#define RIBBON_ELEMENTS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCBaseAccessibleObject@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCBaseAccessibleObject@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_ELEMENTS_GETTERS(CMFCBaseAccessibleObject,
           impl__GetThisClass_CMFCBaseAccessibleObject__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCBaseAccessibleObject__UEBAPEAUCRuntimeClass__XZ)
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
