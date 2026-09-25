// COleServerDocEx — OpenMFC implementation.
//
// COleServerDocEx (atlmfc/include/afxoleserverdocex.h:28) is the feature-pack
// OLE server document: COleServerDoc plus overrides that drive the
// COleCntrFrameWndEx container frame's CDockingManager.  It adds NO data
// members -- the retail ctor/dtor below only chain to COleServerDoc and store
// the class vftable, and classCOleServerDocEx.m_nObjectSize is 0x298, the same
// as COleServerDoc's.
//
// Every body was transcribed from the retail disassembly, except the
// container-frame branch of OnResizeBorder, which is written out in a comment
// and deliberately NOT executed (see that function).  RVAs are cited for
// BOTH images where they were read: "mfc140u" is the Unicode DLL OpenMFC
// reimplements (entry RVAs taken from its export table by ordinal); "mfc140" is
// the ANSI twin that disas.py opens.  The bodies are byte-identical; the
// addresses are not interchangeable.
//
// Retail layout touched here:
//   COleServerDoc  +0x250  COleIPFrameWnd* m_pInPlaceFrame
//       (COleServerDoc ctor, mfc140 0x264560, zeroes it: `mov %rax,0x250(%rbx)`;
//        core/ole/COleServerDoc.cpp documents the same offset.)  OpenMFC's
//       COleServerDoc keeps that word inside its zero-filled
//       _coleserverdoc_padding; core/ole/COleServerDoc.cpp static_asserts that.
//   COleCntrFrameWndEx +0x1e0  CDockingManager m_dockManager
//       (see the layout comment at the top of core/ole/COleCntrFrameWndEx.cpp).
//
// vftable.  Retail stores its own COleServerDocEx vftable (mfc140u 0x1802f1ed8,
// mfc140 0x1802efe28) in the ctor and dtor.  OpenMFC declares no
// COleServerDocEx class in include/openmfc/ and so builds no vtable for it;
// the object keeps the vfptr the COleServerDoc constructor thunk installs.  The
// consequence is spelled out at the ctor.
//
// Virtual calls on the embedded CDockingManager.  Retail calls
// CDockingManager::ShowPanes through that object's vtable (+0xb0, slot 22 of
// the retail CDockingManager vftable, mfc140 0x1802e4070).  OpenMFC
// placement-constructs its own CDockingManager at COleCntrFrameWndEx+0x1e0,
// whose vtable is not MSVC-shaped, so -- following core/ole/COleCntrFrameWndEx.cpp
// -- the call is made directly to the CDockingManager export thunk.  The
// sub-object's dynamic type is always exactly CDockingManager (it is an
// embedded member, not a heap object a client could subclass), so the direct
// call resolves to the same body the virtual call would.

// No openmfc/ header is included: pulling afxwin.h/afxole.h into this unit
// drags in C++ symbols that exist only as impl__ thunks (checkfile.sh link
// audit).  sizeof(COleServerDoc) == 0x298 and the +0x250 placement are
// static_asserted in core/ole/COleServerDoc.cpp.
#include <windows.h>
#include <cstddef>

