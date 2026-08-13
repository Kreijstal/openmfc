// COleDocIPFrameWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ??0COleDocIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWndEx__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?OnUpdatePaneMenu@COleDocIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleDocIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ??1COleDocIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleDocIPFrameWndEx__MEAA_XZ() {
    return nullptr;
}

// Symbol: ?AddDockSite@COleDocIPFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleDocIPFrameWndEx__QEAAXXZ() {}

// Symbol: ?AddPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AdjustDockingLayout@COleDocIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleDocIPFrameWndEx__UEAAXPEAX_Z(void* p0) {}

// Symbol: ?CreateObject@COleDocIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleDocIPFrameWndEx__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DockPane@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(void* /*class*/* p0, unsigned int p1, const void* /*struct*/* p2) {}

// Symbol: ?DockPaneLeftOf@COleDocIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleDocIPFrameWndEx__QEAAHPEAVCPane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?EnableAutoHidePanes@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_COleDocIPFrameWndEx__QEAAHK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?EnableDocking@COleDocIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleDocIPFrameWndEx__QEAAHK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?GetContainerFrameWindow@COleDocIPFrameWndEx@@QEAAPEAVCOleCntrFrameWndEx@@XZ
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleDocIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ() {
    return nullptr;
}

// Symbol: ?GetPane@COleDocIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleDocIPFrameWndEx__QEAAPEAVCBasePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?HtmlHelpW@COleDocIPFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_COleDocIPFrameWndEx__MEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}

// Symbol: ?InsertPane@COleDocIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleDocIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?IsPointNearDockSite@COleDocIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_COleDocIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(void* /*class*/ p0, unsigned long* p1, int* p2) {
    return 0;
}

// Symbol: ?LoadFrame@COleDocIPFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleDocIPFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(unsigned int p0, unsigned long p1, void* /*class*/* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnActivate@COleDocIPFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_COleDocIPFrameWndEx__IEAAXIPEAVCWnd__H_Z(unsigned int p0, void* /*class*/* p1, int p2) {}

// Symbol: ?OnClose@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_COleDocIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnClosePopupMenu@COleDocIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleDocIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {}

// Symbol: ?OnCmdMsg@COleDocIPFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleDocIPFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, int p1, void* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnCommand@COleDocIPFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COleDocIPFrameWndEx__MEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnContextHelp@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleDocIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnCreateControlBars@COleDocIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleDocIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnDestroy@COleDocIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleDocIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnIdleUpdateCmdUI@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_COleDocIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnMenuChar@COleDocIPFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_COleDocIPFrameWndEx__IEAA_JIIPEAVCMenu___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnMoveMiniFrame@COleDocIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleDocIPFrameWndEx__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnPaneCheck@COleDocIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleDocIPFrameWndEx__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSetMenu@COleDocIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleDocIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnSetPreviewMode@COleDocIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleDocIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnShowPanes@COleDocIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleDocIPFrameWndEx__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnToolbarContextMenu@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_COleDocIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnToolbarCreateNew@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_COleDocIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnToolbarDelete@COleDocIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleDocIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, bool p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PaneFromPoint@COleDocIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleDocIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, unsigned long* p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PreCreateWindow@COleDocIPFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_COleDocIPFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?PreTranslateMessage@COleDocIPFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleDocIPFrameWndEx__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@COleDocIPFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleDocIPFrameWndEx__UEAAXH_Z(int p0) {}

// Symbol: ?RemovePaneFromDockManager@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?ShowPane@COleDocIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_COleDocIPFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {}

// Symbol: ?ShowPopupMenu@COleDocIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleDocIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?WinHelpW@COleDocIPFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_COleDocIPFrameWndEx__UEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}
