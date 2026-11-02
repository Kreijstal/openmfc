// COleDocObjectItem — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleDocObjectItem@@QEAA@PEAVCOleDocument@@@Z
// COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc)
extern "C" void* MS_ABI impl___0COleDocObjectItem__QEAA_PEAVCOleDocument___Z(COleDocument* pContainerDoc) {
    return new COleDocObjectItem(pContainerDoc);
}
COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc) : COleClientItem(pContainerDoc) { memset(_coledocobjectitem_padding, 0, sizeof(_coledocobjectitem_padding)); }
COleDocObjectItem::~COleDocObjectItem() {}
BOOL COleDocObjectItem::IsDocObject() const {
    if (!m_lpObject) return FALSE;
    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (document) document->Release();
    return SUCCEEDED(hr);
}
BOOL COleDocObjectItem::IsActive() const { return m_bInPlaceActive && IsOpen(); }
IOleDocumentView* COleDocObjectItem::GetActiveView() const {
    if (!m_lpObject) return nullptr;
    IOleDocumentView* view = nullptr;
    if (m_lpInPlaceObject &&
        SUCCEEDED(m_lpInPlaceObject->QueryInterface(IID_IOleDocumentView, reinterpret_cast<void**>(&view)))) {
        return view;
    }

    IOleDocument* document = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document))) && document) {
        if (SUCCEEDED(document->CreateView(nullptr, nullptr, 0, &view)) && view) {
            InitializeDocumentView(const_cast<COleDocObjectItem*>(this), view);
        }
        document->Release();
    }
    return view;
}
HRESULT COleDocObjectItem::GetDocument(IUnknown** ppDocument) {
    if (!ppDocument) return E_POINTER;
    *ppDocument = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleDocument* document = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleDocument, reinterpret_cast<void**>(&document));
    if (SUCCEEDED(hr) && document) {
        *ppDocument = static_cast<IUnknown*>(document);
        return S_OK;
    }
    return hr;
}
void COleDocObjectItem::ActivateAndShow() {
    if (!m_lpObject) return;
    Activate(OLEIVERB_SHOW);
    IOleDocumentView* view = GetActiveView();
    if (view) {
        view->Show(TRUE);
        view->Release();
    }
}
BOOL COleDocObjectItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
void COleDocObjectItem::OnActivateView() {
    if (!m_lpObject) return;

    if (!IsOpen()) {
        OnOpen();
        if (m_nStatus == OLE_OPEN) {
            Activate(OLEIVERB_OPEN, nullptr, nullptr, nullptr, nullptr, FALSE);
        }
    }

    IOleDocumentView* view = GetActiveView();
    if (!view) return;
    view->UIActivate(TRUE);
    if (m_lpInPlaceObject && !m_bInPlaceActive) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_OPEN;
    view->Show(TRUE);
    view->Release();
}
BOOL COleDocObjectItem::OnPreparePrinting(void* pInfo) { (void)pInfo; return TRUE; }
void COleDocObjectItem::OnBeginPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnPrint(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
void COleDocObjectItem::OnEndPrinting(CDC* pDC, void* pInfo) { (void)pDC; (void)pInfo; }
HRESULT COleDocObjectItem::ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt, const GUID* pguidCmdGroup) {
    if (!m_lpObject) return OLE_E_NOTRUNNING;

    IOleCommandTarget* commandTarget = nullptr;
    HRESULT hr = m_lpObject->QueryInterface(IID_IOleCommandTarget, reinterpret_cast<void**>(&commandTarget));
    if (FAILED(hr) || !commandTarget) return hr == E_NOINTERFACE ? OLECMDERR_E_NOTSUPPORTED : hr;

    hr = commandTarget->Exec(pguidCmdGroup, nCmdID, nCmdExecOpt, nullptr, nullptr);
    commandTarget->Release();
    return hr;
}


//=============================================================================
// Exported COleDocObjectItem thunks, decoded from the retail export bodies.
//
// Image naming: "mfc140u" RVAs come from `disas.py --u`, or, for exports the
// Unicode symbol map does not list, from the mfc140u export table by ordinal
// (ordrva.py) and then disassembled there.  Where the ANSI twin mfc140.dll was
// what was read, its address is labelled "(mfc140)".  The two images have
// byte-identical function bodies, so control flow and member offsets read
// from either are the same; only the addresses differ.
//
// Retail layout.  The RTTI descriptors in retail (mfc140 .rdata 0x32c5f8 /
// 0x32c8f0) give sizeof(COleClientItem) == 0xf0 and sizeof(COleDocObjectItem)
// == 0x118, so COleDocObjectItem's own members occupy +0xf0..+0x118.  From
// retail afxole.h:799 (declaration order) and the disassembly below:
//   +0xf0   CMenu*            m_pHelpPopupMenu   OnInsertMenus   (mfc140u 0x255f00)
//   +0xf8   LPOLEDOCUMENTVIEW m_pActiveView      Release         (mfc140u 0x255c40),
//                                                 ActivateMe      (mfc140u 0x2559d0)
//   +0x100  LPPRINT           m_pIPrint          SupportsIPrint  (mfc140u 0x255da0),
//                                                 GetPageCount    (mfc140u 0x255e20)
//   +0x108  BOOL              m_bInHelpMenu      (not touched by any body here)
//   +0x110  XOleDocumentSite  m_xOleDocumentSite METHOD_PROLOGUE `lea -0x110(%rcx)`
//                                                 in ActivateMe; mfc140u .rdata
//                                                 0x34dff0 also holds the pair
//                                                 {&IID_IOleDocumentSite, 0x110}
//                                                 (interface-map entry shape)
//
// OpenMFC's COleDocObjectItem (include/openmfc/afxole.h) now names these
// members in a union over the existing 40-byte tail, which its constructor
// zeroes. The offsets are pinned below against that same storage; the object
// view and IPrint pointer are still not initialized with retail interface
// vtables/ownership semantics.
//
// Base-class members still differ: retail m_lpObject is +0x48, while OpenMFC
// keeps it at +0x50 and uses +0x48 for m_pContainerDoc. Base fields already
// declared by OpenMFC are reached BY NAME; newly modeled retail-only tail
// fields such as m_pView (+0x98) are accessed through RetailTailStorage. Some
// bodies remain stubs because other dependencies (notably CPrintDialog's
// PRINTDLG state and the uninstalled nested interface parts) are still absent.
//=============================================================================

