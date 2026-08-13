// CMFCPopupMenuBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?ImportFromMenu@CMFCPopupMenuBar@@UEAAHPEAUHMENU__@@H@Z
extern "C" int MS_ABI impl__ImportFromMenu_CMFCPopupMenuBar__UEAAHPEAUHMENU____H_Z(CMFCPopupMenuBar* pThis, HMENU hMenu, int bShowAllCommands) {
    return pThis ? pThis->ImportFromMenu(hMenu, bShowAllCommands) : FALSE;
}
// Symbol: ?ExportToMenu@CMFCPopupMenuBar@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__ExportToMenu_CMFCPopupMenuBar__UEBAPEAUHMENU____XZ(const CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->ExportToMenu() : nullptr;
}
// Symbol: ?BuildOrigItems@CMFCPopupMenuBar@@QEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCPopupMenuBar__QEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int uiMenuResID) {
    return pThis ? pThis->BuildOrigItems(uiMenuResID) : FALSE;
}
// Symbol: ?GetGutterWidth@CMFCPopupMenuBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->GetGutterWidth() : 0;
}
// Symbol: ?CalcSize@CMFCPopupMenuBar@@MEAA?AVCSize@@H@Z
extern "C" void MS_ABI impl__CalcSize_CMFCPopupMenuBar__MEAA_AVCSize__H_Z(CSize* pRet, CMFCPopupMenuBar* pThis, int bVertDock) {
    new (pRet) CSize(pThis ? pThis->CalcSize(bVertDock) : CSize());
}
// Symbol: ?AdjustLayout@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLayout();
}
// Symbol: ?AdjustLocations@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}
// Symbol: ?CloseDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->CloseDelayedSubMenu();
}
CMFCPopupMenuBar::CMFCPopupMenuBar() {
    memset(_mfcpopupmenubar_padding, 0, sizeof(_mfcpopupmenubar_padding));
}
CMFCPopupMenuBar::~CMFCPopupMenuBar() {
    ClearPopupMenuBarState(this);
}
BOOL CMFCPopupMenuBar::ImportFromMenu(HMENU hMenu, BOOL) {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.menu = hMenu;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    return TRUE;
}
HMENU CMFCPopupMenuBar::ExportToMenu() const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (it->second.menu) return it->second.menu;

    HMENU hMenu = ::CreatePopupMenu();
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (!item) continue;
        if (item->m_hMenu) {
            ::AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(item->m_hMenu), static_cast<const wchar_t*>(item->m_strText));
        } else {
            ::AppendMenuW(hMenu, MF_STRING, static_cast<UINT_PTR>(item->m_nID), static_cast<const wchar_t*>(item->m_strText));
        }
    }
    return hMenu;
}
BOOL CMFCPopupMenuBar::BuildOrigItems(UINT) { return TRUE; }
CMFCToolBarMenuButton* CMFCPopupMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
int CMFCPopupMenuBar::GetGutterWidth() const { return 22; }
CSize CMFCPopupMenuBar::CalcSize(BOOL) {
    auto it = g_popupMenuBarStates.find(this);
    const int count = it == g_popupMenuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(160, std::max(22, count * 22));
}
void CMFCPopupMenuBar::AdjustLayout() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    if (GetSafeHwnd()) {
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
        ::UpdateWindow(GetSafeHwnd());
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].recentRect = CRect(0, 0, 160, std::max(22, static_cast<int>(state.items.size()) * 22));
}
void CMFCPopupMenuBar::AdjustLocations() {
    AdjustLayout();
}
void CMFCPopupMenuBar::CloseDelayedSubMenu() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.delayedSubMenuOpen = false;
    state.selected = -1;
}
// Symbol: ?CreateDroppedButton@CMFCPopupMenuBar@@MEAAPEAVCMFCToolBarButton@@PEAVCOleDataObject@@@Z
extern "C" void* MS_ABI impl__CreateDroppedButton_CMFCPopupMenuBar__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?DrawDragCursor@CMFCPopupMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawDragCursor_CMFCPopupMenuBar__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?DrawSeparator@CMFCPopupMenuBar@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCPopupMenuBar__MEAAXPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {}

