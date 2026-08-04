#pragma once
// Shared internals of the former global_mfc-feature-20_impl.cpp translation unit.
// Definitions live in detail/CMFCTasksPaneSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctaskspane {} } }
using namespace openmfc::detail::cmfctaskspane;
// OpenMFC: real implementation of the CMFCTasksPane handlers exported as the
// wave1_mfc-feature-20 batch.
//
// The retail CMFCTasksPane (afxtaskspane.h) is a CDockablePane-derived
// control whose own members start at offset 0x4E0 (the CWnd + CBasePane +
// CPane + CDockablePane subobjects fill 0..0x4DF). OpenMFC's CMFCTasksPane
// carries only opaque padding, so every member access below goes through a
// layout-faithful mirror struct whose offsets were harvested from the
// shipping MSVC atlmfc headers (/d1reportSingleClassLayoutCMFCTasksPane)
// and cross-checked byte-for-byte against the retail mfc140u disassembly
// (the RVA column in each comment refers to the retail export).
//
// The small navigation/font/accessibility handlers (OnEraseBkgnd, OnGetFont,
// OnSetFont, OnPressBackButton/Forward/Home, OnPressButtons, OnSetAccData,
// OnSize, OnSettingChange) are transcribed faithfully. Anything that walks
// the task-group/task object lists (CObList at 0x608 / 0x640), the vertical
// scroll-bar sub-window (0x680), the icon list or the history stack deep
// enough to need object layout OpenMFC does not model gets a type-correct
// conservative body with a TODO(clean-room) note, exactly like the earlier
// feature batches. Internal MFC globals (the last-navigation index at
// 0x1803b1b3c, the animation state at 0x1803be288, the visual-manager
// initialized flag at 0x1803c1620) are never touched.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of CMFCTasksPane. The base subobjects are opaque
// (CWnd m_hWnd at +0x40 is still readable through the typed CWnd base of
// CMFCTasksPane, and CBasePane::m_AccData at +0x150 is a declared member);
// only the pane's own members are mirrored here.
namespace openmfc { namespace detail { namespace cmfctaskspane {
struct S_MfcFeature20Impl {
    char  _base[0x4E0];                // 0x000  CWnd + CBasePane + CPane + CDockablePane

    BOOL  m_bCanCollapse;              // 0x4E0
    BOOL  m_bUseNavigationToolbar;     // 0x4E4
    BOOL  m_bHistoryMenuButtons;       // 0x4E8
    BOOL  m_bUseScrollButtons;         // 0x4EC
    BOOL  m_bAnimationEnabled;         // 0x4F0
    BOOL  m_bOffsetCustomControls;     // 0x4F4
    BOOL  m_bMenuBtnPressed;           // 0x4F8
    BOOL  m_bWrapTasks;                // 0x4FC
    BOOL  m_bWrapLabels;               // 0x500
    int   m_iActivePage;               // 0x504
    int   m_iScrollMode;               // 0x508
    int   m_iScrollBtnHeight;          // 0x50C
    int   m_nVertScrollOffset;         // 0x510
    int   m_nVertScrollTotal;          // 0x514
    int   m_nVertScrollPage;           // 0x518
    int   m_nRowHeight;                // 0x51C
    int   m_nAnimGroupExtraHeight;     // 0x520
    int   m_nVertMargin;               // 0x524
    int   m_nHorzMargin;               // 0x528
    int   m_nGroupVertOffset;          // 0x52C
    int   m_nGroupCaptionHeight;       // 0x530
    int   m_nGroupCaptionHorzOffset;   // 0x534
    int   m_nGroupCaptionVertOffset;   // 0x538
    int   m_nTasksHorzOffset;          // 0x53C
    int   m_nTasksIconHorzOffset;      // 0x540
    int   m_nTasksIconVertOffset;      // 0x544
    int   m_nMaxHistory;               // 0x548  (const int)
    UINT  m_uiToolbarBmpRes;           // 0x54C
    HFONT m_hFont;                     // 0x550
    char  m_fonts[0x30];               // 0x558  m_fontBold / m_fontBoldUnderline / m_fontUnderline (CFont, 16 each; HFONT m_hObject at +8)
    SIZE  m_sizeIcon;                  // 0x588
    SIZE  m_sizeToolbarImage;          // 0x590
    SIZE  m_sizeToolbarButton;         // 0x598
    SIZE  m_sizeAnim;                  // 0x5A0
    RECT  m_rectToolbar;               // 0x5A8
    RECT  m_rectScrollUp;              // 0x5B8
    RECT  m_rectScrollDn;              // 0x5C8
    RECT  m_rectTasks;                 // 0x5D8
    char  m_menuOther[0x10];           // 0x5E8  CMenu (HMENU m_hMenu at +8 = 0x5F0)
    void* m_pHotTask;                  // 0x5F8
    void* m_pClickedTask;              // 0x600
    char  m_lstTasksPanes[0x38];       // 0x608  CObList (head at +8 = 0x610, count at +0x18 = 0x620)
    char  m_lstTaskGroups[0x38];       // 0x640  CObList (head at +8 = 0x648, count at +0x18 = 0x658)
    wchar_t* m_strCaption;             // 0x678  CString -> single LPWSTR data pointer
    char  m_wndScrollVert[0xE8];       // 0x680  vertical scroll sub-window blob (232 bytes)
    char  m_lstIcons[0x10];            // 0x768
    void* m_arrHistoryVfptr;           // 0x778  CArray<int,int> : CObject (vtable ptr)
    const int* m_pHistoryData;         // 0x780  m_pData
    __int64    m_nHistorySize;         // 0x788  m_nSize (INT_PTR)
    __int64    m_nHistoryMaxSize;      // 0x790  m_nMaxSize
    __int64    m_nHistoryGrowBy;       // 0x798  m_nGrowBy
    void* m_pHotGroupCaption;          // 0x7A0
    void* m_pClickedGroupCaption;      // 0x7A8
    void* m_pAnimatedGroup;            // 0x7B0
    char  m_wndToolBar[0x48];          // 0x7B8  toolbar sub-window blob
};
} } }


// Refresh idiom used by the faithful bodies below: the retail handlers call
// internal layout helpers (0x148990 / 0x148200) that are not exported, then
// either the virtual RecalcLayout (vtable slot 0x7d0) or a redraw with
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW (0x105). RedrawWindow on the whole
// pane is the safe equivalent partial behaviour (see feature-6 house style).
namespace openmfc { namespace detail { namespace cmfctaskspane {
void RefreshPane(CMFCTasksPane* pThis);
} } }


// CMFCTasksPane::OnPressHomeButton -- forward-declared so OnHome (which
// tail-calls the virtual slot 0x760 that points at it) can dispatch to it.
extern "C" void MS_ABI impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis);

//=============================================================================
// Navigation buttons
//=============================================================================









//=============================================================================
// Font / text / accessibility
//=============================================================================






//=============================================================================
// Mouse / cursor
//=============================================================================







//=============================================================================
// Drawing
//=============================================================================




//=============================================================================
// Window lifecycle / misc messages
//=============================================================================








