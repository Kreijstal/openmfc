// CMFCDropDownListBox uses the popup menu's window and viewport.

#include "detail/CMFCDropDownListBoxSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using namespace openmfc::detail::mfccore;

static_assert(offsetof(DropDownListBox, scrollable) == 0x16a4);
static_assert(offsetof(DropDownListBox, showScrollBar) == 0x16a8);
static_assert(offsetof(DropDownListBox, maxHeight) == 0x16d4);
static_assert(offsetof(DropDownListBox, menu) == 0x19c8);
static_assert(offsetof(DropDownListBox, selection) == 0x19d8);
static_assert(offsetof(DropDownListBox, minWidth) == 0x19dc);
static_assert(offsetof(DropDownListBox, edit) == 0x19e0);
static_assert(offsetof(DropDownListBox, ribbonCombo) == 0x19e8);
static_assert(sizeof(DropDownListBox) == 0x19f0);
static_assert(sizeof(CSize) == sizeof(std::int64_t));

extern "C" IMAGE_DOS_HEADER __ImageBase;
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCDropDownListBox__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__OnSelectItem_CMFCRibbonComboBox__UEAAXH_Z(void*, int);

namespace {
constexpr UINT kFirstListCommand = 0xffffff38u;

template<class Fn>
Fn RibbonOverride(void* ribbon, size_t offset) {
    if (!ribbon) return nullptr;
    auto** table = *static_cast<void***>(ribbon);
    MEMORY_BASIC_INFORMATION region{};
    if (!table || !::VirtualQuery(table, &region, sizeof(region)) || region.State != MEM_COMMIT ||
        region.AllocationBase == &__ImageBase) {
        return nullptr;
    }
    return reinterpret_cast<Fn>(table[offset / sizeof(void*)]);
}

CPoint UnpackPoint(long long point) {
    POINT result{};
    std::memcpy(&result, &point, sizeof(result));
    return CPoint(result);
}
}

DropDownListBox::DropDownListBox() {
    menu.CreatePopupMenu();
}

DropDownListBox::~DropDownListBox() {
    const auto it = g_popupMenuStates.find(this);
    if (it != g_popupMenuStates.end()) it->second.deleteOnClose = false;
    CloseMenu();
}

CRuntimeClass* DropDownListBox::GetRuntimeClass() const {
    return impl__GetThisClass_CMFCDropDownListBox__SAPEAUCRuntimeClass__XZ();
}

void DropDownListBox::ResetContent() {
    if (GetSafeHwnd()) return;
    menu.DestroyMenu();
    menu.CreatePopupMenu();
    selection = -1;
}

void DropDownListBox::AddString(const wchar_t* text) {
    if (!text || GetSafeHwnd()) return;
    const int count = GetCount();
    if (count >= 0) ::AppendMenuW(menu.GetSafeHmenu(), MF_STRING, kFirstListCommand - count, text);
}

int DropDownListBox::GetCount() const {
    return ::GetMenuItemCount(menu.GetSafeHmenu());
}

CMFCToolBarMenuButton* DropDownListBox::GetItem(int index) const {
    if (index < 0) return nullptr;
    for (int i = 0; i < GetMenuItemCount(); ++i) {
        auto* item = GetMenuItem(i);
        if (item && !(item->m_nStyle & TBSTYLE_SEP) && index-- == 0) return item;
    }
    return nullptr;
}

void DropDownListBox::GetText(int index, CString& text) const {
    if (index < 0 || index >= GetCount()) return;
    MENUITEMINFOW item{sizeof(item)};
    item.fMask = MIIM_FTYPE | MIIM_STRING;
    const int count = GetCount();
    int position = 0;
    for (; position < count; ++position) {
        if (!::GetMenuItemInfoW(menu.GetSafeHmenu(), position, TRUE, &item)) return;
        if (!(item.fType & MFT_SEPARATOR) && index-- == 0) break;
    }
    if (position == count) return;
    CString value;
    ++item.cch;
    item.dwTypeData = value.GetBuffer(item.cch);
    const BOOL copied = ::GetMenuItemInfoW(menu.GetSafeHmenu(), position, TRUE, &item);
    value.ReleaseBuffer(copied ? item.cch : 0);
    if (copied) text = value;
}

void DropDownListBox::HighlightItem(int physicalIndex) {
    if (physicalIndex >= 0 && GetSafeHwnd()) SelectDropDownPopupRow(*this, physicalIndex);
}

