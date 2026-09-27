// CHtmlEditDoc — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp; retail mfc140u.dll
// disassembly (every RVA below is mfc140u, resolved through the export's
// ordinal in mfc_complete_ordinal_mapping.json and the mfc140u export table).
//
// ---------------------------------------------------------------------------
// Retail layout.  `class AFX_NOVTABLE CHtmlEditDoc : public CDocument`
// (atlmfc/include/afxhtml.h:1705) declares no data members: its constructor
// (RVA 0x27e130) is the whole of `call ??0CDocument (0x21a9c0); mov %rbx,%rax`
// (AFX_NOVTABLE, so it stores no vptr of its own), and the OpenMFC runtime
// class descriptor records m_nObjectSize 384 == 0x180 == retail
// sizeof(CDocument).  No member offset is dereferenced in this file.
//
// OpenMFC has no C++ CHtmlEditDoc class and its CHtmlEditDoc descriptor has no
// CreateObject (core/doc/RuntimeClasses.cpp), so every CHtmlEditDoc that
// reaches these thunks is built by a CLIENT class compiled against the real MFC
// headers; its vftable therefore has retail's layout.  Retail reaches every
// document method below through that vftable, and so does this file.  The
// CDocument slots were derived from the declaration order in the retail
// afxwin.h (CObject 0..4, CCmdTarget 5..21, then CDocument from GetAdapter at
// 22) and each one agrees with a retail call site in this class:
//     slot 23 (+0xb8)  SetTitle              OnOpenDocument, rdx = path, no r8
//     slot 27 (+0xd8)  SetModifiedFlag       OnSaveDocument, edx = FALSE
//     slot 28 (+0xe0)  GetFirstViewPosition  GetView
//     slot 29 (+0xe8)  GetNextView           GetView, rdx = &pos
//     slot 31 (+0xf8)  DeleteContents        OnNewDocument (0x21c060)
//     slot 54 (+0x1b0) OnDocumentEvent       OnOpenDocument
//     slot 70 (+0x230) GetView               called on `this` by every body below;
//                                            the first virtual afxhtml.h declares
//                                            on CHtmlEditDoc
// (CDocument::OnNewDocument, RVA 0x21c060, calls slots 31 DeleteContents,
// 27 SetModifiedFlag and 54 OnDocumentEvent, consistent with the same count.)
// The view is dispatched the way core/view/CHtmlEditView.cpp dispatches it:
// slot 151 (+0x4b8) GetStartDocument.
//
// Defensive fallback (not in retail): an object whose vptr points into THIS
// image was built by OpenMFC with an Itanium vtable, for which retail's slot
// numbers mean something else.  For such an object the exported CDocument /
// CHtmlEditView / CHtmlEditDoc thunk that the slot names is called directly
// instead -- the same IsOwnObject test core/view/CHtmlEditView.cpp uses.  No
// in-image CHtmlEditDoc can exist today (see above), so for documents this is
// only a guard against a wrong-slot call.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <ocidl.h>
#include <mshtml.h>

// Thunks defined elsewhere in the tree (each definition located with grep;
// parameter lists follow the mangled names).
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis);                                    // core/doc/CDocument.cpp
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis);                                    // core/doc/CDocument.cpp
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis);                         // core/doc/CDocument.cpp
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(CDocument* pThis, const wchar_t* lpszTitle);  // core/doc/CDocument.cpp
extern "C" void MS_ABI impl__OnDocumentEvent_CDocument__UEAAXW4DocumentEvent_1__Z(CDocument* pThis, int eventId);  // core/doc/CDocument.cpp
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(CDocument* pThis, int bModified);      // detail/DocviewSupport.cpp
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAU__POSITION__XZ(const CDocument* pThis);  // core/doc/Thunks.cpp
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAU__POSITION___Z(const CDocument* pThis, void** pPos);  // core/doc/Thunks.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);  // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ();          // core/view/RuntimeClasses.cpp
extern "C" int MS_ABI impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(
    const void* pThis, IHTMLDocument2** ppDocument);                                                     // core/view/CHtmlEditView.cpp
extern "C" const wchar_t* MS_ABI impl__GetStartDocument_CHtmlEditView__UEAAPEB_WXZ(void* pThis);        // core/view/CHtmlEditView.cpp
extern "C" void MS_ABI impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen);                                                     // core/view/CHtmlView.cpp
extern "C" void* MS_ABI impl___0CFile__QEAA_XZ(void* pThis);                                            // core/file/Thunks.cpp
extern "C" void MS_ABI impl___1CFile__UEAA_XZ(void* pThis);                                             // core/file/Thunks.cpp
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pException);               // core/file/CFile.cpp
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
    void* p, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf);                               // core/runtime/CArchive.cpp
