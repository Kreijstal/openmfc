// MessageMaps — OpenMFC implementation.
// Sources: global_propgrid_18_msgmap.cpp

#include "detail/Propgrid18MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCPropertyGridCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyGridCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyGridCtrl,
       impl__GetMessageMap_CMFCPropertyGridCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyGridCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertyGridToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyGridToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyGridToolTipCtrl,
       impl__GetMessageMap_CMFCPropertyGridToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyGridToolTipCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertyPage,
       impl__GetMessageMap_CMFCPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertySheet@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertySheet@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertySheet,
       impl__GetMessageMap_CMFCPropertySheet__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertySheet__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPropertySheetListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPropertySheetListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPropertySheetListBox,
       impl__GetMessageMap_CMFCPropertySheetListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPropertySheetListBox__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