int DropDownListBox::SetCurSel(int index) {
    const int previous = selection;
    if (!GetSafeHwnd()) {
        selection = index;
        return previous;
    }
    int logicalIndex = 0;
    for (int physicalIndex = 0; physicalIndex < GetMenuItemCount(); ++physicalIndex) {
        const auto* item = GetMenuItem(physicalIndex);
        if (item && !(item->m_nStyle & TBSTYLE_SEP) && logicalIndex++ == index) {
            HighlightItem(physicalIndex);
            return previous;
        }
    }
    return -1;
}

HWND DropDownListBox::EditWindow() const {
    return edit ? edit->GetSafeHwnd() : nullptr;
}

void DropDownListBox::Track(CPoint point, CWnd* owner) {
    if (GetSafeHwnd() || GetCount() <= 0) return;
    auto& state = g_popupMenuStates[this];
    state.dropDownList = this;
    if (!Create(owner ? owner : edit, point.x, point.y, menu.GetSafeHmenu(), FALSE, TRUE)) return;
    state.deleteOnClose = true;
    if (selection >= 0) SetCurSel(selection);
}

void DropDownListBox::OnChangeHot(int index) {
    (void)index;
    using Notify = void (MS_ABI *)(void*);
    if (const auto notify = RibbonOverride<Notify>(ribbonCombo, 0x410)) notify(ribbonCombo);
}

void DropDownListBox::OnChooseItem(UINT command) {
    if (!ribbonCombo) return;
    const int index = static_cast<int>(kFirstListCommand - command);
    using Choose = void (MS_ABI *)(void*, int);
    if (const auto choose = RibbonOverride<Choose>(ribbonCombo, 0x550)) choose(ribbonCombo, index);
    else impl__OnSelectItem_CMFCRibbonComboBox__UEAAXH_Z(ribbonCombo, index);
}

void DropDownListBox::OnDrawItem(CDC* dc, CMFCToolBarMenuButton* item, BOOL hot) {
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || !item) return;
    using Draw = BOOL (MS_ABI *)(void*, CDC*, UINT, CMFCToolBarMenuButton*, BOOL);
    if (const auto draw = RibbonOverride<Draw>(ribbonCombo, 0x560)) {
        if (draw(ribbonCombo, dc, kFirstListCommand - item->m_nID, item, hot)) return;
    }
    CRect text = item->m_rect;
    text.DeflateRect(6, 0);
    ::SetBkMode(hdc, TRANSPARENT);
    ::SetTextColor(hdc, ::GetSysColor(hot ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));
    ::DrawTextW(hdc, item->m_strText, item->m_strText.GetLength(), text,
                DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);
}

CSize DropDownListBox::OnGetItemSize(CDC* dc, CMFCToolBarMenuButton* item, CSize sizeDefault) {
    using Measure = CSize* (MS_ABI *)(void*, CSize*, CDC*, UINT, CMFCToolBarMenuButton*, long long);
    if (const auto measure = RibbonOverride<Measure>(ribbonCombo, 0x568); measure && item) {
        CSize size(0, 0);
        long long packed{};
        std::memcpy(&packed, &sizeDefault, sizeof(packed));
        measure(ribbonCombo, &size, dc, kFirstListCommand - item->m_nID, item, packed);
        if (size.cx > 0 && size.cy > 0) return size;
    }
    return sizeDefault;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_XZ(void* pThis) {
    return pThis ? new (pThis) DropDownListBox() : nullptr;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCMFCRibbonComboBox@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCMFCRibbonComboBox___Z(void* pThis, void* combo) {
    auto* list = static_cast<DropDownListBox*>(impl___0CMFCDropDownListBox__QEAA_XZ(pThis));
    if (list && combo) {
        list->ribbonCombo = combo;
        // CMFCRibbonEdit::m_pWndEdit remains at this offset in CMFCRibbonComboBox.
        std::memcpy(&list->edit, static_cast<const char*>(combo) + 0x2a0, sizeof(list->edit));
    }
    return list;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCWnd___Z(void* pThis, CWnd* edit) {
    auto* list = static_cast<DropDownListBox*>(impl___0CMFCDropDownListBox__QEAA_XZ(pThis));
    if (list) list->edit = edit;
    return list;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEB_WPEAVCMFCDropDownToolBar___Z(
    void* pThis, const wchar_t* name, void* toolbar) {
    (void)name;
    (void)toolbar;
    return impl___0CMFCDropDownListBox__QEAA_XZ(pThis);
}

// Symbol: ??1CMFCDropDownListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDropDownListBox__UEAA_XZ(DropDownListBox* list) {
    if (list) list->~DropDownListBox();
}

// Symbol: ?AddString@CMFCDropDownListBox@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddString_CMFCDropDownListBox__QEAAXPEB_W_Z(DropDownListBox* list, const wchar_t* text) {
    if (list) list->AddString(text);
}

// Symbol: ?CommonInit@CMFCDropDownListBox@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCDropDownListBox__IEAAXXZ(DropDownListBox* list) {
    if (!list || list->GetSafeHwnd()) return;
    list->edit = nullptr;
    list->ribbonCombo = nullptr;
    list->minWidth = -1;
    list->maxHeight = -1;
    list->ResetContent();
}

// Symbol: ?GetCount@CMFCDropDownListBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CMFCDropDownListBox__QEBAHXZ(const DropDownListBox* list) {
    return list ? list->GetCount() : -1;
}

