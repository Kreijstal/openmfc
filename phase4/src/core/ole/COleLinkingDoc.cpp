// COleLinkingDoc — OpenMFC implementation.
// Sources: olecore.cpp
//
// Every implemented body below was transcribed from the retail disassembly
// (OnFindEmbeddedItem stays a stub, see there).  The disassembler reads
// mfc140.dll, the ANSI twin of the mfc140u.dll this project reimplements.
// Control flow, member offsets and constants agree between the two images for
// every function in this file, but code that handles TCHAR strings does NOT
// match byte for byte: OnSaveDocument compares with _mbscmp in mfc140 and
// wcscmp in mfc140u, the GetCurFile string duplicator converts with
// MultiByteToWideChar in mfc140 and copies with wcslen/memcpy in mfc140u, and
// ParseDisplayName's _ismbblead pairs exist only in mfc140.  Those were checked
// against mfc140u and are called out where they occur.  Unless a comment says
// "(mfc140u)", every RVA quoted in this file is an *mfc140* RVA and is NOT an
// mfc140u address.  Each RVA named is a function ENTRY unless the comment says
// otherwise.
//
// Retail COleLinkingDoc layout, read out of ??0COleLinkingDoc@@QEAA@XZ
// (mfc140 0x25cc00) and cross-checked against the real
// .../atlmfc/include/afxole.h:1081 declaration on this host.  sizeof == 0x230:
//   +0x1e8 COleObjectFactory* m_pFactory           ctor stores 0 (0x25cc5f)
//   +0x1f0 BOOL               m_bDeferErrors       ctor stores 0 (0x25cc6c)
//   +0x1f8 CException*        m_pLastException     ctor stores 0 (0x25cc72)
//   +0x200 DWORD              m_dwRegister         ctor stores 0 (0x25cc59)
//   +0x208 LPMONIKER          m_lpMonikerROT       ctor stores 0 (0x25cc79)
//   +0x210 CString            m_strMoniker         ctor stores the nil string
//   +0x218 BOOL               m_bVisibleLock       ctor stores 0 (0x25cc66)
//   +0x220 XPersistFile       m_xPersistFile       vftable .rdata 0x32e350
//   +0x228 XOleItemContainer  m_xOleItemContainer  vftable .rdata 0x32e300
// The two nested-part offsets are also the METHOD_PROLOGUE_EX this-adjustments
// the interface methods open with: `lea -0x220(%rcx),%rbx` in
// XPersistFile::IsDirty (0x25d670) and `lea -0x228(%rcx),%rsi` in
// XOleItemContainer::GetObject (0x25dcb0).
//
// Inherited RETAIL offsets the transcriptions below refer to (OpenMFC's own
// CDocument/CCmdTarget are laid out differently, so the code itself goes
// through OpenMFC's members and thunks, never these numbers), read out of
// ??0CDocument@@QEAA@XZ (0x218ea0) and ??0COleDocument@@QEAA@XZ (0x252f50):
//   CCmdTarget  +0x08 m_dwRef (the `lock incl 0x8(%rcx)` in OnCloseDocument and
//               UpdateVisibleLock), +0x38 m_pModuleState (what every nested
//               METHOD_PROLOGUE_EX hands to AFX_MAINTAIN_STATE2 at 0x133df0)
//   CDocument   +0x40 m_strTitle, +0x48 m_strPathName, +0x50 m_pDocTemplate,
//               +0x94 m_bFinalRelease, +0x120 m_bAutoDelete, +0x124 m_bEmbedded,
//               +0x128 m_bGetThumbnailMode, +0x12c m_bPreviewHandlerMode,
//               +0x130 m_bSearchMode
//   COleDocument +0x1c8 m_lpRootStg, +0x1d4 m_bRemember
// CDocument::IsSearchAndOrganizeHandler (0x21b2d0) is exactly
// "m_bPreviewHandlerMode || m_bSearchMode || m_bGetThumbnailMode", which is the
// three-way test several of the bodies below open with; OpenMFC has that
// predicate as impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ, so the test
// is written through it rather than by re-reading three flags OpenMFC's
// CDocument does not declare.
//
// HOW OpenMFC's DECLARATION DIFFERS (reported as a headerRequest, not patched
// here -- include/openmfc/afxole.h is not this agent's file).  OpenMFC's
// COleLinkingDoc declares, at the same total size of 560 bytes:
//     BOOL m_bRegistered;              // sits where retail has m_pFactory
//     LPMONIKER m_lpMoniker;           // sits where retail has m_bDeferErrors
//     char _colelinkingdoc_padding[56];// covers retail +0x1f8 .. +0x230
// Neither of those two is a retail member: m_bRegistered occupies the bytes
// retail gives to m_pFactory and m_lpMoniker the bytes retail gives to
// m_bDeferErrors, so none of retail's nine members is present under its own
// name, and neither nested interface part is declared or constructed.  The
// mapping used throughout this file is
//     m_lpMoniker   <-> retail m_lpMonikerROT
//     m_bRegistered <-> "m_dwRegister != 0"  (retail has no such BOOL)
// and every remaining retail member is laid over the declared-but-unused
// _colelinkingdoc_padding through the S_COleLinkingDocExtra overlay below,
// which the constructor already zeroes.
//
// DEVIATION, stated once for the whole file: OpenMFC's COleLinkingDoc declares
// none of Register/LockExternal/UpdateVisibleLock/OnShowViews/OnNewDocument/
// OnOpenDocument/OnSaveDocument/OnCloseDocument/OnFindEmbeddedItem/
// OnGetLinkedItem/SaveToStorage/ReportSaveLoadException/EndDeferErrors as C++
// members, so where retail makes a virtual call in the bodies below -- vftable
// bytes 0x108 OnOpenDocument and 0x110 OnSaveDocument (XPersistFile::Load/
// Save), 0x290 OnGetLinkedItem and 0x298 OnFindEmbeddedItem (the
// XOleItemContainer methods), all slots of the COleLinkingDoc vftable at
// .rdata 0x32e3a0 -- this file calls the sibling impl__ thunk directly, and it
// does the same for 0xc0 COleDocument::SetPathName and 0xd0 IsModified.  A
// COleServerDoc (or client) override is therefore NOT picked up at those sites.
// The one exception is RegisterIfServerAttached (0x2a0), which OpenMFC does
// declare virtual, so those call sites are real virtual calls.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

//=============================================================================
// Sibling implementations this unit calls.  Every one of these is an export
// that exists only as an extern "C" impl__ thunk inside this DLL; calling the
// C++ method instead would compile and fail at link.
//=============================================================================
// core/runtime/CCmdTarget.cpp
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
    CCmdTarget* pThis, const void* iid);
extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
    CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
// core/doc/CDocument.cpp
extern "C" int MS_ABI impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__SendInitialUpdate_CDocument__QEAAXXZ(CDocument* pThis);
extern "C" void MS_ABI impl__ReportSaveLoadException_CDocument__UEAAXPEB_WPEAVCException__HI_Z(
    CDocument* pThis, const wchar_t* lpszPathName, CException* e, int bSaving, unsigned int nIDPDefault);
// detail/DocviewSupport.cpp
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis);
// core/ole/COleDocument.cpp
extern "C" int MS_ABI impl__OnNewDocument_COleDocument__UEAAHXZ(COleDocument* pThis);
extern "C" int MS_ABI impl__OnOpenDocument_COleDocument__UEAAHPEB_W_Z(COleDocument* pThis, const wchar_t* lpszPathName);
extern "C" int MS_ABI impl__OnSaveDocument_COleDocument__UEAAHPEB_W_Z(COleDocument* pThis, const wchar_t* lpszPathName);
extern "C" void MS_ABI impl__OnCloseDocument_COleDocument__UEAAXXZ(COleDocument* pThis);
extern "C" void MS_ABI impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(COleDocument* pThis, CObject* pObject);
extern "C" void MS_ABI impl__SetPathName_COleDocument__UEAAXPEB_WH_Z(COleDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU);
extern "C" void* MS_ABI impl__GetMoniker_COleDocument__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(COleDocument* pThis, int nAssign);
// core/exceptions/COleException.cpp
extern "C" long MS_ABI impl__Process_COleException__SAJPEBVCException___Z(const CException* pAnyException);
// featurepack/CMFC_misc_stubs.cpp  (both write/read detail::olecore::g_bOleUserCtrl,
// which is OpenMFC's stand-in for retail's AFX_MODULE_STATE +0x64)
extern "C" void MS_ABI impl__AfxOleSetUserCtrl__YAXH_Z(int bUserCtrl);
extern "C" int MS_ABI impl__AfxOleGetUserCtrl__YAHXZ();

// Forward declarations of this file's own thunks, for the sites that call a
// sibling entry point defined further down.
extern "C" void MS_ABI impl__LockExternal_COleLinkingDoc__IEAAXHH_Z(COleLinkingDoc* pThis, int bLock, int bRemoveRefs);
extern "C" void MS_ABI impl__UpdateVisibleLock_COleLinkingDoc__IEAAXHH_Z(COleLinkingDoc* pThis, int bVisible, int bRemoveRefs);
extern "C" long MS_ABI impl__EndDeferErrors_COleLinkingDoc__QEAAJJ_Z(COleLinkingDoc* pThis, long sc);
extern "C" COleClientItem* MS_ABI impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(
    COleLinkingDoc* pThis, const wchar_t* lpszItemName);
