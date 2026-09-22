// CReBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Method: every body below was decoded from the mfc140u.dll export (disas.py
// --u), the way core/ole/COleControl.cpp and core/controlbar/CStatusBar.cpp
// do.  Of the bodies in this file only the constructor, both AddBar overloads,
// CalcFixedLayout and Create resolve in the wf2 RVA symbol map; every other
// export was resolved through the mfc140u export table by ordinal (urva.py /
// uwho.py) and cross-checked against CReBar's retail message map (AFX_MSGMAP
// at 0x323110, 32-byte entries from 0x323120, mfc140u) and vftable (0x322da8,
// mfc140u).  All RVAs in this file are mfc140u RVAs.
//    WM_NCCREATE    -> 0xd0d60  OnNcCreate    (shared with CMFCReBar by ICF)
//    WM_PAINT       -> 0xda30   OnPaint       (`jmp CWnd::Default`, ICF-shared)
//    WM_NCCALCSIZE  -> 0x1ddf30 OnNcCalcSize
//    WM_ERASEBKGND  -> 0xda30   OnEraseBkgnd  (same `jmp CWnd::Default`)
//    WM_NCPAINT     -> 0x1d9430 OnNcPaint     (`jmp EraseNonClient`, shared with CStatusBar)
//    WM_REFLECT_BASE+WM_NOTIFY (0xbc4e), codes RBN_HEIGHTCHANGE (0xfcc1) and
//        RBN_ENDDRAG (0xfcbc) -> 0x1dd7c0 OnHeightChange
//    RB_SHOWBAND (0x423) -> 0x1dd830 OnShowBand
//    WM_RECALCPARENT (0x368) -> 0x1dd780 OnRecalcParent
//  vftable slots: 31 OnToolHitTest (0x1ddfc0), 71 WindowProc (0x1de030),
//  91 CalcFixedLayout (0x1ddc00), 92 CalcDynamicLayout (0x1d61d0, which the
//  export table also gives to CControlBar::CalcDynamicLayout: the two bodies
//  are ICF-folded), 93 OnUpdateCmdUI (0xd0d50 = `jmp UpdateDialogControls`),
//  108 Create (0x1dde60).  Slot 94 (CalcInsideRect) and 102 (IsVisible) are
//  CControlBar's; slot 73 (DefWindowProc) is CWnd's (0x28bb80).
//
// Layout: OpenMFC declares no CReBar class, so `this` is typed CControlBar*
// (retail afxext.h: `class CReBar : public CControlBar`) and the one member
// CReBar adds, `UINT m_nReBarBandInfoSize`, is read at +0x148 == sizeof
// (CControlBar), the offset every retail body uses.  sizeof(CReBar) == 0x150
// (336, pinned in core/controlbar/RuntimeClasses.cpp).
//
// Deviations shared by the bodies (each site repeats the ones it makes):
//  * DefWindowProc (CWnd vslot 73).  Retail's CWnd::DefWindowProc
//    (0x28bb80) calls ::CallWindowProc through m_pfnSuper (+0xb0) when it is
//    set, else through *GetSuperWndProcAddr() (vslot 65), else
//    ::DefWindowProc.  For a rebar created by retail's CWnd::Create, MFC's
//    creation hook has subclassed the window and parked comctl32's
//    ReBarWindow32 procedure there (standard MFC behaviour, not disassembled
//    for this file), so RB_* reaches comctl32.  OpenMFC's
//    CWnd::Create (core/window/CWnd.cpp) does not subclass, and its
//    CWnd::DefWindowProc thunk calls ::DefWindowProcW, which would drop every
//    RB_* message.  RbDefWindowProc below therefore does what
//    core/controlbar/CStatusBar.cpp's SbDefWindowProc does: CallWindowProc
//    through m_pfnSuper when a window WAS subclassed, otherwise ::SendMessage
//    straight to the HWND (whose own procedure is still comctl32's).
//  * CWnd::Default is called through its thunk exactly where retail calls it
//    (OnNcCreate, OnPaint, OnEraseBkgnd, OnShowBand).  NOTE: OpenMFC's
//    CWnd::Default (core/window/CWnd.cpp) re-sends the current message to
//    ::DefWindowProcW (the thunk at core/window/CWnd.cpp), not to the rebar
//    procedure, so on a non-subclassed rebar those four do not reach comctl32.
//    That gap is CWnd's and is not patched here.
//  * Virtual calls on OTHER objects cannot go through their vtables (OpenMFC
//    objects carry mingw vtables); each site says which exported body it calls
//    instead.  Virtual calls on `this` go straight to the body retail's own
//    CReBar vftable holds in that slot, so only a client subclass override is
//    lost.
//  * Every REBARBANDINFOW local is zero-initialised; retail sets only the
//    members named at each site and leaves the rest indeterminate.
//  * Retail's ENSURE()s call AfxThrowInvalidArgException (0x227720); the
//    exported thunk is called and the body then returns, as
//    featurepack/controls/CMFCReBar.cpp does.
//  * OpenMFC's headers do not define CBRS_ALL / CBRS_ORIENT_HORZ, and the one
//    CBRS_* macro they do define here (afxole.h CBRS_HIDE_INPLACE = 0x80) is
//    not MFC's value (afxres.h: 0x0008), so the retail literals are spelled
//    out as k* constants below.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <commctrl.h>
#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Sibling thunks (declared with the signature their mangled name describes;
// every definition was located with grep before use).
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CControlBar__IEAA_XZ(void* pThis);                                     // core/controlbar/Thunks.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                  // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                     // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);            // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);     // core/window/Thunks.cpp
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);         // core/frame/Thunks.cpp
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
    const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);                             // core/window/CWnd.cpp
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);                                          // core/window/Thunks.cpp
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI);                                                   // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                                 // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ();           // core/controlbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ();                // core/controlbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ();            // core/controlbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ();              // core/controlbar/CToolBar.cpp
extern "C" int MS_ABI impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* pThis);                 // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis);                 // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(
    const CControlBar* pThis, CRect& rect, int bHorz);                                                  // core/controlbar/CControlBar.cpp
