// CMFCDropDownListBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMFCPopupMenuSupport.h"
#include "detail/CMenuSupport.h"

#include <cstddef>
#include <cstdint>

// CMFCDropDownListBox is not declared in include/openmfc.  The retail class is a
// 0x19c8-byte CMFCPopupMenu followed by CMenu, two ints and two pointers.
struct DropDownListBoxShadow {
    unsigned char popupMenu[0x19c8];
    unsigned char menuStorage[0x10];
    int nCurSel;
    int nMinWidth;
    void* pEditCtrl;
    void* pRibbonCombo;
};
static_assert(sizeof(openmfc::detail::cmfcpopupmenu::PopupMenu) == 0x19c8,
              "CMFCPopupMenu base size");
static_assert(offsetof(openmfc::detail::cmfcpopupmenu::PopupMenu, m_hWnd) == 0x40,
              "CMFCPopupMenu::m_hWnd");
static_assert(offsetof(openmfc::detail::cmfcpopupmenu::PopupMenu, m_bLocked) == 0x1660,
              "CMFCPopupMenu::m_bLocked");
static_assert(offsetof(openmfc::detail::cmfcpopupmenu::PopupMenu, m_pAnimMgr) == 0x16a0,
              "CMFCPopupMenu pointer-sized slot at +0x16a0");
static_assert(offsetof(openmfc::detail::cmfcpopupmenu::PopupMenu, m_nAnimType) == 0x16a8,
              "CMFCPopupMenu animation type");
static_assert(offsetof(openmfc::detail::cmfcpopupmenu::PopupMenu, m_nMinWidth) == 0x16d4,
              "CMFCPopupMenu::m_nMinWidth");
static_assert(sizeof(CMenu) == 0x10, "CMenu member size");
static_assert(offsetof(CMenu, m_hMenu) == 0x08, "CMenu::m_hMenu");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(sizeof(CSize) == 0x08, "CSize by-value ABI size");
static_assert(offsetof(DropDownListBoxShadow, menuStorage) == 0x19c8,
              "CMFCDropDownListBox::m_Menu");
static_assert(offsetof(DropDownListBoxShadow, nCurSel) == 0x19d8,
              "CMFCDropDownListBox::m_nCurSel");
static_assert(offsetof(DropDownListBoxShadow, nMinWidth) == 0x19dc,
              "CMFCDropDownListBox::m_nMinWidth");
static_assert(offsetof(DropDownListBoxShadow, pEditCtrl) == 0x19e0,
              "CMFCDropDownListBox::m_pEditCtrl");
static_assert(offsetof(DropDownListBoxShadow, pRibbonCombo) == 0x19e8,
              "CMFCDropDownListBox::m_pRibbonCombo");
static_assert(sizeof(DropDownListBoxShadow) == 0x19f0,
              "CMFCDropDownListBox complete size");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24,
              "CMFCToolBarButton::m_nID");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28,
              "CMFCToolBarButton::m_nStyle");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38,
              "CMFCToolBarButton::m_strText");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68,
              "CMFCToolBarButton::m_rect");
static_assert(offsetof(CMFCToolBar, m_iHighlighted) == 0x1138,
              "CMFCToolBar::m_iHighlighted");

// Null/malformed-object guards are defensive deviations.  AddString and
// ResetContent also return safely, rather than taking retail's assertion/trap
// path, when called while the popup HWND is live.
static DropDownListBoxShadow* DropDownState(void* pThis) {
    return static_cast<DropDownListBoxShadow*>(pThis);
}
static const DropDownListBoxShadow* DropDownState(const void* pThis) {
    return static_cast<const DropDownListBoxShadow*>(pThis);
}
static CMenu* DropDownMenu(void* pThis) {
    return reinterpret_cast<CMenu*>(static_cast<unsigned char*>(pThis) + 0x19c8);
}

// The CMFCPopupMenu virtual at byte offset 0x3a0 returns its embedded menu bar.
static CMFCToolBar* PopupMenuBar(void* pThis) {
    void** vtable = *reinterpret_cast<void***>(pThis);
    using GetMenuBarFn = void* (MS_ABI *)(void*);
    auto getMenuBar = reinterpret_cast<GetMenuBarFn>(vtable[0x3a0 / sizeof(void*)]);
    return static_cast<CMFCToolBar*>(getMenuBar(pThis));
}

extern "C" int MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis);
extern "C" int MS_ABI impl__CreatePopupMenu_CMenu__QEAAHXZ(CMenu* pThis);
extern "C" void MS_ABI impl___1CMFCPopupMenu__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnKeyDown_CMFCPopupMenu__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
    const CMFCToolBar* pThis, int nIndex);
