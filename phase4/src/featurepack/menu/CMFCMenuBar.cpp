// CMFCMenuBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?Create@CMFCMenuBar@@UEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCMenuBar__UEAAHPEAVCWnd__KI_Z(CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID) {
    return pThis ? pThis->Create(pParentWnd, dwStyle, nID) : FALSE;
}
// Symbol: ?CreateEx@CMFCMenuBar@@UEAAHPEAVCWnd@@KKVCRect@@I@Z
extern "C" int MS_ABI impl__CreateEx_CMFCMenuBar__UEAAHPEAVCWnd__KKVCRect__I_Z(
    CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, CRect rcBorders, unsigned int nID) {
    return pThis ? pThis->CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID) : FALSE;
}
// Symbol: ?CreateFromMenu@CMFCMenuBar@@UEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(CMFCMenuBar* pThis, HMENU hMenu, int bDefaultMenu, int bForceUpdate) {
    if (pThis) pThis->CreateFromMenu(hMenu, bDefaultMenu, bForceUpdate);
}
// Symbol: ?GetMenuItem@CMFCMenuBar@@QEBAPEAVCMFCToolBarButton@@H@Z
extern "C" CMFCToolBarButton* MS_ABI impl__GetMenuItem_CMFCMenuBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCMenuBar* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}
// Symbol: ?CalcFixedLayout@CMFCMenuBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CMFCMenuBar__UEAA_AVCSize__HH_Z(CSize* pRet, CMFCMenuBar* pThis, int bStretch, int bHorz) {
    (void)bStretch;
    (void)bHorz;
    new (pRet) CSize(CalcMenuBarFixedLayout(pThis));
}
// Symbol: ?CalcLayout@CMFCMenuBar@@UEAA?AVCSize@@KH@Z
extern "C" void MS_ABI impl__CalcLayout_CMFCMenuBar__UEAA_AVCSize__KH_Z(CSize* pRet, CMFCMenuBar* pThis, unsigned long dwMode, int nLength) {
    new (pRet) CSize(pThis ? pThis->CalcLayout(dwMode, nLength) : CSize());
}
// Symbol: ?AdjustLocations@CMFCMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCMenuBar__UEAAXXZ(CMFCMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}
// Symbol: ?GetMenuFont@CMFCMenuBar@@SAAEBVCFont@@H@Z
extern "C" CFont* MS_ABI impl__GetMenuFont_CMFCMenuBar__SAAEBVCFont__H_Z(int bHorz) {
    return &CMFCMenuBar::GetMenuFont(bHorz);
}
CMFCMenuBar::CMFCMenuBar() {
    memset(_mfcmenubar_padding, 0, sizeof(_mfcmenubar_padding));
}
CMFCMenuBar::~CMFCMenuBar() {
    ClearMenuBarState(this);
}
BOOL CMFCMenuBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    g_menuBarStates[this].parent = pParentWnd;
    return CMFCToolBar::Create(pParentWnd, dwStyle, nID);
}
BOOL CMFCMenuBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}
void CMFCMenuBar::CreateFromMenu(HMENU hMenu, BOOL, BOOL) {
    MenuBarState& state = g_menuBarStates[this];
    for (CMFCToolBarButton* item : state.ownedItems) {
        delete item;
    }
    state.items.clear();
    state.ownedItems.clear();
    state.menu = hMenu;

    if (!hMenu) return;
    int count = ::GetMenuItemCount(hMenu);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarMenuButton* button = CreateMenuButtonFromMenu(hMenu, i);
        state.items.push_back(button);
        state.ownedItems.insert(button);
    }
}
CMFCToolBarButton* CMFCMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_menuBarStates.find(this);
    if (it == g_menuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
CSize CMFCMenuBar::CalcLayout(DWORD, int nLength) {
    CSize size = CalcMenuBarFixedLayout(this);
    if (nLength > 0) size.cx = nLength;
    return size;
}
void CMFCMenuBar::AdjustLocations() {
    MenuBarState& state = g_menuBarStates[this];
    const int count = static_cast<int>(state.items.size());
    const int height = 22;
    const int width = count > 0 ? std::max(23, (count * 80)) : 23;
    if (GetSafeHwnd()) {
        ::SendMessageW(GetSafeHwnd(), TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].recentRect = CRect(0, 0, width, height);
}
CFont& CMFCMenuBar::GetMenuFont(BOOL) {
    return EnsureMenuFont_Mfccore();
}
// Symbol: ?BuildOrigItems@CMFCMenuBar@@MEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCMenuBar__MEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?CalcMaxButtonHeight@CMFCMenuBar@@UEAAHXZ
extern "C" int MS_ABI impl__CalcMaxButtonHeight_CMFCMenuBar__UEAAHXZ() {
    return 0;
}

// Symbol: ?CalcSysButtonSize@CMFCMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__CalcSysButtonSize_CMFCMenuBar__IEAAXXZ() {}

// Symbol: ?EnableHelpCombobox@CMFCMenuBar@@QEAAXIPEB_WH@Z
extern "C" void MS_ABI impl__EnableHelpCombobox_CMFCMenuBar__QEAAXIPEB_WH_Z(unsigned int p0, const wchar_t* p1, int p2) {}

// Symbol: ?FindDropIndex@CMFCMenuBar@@MEBAHVCPoint@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__FindDropIndex_CMFCMenuBar__MEBAHVCPoint__AEAVCRect___Z(void* /*class*/ p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?FindMenuItemText@CMFCMenuBar@@KAHPEAUHMENU__@@IAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__FindMenuItemText_CMFCMenuBar__KAHPEAUHMENU____IAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*struct*/* p0, unsigned int p1, void* /*class*/* p2, void** p3, void* p4) {
    return 0;
}

// Symbol: ?GetFloatPopupDirection@CMFCMenuBar@@QEAAHPEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__GetFloatPopupDirection_CMFCMenuBar__QEAAHPEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetHelpCombobox@CMFCMenuBar@@QEAAPEAVCMFCToolBarComboBoxButton@@XZ
extern "C" void* MS_ABI impl__GetHelpCombobox_CMFCMenuBar__QEAAPEAVCMFCToolBarComboBoxButton__XZ() {
    return nullptr;
}

// Symbol: ?GetRowHeight@CMFCMenuBar@@UEBAHXZ
extern "C" int MS_ABI impl__GetRowHeight_CMFCMenuBar__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetSystemButton@CMFCMenuBar@@QEBAPEAVCMFCToolBarMenuButtonsButton@@IH@Z
extern "C" void* MS_ABI impl__GetSystemButton_CMFCMenuBar__QEBAPEAVCMFCToolBarMenuButtonsButton__IH_Z(unsigned int p0, int p1) {
    return nullptr;
}

// Symbol: ?GetSystemMenu@CMFCMenuBar@@QEBAPEAVCMFCToolBarSystemMenuButton@@XZ
extern "C" void* MS_ABI impl__GetSystemMenu_CMFCMenuBar__QEBAPEAVCMFCToolBarSystemMenuButton__XZ() {
    return nullptr;
}

// Symbol: ?IsPureMenuButton@CMFCMenuBar@@MEBAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__IsPureMenuButton_CMFCMenuBar__MEBAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?LoadState@CMFCMenuBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCMenuBar__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnChangeHot@CMFCMenuBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(int p0) {}

// Symbol: ?OnCreate@CMFCMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnLButtonDblClk@CMFCMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonDown@CMFCMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCMenuBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnSendCommand@CMFCMenuBar@@UEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCMenuBar__UEAAHPEBVCMFCToolBarButton___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnSetDefaultButtonText@CMFCMenuBar@@UEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCMenuBar__UEAAHPEAVCMFCToolBarButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnSetFocus@CMFCMenuBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCMenuBar__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSettingChange@CMFCMenuBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCMenuBar__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnTimer@CMFCMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCMenuBar__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnToolHitTest@CMFCMenuBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCMenuBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(void* /*class*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?PreTranslateMessage@CMFCMenuBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCMenuBar__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ResetImages@CMFCMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__ResetImages_CMFCMenuBar__MEAAXXZ() {}

// Symbol: ?RestoreMaximizeMode@CMFCMenuBar@@IEAAXH@Z
extern "C" void MS_ABI impl__RestoreMaximizeMode_CMFCMenuBar__IEAAXH_Z(int p0) {}

// Symbol: ?RestoreOriginalState@CMFCMenuBar@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreOriginalState_CMFCMenuBar__UEAAHXZ() {
    return 0;
}