extern "C" __int64 MS_ABI impl__WindowProc_CControlBar__UEAA_JI_K_J_Z(
    CControlBar* pThis, unsigned int msg, UINT64 wParam, __int64 lParam);                               // core/controlbar/CControlBar.cpp
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CStatusBar__UEAA_AVCSize__HH_Z(
    CStatusBar* pThis, CSize* pRet, int bStretch, int bHorz);                                           // core/controlbar/CStatusBar.cpp
extern "C" __int64 MS_ABI impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CToolBar* pThis, CPoint point, TOOLINFOW* pTI);                                               // core/controlbar/CToolBar.cpp
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);                         // featurepack/CMFC_misc_stubs.cpp
extern "C" long MS_ABI impl__GetCommCtrlVersion__YAJPEAK0_Z(unsigned long* pdwMajor, unsigned long* pdwMinor); // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp

// Thunks defined further down in this file that earlier bodies call.
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CReBar__UEAA_AVCSize__HH_Z(
    CControlBar* pThis, CSize* pRet, int bStretch, int bHorz);
extern "C" __int64 MS_ABI impl__OnToolHitTest_CReBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CControlBar* pThis, CPoint point, TOOLINFOW* pTI);

namespace {

// ---- CReBar layout (see the header comment) ----
constexpr std::size_t kOffReBarBandInfoSize = 0x148;   // UINT m_nReBarBandInfoSize
constexpr std::size_t kSizeofCReBar         = 0x150;
static_assert(sizeof(CControlBar) == kOffReBarBandInfoSize, "CReBar::m_nReBarBandInfoSize follows the 0x148-byte CControlBar base");
static_assert(kOffReBarBandInfoSize + sizeof(UINT) <= kSizeofCReBar, "sizeof(CReBar) == 0x150");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CWnd, m_pfnSuper) == 0x48, "CWnd::m_pfnSuper (OpenMFC's slot; retail's is +0xb0, see CStatusBar.cpp)");
static_assert(offsetof(CControlBar, m_cxLeftBorder) == 0xf4 && offsetof(CControlBar, m_cxRightBorder) == 0xf8 &&
              offsetof(CControlBar, m_cyTopBorder) == 0xfc && offsetof(CControlBar, m_cyBottomBorder) == 0x100,
              "CControlBar border ints the constructor zeroes (two 8-byte stores at +0xf4 / +0xfc)");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle");
static_assert(offsetof(CGdiObject, m_hObject) == 0x08, "CGdiObject::m_hObject (AddBar reads pbmp+8)");

inline UINT& BandInfoSizeOf(CControlBar* pThis) {
    return *reinterpret_cast<UINT*>(reinterpret_cast<unsigned char*>(pThis) + kOffReBarBandInfoSize);
}
inline UINT BandInfoSizeOf(const CControlBar* pThis) {
    UINT n = 0;
    std::memcpy(&n, reinterpret_cast<const unsigned char*>(pThis) + kOffReBarBandInfoSize, sizeof(n));
    return n;
}

// CWnd::m_hWndOwner lives at CWnd + 0xa0 (retail OnNcCreate reads it there;
// WindowProc's inlined GetOwner() does too).  OpenMFC's CWnd does not name it
// -- the slot is inside the zero-initialised _cwnd_padding2 -- but MSVC
// clients inline CWnd::SetOwner and write it, so it is read by offset exactly
// as core/controlbar/CControlBar.cpp (OwnerHwnd) does.
constexpr std::size_t kOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's padding");
inline HWND RawOwnerHwnd(const CWnd* pWnd) {
    HWND h = nullptr;
    std::memcpy(&h, reinterpret_cast<const unsigned char*>(pWnd) + kOffHWndOwner, sizeof(h));
    return h;
}

// CFrameWnd::m_bInRecalcLayout: retail OnHeightChange tests `cmpl $0,0x1a0(frame)`
// (the same guard core/frame/CMDIFrameWndEx.cpp documents at CFrameWnd +0x1a0).
// OpenMFC's CFrameWnd does not name it: +0x1a0 lies in the protected
// _framewnd_padding that follows m_nIDHelp and runs to the end of the object,
// which the CFrameWnd constructor thunk zero-fills and nothing in the tree
// writes.  It is read by offset as retail does; in practice it reads FALSE.
constexpr std::size_t kOffFrameInRecalcLayout = 0x1a0;
static_assert(offsetof(CFrameWnd, m_nIDHelp) + sizeof(UINT) <= kOffFrameInRecalcLayout &&
              kOffFrameInRecalcLayout + sizeof(BOOL) <= sizeof(CFrameWnd),
              "m_bInRecalcLayout slot must lie inside CFrameWnd's trailing padding");
inline BOOL FrameInRecalcLayout(const CFrameWnd* pFrame) {
    BOOL b = FALSE;
    std::memcpy(&b, reinterpret_cast<const unsigned char*>(pFrame) + kOffFrameInRecalcLayout, sizeof(b));
    return b;
}

// ---- REBARBANDINFOW offsets / message ids / bits the retail bodies use ----
static_assert(sizeof(REBARBANDINFOW) == 0x80, "sizeof(REBARBANDINFOW) with the Vista members");
static_assert(offsetof(REBARBANDINFOW, fMask) == 0x04 && offsetof(REBARBANDINFOW, fStyle) == 0x08 &&
              offsetof(REBARBANDINFOW, clrFore) == 0x0c && offsetof(REBARBANDINFOW, clrBack) == 0x10 &&
              offsetof(REBARBANDINFOW, lpText) == 0x18 && offsetof(REBARBANDINFOW, hwndChild) == 0x28 &&
              offsetof(REBARBANDINFOW, cxMinChild) == 0x30 && offsetof(REBARBANDINFOW, cyMinChild) == 0x34 &&
              offsetof(REBARBANDINFOW, cx) == 0x38 && offsetof(REBARBANDINFOW, hbmBack) == 0x40 &&
              offsetof(REBARBANDINFOW, cxIdeal) == 0x58,
              "REBARBANDINFOW member offsets read / written by the retail bodies");
static_assert(RB_SETPARENT == 0x407 && RB_GETRECT == 0x409 && RB_INSERTBANDW == 0x40a &&
              RB_GETBANDCOUNT == 0x40c && RB_GETBANDINFOW == 0x41c && RB_MAXIMIZEBAND == 0x41f &&
              RB_SHOWBAND == 0x423, "RB_* message ids as the retail bodies encode them");
