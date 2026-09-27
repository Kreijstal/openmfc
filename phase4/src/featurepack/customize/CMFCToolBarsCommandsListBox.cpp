// CMFCToolBarsCommandsListBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ===========================================================================
// CMFCToolBarsCommandsListBox -- the owner-draw command list on the
// "Commands" page of the feature-pack Customize sheet
// (afxtoolbarscommandslistbox.h, derives CListBox).  Its one own member is
// CSize m_sizeButton.  The class is NOT declared in OpenMFC's public headers,
// so S_CmdsListBox below is the only place its layout lives here (the same
// block is mirrored as S_CmdsListBox in CMFCToolBarsCommandsPropertyPage.cpp,
// which embeds it at +0x240).
//
// Bodies were transcribed from the retail disassembly (the method in the
// header of core/ole/COleControl.cpp).  All RVAs and absolute addresses in
// this file are mfc140u unless marked "ANSI".  The ctor, dtor, DrawItem and
// MeasureItem resolve through disas.py --u.  The other three have no mfc140u
// entry in the RVA map; they resolve only in mfc140.dll (ANSI), and all four
// resolvable exports sit at ANSI RVA + 0x19b0 in mfc140u (ctor 0x174d70 ->
// 0x176720, dtor 0x174df0 -> 0x1767a0, MeasureItem 0x174f90 -> 0x176940,
// DrawItem 0x174fc0 -> 0x176970).  The three mfc140u entries below were
// located at that delta, their instruction streams compared against the ANSI
// bodies, and independently confirmed from retail's own tables:
//   * the class vftable 0x180318cc8 (installed by the ctor) holds
//     PreSubclassWindow 0x176ab0 in slot 22 and GetMessageMap 0x1767b0 in
//     slot 12;
//   * that returns the AFX_MSGMAP at VA 0x180318c50, whose entries are
//     WM_LBUTTONDOWN (0x201, sig 0x36) -> 0x1767c0 and WM_ERASEBKGND (0x14,
//     sig 0x1) -> 0x176af0, then the terminator.
//
//   ctor         0x176720            OnLButtonDown      0x1767c0 (ANSI 0x174e10)
//   dtor         0x1767a0            OnEraseBkgnd       0x176af0 (ANSI 0x175140)
//   MeasureItem  0x176940            PreSubclassWindow  0x176ab0 (ANSI 0x175100)
//   DrawItem     0x176970
//
// Import slots named below were resolved with iatu.py against mfc140u.dll:
// 0x1802c7120 = USER32!SendMessageW, 0x1802c7218 = USER32!CopyRect,
// 0x1802c7330 = USER32!GetClientRect, 0x1802c72f8 = USER32!PtInRect,
// 0x1802c71a8 = USER32!LoadCursorW, 0x1802c71e0 = USER32!SetCursor.
// 0x1802c7b30 is not an import; it is the pointer every virtual call in
// these bodies is made through (the target is loaded into %rax first).
//
// Layout (retail): CListBox (a bare CWnd, sizeof 0xe8) then m_sizeButton at
// +0xe8, written by the ctor as one 8-byte zero store and read back by
// MeasureItem (+0xec, cy) and written by PreSubclassWindow (+0xe8 / +0xec).
//
// Structural deviations, applied uniformly:
//
//  (1) vtable pointers.  Retail's ctor and dtor store the class vftable
//      0x180318cc8 at +0x00.  OpenMFC has no MSVC-layout vftable for this
//      class, and its CListBox vtable cannot be named here without a C++
//      symbol reference (the impl__ thunk rule).  As in the sibling
//      CMFCToolBarsListCheckBox.cpp, the ctor builds the CWnd part with the
//      exported CWnd ctor thunk and keeps the vptr it installs; the dtor
//      re-stamps that captured vptr (s_cwndVptr) before tearing down the
//      CListBox base, so the base destructor dispatches on OpenMFC's vtable
//      even when an MSVC client-derived destructor has stamped its own.
//
//  (2) CMFCVisualManager::GetInstance().  It is header-inline in MFC and not
//      exported; retail calls an out-of-line, non-exported copy of it at
//      mfc140u 0x9774 (ANSI 0x97f4).  That helper returns
//      ?m_pVisManager@CMFCVisualManager@@ (0x3be3c0) when it is non-NULL;
//      otherwise it creates the manager -- m_pRTIDefault (0x3be3b8)
//      ->CreateObject() when a default class is set, else
//      new CMFCVisualManager(FALSE) (0x184030) -- stores it in m_pVisManager,
//      sets its +0x104 flag to 1 and calls its vslot 14 (+0x70) before
//      returning it.  OpenMFC's equivalent is the C++ static
//      CMFCVisualManager::GetInstance() defined in
//      visualmanager/CMFCVisualManager.cpp (a few ribbon files call it
//      directly), but this workflow's per-file link audit rejects any new
//      C++ symbol reference, so -- as controls/CMFCHeaderCtrl.cpp and
//      docking/CPaneDivider.cpp do -- the exported pointer is read and the
//      visual-manager call is skipped while it is NULL (no lazy creation).
//      The call itself is vslot 80 (+0x280); slot 80 of the CMFCVisualManager
//      vftable 0x18031c128 is 0x187320, the base OnFillCommandsListBackground
//      (byte-identical to the ANSI export at 0x185930 apart from RIP-relative
//      displacements).  OpenMFC's CMFCVisualManager declares no such C++
//      virtual, so the call goes to that base export thunk
//      (visualmanager/CMFCVisualManager.cpp, a concrete body): a derived
//      manager's override (Office2003 / OfficeXP / VS2008 / Windows have
//      exports) is NOT honoured.
//
//  (3) Button virtuals cannot be dispatched.  DrawItem and OnLButtonDown call
//      CMFCToolBarButton virtuals (OnDrawOnCustomizeList, vslot 29 +0xe8;
//      PrepareDrag, vslot 5 +0x28).  An OpenMFC button's vptr is the mingw
//      (Itanium) vtable -- OpenMFC_PatchToolBarButtonVtable has no caller, see
//      visualmanager/CMFCVisualManagerOfficeXP.cpp -- so those slots cannot be
//      read, and the base-class exports in toolbar/CMFCToolBarButton.cpp are
//      still `return 0` placeholders with parameter lists that do not match
//      their mangled names.  Both handlers are therefore left stubbed, with
//      the retail body recorded above each.
//
//  (4) Message routing.  OnLButtonDown / OnEraseBkgnd are reached in retail
//      through this class's message map.  OpenMFC's map for it
//      (classCMFCToolBarsCommandsListBox_msgmap,
//      detail/Toolbar22MsgmapSupport.cpp) has no entries of its own, and the
//      object's vptr is OpenMFC's CWnd vtable (deviation (1)), so nothing in
//      OpenMFC reaches these handlers or the MeasureItem / DrawItem /
//      PreSubclassWindow overrides today; they run only when called through
//      their exports.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the definitions in the tree
// (file named on each line).
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                      // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);                              // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                              // detail/MfcExceptionsSupport.cpp
extern "C" CSize* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(CSize* pRet);        // featurepack/toolbar/CMFCToolBar.cpp
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManager__UEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bIsSelected);                            // featurepack/visualmanager/CMFCVisualManager.cpp
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                               // core/runtime/StaticData.cpp

