// CSmartDockingGroupGuide — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// One of the five markers of the central smart-docking "diamond".  Retail
// declaration: atlmfc/include/afxsmartdockingguide.h:134 (class
// CSmartDockingGroupGuide : public CSmartDockingStandaloneGuide).  OpenMFC
// declares no matching class, so every body below works on `void* pThis`
// through the layout pinned in SdGroupGuideLayout.  The base part reuses the
// CSmartDockingStandaloneGuide layout pinned in CSmartDockingStandaloneGuide.cpp
// verbatim; the derived members were read from the retail constructor
// (RVA 0x12fc90 mfc140u), destructor (RVA 0x12fd50 mfc140u) and Create
// (RVA 0x12fe50 mfc140u).  The mfc140.dll bodies (ctor 0x130910, dtor 0x1309d0)
// are instruction-for-instruction identical.
//
// The group guide overrides Create(SDMarkerPlace, CWnd*), Destroy, Show and
// AdjustPos with EMPTY bodies: slots 5..8 of its vftable (mfc140u VA
// 0x18030f670, the one the constructor installs) all point at RVA 0x27d0
// (mfc140u), a shared `ret 0` body (identical-COMDAT-folded; the symbol map
// names it ?AddDockSite@CFrameWndEx@@QEAAXXZ).  The four exports resolve to that
// same RVA through the export table.  In the base vftable (installed by
// ??0CSmartDockingStandaloneGuide; checked in mfc140.dll, VA 0x18030d9b8 in THAT
// image) those slots are Create / Destroy / Show / AdjustPos, so the slot
// mapping is established from the retail tables, not assumed.  Each of those
// four bodies is therefore complete as written.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// core/gdi/CGdiObject.cpp -- used on the embedded m_Rgn by DestroyImages.
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);
// featurepack/docking/CSmartDockingGroupGuidesWnd.cpp --
// ?Update@CSmartDockingGroupGuidesWnd@@QEAAXXZ, a non-static member taking only
// `this`.  This declaration follows the mangled name.  NOTE: at the time of
// writing the definition in that file is a placeholder `()` with an empty body
// (no `this` parameter); under the x64 MS ABI a call passing `this` in RCX to it
// is harmless, and the definition's parameter list is reported for repair.
extern "C" void MS_ABI impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(void* pThis);

