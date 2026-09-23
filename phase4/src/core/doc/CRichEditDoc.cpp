// CRichEditDoc — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, richedit_ole_ext.cpp
//
// Retail layout (mfc140u ctor ??0CRichEditDoc@@IEAA@XZ, RVA 0x286c00, and the
// real afxrich.h on this host): CRichEditDoc derives from COleServerDoc and
// appends BOOL m_bRTF at +0x298 and BOOL m_bUpdateObjectCache at +0x29c (the
// ctor's single `movq $1,0x298(this)` sets m_bRTF = TRUE and
// m_bUpdateObjectCache = FALSE); sizeof == 0x2a0 (m_nObjectSize 672 in the
// retail descriptor returned by GetThisClass, mfc140u 0x287830).  Retail's item walks read
// COleDocument::m_docItemList.m_pNodeHead at +0x188.  Retail
// CRichEditCntrItem (sizeof 0x100) keeps BOOL m_bMark at +0xf8 and BOOL m_bLock
// at +0xfc.
//
// OpenMFC's CRichEditDoc (include/openmfc/afxole.h) derives from COleDocument,
// not COleServerDoc, declares neither m_bRTF nor m_bUpdateObjectCache (nor
// InvalidateObjectCache, the retail inline that sets the latter), and its
// CRichEditCntrItem declares neither m_bMark nor m_bLock.  None of the retail
// offsets above are dereferenced here.  The item list is walked through
// OpenMFC's COleDocument model (core/ole/COleDocument.cpp), m_bMark lives in the
// file-local side table below, and the view's HWND / IRichEditOle come from
// OpenMFC's CRichEditView model (the child control m_richEdit; retail's view
// *is* the control and caches IRichEditOle* at +0x110), exactly as
// core/view/CRichEditView.cpp does.
//
// Retail reaches its own virtuals through the vtable (GetView = slot 107
// /+0x358, GetStartPosition = slot 71 /+0x238, CreateClientItem = slot 106
// /+0x350).  OpenMFC's header declares none of those three on CRichEditDoc, so
// the bodies below call this file's thunks directly: a client override of them
// is not dispatched to (deviation).

#include "detail/CRichEditDocSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <richedit.h>
#include <richole.h>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Cross-file thunks (each definition located with grep; parameter lists follow
// the mangled names).
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);  // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditView__SAPEAUCRuntimeClass__XZ();          // core/view/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ();      // core/ole/RuntimeClasses.cpp
extern "C" void* MS_ABI impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(const COleDocument* pThis, void** pPos);  // core/ole/COleDocument.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                      // core/window/Thunks.cpp
extern "C" void MS_ABI impl__Close_COleClientItem__QEAAXW4tagOLECLOSE___Z(COleClientItem* pThis, tagOLECLOSE dwCloseOption);  // core/ole/Thunks.cpp
extern "C" void MS_ABI impl__UpdateItemType_COleClientItem__QEAAXXZ(COleClientItem* pThis);             // core/ole/COleClientItem.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                                       // detail/RegcoreSupport.cpp
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                    // core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                      // core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__DeleteContents_CRichEditView__UEAAXXZ(CRichEditView* pThis);               // core/view/CRichEditView.cpp
extern "C" void MS_ABI impl__Serialize_CRichEditView__UEAAXAEAVCArchive___Z(CRichEditView* pThis, CArchive& ar);  // core/view/Thunks.cpp
extern "C" void MS_ABI impl__DeleteContents_COleDocument__UEAAXXZ(COleDocument* pThis);                 // core/ole/COleDocument.cpp

// Same-file thunks the bodies below call before their definitions.
extern "C" CRichEditView* MS_ABI impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(const CRichEditDoc* pThis);
extern "C" void* MS_ABI impl__GetStartPosition_CRichEditDoc__UEBAPEAU__POSITION__XZ(const CRichEditDoc* pThis);
extern "C" CRichEditCntrItem* MS_ABI impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(const CRichEditDoc* pThis, REOBJECT* preo);
extern "C" void MS_ABI impl__MarkItemsClear_CRichEditDoc__IEBAXXZ(const CRichEditDoc* pThis);
extern "C" void MS_ABI impl__DeleteUnmarkedItems_CRichEditDoc__IEBAXXZ(const CRichEditDoc* pThis);

