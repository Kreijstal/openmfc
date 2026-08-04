// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_07_msgmap.cpp, global_mfc_08_msgmap.cpp

#include "detail/Mfc07MsgmapSupport.h"
#include "detail/Mfc08MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCTasksPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPane,
       impl__GetMessageMap_CMFCTasksPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCTasksPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPaneFrameWnd,
       impl__GetMessageMap_CMFCTasksPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCTasksPaneToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCTasksPaneToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCTasksPaneToolBar,
       impl__GetMessageMap_CMFCTasksPaneToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCTasksPaneToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