namespace {

// Retail object layout, sizeof == 0x4a8.
//   * The scalar deleting destructor (vftable slot 1, unexported; RVA 0x12fd00
//     mfc140u, 0x130980 in mfc140.dll) passes the size 0x4a8 on its sized-delete
//     path.
//   * The constructor (RVA 0x12fc90 mfc140u) calls ??0CSmartDockingStandaloneGuide
//     (RVA 0x12efc0 mfc140u), stores m_clrFrame = -1 at +0x470, installs the class
//     vftable, zeroes m_pCentralGroup at +0x468, constructs the two CPen members
//     inline (CPen vftable mfc140u VA 0x1802e0d70 at +0x478 / +0x488, m_hObject 0
//     at +0x480 / +0x490) and sets m_bVisible = 1 at +0x4a0.
//   * Create (RVA 0x12fe50 mfc140u) stores m_nOffsetX / m_nOffsetY at +0x498 /
//     +0x49c.
// The declaration order in afxsmartdockingguide.h:157-163 matches these offsets.
struct SdGroupGuideLayout {
    // ---- CSmartDockingStandaloneGuide part (sizeof 0x468), copied verbatim
    // ---- from SdStandaloneGuideLayout in CSmartDockingStandaloneGuide.cpp.
    void*         vfptr;                 // +0x000
    int           m_nSideNo;             // +0x008  SDMarkerPlace
    int           _pad0c;                // +0x00c
    unsigned char m_wndBmp[0x100];       // +0x010  CSmartDockingStandaloneGuideWnd (CWnd, m_hWnd at +0x40)
    unsigned char m_Image[0x198];        // +0x110  CMFCToolBarImages
    unsigned char m_ImageHot[0x198];     // +0x2a8  CMFCToolBarImages
    void*         m_Rgn_vfptr;           // +0x440  CRgn
    HRGN          m_Rgn_hObject;         // +0x448  CRgn::m_hObject
    int           m_cx;                  // +0x450
    int           m_cy;                  // +0x454
    BOOL          m_bHiLited;            // +0x458
    BOOL          m_bLayered;            // +0x45c
    BOOL          m_bIsDefaultImage;     // +0x460
    int           _pad464;               // +0x464
    // ---- CSmartDockingGroupGuide members.
    void*         m_pCentralGroup;       // +0x468  CSmartDockingGroupGuidesManager*
    COLORREF      m_clrFrame;            // +0x470
    int           _pad474;               // +0x474
    void*         m_penFrame_vfptr;      // +0x478  CPen
    HGDIOBJ       m_penFrame_hObject;    // +0x480
    void*         m_penHighlight_vfptr;  // +0x488  CPen
    HGDIOBJ       m_penHighlight_hObject;// +0x490
    int           m_nOffsetX;            // +0x498
    int           m_nOffsetY;            // +0x49c
    BOOL          m_bVisible;            // +0x4a0
    int           _pad4a4;               // +0x4a4
};
static_assert(offsetof(SdGroupGuideLayout, m_nSideNo) == 0x08, "m_nSideNo +0x08");
static_assert(offsetof(SdGroupGuideLayout, m_Rgn_vfptr) == 0x440, "m_Rgn +0x440");
static_assert(offsetof(SdGroupGuideLayout, m_Rgn_hObject) == 0x448, "m_Rgn.m_hObject +0x448");
static_assert(offsetof(SdGroupGuideLayout, m_bHiLited) == 0x458, "m_bHiLited +0x458");
static_assert(offsetof(SdGroupGuideLayout, m_bIsDefaultImage) == 0x460, "m_bIsDefaultImage +0x460");
static_assert(offsetof(SdGroupGuideLayout, m_pCentralGroup) == 0x468, "m_pCentralGroup +0x468");
static_assert(offsetof(SdGroupGuideLayout, m_clrFrame) == 0x470, "m_clrFrame +0x470");
static_assert(offsetof(SdGroupGuideLayout, m_penFrame_vfptr) == 0x478, "m_penFrame +0x478");
static_assert(offsetof(SdGroupGuideLayout, m_penHighlight_vfptr) == 0x488, "m_penHighlight +0x488");
static_assert(offsetof(SdGroupGuideLayout, m_nOffsetX) == 0x498, "m_nOffsetX +0x498");
static_assert(offsetof(SdGroupGuideLayout, m_nOffsetY) == 0x49c, "m_nOffsetY +0x49c");
static_assert(offsetof(SdGroupGuideLayout, m_bVisible) == 0x4a0, "m_bVisible +0x4a0");
static_assert(sizeof(SdGroupGuideLayout) == 0x4a8, "sized delete of 0x4a8");

// CSmartDockingGroupGuidesManager (afxsmartdockingguide.h:168, : public CObject)
// begins with its vfptr followed by `CSmartDockingGroupGuidesWnd m_Wnd`.  Retail
// reaches m_Wnd as m_pCentralGroup + 0x8 (the `add $0x8,%rcx` before the Update
// call in Highlight / SetVisible) and m_Wnd.m_hWnd as m_pCentralGroup + 0x48
// (IsPtIn), i.e. m_Wnd + CWnd::m_hWnd (+0x40).
constexpr std::size_t kMgrWnd = 0x08;
constexpr std::size_t kMgrWndHwnd = 0x48;
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");
static_assert(kMgrWndHwnd == kMgrWnd + 0x40, "m_Wnd.m_hWnd = m_Wnd + CWnd::m_hWnd");

// DestroyImages passes this+0x440 (m_Rgn) to CGdiObject::DeleteObject, which
// touches only m_hObject (+0x8; retail RVA 0x2a3f60 mfc140u tests it, then
// Detaches and ::DeleteObject's it); OpenMFC's CGdiObject keeps it at the same
// offset.
static_assert(offsetof(CGdiObject, m_hObject) == 0x08, "CGdiObject::m_hObject +0x08");

inline SdGroupGuideLayout* Guide(void* pThis) {
    return static_cast<SdGroupGuideLayout*>(pThis);
}
inline const SdGroupGuideLayout* Guide(const void* pThis) {
    return static_cast<const SdGroupGuideLayout*>(pThis);
}
inline void* CentralWnd(const SdGroupGuideLayout* g) {
    return static_cast<unsigned char*>(g->m_pCentralGroup) + kMgrWnd;
}
inline HWND CentralWndHwnd(const SdGroupGuideLayout* g) {
    return *reinterpret_cast<const HWND*>(
        static_cast<const unsigned char*>(g->m_pCentralGroup) + kMgrWndHwnd);
}

} // namespace

