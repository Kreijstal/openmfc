// OpenMFC: CMFCPopupMenu exports.
//
// All 21 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// CMFCPopupMenu is declared in afxmfc.h but all of its members live in opaque
// padding, so `this` is taken as void* and the layout is mirrored with the
// file-local struct below.  The class derives from CMiniFrameWnd and is 6600
// bytes (0x19c8).
//
// The retail layout is 4-byte packed (as the MFC headers are).  That is
// required by the harvested offsets: retail Initialize stores 64-bit zeros at
// 0x168c and 0x16a0 (8-byte fields NOT on 8-byte boundaries) and AdjustScroll
// reads 32-bit values at 0x16a4/0x16a8 and addresses the parent / screen rects
// at 0x16ac/0x16bc.  Without pack(4) the compiler would push m_pSelItem to
// 0x1690 and inflate the class to 0x19d8.
//
// Faithfully transcribed: EnableMenuLogo, OnActivateApp, OnCmdMsg, OnCreate and
// the constant part of Initialize.  The remaining bodies depend on unmodeled
// vtable slots (0x3a0 GetMenuBar, 0x300 relayout, 0x730 hit-test, 0x158
// getter), opaque runtime classes or heavy menu-bar machinery; they are
// type-correct safe defaults with the retail return value where it is
// observable, marked `// TODO(clean-room): transcribed partially`.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CFrameWnd::OnCmdMsg (frame_font_exports.cpp / global_core-frames-1_impl.cpp);
// the sibling declaration uses void* for the trailing pointers, which is the
// same MS x64 ABI as the AFX_CMDHANDLERINFO* definition.
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
// CFrameWnd::OnCreateHelper (thunks.cpp -> wincore.cpp member).
extern "C" int MS_ABI impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pCreateStruct, CCreateContext* pContext);
// CMFCPopupMenu::m_bForceMenuFocus class static (feature_static_data.cpp,
// retail 0x1803be35c).
extern "C" std::int32_t impl__m_bForceMenuFocus_CMFCPopupMenu__1HA;

namespace {

//---------------------------------------------------------------------------
// Layout mirrors (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCPopupMenu, sizeof 6600 (0x19c8).  The first 0x40 bytes are the
// CObject/CWnd block (m_hWnd at 0x40); CMiniFrameWnd/CFrameWnd add the 0x48..
// 0x1ef region.  Only the fields this batch touches are named; names without a
// documented retail member are descriptive best-effort guesses (the class is
// opaque in the public header).
//
// The retail layout is 4-byte packed (as the MFC headers are).  This is
// required by the harvested offsets: retail Initialize stores 64-bit zeros at
// 0x168c and 0x16a0 (8-byte fields NOT on 8-byte boundaries) and AdjustScroll
// reads 32-bit values at 0x16a4/0x16a8 and addresses the parent/screen rects at
// 0x16ac/0x16bc.  Without pack(4) the compiler would push m_pSelItem to 0x1690
// and inflate the class to 0x19d8.
#pragma pack(push, 4)
struct PopupMenu
{
    void*  vfptr;                    // 0x00 CObject vfptr
    char   _pad0[0x40 - 0x08];       // 0x08..0x3f
    HWND   m_hWnd;                   // 0x40 CWnd::m_hWnd
    char   _pad1[0x1f0 - 0x48];      // 0x48..0x1ef
    int    m_bInited;                // 0x1f0 Initialize -> 1
    char   _pad1a[0x1f8 - 0x1f4];    // 0x1f4..0x1f7
    void*  m_pOwnerWnd;              // 0x1f8 owner window (OnCmdMsg forward)
    char   _pad2[0x228 - 0x200];     // 0x200..0x227
    void*  m_pParentButton;          // 0x228 parent CMFCToolBarButton*
    char   _pad3[0x1658 - 0x230];    // 0x230..0x1657
    void*  m_pMenu;                  // 0x1658
    int    m_bLocked;                // 0x1660
    int    m_bDialogParent;          // 0x1664 (OnActivateApp / NotifyParentDlg)
    int    m_bRecalcLayout;          // 0x1668
    int    m_bIsSubMenu;             // 0x166c
    int    m_nFadeX;                 // 0x1670
    int    m_nFadeY;                 // 0x1674 (OnActivateApp -> 1)
    int    m_nFadeStart;             // 0x1678
    int    m_nFadeStep;              // 0x167c (Initialize -> -1)
    int    m_bShowLogo;              // 0x1680 (EnableMenuLogo)
    int    m_bForceRarelyUsed;       // 0x1684
    int    m_nLogoLocation;          // 0x1688 (LOGO_LOCATION)
    void*  m_pSelItem;               // 0x168c
    int    m_nGutter;                // 0x1694
    int    m_nShadowSize;            // 0x1698
    int    m_bMenuSound;             // 0x169c
    void*  m_pAnimMgr;               // 0x16a0
    int    m_nAnimType;              // 0x16a8
    RECT   m_rcParent;               // 0x16ac
    RECT   m_rcScreen;               // 0x16bc
    int    m_nLogoHeight;            // 0x16cc
    int    m_nBorderSize;            // 0x16d0
    int    m_nMinWidth;              // 0x16d4
    int    m_nMaxHeight;             // 0x16d8
    char   _pad4[0x1810 - 0x16dc];   // 0x16dc..0x180f
    int    m_nMenuBorder;            // 0x1810
    char   _pad5[0x1838 - 0x1814];   // 0x1814..0x1837
    void*  m_pMenuBarFrame;          // 0x1838
    void*  m_pToolTip;               // 0x1840
    RECT   m_rcLastMenuBar;          // 0x1848
    char   _pad6[0x1978 - 0x1858];   // 0x1858..0x1977
    void*  m_pDelayedPopup;          // 0x1978
    int    m_bCustomizing;           // 0x1980
    void*  m_pFadeBmp;               // 0x1984
    int    m_nFadeOpacity;           // 0x198c
    void*  m_pScroll;                // 0x1990
    RECT   m_rcScroll;               // 0x1998
    void*  m_pMenuBarList;           // 0x19a8
    int    m_nAnimState;             // 0x19b0
    char   _pad7[0x19b8 - 0x19b4];   // 0x19b4..0x19b7
    void*  m_pMenuBar;               // 0x19b8 internal CMFCPopupMenuBar*
    void*  m_pParentFloaty;          // 0x19c0
};
static_assert(sizeof(PopupMenu) == 0x19c8, "CMFCPopupMenu size mismatch");
#pragma pack(pop)

// The parent window's OnCmdMsg virtual lives at vtable slot 5 (offset 0x28)
// for every CWnd-derived MFC type (see global_colecntrframewnd.cpp for the
// same dispatch idiom).
typedef int (MS_ABI *OnCmdMsgFn)(void*, unsigned int, int, void*, void*);

enum { VT_ONCMDMSG_SLOT = 5 };

} // namespace

