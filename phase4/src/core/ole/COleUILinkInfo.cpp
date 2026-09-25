// COleUILinkInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// =============================================================================
// COleUILinkInfo is MFC's internal IOleUILinkInfoW implementation (retail
// atlmfc/include/afxodlgs.h:69, `class COleUILinkInfo : public IOleUILinkInfo`),
// embedded in COleLinksDialog as m_xLinkInfo and handed to OleUIEditLinks.
// OpenMFC's headers do not declare it, so its retail layout is pinned here from
// the two exported constructors (mfc140u RVAs 0x24c310 / 0x24c340; the mfc140
// twins at 0x24ade0 / 0x24ae10 have the same instruction sequence):
//
//   ??0COleUILinkInfo@@QEAA@PEAVCOleDocument@@@Z   (RVA 0x24c310, mfc140u)
//       lea  vftable,%rax          ; mov %rdx,0x8(%rcx)   m_pDocument = pDoc
//       mov  %rax,(%rcx)           ; vptr
//       xor  %eax,%eax
//       mov  %rax,0x20(%rcx)       ; m_pItem          = NULL
//       mov  %rax,0x10(%rcx)       ; m_pSelectedItem  = NULL
//       mov  %rax,0x18(%rcx)       ; m_pos            = NULL
//       mov  %rax,0x28(%rcx)       ; m_bUpdateLinks = m_bUpdateEmbeddings = FALSE (one 8-byte store)
//   ??0COleUILinkInfo@@QEAA@PEAVCOleClientItem@@@Z (RVA 0x24c340, mfc140u)
//       the same, with m_pItem = pItem (+0x20) and m_pDocument = NULL (+0x08).
//
// Member order and names follow afxodlgs.h; GetNextLink (below) reads +0x28 for
// the OT_LINK test and +0x2c for the OT_EMBEDDED test, which is the header's
// m_bUpdateLinks / m_bUpdateEmbeddings order.
//
// Retail vftable (mfc140u 0x18032d8a0, read with uvt.py), IOleUILinkInfoW order:
//    0 QueryInterface        0xf4d0   (folded `mov $0x80004001,%eax; ret`)
//    1 AddRef                0x71e0   (folded `xor %eax,%eax; ret`)
//    2 Release               0x71e0   (same body)
//    3 GetNextLink           0x24c370    7 GetLinkSource   0x24c9d0
//    4 SetLinkUpdateOptions  0x24c4a0    8 OpenLinkSource  0x24cd80
//    5 GetLinkUpdateOptions  0x24c4e0    9 UpdateLink      0x24cdc0
//    6 SetLinkSource         0x24c520   10 CancelLink      0x24ce20
//                                       11 GetLastUpdate   0x24ce80
// GetLinkItem (private, non-virtual) is 0x24c430 (mfc140u).
//
// OpenMFC deviations (all deliberate):
//   * The vtable.  Callers (oledlg's OleUIEditLinks) reach the object through
//     its vptr, so the constructors install kCOleUILinkInfoVtbl, a
//     hand-authored MSVC-layout table in the retail slot order above whose
//     entries are this file's own MS_ABI thunks -- the technique
//     core/ole/CArchiveStream.cpp uses.
//   * The constructors additionally keep registering the (document, item) pair
//     in the ManualSmallStubImplementations side table, because UpdateLink
//     (below, not a retail transcription) still reads its target from there.
//   * Calls into COleDocument / COleClientItem go through those classes'
//     impl__ export thunks.  Retail reaches COleDocument::GetStartPosition via
//     vftable slot 71 (+0x238) and COleClientItem::DoVerb / ReportError /
//     FreezeLink via slots 25 / 48 / 49 (+0xc8 / +0x180 / +0x188); OpenMFC
//     calls the exported bodies directly, as core/ole/COleDocument.cpp does
//     for the same GetStartPosition slot.  A client override of one of those
//     virtuals is therefore not reached.
//   * Exceptions.  Retail wraps the work in TRY { ... sc = S_OK; }
//     CATCH_ALL(e) { sc = COleException::Process(e); e->Delete(); }.  Catch
//     funclets, read from each function's FuncInfo4 handler map (mfc140u):
//     Set/GetLinkUpdateOptions share 0x2c2545, OpenLinkSource 0x2c2c0e,
//     CancelLink 0x2c2648; each calls Process (0x25f3b0), stores the SCODE
//     into the parent frame and calls CException::Delete (0x227370) when e is
//     non-NULL.  WHERE the TRY starts matters: the FuncInfo4 IP-to-state maps
//     put the GetLinkItem call AND the ENSURE's AfxThrowInvalidArgException
//     call (0x227720) in try-state 0 for Set/GetLinkUpdateOptions (states from
//     0x24c4a9 / 0x24c4e9), OpenLinkSource (from 0x24cd84) and UpdateLink
//     (from 0x24cdcf), so in retail a NULL item never escapes those methods:
//     the CInvalidArgException is caught, and Process -- which maps only
//     COleException (m_sc), CMemoryException (E_OUTOFMEMORY) and
//     CNotSupportedException (E_NOTIMPL), everything else to E_UNEXPECTED --
//     yields E_UNEXPECTED (0x8000FFFF), which the method returns.  OpenMFC
//     returns E_UNEXPECTED directly on that path.  CancelLink is different:
//     its GetLinkItem call (0x24ce26) is outside the try range (state -1),
//     only the FreezeLink call is inside.  Other exceptions: this DLL raises
//     MFC exceptions through the MSVC runtime's _CxxThrowException, which a
//     gcc catch clause does not see (see core/ole/CArchiveStream.cpp,
//     "Exceptions"), so the catch arms are not reproduced: an exception
//     thrown by a COleClientItem call propagates to the caller instead of
//     becoming an SCODE.
//   * COleClientItem::m_nItemType (retail +0x94) is not modelled by OpenMFC's
//     COleClientItem (see UpdateItemType in core/ole/COleClientItem.cpp).
//     ItemTypeOf() below evaluates retail's own UpdateItemType rule live
//     instead of reading the cached member -- see its comment.
//   * Get/SetLinkUpdateOptions, OpenLinkSource: a NULL item returns
//     E_UNEXPECTED directly (see "Exceptions" above for why that is retail's
//     observable result).
// =============================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"

