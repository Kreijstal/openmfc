// CMFCReBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

// ===========================================================================
// Two classes live in this file, neither of which is declared in OpenMFC's
// public headers (afxmfc.h carries only a 16-byte placeholder `CMenuImages`
// for the IMAGES_IDS enum), so every export takes a `void*` this and the
// retail member layout is pinned HERE, following the precedent of
// featurepack/ribbon/CMFCRibbonInfo.cpp and core/ole/CDocObjectServer.cpp.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (disas.py --u); every RVA quoted is an mfc140u FUNCTION ENTRY unless the
// comment names a specific instruction.  The `call *0x1802c7b30` in the
// retail listings is the Control Flow Guard dispatch
// (__guard_dispatch_icall_fptr), i.e. an indirect call through RAX -- every
// vtable slot cited below is (vtable byte offset / 8) read off such a call
// site.  Every USER32 import slot named below was resolved with iatu.py.
//
// ---------------------------------------------------------------------------
// CMFCReBar (afxrebar.h: `class CMFCReBar : public CPane`)
// ---------------------------------------------------------------------------
// Retail layout, read out of the constructor ??0CMFCReBar@@QEAA@XZ
// (RVA 0xd0580, mfc140u) and the CRuntimeClass descriptor it returns from
// GetRuntimeClass (0xd0f40 -> 0x1802fd8b8, m_nObjectSize = 0x410):
//
//   +0x000  CPane base            sizeof(CPane) == 0x3f8 (afxmfc.h, harvested)
//   +0x3f8  CMFCControlBarImpl m_Impl   { vfptr @+0x3f8, CPane* m_pBar @+0x400 }
//                                 -- the ctor stores the CMFCControlBarImpl
//                                    vftable and `this` there
//   +0x408  UINT m_nReBarBandInfoSize
//   +0x40c  (tail pad)            total 0x410 == the 1040 that
//                                 featurepack/controls/RuntimeClasses.cpp
//                                 already records for the class
//
// The constructor (0xd0580), fully transcribed:
//     CPane::CPane();                                    // 0x9f2f0
//     vfptr = &CMFCReBar::`vftable' (0x1802fd8f0);      // NOT reproduced, see below
//     m_Impl.vfptr = &CMFCControlBarImpl::`vftable' (0x1802e2ff8); m_Impl.m_pBar = this;
//     m_cxLeftBorder = m_cxRightBorder = 0;              // two 8-byte zero stores
//     m_cyTopBorder  = m_cyBottomBorder = 0;             //   at +0x1c8 / +0x1d0
//     m_nReBarBandInfoSize = (<comctl version> < 0x60001) ? 112 : 128;   // helper 0x1db004
// where <comctl version> is the (major << 16 | minor) word the unexported
// helper at 0x1db004 caches from ?GetCommCtrlVersion@@YAJPEAK0@Z (0x2ae720).
// 128 == sizeof(REBARBANDINFOW) with the Vista members; 112 is the size of
// the struct without them (cxHeader ends at 0x6c, padded to 0x70).
//
// Retail CMFCReBar vftable (mfc140u VA 0x1802fd8f0 = RVA 0x2fd8f0, dumped with vtdump_u.py;
// slot 0 anchors on GetRuntimeClass@CMFCReBar 0xd0f40) -- the slots the
// bodies below dispatch through, with what retail has in them:
//   slot  73 (+0x248) 0x28bb80  CWnd::DefWindowProc            (not overridden)
//   slot  97 (+0x308) 0xb310    CBasePane::IsVisible
//   slot 122 (+0x3d0) 0xd1010   CMFCReBar::SetPaneAlignment    (CBasePane's is
//                                inline: m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | dwAlignment)
//   slot 154 (+0x4d0) 0xd0a10   CMFCReBar::CalcFixedLayout
//   slot 156 (+0x4e0) 0xbd30    CBasePane::AdjustDockingLayout
// Other classes' slots reached: CFrameWnd slot 96 (+0x300) = RecalcLayout(BOOL)
// (the same slot CMFCToolBar.cpp cites for AFXGetParentFrame(this)->RecalcLayout),
// and CWnd slot 31 (+0xf8) = OnToolHitTest.
//
// Message map (0x1802fdf40, dumped with msgmap_u.py; lives in
// featurepack/controls/MessageMaps.cpp, not here): WM_NCCREATE -> OnNcCreate
// (0xd0d60), WM_NCCALCSIZE -> OnNcCalcSize (0xd0db0), WM_NCPAINT -> OnNcPaint
// (0xd0e40), RB_SHOWBAND -> OnShowBand (0xd06c0), WM_RECALCPARENT (0x368) ->
// OnRecalcParent (0xd0650), reflected RBN_HEIGHTCHANGE and RBN_ENDDRAG ->
// OnHeightChange (0xd0690), and WM_PAINT / WM_ERASEBKGND / WM_LBUTTONDOWN /
// WM_LBUTTONUP / WM_MOUSEMOVE all -> 0xda30, which is `jmp CWnd::Default`
// (0x28ac80): those five handlers are one COMDAT-folded `return Default();`.
//
// Deviations shared by every CMFCReBar body (each site repeats the ones it
// makes):
//  * A NULL `this` returns quietly.  Retail never checks it.
//  * Retail's ENSURE()s call AfxThrowInvalidArgException (0x227720); the
//    exported thunk is called and the body then returns, as
//    menu/CMFCPopupMenu.cpp does.
//  * The vftable store in the constructor is not reproduced: OpenMFC has no
//    MSVC-layout vtable for this family, and the CPane constructor thunk has
//    already installed the C++ CPane vtable that the DLL-internal dispatch
//    needs (same choice as every other pane class in this tree).  Every
//    retail virtual call on `this` is therefore devirtualised to the exported
//    body retail's own vtable holds in that slot (listed above), so nothing is
//    lost unless a client subclass overrides it.  Virtual calls on OTHER
//    objects (a band's child window, the parent frame) cannot be dispatched
//    through their vtables either; each site says which exported body it
//    calls instead and what override that loses.
//  * Every REBARBANDINFO local is zero-initialised before the members retail
//    sets are stored; retail leaves the rest of the struct indeterminate.
//  * CWnd::m_hWndOwner (CWnd +0xa0) is not NAMED by OpenMFC's CWnd -- the slot
//    lies inside the zero-filled _cwnd_padding2 -- but it IS written: MSVC
//    clients inline CWnd::SetOwner (afxwin.inl) and store the owner HWND
//    there.  So it is read by offset here, exactly as
//    core/controlbar/CControlBar.cpp (OwnerHwnd) already does, and retail's
//    `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)` and OnNcCreate's
//    `if (m_hWndOwner) RB_SETPARENT` are reproduced as written.  This is NOT
//    a deviation; it is listed here because menu/CMFCPopupMenuBar.cpp and
//    controls/CMFCStatusBar.cpp still take the ::GetParent branch
//    unconditionally on the (incorrect) belief that nothing writes the slot.
//  * OpenMFC's CBRS_* macros (afxole.h) do NOT have the MFC values, so the
//    retail constants are spelled out as file-local kCbrs* below, as
//    docking/CPane.cpp does.
//
// ---------------------------------------------------------------------------
// CMenuImages (afxmenuimages.h: a static-only helper, no instances)
// ---------------------------------------------------------------------------
// Six exported statics `CMFCToolBarImages m_ImagesBlack / DkGray / Gray /
// LtGray / White / Black2` (retail .data 0x3c1900 / 0x3c1aa0 / 0x3c1c40 /
// 0x3c1de0 / 0x3c1f80 / 0x3c2120, 0x1a0 apart -- sizeof(CMFCToolBarImages) is
// 0x198, 16-aligned) plus an unexported `static BOOL m_bInitializing`
// (0x3be258).  The statics are defined below as six 16-aligned
// sizeof(CMFCToolBarImages) blocks that a file-scope static initialiser
// constructs through the exported ??0CMFCToolBarImages@@QEAA@XZ thunk (whose
// body is `new(pThis) CMFCToolBarImages()`) and destroys through
// ??1CMFCToolBarImages@@UEAA@XZ at exit -- the same construct/destroy pair
// retail's own dynamic initialiser and atexit entries run for them.  (The
// C++-object form toolbar/CMFCToolBar.cpp uses for its eight would need the
// Itanium-mangled ctor/dtor symbols, which the per-file link audit rejects.)
//
// The CMFCToolBarImages members the retail bodies read directly (the class
// is declared in afxmfc.h with the harvested retail layout, but these are
// protected there, so they are reached by offset):
//   +0x08  int      m_iCount           +0x68  CSize    m_sizeImage
//   +0xa0  HBITMAP  m_hbmImageWell     +0xd8  COLORREF m_clrTransparent
// AFX_GLOBAL_DATA fields read (offsets from the retail reads, names from the
// shipping afxglobals.h member order, which agrees with the offsets the
// sibling files already pin: bIsWindows7 +0x258, m_bIsBlackHighContrast +0x260):
//   +0x000 init gate           +0x260 m_bIsBlackHighContrast
//   +0x264 m_bIsWhiteHighContrast   +0x268 m_bUseBuiltIn32BitIcons
//   +0x288 m_nBitsPerPixel     +0x2b8 double m_dblRibbonImageScale
//   +0x2c0 m_bIsRibbonImageScale
// Resource IDs: 0x3f01 = IDB_AFXBARRES_MENU_IMAGES (16129) and 0x42b8 =
// IDB_AFXBARRES_MENU_IMAGES24 (17080), both from afxribbonres.h.
//
// KNOWN GAPS, all in featurepack/toolbar/CMFCToolBarImages.cpp and not
// worked around here: LoadStr loads and immediately deletes the bitmap (it
// never stores m_hbmImageWell or m_iCount; it counts in a side table),
// CopyTo / Draw / MapBmpTo3dColors / PrepareDrawImage / SmoothResize are
// stubs, and the last two still carry auto-generated parameter lists that
// omit `this` (declared with the retail-correct lists below; the stub bodies
// read no argument, so that is safe under the MS x64 ABI -- see the note in
// docking/CPane.cpp).  So today CMenuImages::Draw draws nothing and Size()
// is never ribbon-scaled.  Two consequences ARE handled, each marked
// DEVIATION at the site: retail's "already loaded" test
// (m_ImagesBlack.m_hbmImageWell != NULL) and CleanUp's m_iCount guard can
// never fire here, so a file-local g_bMenuImagesLoaded supplements them.
// ===========================================================================