struct CRuntimeClass;   // used only through pointers

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (each definition was read before declaring it) ----
// operator new (detail/MemcoreSupport.cpp)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// CObject::IsKindOf (core/runtime/CObject.cpp)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const void* pThis, const CRuntimeClass* pClass);
// AfxThrowInvalidArgException (detail/MfcExceptionsSupport.cpp)
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// RUNTIME_CLASS getters (core/ole/RuntimeClasses.cpp)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ();
// GetContainerFrameWindow (core/ole/COleIPFrameWndEx.cpp, core/ole/COleDocIPFrameWndEx.cpp).
// Both exports resolve to the same folded body (mfc140u 0x95e70).
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(void* pThis);
extern "C" void* MS_ABI impl__GetContainerFrameWindow_COleDocIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(void* pThis);
// CDockingManager::ShowPanes(BOOL) (featurepack/docking/CDockingManager.cpp).
// NOTE: that definition still carries an auto-generated placeholder parameter
// list `(int p0)` and returns 0.  This declaration is the one the mangled name
// ?ShowPanes@CDockingManager@@UEAAHH@Z describes (this + BOOL), identical to
// the one core/frame/CFrameWndEx.cpp already calls through.  Until that
// definition is repaired the call is a harmless no-op returning FALSE.
extern "C" int MS_ABI impl__ShowPanes_CDockingManager__UEAAHH_Z(void* pThis, int bShow);
// COleServerDoc base implementations (core/ole/Thunks.cpp, core/ole/COleServerDoc.cpp)
extern "C" void* MS_ABI impl___0COleServerDoc__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1COleServerDoc__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnDocWindowActivate_COleServerDoc__UEAAXH_Z(void* pThis, int bActivate);
extern "C" void MS_ABI impl__OnShowControlBars_COleServerDoc__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pFrameWnd, int bShow);
extern "C" void MS_ABI impl__OnResizeBorder_COleServerDoc__UEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    void* pThis, const RECT* lpRectBorder, void* lpUIWindow, int bFrame);
// COleLinkingDoc::OnNewDocument (core/ole/COleLinkingDoc.cpp)
extern "C" int MS_ABI impl__OnNewDocument_COleLinkingDoc__UEAAHXZ(void* pThis);
// COleDocument::Serialize (core/ole/COleDocument.cpp); CArchive& is one pointer
extern "C" void MS_ABI impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(void* pThis, void* ar);

// ---- forward declarations for this file's own exports ----
extern "C" void* MS_ABI impl___0COleServerDocEx__IEAA_XZ(void* pThis);

namespace {

constexpr std::size_t kSize_COleServerDocEx      = 0x298;  // retail classCOleServerDocEx.m_nObjectSize
                                                          // (descriptor mfc140 0x1802f0178) and the
                                                          // `mov $0x298,%ecx` in CreateObject
constexpr std::size_t kOff_m_pInPlaceFrame       = 0x250;  // COleServerDoc::m_pInPlaceFrame
constexpr std::size_t kOff_COleCntrFrameWndEx_m_dockManager = 0x1e0;  // `lea 0x1e0(%rbx),%rcx`


inline void* InPlaceFrame(void* pDoc) {
    return *reinterpret_cast<void**>(static_cast<char*>(pDoc) + kOff_m_pInPlaceFrame);
}
inline void* DockManagerOf(void* pCntrFrameEx) {
    return static_cast<char*>(pCntrFrameEx) + kOff_COleCntrFrameWndEx_m_dockManager;
}
inline bool IsKindOf(void* pObj, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, pClass) != 0;
}

// The container-frame lookup shared by OnDocWindowActivate and OnResizeBorder.
// Both retail bodies contain the identical sequence (mfc140 0x96ca1..0x96cf1
// in OnDocWindowActivate, 0x969bb..0x96a13 in OnResizeBorder, where it follows
// the lpUIWindow and bFrame tests):
//     p = m_pInPlaceFrame;
//     if (p && p->IsKindOf(RUNTIME_CLASS(COleDocIPFrameWndEx)))
//         return p->GetContainerFrameWindow();          // mfc140u 0x95e70 (mfc140 0x96430)
//     p = m_pInPlaceFrame;                              // re-read
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(COleIPFrameWndEx)))
//         -> caller returns immediately, doing nothing else;
//     return p->GetContainerFrameWindow();              // mfc140u 0x95e70 (mfc140 0x96430)
// `*pbHaveFrame` reports whether the caller got past the early-return test.
void* ContainerFrameOf(void* pDoc, bool* pbHaveFrame) {
    *pbHaveFrame = true;
    void* p = InPlaceFrame(pDoc);
    if (p && IsKindOf(p, impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ()))
        return impl__GetContainerFrameWindow_COleDocIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(p);
    p = InPlaceFrame(pDoc);
    if (!p || !IsKindOf(p, impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        *pbHaveFrame = false;
        return nullptr;
    }
    return impl__GetContainerFrameWindow_COleIPFrameWndEx__QEAAPEAVCOleCntrFrameWndEx__XZ(p);
}

}  // namespace

