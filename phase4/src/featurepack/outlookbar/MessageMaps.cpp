// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_05_msgmap.cpp, global_mfc_06_msgmap.cpp, global_other_12_msgmap.cpp

#include "detail/Mfc05MsgmapSupport.h"
#include "detail/Mfc06MsgmapSupport.h"
#include "detail/Other12MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCOutlookBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBar,
       impl__GetMessageMap_CMFCOutlookBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarPane,
       impl__GetMessageMap_CMFCOutlookBarPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCOutlookBarPaneAdapter@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarPaneAdapter@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarPaneAdapter,
       impl__GetMessageMap_CMFCOutlookBarPaneAdapter__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarPaneAdapter__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarTabCtrl,
       impl__GetMessageMap_CMFCOutlookBarTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarTabCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCOutlookBarToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCOutlookBarToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCOutlookBarToolBar,
       impl__GetMessageMap_CMFCOutlookBarToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCOutlookBarToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@COutlookOptionsDlg@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COutlookOptionsDlg@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COutlookOptionsDlg,
       impl__GetMessageMap_COutlookOptionsDlg__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COutlookOptionsDlg__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