extern "C" void MS_ABI impl___1CArchive__QEAA_XZ(void* pThis);                                          // core/runtime/Thunks.cpp
extern "C" void* MS_ABI impl___0CArchiveStream__QEAA_PEAVCArchive___Z(void* pThis, CArchive* pArchive); // core/ole/CArchiveStream.cpp

// Same-file thunk the bodies below call before its definition.
extern "C" void* MS_ABI impl__GetView_CHtmlEditDoc__UEBAPEAVCHtmlEditView__XZ(const void* pThis);

// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// Retail vftable slots (see the file header).
constexpr int kSlotDocSetTitle             = 0xb8 / 8;    // 23
constexpr int kSlotDocSetModifiedFlag      = 0xd8 / 8;    // 27
constexpr int kSlotDocGetFirstViewPosition = 0xe0 / 8;    // 28
constexpr int kSlotDocGetNextView          = 0xe8 / 8;    // 29
constexpr int kSlotDocDeleteContents       = 0xf8 / 8;    // 31 (OnNewDocument, RVA 0x21c060 (mfc140u))
constexpr int kSlotDocOnDocumentEvent      = 0x1b0 / 8;   // 54
constexpr int kSlotDocGetView              = 0x230 / 8;   // 70
constexpr int kSlotViewGetStartDocument    = 0x4b8 / 8;   // 151 (CHtmlEditView vftable, RVA 0x333568 (mfc140u))

// CDocument::onAfterNewDocument (retail afxwin.h enum DocumentEvent).
constexpr int kOnAfterNewDocument = 0;

// CFile::modeCreate | CFile::modeWrite: the `mov $0x1001,%r8d` OnSaveDocument
// passes to CFile::Open.
constexpr unsigned int kSaveOpenFlags = 0x1001;
// CArchive::store and the buffer size OnSaveDocument passes to ??0CArchive
// (`xor %r8d,%r8d` / `mov $0x1000,%r9d`).
constexpr unsigned int kArchiveStore = 0;
constexpr int kArchiveBufSize = 0x1000;

// IID_IPersistStreamInit {7FD52380-4E07-101B-AE2D-08002B2EC713}: the 16 bytes
// DeleteContents/IsModified/OnSaveDocument pass to QueryInterface (read from
// retail .rdata; spelled out locally rather than pulled from libuuid).
const GUID kIID_IPersistStreamInit = { 0x7FD52380, 0x4E07, 0x101B, { 0xAE,0x2D,0x08,0x00,0x2B,0x2E,0xC7,0x13 } };

// Retail constructs CArchiveStream inline (vptr + m_pArchive, sizeof 0x10, see
// core/ole/CArchiveStream.cpp); this file builds it through that file's
// exported constructor into storage of the same size.
struct S_CArchiveStreamStorage {
    void* vptr;
    CArchive* pArchive;
};
static_assert(sizeof(S_CArchiveStreamStorage) == 0x10, "retail CArchiveStream is 0x10 bytes");

bool IsOwnObject(const void* pObject) {
    const void* vptr = *reinterpret_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
Fn VSlot(const void* pObject, int nSlot) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pObject);
    return reinterpret_cast<Fn>(vtbl[nSlot]);
}

CDocument* AsDoc(void* pThis) { return static_cast<CDocument*>(pThis); }
const CDocument* AsDoc(const void* pThis) { return static_cast<const CDocument*>(pThis); }

// this->GetView()  (retail: `call *0x230(vtbl)`).
void* CallGetView(void* pThis) {
    if (IsOwnObject(pThis))
        return impl__GetView_CHtmlEditDoc__UEBAPEAVCHtmlEditView__XZ(pThis);
    using Fn = void* (MS_ABI*)(void*);
    return VSlot<Fn>(pThis, kSlotDocGetView)(pThis);
}

// this->GetFirstViewPosition()  (retail: `call *0xe0(vtbl)`).
void* CallGetFirstViewPosition(const void* pThis) {
    if (IsOwnObject(pThis))
        return impl__GetFirstViewPosition_CDocument__UEBAPEAU__POSITION__XZ(AsDoc(pThis));
    using Fn = void* (MS_ABI*)(const void*);
    return VSlot<Fn>(pThis, kSlotDocGetFirstViewPosition)(pThis);
}

