// CMFCPopupMenu — OpenMFC implementation.
// Sources: global_cmfcpopupmenu.cpp, mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCPopupMenuSupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Retail (RVA 0x9b820): scrolls the internal menu bar by whole pages/rows
// through the vslot-0x3a0 getter and the bar's scroll machinery.  None of that
// is modeled, so the no-op terminal (0) is returned.
// Symbol: ?AdjustScroll@CMFCPopupMenu@@IEAAHH@Z
extern "C" int MS_ABI impl__AdjustScroll_CMFCPopupMenu__IEAAHH_Z(
    void* pThis, int nScrollDir)
{
    (void)nScrollDir;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- menu-bar scroll machinery is
    // not modeled.
    return 0;
}
// Retail (RVA 0x9b9b80): computes the client rect in screen coordinates and
// classifies the point as MENUAREA_OUTSIDE / logo / menu areas using
// m_bShowLogo@0x1680, m_nLogoLocation@0x1688 and PtInRect tests.  The logo
// geometry is not modeled, so 0 is returned conservatively.
// Symbol: ?CheckArea@CMFCPopupMenu@@QEBA?AW4MENUAREA_TYPE@1@AEBVCPoint@@@Z
extern "C" int MS_ABI impl__CheckArea_CMFCPopupMenu__QEBA_AW4MENUAREA_TYPE_1_AEBVCPoint___Z(
    const void* pThis, const CPoint& ptScreen)
{
    (void)ptScreen;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- client-rect classification and
    // the logo-area geometry are not modeled.
    return 0;
}
// Retail (RVA 0x9b80f0): walks the menu bar's button list and closes every
// delayed sub-menu through the per-button vslot-0xc0.  The list/vslot are not
// modeled.
// Symbol: ?CollapseSubmenus@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__CollapseSubmenus_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- button-list walk and the
    // vslot-0xc0 close are not modeled.
}
// Retail (RVA 0xba230): news a CMFCToolBar, creates it as a 0x50402808-style
// child and turns it into a tear-off bar.  The toolbar creation chain is not
// modeled, so nullptr is returned.
// Symbol: ?CreateTearOffBar@CMFCPopupMenu@@MEAAPEAVCPane@@PEAVCFrameWnd@@IPEB_W@Z
extern "C" CPane* MS_ABI impl__CreateTearOffBar_CMFCPopupMenu__MEAAPEAVCPane__PEAVCFrameWnd__IPEB_W_Z(
    void* pThis, CFrameWnd* pFrame, unsigned int uiID, const wchar_t* lpszWindowName)
{
    (void)pFrame; (void)uiID; (void)lpszWindowName;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially -- tear-off bar construction is
    // not modeled.
    return nullptr;
}
// Retail (RVA 0xb8b20): draws the fade animation frames / menu shadow through
// the CDC and CMenuImages machinery.  Not modeled.
// Symbol: ?DoPaint@CMFCPopupMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    (void)pDC;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- drawing is not modeled.
}
// Retail (RVA 0xb91a0): draws the fade-in animation frame list.  Not modeled.
// Symbol: ?DrawFade@CMFCPopupMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawFade_CMFCPopupMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    (void)pDC;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- fade animation is not modeled.
}
// Retail (RVA 0xb97f0):
//     +0x1680 = bShow;
//     +0x1688 = location;
//     return vslot0x300(this, TRUE);   // relayout tail call
// The two stores are reproduced; the vslot-0x300 relayout is not modeled.
// Symbol: ?EnableMenuLogo@CMFCPopupMenu@@QEAAXHW4LOGO_LOCATION@1@@Z
extern "C" void MS_ABI impl__EnableMenuLogo_CMFCPopupMenu__QEAAXHW4LOGO_LOCATION_1__Z(
    void* pThis, int bShow, int location)
{
    if (!pThis) return;
    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    p->m_bShowLogo = bShow;
    p->m_nLogoLocation = location;
    // TODO(clean-room): transcribed partially -- the vslot-0x300 relayout call
    // is not modeled.
}
// Retail (RVA 0xbb990): walks the chain rooted at two file globals
// (0x1803be2a8 / 0x1803be288), returning the popup whose +0x19c0 floaty
// pointer is set.  The globals are not in the harvested static set, so nullptr
// is returned.
// Symbol: ?FindMenuWithConnectedFloaty@CMFCPopupMenu@@KAPEAV1@XZ
extern "C" void* MS_ABI impl__FindMenuWithConnectedFloaty_CMFCPopupMenu__KAPEAV1_XZ()
{
    // TODO(clean-room): transcribed partially -- the two file globals and the
    // +0x19c0 floaty walk are not modeled.
    return nullptr;
}
// Retail (RVA 0xbb420): returns the border size obtained through the app
// getter 0x180009774 and its vtable-0x5f8 virtual.  Neither is modeled, so 0
// (a neutral border size) is returned.
// Symbol: ?GetBorderSize@CMFCPopupMenu@@MEBAHXZ
extern "C" int MS_ABI impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(const void* pThis)
{
    (void)pThis;
    // TODO(clean-room): transcribed partially -- the app-getter/virtual chain
    // (0x180009774 + vtable 0x5f8) is not modeled.
    return 0;
}
// Retail (RVA 0xbb3a0): when a parent button exists (+0x228), copies the
// button's rect (button+0x68) into rectArea and returns the button's parent
// window (button+0x80 = m_pWndParent); otherwise it falls back to the internal
// menu bar (+0x19b8) through the vslot-0x158 getter and copies its +0xc8 rect.
// The first branch is transcribed; the menu-bar branch needs an unmodeled
// vtable slot.
// Symbol: ?GetParentArea@CMFCPopupMenu@@UEAAPEAVCWnd@@AEAVCRect@@@Z
extern "C" CWnd* MS_ABI impl__GetParentArea_CMFCPopupMenu__UEAAPEAVCWnd__AEAVCRect___Z(
    void* pThis, CRect& rectArea)
{
    if (!pThis) return nullptr;
    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    if (p->m_pParentButton) {
        CWnd* pParentWnd = *reinterpret_cast<CWnd**>(
            static_cast<char*>(p->m_pParentButton) + 0x80);
        if (pParentWnd) {
            rectArea = *reinterpret_cast<CRect*>(
                static_cast<char*>(p->m_pParentButton) + 0x68);
            return pParentWnd;
        }
        return nullptr;
    }
    // TODO(clean-room): transcribed partially -- retail falls back to the
    // internal menu bar (+0x19b8) via the vslot-0x158 getter and copies its
    // +0xc8 rect; not modeled.
    return nullptr;
}
// Retail (RVA 0xb7550): walks from the parent button (+0x228 -> +0x80),
// IsKindOf checks against the opaque runtime classes 0x1803b1478 and
// 0x1803b1448 and a parent helper (0x18006bd00).  The classes are opaque and
// the helper is unmodeled, so nullptr is returned.
// Symbol: ?GetParentPopupMenu@CMFCPopupMenu@@QEBAPEAV1@XZ
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(
    const void* pThis)
{
    (void)pThis;
    // TODO(clean-room): transcribed partially -- the parent-button walk and the
    // RTTI checks against 0x1803b1478/0x1803b1448 are not modeled.
    return nullptr;
}
// Retail (RVA 0xb75f0): walks from the parent button (+0x228 -> +0x80) and
// IsKindOf checks it against the opaque runtime class 0x1803b15f8 (the toolbar
// type).  The class is unidentifiable (see global_cmfccustomizebutton.cpp), so
// nullptr is returned.
// Symbol: ?GetParentToolBar@CMFCPopupMenu@@QEBAPEAVCMFCToolBar@@XZ
extern "C" CMFCToolBar* MS_ABI impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(
    const void* pThis)
{
    (void)pThis;
    // TODO(clean-room): transcribed partially -- the parent-button walk and the
    // IsKindOf check against 0x1803b15f8 are not modeled.
    return nullptr;
}
// Retail (RVA 0xb8320): imports the menu from m_pMenu@+0x1658 into the
// internal CMFCPopupMenuBar, installs the button list and finishes with a
// vslot-0x300 relayout; returns TRUE on success and FALSE on failure.  The
// menu-bar machinery is not modeled, so the retail success value (TRUE) is
// returned.
// Symbol: ?InitMenuBar@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__InitMenuBar_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- menu import into the internal
    // CMFCPopupMenuBar (+0x19b8) is not modeled; the retail success value is
    // returned.
    return TRUE;
}
// Retail (RVA 0xb4fa0): applies the class-wide defaults.  Every constant store
// below is transcribed 1:1.  The +0x48 flag, the +0x169c/+0x16d0 fields and
// the 0x1803be290 global depend on unmodeled helpers (0x18002e600,
// 0x180009774 + vtable 0x5f8, CMenuImages::Size 0x18008f600) and globals
// (0x1803c1898, 0x1803c187c, 0x1803b1b00, 0x1803c18a8 OS version), so they
// keep their neutral defaults.
// Symbol: ?Initialize@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    char* const b = static_cast<char*>(pThis);

    *reinterpret_cast<std::uint64_t*>(b + 0x1658) = 0;  // menu handle
    *reinterpret_cast<std::uint64_t*>(b + 0x200)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x208)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x228)  = 0;  // parent button
    *reinterpret_cast<std::int32_t*>(b + 0x1f0)   = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1660)  = 1;
    *reinterpret_cast<std::uint64_t*>(b + 0x210)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x168c) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1678)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x16a0) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x16a8)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1670) = 0;  // fade x/y pair
    *reinterpret_cast<std::int32_t*>(b + 0x16d4)  = -1;
    *reinterpret_cast<std::int32_t*>(b + 0x167c)  = -1;
    *reinterpret_cast<std::int32_t*>(b + 0x1680)  = 0;  // logo show
    *reinterpret_cast<std::int32_t*>(b + 0x1688)  = 0;  // logo location
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x16ac));
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x16bc));
    *reinterpret_cast<std::int32_t*>(b + 0x16cc)  = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1694)  = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1698)  = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1810)  = 0;  // menu border size
    *reinterpret_cast<std::int32_t*>(b + 0x16d8)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1840) = 0;
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x1848));
    *reinterpret_cast<std::int32_t*>(b + 0x1650)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1f8)  = 0;  // owner window
    *reinterpret_cast<std::int32_t*>(b + 0x1664)  = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x166c)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x19a8) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x19b0)  = 2;
    *reinterpret_cast<std::uint64_t*>(b + 0x19b8) = 0;  // menu bar
    *reinterpret_cast<std::uint64_t*>(b + 0x19c0) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1684)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1984) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x198c)  = 0;
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x1998));
    *reinterpret_cast<std::uint64_t*>(b + 0x1978) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1980)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1990) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1668)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1838) = 0;

    // TODO(clean-room): transcribed partially -- the +0x48 flag, the
    // +0x169c/+0x16d0 fields and the 0x1803be290 global depend on unmodeled
    // helpers/globals and keep their neutral defaults.
}
// Retail (RVA 0xb8040): asks the menu bar (vslot 0x3a0) for its client rect
// and compares the last item's bottom against the bar height + the fade offset.
// The vslot getter is not modeled, so FALSE is returned conservatively.
// Symbol: ?IsScrollDnAvailable@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__IsScrollDnAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially -- vslot-0x3a0 menu-bar getter
    // and the item-rect comparison are not modeled.
    return FALSE;
}
// Retail (RVA 0xb8010): asks the menu bar (vslot 0x3a0) and returns whether its
// +0x1364 scroll offset is positive.  The vslot getter is not modeled, so FALSE
// is returned conservatively.
// Symbol: ?IsScrollUpAvailable@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__IsScrollUpAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially -- vslot-0x3a0 menu-bar getter
    // and the +0x1364 scroll-offset read are not modeled.
    return FALSE;
}
// Retail (RVA 0xba7f0): when the owner (+0x1f8) is a CDialog/CFrameWnd and the
// global torn-off-menu pointer (0x1803be288) matches `this`, sends the
// notification to the dialog's +0x158 member.  The RTTI classes and the global
// are not modeled, so 0 is returned.
// Symbol: ?NotifyParentDlg@CMFCPopupMenu@@IEAAHH@Z
extern "C" int MS_ABI impl__NotifyParentDlg_CMFCPopupMenu__IEAAHH_Z(
    void* pThis, int bMsgWhat)
{
    (void)bMsgWhat;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the CDialog/CFrameWnd RTTI
    // checks and the 0x1803be288 global compare are not modeled.
    return 0;
}
// Retail (RVA 0xb7b10):
//     if (bActive) return;
//     if (m_bForceMenuFocus) return;
//     if (vslot0x3a8(this)) return;            // sub-menu open check
//     if (+0x1664 != 0) +0x1674 = 1;
//     SendMessageW(m_hWnd@+0x40, WM_CLOSE, 0, 0);
// Symbol: ?OnActivateApp@CMFCPopupMenu@@IEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCPopupMenu__IEAAXHK_Z(
    void* pThis, int bActive, unsigned long dwThreadID)
{
    (void)dwThreadID;
    if (!pThis) return;
    if (bActive != 0) return;
    if (impl__m_bForceMenuFocus_CMFCPopupMenu__1HA != 0) return;

    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    // TODO(clean-room): transcribed partially -- retail first consults the
    // vslot-0x3a8 menu-bar getter and bails when it reports an open sub-menu.
    if (p->m_bDialogParent != 0)
        p->m_nFadeY = 1;   // retail +0x1674 = 1
    if (p->m_hWnd != nullptr)
        ::SendMessageW(p->m_hWnd, WM_CLOSE, 0, 0);
}
// Retail (RVA 0xb9740):
//     if (CFrameWnd::OnCmdMsg(this, nID, nCode, pExtra, pHandlerInfo))
//         return TRUE;
//     pParent = this->+0x1f8;
//     if (pParent) return pParent->vtable[0x28](pParent, nID, nCode, pExtra,
//                                               pHandlerInfo);  // OnCmdMsg slot 5
//     return FALSE;
// Symbol: ?OnCmdMsg@CMFCPopupMenu@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CMFCPopupMenu__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo)
{
    if (!pThis) return 0;

    if (impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }

    void* pParent = *reinterpret_cast<void**>(static_cast<char*>(pThis) + 0x1f8);
    if (pParent) {
        void** vtbl = *reinterpret_cast<void***>(pParent);
        OnCmdMsgFn fn = reinterpret_cast<OnCmdMsgFn>(vtbl[VT_ONCMDMSG_SLOT]);
        return fn(pParent, nID, nCode, pExtra, pHandlerInfo);
    }
    return FALSE;
}
// Retail (RVA 0xb5790): asserts a non-null create struct, runs
// CFrameWnd::OnCreateHelper and returns -1 when that fails; otherwise it builds
// the internal CMFCPopupMenuBar (+0x19b8), transfers the button list and the
// menu style/geometry, and returns 0.  The helper call is modeled; the menu-bar
// construction is not, so 0 (the retail success value) is returned.
// Symbol: ?OnCreate@CMFCPopupMenu@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (!pThis) return -1;
    if (!lpCreateStruct) return -1;   // retail asserts here; fail safely

    int nRet = impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
        reinterpret_cast<CFrameWnd*>(pThis), lpCreateStruct, nullptr);
    if (nRet == -1) return -1;

    // TODO(clean-room): transcribed partially -- the internal CMFCPopupMenuBar
    // construction, button-list transfer and geometry setup are not modeled.
    return 0;
}
// Retail (RVA 0xb6b60): collapses open sub-menus, walks the window's frame
// chain to detach the owner-dialog hook, releases the menu-bar drag button and
// clears the internal bar references.  The frame/list machinery is not modeled.
// Symbol: ?OnDestroy@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- sub-menu collapse, the
    // owner-dialog detach and the menu-bar cleanup are not modeled.
}
// Symbol: ?ActivatePopupMenu@CMFCPopupMenu@@SAHPEAVCFrameWnd@@PEAV1@@Z
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu) {
    return CMFCPopupMenu::ActivatePopupMenu(pTopFrame, pPopupMenu);
}
// Symbol: ?GetSafeActivePopupMenu@CMFCPopupMenu@@SAPEAV1@XZ
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() {
    return CMFCPopupMenu::GetSafeActivePopupMenu();
}
// Symbol: ?GetMenuItemCount@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuItemCount_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetMenuItemCount() : 0;
}
// Symbol: ?GetMenuItem@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@H@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetMenuItem_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__H_Z(const CMFCPopupMenu* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}
// Symbol: ?GetSelItem@CMFCPopupMenu@@QEAAPEAVCMFCToolBarMenuButton@@XZ
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetSelItem_CMFCPopupMenu__QEAAPEAVCMFCToolBarMenuButton__XZ(CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetSelItem() : nullptr;
}
// Symbol: ?FindSubItemByCommand@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@I@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__FindSubItemByCommand_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__I_Z(const CMFCPopupMenu* pThis, unsigned int uiCmd) {
    return pThis ? pThis->FindSubItemByCommand(uiCmd) : nullptr;
}
// Symbol: ?InsertItem@CMFCPopupMenu@@QEAAHAEBVCMFCToolBarMenuButton@@H@Z
extern "C" int MS_ABI impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(CMFCPopupMenu* pThis, const CMFCToolBarMenuButton* pButton, int iInsertAt) {
    return (pThis && pButton) ? pThis->InsertItem(*pButton, iInsertAt) : FALSE;
}
// Symbol: ?InsertSeparator@CMFCPopupMenu@@QEAAHH@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCPopupMenu__QEAAHH_Z(CMFCPopupMenu* pThis, int iInsertAt) {
    return pThis ? pThis->InsertSeparator(iInsertAt) : FALSE;
}
// Symbol: ?CloseMenu@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__CloseMenu_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bSetFocusToBar) {
    if (pThis) pThis->CloseMenu(bSetFocusToBar);
}
// Symbol: ?EnableResize@CMFCPopupMenu@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__EnableResize_CMFCPopupMenu__QEAAXVCSize___Z(CMFCPopupMenu* pThis, CSize sizeMinResize) {
    if (pThis) pThis->EnableResize(sizeMinResize);
}
// Symbol: ?EnableVertResize@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableVertResize_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bEnable) {
    if (pThis) pThis->EnableVertResize(bEnable);
}
// Symbol: ?HideRarelyUsedCommands@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__HideRarelyUsedCommands_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->HideRarelyUsedCommands() : FALSE;
}
CMFCPopupMenu::CMFCPopupMenu() {
    memset(_mfcpopupmenu_padding, 0, sizeof(_mfcpopupmenu_padding));
}
CMFCPopupMenu::~CMFCPopupMenu() {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
    ClearPopupMenuState(this);
}
CMFCPopupMenu* CMFCPopupMenu::GetActiveMenu() { return g_pActivePopupMenu; }
CMFCPopupMenu* CMFCPopupMenu::GetSafeActivePopupMenu() { return g_pActivePopupMenu; }
void CMFCPopupMenu::SetForceMenuFocus(BOOL bForceFocus) { g_forceMenuFocus = bForceFocus; }
BOOL CMFCPopupMenu::ActivatePopupMenu(CFrameWnd*, CMFCPopupMenu* pPopupMenu) {
    g_pActivePopupMenu = pPopupMenu;
    return pPopupMenu != nullptr;
}
BOOL CMFCPopupMenu::Create(CWnd* pParentWnd, int x, int y, HMENU hMenu, BOOL bLocked, BOOL bIsMainMenu) {
    PopupMenuState& state = g_popupMenuStates[this];
    state.parent = pParentWnd;
    state.x = x;
    state.y = y;
    state.menu = hMenu;
    state.locked = bLocked != FALSE;
    state.mainMenu = bIsMainMenu != FALSE;
    state.selected = -1;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    g_pActivePopupMenu = this;
    return TRUE;
}
void CMFCPopupMenu::CloseMenu(BOOL) {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
}
int CMFCPopupMenu::GetMenuItemCount() const {
    auto it = g_popupMenuStates.find(this);
    return it == g_popupMenuStates.end() ? 0 : static_cast<int>(it->second.items.size());
}
CMFCToolBarMenuButton* CMFCPopupMenu::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
CMFCToolBarMenuButton* CMFCPopupMenu::GetSelItem() {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    return GetMenuItem(it->second.selected);
}
CMFCToolBarMenuButton* CMFCPopupMenu::FindSubItemByCommand(UINT uiCmd) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (item && item->m_nID == uiCmd) return item;
    }
    return nullptr;
}
BOOL CMFCPopupMenu::InsertItem(const CMFCToolBarMenuButton& button, int iInsertAt) {
    PopupMenuState& state = g_popupMenuStates[this];
    CMFCToolBarMenuButton* copy = new CMFCToolBarMenuButton(button);
    if (iInsertAt < 0 || iInsertAt >= static_cast<int>(state.items.size())) {
        state.items.push_back(copy);
    } else {
        state.items.insert(state.items.begin() + iInsertAt, copy);
    }
    state.ownedItems.insert(copy);
    return TRUE;
}
BOOL CMFCPopupMenu::InsertSeparator(int iInsertAt) {
    CMFCToolBarMenuButton separator(0, nullptr, FALSE, nullptr, FALSE);
    return InsertItem(separator, iInsertAt);
}
void CMFCPopupMenu::EnableResize(CSize sizeMinResize) {
    g_popupMenuStates[this].minResize = sizeMinResize;
}
void CMFCPopupMenu::EnableVertResize(BOOL bEnable) {
    g_popupMenuStates[this].vertResize = bEnable != FALSE;
}
BOOL CMFCPopupMenu::HideRarelyUsedCommands() const {
    return FALSE;
}
// Symbol: ??0CMFCPopupMenu@@IEAA@PEAVCMFCToolBarsMenuPropertyPage@@PEB_W@Z
// CMFCPopupMenu::CMFCPopupMenu(CMFCToolBarsMenuPropertyPage*, const wchar_t*)
extern "C" void* MS_ABI impl___0CMFCPopupMenu__IEAA_PEAVCMFCToolBarsMenuPropertyPage__PEB_W_Z(
    void* pPage, const wchar_t* pName) {
    (void)pPage;
    (void)pName;
    return new CMFCPopupMenu();
}
// Symbol: ?get_accChild@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCPopupMenu__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* /*struct*/ p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?get_accChildCount@CMFCPopupMenu@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCPopupMenu__MEAAJPEAJ_Z(long* p0) {
    return 0;
}