// ---- sibling impl__ exports called below (every one was grepped to a
// ---- definition in phase4/src; the file is named where it is not obvious) ----
extern "C" void* MS_ABI impl___0CPane__IEAA_XZ(void* pThis);                       // docking/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCControlBarImpl__QEAA_PEAVCPane___Z(void* pThis, void* pBar);
extern "C" void  MS_ABI impl__DrawNcArea_CMFCControlBarImpl__IEAAXXZ(void* pImpl);
extern "C" long  MS_ABI impl__GetCommCtrlVersion__YAJPEAK0_Z(unsigned long* pdwMajor, unsigned long* pdwMinor);   // CMFC_misc_stubs.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();          // docking/CPane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();      // docking/CBasePane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();    // toolbar/RuntimeClasses.cpp
extern "C" __int64 MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int   MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext);
extern "C" int   MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);   // CMFC_misc_stubs.cpp
extern "C" void  MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(const CWnd* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(const CMFCToolBar* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);   // CMFC_misc_stubs.cpp
extern "C" void* MS_ABI impl__ChWindowFromPoint__YAPEAUHWND____PEAU1_UtagPOINT___Z(HWND hWnd, POINT pt);   // CMFC_misc_stubs.cpp (POINT by value in RDX)
extern "C" void  MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);          // core/frame/Thunks.cpp
extern "C" void  MS_ABI impl__CalcInsideRect_CPane__QEBAXAEAVCRect__H_Z(const CPane* pThis, RECT* pRect, int bHorz);
extern "C" int   MS_ABI impl__IsVisible_CBasePane__UEBAHXZ(const CBasePane* pThis);                     // docking/CBasePane.cpp
extern "C" void  MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* hdwp);
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCToolBar__UEAA_AVCSize__HH_Z(CMFCToolBar* pThis, CSize* pRet, int bStretch, int bHorz);
// CBasePane::WindowProc: the definition in docking/CBasePane.cpp:1439 is still
// an auto-generated placeholder -- `(unsigned p0, u64 p1, i64 p2) { return 0; }`
// with no `this` -- so the call in WindowProc below currently yields 0 for
// every message it forwards.  Declared here with the list the mangled name
// describes, as ribbon/CMFCRibbonBar.cpp already does.
extern "C" __int64 MS_ABI impl__WindowProc_CBasePane__MEAA_JI_K_J_Z(CBasePane* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam);

// CMFCToolBarImages exports (featurepack/toolbar/CMFCToolBarImages.cpp) and
// the two globals the CMenuImages bodies read.
extern "C" void  MS_ABI impl__Initialize_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" int   MS_ABI impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(CMFCToolBarImages* pThis, const wchar_t* lpszResourceName, HINSTANCE hInstRes, int bAdd);
extern "C" int   MS_ABI impl__Mirror_CMFCToolBarImages__QEAAHXZ(CMFCToolBarImages* pThis);
extern "C" int   MS_ABI impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(HBITMAP* pHBitmap, int nWidth);
extern "C" int   MS_ABI impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(CMFCToolBarImages* pThis, CMFCToolBarImages* pDest);
extern "C" int   MS_ABI impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(HBITMAP* pHBitmap, int bMono, unsigned long clrSrc, unsigned long clrDest);
extern "C" void  MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" int   MS_ABI impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(
    CMFCToolBarImages* pThis, CDC* pDC, int x, int y, int iImageIndex, int bHilite, int bDisabled,
    int bIndeterminate, int bShadow, int bInactive, unsigned char alphaSrc);
extern "C" void  MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(CMFCToolBarImages* pThis, void* pDS);
// The next two definitions are placeholders (see the KNOWN GAPS note above);
// these are the lists the mangled names describe.  CSize is passed BY VALUE
// (8 bytes in R8) -- retail loads it with `mov (%r8),%r8` -- so it is modelled
// as a long long.
extern "C" int   MS_ABI impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(
    CMFCToolBarImages* pThis, void* pDS, long long sizeImageDest, int bFadeInactive);
extern "C" int   MS_ABI impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(CMFCToolBarImages* pThis, double dblImageScale);
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis);
extern "C" std::int32_t impl__m_bIsRTL_CMFCToolBarImages__1HA;                     // toolbar/StaticData.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];           // CMFC_misc_stubs.cpp
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);    // core/runtime/AFX_GLOBAL_DATA.cpp -- an empty `() {}` stub today
// Thunks defined further down this file that earlier bodies call.
extern "C" int   MS_ABI impl__Initialize_CMenuImages__KAHXZ();
extern "C" CSize* MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(CSize* pRet);
extern "C" void  MS_ABI impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(CMFCToolBarImages* pImages, unsigned long clr);

namespace {

// ---- CMFCReBar layout (see the header comment for the evidence) ----
constexpr std::size_t kOffImpl              = 0x3f8;   // CMFCControlBarImpl m_Impl
constexpr std::size_t kOffReBarBandInfoSize = 0x408;   // UINT m_nReBarBandInfoSize
constexpr std::size_t kSizeofCMFCReBar      = 0x410;
static_assert(sizeof(CPane) == kOffImpl, "CMFCReBar::m_Impl must follow the 0x3f8-byte CPane base");
static_assert(sizeof(CMFCControlBarImpl) == 0x10, "CMFCControlBarImpl is {vfptr, m_pBar}");
static_assert(kOffImpl + sizeof(CMFCControlBarImpl) == kOffReBarBandInfoSize, "m_nReBarBandInfoSize @0x408");
static_assert(kOffReBarBandInfoSize + sizeof(unsigned int) <= kSizeofCMFCReBar, "sizeof(CMFCReBar) == 0x410");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40");
static_assert(offsetof(CBasePane, m_dwStyle) == 0x104, "CBasePane::m_dwStyle @0x104");
static_assert(offsetof(CBasePane, m_pDockSite) == 0x110, "CBasePane::m_pDockSite @0x110");
static_assert(offsetof(CPane, m_cxLeftBorder) == 0x1c8 && offsetof(CPane, m_cyTopBorder) == 0x1d0,
              "CPane border ints @0x1c8..0x1d4 (the ctor zeroes them as two 8-byte stores)");

// ---- REBARBANDINFOW offsets / constants the retail bodies use ----
static_assert(sizeof(REBARBANDINFOW) == 128, "sizeof(REBARBANDINFOW) with the Vista members");
static_assert(offsetof(REBARBANDINFOW, fStyle) == 0x08 && offsetof(REBARBANDINFOW, clrFore) == 0x0c &&
              offsetof(REBARBANDINFOW, clrBack) == 0x10 && offsetof(REBARBANDINFOW, lpText) == 0x18 &&
              offsetof(REBARBANDINFOW, hwndChild) == 0x28 && offsetof(REBARBANDINFOW, cxMinChild) == 0x30 &&
              offsetof(REBARBANDINFOW, cyMinChild) == 0x34 && offsetof(REBARBANDINFOW, hbmBack) == 0x40,
              "REBARBANDINFOW member offsets read by the retail bodies");
static_assert(RB_GETBANDCOUNT == 0x40c && RB_GETBANDINFOW == 0x41c && RB_SHOWBAND == 0x423 &&
              RB_GETRECT == 0x409 && RB_INSERTBANDW == 0x40a && RB_SETPARENT == 0x407, "RB_* message ids");
static_assert(RBBIM_STYLE == 0x1 && RBBIM_COLORS == 0x2 && RBBIM_TEXT == 0x4 && RBBIM_CHILD == 0x10 &&
              RBBIM_CHILDSIZE == 0x20 && RBBIM_BACKGROUND == 0x80 && RBBIM_IDEALSIZE == 0x200 && RBBS_HIDDEN == 0x8,
              "RBBIM_* / RBBS_* bits");
static_assert((CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE | RBS_VARHEIGHT) == 0x24e,
              "the style bits Create ORs in");

// Real MFC afxres.h values (OpenMFC's afxole.h CBRS_* macros differ; see the
// header comment).  Each one is the literal the retail body tests.
constexpr unsigned long kCbrsAll         = 0x0040FFFFul;   // CBRS_ALL
constexpr unsigned long kCbrsHideInplace = 0x00000008ul;   // CBRS_HIDE_INPLACE
constexpr unsigned long kCbrsOrientHorz  = 0x0000A000ul;   // CBRS_ORIENT_HORZ = CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM
constexpr unsigned long kCbrsAlignAny    = 0x0000F000ul;   // CBRS_ALIGN_ANY
constexpr unsigned int  kAfxIdwReBar     = 0xE804u;        // AFX_IDW_REBAR (afxres.h)
constexpr long          kAfxWndCommCtlReBarReg = 0x8000;   // the AFX_WNDCOMMCTL_*_REG bit retail passes for the rebar class (afximpl.h is not shipped; the name was not verified here)
constexpr unsigned int  kWmSetMessageString = 0x0362u;     // WM_SETMESSAGESTRING (afxpriv.h)
constexpr unsigned int  kWmPopMessageString = 0x0375u;     // WM_POPMESSAGESTRING

template <typename T>
inline T& At(void* p, std::size_t off) { return *reinterpret_cast<T*>(static_cast<char*>(p) + off); }
template <typename T>
inline const T& At(const void* p, std::size_t off) { return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off); }