// this->GetNextView(pos)  (retail: `call *0xe8(vtbl)`, rdx = &pos).
CView* CallGetNextView(const void* pThis, void** pPos) {
    if (IsOwnObject(pThis))
        return impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAU__POSITION___Z(AsDoc(pThis), pPos);
    using Fn = CView* (MS_ABI*)(const void*, void**);
    return VSlot<Fn>(pThis, kSlotDocGetNextView)(pThis, pPos);
}

// this->SetTitle(lpszTitle)  (retail: `call *0xb8(vtbl)`).
void CallSetTitle(void* pThis, const wchar_t* lpszTitle) {
    if (IsOwnObject(pThis)) {
        impl__SetTitle_CDocument__UEAAXPEB_W_Z(AsDoc(pThis), lpszTitle);
        return;
    }
    using Fn = void (MS_ABI*)(void*, const wchar_t*);
    VSlot<Fn>(pThis, kSlotDocSetTitle)(pThis, lpszTitle);
}

// this->SetModifiedFlag(bModified)  (retail: `call *0xd8(vtbl)`).
void CallSetModifiedFlag(void* pThis, int bModified) {
    if (IsOwnObject(pThis)) {
        impl__SetModifiedFlag_CDocument__UEAAXH_Z(AsDoc(pThis), bModified);
        return;
    }
    using Fn = void (MS_ABI*)(void*, int);
    VSlot<Fn>(pThis, kSlotDocSetModifiedFlag)(pThis, bModified);
}

// this->OnDocumentEvent(deEvent)  (retail: `call *0x1b0(vtbl)`).
void CallOnDocumentEvent(void* pThis, int deEvent) {
    if (IsOwnObject(pThis)) {
        impl__OnDocumentEvent_CDocument__UEAAXW4DocumentEvent_1__Z(AsDoc(pThis), deEvent);
        return;
    }
    using Fn = void (MS_ABI*)(void*, int);
    VSlot<Fn>(pThis, kSlotDocOnDocumentEvent)(pThis, deEvent);
}

// pView->GetStartDocument()  (retail: `call *0x4b8(vtbl)` on the view).
const wchar_t* CallGetStartDocument(void* pView) {
    if (IsOwnObject(pView))
        return impl__GetStartDocument_CHtmlEditView__UEAAPEB_WXZ(pView);
    using Fn = const wchar_t* (MS_ABI*)(void*);
    return VSlot<Fn>(pView, kSlotViewGetStartDocument)(pView);
}

// pView->Navigate(lpszURL, 0, NULL, NULL, NULL, 0) -- the direct call to
// CHtmlView::Navigate (0x27c770) OpenURL, OnOpenDocument and DeleteContents
// make, with every optional argument zero (r8d, r9 and the three stack slots).
void NavigateView(void* pView, const wchar_t* lpszURL) {
    impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(static_cast<CHtmlView*>(pView), lpszURL,
                                                   0, nullptr, nullptr, nullptr, 0);
}

// The inlined `CComPtr<IHTMLDocument2> spDoc; pView->GetDHtmlDocument(&spDoc);
// CComQIPtr<IPersistStreamInit> spPSI = spDoc;` sequence DeleteContents,
// IsModified and OnSaveDocument share: GetDHtmlDocument is the direct call to
// 0x27dfd0, and a failed QueryInterface (`test %eax,%eax; js`) leaves spPSI
// NULL.  Returns the IPersistStreamInit (or NULL) and the document through
// *ppDoc, both owned by the caller.
IPersistStreamInit* GetPersistStreamInit(void* pView, IHTMLDocument2** ppDoc) {
    *ppDoc = nullptr;
    impl__GetDHtmlDocument_CHtmlEditView__QEBAHPEAPEAUIHTMLDocument2___Z(pView, ppDoc);
    if (*ppDoc == nullptr)
        return nullptr;
    IPersistStreamInit* pPSI = nullptr;
    if (FAILED((*ppDoc)->QueryInterface(kIID_IPersistStreamInit, reinterpret_cast<void**>(&pPSI))))
        pPSI = nullptr;
    return pPSI;
}