// Symbol: ?get_accName@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CMFCPopupMenu__MEAAJUtagVARIANT__PEAPEA_W_Z(void* /*struct*/ p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?get_accParent@CMFCPopupMenu@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCPopupMenu__MEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?get_accRole@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCPopupMenu__MEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?get_accState@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCPopupMenu__MEAAJUtagVARIANT__PEAU2__Z(void* /*struct*/ p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnEraseBkgnd@CMFCPopupMenu@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPopupMenu__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnKeyDown@CMFCPopupMenu@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCPopupMenu__IEAAXIII_Z(unsigned int p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?OnLButtonDown@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPopupMenu__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPopupMenu__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseActivate@CMFCPopupMenu@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMFCPopupMenu__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnMouseMove@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenu__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseWheel@CMFCPopupMenu@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCPopupMenu__IEAAHIFVCPoint___Z(unsigned int p0, short p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnNcHitTest@CMFCPopupMenu@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCPopupMenu__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcLButtonDown@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CMFCPopupMenu__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNotify@CMFCPopupMenu@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCPopupMenu__MEAAH_K_JPEA_J_Z(unsigned __int64 p0, __int64 p1, __int64* p2) {
    return 0;
}

// Symbol: ?OnPaint@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCPopupMenu__IEAAXXZ() {}