inline CWnd*      AsWnd(void* p)       { return static_cast<CWnd*>(static_cast<CPane*>(p)); }
inline CPane*     AsPane(void* p)      { return static_cast<CPane*>(p); }
inline CBasePane* AsBasePane(void* p)  { return static_cast<CBasePane*>(static_cast<CPane*>(p)); }
inline HWND       HWndOf(const void* p){ return At<HWND>(p, offsetof(CWnd, m_hWnd)); }

// CWnd::m_hWndOwner (retail CWnd +0xa0; header comment).  Not named by
// OpenMFC's CWnd, so it is read out of _cwnd_padding2 by offset, as
// core/controlbar/CControlBar.cpp does; the static_assert keeps the slot
// inside that padding.
constexpr std::size_t kOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "CWnd::m_hWndOwner slot must lie inside CWnd's padding");
inline HWND RawOwnerHwnd(const void* p) {
    HWND h = nullptr;
    std::memcpy(&h, static_cast<const char*>(p) + kOffHWndOwner, sizeof h);
    return h;
}
// GetOwner() as retail inlines it: `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`.
inline HWND OwnerHwnd(const void* p) {
    const HWND h = RawOwnerHwnd(p);
    return h != nullptr ? h : ::GetParent(HWndOf(p));
}
inline void*      ImplOf(void* p)      { return static_cast<char*>(p) + kOffImpl; }
inline unsigned int& BandInfoSizeOf(void* p) { return At<unsigned int>(p, kOffReBarBandInfoSize); }

// this->DefWindowProc(...), vtable slot 73 (+0x248) in retail; CWnd's body is
// what the CMFCReBar vftable holds there.
inline __int64 RbDefWindowProc(void* pThis, unsigned int msg, unsigned __int64 wParam, __int64 lParam) {
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(AsWnd(pThis), msg, wParam, lParam);
}

inline int IsKindOfPane(const CWnd* p) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ());
}
inline int IsKindOfToolBar(const CWnd* p) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ());
}

// The unexported retail helper at RVA 0x1db004 (mfc140u), transcribed:
//     static DWORD s_ver = -1;                                   // 0x3b1b94
//     if (s_ver == -1) {
//         DWORD major = 0, minor = 0;
//         GetCommCtrlVersion(&major, &minor);                    // 0x2ae720; HRESULT ignored
//         s_ver = ((WORD)major << 16) | (WORD)minor;
//     }
//     return s_ver;
unsigned long ComCtlVersionPacked() {
    static unsigned long s_ver = 0xFFFFFFFFul;
    if (s_ver == 0xFFFFFFFFul) {
        unsigned long major = 0, minor = 0;
        impl__GetCommCtrlVersion__YAJPEAK0_Z(&major, &minor);
        s_ver = ((major & 0xFFFFul) << 16) | (minor & 0xFFFFul);
    }
    return s_ver;
}

// "Is this band's child window visible?" -- the test both CalcFixedLayout
// (0xd0a10) and OnShowBand (0xd06c0) make on rbbi.hwndChild:
//     CWnd* pWnd = CWnd::FromHandlePermanent(hwndChild);            // 0x28adc0
//     if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CPane)))             // 0x234cf0, RTC 0x1802f5918
//         return pWnd->IsVisible();                                 // vslot 97 (+0x308) = CBasePane::IsVisible
//     return (::GetWindowLong(hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;   // shr $0x1c; and $1
// DEVIATION: IsVisible is devirtualised to the exported CBasePane body (an
// override in a client pane subclass would not be seen).
int BandChildIsVisible(HWND hwndChild) {
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndChild);
    if (pWnd != nullptr && IsKindOfPane(pWnd)) {
        return impl__IsVisible_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pWnd)) ? 1 : 0;
    }
    return (::GetWindowLongW(hwndChild, GWL_STYLE) & WS_VISIBLE) ? 1 : 0;
}

// _AddMFCToolBar(CWnd* pBar, REBARBANDINFO* pRBBI) -- the unexported retail
// helper at RVA 0xd07d0 (mfc140u) that both AddBar overloads tail into.
// Fully transcribed:
//     ENSURE(::IsWindow(m_hWnd)); ENSURE(pBar != NULL); ENSURE(::IsWindow(pBar->m_hWnd));
//     pRBBI->cbSize = m_nReBarBandInfoSize;
//     pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE;                 // orl $0x30,0x4(%rdi)
//     pRBBI->hwndChild = pBar->m_hWnd;
//     CSize size;
//     if (pBar->IsKindOf(RUNTIME_CLASS(CPane)))                      // RTC 0x1802f5918
//         size = ((CPane*)pBar)->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);   // vslot 154 (+0x4d0)
//     else { CRect rect; ::GetWindowRect(pBar->m_hWnd, &rect); size = rect.Size(); }
//     pRBBI->cxMinChild = size.cx;  pRBBI->cyMinChild = size.cy;
//     BOOL bResult = (BOOL)DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI);   // vslot 73
//     CFrameWnd* pFrame = AFXGetParentFrame(this);                   // 0x6bd00
//     if (pFrame != NULL) pFrame->RecalcLayout(TRUE);                // frame vslot 96 (+0x300)
//     return bResult;
// DEVIATIONS: (1) the child's CalcFixedLayout cannot be dispatched through
// its vtable; a CMFCToolBar-derived child goes to the exported
// CMFCToolBar::CalcFixedLayout (whose CalcLayout is still a stub returning
// (0,0)); any other CPane gets CBasePane's inline default, which for
// bStretch == FALSE is CSize(0, 0) -- so the in-tree overrides of
// CMFCStatusBar / CDockablePane / CDockingPanesRow / CMFCMenuBar (whose thunk
// has its this/CSize parameters swapped, see the file notes) and any client
// subclass override are not seen.  (2) RecalcLayout goes to the exported CFrameWnd body, as
// toolbar/CMFCToolBar.cpp and menu/CMFCMenuBar.cpp do, so a CFrameWndEx /
// CMDIFrameWndEx parent does not get its own override.
int AddMFCToolBar(void* pThis, CWnd* pBar, REBARBANDINFOW* pRBBI) {
    if (!::IsWindow(HWndOf(pThis)) || pBar == nullptr || !::IsWindow(pBar->m_hWnd)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached: the thunk throws
    }
    pRBBI->cbSize = BandInfoSizeOf(pThis);
    pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE;
    pRBBI->hwndChild = pBar->m_hWnd;

    CSize size(0, 0);
    if (IsKindOfPane(pBar)) {
        const int bHorz = (AsBasePane(pThis)->m_dwStyle & kCbrsOrientHorz) ? 1 : 0;
        if (IsKindOfToolBar(pBar)) {
            impl__CalcFixedLayout_CMFCToolBar__UEAA_AVCSize__HH_Z(static_cast<CMFCToolBar*>(pBar), &size, FALSE, bHorz);
        } else {
            // CBasePane::CalcFixedLayout (afxbasepane.h, inline):
            //   cx = bStretch && bHorz ? 32767 : 0;  cy = bStretch && !bHorz ? 32767 : 0;
            size = CSize(0, 0);
        }
    } else {
        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(pBar->m_hWnd, &rect);
        size = CSize(rect.right - rect.left, rect.bottom - rect.top);
    }
    pRBBI->cxMinChild = static_cast<UINT>(size.cx);
    pRBBI->cyMinChild = static_cast<UINT>(size.cy);

    const int bResult = RbDefWindowProc(pThis, RB_INSERTBANDW, static_cast<unsigned __int64>(-1),
                                        reinterpret_cast<__int64>(pRBBI)) ? TRUE : FALSE;
    CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(AsWnd(pThis));
    if (pFrame != nullptr) {
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrame, TRUE);
    }
    return bResult;
}

// ---- CMenuImages state ----
// The unexported `static BOOL CMenuImages::m_bInitializing` (retail 0x3be258).
BOOL g_bMenuImagesInitializing = FALSE;
// DEVIATION (see the header comment): set when Initialize's LoadStr succeeds,
// cleared by CleanUp; stands in for the m_hbmImageWell / m_iCount tests that
// OpenMFC's LoadStr never satisfies.
BOOL g_bMenuImagesLoaded = FALSE;

constexpr std::size_t kTiOffCount          = 0x08;   // CMFCToolBarImages::m_iCount
constexpr std::size_t kTiOffSizeImage      = 0x68;   // CMFCToolBarImages::m_sizeImage (CSize)
constexpr std::size_t kTiOffImageWell      = 0xa0;   // CMFCToolBarImages::m_hbmImageWell
constexpr std::size_t kTiOffClrTransparent = 0xd8;   // CMFCToolBarImages::m_clrTransparent
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages retail size (afxmfc.h harvested layout)");

constexpr std::size_t kGdOffInitGate            = 0x000;
constexpr std::size_t kGdOffIsBlackHighContrast = 0x260;
constexpr std::size_t kGdOffIsWhiteHighContrast = 0x264;
constexpr std::size_t kGdOffUseBuiltIn32BitIcons= 0x268;
constexpr std::size_t kGdOffBitsPerPixel        = 0x288;
constexpr std::size_t kGdOffRibbonImageScale    = 0x2b8;   // double
constexpr std::size_t kGdOffIsRibbonImageScale  = 0x2c0;   // BOOL