// Sibling / base thunks this section calls.  Signatures match the definitions:
// core/ole/COleClientItem.cpp, core/ole/Thunks.cpp, core/ole/COleDocument.cpp,
// core/ole/RuntimeClasses.cpp, core/runtime/CObject.cpp,
// core/runtime/CCmdTarget.cpp, detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__Release_COleClientItem__UEAAXW4tagOLECLOSE___Z(
    COleClientItem* pThis, unsigned long dwCloseOption);
extern "C" void* MS_ABI impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(
    const COleDocument* pThis);
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
    const COleDocument* pThis, void** pPos, CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" const void* MS_ABI impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
    const CWnd* pThis);
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);

// Forward declaration of this file's own SupportsIPrint thunk (defined below),
// which GetPageCount calls directly, as retail does (`call` to the
// SupportsIPrint entry, not through the vtable -- it is non-virtual).
extern "C" int MS_ABI impl__SupportsIPrint_COleDocObjectItem__IEAAHXZ(COleDocObjectItem* pThis);

namespace {

// ---- named retail tail offsets of COleDocObjectItem -------------------------
static_assert(sizeof(COleDocObjectItem) == 0x118,
              "COleDocObjectItem must retain its retail size");
#ifndef __GNUC__
static_assert(offsetof(COleDocObjectItem, m_retailTail) == 0xf0,
              "COleDocObjectItem retail tail begins at +0xf0");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pHelpPopupMenu) == 0xf0,
              "COleDocObjectItem help menu at +0xf0");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pActiveView) == 0xf8,
              "COleDocObjectItem active view at +0xf8");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_pIPrint) == 0x100,
              "COleDocObjectItem IPrint pointer at +0x100");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_bInHelpMenu) == 0x108,
              "COleDocObjectItem help-menu state at +0x108");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_reserved_10c) == 0x10c,
              "COleDocObjectItem reserved gap at +0x10c");
static_assert(offsetof(COleDocObjectItem, m_retailTail) +
                  offsetof(COleDocObjectItem::RetailTailStorage, m_xOleDocumentSiteVftable) == 0x110,
              "COleDocObjectItem document-site part at +0x110");
static_assert(offsetof(COleClientItem, m_retailTail) +
                  offsetof(COleClientItem::RetailTailStorage, m_pView) == 0x98,
              "COleClientItem retail view at +0x98");
static_assert(offsetof(CFrameWnd, m_retailPrefix) +
                  offsetof(CFrameWnd::RetailPrefixStorage, m_hMenuDefault) == 0xf0,
              "CFrameWnd default menu at +0xf0");
#endif

inline IOleDocumentView*& ItemActiveView(COleDocObjectItem* pThis) {
    return pThis->m_retailTail.m_pActiveView;
}
inline IPrint*& ItemIPrint(COleDocObjectItem* pThis) {
    return pThis->m_retailTail.m_pIPrint;
}
// SupportsIPrint caches (LPPRINT)-1 in m_pIPrint once OleRun has failed, so
// that later calls answer FALSE without retrying; Release skips that value.
IPrint* const kIPrintUnsupported = reinterpret_cast<IPrint*>(static_cast<intptr_t>(-1));

// ---- retail CPrintInfo prefix (retail afxext.h:1112) ------------------------
// OpenMFC declares no CPrintInfo; the object is the caller's.  Only m_bPreview
// is read here (DoDefaultPrinting, `mov 0xc(%rsi),%eax` in mfc140 0x255710).
struct S_PrintInfoHead {
    void* m_pPD;           // +0x00  CPrintDialog*
    BOOL  m_bDocObject;    // +0x08
    BOOL  m_bPreview;      // +0x0c
};
static_assert(offsetof(S_PrintInfoHead, m_bDocObject) == 0x08, "CPrintInfo::m_bDocObject at +0x08");
static_assert(offsetof(S_PrintInfoHead, m_bPreview) == 0x0c, "CPrintInfo::m_bPreview at +0x0c");