static_assert((RBN_HEIGHTCHANGE & 0xffff) == 0xfcc1 && (RBN_ENDDRAG & 0xffff) == 0xfcbc,
              "the two notification codes the retail message map routes to OnHeightChange");
static_assert(RBBIM_STYLE == 0x1 && RBBIM_COLORS == 0x2 && RBBIM_TEXT == 0x4 && RBBIM_CHILD == 0x10 &&
              RBBIM_CHILDSIZE == 0x20 && RBBIM_SIZE == 0x40 && RBBIM_BACKGROUND == 0x80 &&
              RBBIM_IDEALSIZE == 0x200 && RBBS_HIDDEN == 0x8 && RBBS_USECHEVRON == 0x200,
              "RBBIM_* / RBBS_* bits");
static_assert((CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE | RBS_VARHEIGHT) == 0x24e,
              "the style bits Create ORs in");

// Retail MFC values (afxres.h / afxpriv.h / afxext.h), as the disassembly
// encodes them.
constexpr DWORD kCBRS_ALL           = 0x0040ffff;
constexpr DWORD kCBRS_HIDE_INPLACE  = 0x00000008;
constexpr DWORD kCBRS_ORIENT_HORZ   = 0x0000a000;   // CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM
constexpr UINT  kAFX_IDW_REBAR      = 0xe804;
constexpr UINT  kWM_SETMESSAGESTRING = 0x0362;
constexpr UINT  kWM_POPMESSAGESTRING = 0x0375;
constexpr UINT  kWM_RECALCPARENT     = 0x0368;
constexpr DWORD kLM_STRETCH         = 0x01;
constexpr DWORD kLM_HORZ            = 0x02;
// The bit retail passes to AfxEndDeferRegisterClass in Create.  Its branch in
// retail AfxEndDeferRegisterClass (0x2918f0, mfc140u; the test at 0x291ba2)
// hands dwICC = 0x400 (ICC_COOL_CLASSES) to the common-controls initialiser
// at 0x29334c.  (afximpl.h, which names it, is not shipped.)
constexpr long  kAFX_WNDCOMMCTL_COOL_REG = 0x8000;
// _afxComCtlVersion threshold in _AddBar: VERSION_IE401 = MAKELONG(72, 4).
constexpr long  kVERSION_IE401      = 0x40048;
// The two m_nReBarBandInfoSize values the retail constructor computes.  0x70
// equals the pre-Vista part of REBARBANDINFOW (through cxHeader, ending at
// 0x6c) rounded up to the struct's 8-byte alignment; it is NOT commctrl.h's
// REBARBANDINFOW_V6_SIZE (CCSIZEOF_STRUCT(..., cxHeader) == 0x6c).  Which MFC
// source expression yields 0x70 was not determined -- only the constant is
// taken from the disassembly.
constexpr UINT  kBandInfoSizeV6     = 0x80;   // sizeof(REBARBANDINFOW)
constexpr UINT  kBandInfoSizePreV6  = 0x70;
static_assert(offsetof(REBARBANDINFOW, rcChevronLocation) == 0x6c, "Vista members start at 0x6c");
constexpr DWORD kComCtlVersion61    = 0x60001;

// The unexported retail helper at RVA 0x1db004 (mfc140u), transcribed (the
// names _AfxGetComCtlVersion / _afxComCtlVersion are MFC's, inferred from the
// behaviour; neither is in the image's export table):
//     if (_afxComCtlVersion == -1) {                            // global 0x3b1b94
//         DWORD major = 0, minor = 0;
//         GetCommCtrlVersion(&major, &minor);                   // 0x2ae720; HRESULT ignored
//         _afxComCtlVersion = MAKELONG((WORD)minor, (WORD)major);
//     }
//     return _afxComCtlVersion;
// DEVIATION: retail keeps one module-wide cache; this file keeps its own
// (featurepack/controls/CMFCReBar.cpp keeps another), all filled from the
// same exported GetCommCtrlVersion, so they agree.
DWORD ComCtlVersion() {
    static DWORD s_ver = 0xffffffffu;
    if (s_ver == 0xffffffffu) {
        unsigned long major = 0, minor = 0;
        impl__GetCommCtrlVersion__YAJPEAK0_Z(&major, &minor);
        s_ver = ((major & 0xffffu) << 16) | (minor & 0xffffu);
    }
    return s_ver;
}

// this->DefWindowProc(msg, wParam, lParam): CWnd vslot 73 (+0x248) in retail.
// See the header comment for why this is not the CWnd::DefWindowProc thunk.
inline LRESULT RbDefWindowProc(const CControlBar* pThis, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (pThis->m_hWnd == nullptr) return 0;
    if (pThis->m_pfnSuper != nullptr) {
        return ::CallWindowProcW(pThis->m_pfnSuper, pThis->m_hWnd, msg, wParam, lParam);
    }
    return ::SendMessageW(pThis->m_hWnd, msg, wParam, lParam);
}

inline int KindOf(const CWnd* pWnd, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pWnd), pClass);
}

// DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(hwndChild)) followed
// by the band-visibility test CalcFixedLayout's first loop and OnShowBand both
// inline:
//     CControlBar* pBar = ...FromHandlePermanent(hwndChild)...;       // 0x28adc0
//     if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CControlBar))) // 0x234cf0, RTC 0x33aec0
//         bWindowVisible = pBar->IsVisible();                         // vslot 102 (+0x330)
//     else
//         bWindowVisible = (::GetWindowLong(hwndChild, GWL_STYLE) >> 28) & 1;   // WS_VISIBLE
// DEVIATION: vslot 102 is devirtualised to the exported CControlBar body (no
// retail control bar overrides it; a client override would not be seen).
BOOL BandChildIsVisible(HWND hwndChild) {
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndChild);
    if (pWnd != nullptr && KindOf(pWnd, impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ())) {
        return impl__IsVisible_CControlBar__UEBAHXZ(static_cast<const CControlBar*>(pWnd));
    }
    return (static_cast<DWORD>(::GetWindowLongW(hwndChild, GWL_STYLE)) >> 28) & 1;
}