extern "C" int MS_ABI impl__GetCount_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);
extern "C" void* MS_ABI impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(
    void* pThis, const void* pSource);

// Symbol: ??0CMFCDropDownListBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCMFCRibbonComboBox@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCMFCRibbonComboBox___Z(void* pThis, void* pComboBox) {
    (void)pComboBox;
    return pThis;
}
// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCWnd___Z(void* pThis, void* pWndParent) {
    (void)pWndParent;
    return pThis;
}
// Symbol: ??0CMFCDropDownListBox@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEB_WPEAVCMFCDropDownToolBar___Z(
    void* pThis, const wchar_t* name, void* pDropDownToolbar) {
    (void)name;
    (void)pDropDownToolbar;
    return pThis;
}

// Retail RVA 0x5c460 (mfc140u): call CMenu::DestroyMenu (callee RVA 0x2a8190,
// mfc140u), then tail-call the CMFCPopupMenu base destructor.  The existing
// base thunk invokes OpenMFC's CMFCPopupMenu destructor (CMFCPopupMenu.cpp:754
// clears active-menu state); it is not a transcription of retail's full cleanup.
// Symbol: ??1CMFCDropDownListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDropDownListBox__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    impl__DestroyMenu_CMenu__QEAAHXZ(DropDownMenu(pThis));
    impl___1CMFCPopupMenu__UEAA_XZ(pThis);
}

// Retail RVA 0x5c7f0 (mfc140u): append a menu string with command ID
// 0xffffff38 - the pre-append menu item count.  Retail asserts/traps for null
// arguments and for a live popup HWND; this implementation returns safely on
// all of those paths.  Resolved mfc140u imports: GetMenuItemCount, AppendMenuW.
// Symbol: ?AddString@CMFCDropDownListBox@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddString_CMFCDropDownListBox__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* lpszItem) {
    if (!pThis || !lpszItem) return;
    auto* base = reinterpret_cast<openmfc::detail::cmfcpopupmenu::PopupMenu*>(pThis);
    if (base->m_hWnd) return;
    const int count = ::GetMenuItemCount(DropDownMenu(pThis)->m_hMenu);
    const UINT commandId = 0xffffff38u - static_cast<UINT>(count);
    ::AppendMenuW(DropDownMenu(pThis)->m_hMenu, 0, commandId, lpszItem);
}

// Retail RVA 0x5c3f0 (mfc140u): clear the edit/ribbon pointers, initialize
// popup defaults, create the embedded popup menu, set selection to -1, and
// store 1 at +0x16a0 and 0 at +0x1660.
// Symbol: ?CommonInit@CMFCDropDownListBox@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCDropDownListBox__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    auto* state = DropDownState(pThis);
    auto* base = reinterpret_cast<openmfc::detail::cmfcpopupmenu::PopupMenu*>(pThis);
    state->pEditCtrl = nullptr;
    state->pRibbonCombo = nullptr;
    base->m_nAnimType = 1;
    base->m_nMinWidth = -1;
    impl__CreatePopupMenu_CMenu__QEAAHXZ(DropDownMenu(pThis));
    state->nCurSel = -1;
    // Retail writes one DWORD within the pointer-sized slot; preserve that
    // write width and leave the upper DWORD untouched.
    *reinterpret_cast<std::int32_t*>(static_cast<unsigned char*>(pThis) + 0x16a0) = 1;
    base->m_bLocked = 0;
}

// Retail RVA 0x5c670 (mfc140u): tail-call GetMenuItemCount(m_Menu.m_hMenu).
// Symbol: ?GetCount@CMFCDropDownListBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CMFCDropDownListBox__QEBAHXZ(const void* pThis) {
    if (!pThis) return -1;
    const auto* menu = reinterpret_cast<const CMenu*>(
        static_cast<const unsigned char*>(pThis) + 0x19c8);
    return ::GetMenuItemCount(menu->m_hMenu);
}

