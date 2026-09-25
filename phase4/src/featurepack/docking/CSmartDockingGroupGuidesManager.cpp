// CSmartDockingGroupGuidesManager — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The central smart-docking "diamond": one window (m_Wnd) that paints the five
// group markers.  Retail declaration: atlmfc/include/afxsmartdockingguide.h:168
// (class CSmartDockingGroupGuidesManager : public CObject).  OpenMFC declares no
// matching class, so every body below works on `void* pThis` through the layout
// pinned in SdGroupMgrLayout.
//
// Images.  Every body was disassembled in mfc140u.dll with `disas.py --u`.
// mfc140u_rva_symbols.json names only some of these exports; Show (RVA 0x130860)
// and ShowGuide (RVA 0x130a20) were resolved through mfc140u.dll's own export
// address table via the ordinals in mfc_complete_ordinal_mapping.json (urva.py);
// callees are named from mfc140u_rva_symbols.json or, where it has no name,
// through the export table (uwho.py).  Every RVA
// and VA cited in this file is an mfc140u.dll address.
//
// Retail vftable (mfc140u VA 0x18030f618, installed by the constructor at
// RVA 0x130340 and again by the destructor at RVA 0x130420):
//   slot 0 GetRuntimeClass   slot 1 scalar deleting dtor (RVA 0x1303d0)
//   slot 5 (+0x28) Create (RVA 0x1304c0)   slot 6 (+0x30) Destroy (RVA 0x1307e0)
//   slot 7 (+0x38) Show (RVA 0x130860)     slot 8 (+0x40) AdjustPos (RVA 0x1308f0)
//   slot 9 (+0x48) GetGuide -- the inline body at RVA 0x12ede0:
//       n - 4 <= 4 (unsigned) ? this + 0x118 + (n - 4) * 0x4a8 : NULL
//
// The constructor (retail RVA 0x130340) is still an OpenMFC stub that constructs
// nothing (see below).  Bodies here that touch only plain fields, Win32 and
// exported thunks are transcribed anyway, as the sibling CSmartDockingGroupGuide.cpp
// does; on an object whose members were really constructed they follow retail's
// control flow, with the Win32 work done by OpenMFC's own CWnd / CGdiObject
// thunks (which are simpler than retail's, e.g. no control-site handling).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// ---- core/window/CWnd.cpp -- CWnd thunks used on the embedded m_Wnd.
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
// core/window/CWnd.cpp -- the CWnd::wndTopMost pseudo-window static (its m_hWnd
// slot holds HWND_TOPMOST); only its address is passed, exactly as retail does
// (AdjustPos loads mfc140u VA 0x1803c3550 = ?wndTopMost@CWnd@@2V1@B).
extern "C" const unsigned char impl__wndTopMost_CWnd__2V1_B[];
// ---- core/gdi/CGdiObject.cpp -- used on the embedded m_rgnBase by Destroy.
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);
// ---- featurepack/docking/CSmartDockingGroupGuide.cpp (the five m_arMarkers).
extern "C" void MS_ABI impl__SetVisible_CSmartDockingGroupGuide__QEAAXHH_Z(
    void* pThis, int bVisible, int bRedraw);
extern "C" void MS_ABI impl__DestroyImages_CSmartDockingGroupGuide__MEAAXXZ(void* pThis);
// ---- featurepack/docking/CSmartDockingGroupGuidesWnd.cpp --
// ?Update@CSmartDockingGroupGuidesWnd@@QEAAXXZ, a non-static member taking only
// `this`.  This declaration follows the mangled name.  NOTE: at the time of
// writing the definition in that file is a placeholder `()` with an empty body
// (no `this` parameter); under the x64 MS ABI a call passing `this` in RCX to it
// is harmless, and the definition's parameter list is reported for repair.
extern "C" void MS_ABI impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(void* pThis);

