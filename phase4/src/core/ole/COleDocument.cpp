// COleDocument — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ?OnUpdateEditChangeIcon@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditChangeIcon_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateEditLinksMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditLinksMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateObjectVerbMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateObjectVerbPopup@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbPopup_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdatePasteLinkMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteLinkMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdatePasteMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
COleDocument::COleDocument()
    : CDocument(), m_bCompoundFile(FALSE), m_bRemember(TRUE) {
    memset(_coledocument_padding, 0, sizeof(_coledocument_padding));
}
COleDocument::~COleDocument() {
    DocumentState* state = GetDocumentState(this, false);
    if (state) {
        for (COleClientItem* item : state->items) {
            if (item && item->m_pContainerDoc == this) item->m_pContainerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    RemoveDocumentState(this);
}
void COleDocument::AddItem(COleClientItem* pItem) {
    if (pItem) AddDocumentItem(this, pItem);
}
void COleDocument::RemoveItem(COleClientItem* pItem) {
    RemoveDocumentItem(this, pItem);
}
COleClientItem* COleDocument::GetStartPosition() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state || state->items.empty()) return nullptr;
    return reinterpret_cast<COleClientItem*>(static_cast<uintptr_t>(1));
}
COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    uintptr_t index = reinterpret_cast<uintptr_t>(pos);
    if (!state || index == 0 || index > state->items.size()) {
        pos = nullptr;
        return nullptr;
    }
    COleClientItem* item = state->items[index - 1];
    pos = (index < state->items.size())
        ? reinterpret_cast<POSITION>(index + 1)
        : nullptr;
    return item;
}
COleClientItem* COleDocument::GetPrimarySelectedItem(CView* pView) const {
    (void)pView;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}
int COleDocument::GetItemCount() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    return state ? static_cast<int>(state->items.size()) : 0;
}
void COleDocument::EnableCompoundFile(BOOL bEnable) {
    m_bCompoundFile = bEnable;
}
BOOL COleDocument::HasBlankItems() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->GetItemState() == OLE_EMPTY) return TRUE;
    }
    return FALSE;
}
BOOL COleDocument::IsInPlaceActive() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->IsInPlaceActive()) return TRUE;
    }
    return FALSE;
}
void COleDocument::OnShowViews(BOOL bVisible) {
    POSITION pos = GetFirstViewPosition();
    while (pos) {
        CWnd* pView = static_cast<CWnd*>(GetNextView(pos));
        if (pView && pView->GetSafeHwnd()) {
            ::ShowWindow(pView->GetSafeHwnd(), bVisible ? SW_SHOW : SW_HIDE);
        }
    }
}
COleClientItem* COleDocument::OnGetLinkedItem(const wchar_t* lpszItemName) {
    DocumentState* state = GetDocumentState(this, false);
    if (!state || state->items.empty()) return nullptr;
    if (!lpszItemName || !*lpszItemName) return state->items.front();

    size_t index = ParseLinkedItemIndex(lpszItemName);
    if (index != 0 && index <= state->items.size()) {
        return state->items[index - 1];
    }

    for (COleClientItem* item : state->items) {
        if (!item) continue;
        CLSID clsid = CLSID_NULL;
        item->GetClassID(&clsid);
        LPOLEOBJECT object = item->GetObject();
        if (object) {
            LPOLESTR userType = nullptr;
            if (SUCCEEDED(object->GetUserType(USERCLASSTYPE_FULL, &userType)) && userType) {
                bool match = wcscmp(userType, lpszItemName) == 0;
                CoTaskMemFree(userType);
                if (match) return item;
            }
        }
    }
    return state->items.size() == 1 ? state->items.front() : nullptr;
}
COleClientItem* COleDocument::OnGetEmbeddedItem() {
    return GetPrimarySelectedItem(nullptr);
}
void COleDocument::OnEditChangeIcon(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    COleChangeIconDialog dlg(target);
    if (dlg.DoChangeIcon(target) == TRUE) {
        SetModifiedFlag(TRUE);
    }
}
void COleDocument::OnEditConvert(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    CLSID oldClass = {};
    target->GetClassID(&oldClass);
    COleConvertDialog dlg(target);
    if (dlg.DoModal() != IDOK) return;

    OLEUICONVERTW* cv = dlg.GetOleUIConvert();
    if (!cv) return;

    CLSID newClass = cv->clsidNew;
    if (IsEqualGUID(newClass, CLSID_NULL) || IsEqualGUID(oldClass, newClass)) {
        return;
    }

    if (dlg.IsConvertTo()) {
        if (target->ConvertTo(newClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    } else {
        if (target->ActivateAs(newClass, oldClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    }
}
void COleDocument::OnEditLinks() {
    COleLinksDialog dlg(this);
    if (dlg.DoModal() == IDOK) {
        SetModifiedFlag(TRUE);
    }
}
LPOLEITEMCONTAINER COleDocument::GetItemContainer() {
    return new(std::nothrow) OleItemContainerAdapter(this);
}
