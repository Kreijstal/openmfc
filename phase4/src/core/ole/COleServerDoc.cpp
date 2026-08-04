// COleServerDoc — OpenMFC implementation.
// Sources: global_ole-2_impl.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// COleClientItem / COleServerDoc methods, wave 2 (ole-2). Every function here
// was decoded from the retail mfc140u export (ordinal -> RVA -> objdump, see
// /tmp/openmfc_remaining/disasm.py) rather than inferred, and the members
// touched are exactly the ones declared in include/openmfc/afxole.h.
//
// COleClientItem layout (per afxole.h, member names only):
//   m_pDocument@0x40 (CDocItem) m_pContainerDoc@0x48 m_lpObject@0x50
//   m_lpLink@0x58 m_lpInPlaceObject@0x60 m_nStatus@0x68 m_nDrawAspect@0x6C
//   m_pControlSite@0x70 m_lpFrame@0x78 m_lpDocFrame@0x80 m_bInPlaceActive@0x88
// COleServerDoc declares only m_bEmbedded@0x230; the retail in-place state
// (m_pInPlaceFrame@0x230, m_pInPlaceDocFrame@0x250, m_rectPos/m_rectClip,
// m_pDocObjectServer@0x268, ...) is NOT modeled, so COleServerDoc methods that
// need it are implemented conservatively (safe defaults + TODO(clean-room)).
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these symbols.

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxstr.h"

#include <ole2.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" int MS_ABI impl__CanCloseFrame_CDocument__UEAAHPEAVCFrameWnd___Z(
    CDocument* pThis, CFrameWnd* pFrameWnd);
// AfxMessageBox(UINT,UINT,UINT) — impl defined in strcore.cpp
// (mangled: ?AfxMessageBox@@YAHIII@Z)
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType,
                                                    UINT nIDHelp);

// ---- forward declarations for impl_ functions defined later in this file ----
extern "C" void MS_ABI impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar);
extern "C" void MS_ABI impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar);

//=============================================================================
// COleClientItem
//=============================================================================












//=============================================================================
// COleServerDoc
//=============================================================================
//
// The retail COleServerDoc carries its in-place frame, doc-frame, item rects,
// and doc-object server inside the region that OpenMFC leaves as padding (only
// m_bEmbedded is declared). Functions below that depend on that region return
// safe defaults and are marked TODO(clean-room).




















// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// COleServerDoc::ActivateDocObject() — retail (0x265ad0):
//     if (m_pDocObjectServer@0x268) m_pDocObjectServer->ActivateDocObject();
// The m_pDocObjectServer member is not modeled.
// Symbol: ?ActivateDocObject@COleServerDoc@@QEAAXXZ
extern "C" void MS_ABI impl__ActivateDocObject_COleServerDoc__QEAAXXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail forwards to the
    // CDocObjectServer (member at retail offset 0x268); not modeled.
}
// COleServerDoc::ActivateInPlace() — retail (0x266da0) is the full in-place
// activation handshake over m_pInPlaceFrame@0x230 / m_pInPlaceDocFrame@0x250
// and the frame-creation virtuals. None of that state is modeled, so the safe
// answer is FALSE (no in-place activation); callers fall back to non-inplace
// editing rather than corrupting state.
// Symbol: ?ActivateInPlace@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__ActivateInPlace_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail checks/creates the
    // in-place frame (m_pInPlaceFrame@0x230) and doc frame (m_pInPlaceDocFrame
    // @0x250) and performs the OLE in-place handshake; not modeled.
    return FALSE;
}
// COleServerDoc::CanCloseFrame(CFrameWnd*) — retail (0x2660a0):
//     m_bInPlaceCloseDeactivate@0x244 = 1;
//     if (CDocument::CanCloseFrame(pFrameWnd)) return TRUE;
//     m_bInPlaceCloseDeactivate@0x244 = 0;
//     return FALSE;
// The 0x244 flag is not modeled; the base result is forwarded unchanged.
// Symbol: ?CanCloseFrame@COleServerDoc@@UEAAHPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__CanCloseFrame_COleServerDoc__UEAAHPEAVCFrameWnd___Z(
    COleServerDoc* pThis, CFrameWnd* pFrameWnd)
{
    if (!pThis) return TRUE;
    // TODO(clean-room): retail also tracks the result in the unmodeled
    // m_bInPlaceCloseDeactivate flag (offset 0x244).
    return impl__CanCloseFrame_CDocument__UEAAHPEAVCFrameWnd___Z(pThis, pFrameWnd);
}
// COleServerDoc::ConnectView(CWnd*, CView*) — retail (0x2676f0) connects the
// view's HWND into the frame/view machinery (SetWindowLong of the view
// association, then virtual OnActivateView on the frame and the view
// redirection helpers 0x28e200/0x29ed20). The frame/view machinery is not
// modeled; a no-op is the safe, type-correct body.
// Symbol: ?ConnectView@COleServerDoc@@IEAAXPEAVCWnd@@PEAVCView@@@Z
extern "C" void MS_ABI impl__ConnectView_COleServerDoc__IEAAXPEAVCWnd__PEAVCView___Z(
    COleServerDoc* pThis, CWnd* /*pWnd*/, CView* /*pView*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail links the view to the
    // frame window and notifies the frame (virtual OnActivateView, vtable slot
    // 0x2B0); the frame/view association is not modeled.
}
// COleServerDoc::CreateInPlaceFrame(CWnd*) — retail (0x2674b0) allocates a
// COleIPFrameWnd (retail helper 0x28e560, id 0xE900), fills the frame's
// window-context data and stores the menu/accelerator ids into unmodeled
// members (0x260/0x264). Not reproducible; return NULL (conservative).
// Symbol: ?CreateInPlaceFrame@COleServerDoc@@MEAAPEAVCOleIPFrameWnd@@PEAVCWnd@@@Z
extern "C" COleIPFrameWnd* MS_ABI
impl__CreateInPlaceFrame_COleServerDoc__MEAAPEAVCOleIPFrameWnd__PEAVCWnd___Z(
    COleServerDoc* pThis, CWnd* /*pParentWnd*/)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail news a COleIPFrameWnd,
    // initializes it with the frame/menu resources and stores the resulting
    // resource ids in unmodeled members at 0x260/0x264.
    return nullptr;
}
// COleServerDoc::DeactivateAndUndo() — retail (0x2680d0):
//     pSite = QueryInterface(m_pInPlaceFrame@0x230, IID_IOleInPlaceSite);
//     if (pSite) { hr = pSite->DeactivateAndUndo(); pSite->Release();
//                  return SUCCEEDED(hr); }
//     return FALSE;
// m_pInPlaceFrame is not modeled.
// Symbol: ?DeactivateAndUndo@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__DeactivateAndUndo_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail asks the in-place frame
    // (m_pInPlaceFrame@0x230, queried for IID_IOleInPlaceSite) to
    // DeactivateAndUndo; the frame member is not modeled.
    return FALSE;
}
// COleServerDoc::DeleteContents() — retail (0x265960) calls the base
// COleDocument::DeleteContents and then iterates the document's client items,
// deleting each. The base override and the item-list iteration are not modeled
// in OpenMFC (COleDocument does not declare DeleteContents); the closest base
// (CDocument::DeleteContents) is forwarded to.
// Symbol: ?DeleteContents@COleServerDoc@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_COleServerDoc__UEAAXXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return;
    pThis->CDocument::DeleteContents();
    // TODO(clean-room): transcribed partially. Retail additionally iterates the
    // document's COleClientItem list and calls Delete(TRUE) on each; the item
    // list iteration is not modeled in OpenMFC.
}
// COleServerDoc::DestroyInPlaceFrame(COleIPFrameWnd*) — retail (0x2675a0)
// hides/removes the shared menus and destroys the in-place frame window
// (helpers 0x28e560 and the frame destroy path). Not reproducible; a no-op is
// the safe, type-correct body.
// Symbol: ?DestroyInPlaceFrame@COleServerDoc@@MEAAXPEAVCOleIPFrameWnd@@@Z
extern "C" void MS_ABI impl__DestroyInPlaceFrame_COleServerDoc__MEAAXPEAVCOleIPFrameWnd___Z(
    COleServerDoc* pThis, COleIPFrameWnd* /*pFrameWnd*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail removes the shared
    // menu/accelerator from the in-place frame and destroys its window; the
    // frame-shared-menu machinery is not modeled.
}
// COleServerDoc::DiscardUndoState() — retail (0x268130): mirror image of
// DeactivateAndUndo using IOleInPlaceSite::DiscardUndoState (vtable slot 0x60)
// on the in-place frame's site. m_pInPlaceFrame is not modeled.
// Symbol: ?DiscardUndoState@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__DiscardUndoState_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail asks the in-place frame's
    // IOleInPlaceSite to DiscardUndoState; not modeled.
    return FALSE;
}
// COleServerDoc::GetDefaultAccelerator() — retail (0x266410) returns the
// accelerator table held by the in-place doc frame (member 0x250) or the
// in-place frame (member 0x230) through a server-state object at 0x50. Those
// members are not modeled; NULL is the safe default.
// Symbol: ?GetDefaultAccelerator@COleServerDoc@@MEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_COleServerDoc__MEAAPEAUHACCEL____XZ(
    COleServerDoc* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail reads the accelerator
    // from the in-place frame/doc-frame state (retail offsets 0x230/0x250/0x50);
    // not modeled.
    return nullptr;
}
// COleServerDoc::GetDefaultMenu() — retail (0x2663e0): same shape as
// GetDefaultAccelerator but returns the frame menu handle (offsets 0x88/0x78 of
// the state object at 0x50). Not modeled; NULL is the safe default.
// Symbol: ?GetDefaultMenu@COleServerDoc@@MEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetDefaultMenu_COleServerDoc__MEAAPEAUHMENU____XZ(
    COleServerDoc* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail reads the menu from the
    // in-place frame/doc-frame state (retail offsets 0x230/0x250/0x50); not
    // modeled.
    return nullptr;
}
// COleServerDoc::GetDocObjectServer(IOleDocumentSite*) — retail (0x71e0) is a
// bare `xor eax,eax; ret` for the base class: the base server document creates
// no doc-object server. CDocObjectServer is not declared in OpenMFC, so the
// return is carried as void*.
// Symbol: ?GetDocObjectServer@COleServerDoc@@MEAAPEAVCDocObjectServer@@PEAUIOleDocumentSite@@@Z
extern "C" void* MS_ABI
impl__GetDocObjectServer_COleServerDoc__MEAAPEAVCDocObjectServer__PEAUIOleDocumentSite___Z(
    COleServerDoc* pThis, void* /*pDocSite*/)
{
    (void)pThis;
    return nullptr;
}
// COleServerDoc::GetFileTypeString(CString&) — retail (0x2660e0) fills the
// string from the server-state object at 0x50 (a virtual GetFileTypeString on
// it) and returns TRUE when the string is non-empty. The server-state object is
// not modeled, so FALSE (no string) is returned and the string left untouched.
// Symbol: ?GetFileTypeString@COleServerDoc@@MEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI
impl__GetFileTypeString_COleServerDoc__MEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleServerDoc* pThis, CString& /*rString*/)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially. Retail asks the server-state
    // object (retail offset 0x50) for the file type string; not modeled.
    return 0;
}
// COleServerDoc::GetInterfaceHook(const void*) — retail (0x265a80):
//     if (m_pDocObjectServer@0x268 && IsEqualIID(iid, IID_IUnknown))
//         return NULL;               // don't hook IUnknown queries
//     return <base GetInterfaceHook>;
// Both the doc-object-server member and the base hook are not modeled; NULL
// (no interception) is the safe default.
// Symbol: ?GetInterfaceHook@COleServerDoc@@MEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_COleServerDoc__MEAAPEAUIUnknown__PEBX_Z(
    COleServerDoc* pThis, const void* /*pInterfaceID*/)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail only intercepts
    // non-IUnknown queries when a doc-object server exists (retail offset
    // 0x268); not modeled.
    return nullptr;
}
// COleServerDoc::GetItemClipRect(RECT*) const — retail (0x265e20) copies 16
// bytes from the in-place doc frame's clip rect (frame@0x250 + 0x258). The
// frame member is not modeled, so an empty rect is returned (type-correct).
// Symbol: ?GetItemClipRect@COleServerDoc@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemClipRect_COleServerDoc__QEBAXPEAUtagRECT___Z(
    const COleServerDoc* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    // TODO(clean-room): transcribed partially. Retail copies the clip rect
    // stored in the in-place doc frame (member at retail offset 0x250); the
    // frame member is not modeled, so an empty rect is reported.
    lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
}
// COleServerDoc::GetItemPosition(RECT*) const — retail (0x265e00) copies 16
// bytes from the in-place doc frame's item rect (frame@0x250 + 0x248). Not
// modeled; empty rect reported.
// Symbol: ?GetItemPosition@COleServerDoc@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemPosition_COleServerDoc__QEBAXPEAUtagRECT___Z(
    const COleServerDoc* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    // TODO(clean-room): transcribed partially. Retail copies the item rect from
    // the in-place doc frame (member at retail offset 0x250); not modeled.
    lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
}
// COleServerDoc::GetMoniker(OLEGETMONIKER) — retail (0x266330):
//     if (m_pInPlaceFrame@0x230) { ... query the frame for a moniker and return it; }
//     return COleLinkingDoc::GetMoniker(nAssign);
// The in-place frame is not modeled, so the base (document-file moniker) path
// always applies — which is the retail fallback.
// Symbol: ?GetMoniker@COleServerDoc@@UEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" LPMONIKER MS_ABI impl__GetMoniker_COleServerDoc__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(
    COleServerDoc* pThis, int nAssign)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail, when in-place activated,
    // asks the in-place frame for a moniker; without the frame member the base
    // COleLinkingDoc::GetMoniker result is returned.
    return pThis->COleLinkingDoc::GetMoniker(static_cast<OLEGETMONIKER>(nAssign));
}
// COleServerDoc::GetZoomFactor(SIZE*, SIZE*, const RECT*) const — retail
// (0x265cd0): with no in-place doc frame, both factors are set to 1 and FALSE
// is returned; with a frame, the factors are computed from the frame's rects
// (frame@0x250). Only the no-frame branch is reproducible in OpenMFC.
// Symbol: ?GetZoomFactor@COleServerDoc@@QEBAHPEAUtagSIZE@@0PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__GetZoomFactor_COleServerDoc__QEBAHPEAUtagSIZE__0PEBUtagRECT___Z(
    const COleServerDoc* pThis, tagSIZE* lpSizeNum, tagSIZE* lpSizeDen,
    const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // The in-place doc frame (retail offset 0x250) is not modeled, so the
    // "no frame" branch always applies.
    if (lpSizeNum) {
        lpSizeNum->cx = 1;
        lpSizeNum->cy = 1;
        if (lpSizeDen) {
            lpSizeDen->cx = 1;
            lpSizeDen->cy = 1;
        }
    }
    // TODO(clean-room): transcribed partially. Retail computes the zoom from
    // the in-place doc frame's item/clip rects when the frame exists.
    return FALSE;
}
// COleServerDoc::NotifyAllItems(OLE_NOTIFICATION, unsigned __int64) — retail
// (0x265ba0) iterates the document's COleServerItem list and calls each item's
// Notify(nCode, dwParam). The server-item list iteration is not modeled in
// OpenMFC, so a no-op is emitted.
// Symbol: ?NotifyAllItems@COleServerDoc@@IEAAXW4OLE_NOTIFICATION@@_K@Z
extern "C" void MS_ABI impl__NotifyAllItems_COleServerDoc__IEAAXW4OLE_NOTIFICATION___K_Z(
    COleServerDoc* pThis, int /*nCode*/, unsigned long long /*dwParam*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail forwards to each
    // COleServerItem::Notify in the document's item list; the list is not
    // modeled in OpenMFC.
}
// Symbol: ?OnUpdateFileExit@COleServerDoc@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileExit_COleServerDoc__IEAAXPEAVCCmdUI___Z(COleServerDoc*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateFileUpdate@COleServerDoc@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateFileUpdate_COleServerDoc__IEAAXPEAVCCmdUI___Z(COleServerDoc*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?UpdateUsingHostObj@COleServerDoc@@IEAAXIPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__UpdateUsingHostObj_COleServerDoc__IEAAXIPEAVCCmdUI___Z(COleServerDoc*, unsigned int nID, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
    (void)nID;
}
COleServerDoc::COleServerDoc()
    : COleLinkingDoc(), m_bEmbedded(FALSE) {
    memset(_coleserverdoc_padding, 0, sizeof(_coleserverdoc_padding));
    GetServerDocState(this, true);
}
COleServerDoc::~COleServerDoc() {
    ServerDocState* state = GetServerDocState(this, false);
    if (state) {
        for (COleServerItem* item : state->items) {
            if (item && item->m_pServerDoc == this) item->m_pServerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    RemoveServerDocState(this);
}
void COleServerDoc::NotifyChanged() {
    SetModifiedFlag(TRUE);
    UpdateAllViews(nullptr, 0, nullptr);
}
void COleServerDoc::NotifyClosed() {
    if (m_bRegistered || m_lpMoniker) Revoke();
}
void COleServerDoc::NotifyRename(const wchar_t* lpszNewName) {
    const wchar_t* newName = lpszNewName ? lpszNewName : L"";
    SetTitle(newName);
    if (*newName) {
        SetPathName(newName, FALSE);
        RegisterIfServerAttached(newName, FALSE);
    }
}
void COleServerDoc::NotifySaved() {
    SetModifiedFlag(FALSE);
}
void COleServerDoc::SaveEmbedding() {
    const wchar_t* path = GetPathName();
    if (path && *path) {
        OnSaveDocument(path);
    } else {
        SetModifiedFlag(FALSE);
    }
}
COleClientItem* COleServerDoc::GetEmbeddedItem() {
    return OnGetEmbeddedItem();
}
COleServerItem* COleServerDoc::GetEmbeddedServerItem() {
    ServerDocState* state = GetServerDocState(this, false);
    if (!state || state->items.empty()) return nullptr;
    return state->items.front();
}
COleServerItem* COleServerDoc::GetLinkedServerItem(const wchar_t* lpszItemName) {
    ServerDocState* state = GetServerDocState(this, false);
    if (!state || state->items.empty()) return nullptr;
    if (!lpszItemName || !*lpszItemName) return state->items.front();
    size_t index = ParseLinkedItemIndex(lpszItemName);
    if (index != 0 && index <= state->items.size()) return state->items[index - 1];
    for (COleServerItem* item : state->items) {
        if (!item) continue;
        COleServerDoc* itemDoc = item->GetDocument();
        const wchar_t* title = itemDoc ? itemDoc->GetTitle() : nullptr;
        if (title && wcscmp(title, lpszItemName) == 0) return item;
    }
    return state->items.size() == 1 ? state->items.front() : nullptr;
}
BOOL COleServerDoc::OnSetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) {
    ServerDocState* state = GetServerDocState(this, true);
    if (state) {
        state->hostName = lpszHost ? lpszHost : L"";
        state->hostObjectName = lpszHostObj ? lpszHostObj : L"";
    }
    return TRUE;
}