// Transcribed from the unexported retail helper at RVA 0x2ae3f4 (mfc140u;
// MFC's _AfxChildWindowFromPoint by behaviour, the name is not in the image):
//     ::ClientToScreen(hWnd, &pt);                                     // IAT ClientToScreen
//     for (HWND h = ::GetWindow(hWnd, GW_CHILD); h; h = ::GetWindow(h, GW_HWNDNEXT)) {
//         if ((UINT)::GetDlgCtrlID(h) != 0xffff &&                     // IAT GetDlgCtrlID; full 32-bit `cmp $0xffff,%eax`
//             (::GetWindowLong(h, GWL_STYLE) & WS_VISIBLE)) {          // bt $0x1c
//             CRect rect;  ::GetWindowRect(h, &rect);
//             if (::PtInRect(&rect, pt)) return h;
//         }
//     }
//     return NULL;
HWND AfxChildWindowFromPointImpl(HWND hWnd, POINT pt) {
    ::ClientToScreen(hWnd, &pt);
    for (HWND h = ::GetWindow(hWnd, GW_CHILD); h != nullptr; h = ::GetWindow(h, GW_HWNDNEXT)) {
        if (static_cast<UINT>(::GetDlgCtrlID(h)) != 0xffffu &&
            (static_cast<DWORD>(::GetWindowLongW(h, GWL_STYLE)) & WS_VISIBLE)) {
            RECT rect = {};
            ::GetWindowRect(h, &rect);
            if (::PtInRect(&rect, pt)) return h;
        }
    }
    return nullptr;
}

// Transcribed from retail CReBar::_AddBar (unexported, RVA 0x1dd940, mfc140u;
// the callee of both AddBar overloads):
//     ENSURE_ARG(pBar != NULL);                                       // 0x227720
//     pRBBI->cbSize = m_nReBarBandInfoSize;                           // +0x148
//     pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;     // 0x70
//     pRBBI->hwndChild = pBar->m_hWnd;
//     CSize size;
//     if (pBar->IsKindOf(RUNTIME_CLASS(CControlBar)))                 // 0x234cf0, RTC 0x33aec0
//         size = ((CControlBar*)pBar)->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);   // vslot 91
//     else { CRect rect;  ::GetWindowRect(pBar->m_hWnd, &rect);  size = rect.Size(); }
//     pRBBI->cyMinChild = size.cy;
//     int cx = size.cx + (_afxComCtlVersion < VERSION_IE401 ? 4 : 0);   // signed `cmpl; cmovl`
//     pRBBI->cxIdeal = cx;
//     if (pRBBI->fStyle & RBBS_USECHEVRON) {
//         pRBBI->fMask |= RBBIM_IDEALSIZE;
//         pRBBI->cxMinChild = size.cy;                                // ecx still holds cy here
//     } else
//         pRBBI->cxMinChild = cx;
//     pRBBI->cx = cx;
//     if (!DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI))   // vslot 73
//         return FALSE;
//     if (CFrameWnd* pFrameWnd = GetParentFrame())                    // 0x28e200
//         pFrameWnd->RecalcLayout(TRUE);                              // CFrameWnd vslot 96 (+0x300)
//     REBARBANDINFO rbBand;
//     rbBand.cbSize = m_nReBarBandInfoSize;  rbBand.fMask = RBBIM_STYLE;
//     DefWindowProc(RB_GETBANDINFO, 0, (LPARAM)&rbBand);              // band 0, result ignored
//     if (!(rbBand.fStyle & RBBS_HIDDEN))
//         ::SendMessage(m_hWnd, RB_MAXIMIZEBAND, 0, 0);               // IAT SendMessageW
//     return TRUE;
// DEVIATIONS: vslot 91 is devirtualised on the runtime class to the two
// CalcFixedLayout thunks whose parameter order matches the MSVC ABI (this,
// hidden CSize*, ...): CReBar's (this file) and CStatusBar's.  The CControlBar,
// CToolBar, CDialogBar and CDockBar thunks take the hidden return pointer
// FIRST, which is not what their mangled names describe (see headerRequests),
// so a bar of those classes -- or of a client class -- is sized from its
// window rectangle, i.e. retail's non-control-bar branch.  RecalcLayout goes
// through the CFrameWnd thunk, which dispatches to the frame's C++ override.
// Retail reads the cached global _afxComCtlVersion (0x3b1b94, mfc140u; its
// static initialiser is -1) directly rather than calling the 0x1db004 getter;
// ComCtlVersion() below also fills the cache when it is still unset.  The two
// agree whenever the CReBar constructor has run, because it calls the getter.
BOOL AddBarImpl(CControlBar* pThis, CWnd* pBar, REBARBANDINFOW* pRBBI) {
    if (pBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    pRBBI->cbSize = BandInfoSizeOf(pThis);
    pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
    pRBBI->hwndChild = pBar->m_hWnd;

    int cxSize = 0;
    int cySize = 0;
    bool bSized = false;
    if (KindOf(pBar, impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ())) {
        const int bHorz = static_cast<int>(pThis->m_dwStyle & kCBRS_ORIENT_HORZ);
        CSize size(0, 0);
        if (KindOf(pBar, impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ())) {
            impl__CalcFixedLayout_CReBar__UEAA_AVCSize__HH_Z(static_cast<CControlBar*>(pBar), &size, FALSE, bHorz);   // vslot 91
            bSized = true;
        } else if (KindOf(pBar, impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ())) {
            impl__CalcFixedLayout_CStatusBar__UEAA_AVCSize__HH_Z(static_cast<CStatusBar*>(pBar), &size, FALSE, bHorz);   // vslot 91
            bSized = true;
        }
        cxSize = size.cx;
        cySize = size.cy;
    }
    if (!bSized) {
        RECT rect = {};
        ::GetWindowRect(pBar->m_hWnd, &rect);
        cxSize = rect.right - rect.left;
        cySize = rect.bottom - rect.top;
    }

    pRBBI->cyMinChild = cySize;
    const int cx = cxSize + (static_cast<long>(ComCtlVersion()) < kVERSION_IE401 ? 4 : 0);
    pRBBI->cxIdeal = cx;
    if (pRBBI->fStyle & RBBS_USECHEVRON) {
        pRBBI->fMask |= RBBIM_IDEALSIZE;
        pRBBI->cxMinChild = cySize;
    } else {
        pRBBI->cxMinChild = cx;
    }
    pRBBI->cx = cx;

    if (RbDefWindowProc(pThis, RB_INSERTBANDW, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(pRBBI)) == 0) {   // vslot 73
        return FALSE;
    }
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrameWnd != nullptr) {
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrameWnd, TRUE);   // CFrameWnd vslot 96
    }
    REBARBANDINFOW rbBand;
    std::memset(&rbBand, 0, sizeof(rbBand));
    rbBand.cbSize = BandInfoSizeOf(pThis);
    rbBand.fMask = RBBIM_STYLE;
    RbDefWindowProc(pThis, RB_GETBANDINFOW, 0, reinterpret_cast<LPARAM>(&rbBand));   // vslot 73
    if (!(rbBand.fStyle & RBBS_HIDDEN)) {
        ::SendMessage(pThis->m_hWnd, RB_MAXIMIZEBAND, 0, 0);
    }
    return TRUE;
}

} // namespace