// Sibling export thunks (definitions: COleDocument.cpp, Thunks.cpp,
// RuntimeClasses.cpp, COleClientItem.cpp),
// declared with the parameter lists their definitions use.
extern "C" void* MS_ABI impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(
    const COleDocument* pThis);
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
    const COleDocument* pThis, void** pPos, CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__SetLinkUpdateOptions_COleClientItem__QEAAXW4tagOLEUPDATE___Z(
    COleClientItem* pThis, int nUpdateOptions);
extern "C" int MS_ABI impl__GetLinkUpdateOptions_COleClientItem__QEAA_AW4tagOLEUPDATE__XZ(
    COleClientItem* pThis);
extern "C" int MS_ABI impl__DoVerb_COleClientItem__UEAAHJPEAVCView__PEAUtagMSG___Z(
    COleClientItem* pThis, long nVerb, CView* pView, MSG* lpMsg);
extern "C" int MS_ABI impl__FreezeLink_COleClientItem__UEAAHXZ(COleClientItem* pThis);
extern "C" int MS_ABI impl__ReportError_COleClientItem__UEBAHJ_Z(
    const COleClientItem* pThis, long nCode);

// Forward declarations of this file's thunks, so the vtable can point at them.
extern "C" long MS_ABI impl__QueryInterface_COleUILinkInfo__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvObject);
extern "C" unsigned long MS_ABI impl__AddRef_COleUILinkInfo__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__Release_COleUILinkInfo__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__GetNextLink_COleUILinkInfo__UEAAKK_Z(
    void* pThis, unsigned long dwLink);
extern "C" long MS_ABI impl__SetLinkUpdateOptions_COleUILinkInfo__UEAAJKK_Z(
    void* pThis, unsigned long dwLink, unsigned long dwUpdateOpt);
extern "C" long MS_ABI impl__GetLinkUpdateOptions_COleUILinkInfo__UEAAJKPEAK_Z(
    void* pThis, unsigned long dwLink, unsigned long* lpdwUpdateOpt);
extern "C" long MS_ABI impl__SetLinkSource_COleUILinkInfo__UEAAJKPEA_WKPEAKH_Z(
    void* pThis, unsigned long dwLink, wchar_t* lpszDisplayName, unsigned long lenFileName,
    unsigned long* pchEaten, int fValidateSource);
extern "C" long MS_ABI impl__GetLinkSource_COleUILinkInfo__UEAAJKPEAPEA_WPEAK00PEAH2_Z(
    void* pThis, unsigned long dwLink, wchar_t** lplpszDisplayName, unsigned long* lplenFileName,
    wchar_t** lplpszFullLinkType, wchar_t** lplpszShortLinkType, int* lpfSourceAvailable,
    int* lpfIsSelected);
