// MessageMaps — OpenMFC implementation.
// Sources: global_ole_10_msgmap.cpp, global_other_11_msgmap.cpp, global_other_12_msgmap.cpp, global_other_13_msgmap.cpp, global_pane_15_msgmap.cpp

#include "detail/Ole10MsgmapSupport.h"
#include "detail/Other11MsgmapSupport.h"
#include "detail/Other12MsgmapSupport.h"
#include "detail/Other13MsgmapSupport.h"
#include "detail/Pane15MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@COleResizeBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleResizeBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleResizeBar,
       impl__GetMessageMap_COleResizeBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleResizeBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CControlBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CControlBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CControlBar,
       impl__GetMessageMap_CControlBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDialogBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDialogBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDialogBar,
       impl__GetMessageMap_CDialogBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDialogBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CReBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CReBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CReBar,
       impl__GetMessageMap_CReBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CReBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CStatusBar,
       impl__GetMessageMap_CStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CStatusBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CToolBar,
       impl__GetMessageMap_CToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CToolBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CDockBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockBar,
       impl__GetMessageMap_CDockBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