// Retail RVA 0x5c680 (mfc140u): with no popup HWND return the cached selection;
// otherwise map the highlighted physical toolbar button to its non-separator
// list index and cache that index.  A negative highlight or unmatched/missing
// button returns -1 without changing the cached selection.
// Symbol: ?GetCurSel@CMFCDropDownListBox@@QEAAHXZ
extern "C" int MS_ABI impl__GetCurSel_CMFCDropDownListBox__QEAAHXZ(void* pThis) {
    if (!pThis) return -1;
    auto* state = DropDownState(pThis);
    auto* base = reinterpret_cast<openmfc::detail::cmfcpopupmenu::PopupMenu*>(pThis);
    if (!base->m_hWnd) return state->nCurSel;

    CMFCToolBar* menuBar = PopupMenuBar(pThis);
    if (!menuBar) return -1;
    const int selectedPhysical = menuBar->m_iHighlighted;
    if (selectedPhysical < 0) return -1;
    CMFCToolBarButton* selected = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
        menuBar, selectedPhysical);
    if (!selected) return -1;

    int logicalIndex = 0;
    const int count = impl__GetCount_CMFCToolBar__QEBAHXZ(menuBar);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarButton* item = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
            menuBar, i);
        if (item && (item->m_nStyle & 1) == 0) {
            if (item == selected) {
                state->nCurSel = logicalIndex;
                return logicalIndex;
            }
            ++logicalIndex;
        }
    }
    return -1;
}

// Retail RVA 0x5c980 (mfc140u): only nIndex == 0 can return a button; it
// scans from the start, skips style-bit-0 separator buttons, and returns the
// first remaining item.  A nonzero nIndex scans to the end and returns null.
// Symbol: ?GetItem@CMFCDropDownListBox@@IEBAPEAVCMFCToolBarButton@@H@Z
extern "C" void* MS_ABI impl__GetItem_CMFCDropDownListBox__IEBAPEAVCMFCToolBarButton__H_Z(
    const void* pThis, int nIndex) {
    if (!pThis) return nullptr;
    CMFCToolBar* menuBar = PopupMenuBar(const_cast<void*>(pThis));
    if (!menuBar) return nullptr;
    const int count = impl__GetCount_CMFCToolBar__QEBAHXZ(menuBar);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarButton* item = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
            menuBar, i);
        if (item && (item->m_nStyle & 1) == 0 && nIndex == 0) return item;
    }
    return nullptr;
}

// Retail RVA 0x5c7c0 (mfc140u): if GetItem returns a button, assign its
// CString at +0x38 to the output CString; otherwise leave the output unchanged.
// The CSimpleStringT assignment is routed through its existing impl__ thunk.
// Symbol: ?GetText@CMFCDropDownListBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetText_CMFCDropDownListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, int nIndex, CString* rString) {
    if (!pThis || !rString) return;
    auto* item = static_cast<CMFCToolBarButton*>(
        impl__GetItem_CMFCDropDownListBox__IEBAPEAVCMFCToolBarButton__H_Z(pThis, nIndex));
    if (item) {
        impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(
            rString, &item->m_strText);
    }
}

// Retail RVA 0x5c890 (mfc140u): selecting a nonnegative item updates the menu
// bar, clamps against SCROLLINFO.nMax, adjusts locations, updates the vertical
// scroll position and calls CMFCPopupMenu::AdjustScroll.  The required scroll
// child HWND at +0x1720 is still opaque in CMFCPopupMenuSupport.h.
// TODO(clean-room): keep stubbed until that base-layout field is modeled.
// Symbol: ?HighlightItem@CMFCDropDownListBox@@IEAAXH@Z
extern "C" void MS_ABI impl__HighlightItem_CMFCDropDownListBox__IEAAXH_Z(
    void* pThis, int nIndex) {
    (void)pThis;
    (void)nIndex;
}

// Retail RVA 0x5cba0 (mfc140u): if m_pRibbonCombo is non-null, call its
// vtable byte offset 0x410 (slot 130) with no additional arguments.
// Symbol: ?OnChangeHot@CMFCDropDownListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCDropDownListBox__UEAAXH_Z(
    void* pThis, int nHot) {
    (void)nHot;
    if (!pThis) return;
    void* ribbonCombo = DropDownState(pThis)->pRibbonCombo;
    if (!ribbonCombo) return;
    void** vtable = *reinterpret_cast<void***>(ribbonCombo);
    using OnChangeHotFn = void (MS_ABI *)(void*);
    reinterpret_cast<OnChangeHotFn>(vtable[0x410 / sizeof(void*)])(ribbonCombo);
}

// Retail RVA 0x5cb70 (mfc140u): if m_pRibbonCombo is non-null, call its
// vtable byte offset 0x550 (slot 170) with command ID 0xffffff38 - uidCmdID.
// Symbol: ?OnChooseItem@CMFCDropDownListBox@@UEAAXI@Z
extern "C" void MS_ABI impl__OnChooseItem_CMFCDropDownListBox__UEAAXI_Z(
    void* pThis, unsigned int uidCmdID) {
    if (!pThis) return;
    void* ribbonCombo = DropDownState(pThis)->pRibbonCombo;
    if (!ribbonCombo) return;
    void** vtable = *reinterpret_cast<void***>(ribbonCombo);
    using OnChooseItemFn = void (MS_ABI *)(void*, unsigned int);
    const unsigned int commandId = 0xffffff38u - uidCmdID;
    reinterpret_cast<OnChooseItemFn>(vtable[0x550 / sizeof(void*)])(ribbonCombo, commandId);
}

