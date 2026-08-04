// MessageMaps — OpenMFC implementation.
// Sources: global_ribbon_19_msgmap.cpp, global_ribbon_20_msgmap.cpp

#include "detail/Ribbon19MsgmapSupport.h"
#include "detail/Ribbon20MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCRibbonBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonBar,
       impl__GetMessageMap_CMFCRibbonBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonCommandsListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonCommandsListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonCommandsListBox,
       impl__GetMessageMap_CMFCRibbonCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonCommandsListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonCustomizePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonCustomizePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonCustomizePropertyPage,
       impl__GetMessageMap_CMFCRibbonCustomizePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonCustomizePropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonKeyTip@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonKeyTip@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonKeyTip,
       impl__GetMessageMap_CMFCRibbonKeyTip__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonKeyTip__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonKeyboardCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonKeyboardCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonKeyboardCustomizeDialog,
       impl__GetMessageMap_CMFCRibbonKeyboardCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonKeyboardCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonMiniToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonMiniToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonMiniToolBar,
       impl__GetMessageMap_CMFCRibbonMiniToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonMiniToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonPanelMenu@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonPanelMenu@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonPanelMenu,
       impl__GetMessageMap_CMFCRibbonPanelMenu__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonPanelMenu__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonPanelMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonPanelMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonPanelMenuBar,
       impl__GetMessageMap_CMFCRibbonPanelMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonPanelMenuBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCRibbonRichEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonRichEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonRichEditCtrl,
       impl__GetMessageMap_CMFCRibbonRichEditCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonRichEditCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonSpinButtonCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonSpinButtonCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonSpinButtonCtrl,
       impl__GetMessageMap_CMFCRibbonSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCRibbonStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCRibbonStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCRibbonStatusBar,
       impl__GetMessageMap_CMFCRibbonStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCRibbonStatusBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
