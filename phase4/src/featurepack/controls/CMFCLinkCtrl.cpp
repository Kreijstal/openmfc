// CMFCLinkCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// How this file was written
// ---------------------------------------------------------------------------
// Every body below was transcribed from the retail disassembly (the method in
// the header comment of core/ole/COleControl.cpp).  Every RVA quoted in this
// file is an mfc140u.dll address: the export entries were resolved through the
// mfc140u ordinal table (urva.py / disas.py --u), callee RVAs were named with
// uwho.py, and every IAT slot with iatu.py / dlyu.py against mfc140u.  The
// mfc140.dll (ANSI twin) bodies of the ctor, dtor and OnClicked were also
// read and are instruction-for-instruction the same apart from addresses.
//
// ---------------------------------------------------------------------------
// Layout
// ---------------------------------------------------------------------------
// OpenMFC declares no `class CMFCLinkCtrl` in include/openmfc/ and there is no
// detail/*Support.h shadow for it, so this file pins the retail layout itself
// (S_LinkCtrl below).  The derived members and their order come from the
// shipping afxlinkctrl.h:30 (read on this host); their offsets come from the
// constructor ??0CMFCLinkCtrl@@QEAA@XZ (entry RVA 0x781b0, mfc140u):
//     call ??0CMFCButton@@QEAA@XZ              (0x1c930)
//     vfptr = CMFCLinkCtrl vftable             (0x1802ecaf8, mfc140u)
//     +0xb30 = nil CString                     m_strURL
//     +0xb38 = nil CString                     m_strPrefix
//     +0xe8  = 3                               m_nFlatStyle = BUTTONSTYLE_NOBORDERS
//     +0x150 = 0 (8 bytes)                     m_sizePushOffset = CSize(0, 0)
//     +0xf8  = 1                               m_bTransparent = TRUE
//     +0xb20 = 0                               m_bMultilineText = FALSE
//     +0xb24 = 1 (8-byte store of 1)           m_bAlwaysUnderlineText = TRUE,
//                                              m_bDefaultClickProcess (+0xb28) = FALSE
//     +0xb2c = 0                               m_bVisited = FALSE
//     call ?SetMouseCursorHand@CMFCButton@@    (0x1e7e0)
// SetURL writes +0xb30 and SetURLPrefix +0xb38 (see below), which confirms the
// two CStrings; sizeof == 0xb40 (2880), the size the CRuntimeClass descriptor
// in featurepack/controls/RuntimeClasses.cpp records.
//
// The CMFCButton members the retail bodies touch are named from the shipping
// afxbutton.h (member order counted from CButton's 0xe8 bytes): +0xe8
// m_nFlatStyle, +0xf8 m_bTransparent, +0x124 m_bHover, +0x13c
// m_bFullTextTooltip, +0x140 m_bDelayFullTextTooltipSet, +0x148 m_sizeImage,
// +0x150 m_sizePushOffset.  The retail body of
// ?EnableFullTextTooltip@CMFCButton@@ (entry 0x1ec00, mfc140u) is exactly
// "store edx to +0x13c and +0x140", which confirms that pair.  NOTE: the
// names detail/CMFCButtonSupport.h (S_Cmfcbutton) gives several of these
// offsets differ (+0xf8 m_bMenuFont, +0x124 m_bHighlighted, +0x13c/+0x140
// m_bDrawFocusA/B); only the offsets are relied on here, the names used are
// the shipping header's.
//
// ---------------------------------------------------------------------------
// vptr
// ---------------------------------------------------------------------------
// ??0CMFCButton@@QEAA@XZ (featurepack/controls/Thunks.cpp) placement-news
// OpenMFC's C++ CMFCButton, so an object built here carries OpenMFC's mingw
// CMFCButton vtable; there is no MSVC-layout CMFCLinkCtrl vtable in this DLL,
// so retail's store of the CMFCLinkCtrl vftable is not reproduced.  None of
// the retail bodies below makes a virtual call on `this` (OnClicked is called
// directly, and the CMFCButton base calls are direct calls), so no slot
// dispatch helper is needed.  The destructor stores back the vptr the
// constructor recorded (g_ownVptr) -- retail's "vfptr = &vftable" store --
// so that ??1CMFCButton@@'s C++ virtual-destructor call dispatches on
// OpenMFC's table and not on a client subclass's MSVC table (the scheme of
// featurepack/menu/CMFCMenuButton.cpp).
//
// ---------------------------------------------------------------------------
// Deviations common to several bodies
// ---------------------------------------------------------------------------
//  * DC calls go through the CDC impl__ thunks (BRIEFING §1) rather than
//    through pDC's vtable.  Retail's slots are, on the CDC vftable
//    0x18033b510 (mfc140u, dumped with vtdump_u.py): +0x60 slot 12 =
//    SelectObject(CFont*) (0x2a2730), +0x70 slot 14 = SetTextColor
//    (0x2a2960), +0xe0 slot 28 = 0x1e6ce0, a `mov 8(%rcx),%rcx; jmp
//    [DrawTextW]` stub (IAT 0x2c6f08 USER32!DrawTextW), i.e. DrawText.
//  * null checks on `this`, pDC, rect pointers, pMsg and SizeToContent's
//    hidden return slot where retail has only ASSERT_VALID or dereferences
//    unconditionally (SizeToContent throws on a NULL `this` rather than
//    faulting on its first m_sizeImage read).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <shellapi.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <new>