namespace {

// Retail object layout, sizeof == 0x1a18.
//   * CreateObject (RVA 0x12ee50) and the scalar deleting destructor (RVA
//     0x1303d0, sized-delete path) both use 0x1a18, as does OpenMFC's own
//     descriptor in featurepack/docking/RuntimeClasses.cpp (6680).
//   * The constructor (RVA 0x130340) installs the vftable at +0x0, calls
//     ??0CSmartDockingGroupGuidesWnd (RVA 0x12f7d0) on +0x8, calls the
//     unexported array-construction helper at RVA 0x2b77c0 with (+0x118, 0x4a8,
//     5, ??0CSmartDockingGroupGuide RVA 0x12fc90, ??1CSmartDockingGroupGuide
//     RVA 0x12fd50), calls ??0CMFCToolBarImages
//     (RVA 0x16b0f0) on +0x1860, stores a vftable (VA 0x1802e29e0) at +0x19f8
//     with m_hObject 0 at +0x1a00, and zeroes the qword at +0x1a08 and the dword
//     at +0x1a10.
//   * ??0CSmartDockingGroupGuidesWnd stores m_pCentralGroup at wnd+0xe8 and
//     builds its two CBrush members at wnd+0xf0 / wnd+0x100 (CBrush vftable VA
//     0x1802dde08), i.e. this+0xf8 / this+0x108; the destructor destroys those
//     two at exactly those addresses, so m_Wnd spans +0x8 .. +0x118.
//   * Create writes m_Wnd.m_pCentralGroup at +0xf0, m_bLayered at +0x1a10 and
//     m_bCreated at +0x1a08; Destroy tests and clears +0x1a08; AdjustPos reads and
//     writes m_bMiddleIsOn at +0x1a0c.
// The declaration order in afxsmartdockingguide.h:200-209 (m_Wnd .. m_bLayered)
// matches these offsets.
struct SdGroupMgrLayout {
    void*         vfptr;                   // +0x0000
    unsigned char m_Wnd[0x110];            // +0x0008  CSmartDockingGroupGuidesWnd (CWnd, m_hWnd at +0x40)
    unsigned char m_arMarkers[5][0x4a8];   // +0x0118  CSmartDockingGroupGuide[5]
    unsigned char m_Image[0x198];          // +0x1860  CMFCToolBarImages
    void*         m_rgnBase_vfptr;         // +0x19f8  CRgn
    HRGN          m_rgnBase_hObject;       // +0x1a00  CRgn::m_hObject
    BOOL          m_bCreated;              // +0x1a08
    BOOL          m_bMiddleIsOn;           // +0x1a0c
    BOOL          m_bLayered;              // +0x1a10
    int           _pad1a14;                // +0x1a14
};
static_assert(offsetof(SdGroupMgrLayout, m_Wnd) == 0x08, "m_Wnd +0x08");
static_assert(offsetof(SdGroupMgrLayout, m_arMarkers) == 0x118, "m_arMarkers +0x118");
static_assert(offsetof(SdGroupMgrLayout, m_Image) == 0x1860, "m_Image +0x1860");
static_assert(offsetof(SdGroupMgrLayout, m_rgnBase_vfptr) == 0x19f8, "m_rgnBase +0x19f8");
static_assert(offsetof(SdGroupMgrLayout, m_rgnBase_hObject) == 0x1a00, "m_rgnBase.m_hObject +0x1a00");
static_assert(offsetof(SdGroupMgrLayout, m_bCreated) == 0x1a08, "m_bCreated +0x1a08");
static_assert(offsetof(SdGroupMgrLayout, m_bMiddleIsOn) == 0x1a0c, "m_bMiddleIsOn +0x1a0c");
static_assert(offsetof(SdGroupMgrLayout, m_bLayered) == 0x1a10, "m_bLayered +0x1a10");
static_assert(sizeof(SdGroupMgrLayout) == 0x1a18, "CreateObject allocates 0x1a18");

// Retail reads m_Wnd.m_hWnd at this+0x48 (GetWindowRect, AdjustPos) or as
// 0x40(&m_Wnd) (Show), i.e. m_Wnd + CWnd::m_hWnd.  The CWnd thunks read it
// through OpenMFC's CWnd, which keeps it at the same offset.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");
// Destroy passes this+0x19f8 (m_rgnBase) to CGdiObject::DeleteObject, which
// touches only m_hObject (+0x8); OpenMFC's CGdiObject keeps it there too.
static_assert(offsetof(CGdiObject, m_hObject) == 0x08, "CGdiObject::m_hObject +0x08");

// CSmartDockingGroupGuide::m_bVisible, read by ShowGuide as 0x4a0(marker); the
// same offset is pinned in SdGroupGuideLayout (CSmartDockingGroupGuide.cpp).
constexpr std::size_t kMarkerBVisible = 0x4a0;

inline SdGroupMgrLayout* Mgr(void* pThis) { return static_cast<SdGroupMgrLayout*>(pThis); }
inline CWnd* Wnd(void* pThis) { return reinterpret_cast<CWnd*>(Mgr(pThis)->m_Wnd); }
inline HWND WndHwnd(void* pThis) { return Wnd(pThis)->m_hWnd; }

// MSVC-vftable dispatch on `this` (slot 9, GetGuide).  `this` may be a client
// class derived from CSmartDockingGroupGuidesManager that overrides GetGuide,
// so this call is kept virtual as in retail.
inline void* VGetGuide(void* pThis, int nMarkerNo) {
    using Fn = void* (MS_ABI*)(void*, int);
    return reinterpret_cast<Fn>((*static_cast<void***>(pThis))[9])(pThis, nMarkerNo);
}

} // namespace