// Transcribed from retail RVA 0x1dd6e0 (mfc140u):
//     CControlBar::CControlBar();                                     // 0x1d5ef0
//     vfptr = &CReBar::`vftable';                                     // 0x322da8 (NOT reproduced)
//     m_nReBarBandInfoSize = (_AfxGetComCtlVersion() < 0x60001)       // 0x1db004; unsigned `cmp; sbb; and; sub`
//                          ? 0x70 : 0x80 /* sizeof(REBARBANDINFO) */;   // see kBandInfoSizePreV6
//     m_cxLeftBorder = m_cxRightBorder = m_cyTopBorder = m_cyBottomBorder = 0;   // 8-byte stores at +0xf4, +0xfc
// This body used to be `return pThis;`, which left the CControlBar base
// unconstructed and m_nReBarBandInfoSize indeterminate (every RB_* call below
// sends it as cbSize).  DEVIATION: the retail vftable store is not reproduced.
// OpenMFC has no C++ CReBar class, so the object is left with the mingw
// CControlBar vtable that `new (p) CControlBar()` in the CControlBar
// constructor thunk (core/controlbar/Thunks.cpp) installs.  The sibling
// constructor thunks there likewise install mingw vtables (each its own
// class's) rather than the retail vftables.
// Symbol: ??0CReBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CReBar__QEAA_XZ(void* pThis) {
    impl___0CControlBar__IEAA_XZ(pThis);
    CControlBar* pBar = static_cast<CControlBar*>(pThis);
    BandInfoSizeOf(pBar) = (ComCtlVersion() < kComCtlVersion61) ? kBandInfoSizePreV6 : kBandInfoSizeV6;
    pBar->m_cxLeftBorder = 0;
    pBar->m_cxRightBorder = 0;
    pBar->m_cyTopBorder = 0;
    pBar->m_cyBottomBorder = 0;
    return pThis;
}

// Transcribed from retail RVA 0x1ddb80 (mfc140u):
//     REBARBANDINFO rbBand;
//     rbBand.fMask = RBBIM_STYLE | RBBIM_COLORS;                      // 3
//     rbBand.fStyle = dwStyle;  rbBand.clrFore = clrFore;  rbBand.clrBack = clrBack;
//     if (pszText != NULL) { rbBand.fMask |= RBBIM_TEXT;  rbBand.lpText = (LPTSTR)pszText; }   // 7
//     return _AddBar(pBar, &rbBand);                                  // 0x1dd940
// Retail does not test pBar here; _AddBar does.
// Symbol: ?AddBar@CReBar@@QEAAHPEAVCWnd@@KKPEB_WK@Z
extern "C" int MS_ABI impl__AddBar_CReBar__QEAAHPEAVCWnd__KKPEB_WK_Z(
    CControlBar* pThis, CWnd* pBar, unsigned long clrFore, unsigned long clrBack,
    const wchar_t* pszText, unsigned long dwStyle) {
    REBARBANDINFOW rbBand;
    std::memset(&rbBand, 0, sizeof(rbBand));
    rbBand.fMask = RBBIM_STYLE | RBBIM_COLORS;
    rbBand.fStyle = dwStyle;
    rbBand.clrFore = clrFore;
    rbBand.clrBack = clrBack;
    if (pszText != nullptr) {
        rbBand.fMask = RBBIM_STYLE | RBBIM_COLORS | RBBIM_TEXT;
        rbBand.lpText = const_cast<wchar_t*>(pszText);
    }
    return AddBarImpl(pThis, pBar, &rbBand);
}