// ---- IIDs, read out of retail .rdata with rd.py -----------------------------
// mfc140 0x2d7f00: c9 bc 22 b7 68 4e 1b 10 a2 bc 00 aa 00 40 47 70 (IID_IPrint),
//   the operand of the `lea` in SupportsIPrint.
// mfc140 0x2d7f10: cb bc 22 b7 68 4e 1b 10 a2 bc 00 aa 00 40 47 70
//   (IID_IOleCommandTarget), the operand in QueryCommand / DoDefaultPrinting.
// Spelled out because this DLL links no uuid library.
const GUID kIID_IPrint = {
    0xB722BCC9, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
const GUID kIID_IOleCommandTarget = {
    0xB722BCCB, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };

// Retail's file-local QueryInterface helper (mfc140u 0x26cc90 / mfc140
// 0x26ba50, no export):
//     p = NULL; hr = pUnk->QueryInterface(iid, &p); return hr != S_OK ? NULL : p;
// It does not null-check pUnk; callers here do (see each deviation note).
template <class I>
inline I* QueryIface(IUnknown* pUnk, const GUID& iid) {
    void* p = nullptr;
    HRESULT hr = pUnk->QueryInterface(iid, &p);
    return hr != S_OK ? nullptr : static_cast<I*>(p);
}
// Retail's release-and-clear helper (mfc140u 0x26ccc4 / mfc140 0x26ba84, no
// export):
//     if (*pp) { (*pp)->Release(); *pp = NULL; }
template <class I>
inline void ReleaseAndClear(I*& p) {
    if (p != nullptr) {
        p->Release();
        p = nullptr;
    }
}

// ---- COleDocObjectItem's OLE command map ------------------------------------
// Local mirror of AFX_OLECMDMAP / AFX_OLECMDMAP_ENTRY (retail afxwin.h, the
// _AFXDLL form: {pfnGetBaseMap, lpEntries}); CCmdTarget.cpp keeps the same
// private copy.  Retail's map (mfc140u .rdata 0x32eae0) is
//     { 0x1de8f0 /*CCmdTarget::GetThisCommandMap, folded with GetCommandMap*/,
//       0x34dfe0 }
// and the entry array at 0x34dfe0 starts with sixteen zero bytes, i.e. the
// {NULL, 0, 0} END_OLECMD_MAP terminator: the map is empty and chains to
// CCmdTarget.
struct S_OleCmdMapEntry { const GUID* pguid; ULONG cmdID; UINT nID; };
struct S_OleCmdMap {
    const void* (MS_ABI* pfnGetBaseMap)();
    const S_OleCmdMapEntry* lpEntries;
};
static_assert(sizeof(S_OleCmdMapEntry) == 0x10, "AFX_OLECMDMAP_ENTRY is 16 bytes");
static_assert(sizeof(S_OleCmdMap) == 0x10, "AFX_OLECMDMAP is 16 bytes");
const S_OleCmdMapEntry kCommandEntries_COleDocObjectItem[] = { { nullptr, 0, 0 } };
const S_OleCmdMap kCommandMap_COleDocObjectItem = {
    impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ,
    kCommandEntries_COleDocObjectItem
};

} // namespace

// Symbol: ?OnPreparePrinting@COleDocObjectItem@@SAHPEAVCView@@PEAUCPrintInfo@@H@Z
// static BOOL COleDocObjectItem::OnPreparePrinting(CView*, CPrintInfo*, BOOL bPrintAll)
// -- PARTIAL IMPLEMENTATION. Retail function entry RVA 0x256270 (mfc140u;
// ordinal-resolved) walks the document and updates print-page state; this body
// implements only its null/non-COleDocument early-out and success return.
// The corresponding ANSI-twin entry is RVA 0x255330 (mfc140). It walks
// the caller's COleDocument (pCaller->m_pDocument, +0xe8; returns FALSE when
// that is NULL or not a COleDocument) with GetStartPosition /
// GetNextItemOfKind(COleClientItem), and for each item that
// IsKindOf(COleDocObjectItem):
//   * when !bPrintAll, skips the item unless its m_pView (item +0x98) is
//     non-NULL and m_pView->m_hWnd == pCaller->m_hWnd;
//   * if SupportsIPrint(): calls GetPageCount(NULL, &nPages), adds nPages only
//     when that returns TRUE, and sets pInfo->m_bDocObject either way;
//     otherwise counts one page (m_bDocObject untouched);
//   * when !bPrintAll, stops after that item.
// If the total is > 0 it adds it to pInfo->m_pPD->m_pd.nMaxPage (or stores it
// when nMaxPage is 0xffff) and sets m_bDocObject.  Then, if m_bDocObject,
// m_pd.Flags |= PD_NOSELECTION, and also |= PD_NOPAGENUMS when bPrintAll.
// Returns TRUE.
// This body does not implement item enumeration/filtering or page counts.
// Although the item's m_pView (+0x98) is now modeled, no in-tree path assigns
// it. It also cannot update nMaxPage/Flags faithfully because OpenMFC lacks the
// retail CPrintDialog PRINTDLG-backed state (reference slot at +0x130).
// What IS done is retail's own early-out and its return value: FALSE when the
// caller has no document or it is not a COleDocument, TRUE otherwise.  The
// previous placeholder returned FALSE unconditionally, which cancels printing
// in any view whose OnPreparePrinting returns FALSE when this call does --
// retail only answers FALSE for a missing or non-OLE document.  For a document with no qualifying COleDocObjectItem (and
// m_bDocObject FALSE on entry, as CPrintInfo's constructor leaves it) this is
// exactly retail's result; for one with such items the page-count, the
// m_bDocObject flag and the m_pd.Flags/nMaxPage updates are all skipped.
extern "C" int MS_ABI impl__OnPreparePrinting_COleDocObjectItem__SAHPEAVCView__PEAUCPrintInfo__H_Z(
        CView* pCaller, void* pInfo, int bPrintAll) {
    (void)pInfo;
    (void)bPrintAll;
    // Retail does not null-check pCaller (it loads +0xe8 straight away).
    CDocument* pDoc = pCaller != nullptr ? pCaller->m_pDocument : nullptr;
    if (pDoc == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pDoc, impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }
    return TRUE;
}