// Symbol: ??0CSmartDockingGroupGuidesManager@@QEAA@XZ
// STUB (not on this pass's list; documented because several bodies below
// depend on it).  Retail RVA 0x130340 constructs the members listed at
// SdGroupMgrLayout.  Not reproduced: OpenMFC has no MSVC vftable for this class,
// and the CSmartDockingGroupGuidesWnd / CSmartDockingGroupGuide constructor
// thunks it would call are themselves stubs.
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuidesManager__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CSmartDockingGroupGuidesManager@@UEAA@XZ
// STUB.  Retail (RVA 0x130420) re-installs the class vftable (VA 0x18030f618),
// calls Destroy (RVA 0x1307e0) directly, then destroys the members in reverse
// order: m_rgnBase (+0x19f8: stores VA 0x1802e29e0 and calls the unexported
// ~CGdiObject at RVA 0x1c6f0), ??1CMFCToolBarImages (RVA 0x16b550) on m_Image
// (+0x1860), the unexported array-destruction helper at RVA 0x2b7840 over
// m_arMarkers (+0x118, 0x4a8 bytes, 5 elements, ??1CSmartDockingGroupGuide at
// RVA 0x12fd50), m_Wnd's two
// CBrush members (+0x108 then +0xf8: CBrush vftable VA 0x1802dde08, then
// RVA 0x1c6f0), and tail-jumps to ??1CWnd (RVA 0x28b740) on m_Wnd (+0x8).
// Not reproduced: the constructor above is a stub that constructs none of those
// members (m_bCreated included, which Destroy tests), so running Destroy and the
// member destructors here would act on uninitialised memory.
extern "C" void* MS_ABI impl___1CSmartDockingGroupGuidesManager__UEAA_XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?AdjustPos@CSmartDockingGroupGuidesManager@@UEAAHVCRect@@H@Z
// Transcribed from retail RVA 0x1308f0.  CRect by value arrives as a pointer to
// a caller-owned copy (RDX).  IAT slots resolved with iatu.py: 0x1802c7330 =
// USER32!GetClientRect, 0x1802c6c08 = USER32!GetWindowRect.
//     if (m_Wnd.GetSafeHwnd() == NULL) return FALSE;
//     if (nMiddleIsOn == 0) {
//         if (m_bMiddleIsOn != 0) { m_bMiddleIsOn = 0; m_Wnd.Update(); }
//     } else if (nMiddleIsOn == 1) {
//         if (m_bMiddleIsOn == 0) { m_bMiddleIsOn = 1; m_Wnd.Update(); }
//     }                                   // -1 and any other value: no change
//     CRect rcClient; ::GetClientRect(m_Wnd.m_hWnd, &rcClient);
//     x = (rcHost.left + rcHost.right - rcClient.Width()) >> 1;    // sar
//     y = (rcHost.top + rcHost.bottom - rcClient.Height()) >> 1;   // sar
//     CRect rcWnd; ::GetWindowRect(m_Wnd.m_hWnd, &rcWnd);
//     if (rcWnd.left == x && rcWnd.top == y) return FALSE;
//     m_Wnd.SetWindowPos(&CWnd::wndTopMost, x, y, -1, -1, SWP_NOSIZE);
//     return TRUE;
// Update is the call to RVA 0x12f960 (?Update@CSmartDockingGroupGuidesWnd@@QEAAXXZ)
// on &m_Wnd; SetWindowPos is the call to RVA 0x2a9a60
// (?SetWindowPos@CWnd@@QEAAHPEBV1@HHHHI@Z) with the stack arguments -1, -1, 1;
// its result is ignored.  The GetSafeHwnd line is retail's `test %rdi,%rdi`
// (&m_Wnd != NULL) plus `cmp %rax,0x40(%rdi)` (m_hWnd != NULL); &m_Wnd cannot be
// NULL for a real object, so only the m_hWnd test is written out.  The two
// halvings are arithmetic shifts in retail (no rounding toward zero), and so
// are these (`>>` on a signed int).
extern "C" int MS_ABI impl__AdjustPos_CSmartDockingGroupGuidesManager__UEAAHVCRect__H_Z(
    void* pThis, const RECT* prcHost, int nMiddleIsOn) {
    SdGroupMgrLayout* m = Mgr(pThis);
    if (WndHwnd(pThis) == nullptr) {
        return FALSE;
    }
    if (nMiddleIsOn == 0) {
        if (m->m_bMiddleIsOn != 0) {
            m->m_bMiddleIsOn = 0;
            impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(m->m_Wnd);
        }
    } else if (nMiddleIsOn == 1) {
        if (m->m_bMiddleIsOn == 0) {
            m->m_bMiddleIsOn = 1;
            impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(m->m_Wnd);
        }
    }

    RECT rcClient = {0, 0, 0, 0};
    ::GetClientRect(WndHwnd(pThis), &rcClient);
    const int x = (prcHost->right - rcClient.right + rcClient.left + prcHost->left) >> 1;
    const int y = (prcHost->bottom + prcHost->top - rcClient.bottom + rcClient.top) >> 1;

    RECT rcWnd = {0, 0, 0, 0};
    ::GetWindowRect(WndHwnd(pThis), &rcWnd);
    if (rcWnd.left == x && rcWnd.top == y) {
        return FALSE;
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
        Wnd(pThis), reinterpret_cast<const CWnd*>(impl__wndTopMost_CWnd__2V1_B),
        x, y, -1, -1, SWP_NOSIZE);
    return TRUE;
}

