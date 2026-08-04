// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_02_msgmap.cpp, global_mfc_07_msgmap.cpp

#include "detail/Mfc02MsgmapSupport.h"
#include "detail/Mfc07MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCBaseTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCBaseTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCBaseTabCtrl,
       impl__GetMessageMap_CMFCBaseTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTabCtrl,
       impl__GetMessageMap_CMFCTabCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTabCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