// Transcribed from retail RVA 0x1ddaf0 (mfc140u):
//     ENSURE_ARG(pBar != NULL);                                       // 0x227720
//     REBARBANDINFO rbBand;
//     rbBand.fMask = RBBIM_STYLE;  rbBand.fStyle = dwStyle;
//     if (pszText != NULL) { rbBand.fMask |= RBBIM_TEXT;  rbBand.lpText = (LPTSTR)pszText; }
//     if (pbmp != NULL)    { rbBand.fMask |= RBBIM_BACKGROUND;  rbBand.hbmBack = (HBITMAP)pbmp->m_hObject; }   // bts $7; +8
//     return _AddBar(pBar, &rbBand);                                  // 0x1dd940
// Symbol: ?AddBar@CReBar@@QEAAHPEAVCWnd@@PEB_WPEAVCBitmap@@K@Z
extern "C" int MS_ABI impl__AddBar_CReBar__QEAAHPEAVCWnd__PEB_WPEAVCBitmap__K_Z(
    CControlBar* pThis, CWnd* pBar, const wchar_t* pszText, CBitmap* pbmp, unsigned long dwStyle) {
    if (pBar == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    REBARBANDINFOW rbBand;
    std::memset(&rbBand, 0, sizeof(rbBand));
    rbBand.fMask = RBBIM_STYLE;
    rbBand.fStyle = dwStyle;
    if (pszText != nullptr) {
        rbBand.fMask |= RBBIM_TEXT;
        rbBand.lpText = const_cast<wchar_t*>(pszText);
    }
    if (pbmp != nullptr) {
        rbBand.fMask |= RBBIM_BACKGROUND;
        rbBand.hbmBack = static_cast<HBITMAP>(pbmp->m_hObject);
    }
    return AddBarImpl(pThis, pBar, &rbBand);
}

// Retail: the export table resolves this symbol to RVA 0x1d61d0 (mfc140u), the
// same ICF-folded body as ?CalcDynamicLayout@CControlBar@@, and the CReBar
// vftable holds 0x1d61d0 in slot 92.  Transcribed:
//     return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);  // vslot 91 (+0x2d8)
// For a CReBar `this`, slot 91 is CReBar::CalcFixedLayout (0x1ddc00), which is
// called directly (a client override of CalcFixedLayout would not be seen).
// Symbol: ?CalcDynamicLayout@CReBar@@UEAA?AVCSize@@HK@Z
extern "C" CSize* MS_ABI impl__CalcDynamicLayout_CReBar__UEAA_AVCSize__HK_Z(
    CControlBar* pThis, CSize* pRet, int nLength, unsigned long dwMode) {
    (void)nLength;
    return impl__CalcFixedLayout_CReBar__UEAA_AVCSize__HH_Z(
        pThis, pRet, static_cast<int>(dwMode & kLM_STRETCH), static_cast<int>(dwMode & kLM_HORZ));   // vslot 91
}

// Transcribed from retail RVA 0x1ddc00 (mfc140u):
//     int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);         // vslot 73
//     REBARBANDINFO rbBand;  rbBand.cbSize = m_nReBarBandInfoSize;
//     for (int i = nCount; i--; ) {                                   // sync hidden state
//         rbBand.fMask = RBBIM_CHILD | RBBIM_STYLE;                   // 0x11
//         DefWindowProc(RB_GETBANDINFO, i, (LPARAM)&rbBand);
//         BOOL bWindowVisible = <BandChildIsVisible(rbBand.hwndChild)>;
//         BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
//         if (bWindowVisible != bBandVisible)
//             DefWindowProc(RB_SHOWBAND, i, bWindowVisible);
//     }
//     CRect rectBound;  rectBound.SetRectEmpty();                     // IAT SetRectEmpty
//     for (int i = nCount; i--; ) {                                   // union of visible bands
//         rbBand.fMask = RBBIM_STYLE;
//         DefWindowProc(RB_GETBANDINFO, i, (LPARAM)&rbBand);
//         if (!(rbBand.fStyle & RBBS_HIDDEN)) {
//             CRect rect;  (zeroed)
//             DefWindowProc(RB_GETRECT, i, (LPARAM)&rect);
//             ::UnionRect(&rectBound, &rectBound, &rect);
//         }
//     }
//     if (!::IsRectEmpty(&rectBound)) {
//         CRect rect;  rect.SetRectEmpty();
//         CalcInsideRect(rect, bHorz);                                // vslot 94 (+0x2f0)
//         rectBound.right  -= rect.Width();
//         rectBound.bottom -= rect.Height();
//     }
//     return CSize((bHorz && bStretch) ? 32767 : rectBound.Width(),
//                  (!bHorz && bStretch) ? 32767 : rectBound.Height());
// Slot 94 of the CReBar vftable is CControlBar::CalcInsideRect (0x1d7c50),
// called here through its thunk; slot 102 inside BandChildIsVisible is
// devirtualised as that helper describes.
// Symbol: ?CalcFixedLayout@CReBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CReBar__UEAA_AVCSize__HH_Z(
    CControlBar* pThis, CSize* pRet, int bStretch, int bHorz) {
    const int nCount = static_cast<int>(RbDefWindowProc(pThis, RB_GETBANDCOUNT, 0, 0));   // vslot 73
    REBARBANDINFOW rbBand;
    std::memset(&rbBand, 0, sizeof(rbBand));
    rbBand.cbSize = BandInfoSizeOf(pThis);

    for (int i = nCount; i-- != 0; ) {
        rbBand.fMask = RBBIM_CHILD | RBBIM_STYLE;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, static_cast<WPARAM>(static_cast<INT_PTR>(i)), reinterpret_cast<LPARAM>(&rbBand));
        const BOOL bWindowVisible = BandChildIsVisible(rbBand.hwndChild);
        const BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
        if (bWindowVisible != bBandVisible) {
            RbDefWindowProc(pThis, RB_SHOWBAND, static_cast<WPARAM>(static_cast<INT_PTR>(i)), static_cast<LPARAM>(bWindowVisible));
        }
    }

    RECT rectBound;
    ::SetRectEmpty(&rectBound);
    for (int i = nCount; i-- != 0; ) {
        rbBand.fMask = RBBIM_STYLE;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, static_cast<WPARAM>(static_cast<INT_PTR>(i)), reinterpret_cast<LPARAM>(&rbBand));
        if (!(rbBand.fStyle & RBBS_HIDDEN)) {
            RECT rect = {};
            RbDefWindowProc(pThis, RB_GETRECT, static_cast<WPARAM>(static_cast<INT_PTR>(i)), reinterpret_cast<LPARAM>(&rect));
            ::UnionRect(&rectBound, &rectBound, &rect);
        }
    }

    if (!::IsRectEmpty(&rectBound)) {
        CRect rect;
        rect.SetRectEmpty();
        impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pThis, rect, bHorz);   // vslot 94
        rectBound.right -= rect.Width();
        rectBound.bottom -= rect.Height();
    }

    const int cx = (bHorz && bStretch) ? 32767 : rectBound.right - rectBound.left;
    const int cy = (!bHorz && bStretch) ? 32767 : rectBound.bottom - rectBound.top;
    new (pRet) CSize(cx, cy);
    return pRet;
}

