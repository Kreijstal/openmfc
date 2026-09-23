// CSmartDockingStandaloneGuide — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The standalone (side-arrow) smart-docking marker.  Retail declaration:
// atlmfc/include/afxsmartdockingguide.h:58 (class CSmartDockingStandaloneGuide
// : public CObject).  OpenMFC declares no matching class, so every body below
// works on `void* pThis` through the layout pinned in SdStandaloneGuideLayout,
// which was read from the retail constructor / destructor / Create / Highlight /
// InitImages bodies (mfc140.dll).  The mfc140u.dll bodies are instruction-for-
// instruction identical; the raw bytes differ only in rip-relative call / IAT
// displacements, because every function sits at a different RVA in each image.
//
// Bodies marked "transcribed" follow the retail disassembly instruction for
// instruction except where a "Deviation" note says otherwise (Destroy calls the
// CWnd::DestroyWindow thunk directly instead of through the vftable; IsPtIn uses
// a raw HRGN for its temporary CRgn).  Each cites the function ENTRY RVA in the
// image named.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// core/window/CWnd.cpp -- CWnd thunks used on the embedded m_wndBmp window.
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
// core/window/CWnd.cpp -- the CWnd::wndTopMost pseudo-window static (its m_hWnd
// slot holds HWND_TOPMOST); only its address is passed, exactly as retail does.
extern "C" const unsigned char impl__wndTopMost_CWnd__2V1_B[];

namespace {

// Retail object layout, sizeof == 0x468.
//   * CreateObject (RVA 0x12ee10 mfc140u) allocates 0x468 bytes.
//   * The constructor (RVA 0x12efc0 mfc140u) stores m_nSideNo = -1 (sdNONE) at
//     +0x08, constructs CSmartDockingStandaloneGuideWnd at +0x10 and the two
//     CMFCToolBarImages at +0x110 / +0x2a8, installs the CRgn vfptr at +0x440 and
//     zeroes m_hObject at +0x448, sets the qword at +0x450 to -1 (m_cx/m_cy), the
//     qword at +0x458 to 0 (m_bHiLited/m_bLayered) and +0x460 to 1
//     (m_bIsDefaultImage).
//   * Create (RVA 0x12f0f0 mfc140u) writes GetRgnBox width/height to +0x450 /
//     +0x454 and, after creating the window, sets +0x45c = 1; Highlight (RVA 0x12f3d0
//     mfc140u) compares/stores its argument at +0x458; InitImages (RVA 0x12f540
//     mfc140u) stores the "no custom bitmap" flag at +0x460.
//   * The destructor (RVA 0x12f090 mfc140u) destroys the members in reverse order
//     at exactly these addresses.
// The declaration order in afxsmartdockingguide.h matches these offsets.
struct SdStandaloneGuideLayout {
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
};
static_assert(offsetof(SdStandaloneGuideLayout, m_nSideNo) == 0x08, "m_nSideNo +0x08");
static_assert(offsetof(SdStandaloneGuideLayout, m_wndBmp) == 0x10, "m_wndBmp +0x10");
static_assert(offsetof(SdStandaloneGuideLayout, m_Image) == 0x110, "m_Image +0x110");
static_assert(offsetof(SdStandaloneGuideLayout, m_ImageHot) == 0x2a8, "m_ImageHot +0x2a8");
static_assert(offsetof(SdStandaloneGuideLayout, m_Rgn_vfptr) == 0x440, "m_Rgn +0x440");
static_assert(offsetof(SdStandaloneGuideLayout, m_Rgn_hObject) == 0x448, "m_Rgn.m_hObject +0x448");
static_assert(offsetof(SdStandaloneGuideLayout, m_cx) == 0x450, "m_cx +0x450");
static_assert(offsetof(SdStandaloneGuideLayout, m_cy) == 0x454, "m_cy +0x454");
static_assert(offsetof(SdStandaloneGuideLayout, m_bHiLited) == 0x458, "m_bHiLited +0x458");
static_assert(offsetof(SdStandaloneGuideLayout, m_bLayered) == 0x45c, "m_bLayered +0x45c");
static_assert(offsetof(SdStandaloneGuideLayout, m_bIsDefaultImage) == 0x460, "m_bIsDefaultImage +0x460");
static_assert(sizeof(SdStandaloneGuideLayout) == 0x468, "CreateObject allocates 0x468");

// Retail reads the marker window's HWND at this+0x50 = m_wndBmp + 0x40, the
// CWnd::m_hWnd slot; the CWnd thunks below read it through OpenMFC's CWnd.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");

inline SdStandaloneGuideLayout* Guide(void* pThis) {
    return static_cast<SdStandaloneGuideLayout*>(pThis);
}
inline const SdStandaloneGuideLayout* Guide(const void* pThis) {
    return static_cast<const SdStandaloneGuideLayout*>(pThis);
}
inline CWnd* WndBmp(void* pThis) {
    return reinterpret_cast<CWnd*>(Guide(pThis)->m_wndBmp);
}
inline HWND WndBmpHwnd(const void* pThis) {
    return reinterpret_cast<const CWnd*>(Guide(pThis)->m_wndBmp)->m_hWnd;
}

// afxsmartdockingguide.h:61 SDMarkerPlace (sdNONE = -1, sdLEFT = 0, ...).
enum : int { kSdLeft = 0, kSdRight = 1, kSdTop = 2, kSdBottom = 3 };

} // namespace