// Symbol: ?OnPrint@COleDocObjectItem@@SAXPEAVCView@@PEAUCPrintInfo@@H@Z
// static void COleDocObjectItem::OnPrint(CView*, CPrintInfo*, BOOL bPrintAll)
// -- STUB. Retail function entry RVA 0x2563d0 (mfc140u; ordinal-resolved,
// no direct symbol-map entry) walks
// the caller's document items with the same COleDocument / COleDocObjectItem
// checks and the same m_pView filter (when !bPrintAll) as OnPreparePrinting,
// but, unlike it, does not stop after the first matching item.  For an item
// that SupportsIPrint() it GlobalLocks m_pd.hDevNames and m_pd.hDevMode, builds
// a target device from them (helper entry RVA 0x26031c (mfc140u) / 0x25f0f8
// (mfc140), no export), CoTaskMemAllocs a 0x18-byte PAGESET whose range is
// m_pd.nFromPage..m_pd.nToPage, and calls m_pIPrint->Print (IPrint vtable byte
// 0x28) if both locks and allocations succeed. On those failures retail frees
// any acquired resources and skips printing that item. For any other item it
// calls ExecCommand(OLECMDID_PRINT = 6, OLECMDEXECOPT_DONTPROMPTUSER = 2, NULL).
// m_pView is now modeled but is not assigned by any in-tree path. OpenMFC's
// CPrintDialog stores hDevNames, hDevMode, nFromPage, and nToPage separately,
// but has no retail PRINTDLG view;
// in particular DoModal does not preserve the returned PRINTDLG.Flags, which
// this body reads at +0x28 to derive IPrint flags. Reconstructing m_pd from the
// available fields would therefore not faithfully reproduce this path.
extern "C" void MS_ABI impl__OnPrint_COleDocObjectItem__SAXPEAVCView__PEAUCPrintInfo__H_Z(
        CView* pCaller, void* pInfo, int bPrintAll) {
    (void)pCaller;
    (void)pInfo;
    (void)bPrintAll;
}

// Symbol: ?DoDefaultPrinting@COleDocObjectItem@@SAJPEAVCView@@PEAUCPrintInfo@@@Z
// static HRESULT COleDocObjectItem::DoDefaultPrinting(CView* pCaller, CPrintInfo* pInfo)
// -- transcribed from retail mfc140u 0x256650:
//     if (pCaller == NULL || pInfo == NULL) AfxThrowInvalidArgException();
//     CDocument* pDoc = pCaller->m_pDocument;                       // +0xe8
//     if (pDoc == NULL || !pDoc->IsKindOf(RUNTIME_CLASS(COleDocument)))
//         return E_FAIL;
//     POSITION pos = pDoc->GetStartPosition();       // COleDocument vtable 0x238
//                                   // (slot 71; identified in COleServerItem.cpp)
//     if (pos == NULL) return E_FAIL;
//     do {
//         CDocItem* pItem = pDoc->GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem));
//         if (pItem != NULL && pItem->IsKindOf(RUNTIME_CLASS(COleDocObjectItem)))
//             goto found;
//     } while (pos != NULL);
//     return E_FAIL;
//   found:
//     if (pItem->m_lpObject == NULL) return E_FAIL;
//     IOleCommandTarget* pCmd = NULL;
//     HRESULT hr = pItem->m_lpObject->QueryInterface(IID_IOleCommandTarget, &pCmd);
//     if (hr != S_OK) return hr;
//     OLECMD cmd;
//     cmd.cmdID = pInfo->m_bPreview ? OLECMDID_PRINTPREVIEW /*7*/ : OLECMDID_PRINT /*6*/;
//     hr = pCmd->QueryStatus(NULL, 1, &cmd, NULL);                // vtable 0x18
//     if (hr == S_OK && (cmd.cmdf & (OLECMDF_SUPPORTED|OLECMDF_ENABLED)) == 3) {
//         hr = pCmd->Exec(NULL, cmd.cmdID, OLECMDEXECOPT_PROMPTUSER /*1*/,
//                         NULL, NULL);                            // vtable 0x20
//         if (hr == OLECMDERR_E_CANCELED /*0x80040103*/) hr = S_OK;
//     }
//     pCmd->Release();
//     return hr;
// Note that retail returns S_OK when QueryStatus succeeds but the command is
// not both supported and enabled -- nothing is printed and no error surfaces.
// Deviations:
//  * GetStartPosition is called through the exported thunk rather than
//    through the COleDocument vtable, so a client override of it is not seen
//    (OpenMFC's POSITION encoding is private to COleDocument.cpp, and the
//    thunk and GetNextItemOfKind agree on it).  COleServerItem.cpp and
//    COleUILinkInfo.cpp iterate documents the same way.
//  * pItem->m_lpObject is OpenMFC's named member (+0x50), not retail +0x48
//    (see the layout note at the top of this section).
//  * cmd.cmdf is zeroed before QueryStatus; retail leaves it uninitialised.
extern "C" long MS_ABI impl__DoDefaultPrinting_COleDocObjectItem__SAJPEAVCView__PEAUCPrintInfo___Z(
        CView* pCaller, void* pInfo) {
    if (pCaller == nullptr || pInfo == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_FAIL;
    }
    CDocument* pDoc = pCaller->m_pDocument;
    if (pDoc == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pDoc, impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ())) {
        return E_FAIL;
    }
    const COleDocument* pOleDoc = static_cast<const COleDocument*>(pDoc);

    COleDocObjectItem* pItem = nullptr;
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(pOleDoc);
    while (pos != nullptr) {
        CDocItem* pDocItem = static_cast<CDocItem*>(
            impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
                pOleDoc, &pos, impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ()));
        if (pDocItem != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pDocItem, impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ())) {
            pItem = static_cast<COleDocObjectItem*>(pDocItem);
            break;
        }
    }
    if (pItem == nullptr) return E_FAIL;

    if (pItem->m_lpObject == nullptr) return E_FAIL;
    IOleCommandTarget* pCmdTarget = nullptr;
    HRESULT hr = pItem->m_lpObject->QueryInterface(kIID_IOleCommandTarget,
                                                   reinterpret_cast<void**>(&pCmdTarget));
    if (hr != S_OK) return hr;

    OLECMD cmd;
    cmd.cmdID = static_cast<const S_PrintInfoHead*>(pInfo)->m_bPreview != 0
                    ? OLECMDID_PRINTPREVIEW : OLECMDID_PRINT;
    cmd.cmdf = 0;
    hr = pCmdTarget->QueryStatus(nullptr, 1, &cmd, nullptr);
    if (hr == S_OK && (cmd.cmdf & (OLECMDF_SUPPORTED | OLECMDF_ENABLED)) ==
                          (OLECMDF_SUPPORTED | OLECMDF_ENABLED)) {
        hr = pCmdTarget->Exec(nullptr, cmd.cmdID, OLECMDEXECOPT_PROMPTUSER, nullptr, nullptr);
        if (hr == OLECMDERR_E_CANCELED) hr = S_OK;
    }
    pCmdTarget->Release();
    return hr;
}

