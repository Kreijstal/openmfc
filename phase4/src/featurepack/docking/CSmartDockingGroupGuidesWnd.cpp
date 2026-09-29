// CSmartDockingGroupGuidesWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The window that paints the central smart-docking "diamond" (the five group
// markers).  Retail declaration: atlmfc/include/afxsmartdockingguide.h:113,
// `class CSmartDockingGroupGuidesWnd : public CWnd` (friend
// CSmartDockingGroupGuidesManager) with protected members m_pCentralGroup,
// m_brBaseBackground, m_brBaseBorder, in that order.  include/openmfc declares
// no such class, so every body below works on `void* pThis` through the layout
// pinned in S_SdGroupGuidesWnd.
//
// Retail layout, read from the constructor ??0CSmartDockingGroupGuidesWnd@@IEAA@XZ
// (entry RVA 0x12f7d0, mfc140u) and from OnPaint (0x12fb70) and Update
// (0x12f960), all mfc140u:
//   +0x000  CWnd base (0xe8 bytes; the ctor calls ??0CWnd@@QEAA@XZ, 0x28a700
//           mfc140u, then stores the class vftable, VA 0x18030f788 mfc140u)
//   +0x040  CWnd::m_hWnd
//   +0x0e8  CSmartDockingGroupGuidesManager* m_pCentralGroup   (ctor: NULL)
//   +0x0f0  CBrush m_brBaseBackground   (CBrush vftable VA 0x1802dde08 mfc140u,
//                                        m_hObject at +0xf8)
//   +0x100  CBrush m_brBaseBorder       (same vftable, m_hObject at +0x108)
//   sizeof == 0x110 (CSmartDockingGroupGuidesManager.cpp pins this object as
//   its 0x110-byte m_Wnd member at +0x8 .. +0x118).
// OnPaint and Update pass &m_brBaseBackground (this+0xf0) and &m_brBaseBorder
// (this+0x100) to DrawCentralGroupGuides and load m_pCentralGroup from
// this+0xe8, which agrees with the declaration order above.
//
// Where the handlers live.  mfc140u_rva_symbols.json names none of OnClose,
// OnEraseBkgnd, OnPaint or Update; mfc140u.dll's own export address table
// (ordrva.py, ordinals from mfc_complete_ordinal_mapping.json) resolves them:
//     ordinal  8881  OnClose       0x27d0     (the shared bare `ret`)
//     ordinal  9790  OnEraseBkgnd  0x3a60     (the shared `mov $1,%eax; ret`)
//     ordinal 10761  OnPaint       0x12fb70
//     ordinal 14101  Update        0x12f960
// and the retail class message map (VA 0x18030f6e8 mfc140u, returned by
// GetMessageMap at 0x12f950; base map CWnd's; dumped with msgmap_u.py) agrees:
//     WM_PAINT      sig 19  0x12fb70
//     WM_CLOSE      sig 19  0x27d0
//     WM_ERASEBKGND sig  1  0x3a60
// NOTE: OpenMFC's message map for this class (featurepack/docking/MessageMaps.cpp
// -> detail/Pane16MsgmapSupport.cpp) currently has NO entries, so none of these
// handlers is dispatched by OpenMFC's window procedure yet; they are correct
// when reached by export.
//
// Why OnPaint and Update are still stubs.  Both read
// CDockingManager::m_SDParams (a CSmartDockingInfo; ?m_SDParams@CDockingManager@@
// is at VA 0x1803c15b0 mfc140u, field offsets in detail/CSmartDockingInfoSupport.h)
// -- OnPaint its m_clrTransparent (+0x14), Update its m_bIsAlphaMarkers (+0x58)
// and, through the unexported Theme() helper, m_uiMarkerBmpResID[0] (+0x28).
// OpenMFC exports m_SDParams as an 8-byte placeholder
// (featurepack/docking/CDockingManager.cpp), so none of those fields exists.
// Both also delegate the actual drawing to
// CSmartDockingGroupGuidesManager::DrawCentralGroupGuides, which is itself a
// stub in CSmartDockingGroupGuidesManager.cpp.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