// Retail's CReObject ctor (mfc140u 0x283250) stores cbStruct = 0x48.
static_assert(sizeof(REOBJECT) == 0x48, "REOBJECT must match the retail CReObject cbStruct");

namespace {

// Side table standing in for retail CRichEditCntrItem::m_bMark (+0xf8), which
// OpenMFC's CRichEditCntrItem does not declare.  Only MarkItemsClear,
// UpdateObjectCache and DeleteUnmarkedItems (all in this file) read or write
// it.  An item with no entry reads as FALSE.  MarkItemsClear drops every entry
// of its document before re-marking, and ~CRichEditDoc drops them all, so the
// table only holds items of live documents.  (malloc/free rather than a
// container so this unit adds no new C++ runtime symbols.)
struct ItemMark {
    const CRichEditDoc* doc;
    const CDocItem* item;
    BOOL bMark;
    ItemMark* next;
};
ItemMark* g_itemMarks = nullptr;

void DropMarks(const CRichEditDoc* doc, const CDocItem* item /* nullptr = all of doc */) {
    ItemMark** link = &g_itemMarks;
    while (*link != nullptr) {
        ItemMark* e = *link;
        if (e->doc == doc && (item == nullptr || e->item == item)) {
            *link = e->next;
            std::free(e);
        } else {
            link = &e->next;
        }
    }
}
void SetMark(const CRichEditDoc* doc, const CDocItem* item, BOOL bMark) {
    for (ItemMark* e = g_itemMarks; e != nullptr; e = e->next) {
        if (e->doc == doc && e->item == item) { e->bMark = bMark; return; }
    }
    ItemMark* e = static_cast<ItemMark*>(std::malloc(sizeof(ItemMark)));
    if (e == nullptr) return;
    e->doc = doc; e->item = item; e->bMark = bMark; e->next = g_itemMarks;
    g_itemMarks = e;
}
BOOL GetMark(const CRichEditDoc* doc, const CDocItem* item) {
    for (ItemMark* e = g_itemMarks; e != nullptr; e = e->next) {
        if (e->doc == doc && e->item == item) return e->bMark;
    }
    return FALSE;
}

// IsKindOf(RUNTIME_CLASS(CRichEditCntrItem)) against the exported descriptor
// (retail compares against the static at RVA 0x3b1a48 (mfc140u), the one its
// GetThisClass returns).  Caveat, measured with nm on build-phase4/obj: the
// GetThisClass thunk returns the global `classCRichEditCntrItem` defined in
// core/ole/RuntimeClasses.cpp, while an item built by OpenMFC's own C++
// constructor reports CRichEditCntrItem::classCRichEditCntrItem (defined by
// IMPLEMENT_DYNAMIC in detail/CRichEditDocSupport.cpp) -- two distinct objects.
// Until those are unified, this test is FALSE for items created through
// OpenMFC's ctor thunk, so the mark/delete pass below leaves them alone.
bool IsRichEditItem(const CDocItem* pItem) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               pItem, impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ()) != 0;
}

// The view's rich-edit control window.  Retail sends to pView->m_hWnd (the
// retail view is the control); OpenMFC's view hosts the control as m_richEdit.
HWND ViewCtrlHwnd(const CRichEditView* pView) {
    return pView->m_richEdit.m_hWnd;
}

// Retail reads the view's cached IRichEditOle* (pView+0x110).  OpenMFC does
// not cache it, so it is fetched with EM_GETOLEINTERFACE (which AddRefs) and
// the caller Releases it -- the same model core/view/CRichEditView.cpp uses.
IRichEditOle* ViewRichEditOle(const CRichEditView* pView) {
    HWND hWnd = ViewCtrlHwnd(pView);
    if (hWnd == nullptr) return nullptr;
    IRichEditOle* pOle = nullptr;
    ::SendMessageW(hWnd, EM_GETOLEINTERFACE, 0, reinterpret_cast<LPARAM>(&pOle));
    return pOle;
}