// Symbol: ??0CSmartDockingStandaloneGuide@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingStandaloneGuide__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CSmartDockingStandaloneGuide@@UEAA@XZ
// STUB.  Retail (RVA 0x12f090 mfc140u) re-installs the class vftable, calls
// Destroy() non-virtually, then destroys m_Rgn (CGdiObject dtor ->
// DeleteObject), m_ImageHot and m_Image (??1CMFCToolBarImages) and m_wndBmp
// (CSmartDockingStandaloneGuideWnd vftable, tail-jump to ??1CWnd).  Not
// reproduced: OpenMFC's constructor above is still a stub that constructs none
// of those members and installs no vftable (OpenMFC has no vftable for this
// class), so running their destructors here would act on uninitialised memory.
extern "C" void* MS_ABI impl___1CSmartDockingStandaloneGuide__UEAA_XZ() {
    return nullptr;
}

// Symbol: ?AdjustPos@CSmartDockingStandaloneGuide@@UEAAXVCRect@@@Z
// Transcribed from retail RVA 0x12f300 (mfc140u).  CRect is passed by value,
// which under the x64 MS ABI means by pointer to a caller-owned copy (RDX).
// The four offsets are the literal immediate 0x10 in retail (it does not load
// the m_n*Offset* statics, which also hold 16).  Halving is `sar $1`, i.e. an
// arithmetic shift, reproduced with >> on int.
//     switch (m_nSideNo) {
//     case sdLEFT:   x = rc.left + 16;                            y = ((rc.top+rc.bottom)>>1) - (m_cy>>1); break;
//     case sdRIGHT:  x = rc.right - m_cx - 16;                    y = ((rc.top+rc.bottom)>>1) - (m_cy>>1); break;
//     case sdTOP:    x = ((rc.left+rc.right)>>1) - (m_cx>>1);     y = rc.top + 16;                         break;
//     case sdBOTTOM: x = ((rc.left+rc.right)>>1) - (m_cx>>1);     y = rc.bottom - m_cy - 16;               break;
//     default: return;
//     }
//     if (m_wndBmp.GetSafeHwnd() != NULL)
//         m_wndBmp.SetWindowPos(&CWnd::wndTopMost, x, y, -1, -1, SWP_NOSIZE | SWP_NOACTIVATE /*0x11*/);
extern "C" void MS_ABI impl__AdjustPos_CSmartDockingStandaloneGuide__UEAAXVCRect___Z(
    void* pThis, const RECT* prcHost) {
    SdStandaloneGuideLayout* g = Guide(pThis);
    const RECT& rc = *prcHost;
    int x;
    int y;
    switch (g->m_nSideNo) {
    case kSdLeft:
        x = rc.left + 16;
        y = ((rc.bottom + rc.top) >> 1) - (g->m_cy >> 1);
        break;
    case kSdRight:
        x = rc.right - g->m_cx - 16;
        y = ((rc.bottom + rc.top) >> 1) - (g->m_cy >> 1);
        break;
    case kSdTop:
        x = ((rc.right + rc.left) >> 1) - (g->m_cx >> 1);
        y = rc.top + 16;
        break;
    case kSdBottom:
        x = ((rc.right + rc.left) >> 1) - (g->m_cx >> 1);
        y = rc.bottom - g->m_cy - 16;
        break;
    default:
        return;
    }
    if (WndBmpHwnd(pThis) != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            WndBmp(pThis), reinterpret_cast<const CWnd*>(impl__wndTopMost_CWnd__2V1_B),
            x, y, -1, -1, SWP_NOSIZE | SWP_NOACTIVATE);
    }
}

