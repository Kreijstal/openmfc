// CDocument — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

//=============================================================================
// Sibling entry points the nested-interface bodies at the end of this file
// call.  Each exists only as an extern "C" impl__ thunk; the C++ method would
// compile and then fail at link.
//=============================================================================
// core/doc/Thunks.cpp -- the exported non-virtual ?UpdateAllViews@CDocument@@QEAAXPEAVCView@@_JPEAVCObject@@@Z
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__QEAAXPEAVCView___JPEAVCObject___Z(
    CDocument* pThis, CView* pSender, __int64 lHint, CObject* pHint);
// core/window/CWnd.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
// core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
// featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxOleLockApp__YAXXZ();

namespace {

//-----------------------------------------------------------------------------
// Retail CDocument members OpenMFC's declaration does not have.
//
// Retail (atlmfc/include/afxwin.h:5773, x64 _AFXDLL) lays CDocument out as
// CCmdTarget (0x40 bytes) followed by m_strTitle +0x40 ... m_pPreviewHandlerSite
// +0x178, sizeof 0x180.  The offsets below were read out of the retail
// constructor ??0CDocument@@QEAA@XZ (RVA 0x21a9c0 (mfc140u)), which stores
// each named field below, and they agree with the this-adjustments the nested
// interface methods open with (lea -0x158(%rcx) for XPreviewHandler, -0x160
// XPreviewHandlerVisuals, -0x168 XObjectWithSite; -0x118/-0x138 relative
// module-state loads for XInitializeWithStream/XOleWindow, i.e. parts at
// +0x150 and +0x170).
//
// OpenMFC's CDocument (include/openmfc/afxwin.h) is also 0x180 bytes, but its
// CCmdTarget is only 0x20 bytes and its own members end at +0x50
// (m_pNextDoc +0x48); everything from +0x50 to the end is the declared-but-
// unused _document_padding[304].  The retail tail from +0x90 is therefore laid
// over that padding here, at the RETAIL offsets.  Nothing else in the tree
// reads or writes these bytes (checked by grep for overlays of CDocument).
// Retail members that OpenMFC DOES declare under its own layout are used
// through OpenMFC's member instead: m_bAutoDelete (retail +0x120, OpenMFC
// +0x34).  Retail's m_lstChunks/m_posReadChunk (+0xd8..+0x118) are kept by
// this file in g_documentExtraStates and are not modelled here.
//-----------------------------------------------------------------------------
struct S_CDocumentRetailTail {             // mapped at doc+0x90
    BOOL     unused_bModified;             // +0x90 retail m_bModified -- OpenMFC's is +0x30; never touched here
    BOOL     bFinalRelease;                // +0x94 retail m_bFinalRelease
    BOOL     bOLELocked;                   // +0x98 retail m_bOLELocked
    HWND     hWndHost;                     // +0xa0 retail m_hWndHost
    RECT     rectHost;                     // +0xa8 retail m_rectHost
    IStream* pStream;                      // +0xb8 retail m_pStream
    DWORD    grfMode;                      // +0xc0 retail m_grfMode
    CWnd*    pPreviewFrame;                // +0xc8 retail m_pPreviewFrame (declared CFrameWnd*)
    IUnknown* pUnkSite;                    // +0xd0 retail m_spUnkSite (CComPtr<IUnknown>, one pointer)
    BYTE     notModelled_chunks[0x48];     // +0xd8 retail m_lstChunks/m_posReadChunk/m_pDocumentAdapter
    BOOL     unused_bAutoDelete;           // +0x120 retail m_bAutoDelete -- OpenMFC's is +0x34; never touched here
    BOOL     bEmbedded;                    // +0x124 retail m_bEmbedded
    BOOL     bGetThumbnailMode;            // +0x128 retail m_bGetThumbnailMode
    BOOL     bPreviewHandlerMode;          // +0x12c retail m_bPreviewHandlerMode
    BOOL     bSearchMode;                  // +0x130 retail m_bSearchMode
    COLORREF clrRichPreviewBackColor;      // +0x134
    COLORREF clrRichPreviewTextColor;      // +0x138
    DWORD    pad13c;                       // +0x13c
    void*    notModelled_fontVftable;      // +0x140 retail m_lfRichPreviewFont's CFont vftable -- no CFont is constructed here
    HFONT    hRichPreviewFont;             // +0x148 retail m_lfRichPreviewFont.m_hObject
    void*    notModelled_parts[5];         // +0x150..+0x178 the five nested interface parts -- never written here
    IUnknown* pPreviewHandlerSite;         // +0x178 retail m_pPreviewHandlerSite (IPreviewHandlerFrame*)
};
const size_t kRetailTailOffset = 0x90;
static_assert(sizeof(CDocument) == 0x180, "OpenMFC CDocument must stay retail-sized (0x180)");
static_assert(offsetof(CDocument, m_pNextDoc) == 0x48,
              "OpenMFC's own CDocument members must end at +0x50, where _document_padding starts");
static_assert(sizeof(S_CDocumentRetailTail) == 0x180 - kRetailTailOffset, "overlay must end exactly at sizeof(CDocument)");
static_assert(offsetof(S_CDocumentRetailTail, bOLELocked) + kRetailTailOffset == 0x98, "m_bOLELocked");
static_assert(offsetof(S_CDocumentRetailTail, hWndHost) + kRetailTailOffset == 0xa0, "m_hWndHost");
static_assert(offsetof(S_CDocumentRetailTail, rectHost) + kRetailTailOffset == 0xa8, "m_rectHost");
static_assert(offsetof(S_CDocumentRetailTail, pStream) + kRetailTailOffset == 0xb8, "m_pStream");
static_assert(offsetof(S_CDocumentRetailTail, grfMode) + kRetailTailOffset == 0xc0, "m_grfMode");
static_assert(offsetof(S_CDocumentRetailTail, pPreviewFrame) + kRetailTailOffset == 0xc8, "m_pPreviewFrame");
static_assert(offsetof(S_CDocumentRetailTail, pUnkSite) + kRetailTailOffset == 0xd0, "m_spUnkSite");
static_assert(offsetof(S_CDocumentRetailTail, bEmbedded) + kRetailTailOffset == 0x124, "m_bEmbedded");
static_assert(offsetof(S_CDocumentRetailTail, bPreviewHandlerMode) + kRetailTailOffset == 0x12c, "m_bPreviewHandlerMode");
static_assert(offsetof(S_CDocumentRetailTail, clrRichPreviewBackColor) + kRetailTailOffset == 0x134, "m_clrRichPreviewBackColor");
static_assert(offsetof(S_CDocumentRetailTail, clrRichPreviewTextColor) + kRetailTailOffset == 0x138, "m_clrRichPreviewTextColor");
static_assert(offsetof(S_CDocumentRetailTail, hRichPreviewFont) + kRetailTailOffset == 0x148, "m_lfRichPreviewFont.m_hObject");
static_assert(offsetof(S_CDocumentRetailTail, pPreviewHandlerSite) + kRetailTailOffset == 0x178, "m_pPreviewHandlerSite");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail SetFocus reads m_pPreviewFrame->m_hWnd at +0x40");

S_CDocumentRetailTail* Tail(CDocument* p) {
    return reinterpret_cast<S_CDocumentRetailTail*>(reinterpret_cast<char*>(p) + kRetailTailOffset);
}

// The nested interface parts, at their retail offsets.  OpenMFC neither
// declares nor constructs them (CDocument's interface map here is
// g_docviewEmptyInterfaceMap and the constructor parks no part vftable), so
// nothing in a running OpenMFC process can hand out one of these interface
// pointers today: the fifteen X* entry points at the end of this file are
// written against the retail offsets so they are correct the moment the parts
// are added.  (headerRequest filed for the declaration.)
const ptrdiff_t kXInitializeWithStreamOffset = 0x150;
const ptrdiff_t kXPreviewHandlerOffset       = 0x158;
const ptrdiff_t kXPreviewHandlerVisualsOffset = 0x160;
const ptrdiff_t kXObjectWithSiteOffset       = 0x168;
const ptrdiff_t kXOleWindowOffset            = 0x170;

CDocument* DocFromPart(void* pThisItf, ptrdiff_t partOffset) {
    return reinterpret_cast<CDocument*>(reinterpret_cast<char*>(pThisItf) - partOffset);
}

// IID_IPreviewHandlerFrame {fec87aaf-35f9-447a-adb7-20234491401a}: the 16 bytes
// XObjectWithSite::SetSite passes to QueryInterface (.rdata RVA 0x34ca50
// (mfc140u)).  Spelled out locally rather than pulled from libuuid.
const IID kIID_IPreviewHandlerFrame =
    { 0xfec87aaf, 0x35f9, 0x447a, { 0xad, 0xb7, 0x20, 0x23, 0x44, 0x91, 0x40, 0x1a } };

} // namespace