extern "C" COleServerItem* MS_ABI impl__OnGetLinkedItem_COleLinkingDoc__MEAAPEAVCOleServerItem__PEB_W_Z(
    COleLinkingDoc* pThis, const wchar_t* lpszItemName);
extern "C" long MS_ABI impl__GetObjectW_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WKPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(
    void* pThisItf, wchar_t* lpszItem, unsigned long dwSpeedNeeded, IBindCtx* pbc,
    const GUID& riid, void** ppvObject);

namespace {

//-----------------------------------------------------------------------------
// The retail members OpenMFC's COleLinkingDoc declaration is missing.
//
// They are laid over _colelinkingdoc_padding, the 56 declared-but-unused bytes
// that start at COleLinkingDoc+0x1f8 and run to the end of the object -- the
// same span retail fills with m_pLastException/m_dwRegister/m_lpMonikerROT/
// m_strMoniker/m_bVisibleLock and the two nested interface parts.  Only the
// first 0x28 bytes are claimed here; 0x220..0x230 is deliberately left alone so
// that the two sub-objects can be declared there later without moving anything.
// The constructor already zeroes the whole span, which is exactly the initial
// value retail's constructor writes into every one of these fields.
//
// Field placement differs from retail's -- it has to, because OpenMFC's two
// declared members (m_bRegistered at 0x1e8, m_lpMoniker at 0x1f0) sit on top of
// retail's m_pFactory and m_bDeferErrors -- so the retail offset each field
// stands in for is named next to it.  Nothing outside this file reads these
// bytes, and no offset already asserted anywhere in the tree is touched.
//-----------------------------------------------------------------------------
struct S_COleLinkingDocExtra {
    COleObjectFactory* pFactory;   // doc+0x1f8   (retail m_pFactory,       +0x1e8)
    DWORD dwRegister;              // doc+0x200   (retail m_dwRegister,     +0x200)
    BOOL  bVisibleLock;            // doc+0x204   (retail m_bVisibleLock,   +0x218)
    int   nDeferErrors;            // doc+0x208   (retail m_bDeferErrors,   +0x1f0)
    long  scDeferred;              // doc+0x20c   (see ReportSaveLoadException)
    int   bHaveDeferred;           // doc+0x210   (retail m_pLastException != NULL)
    int   reserved;                // doc+0x214
    wchar_t* pszMoniker;           // doc+0x218   (retail m_strMoniker,     +0x210)
};

const size_t kExtraOffset = 0x1f8;
static_assert(sizeof(COleLinkingDoc) == 0x230, "COleLinkingDoc must stay 560 bytes (retail sizeof)");
static_assert(offsetof(COleLinkingDoc, m_bRegistered) == 0x1e8,
              "COleDocument must still end at 0x1e8, where retail starts COleLinkingDoc");
static_assert(offsetof(COleLinkingDoc, m_lpMoniker) == 0x1f0, "m_lpMoniker moved");
static_assert(sizeof(S_COleLinkingDocExtra) == 0x28,
              "the overlay must stop at doc+0x220, where the nested parts belong");

S_COleLinkingDocExtra* Extra(const COleLinkingDoc* p) {
    return reinterpret_cast<S_COleLinkingDocExtra*>(
        const_cast<char*>(reinterpret_cast<const char*>(p)) + kExtraOffset);
}

// Assign to / clear the stand-in for retail's m_strMoniker.  malloc/free rather
// than new[]/CString so the overlay stays a plain POD sitting in the padding.
void SetMonikerPath(S_COleLinkingDocExtra* x, const wchar_t* psz) {
    if (x->pszMoniker != nullptr) { ::free(x->pszMoniker); x->pszMoniker = nullptr; }
    if (psz == nullptr || *psz == L'\0') return;
    const size_t cch = ::wcslen(psz) + 1;
    x->pszMoniker = static_cast<wchar_t*>(::malloc(cch * sizeof(wchar_t)));
    if (x->pszMoniker != nullptr) ::memcpy(x->pszMoniker, psz, cch * sizeof(wchar_t));
}

// The two nested interface sub-objects, at the retail offsets pinned above.
// OpenMFC does not declare or construct them (headerRequest filed), so no
// vftable pointer is ever parked at either address and nothing in a running
// OpenMFC process can hand out one of these interface pointers -- the twelve
// XPersistFile/XOleItemContainer entry points below are therefore unreachable
// today.  They are written against the retail offsets so that they become live
// and correct the moment the parts are added; the static_asserts above prove
// the object is still laid out the way the offsets assume.
const ptrdiff_t kXPersistFileOffset      = 0x220;
const ptrdiff_t kXOleItemContainerOffset = 0x228;

COleLinkingDoc* DocFromPersistFile(void* pThisItf) {
    return reinterpret_cast<COleLinkingDoc*>(
        reinterpret_cast<char*>(pThisItf) - kXPersistFileOffset);
}
COleLinkingDoc* DocFromItemContainer(void* pThisItf) {
    return reinterpret_cast<COleLinkingDoc*>(
        reinterpret_cast<char*>(pThisItf) - kXOleItemContainerOffset);
}

// AfxRelease(LPUNKNOWN*) -- the unexported helper at mfc140 0x26ba84 that
// Revoke (0x25cfc0) and Register (0x25cdf0) both call:
//     if (*pp != NULL) { (*pp)->Release(); *pp = NULL; }
void ReleaseAndClear(LPMONIKER& rpmk) {
    if (rpmk != nullptr) { rpmk->Release(); rpmk = nullptr; }
}

// CCmdTarget::m_dwRef, the word retail's `lock incl 0x8(%rcx)` /
// `lock decl 0x8(%rcx)` touch in UpdateVisibleLock and OnCloseDocument (that is
// the inlined CCmdTarget::InternalAddRef, which has no thunk in OpenMFC).
// OpenMFC's CCmdTarget keeps m_dwRef at the same +0x08 -- pinned by the
// static_asserts on S_CCmdTarget in core/runtime/CCmdTarget.cpp -- so the raw
// interlocked update is reproduced exactly.
static_assert(sizeof(CCmdTarget) == 32, "OpenMFC CCmdTarget is 32 bytes (m_dwRef at +0x08)");
LONG volatile* RefWord(CCmdTarget* p) {
    return reinterpret_cast<LONG volatile*>(reinterpret_cast<char*>(p) + 0x08);
}

// IID_IStorage, {0000000B-0000-0000-C000-000000000046}: the 16 bytes at .rdata
// 0x2d7a78 (mfc140) / 0x2d9b28 (mfc140u) that GetObjectStorage memcmp's riid
// against.  Spelled out locally rather than pulled from libuuid.
const GUID kIID_IStorage =
    { 0x0000000B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

// The delimiter set XOleItemContainer::ParseDisplayName (0x25da60) tests with
// `bt %rax,%r14` against the immediate 0xc00000002004001 biased by 0x21: bits
// 0, 14, 25, 58 and 59, i.e. '!', '/', ':', '[' and '\\'.
bool IsItemDelimiter(wchar_t ch) {
    return ch == L'!' || ch == L'/' || ch == L':' || ch == L'[' || ch == L'\\';
}

} // namespace

//=============================================================================
// Construction / teardown
//=============================================================================

// ??0COleLinkingDoc@@QEAA@XZ -- retail 0x25cc00 runs the COleDocument ctor,
// installs the COleLinkingDoc vftable (.rdata 0x32e3a0) and the two nested-part
// vftables at +0x220/+0x228, default-constructs m_strMoniker and zeroes
// m_pFactory, m_bDeferErrors, m_pLastException, m_dwRegister, m_lpMonikerROT
// and m_bVisibleLock.  OpenMFC installs no nested-part vftables (the parts are
// not declared); the memset below zeroes the whole padding span, which gives
// the S_COleLinkingDocExtra overlay exactly the initial values retail's
// constructor writes.
COleLinkingDoc::COleLinkingDoc()
    : COleDocument(), m_bRegistered(FALSE), m_lpMoniker(nullptr) {
    memset(_colelinkingdoc_padding, 0, sizeof(_colelinkingdoc_padding));
}

// ??1COleLinkingDoc@@UEAA@XZ -- retail 0x25cce0:
//     CDocument::DisconnectViews();          // 0x219200, via 0x25cd0d
//     Revoke();                              // 0x25cfc0, via 0x25cd15
//     CCmdTarget::ExternalDisconnect();      // 0x26bec0, via 0x25cd1d
//     <release m_strMoniker>                 // 0x25cd2a
//     ~COleDocument();                       // 0x253090
// (Retail wraps the first three calls in TRY/END_TRY; catch continuation at
// 0x25cd25.)  DisconnectViews is not called here: OpenMFC's ~CDocument
// (impl___1CDocument__UEAA_XZ in core/doc/CDocument.cpp) already detaches every
// view from the document, so the only difference is that views are detached
// after Revoke/ExternalDisconnect instead of before them.  (Calling it here
// too would be harmless -- the second pass finds an empty list.)
COleLinkingDoc::~COleLinkingDoc() {
    Revoke();
    impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(this);
    SetMonikerPath(Extra(this), nullptr);
}

//=============================================================================
// Moniker / ROT registration (typed members; their exports are the thunks in
// core/ole/Thunks.cpp, which forward to these bodies)
//=============================================================================

// ?GetMoniker@COleLinkingDoc@@UEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z --
// retail 0x25cd80:
//     if (m_strMoniker.IsEmpty())                       // 0x25cd8d
//         return COleDocument::GetMoniker(nAssign);     // tail call 0x253aa0
//     LPMONIKER pmk = NULL;
//     ::CreateFileMoniker(m_strMoniker, &pmk);          // 0x25cdae, HRESULT ignored
//     return pmk;
// Note the retail contract: the returned moniker is a NEW reference the caller
// owns; m_lpMonikerROT is not what comes back.  The body that used to be here
// returned the cached m_lpMoniker without an AddRef, which is the opposite
// contract, so it is corrected.
LPMONIKER COleLinkingDoc::GetMoniker(OLEGETMONIKER nAssign) {
    S_COleLinkingDocExtra* x = Extra(this);
    if (x->pszMoniker == nullptr) {
        return reinterpret_cast<LPMONIKER>(
            impl__GetMoniker_COleDocument__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(
                this, static_cast<int>(nAssign)));
    }
    LPMONIKER pmk = nullptr;
    ::CreateFileMoniker(x->pszMoniker, &pmk);
    return pmk;
}

// GetFileMoniker is not an MFC entry point (retail COleLinkingDoc has no such
// method); it is an OpenMFC convenience that hands back the cached ROT moniker,
// retail's m_lpMonikerROT, without adding a reference.
LPMONIKER COleLinkingDoc::GetFileMoniker() {
    return m_lpMoniker;
}

// ?RegisterIfServerAttached@COleLinkingDoc@@MEAAHPEB_WH@Z -- retail 0x25d350:
//     CDocTemplate* pTemplate = m_pDocTemplate;               // doc+0x50
//     if (IsSearchAndOrganizeHandler() && pTemplate == NULL)
//         return TRUE;                                        // 0x25d38a
//     COleObjectFactory* pFactory = pTemplate->[tmpl+0x48];   // m_pAttachedFactory
//     if (pFactory == NULL) return TRUE;                      // 0x25d393
//     m_pFactory = pFactory;                                  // 0x25d398
//     if (Register(pFactory, lpszPathName)) return TRUE;      // 0x25d39f
//     if (bMessage)
//         ReportSaveLoadException(lpszPathName, NULL, FALSE, 0xf185);
//             // vftable slot 0x120; 0xf185 == 61829
//     return FALSE;
// NOT reproduced, and left on the pre-existing OpenMFC helper: neither
// CDocTemplate::m_pAttachedFactory nor CDocument::m_pDocTemplate is declared by
// OpenMFC, so the factory this whole function pivots on cannot be reached.
// EnsureLinkingDocMoniker (detail/OlecoreSupport.cpp:1482) creates the file
// moniker and sets m_bRegistered, which is the moniker half of the job without
// the server-attachment test.
// The second parameter is retail's bMessage (the real afxole.h declares
// `RegisterIfServerAttached(LPCTSTR lpszPathName, BOOL bMessage)`): it only
// selects whether a failed Register is reported.  Retail never touches the
// modified flag here, so it is NOT forwarded as EnsureLinkingDocMoniker's
// setModified -- doing so marked the document dirty straight after every
// successful OnSaveDocument (which passes TRUE).  OpenMFC has no Register
// failure to report on this path, so the flag is unused.
BOOL COleLinkingDoc::RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified) {
    (void)bSetModified;   // retail bMessage, see above
    return EnsureLinkingDocMoniker(this, lpszFileName, FALSE);
}