// COleServerDocEx::CreateObject — retail mfc140u 0x96290 (mfc140 0x96850):
//     p = operator new(0x298);             // ??2@YAPEAX_K@Z, mfc140u 0x27f0 (mfc140 0x2840)
//     return p ? new(p) COleServerDocEx : NULL;   // ctor, mfc140u 0x962e0 (mfc140 0x968a0)
// Symbol: ?CreateObject@COleServerDocEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COleServerDocEx__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSize_COleServerDocEx);
    if (p == nullptr) return nullptr;
    return impl___0COleServerDocEx__IEAA_XZ(p);
}

// COleServerDocEx::OnDocWindowActivate(BOOL) — retail mfc140u 0x966d0
// (mfc140 0x96c90), transcribed in full:
//     if (!bActivate) {
//         <container-frame lookup, see ContainerFrameOf>   // may return early
//         if (pCntr == NULL) return;                       // mfc140 insn 0x96cf4
//         pCntr->m_dockManager.ShowPanes(FALSE);           // vtable +0xb0, edx = 0
//     }
//     COleServerDoc::OnDocWindowActivate(bActivate);       // mfc140u 0x267840 (mfc140 0x266600)
// Note the early returns: on deactivation with no in-place frame of either
// kind, or no container frame, retail does NOT call the base implementation.
// Deviation: ShowPanes is called directly, not through the vtable (see the
// file header).  OpenMFC's GetContainerFrameWindow thunks are still stubs
// returning NULL, so today every deactivation returns early (at the IsKindOf
// test or at the NULL test) without reaching ShowPanes or the base call.
// Symbol: ?OnDocWindowActivate@COleServerDocEx@@MEAAXH@Z
extern "C" void MS_ABI impl__OnDocWindowActivate_COleServerDocEx__MEAAXH_Z(void* pThis, int bActivate) {
    if (!bActivate) {
        bool bHaveFrame;
        void* pCntr = ContainerFrameOf(pThis, &bHaveFrame);
        if (!bHaveFrame || pCntr == nullptr) return;
        (void)impl__ShowPanes_CDockingManager__UEAAHH_Z(DockManagerOf(pCntr), FALSE);
    }
    impl__OnDocWindowActivate_COleServerDoc__UEAAXH_Z(pThis, bActivate);
}

// COleServerDocEx::OnGetEmbeddedItem() — the mfc140u export table maps this
// symbol (ordinal 10004) to RVA 0x71e0, a two-instruction body shared by every
// COMDAT-folded "return 0" function (the symbol map names it
// ?AddRef@COleUILinkInfo@@):
//     xor %eax,%eax
//     ret
// Cross-check: slot 86 (+0x2b0) of the retail COleServerDocEx vftable (mfc140
// 0x1802efe28) -- the slot of COleServerDoc's pure OnGetEmbeddedItem, between
// OnUpdateDocument (85) and OnClose (87) -- points at the same folded body
// (mfc140 0x7260).  So the retail override returns NULL; that is the whole body.
// Symbol: ?OnGetEmbeddedItem@COleServerDocEx@@MEAAPEAVCOleServerItem@@XZ
extern "C" void* MS_ABI impl__OnGetEmbeddedItem_COleServerDocEx__MEAAPEAVCOleServerItem__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// COleServerDocEx::OnNewDocument() — retail mfc140u 0x96360 (mfc140 0x96920):
//     return COleLinkingDoc::OnNewDocument() != 0;   // mfc140u 0x25e150 (mfc140 0x25d060), setne
// A fixed-address (non-virtual) base call, with the result normalised to 0/1.
// It lands on COleLinkingDoc's body because COleServerDoc does not override
// OnNewDocument (atlmfc afxole.h declares it on COleDocument and
// COleLinkingDoc only), so COleServerDoc::OnNewDocument IS COleLinkingDoc's.
// Symbol: ?OnNewDocument@COleServerDocEx@@MEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_COleServerDocEx__MEAAHXZ(void* pThis) {
    return impl__OnNewDocument_COleLinkingDoc__UEAAHXZ(pThis) != 0 ? TRUE : FALSE;
}

