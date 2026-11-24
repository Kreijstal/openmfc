// CMFCRibbonComboBox — item selection, editing, and popup tracking.

#define OPENMFC_APPCORE_IMPL
#include "detail/CMFCDropDownListBoxSupport.h"
#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <commctrl.h>
#include <oleacc.h>
#include <algorithm>
#include <cstring>
#include <memory>
#include <new>

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonComboBox__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned int impl__AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM__3IA;
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

static_assert(sizeof(CMFCRibbonComboBox) == 0x330);
static_assert(offsetof(CMFCRibbonComboBox, m_comboRect) == 0xc8);
static_assert(offsetof(CMFCRibbonComboBox, m_openList) == 0x108);
static_assert(offsetof(CMFCRibbonComboBox, m_sdkCommandId) == 0x118);
static_assert(offsetof(CMFCRibbonComboBox, m_droppedDown) == 0x154);
static_assert(offsetof(CMFCRibbonComboBox, m_comboArrow) == 0x198);
static_assert(offsetof(CMFCRibbonComboBox, m_editWidth) == 0x270);
static_assert(offsetof(CMFCRibbonComboBox, m_editText) == 0x298);
static_assert(offsetof(CMFCRibbonComboBox, m_editWindow) == 0x2a0);
static_assert(offsetof(CMFCRibbonComboBox, m_itemStrings) == 0x2b0);
static_assert(offsetof(CMFCRibbonComboBox, m_nativeStringCount) == 0x2c8);
static_assert(offsetof(CMFCRibbonComboBox, m_itemData) == 0x2e8);
static_assert(offsetof(CMFCRibbonComboBox, m_selectedIndex) == 0x320);
static_assert(offsetof(CMFCRibbonComboBox, m_dropDownHeight) == 0x324);
static_assert(offsetof(CMFCRibbonComboBox, m_hasEditBox) == 0x328);
static_assert(offsetof(CMFCRibbonComboBox, m_resizeDropDownList) == 0x32c);

namespace {
struct ItemData {
    ItemData* next;
    ItemData* prev;
    DWORD_PTR value;
};

struct ItemDataList {
    void* vptr;
    ItemData* head;
    ItemData* tail;
    INT_PTR count;
    void* freeNodes;
    void* blocks;
    INT_PTR blockSize;
};
static_assert(sizeof(ItemDataList) == sizeof(CMFCRibbonComboBox::m_itemData));

ItemDataList& DataList(CMFCRibbonComboBox& combo) {
    return *reinterpret_cast<ItemDataList*>(combo.m_itemData);
}

ItemData* FindData(const CMFCRibbonComboBox& combo, int index) {
    if (index < 0) return nullptr;
    const auto& list = *reinterpret_cast<const ItemDataList*>(combo.m_itemData);
    if (index >= list.count) return nullptr;
    auto* node = list.head;
    while (index-- > 0) node = node->next;
    return node;
}

void ClearItemData(CMFCRibbonComboBox& combo) {
    auto& list = DataList(combo);
    while (list.head) {
        auto* item = list.head;
        list.head = item->next;
        delete item;
    }
    list.tail = nullptr;
    list.count = 0;
}

CPoint UnpackPoint(long long point) {
    POINT unpacked{};
    std::memcpy(&unpacked, &point, sizeof(unpacked));
    return CPoint(unpacked);
}

using RibbonElements = CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*&>;
}

extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonBar__QEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBar* bar, UINT id, RibbonElements& elements);

