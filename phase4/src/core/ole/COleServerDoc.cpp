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
// COleServerDoc declares only m_bEmbedded plus 96 bytes of padding; the retail
// server state (m_lpClientSite@0x230, m_pInPlaceFrame@0x250, m_rectPos /
// m_rectClip, m_pDocObjectServer@0x268, ...) is NOT modeled, so COleServerDoc
// methods that need it are implemented conservatively (safe defaults +
// TODO(clean-room)).
//
// CORRECTION (wave-4 review, evidenced below): the labels originally written
// here had +0x230 and +0x250 the wrong way round.  +0x230 is the
// IOleClientSite* and +0x250 is the COleIPFrameWnd*.  Proof, all from the
// retail disassembly:
//   * OnDeactivate (mfc140.dll 0x266b30) passes *(this+0x250) as the argument
//     to DestroyInPlaceFrame (vtable byte offset 0x320, whose retail body is
//     ?DestroyInPlaceFrame@COleServerDoc@@MEAAXPEAVCOleIPFrameWnd@@@Z), and
//     passes *(this+0x230) to the QI helper 0x26ba50 with IID 0x2d7b68, then
//     calls slot 11 (IOleInPlaceSite::OnInPlaceDeactivate) on the result.
//   * XOleObject::GetClientSite (0x267420) returns *(subobject-0x48) with the
//     XOleObject sub-object at doc+0x278 (its prologue is
//     `lea -0x278(%rcx),%rbx`, see Close@XOleObject 0x267550), i.e. doc+0x230.
//   * GetMoniker (0x2650e0) calls slot 4 (IOleClientSite::GetMoniker) on
//     *(this+0x230).
// Every mislabelled per-function comment below has been corrected in place.
//
// RVA NAMESPACE WARNING.  The comments in this first block cite RVAs in
// **mfc140u.dll** (the Unicode retail DLL).  The wave-3 block further down
// cites RVAs in **mfc140.dll** (the MBCS retail DLL) -- that is the image the
// campaign's disas.py actually opens.  The two numbering spaces collide: e.g.
// 0x265ad0 is ActivateDocObject in mfc140u.dll but OnSaveEmbedding in
// mfc140.dll, and 0x266da0 is ActivateInPlace in mfc140u.dll but OnSetItemRects
// in mfc140.dll.  Always check which image a cited RVA belongs to.  The
// mfc140.dll RVAs for the functions commented in this first block are:
//   ActivateDocObject 0x264880   ActivateInPlace   0x265b50
//   CanCloseFrame     0x264e50   ConnectView       0x2664b0
//   CreateInPlaceFrame 0x266270  DeactivateAndUndo 0x266e90
//   DeleteContents    0x264710   DestroyInPlaceFrame 0x266360
//   DiscardUndoState  0x266ef0   GetDefaultAccelerator 0x2651c0
//   GetDefaultMenu    0x265190   GetFileTypeString 0x264e90
//   GetInterfaceHook  0x264830   GetItemClipRect   0x264bd0
//   GetItemPosition   0x264bb0   GetMoniker        0x2650e0
//   GetZoomFactor     0x264a80   NotifyAllItems    0x264950
//   OnSetHostNames    0x264ed0
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
// COleServerDoc::ActivateInPlace() — retail (mfc140u 0x266da0 = mfc140
// 0x265b50) is the full in-place activation handshake over m_lpClientSite@0x230
// / m_pInPlaceFrame@0x250 and the frame-creation virtuals. None of that state
// is modeled, so the safe answer is FALSE (no in-place activation); callers
// fall back to non-inplace editing rather than corrupting state.
// Symbol: ?ActivateInPlace@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__ActivateInPlace_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail negotiates with the
    // client site (m_lpClientSite@0x230) and creates the in-place frame
    // (m_pInPlaceFrame@0x250); neither member is modeled.
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
//     pSite = QueryInterface(m_lpClientSite@0x230, IID_IOleInPlaceSite);
//     if (pSite) { hr = pSite->DeactivateAndUndo(); pSite->Release();
//                  return SUCCEEDED(hr); }
//     return FALSE;
// Verified against mfc140.dll 0x266e90: `mov 0x230(%rcx),%rcx` feeds the QI
// helper 0x26ba50 with the IID at 0x2d7b68, then vtable slot 13 (0x68) is
// called and the result Release'd (slot 2).  m_lpClientSite is not modeled.
// Symbol: ?DeactivateAndUndo@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__DeactivateAndUndo_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail asks the client site
    // (m_lpClientSite@0x230, queried for IID_IOleInPlaceSite) to
    // DeactivateAndUndo; the client-site member is not modeled.
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
// COleServerDoc::DiscardUndoState() — retail (mfc140u 0x268130 = mfc140
// 0x266ef0): mirror image of DeactivateAndUndo using
// IOleInPlaceSite::DiscardUndoState (vtable byte offset 0x60, slot 12) on
// m_lpClientSite@0x230.  m_lpClientSite is not modeled.
// Symbol: ?DiscardUndoState@COleServerDoc@@QEAAHXZ
extern "C" int MS_ABI impl__DiscardUndoState_COleServerDoc__QEAAHXZ(
    COleServerDoc* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail asks the client site's
    // IOleInPlaceSite to DiscardUndoState; m_lpClientSite is not modeled.
    return FALSE;
}
// COleServerDoc::GetDefaultAccelerator() — retail (mfc140u 0x266410 = mfc140
// 0x2651c0):
//     void* pState = *(this+0x50);          // server-state object
//     if (pState == NULL) return NULL;
//     if (m_pInPlaceFrame@0x250 != NULL) return *(pState+0x90);
//     if (m_lpClientSite@0x230 != NULL) return *(pState+0x80);
//     return NULL;
// None of the three members is modeled; NULL is the safe default.
// Symbol: ?GetDefaultAccelerator@COleServerDoc@@MEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_COleServerDoc__MEAAPEAUHACCEL____XZ(
    COleServerDoc* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail picks the accelerator
    // out of the server-state object at retail offset 0x50, selecting on
    // m_pInPlaceFrame@0x250 / m_lpClientSite@0x230; not modeled.
    return nullptr;
}
// COleServerDoc::GetDefaultMenu() — retail (mfc140u 0x2663e0 = mfc140
// 0x265190): same shape as GetDefaultAccelerator but returns the menu handle
// (*(pState+0x88) when m_pInPlaceFrame@0x250 is set, *(pState+0x78) when only
// m_lpClientSite@0x230 is).  Not modeled; NULL is the safe default.
// Symbol: ?GetDefaultMenu@COleServerDoc@@MEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__GetDefaultMenu_COleServerDoc__MEAAPEAUHMENU____XZ(
    COleServerDoc* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail picks the menu out of
    // the server-state object at retail offset 0x50, selecting on
    // m_pInPlaceFrame@0x250 / m_lpClientSite@0x230; not modeled.
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
// COleServerDoc::GetInterfaceHook(const void*) — retail (mfc140u 0x265a80 =
// mfc140 0x264830).  Re-read from the disassembly in the wave-4 review; the
// earlier note here had the control flow wrong:
//     if (m_pDocObjectServer@0x268 == NULL) return NULL;
//     if (IsEqualIID(*pIID, *(const IID*)0x2d79a8 /* IID_IUnknown */))
//         return NULL;               // don't hook IUnknown queries
//     return CCmdTarget::GetInterface(pIID);        // 0x26bc00
// So with no doc-object server retail returns NULL outright — it does NOT fall
// through to a base hook, and the fallback is GetInterface, not
// GetInterfaceHook.  The member is not modeled and CCmdTarget::GetInterface is
// an unimplemented stub here (core/runtime/CCmdTarget.cpp:340), so NULL (no
// interception) is both the safe default and what retail returns in this
// port's state.
// Symbol: ?GetInterfaceHook@COleServerDoc@@MEAAPEAUIUnknown@@PEBX@Z
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_COleServerDoc__MEAAPEAUIUnknown__PEBX_Z(
    COleServerDoc* pThis, const void* /*pInterfaceID*/)
{
    if (!pThis) return nullptr;
    // Retail returns NULL whenever there is no doc-object server (retail
    // offset 0x268), which is always the case here, so NULL is exact rather
    // than merely conservative.
    return nullptr;
}
// COleServerDoc::GetItemClipRect(RECT*) const — retail (mfc140u 0x265e20 =
// mfc140 0x264bd0) copies 16 bytes from m_pInPlaceFrame@0x250 + 0x258.  Note
// retail does NOT null-check the frame here.  The frame member is not modeled,
// so an empty rect is returned (type-correct).
// Symbol: ?GetItemClipRect@COleServerDoc@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemClipRect_COleServerDoc__QEBAXPEAUtagRECT___Z(
    const COleServerDoc* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    // TODO(clean-room): transcribed partially. Retail copies the clip rect
    // stored in the in-place frame (m_pInPlaceFrame, retail offset 0x250); the
    // frame member is not modeled, so an empty rect is reported.
    lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
}
// COleServerDoc::GetItemPosition(RECT*) const — retail (mfc140u 0x265e00 =
// mfc140 0x264bb0) copies 16 bytes from m_pInPlaceFrame@0x250 + 0x248 (again
// with no null check).  Not modeled; empty rect reported.
// Symbol: ?GetItemPosition@COleServerDoc@@QEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemPosition_COleServerDoc__QEBAXPEAUtagRECT___Z(
    const COleServerDoc* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    // TODO(clean-room): transcribed partially. Retail copies the item rect from
    // the in-place frame (m_pInPlaceFrame, retail offset 0x250); not modeled.
    lpRect->left = lpRect->top = lpRect->right = lpRect->bottom = 0;
}
// COleServerDoc::GetMoniker(OLEGETMONIKER) — retail (mfc140u 0x266330 = mfc140
// 0x2650e0):
//     LPOLECLIENTSITE pSite = m_lpClientSite@0x230;
//     if (pSite != NULL) {
//         LPMONIKER pmk = NULL;
//         pSite->GetMoniker(nAssign, OLEWHICHMK_OBJFULL /*3*/, &pmk);  // slot 4
//         return pmk;
//     }
//     return COleLinkingDoc::GetMoniker(nAssign);   // tail jmp 0x25cd80
// m_lpClientSite is not modeled, so the base (document-file moniker) path
// always applies — which is the retail fallback.
// Symbol: ?GetMoniker@COleServerDoc@@UEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" LPMONIKER MS_ABI impl__GetMoniker_COleServerDoc__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(
    COleServerDoc* pThis, int nAssign)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially. Retail, when it has a client
    // site, asks IOleClientSite::GetMoniker; without m_lpClientSite the base
    // COleLinkingDoc::GetMoniker result is returned.
    return pThis->COleLinkingDoc::GetMoniker(static_cast<OLEGETMONIKER>(nAssign));
}
// COleServerDoc::GetZoomFactor(SIZE*, SIZE*, const RECT*) const — retail
// (mfc140u 0x265cd0 = mfc140 0x264a80): with no in-place frame, both factors
// are set to 1 and FALSE is returned; with a frame, the factors are computed
// from the frame's rects (m_pInPlaceFrame@0x250). Only the no-frame branch is
// reproducible in OpenMFC.
// Symbol: ?GetZoomFactor@COleServerDoc@@QEBAHPEAUtagSIZE@@0PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__GetZoomFactor_COleServerDoc__QEBAHPEAUtagSIZE__0PEBUtagRECT___Z(
    const COleServerDoc* pThis, tagSIZE* lpSizeNum, tagSIZE* lpSizeDen,
    const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // m_pInPlaceFrame (retail offset 0x250) is not modeled, so the
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
//=============================================================================
// COleServerDoc — wave 3
//
// Every symbol below was decoded from the retail export (mangled name -> RVA
// -> objdump), not inferred.  The image disassembled is the retail mfc140.dll
// shipped with VC 14.51; it is the MBCS build, so a few RVAs are cited from the
// `PEBD` (char) spelling of a method whose Unicode `PEB_W` spelling this file
// exports.  Only the string character type differs.
//
// METHOD NOTE (wave-4).  This block originally reported three symbols —
// OnExecOleCmd, OnReactivateAndUndo and XDataObject::GetCanonicalFormatEtc — as
// having "no RVA" and therefore undecodable, and left generated return values
// standing for all three.  That conclusion came from searching only
// mfc140_rva_symbols.json, which is built from the MBCS mfc140.dll.  All three
// resolve in the **Unicode** image: the campaign scratchpad also carries
// sym2rva_mfc140u.dll.json (14,103 symbols), and mfc140u.dll sits beside
// mfc140.dll in the redist directory, so it can be disassembled with plain
// objdump at base 0x180000000.  Two of the three turned out to return
// something other than the generated value, and both are corrected below.
// BEFORE DECLARING A SYMBOL UNDECODABLE, CHECK THE UNICODE MAP TOO.
//
// Retail member offsets established for this wave, from the disassembly:
//   +0x230  m_lpClientSite (IOleClientSite*).  Proved by
//           COleServerDoc::XOleObject::GetClientSite (0x267420): the
//           XOleObject sub-object sits at +0x278 (its METHOD_PROLOGUE does
//           `lea -0x278(%rcx)`), and GetClientSite hands out *(this-0x48)
//           = doc+0x230 after AddRef'ing it through vtable slot 1.
//   +0x250  m_pInPlaceFrame (COleIPFrameWnd*).  Proved by
//           COleServerDoc::OnDeactivate (0x266b30), which passes it as the
//           argument to DestroyInPlaceFrame (vtable slot 100 = 0x320).
//   COleDocument: +0x1c8 holds an IStorage* that OnNewEmbedding (0x265950)
//           AddRefs and swaps in and OnSaveEmbedding (0x265ad0) swaps in and
//           out — i.e. m_lpRootStg; +0x1d0 gates that swap (m_bCompoundFile)
//           and +0x1d4 is the BOOL OnSaveDocument (0x265390) reads as
//           bRemember and OnFileSaveCopyAs (0x265240) clears and re-sets
//           around DoSave — i.e. m_bRemember.  Those three names are read off
//           their use plus COleDocument's declaration order; there is no
//           symbol in the image that confirms them.
// The file header at the top of this .cpp used to label 0x230/0x250 the other
// way round; the wave-4 review confirmed the reading above and corrected the
// header and every per-function comment that repeated the old labels.  Either
// way OpenMFC models none of them: COleServerDoc declares only m_bEmbedded plus
// 96 bytes of padding, and nothing in the port ever stores a client site or an
// in-place frame.
//
// Vtable slot numbers used below were read out of the retail vtable in .rdata.
// CORRECTION (wave-4): that vtable BEGINS at RVA 0x2efe28, not at 0x2eff40 as
// this block originally said — 0x2eff40 is merely the slot inside it that holds
// OnCloseDocument (base + 0x118).  The slot numbers themselves, listed below,
// were and are correct: each is the byte offset divided by 8 measured from
// 0x2efe28.  Note also that the image contains exactly one vtable in this
// family (its slot 0 is ?GetThisClass@COleServerDocEx@@SAPEAUCRuntimeClass@@XZ,
// so it is COleServerDocEx's); the COleServerDoc portion of the layout is
// shared, and every slot cited here resolves to the COleServerDoc/COleDocument/
// CDocument body named:
//   26 (0xd0) CDocument::IsModified        27 (0xd8)  SetModifiedFlag
//        (slots 26/27 hold unexported one-instruction bodies at 0x96d40 /
//         0x96d30 — `mov 0x90(%rcx),%eax` / `mov %edx,0x90(%rcx)` — i.e. the
//         getter and setter for CDocument::m_bModified at retail offset 0x90)
//   35 (0x118) OnCloseDocument             56 (0x1c0) SaveModified
//   58 (0x1d0) COleDocument::DoSave        71 (0x238) GetStartPosition
//   75 (0x258) COleDocument::UpdateModifiedFlag
//   85 (0x2a8) COleServerDoc::OnUpdateDocument
//   92 (0x2e0) OnDeactivate               100 (0x320) DestroyInPlaceFrame
//=============================================================================