// Symbol: ?Create@CSmartDockingStandaloneGuide@@UEAAXW4SDMarkerPlace@1@PEAVCWnd@@@Z
// STUB.  Retail (RVA 0x12f0f0 mfc140u), read from the instruction-identical
// mfc140.dll body at 0x12fd70:
//   * stores m_nSideNo;
//   * bSkipColorKey = CDockingManager::m_SDParams.m_bIsAlphaMarkers (mfc140 VA
//     0x1803ba368 = m_SDParams 0x1803ba310 + 0x58) != 0 || Theme() == 2, where
//     Theme() is an unexported helper (RVA 0x12fb08 in mfc140.dll): it returns 0
//     when m_SDParams.m_uiMarkerBmpResID[0] (m_SDParams + 0x28) is non-zero,
//     else an unexported int at mfc140 VA 0x1803b6f6c if non-zero (presumably
//     CDockingManager::m_SDTheme, which afxdockingmanager.h:385 declares
//     AFX_IMPORT_DATA but which is absent from the export table), else
//     CMFCVisualManager::GetInstance()->vftable +0x3c0 (GetSmartDockingTheme);
//   * InitImages(CDockingManager::m_SDParams) through this vftable +0x58;
//   * m_Rgn.Attach(CMFCToolBarImages::CreateRegionFromImage(m_Image.m_hbmImageWell
//     (this+0x1b0), m_SDParams.m_clrTransparent (m_SDParams + 0x14)));
//   * ::GetRgnBox(m_Rgn) and stores its width/height in m_cx/m_cy;
//   * m_wndBmp.Create(&rcBox, m_Image.m_hbmImageLight (this+0x1b8), else
//     m_hbmImageWell (this+0x1b0), NULL, pwndOwner, m_bIsDefaultImage,
//     side is sdTOP or sdBOTTOM);
//   * m_wndBmp.ModifyStyleEx(0, WS_EX_LAYERED);
//   * when !bSkipColorKey: if the first dword of afxGlobalData is 0, calls the
//     unexported routine at RVA 0x6a5c0 (mfc140) on &afxGlobalData and sets that
//     dword to 1 (call site 0x12fe8d inside Create), then ::SetLayeredWindowAttributes(
//     m_wndBmp.m_hWnd, m_SDParams.m_clrTransparent, 0, LWA_COLORKEY);
//   * unconditionally m_bLayered = TRUE and m_wndBmp.ModifyStyleEx(0, WS_EX_TOPMOST).
// Not reproduced: OpenMFC's exported m_SDParams is an 8-byte placeholder (see
// CDockingManager.cpp), not a CSmartDockingInfo; the m_SDTheme value Theme()
// reads has no OpenMFC counterpart (CDockingManager::SetDockingMode is itself a
// placeholder stub); OpenMFC has no vftable for the InitImages dispatch; and
// CSmartDockingStandaloneGuideWnd::Create is a placeholder stub.
extern "C" void MS_ABI impl__Create_CSmartDockingStandaloneGuide__UEAAXW4SDMarkerPlace_1_PEAVCWnd___Z(
    void* pThis, int nSideNo, CWnd* pwndOwner) {
    (void)pThis; (void)nSideNo; (void)pwndOwner;
}

// Symbol: ?CreateObject@CSmartDockingStandaloneGuide@@SAPEAVCObject@@XZ
// STUB.  Retail (RVA 0x12ee10 mfc140u) is `new(0x468) CSmartDockingStandaloneGuide`.
// OpenMFC's constructor above is still a stub that installs no vftable (none is
// modelled for this class), so a returned object would have no working virtual
// dispatch; nullptr (the allocation-failure result) is returned instead, as the
// other opaque docking classes do (see CMFCAutoHideButton.cpp).
extern "C" void* MS_ABI impl__CreateObject_CSmartDockingStandaloneGuide__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?Destroy@CSmartDockingStandaloneGuide@@UEAAXXZ
// Transcribed from retail RVA 0x12f280 (mfc140u):
//     if (::IsWindow(m_wndBmp.m_hWnd))
//         m_wndBmp.DestroyWindow();            // vftable +0xd0 (slot 26)
// Deviation: retail dispatches virtually; slot 26 of the CSmartDockingStandaloneGuideWnd
// vftable (mfc140.dll VA 0x18030d288, the one the destructor installs at +0x10)
// is ?DestroyWindow@CWnd@@UEAAHXZ, and m_wndBmp is an embedded member whose
// dynamic type is exactly that class, so the thunk is called directly.
extern "C" void MS_ABI impl__Destroy_CSmartDockingStandaloneGuide__UEAAXXZ(void* pThis) {
    if (::IsWindow(WndBmpHwnd(pThis))) {
        impl__DestroyWindow_CWnd__UEAAHXZ(WndBmp(pThis));
    }
}