// Retail RVA 0x5c9e0 (mfc140u): inflate the button rectangle by (-6,0), offer
// the item to the ribbon combo at vtable byte offset 0x560, and, unless handled,
// draw the button text through CDC vtable byte offset 0xe0 with format 0x824.
// Symbol: ?OnDrawItem@CMFCDropDownListBox@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@H@Z
extern "C" void MS_ABI impl__OnDrawItem_CMFCDropDownListBox__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__H_Z(
    void* pThis, CDC* pDC, CMFCToolBarMenuButton* pItem, int bHighlight) {
    if (!pThis || !pDC || !pItem) return;
    RECT rect{pItem->m_rect.left, pItem->m_rect.top,
              pItem->m_rect.right, pItem->m_rect.bottom};
    ::InflateRect(&rect, -6, 0);

    void* ribbonCombo = DropDownState(pThis)->pRibbonCombo;
    if (ribbonCombo) {
        void** vtable = *reinterpret_cast<void***>(ribbonCombo);
        using DrawDropListItemFn = int (MS_ABI *)(void*, void*, unsigned int, void*, int);
        const unsigned int commandId = 0xffffff38u - pItem->m_nID;
        if (reinterpret_cast<DrawDropListItemFn>(vtable[0x560 / sizeof(void*)])(
                ribbonCombo, pDC, commandId, pItem, bHighlight)) {
            return;
        }
    }

    void** dcVtable = *reinterpret_cast<void***>(pDC);
    using DrawTextFn = int (MS_ABI *)(void*, const wchar_t*, int, RECT*, unsigned int);
    reinterpret_cast<DrawTextFn>(dcVtable[0xe0 / sizeof(void*)])(
        pDC, pItem->m_strText.GetString(), pItem->m_strText.GetLength(), &rect, 0x824);
}

// The Unicode symbol has no resolved RVA; the ANSI twin body at RVA 0x5c8f0
// (mfc140.dll) is transcribed.  It asks the ribbon combo at vtable byte offset
// 0x568 for a size and uses the returned nonzero CSize, otherwise sizeDefault.
// CSize is an 8-byte by-value aggregate (long long under MS_ABI); the explicit
// result pointer is the ABI's hidden structure-return argument.
// Symbol: ?OnGetItemSize@CMFCDropDownListBox@@UEAA?AVCSize@@PEAVCDC@@PEAVCMFCToolBarMenuButton@@V2@@Z
extern "C" CSize* MS_ABI impl__OnGetItemSize_CMFCDropDownListBox__UEAA_AVCSize__PEAVCDC__PEAVCMFCToolBarMenuButton__V2__Z(
    void* pThis, CSize* pResult, CDC* pDC, CMFCToolBarMenuButton* pItem,
    long long sizeDefault) {
    if (!pResult) return nullptr;
    const std::uint64_t packed = static_cast<std::uint64_t>(sizeDefault);
    const CSize fallback(static_cast<LONG>(static_cast<std::uint32_t>(packed)),
                         static_cast<LONG>(static_cast<std::uint32_t>(packed >> 32)));
    if (!pThis || !pItem) {
        pResult->cx = fallback.cx;
        pResult->cy = fallback.cy;
        return pResult;
    }

    void* ribbonCombo = DropDownState(pThis)->pRibbonCombo;
    if (ribbonCombo) {
        CSize comboSize(0, 0);
        void** vtable = *reinterpret_cast<void***>(ribbonCombo);
        using GetItemSizeFn = CSize* (MS_ABI *)(void*, CSize*, CDC*, unsigned int,
                                                CMFCToolBarMenuButton*, long long);
        const unsigned int commandId = 0xffffff38u - pItem->m_nID;
        reinterpret_cast<GetItemSizeFn>(vtable[0x568 / sizeof(void*)])(
            ribbonCombo, &comboSize, pDC, commandId, pItem, sizeDefault);
        if (comboSize.cx != 0 || comboSize.cy != 0) {
            pResult->cx = comboSize.cx;
            pResult->cy = comboSize.cy;
            return pResult;
        }
    }
    pResult->cx = fallback.cx;
    pResult->cy = fallback.cy;
    return pResult;
}

