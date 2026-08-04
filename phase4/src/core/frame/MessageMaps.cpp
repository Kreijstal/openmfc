// MessageMaps — OpenMFC implementation.
// Sources: global_frame_01_msgmap.cpp, global_other_13_msgmap.cpp

#include "detail/Frame01MsgmapSupport.h"
#include "detail/Other13MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CControlFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CControlFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CControlFrameWnd,
       impl__GetMessageMap_CControlFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CControlFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CFrameWndEx,
       impl__GetMessageMap_CFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIChildWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIChildWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIChildWndEx,
       impl__GetMessageMap_CMDIChildWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIChildWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIClientAreaWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIClientAreaWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIClientAreaWnd,
       impl__GetMessageMap_CMDIClientAreaWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIClientAreaWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDIFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDIFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDIFrameWndEx,
       impl__GetMessageMap_CMDIFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDIFrameWndEx__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMDITabProxyWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMDITabProxyWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMDITabProxyWnd,
       impl__GetMessageMap_CMDITabProxyWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMDITabProxyWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMiniFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMiniFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMiniFrameWnd,
       impl__GetMessageMap_CMiniFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CSplitterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitterWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitterWnd,
       impl__GetMessageMap_CSplitterWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitterWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSplitterWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSplitterWndEx@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSplitterWndEx,
       impl__GetMessageMap_CSplitterWndEx__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSplitterWndEx__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