// Symbol: ?Highlight@CSmartDockingStandaloneGuide@@UEAAXH@Z
// STUB.  Retail (RVA 0x12f3d0 mfc140u): if bHiLite == m_bHiLited it calls
// m_wndBmp.UpdateLayered() and returns; otherwise it stores m_bHiLited, calls
// m_wndBmp.Highlight(bHiLite), and -- when !m_bIsDefaultImage or the unexported
// Theme() helper described at Create (RVA 0x12fb08 in mfc140.dll) returns 2 --
// picks bHiLite ? m_ImageHot.m_hbmImageWell (this+0x348), else
// m_Image.m_hbmImageLight (this+0x1b8), falling back to m_Image.m_hbmImageWell
// (this+0x1b0) when the chosen handle is NULL; stores it, if non-NULL, into
// m_wndBmp.m_hbmpFace (this+0xf8 = m_wndBmp+0xe8, the first member after the
// 0xe8-byte CWnd base) and calls ::InvalidateRect(m_wndBmp.m_hWnd, NULL, TRUE).
// Not reproduced: Theme() needs the unexported m_SDTheme and the real
// CDockingManager::m_SDParams, neither modelled by OpenMFC (see Create), and
// CSmartDockingStandaloneGuideWnd::UpdateLayered / ::Highlight are placeholder
// stubs whose parameter lists lack `this`.
extern "C" void MS_ABI impl__Highlight_CSmartDockingStandaloneGuide__UEAAXH_Z(void* pThis, int bHiLite) {
    (void)pThis; (void)bHiLite;
}

// Symbol: ?InitImages@CSmartDockingStandaloneGuide@@MEAAXAEAVCSmartDockingInfo@@@Z
// STUB.  Retail (RVA 0x12f540 mfc140u) calls CMFCToolBarImages::Clear on m_Image
// and m_ImageHot, picks per-side bitmap resource IDs out of the CSmartDockingInfo
// argument (m_uiMarkerBmpResID[] at +0x28, m_uiMarkerLightBmpResID[] at +0x3c;
// sides sdCLEFT..sdCMIDDLE fold onto the same five entries), falling back, when
// the ID is 0, to built-in tables chosen by the unexported Theme() helper (RVA
// 0x12fb08 in mfc140.dll, described at Create); sets m_bIsDefaultImage =
// (ID == 0); loads each image with CMFCToolBarImages::LoadStr(MAKEINTRESOURCE
// (id)) + SetSingleImage and sets its m_clrTransparent from params; recolours
// m_Image with AdaptColors or a CMFCVisualManager virtual (+0x3b8); and, when
// m_wndBmp has a window, stores m_hbmpFace, ::InvalidateRect()s it and, unless
// params.m_bIsAlphaMarkers or Theme() == 2, calls ::SetLayeredWindowAttributes
// (hwnd, params.m_clrTransparent, 0, LWA_COLORKEY).  Both CSmartDockingInfo
// (detail/CSmartDockingInfoSupport.h) and the CMFCToolBarImages layout
// (afxmfc.h) ARE modelled.  Not reproduced because: Theme() needs the unexported
// m_SDTheme, which OpenMFC does not model (see Create);
// CMFCToolBarImages::SetSingleImage is a placeholder stub without `this`; and no
// OpenMFC path constructs this object (the constructor above is a stub).
extern "C" void MS_ABI impl__InitImages_CSmartDockingStandaloneGuide__MEAAXAEAVCSmartDockingInfo___Z(
    void* pThis, void* params) {
    (void)pThis; (void)params;
}