// The CComPtr destructors at the end of each of those bodies: spPSI, then spDoc.
void ReleasePair(IPersistStreamInit* pPSI, IHTMLDocument2* pDoc) {
    if (pPSI != nullptr)
        pPSI->Release();
    if (pDoc != nullptr)
        pDoc->Release();
}

} // namespace

// Retail ??0CHtmlEditDoc (RVA 0x27e130, mfc140u):
//     CDocument::CDocument(this);  return this;
// No vptr store (AFX_NOVTABLE); the client's derived constructor installs its own.
// Symbol: ??0CHtmlEditDoc@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditDoc__QEAA_XZ(void* pThis) {
    impl___0CDocument__QEAA_XZ(AsDoc(pThis));
    return pThis;
}

// Retail ??1CHtmlEditDoc (RVA 0x27e150, mfc140u) is a single
// `jmp ??1CDocument@@UEAA@XZ` (0x21ab60): the class has nothing of its own to tear down.
// Symbol: ??1CHtmlEditDoc@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditDoc__UEAA_XZ(void* pThis) {
    impl___1CDocument__UEAA_XZ(AsDoc(pThis));
}

// Retail DeleteContents (RVA 0x27e1e0, mfc140u):
//     CHtmlEditView* pView = GetView();                      // vslot 70
//     if (pView == NULL) return;
//     pView->NewDocument();       // inlined CHtmlEditCtrlBase::NewDocument (afxhtml.h):
//         CComPtr<IHTMLDocument2> spDoc;  CComQIPtr<IPersistStreamInit> spPSI;
//         CStreamOnCString stream;                           // constructed, never used
//         pView->GetDHtmlDocument(&spDoc);                   // direct call 0x27dfd0
//         if (spDoc) { spPSI = spDoc; if (spPSI) spPSI->InitNew(); }   // vslot 8 (+0x40); HRESULT dropped
//     pView->Navigate(pView->GetStartDocument(), 0, NULL, NULL, NULL, 0);  // view vslot 151, direct 0x27c770
// The unused CStreamOnCString local (vftable + two empty CStrings) is not
// reproduced: its construction and destruction have no observable effect.
// Symbol: ?DeleteContents@CHtmlEditDoc@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CHtmlEditDoc__UEAAXXZ(void* pThis) {
    void* pView = CallGetView(pThis);
    if (pView == nullptr)
        return;
    IHTMLDocument2* pDoc = nullptr;
    IPersistStreamInit* pPSI = GetPersistStreamInit(pView, &pDoc);
    if (pPSI != nullptr)
        pPSI->InitNew();
    ReleasePair(pPSI, pDoc);
    NavigateView(pView, CallGetStartDocument(pView));
}

// Retail GetView (RVA 0x27e160, mfc140u):
//     POSITION pos = GetFirstViewPosition();                 // vslot 28
//     while (pos != NULL) {
//         CView* pView = GetNextView(pos);                   // vslot 29
//         if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CHtmlEditView)))   // direct 0x234cf0
//             return (CHtmlEditView*)pView;
//     }
//     return NULL;
// RUNTIME_CLASS(CHtmlEditView) is the descriptor at 0x180333520 (mfc140u); here
// it is the one the exported GetThisClass thunk returns.  Caveat: a view
// OpenMFC itself creates through CHtmlEditView::CreateObject keeps OpenMFC's
// CHtmlView vptr (see core/view/CHtmlEditView.cpp), so it reports CHtmlView's
// runtime class and is not found by this test; client-derived views are.
// Symbol: ?GetView@CHtmlEditDoc@@UEBAPEAVCHtmlEditView@@XZ
extern "C" void* MS_ABI impl__GetView_CHtmlEditDoc__UEBAPEAVCHtmlEditView__XZ(const void* pThis) {
    void* pos = CallGetFirstViewPosition(pThis);
    while (pos != nullptr) {
        CView* pView = CallGetNextView(pThis, &pos);
        if (pView != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pView, impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ()))
            return pView;
    }
    return nullptr;
}