// ---------------------------------------------------------------------------
// Thunks this file calls (definitions named on each line).
// ---------------------------------------------------------------------------
// featurepack/controls/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCButton__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCButton__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetTooltip_CMFCButton__QEAAXPEB_W_Z(CMFCButton* pThis, const wchar_t* lpszToolTipText);
// featurepack/controls/CMFCButton.cpp
extern "C" void  MS_ABI impl__SetMouseCursorHand_CMFCButton__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__SelectFont_CMFCButton__MEAAPEAVCFont__PEAVCDC___Z(void* pThis, void* pDC);
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(void* pThis, CSize* pRet, int bCalcOnly);
extern "C" int   MS_ABI impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg);
// core/window/CWnd.cpp
extern "C" int   MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
// detail/RegcoreSupport.cpp, core/runtime/CCmdTarget.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// core/gdi/CDC.cpp, detail/GdicoreSupport.cpp, core/gdi/CClientDC.cpp
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" int   MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);
extern "C" int   MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd);
extern "C" void  MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);
// core/runtime/AFX_GLOBAL_DATA.cpp, featurepack/CMFC_misc_stubs.cpp
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];   // 0x3c1620 (mfc140u)
// featurepack/controls/CMFCControlContainer.cpp, featurepack/controls/CTagManager.cpp
extern "C" int   MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8);
extern "C" void  MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);
extern "C" void  MS_ABI impl___1CTagManager__UEAA_XZ(void* self);
extern "C" int   MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* self, const wchar_t* tag, CString* value, int trim);

// Forward declarations of this file's own exports (called by later bodies).
extern "C" int  MS_ABI impl__OnClicked_CMFCLinkCtrl__IEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__SetURL_CMFCLinkCtrl__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszURL);
extern "C" void MS_ABI impl__SetURLPrefix_CMFCLinkCtrl__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszPrefix);

