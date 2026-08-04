// MessageMaps — OpenMFC implementation.
// Sources: global_other_11_msgmap.cpp

#include "detail/Other11MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CCmdTarget@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCmdTarget@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCmdTarget,
       impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