// COleServerDocEx::OnResizeBorder(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL) —
// retail mfc140u 0x963b0 (mfc140 0x96970).  PARTIAL transcription.
// Retail control flow (bare hex = mfc140 instruction addresses):
//     if (lpUIWindow == NULL) AfxThrowInvalidArgException();       // 0x969a6 -> 0x96c22
//     if (!bFrame) { COleServerDoc::OnResizeBorder(...); return; } // 0x969af -> mfc140u 0x267b30 (mfc140 0x2668f0)
//     <container-frame lookup, see ContainerFrameOf>               // may return early
//     if (pCntr == NULL) {                                         // 0x96a19
//         pDocIP = m_pInPlaceFrame, kept only if IsKindOf(COleDocIPFrameWnd);
//         pIP    = m_pInPlaceFrame, kept only if IsKindOf(COleIPFrameWnd);
//         if (pDocIP == NULL && pIP == NULL) return;
//         COleServerDoc::OnResizeBorder(lpRectBorder, lpUIWindow, bFrame);   // jmp 0x969b1
//         return;
//     }
//     ... container-frame branch (0x96a7c..0x96bfc), NOT implemented here:
//     rect = lpRectBorder ? *lpRectBorder (CopyRect)
//                         : lpUIWindow->GetBorder(&rect) [IOleInPlaceUIWindow +0x28],
//            and on GetBorder failure lpUIWindow->SetBorderSpace(NULL) [+0x38] and return;
//     if (AfxGetModuleThreadState()->m_pCurrentWinThread->m_pActiveWnd == m_pInPlaceFrame)
//         this->OnShowControlBars(pCntr, TRUE);                  // vtable +0x2f8
//     pCntr->m_dockManager.m_rectInPlace (+0x14) = rect;
//     pCntr->m_dockManager.AdjustDockingLayout(NULL);            // vtable +0x80
//     border = labs() of each edge of m_dockManager.m_rectClientAreaBounds (+0x1c8) minus rect;
//     if (border is all zero && lpUIWindow->RequestBorderSpace(&border) [+0x30] fails) {
//         this->OnShowControlBars(pCntr, FALSE);
//         lpUIWindow->SetBorderSpace(&emptyRect);
//     } else {
//         lpUIWindow->SetBorderSpace(&border);
//         pCntr->m_dockManager.AdjustDockingLayout(NULL);
//     }
// Why the container branch is missing: it reads and writes the retail
// CDockingManager members m_rectInPlace (+0x14) and m_rectClientAreaBounds
// (+0x1c8).  OpenMFC's CDockingManager at COleCntrFrameWndEx+0x1e0 is a
// 144-byte object with a different layout, so +0x14 lands inside its own
// members and +0x1c8 past its end.  DEVIATION: on that branch this body
// returns without doing anything.  It is unreachable today because OpenMFC's
// GetContainerFrameWindow thunks are stubs returning NULL.
// Symbol: ?OnResizeBorder@COleServerDocEx@@MEAAXPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" void MS_ABI impl__OnResizeBorder_COleServerDocEx__MEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    void* pThis, const RECT* lpRectBorder, void* lpUIWindow, int bFrame) {
    if (lpUIWindow == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (!bFrame) {
        impl__OnResizeBorder_COleServerDoc__UEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
            pThis, lpRectBorder, lpUIWindow, bFrame);
        return;
    }
    bool bHaveFrame;
    void* pCntr = ContainerFrameOf(pThis, &bHaveFrame);
    if (!bHaveFrame) return;
    if (pCntr == nullptr) {
        void* pDocIP = InPlaceFrame(pThis);
        if (pDocIP && !IsKindOf(pDocIP, impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ()))
            pDocIP = nullptr;
        void* pIP = InPlaceFrame(pThis);
        if (pIP && !IsKindOf(pIP, impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ()))
            pIP = nullptr;
        if (pDocIP == nullptr && pIP == nullptr) return;
        impl__OnResizeBorder_COleServerDoc__UEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
            pThis, lpRectBorder, lpUIWindow, bFrame);
        return;
    }
    // TODO(clean-room): container-frame branch (see above) needs the retail
    // CDockingManager m_rectInPlace / m_rectClientAreaBounds members.
}