constexpr unsigned int kIdbMenuImages   = 0x3f01u;   // IDB_AFXBARRES_MENU_IMAGES
constexpr unsigned int kIdbMenuImages24 = 0x42b8u;   // IDB_AFXBARRES_MENU_IMAGES24
constexpr unsigned long kClrMagenta = 0x00FF00FFul;  // RGB(255, 0, 255), the menu-image transparent colour

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(std::size_t off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline double GlobalDataDouble(std::size_t off) { double v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }

// The retail one-time gate, exactly as toolbar/CMFCToolBar.cpp reproduces it:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }   // 0x3c1620, 0x6a790
// KNOWN GAP: OpenMFC's AFX_GLOBAL_DATA::Initialize export is an empty stub
// (core/runtime/AFX_GLOBAL_DATA.cpp), so the gate flag is set but no field is
// populated; Is32BitIcons() / GetRibbonImageScale() below therefore read
// whatever the 720-byte block holds (zero unless a client wrote it).
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGdOffInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGdOffInitGate, &one, sizeof one);
    }
}
// AFX_GLOBAL_DATA::Is32BitIcons() (afxglobals.h, inline):
//   m_bUseBuiltIn32BitIcons && m_nBitsPerPixel >= 16 && !m_bIsBlackHighContrast && !m_bIsWhiteHighContrast
inline bool GlobalDataIs32BitIcons() {
    return GlobalDataInt(kGdOffUseBuiltIn32BitIcons) != 0 && GlobalDataInt(kGdOffBitsPerPixel) >= 16 &&
           GlobalDataInt(kGdOffIsBlackHighContrast) == 0 && GlobalDataInt(kGdOffIsWhiteHighContrast) == 0;
}
// AFX_GLOBAL_DATA::GetRibbonImageScale() (inline): m_bIsRibbonImageScale ? m_dblRibbonImageScale : 1.0
inline double GlobalDataRibbonImageScale() {
    return GlobalDataInt(kGdOffIsRibbonImageScale) != 0 ? GlobalDataDouble(kGdOffRibbonImageScale) : 1.0;
}
// The resource the menu images come from: Is32BitIcons() ? _MENU_IMAGES24 : _MENU_IMAGES
// (retail: neg/sbb/and $0x3b7/add $0x3f01, i.e. 0x3f01 + (is32 ? 0x3b7 : 0)).
inline const wchar_t* MenuImagesResource() {
    return MAKEINTRESOURCEW(GlobalDataIs32BitIcons() ? kIdbMenuImages24 : kIdbMenuImages);
}

inline CMFCToolBarImages* TiBlack();
inline CMFCToolBarImages* TiDkGray();
inline CMFCToolBarImages* TiGray();
inline CMFCToolBarImages* TiLtGray();
inline CMFCToolBarImages* TiWhite();
inline CMFCToolBarImages* TiBlack2();

// The state -> image-set selection shared by Draw (0x8f6a0) and SetColor
// (0x8f8f0): ImageBlack(0) -> m_ImagesBlack, ImageGray(1) -> m_ImagesGray,
// ImageDkGray(4) -> m_ImagesDkGray, ImageLtGray(2) -> m_ImagesLtGray,
// ImageWhite(3) -> m_ImagesWhite, anything else -> m_ImagesBlack2.
CMFCToolBarImages* ImagesForState(int state) {
    switch (state) {
    case 0:  return TiBlack();
    case 1:  return TiGray();
    case 4:  return TiDkGray();
    case 2:  return TiLtGray();
    case 3:  return TiWhite();
    default: return TiBlack2();
    }
}

} // namespace

// ===========================================================================
// CMFCReBar
// ===========================================================================

// See the header comment for the full transcription of RVA 0xd0580 (mfc140u).
// DEVIATION: the CMFCReBar vftable store is not reproduced (header comment);
// the C++ CPane vtable the CPane ctor thunk installs stays in place.
// Symbol: ??0CMFCReBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCReBar__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CPane__IEAA_XZ(pThis);
    impl___0CMFCControlBarImpl__QEAA_PEAVCPane___Z(ImplOf(pThis), pThis);
    CPane* p = AsPane(pThis);
    p->m_cxLeftBorder = 0;
    p->m_cxRightBorder = 0;
    p->m_cyTopBorder = 0;
    p->m_cyBottomBorder = 0;
    BandInfoSizeOf(pThis) = (ComCtlVersionPacked() < 0x60001ul) ? 112u : 128u;
    return pThis;
}

// Retail (RVA 0xd0990, mfc140u), fully transcribed:
//     REBARBANDINFO rbbi;                    // stack, NOT zero-initialised in retail
//     rbbi.fMask  = RBBIM_STYLE | RBBIM_COLORS;
//     rbbi.fStyle = dwStyle;  rbbi.clrFore = clrFore;  rbbi.clrBack = clrBack;
//     if (pszText != NULL) { rbbi.fMask |= RBBIM_TEXT; rbbi.lpText = (LPTSTR)pszText; }
//     return _AddMFCToolBar(pBar, &rbbi);    // 0xd07d0
// DEVIATION: the struct is zero-initialised here (retail leaves the members
// it does not set indeterminate; _AddMFCToolBar sets cbSize).
// Symbol: ?AddBar@CMFCReBar@@QEAAHPEAVCWnd@@KKPEB_WK@Z
extern "C" int MS_ABI impl__AddBar_CMFCReBar__QEAAHPEAVCWnd__KKPEB_WK_Z(
    void* pThis, CWnd* pBar, unsigned long clrFore, unsigned long clrBack, const wchar_t* pszText, unsigned long dwStyle) {
    if (pThis == nullptr) return FALSE;
    REBARBANDINFOW rbbi;
    std::memset(&rbbi, 0, sizeof rbbi);
    rbbi.fMask = RBBIM_STYLE | RBBIM_COLORS;
    rbbi.fStyle = static_cast<UINT>(dwStyle);
    rbbi.clrFore = static_cast<COLORREF>(clrFore);
    rbbi.clrBack = static_cast<COLORREF>(clrBack);
    if (pszText != nullptr) {
        rbbi.fMask |= RBBIM_TEXT;
        rbbi.lpText = const_cast<wchar_t*>(pszText);
    }
    return AddMFCToolBar(pThis, pBar, &rbbi);
}

// Retail (RVA 0xd0910, mfc140u), fully transcribed:
//     REBARBANDINFO rbbi;                    // stack, NOT zero-initialised in retail
//     rbbi.fStyle = dwStyle;  rbbi.fMask = RBBIM_STYLE;
//     if (pszText != NULL) { rbbi.fMask |= RBBIM_TEXT;       rbbi.lpText  = (LPTSTR)pszText; }
//     if (pbmp    != NULL) { rbbi.fMask |= RBBIM_BACKGROUND; rbbi.hbmBack = (HBITMAP)pbmp->m_hObject; }   // CGdiObject +0x8
//     return _AddMFCToolBar(pBar, &rbbi);    // 0xd07d0
// DEVIATION: the struct is zero-initialised here (see the other overload).
// Symbol: ?AddBar@CMFCReBar@@QEAAHPEAVCWnd@@PEB_WPEAVCBitmap@@K@Z
extern "C" int MS_ABI impl__AddBar_CMFCReBar__QEAAHPEAVCWnd__PEB_WPEAVCBitmap__K_Z(
    void* pThis, CWnd* pBar, const wchar_t* pszText, CBitmap* pbmp, unsigned long dwStyle) {
    if (pThis == nullptr) return FALSE;
    REBARBANDINFOW rbbi;
    std::memset(&rbbi, 0, sizeof rbbi);
    rbbi.fStyle = static_cast<UINT>(dwStyle);
    rbbi.fMask = RBBIM_STYLE;
    if (pszText != nullptr) {
        rbbi.fMask |= RBBIM_TEXT;
        rbbi.lpText = const_cast<wchar_t*>(pszText);
    }
    if (pbmp != nullptr) {
        rbbi.fMask |= RBBIM_BACKGROUND;
        rbbi.hbmBack = static_cast<HBITMAP>(pbmp->m_hObject);
    }
    return AddMFCToolBar(pThis, pBar, &rbbi);
}

