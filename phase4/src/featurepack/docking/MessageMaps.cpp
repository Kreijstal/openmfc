// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_02_msgmap.cpp, global_pane_15_msgmap.cpp, global_pane_16_msgmap.cpp, global_pane_17_msgmap.cpp

#include "detail/Mfc02MsgmapSupport.h"
#include "detail/Pane15MsgmapSupport.h"
#include "detail/Pane16MsgmapSupport.h"
#include "detail/Pane17MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCAutoHideBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCAutoHideBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCAutoHideBar,
       impl__GetMessageMap_CMFCAutoHideBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCAutoHideBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CAutoHideDockSite@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CAutoHideDockSite@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CAutoHideDockSite,
       impl__GetMessageMap_CAutoHideDockSite__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CAutoHideDockSite__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CBasePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CBasePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CBasePane,
       impl__GetMessageMap_CBasePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CBaseTabbedPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CBaseTabbedPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CBaseTabbedPane,
       impl__GetMessageMap_CBaseTabbedPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockSite@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockSite@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockSite,
       impl__GetMessageMap_CDockSite__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockSite__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockablePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockablePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockablePane,
       impl__GetMessageMap_CDockablePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDockablePaneAdapter@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDockablePaneAdapter@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDockablePaneAdapter,
       impl__GetMessageMap_CDockablePaneAdapter__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CDummyDockablePane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CDummyDockablePane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CDummyDockablePane,
       impl__GetMessageMap_CDummyDockablePane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CDummyDockablePane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMiniDockFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMiniDockFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMiniDockFrameWnd,
       impl__GetMessageMap_CMiniDockFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMiniDockFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMultiPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMultiPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMultiPaneFrameWnd,
       impl__GetMessageMap_CMultiPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMultiPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPane,
       impl__GetMessageMap_CPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneDialog,
       impl__GetMessageMap_CPaneDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneDivider@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneDivider@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneDivider,
       impl__GetMessageMap_CPaneDivider__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneDivider__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneFrameWnd,
       impl__GetMessageMap_CPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CPaneTrackingWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CPaneTrackingWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CPaneTrackingWnd,
       impl__GetMessageMap_CPaneTrackingWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CPaneTrackingWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSmartDockingGroupGuidesWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingGroupGuidesWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingGroupGuidesWnd,
       impl__GetMessageMap_CSmartDockingGroupGuidesWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingGroupGuidesWnd__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CSmartDockingHighlighterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingHighlighterWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingHighlighterWnd,
       impl__GetMessageMap_CSmartDockingHighlighterWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingHighlighterWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CSmartDockingStandaloneGuideWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CSmartDockingStandaloneGuideWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CSmartDockingStandaloneGuideWnd,
       impl__GetMessageMap_CSmartDockingStandaloneGuideWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CSmartDockingStandaloneGuideWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CTabbedPane@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CTabbedPane@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CTabbedPane,
       impl__GetMessageMap_CTabbedPane__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CTabbedPane__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