// Retail IsModified (RVA 0x27e390, mfc140u):
//     CHtmlEditView* pView = GetView();                      // vslot 70
//     if (pView == NULL) return FALSE;
//     HRESULT hr = pView->GetIsDirty();   // inlined CHtmlEditCtrlBase::GetIsDirty (afxhtml.h):
//         hr = E_NOINTERFACE (`mov $0x80004002,%edi`);
//         pView->GetDHtmlDocument(&spDoc);                   // direct call 0x27dfd0
//         if (spDoc) { spPSI = spDoc; if (spPSI) hr = spPSI->IsDirty(); }  // vslot 4 (+0x20)
//     return hr != S_FALSE;                                  // `cmp $1,%edi; setne %al`
// So with a view but no reachable IPersistStreamInit the result is TRUE
// (E_NOINTERFACE != S_FALSE), exactly as retail.
// Symbol: ?IsModified@CHtmlEditDoc@@UEAAHXZ
extern "C" int MS_ABI impl__IsModified_CHtmlEditDoc__UEAAHXZ(void* pThis) {
    void* pView = CallGetView(pThis);
    if (pView == nullptr)
        return FALSE;
    HRESULT hr = E_NOINTERFACE;
    IHTMLDocument2* pDoc = nullptr;
    IPersistStreamInit* pPSI = GetPersistStreamInit(pView, &pDoc);
    if (pPSI != nullptr)
        hr = pPSI->IsDirty();
    ReleasePair(pPSI, pDoc);
    return hr != S_FALSE;
}

// Retail exports ?OnNewDocument@CHtmlEditDoc@@UEAAHXZ (ordinal 10666) and
// ?OnNewDocument@CDocument@@UEAAHXZ (ordinal 10665) at the SAME address, RVA
// 0x21c060 (mfc140u), whose whole body is:
//     DeleteContents();                                      // vslot 31 (+0xf8)
//     m_strPathName.Empty();                                 // CString at +0x48, direct 0x33b0
//     SetModifiedFlag(FALSE);                                // vslot 27 (+0xd8)
//     OnDocumentEvent(onAfterNewDocument);                   // vslot 54 (+0x1b0), edx = 0
//     return TRUE;
// It is transcribed here through the retail slots rather than forwarded to
// OpenMFC's CDocument::OnNewDocument export (core/doc/CDocument.cpp): that body
// calls DeleteContents/SetModifiedFlag as g++ virtuals, i.e. Itanium vtable
// indices 12 and 13 (computed from the member-pointer values g++ emits for
// OpenMFC's CDocument), which on a client's retail-layout vftable are
// GetMessageMap and GetCommandMap -- so on every object that can reach this
// export it did nothing but return TRUE.
// DEVIATION: m_strPathName.Empty() is omitted.  Retail's CString at +0x48 is
// m_pNextDoc in OpenMFC's CDocument (static_assert in core/doc/CDocument.cpp),
// and OpenMFC's ??0CDocument thunk does not construct a CString there, so the
// retail call would dereference a non-string.  An object built by OpenMFC
// itself (vptr in this image) still goes to the CDocument export, as the other
// dispatch helpers in this file do.
// Symbol: ?OnNewDocument@CHtmlEditDoc@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_CHtmlEditDoc__UEAAHXZ(void* pThis) {
    if (IsOwnObject(pThis))
        return impl__OnNewDocument_CDocument__UEAAHXZ(AsDoc(pThis));
    using Fn = void (MS_ABI*)(void*);
    VSlot<Fn>(pThis, kSlotDocDeleteContents)(pThis);
    CallSetModifiedFlag(pThis, FALSE);
    CallOnDocumentEvent(pThis, kOnAfterNewDocument);
    return TRUE;
}

// Retail OnOpenDocument (RVA 0x27e520, mfc140u):
//     CHtmlEditView* pView = GetView();                      // vslot 70
//     BOOL bRet = FALSE;
//     if (pView != NULL) {
//         pView->Navigate(lpszPathName, 0, NULL, NULL, NULL, 0);   // direct 0x27c770
//         SetTitle(lpszPathName);                            // vslot 23
//         bRet = TRUE;
//     }
//     OnDocumentEvent((DocumentEvent)0);                     // vslot 54, UNCONDITIONAL
//     return bRet;
// The event argument is `xor %edx,%edx`, i.e. onAfterNewDocument (0), not
// onAfterOpenDocument (1); transcribed as retail has it.
// Known downstream problem (core/doc/CDocument.cpp, not this file): when the
// client's slot 54 is the inherited CDocument::OnDocumentEvent, it reaches
// OpenMFC's export, which calls UpdateAllViews(NULL, 0, NULL) as a g++ virtual
// -- Itanium index 23, which is SetTitle on a retail-layout vftable -- so,
// unless the client overrides SetTitle, the title set just above is
// overwritten with an empty one.
// Symbol: ?OnOpenDocument@CHtmlEditDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_CHtmlEditDoc__UEAAHPEB_W_Z(void* pThis, const wchar_t* lpszPathName) {
    int bRet = FALSE;
    void* pView = CallGetView(pThis);
    if (pView != nullptr) {
        NavigateView(pView, lpszPathName);
        CallSetTitle(pThis, lpszPathName);
        bRet = TRUE;
    }
    CallOnDocumentEvent(pThis, kOnAfterNewDocument);
    return bRet;
}