// Symbol: ?Create@CSmartDockingGroupGuidesManager@@UEAAXPEAVCWnd@@@Z
// STUB.  Retail (RVA 0x1304c0), in outline -- IAT slots resolved with iatu.py:
//   * returns at once if m_bCreated (+0x1a08);
//   * builds a local CRgn from ::CreateRectRgn(0,0,0,0) and, for i = sdCLEFT (4)
//     .. sdCMIDDLE (8), calls m_arMarkers[i-4] vftable +0x60 (slot 12, the
//     protected Create(SDMarkerPlace, CSmartDockingGroupGuidesManager*), RVA
//     0x12fe50) with (i, this), then ::CombineRgn(rgn, rgn, marker.m_Rgn, RGN_OR);
//   * picks the base image resource id: CDockingManager::m_SDParams.m_uiBaseBmpResID
//     (VA 0x1803c1600 = m_SDParams 0x1803c15b0 + 0x50) if non-zero, else 0x42e7
//     when the unexported Theme() helper (RVA 0x12ee88) returns 2, else none; with
//     an id it clears the dword at m_Image+0x38, stores 1 at +0x189c
//     (m_Image+0x3c), calls CMFCToolBarImages::LoadStr (RVA 0x16b6c0) and
//     SetSingleImage (RVA 0x171320) on m_Image and copies m_SDParams.m_clrTransparent
//     (+0x14) into +0x1938 (m_Image+0xd8) if different;
//   * ::GetRgnBox, ::InflateRect by -m_SDParams.m_nCentralGroupOffset (+0x10),
//     builds the diamond (left, midY) (midX, bottom) (right, midY) (midX, top)
//     (signed /2 halvings), ::CreatePolygonRgn(pts, 4, ALTERNATE) attached to
//     m_rgnBase (CGdiObject::Attach, RVA 0x2a3ed0), ::CombineRgn(rgn, rgn,
//     m_rgnBase, RGN_OR), ::GetRgnBox again for the window rectangle;
//   * m_Wnd.CreateEx (vftable +0xc0, slot 24) with WS_EX_TOPMOST (8), the class
//     from AfxRegisterWndClass(0x820 = CS_SAVEBITS|CS_OWNDC, 0, 0, 0) (RVA
//     0x28c4a0), the string at VA 0x18033d19c, WS_POPUP, the rectangle,
//     pwndOwner, id 0, NULL;
//   * on success: m_Wnd.m_pCentralGroup (+0xf0) = this;
//     m_Wnd.ModifyStyleEx(0, WS_EX_LAYERED, 0) (RVA 0x2a9740); then, when
//     !m_SDParams.m_bIsAlphaMarkers (+0x58) and Theme() != 2, it initialises
//     afxGlobalData if needed (AFX_GLOBAL_DATA::Initialize, RVA 0x6a790) and calls
//     ::SetLayeredWindowAttributes(m_Wnd.m_hWnd, m_clrTransparent, 0, LWA_COLORKEY);
//     otherwise m_Wnd.Update() (RVA 0x12f960); then m_bLayered = m_bCreated = 1;
//   * the local CRgn is destroyed.
// Not reproduced: OpenMFC's exported CDockingManager::m_SDParams is an 8-byte
// placeholder, not a CSmartDockingInfo (see CSmartDockingGroupGuide.cpp); Theme()
// needs the unexported m_SDTheme; and the per-marker Create it relies on is itself
// a stub in CSmartDockingGroupGuide.cpp.
extern "C" void MS_ABI impl__Create_CSmartDockingGroupGuidesManager__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pwndOwner) {
    (void)pThis; (void)pwndOwner;
}