// CStringA(LPCWSTR) stand-in: a malloc'd ANSI copy (free with std::free), ""
// for NULL.  Converted with CP_THREAD_ACP, ATL's default conversion code page;
// retail's conversion chain below the CStringA ctor was not traced.
char* ToAnsi(const wchar_t* lpsz) {
    if (lpsz == nullptr) lpsz = L"";
    int cb = ::WideCharToMultiByte(CP_THREAD_ACP, 0, lpsz, -1, nullptr, 0, nullptr, nullptr);
    if (cb <= 0) cb = 1;
    char* p = static_cast<char*>(std::malloc(static_cast<size_t>(cb)));
    if (p == nullptr) return nullptr;
    p[0] = '\0';
    ::WideCharToMultiByte(CP_THREAD_ACP, 0, lpsz, -1, p, cb, nullptr, nullptr);
    return p;
}

} // namespace

// Symbol: ??0CRichEditDoc@@IEAA@XZ
// CRichEditDoc::CRichEditDoc()
extern "C" void* MS_ABI impl___0CRichEditDoc__IEAA_XZ() {
    return new CRichEditDoc();
}
// Symbol: ?GetInPlaceActiveItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCWnd___Z(CRichEditDoc* pThis, CWnd* pWnd) {
    return pThis ? pThis->GetInPlaceActiveItem(pWnd) : nullptr;
}
// Symbol: ?GetPrimarySelectedItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCView@@@Z
extern "C" void* MS_ABI impl__GetPrimarySelectedItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCView___Z(CRichEditDoc* pThis, CView* pView) {
    return pThis ? pThis->GetPrimarySelectedItem(pView) : nullptr;
}
// Symbol: ?LookupItem@CRichEditDoc@@QEBAPEAVCRichEditCntrItem@@PEAUIOleObject@@@Z
extern "C" void* MS_ABI impl__LookupItem_CRichEditDoc__QEBAPEAVCRichEditCntrItem__PEAUIOleObject___Z(CRichEditDoc* pThis, IOleObject* pOleObject) {
    return pThis ? pThis->LookupItem(pOleObject) : nullptr;
}
// Symbol: ?OnGetEmbeddedItem@CRichEditDoc@@MEAAPEAVCOleServerItem@@XZ
extern "C" COleServerItem* MS_ABI impl__OnGetEmbeddedItem_CRichEditDoc__MEAAPEAVCOleServerItem__XZ(CRichEditDoc* pThis) {
    COleClientItem* item = pThis ? pThis->GetPrimarySelectedItem(nullptr) : nullptr;
    return dynamic_cast<COleServerItem*>(item);
}
CRichEditDoc::CRichEditDoc() : COleDocument() {
    memset(_richeditdoc_padding, 0, sizeof(_richeditdoc_padding));
}
CRichEditDoc::~CRichEditDoc() {
    DropMarks(this, nullptr);   // OpenMFC-only m_bMark side table (see top of file)
}
COleClientItem* CRichEditDoc::GetInPlaceActiveItem(CWnd* pWnd) {
    (void)pWnd;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->IsInPlaceActive()) {
            return item;
        }
    }
    return nullptr;
}
COleClientItem* CRichEditDoc::GetPrimarySelectedItem(CView* pView) {
    (void)pView;
    COleClientItem* active = GetInPlaceActiveItem(nullptr);
    if (active) {
        return active;
    }

    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}
