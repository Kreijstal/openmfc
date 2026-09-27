// CMiniDockFrameWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Method: every body below was decoded from the retail export, the way
// core/ole/COleControl.cpp and core/controlbar/CDockBar.cpp do.  The
// constructor, Create, CreateObject and OnClose are in the wf2 mfc140u RVA
// symbol map; OnMouseActivate, OnNcLButtonDblClk, OnNcLButtonDown and
// RecalcLayout were resolved through the mfc140u export table (ordinal ->
// RVA, ordrva.py).  Entries (mfc140u):
//   ??0 CMiniDockFrameWnd   0x1d98e0      Create              0x1d9990
//   CreateObject            0x1d9d90      OnClose             0x1d9c10
//   OnMouseActivate         0x1d98c0      OnNcLButtonDblClk   0x1d9d00
//   OnNcLButtonDown         0x1d9c20      RecalcLayout        0x1d9ba0
// The ANSI twin (mfc140.dll) has the same instruction sequences, member
// offsets and constants (Create was compared at its ANSI RVA 0x1d7920); only
// rel32 call targets and RIP-relative displacements differ.  Every address
// quoted below is mfc140u unless stated otherwise.
//
// The class is the classic floating control-bar frame (shipping afxpriv.h:654):
//     class CMiniDockFrameWnd : public CMiniFrameWnd {
//         DECLARE_DYNCREATE(CMiniDockFrameWnd)
//         CMiniDockFrameWnd();
//         virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
//         virtual void RecalcLayout(BOOL bNotify = TRUE);
//         CDockBar m_wndDockBar;
//         afx_msg OnClose / OnNcLButtonDown / OnNcLButtonDblClk / OnMouseActivate
//     };
// OpenMFC's public headers only forward-declare it (include/openmfc/afxwin.h),
// so the layout is pinned here, as offsets on `this`:
//   +0x040  CWnd::m_hWnd                 Create/RecalcLayout `mov 0x40(%rdi),...`
//   +0x1a0  CFrameWnd::m_bInRecalcLayout Create `mov %r15d,0x1a0(%rcx)`;
//                                        RecalcLayout `cmpl $0x0,0x1a0(%rcx)`
//   +0x1f0  CDockBar m_wndDockBar        ctor `lea 0x1f0(%rbx),%rcx` -> ??0CDockBar(TRUE)
//           (retail sizeof(CMiniFrameWnd) == 0x1f0, core/frame/CMiniFrameWnd.cpp)
//   sizeof 0x380                         CreateObject `mov $0x380,%ecx`; also the
//                                        m_nObjectSize (896) RuntimeClasses.cpp records
// and, inside m_wndDockBar (CDockBar layout: core/controlbar/CDockBar.cpp header):
//   +0x230  m_wndDockBar.m_hWnd          (0x1f0 + 0x40)  Create -> ::SetParent
//   +0x2e0  m_wndDockBar.m_bAutoDelete   (0x1f0 + 0xf0)  ctor `movl $0x0,0x2e0(%rbx)`
//   +0x314  m_wndDockBar.m_dwStyle       (0x1f0 + 0x124) `testb $0x40,0x314(..)` = CBRS_FLOAT_MULTI
//   +0x350  m_wndDockBar.m_arrBars.m_nSize (0x1f0 + 0x160) `cmp 0x350(%rdi),%rax`
// and the CControlBar returned by GetDockedControlBar:
//   +0x140  CControlBar::m_pDockContext  `mov 0x140(%rax),%rcx`
//
// Virtual dispatch.  Retail calls three kinds of virtuals here:
//   * CDockContext slots 0/1/2 (StartDrag / StartResize / ToggleDocking) on
//     pBar->m_pDockContext.  core/controlbar/CDockContext.cpp gives every
//     CDockContext it constructs an MSVC-layout vtable with exactly those
//     slots (g_DockContextVtbl), and a client-built one carries the retail
//     layout, so the bodies below dispatch through the object's own vtable at
//     the retail slot, as retail does.
//   * CDockBar vslot 108 (Create, `call *0x360(%rax)` on m_wndDockBar).  The
//     embedded CDockBar keeps the CControlBar mingw vtable its constructor
//     thunk installs (CDockBar.cpp file header), so the CDockBar::Create thunk
//     is called directly; a client-derived override is not honoured.
//   * Nothing else: CMiniFrameWnd::CreateEx, CFrameWnd::RecalcLayout and
//     CWnd::Default are DIRECT calls in retail.
//
// vtable pointer.  The retail constructor stores the CMiniDockFrameWnd
// vftable (0x180321960, mfc140u) at +0.  OpenMFC has no C++ CMiniDockFrameWnd
// class, so an object built by the constructor below keeps the mingw
// CMiniFrameWnd vtable its base constructor thunk installs.  KNOWN GAPS that
// follow from that (not fixable in this file): OpenMFC-internal virtual calls
// on such an object reach CMiniFrameWnd/CFrameWnd, not the Create/RecalcLayout
// exports below; its GetRuntimeClass reports CMiniFrameWnd; and deleting it
// through that vtable runs ~CMiniFrameWnd without destroying m_wndDockBar
// (in retail the compiler-generated destructor destroys the member; its
// body was not disassembled for this file).  An MSVC client that makes a
// virtual call on such an object (e.g. `pFrame->Create(...)`, compiled
// against the retail CMiniDockFrameWnd vftable layout) indexes this mingw
// vtable with a retail slot number and is not routed to these exports
// either; the old stub constructor returned unconstructed memory, so this is
// no regression, but it is not retail behaviour.
//
// Message map.  The retail map (AFX_MSGMAP at 0x180321d08, mfc140u, returned
// by GetThisMessageMap at RVA 0x1d98b0 (mfc140u); entries dumped with
// msgmap_u.py) cross-checks the four handler entries above:
//   WM_CLOSE (0x10) sig 19 -> 0x1d9c10      WM_NCLBUTTONDOWN (0xa1) sig 54 -> 0x1d9c20
//   WM_NCLBUTTONDBLCLK (0xa3) sig 54 -> 0x1d9d00   WM_MOUSEACTIVATE (0x21) sig 12 -> 0x1d98c0
// OpenMFC's copy (featurepack/docking/MessageMaps.cpp ->
// detail/Pane16MsgmapSupport.cpp) has an EMPTY entry table, so the handlers
// below are reached only when client code calls them (e.g. a derived frame
// forwarding to CMiniDockFrameWnd::OnNcLButtonDown), never through that map.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <cstddef>