// Symbol: ?IsPtIn@CSmartDockingStandaloneGuide@@UEBAHVCPoint@@@Z
// Transcribed from retail RVA 0x12f460 (mfc140u).  CPoint arrives by value in
// RDX as an 8-byte aggregate (x low dword, y high dword); retail spills it and
// passes its address to ScreenToClient.
//     if (m_wndBmp.GetSafeHwnd() == NULL || !::IsWindowVisible(m_wndBmp.m_hWnd)) return FALSE;
//     ::ScreenToClient(m_wndBmp.m_hWnd, &point);
//     if (m_bLayered) return ::PtInRegion(m_Rgn.m_hObject, point.x, point.y);
//     CRgn rgn; rgn.Attach(::CreateRectRgn(0, 0, 0, 0));
//     ::GetWindowRgn(m_wndBmp.m_hWnd, rgn.m_hObject);
//     BOOL b = ::PtInRegion(rgn.m_hObject, point.x, point.y);
//     return b;                                   // ~CRgn -> DeleteObject
// Deviation: the temporary CRgn is modelled as a raw HRGN freed with
// ::DeleteObject.  Retail's CGdiObject::Attach / DeleteObject additionally
// enter and leave the permanent GDI handle map for that handle for the duration
// of the call; nothing can observe the map entry in between, so the result is
// the same.
extern "C" int MS_ABI impl__IsPtIn_CSmartDockingStandaloneGuide__UEBAHVCPoint___Z(
    const void* pThis, long long point) {
    const SdStandaloneGuideLayout* g = Guide(pThis);
    const HWND hWnd = WndBmpHwnd(pThis);
    if (hWnd == nullptr || !::IsWindowVisible(hWnd)) {
        return FALSE;
    }
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<unsigned long long>(point) & 0xffffffffu);
    pt.y = static_cast<LONG>(static_cast<unsigned long long>(point) >> 32);
    ::ScreenToClient(hWnd, &pt);
    if (g->m_bLayered != 0) {
        return ::PtInRegion(g->m_Rgn_hObject, pt.x, pt.y);
    }
    HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
    ::GetWindowRgn(hWnd, hRgn);
    const BOOL bIn = ::PtInRegion(hRgn, pt.x, pt.y);
    if (hRgn != nullptr) {
        ::DeleteObject(hRgn);
    }
    return bIn;
}

// Symbol: ?Show@CSmartDockingStandaloneGuide@@UEAAXH@Z
// Transcribed from retail RVA 0x12ff40 (mfc140.dll).  mfc140u_rva_symbols.json
// has no entry for this export; the instruction-identical body sits at RVA
// 0x12f2c0 (mfc140u), between Destroy (0x12f280) and AdjustPos (0x12f300), and
// is also slot 7 of the class vftable.
//     if (::IsWindow(m_wndBmp.m_hWnd))
//         m_wndBmp.ShowWindow(bShow ? SW_SHOW : SW_HIDE);   // neg/sbb/and 5
extern "C" void MS_ABI impl__Show_CSmartDockingStandaloneGuide__UEAAXH_Z(void* pThis, int bShow) {
    if (::IsWindow(WndBmpHwnd(pThis))) {
        impl__ShowWindow_CWnd__QEAAHH_Z(WndBmp(pThis), bShow != 0 ? SW_SHOW : SW_HIDE);
    }
}

// The four marker offsets are `const int` statics in retail .rdata, each 16:
// read from mfc140.dll RVAs 0x33b7a8 (Top) / 0x33b7ac (Bottom) / 0x33b7b0 (Left)
// / 0x33b7b4 (Right) and mfc140u.dll RVAs 0x33e000 (Top) / 0x33dff8 (Bottom) /
// 0x33e004 (Left) / 0x33dffc (Right).

// Symbol: ?m_nBottomOffsetY@CSmartDockingStandaloneGuide@@1HB
extern "C" int MS_ABI impl__m_nBottomOffsetY_CSmartDockingStandaloneGuide__1HB = 16;

// Symbol: ?m_nLeftOffsetX@CSmartDockingStandaloneGuide@@1HB
extern "C" int MS_ABI impl__m_nLeftOffsetX_CSmartDockingStandaloneGuide__1HB = 16;

// Symbol: ?m_nRightOffsetX@CSmartDockingStandaloneGuide@@1HB
extern "C" int MS_ABI impl__m_nRightOffsetX_CSmartDockingStandaloneGuide__1HB = 16;

// Symbol: ?m_nTopOffsetY@CSmartDockingStandaloneGuide@@1HB
extern "C" int MS_ABI impl__m_nTopOffsetY_CSmartDockingStandaloneGuide__1HB = 16;