// ---- sibling impl_ exports called by the bodies below (defined elsewhere in
// ---- phase4/src; see §1 of the campaign briefing -- the C++ methods of these
// ---- classes exist only as impl__ thunks).
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName);          // core/doc/CDocument.cpp
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(
    CDocument* pThis);                                       // core/doc/CDocument.cpp
// NOTE: the generated definitions of the next two in core/ole/COleDocument.cpp
// (lines 325 and 241) are still parameterless -- `impl__..._UEAAXXZ() {}` -- so
// today the `this` argument declared here is placed in RCX and ignored by the
// callee.  That is harmless under the MS x64 convention (caller-cleaned,
// register-passed) and it is the shape the real exports have, so these calls
// start working unchanged the moment those two stubs grow a body.  Do not
// "fix" this by dropping the parameter.
extern "C" void MS_ABI impl__UpdateModifiedFlag_COleDocument__UEAAXXZ(
    COleDocument* pThis);                                    // core/ole/COleDocument.cpp
extern "C" void* MS_ABI impl__GetFirstFrame_COleDocument__QEAAPEAVCFrameWnd__XZ(
    COleDocument* pThis);                                    // core/ole/COleDocument.cpp
extern "C" int MS_ABI impl__AfxComparePath__YAHPEB_W0_Z(
    const wchar_t* pszPath1, const wchar_t* pszPath2);       // core/collections/Globals.cpp
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc); // detail/MfcExceptionsSupport.cpp