// ---------------------------------------------------------------------------
// Sibling / base thunks, declared with the signature their mangled name
// describes; every definition was located with grep before use.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CMiniFrameWnd__QEAA_XZ(void* pThis);                                  // core/frame/Thunks.cpp
extern "C" void* MS_ABI impl___0CDockBar__QEAA_H_Z(void* pThis, int bFloating);                       // core/controlbar/CDockBar.cpp
extern "C" int MS_ABI impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(
    void* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID);                          // core/controlbar/CDockBar.cpp
extern "C" void MS_ABI impl__ShowAll_CDockBar__QEAAXH_Z(void* pThis, int bShow);                      // core/controlbar/CDockBar.cpp
extern "C" CControlBar* MS_ABI impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(
    const void* pThis, int nPos);                                                                     // core/controlbar/CDockBar.cpp
extern "C" int MS_ABI impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
    CMiniFrameWnd* pThis, DWORD dwExStyle, const wchar_t* lpClassName, const wchar_t* lpWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);                                     // core/frame/CMiniFrameWnd.cpp
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);        // core/frame/Thunks.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                  // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ActivateTopParent_CWnd__QEAAXXZ(CWnd* pThis);                            // core/window/Thunks.cpp
extern "C" int MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(
    const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount);                                        // core/window/CWnd.cpp
extern "C" void MS_ABI impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(HWND hWnd, const wchar_t* lpszText);  // core/collections/Globals.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                     // core/window/CWnd.cpp
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);               // core/window/CMenu.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);  // featurepack/CMFC_misc_stubs.cpp
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);                                                       // core/collections/CStringT.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                        // detail/MemcoreSupport.cpp