// Retail (RVA 0xd0a10, mfc140u).  The CSize result is returned through the
// hidden pointer in RDX (bStretch = R8D is never read, bHorz = R9D):
//     ENSURE(::IsWindow(m_hWnd));
//     int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);              // vslot 73
//     REBARBANDINFO rbbi;  rbbi.cbSize = m_nReBarBandInfoSize;
//     // pass 1: sync each band's RBBS_HIDDEN with its child window's visibility
//     for (int i = nCount - 1; i >= 0; i--) {
//         rbbi.fMask = RBBIM_STYLE | RBBIM_CHILD;
//         DefWindowProc(RB_GETBANDINFO, i, (LPARAM)&rbbi);
//         BOOL bWindowVisible = <BandChildIsVisible(rbbi.hwndChild)>;
//         BOOL bBandVisible = (rbbi.fStyle & RBBS_HIDDEN) == 0;             // shr $3; not; and $1
//         if (bWindowVisible != bBandVisible)
//             DefWindowProc(RB_SHOWBAND, i, bWindowVisible);
//     }
//     // pass 2: union of the visible bands' rectangles
//     CRect rectTotal;  rectTotal.SetRectEmpty();
//     for (int i = nCount - 1; i >= 0; i--) {
//         rbbi.fMask = RBBIM_STYLE;
//         DefWindowProc(RB_GETBANDINFO, i, (LPARAM)&rbbi);
//         if (!(rbbi.fStyle & RBBS_HIDDEN)) {
//             CRect rect;  rect.SetRectEmpty();
//             DefWindowProc(RB_GETRECT, i, (LPARAM)&rect);
//             ::UnionRect(&rectTotal, &rectTotal, &rect);
//         }
//     }
//     int cx, cy;
//     if (::IsRectEmpty(&rectTotal)) {
//         CRect rect;  rect.SetRectEmpty();
//         CalcInsideRect(rect, bHorz);                                      // 0xa1950 (CPane, direct call)
//         cx = rectTotal.right  + (rect.left - rect.right);
//         cy = rectTotal.bottom + (rect.top  - rect.bottom);
//     } else { cx = rectTotal.right;  cy = rectTotal.bottom; }
//     if (bHorz) return CSize(32767, cy - rectTotal.top);
//     else       return CSize(cx - rectTotal.left, 32767);
// DEVIATION: none beyond the shared ones (rbbi is zero-initialised; a NULL
// this / out pointer returns CSize(0,0); the throw path returns the same).
// Symbol: ?CalcFixedLayout@CMFCReBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCReBar__UEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int bStretch, int bHorz) {
    (void)bStretch;
    if (pRet == nullptr) return nullptr;
    *pRet = CSize(0, 0);
    if (pThis == nullptr) return pRet;
    if (!::IsWindow(HWndOf(pThis))) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pRet;   // not reached: the thunk throws
    }
    const int nCount = static_cast<int>(RbDefWindowProc(pThis, RB_GETBANDCOUNT, 0, 0));
    REBARBANDINFOW rbbi;
    std::memset(&rbbi, 0, sizeof rbbi);
    rbbi.cbSize = BandInfoSizeOf(pThis);

    for (int i = nCount - 1; i >= 0; i--) {
        rbbi.fMask = RBBIM_STYLE | RBBIM_CHILD;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, static_cast<unsigned __int64>(i), reinterpret_cast<__int64>(&rbbi));
        const int bWindowVisible = BandChildIsVisible(rbbi.hwndChild);
        const int bBandVisible = (rbbi.fStyle & RBBS_HIDDEN) ? 0 : 1;
        if (bWindowVisible != bBandVisible) {
            RbDefWindowProc(pThis, RB_SHOWBAND, static_cast<unsigned __int64>(i), bWindowVisible);
        }
    }

    RECT rectTotal;
    ::SetRectEmpty(&rectTotal);
    for (int i = nCount - 1; i >= 0; i--) {
        rbbi.fMask = RBBIM_STYLE;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, static_cast<unsigned __int64>(i), reinterpret_cast<__int64>(&rbbi));
        if (!(rbbi.fStyle & RBBS_HIDDEN)) {
            RECT rect;
            ::SetRectEmpty(&rect);
            RbDefWindowProc(pThis, RB_GETRECT, static_cast<unsigned __int64>(i), reinterpret_cast<__int64>(&rect));
            ::UnionRect(&rectTotal, &rectTotal, &rect);
        }
    }

    int cx, cy;
    if (::IsRectEmpty(&rectTotal)) {
        RECT rect;
        ::SetRectEmpty(&rect);
        impl__CalcInsideRect_CPane__QEBAXAEAVCRect__H_Z(AsPane(pThis), &rect, bHorz);
        cx = rectTotal.right + (rect.left - rect.right);
        cy = rectTotal.bottom + (rect.top - rect.bottom);
    } else {
        cx = rectTotal.right;
        cy = rectTotal.bottom;
    }
    if (bHorz) {
        *pRet = CSize(32767, cy - rectTotal.top);
    } else {
        *pRet = CSize(cx - rectTotal.left, 32767);
    }
    return pRet;
}

// Retail (RVA 0xd0c60, mfc140u), fully transcribed:
//     ENSURE(AfxIsExtendedFrameClass(pParentWnd));                         // 0x6c840
//     m_dwStyle = dwStyle & CBRS_ALL;                                       // +0x104, & 0x40ffff
//     if (nID == AFX_IDW_REBAR /*0xe804*/) m_dwStyle |= CBRS_HIDE_INPLACE;  // | 0x8
//     m_pDockSite = (CDockSite*)pParentWnd;                                 // +0x110, raw store
//     AfxEndDeferRegisterClass(0x8000 /*the rebar AFX_WNDCOMMCTL_*_REG bit*/); // 0x2918f0
//     CRect rect;  rect.SetRectEmpty();
//     dwStyle = (dwStyle & 0xffbf024e) | dwCtrlStyle | 0x0200024e;
//         // i.e. dwStyle &= ~CBRS_ALL; dwStyle |= dwCtrlStyle | WS_CLIPCHILDREN |
//         //      CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE | RBS_VARHEIGHT
//         //      (the 0x24e bits survive the AND only to be ORed back in)
//     return CWnd::Create(REBARCLASSNAME /*L"ReBarWindow32", 0x3406e0*/, NULL, dwStyle,
//                         rect, pParentWnd, nID, NULL) != 0;                // 0x28b690, direct call
// DEVIATIONS: (1) the AfxIsExtendedFrameClass gate is not applied: OpenMFC's
// export is a generated stub returning 0 (build-phase4/typed_stubs.cpp), so
// applying it would reject every parent -- the same choice
// controls/CMFCStatusBar.cpp makes.  (2) CWnd::Create is the exported CWnd
// body, which runs CWnd's PreCreateWindow rather than a client override
// (retail's call is non-virtual too, but its CWnd::Create dispatches
// PreCreateWindow virtually).
// Symbol: ?Create@CMFCReBar@@QEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__Create_CMFCReBar__QEAAHPEAVCWnd__KKI_Z(
    void* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, unsigned int nID) {
    if (pThis == nullptr) return FALSE;
    CBasePane* pBase = AsBasePane(pThis);
    pBase->m_dwStyle = static_cast<DWORD>(dwStyle & kCbrsAll);
    if (nID == kAfxIdwReBar) {
        pBase->m_dwStyle |= static_cast<DWORD>(kCbrsHideInplace);
    }
    pBase->m_pDockSite = reinterpret_cast<CDockSite*>(pParentWnd);
    impl__AfxEndDeferRegisterClass__YAHJ_Z(kAfxWndCommCtlReBarReg);
    RECT rect;
    ::SetRectEmpty(&rect);
    dwStyle = (dwStyle & 0xffbf024eul) | dwCtrlStyle | 0x0200024eul;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
               AsWnd(pThis), REBARCLASSNAMEW, nullptr, dwStyle, rect, pParentWnd, nID, nullptr) != 0 ? TRUE : FALSE;
}

// Retail (RVA 0x10130, mfc140u): `mov %rcx,%rax; ret` -- the inline
// `return *(CReBarCtrl*)this;` of afxrebar.h, COMDAT-folded with every other
// identity accessor in the image.
// Symbol: ?GetReBarCtrl@CMFCReBar@@QEBAAEAVCReBarCtrl@@XZ
extern "C" void* MS_ABI impl__GetReBarCtrl_CMFCReBar__QEBAAEAVCReBarCtrl__XZ(void* pThis) {
    return pThis;
}

// Retail: the WM_ERASEBKGND entry of the message map is 0xda30 (mfc140u),
// `jmp ?Default@CWnd@@IEAA_JXZ` (0x28ac80) -- `return (BOOL)Default();`, the
// same folded body WM_PAINT / WM_LBUTTONDOWN / WM_LBUTTONUP / WM_MOUSEMOVE
// use.  pDC is not read.
// Symbol: ?OnEraseBkgnd@CMFCReBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCReBar__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pDC;
    if (pThis == nullptr) return FALSE;
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis)));
}

// Retail (RVA 0xd0690, mfc140u; the reflected RBN_HEIGHTCHANGE and RBN_ENDDRAG
// entries of the message map), fully transcribed:
//     AdjustDockingLayout(NULL);            // vslot 156 (+0x4e0) = CBasePane::AdjustDockingLayout
//     *pResult = 0;
// pNMHDR is not read.  DEVIATION: AdjustDockingLayout is devirtualised to the
// exported CBasePane body (retail's own slot holds exactly that).
// Symbol: ?OnHeightChange@CMFCReBar@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnHeightChange_CMFCReBar__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pNMHDR;
    if (pThis == nullptr) return;
    impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(AsBasePane(pThis), nullptr);
    if (pResult != nullptr) *pResult = 0;
}

// Retail: 0xda30 (mfc140u), `jmp CWnd::Default` -- `Default();` (see
// OnEraseBkgnd).  nFlags / point are not read.
// Symbol: ?OnLButtonDown@CMFCReBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCReBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags; (void)point;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail: 0xda30 (mfc140u), `jmp CWnd::Default` -- `Default();`.
// Symbol: ?OnLButtonUp@CMFCReBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCReBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags; (void)point;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail: 0xda30 (mfc140u), `jmp CWnd::Default` -- `Default();`.
// Symbol: ?OnMouseMove@CMFCReBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCReBar__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, CPoint point) {
    (void)nFlags; (void)point;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail (RVA 0xd0db0, mfc140u; the WM_NCCALCSIZE entry), fully transcribed:
//     CRect rect;  rect.SetRectEmpty();
//     CalcInsideRect(rect, (m_dwStyle & CBRS_ORIENT_HORZ) != 0);           // 0xa1950 (CPane, direct)
//     for (int i = 0; i < 4; i++)                                           // left, top, right, bottom
//         ((LONG*)&lpncsp->rgrc[0])[i] += ((LONG*)&rect)[i];
// bCalcValidRects is not read.  (CalcInsideRect on an empty rect yields the
// border insets: left/top positive, right/bottom negative.)
// Symbol: ?OnNcCalcSize@CMFCReBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCReBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (pThis == nullptr || lpncsp == nullptr) return;
    RECT rect;
    ::SetRectEmpty(&rect);
    const int bHorz = (AsBasePane(pThis)->m_dwStyle & kCbrsOrientHorz) ? 1 : 0;
    impl__CalcInsideRect_CPane__QEBAXAEAVCRect__H_Z(AsPane(pThis), &rect, bHorz);
    lpncsp->rgrc[0].left   += rect.left;
    lpncsp->rgrc[0].top    += rect.top;
    lpncsp->rgrc[0].right  += rect.right;
    lpncsp->rgrc[0].bottom += rect.bottom;
}

