// COleDocIPFrameWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, then the retail disassembly.
//
// Every body below marked "retail" was transcribed from the retail
// disassembly, the method phase4/src/core/ole/COleControl.cpp describes.
//
// WHICH IMAGE.  Every RVA in this file is an mfc140u.dll (Unicode) RVA unless
// it says otherwise.  Entries were resolved by ordinal through the mfc140u
// export table (mfc_complete_ordinal_mapping.json, urva.py) and disassembled
// with `disas.py --u --at`:
//   CreateObject 0x256850   ??0 ctor 0x256890   ??1 dtor 0x2568c0
//   OnRequestPositionChange 0x2568d0   RecalcLayout 0x256910
//   BuildSharedMenu 0x256b10   DestroySharedMenu 0x256c00
// Two of them are COMDAT-folded with COleIPFrameWnd's identical bodies: the
// mfc140u export table gives ?OnRequestPositionChange@COleIPFrameWnd@@ the
// same RVA 0x2568d0 and ?DestroySharedMenu@COleIPFrameWnd@@ the same RVA
// 0x256c00.  (mfc140.dll's symbol map attaches the COleIPFrameWnd names to
// the ANSI copies, 0x255990 and 0x255cc0 (mfc140), which is why a plain
// `disas.py` on the COleDocIPFrameWnd names reports NOT FOUND.)
//
// VFTABLE.  The class vftable is at mfc140u RVA 0x32ed10 (the ctor loads it at
// 0x25689e; the dtor re-plants it at 0x2568c0).  Dumped with vtdump_u.py:
//   28 +0x0e0 CWnd::CalcWindowRect (0x28edc0)
//   93 +0x2e8 CFrameWnd::GetActiveDocument (0x29ede0)
//   96 +0x300 RecalcLayout (0x256910, this file)
//  118 +0x3b0 BuildSharedMenu (0x256b10)   119 +0x3b8 DestroySharedMenu (0x256c00)
//  120 +0x3c0 COleIPFrameWnd::GetInPlaceMenu (0x25db60, not overridden)
//  121 +0x3c8 OnRequestPositionChange (0x2568d0)
// Virtual calls go through the object's own vftable by MSVC slot index, as
// retail does and as core/ole/COleIPFrameWnd.cpp does for the same slots.
// CAVEAT (shared with that file): an object built by OpenMFC itself
// (CreateObject below) carries the g++ vftable of OpenMFC's C++
// COleIPFrameWnd, whose slot indices are not retail's.
//
// LAYOUT.  OpenMFC's headers only forward-declare COleDocIPFrameWnd
// (include/openmfc/afxole.h:528).  Retail afxdocob.h:287 (atlmfc 14.51)
// declares it as COleIPFrameWnd plus overrides only -- no data members -- and
// retail's sizeof is COleIPFrameWnd's 0x280 (CreateObject's
// `mov $0x280,%ecx`, 0x256854).  So `this` is taken as void* and every member
// is a retail COleIPFrameWnd member, addressed at the retail offsets that
// core/ole/COleIPFrameWnd.cpp pins (its RetailIPFrameTail lives in an
// anonymous namespace there, so the members this file touches are re-pinned
// below against the same instructions).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// ---- sibling impl__ exports called by the bodies in this file ----
// core/ole/Thunks.cpp:1158 (placement-new of OpenMFC's C++ COleIPFrameWnd)
extern "C" void* MS_ABI impl___0COleIPFrameWnd__QEAA_XZ(void* pThis);
// core/ole/COleIPFrameWnd.cpp (same retail body, RVA 0x256c00, see above)
extern "C" void MS_ABI impl__DestroySharedMenu_COleIPFrameWnd__MEAAXXZ(void* pThis);
// core/ole/COleServerDoc.cpp
extern "C" void MS_ABI impl__RequestPositionChange_COleServerDoc__QEAAXPEBUtagRECT___Z(
    COleServerDoc* pThis, const RECT* lpPosRect);