// Symbol: ?GetCommandMap@COleDocObjectItem@@MEBAPEBUAFX_OLECMDMAP@@XZ
// const AFX_OLECMDMAP* COleDocObjectItem::GetCommandMap() const -- retail
// mfc140u 0x2567d0 is `lea 0x18032eae0,%rax ; ret`: it returns the class's own
// command map (the BEGIN_OLECMD_MAP body `return &theClass::commandMap;`).
// GetThisCommandMap below has the identical body and is folded onto it.
extern "C" const void* MS_ABI impl__GetCommandMap_COleDocObjectItem__MEBAPEBUAFX_OLECMDMAP__XZ(
        const COleDocObjectItem* pThis) {
    (void)pThis;
    return &kCommandMap_COleDocObjectItem;
}

// Symbol: ?GetHelpMenu@COleDocObjectItem@@MEAAPEAVCMenu@@AEAI@Z
// CMenu* COleDocObjectItem::GetHelpMenu(UINT& nPosition) -- transcribed from
// the retail mfc140u function entry RVA 0x255ea0. It loads m_pView at item
// +0x98, calls CWnd::GetTopLevelFrame (entry RVA 0x28e490, mfc140u), and throws
// AfxThrowInvalidArgException if no frame is returned. It then wraps the
// frame's m_hMenuDefault at +0xf0 with CMenu::FromHandle (entry RVA 0x2a80a0,
// mfc140u).
// If that wrapper is non-NULL, it stores GetMenuItemCount(m_hMenu) - 1 through
// nPosition; the call's ANSI-twin IAT slot 0x1802c4c90 resolves to
// USER32!GetMenuItemCount. A NULL menu leaves nPosition untouched.
// The two named retail fields are pinned by the static_asserts above. The
// non-MSVC source-check build has no COleClientItem RetailTailStorage member;
// its local path supplies NULL rather than treating the compact layout as the
// retail overlay. The deployed MSVC ABI path reads the modeled +0x98 field.
// OpenMFC zeroes this slot in the item constructor but currently has no in-tree
// assignment to it, so absent caller-provided view state this follows retail's
// null-view outcome and throws the invalid-argument exception.
extern "C" CMenu* MS_ABI impl__GetHelpMenu_COleDocObjectItem__MEAAPEAVCMenu__AEAI_Z(
        COleDocObjectItem* pThis, unsigned int* pnPosition) {
#if defined(_MSC_VER)
    CView* pView = static_cast<COleClientItem*>(pThis)->m_retailTail.m_pView;
#else
    CView* pView = nullptr;
#endif
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pView);
    if (pFrame == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    CMenu* pMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(
        pFrame->m_retailPrefix.m_hMenuDefault);
    if (pMenu != nullptr) {
        *pnPosition = static_cast<unsigned int>(::GetMenuItemCount(pMenu->m_hMenu) - 1);
    }
    return pMenu;
}

