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

// COleClientItem::ReportError(int) const — retail (0x26c4c0). Maps the given
// HRESULT to a prompt resource and shows it with a message box; returns
// TRUE when a prompt was shown, FALSE otherwise.
//   E_NOTIMPL(0x80004001) / E_NOINTERFACE(0x80004002) / E_FAIL(0x80004005)
//     -> AFX_IDP_OLE_INIT_FAILED (0xF181)
//   OLE_E_STATIC(0x8004000B) -> AFX_IDP_OLE_CANT_INSERT (0xF180)
//   E_OUTOFMEMORY(0x8007000E) -> AFX_IDP_E_OUTOFMEMORY (0xF10A)
// The message box uses nType = MB_OK(0) and nIDHelp = -1, exactly as retail
// passes them (rdx=0, r8d=-1).
// Symbol: ?ReportError@COleClientItem@@UEBAHJ@Z
extern "C" int MS_ABI impl__ReportError_COleClientItem__UEBAHJ_Z(
    const COleClientItem* pThis, long nCode)
{
    (void)pThis;
    if (nCode == static_cast<long>(0x80004001) || nCode == static_cast<long>(0x80004002) ||
        nCode == static_cast<long>(0x80004005)) {
        impl__AfxMessageBox__YAHIII_Z(0xF181, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    if (nCode == static_cast<long>(0x8004000B) /* OLE_E_STATIC */) {
        impl__AfxMessageBox__YAHIII_Z(0xF180, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    if (nCode == static_cast<long>(0x8007000E) /* E_OUTOFMEMORY */) {
        impl__AfxMessageBox__YAHIII_Z(0xF10A, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    return FALSE;
}

// COleClientItem::Run() — retail (0x24af10):
//     if (m_lpObject) {
//         if (!OleIsRunning(m_lpObject)) {
//             hr = OleRun(m_lpObject);
//             OnRun(hr);                 // virtual, vtable slot 0x1A0
//         }
//     }
// The OnRun notification virtual is not declared in OpenMFC's header, so the
// notification step is skipped (TODO below).
// Symbol: ?Run@COleClientItem@@QEAAXXZ
extern "C" void MS_ABI impl__Run_COleClientItem__QEAAXXZ(COleClientItem* pThis)
{
    if (!pThis || !pThis->m_lpObject) return;
    if (!OleIsRunning(pThis->m_lpObject)) {
        HRESULT hr = OleRun(pThis->m_lpObject);
        // TODO(clean-room): transcribed partially. Retail dispatches the
        // virtual OnRun(HRESULT) (COleClientItem vtable slot 0x1A0) so derived
        // classes can react to the run result; OnRun is not modeled in the
        // OpenMFC header.
        (void)hr;
    }
}

// COleClientItem::SetDrawAspect(DVASPECT) — retail (0x246c20):
//     if (nDrawAspect == DVASPECT_ICON) {
//         // ensure the icon metafile is cached: render via IOleItemContainer
//         // (helpers 0x246760 / 0x246850) and release a temporary
//         // TYMED_MFPICT medium (ReleaseStgMedium).
//     }
//     m_nDrawAspect = nDrawAspect;
//     GetDocument()->SetModifiedFlag();
// Symbol: ?SetDrawAspect@COleClientItem@@UEAAXW4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(
    COleClientItem* pThis, int nDrawAspect)
{
    if (!pThis) return;
    if (nDrawAspect == 4 /* DVASPECT_ICON */ && !pThis->GetIconicMetafile()) {
        // TODO(clean-room): transcribed partially. Retail renders and caches
        // the icon metafile through the object's IOleItemContainer render
        // machinery (retail helpers 0x246760/0x246850), then releases a
        // temporary TYMED_MFPICT medium. That render path is not modeled;
        // an absent icon is created lazily by the caller of GetIconicMetafile.
    }
    pThis->m_nDrawAspect = nDrawAspect;
    if (pThis->m_pContainerDoc) {
        pThis->m_pContainerDoc->SetModifiedFlag(TRUE);
    }
}

// COleClientItem::SetItemRects(const RECT*, const RECT*) — retail (0x249140):
//     if (m_lpObject) {
//         pIPO = QueryInterface(m_lpObject, IID_IOleInPlaceObject);
//         if (pIPO) {
//             if (lpPosRect) rcPos = *lpPosRect; else OnGetItemPosition(rcPos);
//             if (lpClipRect) rcClip = *lpClipRect; else <OnGetClipRect virtual, slot 0x120>;
//             hr = pIPO->SetObjectRects(&rcPos, &rcClip);
//             m_nStatus = hr;
//             pIPO->Release();
//             return SUCCEEDED(hr);
//         }
//     }
//     return FALSE;
// The clip-rect default virtual (OnGetClipRect) is not declared in the header;
// its documented action is IOleInPlaceObject::GetClipRect, which is used here.
// Symbol: ?SetItemRects@COleClientItem@@QEAAHPEBUtagRECT@@0@Z
extern "C" int MS_ABI impl__SetItemRects_COleClientItem__QEAAHPEBUtagRECT__0_Z(
    COleClientItem* pThis, const RECT* lpPosRect, const RECT* lpClipRect)
{
    if (!pThis || !pThis->m_lpObject) return FALSE;
    IOleInPlaceObject* pInPlace = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(
            IID_IOleInPlaceObject, reinterpret_cast<void**>(&pInPlace))) ||
        !pInPlace) {
        return FALSE;
    }
    CRect rcPos;
    if (lpPosRect) {
        rcPos = *lpPosRect;
    } else {
        pThis->OnGetItemPosition(rcPos);
    }
    CRect rcClip;
    if (lpClipRect) {
        rcClip = *lpClipRect;
    } else {
        // Retail dispatches the virtual OnGetClipRect (vtable slot 0x120),
        // which queries the object for IID_IOleInPlaceActiveObject and calls
        // its GetClipRect extension. Neither that virtual nor the extension
        // exists in OpenMFC/MinGW headers, so the position rect is reused
        // (a conservative, type-correct stand-in).
        rcClip = rcPos;
    }
    HRESULT hr = pInPlace->SetObjectRects(rcPos, rcClip);
    pThis->m_nStatus = (OLE_STATUS)hr;
    pInPlace->Release();
    return SUCCEEDED(hr);
}

// COleClientItem::SetLinkUpdateOptions(OLEUPDATE) — retail (0x24b240):
//     pLink = QueryInterface(m_lpObject, IID_IOleLink);
//     if (pLink) {
//         hr = pLink->SetUpdateOptions(nUpdateOptions);
//         pLink->Release();
//         OnRun(hr);                   // virtual, vtable slot 0x1A0
//     }
// The OnRun notification virtual is not declared in OpenMFC's header.
// Symbol: ?SetLinkUpdateOptions@COleClientItem@@QEAAXW4tagOLEUPDATE@@@Z
extern "C" void MS_ABI impl__SetLinkUpdateOptions_COleClientItem__QEAAXW4tagOLEUPDATE___Z(
    COleClientItem* pThis, int nUpdateOptions)
{
    if (!pThis || !pThis->m_lpObject) return;
    IOleLink* pLink = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(
            IID_IOleLink, reinterpret_cast<void**>(&pLink))) || !pLink) {
        return;
    }
    HRESULT hr = pLink->SetUpdateOptions(
        static_cast<OLEUPDATE>(nUpdateOptions));
    pLink->Release();
    // TODO(clean-room): transcribed partially. Retail then dispatches the
    // virtual OnRun(HRESULT) (vtable slot 0x1A0); OnRun is not modeled in the
    // OpenMFC header.
    (void)hr;
}

// COleClientItem::SetPrintDevice(const DVTARGETDEVICE*) — retail (0x24acd0).
// The retail body wraps the target device in an IStream (helper 0x246c90) and
// hands it to the item's storage / IOleItemContainer machinery so the object
// can render for printing. That state is not modeled, so a conservative
// failure is emitted (safe, type-correct).
// Symbol: ?SetPrintDevice@COleClientItem@@QEAAHPEBUtagDVTARGETDEVICE@@@Z
extern "C" int MS_ABI impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagDVTARGETDEVICE___Z(
    COleClientItem* pThis, const tagDVTARGETDEVICE* ptd)
{
    (void)ptd;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail serializes the target
    // device into a stream (CreateStreamOnHGlobal) and passes it through the
    // item's unmodeled storage state; OpenMFC cannot reproduce that path.
    return FALSE;
}

// COleClientItem::SetPrintDevice(const PRINTDLG*) — retail (0x24ae30):
//     ptd = (pPD ? BuildTargetDevice(pPD) : NULL);   // helper 0x2605d8
//     bResult = SetPrintDevice(ptd);
//     if (ptd) CoTaskMemFree(ptd);
//     return bResult;
// Both the DVTARGETDEVICE builder and the ptd overload are unimplementable
// without the unmodeled print/stream state, so the wrapper fails too.
// Symbol: ?SetPrintDevice@COleClientItem@@QEAAHPEBUtagPDW@@@Z
extern "C" int MS_ABI impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagPDW___Z(
    COleClientItem* pThis, const tagPDW* pPD)
{
    (void)pPD;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail converts the PRINTDLG
    // (hDevMode/hDevNames) into a DVTARGETDEVICE (helper 0x2605d8) and
    // forwards to SetPrintDevice(const DVTARGETDEVICE*); see that stub.
    return FALSE;
}

// COleClientItem::UpdateItemType() — retail (0x244b20) stores the resolved
// item type into the unmodeled member at offset 0x94:
//     if (QueryInterface(m_lpObject, IID_IOleLink)) { m_nItemType = 1; pLink->Release(); }
//     else if (SUCCEEDED(m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &status)) &&
//              !(status & OLEMISC_STATIC)) m_nItemType = 2;
//     else m_nItemType = 3;
// OpenMFC does not model m_nItemType, so no state can be updated.
// Symbol: ?UpdateItemType@COleClientItem@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateItemType_COleClientItem__QEAAXXZ(
    COleClientItem* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail sets m_nItemType@0x94 to
    // otLink(1) / otEmbedded(2) / otStatic(3) after querying the object for
    // IOleLink and OLEMISC_STATIC; the member is not modeled in OpenMFC.
}