// detail/MemcoreSupport.cpp (the DLL's exported MFC operator new, ??2@YAPEAX_K@Z)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void* lpLayout, HWND hWnd, const RECT* lpRect);
// core/window/CWnd.cpp, core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" void MS_ABI impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
    CWnd* pThis, unsigned int nIDFirst, unsigned int nIDLast, unsigned int nIDLeftOver,
    unsigned int nFlag, RECT* lpRectParam, const RECT* lpRectClient, int bStretch);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);

namespace {

constexpr size_t kSizeof_COleDocIPFrameWnd = 0x280;   // CreateObject 0x256854 `mov $0x280,%ecx`

// ---- retail COleIPFrameWnd members touched here (retail afxole.h:1468-1483) --
// Each offset is followed by the mfc140u instruction in THIS class's bodies
// that establishes it; the same offsets are pinned in core/ole/COleIPFrameWnd.cpp.
constexpr size_t kOff_m_rectPos        = 0x248;   // RecalcLayout `movups 0x248(%rbx),%xmm0`
constexpr size_t kOff_m_rectClip       = 0x258;   // RecalcLayout `lea 0x258(%rbx),%r8` (IntersectRect)
constexpr size_t kOff_m_hMenuHelpPopup = 0x270;   // ctor 0x2568a5 `movq $0x0,0x270(%rbx)`;
                                                  // BuildSharedMenu 0x256bd1 `mov %rax,0x270(%rbx)`

static_assert(sizeof(COleIPFrameWnd) == kSizeof_COleDocIPFrameWnd,
              "retail COleDocIPFrameWnd adds no data to a 0x280-byte COleIPFrameWnd");
static_assert(kOff_m_hMenuHelpPopup + 2 * sizeof(void*) == kSizeof_COleDocIPFrameWnd,
              "m_hMenuHelpPopup @0x270 is followed only by _m_Reserved @0x278");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40, where RecalcLayout reads it");

// ---- retail COleServerDoc member read by OnRequestPositionChange / RecalcLayout
// m_pDocObjectServer: `cmpq $0x0,0x268(%rax)` at 0x2568e9 and 0x256947.
constexpr size_t kOff_COleServerDoc_m_pDocObjectServer = 0x268;
static_assert(offsetof(COleServerDoc, m_bEmbedded) + sizeof(BOOL) <= kOff_COleServerDoc_m_pDocObjectServer &&
              kOff_COleServerDoc_m_pDocObjectServer + sizeof(void*) <= sizeof(COleServerDoc) &&
              sizeof(COleServerDoc) == 0x298,
              "retail COleServerDoc::m_pDocObjectServer (+0x268) must lie inside OpenMFC's "
              "_coleserverdoc_padding of a retail-sized (0x298) COleServerDoc");

template <typename T> inline T& Member(void* pThis, size_t off) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(pThis) + off);
}
inline RECT* RectAt(void* pThis, size_t off) { return &Member<RECT>(pThis, off); }
inline CWnd* AsWnd(void* pThis) { return static_cast<CWnd*>(static_cast<COleIPFrameWnd*>(pThis)); }

// IsDocObject(), inlined by retail: m_pDocObjectServer != NULL, no class check.
inline bool DocIsDocObject(CDocument* pDoc) {
    return *reinterpret_cast<void* const*>(reinterpret_cast<const unsigned char*>(pDoc) +
                                           kOff_COleServerDoc_m_pDocObjectServer) != nullptr;
}

// ---- MSVC vftable slot dispatch (see the header comment) -------------------
template <typename Fn> inline Fn VSlot(const void* pObj, int slot) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void* const* const*>(pObj))[slot]);
}
enum : int {
    kSlot_CalcWindowRect    = 0x0e0 / 8,   // 28
    kSlot_GetActiveDocument = 0x2e8 / 8,   // 93
};
typedef void      (MS_ABI* Fn_CalcWindowRect)(CWnd*, RECT*, UINT);
typedef CDocument*(MS_ABI* Fn_GetActiveDocument)(void*);