// ?Revoke@COleLinkingDoc@@QEAAXXZ -- retail 0x25cfc0:
//     if (m_dwRegister != 0) {                                    // 0x25cfc6
//         LPRUNNINGOBJECTTABLE pROT = NULL;
//         if (SUCCEEDED(::GetRunningObjectTable(0, &pROT)) && pROT != NULL) {
//             pROT->Revoke(m_dwRegister);   // IROT vftable byte 0x20, slot 4
//             pROT->Release();              // IUnknown slot 2
//         }
//         m_dwRegister = 0;                                       // 0x25d01b
//     }
//     AfxRelease(&m_lpMonikerROT);                                // 0x25d02c
//     m_strMoniker.Empty();      // tail call to CSimpleStringT::SetString("",0)
// Retail has no m_bRegistered; OpenMFC's stands in for "m_dwRegister != 0".
void COleLinkingDoc::Revoke() {
    S_COleLinkingDocExtra* x = Extra(this);
    if (x->dwRegister != 0) {
        LPRUNNINGOBJECTTABLE pROT = nullptr;
        if (SUCCEEDED(::GetRunningObjectTable(0, &pROT)) && pROT != nullptr) {
            pROT->Revoke(x->dwRegister);
            pROT->Release();
        }
        x->dwRegister = 0;
    }
    ReleaseAndClear(m_lpMoniker);
    SetMonikerPath(x, nullptr);
    m_bRegistered = FALSE;
}

// OpenMFC-only accessor; retail tests m_dwRegister directly.
BOOL COleLinkingDoc::IsRegistered() const {
    return m_bRegistered;
}

// OnShowDocument is an OpenMFC-only entry point (retail COleLinkingDoc has no
// such method -- the visibility hook it does have is OnShowViews, below).
void COleLinkingDoc::OnShowDocument(BOOL bShow) {
    if (!bShow) Revoke();
}

//=============================================================================
// COleLinkingDoc own methods
//=============================================================================

// Symbol: ?EndDeferErrors@COleLinkingDoc@@QEAAJJ@Z
// COleLinkingDoc::EndDeferErrors(SCODE sc) -- retail 0x25d560:
//     --m_bDeferErrors;                                    // 0x25d56a
//     if (m_pLastException != NULL) {                      // 0x25d573
//         if (sc == S_OK)                                  // 0x25d581
//             sc = COleException::Process(m_pLastException);  // 0x25e2e0
//         ++m_pLastException->m_bAutoDelete;               // 0x25d593
//         m_pLastException->Delete();                      // 0x2257d0
//         m_pLastException = NULL;                         // 0x25d5a2
//     }
//     return sc;
// DEVIATION: the deferred CException* itself is not kept.  CException::
// m_bAutoDelete is protected in OpenMFC's declaration, so the ownership
// transfer retail performs (ReportSaveLoadException decrements it, this
// function increments it back and Deletes) cannot be reproduced, and holding a
// raw pointer the thrower may already have Deleted would dangle.  What is kept
// instead is the SCODE COleException::Process would have produced, computed at
// the moment the exception was reported -- so the value this function returns
// matches retail even though the object is not carried across.
extern "C" long MS_ABI impl__EndDeferErrors_COleLinkingDoc__QEAAJJ_Z(COleLinkingDoc* pThis, long sc) {
    if (!pThis) return sc;
    S_COleLinkingDocExtra* x = Extra(pThis);
    --x->nDeferErrors;
    if (x->bHaveDeferred) {
        if (sc == S_OK) sc = x->scDeferred;
        x->bHaveDeferred = 0;
        x->scDeferred = 0;
    }
    return sc;
}

// Symbol: ?GetContainer@COleLinkingDoc@@UEAAPEAUIOleItemContainer@@XZ
// COleLinkingDoc::GetContainer() -- retail 0x25d3f0 is four instructions:
//     LPOLEITEMCONTAINER lp;
//     InternalQueryInterface(&IID_IOleItemContainer, (LPVOID*)&lp);  // 0x26bda0
//     return lp;                                          // 0x25d405
// The HRESULT is discarded; InternalQueryInterface writes NULL into the slot on
// failure.  The IID is the {0000011C-...} at .rdata 0x2d7b18.
// This returns NULL in OpenMFC today, for two independent reasons: OpenMFC's
// CCmdTarget::GetInterface (core/runtime/CCmdTarget.cpp) only ever walks
// CCmdTarget's OWN interface map, never the object's, and that map has no
// entries; and g_imap_COleLinkingDoc (detail/InterfaceMapsSupport.cpp:40-41)
// is itself entry-less.  Populating the latter alone would not change the
// result.
extern "C" IOleItemContainer* MS_ABI impl__GetContainer_COleLinkingDoc__UEAAPEAUIOleItemContainer__XZ(
        COleLinkingDoc* pThis) {
    if (!pThis) return nullptr;
    void* lp = nullptr;
    impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(pThis, &IID_IOleItemContainer, &lp);
    return static_cast<IOleItemContainer*>(lp);
}

// Symbol: ?LockExternal@COleLinkingDoc@@IEAAXHH@Z
// COleLinkingDoc::LockExternal(BOOL bLock, BOOL bRemoveRefs) -- retail 0x25d4c0:
//     ::CoLockObjectExternal(GetInterface(&IID_IUnknown), bLock, bRemoveRefs);
//         // GetInterface 0x26bc00; the import slot at 0x2c5b38 resolves to
//         // ole32!CoLockObjectExternal
//     if (bLock && !m_strPathName.IsEmpty()) {            // 0x25d4f1/0x25d4f9
//         Revoke();                                        // 0x25cfc0
//         RegisterIfServerAttached(m_strPathName, FALSE);  // vftable slot 0x2a0
//     }
// The CoLockObjectExternal call is unconditional in retail, but OpenMFC's
// GetInterface returns NULL for every IID until the interface maps are
// populated, and CoLockObjectExternal(NULL, ...) faults -- so the call is
// skipped on NULL, the same guard core/runtime/CCmdTarget.cpp applies in
// ExternalDisconnect.  The re-registration half is unaffected.
extern "C" void MS_ABI impl__LockExternal_COleLinkingDoc__IEAAXHH_Z(
        COleLinkingDoc* pThis, int bLock, int bRemoveRefs) {
    if (!pThis) return;
    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &IID_IUnknown);
    if (pUnk != nullptr)
        ::CoLockObjectExternal(pUnk, bLock ? TRUE : FALSE, bRemoveRefs ? TRUE : FALSE);

    const wchar_t* lpszPathName = pThis->GetPathName();
    if (bLock && lpszPathName != nullptr && *lpszPathName != L'\0') {
        pThis->Revoke();
        pThis->RegisterIfServerAttached(lpszPathName, FALSE);
    }
}