namespace {

// ---------------------------------------------------------------------------
// Retail CMFCLinkCtrl layout (see the file header).
// ---------------------------------------------------------------------------
struct S_LinkCtrl {
    void*   vfptr;                          // 0x000
    char    _pad008[0x040 - 0x008];
    HWND    m_hWnd;                         // 0x040 CWnd::m_hWnd
    char    _pad048[0x0e8 - 0x048];
    int     m_nFlatStyle;                   // 0x0e8 CMFCButton
    char    _pad0ec[0x0f8 - 0x0ec];
    BOOL    m_bTransparent;                 // 0x0f8 CMFCButton
    char    _pad0fc[0x124 - 0x0fc];
    BOOL    m_bHover;                       // 0x124 CMFCButton
    char    _pad128[0x13c - 0x128];
    BOOL    m_bFullTextTooltip;             // 0x13c CMFCButton
    BOOL    m_bDelayFullTextTooltipSet;     // 0x140 CMFCButton
    char    _pad144[0x148 - 0x144];
    SIZE    m_sizeImage;                    // 0x148 CMFCButton
    SIZE    m_sizePushOffset;               // 0x150 CMFCButton
    char    _pad158[0xb20 - 0x158];
    BOOL    m_bMultilineText;               // 0xb20
    BOOL    m_bAlwaysUnderlineText;         // 0xb24
    BOOL    m_bDefaultClickProcess;         // 0xb28
    BOOL    m_bVisited;                     // 0xb2c
    CString m_strURL;                       // 0xb30 (protected)
    CString m_strPrefix;                    // 0xb38 (protected)
};
static_assert(offsetof(S_LinkCtrl, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_LinkCtrl, m_nFlatStyle) == 0xe8, "ctor 0x781b0: movl $3,0xe8");
static_assert(offsetof(S_LinkCtrl, m_bTransparent) == 0xf8, "ctor 0x781b0: mov %eax(=1),0xf8");
static_assert(offsetof(S_LinkCtrl, m_bHover) == 0x124, "OnDraw 0x78330: lea 0x124(%rcx),%rbx");
static_assert(offsetof(S_LinkCtrl, m_bFullTextTooltip) == 0x13c, "EnableFullTextTooltip 0x1ec00 / OnInitControl 0x78a10");
static_assert(offsetof(S_LinkCtrl, m_bDelayFullTextTooltipSet) == 0x140, "EnableFullTextTooltip 0x1ec00 / OnInitControl 0x78a10");
static_assert(offsetof(S_LinkCtrl, m_sizeImage) == 0x148, "SizeToContent 0x78700: cmpl $0,0x148 / 0x14c");
static_assert(offsetof(S_LinkCtrl, m_sizePushOffset) == 0x150, "ctor 0x781b0: mov %rcx(=0),0x150");
static_assert(offsetof(S_LinkCtrl, m_bMultilineText) == 0xb20, "ctor 0x781b0 / OnDraw 0x78330");
static_assert(offsetof(S_LinkCtrl, m_bAlwaysUnderlineText) == 0xb24, "ctor 0x781b0 / OnDraw 0x78330");
static_assert(offsetof(S_LinkCtrl, m_bDefaultClickProcess) == 0xb28, "ctor 0x781b0 / OnClicked 0x78520");
static_assert(offsetof(S_LinkCtrl, m_bVisited) == 0xb2c, "ctor 0x781b0 / OnClicked 0x78520 / OnDraw 0x78330");
static_assert(offsetof(S_LinkCtrl, m_strURL) == 0xb30, "ctor 0x781b0 / SetURL 0x78670");
static_assert(offsetof(S_LinkCtrl, m_strPrefix) == 0xb38, "ctor 0x781b0 / SetURLPrefix 0x786c0");
static_assert(sizeof(S_LinkCtrl) == 0xb40, "retail sizeof(CMFCLinkCtrl) == 2880");
static_assert(sizeof(CString) == sizeof(void*), "CString is one m_pszData pointer (the retail 8-byte member)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "OpenMFC CWnd::m_hWnd must sit where the retail bodies read it");
static_assert(offsetof(CDC, m_hDC) == 0x8, "OnDrawFocusRect 0x78980: mov 0x8(%rdx),%rcx (CDC::m_hDC)");
static_assert(sizeof(CMFCButton) <= 0xb20, "the ??0CMFCButton thunk placement-constructs OpenMFC's CMFCButton inside the retail 0xb20-byte base");

inline S_LinkCtrl* L(void* p) { return static_cast<S_LinkCtrl*>(p); }
inline CWnd*       W(void* p) { return static_cast<CWnd*>(p); }

// AFX_GLOBAL_DATA offsets (the transcription in core/runtime/AFX_GLOBAL_DATA.cpp,
// which declares all four at these offsets).  Retail reads afxGlobalData
// (0x3c1620, mfc140u) + 0x50 / 0x54 / 0x58 (0x3c1670 / 0x3c1674 / 0x3c1678)
// and takes the address + 0x1f8 (0x3c1818).
constexpr std::size_t kGdInitGate               = 0x000;   // m_bInitialized
constexpr std::size_t kGdClrHotLinkNormalText   = 0x050;
constexpr std::size_t kGdClrHotLinkHoveredText  = 0x054;
constexpr std::size_t kGdClrHotLinkVisitedText  = 0x058;
constexpr std::size_t kGdFontDefaultGUIUnderline = 0x1f8;  // a CFont {vfptr, m_hObject}

// Retail's inline GetGlobalData(): if (!m_bInitialized) { Initialize(); m_bInitialized = TRUE; }
// (Initialize 0x6a790, mfc140u; the gate is inlined before every read below.)
inline void EnsureGlobalDataInitialized() {
    int bInit = 0;
    std::memcpy(&bInit, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, sizeof bInit);
    if (bInit == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, &one, sizeof one);
    }
}
inline COLORREF GlobalColor(std::size_t off) {
    COLORREF clr = 0;
    std::memcpy(&clr, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof clr);
    return clr;
}
// &afxGlobalData.fontDefaultGUIUnderline, as retail passes it (lea 0x3c1818).
inline CFont* GlobalUnderlineFont() {
    return reinterpret_cast<CFont*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdFontDefaultGUIUnderline);
}