// This file's own constructor (CreateObject calls it).
extern "C" void* MS_ABI impl___0CMiniDockFrameWnd__QEAA_XZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Layout (see the file header for where every offset was read).
// ---------------------------------------------------------------------------
constexpr std::size_t kOffInRecalcLayout = 0x1a0;    // CFrameWnd::m_bInRecalcLayout
constexpr std::size_t kOffDockBar        = 0x1f0;    // CDockBar m_wndDockBar
constexpr std::size_t kSizeofMiniDock    = 0x380;    // retail sizeof(CMiniDockFrameWnd)
constexpr std::size_t kOffDockBarArrSize = 0x160;    // CDockBar::m_arrBars.m_nSize (CDockBar.cpp DockBarTail)

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CControlBar, m_bAutoDelete) == 0xf0, "CControlBar::m_bAutoDelete (+0x2e0 on this)");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle (+0x314 on this)");
static_assert(offsetof(CControlBar, m_pDockContext) == 0x140, "CControlBar::m_pDockContext");
static_assert(kOffDockBar + 0x40 == 0x230, "m_wndDockBar.m_hWnd");
static_assert(kOffDockBar + 0xf0 == 0x2e0, "m_wndDockBar.m_bAutoDelete");
static_assert(kOffDockBar + 0x124 == 0x314, "m_wndDockBar.m_dwStyle");
static_assert(kOffDockBar + kOffDockBarArrSize == 0x350, "m_wndDockBar.m_arrBars.m_nSize");
static_assert(kOffDockBar + 0x190 == kSizeofMiniDock, "CDockBar (0x190) ends the object");
static_assert(offsetof(CMenu, m_hMenu) == 0x08, "CMenu::m_hMenu (retail reads pSysMenu+8)");
// OpenMFC's CFrameWnd does not name m_bInRecalcLayout: +0x1a0 lies in the
// _framewnd_padding after m_nIDHelp (the same slot core/controlbar/CReBar.cpp
// reads), so it is accessed by offset as retail does.
static_assert(offsetof(CFrameWnd, m_nIDHelp) + sizeof(UINT) <= kOffInRecalcLayout &&
              kOffInRecalcLayout + sizeof(BOOL) <= sizeof(CFrameWnd),
              "m_bInRecalcLayout slot must lie inside CFrameWnd's trailing padding");

// Style / ID constants the retail bodies use (afxres.h, afxwin.h, afxpriv.h).
constexpr DWORD kCBRS_SIZE_DYNAMIC   = 0x0004;
constexpr DWORD kCBRS_FLOAT_MULTI    = 0x0040;   // `testb $0x40,0x314` (low byte of m_dwStyle), `and $0x40,%esi`
constexpr DWORD kCBRS_ALIGN_LEFT     = 0x1000;
constexpr DWORD kCBRS_ALIGN_TOP      = 0x2000;
constexpr DWORD kCBRS_ALIGN_RIGHT    = 0x4000;
constexpr DWORD kMFS_MOVEFRAME       = 0x0800;
// WS_POPUP|WS_CAPTION|WS_SYSMENU|MFS_MOVEFRAME*|MFS_4THICKFRAME|MFS_SYNCACTIVE|
// MFS_BLOCKSYSMENU|FWS_SNAPTOBARS without MFS_MOVEFRAME: the `or $0x80c83300`.
constexpr DWORD kFrameStyleBase      = 0x80c83300;
constexpr UINT  kAFX_IDW_DOCKBAR_FLOAT = 0xE81F;
constexpr UINT  kAFX_IDS_HIDE          = 0xF011;
constexpr UINT  kHTSIZEFIRST = 10;              // HTLEFT
constexpr UINT  kHTSIZELAST  = 17;              // HTBOTTOMRIGHT

// Retail passes &CFrameWnd::rectDefault, whose bytes in both retail images
// (mfc140.dll 0x1803ab470, read directly) are {CW_USEDEFAULT, CW_USEDEFAULT, 0, 0}.
// OpenMFC's own CFrameWnd::rectDefault (core/frame/CFrameWnd.cpp) is
// zero-initialised, which differs, so the retail value is used here.
const RECT kRectDefault = { static_cast<LONG>(CW_USEDEFAULT), static_cast<LONG>(CW_USEDEFAULT), 0, 0 };

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline CWnd* Wnd(void* pThis) { return static_cast<CWnd*>(pThis); }
inline void* DockBar(void* pThis) { return Bytes(pThis) + kOffDockBar; }
inline BOOL& InRecalcLayout(void* pThis) { return *reinterpret_cast<BOOL*>(Bytes(pThis) + kOffInRecalcLayout); }
inline CControlBar* DockBarAsBar(void* pThis) { return static_cast<CControlBar*>(DockBar(pThis)); }
inline INT_PTR DockBarArrSize(void* pThis) {
    return *reinterpret_cast<INT_PTR*>(Bytes(DockBar(pThis)) + kOffDockBarArrSize);
}