// Symbol: ?UpdateVisibleLock@COleLinkingDoc@@IEAAXHH@Z
// COleLinkingDoc::UpdateVisibleLock(BOOL bVisible, BOOL bRemoveRefs) --
// retail 0x25d2b0:
//     if (bVisible != m_bVisibleLock) {            // 0x25d2b9
//         ++m_dwRef;                               // lock incl 0x8(%rcx)
//         m_bVisibleLock = bVisible;               // 0x25d2c5
//         LockExternal(bVisible, bRemoveRefs);     // 0x25d4c0
//         InternalRelease();                       // 0x26bb70
//     }
// The raw `++m_dwRef` (the inlined CCmdTarget::InternalAddRef, which has no
// thunk) is reproduced as an interlocked increment of the same +0x08 word; see
// RefWord above.
extern "C" void MS_ABI impl__UpdateVisibleLock_COleLinkingDoc__IEAAXHH_Z(
        COleLinkingDoc* pThis, int bVisible, int bRemoveRefs) {
    if (!pThis) return;
    S_COleLinkingDocExtra* x = Extra(pThis);
    if (bVisible != x->bVisibleLock) {
        ::InterlockedIncrement(RefWord(pThis));
        x->bVisibleLock = bVisible;
        impl__LockExternal_COleLinkingDoc__IEAAXHH_Z(pThis, bVisible, bRemoveRefs);
        impl__InternalRelease_CCmdTarget__QEAAKXZ(pThis);
    }
}

// Symbol: ?OnShowViews@COleLinkingDoc@@MEAAXH@Z
// COleLinkingDoc::OnShowViews(BOOL bVisible) -- retail 0x25d2e0 is six
// instructions:
//     if (bVisible)                                  // test %edx,%edx
//         UpdateVisibleLock(bVisible, TRUE);         // 0x25d2b0, r8d = 1
// (%edx is passed through untouched, so the first argument is the caller's
// bVisible, which on that path is known non-zero.)
extern "C" void MS_ABI impl__OnShowViews_COleLinkingDoc__MEAAXH_Z(COleLinkingDoc* pThis, int bVisible) {
    if (!pThis) return;
    if (bVisible)
        impl__UpdateVisibleLock_COleLinkingDoc__IEAAXHH_Z(pThis, bVisible, TRUE);
}

// Symbol: ?Register@COleLinkingDoc@@QEAAHPEAVCOleObjectFactory@@PEB_W@Z
// COleLinkingDoc::Register(COleObjectFactory* pFactory, LPCTSTR lpszPathName) --
// retail 0x25cdf0:
//     m_pFactory = pFactory;                                   // 0x25ce0e
//     AfxRelease(&m_lpMonikerROT);                             // 0x25ce23
//     m_strMoniker.Empty();                                    // 0x25ce32
//     BOOL bResult = TRUE;
//     if (lpszPathName != NULL) {                              // 0x25ce37
//         CString str(lpszPathName);
//         bResult = (::CreateFileMoniker(str, &m_lpMonikerROT) == S_OK);
//     }                                                        // 0x25ce7c
//     if (m_lpMonikerROT != NULL) {                            // 0x25ce82
//         LPRUNNINGOBJECTTABLE pROT = NULL;
//         ::GetRunningObjectTable(0, &pROT);                   // 0x25ce9a
//         LPMONIKER pmkDup = NULL;
//         if (pROT->GetObject(m_lpMonikerROT, &pmkDup) == S_OK) {  // IROT slot 6
//             pmkDup->Release();                               // 0x25cec7
//             AfxRelease(&m_lpMonikerROT);                     // 0x25ced0
//             return FALSE;          // already running -- 0x25ced5
//                 // (retail leaks pROT on this path: no Release)
//         }
//         HRESULT hr = pROT->Register(0, GetInterface(&IID_IUnknown),
//                                     m_lpMonikerROT, &m_dwRegister); // slot 3
//         pROT->Release();                                     // 0x25cf27
//         m_strMoniker.SetString(lpszPathName, lstrlen(lpszPathName)); // 0x25cf48
//         if (hr != S_OK) bResult = FALSE;                     // cmovne 0x25cf51
//     }
//     // then, for every COleClientItem in the document (GetStartPosition
//     // through vftable slot 0x238 and GetNextItemOfKind 0x2531e0 with
//     // RUNTIME_CLASS(COleClientItem) at .rdata 0x32c5f8), retail calls
//     //     pItem->m_lpObject->SetMoniker(OLEWHICHMK_CONTAINER, m_lpMonikerROT)
//     // (IOleObject vftable byte 0x38) guarded by a BOOL byte at item+0x7d.
//     return bResult;
// Two small additions over retail, both confined to failure paths: pROT is
// NULL-checked (below), and it is Released on the already-running path, where
// retail leaks the reference.
// DEVIATION: that final item loop is NOT reproduced.  OpenMFC's COleClientItem
// declares neither the byte at +0x7d that gates it nor any equivalent, so the
// loop could only be run unguarded -- telling every embedded item about the
// container moniker, including the ones retail deliberately skips.  Everything
// up to and including the ROT registration is transcribed as-is.
extern "C" int MS_ABI impl__Register_COleLinkingDoc__QEAAHPEAVCOleObjectFactory__PEB_W_Z(
        COleLinkingDoc* pThis, COleObjectFactory* pFactory, const wchar_t* lpszPathName) {
    if (!pThis) return FALSE;
    S_COleLinkingDocExtra* x = Extra(pThis);
    x->pFactory = pFactory;

    ReleaseAndClear(pThis->m_lpMoniker);
    SetMonikerPath(x, nullptr);
    // m_dwRegister (and so m_bRegistered) is deliberately NOT cleared here:
    // retail only clears it in Revoke, and overwrites it below when the new
    // registration succeeds.
    BOOL bResult = TRUE;
    if (lpszPathName != nullptr)
        bResult = (::CreateFileMoniker(lpszPathName, &pThis->m_lpMoniker) == S_OK);

    if (pThis->m_lpMoniker != nullptr) {
        LPRUNNINGOBJECTTABLE pROT = nullptr;
        ::GetRunningObjectTable(0, &pROT);
        // Retail does not test pROT before using it (0x25cea8 dereferences it
        // straight away); the guard is added because a failed
        // GetRunningObjectTable must not fault here.
        if (pROT == nullptr) return FALSE;

        LPMONIKER pmkDup = nullptr;
        if (pROT->GetObject(pThis->m_lpMoniker, reinterpret_cast<LPUNKNOWN*>(&pmkDup)) == S_OK) {
            if (pmkDup != nullptr) pmkDup->Release();
            pROT->Release();
            ReleaseAndClear(pThis->m_lpMoniker);
            return FALSE;
        }

        // GetInterface returns NULL for every IID until the COleLinkingDoc
        // interface map is populated, so IROT::Register will reject the
        // registration with E_INVALIDARG today and bResult comes back FALSE --
        // which is the same answer retail gives when the ROT refuses.
        IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &IID_IUnknown);
        DWORD dwRegister = 0;
        HRESULT hr = pROT->Register(0, pUnk, pThis->m_lpMoniker, &dwRegister);
        pROT->Release();

        x->dwRegister = (hr == S_OK) ? dwRegister : 0;
        pThis->m_bRegistered = (x->dwRegister != 0);
        SetMonikerPath(x, lpszPathName);
        if (hr != S_OK) bResult = FALSE;
    }
    return bResult;
}

// Symbol: ?OnNewDocument@COleLinkingDoc@@UEAAHXZ
// COleLinkingDoc::OnNewDocument() -- retail 0x25d060:
//     Revoke();                                              // 0x25cfc0
//     RegisterIfServerAttached(NULL, TRUE);                  // vftable 0x2a0
//     if (!COleDocument::OnNewDocument()) return FALSE;      // 0x253b10
//     if (!IsSearchAndOrganizeHandler())                     // 0x25d095..0x25d0ab
//         AfxOleSetUserCtrl(TRUE);   // AfxGetModuleState()->+0x64 = 1, 0x25e1c0
//     return TRUE;
extern "C" int MS_ABI impl__OnNewDocument_COleLinkingDoc__UEAAHXZ(COleLinkingDoc* pThis) {
    if (!pThis) return FALSE;
    pThis->Revoke();
    pThis->RegisterIfServerAttached(nullptr, TRUE);
    if (!impl__OnNewDocument_COleDocument__UEAAHXZ(pThis)) return FALSE;
    if (!impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(pThis))
        impl__AfxOleSetUserCtrl__YAXH_Z(TRUE);
    return TRUE;
}