// Retail OnSaveDocument (RVA 0x27e5b0, mfc140u):
//     CHtmlEditView* pView = GetView();                      // vslot 70
//     BOOL bRet = FALSE;
//     if (pView != NULL) {
//         CFile file;                                        // ??0CFile 0x2277e0
//         if (file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite, NULL)) {  // direct 0x227c50
//             CArchive ar(&file, CArchive::store, 0x1000, NULL);   // ??0CArchive 0x1d1550
//             CArchiveStream stream(&ar);                    // inlined: vftable 0x180321088, m_pArchive
//             CComPtr<IHTMLDocument2> spDoc;  CComQIPtr<IPersistStreamInit> spPSI;
//             pView->GetDHtmlDocument(&spDoc);               // direct call 0x27dfd0
//             if (spDoc) {
//                 spPSI = spDoc;
//                 if (spPSI && spPSI->Save(&stream, TRUE) == S_OK) {   // vslot 6 (+0x30), r8d = 1
//                     SetModifiedFlag(FALSE);                // vslot 27
//                     bRet = TRUE;
//                 }
//             }
//             // spPSI, spDoc released; ~CArchive
//         }
//         // ~CFile
//     }
//     return bRet;
// Deviations: the CArchiveStream is built through the exported constructor
// (core/ole/CArchiveStream.cpp), which installs OpenMFC's hand-authored IStream
// vtable instead of retail's; and retail's unwind actions (destroying the
// CArchive and CFile if an exception propagates out of Save) are not
// reproduced -- on the normal path the destructors run in retail's order.
// Symbol: ?OnSaveDocument@CHtmlEditDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_CHtmlEditDoc__UEAAHPEB_W_Z(void* pThis, const wchar_t* lpszPathName) {
    int bRet = FALSE;
    void* pView = CallGetView(pThis);
    if (pView == nullptr)
        return bRet;

    alignas(CFile) unsigned char fileStorage[sizeof(CFile)];
    impl___0CFile__QEAA_XZ(fileStorage);
    CFile* pFile = reinterpret_cast<CFile*>(fileStorage);
    if (impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(pFile, lpszPathName, kSaveOpenFlags, nullptr)) {
        alignas(CArchive) unsigned char arStorage[sizeof(CArchive)];
        impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(arStorage, pFile, kArchiveStore, kArchiveBufSize, nullptr);
        CArchive* pAr = reinterpret_cast<CArchive*>(arStorage);

        S_CArchiveStreamStorage stream;
        impl___0CArchiveStream__QEAA_PEAVCArchive___Z(&stream, pAr);

        IHTMLDocument2* pDoc = nullptr;
        IPersistStreamInit* pPSI = GetPersistStreamInit(pView, &pDoc);
        if (pPSI != nullptr &&
            pPSI->Save(reinterpret_cast<IStream*>(&stream), TRUE) == S_OK) {
            CallSetModifiedFlag(pThis, FALSE);
            bRet = TRUE;
        }
        ReleasePair(pPSI, pDoc);
        impl___1CArchive__QEAA_XZ(arStorage);
    }
    impl___1CFile__UEAA_XZ(fileStorage);
    return bRet;
}

// Retail OpenURL (RVA 0x27e4b0, mfc140u):
//     CHtmlEditView* pView = GetView();                      // vslot 70
//     if (pView != NULL && lpszURL != NULL && *lpszURL != 0) {   // `cmp %si,(%rbx)`
//         pView->Navigate(lpszURL, 0, NULL, NULL, NULL, 0);  // direct 0x27c770
//         return TRUE;
//     }
//     return FALSE;
// Symbol: ?OpenURL@CHtmlEditDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OpenURL_CHtmlEditDoc__UEAAHPEB_W_Z(void* pThis, const wchar_t* lpszURL) {
    void* pView = CallGetView(pThis);
    if (pView != nullptr && lpszURL != nullptr && *lpszURL != L'\0') {
        NavigateView(pView, lpszURL);
        return TRUE;
    }
    return FALSE;
}
