// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_02_msgmap.cpp, global_mfc_04_msgmap.cpp, global_mfc_06_msgmap.cpp, global_toolbar_21_msgmap.cpp, global_toolbar_22_msgmap.cpp

#include "detail/Mfc02MsgmapSupport.h"
#include "detail/Mfc04MsgmapSupport.h"
#include "detail/Mfc06MsgmapSupport.h"
#include "detail/Toolbar21MsgmapSupport.h"
#include "detail/Toolbar22MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCBaseToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCBaseToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCBaseToolBar,
       impl__GetMessageMap_CMFCBaseToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCDropDownToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDropDownToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDropDownToolBar,
       impl__GetMessageMap_CMFCDropDownToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDropDownToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCPrintPreviewToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPrintPreviewToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPrintPreviewToolBar,
       impl__GetMessageMap_CMFCPrintPreviewToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPrintPreviewToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBar,
       impl__GetMessageMap_CMFCToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarButtonCustomizeDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarButtonCustomizeDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarButtonCustomizeDialog,
       impl__GetMessageMap_CMFCToolBarButtonCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarButtonCustomizeDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarButtonsListButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarButtonsListButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarButtonsListButton,
       impl__GetMessageMap_CMFCToolBarButtonsListButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarButtonsListButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarComboBoxEdit@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarComboBoxEdit@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarComboBoxEdit,
       impl__GetMessageMap_CMFCToolBarComboBoxEdit__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarComboBoxEdit__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDateTimeCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDateTimeCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDateTimeCtrlImpl,
       impl__GetMessageMap_CMFCToolBarDateTimeCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDateTimeCtrlImpl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDropSource@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDropSource@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDropSource,
       impl__GetMessageMap_CMFCToolBarDropSource__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDropSource__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarDropTarget@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarDropTarget@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarDropTarget,
       impl__GetMessageMap_CMFCToolBarDropTarget__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarDropTarget__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCToolBarEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarEditCtrl,
       impl__GetMessageMap_CMFCToolBarEditCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarEditCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCToolBarNameDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolBarNameDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolBarNameDialog,
       impl__GetMessageMap_CMFCToolBarNameDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolBarNameDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