// Symbol: ?EnableContextMenuItems@CMFCPopupMenuBar@@MEAAHPEAVCMFCToolBarButton@@PEAVCMenu@@@Z
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCPopupMenuBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?FindDestintationToolBar@CMFCPopupMenuBar@@QEAAPEAVCMFCToolBar@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?FindDropIndex@CMFCPopupMenuBar@@MEBAHVCPoint@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__FindDropIndex_CMFCPopupMenuBar__MEBAHVCPoint__AEAVCRect___Z(void* /*class*/ p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?get_accRole@CMFCPopupMenuBar@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCPopupMenuBar__UEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?get_accState@CMFCPopupMenuBar@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCPopupMenuBar__UEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetCommandTarget@CMFCPopupMenuBar@@MEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetCommandTarget_CMFCPopupMenuBar__MEBAPEAVCWnd__XZ() {
    return nullptr;
}

// Symbol: ?InvokeMenuCommand@CMFCPopupMenuBar@@IEAAXIPEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__InvokeMenuCommand_CMFCPopupMenuBar__IEAAXIPEBVCMFCToolBarButton___Z(unsigned int p0, const void* /*class*/* p1) {}

// Symbol: ?LoadFromHash@CMFCPopupMenuBar@@QEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__LoadFromHash_CMFCPopupMenuBar__QEAAHPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnAfterButtonDelete@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCPopupMenuBar__MEAAXXZ() {}

// Symbol: ?OnCalcSeparatorRect@CMFCPopupMenuBar@@MEAAXPEAVCMFCToolBarButton@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnCalcSeparatorRect_CMFCPopupMenuBar__MEAAXPEAVCMFCToolBarButton__AEAVCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// Symbol: ?OnChangeHot@CMFCPopupMenuBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCPopupMenuBar__MEAAXH_Z(int p0) {}

// Symbol: ?OnCreate@CMFCPopupMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenuBar__IEAAXXZ() {}

// Symbol: ?OnDragOver@CMFCPopupMenuBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCPopupMenuBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(void* /*class*/* p0, unsigned long p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnFillBackground@CMFCPopupMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCPopupMenuBar__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnIdleUpdateCmdUI@CMFCPopupMenuBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMFCPopupMenuBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnKey@CMFCPopupMenuBar@@MEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCPopupMenuBar__MEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?OnLButtonDblClk@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCPopupMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonDown@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPopupMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPopupMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseMove@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcCalcSize@CMFCPopupMenuBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCPopupMenuBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcPaint@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCPopupMenuBar__IEAAXXZ() {}

// Symbol: ?OnSendCommand@CMFCPopupMenuBar@@MEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCPopupMenuBar__MEAAHPEBVCMFCToolBarButton___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnSetDefaultButtonText@CMFCPopupMenuBar@@MEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCPopupMenuBar__MEAAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnTimer@CMFCPopupMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCPopupMenuBar__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnToolbarImageAndText@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImageAndText_CMFCPopupMenuBar__IEAAXXZ() {}

// Symbol: ?OnToolbarText@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarText_CMFCPopupMenuBar__IEAAXXZ() {}

// Symbol: ?OnToolHitTest@CMFCPopupMenuBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCPopupMenuBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(void* /*class*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?RestoreDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ() {}

// Symbol: ?SetButtonStyle@CMFCPopupMenuBar@@UEAAXHI@Z
extern "C" void MS_ABI impl__SetButtonStyle_CMFCPopupMenuBar__UEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?SetInCommand@CMFCPopupMenuBar@@IEAAXH@Z
extern "C" void MS_ABI impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(int p0) {}

// Symbol: ?ShowCommandMessageString@CMFCPopupMenuBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCPopupMenuBar__MEAAXI_Z(unsigned int p0) {}

// Symbol: ?StartPopupMenuTimer@CMFCPopupMenuBar@@QEAAXPEAVCMFCToolBarMenuButton@@H@Z
extern "C" void MS_ABI impl__StartPopupMenuTimer_CMFCPopupMenuBar__QEAAXPEAVCMFCToolBarMenuButton__H_Z(void* /*class*/* p0, int p1) {}