extern "C" long MS_ABI impl__OpenLinkSource_COleUILinkInfo__UEAAJK_Z(
    void* pThis, unsigned long dwLink);
extern "C" long MS_ABI impl__UpdateLink_COleUILinkInfo__UEAAJKHH_Z(
    void* pThis, unsigned long dwLink, int fErrorMessage, int fReserved);
extern "C" long MS_ABI impl__CancelLink_COleUILinkInfo__UEAAJK_Z(
    void* pThis, unsigned long dwLink);
extern "C" long MS_ABI impl__GetLastUpdate_COleUILinkInfo__UEAAJKPEAU_FILETIME___Z(
    void* pThis, unsigned long dwLink, FILETIME* lpLastUpdate);
extern "C" COleClientItem* MS_ABI impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(
    void* pThis, unsigned long dwLink);

namespace {

// Retail layout, pinned (see the header comment).
struct S_COleUILinkInfo {
    const void* const* vptr;        // +0x00
    COleDocument* m_pDocument;      // +0x08
    COleClientItem* m_pSelectedItem;// +0x10
    void* m_pos;                    // +0x18  POSITION
    COleClientItem* m_pItem;        // +0x20
    int m_bUpdateLinks;             // +0x28  BOOL
    int m_bUpdateEmbeddings;        // +0x2c  BOOL
};
static_assert(offsetof(S_COleUILinkInfo, vptr) == 0x00, "COleUILinkInfo vptr");
static_assert(offsetof(S_COleUILinkInfo, m_pDocument) == 0x08, "m_pDocument (ctor 0x24c310: mov %rdx,0x8(%rcx))");
static_assert(offsetof(S_COleUILinkInfo, m_pSelectedItem) == 0x10, "m_pSelectedItem (ctor: mov %rax,0x10(%rcx))");
static_assert(offsetof(S_COleUILinkInfo, m_pos) == 0x18, "m_pos (ctor: mov %rax,0x18(%rcx))");
static_assert(offsetof(S_COleUILinkInfo, m_pItem) == 0x20, "m_pItem (ctor 0x24c340: mov %rdx,0x20(%rcx))");
static_assert(offsetof(S_COleUILinkInfo, m_bUpdateLinks) == 0x28, "m_bUpdateLinks (GetNextLink: cmpl $0,0x28)");
static_assert(offsetof(S_COleUILinkInfo, m_bUpdateEmbeddings) == 0x2c, "m_bUpdateEmbeddings (GetNextLink: cmpl $0,0x2c)");
static_assert(sizeof(S_COleUILinkInfo) == 0x30, "sizeof(COleUILinkInfo)");

inline S_COleUILinkInfo* Self(void* pThis) { return static_cast<S_COleUILinkInfo*>(pThis); }

// OLE_OBJTYPE values (retail afxole.h enum OLE_OBJTYPE); OpenMFC's header does
// not declare the enum.
constexpr int kOT_UNKNOWN  = 0;
constexpr int kOT_LINK     = 1;
constexpr int kOT_EMBEDDED = 2;
constexpr int kOT_STATIC   = 3;

// Stand-in for the inline COleClientItem::GetType() (`return m_nItemType`,
// retail +0x94), which OpenMFC does not model.  Retail fills that member in
// COleClientItem::UpdateItemType (RVA 0x243140, mfc140):
//     if (_AfxQueryInterface(m_lpObject, IID_IOleLink)) { Release; m_nItemType = OT_LINK; }
//     else if (m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &dw) == S_OK   // IOleObject slot 22
//              && !(dw & OLEMISC_STATIC))                                 // testb $0x8
//         m_nItemType = OT_EMBEDDED;
//     else m_nItemType = OT_STATIC;
// and the constructor zeroes it (OT_UNKNOWN).  This helper applies the same
// rule to the item's current m_lpObject (OpenMFC's own member) at the time of
// the call, and returns OT_UNKNOWN when there is no object.  Deviation: retail
// reads the value cached at the last UpdateItemType call.  Layout caveat:
// OpenMFC's header puts COleClientItem::m_lpObject at +0x50 (checked with
// offsetof), not retail's +0x48 (UpdateItemType: mov 0x48(%rcx)); this reads
// the member OpenMFC's own COleClientItem thunks write.
int ItemTypeOf(COleClientItem* pItem) {
    LPOLEOBJECT lpObject = pItem->m_lpObject;
    if (lpObject == nullptr) return kOT_UNKNOWN;
    IOleLink* pLink = nullptr;
    if (lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pLink)) == S_OK &&
        pLink != nullptr) {
        pLink->Release();
        return kOT_LINK;
    }
    DWORD dwStatus = 0;
    if (lpObject->GetMiscStatus(DVASPECT_CONTENT, &dwStatus) == S_OK &&
        (dwStatus & OLEMISC_STATIC) == 0)
        return kOT_EMBEDDED;
    return kOT_STATIC;
}