// Symbol: ?GetPageCount@COleDocObjectItem@@QEAAHPEAJ0@Z
// BOOL COleDocObjectItem::GetPageCount(LPLONG pnFirstPage, LPLONG pcPages) --
// transcribed from retail mfc140u 0x255e20:
//     if (!SupportsIPrint()) return FALSE;
//     LONG nFirst, nPages;
//     HRESULT hr = m_pIPrint /*+0x100*/->GetPageInfo(&nFirst, &nPages); // vtable 0x20
//     if (pnFirstPage != NULL) *pnFirstPage = nFirst;
//     if (pcPages != NULL) *pcPages = nPages;
//     return SUCCEEDED(hr);                                   // `setns`
// The out-parameters are written whatever GetPageInfo returned, as in retail.
// Deviation: nFirst/nPages start at 0 here; retail leaves them uninitialised,
// so on a failing GetPageInfo that writes nothing retail stores stack garbage.
extern "C" int MS_ABI impl__GetPageCount_COleDocObjectItem__QEAAHPEAJ0_Z(
        COleDocObjectItem* pThis, LONG* pnFirstPage, LONG* pcPages) {
    if (!impl__SupportsIPrint_COleDocObjectItem__IEAAHXZ(pThis)) return FALSE;
    LONG nFirst = 0;
    LONG nPages = 0;
    HRESULT hr = ItemIPrint(pThis)->GetPageInfo(&nFirst, &nPages);
    if (pnFirstPage != nullptr) *pnFirstPage = nFirst;
    if (pcPages != nullptr) *pcPages = nPages;
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// Symbol: ?GetThisCommandMap@COleDocObjectItem@@KAPEBUAFX_OLECMDMAP@@XZ
// static const AFX_OLECMDMAP* COleDocObjectItem::GetThisCommandMap() --
// retail (mfc140) 0x255890 is `lea 0x18032c920,%rax ; ret`.  In mfc140u the
// body is identical-code-folded with GetCommandMap: the export table puts
// this ordinal (7185) and GetCommandMap's (5063) at the same RVA, 0x2567d0.
// Returns the same map as GetCommandMap above: empty, with
// CCmdTarget::GetThisCommandMap as its base.
extern "C" const void* MS_ABI impl__GetThisCommandMap_COleDocObjectItem__KAPEBUAFX_OLECMDMAP__XZ() {
    return &kCommandMap_COleDocObjectItem;
}

// Symbol: ?OnCmdMsg@COleDocObjectItem@@MEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
// BOOL COleDocObjectItem::OnCmdMsg(UINT nID, int nCode, void* pExtra,
//                                  AFX_CMDHANDLERINFO* pHandlerInfo)
// -- retail mfc140u 0x2567e0:
//     if (m_pView /*+0x98*/ != NULL &&
//         m_pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))   // vtable 0x28
//         return TRUE;
//     return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);  // direct call
// DEVIATION: only the second line is implemented. The item's m_pView is now
// named in RetailTailStorage, but this body still does not dispatch to it; the
// first retail branch remains to be implemented.
// The reinterpret_cast: retail's CDocItem derives CCmdTarget (retail afxole.h:
// `class CDocItem : public CCmdTarget`), so the CCmdTarget sub-object sits at
// offset 0 and retail passes `this` unchanged (`mov %rbx,%rcx`).  OpenMFC's
// CDocItem derives CObject instead, so the C++ conversion does not exist here;
// the pointer is passed through unchanged, exactly as retail does.
// Precondition inherited from the callee: the CCmdTarget::OnCmdMsg thunk
// (core/runtime/CCmdTarget.cpp) reaches GetMessageMap / GetCommandMap through
// retail vtable slots 12 / 13, so `this` must carry an MSVC-layout vtable --
// which is the case for the callers of this export (a client class's vtable
// referencing it).  An object built with OpenMFC's g++ COleDocObjectItem
// vtable has unrelated virtuals in those slots; do not call this on one.
extern "C" int MS_ABI impl__OnCmdMsg_COleDocObjectItem__MEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        COleDocObjectItem* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    return impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        reinterpret_cast<CCmdTarget*>(pThis), nID, nCode, pExtra, pHandlerInfo);
}

// Symbol: ?OnGetItemPosition@COleDocObjectItem@@MEAAXAEAVCRect@@@Z
// void COleDocObjectItem::OnGetItemPosition(CRect& rPosition) -- transcribed
// from the retail function entry RVA 0x246570 (mfc140u). The three
// instructions load m_pView from item +0x98, load that CView's m_hWnd at
// +0x40, then tail-jump to USER32!GetClientRect. The ANSI twin's function
// entry is RVA 0x244b70 (mfc140); iat.py resolves its ANSI import slot
// 0x1802c5358 to USER32!GetClientRect. The +0x98 overlay is asserted above.
// The non-MSVC compile-check declaration does not expose that overlay, so this
// source-only path does not attempt a raw-offset substitute. OpenMFC's item
// constructor zeroes the modeled slot, but no in-tree path currently assigns
// m_pView; unlike retail, the native path returns without touching rPosition
// when that unsupported state is NULL rather than dereferencing a null view.
extern "C" void MS_ABI impl__OnGetItemPosition_COleDocObjectItem__MEAAXAEAVCRect___Z(
        COleDocObjectItem* pThis, CRect* rPosition) {
#if defined(_MSC_VER)
    CView* pView = static_cast<COleClientItem*>(pThis)->m_retailTail.m_pView;
    if (pView == nullptr) return;
    ::GetClientRect(pView->GetSafeHwnd(), static_cast<RECT*>(*rPosition));
#else
    (void)pThis;
    (void)rPosition;
#endif
}

// Symbol: ?OnInsertMenus@COleDocObjectItem@@UEAAXPEAVCMenu@@PEAUtagOleMenuGroupWidths@@@Z
// void COleDocObjectItem::OnInsertMenus(CMenu* pMenuShared,
//                                       LPOLEMENUGROUPWIDTHS lpMenuWidths)
// -- STUB.  Retail (mfc140u 0x255f00 by export ordinal; mfc140 0x254fd0):
// sets width[0] = 1, width[2] = width[4] = 0 and turns -1 in width[1]/[3]/[5]
// into 0; reads the in-place menu m_pDocument(+0x40)->[+0x50]->[+0x68]. If it
// is NULL, it returns without help-menu insertion or AfxMergeMenus; otherwise
// it calls the virtual GetHelpMenu(nPos)
// (vtable byte 0x1d0: the mfc140 COleDocObjectItem vftable starts at .rdata
// 0x32c938 -- OnCmdMsg at +0x28 -- and holds GetHelpMenu's address at +0x1d0)
// and, when that yields a menu pHelp whose
// GetMenuString(nPos, strHelp, MF_BYPOSITION) succeeds:
//     CString strTitle;
//     strTitle.Format(L"%Ts %Ts", AfxGetModuleState()->[+0x20] /*app name*/,
//                     strHelp);            // format string mfc140u .rdata 0x34dfb8
//     strTitle.Remove(L'&');
//     HMENU hSub = ::GetSubMenu(pHelp->m_hMenu,
//                               ::GetMenuItemCount(pHelp->m_hMenu) - 1);
//     if (m_pHelpPopupMenu /*+0xf0*/ == NULL) {
//         m_pHelpPopupMenu = new CMenu;
//         m_pHelpPopupMenu->Attach(::CreateMenu());
//         ::InsertMenu(m_pHelpPopupMenu->m_hMenu, -1, MF_BYPOSITION|MF_POPUP,
//                      (UINT_PTR)hSub, strTitle);
//     }
//     ::InsertMenu(pMenuShared->m_hMenu, -1, MF_BYPOSITION|MF_POPUP,
//                  (UINT_PTR)m_pHelpPopupMenu->m_hMenu, strHelp);
//     width[5] = 1;
// finally AfxMergeMenus(pMenuShared ? pMenuShared->m_hMenu : NULL, hMenuOLE,
// lpMenuWidths->width, 0, TRUE).  (A NULL help menu goes straight to the
// merge; a GetMenuString failure returns early, skipping the merge.)  IAT
// slots resolved with iatu.py: GetMenuItemCount, GetSubMenu, CreateMenu,
// InsertMenuW.
// Blocked: OpenMFC does not model retail CDocument::m_pDocTemplate (+0x50) or
// CDocTemplate::m_hMenuInPlace (+0x68), so this function cannot obtain the
// merge source; COleClientItem.cpp leaves its corresponding menu methods
// stubbed for the same missing state. GetHelpMenu and m_pView are now modeled,
// but implementing only that half would remain unbalanced with OnRemoveMenus.
extern "C" void MS_ABI impl__OnInsertMenus_COleDocObjectItem__UEAAXPEAVCMenu__PEAUtagOleMenuGroupWidths___Z(
        COleDocObjectItem* pThis, CMenu* pMenuShared, OLEMENUGROUPWIDTHS* lpMenuWidths) {
    (void)pThis;
    (void)pMenuShared;
    (void)lpMenuWidths;
}