//=============================================================================
// CMFCPopupMenu
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::AdjustScroll(int nScrollDir)
//=============================================================================
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

//=============================================================================
// MENUAREA_TYPE CMFCPopupMenu::CheckArea(const CPoint& ptScreen) const
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::CollapseSubmenus()
//=============================================================================
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

//=============================================================================
// CPane* CMFCPopupMenu::CreateTearOffBar(CFrameWnd* pFrame, UINT uiID,
//                                        const wchar_t* lpszWindowName)
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::DoPaint(CDC* pDC)
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::DrawFade(CDC* pDC)
//=============================================================================
// Retail (RVA 0xb91a0): draws the fade-in animation frame list.  Not modeled.
// Symbol: ?DrawFade@CMFCPopupMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawFade_CMFCPopupMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    (void)pDC;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- fade animation is not modeled.
}

//=============================================================================
// void CMFCPopupMenu::EnableMenuLogo(BOOL bShow, LOGO_LOCATION location)
//=============================================================================
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

//=============================================================================
// static CMFCPopupMenu* CMFCPopupMenu::FindMenuWithConnectedFloaty()
//=============================================================================
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

//=============================================================================
// int CMFCPopupMenu::GetBorderSize() const
//=============================================================================
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

//=============================================================================
// CWnd* CMFCPopupMenu::GetParentArea(CRect& rectArea)
//=============================================================================
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

//=============================================================================
// const CMFCPopupMenu* CMFCPopupMenu::GetParentPopupMenu() const
//=============================================================================
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

//=============================================================================
// const CMFCToolBar* CMFCPopupMenu::GetParentToolBar() const
//=============================================================================
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

//=============================================================================
// int CMFCPopupMenu::InitMenuBar()
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::Initialize()
//=============================================================================
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

//=============================================================================
// BOOL CMFCPopupMenu::IsScrollDnAvailable()
//=============================================================================
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

//=============================================================================
// BOOL CMFCPopupMenu::IsScrollUpAvailable()
//=============================================================================
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

//=============================================================================
// int CMFCPopupMenu::NotifyParentDlg(int bMsgWhat)
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::OnActivateApp(BOOL bActive, unsigned long dwThreadID)
//=============================================================================
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

//=============================================================================
// int CMFCPopupMenu::OnCmdMsg(UINT nID, int nCode, void* pExtra,
//                             AFX_CMDHANDLERINFO* pHandlerInfo)
//=============================================================================
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

//=============================================================================
// int CMFCPopupMenu::OnCreate(CREATESTRUCTW* lpCreateStruct)
//=============================================================================
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

//=============================================================================
// void CMFCPopupMenu::OnDestroy()
//=============================================================================
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