// ENSURE(): retail calls ?AfxThrowInvalidArgException@@YAXXZ (0x227720).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// CWaitCursor stand-in for OnClicked (see the comment above that body).
struct WaitCursorScope {
    CCmdTarget* pApp;
    WaitCursorScope() : pApp(static_cast<CCmdTarget*>(impl__AfxGetApp__YAPEAVCWinApp__XZ())) {
        if (pApp != nullptr) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp != nullptr) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    WaitCursorScope(const WaitCursorScope&) = delete;
    WaitCursorScope& operator=(const WaitCursorScope&) = delete;
};

void* g_ownVptr = nullptr;   // the vptr ??0CMFCButton@@QEAA@XZ installs, recorded by the first ctor to run

} // namespace

// Retail (entry RVA 0x781b0, mfc140u), fully transcribed -- see the file
// header for the member stores, which are made here in retail's order.  The
// vptr store of the CMFCLinkCtrl vftable (0x1802ecaf8, mfc140u) is not
// reproduced (file header); the vptr ??0CMFCButton@@ installed is recorded as
// g_ownVptr instead.  The two CStrings are this DLL's own CString
// constructed empty -- retail stores the string manager's nil string
// (IAtlStringMgr slot 3 on the static at 0x3b25e8, + 0x18) -- the same
// arrangement featurepack/controls/CMFCEditBrowseCtrl.cpp uses.
// Symbol: ??0CMFCLinkCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCLinkCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCButton__QEAA_XZ(pThis);                     // 0x1c930
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    S_LinkCtrl* s = L(pThis);
    ::new (static_cast<void*>(&s->m_strURL)) CString();
    ::new (static_cast<void*>(&s->m_strPrefix)) CString();
    s->m_nFlatStyle = 3;                                    // BUTTONSTYLE_NOBORDERS
    s->m_sizePushOffset.cx = 0;
    s->m_sizePushOffset.cy = 0;
    s->m_bTransparent = TRUE;
    s->m_bMultilineText = FALSE;
    s->m_bAlwaysUnderlineText = TRUE;
    s->m_bDefaultClickProcess = FALSE;
    s->m_bVisited = FALSE;
    impl__SetMouseCursorHand_CMFCButton__QEAAXXZ(pThis);   // 0x1e7e0
    return pThis;
}

// Retail (entry RVA 0x782b0, mfc140u), fully transcribed:
//     vfptr = &CMFCLinkCtrl::`vftable';        // 0x1802ecaf8 -> g_ownVptr here (file header)
//     m_strPrefix.~CString();                  // +0xb38: CStringData refcount release
//     m_strURL.~CString();                     // +0xb30
//     jmp ??1CMFCButton@@UEAA@XZ               // 0x1cad0
// Symbol: ??1CMFCLinkCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCLinkCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    S_LinkCtrl* s = L(pThis);
    s->m_strPrefix.~CString();
    s->m_strURL.~CString();
    impl___1CMFCButton__UEAA_XZ(pThis);
}