// Symbol: ??0CSmartDockingGroupGuide@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuide__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CSmartDockingGroupGuide@@UEAA@XZ
// STUB.  Retail (RVA 0x12fd50 mfc140u) re-installs the class vftable (mfc140u
// VA 0x18030f670), destroys m_penHighlight (+0x488) and then m_penFrame (+0x478)
// -- each by storing the CPen vftable and calling the unexported ~CGdiObject at
// RVA 0x1c6f0 (mfc140u), which installs the CGdiObject vftable and calls
// CGdiObject::DeleteObject -- and tail-jumps to ??1CSmartDockingStandaloneGuide
// (RVA 0x12f090 mfc140u).  Not reproduced: the constructor above is still a stub
// that constructs none of those members, and ??1CSmartDockingStandaloneGuide is
// itself a stub for the same reason, so running the member destructors here
// would act on uninitialised memory.
extern "C" void* MS_ABI impl___1CSmartDockingGroupGuide__UEAA_XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?AdjustPos@CSmartDockingGroupGuide@@UEAAXVCRect@@@Z
// Retail body is empty: the export and vftable slot 8 both resolve to the shared
// `ret 0` at RVA 0x27d0 (mfc140u); see the file header.  The group's position is
// driven by CSmartDockingGroupGuidesManager::AdjustPos instead.  CRect by value
// arrives as a pointer to a caller-owned copy (RDX).
extern "C" void MS_ABI impl__AdjustPos_CSmartDockingGroupGuide__UEAAXVCRect___Z(
    void* pThis, const RECT* prcHost) {
    (void)pThis; (void)prcHost;
}

// Symbol: ?Create@CSmartDockingGroupGuide@@MEAAXW4SDMarkerPlace@CSmartDockingStandaloneGuide@@PEAVCSmartDockingGroupGuidesManager@@@Z
// STUB.  Retail (RVA 0x12fe50 mfc140u):
//   * m_nSideNo = nSideNo; m_pCentralGroup = pCentralGroup;
//   * InitImages(CDockingManager::m_SDParams) through this vftable +0x58
//     (slot 11), passing mfc140u VA 0x1803c15b0;
//   * if m_bIsDefaultImage: switch on the unexported Theme() helper (RVA
//     0x12ee88 mfc140u; described at CSmartDockingStandaloneGuide::Create):
//     Theme()==1 writes 0x58 to the two ints at mfc140u VA 0x1803c15b8 /
//     0x1803c15bc and 9 to 0x1803c15c0; Theme()==2 writes 0x6e / 0x6e / 5;
//     other values write nothing (these are m_SDParams + 0x8 / +0xc / +0x10);
//   * calls the unexported CMFCVisualManager singleton getter (RVA 0x9774
//     mfc140u) and its vftable +0x3b0 (slot 118) with (&local, &m_clrFrame):
//     in the CMFCVisualManager vftable (mfc140u VA 0x18031c128, installed by
//     ??0CMFCVisualManager@@QEAA@H@Z, RVA 0x184030 mfc140u) slot 118 is
//     GetSmartDockingBaseGuideColors(COLORREF& clrBaseGroupBackground,
//     COLORREF& clrBaseGroupBorder) (RVA 0x18b080 mfc140u), so m_clrFrame
//     receives the base-group BORDER colour and the local is discarded;
//   * m_penFrame.Attach(::CreatePen(PS_SOLID /*0*/, 1, m_clrFrame));
//     m_penHighlight.Attach(::CreatePen(PS_SOLID, 1, RGB(0x41,0x70,0xca)));
//   * computes m_nOffsetX / m_nOffsetY from m_SDParams.m_sizeTotal (W, H at
//     m_SDParams + 0x8 / +0xc; see detail/CSmartDockingInfoSupport.h) and the
//     8-byte image size (w, h) at this+0x178 (m_Image + 0x68), per side:
//     sdCLEFT (4): X = 0, Y = (H - h)/2; sdCRIGHT (5): X = W - w, Y = (H - h)/2;
//     sdCTOP (6): X = (W - w)/2, Y = 0; sdCBOTTOM (7): X = (W - w)/2, Y = H - h;
//     sdCMIDDLE (8): X = (W - w)/2, Y = (H - h)/2; other sides leave both;
//     (the halvings are signed cltd/sub/sar divisions by 2);
//   * m_Rgn.Attach(CMFCToolBarImages::CreateRegionFromImage(m_Image.m_hbmImageWell
//     (this+0x1b0), the COLORREF at mfc140u VA 0x1803c15c4 = m_SDParams + 0x14));
//   * ::OffsetRgn(m_Rgn.m_hObject, m_nOffsetX, m_nOffsetY).
// Not reproduced: OpenMFC's exported CDockingManager::m_SDParams is an 8-byte
// placeholder, not a CSmartDockingInfo (see CDockingManager.cpp); Theme() needs
// the unexported m_SDTheme; and OpenMFC has no vftable for this class to
// dispatch InitImages through.
extern "C" void MS_ABI impl__Create_CSmartDockingGroupGuide__MEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__PEAVCSmartDockingGroupGuidesManager___Z(
    void* pThis, int nSideNo, void* pCentralGroup) {
    (void)pThis; (void)nSideNo; (void)pCentralGroup;
}