// Transcribed from retail RVA 0x1dde60 (mfc140u):
//     m_dwStyle = dwStyle & CBRS_ALL;                                 // +0x124, 0x40ffff
//     if (nID == AFX_IDW_REBAR) m_dwStyle |= CBRS_HIDE_INPLACE;       // 0xe804 / 0x8
//     AfxEndDeferRegisterClass(AFX_WNDCOMMCTL_COOL_REG);              // 0x2918f0, bit 0x8000, result ignored
//     _AfxGetComCtlVersion();                                         // 0x1db004, result ignored
//     CRect rect;  rect.SetRectEmpty();                               // IAT SetRectEmpty
//     dwStyle = (dwStyle & ~CBRS_ALL) | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER
//             | CCS_NORESIZE | RBS_VARHEIGHT | dwCtrlStyle;           // `and $0xffbf024e; or; or $0x24e`
//     return CWnd::Create(REBARCLASSNAME /*L"ReBarWindow32", 0x3406e0*/, NULL,
//                         dwStyle, rect, pParentWnd, nID, NULL) != 0; // 0x28b690 (direct call)
// NOTE: OpenMFC's AfxEndDeferRegisterClass (featurepack/CMFC_misc_stubs.cpp)
// ignores its argument and registers only OpenMFC's own window class, so it
// does NOT perform the ICC_COOL_CLASSES initialisation retail's 0x8000 branch
// does (see headerRequests); Create relies on ReBarWindow32 already being
// registered in the process.
// Symbol: ?Create@CReBar@@UEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__Create_CReBar__UEAAHPEAVCWnd__KKI_Z(
    CControlBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, unsigned int nID) {
    pThis->m_dwStyle = dwStyle & kCBRS_ALL;
    if (nID == kAFX_IDW_REBAR) {
        pThis->m_dwStyle |= kCBRS_HIDE_INPLACE;
    }
    impl__AfxEndDeferRegisterClass__YAHJ_Z(kAFX_WNDCOMMCTL_COOL_REG);
    (void)ComCtlVersion();
    RECT rect;
    ::SetRectEmpty(&rect);
    const DWORD dwWinStyle = (dwStyle & ~kCBRS_ALL)
                           | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE | RBS_VARHEIGHT
                           | dwCtrlStyle;
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
               pThis, REBARCLASSNAMEW, nullptr, dwWinStyle, rect, pParentWnd, nID, nullptr) != 0;
}

// Retail: WM_ERASEBKGND maps to RVA 0xda30 (mfc140u), `jmp CWnd::Default`
// (0x28ac80) -- an ICF-folded body the export table also gives to OnPaint and
// some twenty other handler exports.  So: return (BOOL)Default();  (see the header note
// on OpenMFC's CWnd::Default).
// Symbol: ?OnEraseBkgnd@CReBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CReBar__IEAAHPEAVCDC___Z(CControlBar* pThis, CDC* pDC) {
    (void)pDC;
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// Transcribed from retail RVA 0x1dd7c0 (mfc140u):
//     CFrameWnd* pFrameWnd = GetParentFrame();                        // 0x28e200
//     if (pFrameWnd != NULL) {
//         if (!pFrameWnd->m_bInRecalcLayout)                          // frame +0x1a0
//             pFrameWnd->RecalcLayout(TRUE);                          // CFrameWnd vslot 96 (+0x300)
//         else
//             ::PostMessage(m_hWnd, WM_RECALCPARENT, 0, 0);           // IAT PostMessageW, 0x368
//     }
//     *pResult = 0;                                                   // unconditional
// RecalcLayout goes through the CFrameWnd thunk (C++ virtual dispatch to the
// frame's override); m_bInRecalcLayout is read by offset (FrameInRecalcLayout).
// Symbol: ?OnHeightChange@CReBar@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnHeightChange_CReBar__IEAAXPEAUtagNMHDR__PEA_J_Z(
    CControlBar* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pNMHDR;
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrameWnd != nullptr) {
        if (!FrameInRecalcLayout(pFrameWnd)) {
            impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrameWnd, TRUE);   // CFrameWnd vslot 96
        } else {
            ::PostMessage(pThis->m_hWnd, kWM_RECALCPARENT, 0, 0);
        }
    }
    *pResult = 0;
}

// Transcribed from retail RVA 0x1ddf30 (mfc140u):
//     CRect rect;  rect.SetRectEmpty();                               // IAT SetRectEmpty
//     CControlBar::CalcInsideRect(rect, (m_dwStyle & CBRS_ORIENT_HORZ) != 0);   // 0x1d7c50, direct call
//     lpncsp->rgrc[0].left   += rect.left;                            // 4-int loop over the RECT
//     lpncsp->rgrc[0].top    += rect.top;
//     lpncsp->rgrc[0].right  += rect.right;
//     lpncsp->rgrc[0].bottom += rect.bottom;
// bCalcValidRects is not read.
// Symbol: ?OnNcCalcSize@CReBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CReBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CControlBar* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    CRect rect;
    rect.SetRectEmpty();
    impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pThis, rect, (pThis->m_dwStyle & kCBRS_ORIENT_HORZ) != 0);
    lpncsp->rgrc[0].left += rect.left;
    lpncsp->rgrc[0].top += rect.top;
    lpncsp->rgrc[0].right += rect.right;
    lpncsp->rgrc[0].bottom += rect.bottom;
}

// Transcribed from retail RVA 0xd0d60 (mfc140u; the export table gives the
// same body to CMFCReBar::OnNcCreate):
//     if (!(BOOL)Default()) return FALSE;                             // 0x28ac80, `test %eax`
//     if (m_hWndOwner != NULL)                                        // CWnd +0xa0
//         DefWindowProc(RB_SETPARENT, (WPARAM)m_hWndOwner, 0);        // vslot 73, 0x407
//     return TRUE;
// lpCreateStruct is not read (Default re-sends the current message).
// Symbol: ?OnNcCreate@CReBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CReBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CControlBar* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == 0) {
        return FALSE;
    }
    const HWND hWndOwner = RawOwnerHwnd(pThis);
    if (hWndOwner != nullptr) {
        RbDefWindowProc(pThis, RB_SETPARENT, reinterpret_cast<WPARAM>(hWndOwner), 0);   // vslot 73
    }
    return TRUE;
}

// Retail: WM_NCPAINT maps to RVA 0x1d9430 (mfc140u), `jmp
// CControlBar::EraseNonClient` (0x1d6e50) -- the same body CStatusBar's map
// uses.  Transcribed as that tail call.
// Symbol: ?OnNcPaint@CReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CReBar__IEAAXXZ(CControlBar* pThis) {
    impl__EraseNonClient_CControlBar__QEAAXXZ(pThis);
}