CRichEditCntrItem* CRichEditDoc::LookupItem(IOleObject* pOleObject) const {
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->GetObject() == pOleObject) {
            return static_cast<CRichEditCntrItem*>(item);
        }
    }
    return nullptr;
}
// CRichEditDoc::CreateClientItem(REOBJECT*) const -- mfc140u 0x286ea0:
//     return new CRichEditCntrItem(preo, const_cast<CRichEditDoc*>(this));
//         // operator new(0x100), then ??0CRichEditCntrItem@@QEAA@PEAU_reobject@@PEAVCRichEditDoc@@@Z
//         // (mfc140u 0x2872e0); the ctor is skipped when the allocation returns NULL.
// STUB: not reproducible from this unit.  Calling the C++ constructor
// CRichEditCntrItem(COleDocument*) adds a new C++ undefined symbol, which the
// link audit rejects, and the exported ctor thunk in core/ole/CRichEditCntrItem.cpp
// has a placeholder parameter list (void* pObject, CRichEditDoc*) that ignores
// the caller's storage and heap-allocates, so it cannot be called with the
// (this, preo, pDoc) arguments its mangled name describes.  Neither path takes
// the REOBJECT: OpenMFC's CRichEditCntrItem(COleDocument*) is the only ctor,
// so an item made here would not wrap preo->poleobj, LookupItem would never
// find it, and every UpdateObjectCache call would add another one.  Returns NULL.
// Symbol: ?CreateClientItem@CRichEditDoc@@UEBAPEAVCRichEditCntrItem@@PEAU_reobject@@@Z
extern "C" CRichEditCntrItem* MS_ABI impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(const CRichEditDoc* pThis, REOBJECT* preo) {
    (void)pThis; (void)preo;
    return nullptr;
}

// CRichEditDoc::DeleteContents() -- mfc140u 0x287150:
//     COleServerDoc::DeleteContents();              // mfc140u 0x265960
//     CWaitCursor wait;                             // AfxGetModuleState()->m_pCurrentWinApp->BeginWaitCursor()
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView != NULL) {
//         pView->DeleteContents();                  // view vtable slot 121 (+0x3c8)
//         ::SendMessage(pView->m_hWnd, EM_SETMODIFY, 0, 0);   // import SendMessageW
//     }
//     // ~CWaitCursor: m_pCurrentWinApp->EndWaitCursor()
// COleServerDoc::DeleteContents (mfc140u 0x265960) first calls
// COleDocument::DeleteContents (mfc140u 0x2541a0: release, unlist and drop
// every COleClientItem), then makes three passes over the COleServerItems
// (InternalAddRef each; delete those with m_bAutoDelete; InternalRelease each).
// Deviations: OpenMFC's CRichEditDoc is a COleDocument, not a COleServerDoc, so
// the base step here is the COleDocument::DeleteContents thunk
// (core/ole/COleDocument.cpp; OpenMFC's header does not declare that override,
// hence the direct thunk call) and the three COleServerItem passes are not
// reproduced.  The view's DeleteContents is called through its thunk rather
// than virtually.  The wait cursor uses AfxGetApp() and is skipped when there
// is no app object.
// Symbol: ?DeleteContents@CRichEditDoc@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CRichEditDoc__UEAAXXZ(CRichEditDoc* pThis) {
    if (!pThis) return;
    impl__DeleteContents_COleDocument__UEAAXXZ(pThis);
    DropMarks(pThis, nullptr);   // OpenMFC-only: the items just deleted may leave side-table entries
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView != nullptr) {
        impl__DeleteContents_CRichEditView__UEAAXXZ(pView);
        ::SendMessageW(ViewCtrlHwnd(pView), EM_SETMODIFY, 0, 0);
    }
    if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
}

