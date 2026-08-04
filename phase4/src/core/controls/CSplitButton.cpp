// CSplitButton — OpenMFC implementation.
// Sources: ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"

// Symbol: ?Create@CSplitButton@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CSplitButton__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__I_Z(
    CSplitButton* pThis, const wchar_t* lpszCaption, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    return (pThis && pRect) ? pThis->Create(lpszCaption, dwStyle, *pRect, pParentWnd, nID) : FALSE;
}
// Symbol: ?SetDropDownMenu@CSplitButton@@QEAAXII@Z
extern "C" void MS_ABI impl__SetDropDownMenu_CSplitButton__QEAAXII_Z(CSplitButton* pThis, unsigned int nMenuId, unsigned int nSubMenu) {
    if (pThis) pThis->SetDropDownMenu(nMenuId, nSubMenu);
}
// Symbol: ?SetDropDownMenu@CSplitButton@@QEAAXPEAVCMenu@@@Z
extern "C" void MS_ABI impl__SetDropDownMenu_CSplitButton__QEAAXPEAVCMenu___Z(CSplitButton* pThis, CMenu* pMenu) {
    if (pThis) pThis->SetDropDownMenu(pMenu);
}
// CSplitButton::OnDropDown — BCN_DROPDOWN reflection handler. Drops the menu
// registered by SetDropDownMenu() below the button, left-aligned with its lower
// left corner, which is MFC's behaviour for the split-button arrow.
// Symbol: ?OnDropDown@CSplitButton@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDropDown_CSplitButton__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CSplitButton* pThis, NMHDR* /*pNMHDR*/, LRESULT* pResult) {
    if (pResult) *pResult = 0;
    if (!pThis) return;

    auto it = g_splitMenus.find(pThis);
    if (it == g_splitMenus.end() || it->second == nullptr) return;

    HWND hWnd = pThis->GetSafeHwnd();
    if (!hWnd) return;

    // MFC tracks the first submenu when the resource is a menu bar.
    HMENU hSub = ::GetSubMenu(it->second, 0);
    HMENU hTrack = hSub ? hSub : it->second;

    RECT rc{};
    ::GetWindowRect(hWnd, &rc);
    ::TrackPopupMenu(hTrack, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                     rc.left, rc.bottom, 0, ::GetParent(hWnd), nullptr);
}
// Symbol: ?Cleanup@CSplitButton@@IEAAXXZ
extern "C" void MS_ABI impl__Cleanup_CSplitButton__IEAAXXZ(CSplitButton* pThis) {
    if (!pThis) return;
    g_splitMenus.erase(pThis);
}
// Symbol: ??1CSplitButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CSplitButton__UEAA_XZ(CSplitButton* pThis) {
    if (pThis) pThis->~CSplitButton();
}
int CSplitButton::Create(const wchar_t* lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID) {
    return CButton::Create(lpszCaption, dwStyle | BS_SPLITBUTTON, rect, pParentWnd, nID);
}
CSplitButton::~CSplitButton() {
    g_splitMenus.erase(this);
}
void CSplitButton::SetDropDownMenu(unsigned int nMenuId, unsigned int) {
    g_splitMenus[this] = ::LoadMenuW(::GetModuleHandleW(nullptr), MAKEINTRESOURCEW(nMenuId));
}
void CSplitButton::SetDropDownMenu(CMenu* pMenu) {
    g_splitMenus[this] = pMenu ? pMenu->m_hMenu : nullptr;
}