// Symbol: ?Create@CSmartDockingGroupGuide@@UEAAXW4SDMarkerPlace@CSmartDockingStandaloneGuide@@PEAVCWnd@@@Z
// Retail body is empty: the export and vftable slot 5 both resolve to the shared
// `ret 0` at RVA 0x27d0 (mfc140u); see the file header.  Group markers are
// created through the protected Create(SDMarkerPlace, CSmartDockingGroupGuidesManager*)
// overload above.
extern "C" void MS_ABI impl__Create_CSmartDockingGroupGuide__UEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__PEAVCWnd___Z(
    void* pThis, int nSideNo, CWnd* pwndOwner) {
    (void)pThis; (void)nSideNo; (void)pwndOwner;
}

// Symbol: ?Destroy@CSmartDockingGroupGuide@@UEAAXXZ
// Retail body is empty: the export and vftable slot 6 both resolve to the shared
// `ret 0` at RVA 0x27d0 (mfc140u); see the file header.  A group marker owns no
// window of its own (the manager's m_Wnd paints all five).
extern "C" void MS_ABI impl__Destroy_CSmartDockingGroupGuide__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?DestroyImages@CSmartDockingGroupGuide@@MEAAXXZ
// Transcribed from retail RVA 0x130010 (mfc140u): `add $0x440,%rcx; jmp
// ?DeleteObject@CGdiObject@@QEAAHXZ` (RVA 0x2a3f60 mfc140u), i.e.
//     m_Rgn.DeleteObject();
extern "C" void MS_ABI impl__DestroyImages_CSmartDockingGroupGuide__MEAAXXZ(void* pThis) {
    impl__DeleteObject_CGdiObject__QEAAHXZ(
        reinterpret_cast<CGdiObject*>(&Guide(pThis)->m_Rgn_vfptr));
}

// Symbol: ?Draw@CSmartDockingGroupGuide@@MEAAXAEAVCDC@@H@Z
// STUB.  Retail (RVA 0x130020 mfc140u) picks the image to draw (m_ImageHot at
// +0x2a8 when m_bHiLited and m_ImageHot.m_hbmImageWell (+0x348) is non-NULL,
// else m_Image at +0x110; an "inactive" flag is set when neither m_bHiLited,
// m_bIsDefaultImage nor m_ImageHot's bitmap is set).  When bAlpha && !m_bHiLited
// it calls CMFCToolBarImages::DrawEx (RVA 0x16d0b0 mfc140u) with the rectangle
// (m_nOffsetX, m_nOffsetY, m_nOffsetX + w, m_nOffsetY + h) -- w/h from the image
// at +0x68 -- and a final byte argument of 0xc0, and returns.  Otherwise it calls
// PrepareDrawImage(ds, CSize(0,0), inactive) (RVA 0x16c060), Draw(pDC, m_nOffsetX,
// m_nOffsetY, 0, 0, 0, 0, 0, inactive, 0xff) (RVA 0x16c6d0; the flag goes to the
// 9th parameter, bInactive -- NOT bDisabled) and EndDrawImage (RVA 0x16c270), all
// mfc140u,
// and then, only when m_bIsDefaultImage and Theme() != 2, ::GetRgnBox(m_Rgn),
// CDC::SelectObject(m_bHiLited ? &m_penHighlight : &m_penFrame) (RVA 0x2a2730
// mfc140u), and for sdCLEFT..sdCBOTTOM draws a three-segment frame with one end
// inset by 7 pixels (CDC::MoveTo / LineTo, RVAs 0x2a30c0 / 0x2a3120 mfc140u)
// followed by the unexported helper at RVA 0x12eec4 (mfc140u) with (pDC, &rect,
// bVert): bVert is 1 for sdCTOP and sdCBOTTOM, 0 for sdCLEFT and sdCRIGHT (both
// the 6 and 7 paths reach the `mov $0x1,%r8d` at 0x13021d); sdCMIDDLE draws no
// frame and skips the helper; finally the old pen is re-selected.
// The helper (disassembled): ::InflateRect(&rect, -1, -1); then for i = 0, 1 it
// constructs CPen(PS_SOLID, 1, i ? RGB(0xce,0xce,0xce) : RGB(0xc6,0xc6,0xc6)),
// selects it, and draws MoveTo(left+i, top)/LineTo(left+i, bottom) when bVert,
// else MoveTo(left, top+i)/LineTo(right, top+i), then re-selects the old pen.
// Not reproduced: Theme() needs the unexported m_SDTheme and the real
// CDockingManager::m_SDParams (see Create).
extern "C" void MS_ABI impl__Draw_CSmartDockingGroupGuide__MEAAXAEAVCDC__H_Z(
    void* pThis, CDC* pDC, int bAlpha) {
    (void)pThis; (void)pDC; (void)bAlpha;
}