// Symbol: ?OnOpenDocument@COleLinkingDoc@@UEAAHPEB_W@Z
// COleLinkingDoc::OnOpenDocument(LPCTSTR lpszPathName) -- retail 0x25d0d0:
//     Revoke();                                              // 0x25cfc0
//     RegisterIfServerAttached(lpszPathName, FALSE);         // vftable 0x2a0
//     if (!COleDocument::OnOpenDocument(lpszPathName)) {     // 0x253b80
//         Revoke();                                          // 0x25d110
//         return FALSE;
//     }
//     // AfxGetModuleState() (0x1345b0), then three hops:
//     //   state->[+0x8] -> [+0x128] -> DWORD [+0x18]
//     if (state->[+8]->[+0x128] == NULL ||
//         (state->[+8]->[+0x128]->[+0x18] != 2 &&
//          state->[+8]->[+0x128]->[+0x18] != 4))     // (x-2) & ~2 test, 0x25d139
//         AfxOleSetUserCtrl(TRUE);                           // 0x25d146
//     return TRUE;
// Reading of the hops: this is the "started only to print, or via DDE" test.
// The values are pinned by the real afxwin.h:5025 enum (FileNew=0, FileOpen=1,
// FilePrint=2, FilePrintTo=3, FileDDE=4), and the real CWinApp declares
// CCommandLineInfo* m_pCmdInfo (afxwin.h:5415); the hops are consistent with
// AFX_MODULE_STATE::m_pCurrentWinApp -> CWinApp::m_pCmdInfo ->
// CCommandLineInfo::m_nShellCommand (whose ctor, 0x1ca400, starts its CString
// members at +0x20, right after a DWORD at +0x18).  The +0x8 and +0x128
// member identities were not independently confirmed from the disassembly.
// DEVIATION: the guard is not reproduced -- OpenMFC's CWinApp declares no
// m_pCmdInfo (its command-line state lives in detail/CWinAppSupport's side
// table, keyed by the CCommandLineInfo object, with a different enum
// numbering), so the shell command of the running app cannot be read here.
// AfxOleSetUserCtrl(TRUE) is therefore called unconditionally, which is
// retail's behaviour for every shell command except FilePrint and FileDDE.
extern "C" int MS_ABI impl__OnOpenDocument_COleLinkingDoc__UEAAHPEB_W_Z(
        COleLinkingDoc* pThis, const wchar_t* lpszPathName) {
    if (!pThis) return FALSE;
    pThis->Revoke();
    pThis->RegisterIfServerAttached(lpszPathName, FALSE);
    if (!impl__OnOpenDocument_COleDocument__UEAAHPEB_W_Z(pThis, lpszPathName)) {
        pThis->Revoke();
        return FALSE;
    }
    impl__AfxOleSetUserCtrl__YAXH_Z(TRUE);
    return TRUE;
}

// Symbol: ?OnSaveDocument@COleLinkingDoc@@UEAAHPEB_W@Z
// COleLinkingDoc::OnSaveDocument(LPCTSTR lpszPathName) -- retail 0x25d160:
//     BOOL bRemember = m_bRemember;                      // doc+0x1d4, 0x25d175
//     if (!COleDocument::OnSaveDocument(lpszPathName))   // 0x253df0
//         return FALSE;
//     if (bRemember) {
//         if (lpszPathName == NULL) <throw E_FAIL>       // 0x25d1db
//         if (_mbscmp(m_strMoniker, lpszPathName) != 0) {   // 0x25d197
//                 // mfc140u (body at RVA 0x25e250 (mfc140u), vftable byte
//                 // 0x110) calls wcscmp here, import slot 0x1802c7770
//                 // resolved against mfc140u.dll
//             Revoke();                                  // 0x25cfc0
//             RegisterIfServerAttached(lpszPathName, TRUE);  // vftable 0x2a0
//         }
//     }
//     return TRUE;
// The E_FAIL throw is replaced by returning FALSE: OpenMFC has no exception
// path that matches it here and a NULL path name with m_bRemember set cannot
// be saved either way.
extern "C" int MS_ABI impl__OnSaveDocument_COleLinkingDoc__UEAAHPEB_W_Z(
        COleLinkingDoc* pThis, const wchar_t* lpszPathName) {
    if (!pThis) return FALSE;
    const BOOL bRemember = pThis->m_bRemember;
    if (!impl__OnSaveDocument_COleDocument__UEAAHPEB_W_Z(pThis, lpszPathName)) return FALSE;
    if (bRemember) {
        if (lpszPathName == nullptr) return FALSE;
        const wchar_t* pszMoniker = Extra(pThis)->pszMoniker;
        if (::wcscmp(pszMoniker != nullptr ? pszMoniker : L"", lpszPathName) != 0) {
            pThis->Revoke();
            pThis->RegisterIfServerAttached(lpszPathName, TRUE);
        }
    }
    return TRUE;
}

// Symbol: ?OnCloseDocument@COleLinkingDoc@@UEAAXXZ
// COleLinkingDoc::OnCloseDocument() -- retail 0x25d1f0:
//     if (IsSearchAndOrganizeHandler() && !m_bFinalRelease /*doc+0x94*/)
//         return;                                        // 0x25d218/0x25d21f
//     ++m_dwRef;                                         // lock incl 0x8
//     UpdateVisibleLock(FALSE, FALSE);                   // 0x25d2b0
//     Revoke();                                          // 0x25cfc0
//     if (m_bVisibleLock) {                              // 0x25d237
//         m_bVisibleLock = FALSE;
//         ::CoLockObjectExternal(GetInterface(&IID_IUnknown), FALSE, FALSE);
//     }
//     BOOL bAutoDelete = m_bAutoDelete;   // doc+0x120, 0x25d267
//     m_bAutoDelete = FALSE;                             // 0x25d270
//     COleDocument::OnCloseDocument();                   // 0x254000
//     --m_dwRef;                                         // lock decl 0x8
//     if (bAutoDelete)
//         this->vftable[+0x08](1);   // scalar deleting destructor: delete this
// DEVIATIONS:
//   * m_bFinalRelease is not declared by OpenMFC's CDocument, so the early-out
//     degenerates to `if (IsSearchAndOrganizeHandler()) return;` -- i.e. the
//     handler-mode document is never closed here, where retail still closes it
//     during its own final release.
//   * the ++/-- pair is reproduced as interlocked updates of m_dwRef (+0x08),
//     exactly as retail does it; see RefWord.
//   * `delete this` is spelled directly rather than through the retail vftable
//     slot; OpenMFC's C++ vftable is unrelated to retail's, and the virtual
//     ~COleLinkingDoc gives the same dispatch.
extern "C" void MS_ABI impl__OnCloseDocument_COleLinkingDoc__UEAAXXZ(COleLinkingDoc* pThis) {
    if (!pThis) return;
    if (impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(pThis)) return;

    ::InterlockedIncrement(RefWord(pThis));
    impl__UpdateVisibleLock_COleLinkingDoc__IEAAXHH_Z(pThis, FALSE, FALSE);
    pThis->Revoke();

    S_COleLinkingDocExtra* x = Extra(pThis);
    if (x->bVisibleLock) {
        x->bVisibleLock = FALSE;
        IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(pThis, &IID_IUnknown);
        if (pUnk != nullptr) ::CoLockObjectExternal(pUnk, FALSE, FALSE);
    }

    const BOOL bAutoDelete = pThis->m_bAutoDelete;
    pThis->m_bAutoDelete = FALSE;
    impl__OnCloseDocument_COleDocument__UEAAXXZ(pThis);
    ::InterlockedDecrement(RefWord(pThis));
    if (bAutoDelete) delete pThis;
}

// Symbol: ?ReportSaveLoadException@COleLinkingDoc@@UEAAXPEB_WPEAVCException@@HI@Z
// COleLinkingDoc::ReportSaveLoadException(LPCTSTR lpszPathName, CException* e,
//                                         BOOL bSaving, UINT nIDPDefault) --
// retail 0x25d540, eight instructions:
//     if (m_bDeferErrors == 0)                           // 0x25d540
//         CDocument::ReportSaveLoadException(...);       // tail jmp 0x219d90
//     else {
//         if (e != NULL) --e->m_bAutoDelete;             // 0x25d54e
//         m_pLastException = e;                          // 0x25d552
//     }
// Note the store at 0x25d552 is unconditional: a NULL e overwrites any earlier
// deferred exception with NULL, so EndDeferErrors then leaves sc untouched.
// That is reproduced (bHaveDeferred is cleared for a NULL e).
// DEVIATION, as described at EndDeferErrors: the exception object is not
// retained (its m_bAutoDelete is protected in OpenMFC and cannot be
// decremented, so the pointer could not safely outlive the caller's handler).
// The SCODE COleException::Process would yield is computed here instead and
// handed back by EndDeferErrors.  Nothing is reported to the user on the
// deferred path, which matches retail.
extern "C" void MS_ABI impl__ReportSaveLoadException_COleLinkingDoc__UEAAXPEB_WPEAVCException__HI_Z(
        COleLinkingDoc* pThis, const wchar_t* lpszPathName, CException* e,
        int bSaving, unsigned int nIDPDefault) {
    if (!pThis) return;
    S_COleLinkingDocExtra* x = Extra(pThis);
    if (x->nDeferErrors == 0) {
        impl__ReportSaveLoadException_CDocument__UEAAXPEB_WPEAVCException__HI_Z(
            pThis, lpszPathName, e, bSaving, nIDPDefault);
        return;
    }
    if (e == nullptr) {
        x->bHaveDeferred = 0;
        x->scDeferred = 0;
        return;
    }
    x->bHaveDeferred = 1;
    x->scDeferred = impl__Process_COleException__SAJPEBVCException___Z(e);
}