namespace {
class RibbonComboList final : public openmfc::detail::mfccore::DropDownListBox {
public:
    explicit RibbonComboList(CMFCRibbonComboBox& combo) : m_combo(combo) {}
    ~RibbonComboList() override { m_combo.OnListClosed(this); }
    void OnChangeHot(int) override { if (m_combo.m_openList == this) m_combo.OnListHighlight(selection); }
    void OnChooseItem(UINT command) override { m_combo.OnSelectItem(static_cast<int>(0xffffff38u - command)); }
private:
    CMFCRibbonComboBox& m_combo;
};

LRESULT CALLBACK RibbonComboEditProc(HWND window, UINT message, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR data) {
    auto& combo = *reinterpret_cast<CMFCRibbonComboBox*>(data);
    if (message == WM_NCDESTROY) {
        ::RemoveWindowSubclass(window, RibbonComboEditProc, id);
        if (combo.m_editWindow && combo.m_editWindow->GetSafeHwnd() == window) combo.m_editWindow->Detach();
    } else if ((message == WM_KEYDOWN && wp == VK_F4) || (message == WM_SYSKEYDOWN && wp == VK_DOWN)) {
        if (combo.m_openList) combo.CloseDropDown();
        else combo.DropDownList();
        return 0;
    } else if (message == WM_KEYDOWN && wp == VK_RETURN) {
        combo.SyncEditText();
        const int index = combo.FindItem(combo.GetEditText());
        if (index >= 0) combo.OnSelectItem(index);
        return 0;
    }
    return ::DefSubclassProc(window, message, wp, lp);
}
}

CMFCRibbonComboBox::CMFCRibbonComboBox(UINT id, BOOL hasEditBox, int width, const wchar_t* label, int image)
    : CMFCRibbonEdit(id, width, label, image), m_editWidth(width == -1 ? 108 : width), m_hasEditBox(hasEditBox) {
    m_sdkCommandId = id;
    new (m_itemData) ItemDataList{nullptr, nullptr, nullptr, 0, nullptr, nullptr, 10};
}

CMFCRibbonComboBox::~CMFCRibbonComboBox() {
    CloseDropDown();
    DestroyEditWindow();
    ClearItemData(*this);
}

CRuntimeClass* PASCAL CMFCRibbonComboBox::GetThisClass() {
    return impl__GetThisClass_CMFCRibbonComboBox__SAPEAUCRuntimeClass__XZ();
}
CRuntimeClass* CMFCRibbonComboBox::GetRuntimeClass() const { return GetThisClass(); }

INT_PTR CMFCRibbonComboBox::AddItem(const wchar_t* text, DWORD_PTR data) {
    if (!text) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    if (FindItem(text) < 0) {
        CloseDropDown();
        auto& list = DataList(*this);
        auto item = std::make_unique<ItemData>(ItemData{nullptr, list.tail, data});
        m_itemStrings.AddTail(text);
        if (list.tail) list.tail->next = item.get();
        else list.head = item.get();
        list.tail = item.release();
        m_nativeStringCount = ++list.count;
    }
    return GetCount() - 1;
}

INT_PTR CMFCRibbonComboBox::GetCount() const { return m_itemStrings.GetCount(); }

const wchar_t* CMFCRibbonComboBox::GetItem(int index) const {
    if (index == -1) index = m_selectedIndex;
    if (index < 0) return nullptr;
    auto pos = m_itemStrings.FindIndex(index);
    if (pos == CStringList::POSITION(nullptr)) return nullptr;
    return const_cast<CStringList&>(m_itemStrings).GetNext(pos).GetString();
}

DWORD_PTR CMFCRibbonComboBox::GetItemData(int index) const {
    if (const auto* item = FindData(*this, index == -1 ? m_selectedIndex : index)) return item->value;
    return 0;
}

int CMFCRibbonComboBox::FindItem(const wchar_t* text) const {
    if (!text) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    int index = 0;
    for (auto pos = m_itemStrings.GetHeadPosition(); pos != CStringList::POSITION(nullptr); ++index) {
        if (m_itemStrings.GetNext(pos).CompareNoCase(text) == 0) return index;
    }
    return -1;
}

BOOL CMFCRibbonComboBox::ApplySelection(int index) {
    if (index >= GetCount()) return FALSE;
    m_selectedIndex = (std::max)(-1, index);
    SetEditText(GetItem(m_selectedIndex));
    return TRUE;
}

BOOL CMFCRibbonComboBox::SelectItem(int index) {
    if (!ApplySelection(index)) return FALSE;
    if (auto* bar = GetTopLevelRibbonBar()) {
        RibbonElements peers;
        impl__GetElementsByID_CMFCRibbonBar__QEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(bar, GetID(), peers);
        for (INT_PTR i = 0; i < peers.GetSize(); ++i) {
            auto* element = peers.GetAt(i);
            if (element && element != this && element->IsKindOf(GetThisClass())) {
                static_cast<CMFCRibbonComboBox*>(element)->ApplySelection(index);
            }
        }
    }
    return TRUE;
}