// COleDocument::GetNextClientItem as retail inlines it here:
// GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem)) -- the call at
// 0x24c3bc inside GetNextLink (0x24c370) passes the class descriptor directly;
// OpenMFC obtains the same descriptor through GetThisClass.
COleClientItem* NextClientItem(COleDocument* pDoc, void** pPos) {
    return static_cast<COleClientItem*>(
        impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
            pDoc, pPos, impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ()));
}

// Hand-authored MSVC-layout IOleUILinkInfoW vtable, retail slot order
// (vftable 0x18032d8a0, mfc140u).
const void* const kCOleUILinkInfoVtbl[12] = {
    (const void*)&impl__QueryInterface_COleUILinkInfo__UEAAJAEBU_GUID__PEAPEAX_Z,        //  0
    (const void*)&impl__AddRef_COleUILinkInfo__UEAAKXZ,                                  //  1
    (const void*)&impl__Release_COleUILinkInfo__UEAAKXZ,                                 //  2
    (const void*)&impl__GetNextLink_COleUILinkInfo__UEAAKK_Z,                            //  3
    (const void*)&impl__SetLinkUpdateOptions_COleUILinkInfo__UEAAJKK_Z,                  //  4
    (const void*)&impl__GetLinkUpdateOptions_COleUILinkInfo__UEAAJKPEAK_Z,               //  5
    (const void*)&impl__SetLinkSource_COleUILinkInfo__UEAAJKPEA_WKPEAKH_Z,               //  6
    (const void*)&impl__GetLinkSource_COleUILinkInfo__UEAAJKPEAPEA_WPEAK00PEAH2_Z,       //  7
    (const void*)&impl__OpenLinkSource_COleUILinkInfo__UEAAJK_Z,                         //  8
    (const void*)&impl__UpdateLink_COleUILinkInfo__UEAAJKHH_Z,                           //  9
    (const void*)&impl__CancelLink_COleUILinkInfo__UEAAJK_Z,                             // 10
    (const void*)&impl__GetLastUpdate_COleUILinkInfo__UEAAJKPEAU_FILETIME___Z,           // 11
};

} // namespace

