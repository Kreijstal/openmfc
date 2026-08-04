// MessageMaps — OpenMFC implementation.
// Sources: global_other_12_msgmap.cpp

#include "detail/Other12MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CScreenWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CScreenWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CScreenWnd,
       impl__GetMessageMap_CScreenWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CScreenWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
