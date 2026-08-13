// COleIPFrameWnd — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ?OnUpdateControlBarMenu@COleIPFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateControlBarMenu_COleIPFrameWnd__IEAAXPEAVCCmdUI___Z(COleIPFrameWnd*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
COleIPFrameWnd::COleIPFrameWnd()
    : CFrameWnd(), m_pResizeBar(nullptr) {
    memset(_oleipframewnd_padding, 0, sizeof(_oleipframewnd_padding));
}
COleIPFrameWnd::~COleIPFrameWnd() {
}
BOOL COleIPFrameWnd::OnCreateAggregates() {
    return TRUE;
}
BOOL COleIPFrameWnd::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc) {
    return TRUE;
}
void COleIPFrameWnd::OnRequestPositionChange(LPCRECT lpRect) {
    if (!m_hWnd || !lpRect) return;

    const int width = lpRect->right - lpRect->left;
    const int height = lpRect->bottom - lpRect->top;
    ::SetWindowPos(m_hWnd, nullptr, lpRect->left, lpRect->top,
                   width > 0 ? width : 0, height > 0 ? height : 0,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    if (m_pResizeBar && m_pResizeBar->GetSafeHwnd()) {
        m_pResizeBar->MoveWindow(lpRect->left, lpRect->top,
                                 width > 0 ? width : 0, height > 0 ? height : 0, TRUE);
    }
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnCreateControlBars@COleIPFrameWnd@@UEAAHPEAVCWnd@@0@Z
extern "C" int MS_ABI impl__OnCreateControlBars_COleIPFrameWnd__UEAAHPEAVCWnd__0_Z(void* pThis, void* p0, void* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
    return 0;
}

// Symbol: ?BuildSharedMenu@COleIPFrameWnd@@MEAAHXZ
extern "C" int MS_ABI impl__BuildSharedMenu_COleIPFrameWnd__MEAAHXZ() {
    return 0;
}

// Symbol: ?CreateObject@COleIPFrameWnd@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleIPFrameWnd__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DestroySharedMenu@COleIPFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__DestroySharedMenu_COleIPFrameWnd__MEAAXXZ() {}

// Symbol: ?GetInPlaceMenu@COleIPFrameWnd@@MEAAPEAUHMENU__@@XZ
extern "C" void* MS_ABI impl__GetInPlaceMenu_COleIPFrameWnd__MEAAPEAUHMENU____XZ() {
    return nullptr;
}

// Symbol: ?LoadFrame@COleIPFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_COleIPFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(unsigned int p0, unsigned long p1, void* /*class*/* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnBarCheck@COleIPFrameWnd@@IEAAHI@Z
extern "C" int MS_ABI impl__OnBarCheck_COleIPFrameWnd__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnContextHelp@COleIPFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_COleIPFrameWnd__IEAAXXZ() {}

// Symbol: ?OnCreate@COleIPFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_COleIPFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@COleIPFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleIPFrameWnd__IEAAXXZ() {}

// Symbol: ?OnRecalcParent@COleIPFrameWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnRecalcParent_COleIPFrameWnd__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnResizeChild@COleIPFrameWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnResizeChild_COleIPFrameWnd__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSetMessageString@COleIPFrameWnd@@UEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMessageString_COleIPFrameWnd__UEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnSize@COleIPFrameWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleIPFrameWnd__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnWindowPosChanging@COleIPFrameWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_COleIPFrameWnd__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Symbol: ?PreTranslateMessage@COleIPFrameWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_COleIPFrameWnd__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@COleIPFrameWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleIPFrameWnd__UEAAXH_Z(int p0) {}

// Symbol: ?RepositionFrame@COleIPFrameWnd@@UEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__RepositionFrame_COleIPFrameWnd__UEAAXPEBUtagRECT__0_Z(const void* /*struct*/* p0, const void* /*struct*/* p1) {}