// ---- forward declarations for impl_ functions defined later in this file ----
extern "C" int MS_ABI impl__SaveModifiedPrompt_COleServerDoc__IEAAHXZ(COleServerDoc* pThis);
extern "C" int MS_ABI impl__SaveModified_COleServerDoc__MEAAHXZ(COleServerDoc* pThis);
extern "C" int MS_ABI impl__OnUpdateDocument_COleServerDoc__UEAAHXZ(COleServerDoc* pThis);

// COleServerDoc::OnClose(OLECLOSE) — retail 0x264c60:
//     if (flag@0x244 != 0) return;   // the same flag CanCloseFrame above sets
//     CFrameWnd* pFrame = GetFirstFrame();                       // 0x253a50
//     if (pFrame != NULL && ::IsWindowVisible(pFrame->m_hWnd@0x40))
//         dwCloseOption = 0;                                     // OLECLOSE_SAVEIFDIRTY
//     if (IsModified()) {                                        // slot 26
//         if (dwCloseOption == OLECLOSE_SAVEIFDIRTY)
//             SaveEmbedding();                                   // 0x265050
//         else if (dwCloseOption == OLECLOSE_PROMPTSAVE &&
//                  !SaveModifiedPrompt())                        // 0x264d40
//             AfxThrowOleException(OLE_E_PROMPTSAVECANCELLED);   // 0x25e200, 0x8004000c
//     }
//     if (m_pInPlaceFrame@0x250 != NULL) OnDeactivate();         // slot 92
//     BOOL bAutoDelete = m_bAutoDelete@0x120; m_bAutoDelete = FALSE;
//     OnCloseDocument();                                         // slot 35
//     m_bAutoDelete = bAutoDelete;
// The 0x120 = m_bAutoDelete identification is not a guess: COleServerDoc::
// OnCloseDocument (0x265420) reads that same slot at 0x2654d5 and uses it to
// decide whether to invoke the scalar-deleting destructor (vtable slot 1) on
// the document at 0x265517.  The save/clear/restore around OnCloseDocument is
// therefore load-bearing here too -- OpenMFC's CDocument::OnCloseDocument
// (core/doc/CDocument.cpp:83) also ends in `if (m_bAutoDelete) delete pThis;`,
// so removing the guard would free the document under the caller.
// Transcribed except for the m_bInPlaceCloseDeactivate guard and the in-place
// deactivation, neither of which OpenMFC models (see the offsets note above).
// The GetFirstFrame/IsWindowVisible override is kept as written; the
// COleDocument::GetFirstFrame thunk it goes through currently returns NULL,
// so the override does not fire yet and will start working on its own once
// that thunk is implemented.
// Symbol: ?OnClose@COleServerDoc@@MEAAXW4tagOLECLOSE@@@Z
extern "C" void MS_ABI impl__OnClose_COleServerDoc__MEAAXW4tagOLECLOSE___Z(
    COleServerDoc* pThis, unsigned long dwCloseOption)
{
    if (!pThis) return;
    // TODO(clean-room): retail returns early here when the unmodeled flag at
    // retail offset 0x244 is set (the one CanCloseFrame above sets/clears).
    CWnd* pFrame = reinterpret_cast<CWnd*>(
        impl__GetFirstFrame_COleDocument__QEAAPEAVCFrameWnd__XZ(pThis));
    if (pFrame != nullptr && ::IsWindowVisible(pFrame->m_hWnd))
        dwCloseOption = OLECLOSE_SAVEIFDIRTY;

    if (pThis->IsModified()) {
        if (dwCloseOption == OLECLOSE_SAVEIFDIRTY) {
            pThis->SaveEmbedding();
        } else if (dwCloseOption == OLECLOSE_PROMPTSAVE) {
            if (!impl__SaveModifiedPrompt_COleServerDoc__IEAAHXZ(pThis))
                impl__AfxThrowOleException__YAXJ_Z((long)0x8004000CL); // OLE_E_PROMPTSAVECANCELLED
        }
    }
    // TODO(clean-room): retail deactivates the in-place frame here
    // (m_pInPlaceFrame, retail offset 0x250); not modeled.
    int bAutoDelete = pThis->m_bAutoDelete;
    pThis->m_bAutoDelete = FALSE;
    pThis->OnCloseDocument();
    pThis->m_bAutoDelete = bAutoDelete;
}

// COleServerDoc::OnCloseDocument() — retail 0x265420 is the full server
// shutdown: it inspects four CDocument/COleDocument flags (retail offsets
// 0x128/0x12c/0x130 and 0x94) to decide whether to run at all, releases the
// in-place frame (0x250) and the moniker/registration state through
// COleLinkingDoc::UpdateVisibleLock (0x25d2b0) and
// COleLinkingDoc::OnCloseDocument (0x25d1f0), calls NotifyAllItems(OLE_CLOSED)
// (0x264950), releases m_lpClientSite@0x230, and finally
// ::CoDisconnectObject() on this document's IUnknown obtained through
// CCmdTarget::GetInterface.  None of that state (client site, in-place frame,
// registration lock) is modeled by OpenMFC and CCmdTarget::GetInterface is
// itself an unimplemented stub, so this is left as a no-op rather than a
// partially-correct shutdown.
// Symbol: ?OnCloseDocument@COleServerDoc@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_COleServerDoc__UEAAXXZ(COleServerDoc* pThis) {
    (void)pThis;
}