// COleUILinkInfo::COleUILinkInfo(COleClientItem*) -- RVA 0x24c340 (mfc140u);
// see the header comment for the transcription.  Deviations: the vptr is this
// file's kCOleUILinkInfoVtbl, and the side-table registration is OpenMFC's.
// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleClientItem@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleClientItem___Z(void* pThis, void* pItem) {
    S_COleUILinkInfo* self = Self(pThis);
    self->vptr = kCOleUILinkInfoVtbl;
    self->m_pItem = static_cast<COleClientItem*>(pItem);
    self->m_pDocument = nullptr;
    self->m_pSelectedItem = nullptr;
    self->m_pos = nullptr;
    self->m_bUpdateLinks = FALSE;
    self->m_bUpdateEmbeddings = FALSE;
    SetCOleUILinkInfoTarget(pThis, nullptr, static_cast<COleClientItem*>(pItem));
    return pThis;
}
// COleUILinkInfo::COleUILinkInfo(COleDocument*) -- RVA 0x24c310 (mfc140u);
// same deviations as above.
// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleDocument@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleDocument___Z(void* pThis, void* pDocument) {
    S_COleUILinkInfo* self = Self(pThis);
    self->vptr = kCOleUILinkInfoVtbl;
    self->m_pDocument = static_cast<COleDocument*>(pDocument);
    self->m_pItem = nullptr;
    self->m_pSelectedItem = nullptr;
    self->m_pos = nullptr;
    self->m_bUpdateLinks = FALSE;
    self->m_bUpdateEmbeddings = FALSE;
    SetCOleUILinkInfoTarget(pThis, static_cast<COleDocument*>(pDocument), nullptr);
    return pThis;
}
// COleUILinkInfo::UpdateLink -- NOT a retail transcription (it predates this
// file's layout pinning and reads its target from the side table).  Retail
// (RVA 0x24cdc0, mfc140u) is:
//     COleClientItem* pItem = NULL;                                   // movq $0,0x20(%rsp)
//     TRY { pItem = GetLinkItem(dwLink); ENSURE(pItem != NULL);        // both inside the try
//           if (!pItem->UpdateLink())                                 // 0x24af50
//               AfxThrowOleException(pItem->m_scLast);                // +0x60 via 0x24e888
//           pItem->m_bLinkUnavail = FALSE; sc = S_OK; }               // movb $0,0x7c
//     CATCH_ALL(e) { if (pItem) pItem->m_bLinkUnavail = TRUE;         // funclet 0x2c2c49
//                    sc = COleException::Process(e);
//                    if (pItem) pItem->ReportError(sc);               // vftable +0x180
//                    e->Delete(); }
//     return sc;
// so a NULL item (including dwLink == 0) returns E_UNEXPECTED.  This body
// differs: it returns E_INVALIDARG for an unresolvable item, and for
// dwLink == 0 on a document it updates every client item instead.
// m_scLast and m_bLinkUnavail are not modelled by OpenMFC's COleClientItem.
// The parameter list was a placeholder (`unsigned short` for the two BOOLs,
// `unsigned long` return); it now matches ?UpdateLink@COleUILinkInfo@@UEAAJKHH@Z.
// Symbol: ?UpdateLink@COleUILinkInfo@@UEAAJKHH@Z
extern "C" long MS_ABI impl__UpdateLink_COleUILinkInfo__UEAAJKHH_Z(
    void* pThis, unsigned long dwLink, int fErrorMessage, int fReserved) {
    (void)fErrorMessage;
    (void)fReserved;

    if (pThis == nullptr) return E_POINTER;

    COleDocument* pDocument = nullptr;
    COleClientItem* pItem = nullptr;
    if (!GetCOleUILinkInfoTarget(pThis, pDocument, pItem)) return E_INVALIDARG;

    if (pItem != nullptr) {
        if (dwLink != 0 && dwLink != 1) return E_INVALIDARG;
        return UpdateSingleOleLinkItem(pItem);
    }

    if (pDocument == nullptr) return E_INVALIDARG;

    if (dwLink == 0) {
        POSITION pos = pDocument->GetStartPosition();
        if (pos == nullptr) return S_FALSE;

        HRESULT hr = S_OK;
        bool didUpdate = false;
        while (pos != nullptr) {
            COleClientItem* updateItem = pDocument->GetNextClientItem(pos);
            if (updateItem == nullptr) continue;
            didUpdate = true;
            const HRESULT linkResult = UpdateSingleOleLinkItem(updateItem);
            if (FAILED(linkResult) && SUCCEEDED(hr)) hr = linkResult;
        }
        return didUpdate ? hr : S_FALSE;
    }

    COleClientItem* targetItem = FindItemByLinkIndex(pDocument, dwLink);
    if (targetItem == nullptr) return E_INVALIDARG;
    return UpdateSingleOleLinkItem(targetItem);
}
// COleUILinkInfo::AddRef -- vftable slot 1 is RVA 0x71e0 (mfc140u), a folded
// `xor %eax,%eax; ret`; ures.py resolves this export to the same RVA.  The
// object is not reference counted: it lives inside its owning dialog.
// Symbol: ?AddRef@COleUILinkInfo@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_COleUILinkInfo__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// COleUILinkInfo::CancelLink(DWORD) -- RVA 0x24ce20 (mfc140u):
//     COleClientItem* pItem = GetLinkItem(dwLink);          // 0x24c430; NO null check
//     SCODE sc;
//     TRY { sc = pItem->FreezeLink() ? S_OK : E_FAIL; }       // vftable +0x188 (slot 49);
//                                                             // neg/sbb/not/and $0x80004005
//     CATCH_ALL(e) { sc = COleException::Process(e); e->Delete(); }   // funclet 0x2c2648
//     if (sc != S_OK)
//         pItem->ReportError(sc);                             // vftable +0x180 (slot 48), edx = sc
//     return S_OK;
// Deviations: FreezeLink / ReportError are called through their export thunks
// and the catch arm is not reproduced (header comment).  Retail dereferences
// a NULL pItem (an access violation); OpenMFC returns E_INVALIDARG instead.
// Note: OpenMFC's FreezeLink thunk (core/ole/COleClientItem.cpp) is itself
// still a stub that returns FALSE, so until it is implemented this path
// always reaches ReportError(E_FAIL), which shows a message box.
// Symbol: ?CancelLink@COleUILinkInfo@@UEAAJK@Z
extern "C" long MS_ABI impl__CancelLink_COleUILinkInfo__UEAAJK_Z(void* pThis, unsigned long dwLink) {
    COleClientItem* pItem = impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(pThis, dwLink);
    if (pItem == nullptr) return E_INVALIDARG;   // OpenMFC guard; see above
    const long sc = impl__FreezeLink_COleClientItem__UEAAHXZ(pItem) ? S_OK : E_FAIL;
    if (sc != S_OK)
        impl__ReportError_COleClientItem__UEBAHJ_Z(pItem, sc);
    return S_OK;
}