inline CDocument* GetActiveDocumentV(void* pThis) {
    return VSlot<Fn_GetActiveDocument>(pThis, kSlot_GetActiveDocument)(pThis);
}
inline void CalcWindowRectV(CWnd* pWnd, RECT* lpRect, UINT nAdjustType) {
    VSlot<Fn_CalcWindowRect>(pWnd, kSlot_CalcWindowRect)(pWnd, lpRect, nAdjustType);
}

constexpr UINT kAFX_IDW_PANE_FIRST = 0xE900;   // afxres.h
constexpr UINT kAdjustBorder       = 0;        // CWnd::adjustBorder
constexpr UINT kAdjustOutside      = 1;        // CWnd::adjustOutside
constexpr UINT kReposDefault       = 0;        // CWnd::reposDefault
constexpr UINT kReposQuery         = 1;        // CWnd::reposQuery

}  // namespace

// COleDocIPFrameWnd::COleDocIPFrameWnd() -- retail mfc140u 0x256890:
//     COleIPFrameWnd::COleIPFrameWnd(this);               // 0x25d010
//     this->vfptr = &COleDocIPFrameWnd::`vftable';        // 0x32ed10
//     m_hMenuHelpPopup = NULL;                            // +0x270
//     return this;
// DEVIATION: OpenMFC has no COleDocIPFrameWnd vftable to plant, so the object
// keeps the vptr the base ctor thunk leaves (OpenMFC's g++ COleIPFrameWnd
// vftable).  For a client-derived object the client's ctor overwrites it
// straight afterwards, as MSVC does; for an OpenMFC-built one (CreateObject)
// the COleDocIPFrameWnd overrides are not reached through it.
// Symbol: ??0COleDocIPFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWnd__QEAA_XZ(void* pThis) {
    impl___0COleIPFrameWnd__QEAA_XZ(pThis);
    Member<HMENU>(pThis, kOff_m_hMenuHelpPopup) = nullptr;
    return pThis;
}

// COleDocIPFrameWnd::~COleDocIPFrameWnd() -- retail mfc140u 0x2568c0:
//     this->vfptr = &COleDocIPFrameWnd::`vftable';        // 0x32ed10
//     jmp COleIPFrameWnd::~COleIPFrameWnd                 // 0x25d0d0, tail jump
// COleDocIPFrameWnd adds no data members (see LAYOUT above), so the body is
// exactly the base destructor.  It is reached through a qualified,
// NON-virtual call to OpenMFC's C++ COleIPFrameWnd::~COleIPFrameWnd
// (core/ole/COleIPFrameWnd.cpp), not through the impl___1COleIPFrameWnd
// thunk: that thunk is a virtual destructor call, which on a client-derived
// object would re-enter the client destructor and recurse.
// DEVIATION: OpenMFC has no COleDocIPFrameWnd vftable to re-plant (see the
// ctor note); the base destructor installs its own vptr, as MSVC does.
// Symbol: ??1COleDocIPFrameWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1COleDocIPFrameWnd__UEAA_XZ(void* pThis) {
    static_cast<COleIPFrameWnd*>(pThis)->COleIPFrameWnd::~COleIPFrameWnd();
}

// Symbol: ?BuildSharedMenu@COleDocIPFrameWnd@@MEAAHXZ
extern "C" int MS_ABI impl__BuildSharedMenu_COleDocIPFrameWnd__MEAAHXZ(COleIPFrameWnd* frame) {
    return frame && frame->BuildSharedMenu(TRUE) == S_OK;
}