// Symbol: ?OnRemoveMenus@COleDocObjectItem@@UEAAXPEAVCMenu@@@Z
// void COleDocObjectItem::OnRemoveMenus(CMenu* pMenuShared) -- STUB.
// Retail (mfc140u 0x2561c0 by export ordinal; mfc140 0x255280):
//     int n = ::GetMenuItemCount(pMenuShared->m_hMenu);
//     if (n != 0) {
//         CMenu* pSub = CMenu::FromHandle(::GetSubMenu(pMenuShared->m_hMenu, n - 1));
//         if (pSub != NULL) {
//             int c = ::GetMenuItemCount(pSub->m_hMenu);
//             for (int i = 1; i < c; i++)
//                 ::DeleteMenu(pSub->m_hMenu, i, MF_BYPOSITION);
//         }
//         ::RemoveMenu(pMenuShared->m_hMenu, n - 1, MF_BYPOSITION);
//     }
//     HMENU hMenuOLE = m_pDocument(+0x40)->[+0x50]->[+0x68];
//     if (hMenuOLE != NULL) AfxUnmergeMenus(pMenuShared->m_hMenu, hMenuOLE, NULL);
// (IAT slots resolved with iat.py: GetMenuItemCount, GetSubMenu, DeleteMenu,
// RemoveMenu.) Blocked on the same unmodeled CDocument::m_pDocTemplate and
// CDocTemplate::m_hMenuInPlace fields as OnInsertMenus. Its destructive
// last-popup removal must not run while this implementation's OnInsertMenus is
// a stub: without a paired insertion it could remove pre-existing shared-menu
// content belonging to the caller.
extern "C" void MS_ABI impl__OnRemoveMenus_COleDocObjectItem__UEAAXPEAVCMenu___Z(
        COleDocObjectItem* pThis, CMenu* pMenuShared) {
    (void)pThis;
    (void)pMenuShared;
}

// Symbol: ?QueryCommand@COleDocObjectItem@@QEAAJKPEAKPEAU_tagOLECMDTEXT@@PEBU_GUID@@@Z
// HRESULT COleDocObjectItem::QueryCommand(ULONG nCmdID, DWORD* pdwStatus,
//                                         OLECMDTEXT* pCmdText,
//                                         const GUID* pguidCmdGroup)
// -- transcribed from retail mfc140u 0x255d10 (by export ordinal; mfc140
// 0x254de0):
//     IOleCommandTarget* p = _AfxQueryInterface(m_lpObject, IID_IOleCommandTarget);
//     HRESULT hr = E_NOTIMPL;
//     if (p != NULL) {
//         OLECMD cmd = { nCmdID, 0 };
//         hr = p->QueryStatus(pguidCmdGroup, 1, &cmd, pCmdText); // vtable 0x18
//         if (hr == S_OK) *pdwStatus = cmd.cmdf;
//     }
//     _AfxRelease(&p);
//     return hr;
// Deviation: a NULL m_lpObject is treated as a failed QueryInterface (E_NOTIMPL);
// retail's helper dereferences it unchecked.
extern "C" long MS_ABI impl__QueryCommand_COleDocObjectItem__QEAAJKPEAKPEAU_tagOLECMDTEXT__PEBU_GUID___Z(
        COleDocObjectItem* pThis, ULONG nCmdID, DWORD* pdwStatus, OLECMDTEXT* pCmdText,
        const GUID* pguidCmdGroup) {
    IOleCommandTarget* pCmdTarget = pThis->m_lpObject != nullptr
        ? QueryIface<IOleCommandTarget>(pThis->m_lpObject, kIID_IOleCommandTarget)
        : nullptr;
    HRESULT hr = E_NOTIMPL;
    if (pCmdTarget != nullptr) {
        OLECMD cmd;
        cmd.cmdID = nCmdID;
        cmd.cmdf = 0;
        hr = pCmdTarget->QueryStatus(pguidCmdGroup, 1, &cmd, pCmdText);
        if (hr == S_OK) *pdwStatus = cmd.cmdf;
    }
    ReleaseAndClear(pCmdTarget);
    return hr;
}

