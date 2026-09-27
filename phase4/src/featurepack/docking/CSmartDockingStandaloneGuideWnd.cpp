// CSmartDockingStandaloneGuideWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The window that paints one standalone smart-docking marker.  Retail
// declaration: atlmfc/include/afxsmartdockingguide.h:29, `class
// CSmartDockingStandaloneGuideWnd : public CWnd` with protected members
// m_hbmpFace, m_clrFrame, m_bIsDefaultImage, m_bIsHighlighted, m_bIsVert, in
// that order.  include/openmfc declares no such class, so every body below
// works on `void* pThis` through the layout pinned in S_SdGuideWnd.
//
// Retail layout, read from the constructor ??0CSmartDockingStandaloneGuideWnd@@QEAA@XZ
// (entry RVA 0x130d10, mfc140u) and cross-checked against Create (0x130dc0),
// Highlight (0x130ea0), Assign (0x131110), UpdateLayered (0x130ee0) and OnPaint
// (0x131140), all mfc140u:
//   +0x000  CWnd base (0xe8 bytes; the ctor calls ??0CWnd@@QEAA@XZ, 0x28a700
//           mfc140u, then stores the class vftable 0x18030f338 (mfc140u))
//   +0x040  CWnd::m_hWnd
//   +0x0e8  HBITMAP  m_hbmpFace        NOT written by the ctor
//   +0x0f0  COLORREF m_clrFrame        ctor: 0xffffffff
//   +0x0f4  BOOL     m_bIsDefaultImage ctor: one 8-byte zero store covers
//   +0x0f8  BOOL     m_bIsHighlighted    both of these
//   +0x0fc  BOOL     m_bIsVert         ctor: 0
//   sizeof == 0x100 (matches the 0x100-byte m_wndBmp member that
//   CSmartDockingStandaloneGuide.cpp pins at +0x10 .. +0x110 of its object).
//
// Where the unnamed bodies live.  mfc140u_rva_symbols.json does not name
// OnClose, OnEraseBkgnd, OnPaint or UpdateLayered; the mfc140u export directory
// (via the ordinal map) resolves them, and the class message map (mfc140u VA
// 0x18030f298, returned by GetMessageMap 0x130db0) agrees:
//     WM_PAINT      sig 19  0x131140  OnPaint
//     WM_CLOSE      sig 19  0x27d0    OnClose       (the shared bare `ret`)
//     WM_ERASEBKGND sig  1  0x3a60    OnEraseBkgnd  (the shared `mov $1,%eax; ret`)
//   UpdateLayered is ordinal 14139 at 0x130ee0 (mfc140u), the direct call
//   target inside Highlight.
// NOTE: OpenMFC's message map for this class (featurepack/docking/MessageMaps.cpp
// -> detail/Pane17MsgmapSupport.cpp) currently has NO entries, so none of the
// three handlers is dispatched by OpenMFC's window procedure yet.
//
// Virtual dispatch.  Create calls CreateEx through vftable slot 24 (+0xc0).
// OpenMFC has no MSVC-layout vftable for this class: ??0CWnd@@QEAA@XZ
// (core/window/CtorDtorPlacement.cpp) placement-news OpenMFC's CWnd, so an object
// built by the constructor here carries OpenMFC's mingw CWnd vtable.  Same
// convention as featurepack/controls/CMFCPreviewCtrlImpl.cpp: the constructor
// records that vptr (g_ownVptr); an object still carrying it is exactly this
// class (slot 24 of the retail class vftable is CWnd's own
// ?CreateEx@CWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAV1@IPEAX@Z, 0x28b440 mfc140u), so
// the CWnd thunk is called directly; any other vptr belongs to a client class
// compiled against the real headers, and the call goes through that table's
// slot 24 exactly as retail does.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                     // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                                     // core/window/CtorDtorPlacement.cpp
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);                      // core/runtime/Globals.cpp
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* lpParam);                // core/window/CWnd.cpp
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManager__UEAAXAEAK0_Z(
    CMFCVisualManager* pThis, unsigned long& clrBaseGroupBackground,
    unsigned long& clrBaseGroupBorder);                                                          // featurepack/visualmanager/CMFCVisualManager.cpp
// ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA (core/runtime/StaticData.cpp).
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;

// This file's own thunk that Highlight calls directly (defined below).
extern "C" void MS_ABI impl__UpdateLayered_CSmartDockingStandaloneGuideWnd__QEAAXXZ(void* pThis);