// COleUILinkInfo::GetLastUpdate(DWORD, FILETIME*) -- RVA 0x24ce80 (mfc140u):
//     GetLinkItem(dwLink);        // call 0x24c430, result discarded
//     return S_OK;                // *lpLastUpdate is never written
// Symbol: ?GetLastUpdate@COleUILinkInfo@@UEAAJKPEAU_FILETIME@@@Z
extern "C" long MS_ABI impl__GetLastUpdate_COleUILinkInfo__UEAAJKPEAU_FILETIME___Z(
    void* pThis, unsigned long dwLink, FILETIME* lpLastUpdate) {
    (void)lpLastUpdate;
    (void)impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(pThis, dwLink);
    return S_OK;
}

// COleUILinkInfo::GetLinkItem(DWORD) -- RVA 0x24c430 (mfc140u):
//     if (m_pDocument == NULL)                               // +0x08
//         return m_pItem;                                    // +0x20
//     POSITION pos = m_pDocument->GetStartPosition();        // vftable +0x238 (slot 71)
//     COleClientItem* pItem = NULL;
//     for (DWORD i = 0; i < dwLink; i++)                     // dwLink is 1-based; 0 -> NULL
//         pItem = m_pDocument->GetNextClientItem(pos);       // GetNextItemOfKind 0x2531e0 (mfc140)
//                                                            // with RUNTIME_CLASS(COleClientItem)
//     return pItem;
// Deviation: GetStartPosition is called through its export thunk (header comment).
// Symbol: ?GetLinkItem@COleUILinkInfo@@AEAAPEAVCOleClientItem@@K@Z
extern "C" COleClientItem* MS_ABI impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(
    void* pThis, unsigned long dwLink) {
    S_COleUILinkInfo* self = Self(pThis);
    if (self->m_pDocument == nullptr)
        return self->m_pItem;
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(self->m_pDocument);
    COleClientItem* pItem = nullptr;
    for (unsigned long i = 0; i < dwLink; ++i)
        pItem = NextClientItem(self->m_pDocument, &pos);
    return pItem;
}

// COleUILinkInfo::GetLinkSource -- RVA 0x24c9d0 (mfc140u).  STUB.  Retail
// (~230 instructions) builds the display name and link-type strings from the
// item's IOleLink / moniker through CString temporaries and reports source
// availability from COleClientItem::m_bLinkUnavail (retail +0x7c,
// `cmp %cl,0x7c(%rsi)`), which OpenMFC's COleClientItem does not model.
// Returns E_NOTIMPL without writing the out-parameters (an OpenMFC stub
// value, not retail's) so OleUIEditLinks never reads uninitialised strings.
// Symbol: ?GetLinkSource@COleUILinkInfo@@UEAAJKPEAPEA_WPEAK00PEAH2@Z
extern "C" long MS_ABI impl__GetLinkSource_COleUILinkInfo__UEAAJKPEAPEA_WPEAK00PEAH2_Z(
    void* pThis, unsigned long dwLink, wchar_t** lplpszDisplayName, unsigned long* lplenFileName,
    wchar_t** lplpszFullLinkType, wchar_t** lplpszShortLinkType, int* lpfSourceAvailable,
    int* lpfIsSelected) {
    (void)pThis; (void)dwLink; (void)lplpszDisplayName; (void)lplenFileName;
    (void)lplpszFullLinkType; (void)lplpszShortLinkType; (void)lpfSourceAvailable;
    (void)lpfIsSelected;
    return E_NOTIMPL;
}