// Symbol: ?SaveToStorage@COleLinkingDoc@@MEAAXPEAVCObject@@@Z
// COleLinkingDoc::SaveToStorage(CObject* pObject) -- retail 0x25d300:
//     if (m_pFactory != NULL)                                  // 0x25d310
//         ::WriteClassStg(m_lpRootStg /*doc+0x1c8*/,
//                         m_pFactory->m_clsid /*factory+0x4c*/);  // 0x25d327
//     COleDocument::SaveToStorage(pObject);   // tail jmp 0x254080
// DEVIATION: the WriteClassStg half is not reproduced.  OpenMFC's COleDocument
// does not declare the root IStorage (COleDocument.cpp keeps it in that file's
// own OleDocExtra side table, which is private to that translation unit), so
// there is no storage to stamp the class id onto.  The unconditional tail call
// to the base implementation is reproduced exactly.
extern "C" void MS_ABI impl__SaveToStorage_COleLinkingDoc__MEAAXPEAVCObject___Z(
        COleLinkingDoc* pThis, CObject* pObject) {
    if (!pThis) return;
    impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(pThis, pObject);
}

// Symbol: ?OnFindEmbeddedItem@COleLinkingDoc@@MEAAPEAVCOleClientItem@@PEB_W@Z
// COleLinkingDoc::OnFindEmbeddedItem(LPCTSTR lpszItemName) -- retail 0x25d410:
//     POSITION pos = GetStartPosition();        // vftable slot 0x238, 0x25d43e
//     CDocItem* p;
//     while ((p = GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem)))  // 0x2531e0
//            != NULL) {                         // class descriptor .rdata 0x32c5f8
//         TCHAR szName[0x15];
//         <format helper 0x1ce450>(szName, 0x15, "Embedding %lu",  // .rdata 0x346e00
//                                  p->[item+0x58] /* m_nItemNumber */);
//         if (lstrcmp(szName, lpszItemName) == 0) return (COleClientItem*)p;
//     }
//     return NULL;
// Left as a stub: the per-item number at COleClientItem+0x58 is the only thing
// the comparison is built from, and OpenMFC's COleClientItem does not declare
// it -- the same gap that leaves COleClientItem::GetNewItemNumber and
// GetItemStorageCompound stubbed in core/ole/COleClientItem.cpp.  Returning an
// arbitrary item instead of the one whose number matches would hand a linking
// client the wrong embedding.
extern "C" COleClientItem* MS_ABI impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(
        COleLinkingDoc* pThis, const wchar_t* lpszItemName) {
    (void)pThis; (void)lpszItemName;
    return nullptr;
}

// Symbol: ?OnGetLinkedItem@COleLinkingDoc@@MEAAPEAVCOleServerItem@@PEB_W@Z
// COleLinkingDoc::OnGetLinkedItem(LPCTSTR) -- COleLinkingDoc's own override has
// no RVA of its own in the export map; slot 0x290 of the COleLinkingDoc vftable
// (.rdata 0x32e3a0) holds the identical-code-folded two-instruction body at
// mfc140 0x7260, `xor %eax,%eax ; ret`.  The base class knows about no linked
// items; only COleServerDoc's override (0x264bf0) searches.
extern "C" COleServerItem* MS_ABI impl__OnGetLinkedItem_COleLinkingDoc__MEAAPEAVCOleServerItem__PEB_W_Z(
        COleLinkingDoc* pThis, const wchar_t* lpszItemName) {
    (void)pThis; (void)lpszItemName;
    return nullptr;
}

//=============================================================================
// COleLinkingDoc::XPersistFile -- nested IPersistFile part, retail +0x220.
// Every method opens with METHOD_PROLOGUE_EX_(COleLinkingDoc, PersistFile),
// which is `lea -0x220(%rcx),<reg>` plus, where the body needs it, an
// AFX_MAINTAIN_STATE2 built from pThis->m_pModuleState (doc+0x38) at 0x133df0
// and popped again on the way out.  The module-state push is never reproduced
// below: it pushes CCmdTarget::m_pModuleState from doc+0x38, and OpenMFC's
// CCmdTarget stops at 32 bytes (core/runtime/CCmdTarget.cpp), so there is no
// stored per-object module state to push.
//=============================================================================

// Symbol: ?GetClassID@XPersistFile@COleLinkingDoc@@UEAAJPEAU_GUID@@@Z
// XPersistFile::GetClassID(LPCLSID lpClassID) -- retail 0x25d640, no module
// state at all, seven instructions:
//     COleObjectFactory* pF = pThis->m_pFactory;   // mov -0x38(%rcx) == doc+0x1e8
//     if (pF == NULL) { *lpClassID = GUID_NULL /* .rdata 0x2d7828 */;
//                       return E_FAIL; }
//     *lpClassID = pF->m_clsid;                    // movups 0x4c(%rax)
//     return S_OK;
// Addition over retail: a NULL lpClassID returns E_POINTER here, where retail
// writes through it unconditionally (movdqu %xmm0,(%rdx)) and faults.
extern "C" long MS_ABI impl__GetClassID_XPersistFile_COleLinkingDoc__UEAAJPEAU_GUID___Z(
        void* pThisItf, GUID* lpClassID) {
    if (!lpClassID) return E_POINTER;
    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    COleObjectFactory* pFactory = Extra(pThis)->pFactory;
    if (pFactory == nullptr) { *lpClassID = GUID_NULL; return E_FAIL; }
    *lpClassID = pFactory->GetClassID();
    return S_OK;
}

// Symbol: ?IsDirty@XPersistFile@COleLinkingDoc@@UEAAJXZ
// XPersistFile::IsDirty() -- retail 0x25d670:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, PersistFile)   // lea -0x220(%rcx)
//     return pThis->IsModified() ? S_OK : S_FALSE;
//         // vftable byte 0xd0 (CDocument::IsModified); `sete %dl` inverts the
//         // BOOL into 0/1, i.e. S_OK when modified.
extern "C" long MS_ABI impl__IsDirty_XPersistFile_COleLinkingDoc__UEAAJXZ(void* pThisItf) {
    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    return impl__IsModified_CDocument__UEBAHXZ(pThis) ? S_OK : S_FALSE;
}

// Symbol: ?Load@XPersistFile@COleLinkingDoc@@UEAAJPEB_WK@Z
// XPersistFile::Load(LPCOLESTR lpszFileName, DWORD dwMode) -- retail 0x25d6c0:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, PersistFile)
//     SCODE sc = E_FAIL;
//     ++pThis->m_bDeferErrors;              // BeginDeferErrors, incl 0x1f0
//     CString strFileName(lpszFileName);
//     BOOL bUserCtrl = AfxOleGetUserCtrl();                      // 0x25d704
//     if (!pThis->OnOpenDocument(strFileName)) {   // vftable byte 0x108
//         AfxOleSetUserCtrl(bUserCtrl);                          // 0x25d728
//         return E_FAIL;                   // (after EndDeferErrors is skipped:
//                                          //  0x25d76a returns 0x80004005)
//     }
//     pThis->SendInitialUpdate();                                // 0x21acc0
//     pThis->SetPathName(strFileName, FALSE);   // vftable byte 0xc0
//     AfxOleSetUserCtrl(bUserCtrl);                              // 0x25d797
//     sc = S_OK;
//     return pThis->EndDeferErrors(sc);                          // 0x25d560
// Note the failure path really does return E_FAIL *without* running
// EndDeferErrors -- the deferred-error counter stays incremented; that is
// reproduced literally.  dwMode is never read.
// NOT reproduced: retail wraps everything between BeginDeferErrors and the
// EndDeferErrors call in TRY/END_TRY.  The catch continuation at 0x25d7a7
// (no branch in the body targets it; 0x25d7a5 jumps over it) reloads sc from
// its stack slot -- initialised to E_FAIL at 0x25d6e8 and only overwritten by
// the success path at 0x25d7a1 -- and falls into EndDeferErrors(sc).  This DLL raises
// its MFC exceptions through _CxxThrowException, which a gcc catch clause
// cannot see (see the note in core/ole/CMonikerFile.cpp), so a CException
// escaping OnOpenDocument propagates out of this method instead.
extern "C" long MS_ABI impl__Load_XPersistFile_COleLinkingDoc__UEAAJPEB_WK_Z(
        void* pThisItf, const wchar_t* lpszFileName, unsigned long dwMode) {
    (void)dwMode;
    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    ++Extra(pThis)->nDeferErrors;

    const int bUserCtrl = impl__AfxOleGetUserCtrl__YAHXZ();
    if (!impl__OnOpenDocument_COleLinkingDoc__UEAAHPEB_W_Z(pThis, lpszFileName)) {
        impl__AfxOleSetUserCtrl__YAXH_Z(bUserCtrl);
        return E_FAIL;
    }
    impl__SendInitialUpdate_CDocument__QEAAXXZ(pThis);
    impl__SetPathName_COleDocument__UEAAXPEB_WH_Z(pThis, lpszFileName, FALSE);
    impl__AfxOleSetUserCtrl__YAXH_Z(bUserCtrl);
    return impl__EndDeferErrors_COleLinkingDoc__QEAAJJ_Z(pThis, S_OK);
}