// Symbol: ?CreateObject@CSmartDockingGroupGuidesManager@@SAPEAVCObject@@XZ
// STUB.  Retail (RVA 0x12ee50) is `new(0x1a18) CSmartDockingGroupGuidesManager`
// (operator new at RVA 0x27f0, then the constructor at RVA 0x130340 when the
// allocation succeeded).  OpenMFC's constructor above is still a stub that
// installs no vftable and constructs no member, so a returned object would have
// no working virtual dispatch; nullptr (the allocation-failure result) is
// returned instead, as CSmartDockingStandaloneGuide::CreateObject does.
extern "C" void* MS_ABI impl__CreateObject_CSmartDockingGroupGuidesManager__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?Destroy@CSmartDockingGroupGuidesManager@@UEAAXXZ
// Transcribed from retail RVA 0x1307e0:
//     if (!m_bCreated) return;
//     for (i = sdCLEFT; i <= sdCMIDDLE; ++i)
//         m_arMarkers[i - sdCLEFT].DestroyImages();   // marker vftable +0x68 (slot 13)
//     m_Wnd.DestroyWindow();                           // m_Wnd vftable +0xd0 (slot 26)
//     m_rgnBase.DeleteObject();                        // call to RVA 0x2a3f60
//     m_bCreated = FALSE;
// Deviation: retail dispatches both member calls virtually.  Both objects are
// embedded members whose dynamic type is fixed by this class: slot 13 of the
// CSmartDockingGroupGuide vftable (VA 0x18030f670) is
// ?DestroyImages@CSmartDockingGroupGuide@@MEAAXXZ (RVA 0x130010), and slot 26 of
// the CSmartDockingGroupGuidesWnd vftable (VA 0x18030f788, installed by its
// constructor at RVA 0x12f7d0) is ?DestroyWindow@CWnd@@UEAAHXZ (RVA 0x28baf0).
// The thunks are therefore called directly, which does not depend on the
// (unmodelled) vftables being installed.  Retail calls DestroyWindow without
// an ::IsWindow test; so does this body.
extern "C" void MS_ABI impl__Destroy_CSmartDockingGroupGuidesManager__UEAAXXZ(void* pThis) {
    SdGroupMgrLayout* m = Mgr(pThis);
    if (m->m_bCreated == 0) {
        return;
    }
    for (int i = 0; i < 5; ++i) {
        impl__DestroyImages_CSmartDockingGroupGuide__MEAAXXZ(m->m_arMarkers[i]);
    }
    impl__DestroyWindow_CWnd__UEAAHXZ(Wnd(pThis));
    impl__DeleteObject_CGdiObject__QEAAHXZ(reinterpret_cast<CGdiObject*>(&m->m_rgnBase_vfptr));
    m->m_bCreated = FALSE;
}