// Symbol: ?OnSetCursor@CMFCPopupMenu@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCPopupMenu__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnShowWindow@CMFCPopupMenu@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCPopupMenu__IEAAXHI_Z(int p0, unsigned int p1) {}

// Symbol: ?OnSize@CMFCPopupMenu@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCPopupMenu__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnTimer@CMFCPopupMenu@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCPopupMenu__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnVScroll@CMFCPopupMenu@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CMFCPopupMenu__IEAAXIIPEAVCScrollBar___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {}

// Symbol: ?OnWindowPosChanged@CMFCPopupMenu@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCPopupMenu__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Symbol: ?PostCommand@CMFCPopupMenu@@QEAAHI@Z
extern "C" int MS_ABI impl__PostCommand_CMFCPopupMenu__QEAAHI_Z(unsigned int p0) {
    return 0;
}

// Symbol: ?PostNcDestroy@CMFCPopupMenu@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CMFCPopupMenu__MEAAXXZ() {}

// Symbol: ?PreTranslateMessage@CMFCPopupMenu@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPopupMenu__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcLayout@CMFCPopupMenu@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCPopupMenu__UEAAXH_Z(int p0) {}

// Symbol: ?RemoveAllItems@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCPopupMenu__QEAAXXZ() {}

// Symbol: ?RemoveItem@CMFCPopupMenu@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveItem_CMFCPopupMenu__QEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?SaveState@CMFCPopupMenu@@UEAAXXZ
extern "C" void MS_ABI impl__SaveState_CMFCPopupMenu__UEAAXXZ() {}