BOOL CMFCRibbonComboBox::SelectItem(DWORD_PTR data) {
    int index = 0;
    for (auto* item = DataList(*this).head; item; item = item->next, ++index) {
        if (item->value == data) return SelectItem(index);
    }
    return FALSE;
}

BOOL CMFCRibbonComboBox::SelectItem(const wchar_t* text) {
    const int index = FindItem(text);
    return index >= 0 && SelectItem(index);
}

BOOL CMFCRibbonComboBox::DeleteItem(int index) {
    auto* item = FindData(*this, index);
    const auto pos = m_itemStrings.FindIndex(index);
    if (!item || pos == CStringList::POSITION(nullptr) || index < 0) return FALSE;
    CloseDropDown();
    m_itemStrings.RemoveAt(pos);
    auto& list = DataList(*this);
    if (item->prev) item->prev->next = item->next;
    else list.head = item->next;
    if (item->next) item->next->prev = item->prev;
    else list.tail = item->prev;
    delete item;
    m_nativeStringCount = --list.count;
    if (index == m_selectedIndex) SelectItem((std::min)(index, static_cast<int>(list.count) - 1));
    else if (index < m_selectedIndex) --m_selectedIndex;
    return TRUE;
}

void CMFCRibbonComboBox::RemoveAllItems() {
    CloseDropDown();
    m_itemStrings.RemoveAll();
    ClearItemData(*this);
    m_nativeStringCount = 0;
    m_selectedIndex = -1;
    SetEditText(nullptr);
}

void CMFCRibbonComboBox::SetEditText(const wchar_t* text) {
    m_editText = text ? text : L"";
    if (m_editWindow && m_editWindow->GetSafeHwnd()) {
        ::SetWindowTextW(m_editWindow->GetSafeHwnd(), m_editText);
    }
    if (auto* bar = GetTopLevelRibbonBar(); bar && bar->GetSafeHwnd()) {
        ::InvalidateRect(bar->GetSafeHwnd(), m_comboRect, FALSE);
    }
}

void CMFCRibbonComboBox::SyncEditText() {
    const HWND window = m_editWindow ? m_editWindow->GetSafeHwnd() : nullptr;
    if (!window) return;
    const int length = ::GetWindowTextLengthW(window);
    auto* buffer = m_editText.GetBuffer(length + 1);
    m_editText.ReleaseBuffer(::GetWindowTextW(window, buffer, length + 1));
}

void CMFCRibbonComboBox::SetID(UINT id) {
    CMFCRibbonBaseElement::SetID(id);
    m_sdkCommandId = id;
}

CSize CMFCRibbonComboBox::GetIntermediateSize(CDC* dc) const {
    const CString label = GetText();
    RECT text{};
    if (dc && dc->GetSafeHdc()) {
        ::DrawTextW(dc->GetSafeHdc(), label, label.GetLength(), &text, DT_CALCRECT | DT_SINGLELINE);
    }
    return CSize((std::max)(m_editWidth, 48) + (text.right ? text.right + 8 : 0) +
                 ::GetSystemMetrics(SM_CXVSCROLL), (std::max)(24L, text.bottom + 8));
}

void CMFCRibbonComboBox::OnDraw(CDC* dc) const {
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || m_comboRect.IsRectEmpty()) return;
    const int saved = ::SaveDC(hdc);
    CRect field = m_comboRect;
    const CString label = GetText();
    if (!label.IsEmpty()) {
        CRect text(0, 0, 0, 0);
        ::DrawTextW(hdc, label, label.GetLength(), text, DT_CALCRECT | DT_SINGLELINE);
        field.left = (std::min)(m_comboArrow.left, m_comboRect.left + text.Width() + 8);
        CRect labelRect = m_comboRect;
        labelRect.right = field.left;
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, ::GetSysColor(COLOR_BTNTEXT));
        ::DrawTextW(hdc, label, label.GetLength(), labelRect, DT_SINGLELINE | DT_VCENTER);
    }
    ::FillRect(hdc, field, ::GetSysColorBrush(COLOR_WINDOW));
    ::FrameRect(hdc, field, ::GetSysColorBrush(COLOR_BTNSHADOW));
    if (!m_editWindow || !m_editWindow->GetSafeHwnd()) {
        CRect text(field.left + 3, field.top, m_comboArrow.left - 2, field.bottom);
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, ::GetSysColor(COLOR_WINDOWTEXT));
        ::DrawTextW(hdc, m_editText, m_editText.GetLength(), text,
                    DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);
    }
    RECT arrow = *static_cast<const RECT*>(m_comboArrow);
    ::DrawFrameControl(hdc, &arrow, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | (m_openList ? DFCS_PUSHED : 0));
    if (saved) ::RestoreDC(hdc, saved);
}