// Retail (entry RVA 0x78520, mfc140u), fully transcribed:
//     if (!IsWindowEnabled()) return TRUE;                          // 0x2a9b00
//     if (m_bDefaultClickProcess) {                                 // +0xb28
//         m_bHover = FALSE;                                         // +0x124
//         ::InvalidateRect(m_hWnd, NULL, TRUE);                     // import 0x2c7128 (inline Invalidate())
//         ::UpdateWindow(m_hWnd);                                   // import 0x2c7300
//         return FALSE;
//     }
//     CWaitCursor wait;        // AfxGetModuleState() (0x133930) ->m_pCurrentWinApp (+8)
//                              //   ->BeginWaitCursor() (0x1de7b0); the destructor
//                              //   helper 0x7687c calls ->EndWaitCursor() (0x1de7e0)
//     CString strURL = m_strURL;                                    // +0xb30
//     if (strURL.IsEmpty()) GetWindowText(strURL);                  // 0x28be00
//     ::ShellExecute(NULL, NULL, m_strPrefix + strURL,              // +0xb38; unexported concat helper 0x78c90
//                    NULL, NULL, SW_SHOWNORMAL);                    // delay-import 0x3e91b8 SHELL32!ShellExecuteW
//     m_bVisited = TRUE;                                            // +0xb2c
//     m_bHover = FALSE;
//     ::InvalidateRect(m_hWnd, NULL, TRUE);
//     ::UpdateWindow(m_hWnd);
//     return TRUE;                  // ~CString(strURL), then ~CWaitCursor
// The ShellExecute result is not examined.  Deviation: the wait cursor is
// taken on the object ?AfxGetApp@@ (detail/RegcoreSupport.cpp) returns, which
// is OpenMFC's AfxGetThread() (detail/CWinAppSupport.cpp): the thread-local
// current CWinThread when one is set, otherwise the application object.  On
// a secondary UI thread that is NOT retail's module-state m_pCurrentWinApp;
// it makes no difference here because OpenMFC's Begin/EndWaitCursor thunks
// (core/runtime/CCmdTarget.cpp) only keep a per-pointer depth counter and set
// the cursor, and both calls get the same pointer.  With no object at all the
// pair is skipped (retail's BeginWaitCursor 0x1de7b0 re-reads m_pCurrentWinApp
// and calls through its vtable, so it would fault).  The pair is held in a
// scope object (WaitCursorScope) so that, as with retail's CWaitCursor,
// EndWaitCursor also runs if GetWindowText or the concatenation throws.
// Symbol: ?OnClicked@CMFCLinkCtrl@@IEAAHXZ
extern "C" int MS_ABI impl__OnClicked_CMFCLinkCtrl__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return TRUE;
    S_LinkCtrl* s = L(pThis);
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(W(pThis))) return TRUE;

    if (s->m_bDefaultClickProcess) {
        s->m_bHover = FALSE;
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
        return FALSE;
    }

    WaitCursorScope wait;
    {
        CString strURL = s->m_strURL;
        if (strURL.IsEmpty()) {
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(W(pThis), &strURL);
        }
        {
            CString strFull = s->m_strPrefix;
            strFull += strURL;
            ::ShellExecuteW(nullptr, nullptr, strFull.GetString(), nullptr, nullptr, SW_SHOWNORMAL);
        }
        s->m_bVisited = TRUE;
        s->m_bHover = FALSE;
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    return TRUE;
}

// Retail (entry RVA 0x78330, mfc140u), fully transcribed:
//     CFont* pOldFont;
//     if (!m_bAlwaysUnderlineText && !m_bHover)                     // +0xb24, +0x124
//         pOldFont = CMFCButton::SelectFont(pDC);                   // direct call 0x1eaa0
//     else
//         pOldFont = pDC->SelectObject(&afxGlobalData.fontDefaultGUIUnderline);  // vslot 12 (+0x60), +0x1f8
//     ENSURE(pOldFont != NULL);                                     // else 0x227720
//     pDC->SetTextColor(m_bHover   ? afxGlobalData.clrHotLinkHoveredText    // +0x54
//                     : m_bVisited ? afxGlobalData.clrHotLinkVisitedText    // +0xb2c, +0x58
//                     :              afxGlobalData.clrHotLinkNormalText);   // +0x50; vslot 14 (+0x70)
//     pDC->SetBkMode(TRANSPARENT);                                  // 0x2a2860
//     CString strLabel;  GetWindowText(strLabel);                   // 0x28be00
//     CRect rectText = rect;
//     pDC->DrawText(strLabel, strLabel.GetLength(), rectText,
//                   m_bMultilineText ? DT_WORDBREAK : DT_SINGLELINE);  // +0xb20 (neg/sbb/and -16/add 0x20); vslot 28 (+0xe0)
//     pDC->SelectObject(pOldFont);                                  // vslot 12
// with the inline GetGlobalData() gate (Initialize 0x6a790) before each
// afxGlobalData read.  uiState is not read.
// Deviation, SelectFont path only: OpenMFC's ?SelectFont@CMFCButton@@ thunk
// selects its font but always returns NULL (featurepack/controls/CMFCButton.cpp,
// "previously selected CFont* not returned"), so retail's ENSURE would throw on
// every non-underlined paint.  On that path a NULL result does not throw; the
// DC's current font is captured with ::GetCurrentObject before the call and
// restored directly afterwards.  The underline path keeps retail's ENSURE.
// Symbol: ?OnDraw@CMFCLinkCtrl@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI impl__OnDraw_CMFCLinkCtrl__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, CDC* pDC, const RECT* rect, unsigned int uiState) {
    (void)uiState;
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    S_LinkCtrl* s = L(pThis);

    CFont* pOldFont = nullptr;
    HGDIOBJ hFontBefore = nullptr;   // deviation (see above)
    if (!s->m_bAlwaysUnderlineText && !s->m_bHover) {
        hFontBefore = ::GetCurrentObject(pDC->m_hDC, OBJ_FONT);
        pOldFont = static_cast<CFont*>(impl__SelectFont_CMFCButton__MEAAPEAVCFont__PEAVCDC___Z(pThis, pDC));
    } else {
        EnsureGlobalDataInitialized();
        pOldFont = impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, GlobalUnderlineFont());
        if (pOldFont == nullptr) { ThrowInvalidArg(); return; }
    }

    EnsureGlobalDataInitialized();
    const COLORREF clrText = s->m_bHover   ? GlobalColor(kGdClrHotLinkHoveredText)
                           : s->m_bVisited ? GlobalColor(kGdClrHotLinkVisitedText)
                           :                 GlobalColor(kGdClrHotLinkNormalText);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);

    CString strLabel;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(W(pThis), &strLabel);
    RECT rectText = *rect;
    impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
        pDC, strLabel.GetString(), strLabel.GetLength(), &rectText,
        s->m_bMultilineText ? DT_WORDBREAK : DT_SINGLELINE);

    if (pOldFont != nullptr) {
        impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    } else if (hFontBefore != nullptr && pDC->m_hDC != nullptr) {
        ::SelectObject(pDC->m_hDC, hFontBefore);             // deviation (see above)
    }
}