// COleServerDocEx::OnShowControlBars(CFrameWnd*, BOOL) — retail mfc140u
// 0x96670 (mfc140 0x96c30), transcribed in full:
//     COleServerDoc::OnShowControlBars(pFrameWnd, bShow);   // unconditional; mfc140u 0x267a90 (mfc140 0x266850)
//     if (pFrameWnd && pFrameWnd->IsKindOf(RUNTIME_CLASS(COleCntrFrameWndEx)))
//         pFrameWnd->m_dockManager.ShowPanes(bShow);         // +0x1e0, vtable +0xb0
// (Retail also null-tests &m_dockManager, which cannot be NULL for a non-NULL
// frame; that test is omitted.)  Deviation: ShowPanes is called directly, not
// through the vtable (see the file header).
// Symbol: ?OnShowControlBars@COleServerDocEx@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnShowControlBars_COleServerDocEx__MEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pFrameWnd, int bShow) {
    impl__OnShowControlBars_COleServerDoc__UEAAXPEAVCFrameWnd__H_Z(pThis, pFrameWnd, bShow);
    if (pFrameWnd && IsKindOf(pFrameWnd, impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ()))
        (void)impl__ShowPanes_CDockingManager__UEAAHH_Z(DockManagerOf(pFrameWnd), bShow);
}

// COleServerDocEx::Serialize(CArchive&) — retail mfc140u 0x963a0 (mfc140
// 0x96960) is a single tail jump:
//     jmp COleDocument::Serialize              // mfc140u 0x254390 (mfc140 0x253460)
// Symbol: ?Serialize@COleServerDocEx@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_COleServerDocEx__UEAAXAEAVCArchive___Z(void* pThis, void* ar) {
    impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(pThis, ar);
}

// COleServerDocEx::COleServerDocEx() — retail mfc140u 0x962e0 (mfc140 0x968a0):
//     COleServerDoc::COleServerDoc();          // mfc140u 0x2657b0 (mfc140 0x264560)
//     this->vfptr = &COleServerDocEx::`vftable'   // mfc140u 0x1802f1ed8
//     return this;
// DEVIATION: the vftable store is not reproduced -- OpenMFC has no
// COleServerDocEx vtable (the class is not declared in include/openmfc/), so
// the object keeps the vfptr the COleServerDoc constructor installs.  Virtual
// calls on it therefore reach COleServerDoc's behaviour, not the overrides in
// this file, and a virtual GetRuntimeClass reports COleServerDoc.
// Symbol: ??0COleServerDocEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleServerDocEx__IEAA_XZ(void* pThis) {
    impl___0COleServerDoc__QEAA_XZ(pThis);
    return pThis;
}

// COleServerDocEx::~COleServerDocEx() — retail mfc140u 0x96380 (mfc140 0x96940):
//     this->vfptr = &COleServerDocEx::`vftable'   // mfc140u 0x1802f1ed8
//     jmp COleServerDoc::~COleServerDoc           // mfc140u 0x265870 (mfc140 0x264620)
// The vftable store is omitted for the same reason as in the constructor.
// Symbol: ??1COleServerDocEx@@UEAA@XZ
extern "C" void MS_ABI impl___1COleServerDocEx__UEAA_XZ(void* pThis) {
    impl___1COleServerDoc__UEAA_XZ(pThis);
}