// Default Constructor
// Symbol: ??0CDocument@@QEAA@XZ
// The last block initialises the S_CDocumentRetailTail overlay with exactly the
// values the retail constructor (RVA 0x21a9c0 (mfc140u)) stores at those
// offsets: 8-byte zero at +0x94 (m_bFinalRelease, m_bOLELocked), zero
// m_hWndHost/m_pStream/m_pPreviewFrame/m_spUnkSite/m_grfMode,
// ::SetRectEmpty(&m_rectHost), 8-byte zero at +0x128 (m_bGetThumbnailMode,
// m_bPreviewHandlerMode), zero +0x124 (high half of the `movq $1,0x120`) and
// +0x130, m_clrRichPreviewBackColor = 0x00ffffff with the text colour zeroed by
// the same 8-byte store, CFont m_hObject = NULL and m_pPreviewHandlerSite = NULL.
// Only the fields assigned below are written.  unused_bModified (+0x90) and
// unused_bAutoDelete (+0x120), which retail also stores (0 and 1), plus pad13c,
// the notModelled_* ranges and _document_padding below +0x90, are left as they
// were.
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Initialize members
    pThis->m_bModified = FALSE;
    pThis->m_bAutoDelete = TRUE;
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
    pThis->m_pNextDoc = nullptr;

    S_CDocumentRetailTail* t = Tail(pThis);
    t->bFinalRelease = FALSE;
    t->bOLELocked = FALSE;
    t->hWndHost = nullptr;
    ::SetRectEmpty(&t->rectHost);
    t->pStream = nullptr;
    t->grfMode = 0;
    t->pPreviewFrame = nullptr;
    t->pUnkSite = nullptr;
    t->bEmbedded = FALSE;
    t->bGetThumbnailMode = FALSE;
    t->bPreviewHandlerMode = FALSE;
    t->bSearchMode = FALSE;
    t->clrRichPreviewBackColor = 0x00ffffff;
    t->clrRichPreviewTextColor = 0;
    t->hRichPreviewFont = nullptr;
    t->pPreviewHandlerSite = nullptr;
}
// Destructor
// Symbol: ??1CDocument@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Disconnect all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
    g_documentExtraStates.erase(pThis);
}
// OnNewDocument
// Symbol: ?OnNewDocument@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    pThis->DeleteContents();
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnOpenDocument
// Symbol: ?OnOpenDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    if (!pThis || !lpszPathName || !lpszPathName[0]) return FALSE;
    pThis->DeleteContents();
    CFile file;
    CFileException fileException;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(&file, lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fileException)) {
        return FALSE;
    }
    CArchive archive(&file, CArchive::load);
    pThis->Serialize(archive);
    archive.Close();
    file.Close();
    pThis->SetPathName(lpszPathName, TRUE);
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnSaveDocument
// Symbol: ?OnSaveDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    if (!pThis || !lpszPathName || !lpszPathName[0]) return FALSE;
    CFile file;
    CFileException fileException;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(&file, lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &fileException)) {
        return FALSE;
    }
    CArchive archive(&file, CArchive::store);
    pThis->Serialize(archive);
    archive.Close();
    file.Close();
    pThis->SetPathName(lpszPathName, TRUE);
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}
// OnCloseDocument
// Symbol: ?OnCloseDocument@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis) {
    if (!pThis) return;

    // Destroy all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        if (pView->m_hWnd) {
            ::DestroyWindow(pView->m_hWnd);
        }
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;

    // Auto-delete if enabled
    if (pThis->m_bAutoDelete) {
        delete pThis;
    }
}
// DeleteContents
// Symbol: ?DeleteContents@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis) {
    if (pThis) g_documentExtraStates[pThis].chunks.clear();
}
// SetTitle
// Symbol: ?SetTitle@CDocument@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszTitle)
{
    if (pThis) {
        pThis->m_strTitle = lpszTitle ? lpszTitle : L"";
    }
}
// SetPathName
// Symbol: ?SetPathName@CDocument@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_CDocument__UEAAXPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU)
{
    (void)bAddToMRU;
    if (pThis) {
        pThis->m_strPathName = lpszPathName ? lpszPathName : L"";
        if (!pThis->m_strPathName.IsEmpty()) {
            const wchar_t* path = pThis->m_strPathName;
            const wchar_t* slash = wcsrchr(path, L'\\');
            const wchar_t* fslash = wcsrchr(path, L'/');
            const wchar_t* name = slash > fslash ? slash : fslash;
            pThis->m_strTitle = name ? name + 1 : path;
        }
    }
}
// SaveModified
// Symbol: ?SaveModified@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis || !pThis->IsModified()) return TRUE;
    const wchar_t* path = pThis->GetPathName();
    return path && *path ? pThis->DoFileSave() : TRUE;
}
// DoSave
// Symbol: ?DoSave@CDocument@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DoSave_CDocument__UEAAHPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bReplace)
{
    (void)bReplace;
    if (!pThis) return FALSE;
    return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(pThis, lpszPathName);
}
// DoFileSave
// Symbol: ?DoFileSave@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    const wchar_t* path = impl__GetPathName_CDocument__UEBAPEB_WXZ(pThis);
    return impl__DoSave_CDocument__UEAAHPEB_WH_Z(pThis, path, TRUE);
}
// Symbol: ?GetRuntimeClass@CDocument@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDocument__UEBAPEAUCRuntimeClass__XZ(const CDocument*) { return &CDocument::classCDocument; }
// Symbol: ?GetThisClass@CDocument@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocument__SAPEAUCRuntimeClass__XZ() { return &CDocument::classCDocument; }
// Symbol: ?GetMessageMap@CDocument@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDocument__MEBAPEBUAFX_MSGMAP__XZ(const CDocument*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CDocument@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDocument__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// Symbol: ?GetInterfaceMap@CDocument@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CDocument__MEBAPEBUAFX_INTERFACEMAP__XZ(const CDocument*) { return g_docviewEmptyInterfaceMap; }
// Symbol: ?GetThisInterfaceMap@CDocument@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CDocument__KAPEBUAFX_INTERFACEMAP__XZ() { return g_docviewEmptyInterfaceMap; }
// Symbol: ?GetFile@CDocument@@UEAAPEAVCFile@@PEB_WIPEAVCFileException@@@Z
extern "C" CFile* MS_ABI impl__GetFile_CDocument__UEAAPEAVCFile__PEB_WIPEAVCFileException___Z(CDocument*, const wchar_t* path, unsigned int openFlags, CFileException* exception) {
    CFile* file = new (std::nothrow) CFile();
    if (!file) return nullptr;
    if (!impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(file, path, openFlags, exception)) {
        delete file;
        return nullptr;
    }
    return file;
}
// Symbol: ?ReleaseFile@CDocument@@UEAAXPEAVCFile@@H@Z
extern "C" void MS_ABI impl__ReleaseFile_CDocument__UEAAXPEAVCFile__H_Z(CDocument*, CFile* file, int abort) {
    if (!file) return;
    if (!abort) file->Close();
    delete file;
}
// Symbol: ?ClearPathName@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__ClearPathName_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->m_strPathName.Empty(); }
// Symbol: ?CanCloseFrame@CDocument@@UEAAHPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__CanCloseFrame_CDocument__UEAAHPEAVCFrameWnd___Z(CDocument* pThis, CFrameWnd*) { return pThis ? pThis->SaveModified() : TRUE; }
// Symbol: ?PreCloseFrame@CDocument@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_CDocument__UEAAXPEAVCFrameWnd___Z(CDocument* pThis, CFrameWnd*) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?DisconnectViews@CDocument@@QEAAXXZ
extern "C" void MS_ABI impl__DisconnectViews_CDocument__QEAAXXZ(CDocument* pThis) {
    if (!pThis) return;
    for (CView* view = pThis->m_pFirstView; view; ) {
        CView* next = view->m_pNextView;
        view->m_pDocument = nullptr;
        view->m_pNextView = nullptr;
        view = next;
    }
    pThis->m_pFirstView = pThis->m_pLastView = nullptr;
}
// Symbol: ?SendInitialUpdate@CDocument@@QEAAXXZ
extern "C" void MS_ABI impl__SendInitialUpdate_CDocument__QEAAXXZ(CDocument* pThis) {
    if (!pThis) return;
    for (CView* view = pThis->m_pFirstView; view; view = view->m_pNextView) view->OnInitialUpdate();
}
// Symbol: ?UpdateFrameCounts@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateFrameCounts_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?OnChangedViewList@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangedViewList_CDocument__UEAAXXZ(CDocument* pThis) { impl__UpdateFrameCounts_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnCmdMsg@CDocument@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDocument* pThis, unsigned int id, int code, void* extra, AFX_CMDHANDLERINFO* info) { return pThis ? pThis->CCmdTarget::OnCmdMsg(id, code, extra, info) : FALSE; }
// Symbol: ?OnFileSave@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSave_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis) pThis->DoFileSave(); }
// Symbol: ?OnFileSaveAs@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSaveAs_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis) pThis->DoSave(pThis->GetPathName(), TRUE); }
// Symbol: ?OnFileClose@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileClose_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis && pThis->SaveModified()) pThis->OnCloseDocument(); }
// Symbol: ?ReportSaveLoadException@CDocument@@UEAAXPEB_WPEAVCException@@HI@Z
extern "C" void MS_ABI impl__ReportSaveLoadException_CDocument__UEAAXPEB_WPEAVCException__HI_Z(CDocument*, const wchar_t*, CException* ex, int, unsigned int) { if (ex) impl__ReportError_CException__UEAAHII_Z(ex, MB_OK | MB_ICONEXCLAMATION, 0); }
// Chunk/read-preview state.
// Symbol: ?ClearChunkList@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__ClearChunkList_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) g_documentExtraStates[pThis].chunks.clear(); }
// Symbol: ?BeginReadChunks@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__BeginReadChunks_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) g_documentExtraStates[pThis].chunkReadIndex = 0; }
// Symbol: ?SetChunkValue@CDocument@@UEAAHPEAUIFilterChunkValue@ATL@@@Z
extern "C" int MS_ABI impl__SetChunkValue_CDocument__UEAAHPEAUIFilterChunkValue_ATL___Z(CDocument* pThis, void* value) {
    if (!pThis || !value) return FALSE;
    DocChunkState chunk;
    chunk.id = static_cast<DWORD>(g_documentExtraStates[pThis].chunks.size());
    chunk.value = value;
    g_documentExtraStates[pThis].chunks.push_back(chunk);
    return TRUE;
}
// Symbol: ?ReadNextChunkValue@CDocument@@UEAAHPEAPEAUIFilterChunkValue@ATL@@@Z
extern "C" int MS_ABI impl__ReadNextChunkValue_CDocument__UEAAHPEAPEAUIFilterChunkValue_ATL___Z(CDocument* pThis, void** value) {
    if (value) *value = nullptr;
    if (!pThis || !value) return FALSE;
    auto& state = g_documentExtraStates[pThis];
    if (state.chunkReadIndex >= state.chunks.size()) return FALSE;
    *value = state.chunks[state.chunkReadIndex++].value;
    return TRUE;
}
// Symbol: ?FindChunk@CDocument@@UEAAPEAU__POSITION@@AEBU_GUID@@K@Z
extern "C" void* MS_ABI impl__FindChunk_CDocument__UEAAPEAU__POSITION__AEBU_GUID__K_Z(CDocument* pThis, const GUID* guid, unsigned long id) {
    if (!pThis || !guid) return nullptr;
    auto& chunks = g_documentExtraStates[pThis].chunks;
    for (size_t i = 0; i < chunks.size(); ++i) if (chunks[i].id == id && IsEqualGUID(chunks[i].guid, *guid)) return reinterpret_cast<void*>(i + 1);
    return nullptr;
}
// Symbol: ?RemoveChunk@CDocument@@UEAAXAEBU_GUID@@K@Z
extern "C" void MS_ABI impl__RemoveChunk_CDocument__UEAAXAEBU_GUID__K_Z(CDocument* pThis, const GUID* guid, unsigned long id) {
    if (!pThis || !guid) return;
    auto& chunks = g_documentExtraStates[pThis].chunks;
    chunks.erase(std::remove_if(chunks.begin(), chunks.end(), [&](const DocChunkState& c){ return c.id == id && IsEqualGUID(c.guid, *guid); }), chunks.end());
}
// Symbol: ?LoadDocumentFromStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__LoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(CDocument* pThis, void* stream, unsigned long) {
    if (!pThis || !stream) return E_POINTER;
    IStream* pStream = static_cast<IStream*>(stream);
    CMemFile memFile;
    BYTE buffer[4096];
    for (;;) {
        ULONG read = 0;
        HRESULT hr = pStream->Read(buffer, sizeof(buffer), &read);
        if (FAILED(hr)) return hr;
        if (read == 0) break;
        memFile.Write(buffer, read);
    }
    memFile.Seek(0, CFile::begin);
    CArchive archive(&memFile, CArchive::load);
    pThis->DeleteContents();
    pThis->Serialize(archive);
    archive.Close();
    pThis->SetModifiedFlag(FALSE);
    return S_OK;
}
// Symbol: ?OnLoadDocumentFromStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__OnLoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(CDocument* pThis, void* stream, unsigned long mode) { return impl__LoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(pThis, stream, mode); }
// Symbol: ?GetThumbnail@CDocument@@UEAAHIPEAPEAUHBITMAP__@@PEAW4WTS_ALPHATYPE@@@Z
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect);
// Symbol: ?OnDrawThumbnail@CDocument@@UEAAXAEAVCDC@@PEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnDrawThumbnail_CDocument__UEAAXAEAVCDC__PEAUtagRECT___Z(CDocument*, CDC* dc, RECT* rect) { if (dc && rect && dc->m_hDC) ::DrawTextW(dc->m_hDC, L"OpenMFC", -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); }
// Symbol: ?OnPreviewHandlerQueryFocus@CDocument@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerQueryFocus_CDocument__UEAAJPEAPEAUHWND_____Z(CDocument* pThis, HWND* hwnd) { if (hwnd) *hwnd = pThis && pThis->m_pFirstView ? pThis->m_pFirstView->GetSafeHwnd() : nullptr; return S_OK; }
// Symbol: ?OnPreviewHandlerTranslateAccelerator@CDocument@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnPreviewHandlerTranslateAccelerator_CDocument__UEAAJPEAUtagMSG___Z(CDocument* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) {
        return S_FALSE;
    }

    void* pos = pThis->GetFirstViewPosition();
    while (pos) {
        CView* pView = pThis->GetNextView(pos);
        if (pView && pView->m_hWnd && ::IsWindow(pView->m_hWnd) && ::IsDialogMessageW(pView->m_hWnd, pMsg)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
// Symbol: ?IsSearchAndOrganizeHandler@CDocument@@QEBAHXZ
extern "C" int MS_ABI impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(const CDocument* pThis) { auto it = g_documentExtraStates.find(pThis); return it != g_documentExtraStates.end() && it->second.searchHandler; }
// Symbol: ?GetDefaultMenu@CDocument@@UEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetDefaultMenu_CDocument__UEAAPEAUHMENU____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultMenu : nullptr; }
// Symbol: ?GetDefaultAccelerator@CDocument@@UEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_CDocument__UEAAPEAUHACCEL____XZ(CDocument* pThis) { return pThis ? g_documentExtraStates[pThis].defaultAccel : nullptr; }
// Symbol: ?OnCreatePreviewFrame@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnCreatePreviewFrame_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) {
        return FALSE;
    }
    return pThis->m_pFirstView != nullptr ? TRUE : FALSE;
}
// Symbol: ?OnDocumentEvent@CDocument@@UEAAXW4DocumentEvent@1@@Z
extern "C" void MS_ABI impl__OnDocumentEvent_CDocument__UEAAXW4DocumentEvent_1__Z(CDocument* pThis, int eventId) { if (pThis) pThis->UpdateAllViews(nullptr, static_cast<LPARAM>(eventId), nullptr); }
// Symbol: ?OnFinalRelease@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis && pThis->m_bAutoDelete) delete pThis; }
// Symbol: ?OnIdle@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnIdle_CDocument__UEAAXXZ(CDocument* pThis) { if (pThis) pThis->UpdateAllViews(nullptr, 0, nullptr); }
// Symbol: ?OnRichPreviewUnload@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnRichPreviewUnload_CDocument__UEAAXXZ(CDocument* pThis) { impl__ClearChunkList_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnUnloadHandler@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnUnloadHandler_CDocument__UEAAXXZ(CDocument* pThis) { impl__OnRichPreviewUnload_CDocument__UEAAXXZ(pThis); }
// Symbol: ?OnFileSendMail@CDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSendMail_CDocument__IEAAXXZ(CDocument* pThis) { if (pThis && pThis->GetPathName()[0]) ::ShellExecuteW(nullptr, L"open", pThis->GetPathName(), nullptr, nullptr, SW_SHOWNORMAL); }
// Symbol: ?OnUpdateFileSendMail@CDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileSendMail_CDocument__IEAAXPEAVCCmdUI___Z(CDocument* pThis, CCmdUI* ui) { if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, pThis && pThis->GetPathName()[0]); }
CDocument::CDocument() { impl___0CDocument__QEAA_XZ(this); }
CDocument::~CDocument() { impl___1CDocument__UEAA_XZ(this); }
int CDocument::OnNewDocument() { return impl__OnNewDocument_CDocument__UEAAHXZ(this); }
int CDocument::OnOpenDocument(const wchar_t* lpszPathName) { return impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
int CDocument::OnSaveDocument(const wchar_t* lpszPathName) { return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
void CDocument::OnCloseDocument() { impl__OnCloseDocument_CDocument__UEAAXXZ(this); }
void CDocument::DeleteContents() { impl__DeleteContents_CDocument__UEAAXXZ(this); }
void CDocument::SetModifiedFlag(int bModified) { impl__SetModifiedFlag_CDocument__UEAAXH_Z(this, bModified); }
int CDocument::IsModified() const { return impl__IsModified_CDocument__UEBAHXZ(this); }
void CDocument::SetTitle(const wchar_t* lpszTitle) { impl__SetTitle_CDocument__UEAAXPEB_W_Z(this, lpszTitle); }
const wchar_t* CDocument::GetTitle() const { return impl__GetTitle_CDocument__UEBAPEB_WXZ(this); }
void CDocument::SetPathName(const wchar_t* lpszPathName, int bAddToMRU) { impl__SetPathName_CDocument__UEAAXPEB_WH_Z(this, lpszPathName, bAddToMRU); }
const wchar_t* CDocument::GetPathName() const { return impl__GetPathName_CDocument__UEBAPEB_WXZ(this); }
void CDocument::AddView(CView* pView) { impl__AddView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void CDocument::RemoveView(CView* pView) { impl__RemoveView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void* CDocument::GetFirstViewPosition() const { return impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(this); }
CView* CDocument::GetNextView(void*& pos) const { return impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(this, pos); }
void CDocument::UpdateAllViews(CView* pSender, unsigned long lHint, CObject* pHint) { impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(this, pSender, lHint, pHint); }
int CDocument::SaveModified() { return impl__SaveModified_CDocument__UEAAHXZ(this); }
int CDocument::DoSave(const wchar_t* lpszPathName, int bReplace) { return impl__DoSave_CDocument__UEAAHPEB_WH_Z(this, lpszPathName, bReplace); }
int CDocument::DoFileSave() { return impl__DoFileSave_CDocument__UEAAHXZ(this); }
void CDocument::Serialize(CArchive& ar) { impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(this, &ar); }

//=============================================================================
// CDocument's nested interface parts (rich-preview / search handler support).
//
// Every method below is transcribed from the retail body in mfc140u.dll.  Only
// XInitializeWithStream::Initialize, XPreviewHandler::DoPreview,
// XObjectWithSite::GetSite and the two XOleWindow methods resolve by name in
// mfc140u_rva_symbols.json; the others were located through the part
// vftables the retail constructor (RVA 0x21a9c0 (mfc140u)) parks at +0x150..
// +0x170 (vftables at 0x180328ec8/0x180328e70/0x180328e38/0x180328e08/
// 0x180328dd8 (mfc140u)), in IUnknown-then-interface-declaration slot order.
//
// Every retail body opens with METHOD_PROLOGUE_EX_(CDocument, <Part>): the
// part-to-document this-adjustment plus an AFX_MAINTAIN_STATE2 built from
// CCmdTarget::m_pModuleState (doc+0x38) and popped on exit.  The module-state
// push is NOT reproduced: OpenMFC's CCmdTarget is 32 bytes and has no stored
// per-object module state to push.
//
// Retail virtual calls go through CDocument's vftable; the byte offsets named
// below were identified from a CDocument-derived vftable in the ANSI twin
// (RVA 0x2efe28 (mfc140), whose slot 0 resolves to COleServerDocEx and whose
// slots 0x208/0x210/0x218 are CDocument's own bodies): 0x100 OnNewDocument,
// 0x160 OnRichPreviewUnload, 0x168 LoadDocumentFromStream,
// 0x208 OnPreviewHandlerQueryFocus, 0x210 OnPreviewHandlerTranslateAccelerator,
// 0x218 OnCreatePreviewFrame.  Slots 0x138..0x158 all point at one shared bare
// `ret` (0x2820 (mfc140)); by declaration order between ReleaseFile (0x130)
// and OnRichPreviewUnload (0x160) they are OnBeforeRichPreviewFontChanged,
// OnRichPreviewFontChanged, OnRichPreviewTextColorChanged,
// OnRichPreviewBackColorChanged and OnRichPreviewSiteChanged -- the empty
// inline notifications of afxwin.h, which are not exported.
//
// DEVIATION, stated once for the whole block: OpenMFC's CDocument declares
// OnNewDocument as a virtual, so that call is a C++ virtual call -- but through
// OpenMFC's OWN g++ vtable layout, where OnNewDocument sits at byte offset 0x40
// (measured: &CDocument::OnNewDocument encodes 0x41 under the Itanium ABI), not
// at retail's 0x100.  It is correct for a document whose vptr is an OpenMFC-
// compiled vtable; for an object carrying a retail-layout MSVC vtable it would
// reach a different slot.  OpenMFC declares none of OnCreatePreviewFrame,
// LoadDocumentFromStream, OnRichPreviewUnload, OnPreviewHandlerQueryFocus or
// OnPreviewHandlerTranslateAccelerator, so those sites call this file's
// CDocument base thunk directly and a derived override is NOT picked up.  Note
// that several of those OpenMFC base thunks do not match their retail bodies
// (see the notes on Unload and QueryFocus).  The five empty notifications are
// omitted: with no OpenMFC declaration no override can exist, and the base
// body is a no-op.
//
// All fifteen entry points are also present in mfc140u.dll's own export table
// (mfc_complete_ordinal_mapping.json ordinals), at exactly the RVAs the part
// vftable slots give; they are merely missing from mfc140u_rva_symbols.json.
//=============================================================================

// Symbol: ?ContextSensitiveHelp@XOleWindow@CDocument@@UEAAJH@Z
// XOleWindow::ContextSensitiveHelp(BOOL) -- RVA 0x21dcb0 (mfc140u): only the
// module-state push/pop, then `mov $0x80004001,%eax`: always E_NOTIMPL, the
// argument is never read.
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleWindow_CDocument__UEAAJH_Z(void* pThisItf, int fEnterMode) {
    (void)pThisItf; (void)fEnterMode;
    return E_NOTIMPL;
}

// Symbol: ?DoPreview@XPreviewHandler@CDocument@@UEAAJXZ
// XPreviewHandler::DoPreview() -- RVA 0x21d330 (mfc140u):
//     if (pThis->m_pStream == NULL) return E_FAIL;           // cmpq $0,0xb8
//     pThis->m_bEmbedded = TRUE;                              // movl $1,0x124
//     pThis->OnNewDocument();                  // vftable 0x100, result ignored
//     if (!pThis->OnCreatePreviewFrame())      // vftable 0x218
//         return E_FAIL;
//     HRESULT hr = pThis->LoadDocumentFromStream(pThis->m_pStream,
//                                                pThis->m_grfMode); // 0x168
//     pThis->m_pStream->Release(); pThis->m_pStream = NULL;
//     if (hr != S_OK) return hr;               // `test %esi,%esi`: any nonzero,
//                                              // S_FALSE included
//     pThis->m_pPreviewFrame->SetWindowPos(NULL, m_rectHost.left,
//         m_rectHost.top, width, height, SWP_NOZORDER | SWP_NOACTIVATE); // 0x14
//     pThis->m_pPreviewFrame->ShowWindow(SW_SHOW);            // 5
//     pThis->SendInitialUpdate();
//     return S_OK;
// The three direct calls are CWnd::SetWindowPos, CWnd::ShowWindow and
// CDocument::SendInitialUpdate (their ANSI-twin targets 0x2a7970, 0x2a79e0 and
// 0x21acc0 (mfc140) resolve to those names).
// NOT reproduced: retail wraps the LoadDocumentFromStream call in
// TRY/CATCH_ALL.  The FuncInfo4 of 0x21d330 (handler data at RVA 0x391cb0
// (mfc140u)) names one catch of type .PEAVCException@@ whose funclet is at RVA
// 0x2c1c2f (mfc140u): it calls CException::Delete (0x227370 (mfc140u)) on the
// caught object and stores E_FAIL (0x80004005) into the hr home slot; the
// continuation at 0x21d3dd (mfc140u), which no branch in the body targets,
// reloads hr from that slot and joins the Release.  So retail is
//     TRY { hr = LoadDocumentFromStream(...); }
//     CATCH_ALL(e) { e->Delete(); hr = E_FAIL; } END_CATCH_ALL
// This DLL raises MFC exceptions through _CxxThrowException, which a gcc
// catch cannot see, so here an exception propagates out of the method and
// m_pStream is neither released nor cleared.
// DEVIATION: retail dereferences m_pPreviewFrame unchecked.  OpenMFC's
// OnCreatePreviewFrame never sets it (it only reports whether a view exists),
// so a NULL frame skips the SetWindowPos/ShowWindow pair here instead of
// faulting.
extern "C" long MS_ABI impl__DoPreview_XPreviewHandler_CDocument__UEAAJXZ(void* pThisItf) {
    CDocument* pThis = DocFromPart(pThisItf, kXPreviewHandlerOffset);
    S_CDocumentRetailTail* t = Tail(pThis);
    if (t->pStream == nullptr) return E_FAIL;
    t->bEmbedded = TRUE;
    pThis->OnNewDocument();
    if (!impl__OnCreatePreviewFrame_CDocument__UEAAHXZ(pThis)) return E_FAIL;

    const long hr = impl__LoadDocumentFromStream_CDocument__UEAAJPEAUIStream__K_Z(pThis, t->pStream, t->grfMode);
    t->pStream->Release();
    t->pStream = nullptr;
    if (hr != S_OK) return hr;

    if (t->pPreviewFrame != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(t->pPreviewFrame, nullptr,
            t->rectHost.left, t->rectHost.top,
            t->rectHost.right - t->rectHost.left, t->rectHost.bottom - t->rectHost.top,
            SWP_NOZORDER | SWP_NOACTIVATE);
        impl__ShowWindow_CWnd__QEAAHH_Z(t->pPreviewFrame, SW_SHOW);
    }
    impl__SendInitialUpdate_CDocument__QEAAXXZ(pThis);
    return S_OK;
}

// Symbol: ?GetSite@XObjectWithSite@CDocument@@UEAAJAEBU_GUID@@PEAPEAX@Z
// XObjectWithSite::GetSite(REFIID riid, void** ppvSite) -- RVA 0x21daa0 (mfc140u):
//     if (ppvSite == NULL) return E_POINTER;
//     if (pThis->m_spUnkSite != NULL)                     // -0x98 == doc+0xd0
//         return pThis->m_spUnkSite->QueryInterface(riid, ppvSite);
//     *ppvSite = NULL;
//     return E_FAIL;
extern "C" long MS_ABI impl__GetSite_XObjectWithSite_CDocument__UEAAJAEBU_GUID__PEAPEAX_Z(
        void* pThisItf, const GUID* riid, void** ppvSite) {
    if (ppvSite == nullptr) return E_POINTER;
    S_CDocumentRetailTail* t = Tail(DocFromPart(pThisItf, kXObjectWithSiteOffset));
    if (t->pUnkSite != nullptr) return t->pUnkSite->QueryInterface(*riid, ppvSite);
    *ppvSite = nullptr;
    return E_FAIL;
}

// Symbol: ?GetWindow@XOleWindow@CDocument@@UEAAJPEAPEAUHWND__@@@Z
// XOleWindow::GetWindow(HWND* phwnd) -- RVA 0x21dc50 (mfc140u):
//     if (phwnd == NULL) return E_INVALIDARG;
//     *phwnd = pThis->m_hWndHost;                         // -0xd0 == doc+0xa0
//     return S_OK;
extern "C" long MS_ABI impl__GetWindow_XOleWindow_CDocument__UEAAJPEAPEAUHWND_____Z(void* pThisItf, HWND* phwnd) {
    if (phwnd == nullptr) return E_INVALIDARG;
    *phwnd = Tail(DocFromPart(pThisItf, kXOleWindowOffset))->hWndHost;
    return S_OK;
}

// Symbol: ?Initialize@XInitializeWithStream@CDocument@@UEAAJPEAUIStream@@K@Z
// XInitializeWithStream::Initialize(IStream* pStream, DWORD grfMode) --
// RVA 0x21d030 (mfc140u):
//     if (pStream == NULL) return E_INVALIDARG;
//     pThis->m_bPreviewHandlerMode = TRUE;                // -0x24 == doc+0x12c
//     pThis->m_bAutoDelete = FALSE;                       // -0x30 == doc+0x120
//     pStream->AddRef();
//     pThis->m_pStream = pStream;                         // doc+0xb8
//     pThis->m_grfMode = grfMode;                         // doc+0xc0
//     if (!pThis->m_bOLELocked) {                         // doc+0x98
//         AfxOleLockApp();  // inlined: AfxGetModuleState() then lock incl 0x60
//         pThis->m_bOLELocked = TRUE;
//     }
//     return S_OK;
// A stream already held in m_pStream is overwritten without a Release, as in
// retail.  m_bAutoDelete is OpenMFC's declared member (+0x34), not the
// overlay.  The inlined lock sequence is exactly the body of AfxOleLockApp
// (RVA 0x25f240 (mfc140u)), so that thunk is called.  OpenMFC's AfxOleLockApp
// (detail/OlecoreSupport.cpp) is not a byte copy of retail's: besides counting
// the lock it also calls ::CoAddRefServerProcess.
// Not mirrored: OpenMFC's IsSearchAndOrganizeHandler reads its own side-table
// flag, not the overlay's m_bPreviewHandlerMode set here.
extern "C" long MS_ABI impl__Initialize_XInitializeWithStream_CDocument__UEAAJPEAUIStream__K_Z(
        void* pThisItf, IStream* pStream, unsigned long grfMode) {
    if (pStream == nullptr) return E_INVALIDARG;
    CDocument* pThis = DocFromPart(pThisItf, kXInitializeWithStreamOffset);
    S_CDocumentRetailTail* t = Tail(pThis);
    t->bPreviewHandlerMode = TRUE;
    pThis->m_bAutoDelete = FALSE;
    pStream->AddRef();
    t->pStream = pStream;
    t->grfMode = grfMode;
    if (!t->bOLELocked) {
        impl__AfxOleLockApp__YAXXZ();
        t->bOLELocked = TRUE;
    }
    return S_OK;
}

// Symbol: ?QueryFocus@XPreviewHandler@CDocument@@UEAAJPEAPEAUHWND__@@@Z
// XPreviewHandler::QueryFocus(HWND* phwnd) -- RVA 0x21d550 (mfc140u):
//     return pThis->OnPreviewHandlerQueryFocus(phwnd);    // vftable 0x208
// The dispatch is transcribed faithfully, but OpenMFC's base thunk returns the
// first view's HWND; retail's base (RVA 0x21c9a0 (mfc140u)) stores ::GetFocus()
// and returns HRESULT_FROM_WIN32(GetLastError()) when that is NULL.
extern "C" long MS_ABI impl__QueryFocus_XPreviewHandler_CDocument__UEAAJPEAPEAUHWND_____Z(void* pThisItf, HWND* phwnd) {
    return impl__OnPreviewHandlerQueryFocus_CDocument__UEAAJPEAPEAUHWND_____Z(
        DocFromPart(pThisItf, kXPreviewHandlerOffset), phwnd);
}

// Symbol: ?SetBackgroundColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
// XPreviewHandlerVisuals::SetBackgroundColor(COLORREF) -- RVA 0x21d730 (mfc140u):
//     pThis->m_clrRichPreviewBackColor = color;           // doc+0x134
//     pThis->OnRichPreviewBackColorChanged();   // vftable 0x150 (empty in base; see block note)
//     return S_OK;
extern "C" long MS_ABI impl__SetBackgroundColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(void* pThisItf, unsigned long color) {
    Tail(DocFromPart(pThisItf, kXPreviewHandlerVisualsOffset))->clrRichPreviewBackColor = color;
    return S_OK;
}

// Symbol: ?SetFocus@XPreviewHandler@CDocument@@UEAAJXZ
// XPreviewHandler::SetFocus() -- RVA 0x21d4f0 (mfc140u):
//     if (pThis->m_pPreviewFrame != NULL                  // -0x90 == doc+0xc8
//         && ::IsWindow(pThis->m_pPreviewFrame->m_hWnd))  // IAT 0x1802c7138 (mfc140u) = IsWindow
//         pThis->m_pPreviewFrame->SetFocus();             // CWnd::SetFocus (0x2a7a70 (mfc140))
//     return S_OK;
extern "C" long MS_ABI impl__SetFocus_XPreviewHandler_CDocument__UEAAJXZ(void* pThisItf) {
    S_CDocumentRetailTail* t = Tail(DocFromPart(pThisItf, kXPreviewHandlerOffset));
    if (t->pPreviewFrame != nullptr && ::IsWindow(t->pPreviewFrame->m_hWnd))
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(t->pPreviewFrame);
    return S_OK;
}

// Symbol: ?SetFont@XPreviewHandlerVisuals@CDocument@@UEAAJPEBUtagLOGFONTW@@@Z
// XPreviewHandlerVisuals::SetFont(const LOGFONTW* plf) -- RVA 0x21d790
// (mfc140u).  This is one body that is NOT byte-identical to the ANSI twin
// (0x21bc70 (mfc140) converts the LOGFONTW to a LOGFONTA first); the Unicode
// body is:
//     if (plf == NULL) return E_POINTER;
//     pThis->OnBeforeRichPreviewFontChanged();   // vftable 0x138 (empty in base)
//     pThis->m_lfRichPreviewFont.DeleteObject(); // CGdiObject::DeleteObject 0x2a3f60 (mfc140u)
//     pThis->m_lfRichPreviewFont.Attach(::CreateFontIndirectW(plf));
//                              // IAT 0x1802c61b0 (mfc140u) = CreateFontIndirectW;
//                              // CGdiObject::Attach 0x2a3ed0 (mfc140u)
//     pThis->OnRichPreviewFontChanged();         // vftable 0x140 (empty in base)
//     return S_OK;
// DEVIATION: no CFont object is constructed in the overlay, so the font is
// kept as the bare HFONT at retail's m_hObject offset (doc+0x148) and the two
// CGdiObject members are open-coded: DeleteObject is ::DeleteObject on a
// non-NULL handle followed by clearing it, Attach stores a non-NULL handle.
// Retail's Attach also registers the handle in the permanent GDI handle map;
// that registration is not reproduced.
extern "C" long MS_ABI impl__SetFont_XPreviewHandlerVisuals_CDocument__UEAAJPEBUtagLOGFONTW___Z(void* pThisItf, const LOGFONTW* plf) {
    if (plf == nullptr) return E_POINTER;
    S_CDocumentRetailTail* t = Tail(DocFromPart(pThisItf, kXPreviewHandlerVisualsOffset));
    if (t->hRichPreviewFont != nullptr) {
        HFONT hOld = t->hRichPreviewFont;
        t->hRichPreviewFont = nullptr;
        ::DeleteObject(hOld);
    }
    HFONT hNew = ::CreateFontIndirectW(plf);
    if (hNew != nullptr) t->hRichPreviewFont = hNew;
    return S_OK;
}

// Symbol: ?SetRect@XPreviewHandler@CDocument@@UEAAJPEBUtagRECT@@@Z
// XPreviewHandler::SetRect(const RECT* prc) -- RVA 0x21d270 (mfc140u):
//     if (prc == NULL) return E_INVALIDARG;
//     ::CopyRect(&pThis->m_rectHost, prc);      // IAT 0x1802c7218 (mfc140u) = CopyRect
//     if (pThis->m_pPreviewFrame != NULL) {
//         pThis->m_pPreviewFrame->SetWindowPos(NULL, 0, 0,
//             m_rectHost.right - m_rectHost.left, m_rectHost.bottom - m_rectHost.top,
//             SWP_NOZORDER | SWP_NOACTIVATE);   // CWnd::SetWindowPos
//         pThis->UpdateAllViews(NULL, 0, NULL); // the exported non-virtual,
//                                               // 0x21ac40 (mfc140)
//     }
//     return S_OK;
extern "C" long MS_ABI impl__SetRect_XPreviewHandler_CDocument__UEAAJPEBUtagRECT___Z(void* pThisItf, const RECT* prc) {
    if (prc == nullptr) return E_INVALIDARG;
    CDocument* pThis = DocFromPart(pThisItf, kXPreviewHandlerOffset);
    S_CDocumentRetailTail* t = Tail(pThis);
    ::CopyRect(&t->rectHost, prc);
    if (t->pPreviewFrame != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(t->pPreviewFrame, nullptr, 0, 0,
            t->rectHost.right - t->rectHost.left, t->rectHost.bottom - t->rectHost.top,
            SWP_NOZORDER | SWP_NOACTIVATE);
        impl__UpdateAllViews_CDocument__QEAAXPEAVCView___JPEAVCObject___Z(pThis, nullptr, 0, nullptr);
    }
    return S_OK;
}

// Symbol: ?SetSite@XObjectWithSite@CDocument@@UEAAJPEAUIUnknown@@@Z
// XObjectWithSite::SetSite(IUnknown* punkSite) -- RVA 0x21d9c0 (mfc140u):
//     pThis->m_spUnkSite = punkSite;   // inlined CComPtr assignment: skipped
//                                      // when equal, else AddRef new, swap,
//                                      // Release old
//     if (pThis->m_pPreviewHandlerSite != NULL) {         // doc+0x178
//         pThis->m_pPreviewHandlerSite->Release();
//         pThis->m_pPreviewHandlerSite = NULL;
//     }
//     if (pThis->m_spUnkSite != NULL)       // HRESULT discarded
//         pThis->m_spUnkSite->QueryInterface(IID_IPreviewHandlerFrame,
//                                            (void**)&pThis->m_pPreviewHandlerSite);
//     pThis->OnRichPreviewSiteChanged();    // vftable 0x158 (empty in base)
//     return S_OK;
extern "C" long MS_ABI impl__SetSite_XObjectWithSite_CDocument__UEAAJPEAUIUnknown___Z(void* pThisItf, IUnknown* punkSite) {
    S_CDocumentRetailTail* t = Tail(DocFromPart(pThisItf, kXObjectWithSiteOffset));
    if (t->pUnkSite != punkSite) {
        if (punkSite != nullptr) punkSite->AddRef();
        IUnknown* pOld = t->pUnkSite;
        t->pUnkSite = punkSite;
        if (pOld != nullptr) pOld->Release();
    }
    if (t->pPreviewHandlerSite != nullptr) {
        t->pPreviewHandlerSite->Release();
        t->pPreviewHandlerSite = nullptr;
    }
    if (t->pUnkSite != nullptr)
        t->pUnkSite->QueryInterface(kIID_IPreviewHandlerFrame, reinterpret_cast<void**>(&t->pPreviewHandlerSite));
    return S_OK;
}

// Symbol: ?SetTextColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
// XPreviewHandlerVisuals::SetTextColor(COLORREF) -- RVA 0x21d840 (mfc140u):
//     pThis->m_clrRichPreviewTextColor = color;           // doc+0x138
//     pThis->OnRichPreviewTextColorChanged();   // vftable 0x148 (empty in base)
//     return S_OK;
extern "C" long MS_ABI impl__SetTextColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(void* pThisItf, unsigned long color) {
    Tail(DocFromPart(pThisItf, kXPreviewHandlerVisualsOffset))->clrRichPreviewTextColor = color;
    return S_OK;
}

// Symbol: ?SetWindow@XPreviewHandler@CDocument@@UEAAJPEAUHWND__@@PEBUtagRECT@@@Z
// XPreviewHandler::SetWindow(HWND hwnd, const RECT* prc) -- RVA 0x21d200 (mfc140u):
//     pThis->m_hWndHost = hwnd;                           // -0xb8 == doc+0xa0
//     if (prc != NULL)
//         ::CopyRect(&pThis->m_rectHost, prc);  // IAT 0x1802c7218 (mfc140u) = CopyRect
//     return S_OK;
// hwnd is stored unvalidated and nothing is re-parented or resized here.
extern "C" long MS_ABI impl__SetWindow_XPreviewHandler_CDocument__UEAAJPEAUHWND____PEBUtagRECT___Z(
        void* pThisItf, HWND hwnd, const RECT* prc) {
    S_CDocumentRetailTail* t = Tail(DocFromPart(pThisItf, kXPreviewHandlerOffset));
    t->hWndHost = hwnd;
    if (prc != nullptr) ::CopyRect(&t->rectHost, prc);
    return S_OK;
}

// Symbol: ?TranslateAcceleratorW@XPreviewHandler@CDocument@@UEAAJPEAUtagMSG@@@Z
// XPreviewHandler::TranslateAccelerator(MSG* pmsg) -- missing from
// mfc140u_rva_symbols.json, but mfc140u's export table resolves this symbol to
// RVA 0x21d5b0 (mfc140u), which is also slot 9 of the XPreviewHandler vftable
// (0x180328e70 (mfc140u)); identical in shape to the ANSI twin's
// ?TranslateAcceleratorA@... at 0x21ba90 (mfc140):
//     return pThis->OnPreviewHandlerTranslateAccelerator(pmsg); // vftable 0x210
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPreviewHandler_CDocument__UEAAJPEAUtagMSG___Z(void* pThisItf, MSG* pmsg) {
    return impl__OnPreviewHandlerTranslateAccelerator_CDocument__UEAAJPEAUtagMSG___Z(
        DocFromPart(pThisItf, kXPreviewHandlerOffset), pmsg);
}

// Symbol: ?Unload@XPreviewHandler@CDocument@@UEAAJXZ
// XPreviewHandler::Unload() -- RVA 0x21d4a0 (mfc140u):
//     pThis->OnRichPreviewUnload();                       // vftable 0x160
//     return S_OK;
// The dispatch is transcribed faithfully, but OpenMFC's base thunk only clears
// the chunk list; retail's base (RVA 0x21ce10 (mfc140u)) also clears
// m_bModified, calls vftable 0x220 and DeleteContents, releases
// m_pPreviewHandlerSite, calls ::DestroyWindow on m_pPreviewFrame's HWND (when
// ::IsWindow) and clears the pointer, releases m_pStream and
// resets m_hWndHost/m_rectHost/m_grfMode.
extern "C" long MS_ABI impl__Unload_XPreviewHandler_CDocument__UEAAJXZ(void* pThisItf) {
    impl__OnRichPreviewUnload_CDocument__UEAAXXZ(DocFromPart(pThisItf, kXPreviewHandlerOffset));
    return S_OK;
}