// Retail (RVA 0xd0d60, mfc140u; the WM_NCCREATE entry), fully transcribed:
//     if (!Default()) return FALSE;                                        // 0x28ac80
//     if (m_hWndOwner != NULL)                                              // CWnd +0xa0
//         DefWindowProc(RB_SETPARENT, (WPARAM)m_hWndOwner, 0);              // vslot 73
//     return TRUE;
// lpCreateStruct is not read.  m_hWndOwner is read by offset (RawOwnerHwnd,
// header comment); it is non-NULL only when the client called SetOwner
// before creation.
// Symbol: ?OnNcCreate@CMFCReBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CMFCReBar__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (pThis == nullptr) return FALSE;
    if (!impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis))) return FALSE;
    const HWND hWndOwner = RawOwnerHwnd(pThis);
    if (hWndOwner != nullptr) {
        RbDefWindowProc(pThis, RB_SETPARENT, reinterpret_cast<unsigned __int64>(hWndOwner), 0);
    }
    return TRUE;
}

// Retail (RVA 0xd0e40, mfc140u; the WM_NCPAINT entry): `add $0x3f8,%rcx; jmp
// ?DrawNcArea@CMFCControlBarImpl@@IEAAXXZ` (0x31620) -- `m_Impl.DrawNcArea();`.
// Symbol: ?OnNcPaint@CMFCReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCReBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__DrawNcArea_CMFCControlBarImpl__IEAAXXZ(ImplOf(pThis));
}

// Retail: 0xda30 (mfc140u), `jmp CWnd::Default` -- `Default();`.
// Symbol: ?OnPaint@CMFCReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCReBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail (RVA 0xd0650, mfc140u; the WM_RECALCPARENT entry), fully transcribed:
//     CFrameWnd* pFrame = AFXGetParentFrame(this);                          // 0x6bd00
//     ENSURE(pFrame != NULL);
//     pFrame->RecalcLayout(TRUE);                                           // frame vslot 96 (+0x300), tail call
// DEVIATION: RecalcLayout goes to the exported CFrameWnd body (see
// AddMFCToolBar), not a CFrameWndEx / CMDIFrameWndEx override.
// Symbol: ?OnRecalcParent@CMFCReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnRecalcParent_CMFCReBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(AsWnd(pThis));
    if (pFrame == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;   // not reached: the thunk throws
    }
    impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrame, TRUE);
}

// Retail (RVA 0xd06c0, mfc140u; the RB_SHOWBAND entry), fully transcribed:
//     LRESULT lResult = Default();                                          // 0x28ac80
//     if (lResult) {
//         REBARBANDINFO rbbi;  rbbi.cbSize = m_nReBarBandInfoSize;
//         rbbi.fMask = RBBIM_STYLE | RBBIM_CHILD;
//         DefWindowProc(RB_GETBANDINFO, wParam /*band index*/, (LPARAM)&rbbi);   // vslot 73
//         BOOL bWindowVisible = <BandChildIsVisible(rbbi.hwndChild)>;
//         BOOL bBandVisible = (rbbi.fStyle & RBBS_HIDDEN) == 0;
//         if (bWindowVisible != bBandVisible)
//             ::ShowWindow(rbbi.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE);   // neg/sbb/not/and $5
//     }
//     return lResult;
// lParam is not read.
// Symbol: ?OnShowBand@CMFCReBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnShowBand_CMFCReBar__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    if (pThis == nullptr) return 0;
    const __int64 lResult = impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    if (lResult) {
        REBARBANDINFOW rbbi;
        std::memset(&rbbi, 0, sizeof rbbi);
        rbbi.cbSize = BandInfoSizeOf(pThis);
        rbbi.fMask = RBBIM_STYLE | RBBIM_CHILD;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, wParam, reinterpret_cast<__int64>(&rbbi));
        const int bWindowVisible = BandChildIsVisible(rbbi.hwndChild);
        const int bBandVisible = (rbbi.fStyle & RBBS_HIDDEN) ? 0 : 1;
        if (bWindowVisible != bBandVisible) {
            ::ShowWindow(rbbi.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE);
        }
    }
    return lResult;
}

// Retail (RVA 0xd0e50, mfc140u), fully transcribed:
//     ENSURE(::IsWindow(m_hWnd));
//     HWND hWndChild = _AfxChildWindowFromPoint(m_hWnd, point);             // 0xd0f50 (unexported helper)
//     CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);                    // 0x28adc0
//     if (pWnd == NULL) return CWnd::OnToolHitTest(point, pTI);             // 0x28bcc0, direct call
//     ENSURE(pWnd->m_hWnd == hWndChild);
//     return pWnd->OnToolHitTest(point, pTI);                               // CWnd vslot 31 (+0xf8)
// The helper 0xd0f50 IS an export, ?ChWindowFromPoint@@YAPEAUHWND__@@PEAU1@UtagPOINT@@@Z
// (ordinal 2724), and is called through its thunk.  Its retail body, for the
// record (CMFC_misc_stubs.cpp's current body is a different algorithm, a
// ::ChildWindowFromPoint / ::MapWindowPoints descent): ENSURE(hWnd);
// ::ClientToScreen(hWnd, &pt); then walk ::GetWindow(GW_CHILD) /
// ::GetWindow(GW_HWNDNEXT) and return the first child whose ::GetDlgCtrlID
// != 0xffff, whose GWL_STYLE has WS_VISIBLE and whose ::GetWindowRect
// contains pt (::PtInRect); NULL if none.
// DEVIATION: the child's OnToolHitTest cannot be dispatched through its
// vtable: a CMFCToolBar-derived child goes to the exported
// CMFCToolBar::OnToolHitTest, everything else to the exported CWnd body, so
// other pane classes' overrides are not seen.
// Symbol: ?OnToolHitTest@CMFCReBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCReBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const void* pThis, CPoint point, TOOLINFOW* pTI) {
    if (pThis == nullptr) return -1;
    const HWND hWnd = HWndOf(pThis);
    if (!::IsWindow(hWnd)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;   // not reached: the thunk throws
    }
    const POINT pt = { point.x, point.y };
    const HWND hWndChild = static_cast<HWND>(impl__ChWindowFromPoint__YAPEAUHWND____PEAU1_UtagPOINT___Z(hWnd, pt));
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hWndChild);
    if (pWnd == nullptr) {
        return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
            static_cast<const CWnd*>(static_cast<const CPane*>(pThis)), point, pTI);
    }
    if (pWnd->m_hWnd != hWndChild) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;   // not reached: the thunk throws
    }
    if (IsKindOfToolBar(pWnd)) {
        return impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(static_cast<const CMFCToolBar*>(pWnd), point, pTI);
    }
    return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pWnd, point, pTI);
}

// Retail (RVA 0xd0d50, mfc140u): `jmp ?UpdateDialogControls@CWnd@@QEAAXPEAVCCmdTarget@@H@Z`
// (0x291460) -- `UpdateDialogControls(pTarget, bDisableIfNoHndler);`.
// Symbol: ?OnUpdateCmdUI@CMFCReBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCReBar__UEAAXPEAVCFrameWnd__H_Z(void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(AsWnd(pThis), static_cast<CCmdTarget*>(pTarget), bDisableIfNoHndler);
}

// Retail (RVA 0xd1010, mfc140u), fully transcribed -- note it does NOT change
// this pane's own alignment, only its bands' children:
//     UINT nCount = (UINT)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);       // import 0x1802c7120
//     REBARBANDINFO rbbi;  rbbi.cbSize = m_nReBarBandInfoSize;
//     rbbi.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;         // 0x230
//     for (UINT i = 0; i < nCount; i++) {
//         ::SendMessage(m_hWnd, RB_GETBANDINFO, i, (LPARAM)&rbbi);
//         if (rbbi.hwndChild == NULL) continue;
//         CWnd* pWnd = CWnd::FromHandlePermanent(rbbi.hwndChild);           // 0x28adc0
//         if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CBasePane)))     // RTC 0x1802dc540
//             ((CBasePane*)pWnd)->SetPaneAlignment(dwAlignment);            // vslot 122 (+0x3d0)
//     }
// DEVIATION: the child's SetPaneAlignment cannot be dispatched through its
// vtable; CBasePane's inline body (afxbasepane.h: m_dwStyle =
// (m_dwStyle & ~CBRS_ALIGN_ANY) | dwAlignment) is applied directly, so an
// override (CMFCReBar's own, or a client's) on a child is not seen.
// Symbol: ?SetPaneAlignment@CMFCReBar@@UEAAXK@Z
extern "C" void MS_ABI impl__SetPaneAlignment_CMFCReBar__UEAAXK_Z(void* pThis, unsigned long dwAlignment) {
    if (pThis == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    const UINT nCount = static_cast<UINT>(::SendMessageW(hWnd, RB_GETBANDCOUNT, 0, 0));
    REBARBANDINFOW rbbi;
    std::memset(&rbbi, 0, sizeof rbbi);
    rbbi.cbSize = BandInfoSizeOf(pThis);
    rbbi.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
    for (UINT i = 0; i < nCount; i++) {
        ::SendMessageW(hWnd, RB_GETBANDINFOW, i, reinterpret_cast<LPARAM>(&rbbi));
        if (rbbi.hwndChild == nullptr) continue;
        CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(rbbi.hwndChild);
        if (pWnd != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ())) {
            CBasePane* pChild = static_cast<CBasePane*>(pWnd);
            pChild->m_dwStyle = static_cast<DWORD>((pChild->m_dwStyle & ~kCbrsAlignAny) | dwAlignment);
        }
    }
}