// COleClientItem::WriteItem(CArchive&) — retail (0x2460a0) dispatches on the
// container document's compound-file flag:
//     if (GetDocument()->m_bCompoundFile) WriteItemCompound(ar);
//     else                               WriteItemFlat(ar);
// Symbol: ?WriteItem@COleClientItem@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItem_COleClientItem__UEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar)
{
    if (!pThis) return;
    if (pThis->m_pContainerDoc && pThis->m_pContainerDoc->m_bCompoundFile) {
        impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    } else {
        impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    }
}

// COleClientItem::WriteItemCompound(CArchive&) — retail (0x245e60). The retail
// body saves the item into a sub-storage of the archive's compound file using
// the item's unmodeled storage state (retail offsets 0x68/0x70) and the
// archive's IStorage; it also forwards to WriteItemFlat. OpenMFC models neither
// the storage members nor the archive storage, so this is a no-op.
// Symbol: ?WriteItemCompound@COleClientItem@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail writes the item into a
    // compound-document sub-storage (item storage members at retail 0x68/0x70
    // and CArchive's IStorage are not modeled in OpenMFC).
}

// COleClientItem::WriteItemFlat(CArchive&) — retail (0x245910). The retail
// body writes the item's CLSID (via IPersist::GetClassID) and its lock-object
// / storage state through the unmodeled members at retail offsets 0x68/0x70
// and CArchive internals. Not reproducible in OpenMFC, so a no-op is emitted.
// Symbol: ?WriteItemFlat@COleClientItem@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail queries the object for
    // IPersist, writes the class id and the item's storage/stream state into
    // the archive; those members and the archive internals are not modeled.
}

//=============================================================================
// COleServerDoc
//=============================================================================
//
// The retail COleServerDoc carries its in-place frame, doc-frame, item rects,
// and doc-object server inside the region that OpenMFC leaves as padding (only
// m_bEmbedded is declared). Functions below that depend on that region return
// safe defaults and are marked TODO(clean-room).

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