// Symbol: ?SetAnimationSpeed@CMFCPopupMenu@@SAXI@Z
extern "C" void MS_ABI impl__SetAnimationSpeed_CMFCPopupMenu__SAXI_Z(unsigned int p0) {}

// Symbol: ?SetMaxWidth@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMaxWidth_CMFCPopupMenu__QEAAXH_Z(int p0) {}

// Symbol: ?SetParentRibbonElement@CMFCPopupMenu@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetParentRibbonElement_CMFCPopupMenu__QEAAXPEAVCMFCRibbonBaseElement___Z(void* /*class*/* p0) {}

// Symbol: ?SetQuickMode@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__SetQuickMode_CMFCPopupMenu__QEAAXXZ() {}

// Symbol: ?SetScrollBar@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__SetScrollBar_CMFCPopupMenu__IEAAXXZ() {}

// Symbol: ?ShowAllCommands@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__ShowAllCommands_CMFCPopupMenu__QEAAXXZ() {}

// Symbol: ?StartResize@CMFCPopupMenu@@IEAAHXZ
extern "C" int MS_ABI impl__StartResize_CMFCPopupMenu__IEAAHXZ() {
    return 0;
}

// Symbol: ?TearOff@CMFCPopupMenu@@MEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__TearOff_CMFCPopupMenu__MEAAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?TriggerResize@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__TriggerResize_CMFCPopupMenu__QEAAXXZ() {}

// Symbol: ?UpdateAllShadows@CMFCPopupMenu@@SAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(void* /*struct*/* p0) {}

// Symbol: ?UpdateBottomWindows@CMFCPopupMenu@@IEAAXH@Z
extern "C" void MS_ABI impl__UpdateBottomWindows_CMFCPopupMenu__IEAAXH_Z(int p0) {}

// Symbol: ?UpdateShadow@CMFCPopupMenu@@QEAAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__UpdateShadow_CMFCPopupMenu__QEAAXPEAUtagRECT___Z(void* /*struct*/* p0) {}