// Retail (RVA 0xd0ed0, mfc140u), fully transcribed:
//     if (message == WM_SETMESSAGESTRING /*0x362*/ || message == WM_POPMESSAGESTRING /*0x375*/) {
//         CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd));   // +0xa0, import GetParent, 0x28ad70
//         return ::SendMessage(pOwner->m_hWnd, message, wParam, lParam);   // import 0x1802c7120
//     }
//     return CBasePane::WindowProc(message, wParam, lParam);                // 0xca20, direct call
// DEVIATIONS: (1) retail dereferences the FromHandle result unconditionally;
// a NULL result returns 0 here instead.  (2) the CBasePane::WindowProc export is still a
// placeholder returning 0 (see its declaration above), so every forwarded
// message currently yields 0 -- retail's body (0xca20) forwards the
// owner-draw / WM_NOTIFY family to the owner and otherwise runs
// CWnd::WindowProc.
// Symbol: ?WindowProc@CMFCReBar@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CMFCReBar__MEAA_JI_K_J_Z(void* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    if (message == kWmSetMessageString || message == kWmPopMessageString) {
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
        if (pOwner == nullptr) return 0;
        return static_cast<__int64>(::SendMessageW(pOwner->m_hWnd, message, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam)));
    }
    return impl__WindowProc_CBasePane__MEAA_JI_K_J_Z(AsBasePane(pThis), message, wParam, lParam);
}

// ===========================================================================
// CMenuImages
// ===========================================================================

// The six exported statics: retail-sized storage, constructed and destroyed
// through the exported CMFCToolBarImages ctor / dtor thunks by the
// file-scope initialiser that follows (header comment).
// Symbol: ?m_ImagesBlack@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesBlack_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};
// Symbol: ?m_ImagesDkGray@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesDkGray_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};
// Symbol: ?m_ImagesGray@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesGray_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};
// Symbol: ?m_ImagesLtGray@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesLtGray_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};
// Symbol: ?m_ImagesWhite@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesWhite_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};
// Symbol: ?m_ImagesBlack2@CMenuImages@@1VCMFCToolBarImages@@A
extern "C" alignas(16) unsigned char impl__m_ImagesBlack2_CMenuImages__1VCMFCToolBarImages__A[sizeof(CMFCToolBarImages)] = {};

namespace {
inline CMFCToolBarImages* TiBlack()  { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesBlack_CMenuImages__1VCMFCToolBarImages__A); }
inline CMFCToolBarImages* TiDkGray() { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesDkGray_CMenuImages__1VCMFCToolBarImages__A); }
inline CMFCToolBarImages* TiGray()   { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesGray_CMenuImages__1VCMFCToolBarImages__A); }
inline CMFCToolBarImages* TiLtGray() { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesLtGray_CMenuImages__1VCMFCToolBarImages__A); }
inline CMFCToolBarImages* TiWhite()  { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesWhite_CMenuImages__1VCMFCToolBarImages__A); }
inline CMFCToolBarImages* TiBlack2() { return reinterpret_cast<CMFCToolBarImages*>(impl__m_ImagesBlack2_CMenuImages__1VCMFCToolBarImages__A); }

// Runs the six constructions at DLL initialisation, in the order of their
// retail .data addresses (Black, DkGray, Gray, LtGray, White, Black2), and
// the six destructions, in reverse, at exit.  (Retail's own dynamic
// initialiser order was not checked; nothing in the bodies depends on it.)
struct MenuImagesStaticInit {
    MenuImagesStaticInit() {
        impl___0CMFCToolBarImages__QEAA_XZ(TiBlack());
        impl___0CMFCToolBarImages__QEAA_XZ(TiDkGray());
        impl___0CMFCToolBarImages__QEAA_XZ(TiGray());
        impl___0CMFCToolBarImages__QEAA_XZ(TiLtGray());
        impl___0CMFCToolBarImages__QEAA_XZ(TiWhite());
        impl___0CMFCToolBarImages__QEAA_XZ(TiBlack2());
    }
    ~MenuImagesStaticInit() {
        impl___1CMFCToolBarImages__UEAA_XZ(TiBlack2());
        impl___1CMFCToolBarImages__UEAA_XZ(TiWhite());
        impl___1CMFCToolBarImages__UEAA_XZ(TiLtGray());
        impl___1CMFCToolBarImages__UEAA_XZ(TiGray());
        impl___1CMFCToolBarImages__UEAA_XZ(TiDkGray());
        impl___1CMFCToolBarImages__UEAA_XZ(TiBlack());
    }
};
MenuImagesStaticInit g_menuImagesStaticInit;
} // namespace

// Retail (RVA 0x8f840, mfc140u), fully transcribed:
//     if (m_bInitializing) return;                                          // 0x3be258
//     if (m_ImagesBlack.m_iCount <= 0) return;                              // +0x08 of 0x3c1900
//     m_ImagesBlack.Clear(); m_ImagesGray.Clear(); m_ImagesDkGray.Clear();  // 0x16f690, in this order
//     m_ImagesLtGray.Clear(); m_ImagesWhite.Clear(); m_ImagesBlack2.Clear();
// DEVIATION: the m_iCount guard is widened with g_bMenuImagesLoaded (header
// comment: OpenMFC's LoadStr never writes m_iCount), and the flag is cleared.
// Symbol: ?CleanUp@CMenuImages@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CMenuImages__SAXXZ() {
    if (g_bMenuImagesInitializing) return;
    if (At<int>(TiBlack(), kTiOffCount) <= 0 && !g_bMenuImagesLoaded) return;
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiBlack());
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiGray());
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiDkGray());
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiLtGray());
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiWhite());
    impl__Clear_CMFCToolBarImages__QEAAXXZ(TiBlack2());
    g_bMenuImagesLoaded = FALSE;
}

// Retail (RVA 0x8f8b0, mfc140u), fully transcribed:
//     m_ImagesBlack.CopyTo(images);                                                     // 0x16f2f0
//     CMFCToolBarImages::MapBmpTo3dColors(images.m_hbmImageWell, TRUE, RGB(0,0,0), clr);  // tail jump 0x16fc10; +0xa0
// (Both callees are stubs in OpenMFC today -- header comment.)
// Symbol: ?CreateCopy@CMenuImages@@KAXAEAVCMFCToolBarImages@@K@Z
extern "C" void MS_ABI impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(CMFCToolBarImages* pImages, unsigned long clr) {
    if (pImages == nullptr) return;
    impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(TiBlack(), pImages);
    impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(&At<HBITMAP>(pImages, kTiOffImageWell), TRUE, 0, clr);
}

// Retail (RVA 0x8f6a0, mfc140u), fully transcribed:
//     if (!Initialize()) return;                                            // 0x8f3c0
//     CMFCToolBarImages* pImages = <ImagesForState(state)>;
//     CAfxDrawState ds;
//     pImages->PrepareDrawImage(ds, sizeImage, FALSE);                      // 0x16c060, CSize by value
//     pImages->Draw(pDC, ptImage.x, ptImage.y, id, FALSE, FALSE, FALSE, FALSE, FALSE, 255);   // 0x16c6d0
//     pImages->EndDrawImage(ds);                                            // 0x16c270
// (PrepareDrawImage / Draw are stubs in OpenMFC today -- header comment.)
// Symbol: ?Draw@CMenuImages@@SAXPEAVCDC@@W4IMAGES_IDS@1@AEBVCPoint@@W4IMAGE_STATE@1@AEBVCSize@@@Z
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int id, const CPoint* ptImage, int state, const CSize* sizeImage) {
    if (ptImage == nullptr || sizeImage == nullptr) return;
    if (!impl__Initialize_CMenuImages__KAHXZ()) return;
    CMFCToolBarImages* pImages = ImagesForState(state);
    HBITMAP ds[3] = { nullptr, nullptr, nullptr };   // CAfxDrawState { hbmMono, hbmMonoOld, hbmOldGlyphs }
    long long sizePacked;
    std::memcpy(&sizePacked, sizeImage, sizeof sizePacked);
    impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(pImages, ds, sizePacked, FALSE);
    impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(pImages, pDC, ptImage->x, ptImage->y, id,
                                                           FALSE, FALSE, FALSE, FALSE, FALSE, 255);
    impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(pImages, ds);
}

// Retail (RVA 0x8f790, mfc140u), fully transcribed:
//     CSize size = (sizeImage.cx == 0 && sizeImage.cy == 0) ? Size() : sizeImage;   // 0x8f600
//     int dx = rectImage.Width()  - size.cx;
//     int dy = rectImage.Height() - size.cy;
//     CPoint pt(rectImage.left + dx / 2 + dx % 2,                           // cltd/idiv: quotient AND remainder added
//               rectImage.top  + dy / 2 + dy % 2);
//     Draw(pDC, id, pt, state, sizeImage);                                  // 0x8f6a0 -- the ORIGINAL sizeImage
// Symbol: ?Draw@CMenuImages@@SAXPEAVCDC@@W4IMAGES_IDS@1@AEBVCRect@@W4IMAGE_STATE@1@AEBVCSize@@@Z
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int id, const CRect* rectImage, int state, const CSize* sizeImage) {
    if (rectImage == nullptr || sizeImage == nullptr) return;
    CSize size;
    if (sizeImage->cx == 0 && sizeImage->cy == 0) {
        impl__Size_CMenuImages__SA_AVCSize__XZ(&size);
    } else {
        size = *sizeImage;
    }
    const int dx = (rectImage->right - rectImage->left) - size.cx;
    const int dy = (rectImage->bottom - rectImage->top) - size.cy;
    const CPoint pt(rectImage->left + dx / 2 + dx % 2, rectImage->top + dy / 2 + dy % 2);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, id, &pt, state, sizeImage);
}

