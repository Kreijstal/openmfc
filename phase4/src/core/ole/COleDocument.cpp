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

// === Moved from ManualThunks.cpp ===
// Symbol: ?AddItem@COleDocument@@UEAAXPEAVCDocItem@@@Z
extern "C" void MS_ABI impl__AddItem_COleDocument__UEAAXPEAVCDocItem___Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
}


// Symbol: ?OnEditChangeIcon@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditChangeIcon_COleDocument__IEAAXXZ(void* unused) {
    (void)unused;
}


// Symbol: ?OnEditConvert@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditConvert_COleDocument__IEAAXXZ(void* unused) {
    (void)unused;
}


// Symbol: ?RemoveItem@COleDocument@@UEAAXPEAVCDocItem@@@Z
extern "C" void MS_ABI impl__RemoveItem_COleDocument__UEAAXPEAVCDocItem___Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
}

// Symbol: ?ApplyPrintDevice@COleDocument@@QEAAHPEBUtagDVTARGETDEVICE@@@Z
extern "C" int MS_ABI impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagDVTARGETDEVICE___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ApplyPrintDevice@COleDocument@@QEAAHPEBUtagPDW@@@Z
extern "C" int MS_ABI impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagPDW___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?CommitItems@COleDocument@@QEAAXHPEAUIStorage@@@Z
extern "C" void MS_ABI impl__CommitItems_COleDocument__QEAAXHPEAUIStorage___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?DeleteContents@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_COleDocument__UEAAXXZ() {}

// Symbol: ?DoSave@COleDocument@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DoSave_COleDocument__UEAAHPEB_WH_Z(const wchar_t* p0, int p1) {
    return 0;
}

// Symbol: ?GetContainer@COleDocument@@UEAAPEAUIOleItemContainer@@XZ
extern "C" void* MS_ABI impl__GetContainer_COleDocument__UEAAPEAUIOleItemContainer__XZ() {
    return nullptr;
}

// Symbol: ?GetFirstFrame@COleDocument@@QEAAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstFrame_COleDocument__QEAAPEAVCFrameWnd__XZ() {
    return nullptr;
}

// Symbol: ?GetInPlaceActiveItem@COleDocument@@UEAAPEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetMoniker@COleDocument@@UEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" void* MS_ABI impl__GetMoniker_COleDocument__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(int /*enum*/ p0, void* p1, void* p2, void* p3, void* p4, void* p5, unsigned char p6, unsigned short p7, unsigned char p8, void* p9, float p10, void* p11, double p12, unsigned int p13, unsigned long p14, unsigned char p15, void* p16) {
    return nullptr;
}

// Symbol: ?GetNextItem@COleDocument@@UEBAPEAVCDocItem@@AEAPEAU__POSITION@@@Z
extern "C" void* MS_ABI impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(void* /*struct*/** p0) {
    return nullptr;
}

// Symbol: ?GetNextItemOfKind@COleDocument@@IEBAPEAVCDocItem@@AEAPEAU__POSITION@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(void* /*struct*/** p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?GetNextServerItem@COleDocument@@QEBAPEAVCOleServerItem@@AEAPEAU__POSITION@@@Z
extern "C" void* MS_ABI impl__GetNextServerItem_COleDocument__QEBAPEAVCOleServerItem__AEAPEAU__POSITION___Z(void* /*struct*/** p0) {
    return nullptr;
}

// Symbol: ?LoadFromStorage@COleDocument@@MEAAXXZ
extern "C" void MS_ABI impl__LoadFromStorage_COleDocument__MEAAXXZ() {}

// Symbol: ?OnCloseDocument@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_COleDocument__UEAAXXZ() {}

// Symbol: ?OnCmdMsg@COleDocument@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(unsigned int p0, int p1, void* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?OnFileSendMail@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSendMail_COleDocument__IEAAXXZ() {}

// Symbol: ?OnIdle@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnIdle_COleDocument__UEAAXXZ() {}

// Symbol: ?OnLoadDocumentFromStream@COleDocument@@MEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__OnLoadDocumentFromStream_COleDocument__MEAAJPEAUIStream__K_Z(void* /*struct*/* p0, unsigned long p1) {
    return 0;
}

// Symbol: ?OnNewDocument@COleDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_COleDocument__UEAAHXZ() {
    return 0;
}

// Symbol: ?OnOpenDocument@COleDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_COleDocument__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSaveDocument@COleDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_COleDocument__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?PreCloseFrame@COleDocument@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_COleDocument__UEAAXPEAVCFrameWnd___Z(void* /*class*/* p0) {}

// Symbol: ?SaveModified@COleDocument@@UEAAHXZ
extern "C" int MS_ABI impl__SaveModified_COleDocument__UEAAHXZ() {
    return 0;
}

// Symbol: ?SaveToStorage@COleDocument@@MEAAXPEAVCObject@@@Z
extern "C" void MS_ABI impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(void* /*class*/* p0) {}

// Symbol: ?Serialize@COleDocument@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetPathName@COleDocument@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_COleDocument__UEAAXPEB_WH_Z(const wchar_t* p0, int p1) {}

// Symbol: ?UpdateModifiedFlag@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateModifiedFlag_COleDocument__UEAAXXZ() {}