namespace {

struct S_SdGuideWnd {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                     // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    HBITMAP       m_hbmpFace;                 // +0x0e8
    COLORREF      m_clrFrame;                 // +0x0f0
    BOOL          m_bIsDefaultImage;          // +0x0f4
    BOOL          m_bIsHighlighted;           // +0x0f8
    BOOL          m_bIsVert;                  // +0x0fc
};
static_assert(offsetof(S_SdGuideWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_SdGuideWnd, m_hbmpFace) == 0xe8, "m_hbmpFace");
static_assert(offsetof(S_SdGuideWnd, m_clrFrame) == 0xf0, "m_clrFrame");
static_assert(offsetof(S_SdGuideWnd, m_bIsDefaultImage) == 0xf4, "m_bIsDefaultImage");
static_assert(offsetof(S_SdGuideWnd, m_bIsHighlighted) == 0xf8, "m_bIsHighlighted");
static_assert(offsetof(S_SdGuideWnd, m_bIsVert) == 0xfc, "m_bIsVert");
static_assert(sizeof(S_SdGuideWnd) == 0x100, "sizeof(CSmartDockingStandaloneGuideWnd)");
static_assert(sizeof(CWnd) == 0xe8, "CWnd base must end where m_hbmpFace starts");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");

inline S_SdGuideWnd* L(void* p) { return static_cast<S_SdGuideWnd*>(p); }

// ---------------------------------------------------------------------------
// Virtual dispatch on `this` (file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *static_cast<void* const*>(p) == g_ownVptr;
}
// Entry at vftable byte offset `off` (retail's `mov off(%rax),%rax`).
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}

using PFN_CreateExRect = int (MS_ABI*)(void*, DWORD, const wchar_t*, const wchar_t*, DWORD,
                                       const RECT*, void*, UINT, void*);   // CWnd slot 24 (+0xc0)

} // namespace

// Retail ??0CSmartDockingStandaloneGuideWnd@@QEAA@XZ (entry RVA 0x130d10, mfc140u):
//     CWnd::CWnd();                                       // 0x28a700
//     vfptr = &CSmartDockingStandaloneGuideWnd::`vftable';  // 0x18030f338 (mfc140u)
//     m_clrFrame = (COLORREF)-1;
//     *(ULONGLONG*)&m_bIsDefaultImage = 0;                // m_bIsDefaultImage = m_bIsHighlighted = FALSE
//     m_bIsVert = FALSE;
//     return this;                                        // m_hbmpFace is left unwritten
// DEVIATION: no MSVC-layout vftable exists for this class here, so `this`
// keeps the vptr ??0CWnd@@ installs (recorded as g_ownVptr, file header).
// Symbol: ??0CSmartDockingStandaloneGuideWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingStandaloneGuideWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;   // deviation: retail has no NULL check
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    S_SdGuideWnd* self = L(pThis);
    self->m_clrFrame = 0xffffffffu;
    self->m_bIsDefaultImage = FALSE;
    self->m_bIsHighlighted = FALSE;
    self->m_bIsVert = FALSE;
    return pThis;
}

// Retail ??1CSmartDockingStandaloneGuideWnd@@UEAA@XZ (entry RVA 0x130da0, mfc140u), fully transcribed:
//     vfptr = &CSmartDockingStandaloneGuideWnd::`vftable';  // 0x18030f338 (mfc140u)
//     CWnd::~CWnd();                                         // tail jump to 0x28b740
// DEVIATION: the own-vftable store becomes the g_ownVptr store, so that
// OpenMFC's ??1CWnd@@ (whose C++ destructor call is virtual) dispatches on
// OpenMFC's CWnd table rather than on a client-derived MSVC table.
// Symbol: ??1CSmartDockingStandaloneGuideWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CSmartDockingStandaloneGuideWnd__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CWnd__UEAA_XZ(pThis);
}