// Retail (RVA 0x8f3c0, mfc140u), fully transcribed:
//     if (m_bInitializing) return FALSE;                                    // 0x3be258
//     if (m_ImagesBlack.m_hbmImageWell != NULL) return TRUE;                // +0xa0 of 0x3c1900
//     m_bInitializing = TRUE;
//     m_ImagesBlack.Initialize(); m_ImagesDkGray.Initialize(); m_ImagesGray.Initialize();   // 0x16b450, in this order
//     m_ImagesLtGray.Initialize(); m_ImagesWhite.Initialize(); m_ImagesBlack2.Initialize();
//     m_ImagesBlack.m_sizeImage = CSize(9, 9);                              // +0x68 / +0x6c, direct stores
//     <afxGlobalData init gate>
//     BOOL bResult = TRUE;
//     if (!m_ImagesBlack.LoadStr(MAKEINTRESOURCE(Is32BitIcons() ? IDB_AFXBARRES_MENU_IMAGES24
//                                                               : IDB_AFXBARRES_MENU_IMAGES), NULL, FALSE)) {   // 0x16b6c0
//         bResult = FALSE;
//     } else {
//         if (CMFCToolBarImages::m_bIsRTL) m_ImagesBlack.Mirror();          // 0x3be390, 0x170810
//         if (m_ImagesBlack.m_clrTransparent != RGB(255,0,255)) m_ImagesBlack.m_clrTransparent = RGB(255,0,255);   // +0xd8
//         CreateCopy(m_ImagesGray,   RGB(128,128,128));                     // 0x8f8b0
//         CreateCopy(m_ImagesDkGray, RGB( 72, 72, 72));
//         CreateCopy(m_ImagesLtGray, RGB(192,192,192));
//         CreateCopy(m_ImagesWhite,  RGB(255,255,255));
//         CreateCopy(m_ImagesBlack2, RGB(  0,  0,  0));
//         if (m_ImagesBlack.m_hbmImageWell != NULL) {
//             <afxGlobalData init gate>
//             double dblScale = afxGlobalData.GetRibbonImageScale();        // +0x2c0 ? +0x2b8 : 1.0
//             if (dblScale != 1.0) {
//                 m_ImagesBlack.SmoothResize(dblScale); m_ImagesGray.SmoothResize(dblScale);   // 0x171a30, in this order
//                 m_ImagesDkGray.SmoothResize(dblScale); m_ImagesLtGray.SmoothResize(dblScale);
//                 m_ImagesWhite.SmoothResize(dblScale); m_ImagesBlack2.SmoothResize(dblScale);
//             }
//         }
//     }
//     m_bInitializing = FALSE;
//     return bResult;
// DEVIATION: the two m_hbmImageWell tests are widened with
// g_bMenuImagesLoaded (header comment: OpenMFC's LoadStr never stores the
// well), so a second call returns TRUE early as retail's would after a
// successful load, and the SmoothResize block is reached.
// Symbol: ?Initialize@CMenuImages@@KAHXZ
extern "C" int MS_ABI impl__Initialize_CMenuImages__KAHXZ() {
    if (g_bMenuImagesInitializing) return FALSE;
    if (At<HBITMAP>(TiBlack(), kTiOffImageWell) != nullptr || g_bMenuImagesLoaded) return TRUE;
    g_bMenuImagesInitializing = TRUE;

    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiBlack());
    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiDkGray());
    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiGray());
    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiLtGray());
    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiWhite());
    impl__Initialize_CMFCToolBarImages__QEAAXXZ(TiBlack2());
    At<CSize>(TiBlack(), kTiOffSizeImage) = CSize(9, 9);

    EnsureGlobalDataInitialized();
    int bResult = TRUE;
    if (!impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(TiBlack(), MenuImagesResource(), nullptr, FALSE)) {
        bResult = FALSE;
    } else {
        g_bMenuImagesLoaded = TRUE;
        if (impl__m_bIsRTL_CMFCToolBarImages__1HA) {
            impl__Mirror_CMFCToolBarImages__QEAAHXZ(TiBlack());
        }
        if (At<COLORREF>(TiBlack(), kTiOffClrTransparent) != kClrMagenta) {
            At<COLORREF>(TiBlack(), kTiOffClrTransparent) = kClrMagenta;
        }
        impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(TiGray(),   RGB(128, 128, 128));
        impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(TiDkGray(), RGB(72, 72, 72));
        impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(TiLtGray(), RGB(192, 192, 192));
        impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(TiWhite(),  RGB(255, 255, 255));
        impl__CreateCopy_CMenuImages__KAXAEAVCMFCToolBarImages__K_Z(TiBlack2(), RGB(0, 0, 0));
        if (At<HBITMAP>(TiBlack(), kTiOffImageWell) != nullptr || g_bMenuImagesLoaded) {
            EnsureGlobalDataInitialized();
            const double dblScale = GlobalDataRibbonImageScale();
            if (dblScale != 1.0) {
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiBlack(),  dblScale);
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiGray(),   dblScale);
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiDkGray(), dblScale);
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiLtGray(), dblScale);
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiWhite(),  dblScale);
                impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(TiBlack2(), dblScale);
            }
        }
    }
    g_bMenuImagesInitializing = FALSE;
    return bResult;
}

// Retail (RVA 0x8f8f0, mfc140u), fully transcribed:
//     Initialize();                                                         // 0x8f3c0, result ignored
//     CMFCToolBarImages images;                                             // 0x16b0f0 (local, at rsp+0x20)
//     images.m_sizeImage = CSize(9, 9);                                     // +0x68 / +0x6c
//     <afxGlobalData init gate>
//     images.LoadStr(MAKEINTRESOURCE(Is32BitIcons() ? IDB_AFXBARRES_MENU_IMAGES24
//                                                   : IDB_AFXBARRES_MENU_IMAGES), NULL, FALSE);   // 0x16b6c0, result ignored
//     images.m_clrTransparent = RGB(255,0,255);                             // +0xd8 (cmp/cmovne == unconditional store)
//     if (CMFCToolBarImages::m_bIsRTL)                                      // 0x3be390
//         CMFCToolBarImages::MirrorBitmap(images.m_hbmImageWell, images.m_sizeImage.cx);   // 0x170510
//     CMFCToolBarImages* pImages = <ImagesForState(state)>;
//     if (color != (COLORREF)-1)
//         CMFCToolBarImages::MapBmpTo3dColors(images.m_hbmImageWell, TRUE, RGB(0,0,0), color);   // 0x16fc10
//     if (!m_bInitializing) {                                               // 0x3be258
//         <afxGlobalData init gate>
//         images.SmoothResize(afxGlobalData.GetRibbonImageScale());         // 0x171a30 -- called even for 1.0
//     }
//     pImages->Clear();                                                     // 0x16f690
//     images.CopyTo(*pImages);                                              // 0x16f2f0
//     images.~CMFCToolBarImages();                                          // 0x16b550
// The local is built and torn down through the exported ctor / dtor thunks.
// Symbol: ?SetColor@CMenuImages@@SAXW4IMAGE_STATE@1@K@Z
extern "C" void MS_ABI impl__SetColor_CMenuImages__SAXW4IMAGE_STATE_1_K_Z(int state, unsigned long color) {
    impl__Initialize_CMenuImages__KAHXZ();

    alignas(16) unsigned char storage[sizeof(CMFCToolBarImages)];
    CMFCToolBarImages* pLocal = static_cast<CMFCToolBarImages*>(impl___0CMFCToolBarImages__QEAA_XZ(storage));
    At<CSize>(pLocal, kTiOffSizeImage) = CSize(9, 9);

    EnsureGlobalDataInitialized();
    impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(pLocal, MenuImagesResource(), nullptr, FALSE);
    At<COLORREF>(pLocal, kTiOffClrTransparent) = kClrMagenta;
    if (impl__m_bIsRTL_CMFCToolBarImages__1HA) {
        impl__MirrorBitmap_CMFCToolBarImages__SAHAEAPEAUHBITMAP____H_Z(&At<HBITMAP>(pLocal, kTiOffImageWell),
                                                                       At<CSize>(pLocal, kTiOffSizeImage).cx);
    }
    CMFCToolBarImages* pImages = ImagesForState(state);
    if (color != static_cast<unsigned long>(-1)) {
        impl__MapBmpTo3dColors_CMFCToolBarImages__KAHAEAPEAUHBITMAP____HKK_Z(&At<HBITMAP>(pLocal, kTiOffImageWell), TRUE, 0, color);
    }
    if (!g_bMenuImagesInitializing) {
        EnsureGlobalDataInitialized();
        impl__SmoothResize_CMFCToolBarImages__QEAAHN_Z(pLocal, GlobalDataRibbonImageScale());
    }
    impl__Clear_CMFCToolBarImages__QEAAXXZ(pImages);
    impl__CopyTo_CMFCToolBarImages__QEAAHAEAV1__Z(pLocal, pImages);
    impl___1CMFCToolBarImages__UEAA_XZ(pLocal);
}

// Retail (RVA 0x8f600, mfc140u).  The CSize result is returned through the
// hidden pointer in RCX.  Fully transcribed:
//     if (m_bInitializing) {                                                // 0x3be258 -- re-entrant call only
//         int n = 9;
//         <afxGlobalData init gate>
//         double dblScale = afxGlobalData.GetRibbonImageScale();
//         if (dblScale != 1.0) n = (int)(9.0 * dblScale + 0.5);             // constants 0x350020 / 0x34fef0
//         return CSize(n, n);
//     }
//     Initialize();                                                         // 0x8f3c0, result ignored
//     return m_ImagesBlack.m_sizeImage;                                     // +0x68, 8-byte load
// The scaled 9 is ONLY the re-entrancy path; the normal path returns
// m_ImagesBlack.m_sizeImage, which Initialize set to (9, 9) and which retail's
// SmoothResize would rescale (OpenMFC's is a stub, so (9, 9) is what comes
// back here).
// Symbol: ?Size@CMenuImages@@SA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(CSize* pRet) {
    if (pRet == nullptr) return nullptr;
    if (g_bMenuImagesInitializing) {
        int n = 9;
        EnsureGlobalDataInitialized();
        const double dblScale = GlobalDataRibbonImageScale();
        if (dblScale != 1.0) {
            n = static_cast<int>(9.0 * dblScale + 0.5);
        }
        *pRet = CSize(n, n);
        return pRet;
    }
    impl__Initialize_CMenuImages__KAHXZ();
    *pRet = At<CSize>(TiBlack(), kTiOffSizeImage);
    return pRet;
}
