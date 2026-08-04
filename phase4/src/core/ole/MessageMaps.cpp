// MessageMaps — OpenMFC implementation.
// Sources: global_ole_09_msgmap.cpp, global_ole_10_msgmap.cpp, global_other_11_msgmap.cpp

#include "detail/Ole09MsgmapSupport.h"
#include "detail/Ole10MsgmapSupport.h"
#include "detail/Other11MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@COleCntrFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleCntrFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleCntrFrameWndEx,
       impl__GetMessageMap_COleCntrFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleCntrFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleControl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleControl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleControl,
       impl__GetMessageMap_COleControl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleControl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleDocIPFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDocIPFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDocIPFrameWnd,
       impl__GetMessageMap_COleDocIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDocIPFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleDocIPFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDocIPFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDocIPFrameWndEx,
       impl__GetMessageMap_COleDocIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDocIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleIPFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleIPFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleIPFrameWnd,
       impl__GetMessageMap_COleIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleIPFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleIPFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleIPFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleIPFrameWndEx,
       impl__GetMessageMap_COleIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COlePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COlePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COlePropertyPage,
       impl__GetMessageMap_COlePropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COlePropertyPage__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@COleServerDoc@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleServerDoc@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleServerDoc,
       impl__GetMessageMap_COleServerDoc__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleServerDoc__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@COleServerDocEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleServerDocEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleServerDocEx,
       impl__GetMessageMap_COleServerDocEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleServerDocEx__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CDocObjectServer@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDocObjectServer@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDocObjectServer,
       impl__GetMessageMap_CDocObjectServer__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDocObjectServer__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