// CRichEditDoc::DeleteUnmarkedItems() const -- mfc140u 0x286f40:
//     for (CNode* p = m_docItemList.m_pNodeHead; p != NULL; ) {   // +0x188
//         CDocItem* pItem = (CDocItem*)p->data; p = p->pNext;     // next taken first
//         if (pItem->IsKindOf(RUNTIME_CLASS(CRichEditCntrItem)) &&
//             !pItem->m_bMark /*+0xf8*/ && !pItem->m_bLock /*+0xfc*/)
//             delete pItem;                                       // vtable slot 1, flag 1
//     }
// Here the list is OpenMFC's COleDocument item list, whose POSITION is an
// index that deleting an item (which unlists itself in ~COleClientItem)
// would shift, so the candidates are collected first and deleted afterwards.
// m_bMark comes from the side table; m_bLock has no OpenMFC counterpart and
// nothing here can set it, so it reads as FALSE.
// Symbol: ?DeleteUnmarkedItems@CRichEditDoc@@IEBAXXZ
extern "C" void MS_ABI impl__DeleteUnmarkedItems_CRichEditDoc__IEBAXXZ(const CRichEditDoc* pThis) {
    if (!pThis) return;
    size_t count = 0;
    void* pos = reinterpret_cast<void*>(pThis->GetStartPosition());
    while (pos != nullptr) {
        CDocItem* pItem = static_cast<CDocItem*>(
            impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, &pos));
        if (pItem && IsRichEditItem(pItem) && !GetMark(pThis, pItem)) ++count;
    }
    if (count == 0) return;
    CDocItem** victims = static_cast<CDocItem**>(std::malloc(count * sizeof(CDocItem*)));
    if (victims == nullptr) return;
    size_t n = 0;
    pos = reinterpret_cast<void*>(pThis->GetStartPosition());
    while (pos != nullptr && n < count) {
        CDocItem* pItem = static_cast<CDocItem*>(
            impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, &pos));
        if (pItem && IsRichEditItem(pItem) && !GetMark(pThis, pItem)) victims[n++] = pItem;
    }
    for (size_t i = 0; i < n; ++i) {
        DropMarks(pThis, victims[i]);
        delete victims[i];
    }
    std::free(victims);
}

// CRichEditDoc::GetStartPosition() const -- mfc140u 0x286fb0:
//     if (m_bUpdateObjectCache)                     // +0x29c
//         ((CRichEditDoc*)this)->UpdateObjectCache();
//     return m_docItemList.GetHeadPosition();       // +0x188
// OpenMFC has no m_bUpdateObjectCache and no InvalidateObjectCache to set it
// (retail's ctor starts the flag FALSE), so the refresh branch is omitted:
// this body never calls UpdateObjectCache (deviation).  What remains is the
// item list's head position in OpenMFC's COleDocument model.
// Symbol: ?GetStartPosition@CRichEditDoc@@UEBAPEAU__POSITION@@XZ
extern "C" void* MS_ABI impl__GetStartPosition_CRichEditDoc__UEBAPEAU__POSITION__XZ(const CRichEditDoc* pThis) {
    if (!pThis) return nullptr;
    return reinterpret_cast<void*>(pThis->GetStartPosition());   // COleDocument::GetStartPosition
}

// CRichEditDoc::GetView() const -- mfc140u 0x286c30:
//     POSITION pos = GetFirstViewPosition();        // vtable slot 28 (+0xe0)
//     while (pos != NULL) {
//         CView* pView = GetNextView(pos);          // vtable slot 29 (+0xe8)
//         if (pView->IsKindOf(RUNTIME_CLASS(CRichEditView)))   // CObject::IsKindOf, mfc140u 0x234cf0
//             return (CRichEditView*)pView;
//     }
//     return NULL;
// Symbol: ?GetView@CRichEditDoc@@UEBAPEAVCRichEditView@@XZ
extern "C" CRichEditView* MS_ABI impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(const CRichEditDoc* pThis) {
    if (!pThis) return nullptr;
    void* pos = pThis->GetFirstViewPosition();
    while (pos != nullptr) {
        CView* pView = pThis->GetNextView(pos);
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pView, impl__GetThisClass_CRichEditView__SAPEAUCRuntimeClass__XZ())) {
            return static_cast<CRichEditView*>(pView);
        }
    }
    return nullptr;
}

// CRichEditDoc::IsModified() -- mfc140u 0x286ca0:
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView == NULL) return FALSE;
//     return (BOOL)::SendMessage(pView->m_hWnd, EM_GETMODIFY, 0, 0);   // tail-jump, import SendMessageW
// Symbol: ?IsModified@CRichEditDoc@@UEAAHXZ
extern "C" int MS_ABI impl__IsModified_CRichEditDoc__UEAAHXZ(CRichEditDoc* pThis) {
    if (!pThis) return FALSE;
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView == nullptr) return FALSE;
    return static_cast<int>(::SendMessageW(ViewCtrlHwnd(pView), EM_GETMODIFY, 0, 0));
}