// Symbol: ?Release@COleDocObjectItem@@UEAAXW4tagOLECLOSE@@@Z
// void COleDocObjectItem::Release(OLECLOSE dwCloseOption) -- transcribed from
// retail mfc140u 0x255c40 (by export ordinal; mfc140 0x254d10):
//     _AfxRelease(&m_pActiveView);                       // +0xf8
//     if (m_pIPrint != (LPPRINT)-1) _AfxRelease(&m_pIPrint);   // +0x100
//     COleClientItem::Release(dwCloseOption);            // tail jump, direct
// A cached -1 ("IPrint unsupported") in m_pIPrint is left in place.
// m_pActiveView / m_pIPrint are the pinned retail offsets (see the layout
// note at the top of this section); the base Release is OpenMFC's own thunk,
// whose deviations from retail are documented in COleClientItem.cpp.
extern "C" void MS_ABI impl__Release_COleDocObjectItem__UEAAXW4tagOLECLOSE___Z(
        COleDocObjectItem* pThis, OLECLOSE dwCloseOption) {
    ReleaseAndClear(ItemActiveView(pThis));
    if (ItemIPrint(pThis) != kIPrintUnsupported) ReleaseAndClear(ItemIPrint(pThis));
    impl__Release_COleClientItem__UEAAXW4tagOLECLOSE___Z(
        pThis, static_cast<unsigned long>(dwCloseOption));
}

// Symbol: ?SupportsIPrint@COleDocObjectItem@@IEAAHXZ
// BOOL COleDocObjectItem::SupportsIPrint() -- transcribed from retail mfc140u
// 0x255da0 (by export ordinal; mfc140 0x254e70):
//     if (m_pIPrint /*+0x100*/ == NULL) {
//         m_pIPrint = _AfxQueryInterface(m_lpObject, IID_IPrint);
//         if (m_pIPrint == NULL) {
//             if (FAILED(::OleRun(m_lpObject))) {       // IAT: ole32!OleRun
//                 m_pIPrint = (LPPRINT)-1;
//                 return FALSE;
//             }
//             m_pIPrint = _AfxQueryInterface(m_lpObject, IID_IPrint);
//             if (m_pIPrint == NULL) return FALSE;
//         }
//     }
//     return m_pIPrint != (LPPRINT)-1;
// Deviation: with a NULL m_lpObject this returns FALSE without touching
// m_pIPrint; retail dereferences m_lpObject unchecked.
extern "C" int MS_ABI impl__SupportsIPrint_COleDocObjectItem__IEAAHXZ(COleDocObjectItem* pThis) {
    IPrint*& pIPrint = ItemIPrint(pThis);
    if (pIPrint == nullptr) {
        if (pThis->m_lpObject == nullptr) return FALSE;
        pIPrint = QueryIface<IPrint>(pThis->m_lpObject, kIID_IPrint);
        if (pIPrint == nullptr) {
            if (FAILED(::OleRun(pThis->m_lpObject))) {
                pIPrint = kIPrintUnsupported;
                return FALSE;
            }
            pIPrint = QueryIface<IPrint>(pThis->m_lpObject, kIID_IPrint);
            if (pIPrint == nullptr) return FALSE;
        }
    }
    return pIPrint != kIPrintUnsupported ? TRUE : FALSE;
}

// Symbol: ?ActivateMe@XOleDocumentSite@COleDocObjectItem@@UEAAJPEAUIOleDocumentView@@@Z
// HRESULT COleDocObjectItem::XOleDocumentSite::ActivateMe(LPOLEDOCUMENTVIEW
// pViewToActivate) -- STUB.  Retail mfc140u 0x2559d0 opens with METHOD_PROLOGUE
// (pThis = this - 0x110, then AFX_MANAGE_STATE on pThis's module state at
// +0x38), gets the client site from the virtual GetClientSite (vtable 0x190)
// and the item's own IOleInPlaceSite through CCmdTarget::GetInterface, and
// returns E_FAIL if either is NULL.  Then:
//   * pViewToActivate == NULL: if m_pActiveView (+0xf8) and m_pView (+0x98)
//     are both non-NULL it just calls ActivateAndShow and returns S_OK;
//     otherwise it QIs m_lpObject for IOleDocument and calls
//     CreateView(pInPlaceSite, NULL, 0, &pView) (E_FAIL when m_lpObject or
//     IOleDocument is missing, E_OUTOFMEMORY when CreateView fails);
//   * pViewToActivate != NULL: if it equals a non-NULL m_pActiveView it just
//     calls ActivateAndShow and returns S_OK; otherwise
//     pView->SetInPlaceSite(pInPlaceSite) and pView->AddRef().
// With a new view in hand: if m_pActiveView was non-NULL it gets
// UIActivate(FALSE), Show(FALSE) and Release, m_pIPrint is released unless it
// is NULL or -1 and is then set to NULL; the new view is stored in
// m_pActiveView, ActivateAndShow is called, and S_OK is returned.
// Blocked: the +0x110 XOleDocumentSite slot is named but no compatible vtable
// is installed there, and OpenMFC's GetClientSite thunk returns NULL. m_pView
// and m_pActiveView are now modeled. E_FAIL is what retail itself returns when
// it cannot obtain the client site; S_OK would claim an activation that did
// not happen.
extern "C" long MS_ABI impl__ActivateMe_XOleDocumentSite_COleDocObjectItem__UEAAJPEAUIOleDocumentView___Z(
        void* pThisSite, IOleDocumentView* pViewToActivate) {
    (void)pThisSite;
    (void)pViewToActivate;
    return E_FAIL;
}