void CMFCRibbonComboBox::SetRect(CRect rect) {
    CRect arrow = rect;
    arrow.left = (std::max)(rect.left, rect.right - ::GetSystemMetrics(SM_CXVSCROLL));
    if (rect == m_comboRect && arrow == m_comboArrow && m_editWindow && m_editWindow->GetSafeHwnd()) return;
    if (rect.IsRectEmpty() && m_openList) CloseDropDown();
    m_comboRect = rect;
    m_comboArrow = arrow;
    UpdateEditWindow();
}

void CMFCRibbonComboBox::UpdateEditWindow() {
    const auto* bar = GetTopLevelRibbonBar();
    const HWND parent = bar ? bar->GetSafeHwnd() : nullptr;
    HWND edit = m_editWindow ? m_editWindow->GetSafeHwnd() : nullptr;
    if (!m_hasEditBox || m_comboRect.IsRectEmpty() || !parent) {
        if (edit) ::ShowWindow(edit, SW_HIDE);
        return;
    }
    if (!edit || ::GetParent(edit) != parent) {
        DestroyEditWindow();
        auto wrapper = std::make_unique<CWnd>();
        edit = ::CreateWindowExW(0, L"EDIT", m_editText, WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
                                 0, 0, 0, 0, parent, nullptr, ::GetModuleHandleW(nullptr), nullptr);
        if (!edit) return;
        if (!wrapper->Attach(edit)) {
            ::DestroyWindow(edit);
            return;
        }
        m_editWindow = wrapper.release();
        ::SendMessageW(edit, WM_SETFONT, reinterpret_cast<WPARAM>(::GetStockObject(DEFAULT_GUI_FONT)), FALSE);
        ::SetWindowSubclass(edit, RibbonComboEditProc, reinterpret_cast<UINT_PTR>(this), reinterpret_cast<DWORD_PTR>(this));
    }
    CRect editRect = m_comboRect;
    const CString label = GetText();
    if (!label.IsEmpty()) {
        if (const HDC dc = ::GetDC(parent)) {
            const auto font = ::SelectObject(dc, ::GetStockObject(DEFAULT_GUI_FONT));
            RECT measure{};
            ::DrawTextW(dc, label, label.GetLength(), &measure, DT_CALCRECT | DT_SINGLELINE);
            editRect.left += measure.right + 8;
            if (font) ::SelectObject(dc, font);
            ::ReleaseDC(parent, dc);
        }
    }
    editRect.right = m_comboArrow.left;
    editRect.DeflateRect(2, 2);
    if (editRect.IsRectEmpty()) {
        ::ShowWindow(edit, SW_HIDE);
        return;
    }
    ::SetWindowPos(edit, nullptr, editRect.left, editRect.top, editRect.Width(), editRect.Height(),
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void CMFCRibbonComboBox::DestroyEditWindow() {
    std::unique_ptr<CWnd> edit(m_editWindow);
    m_editWindow = nullptr;
    if (edit && edit->GetSafeHwnd()) {
        const HWND window = edit->Detach();
        ::RemoveWindowSubclass(window, RibbonComboEditProc, reinterpret_cast<UINT_PTR>(this));
        if (::IsWindow(window)) ::DestroyWindow(window);
    }
}

void CMFCRibbonComboBox::OnLButtonDown(CPoint point) {
    if (!(m_hasEditBox ? m_comboArrow : m_comboRect).PtInRect(point)) return;
    if (m_openList) CloseDropDown();
    else DropDownList();
}

HWND CMFCRibbonComboBox::CommandWindow() const {
    auto* bar = GetTopLevelRibbonBar();
    if (!bar) return nullptr;
    return bar->m_hWndOwner ? bar->m_hWndOwner : ::GetParent(bar->GetSafeHwnd());
}

void CMFCRibbonComboBox::OnSelectItem(int index) {
    if (index < 0 || !SelectItem(index)) return;
    if (const HWND owner = CommandWindow(); owner && GetID() && GetID() != UINT(-1)) {
        ::PostMessageW(owner, WM_COMMAND, MAKEWPARAM(GetID(), 0), 0);
    }
}

void CMFCRibbonComboBox::OnListHighlight(int index) {
    if (const HWND owner = CommandWindow(); owner && impl__AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM__3IA) {
        ::SendMessageW(owner, impl__AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM__3IA, index, reinterpret_cast<LPARAM>(this));
    }
}

void CMFCRibbonComboBox::DropDownList() {
    auto* bar = GetTopLevelRibbonBar();
    if (!bar || !bar->GetSafeHwnd() || m_comboRect.IsRectEmpty() || m_openList || !GetCount()) return;
    auto list = std::make_unique<RibbonComboList>(*this);
    list->edit = m_editWindow;
    list->maxHeight = m_dropDownHeight;
    list->minWidth = m_comboRect.Width();
    for (auto pos = m_itemStrings.GetHeadPosition(); pos != CStringList::POSITION(nullptr);) list->AddString(m_itemStrings.GetNext(pos));
    list->SetCurSel(m_selectedIndex);
    if (m_resizeDropDownList) list->EnableVertResize();
    POINT anchor{m_comboRect.left, m_comboRect.bottom};
    ::ClientToScreen(bar->GetSafeHwnd(), &anchor);
    list->Track(CPoint(anchor), bar);
    if (list->GetSafeHwnd()) {
        m_openList = list.release();
        m_droppedDown = TRUE;
        auto& state = openmfc::detail::mfccore::g_popupMenuStates.at(m_openList);
        state.anchor = *static_cast<const RECT*>(m_comboRect);
        ::MapWindowPoints(bar->GetSafeHwnd(), nullptr, reinterpret_cast<POINT*>(&state.anchor), 2);
        state.awaitingButtonRelease = true;
        ::InvalidateRect(bar->GetSafeHwnd(), m_comboRect, FALSE);
    }
}

void CMFCRibbonComboBox::CloseDropDown() {
    if (m_openList) m_openList->CloseMenu(TRUE);
}

void CMFCRibbonComboBox::OnListClosed(const CMFCPopupMenu* popup) {
    if (m_openList != popup) return;
    m_openList = nullptr;
    m_droppedDown = FALSE;
    if (auto* bar = GetTopLevelRibbonBar(); bar && bar->GetSafeHwnd()) {
        ::InvalidateRect(bar->GetSafeHwnd(), m_comboRect, FALSE);
    }
}

void CMFCRibbonComboBox::CopyFrom(const CMFCRibbonComboBox& source) {
    if (this == &source) return;
    RemoveAllItems();
    DestroyEditWindow();
    SetID(source.GetID());
    SetText(source.GetText());
    m_editWidth = source.m_editWidth;
    m_hasEditBox = source.m_hasEditBox;
    m_dropDownHeight = source.m_dropDownHeight;
    m_resizeDropDownList = source.m_resizeDropDownList;
    auto* data = DataList(const_cast<CMFCRibbonComboBox&>(source)).head;
    for (auto pos = source.m_itemStrings.GetHeadPosition(); pos != CStringList::POSITION(nullptr);) {
        AddItem(source.m_itemStrings.GetNext(pos), data ? data->value : 0);
        if (data) data = data->next;
    }
    m_selectedIndex = source.m_selectedIndex;
    SetEditText(source.m_editText);
}

// Symbol: ??0CMFCRibbonComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__IEAA_XZ(void* storage) {
    return storage ? new (storage) CMFCRibbonComboBox(0, FALSE) : nullptr;
}

// Symbol: ??0CMFCRibbonComboBox@@QEAA@IHHPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__QEAA_IHHPEB_WH_Z(
    void* storage, UINT id, BOOL hasEditBox, int width, const wchar_t* label, int image) {
    return storage ? new (storage) CMFCRibbonComboBox(id, hasEditBox, width, label, image) : nullptr;
}