namespace {

struct alignas(8) S_CmdsListBox {
    unsigned char base[0xe8];   // CWnd / CListBox part (OpenMFC sizeof(CWnd) == 0xe8, asserted below)
    int           cxButton;     // +0xe8 m_sizeButton.cx
    int           cyButton;     // +0xec m_sizeButton.cy
};
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CListBox) == 0xe8,
              "retail CMFCToolBarsCommandsListBox's own member starts at +0xe8 (ctor's store after ??0CWnd)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(offsetof(S_CmdsListBox, cxButton) == 0xe8, "ctor 0x176720: movq $0x0,0xe8(%rbx); PreSubclassWindow: mov %eax,0xe8(%rbx)");
static_assert(offsetof(S_CmdsListBox, cyButton) == 0xec, "MeasureItem 0x176940: mov 0xec(%rcx),%eax");
static_assert(sizeof(S_CmdsListBox) == 0xf0, "CMFCToolBarsCommandsPropertyPage embeds it as 0xf0 bytes (+0x240..+0x330)");
static_assert(offsetof(MEASUREITEMSTRUCT, itemHeight) == 0x10, "MeasureItem: 0x10(%rdx)");

inline S_CmdsListBox* D(void* pThis) { return static_cast<S_CmdsListBox*>(pThis); }
inline HWND Hwnd(const void* p) { return static_cast<const CWnd*>(p)->m_hWnd; }