// Retail entry RVA 0x131110 (mfc140u), fully transcribed:
//     if (hbmpFace != NULL) m_hbmpFace = hbmpFace;
//     ::InvalidateRect(m_hWnd, NULL, bRedraw);   // IAT 0x1802c7128 (mfc140u); r8d = bRedraw untouched
//     return TRUE;
// (No NULL-HWND check, as in retail.)
// Symbol: ?Assign@CSmartDockingStandaloneGuideWnd@@QEAAHPEAUHBITMAP__@@H@Z
extern "C" int MS_ABI impl__Assign_CSmartDockingStandaloneGuideWnd__QEAAHPEAUHBITMAP____H_Z(
    void* pThis, HBITMAP hbmpFace, int bRedraw) {
    S_SdGuideWnd* self = L(pThis);
    if (hbmpFace != nullptr) self->m_hbmpFace = hbmpFace;
    ::InvalidateRect(self->m_hWnd, nullptr, bRedraw);
    return TRUE;
}

// Retail entry RVA 0x130dc0 (mfc140u), transcribed:
//     m_bIsDefaultImage = bIsDefaultImage;
//     m_bIsVert = bIsVert;
//     m_hbmpFace = hbmpFace;
//     LPCTSTR lpszClass = AfxRegisterWndClass(CS_SAVEBITS | CS_OWNDC /*0x820*/, NULL, NULL, NULL);  // 0x28c4a0
//     BOOL bResult = CreateEx(0, lpszClass, _T("") /*0x18033d19c*/, WS_POPUP,
//                             *pWndRect, pwndOwner, 0, NULL);   // this vftable +0xc0 (slot 24)
//     if (bResult)
//         ::SetWindowRgn(m_hWnd, hrgnShape, FALSE);             // IAT 0x1802c6d10 (mfc140u)
//     COLORREF clrBaseGroupBackground;                          // discarded
//     CMFCVisualManager::GetInstance()                          // unexported helper 0x9774
//         ->GetSmartDockingBaseGuideColors(clrBaseGroupBackground, m_clrFrame);   // vslot 118 (+0x3b0)
//     return bResult;
// (The visual-manager call is unconditional in retail: it runs whether or not
// CreateEx succeeded.)
// DEVIATIONS:
//  * CreateEx: see the file header -- direct CWnd thunk for an object carrying
//    OpenMFC's own vptr, MSVC slot 24 otherwise.
//  * GetInstance: retail's helper at 0x9774 (mfc140u) lazily creates the
//    default manager when ?m_pVisManager@CMFCVisualManager@@ is NULL (from
//    ?m_pRTIDefault@CMFCVisualManager@@ when set, else a plain
//    CMFCVisualManager).  That creation exists here only as the out-of-line
//    C++ static CMFCVisualManager::GetInstance()
//    (featurepack/visualmanager/CMFCVisualManager.cpp), which has no impl__
//    thunk.  It is defined in the DLL (so it would link), but the campaign's
//    rule (BRIEFING S1, enforced by checkfile.sh's link audit) is that no new
//    C++-mangled reference may be added to a TU, so it is not called.  As in
//    featurepack/controls/CMFCHeaderCtrl.cpp, the exported pointer is read and
//    the call is skipped while it is NULL, leaving m_clrFrame at its ctor value.
//  * vslot 118 is reached through the exported
//    ?GetSmartDockingBaseGuideColors@CMFCVisualManager@@ thunk, which is the BASE
//    class body, so a derived manager's override (e.g. OfficeXP / Office2003) is
//    not reached.  OpenMFC's visual managers are mingw-layout C++ objects, on
//    which retail's +0x3b0 slot means something else.
// Symbol: ?Create@CSmartDockingStandaloneGuideWnd@@QEAAHPEAUtagRECT@@PEAUHBITMAP__@@PEAUHRGN__@@PEAVCWnd@@HH@Z
extern "C" int MS_ABI impl__Create_CSmartDockingStandaloneGuideWnd__QEAAHPEAUtagRECT__PEAUHBITMAP____PEAUHRGN____PEAVCWnd__HH_Z(
    void* pThis, RECT* pWndRect, HBITMAP hbmpFace, HRGN hrgnShape, CWnd* pwndOwner,
    int bIsDefaultImage, int bIsVert) {
    S_SdGuideWnd* self = L(pThis);
    self->m_bIsDefaultImage = bIsDefaultImage;
    self->m_bIsVert = bIsVert;
    self->m_hbmpFace = hbmpFace;

    const wchar_t* lpszClass =
        impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(CS_SAVEBITS | CS_OWNDC,
                                                                          nullptr, nullptr, nullptr);
    int bResult;
    if (HasOwnVptr(pThis)) {
        bResult = impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
            static_cast<CWnd*>(pThis), 0, lpszClass, L"", WS_POPUP, *pWndRect, pwndOwner, 0, nullptr);
    } else {
        bResult = reinterpret_cast<PFN_CreateExRect>(SlotAt(pThis, 0xc0))(
            pThis, 0, lpszClass, L"", WS_POPUP, pWndRect, pwndOwner, 0, nullptr);
    }
    if (bResult) {
        ::SetWindowRgn(self->m_hWnd, hrgnShape, FALSE);
    }

    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVM != nullptr) {   // deviation: retail would create the default manager
        unsigned long clrBaseGroupBackground = 0;   // discarded, as in retail
        impl__GetSmartDockingBaseGuideColors_CMFCVisualManager__UEAAXAEAK0_Z(
            pVM, clrBaseGroupBackground, self->m_clrFrame);
    }
    return bResult;
}