// Symbol: ??1CMFCRibbonComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonComboBox__UEAA_XZ(CMFCRibbonComboBox* combo) {
    if (combo) combo->~CMFCRibbonComboBox();
}

// Symbol: ?CreateObject@CMFCRibbonComboBox@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonComboBox__SAPEAVCObject__XZ() {
    return new CMFCRibbonComboBox(0, FALSE);
}

// Symbol: ?AddItem@CMFCRibbonComboBox@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCRibbonComboBox__UEAA_JPEB_W_K_Z(
    CMFCRibbonComboBox* combo, const wchar_t* text, DWORD_PTR data) {
    return combo ? combo->AddItem(text, data) : -1;
}

// Base version has no item-data destructor callback.
// Symbol: ?ClearData@CMFCRibbonComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__ClearData_CMFCRibbonComboBox__MEAAXXZ(CMFCRibbonComboBox*) {}

// Symbol: ?CommonInit@CMFCRibbonComboBox@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonComboBox__IEAAXXZ(CMFCRibbonComboBox* combo) {
    if (!combo) return;
    combo->m_selectedIndex = -1;
    combo->m_dropDownHeight = 150;
    combo->m_resizeDropDownList = TRUE;
}

// Symbol: ?CopyFrom@CMFCRibbonComboBox@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonComboBox__MEAAXAEBVCMFCRibbonBaseElement___Z(
    CMFCRibbonComboBox* combo, const CMFCRibbonBaseElement* source) {
    if (combo && source && source->IsKindOf(CMFCRibbonComboBox::GetThisClass())) {
        combo->CopyFrom(*static_cast<const CMFCRibbonComboBox*>(source));
    }
}

// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAH_K@Z
extern "C" BOOL MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAH_K_Z(CMFCRibbonComboBox* combo, DWORD_PTR data) {
    if (!combo) return FALSE;
    int index = 0;
    for (auto* item = DataList(*combo).head; item; item = item->next, ++index) {
        if (item->value == data) return combo->DeleteItem(index);
    }
    return FALSE;
}

// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAHH@Z
extern "C" BOOL MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAHH_Z(CMFCRibbonComboBox* combo, int index) {
    return combo && combo->DeleteItem(index);
}

// Symbol: ?DeleteItem@CMFCRibbonComboBox@@QEAAHPEB_W@Z
extern "C" BOOL MS_ABI impl__DeleteItem_CMFCRibbonComboBox__QEAAHPEB_W_Z(CMFCRibbonComboBox* combo, const wchar_t* text) {
    return combo && combo->DeleteItem(combo->FindItem(text));
}

// Symbol: ?DropDownList@CMFCRibbonComboBox@@MEAAXXZ
extern "C" void MS_ABI impl__DropDownList_CMFCRibbonComboBox__MEAAXXZ(CMFCRibbonComboBox* combo) {
    if (combo) combo->DropDownList();
}

// Symbol: ?FindItem@CMFCRibbonComboBox@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindItem_CMFCRibbonComboBox__QEBAHPEB_W_Z(const CMFCRibbonComboBox* combo, const wchar_t* text) {
    return combo ? combo->FindItem(text) : -1;
}

// Symbol: ?GetIntermediateSize@CMFCRibbonComboBox@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetIntermediateSize_CMFCRibbonComboBox__UEAA_AVCSize__PEAVCDC___Z(
    CMFCRibbonComboBox* combo, CSize* result, CDC* dc) {
    if (result) *result = combo ? combo->GetIntermediateSize(dc) : CSize(0, 0);
    return result;
}

// Symbol: ?GetItem@CMFCRibbonComboBox@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCRibbonComboBox__QEBAPEB_WH_Z(const CMFCRibbonComboBox* combo, int index) {
    return combo ? combo->GetItem(index) : nullptr;
}

// Symbol: ?GetItemData@CMFCRibbonComboBox@@QEBA_KH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCRibbonComboBox__QEBA_KH_Z(const CMFCRibbonComboBox* combo, int index) {
    return combo ? combo->GetItemData(index) : 0;
}

// Symbol: ?OnAfterChangeRect@CMFCRibbonComboBox@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonComboBox__MEAAXPEAVCDC___Z(CMFCRibbonComboBox* combo, CDC*) {
    if (combo) combo->SetRect(combo->GetRect());
}