// The vptr impl___0CWnd__QEAA_XZ installs; see deviation (1).
void* s_cwndVptr = nullptr;

} // namespace

// Symbol: ??0CMFCToolBarsCommandsListBox@@QEAA@XZ
// Retail 0x176720 (mfc140u), fully transcribed:
//     CWnd::CWnd();                        // 0x28a700 (CListBox() inlined)
//     vptr = 0x180318cc8;                  // class vftable
//     m_sizeButton = CSize(0, 0);          // movq $0x0,0xe8(%rbx)
//     return this;
// Deviation (1) for the vftable store.  This export was not on this file's
// work list (its previous body only returned pThis), but the destructor below
// tears down the CListBox base, which requires the CWnd part the ctor builds.
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsListBox__QEAA_XZ(void* pThis) {
    impl___0CWnd__QEAA_XZ(pThis);
    std::memcpy(&s_cwndVptr, pThis, sizeof s_cwndVptr);
    S_CmdsListBox* d = D(pThis);
    d->cxButton = 0;
    d->cyButton = 0;
    return pThis;
}

// Symbol: ??1CMFCToolBarsCommandsListBox@@UEAA@XZ
// Retail 0x1767a0 (mfc140u), fully transcribed:
//     vptr = 0x180318cc8;
//     tail-jump ??1CListBox@@UEAA@XZ (0x293f30).
// Deviation (1): the re-stamped vptr is the one the CWnd ctor thunk installed.
extern "C" void MS_ABI impl___1CMFCToolBarsCommandsListBox__UEAA_XZ(void* pThis) {
    if (s_cwndVptr != nullptr)
        std::memcpy(pThis, &s_cwndVptr, sizeof s_cwndVptr);
    impl___1CListBox__UEAA_XZ(static_cast<CListBox*>(static_cast<CWnd*>(pThis)));
}

// Symbol: ?DrawItem@CMFCToolBarsCommandsListBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// STUB -- deviation (3).  Retail 0x176970 (mfc140u):
//     CDC* pDC = CDC::FromHandle(lpDIS->hDC);                  // 0x2a2450, +0x20
//     CRect rect; ::CopyRect(&rect, &lpDIS->rcItem);            // IAT 0x1802c7218, +0x28
//     if (lpDIS->itemID == (UINT)-1) return;                    // +0x08
//     CMFCToolBarButton* pButton = (CMFCToolBarButton*)
//         ::SendMessage(m_hWnd, LB_GETITEMDATA /*0x199*/, (int)lpDIS->itemID, 0);
//     CString strText = pButton->m_strText;                     // +0x38, CloneData 0xdd40
//     GetText(lpDIS->itemID, pButton->m_strText);               // 0x294030
//     CMFCVisualManager::GetInstance()                          // 0x9774
//         ->OnFillCommandsListBackground(pDC, rect, FALSE);     // vslot 80 (+0x280), result unused
//     pButton->OnDrawOnCustomizeList(pDC, rect,                 // button vslot 29 (+0xe8)
//         (lpDIS->itemState & (ODS_SELECTED | ODS_FOCUS)) == (ODS_SELECTED | ODS_FOCUS));   // +0x10, and $0x11
//     pButton->m_strText = strText;                             // 0xde30, then strText released
// The button draw -- the part that paints each command's image and text --
// cannot be reached (deviation (3)), so the body is not implemented; painting
// only the background would leave every item blank.
extern "C" void MS_ABI impl__DrawItem_CMFCToolBarsCommandsListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS) {
    (void)pThis;
    (void)lpDIS;
}