// CDockContext vtable slots (retail vftable 0x180328f98, mfc140u; see the
// core/controlbar/CDockContext.cpp header).
using PFN_StartDrag     = void (MS_ABI*)(void* pCtx, long long pt);
using PFN_StartResize   = void (MS_ABI*)(void* pCtx, int nHitTest, long long pt);
using PFN_ToggleDocking = void (MS_ABI*)(void* pCtx);
inline void* const* Vtbl(void* pObj) { return *static_cast<void* const* const*>(pObj); }

// The loop retail inlines three times (OnNcLButtonDown twice, OnNcLButtonDblClk
// once), transcribed from OnNcLButtonDown 0x1d9c20 (mfc140u):
//     int nPos = 1;
//     for (;;) {
//         if ((INT_PTR)nPos >= m_wndDockBar.m_arrBars.m_nSize)   // movslq; cmp 0x350; jge
//             AfxThrowInvalidArgException();                     // 0x227720, does not return
//         pBar = m_wndDockBar.GetDockedControlBar(nPos++);       // 0x1d95f0
//         if (pBar != NULL) break;
//     }
//     pCtx = pBar->m_pDockContext;                               // +0x140
//     if (pCtx == NULL) AfxThrowInvalidArgException();
// (This reads like a `while (pBar == NULL && nPos < size) ...` search followed
// by ENSURE checks, but the MFC source is not on this host; the loop above is
// what the disassembly shows.)  Returns NULL only if the throw thunk returns
// (it does not).
void* FirstDockContext(void* pThis) {
    CControlBar* pBar = nullptr;
    int nPos = 1;
    for (;;) {
        if (static_cast<INT_PTR>(nPos) >= DockBarArrSize(pThis)) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return nullptr;
        }
        pBar = impl__GetDockedControlBar_CDockBar__IEBAPEAVCControlBar__H_Z(DockBar(pThis), nPos++);
        if (pBar != nullptr) break;
    }
    void* pCtx = pBar->m_pDockContext;
    if (pCtx == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    return pCtx;
}

inline bool DockBarIsFloatMulti(void* pThis) {
    return (DockBarAsBar(pThis)->m_dwStyle & kCBRS_FLOAT_MULTI) != 0;   // testb $0x40,0x314
}

} // namespace

// Transcribed from retail RVA 0x1d98e0 (mfc140u):
//     CMiniFrameWnd::CMiniFrameWnd();                         // 0x2a8cf0
//     vfptr = &CMiniDockFrameWnd::`vftable' (0x180321960);    // not reproduced (file header)
//     m_wndDockBar.CDockBar::CDockBar(TRUE);                  // +0x1f0, 0x1d8040
//     m_wndDockBar.m_bAutoDelete = FALSE;                     // +0x2e0
//     return this;
// OpenMFC's CMiniFrameWnd constructor writes its _pad block up to +0x1f4
// (core/frame/CMiniFrameWnd.cpp); the CDockBar constructor runs after it and
// owns +0x1f0 onward from then on, so the overlap is harmless.
// Symbol: ??0CMiniDockFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMiniDockFrameWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    impl___0CMiniFrameWnd__QEAA_XZ(pThis);
    impl___0CDockBar__QEAA_H_Z(DockBar(pThis), TRUE);
    DockBarAsBar(pThis)->m_bAutoDelete = FALSE;
    return pThis;
}

