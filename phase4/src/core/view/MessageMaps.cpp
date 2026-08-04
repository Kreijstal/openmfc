// MessageMaps — OpenMFC implementation.
// Sources: global_ole_09_msgmap.cpp, global_other_11_msgmap.cpp, global_other_12_msgmap.cpp, global_other_13_msgmap.cpp

#include "detail/Ole09MsgmapSupport.h"
#include "detail/Other11MsgmapSupport.h"
#include "detail/Other12MsgmapSupport.h"
#include "detail/Other13MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@COleDBRecordView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@COleDBRecordView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(COleDBRecordView,
       impl__GetMessageMap_COleDBRecordView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_COleDBRecordView__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CCtrlView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CCtrlView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CCtrlView,
       impl__GetMessageMap_CCtrlView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDHtmlDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDHtmlDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDHtmlDialog,
       impl__GetMessageMap_CDHtmlDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDHtmlDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CHtmlEditView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CHtmlEditView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CHtmlEditView,
       impl__GetMessageMap_CHtmlEditView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CHtmlEditView__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CHtmlView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CHtmlView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CHtmlView,
       impl__GetMessageMap_CHtmlView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CHtmlView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPreviewView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPreviewView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPreviewView,
       impl__GetMessageMap_CPreviewView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPreviewView__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPreviewViewEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPreviewViewEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPreviewViewEx,
       impl__GetMessageMap_CPreviewViewEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPreviewViewEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CRichEditView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CRichEditView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CRichEditView,
       impl__GetMessageMap_CRichEditView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CRichEditView__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CTabView@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CTabView@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CTabView,
       impl__GetMessageMap_CTabView__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CTabView__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