// Symbol: ?Save@XPersistFile@COleLinkingDoc@@UEAAJPEB_WH@Z
// XPersistFile::Save(LPCOLESTR lpszFileName, BOOL fRemember) -- retail 0x25d810:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, PersistFile)
//     ++pThis->m_bDeferErrors;                   // BeginDeferErrors
//     pThis->m_bRemember = fRemember;            // doc+0x1d4, 0x25d847
//     if (lpszFileName == NULL)
//         pThis->OnSaveDocument(NULL);           // vftable byte 0x110
//     else {
//         CString str(lpszFileName);
//         pThis->OnSaveDocument(str);
//     }
//     return pThis->EndDeferErrors(S_OK);        // 0x25d560
// The OnSaveDocument return value really is discarded and S_OK is the
// argument to EndDeferErrors on both normal paths (0x25d8b0); a failure only
// surfaces through the deferred exception EndDeferErrors converts.
// NOT reproduced, as in Load: the TRY/END_TRY catch continuation at 0x25d8b8
// (no branch in the body targets it) calls EndDeferErrors with sc reloaded
// from its slot, which holds the E_FAIL stored at 0x25d839 unless the normal
// path already reached 0x25d8b2; a CException escaping OnSaveDocument propagates
// out of this method here.
extern "C" long MS_ABI impl__Save_XPersistFile_COleLinkingDoc__UEAAJPEB_WH_Z(
        void* pThisItf, const wchar_t* lpszFileName, int fRemember) {
    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    ++Extra(pThis)->nDeferErrors;
    pThis->m_bRemember = fRemember;
    impl__OnSaveDocument_COleLinkingDoc__UEAAHPEB_W_Z(pThis, lpszFileName);
    return impl__EndDeferErrors_COleLinkingDoc__QEAAJJ_Z(pThis, S_OK);
}

// Symbol: ?SaveCompleted@XPersistFile@COleLinkingDoc@@UEAAJPEB_W@Z
// XPersistFile::SaveCompleted(LPCOLESTR lpszFileName) -- retail 0x25d8f0:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, PersistFile)
//     if (lpszFileName != NULL) {                              // 0x25d918
//         CString str(lpszFileName);
//         pThis->SetPathName(str, FALSE);       // vftable byte 0xc0, r8d = 0
//     }
//     return S_OK;                                             // 0x25d982
// There is no BeginDeferErrors/EndDeferErrors pair here.
extern "C" long MS_ABI impl__SaveCompleted_XPersistFile_COleLinkingDoc__UEAAJPEB_W_Z(
        void* pThisItf, const wchar_t* lpszFileName) {
    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    if (lpszFileName != nullptr)
        impl__SetPathName_COleDocument__UEAAXPEB_WH_Z(pThis, lpszFileName, FALSE);
    return S_OK;
}

// Symbol: ?GetCurFile@XPersistFile@COleLinkingDoc@@UEAAJPEAPEA_W@Z
// XPersistFile::GetCurFile(LPOLESTR* ppszFileName) -- retail 0x25d9a0, no
// module state:
//     if (ppszFileName == NULL) return E_POINTER;              // 0x25d9b1
//     *ppszFileName = NULL;
//     LPCTSTR psz = pThis->m_strPathName;    // mov -0x1d8(%rcx) == doc+0x48
//     if (psz is the empty string)                             // 0x25d9c6
//         psz = pThis->m_strTitle;           // mov -0x1e0(%rax) == doc+0x40
//     *ppszFileName = <task-memory strdup helper 0x1e5714>(psz);
//     return *ppszFileName ? S_OK : 0x8007000E /* E_OUTOFMEMORY */;
//         // the neg/sbb/not/and sequence at 0x25d9db
// The helper differs between the images: in mfc140 (0x1e5714) it is strlen +
// CoTaskMemAlloc + MultiByteToWideChar (CoTaskMemFree on conversion failure);
// in mfc140u (RVA 0x1e77b4 (mfc140u), called from GetCurFile at RVA 0x25ea90
// (mfc140u)) it is wcslen + CoTaskMemAlloc + memcpy, all three import slots
// resolved against mfc140u.dll.  The body below is the mfc140u one.
extern "C" long MS_ABI impl__GetCurFile_XPersistFile_COleLinkingDoc__UEAAJPEAPEA_W_Z(
        void* pThisItf, wchar_t** ppszFileName) {
    if (!ppszFileName) return E_POINTER;
    *ppszFileName = nullptr;

    COleLinkingDoc* pThis = DocFromPersistFile(pThisItf);
    const wchar_t* psz = pThis->GetPathName();
    if (psz == nullptr || *psz == L'\0') psz = pThis->GetTitle();
    if (psz == nullptr) psz = L"";

    const size_t cch = ::wcslen(psz) + 1;
    wchar_t* pszCopy = static_cast<wchar_t*>(::CoTaskMemAlloc(cch * sizeof(wchar_t)));
    if (pszCopy == nullptr) return 0x8007000EL;   // E_OUTOFMEMORY
    ::memcpy(pszCopy, psz, cch * sizeof(wchar_t));
    *ppszFileName = pszCopy;
    return S_OK;
}

//=============================================================================
// COleLinkingDoc::XOleItemContainer -- nested IOleItemContainer part, retail
// +0x228 (`lea -0x228(%rcx),...` in every prologue).
//=============================================================================

// Symbol: ?EnumObjects@XOleItemContainer@COleLinkingDoc@@UEAAJKPEAPEAUIEnumUnknown@@@Z
// XOleItemContainer::EnumObjects(DWORD grfFlags, LPENUMUNKNOWN* ppenumUnknown) --
// the mfc140 export map has no entry for it, but slot 4 of the mfc140
// XOleItemContainer vftable (.rdata 0x32e300) holds the identical-code-folded
// three-instruction body at mfc140 0x213620; the mfc140u map does resolve it,
// to RVA 0x2151e0 (mfc140u), with the same three instructions:
//     movq $0x0,(%r8) ; mov $0x80004001,%eax ; ret
// i.e. *ppenumUnknown = NULL; return E_NOTIMPL.  No this-adjustment, no module
// state, grfFlags unread -- so this one is exact even though the containing
// sub-object does not exist in OpenMFC.
extern "C" long MS_ABI impl__EnumObjects_XOleItemContainer_COleLinkingDoc__UEAAJKPEAPEAUIEnumUnknown___Z(
        void* pThisItf, unsigned long grfFlags, IEnumUnknown** ppenumUnknown) {
    (void)pThisItf; (void)grfFlags;
    if (ppenumUnknown) *ppenumUnknown = nullptr;
    return E_NOTIMPL;
}

// Symbol: ?LockContainer@XOleItemContainer@COleLinkingDoc@@UEAAJH@Z
// XOleItemContainer::LockContainer(BOOL fLock) -- retail 0x25dc90, six
// instructions and no module state:
//     pThis->LockExternal(fLock, TRUE);     // add $-0x228,%rcx ; r8d = 1 ;
//                                           // call 0x25d4c0
//     return S_OK;
extern "C" long MS_ABI impl__LockContainer_XOleItemContainer_COleLinkingDoc__UEAAJH_Z(
        void* pThisItf, int fLock) {
    COleLinkingDoc* pThis = DocFromItemContainer(pThisItf);
    impl__LockExternal_COleLinkingDoc__IEAAXHH_Z(pThis, fLock, TRUE);
    return S_OK;
}

// Symbol: ?GetObjectW@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_WKPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
// XOleItemContainer::GetObject(LPOLESTR lpszItem, DWORD dwSpeedNeeded,
//                              LPBINDCTX pbc, REFIID riid, LPVOID* ppvObject) --
// retail 0x25dcb0 (exported from the ANSI image as GetObjectA):
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, OleItemContainer)   // lea -0x228
//     if (ppvObject == NULL) return E_POINTER;                // 0x25dcff
//     *ppvObject = NULL;
//     SCODE sc = MK_E_NOOBJECT /*0x800401e5*/;
//     CString strItemName(lpszItem);
//     COleClientItem* pItem = pThis->OnFindEmbeddedItem(strItemName); // slot 0x298
//     if (pItem != NULL) {
//         sc = S_OK;                                          // 0x25dd4a
//         if (!::OleIsRunning(pItem->m_lpObject /*item+0x48*/)) {  // 0x25dd59
//             if (dwSpeedNeeded != BINDSPEED_INDEFINITE /*1*/)
//                 sc = MK_E_EXCEEDEDDEADLINE /*0x800401e1*/;  // 0x25dd69
//             else
//                 sc = ::OleRun(pItem->m_lpObject);           // 0x25dd77
//         }
//         if (sc == S_OK)
//             sc = pItem->m_lpObject->QueryInterface(riid, ppvObject); // 0x25dda3
//     } else {
//         COleServerItem* pSrv = pThis->OnGetLinkedItem(strItemName); // slot 0x290
//         if (pSrv != NULL) {
//             if (!pSrv->[item+0x50] /*m_bNeedUnlock*/) {     // 0x25ddd4
//                 pThis->LockExternal(TRUE, FALSE);           // 0x25dde4
//                 pSrv->[item+0x50] = TRUE;
//             }
//             sc = pSrv->ExternalQueryInterface(&riid, ppvObject);
//                 // inlined: m_pOuterUnknown (cmd+0x10) if set, else
//                 // CCmdTarget::InternalQueryInterface (0x26bda0)
//         }
//     }
//     return sc;
// DEVIATIONS, both confined to the linked-item branch:
//   * COleServerItem::m_bNeedUnlock (retail item+0x50) is not declared by
//     OpenMFC, so the one-shot LockExternal(TRUE, FALSE) it gates cannot be
//     tracked and is omitted rather than run on every bind.
//   * the ExternalQueryInterface cannot be issued either: OpenMFC derives
//     COleServerItem from CDocItem : CObject, where retail derives CDocItem
//     from CCmdTarget, so the server item has no CCmdTarget sub-object and no
//     interface map to query.  The branch therefore falls through to
//     MK_E_NOOBJECT.  Both are reported as headerRequests.
// OnFindEmbeddedItem is itself a stub in this file (see above), so the embedded
// branch is reachable only once that gap is closed.
extern "C" long MS_ABI impl__GetObjectW_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WKPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(
        void* pThisItf, wchar_t* lpszItem, unsigned long dwSpeedNeeded, IBindCtx* pbc,
        const GUID& riid, void** ppvObject) {
    (void)pbc;
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;

    COleLinkingDoc* pThis = DocFromItemContainer(pThisItf);
    long sc = 0x800401E5L;                                      // MK_E_NOOBJECT

    COleClientItem* pItem =
        impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(pThis, lpszItem);
    if (pItem != nullptr) {
        LPOLEOBJECT lpObject = pItem->GetObject();
        sc = S_OK;
        if (!::OleIsRunning(lpObject)) {
            if (dwSpeedNeeded != 1 /*BINDSPEED_INDEFINITE*/)
                sc = 0x800401E1L;                               // MK_E_EXCEEDEDDEADLINE
            else
                sc = ::OleRun(lpObject);
        }
        if (sc == S_OK)
            sc = lpObject->QueryInterface(riid, ppvObject);
        return sc;
    }

    (void)impl__OnGetLinkedItem_COleLinkingDoc__MEAAPEAVCOleServerItem__PEB_W_Z(pThis, lpszItem);
    return sc;
}