// CRichEditDoc::MarkItemsClear() const -- mfc140u 0x286ee0:
//     for (CNode* p = m_docItemList.m_pNodeHead; p != NULL; p = p->pNext) {   // +0x188
//         CDocItem* pItem = (CDocItem*)p->data;
//         if (pItem->IsKindOf(RUNTIME_CLASS(CRichEditCntrItem)))
//             pItem->m_bMark /*+0xf8*/ = (pItem->m_lpObject /*+0x48*/ == NULL);
//     }
// m_bMark lives in the side table; this document's stale entries are dropped
// first.  m_lpObject is OpenMFC's public COleClientItem::m_lpObject.
// Symbol: ?MarkItemsClear@CRichEditDoc@@IEBAXXZ
extern "C" void MS_ABI impl__MarkItemsClear_CRichEditDoc__IEBAXXZ(const CRichEditDoc* pThis) {
    if (!pThis) return;
    DropMarks(pThis, nullptr);
    void* pos = reinterpret_cast<void*>(pThis->GetStartPosition());
    while (pos != nullptr) {
        CDocItem* pItem = static_cast<CDocItem*>(
            impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, &pos));
        if (pItem && IsRichEditItem(pItem)) {
            const COleClientItem* pClient = static_cast<const COleClientItem*>(pItem);
            SetMark(pThis, pItem, pClient->m_lpObject == nullptr ? TRUE : FALSE);
        }
    }
}

// CRichEditDoc::PreCloseFrame(CFrameWnd*) -- mfc140u 0x287200:
//     BOOL bSetRedraw = FALSE;
//     if (pFrame->GetStyle() & WS_VISIBLE) {        // CWnd::GetStyle, mfc140u 0x2a9690
//         ::SendMessage(pFrame->m_hWnd, WM_SETREDRAW, FALSE, 0);   // import SendMessageW
//         bSetRedraw = TRUE;
//     }
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView != NULL)
//         pView->m_lpRichEditOle->InPlaceDeactivate();   // +0x110, IRichEditOle slot 15
//     POSITION pos = GetStartPosition();            // vtable slot 71 (+0x238)
//     while (pos != NULL) {
//         COleClientItem* pItem = GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem));  // mfc140u 0x254120
//         if (pItem == NULL) break;
//         pItem->Close(OLECLOSE_SAVEIFDIRTY);       // mfc140u 0x244630
//     }
//     if (bSetRedraw)
//         ::SendMessage(pFrame->m_hWnd, WM_SETREDRAW, TRUE, 0);
// Retail does not call a base-class PreCloseFrame.  Deviations: a NULL pFrame
// skips the redraw bracket instead of faulting, and IRichEditOle is fetched
// from the control per call (see ViewRichEditOle).
// Symbol: ?PreCloseFrame@CRichEditDoc@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_CRichEditDoc__UEAAXPEAVCFrameWnd___Z(CRichEditDoc* pThis, CFrameWnd* pFrame) {
    if (!pThis) return;
    BOOL bSetRedraw = FALSE;
    if (pFrame != nullptr && (impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_VISIBLE)) {
        ::SendMessageW(pFrame->m_hWnd, WM_SETREDRAW, FALSE, 0);
        bSetRedraw = TRUE;
    }
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView != nullptr) {
        IRichEditOle* pOle = ViewRichEditOle(pView);
        if (pOle != nullptr) {
            pOle->InPlaceDeactivate();
            pOle->Release();
        }
    }
    void* pos = impl__GetStartPosition_CRichEditDoc__UEBAPEAU__POSITION__XZ(pThis);
    while (pos != nullptr) {
        COleClientItem* pItem = pThis->GetNextClientItem(pos);   // GetNextItemOfKind(pos, COleClientItem)
        if (pItem == nullptr) break;
        impl__Close_COleClientItem__QEAAXW4tagOLECLOSE___Z(pItem, OLECLOSE_SAVEIFDIRTY);
    }
    if (bSetRedraw) {
        ::SendMessageW(pFrame->m_hWnd, WM_SETREDRAW, TRUE, 0);
    }
}

