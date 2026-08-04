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
