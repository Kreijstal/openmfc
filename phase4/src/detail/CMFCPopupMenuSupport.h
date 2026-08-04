#pragma once
// Shared internals of the former global_cmfcpopupmenu.cpp translation unit.
// Definitions live in detail/CMFCPopupMenuSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcpopupmenu {} } }
using namespace openmfc::detail::cmfcpopupmenu;
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
namespace openmfc { namespace detail { namespace cmfcpopupmenu {
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
} } }
#pragma pack(pop)

// The parent window's OnCmdMsg virtual lives at vtable slot 5 (offset 0x28)
// for every CWnd-derived MFC type (see global_colecntrframewnd.cpp for the
// same dispatch idiom).
namespace openmfc { namespace detail { namespace cmfcpopupmenu {
typedef int (MS_ABI *OnCmdMsgFn)(void*, unsigned int, int, void*, void*);
} } }

namespace openmfc { namespace detail { namespace cmfcpopupmenu {
enum { VT_ONCMDMSG_SLOT = 5 };
} } }


//=============================================================================
// CMFCPopupMenu
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::AdjustScroll(int nScrollDir)
//=============================================================================

//=============================================================================
// MENUAREA_TYPE CMFCPopupMenu::CheckArea(const CPoint& ptScreen) const
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::CollapseSubmenus()
//=============================================================================

//=============================================================================
// CPane* CMFCPopupMenu::CreateTearOffBar(CFrameWnd* pFrame, UINT uiID,
//                                        const wchar_t* lpszWindowName)
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::DoPaint(CDC* pDC)
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::DrawFade(CDC* pDC)
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::EnableMenuLogo(BOOL bShow, LOGO_LOCATION location)
//=============================================================================

//=============================================================================
// static CMFCPopupMenu* CMFCPopupMenu::FindMenuWithConnectedFloaty()
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::GetBorderSize() const
//=============================================================================

//=============================================================================
// CWnd* CMFCPopupMenu::GetParentArea(CRect& rectArea)
//=============================================================================

//=============================================================================
// const CMFCPopupMenu* CMFCPopupMenu::GetParentPopupMenu() const
//=============================================================================

//=============================================================================
// const CMFCToolBar* CMFCPopupMenu::GetParentToolBar() const
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::InitMenuBar()
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::Initialize()
//=============================================================================

//=============================================================================
// BOOL CMFCPopupMenu::IsScrollDnAvailable()
//=============================================================================

//=============================================================================
// BOOL CMFCPopupMenu::IsScrollUpAvailable()
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::NotifyParentDlg(int bMsgWhat)
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::OnActivateApp(BOOL bActive, unsigned long dwThreadID)
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::OnCmdMsg(UINT nID, int nCode, void* pExtra,
//                             AFX_CMDHANDLERINFO* pHandlerInfo)
//=============================================================================

//=============================================================================
// int CMFCPopupMenu::OnCreate(CREATESTRUCTW* lpCreateStruct)
//=============================================================================

//=============================================================================
// void CMFCPopupMenu::OnDestroy()
//=============================================================================