// Symbol: ?GetCurSel@CMFCDropDownListBox@@QEAAHXZ
extern "C" int MS_ABI impl__GetCurSel_CMFCDropDownListBox__QEAAHXZ(const DropDownListBox* list) {
    return list ? list->selection : -1;
}

// Symbol: ?GetItem@CMFCDropDownListBox@@IEBAPEAVCMFCToolBarButton@@H@Z
extern "C" void* MS_ABI impl__GetItem_CMFCDropDownListBox__IEBAPEAVCMFCToolBarButton__H_Z(
    const DropDownListBox* list, int index) {
    return list ? list->GetItem(index) : nullptr;
}

// Symbol: ?GetText@CMFCDropDownListBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetText_CMFCDropDownListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const DropDownListBox* list, int index, CString* text) {
    if (list && text) list->GetText(index, *text);
}

// Symbol: ?HighlightItem@CMFCDropDownListBox@@IEAAXH@Z
extern "C" void MS_ABI impl__HighlightItem_CMFCDropDownListBox__IEAAXH_Z(DropDownListBox* list, int index) {
    if (list) list->HighlightItem(index);
}

// Symbol: ?OnChangeHot@CMFCDropDownListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCDropDownListBox__UEAAXH_Z(DropDownListBox* list, int index) {
    if (list) list->OnChangeHot(index);
}

// Symbol: ?OnChooseItem@CMFCDropDownListBox@@UEAAXI@Z
extern "C" void MS_ABI impl__OnChooseItem_CMFCDropDownListBox__UEAAXI_Z(DropDownListBox* list, UINT command) {
    if (list) list->OnChooseItem(command);
}

// Symbol: ?OnDrawItem@CMFCDropDownListBox@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@H@Z
extern "C" void MS_ABI impl__OnDrawItem_CMFCDropDownListBox__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__H_Z(
    DropDownListBox* list, CDC* dc, CMFCToolBarMenuButton* item, BOOL hot) {
    if (list) list->OnDrawItem(dc, item, hot);
}

// Symbol: ?OnGetItemSize@CMFCDropDownListBox@@UEAA?AVCSize@@PEAVCDC@@PEAVCMFCToolBarMenuButton@@V2@@Z
extern "C" CSize* MS_ABI impl__OnGetItemSize_CMFCDropDownListBox__UEAA_AVCSize__PEAVCDC__PEAVCMFCToolBarMenuButton__V2__Z(
    DropDownListBox* list, CSize* result, CDC* dc, CMFCToolBarMenuButton* item, long long sizeDefault) {
    if (!result) return nullptr;
    const CSize fallback(static_cast<LONG>(sizeDefault), static_cast<LONG>(static_cast<std::uint64_t>(sizeDefault) >> 32));
    *result = list ? list->OnGetItemSize(dc, item, fallback) : fallback;
    return result;
}

// Symbol: ?OnKeyDown@CMFCDropDownListBox@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCDropDownListBox__IEAAXIII_Z(
    DropDownListBox* list, UINT key, UINT repeat, UINT flags) {
    if (list && list->GetSafeHwnd()) ::SendMessageW(list->GetSafeHwnd(), WM_KEYDOWN, key, MAKELPARAM(repeat, flags));
}

// Symbol: ?ResetContent@CMFCDropDownListBox@@QEAAXXZ
extern "C" void MS_ABI impl__ResetContent_CMFCDropDownListBox__QEAAXXZ(DropDownListBox* list) {
    if (list) list->ResetContent();
}

// Symbol: ?SetCurSel@CMFCDropDownListBox@@QEAAHH@Z
extern "C" int MS_ABI impl__SetCurSel_CMFCDropDownListBox__QEAAHH_Z(DropDownListBox* list, int index) {
    return list ? list->SetCurSel(index) : -1;
}

// Symbol: ?Track@CMFCDropDownListBox@@QEAAXVCPoint@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__Track_CMFCDropDownListBox__QEAAXVCPoint__PEAVCWnd___Z(
    DropDownListBox* list, long long point, CWnd* owner) {
    if (list) list->Track(UnpackPoint(point), owner);
}