// COleServerDoc::OnDeactivate() — retail 0x266b30 operates entirely on
// m_pInPlaceFrame@0x250: it calls OnDeactivateUI (slot 93) when the frame's
// UI-active flag (frame+0x1d8) is set, tells the frame to tear down
// (frame vtable slot 0x3b8), frees the two CStrings the frame caches at
// +0x200/+0x208, calls DestroyInPlaceFrame(pFrame) (slot 100), clears the
// member, and finally QueryInterface's m_lpClientSite@0x230 for
// IID_IOleInPlaceSite to call OnInPlaceDeactivate (slot 11).  The in-place
// frame and client site are not modeled; left a no-op.
// Symbol: ?OnDeactivate@COleServerDoc@@UEAAXXZ
extern "C" void MS_ABI impl__OnDeactivate_COleServerDoc__UEAAXXZ(COleServerDoc* pThis) {
    (void)pThis;
}

// COleServerDoc::OnDeactivateUI(BOOL) — retail 0x266c10 returns immediately
// unless m_pInPlaceFrame@0x250 is non-null and UI-active (frame+0x1d8); the
// whole body then drives the frame's two shared menu/toolbar objects
// (frame+0x200/+0x208/+0x210/+0x218) and the doc-object server at +0x268.
// None of that is modeled; left a no-op.
// Symbol: ?OnDeactivateUI@COleServerDoc@@UEAAXH@Z
extern "C" void MS_ABI impl__OnDeactivateUI_COleServerDoc__UEAAXH_Z(
    COleServerDoc* pThis, int bUndoable) {
    (void)pThis; (void)bUndoable;
}

// COleServerDoc::OnDocWindowActivate(BOOL) — retail 0x266600 works on
// m_pInPlaceFrame@0x250 and the per-thread state returned by the module/thread
// state helper at 0x1801346a0 (it parks the frame in a thread-state slot and
// posts WM_ messages to frame->m_hWnd).  Neither the in-place frame nor that
// thread-state slot is modeled; left a no-op.
// Symbol: ?OnDocWindowActivate@COleServerDoc@@UEAAXH@Z
extern "C" void MS_ABI impl__OnDocWindowActivate_COleServerDoc__UEAAXH_Z(
    COleServerDoc* pThis, int bActivate) {
    (void)pThis; (void)bActivate;
}

// COleServerDoc::OnExecOleCmd(const GUID*, DWORD, DWORD, VARIANTARG*,
// VARIANTARG*) — DECODED (wave-4 review).  The previous note here said this
// export "has no entry in the RVA map ... no evidence for what it returns" and
// left the generated `return 0` (S_OK) standing.  That was wrong on both
// counts: the symbol is absent only from mfc140_rva_symbols.json, which is
// built from the MBCS mfc140.dll; it resolves in the Unicode image at
// mfc140u.dll RVA 0xf4d0, whose entire body is
//     mov  $0x80004001,%eax
//     ret
// i.e. `return E_NOTIMPL;`.  Returning S_OK claimed every OLE command had been
// executed successfully, which is the silent-misbehaviour case the campaign
// brief warns about, so it is corrected here.
// Symbol: ?OnExecOleCmd@COleServerDoc@@MEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__OnExecOleCmd_COleServerDoc__MEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    COleServerDoc* pThis, const GUID* pguidCmdGroup, unsigned long nCmdID,
    unsigned long nCmdExecOpt, void* pvarargIn, void* pvarargOut)
{
    (void)pThis; (void)pguidCmdGroup; (void)nCmdID; (void)nCmdExecOpt;
    (void)pvarargIn; (void)pvarargOut;
    return (long)0x80004001L;   // E_NOTIMPL — transcribed from mfc140u 0xf4d0
}

// COleServerDoc::OnFileSaveCopyAs() — retail 0x265240:
//     LPSTORAGE lpSave = m_lpRootStg@0x1c8;   m_lpRootStg = NULL;
//     m_bRemember@0x1d4 = FALSE;
//     TRY { DoSave(NULL, FALSE); }            // vtable slot 58
//     __finally { m_lpRootStg = lpSave; m_bRemember = TRUE; }
// m_lpRootStg is a COleDocument member OpenMFC does not model, so the
// save/restore of it is dropped; m_bRemember *is* modeled, and DoSave is a
// CDocument virtual, so the rest is transcribed as written.
// Symbol: ?OnFileSaveCopyAs@COleServerDoc@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSaveCopyAs_COleServerDoc__IEAAXXZ(COleServerDoc* pThis) {
    if (!pThis) return;
    // TODO(clean-room): retail also detaches and restores m_lpRootStg
    // (COleDocument, retail offset 0x1c8) around the save; not modeled.
    pThis->m_bRemember = FALSE;
    pThis->DoSave(nullptr, FALSE);
    pThis->m_bRemember = TRUE;
}

// COleServerDoc::OnFileUpdate() — retail 0x265200 is exactly two virtual
// calls on this:
//     UpdateModifiedFlag();   // vtable slot 75 -> COleDocument::UpdateModifiedFlag
//     OnUpdateDocument();     // vtable slot 85 -> COleServerDoc::OnUpdateDocument
// COleServerDoc does not declare either method in OpenMFC's headers, so both
// go through their impl__ thunks.  COleDocument::UpdateModifiedFlag is still
// an empty stub in core/ole/COleDocument.cpp; calling it keeps the call shape
// right and makes this function correct as soon as that stub is filled in.
// Symbol: ?OnFileUpdate@COleServerDoc@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileUpdate_COleServerDoc__IEAAXXZ(COleServerDoc* pThis) {
    if (!pThis) return;
    impl__UpdateModifiedFlag_COleDocument__UEAAXXZ(pThis);
    impl__OnUpdateDocument_COleServerDoc__UEAAHXZ(pThis);
}

// COleServerDoc::OnFrameWindowActivate(BOOL) — retail 0x266540 loads
// m_pInPlaceFrame@0x250, then reads *(AfxGetModuleThreadState()+0x8) (helper
// 0x1801346a0) and parks the frame in that object's +0x48 slot on activate /
// clears it on deactivate, driving the frame HWND (frame+0x40) with
// ::SendMessage (IAT 0x2c5378 = SendMessageA) using WM_NCACTIVATE (0x1c) and
// WM_ACTIVATE (0x06).  The in-place frame and that thread-state slot are not
// modeled; left a no-op.
// Symbol: ?OnFrameWindowActivate@COleServerDoc@@MEAAXH@Z
extern "C" void MS_ABI impl__OnFrameWindowActivate_COleServerDoc__MEAAXH_Z(
    COleServerDoc* pThis, int bActivate) {
    (void)pThis; (void)bActivate;
}