// The Unicode symbol has no resolved RVA; the ANSI twin body at RVA 0x5c960
// (mfc140.dll) forwards ordinary keys to the edit control with WM_KEYDOWN and
// sends Enter/Escape/PageUp/PageDown/Up/Down (or invalid edit HWNDs) to the
// CMFCPopupMenu base handler.  The latter is currently a documented stub in
// CMFCPopupMenu.cpp because its embedded menu bar is not modeled.  The ANSI
// SendMessageA IAT slot corresponds to SendMessageW in this UNICODE build.
// Symbol: ?OnKeyDown@CMFCDropDownListBox@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCDropDownListBox__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    if (!pThis) return;
    void* edit = DropDownState(pThis)->pEditCtrl;
    HWND hEdit = edit ? static_cast<CWnd*>(edit)->m_hWnd : nullptr;
    const bool baseKey = nChar == VK_RETURN || nChar == VK_ESCAPE ||
                         nChar == VK_PRIOR || nChar == VK_NEXT ||
                         nChar == VK_UP || nChar == VK_DOWN;
    if (hEdit && !baseKey) {
        const LPARAM keyData = MAKELPARAM(static_cast<WORD>(nRepCnt),
                                          static_cast<WORD>(nFlags));
        ::SendMessage(hEdit, WM_KEYDOWN, nChar, keyData);
        return;
    }
    impl__OnKeyDown_CMFCPopupMenu__IEAAXIII_Z(pThis, nChar, nRepCnt, nFlags);
}

// The Unicode symbol has no resolved RVA; the ANSI twin at RVA 0x5c6a0
// (mfc140.dll) destroys the old menu and creates a fresh popup menu when the
// popup HWND is not created.  Retail asserts/traps for a live HWND; this body
// returns safely on that path instead.
// Symbol: ?ResetContent@CMFCDropDownListBox@@QEAAXXZ
extern "C" void MS_ABI impl__ResetContent_CMFCDropDownListBox__QEAAXXZ(void* pThis) {
    if (!pThis) return;
    auto* base = reinterpret_cast<openmfc::detail::cmfcpopupmenu::PopupMenu*>(pThis);
    if (base->m_hWnd) return;
    CMenu* menu = DropDownMenu(pThis);
    impl__DestroyMenu_CMenu__QEAAHXZ(menu);
    impl__CreatePopupMenu_CMenu__QEAAHXZ(menu);
}

// The Unicode symbol has no resolved RVA; the ANSI twin at RVA 0x5c570
// (mfc140.dll) returns the old selection on success, stores nSelect when no
// popup HWND exists, and otherwise maps the logical index to a physical toolbar
// index before calling HighlightItem; invalid logical indices return -1.  The
// latter remains stubbed because +0x1720 is not modeled, so live
// highlighting/scrolling is still incomplete here.
// Symbol: ?SetCurSel@CMFCDropDownListBox@@QEAAHH@Z
extern "C" int MS_ABI impl__SetCurSel_CMFCDropDownListBox__QEAAHH_Z(
    void* pThis, int nSelect) {
    if (!pThis) return -1;

    const int oldSelection =
        impl__GetCurSel_CMFCDropDownListBox__QEAAHXZ(pThis);
    auto* base = reinterpret_cast<openmfc::detail::cmfcpopupmenu::PopupMenu*>(pThis);
    if (!base->m_hWnd) {
        DropDownState(pThis)->nCurSel = nSelect;
        return oldSelection;
    }

    CMFCToolBar* menuBar = PopupMenuBar(pThis);
    if (!menuBar) return -1;
    const int count = impl__GetCount_CMFCToolBar__QEBAHXZ(menuBar);
    int logicalIndex = 0;
    for (int physicalIndex = 0; physicalIndex < count; ++physicalIndex) {
        CMFCToolBarButton* item =
            impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
                menuBar, physicalIndex);
        if (item && (item->m_nStyle & 1) == 0) {
            if (logicalIndex == nSelect) {
                impl__HighlightItem_CMFCDropDownListBox__IEAAXH_Z(
                    pThis, physicalIndex);
                return oldSelection;
            }
            ++logicalIndex;
        }
    }
    return -1;
}

// The Unicode symbol has no resolved RVA; the ANSI twin body at RVA 0x5c300
// (mfc140.dll) creates/tracks the popup, initializes its owner and scroll state,
// and conditionally redirects to owner windows.  That behavior requires the
// unmodeled popup/window tracking infrastructure, so this remains a stub.
// Symbol: ?Track@CMFCDropDownListBox@@QEAAXVCPoint@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__Track_CMFCDropDownListBox__QEAAXVCPoint__PEAVCWnd___Z(
    void* pThis, long long point, CWnd* pWndOwner) {
    (void)pThis;
    (void)point;
    (void)pWndOwner;
}