// Symbol: ?MeasureItem@CMFCToolBarsCommandsListBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
// Retail 0x176940 (mfc140u), fully transcribed:
//     ENSURE(lpMIS != NULL);                  // NULL -> AfxThrowInvalidArgException (0x227720)
//     if (lpMIS->itemHeight < (UINT)m_sizeButton.cy)   // unsigned compare (jae), +0x10 vs +0xec
//         lpMIS->itemHeight = m_sizeButton.cy;
extern "C" void MS_ABI impl__MeasureItem_CMFCToolBarsCommandsListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, MEASUREITEMSTRUCT* lpMIS) {
    if (lpMIS == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const UINT cy = static_cast<UINT>(D(pThis)->cyButton);
    if (lpMIS->itemHeight < cy)
        lpMIS->itemHeight = cy;
}

// Symbol: ?OnEraseBkgnd@CMFCToolBarsCommandsListBox@@IEAAHPEAVCDC@@@Z
// Retail 0x176af0 (mfc140u), fully transcribed:
//     CRect rectClient(0, 0, 0, 0);
//     ::GetClientRect(m_hWnd, &rectClient);                     // IAT 0x1802c7330
//     CMFCVisualManager::GetInstance()                          // 0x9774 (deviation 2)
//         ->OnFillCommandsListBackground(pDC, rectClient, FALSE);   // vslot 80 (+0x280); CRect by value, result unused
//     return TRUE;
// Deviation (2): the manager is the exported pointer (call skipped while it
// is NULL) and the fill goes to the base-class export.
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolBarsCommandsListBox__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    RECT rc = {0, 0, 0, 0};
    ::GetClientRect(Hwnd(pThis), &rc);
    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVM != nullptr) {
        impl__OnFillCommandsListBackground_CMFCVisualManager__UEAAKPEAVCDC__VCRect__H_Z(pVM, pDC, CRect(rc), FALSE);
    }
    return TRUE;
}

// Symbol: ?OnLButtonDown@CMFCToolBarsCommandsListBox@@IEAAXIVCPoint@@@Z
// STUB -- deviation (3).  Retail 0x1767c0 (mfc140u):
//     CWnd::Default();                                          // 0x28ac80
//     int iIndex = (int)::SendMessage(m_hWnd, LB_GETCURSEL /*0x188*/, 0, 0);
//     if (iIndex == -1) return;
//     CRect rect(0, 0, 0, 0);
//     ::SendMessage(m_hWnd, LB_GETITEMRECT /*0x198*/, iIndex, (LPARAM)&rect);
//     if (!::PtInRect(&rect, point)) return;                    // IAT 0x1802c72f8
//     ::SendMessage(m_hWnd, WM_LBUTTONUP /*0x202*/, nFlags, MAKELPARAM(point.x, point.y));
//     CMFCToolBarButton* pButton = (CMFCToolBarButton*)
//         ::SendMessage(m_hWnd, LB_GETITEMDATA /*0x199*/, iIndex, 0);
//     COleDataSource srcItem;                                   // 0x252f90
//     pButton->m_bDragFromCollection = TRUE;                    // +0x20
//     pButton->PrepareDrag(srcItem);                            // button vslot 5 (+0x28)
//     pButton->m_bDragFromCollection = FALSE;
//     ::SetCursor(AfxGetApp()->LoadCursor(IDC_AFXBARRES_DELETE /*16133*/));
//         // AfxGetModuleState 0x133930 (result unused: CWinApp::LoadCursor is
//         // inline), AfxFindResourceHandle(0x3f05, RT_GROUP_CURSOR) 0x2aeb50,
//         // IAT LoadCursorW 0x1802c71a8, SetCursor 0x1802c71e0
//     srcItem.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE, &rect,
//                        &CMFCToolBar::m_DropSource);           // 0x25a690; static 0x1803c2510
//     // ~COleDataSource                                        // 0x252fd0
// PrepareDrag -- which fills the drag data -- cannot be reached (deviation
// (3)), so the body is not implemented.
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBarsCommandsListBox__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// Symbol: ?PreSubclassWindow@CMFCToolBarsCommandsListBox@@MEAAXXZ
// Retail 0x176ab0 (mfc140u), fully transcribed:
//     CSize sizeMenuImage = CMFCToolBar::GetMenuImageSize();    // 0x157400
//     m_sizeButton = CSize(sizeMenuImage.cx + 6, sizeMenuImage.cy + 6);   // +0xe8 / +0xec
// Retail does not call the base CListBox::PreSubclassWindow.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCToolBarsCommandsListBox__MEAAXXZ(void* pThis) {
    CSize sizeMenuImage(0, 0);
    impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(&sizeMenuImage);
    S_CmdsListBox* d = D(pThis);
    d->cxButton = sizeMenuImage.cx + 6;
    d->cyButton = sizeMenuImage.cy + 6;
}