// COleServerDoc::OnGetLinkedItem(LPCTSTR) — retail 0x264bf0:
//     POSITION pos = GetStartPosition();                        // vtable slot 71
//     while ((pItem = GetNextItemOfKind(pos,
//                        RUNTIME_CLASS(COleServerItem))) != NULL)   // 0x2531e0
//         if (::lstrcmp(pItem->m_strItemName@0x58, lpszItemName) == 0)
//             return pItem;
//     return NULL;
// OpenMFC's COleServerItem declares no item-name member (see
// include/openmfc/afxole.h: m_pServerDoc, m_bAutoDelete and 136 bytes of
// padding, nothing else) and OpenMFC keeps the document's server items in the
// side table ServerDocState::items (phase4/src/detail/OlecoreSupport.h:953)
// rather than in COleDocument's item list, so the retail name comparison
// cannot be reproduced literally.  This forwards to GetLinkedServerItem, the
// lookup this port already uses for the same job (defined earlier in this
// file); that is an approximation of the retail match, not a transcription.
// READ THIS BEFORE RELYING ON IT: the two do not agree at the edges.  Retail
// returns NULL for a name that matches no item; GetLinkedServerItem falls back
// to the first item when the name is empty, and to the single item when the
// document holds exactly one, so it can hand back an item whose name was never
// asked for.  It also matches against the item's *document title*, not an item
// name.  Once COleServerItem grows a real name member this should become a
// literal transcription of 0x264bf0 and the fallbacks should go.
// Symbol: ?OnGetLinkedItem@COleServerDoc@@UEAAPEAVCOleServerItem@@PEB_W@Z
extern "C" COleServerItem* MS_ABI impl__OnGetLinkedItem_COleServerDoc__UEAAPEAVCOleServerItem__PEB_W_Z(
    COleServerDoc* pThis, const wchar_t* lpszItemName)
{
    if (!pThis) return nullptr;
    return pThis->GetLinkedServerItem(lpszItemName);
}

// COleServerDoc::OnNewEmbedding(IStorage*) — retail 0x265950:
//     saves a per-thread flag (state helper 0x1801345b0, +0x64),
//     DeleteContents()            // vtable slot 31
//     lpStorage->AddRef();  release m_lpRootStg@0x1c8; m_lpRootStg = lpStorage;
//     m_strPathName@0x48.Empty();  m_bEmbedded-ish flag @0x124 = 1;
//     if (OnNewDocument())        // vtable slot 32
//         { restore the thread flag; SetModifiedFlag(TRUE); ... }
// The whole point of the function is the m_lpRootStg handoff, and
// COleDocument::m_lpRootStg (retail offset 0x1c8) is not modeled by OpenMFC.
// Running only the DeleteContents/OnNewDocument half would leave the document
// with no root storage while claiming success, so this stays a no-op.
// Symbol: ?OnNewEmbedding@COleServerDoc@@MEAAXPEAUIStorage@@@Z
extern "C" void MS_ABI impl__OnNewEmbedding_COleServerDoc__MEAAXPEAUIStorage___Z(
    COleServerDoc* pThis, IStorage* lpStorage) {
    (void)pThis; (void)lpStorage;
}

// COleServerDoc::OnOpenEmbedding(IStorage*) — retail 0x265a10 has the same
// shape as OnNewEmbedding: DeleteContents() (slot 31), AddRef the storage,
// swap it into m_lpRootStg@0x1c8, then OnOpenDocument(NULL) (slot 33).
// Blocked on the same unmodeled m_lpRootStg; left a no-op.
// Symbol: ?OnOpenEmbedding@COleServerDoc@@MEAAXPEAUIStorage@@@Z
extern "C" void MS_ABI impl__OnOpenEmbedding_COleServerDoc__MEAAXPEAUIStorage___Z(
    COleServerDoc* pThis, IStorage* lpStorage) {
    (void)pThis; (void)lpStorage;
}

// COleServerDoc::OnReactivateAndUndo() — DECODED (wave-4 review).  The
// previous note here said "no entry in the RVA map ... documentation-derived,
// not disassembled".  It is absent only from the MBCS map; in mfc140u.dll it
// is at RVA 0x71e0 and its whole body is
//     xor  %eax,%eax
//     ret
// so `return FALSE;` is a literal transcription, not an inference.  (0x71e0 is
// a folded one-liner shared with other constant-FALSE bodies in the image; the
// export table nonetheless points this symbol at it, so this is exactly what a
// caller of the retail export observes.)
// Symbol: ?OnReactivateAndUndo@COleServerDoc@@MEAAHXZ
extern "C" int MS_ABI impl__OnReactivateAndUndo_COleServerDoc__MEAAHXZ(COleServerDoc* pThis) {
    (void)pThis;
    return FALSE;
}

// COleServerDoc::OnResizeBorder(LPCRECT, IOleInPlaceUIWindow*, BOOL) — retail
// 0x2668f0 forwards the border negotiation to m_pInPlaceFrame@0x250 and to the
// frame's control-bar list; without the in-place frame there is nothing to
// resize.  Left a no-op.
// Symbol: ?OnResizeBorder@COleServerDoc@@UEAAXPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" void MS_ABI impl__OnResizeBorder_COleServerDoc__UEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    COleServerDoc* pThis, const RECT* lpRectBorder, void* lpUIWindow, int bFrame) {
    (void)pThis; (void)lpRectBorder; (void)lpUIWindow; (void)bFrame;
}

// COleServerDoc::OnSaveDocument(LPCTSTR) — retail 0x265390:
//     BOOL bModified = IsModified();          // vtable slot 26
//     BOOL bRemember = m_bRemember@0x1d4;
//     if (!COleLinkingDoc::OnSaveDocument(lpszPathName))   // 0x25d160
//         return FALSE;
//     if (!bRemember)
//         SetModifiedFlag(bModified);         // vtable slot 27
//     if (lpszPathName != NULL && bRemember &&
//         AfxComparePath(m_strPathName@0x48, lpszPathName))   // 0x2271a0
//         NotifyAllItems(OLE_SAVED /*1*/, 0);                 // 0x264950
//     return TRUE;
// Transcribed, with one substitution: OpenMFC's
// COleLinkingDoc::OnSaveDocument is still an empty stub that returns FALSE
// (core/ole/COleLinkingDoc.cpp:77), which would make this function
// unconditionally fail, so the base call goes to CDocument::OnSaveDocument —
// the nearest base that is actually implemented here.  This mirrors what
// DeleteContents above already does.
// Argument order checked against the disassembly (wave-4): bModified is read
// from IsModified() and bRemember from +0x1d4 *before* the base call, and the
// AfxComparePath test uses m_strPathName@0x48 (OpenMFC: GetPathName()) as its
// first argument, so the sequence below is the retail one, not a plausible
// re-ordering of it.
// Symbol: ?OnSaveDocument@COleServerDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_COleServerDoc__UEAAHPEB_W_Z(
    COleServerDoc* pThis, const wchar_t* lpszPathName)
{
    if (!pThis) return FALSE;
    int bModified = pThis->IsModified();
    int bRemember = pThis->m_bRemember;
    if (!impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(pThis, lpszPathName))
        return FALSE;
    if (!bRemember)
        pThis->SetModifiedFlag(bModified);
    if (lpszPathName != nullptr && bRemember &&
        impl__AfxComparePath__YAHPEB_W0_Z(pThis->GetPathName(), lpszPathName)) {
        // OLE_SAVED == 1 in the OLE_NOTIFICATION enum; NotifyAllItems is
        // defined earlier in this file (still a no-op: the per-item notify
        // path is not modeled).
        impl__NotifyAllItems_COleServerDoc__IEAAXW4OLE_NOTIFICATION___K_Z(pThis, 1, 0);
    }
    return TRUE;
}

// COleServerDoc::OnSaveEmbedding(IStorage*) — retail 0x265ad0 temporarily
// swaps lpStorage into m_lpRootStg@0x1c8 (only when m_bCompoundFile@0x1d0 is
// clear), calls OnSaveDocument(NULL) (vtable slot 34), then restores the old
// storage.  Blocked on the unmodeled m_lpRootStg; left a no-op.
// Symbol: ?OnSaveEmbedding@COleServerDoc@@MEAAXPEAUIStorage@@@Z
extern "C" void MS_ABI impl__OnSaveEmbedding_COleServerDoc__MEAAXPEAUIStorage___Z(
    COleServerDoc* pThis, IStorage* lpStorage) {
    (void)pThis; (void)lpStorage;
}