// COleDocIPFrameWnd::CreateObject() -- retail mfc140u 0x256850, transcribed:
//     void* p = operator new(0x280);                      // ??2@YAPEAX_K@Z, 0x27f0
//     if (p != NULL) COleDocIPFrameWnd::COleDocIPFrameWnd(p);   // 0x256890
//     return p;
// Both callees are this DLL's own exports (the ctor is the one above), so the
// object carries OpenMFC's g++ COleIPFrameWnd vftable -- see the ctor's
// DEVIATION note.
// Symbol: ?CreateObject@COleDocIPFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_COleDocIPFrameWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeof_COleDocIPFrameWnd);
    if (p != nullptr)
        impl___0COleDocIPFrameWnd__QEAA_XZ(p);
    return static_cast<CObject*>(static_cast<COleIPFrameWnd*>(p));
}

// Both retail frames use the same DestroySharedMenu implementation (mfc140u 0x256c00).
// Symbol: ?DestroySharedMenu@COleDocIPFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__DestroySharedMenu_COleDocIPFrameWnd__MEAAXXZ(void* pThis) {
    impl__DestroySharedMenu_COleIPFrameWnd__MEAAXXZ(pThis);
}

// COleDocIPFrameWnd::OnRequestPositionChange(LPCRECT) -- retail mfc140u
// 0x2568d0 (vslot 121; COMDAT-folded with COleIPFrameWnd's, see the header),
// transcribed:
//     COleServerDoc* pDoc = (COleServerDoc*)GetActiveDocument();   // vslot 93
//     if (pDoc->m_pDocObjectServer != NULL) return;       // IsDocObject(), +0x268
//     pDoc->RequestPositionChange(lpRect);                // 0x268010, direct
// Retail tests pDoc for NULL nowhere; neither does this.  Callee note:
// OpenMFC's COleServerDoc::RequestPositionChange is a documented no-op
// (core/ole/COleServerDoc.cpp, no client site is modelled), so no
// IOleInPlaceSite::OnPosRectChange is sent yet.
// Symbol: ?OnRequestPositionChange@COleDocIPFrameWnd@@MEAAXPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__OnRequestPositionChange_COleDocIPFrameWnd__MEAAXPEBUtagRECT___Z(
    void* pThis, const RECT* lpRect) {
    CDocument* pDoc = GetActiveDocumentV(pThis);
    if (DocIsDocObject(pDoc))
        return;
    impl__RequestPositionChange_COleServerDoc__QEAAXPEBUtagRECT___Z(static_cast<COleServerDoc*>(pDoc), lpRect);
}