// Symbol: ?DrawCentralGroupGuides@CSmartDockingGroupGuidesManager@@IEAAXAEAVCDC@@AEAVCBrush@@1VCRect@@@Z
// STUB.  Retail (RVA 0x130a70), IAT slots resolved with iatu.py:
//   * bAlpha = CDockingManager::m_SDParams.m_bIsAlphaMarkers (VA 0x1803c1608 =
//     m_SDParams + 0x58) != 0 || Theme() == 2 (unexported helper, RVA 0x12ee88);
//   * attaches ::CreateCompatibleDC(dc.m_hDC) to a local CDC (CDC::Attach, RVA
//     0x2a2480), which is not otherwise used and is destroyed at the end (??1CDC,
//     RVA 0x2a2560);
//   * if !bAlpha: ::GetBoundsRect(dc.m_hAttribDC, &rc, 0) and ::FillRect(dc.m_hDC,
//     &rc, CBrush(::CreateSolidBrush(m_SDParams.m_clrTransparent)));
//   * if m_Image's bitmap (+0x1860+0xa0) is non-NULL: m_Image.DrawEx(&dc,
//     rectClient, 0, 1, 1, CRect(0,0,0,0), bAlpha ? 0xc0 : 0xff) (RVA 0x16d0b0);
//   * otherwise ::FillRgn(dc.m_hDC, m_rgnBase, brBaseBackground), then
//     ::FrameRgn(dc.m_hDC, m_rgnBase, <brush>, 1, 1) with a temporary
//     CBrush(RGB(0x41,0x70,0xca)) (??0CBrush@@QEAA@K@Z, RVA 0x2a4060) when
//     m_bMiddleIsOn && m_SDParams.m_uiMarkerBmpResID[0] (+0x28) == 0 &&
//     m_arMarkers[4].m_bVisible (+0x1858) && m_arMarkers[4].m_bHiLited (+0x1810),
//     else with brBaseBorder;
//   * for i = sdCLEFT .. (m_bMiddleIsOn ? sdCMIDDLE : sdCBOTTOM): if
//     m_arMarkers[i-4].m_bVisible, marker vftable +0x70 (slot 14, Draw, RVA
//     0x130020) with (dc, bAlpha).
// Not reproduced: Theme() needs the unexported m_SDTheme and OpenMFC's exported
// CDockingManager::m_SDParams is an 8-byte placeholder, not a CSmartDockingInfo;
// the per-marker Draw is itself a stub in CSmartDockingGroupGuide.cpp.
extern "C" void MS_ABI impl__DrawCentralGroupGuides_CSmartDockingGroupGuidesManager__IEAAXAEAVCDC__AEAVCBrush__1VCRect___Z(
    void* pThis, CDC* pDC, CBrush* pbrBaseBackground, CBrush* pbrBaseBorder, const RECT* prectClient) {
    (void)pThis; (void)pDC; (void)pbrBaseBackground; (void)pbrBaseBorder; (void)prectClient;
}