// Retail entry RVA 0x130ea0 (mfc140u), fully transcribed:
//     m_bIsHighlighted = bSet;
//     if (m_hWnd != NULL) {
//         ::RedrawWindow(m_hWnd, NULL, NULL,
//                        RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW /*0x105*/);   // IAT 0x1802c7130 (mfc140u)
//         UpdateLayered();                                   // direct call, 0x130ee0
//     }
// (UpdateLayered is still a documented stub below, so the second call is
// currently a no-op.)
// Symbol: ?Highlight@CSmartDockingStandaloneGuideWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__Highlight_CSmartDockingStandaloneGuideWnd__QEAAXH_Z(void* pThis, int bSet) {
    S_SdGuideWnd* self = L(pThis);
    self->m_bIsHighlighted = bSet;
    if (self->m_hWnd != nullptr) {
        ::RedrawWindow(self->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        impl__UpdateLayered_CSmartDockingStandaloneGuideWnd__QEAAXXZ(pThis);
    }
}

// The export resolves (ures.py, ordinal 8883) to RVA 0x27d0 (mfc140u), the
// shared bare `ret` body; the WM_CLOSE entry of the class message map points
// there too.  Retail therefore does nothing on WM_CLOSE -- in particular it does
// not call Default(), so the marker window cannot be closed by WM_CLOSE.  The
// empty body below IS the retail behaviour, not a placeholder.
// Symbol: ?OnClose@CSmartDockingStandaloneGuideWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CSmartDockingStandaloneGuideWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// The export resolves (ures.py, ordinal 9792) to RVA 0x3a60 (mfc140u), the
// shared `mov $1,%eax; ret` body (also the WM_ERASEBKGND entry of the class
// message map): return TRUE, pDC unused.
// Symbol: ?OnEraseBkgnd@CSmartDockingStandaloneGuideWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CSmartDockingStandaloneGuideWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Symbol: ?OnPaint@CSmartDockingStandaloneGuideWnd@@IEAAXXZ
// STUB.  Retail (entry RVA 0x131140, mfc140u; instruction-identical body at
// 0x131dc0 in mfc140.dll):
//     CPaintDC dc(this);                                             // 0x2a3d20
//     CRect rect(0, 0, 0, 0);
//     ::GetClientRect(m_hWnd, &rect);                                // IAT 0x1802c7330
//     ::DrawState(dc.m_hDC, NULL, NULL, (LPARAM)m_hbmpFace, 0,
//                 0, 0, rect.Width(), rect.Height(), DST_BITMAP /*4*/);   // IAT 0x1802c6bb8 (DrawStateW)
//     if (m_bIsDefaultImage && Theme() != 2) {                       // Theme(): unexported 0x12ee88
//         COLORREF clr = m_bIsHighlighted ? RGB(0x41,0x70,0xca) /*0xca7041*/ : m_clrFrame;
//         dc.Draw3dRect(&rect, clr, clr);                            // 0x2a5bc0
//         DrawGripLines(&dc, rect /*by value*/, m_bIsVert);          // unexported 0x12eec4
//     }
//     // ~CPaintDC                                                   // 0x2a3dd0
// DrawGripLines (read from its mfc140.dll twin at 0x12fb44):
//     ::InflateRect(&rect, -1, -1);
//     COLORREF clr[2] = { 0xc6c6c6, 0xcecece };
//     for (int i = 0; i < 2; i++) {
//         CPen pen(PS_SOLID, 1, clr[i]);  CPen* pOld = pDC->SelectObject(&pen);
//         if (bIsVert) { pDC->MoveTo(rect.left + i, rect.top);  pDC->LineTo(rect.left + i, rect.bottom); }
//         else         { pDC->MoveTo(rect.left, rect.top + i);  pDC->LineTo(rect.right, rect.top + i); }
//         pDC->SelectObject(pOld);
//     }
// Theme() returns 0 when CDockingManager::m_SDParams.m_uiMarkerBmpResID[0]
// (m_SDParams + 0x28, mfc140u VA 0x1803c15d8) is non-zero, else the unexported
// int at mfc140u VA 0x1803be20c when non-zero, else
// CMFCVisualManager::GetInstance()->GetSmartDockingTheme() (vslot 120, +0x3c0).
// Not reproduced: OpenMFC's exported m_SDParams is an 8-byte placeholder, not a
// CSmartDockingInfo (featurepack/docking/CDockingManager.cpp), and the
// unexported theme int has no OpenMFC counterpart, so whether the frame is
// drawn cannot be decided; a body that paints only the bitmap would silently
// drop the frame.  OpenMFC's message map for this class has no WM_PAINT entry
// (file header), so this handler is not reached by the window procedure anyway.
extern "C" void MS_ABI impl__OnPaint_CSmartDockingStandaloneGuideWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?UpdateLayered@CSmartDockingStandaloneGuideWnd@@QEAAXXZ
// STUB.  Retail (entry RVA 0x130ee0, mfc140u; ordinal 14139; instruction-identical
// body at 0x131b60 in mfc140.dll):
//     if (!CDockingManager::m_SDParams.m_bIsAlphaMarkers              // m_SDParams + 0x58, VA 0x1803c1608
//         && Theme() != 2)                                           // unexported 0x12ee88, see OnPaint
//         return;
//     CRect rect(0, 0, 0, 0);  ::GetClientRect(m_hWnd, &rect);       // IAT 0x1802c7330
//     CPoint point(0, 0);  CSize size(rect.Width(), rect.Height());
//     LPVOID pBits = NULL;
//     HBITMAP hBitmap = CDrawingManager::CreateBitmap_32(size, &pBits);   // 0x56580
//     if (hBitmap == NULL) return;
//     CBitmap bitmap;  bitmap.Attach(hBitmap);                       // 0x2a3ed0
//     CClientDC clientDC(this);                                      // 0x2a3b20
//     CDC dc;  dc.Attach(::CreateCompatibleDC(clientDC.m_hDC));      // IAT 0x1802c6288, 0x2a2480
//     CBitmap* pOld = (CBitmap*)CGdiObject::FromHandle(
//         ::SelectObject(dc.m_hDC, bitmap.m_hObject));               // IAT 0x1802c64f8, 0x2a3ea0
//     ::DrawState(dc.m_hDC, NULL, NULL, (LPARAM)m_hbmpFace, 0,
//                 0, 0, size.cx, size.cy, DST_BITMAP);               // IAT 0x1802c6bb8
//     BLENDFUNCTION bf = { AC_SRC_OVER, 0, (BYTE)(m_bIsHighlighted ? 255 : 192), AC_SRC_ALPHA };
//     ::UpdateLayeredWindow(m_hWnd, NULL, NULL, &size, dc.m_hDC, &point,
//                           0, &bf, ULW_ALPHA);                      // IAT 0x1802c6dd8
//     ::SelectObject(dc.m_hDC, pOld ? pOld->m_hObject : NULL);  CGdiObject::FromHandle(...);
//     // ~CDC (inlined): if (dc.m_hDC != NULL) ::DeleteDC(dc.Detach())
//     //   (IAT 0x1802c6148, 0x2a24d0); ~CClientDC (0x2a3be0);
//     // ~CBitmap (inlined): CBitmap vftable 0x1802ddc10 store, then the
//     //   unexported ~CGdiObject at 0x1c6f0 (CGdiObject vftable store +
//     //   CGdiObject::DeleteObject 0x2a3f60); all mfc140u
// Not reproduced: the opening gate needs m_SDParams.m_bIsAlphaMarkers and
// Theme(), neither of which OpenMFC models (see OnPaint); running the body
// unconditionally would call ::UpdateLayeredWindow on markers for which retail
// returns before doing anything.
extern "C" void MS_ABI impl__UpdateLayered_CSmartDockingStandaloneGuideWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}