// Retail (entry RVA 0x78980, mfc140u), fully transcribed:
//     CRect rectFocus = rectClient;                 // 16-byte copy to the stack
//     pDC->DrawFocusRect(rectFocus);                // inline: ::DrawFocusRect(pDC->m_hDC, ...), import 0x2c71d8
// Symbol: ?OnDrawFocusRect@CMFCLinkCtrl@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCLinkCtrl__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, CDC* pDC, const RECT* rectClient) {
    (void)pThis;
    if (pDC == nullptr || rectClient == nullptr) return;
    RECT rectFocus = *rectClient;
    ::DrawFocusRect(pDC->m_hDC, &rectFocus);
}

// Retail (entry RVA 0x78a10, mfc140u), fully transcribed:
//     CString strDst;
//     CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                                             // 0x13d840
//     CString strURL;
//     if (tagManager.ExcludeTag(L"MFCLink_Url", strURL, FALSE))        SetURL(strURL);        // 0x13df50; 0x78670
//     CString strURLPrefix;
//     if (tagManager.ExcludeTag(L"MFCLink_UrlPrefix", strURLPrefix, FALSE)) SetURLPrefix(strURLPrefix);  // 0x786c0
//     CString strFullTextTooltip;
//     if (tagManager.ExcludeTag(L"MFCLink_FullTextTooltip", strFullTextTooltip, FALSE) &&
//         !strFullTextTooltip.IsEmpty()) {
//         strFullTextTooltip.MakeUpper();                                         // 0x1fb80
//         BOOL b = (wcscmp(strFullTextTooltip, L"TRUE") == 0);                    // import 0x2c7770
//         m_bFullTextTooltip = b;  m_bDelayFullTextTooltipSet = b;                // +0x13c, +0x140
//     }                                                  // (EnableFullTextTooltip(b) inlined)
//     CString strTooltip;
//     if (tagManager.ExcludeTag(L"MFCLink_Tooltip", strTooltip, FALSE)) SetTooltip(strTooltip);  // 0x1e700
//     return 0;                                          // ~CStrings, ~CTagManager 0x13d8f0
// The tag strings were read from mfc140u's .rdata (0x33f8f0 / 0x33f908 /
// 0x33f930 / 0x33f960, "TRUE" at 0x33d930) with ustr.py.
// The two EnableFullTextTooltip stores are made on the object's raw bytes, as
// the inlined retail code does, rather than through the exported
// ?EnableFullTextTooltip@CMFCButton@@ thunk (featurepack/controls/Thunks.cpp),
// which calls OpenMFC's C++ CMFCButton method on OpenMFC's own member layout.
// Symbol: ?OnInitControl@CMFCLinkCtrl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCLinkCtrl__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    S_LinkCtrl* s = L(pThis);

    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));
    alignas(16) unsigned char tagManagerStorage[64];
    std::memset(tagManagerStorage, 0, sizeof tagManagerStorage);
    impl___0CTagManager__QEAA_PEB_W_Z(tagManagerStorage, strDst.GetString());

    CString strURL;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            tagManagerStorage, L"MFCLink_Url", &strURL, FALSE)) {
        impl__SetURL_CMFCLinkCtrl__QEAAXPEB_W_Z(pThis, strURL.GetString());
    }

    CString strURLPrefix;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            tagManagerStorage, L"MFCLink_UrlPrefix", &strURLPrefix, FALSE)) {
        impl__SetURLPrefix_CMFCLinkCtrl__QEAAXPEB_W_Z(pThis, strURLPrefix.GetString());
    }

    CString strFullTextTooltip;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            tagManagerStorage, L"MFCLink_FullTextTooltip", &strFullTextTooltip, FALSE) &&
        !strFullTextTooltip.IsEmpty()) {
        strFullTextTooltip.MakeUpper();
        const BOOL bFullTextTooltip = (std::wcscmp(strFullTextTooltip.GetString(), L"TRUE") == 0) ? TRUE : FALSE;
        s->m_bFullTextTooltip = bFullTextTooltip;
        s->m_bDelayFullTextTooltipSet = bFullTextTooltip;
    }

    CString strTooltip;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            tagManagerStorage, L"MFCLink_Tooltip", &strTooltip, FALSE)) {
        impl__SetTooltip_CMFCButton__QEAAXPEB_W_Z(static_cast<CMFCButton*>(W(pThis)), strTooltip.GetString());
    }

    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}