// Retail: WM_PAINT maps to RVA 0xda30 (mfc140u), `jmp CWnd::Default`
// (0x28ac80); see OnEraseBkgnd and the header note on OpenMFC's CWnd::Default.
// Symbol: ?OnPaint@CReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CReBar__IEAAXXZ(CControlBar* pThis) {
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Transcribed from retail RVA 0x1dd780 (mfc140u):
//     CFrameWnd* pFrameWnd = GetParentFrame();                        // 0x28e200
//     if (pFrameWnd == NULL) AfxThrowInvalidArgException();           // 0x227720
//     pFrameWnd->RecalcLayout(TRUE);                                  // CFrameWnd vslot 96, tail call
// Symbol: ?OnRecalcParent@CReBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnRecalcParent_CReBar__IEAAXXZ(CControlBar* pThis) {
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrameWnd == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrameWnd, TRUE);   // CFrameWnd vslot 96
}

// Transcribed from retail RVA 0x1dd830 (mfc140u):
//     LRESULT lResult = Default();                                    // 0x28ac80
//     if (lResult) {
//         REBARBANDINFO rbBand;
//         rbBand.cbSize = m_nReBarBandInfoSize;                       // +0x148
//         rbBand.fMask = RBBIM_CHILD | RBBIM_STYLE;                   // 0x11
//         DefWindowProc(RB_GETBANDINFO, wParam, (LPARAM)&rbBand);     // vslot 73, result ignored
//         BOOL bWindowVisible = <BandChildIsVisible(rbBand.hwndChild)>;
//         BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
//         if (bWindowVisible != bBandVisible)
//             ::ShowWindow(rbBand.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE);   // IAT ShowWindow
//     }
//     return lResult;
// lParam is not read.
// Symbol: ?OnShowBand@CReBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnShowBand_CReBar__IEAA_J_K_J_Z(
    CControlBar* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    const __int64 lResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    if (lResult != 0) {
        REBARBANDINFOW rbBand;
        std::memset(&rbBand, 0, sizeof(rbBand));
        rbBand.cbSize = BandInfoSizeOf(pThis);
        rbBand.fMask = RBBIM_CHILD | RBBIM_STYLE;
        RbDefWindowProc(pThis, RB_GETBANDINFOW, static_cast<WPARAM>(wParam), reinterpret_cast<LPARAM>(&rbBand));   // vslot 73
        const BOOL bWindowVisible = BandChildIsVisible(rbBand.hwndChild);
        const BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
        if (bWindowVisible != bBandVisible) {
            ::ShowWindow(rbBand.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE);
        }
    }
    return lResult;
}

// Transcribed from retail RVA 0x1ddfc0 (mfc140u):
//     HWND hWndChild = _AfxChildWindowFromPoint(m_hWnd, point);       // 0x2ae3f4
//     CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);              // 0x28adc0
//     if (pWnd == NULL)
//         return CWnd::OnToolHitTest(point, pTI);                     // 0x28bcc0, direct call on this
//     return pWnd->OnToolHitTest(point, pTI);                         // CWnd vslot 31 (+0xf8) on the child
// DEVIATION: the child's vslot 31 is devirtualised on its runtime class, as
// core/controlbar/CControlBar.cpp (CallOnToolHitTest) does: a CToolBar child
// goes to the CToolBar thunk, a nested CReBar to this one, anything else to
// CWnd's.  An override in any other class (e.g. a client CControlBar) is not
// seen.
// Symbol: ?OnToolHitTest@CReBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CReBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CControlBar* pThis, CPoint point, TOOLINFOW* pTI) {
    POINT pt = { point.x, point.y };
    HWND hWndChild = AfxChildWindowFromPointImpl(pThis->m_hWnd, pt);
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hWndChild);
    if (pWnd == nullptr) {
        return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pThis, point, pTI);
    }
    if (KindOf(pWnd, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        return impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(static_cast<const CToolBar*>(pWnd), point, pTI);   // vslot 31
    }
    if (KindOf(pWnd, impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ())) {
        return impl__OnToolHitTest_CReBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(static_cast<const CControlBar*>(pWnd), point, pTI); // vslot 31
    }
    return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pWnd, point, pTI);   // vslot 31
}

// Retail: vftable slot 93 of CReBar (0x322da8, mfc140u) holds RVA 0xd0d50 --
// the export table's address for this symbol -- which is a single
// `jmp CWnd::UpdateDialogControls` (0x291460):
//     UpdateDialogControls(pTarget, bDisableIfNoHndler);
// Symbol: ?OnUpdateCmdUI@CReBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CReBar__UEAAXPEAVCFrameWnd__H_Z(
    CControlBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisableIfNoHndler);
}

// Transcribed from retail RVA 0x1de030 (mfc140u):
//     if (message == WM_SETMESSAGESTRING || message == WM_POPMESSAGESTRING) {   // 0x362 / 0x362+0x13
//         HWND hOwner = m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd);         // inlined GetOwner(), IAT GetParent
//         CWnd* pOwner = CWnd::FromHandle(hOwner);                    // 0x28ad70
//         if (pOwner == NULL) AfxThrowInvalidArgException();          // 0x227720
//         return ::SendMessage(pOwner->m_hWnd, message, wParam, lParam);   // IAT SendMessageW
//     }
//     return CControlBar::WindowProc(message, wParam, lParam);        // 0x1d6810, direct call
// NOTE: OpenMFC's CControlBar::WindowProc thunk (core/controlbar/CControlBar.cpp)
// currently just returns ::DefWindowProcW; the call is still made through it.
// Symbol: ?WindowProc@CReBar@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CReBar__MEAA_JI_K_J_Z(
    CControlBar* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    if (message == kWM_SETMESSAGESTRING || message == kWM_POPMESSAGESTRING) {
        HWND hOwner = RawOwnerHwnd(pThis);
        if (hOwner == nullptr) hOwner = ::GetParent(pThis->m_hWnd);
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner);
        if (pOwner == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return 0;
        }
        return ::SendMessage(pOwner->m_hWnd, message, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
    }
    return impl__WindowProc_CControlBar__UEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