// Transcribed from retail RVA 0x1d9990 (mfc140u):
//     m_bInRecalcLayout = TRUE;                                            // +0x1a0
//     DWORD dwStyle = 0x80c83300 | (~(dwBarStyle << 9) & MFS_MOVEFRAME);   // i.e. MFS_MOVEFRAME
//                                                  //   unless dwBarStyle & CBRS_SIZE_DYNAMIC
//     if (!CMiniFrameWnd::CreateEx(0, NULL, L"", dwStyle,                  // 0x2a8e20, direct
//                                  CFrameWnd::rectDefault, pParent, 0))
//         goto fail;
//     DWORD dwAlign = (dwBarStyle & (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)) ? CBRS_ALIGN_LEFT
//                                                                      : CBRS_ALIGN_TOP;
//     CMenu* pSysMenu = CMenu::FromHandle(::GetSystemMenu(m_hWnd, FALSE));  // IAT GetSystemMenu; 0x2a80a0
//     if (pSysMenu != NULL) {
//         ::DeleteMenu(pSysMenu->m_hMenu, SC_SIZE /*0xf000*/, MF_BYCOMMAND);      // IAT DeleteMenu
//         ::DeleteMenu(pSysMenu->m_hMenu, SC_MINIMIZE /*0xf020*/, MF_BYCOMMAND);
//         ::DeleteMenu(pSysMenu->m_hMenu, SC_MAXIMIZE /*0xf030*/, MF_BYCOMMAND);
//         ::DeleteMenu(pSysMenu->m_hMenu, SC_RESTORE /*0xf120*/, MF_BYCOMMAND);
//         CString strHide;                                                        // nil string
//         HINSTANCE h = AfxFindStringResourceHandle(AFX_IDS_HIDE /*0xf011*/);     // 0x2aee00
//         if (h != NULL && strHide.LoadString(h, AFX_IDS_HIDE)) {                 // 0xdb70
//             ::DeleteMenu(pSysMenu->m_hMenu, SC_CLOSE /*0xf060*/, MF_BYCOMMAND);
//             ::AppendMenu(pSysMenu->m_hMenu, MF_STRING|MF_ENABLED /*0*/, SC_CLOSE, strHide);  // IAT AppendMenuW
//         }
//     }
//     if (!m_wndDockBar.Create(pParent,                                    // +0x1f0, vslot 108
//             WS_CHILD|WS_VISIBLE | dwAlign | (dwBarStyle & CBRS_FLOAT_MULTI /*0x40*/),
//             AFX_IDW_DOCKBAR_FLOAT /*0xe81f*/))
//         goto fail;
//     CWnd::FromHandle(::SetParent(m_wndDockBar.m_hWnd, m_hWnd));          // IAT SetParent; 0x28ad70,
//                                                                          // result discarded
//     m_bInRecalcLayout = FALSE;  return TRUE;
//   fail:
//     m_bInRecalcLayout = FALSE;  return FALSE;
// Deviations: vslot 108 is the CDockBar::Create thunk called directly (file
// header); rectDefault is this file's retail-valued kRectDefault (see there).
// Symbol: ?Create@CMiniDockFrameWnd@@UEAAHPEAVCWnd@@K@Z
extern "C" int MS_ABI impl__Create_CMiniDockFrameWnd__UEAAHPEAVCWnd__K_Z(void* pThis, CWnd* pParent, unsigned long dwBarStyle) {
    InRecalcLayout(pThis) = TRUE;
    DWORD dwStyle = kFrameStyleBase | (~(dwBarStyle << 9) & kMFS_MOVEFRAME);
    static_assert((kCBRS_SIZE_DYNAMIC << 9) == kMFS_MOVEFRAME, "shl $9 maps CBRS_SIZE_DYNAMIC onto MFS_MOVEFRAME");
    if (!impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
            static_cast<CMiniFrameWnd*>(pThis), 0, nullptr, L"", dwStyle, kRectDefault, pParent, 0)) {
        InRecalcLayout(pThis) = FALSE;
        return FALSE;
    }
    const DWORD dwAlign = (dwBarStyle & (kCBRS_ALIGN_LEFT | kCBRS_ALIGN_RIGHT)) ? kCBRS_ALIGN_LEFT : kCBRS_ALIGN_TOP;

    CMenu* pSysMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(::GetSystemMenu(Wnd(pThis)->m_hWnd, FALSE));
    if (pSysMenu != nullptr) {
        ::DeleteMenu(pSysMenu->m_hMenu, SC_SIZE, MF_BYCOMMAND);
        ::DeleteMenu(pSysMenu->m_hMenu, SC_MINIMIZE, MF_BYCOMMAND);
        ::DeleteMenu(pSysMenu->m_hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
        ::DeleteMenu(pSysMenu->m_hMenu, SC_RESTORE, MF_BYCOMMAND);
        CString strHide;
        HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kAFX_IDS_HIDE));
        if (hInst != nullptr &&
            impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
                &strHide, hInst, kAFX_IDS_HIDE)) {
            ::DeleteMenu(pSysMenu->m_hMenu, SC_CLOSE, MF_BYCOMMAND);
            ::AppendMenu(pSysMenu->m_hMenu, MF_STRING | MF_ENABLED, SC_CLOSE, strHide.GetString());
        }
    }

    const DWORD dwDockStyle = (dwBarStyle & kCBRS_FLOAT_MULTI) | dwAlign | WS_CHILD | WS_VISIBLE;
    if (!impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(DockBar(pThis), pParent, dwDockStyle, kAFX_IDW_DOCKBAR_FLOAT)) {
        InRecalcLayout(pThis) = FALSE;
        return FALSE;
    }
    impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::SetParent(DockBarAsBar(pThis)->m_hWnd, Wnd(pThis)->m_hWnd));
    InRecalcLayout(pThis) = FALSE;
    return TRUE;
}