// COleServerDoc::OnSetItemRects(LPCRECT, LPCRECT) — retail 0x266da0 is four
// instructions:
//     if (m_pInPlaceFrame@0x250 != NULL)
//         m_pInPlaceFrame->vtbl[0x3a0](lpPosRect, lpClipRect);   // slot 116
// m_pInPlaceFrame is not modeled, so the guard is always false and the whole
// function is a no-op here.
// Symbol: ?OnSetItemRects@COleServerDoc@@MEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__OnSetItemRects_COleServerDoc__MEAAXPEBUtagRECT__0_Z(
    COleServerDoc* pThis, const RECT* lpPosRect, const RECT* lpClipRect) {
    (void)pThis; (void)lpPosRect; (void)lpClipRect;
}

// COleServerDoc::OnShowControlBars(CFrameWnd*, BOOL) — retail 0x266850 walks
// the frame's control-bar list (pFrameWnd+0x130) and shows/hides each bar,
// consulting m_pInPlaceFrame@0x250 (+0x26c) to decide whether a bar may be
// shown.  The control-bar list walk and the in-place frame are not modeled;
// left a no-op.
// Symbol: ?OnShowControlBars@COleServerDoc@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnShowControlBars_COleServerDoc__UEAAXPEAVCFrameWnd__H_Z(
    COleServerDoc* pThis, CFrameWnd* pFrameWnd, int bShow) {
    (void)pThis; (void)pFrameWnd; (void)bShow;
}

// COleServerDoc::OnShowDocument(BOOL) — retail 0x265540 is the show/hide of
// the whole server UI: it reads the per-thread state (0x1801345b0), calls
// OnDeactivateUI (slot 92/93) when m_pInPlaceFrame@0x250 is set, then walks
// the document's frames via COleDocument::GetFirstFrame (0x253a50) to
// show/hide and activate them.  Neither the in-place frame nor the frame list
// is modeled here (COleDocument::GetFirstFrame is itself a stub returning
// NULL); left a no-op.
// Symbol: ?OnShowDocument@COleServerDoc@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShowDocument_COleServerDoc__MEAAXH_Z(
    COleServerDoc* pThis, int bShow) {
    (void)pThis; (void)bShow;
}

// COleServerDoc::OnUpdateDocument() — retail 0x2650a0, transcribed in full:
//     if (IsModified())        // vtable slot 26
//         SaveEmbedding();     // 0x265050
//     return TRUE;             // both paths return 1
// Symbol: ?OnUpdateDocument@COleServerDoc@@UEAAHXZ
extern "C" int MS_ABI impl__OnUpdateDocument_COleServerDoc__UEAAHXZ(COleServerDoc* pThis) {
    if (!pThis) return TRUE;
    if (pThis->IsModified())
        pThis->SaveEmbedding();
    return TRUE;
}

// COleServerDoc::RequestPositionChange(LPCRECT) — retail 0x266dd0:
//     LPOLEINPLACESITE pSite = QueryInterface(m_lpClientSite@0x230,
//                                             IID_IOleInPlaceSite);  // 0x26ba50
//     if (pSite != NULL) {
//         pSite->OnPosRectChange(lpPosRect);   // IOleInPlaceSite slot 14 (0x70)
//         pSite->Release();                    // slot 2 (0x10)
//     }
// m_lpClientSite is not modeled (nothing in OpenMFC ever stores one), so the
// site pointer is always NULL and the retail body reduces to doing nothing.
// Symbol: ?RequestPositionChange@COleServerDoc@@QEAAXPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__RequestPositionChange_COleServerDoc__QEAAXPEBUtagRECT___Z(
    COleServerDoc* pThis, const RECT* lpPosRect) {
    (void)pThis; (void)lpPosRect;
}

// COleServerDoc::SaveModified() — retail 0x265130:
//     if (m_lpClientSite@0x230 == NULL)
//         return COleDocument::SaveModified();          // tail jump to 0x2537d0
//     if (m_pInPlaceFrame@0x250 == NULL) {
//         UpdateModifiedFlag();                         // vtable slot 75
//         OnUpdateDocument();                           // vtable slot 85
//     }
//     return TRUE;
// OpenMFC never stores a client site, so the first branch is the one that
// always applies.  COleDocument::SaveModified is still an empty stub that
// returns FALSE (core/ole/COleDocument.cpp:311 — and FALSE here would mean
// "cancel the close"), so the call is routed to CDocument::SaveModified, the
// nearest base that is implemented — same substitution as in OnSaveDocument
// above.  Checked (wave-4) that this loses little: retail
// COleDocument::SaveModified (0x2537d0) is
//     for each COleClientItem in the doc: if in-place active, deactivate it;
//     UpdateModifiedFlag();                      // vtable slot 75
//     return CDocument::SaveModified();          // 0x219aa0
// and both of the parts skipped — the client-item walk and
// COleDocument::UpdateModifiedFlag — are themselves unmodeled or empty stubs
// in this port, so the substitution drops nothing that currently does work.
// Symbol: ?SaveModified@COleServerDoc@@MEAAHXZ
extern "C" int MS_ABI impl__SaveModified_COleServerDoc__MEAAHXZ(COleServerDoc* pThis) {
    if (!pThis) return TRUE;
    return impl__SaveModified_CDocument__UEAAHXZ(pThis);
}

// COleServerDoc::SaveModifiedPrompt() — retail 0x264d40:
//     if (m_lpClientSite@0x230 == NULL)
//         return SaveModified();                        // vtable slot 56
//     UpdateModifiedFlag();                             // vtable slot 75
//     if (!IsModified()) return TRUE;                   // vtable slot 26
//     CString prompt;
//     AfxFormatString1(prompt, 0xF187, m_strTitle@0x40);  // AfxFormatStrings 0x1cbc00
//     switch (AfxMessageBox(prompt, MB_YESNOCANCEL /*3*/, 0xF187)) { // 0x1ccb40
//     case IDCANCEL /*2*/: return FALSE;
//     case IDYES    /*6*/: OnUpdateDocument(); break;    // vtable slot 85
//     }
//     return TRUE;
// OpenMFC never stores a client site, so the first branch always applies and
// this reduces to the virtual SaveModified.  CORRECTION (wave-4): this used to
// call impl__SaveModified_COleServerDoc directly, which de-virtualises the
// retail slot-56 dispatch and would ignore an application subclass's
// SaveModified override.  It now goes through the C++ virtual, which is what
// retail does.  For a plain COleServerDoc that lands on CDocument::SaveModified
// either way, because OpenMFC's COleServerDoc does not override SaveModified in
// include/openmfc/afxole.h.  The prompt path is not reached and is not
// reproduced here.
// Symbol: ?SaveModifiedPrompt@COleServerDoc@@IEAAHXZ
extern "C" int MS_ABI impl__SaveModifiedPrompt_COleServerDoc__IEAAHXZ(COleServerDoc* pThis) {
    if (!pThis) return TRUE;
    return pThis->SaveModified();
}