// Symbol: ?GetWindowRect@CSmartDockingGroupGuidesManager@@QEAAXAEAVCRect@@@Z
// Transcribed from retail RVA 0x1308a0.  IAT slots resolved with iatu.py:
// 0x1802c7348 = USER32!SetRectEmpty, 0x1802c6c08 = USER32!GetWindowRect.
//     ::SetRectEmpty(&rect);
//     if (m_Wnd.GetSafeHwnd() != NULL)
//         ::GetWindowRect(m_Wnd.m_hWnd, &rect);
// (retail's `test %rax,%rax` on &m_Wnd is the inlined GetSafeHwnd NULL-this test,
// which cannot fire for a real object; only the m_hWnd test is written out.)
extern "C" void MS_ABI impl__GetWindowRect_CSmartDockingGroupGuidesManager__QEAAXAEAVCRect___Z(
    void* pThis, RECT* pRect) {
    ::SetRectEmpty(pRect);
    if (WndHwnd(pThis) != nullptr) {
        ::GetWindowRect(WndHwnd(pThis), pRect);
    }
}

// Symbol: ?Show@CSmartDockingGroupGuidesManager@@UEAAXH@Z
// Transcribed from retail RVA 0x130860 (resolved through the export table).  IAT
// slot 0x1802c7138 = USER32!IsWindow (iatu.py); the callee at RVA 0x2a9ad0 is
// ?ShowWindow@CWnd@@QEAAHH@Z (uwho.py).
//     if (::IsWindow(m_Wnd.m_hWnd))
//         m_Wnd.ShowWindow(bShow ? SW_SHOW : SW_HIDE);   // neg/sbb/and $5
extern "C" void MS_ABI impl__Show_CSmartDockingGroupGuidesManager__UEAAXH_Z(void* pThis, int bShow) {
    if (::IsWindow(WndHwnd(pThis))) {
        impl__ShowWindow_CWnd__QEAAHH_Z(Wnd(pThis), bShow != 0 ? SW_SHOW : SW_HIDE);
    }
}

// Symbol: ?ShowGuide@CSmartDockingGroupGuidesManager@@QEAAXW4SDMarkerPlace@CSmartDockingStandaloneGuide@@HH@Z
// Transcribed from retail RVA 0x130a20 (resolved through the export table):
//     CSmartDockingGroupGuide* p = GetGuide(nMarkerNo);   // this vftable +0x48 (slot 9)
//     if (p != NULL && p->m_bVisible != bShow)            // cmp %ebx,0x4a0(%rax)
//         p->SetVisible(bShow, bRedraw);                  // call to RVA 0x12fdd0
// GetGuide stays a virtual call, as in retail (see VGetGuide): on an object
// whose vftable was installed by MSVC-compiled code (a client class derived
// from this one) slot 9 is that class's GetGuide.  On an object built through
// OpenMFC's stub constructor there is no vftable, and this call is as unsafe as
// every other virtual call on it (CSmartDockingManager::Create makes the same
// slot-9 call).
extern "C" void MS_ABI impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
    void* pThis, int nMarkerNo, int bShow, int bRedraw) {
    void* const pGuide = VGetGuide(pThis, nMarkerNo);
    if (pGuide != nullptr &&
        *reinterpret_cast<const BOOL*>(static_cast<unsigned char*>(pGuide) + kMarkerBVisible) != bShow) {
        impl__SetVisible_CSmartDockingGroupGuide__QEAAXHH_Z(pGuide, bShow, bRedraw);
    }
}