// Transcribed from retail RVA 0x1d9d90 (mfc140u): operator new(0x380)
// (0x27f0), and when that returns non-NULL, the constructor (0x1d98e0) on it;
// the result (possibly NULL) is returned.  Allocated through the DLL's
// exported ::operator new thunk, as core/frame/CMiniFrameWnd.cpp does.
// Symbol: ?CreateObject@CMiniDockFrameWnd@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMiniDockFrameWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofMiniDock);
    if (p == nullptr) return nullptr;
    return impl___0CMiniDockFrameWnd__QEAA_XZ(p);
}

// Transcribed from retail RVA 0x1d9c10 (mfc140u): a tail jump to
// m_wndDockBar.ShowAll(FALSE) (`add $0x1f0,%rcx; xor %edx,%edx; jmp 0x1d9580`).
// Symbol: ?OnClose@CMiniDockFrameWnd@@QEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMiniDockFrameWnd__QEAAXXZ(void* pThis) {
    impl__ShowAll_CDockBar__QEAAXH_Z(DockBar(pThis), FALSE);
}

// Transcribed from retail RVA 0x1d98c0 (mfc140u):
//     if (nHitTest - HTSIZEFIRST <= HTSIZELAST - HTSIZEFIRST)   // unsigned, 10..17
//         return MA_NOACTIVATE;                                  // 3
//     return (int)Default();                                     // tail jump, 0x28ac80
// pDesktopWnd and message are not read.
// Symbol: ?OnMouseActivate@CMiniDockFrameWnd@@QEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMiniDockFrameWnd__QEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pDesktopWnd;
    (void)message;
    if (nHitTest - kHTSIZEFIRST <= kHTSIZELAST - kHTSIZEFIRST) {
        return MA_NOACTIVATE;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(Wnd(pThis)));
}

