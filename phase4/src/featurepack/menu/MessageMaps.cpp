// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_05_msgmap.cpp, global_mfc_06_msgmap.cpp

#include "detail/Mfc05MsgmapSupport.h"
#include "detail/Mfc06MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMenuBar,
       impl__GetMessageMap_CMFCMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMenuBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMenuButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMenuButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMenuButton,
       impl__GetMessageMap_CMFCMenuButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMenuButton__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCPopupMenu@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPopupMenu@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPopupMenu,
       impl__GetMessageMap_CMFCPopupMenu__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCPopupMenuBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPopupMenuBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPopupMenuBar,
       impl__GetMessageMap_CMFCPopupMenuBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