// CRichEditDoc::Serialize(CArchive&) -- mfc140u 0x2871c0:
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView != NULL) pView->Serialize(ar);      // view vtable slot 2 (+0x10)
// No base-class Serialize is called.  The view's Serialize is reached through
// its thunk, which dispatches virtually through OpenMFC's vtable.
// Symbol: ?Serialize@CRichEditDoc@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CRichEditDoc__UEAAXAEAVCArchive___Z(CRichEditDoc* pThis, CArchive& ar) {
    if (!pThis) return;
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView != nullptr) impl__Serialize_CRichEditView__UEAAXAEAVCArchive___Z(pView, ar);
}

// CRichEditDoc::SetModifiedFlag(BOOL) -- mfc140u 0x286ce0:
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView != NULL)
//         ::SendMessage(pView->m_hWnd, EM_SETMODIFY, (WPARAM)bModified, 0);  // import SendMessageW
// The document's own modified flag is not touched.
// Symbol: ?SetModifiedFlag@CRichEditDoc@@UEAAXH@Z
extern "C" void MS_ABI impl__SetModifiedFlag_CRichEditDoc__UEAAXH_Z(CRichEditDoc* pThis, int bModified) {
    if (!pThis) return;
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView != nullptr) {
        ::SendMessageW(ViewCtrlHwnd(pView), EM_SETMODIFY, static_cast<WPARAM>(static_cast<INT_PTR>(bModified)), 0);
    }
}

// CRichEditDoc::SetPathName(LPCTSTR, BOOL) -- mfc140u 0x286d60 is a single
// `jmp` to CDocument::SetPathName (mfc140u 0x21afe0), so
// COleDocument::SetPathName (mfc140u 0x254260) and any other intermediate
// override are not run.
// Symbol: ?SetPathName@CRichEditDoc@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_CRichEditDoc__UEAAXPEB_WH_Z(CRichEditDoc* pThis, const wchar_t* lpszPathName, int bAddToMRU) {
    if (!pThis) return;
    pThis->CDocument::SetPathName(lpszPathName, bAddToMRU);
}

// CRichEditDoc::SetTitle(LPCTSTR) -- mfc140u 0x286d70:
//     CDocument::SetTitle(lpszTitle);               // mfc140u 0x21ad60 (called directly)
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     CStringA strApp(AfxGetModuleState()->m_lpszCurrentAppName);   // module state +0x20
//     CStringA strTitle(lpszTitle);
//     pView->m_lpRichEditOle->SetHostNames(strApp, strTitle);      // +0x110, IRichEditOle slot 10
// Retail dereferences pView without a NULL check.  Deviations: no view (or no
// IRichEditOle) skips the SetHostNames call instead of faulting; the app name
// is CWinApp::m_pszAppName, which is what retail's CWinApp::SetCurrentHandles
// copies into the module state (as core/ole/COleDocument.cpp's SetPathName
// also does); the ANSI conversion is ToAnsi above.
// Symbol: ?SetTitle@CRichEditDoc@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTitle_CRichEditDoc__UEAAXPEB_W_Z(CRichEditDoc* pThis, const wchar_t* lpszTitle) {
    if (!pThis) return;
    pThis->CDocument::SetTitle(lpszTitle);
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView == nullptr) return;
    IRichEditOle* pOle = ViewRichEditOle(pView);
    if (pOle == nullptr) return;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    char* strApp = ToAnsi(pApp ? pApp->m_pszAppName : nullptr);
    char* strTitle = ToAnsi(lpszTitle);
    if (strApp != nullptr && strTitle != nullptr) pOle->SetHostNames(strApp, strTitle);
    std::free(strApp);
    std::free(strTitle);
    pOle->Release();
}