// Transcribed from retail RVA 0x1d9d00 (mfc140u):
//     if (nHitTest == HTCAPTION) {
//         ActivateTopParent();                                   // 0x28e430
//         if (!(m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI)) {    // testb $0x40,0x314
//             <first docked bar's m_pDockContext, see FirstDockContext>
//             pCtx->ToggleDocking();                             // CDockContext vslot 2
//             return;
//         }
//     }
//     Default();                                                 // 0x28ac80
// point is not read.
// Symbol: ?OnNcLButtonDblClk@CMiniDockFrameWnd@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDblClk_CMiniDockFrameWnd__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nHitTest, long long point) {
    (void)point;
    if (nHitTest == HTCAPTION) {
        impl__ActivateTopParent_CWnd__QEAAXXZ(Wnd(pThis));
        if (!DockBarIsFloatMulti(pThis)) {
            void* pCtx = FirstDockContext(pThis);
            if (pCtx == nullptr) return;   // only if the throw thunk returned
            reinterpret_cast<PFN_ToggleDocking>(Vtbl(pCtx)[2])(pCtx);
            return;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
}

// Transcribed from retail RVA 0x1d9c20 (mfc140u):
//     if (nHitTest == HTCAPTION) {
//         ActivateTopParent();                                   // 0x28e430
//         if (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI)         // testb $0x40,0x314
//             { Default(); return; }                             // 0x28ac80
//         <first docked bar's m_pDockContext, see FirstDockContext>
//         pCtx->StartDrag(point);                                // CDockContext vslot 0
//     } else if (nHitTest - HTSIZEFIRST <= HTSIZELAST - HTSIZEFIRST) {   // unsigned, 10..17
//         ActivateTopParent();
//         <first docked bar's m_pDockContext>                    // no FLOAT_MULTI test here
//         pCtx->StartResize(nHitTest, point);                    // CDockContext vslot 1
//     } else {
//         Default();
//     }
// CPoint is passed by value as one 8-byte register (x in the low dword).
// Symbol: ?OnNcLButtonDown@CMiniDockFrameWnd@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CMiniDockFrameWnd__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nHitTest, long long point) {
    if (nHitTest == HTCAPTION) {
        impl__ActivateTopParent_CWnd__QEAAXXZ(Wnd(pThis));
        if (DockBarIsFloatMulti(pThis)) {
            impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
            return;
        }
        void* pCtx = FirstDockContext(pThis);
        if (pCtx == nullptr) return;   // only if the throw thunk returned
        reinterpret_cast<PFN_StartDrag>(Vtbl(pCtx)[0])(pCtx, point);
        return;
    }
    if (nHitTest - kHTSIZEFIRST <= kHTSIZELAST - kHTSIZEFIRST) {
        impl__ActivateTopParent_CWnd__QEAAXXZ(Wnd(pThis));
        void* pCtx = FirstDockContext(pThis);
        if (pCtx == nullptr) return;   // only if the throw thunk returned
        reinterpret_cast<PFN_StartResize>(Vtbl(pCtx)[1])(pCtx, static_cast<int>(nHitTest), point);
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
}

// Transcribed from retail RVA 0x1d9ba0 (mfc140u):
//     if (!m_bInRecalcLayout) {                                   // cmpl $0,0x1a0
//         CFrameWnd::RecalcLayout(bNotify);                       // 0x2a0200, direct call
//         TCHAR szTitle[_MAX_PATH];                               // 0x104 wchar_t (0x240 frame)
//         m_wndDockBar.GetWindowText(szTitle, _countof(szTitle)); // +0x1f0, 0x2a9810
//         AfxSetWindowText(m_hWnd, szTitle);                      // 0x2ae4b0
//     }
// DEVIATION: retail's CFrameWnd::RecalcLayout call is non-virtual, but it is
// made here through OpenMFC's impl__RecalcLayout_CFrameWnd__UEAAXH_Z
// (core/frame/Thunks.cpp), whose body is `pThis->RecalcLayout(p0)`, a C++
// virtual call through the object's mingw vtable.  For an object built by
// this file's constructor that reaches CFrameWnd::RecalcLayout (OpenMFC's
// CMiniFrameWnd, include/openmfc/afxmfc.h, declares no override); for an
// MSVC client-derived object it would index an MSVC vtable with a mingw slot
// number.  A qualified `CFrameWnd::RecalcLayout(bNotify)` call to the C++
// definition in core/frame/CFrameWnd.cpp would avoid that, but this tree's
// link audit rejects new C++ method references from thunk files, so the
// thunk is kept and the fix is requested there (make the thunk non-virtual).
// Symbol: ?RecalcLayout@CMiniDockFrameWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMiniDockFrameWnd__UEAAXH_Z(void* pThis, int bNotify) {
    if (InRecalcLayout(pThis)) return;
    impl__RecalcLayout_CFrameWnd__UEAAXH_Z(static_cast<CFrameWnd*>(pThis), bNotify);   // see DEVIATION above
    wchar_t szTitle[MAX_PATH];
    static_assert(MAX_PATH == 0x104, "retail passes 0x104");
    impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(DockBarAsBar(pThis), szTitle, MAX_PATH);
    impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(Wnd(pThis)->m_hWnd, szTitle);
}