// COleUILinkInfo::GetLinkUpdateOptions(DWORD, LPDWORD) -- RVA 0x24c4e0 (mfc140u):
//     TRY {
//         COleClientItem* pItem = GetLinkItem(dwLink);       // 0x24c430
//         ENSURE(pItem != NULL);                             // AfxThrowInvalidArgException
//         DWORD opt = OLEUPDATE_ALWAYS;                      // mov $0x1,%eax
//         if (pItem->GetType() == OT_LINK)                   // cmp %eax,0x94(%rcx)
//             opt = pItem->GetLinkUpdateOptions();           // call, 0x249690 in mfc140
//         *lpdwUpdateOpt = opt;
//         sc = S_OK;
//     } CATCH_ALL(e) { sc = COleException::Process(e); e->Delete(); }
//     return sc;
// A NULL item therefore yields E_UNEXPECTED with *lpdwUpdateOpt untouched,
// which OpenMFC returns directly.  Deviations: GetType() is ItemTypeOf()
// (m_nItemType is unmodelled), and the catch arm is not reproduced for
// exceptions from the COleClientItem call (header comment).
// Symbol: ?GetLinkUpdateOptions@COleUILinkInfo@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetLinkUpdateOptions_COleUILinkInfo__UEAAJKPEAK_Z(
    void* pThis, unsigned long dwLink, unsigned long* lpdwUpdateOpt) {
    COleClientItem* pItem = impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(pThis, dwLink);
    if (pItem == nullptr)
        return E_UNEXPECTED;   // retail: ENSURE throws inside the TRY -> Process -> E_UNEXPECTED
    unsigned long opt = OLEUPDATE_ALWAYS;
    if (ItemTypeOf(pItem) == kOT_LINK)
        opt = static_cast<unsigned long>(
            impl__GetLinkUpdateOptions_COleClientItem__QEAA_AW4tagOLEUPDATE__XZ(pItem));
    *lpdwUpdateOpt = opt;
    return S_OK;
}

// COleUILinkInfo::GetNextLink(DWORD) -- RVA 0x24c370 (mfc140u).  dwLink is a
// 1-based index into the document's client items (0 = start), and the value
// returned is the index of the next item the dialog should list, or 0:
//     if (m_pDocument == NULL) return 0;                     // +0x08
//     POSITION pos = m_pDocument->GetStartPosition();        // vftable +0x238 (slot 71)
//     for (DWORD i = 0; i < dwLink; i++)                     // skip the items already listed
//         m_pDocument->GetNextClientItem(pos);               // GetNextItemOfKind, result ignored
//     DWORD n = dwLink + 1;
//     if (pos == NULL) return 0;
//     do {
//         COleClientItem* pItem = m_pDocument->GetNextClientItem(pos);
//         if (pItem == NULL) return 0;
//         OLE_OBJTYPE t = pItem->GetType();                  // mov 0x94(%rax),%eax
//         if (m_bUpdateLinks && t == OT_LINK) return n;      // +0x28, cmp $0x1
//         if (m_bUpdateEmbeddings && t == OT_EMBEDDED) return n;   // +0x2c, cmp $0x2
//         n++;
//     } while (pos != NULL);
//     return 0;
// m_pos (+0x18) is not used.  Deviations: GetType() is ItemTypeOf(), and
// GetStartPosition is called through its export thunk (header comment).
// Symbol: ?GetNextLink@COleUILinkInfo@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__GetNextLink_COleUILinkInfo__UEAAKK_Z(void* pThis, unsigned long dwLink) {
    S_COleUILinkInfo* self = Self(pThis);
    if (self->m_pDocument == nullptr)
        return 0;
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(self->m_pDocument);
    for (unsigned long i = 0; i < dwLink; ++i)
        (void)NextClientItem(self->m_pDocument, &pos);
    unsigned long n = dwLink + 1;
    if (pos == nullptr)
        return 0;
    do {
        COleClientItem* pItem = NextClientItem(self->m_pDocument, &pos);
        if (pItem == nullptr)
            return 0;
        const int type = ItemTypeOf(pItem);
        if (self->m_bUpdateLinks && type == kOT_LINK)
            return n;
        if (self->m_bUpdateEmbeddings && type == kOT_EMBEDDED)
            return n;
        ++n;
    } while (pos != nullptr);
    return 0;
}