namespace {

struct S_SdGroupGuidesWnd {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                      // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    void*         m_pCentralGroup;             // +0x0e8  CSmartDockingGroupGuidesManager*
    void*         m_brBaseBackground_vfptr;    // +0x0f0  CBrush
    HGDIOBJ       m_brBaseBackground_hObject;  // +0x0f8  CBrush::m_hObject
    void*         m_brBaseBorder_vfptr;        // +0x100  CBrush
    HGDIOBJ       m_brBaseBorder_hObject;      // +0x108  CBrush::m_hObject
};
static_assert(offsetof(S_SdGroupGuidesWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");
static_assert(offsetof(S_SdGroupGuidesWnd, m_pCentralGroup) == 0xe8, "m_pCentralGroup +0xe8");
static_assert(offsetof(S_SdGroupGuidesWnd, m_brBaseBackground_vfptr) == 0xf0, "m_brBaseBackground +0xf0");
static_assert(offsetof(S_SdGroupGuidesWnd, m_brBaseBorder_vfptr) == 0x100, "m_brBaseBorder +0x100");
static_assert(sizeof(S_SdGroupGuidesWnd) == 0x110, "embedded as the 0x110-byte m_Wnd of CSmartDockingGroupGuidesManager");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads HWNDs at +0x40");

} // namespace

// Symbol: ??0CSmartDockingGroupGuidesWnd@@IEAA@XZ
// STUB -- constructs nothing.  Retail (entry RVA 0x12f7d0, mfc140u) calls
// ??0CWnd@@QEAA@XZ (0x28a700), stores the class vftable (VA 0x18030f788),
// sets m_pCentralGroup (+0xe8) = NULL, default-constructs the two CBrushes
// (+0xf0, +0x100; CBrush vftable VA 0x1802dde08, m_hObject = NULL), calls
// CMFCVisualManager::GetInstance()->GetSmartDockingBaseGuideColors(clrBg,
// clrBorder) (vftable +0x3b0, slot 118) unconditionally, then Attaches
// ::CreateSolidBrush(m_SDParams.m_clrBaseBackground (VA 0x1803c15d0), or clrBg
// when that is -1) to m_brBaseBackground and likewise m_clrBaseBorder
// (VA 0x1803c15d4) / clrBorder to m_brBaseBorder -- all mfc140u.  Not
// reproduced: it needs the m_SDParams fields (8-byte placeholder, file header)
// and a CSmartDockingGroupGuidesWnd vftable, which OpenMFC does not emit.
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuidesWnd__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ?OnClose@CSmartDockingGroupGuidesWnd@@QEAAXXZ
// The export (ordinal 8881) resolves in mfc140u.dll's export table to RVA
// 0x27d0 (mfc140u), a COMDAT-folded body that is a single `ret 0` (bytes
// c2 00 00, i.e. a plain return); the WM_CLOSE
// entry of the retail message map points there too.  Retail therefore does
// nothing on WM_CLOSE -- in particular it does not call CWnd::OnClose /
// Default(), so the diamond window cannot be closed by WM_CLOSE.  The empty body
// below IS the complete retail behaviour, not a placeholder.
extern "C" void MS_ABI impl__OnClose_CSmartDockingGroupGuidesWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnEraseBkgnd@CSmartDockingGroupGuidesWnd@@QEAAHPEAVCDC@@@Z
// The export (ordinal 9790) resolves in mfc140u.dll's export table to RVA
// 0x3a60 (mfc140u), the COMDAT-folded `mov $1,%eax; ret` (also the
// WM_ERASEBKGND entry of the retail message map): return TRUE without erasing,
// pDC unused.  Complete transcription.
extern "C" int MS_ABI impl__OnEraseBkgnd_CSmartDockingGroupGuidesWnd__QEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Symbol: ?OnPaint@CSmartDockingGroupGuidesWnd@@QEAAXXZ
// STUB.  Retail (entry RVA 0x12fb70, mfc140u; the same instruction sequence,
// differing only in RIP-relative displacements, sits at 0x1307f0 in
// mfc140.dll), complete, IAT slots resolved with iatu.py:
//     CPaintDC dc(this);                                        // 0x2a3d20
//     CMemDC memDC(dc, this);                                   // 0x69f50
//     CDC* pDC = &memDC.GetDC();     // inlined: m_bMemDC (+0x10) ? m_dcMem (+0x20) : m_dc (+0x8)
//     CRect rectClient(0, 0, 0, 0);
//     ::GetClientRect(m_hWnd, &rectClient);                     // IAT 0x1802c7330
//     CBrush br;  br.Attach(::CreateSolidBrush(                 // IAT 0x1802c6298; Attach 0x2a3ed0
//         CDockingManager::m_SDParams.m_clrTransparent));       // VA 0x1803c15c4 = m_SDParams + 0x14
//     ::FillRect(pDC->m_hDC, &rectClient, (HBRUSH)br.m_hObject);   // IAT 0x1802c7208
//     m_pCentralGroup->DrawCentralGroupGuides(*pDC, m_brBaseBackground,
//                                             m_brBaseBorder, rectClient);  // 0x130a70, no NULL test
//     // ~CBrush: CBrush vftable 0x1802dde08 store, then the unexported
//     //   ~CGdiObject at 0x1c6f0 (CGdiObject::DeleteObject 0x2a3f60);
//     // ~CMemDC (0x6a380); ~CPaintDC (0x2a3dd0)             -- all mfc140u
// Not reproduced: the background colour is m_SDParams.m_clrTransparent (the
// colour key the manager's Create gives ::SetLayeredWindowAttributes), which
// does not exist in OpenMFC's 8-byte m_SDParams placeholder (file header); and
// everything else the handler paints comes from DrawCentralGroupGuides, a stub
// in CSmartDockingGroupGuidesManager.cpp.  A body filling with an invented
// colour key would paint the whole window opaque instead of transparent.
extern "C" void MS_ABI impl__OnPaint_CSmartDockingGroupGuidesWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?Update@CSmartDockingGroupGuidesWnd@@QEAAXXZ
// STUB.  Retail (entry RVA 0x12f960, mfc140u; the same instruction sequence,
// differing only in RIP-relative displacements, sits at 0x1305e0 in
// mfc140.dll), complete, IAT slots resolved with iatu.py:
//     if (!CDockingManager::m_SDParams.m_bIsAlphaMarkers         // VA 0x1803c1608 = m_SDParams + 0x58
//         && Theme() != 2) {                                     // unexported 0x12ee88
//         ::RedrawWindow(m_hWnd, NULL, NULL,
//                        RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);   // IAT 0x1802c7130, 0x105
//         return;
//     }
//     CRect rectClient(0, 0, 0, 0);  ::GetClientRect(m_hWnd, &rectClient);   // IAT 0x1802c7330
//     CPoint point(0, 0);  CSize size(rectClient.Width(), rectClient.Height());
//     LPVOID pBits = NULL;
//     HBITMAP hBitmap = CDrawingManager::CreateBitmap_32(size, &pBits);   // 0x56580
//     if (hBitmap == NULL) return;
//     CBitmap bitmap;  bitmap.Attach(hBitmap);                   // CBitmap vftable 0x1802ddc10; 0x2a3ed0
//     CClientDC clientDC(this);                                  // 0x2a3b20
//     CDC dc;  dc.Attach(::CreateCompatibleDC(clientDC.m_hDC));  // CDC vftable 0x18033b510;
//                                                                //   IAT 0x1802c6288; 0x2a2480
//     CBitmap* pOld = (CBitmap*)CGdiObject::FromHandle(
//         ::SelectObject(dc.m_hDC, bitmap.m_hObject));           // IAT 0x1802c64f8; 0x2a3ea0
//     m_pCentralGroup->DrawCentralGroupGuides(dc, m_brBaseBackground,
//                                             m_brBaseBorder, rectClient);  // 0x130a70
//     BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };  // dword 0x01ff0000
//     ::UpdateLayeredWindow(m_hWnd, NULL, NULL, &size, dc.m_hDC, &point,
//                           0, &bf, ULW_ALPHA);                  // IAT 0x1802c6dd8; result ignored
//     CGdiObject::FromHandle(::SelectObject(dc.m_hDC,
//                            pOld ? pOld->m_hObject : NULL));    // IAT 0x1802c64f8; 0x2a3ea0
//     // ~CDC (inlined): if (dc.m_hDC != NULL) ::DeleteDC(dc.Detach());   // 0x2a24d0, IAT 0x1802c6148
//     // ~CClientDC (0x2a3be0); ~CBitmap: vftable 0x1802ddc10 store, then the
//     //   unexported ~CGdiObject at 0x1c6f0                -- all mfc140u
// Theme() (0x12ee88 mfc140u) returns 0 when m_SDParams.m_uiMarkerBmpResID[0]
// (VA 0x1803c15d8 = m_SDParams + 0x28) is non-zero, else the unexported int at
// VA 0x1803be20c when non-zero, else
// CMFCVisualManager::GetInstance()->GetSmartDockingTheme() (vftable +0x3c0,
// slot 120).
// Not reproduced: the opening gate needs m_SDParams.m_bIsAlphaMarkers and
// Theme(), neither of which OpenMFC models (file header), so it cannot be
// decided whether retail would merely ::RedrawWindow or rebuild the layered
// bitmap; and the alpha path's content comes from DrawCentralGroupGuides, a
// stub in CSmartDockingGroupGuidesManager.cpp (an ::UpdateLayeredWindow with an
// all-zero 32-bit bitmap would make the window fully transparent).
// The parameter list takes `this`, as the mangled name describes and as the
// callers in CSmartDockingGroupGuidesManager.cpp (AdjustPos) and
// CSmartDockingGroupGuide.cpp declare it.
extern "C" void MS_ABI impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}
