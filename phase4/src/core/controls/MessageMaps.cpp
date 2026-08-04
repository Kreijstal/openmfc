// MessageMaps — OpenMFC implementation.
// Sources: global_other_13_msgmap.cpp

#include "detail/Other13MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CSplitButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitButton,
       impl__GetMessageMap_CSplitButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CToolBarCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CToolBarCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CToolBarCtrl,
       impl__GetMessageMap_CToolBarCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CToolBarCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