// Retail (entry RVA 0x789c0, mfc140u), fully transcribed:
//     switch (pMsg->message) {
//     case WM_KEYDOWN:
//         if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN) return TRUE;
//         break;
//     case WM_KEYUP:
//         if (pMsg->wParam == VK_SPACE) return TRUE;
//         if (pMsg->wParam == VK_RETURN) { OnClicked(); return TRUE; }   // direct call 0x78520
//         break;
//     }
//     return CMFCButton::PreTranslateMessage(pMsg);                    // tail jump 0x1e560
// Symbol: ?PreTranslateMessage@CMFCLinkCtrl@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCLinkCtrl__MEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pMsg == nullptr) return FALSE;
    switch (pMsg->message) {
    case WM_KEYDOWN:
        if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN) return TRUE;
        break;
    case WM_KEYUP:
        if (pMsg->wParam == VK_SPACE) return TRUE;
        if (pMsg->wParam == VK_RETURN) {
            impl__OnClicked_CMFCLinkCtrl__IEAAHXZ(pThis);
            return TRUE;
        }
        break;
    default:
        break;
    }
    return impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// Retail (entry RVA 0x78670, mfc140u), fully transcribed -- the inlined
// `m_strURL = lpszURL`:
//     if (lpszURL == NULL) m_strURL.Empty();                        // +0xb30; 0x33b0
//     else m_strURL.SetString(lpszURL, wcslen(lpszURL));            // import 0x2c7748; 0x2e30
// OpenMFC's CString::operator=(const wchar_t*) makes the same NULL/non-NULL split.
// Symbol: ?SetURL@CMFCLinkCtrl@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetURL_CMFCLinkCtrl__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszURL) {
    if (pThis == nullptr) return;
    L(pThis)->m_strURL = lpszURL;
}

// Retail (entry RVA 0x786c0, mfc140u), fully transcribed:
//     ENSURE(lpszPrefix != NULL);                                   // else 0x227720
//     m_strPrefix.SetString(lpszPrefix, wcslen(lpszPrefix));        // +0xb38; tail jump 0x2e30
// Symbol: ?SetURLPrefix@CMFCLinkCtrl@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetURLPrefix_CMFCLinkCtrl__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszPrefix) {
    if (lpszPrefix == nullptr) { ThrowInvalidArg(); return; }
    if (pThis == nullptr) return;
    L(pThis)->m_strPrefix = lpszPrefix;
}