// COleServerDoc::ScrollContainerBy(CSize) — retail 0x266e30:
//     LPOLEINPLACESITE pSite = QueryInterface(m_lpClientSite@0x230,
//                                             IID_IOleInPlaceSite);  // 0x26ba50
//     if (pSite == NULL) return FALSE;   // rax is 0 from the helper
//     BOOL b = SUCCEEDED(pSite->Scroll(sizeScroll));  // slot 9 (0x48)
//     pSite->Release();                               // slot 2 (0x10)
//     return b;
// m_lpClientSite is not modeled, so the no-site branch always applies and
// FALSE is exactly what retail would return in that state.
// CORRECTION (wave-4): an earlier note here claimed this port "has not
// established how MSVC passes CSize by value".  The disassembly does establish
// it — CSize is an 8-byte POD, so MSVC passes it whole in RDX, and retail
// forwards that same RDX verbatim as the by-value SIZE argument of
// IOleInPlaceSite::Scroll (`mov %rdx,%rbx` at 0x266e41, `mov %rbx,%rdx` at
// 0x266e5b).  The parameter is kept as an opaque pointer-sized scalar because
// that is register-identical to the real thing and this body never reads it.
// Symbol: ?ScrollContainerBy@COleServerDoc@@QEAAHVCSize@@@Z
extern "C" int MS_ABI impl__ScrollContainerBy_COleServerDoc__QEAAHVCSize___Z(
    COleServerDoc* pThis, void* /*CSize*/ sizeScroll) {
    (void)pThis; (void)sizeScroll;
    return FALSE;
}

// COleServerDoc::UpdateAllItems(COleServerItem*, LPARAM, CObject*, DVASPECT) —
// retail 0x2649c0:
//     POSITION pos = GetStartPosition();                       // vtable slot 71
//     while ((pItem = GetNextItemOfKind(pos,
//                        RUNTIME_CLASS(COleServerItem))) != NULL)  // 0x2531e0
//         if (pItem != pSender)
//             pItem->OnUpdate(pSender, lHint, pHint, nDrawAspect);
//                                          // COleServerItem vtable slot 24 (0xc0)
// OpenMFC's COleServerItem declares no OnUpdate (and no equivalent notify
// entry point), so the per-item call the loop exists to make cannot be
// delivered.  Left a no-op, for the same reason NotifyAllItems above is.
// Symbol: ?UpdateAllItems@COleServerDoc@@QEAAXPEAVCOleServerItem@@_JPEAVCObject@@W4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__UpdateAllItems_COleServerDoc__QEAAXPEAVCOleServerItem___JPEAVCObject__W4tagDVASPECT___Z(
    COleServerDoc* pThis, COleServerItem* pSender, __int64 lHint, CObject* pHint,
    int nDrawAspect)
{
    (void)pThis; (void)pSender; (void)lHint; (void)pHint; (void)nDrawAspect;
}

//=============================================================================
// COleServerDoc nested COM interface implementations (XOleObject,
// XOleInPlaceObject, XOleInPlaceActiveObject, XDataObject, XPersistStorage)
//
// All of these except GetCanonicalFormatEtc were disassembled.  Every one of
// them begins with the MSVC METHOD_PROLOGUE, which recovers the COleServerDoc
// from the address of a nested interface sub-object embedded inside it: the
// sub-object offsets read out of the prologues are +0x278 for XOleObject
// (`lea -0x278(%rcx),%rbx`, 0x267550) and +0x288 for XOleInPlaceObject
// (`add $-0x288,%rcx`, 0x268610).  XOleInPlaceActiveObject's methods index off
// their own pointer instead of materialising `this` (0x268890 reads
// -0x258(%rcx) for the module state at doc+0x38 and -0x40(%rcx) for
// m_pInPlaceFrame at doc+0x250, which places that sub-object at +0x290).
// Having recovered the document they reach their real work through
// CCmdTarget::GetInterface(&IID_...) (retail 0x26bc00), usually delegating to
// the embedded COleServerItem returned by GetEmbeddedItem (retail 0x2647f0).
// Two fully-decoded examples, so the shape is on record:
//   EnumVerbs   (0x267940): pThis->GetInterface(&IID_IOleObject)
//                           ->GetUserClassID(&clsid)      // IOleObject slot 15
//                           then ::OleRegEnumVerbs(clsid, ppenumOleVerb).
//   GetMiscStatus (0x267ed0): same GetUserClassID, then
//                           ::OleRegGetMiscStatus(clsid, dwAspect, pdwStatus).
//   GetClientSite (0x267420): *ppClientSite = m_lpClientSite@0x230; returns
//                           E_INVALIDARG for a NULL out pointer and E_FAIL
//                           when there is no client site.
//
// OpenMFC models none of the machinery these depend on: COleServerDoc has no
// nested interface sub-objects (so no caller can ever legally reach these
// entry points, and the -0x278 style `this` adjustment would point at
// unrelated memory), there is no interface map, and
// CCmdTarget::GetInterface is itself an unimplemented stub returning NULL
// (phase4/src/core/runtime/CCmdTarget.cpp:340).  Implementing any of them
// would mean inventing a layout OpenMFC does not have, so they are left as
// generated.  The `this` parameter is spelled out below only to document the
// real calling convention; the bodies do not dereference it.
//=============================================================================

// Symbol: ?Advise@XOleObject@COleServerDoc@@UEAAJPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XOleObject_COleServerDoc__UEAAJPEAUIAdviseSink__PEAK_Z(void* pThisItf, void* pAdvSink, unsigned long* pdwConnection) {
    (void)pThisItf; (void)pAdvSink; (void)pdwConnection;
    return 0;
}

// Symbol: ?Close@XOleObject@COleServerDoc@@UEAAJK@Z
extern "C" long MS_ABI impl__Close_XOleObject_COleServerDoc__UEAAJK_Z(void* pThisItf, unsigned long dwSaveOption) {
    (void)pThisItf; (void)dwSaveOption;
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleInPlaceActiveObject@COleServerDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceActiveObject_COleServerDoc__UEAAJH_Z(void* pThisItf, int fEnterMode) {
    (void)pThisItf; (void)fEnterMode;
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleInPlaceObject@COleServerDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceObject_COleServerDoc__UEAAJH_Z(void* pThisItf, int fEnterMode) {
    (void)pThisItf; (void)fEnterMode;
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(void* pThisItf, void* pFormatetc, unsigned long advf, void* pAdvSink, unsigned long* pdwConnection) {
    (void)pThisItf; (void)pFormatetc; (void)advf; (void)pAdvSink; (void)pdwConnection;
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleServerDoc@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleServerDoc__UEAAJK_Z(void* pThisItf, unsigned long dwConnection) {
    (void)pThisItf; (void)dwConnection;
    return 0;
}

// Symbol: ?DoVerb@XOleObject@COleServerDoc@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__DoVerb_XOleObject_COleServerDoc__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(void* pThisItf, long iVerb, void* lpmsg, void* pActiveSite, long lindex, void* hwndParent, const RECT* lprcPosRect) {
    (void)pThisItf; (void)iVerb; (void)lpmsg; (void)pActiveSite; (void)lindex;
    (void)hwndParent; (void)lprcPosRect;
    return 0;
}

// Symbol: ?EnableModeless@XOleInPlaceActiveObject@COleServerDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XOleInPlaceActiveObject_COleServerDoc__UEAAJH_Z(void* pThisItf, int fEnable) {
    (void)pThisItf; (void)fEnable;
    return 0;
}

// Symbol: ?EnumAdvise@XOleObject@COleServerDoc@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumSTATDATA___Z(void* pThisItf, void** ppenumAdvise) {
    (void)pThisItf; (void)ppenumAdvise;
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleServerDoc@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleServerDoc__UEAAJPEAPEAUIEnumSTATDATA___Z(void* pThisItf, void** ppenumAdvise) {
    (void)pThisItf; (void)ppenumAdvise;
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleServerDoc@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleServerDoc__UEAAJKPEAPEAUIEnumFORMATETC___Z(void* pThisItf, unsigned long dwDirection, void** ppenumFormatEtc) {
    (void)pThisItf; (void)dwDirection; (void)ppenumFormatEtc;
    return 0;
}

// Symbol: ?EnumVerbs@XOleObject@COleServerDoc@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumOLEVERB___Z(void* pThisItf, void** ppenumOleVerb) {
    (void)pThisItf; (void)ppenumOleVerb;
    return 0;
}

// COleServerDoc::XDataObject::GetCanonicalFormatEtc — DECODED (wave-4 review).
// The previous note said it "has no entry in the RVA map ... was not
// disassembled at all"; again that is only true of the MBCS map.  In
// mfc140u.dll it is at RVA 0x253c70 and the whole body is
//     mov  $0x40130,%eax
//     ret
// i.e. `return DATA_S_SAMEFORMATETC;`, with pformatetcOut left untouched.
// Alone among the nested-interface methods in this block it never dereferences
// its interface pointer, so it needs neither the +0x278-style sub-object
// layout nor CCmdTarget::GetInterface and can be transcribed exactly.  The
// generated `return 0` (S_OK) was wrong: S_OK means "pformatetcOut has been
// filled with a *different*, canonical FORMATETC", which this body never does.
// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__0_Z(void* pThisItf, void* pformatectIn, void* pformatetcOut) {
    (void)pThisItf; (void)pformatectIn; (void)pformatetcOut;
    return (long)0x00040130L;   // DATA_S_SAMEFORMATETC — mfc140u 0x253c70
}

// Symbol: ?GetClassID@XPersistStorage@COleServerDoc@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistStorage_COleServerDoc__UEAAJPEAU_GUID___Z(void* pThisItf, GUID* pClassID) {
    (void)pThisItf; (void)pClassID;
    return 0;
}

