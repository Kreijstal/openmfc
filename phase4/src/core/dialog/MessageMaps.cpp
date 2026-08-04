// MessageMaps — OpenMFC implementation.
// Sources: global_other_11_msgmap.cpp, global_other_12_msgmap.cpp

#include "detail/Other11MsgmapSupport.h"
#include "detail/Other12MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CCommonDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCommonDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCommonDialog,
       impl__GetMessageMap_CCommonDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CPrintDialogEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPrintDialogEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPrintDialogEx,
       impl__GetMessageMap_CPrintDialogEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPrintDialogEx__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