// Symbol: ?Highlight@CSmartDockingGroupGuide@@UEAAXH@Z
// Transcribed from retail RVA 0x12fda0 (mfc140u):
//     if (m_bHiLited != bHiLite) {
//         m_bHiLited = bHiLite;
//         m_pCentralGroup->m_Wnd.Update();   // call to RVA 0x12f960 (mfc140u)
//     }
// Retail does not test m_pCentralGroup for NULL here; neither does this body.
extern "C" void MS_ABI impl__Highlight_CSmartDockingGroupGuide__UEAAXH_Z(void* pThis, int bHiLite) {
    SdGroupGuideLayout* g = Guide(pThis);
    if (g->m_bHiLited != bHiLite) {
        g->m_bHiLited = bHiLite;
        impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(CentralWnd(g));
    }
}

// Symbol: ?IsPtIn@CSmartDockingGroupGuide@@UEBAHVCPoint@@@Z
// Transcribed from retail RVA 0x12fe00 (mfc140u).  CPoint arrives by value in
// RDX as an 8-byte aggregate (x low dword, y high dword); retail spills it and
// passes its address to ScreenToClient.  IAT slots resolved with iatu.py:
// 0x1802c7318 = USER32!ScreenToClient, 0x1802c6210 = GDI32!PtInRegion.
//     if (!m_bVisible) return FALSE;
//     ::ScreenToClient(m_pCentralGroup->m_Wnd.m_hWnd, &point);
//     return ::PtInRegion(m_Rgn.m_hObject, point.x, point.y);
// Retail does not test m_pCentralGroup for NULL; neither does this body.
extern "C" int MS_ABI impl__IsPtIn_CSmartDockingGroupGuide__UEBAHVCPoint___Z(
    const void* pThis, long long point) {
    const SdGroupGuideLayout* g = Guide(pThis);
    if (g->m_bVisible == 0) {
        return FALSE;
    }
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<unsigned long long>(point) & 0xffffffffu);
    pt.y = static_cast<LONG>(static_cast<unsigned long long>(point) >> 32);
    ::ScreenToClient(CentralWndHwnd(g), &pt);
    return ::PtInRegion(g->m_Rgn_hObject, pt.x, pt.y);
}

// Symbol: ?SetVisible@CSmartDockingGroupGuide@@QEAAXHH@Z
// Transcribed from retail RVA 0x12fdd0 (mfc140u; resolved through the export
// table -- mfc140u_rva_symbols.json has no name at that RVA; mfc140.dll 0x130a50):
//     m_bVisible = bVisible;
//     if (bRedraw && m_pCentralGroup != NULL)
//         m_pCentralGroup->m_Wnd.Update();   // call to RVA 0x12f960 (mfc140u)
extern "C" void MS_ABI impl__SetVisible_CSmartDockingGroupGuide__QEAAXHH_Z(
    void* pThis, int bVisible, int bRedraw) {
    SdGroupGuideLayout* g = Guide(pThis);
    g->m_bVisible = bVisible;
    if (bRedraw != 0 && g->m_pCentralGroup != nullptr) {
        impl__Update_CSmartDockingGroupGuidesWnd__QEAAXXZ(CentralWnd(g));
    }
}

// Symbol: ?Show@CSmartDockingGroupGuide@@UEAAXH@Z
// Retail body is empty: the export and vftable slot 7 both resolve to the shared
// `ret 0` at RVA 0x27d0 (mfc140u); see the file header.  Visibility of a group
// marker is SetVisible's m_bVisible, painted by the manager's window.
extern "C" void MS_ABI impl__Show_CSmartDockingGroupGuide__UEAAXH_Z(void* pThis, int bShow) {
    (void)pThis; (void)bShow;
}