// Symbol: ?GetClientSite@XOleObject@COleServerDoc@@UEAAJPEAPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__GetClientSite_XOleObject_COleServerDoc__UEAAJPEAPEAUIOleClientSite___Z(void* pThisItf, void** ppClientSite) {
    (void)pThisItf; (void)ppClientSite;
    return 0;
}

// Symbol: ?GetClipboardData@XOleObject@COleServerDoc@@UEAAJKPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_COleServerDoc__UEAAJKPEAPEAUIDataObject___Z(void* pThisItf, unsigned long dwReserved, void** ppDataObject) {
    (void)pThisItf; (void)dwReserved; (void)ppDataObject;
    return 0;
}

// Symbol: ?GetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* pThisItf, void* pformatetcIn, void* pmedium) {
    (void)pThisItf; (void)pformatetcIn; (void)pmedium;
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* pThisItf, void* pformatetc, void* pmedium) {
    (void)pThisItf; (void)pformatetc; (void)pmedium;
    return 0;
}

// Symbol: ?GetExtent@XOleObject@COleServerDoc@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(void* pThisItf, unsigned long dwDrawAspect, SIZE* psizel) {
    (void)pThisItf; (void)dwDrawAspect; (void)psizel;
    return 0;
}

// Symbol: ?GetMiscStatus@XOleObject@COleServerDoc@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_COleServerDoc__UEAAJKPEAK_Z(void* pThisItf, unsigned long dwAspect, unsigned long* pdwStatus) {
    (void)pThisItf; (void)dwAspect; (void)pdwStatus;
    return 0;
}

// Symbol: ?GetMoniker@XOleObject@COleServerDoc@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleObject_COleServerDoc__UEAAJKKPEAPEAUIMoniker___Z(unsigned long p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetUserClassID@XOleObject@COleServerDoc@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_COleServerDoc__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetUserType@XOleObject@COleServerDoc@@UEAAJKPEAPEA_W@Z
extern "C" long MS_ABI impl__GetUserType_XOleObject_COleServerDoc__UEAAJKPEAPEA_W_Z(unsigned long p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?GetWindow@XOleInPlaceActiveObject@COleServerDoc@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceActiveObject_COleServerDoc__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleInPlaceObject@COleServerDoc@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceObject_COleServerDoc__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?HandsOffStorage@XPersistStorage@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__HandsOffStorage_XPersistStorage_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?InPlaceDeactivate@XOleInPlaceObject@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__InPlaceDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?InitFromData@XOleObject@COleServerDoc@@UEAAJPEAUIDataObject@@HK@Z
extern "C" long MS_ABI impl__InitFromData_XOleObject_COleServerDoc__UEAAJPEAUIDataObject__HK_Z(void* /*struct*/* p0, int p1, unsigned long p2) {
    return 0;
}

// Symbol: ?InitNew@XPersistStorage@COleServerDoc@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__InitNew_XPersistStorage_COleServerDoc__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?IsDirty@XPersistStorage@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__IsDirty_XPersistStorage_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsUpToDate@XOleObject@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?Load@XPersistStorage@COleServerDoc@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__Load_XPersistStorage_COleServerDoc__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDocWindowActivate@XOleInPlaceActiveObject@COleServerDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_XOleInPlaceActiveObject_COleServerDoc__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnFrameWindowActivate@XOleInPlaceActiveObject@COleServerDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_XOleInPlaceActiveObject_COleServerDoc__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ReactivateAndUndo@XOleInPlaceObject@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__ReactivateAndUndo_XOleInPlaceObject_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?ResizeBorder@XOleInPlaceActiveObject@COleServerDoc@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_XOleInPlaceActiveObject_COleServerDoc__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(const void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?Save@XPersistStorage@COleServerDoc@@UEAAJPEAUIStorage@@H@Z
extern "C" long MS_ABI impl__Save_XPersistStorage_COleServerDoc__UEAAJPEAUIStorage__H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?SaveCompleted@XPersistStorage@COleServerDoc@@UEAAJPEAUIStorage@@@Z
extern "C" long MS_ABI impl__SaveCompleted_XPersistStorage_COleServerDoc__UEAAJPEAUIStorage___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetClientSite@XOleObject@COleServerDoc@@UEAAJPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__SetClientSite_XOleObject_COleServerDoc__UEAAJPEAUIOleClientSite___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetColorScheme@XOleObject@COleServerDoc@@UEAAJPEAUtagLOGPALETTE@@@Z
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_COleServerDoc__UEAAJPEAUtagLOGPALETTE___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?SetExtent@XOleObject@COleServerDoc@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?SetHostNames@XOleObject@COleServerDoc@@UEAAJPEB_W0@Z
extern "C" long MS_ABI impl__SetHostNames_XOleObject_COleServerDoc__UEAAJPEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetMoniker@XOleObject@COleServerDoc@@UEAAJKPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__SetMoniker_XOleObject_COleServerDoc__UEAAJKPEAUIMoniker___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?SetObjectRects@XOleInPlaceObject@COleServerDoc@@UEAAJPEBUtagRECT@@0@Z
extern "C" long MS_ABI impl__SetObjectRects_XOleInPlaceObject_COleServerDoc__UEAAJPEBUtagRECT__0_Z(const void* /*struct*/* p0, const void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleInPlaceActiveObject@COleServerDoc@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleInPlaceActiveObject_COleServerDoc__UEAAJPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?UIDeactivate@XOleInPlaceObject@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__UIDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?Unadvise@XOleObject@COleServerDoc@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XOleObject_COleServerDoc__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?Update@XOleObject@COleServerDoc@@UEAAJXZ
extern "C" long MS_ABI impl__Update_XOleObject_COleServerDoc__UEAAJXZ() {
    return 0;
}
