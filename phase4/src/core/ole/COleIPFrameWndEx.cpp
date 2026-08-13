// COleIPFrameWndEx — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ?OnUpdatePaneMenu@COleIPFrameWndEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_COleIPFrameWndEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ??0COleIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleIPFrameWndEx__IEAA_XZ() {
    return nullptr;
}

// Symbol: ??1COleIPFrameWndEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1COleIPFrameWndEx__MEAA_XZ() {
    return nullptr;
}

// Symbol: ?AddDockSite@COleIPFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleIPFrameWndEx__QEAAXXZ() {}

// Symbol: ?AddPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AdjustDockingLayout@COleIPFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleIPFrameWndEx__UEAAXPEAX_Z(void* p0) {}

// Symbol: ?CreateObject@COleIPFrameWndEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleIPFrameWndEx__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DockPane@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleIPFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(void* /*class*/* p0, unsigned int p1, const void* /*struct*/* p2) {}

// Symbol: ?DockPaneLeftOf@COleIPFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleIPFrameWndEx__QEAAHPEAVCPane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?EnableAutoHidePanes@COleIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_COleIPFrameWndEx__QEAAHK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?EnableDocking@COleIPFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleIPFrameWndEx__QEAAHK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?GetContainerFrameWindow@COleIPFrameWndEx@@QEAAPEAVCOleCntrFrameWndEx@@XZ
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ() {
    return nullptr;
}

// Symbol: ?GetPane@COleIPFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_COleIPFrameWndEx__QEAAPEAVCBasePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?HtmlHelpW@COleIPFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_COleIPFrameWndEx__MEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}

// Symbol: ?InsertPane@COleIPFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleIPFrameWndEx__QEAAHPEAVCBasePane__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?IsPointNearDockSite@COleIPFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_COleIPFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(void* /*class*/ p0, unsigned long* p1, int* p2) {
    return 0;
}

// Symbol: ?LoadFrame@COleIPFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleIPFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(unsigned int p0, unsigned long p1, void* /*class*/* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnActivate@COleIPFrameWndEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_COleIPFrameWndEx__IEAAXIPEAVCWnd__H_Z(unsigned int p0, void* /*class*/* p1, int p2) {}

// Symbol: ?OnClose@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_COleIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnClosePopupMenu@COleIPFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_COleIPFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {}

// Symbol: ?OnCmdMsg@COleIPFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleIPFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, int p1, void* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnCommand@COleIPFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_COleIPFrameWndEx__MEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnContextHelp@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnCreateControlBars@COleIPFrameWndEx@@MEAAHPEAVCFrameWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleIPFrameWndEx__MEAAHPEAVCFrameWnd__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnDestroy@COleIPFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWndEx__IEAAXXZ() {}

// Symbol: ?OnIdleUpdateCmdUI@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnMenuChar@COleIPFrameWndEx@@IEAA_JIIPEAVCMenu@@@Z
extern "C" __int64 MS_ABI impl__OnMenuChar_COleIPFrameWndEx__IEAA_JIIPEAVCMenu___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnMoveMiniFrame@COleIPFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleIPFrameWndEx__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnPaneCheck@COleIPFrameWndEx@@IEAAHI@Z
extern "C" int MS_ABI impl__OnPaneCheck_COleIPFrameWndEx__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSetMenu@COleIPFrameWndEx@@IEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnSetMenu_COleIPFrameWndEx__IEAAHPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnSetPreviewMode@COleIPFrameWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_COleIPFrameWndEx__UEAAXHPEAUCPrintPreviewState___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnShowPanes@COleIPFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleIPFrameWndEx__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnToolbarContextMenu@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarContextMenu_COleIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnToolbarCreateNew@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_COleIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnToolbarDelete@COleIPFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarDelete_COleIPFrameWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, bool p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PaneFromPoint@COleIPFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_COleIPFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(void* /*class*/ p0, int p1, unsigned long* p2, void* /*struct*/* p3) {
    return nullptr;
}

// Symbol: ?PreCreateWindow@COleIPFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_COleIPFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?PreTranslateMessage@COleIPFrameWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleIPFrameWndEx__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@COleIPFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleIPFrameWndEx__UEAAXH_Z(int p0) {}

// Symbol: ?RemovePaneFromDockManager@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleIPFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?ShowPane@COleIPFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_COleIPFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {}

// Symbol: ?ShowPopupMenu@COleIPFrameWndEx@@IEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_COleIPFrameWndEx__IEAAHPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?WinHelpW@COleIPFrameWndEx@@UEAAX_KI@Z
extern "C" void MS_ABI impl__WinHelpW_COleIPFrameWndEx__UEAAX_KI_Z(unsigned __int64 p0, unsigned int p1) {}