// Retail (entry RVA 0x78700, mfc140u), fully transcribed.  `this` in RCX, the
// hidden CSize return slot in RDX, bVCenter in R8D, bHCenter in R9D:
//     if (m_sizeImage.cx != 0 || m_sizeImage.cy != 0)               // +0x148 / +0x14c
//         return CMFCButton::SizeToContent(FALSE);                  // 0x1e280
//     ENSURE(this != NULL && m_hWnd != NULL);                       // else 0x227720
//     CClientDC dc(this);                                           // 0x2a3b20
//     CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontDefaultGUIUnderline);  // GetGlobalData gate; 0x2a2730
//     ENSURE(pOldFont != NULL);                                     // else 0x227720
//     CString strText;  GetWindowText(strText);                     // 0x28be00
//     CRect rectClient;  ::GetClientRect(m_hWnd, rectClient);       // import 0x2c7330
//     CRect rectText = rectClient;
//     dc.DrawText(strText, strText.GetLength(), rectText, DT_SINGLELINE | DT_CALCRECT);  // 0x420, vslot 28
//     ::InflateRect(rectText, 3, 3);                                // import 0x2c72e8
//     if (!bVCenter && !bHCenter) {
//         SetWindowPos(NULL, -1, -1, rectText.Width(), rectText.Height(),
//                      SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);  // 0x16; 0x2a9a60
//     } else {
//         CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));    // import 0x2c72d8; 0x28ad70
//         ::MapWindowPoints(m_hWnd, pParent ? pParent->m_hWnd : NULL,
//                           (LPPOINT)&rectClient, 2);                // import 0x2c7228
//         int dx = bHCenter ? (rectClient.Width()  - rectText.Width())  / 2 : 0;
//         int dy = bVCenter ? (rectClient.Height() - rectText.Height()) / 2 : 0;
//         SetWindowPos(NULL, rectClient.left + dx, rectClient.top + dy,
//                      rectText.Width(), rectText.Height(), SWP_NOZORDER | SWP_NOACTIVATE);  // 0x14
//     }
//     dc.SelectObject(pOldFont);                                    // 0x2a2730
//     return rectText.Size();                     // ~CString, ~CClientDC 0x2a3be0
// (The halvings are signed divisions: cltd / sub / sar.)
// Deviation: when the second ENSURE fails, the CClientDC is released before
// the exception is raised (retail relies on unwinding to run ~CClientDC).
// Symbol: ?SizeToContent@CMFCLinkCtrl@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCLinkCtrl__UEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int bVCenter, int bHCenter) {
    if (pRet == nullptr) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) { ThrowInvalidArg(); return pRet; }
    S_LinkCtrl* s = L(pThis);

    if (s->m_sizeImage.cx != 0 || s->m_sizeImage.cy != 0) {
        return impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(pThis, pRet, FALSE);
    }
    if (s->m_hWnd == nullptr) { ThrowInvalidArg(); return pRet; }

    alignas(CClientDC) unsigned char dcStorage[sizeof(CClientDC)];
    std::memset(dcStorage, 0, sizeof dcStorage);
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, W(pThis));

    EnsureGlobalDataInitialized();
    CFont* pOldFont = impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, GlobalUnderlineFont());
    if (pOldFont == nullptr) {
        impl___1CClientDC__UEAA_XZ(pDC);
        ThrowInvalidArg();
        return pRet;
    }

    CSize sizeText(0, 0);
    {
        CString strText;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(W(pThis), &strText);

        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(s->m_hWnd, &rectClient);
        RECT rectText = rectClient;
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
            pDC, strText.GetString(), strText.GetLength(), &rectText, DT_SINGLELINE | DT_CALCRECT);
        ::InflateRect(&rectText, 3, 3);

        const int cxText = rectText.right - rectText.left;
        const int cyText = rectText.bottom - rectText.top;
        if (!bVCenter && !bHCenter) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(W(pThis), nullptr, -1, -1, cxText, cyText,
                                                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        } else {
            CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(s->m_hWnd));
            ::MapWindowPoints(s->m_hWnd, pParent != nullptr ? pParent->m_hWnd : nullptr,
                              reinterpret_cast<POINT*>(&rectClient), 2);
            const int dx = bHCenter ? ((rectClient.right - rectClient.left) - cxText) / 2 : 0;
            const int dy = bVCenter ? ((rectClient.bottom - rectClient.top) - cyText) / 2 : 0;
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(W(pThis), nullptr,
                                                       rectClient.left + dx, rectClient.top + dy,
                                                       cxText, cyText, SWP_NOZORDER | SWP_NOACTIVATE);
        }

        impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
        sizeText.cx = cxText;
        sizeText.cy = cyText;
    }
    impl___1CClientDC__UEAA_XZ(pDC);
    *pRet = sizeText;
    return pRet;
}