// Symbol: ?IsRunning@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_W@Z
// XOleItemContainer::IsRunning(LPOLESTR lpszItem) -- retail 0x25df90:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, OleItemContainer)   // lea -0x228
//     CString strItemName(lpszItem);
//     COleClientItem* pItem = pThis->OnFindEmbeddedItem(strItemName); // slot 0x298
//     if (pItem != NULL)
//         return ::OleIsRunning(pItem->m_lpObject /*item+0x48*/)     // 0x25dfed
//                    ? S_OK       // 0x25e06d
//                    : S_FALSE;   // 0x25e032
//     SCODE sc = MK_E_NOOBJECT /*0x800401e5*/;
//     if (pThis->OnGetLinkedItem(strItemName) != NULL)   // slot 0x290, 0x25e08c
//         sc = S_OK;                                     // cmovne at 0x25e095
//     return sc;
extern "C" long MS_ABI impl__IsRunning_XOleItemContainer_COleLinkingDoc__UEAAJPEA_W_Z(
        void* pThisItf, wchar_t* lpszItem) {
    COleLinkingDoc* pThis = DocFromItemContainer(pThisItf);
    COleClientItem* pItem =
        impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(pThis, lpszItem);
    if (pItem != nullptr)
        return ::OleIsRunning(pItem->GetObject()) ? S_OK : S_FALSE;

    if (impl__OnGetLinkedItem_COleLinkingDoc__MEAAPEAVCOleServerItem__PEB_W_Z(pThis, lpszItem) != nullptr)
        return S_OK;
    return 0x800401E5L;                                         // MK_E_NOOBJECT
}

// Symbol: ?ParseDisplayName@XOleItemContainer@COleLinkingDoc@@UEAAJPEAUIBindCtx@@PEA_WPEAKPEAPEAUIMoniker@@@Z
// XOleItemContainer::ParseDisplayName(LPBC pbc, LPOLESTR lpszDisplayName,
//                                     ULONG* pchEaten, LPMONIKER* ppmkOut) --
// retail 0x25da60:
//     if (pchEaten == NULL || ppmkOut == NULL) return E_POINTER;  // 0x25dc6b
//     *ppmkOut = NULL;
//     CString strTemp(lpszDisplayName);
//     // skip leading delimiters, counting them, then copy characters into a
//     // 0x100-byte stack buffer until the next delimiter, capping the running
//     // count at 0xff.  The delimiter test is the bitmask 0xc00000002004001
//     // biased by 0x21, i.e. '!', '/', ':', '[', '\\' (loops at 0x25dade and
//     // 0x25db26; the ANSI build also consumes _ismbblead pairs, which the
//     // Unicode build has no equivalent of).
//     *pchEaten = <count>;                                       // 0x25db6b
//     LPUNKNOWN lpUnk = NULL;
//     SCODE sc = this->GetObject(szItem, BINDSPEED_INDEFINITE, pbc,
//                                IID_IUnknown, (LPVOID*)&lpUnk);
//         // a virtual call on the sub-object itself: vftable byte 0x30, 0x25dbb0
//     if (sc != S_OK) return sc;                                 // 0x25dbe3
//     lpUnk->Release();                                          // 0x25dbf1
//     return ::CreateItemMoniker(_T("\\") /* .rdata 0x33d594 */,
//                                szItem, ppmkOut);               // 0x25dc13
extern "C" long MS_ABI impl__ParseDisplayName_XOleItemContainer_COleLinkingDoc__UEAAJPEAUIBindCtx__PEA_WPEAKPEAPEAUIMoniker___Z(
        void* pThisItf, IBindCtx* pbc, wchar_t* lpszDisplayName,
        unsigned long* pchEaten, IMoniker** ppmkOut) {
    if (pchEaten == nullptr || ppmkOut == nullptr) return E_POINTER;
    *ppmkOut = nullptr;

    wchar_t szItem[256];
    unsigned long nEaten = 0;
    const wchar_t* p = (lpszDisplayName != nullptr) ? lpszDisplayName : L"";
    while (*p != L'\0' && IsItemDelimiter(*p)) { ++p; ++nEaten; }

    int nItem = 0;
    while (*p != L'\0' && !IsItemDelimiter(*p) && nEaten < 0xff) {
        szItem[nItem++] = *p++;
        ++nEaten;
    }
    szItem[nItem] = L'\0';
    *pchEaten = nEaten;

    IUnknown* lpUnk = nullptr;
    long sc = impl__GetObjectW_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WKPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(
        pThisItf, szItem, 1 /*BINDSPEED_INDEFINITE*/, pbc, IID_IUnknown,
        reinterpret_cast<void**>(&lpUnk));
    if (sc != S_OK) return sc;
    if (lpUnk != nullptr) lpUnk->Release();
    return ::CreateItemMoniker(L"\\", szItem, ppmkOut);
}

// Symbol: ?GetObjectStorage@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_WPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
// XOleItemContainer::GetObjectStorage(LPOLESTR lpszItem, LPBINDCTX pbc,
//                                     REFIID riid, LPVOID* ppvStorage) --
// retail 0x25de80:
//     METHOD_PROLOGUE_EX_(COleLinkingDoc, OleItemContainer)   // lea -0x228
//     if (ppvStorage == NULL) return E_POINTER;               // 0x25deb1
//     *ppvStorage = NULL;
//     if (memcmp(&riid, &IID_IStorage /* .rdata 0x2d7a78 */, 16) != 0)
//         return E_UNEXPECTED /*0x8000ffff*/;                 // 0x25dedc
//     CString str(lpszItem);
//     COleClientItem* pItem = pThis->OnFindEmbeddedItem(str); // slot 0x298
//     if (pItem != NULL) {
//         LPSTORAGE lpStg = pItem->[item+0x68] /* m_lpStorage */;   // 0x25df40
//         if (lpStg != NULL) {
//             *ppvStorage = lpStg;
//             lpStg->AddRef();                                // 0x25df57
//             return S_OK;
//         }
//     }
//     return MK_E_NOSTORAGE /*0x800401ed*/;                   // 0x25df61
// Checked against RVA 0x25ef40 (mfc140u): same control flow, the memcmp import
// slot resolves to VCRUNTIME140!memcmp and the IID at 0x2d9b28 (mfc140u) is
// {0000000B-0000-0000-C000-000000000046}.
// DEVIATION, confined to the found-item leg: COleClientItem::m_lpStorage
// (retail item+0x68) is not declared by OpenMFC's COleClientItem -- the same
// gap that leaves ReadItemCompound/GetItemStorageFlat stubbed in
// core/ole/COleClientItem.cpp -- so a found item is answered with
// MK_E_NOSTORAGE, retail's answer for an item whose m_lpStorage is NULL.  That
// leg is unreachable today because OnFindEmbeddedItem is itself a stub that
// returns NULL; every other path (E_POINTER, E_UNEXPECTED for a non-IStorage
// riid, MK_E_NOSTORAGE for an unknown item) is retail's.  The earlier body
// returned S_OK with a NULL storage, which a caller would dereference.
extern "C" long MS_ABI impl__GetObjectStorage_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(
        void* pThisItf, wchar_t* lpszItem, IBindCtx* pbc, const GUID& riid, void** ppvStorage) {
    (void)pbc;
    if (ppvStorage == nullptr) return E_POINTER;
    *ppvStorage = nullptr;
    if (::memcmp(&riid, &kIID_IStorage, sizeof(GUID)) != 0)
        return 0x8000FFFFL;                                     // E_UNEXPECTED

    COleLinkingDoc* pThis = DocFromItemContainer(pThisItf);
    COleClientItem* pItem =
        impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(pThis, lpszItem);
    (void)pItem;   // no m_lpStorage to hand back, see above
    return 0x800401EDL;                                         // MK_E_NOSTORAGE
}