// Symbol: ?SaveState@CMFCMenuBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCMenuBar__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?SetDefaultMenuResId@CMFCMenuBar@@QEAAXI@Z
extern "C" void MS_ABI impl__SetDefaultMenuResId_CMFCMenuBar__QEAAXI_Z(unsigned int p0) {}

// Symbol: ?SetMaximizeMode@CMFCMenuBar@@QEAAXHPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__SetMaximizeMode_CMFCMenuBar__QEAAXHPEAVCWnd__H_Z(int p0, void* /*class*/* p1, int p2) {}

// Symbol: ?SetMenuButtonRTC@CMFCMenuBar@@QEAAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SetMenuButtonRTC_CMFCMenuBar__QEAAXPEAUCRuntimeClass___Z(void* /*struct*/* p0) {}

// Symbol: ?SetMenuFont@CMFCMenuBar@@SAHPEAUtagLOGFONTW@@H@Z
extern "C" int MS_ABI impl__SetMenuFont_CMFCMenuBar__SAHPEAUtagLOGFONTW__H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?SetRecentlyUsedMenus@CMFCMenuBar@@SAXH@Z
extern "C" void MS_ABI impl__SetRecentlyUsedMenus_CMFCMenuBar__SAXH_Z(int p0) {}

// Symbol: ?SetShowAllCommands@CMFCMenuBar@@SAXH@Z
extern "C" void MS_ABI impl__SetShowAllCommands_CMFCMenuBar__SAXH_Z(int p0) {}

// Symbol: ?UpdateMDIChildrenMenus@CMFCMenuBar@@KAXPEAVCMultiDocTemplate@@@Z
extern "C" void MS_ABI impl__UpdateMDIChildrenMenus_CMFCMenuBar__KAXPEAVCMultiDocTemplate___Z(void* /*class*/* p0) {}