// COleUILinkInfo::OpenLinkSource(DWORD) -- RVA 0x24cd80 (mfc140u):
//     TRY {
//         COleClientItem* pItem = GetLinkItem(dwLink);       // 0x24c430
//         ENSURE(pItem != NULL);                             // AfxThrowInvalidArgException
//         pItem->DoVerb(OLEIVERB_SHOW, NULL, NULL);          // vftable +0xc8 (slot 25),
//         sc = S_OK;                                         // edx = -1, r8 = r9 = 0; result ignored
//     } CATCH_ALL(e) { sc = COleException::Process(e); e->Delete(); }   // funclet 0x2c2c0e
//     return sc;
// A NULL item therefore yields E_UNEXPECTED, which OpenMFC returns directly.
// Deviations: DoVerb is called through its export thunk and the catch arm is
// not reproduced for exceptions from DoVerb (header comment).
// Symbol: ?OpenLinkSource@COleUILinkInfo@@UEAAJK@Z
extern "C" long MS_ABI impl__OpenLinkSource_COleUILinkInfo__UEAAJK_Z(void* pThis, unsigned long dwLink) {
    COleClientItem* pItem = impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(pThis, dwLink);
    if (pItem == nullptr)
        return E_UNEXPECTED;   // retail: ENSURE throws inside the TRY -> Process -> E_UNEXPECTED
    (void)impl__DoVerb_COleClientItem__UEAAHJPEAVCView__PEAUtagMSG___Z(pItem, OLEIVERB_SHOW, nullptr, nullptr);
    return S_OK;
}

// COleUILinkInfo::QueryInterface -- vftable slot 0 is RVA 0xf4d0 (mfc140u), a
// folded `mov $0x80004001,%eax; ret`: E_NOTIMPL, *ppvObject left untouched.
// Symbol: ?QueryInterface@COleUILinkInfo@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_COleUILinkInfo__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvObject) {
    (void)pThis;
    (void)riid;
    (void)ppvObject;
    return E_NOTIMPL;
}

// COleUILinkInfo::Release -- vftable slot 2 is RVA 0x71e0 (mfc140u), the same
// folded `xor %eax,%eax; ret` as AddRef.
// Symbol: ?Release@COleUILinkInfo@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_COleUILinkInfo__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// COleUILinkInfo::SetLinkSource -- RVA 0x24c520 (mfc140u).  STUB.  Retail
// rejects lenFileName >= MAX_PATH (0x104) with E_INVALIDARG, then parses the
// display name into a moniker (bind context, MkParseDisplayName-style loop
// through CString temporaries), binds it when fValidateSource, sets the item's
// IOleLink source, and records the outcome in COleClientItem::m_bLinkUnavail
// (retail +0x7c: `movb $0x1,0x7c(%rcx)` on failure, cleared on success), a
// member OpenMFC's COleClientItem does not model.  Returns E_NOTIMPL (an
// OpenMFC stub value, not retail's).  Only the first check is reproduced:
// `cmp $0x104,%r9d; jb` -> `mov $0x80070057,%eax` before any other work.
// Symbol: ?SetLinkSource@COleUILinkInfo@@UEAAJKPEA_WKPEAKH@Z
extern "C" long MS_ABI impl__SetLinkSource_COleUILinkInfo__UEAAJKPEA_WKPEAKH_Z(
    void* pThis, unsigned long dwLink, wchar_t* lpszDisplayName, unsigned long lenFileName,
    unsigned long* pchEaten, int fValidateSource) {
    (void)pThis; (void)dwLink; (void)lpszDisplayName;
    (void)pchEaten; (void)fValidateSource;
    if (lenFileName >= 0x104)   // MAX_PATH
        return E_INVALIDARG;
    return E_NOTIMPL;
}

// COleUILinkInfo::SetLinkUpdateOptions(DWORD, DWORD) -- RVA 0x24c4a0 (mfc140u):
//     TRY {
//         COleClientItem* pItem = GetLinkItem(dwLink);       // 0x24c430
//         ENSURE(pItem != NULL);                             // AfxThrowInvalidArgException
//         pItem->SetLinkUpdateOptions((OLEUPDATE)dwUpdateOpt);   // 0x249710 in mfc140
//         sc = S_OK;
//     } CATCH_ALL(e) { sc = COleException::Process(e); e->Delete(); }   // funclet 0x2c2545
//     return sc;
// A NULL item therefore yields E_UNEXPECTED, which OpenMFC returns directly.
// Deviation: the catch arm is not reproduced for exceptions from the
// COleClientItem call (header comment).
// Symbol: ?SetLinkUpdateOptions@COleUILinkInfo@@UEAAJKK@Z
extern "C" long MS_ABI impl__SetLinkUpdateOptions_COleUILinkInfo__UEAAJKK_Z(
    void* pThis, unsigned long dwLink, unsigned long dwUpdateOpt) {
    COleClientItem* pItem = impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(pThis, dwLink);
    if (pItem == nullptr)
        return E_UNEXPECTED;   // retail: ENSURE throws inside the TRY -> Process -> E_UNEXPECTED
    impl__SetLinkUpdateOptions_COleClientItem__QEAAXW4tagOLEUPDATE___Z(pItem, static_cast<int>(dwUpdateOpt));
    return S_OK;
}