// CRichEditDoc::UpdateModifiedFlag() -- the export resolves to mfc140u 0x27d0,
// a bare `ret` that the linker shares with other empty bodies (the mfc140u
// symbol map names it ?AddDockSite@CFrameWndEx@@QEAAXXZ).  Retail therefore
// does nothing here -- in particular it does not run COleDocument's
// item-modified scan -- and neither does this body.
// Symbol: ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateModifiedFlag_CRichEditDoc__UEAAXXZ(CRichEditDoc* pThis) {
    (void)pThis;
}

// CRichEditDoc::UpdateObjectCache() -- mfc140u 0x286fe0:
//     CRichEditView* pView = GetView();             // vtable slot 107 (+0x358)
//     if (pView != NULL) {
//         MarkItemsClear();                         // mfc140u 0x286ee0
//         long nCount = pView->m_lpRichEditOle->GetObjectCount();   // IRichEditOle slot 4
//         for (long i = 0; i < nCount; i++) {
//             CReObject reo;                        // mfc140u 0x283250: cbStruct = 0x48, rest zeroed
//             if (pView->m_lpRichEditOle->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES) == S_OK) {  // slot 6
//                 CRichEditCntrItem* pItem = LookupItem(reo.poleobj);   // mfc140u 0x286e40
//                 if (pItem == NULL) {
//                     pItem = CreateClientItem(&reo);                   // vtable slot 106 (+0x350)
//                     pItem->UpdateItemType();                          // mfc140u 0x244b20
//                 }
//                 pItem->m_bMark = TRUE;                                // +0xf8
//             }
//             // ~CReObject (mfc140u 0x283310): Release poleobj, pstg, polesite
//         }
//         DeleteUnmarkedItems();                    // mfc140u 0x286f40
//     }
//     m_bUpdateObjectCache = FALSE;                 // +0x29c
// Deviations: IRichEditOle is fetched from the control per call (see
// ViewRichEditOle); when the control has none (no window yet) the function
// returns before MarkItemsClear, because running the mark/delete pass without
// the object enumeration would delete every rich-edit item that holds an
// object (retail would fault on the NULL interface instead); CreateClientItem is this file's thunk (a NULL-returning stub, see above), so
// a NULL item is skipped instead of dereferenced; m_bMark is the side table;
// m_bUpdateObjectCache is not modeled (see GetStartPosition).
// Symbol: ?UpdateObjectCache@CRichEditDoc@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateObjectCache_CRichEditDoc__IEAAXXZ(CRichEditDoc* pThis) {
    if (!pThis) return;
    CRichEditView* pView = impl__GetView_CRichEditDoc__UEBAPEAVCRichEditView__XZ(pThis);
    if (pView == nullptr) return;
    IRichEditOle* pOle = ViewRichEditOle(pView);
    if (pOle == nullptr) return;
    impl__MarkItemsClear_CRichEditDoc__IEBAXXZ(pThis);
    const LONG nCount = pOle->GetObjectCount();
    for (LONG i = 0; i < nCount; i++) {
        REOBJECT reo;                              // CReObject: cbStruct set, rest zeroed
        std::memset(&reo, 0, sizeof(reo));
        reo.cbStruct = sizeof(REOBJECT);
        if (pOle->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES) == S_OK) {
            CRichEditCntrItem* pItem = pThis->LookupItem(reo.poleobj);
            if (pItem == nullptr) {
                pItem = impl__CreateClientItem_CRichEditDoc__UEBAPEAVCRichEditCntrItem__PEAU_reobject___Z(pThis, &reo);
                if (pItem != nullptr) impl__UpdateItemType_COleClientItem__QEAAXXZ(pItem);
            }
            if (pItem != nullptr) SetMark(pThis, pItem, TRUE);
        }
        // ~CReObject
        if (reo.poleobj) reo.poleobj->Release();
        if (reo.pstg) reo.pstg->Release();
        if (reo.polesite) reo.polesite->Release();
    }
    pOle->Release();
    impl__DeleteUnmarkedItems_CRichEditDoc__IEBAXXZ(pThis);
}