// COleDocIPFrameWnd::RecalcLayout(BOOL) -- retail mfc140u 0x256910 (vslot 96),
// transcribed (bNotify, %edx, is never read):
//     COleServerDoc* pDoc = (COleServerDoc*)GetActiveDocument();   // vslot 93
//     UINT nAdjust = (pDoc != NULL && pDoc->m_pDocObjectServer != NULL)
//                        ? CWnd::adjustBorder : CWnd::adjustOutside;   // %edi
//     CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_hWnd));
//     CRect rectBig(0, 0, INT_MAX/2, INT_MAX/2);          // .rdata 0x350240
//     CRect rectLeft(0, 0, 0, 0);
//     RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectLeft, &rectBig, TRUE);
//     CRect rect = m_rectPos;
//     rect.left  -= rectLeft.left;               rect.top    -= rectLeft.top;
//     rect.right += INT_MAX/2 - rectLeft.right;  rect.bottom += INT_MAX/2 - rectLeft.bottom;
//     CWnd* pLeftOver = GetDlgItem(AFX_IDW_PANE_FIRST);
//     if (pLeftOver != NULL) {
//         rectBig = m_rectPos;
//         pLeftOver->CalcWindowRect(&rectBig, nAdjust);   // vslot 28
//         rect.left   += rectBig.left   - m_rectPos.left;
//         rect.top    += rectBig.top    - m_rectPos.top;
//         rect.right  += rectBig.right  - m_rectPos.right;
//         rect.bottom += rectBig.bottom - m_rectPos.bottom;
//     }
//     CalcWindowRect(&rect, nAdjust);                     // this, vslot 28
//     CRect rectClip; ::IntersectRect(&rectClip, &rect, &m_rectClip);
//     AfxRepositionWindow(NULL, m_hWnd, &rectClip);
//     pParentWnd->ClientToScreen(&rect);
//     ScreenToClient(&rect);                              // this
//     RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposDefault, NULL, &rect, TRUE);
// Differences from COleIPFrameWnd::RecalcLayout (0x25d600), all reproduced:
// the document is fetched BEFORE GetParent/FromHandle; the adjust type is
// INVERTED relative to the base (adjustBorder for a DocObject, adjustOutside
// otherwise -- %edi is 0 on the DocObject path, 1 otherwise, at
// 0x256951/0x256955); and that same value, not a constant adjustOutside, is
// passed to pLeftOver->CalcWindowRect as well.  The .rdata constant at
// 0x350240 is {0, 0, 0x3fffffff, 0x3fffffff} (read from the image); retail uses
// the immediate 0x3fffffff rather than re-reading rectBig after the query pass,
// as does this.  Callees: GetParent (IAT 0x1802c72d8), FromHandle 0x28ad70,
// RepositionBars 0x28eaf0, GetDlgItem 0x2a9390, IntersectRect (IAT
// 0x1802c6cc8), AfxRepositionWindow 0x28ecc0, ClientToScreen 0x2a3310,
// ScreenToClient 0x2a32b0.  The two IAT entries are import calls; the others
// are direct calls.  Retail tests pParentWnd for NULL nowhere; neither does
// this.
// Symbol: ?RecalcLayout@COleDocIPFrameWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleDocIPFrameWnd__MEAAXH_Z(void* pThis, int bNotify) {
    (void)bNotify;
    CWnd* pSelf = AsWnd(pThis);
    CDocument* pDoc = GetActiveDocumentV(pThis);
    const UINT nAdjust = (pDoc != nullptr && DocIsDocObject(pDoc)) ? kAdjustBorder : kAdjustOutside;
    CWnd* pParentWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pSelf->m_hWnd));

    constexpr LONG kBig = 0x3fffffff;   // INT_MAX / 2
    RECT rectBig = {0, 0, kBig, kBig};
    RECT rectLeft = {0, 0, 0, 0};
    impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
        pSelf, 0, 0xffff, kAFX_IDW_PANE_FIRST, kReposQuery, &rectLeft, &rectBig, TRUE);

    const RECT* pRectPos = RectAt(pThis, kOff_m_rectPos);
    RECT rect = *pRectPos;
    rect.left   -= rectLeft.left;
    rect.top    -= rectLeft.top;
    rect.right  += kBig - rectLeft.right;
    rect.bottom += kBig - rectLeft.bottom;

    CWnd* pLeftOver = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pSelf, kAFX_IDW_PANE_FIRST);
    if (pLeftOver != nullptr) {
        rectBig = *pRectPos;
        CalcWindowRectV(pLeftOver, &rectBig, nAdjust);
        rect.left   += rectBig.left   - pRectPos->left;
        rect.top    += rectBig.top    - pRectPos->top;
        rect.right  += rectBig.right  - pRectPos->right;
        rect.bottom += rectBig.bottom - pRectPos->bottom;
    }

    CalcWindowRectV(pSelf, &rect, nAdjust);

    RECT rectClip = {0, 0, 0, 0};
    ::IntersectRect(&rectClip, &rect, RectAt(pThis, kOff_m_rectClip));
    impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
        nullptr, pSelf->m_hWnd, &rectClip);

    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pParentWnd, &rect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pSelf, &rect);

    impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
        pSelf, 0, 0xffff, kAFX_IDW_PANE_FIRST, kReposDefault, nullptr, &rect, TRUE);
}