// Symbol: ?OnDraw@CMFCRibbonComboBox@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonComboBox__MEAAXPEAVCDC___Z(CMFCRibbonComboBox* combo, CDC* dc) {
    if (combo) combo->OnDraw(dc);
}

// The default list renderer handles rows when the combo supplies no override.
// Symbol: ?OnDrawDropListItem@CMFCRibbonComboBox@@MEAAHPEAVCDC@@HPEAVCMFCToolBarMenuButton@@H@Z
extern "C" BOOL MS_ABI impl__OnDrawDropListItem_CMFCRibbonComboBox__MEAAHPEAVCDC__HPEAVCMFCToolBarMenuButton__H_Z(
    void*, void*, int, void*, int) { return FALSE; }

// Symbol: ?OnGetDropListItemSize@CMFCRibbonComboBox@@MEAA?AVCSize@@PEAVCDC@@HPEAVCMFCToolBarMenuButton@@V2@@Z
extern "C" CSize* MS_ABI impl__OnGetDropListItemSize_CMFCRibbonComboBox__MEAA_AVCSize__PEAVCDC__HPEAVCMFCToolBarMenuButton__V2__Z(
    void*, CSize* result, void*, int, void*, long long) {
    if (result) *result = CSize(0, 0);
    return result;
}

// Symbol: ?OnLButtonDown@CMFCRibbonComboBox@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonComboBox__MEAAXVCPoint___Z(CMFCRibbonComboBox* combo, long long point) {
    if (combo) combo->OnLButtonDown(UnpackPoint(point));
}

// Symbol: ?OnLButtonUp@CMFCRibbonComboBox@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonComboBox__MEAAXVCPoint___Z(CMFCRibbonComboBox*, long long) {}

// Symbol: ?OnSelectItem@CMFCRibbonComboBox@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSelectItem_CMFCRibbonComboBox__UEAAXH_Z(CMFCRibbonComboBox* combo, int index) {
    if (combo) combo->OnSelectItem(index);
}

// Symbol: ?RemoveAllItems@CMFCRibbonComboBox@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCRibbonComboBox__QEAAXXZ(CMFCRibbonComboBox* combo) {
    if (combo) combo->RemoveAllItems();
}

// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAH_K@Z
extern "C" BOOL MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAH_K_Z(CMFCRibbonComboBox* combo, DWORD_PTR data) {
    return combo && combo->SelectItem(data);
}

// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAHH@Z
extern "C" BOOL MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAHH_Z(CMFCRibbonComboBox* combo, int index) {
    return combo && combo->SelectItem(index);
}

// Symbol: ?SelectItem@CMFCRibbonComboBox@@QEAAHPEB_W@Z
extern "C" BOOL MS_ABI impl__SelectItem_CMFCRibbonComboBox__QEAAHPEB_W_Z(CMFCRibbonComboBox* combo, const wchar_t* text) {
    return combo && combo->SelectItem(text);
}

// Symbol: ?SetACCData@CMFCRibbonComboBox@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" BOOL MS_ABI impl__SetACCData_CMFCRibbonComboBox__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonComboBox* combo, CWnd* parent, CAccessibilityData* data) {
    if (!combo || !data) return FALSE;
    data->m_strAccName = combo->GetText();
    data->m_strAccValue = combo->GetEditText();
    data->m_nAccRole = ROLE_SYSTEM_COMBOBOX;
    data->m_bAccState = combo->m_openList ? STATE_SYSTEM_EXPANDED : STATE_SYSTEM_COLLAPSED;
    if (!combo->m_hasEditBox) data->m_bAccState |= STATE_SYSTEM_READONLY;
    data->m_rectAccLocation = combo->GetRect();
    if (parent && parent->GetSafeHwnd()) {
        ::MapWindowPoints(parent->GetSafeHwnd(), nullptr, reinterpret_cast<POINT*>(&data->m_rectAccLocation), 2);
    }
    return TRUE;
}

// Symbol: ?SetDropDownHeight@CMFCRibbonComboBox@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDropDownHeight_CMFCRibbonComboBox__QEAAXH_Z(CMFCRibbonComboBox* combo, int height) {
    if (combo) combo->SetDropDownHeight(height);
}
