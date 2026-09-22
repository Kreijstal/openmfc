// CMFCDesktopAlertDialog — OpenMFC implementation.
//
// The dialog hosted inside the desktop-alert ("toast") popup
// CMFCDesktopAlertWnd (featurepack/controls/CMFCDesktopAlertWnd.cpp).  Retail
// declaration: atlmfc/include/afxdesktopalertdialog.h:62 (class) and :33
// (CMFCDesktopAlertWndInfo, the m_Params member).  This file only has to
// reproduce the exported entry points.
//
// Every IMPLEMENTED body below was transcribed from the retail disassembly
// (disas.py), the method described in the header of core/ole/COleControl.cpp.
// All RVAs in this file are mfc140u.dll FUNCTION ENTRIES unless a comment says
// "mfc140" (the ANSI twin that disas.py reads by default; bodies are
// byte-identical, addresses are not).  Where a specific instruction is meant,
// the comment says so.  Entry points without a map entry of their own were
// located through the class's own tables in mfc140u:
//   vftable      0x1802e3dc8 (stored by the constructor at 0x3b5d0)
//     slot  63 (+0x1f8) OnCommand            0x3bad0
//     slot  69 (+0x228) PreTranslateMessage  0x3c680
//     slot  96 (+0x300) OnInitDialog         0x3bbd0
//     slot 101 (+0x328) OnDraw               0x3bd20
//   message map  0x1802e3ca8 (returned by GetMessageMap 0x3b830; base map
//                CDialogEx::GetMessageMap 0x3ef30), read with msgmap_u.py:
//     WM_CTLCOLOR     (0x0019) -> 0x3b840  OnCtlColor
//     WM_ERASEBKGND   (0x0014) -> 0x3a60   OnEraseBkgnd (ICF-folded, see there)
//     WM_PAINT        (0x000f) -> 0x3b8f0  OnPaint
//     WM_LBUTTONDOWN  (0x0201) -> 0x3b990  OnLButtonDown
//     WM_CREATE       (0x0001) -> 0x3bb70  OnCreate
//     WM_SETFOCUS     (0x0007) -> 0x3c650  OnSetFocus
//     WM_PRINTCLIENT  (0x0318) -> 0x3bcf0  OnPrintClient
// Every USER32/GDI32/CRT import slot named below was resolved with iatu.py
// against mfc140u.dll; `call *0x1802c7b30` in the mfc140u listings is the CFG
// dispatcher (an indirect call through RAX), so every "vftable slot N" is the
// vtable byte offset / 8 read off such a call site.
//
// Structural deviations, applied uniformly and repeated at each site:
//
//  (1) vtable pointers.  Retail installs its vftable 0x1802e3dc8 at +0x00 and
//      the CStatic vftable in m_wndIcon / m_wndText after ??0CWnd@@QEAA@XZ.
//      OpenMFC authors no MSVC-layout vftable for this class, so -- exactly as
//      featurepack/customize/CMFCKeyMapDialog.cpp does for its CDialogEx
//      subclass -- +0x00 keeps whatever the CDialogEx constructor thunk leaves
//      there and the plain CStatic members are built with the exported CWnd
//      constructor alone.  Consequently retail's virtual calls on `this`
//      (OnDraw, slot 101) are made to this file's own thunk, and the virtual
//      calls on other objects are made to the exported thunks they resolve to
//      in retail; each site says so.  A client class overriding one of those
//      virtuals is not honoured from inside the DLL.
//
//  (2) CMFCDesktopAlertWnd::OnClickLinkButton (the alert window's vftable
//      slot 97, +0x308) is an inline virtual with no export; the retail base
//      body is the shared `xor %eax,%eax; ret` at 0x71e0.  OpenMFC cannot
//      dispatch through the parent popup's vftable (deviation (1) again), so
//      OnCommand evaluates it as that base body, i.e. FALSE.
//
//  (3) CMFCVisualManager::GetInstance().  Retail calls it out of line, as the
//      non-exported helper at 0x9774, which lazily creates the default
//      manager when ?m_pVisManager@CMFCVisualManager@@ (0x1803be3c0) is NULL.  As in
//      featurepack/docking/CPaneDivider.cpp and CMFCHeaderCtrl.cpp, the
//      exported pointer is read and the visual-manager call is skipped while
//      it is NULL.  The call itself goes to the exported
//      ?OnFillPopupWindowBackground@CMFCVisualManager@@ thunk
//      (featurepack/visualmanager/CMFCVisualManager.cpp), which is the base
//      class's concrete body, so a derived manager's override is not honoured.
//
//  (4) CDC virtuals (SetTextColor slot 14, DrawText slot 28) are called
//      through the exported CDC thunk / the DLL-internal DrawTextW helper
//      (detail/GdicoreSupport.cpp), the convention of CMFCHeaderCtrl.cpp.
//
//  (5) NULL guards.  Retail has none except where a body says so (HasFocus
//      tests `this`); every thunk here that dereferences `this` bails out on
//      a NULL `this` first, and HwndOf() below is NULL-safe where retail
//      dereferences a CWnd* it just got back from CWnd::FromHandle.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"   // S_Cmemdc view + the CMemDC thunk declarations

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <new>

namespace {

// ---------------------------------------------------------------------------
// Retail layout
// ---------------------------------------------------------------------------
// sizeof == 0xec0 (3776): the size operator new is called with in CreateObject
// (0x3b580: `mov $0xec0,%ecx`), and what this tree's CRuntimeClass descriptor
// already records (featurepack/controls/RuntimeClasses.cpp:32).  Every member
// offset was read off the retail constructor (0x3b5d0) and destructor
// (0x3b770), and the member ORDER matches afxdesktopalertdialog.h:75-85
// (m_pParentPopup, m_Params, m_wndIcon, m_wndText, m_btnURL, m_sizeDlg,
// m_bDefault, m_bDontSetFocus, m_bMenuIsActive).  The constructor, in full:
//
//     CDialogEx::CDialogEx();                        // ??0CDialogEx@@QEAA@XZ 0x3ebb0
//     vfptr = 0x1802e3dc8;
//     m_Params.m_strText = m_Params.m_strURL = <nil string>;  // +0x180 / +0x188
//                                                    // (string manager vslot 3 + 0x18)
//     m_Params.m_hIcon = NULL;  m_Params.m_nURLCmdID = 0;     // +0x178 / +0x190
//     m_wndIcon: CWnd::CWnd() (0x28a700) + CStatic vftable     // +0x198
//     m_wndText: CWnd::CWnd() + CStatic vftable                // +0x280
//     m_btnURL:  CMFCLinkCtrl::CMFCLinkCtrl() (0x781b0)        // +0x368
//     m_pParentPopup = NULL;                                   // +0x170
//     8-byte stores of 0 at +0xeac (m_sizeDlg.cy + m_bDefault), 4 at +0xea8
//     (m_sizeDlg.cx) and 8 at +0xeb4 (m_bDontSetFocus + m_bMenuIsActive).
//
// The same offsets are what the sibling CMFCDesktopAlertWnd.cpp's comments
// quote for its m_pWndDlg (+0xea8 dialog size read back by CommonCreate,
// +0xeb0 = 1 written by Create, +0xeb8 written around the popup menu).
//
// CMFCDesktopAlertWndInfo (afxdesktopalertdialog.h:33) -- m_hIcon, m_strText,
// m_strURL, m_nURLCmdID -- is 0x20 bytes; CreateFromParams (0x3c160) reads a
// caller's instance at +0x00 / +0x08 / +0x10 / +0x18 and copies it into
// m_Params at +0x178 / +0x180 / +0x188 / +0x190.
//
// The CString members are spelled as raw 8-byte slots: OpenMFC's CString is
// the ATL-layout `{ wchar_t* m_pszData }` (sizeof asserted below), constructed
// and destroyed in place through StrAt().
struct S_AlertInfo {
    HICON         m_hIcon;              // +0x00
    void*         m_strText;            // +0x08  CString
    void*         m_strURL;             // +0x10  CString
    unsigned int  m_nURLCmdID;          // +0x18
    unsigned char _pad1c[4];            // +0x1c
};
static_assert(sizeof(S_AlertInfo) == 0x20, "CMFCDesktopAlertWndInfo");
static_assert(offsetof(S_AlertInfo, m_hIcon)     == 0x00, "CreateFromParams 0x3c1b9: mov (%rbx),%rax");
static_assert(offsetof(S_AlertInfo, m_strText)   == 0x08, "CreateFromParams 0x3c1c3: lea 0x8(%rbx)");
static_assert(offsetof(S_AlertInfo, m_strURL)    == 0x10, "CreateFromParams 0x3c1d3: lea 0x10(%rbx)");
static_assert(offsetof(S_AlertInfo, m_nURLCmdID) == 0x18, "CreateFromParams 0x3c1e3: mov 0x18(%rbx)");

struct S_AlertDlg {
    alignas(8) unsigned char m_base[0x170];     // 0x000  CDialogEx
    void*         m_pParentPopup;               // 0x170  CMFCDesktopAlertWnd*
    S_AlertInfo   m_Params;                     // 0x178  CMFCDesktopAlertWndInfo
    alignas(8) unsigned char m_wndIcon[0xe8];   // 0x198  CStatic
    alignas(8) unsigned char m_wndText[0xe8];   // 0x280  CStatic
    alignas(8) unsigned char m_btnURL[0xb40];   // 0x368  CMFCLinkCtrl
    SIZE          m_sizeDlg;                    // 0xea8
    int           m_bDefault;                   // 0xeb0
    int           m_bDontSetFocus;              // 0xeb4
    int           m_bMenuIsActive;              // 0xeb8
    unsigned char _padebc[4];                   // 0xebc
};
static_assert(offsetof(S_AlertDlg, m_pParentPopup) == 0x170, "ctor 0x3b689: movq $0x0,0x170(%rdi)");
static_assert(offsetof(S_AlertDlg, m_Params)       == 0x178, "ctor 0x3b63d: movq $0x0,0x178(%rdi)");
static_assert(offsetof(S_AlertDlg, m_Params) + offsetof(S_AlertInfo, m_strText) == 0x180, "ctor 0x3b613: mov %rax,0x180(%rdi)");
static_assert(offsetof(S_AlertDlg, m_Params) + offsetof(S_AlertInfo, m_strURL)  == 0x188, "ctor 0x3b636: mov %rax,0x188(%rdi)");
static_assert(offsetof(S_AlertDlg, m_Params) + offsetof(S_AlertInfo, m_nURLCmdID) == 0x190, "ctor 0x3b648: movl $0x0,0x190(%rdi)");
static_assert(offsetof(S_AlertDlg, m_wndIcon)  == 0x198, "ctor 0x3b652: lea 0x198(%rdi) -> CWnd ctor");
static_assert(offsetof(S_AlertDlg, m_wndText)  == 0x280, "ctor 0x3b66b: lea 0x280(%rdi) -> CWnd ctor");
static_assert(offsetof(S_AlertDlg, m_btnURL)   == 0x368, "ctor 0x3b67d: lea 0x368(%rdi) -> CMFCLinkCtrl ctor");
static_assert(offsetof(S_AlertDlg, m_sizeDlg)  == 0xea8, "GetDlgSize 0x3c63a: mov 0xea8(%rcx),%rax");
static_assert(offsetof(S_AlertDlg, m_bDefault) == 0xeb0, "GetDlgSize 0x3c632: cmp %eax,0xeb0(%rcx)");
static_assert(offsetof(S_AlertDlg, m_bDontSetFocus) == 0xeb4, "OnSetFocus 0x3c650: cmpl $0,0xeb4(%rcx)");
static_assert(offsetof(S_AlertDlg, m_bMenuIsActive) == 0xeb8, "HasFocus 0x3ba11: cmpl $0,0xeb8(%rcx)");
static_assert(sizeof(S_AlertDlg) == 0xec0, "CreateObject 0x3b584: mov $0xec0,%ecx");

// The OpenMFC types the raw blocks above stand in for.
static_assert(sizeof(CDialogEx) == 0x170, "OpenMFC CDialogEx fills exactly the retail base block");
static_assert(sizeof(CStatic) == 0xe8, "plain CStatic members are bare CWnds");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "every retail body reads HWNDs at object+0x40");
static_assert(offsetof(CDC, m_hDC) == 0x08, "DrawIcon reads pDC->m_hDC at +0x08 (OnDraw 0x3be4b)");
static_assert(offsetof(CGdiObject, m_hObject) == 0x08, "afxGlobalData.fontRegular handle at +0x1a8+8");
static_assert(sizeof(CString) == sizeof(void*), "ATL-layout CString is one pointer");

inline S_AlertDlg*       D(void* p)       { return static_cast<S_AlertDlg*>(p); }
inline const S_AlertDlg* D(const void* p) { return static_cast<const S_AlertDlg*>(p); }
inline CString& StrAt(void*& slot) { return *reinterpret_cast<CString*>(&slot); }

// CWnd::m_hWnd, NULL-safe (deviation (5)).
inline HWND HwndOf(const void* pWnd) {
    if (pWnd == nullptr) return nullptr;
    return *reinterpret_cast<HWND const*>(static_cast<unsigned char const*>(pWnd) + 0x40);
}

// ---------------------------------------------------------------------------
// afxGlobalData (?afxGlobalData@@3UAFX_GLOBAL_DATA@@A, 0x1803c1620 in
// mfc140u), read the way every retail body inlines it:
//     if (afxGlobalData.m_bInitialized /*+0*/ == 0) { Initialize(); = 1; }
// Offsets = retail absolute address - 0x1803c1620, cross-checked against the
// pinned AfxGlobalData shadow in core/runtime/AFX_GLOBAL_DATA.cpp:
//   +0x07c clrWindowText           (0x1803c169c, OnCtlColor 0x3b8b3)
//   +0x1a8 fontRegular (CFont)     (0x1803c17c8, GetOptimalTextSize 0x3bfc5)
//   +0x260 m_bIsBlackHighContrast  (0x1803c1880, OnCtlColor 0x3b889)
//   +0x264 m_bIsWhiteHighContrast  (0x1803c1884, OnCtlColor 0x3b880)
//   +0x2a8 m_nTextHeightHorz       (0x1803c18c8, GetOptimalTextSize 0x3bff4)
// Initialize is implemented (core/runtime/AFX_GLOBAL_DATA.cpp:675), so these
// slots are populated after the gate and are read with no fallback.
// ---------------------------------------------------------------------------
constexpr int kGdInitGate             = 0x000;
constexpr int kGdClrWindowText        = 0x07c;
constexpr int kGdFontRegular          = 0x1a8;
constexpr int kGdIsBlackHighContrast  = 0x260;
constexpr int kGdIsWhiteHighContrast  = 0x264;
constexpr int kGdTextHeightHorz       = 0x2a8;

} // namespace

// ---------------------------------------------------------------------------
// Thunks this file calls.  C++ methods of this DLL exist only as impl__
// thunks, so every cross-class call goes through one; the file each is
// defined in is named on its line.  Parameter lists follow the mangled names.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_XZ(CDialogEx* pThis);                                   // detail/DlgcoreSupport.cpp:298
extern "C" void  MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis);                                       // detail/DlgcoreSupport.cpp:196
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                            // detail/DlgcoreSupport.cpp:14
extern "C" int   MS_ABI impl__OnCommand_CDialogEx__MEAAH_K_J_Z(CDialogEx* pThis, WPARAM wParam, LPARAM lParam);            // core/dialog/CDialogEx.cpp:50
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CDialogEx* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor);                                            // core/dialog/CDialogEx.cpp:66
extern "C" int   MS_ABI impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(CDialogEx* pThis, MSG* pMsg);           // core/dialog/CDialogEx.cpp:152

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                             // core/window/CtorDtorPlacement.cpp:22
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                                       // core/controls/RuntimeClasses.cpp:443
extern "C" void* MS_ABI impl___0CMFCLinkCtrl__QEAA_XZ(void* pThis);                                     // featurepack/controls/CMFCLinkCtrl.cpp:7
// CMFCLinkCtrl.cpp:11 is still a generated stub whose parameter list LOST the
// `this` pointer (`void* impl___1CMFCLinkCtrl__UEAA_XZ()`); the declaration
// here is the one the mangled name describes (see headerRequests).
extern "C" void  MS_ABI impl___1CMFCLinkCtrl__UEAA_XZ(void* pThis);

extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                   // core/window/Thunks.cpp:1183
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                        // core/window/CWnd.cpp:602
extern "C" int   MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);                            // core/window/Thunks.cpp:1293
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                        // core/window/Thunks.cpp:1378
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                                 // core/window/Thunks.cpp:1688
extern "C" void  MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);               // core/window/Thunks.cpp:1588
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                     // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect); // core/window/Thunks.cpp:1658
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp:49
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCDesktopAlertWnd__SAPEAUCRuntimeClass__XZ();      // featurepack/controls/RuntimeClasses.cpp:349
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCButton__SAPEAUCRuntimeClass__XZ();               // featurepack/controls/RuntimeClasses.cpp:428
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                // core/app/Globals.cpp:26
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);               // core/app/CWinThread.cpp:127

extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);           // core/gdi/CPaintDC.cpp:12
extern "C" void  MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                                     // core/gdi/CPaintDC.cpp
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd);        // core/gdi/CClientDC.cpp:11
extern "C" void  MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                                   // core/gdi/CClientDC.cpp:26
extern "C" int   MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);                         // core/gdi/CDC.cpp:80
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);    // core/gdi/CDC.cpp:224
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);     // core/gdi/CDC.cpp:444
extern "C" int   MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);             // detail/GdicoreSupport.cpp:140 (tree helper, not an export)

extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);                         // core/runtime/AFX_GLOBAL_DATA.cpp:675
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                                // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                                      // core/runtime/StaticData.cpp:56
extern "C" void  MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    void* pVisManager, CDC* pDC, CRect rect);                                                           // featurepack/visualmanager/CMFCVisualManager.cpp:1618

extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                          // detail/MemcoreSupport.cpp:5

// The sibling class (featurepack/controls/CMFCDesktopAlertWnd.cpp:878 / :932).
extern "C" int   MS_ABI impl__ProcessCommand_CMFCDesktopAlertWnd__QEAAHPEAUHWND_____Z(void* pThis, HWND hwnd);
extern "C" void  MS_ABI impl__StartWindowMove_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis);

// This file's own thunks, called by siblings below.
extern "C" void* MS_ABI impl___0CMFCDesktopAlertDialog__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__OnDraw_CMFCDesktopAlertDialog__MEAAXPEAVCDC___Z(void* pThis, void* pDC);

namespace {

inline void GdEnsureInitialized() {
    int gate;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        gate = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, &gate, sizeof gate);
    }
}
inline int GdInt(int off) {
    int v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}

// `CPoint` passed BY VALUE is an 8-byte aggregate in one register under the
// MS ABI; the thunk signature models it as `long long` (x low, y high).
inline POINT PointFromArg(long long v) {
    const unsigned long long u = static_cast<unsigned long long>(v);
    POINT pt;
    pt.x = static_cast<int>(static_cast<unsigned int>(u & 0xffffffffu));
    pt.y = static_cast<int>(static_cast<unsigned int>(u >> 32));
    return pt;
}

// The child-window test OnDraw (0x3bdb7..0x3be00), OnInitDialog
// (0x3bc2c..0x3bc71) and PreTranslateMessage share: retail does
//     TCHAR szClass[256]; ::GetClassName(hWnd, szClass, 255);   // IAT 0x1802c7280 GetClassNameW
//     CString strClass(szClass);                                // 0xdcb0
//     wcscmp(strClass, L"Static") == 0                          // IAT 0x1802c7770 wcscmp; literal 0x18033ee98
// i.e. a CASE-SENSITIVE compare with "Static".  The CString copy only feeds
// the compare, so the buffer is compared directly here.  The buffer is zeroed
// first (retail leaves it uninitialised if GetClassName fails).
inline bool IsStaticClass(const void* pWnd) {
    wchar_t szClass[256] = {};
    ::GetClassNameW(HwndOf(pWnd), szClass, 255);
    return std::wcscmp(szClass, L"Static") == 0;
}
// ... followed, in OnDraw / OnInitDialog, by (pWnd->GetStyle() & 0x1f) == SS_ICON
// (CWnd::GetStyle, 0x2a9690; `and $0x1f,%eax; cmp $0x3,%al`).
inline bool IsIconStatic(const void* pWnd) {
    return IsStaticClass(pWnd) &&
           (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pWnd)) & 0x1f) == SS_ICON;
}

// Deviation (2): the retail base body of CMFCDesktopAlertWnd::OnClickLinkButton
// (inline in afxdesktopalertwnd.h:190, `xor %eax,%eax; ret` at 0x71e0).
inline int BaseOnClickLinkButton(unsigned int /*uiCmdID*/) { return FALSE; }

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail RVA 0x3b5d0, transcribed in the layout block above.
// DEVIATIONS: the vftable stores are deviation (1).  m_btnURL's storage is
// zeroed before its constructor runs: the CMFCLinkCtrl constructor thunk
// (featurepack/controls/CMFCLinkCtrl.cpp:7) is still a generated
// `return pThis;` stub that initialises nothing, and OnCommand reads
// m_btnURL.m_hWnd (+0x40), which would otherwise be heap garbage.  Zeroing
// cannot disturb a real constructor once that thunk is implemented.
// Symbol: ??0CMFCDesktopAlertDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDesktopAlertDialog__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_AlertDlg* d = D(pThis);
    impl___0CDialogEx__QEAA_XZ(reinterpret_cast<CDialogEx*>(pThis));
    ::new (static_cast<void*>(&d->m_Params.m_strText)) CString();
    ::new (static_cast<void*>(&d->m_Params.m_strURL)) CString();
    d->m_Params.m_hIcon = nullptr;
    d->m_Params.m_nURLCmdID = 0;
    impl___0CWnd__QEAA_XZ(d->m_wndIcon);
    impl___0CWnd__QEAA_XZ(d->m_wndText);
    std::memset(d->m_btnURL, 0, sizeof d->m_btnURL);     // deviation, see above
    impl___0CMFCLinkCtrl__QEAA_XZ(d->m_btnURL);
    d->m_pParentPopup = nullptr;
    d->m_sizeDlg.cx = 0;
    d->m_sizeDlg.cy = 0;
    d->m_bDefault = FALSE;
    d->m_bDontSetFocus = FALSE;
    d->m_bMenuIsActive = FALSE;
    return pThis;
}

// Retail RVA 0x3b770, transcribed:
//     vfptr = 0x1802e3dc8;
//     m_btnURL.~CMFCLinkCtrl();                   // 0x782b0
//     m_wndText.~CStatic();                       // 0x293b30
//     m_wndIcon.~CStatic();
//     m_Params.m_strURL.~CString();               // inline CStringData release
//     m_Params.m_strText.~CString();
//     ~CDialogEx() inline: a vftable store into m_Impl (+0x158), then a
//         vftable store into the CBrush m_brBkgr (+0x140) and a call to the
//         non-exported destructor helper 0x1c6f0 on it;
//     CDialog::~CDialog();                        // tail jump 0x207eb0
// DEVIATIONS: vftable store, deviation (1).  OpenMFC has no ~CDialogEx thunk
// and keeps CDialogEx's background brush in a side table
// (g_dialogExBackgroundState, detail/DlgcoreSupport.cpp) released by
// CDialogEx::OnDestroy, so there is no m_brBkgr to tear down here -- the same
// arrangement featurepack/customize/CMFCKeyMapDialog.cpp's destructor uses.
// Symbol: ??1CMFCDesktopAlertDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDesktopAlertDialog__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_AlertDlg* d = D(pThis);
    impl___1CMFCLinkCtrl__UEAA_XZ(d->m_btnURL);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndText));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndIcon));
    StrAt(d->m_Params.m_strURL).~CString();
    StrAt(d->m_Params.m_strText).~CString();
    impl___1CDialog__UEAA_XZ(reinterpret_cast<CDialog*>(pThis));
}

// Retail RVA 0x3c160 (1232 bytes).  NOT transcribed -- see the `stubbed`
// report.  What it does, in order, for whoever picks this up:
//   * if (!Create(IDD 0x42a2, pParent)) return FALSE;  -- this vftable slot 91
//     (+0x2d8), CDialog::Create(UINT, CWnd*);
//   * m_Params = params (m_hIcon, the two CString operator= at 0xde30,
//     m_nURLCmdID);
//   * scale = afxGlobalData.m_bIsRibbonImageScale (+0x2c0) ?
//     m_dblRibbonImageScale (+0x2b8) : 1.0 (double at 0x18034ff68); margin =
//     10, or (int)(10.0 * scale + 0.5) (doubles at 0x180350028 / 0x18034fef0)
//     when scale > 1.0;
//   * copies of m_strText / m_strURL, each cut to Left(511) (0x128f0) when
//     longer than 512, sized with GetOptimalTextSize (0x3bec0, below);
//   * with an icon: ::GetIconInfo + ::GetObject(hbmColor, sizeof(BITMAP)) for
//     its size (scaled like the margin), both bitmaps deleted, then
//     m_wndIcon.Create(L"", WS_CHILD|SS_NOPREFIX|SS_ICON (0x40000083), rect,
//     this, 0xffff) -- CStatic vftable slot 91 -- and
//     ::SendMessage(m_wndIcon, STM_SETICON 0x170, m_hIcon, 0);
//   * non-empty text: m_wndText.Create(text, WS_CHILD|WS_VISIBLE, rect, this,
//     0xffff) and WM_SETFONT (0x30) with afxGlobalData.fontRegular, TRUE;
//   * non-empty URL: m_btnURL.Create(url, WS_CHILD|WS_VISIBLE, rect, this,
//     m_Params.m_nURLCmdID) through the link control's vftable slot 91, then
//     stores m_btnURL+0xb20 = 1 (8-byte store, so +0xb24 = 0), +0xb28 = 1 and
//     +0xfc (CMFCButton::m_bDrawFocus) = 0;
//   * m_sizeDlg.cx = max(text cx, URL cx) + 2*margin + (icon ? margin +
//     icon width : 0)  (0x3c597..0x3c59f);
//     m_sizeDlg.cy = max(icon ? icon height + 2*margin : 0,
//                        bottom of the last text/URL control + margin,
//                        or just margin when neither was created)
//     (0x3c5a6..0x3c5ad).  m_bDefault is NOT written here.
// It cannot be written honestly while m_btnURL is not a real object: the
// CMFCLinkCtrl constructor and destructor thunks
// (featurepack/controls/CMFCLinkCtrl.cpp:7/:11) are generated stubs, so
// creating a button window on m_btnURL would subclass an HWND onto an
// unconstructed CMFCButton; and the link control's members at +0xb20..+0xb28
// are not modelled anywhere in this tree.  The only retail call site is the
// call at 0x3cd93 inside CMFCDesktopAlertWnd::CommonCreate (entry 0x3cc00,
// mfc140u), which the params overload of Create reaches; CommonCreate is
// itself still a stub in featurepack/controls/CMFCDesktopAlertWnd.cpp.
// Symbol: ?CreateFromParams@CMFCDesktopAlertDialog@@QEAAHAEAVCMFCDesktopAlertWndInfo@@PEAVCMFCDesktopAlertWnd@@@Z
extern "C" int MS_ABI impl__CreateFromParams_CMFCDesktopAlertDialog__QEAAHAEAVCMFCDesktopAlertWndInfo__PEAVCMFCDesktopAlertWnd___Z(
    void* pThis, void* params, void* pParent) {
    (void)pThis; (void)params; (void)pParent;
    return FALSE;
}

// Retail RVA 0x3b580, transcribed:
//     void* p = operator new(0xec0);              // ??2@YAPEAX_K@Z 0x27f0
//     if (p) CMFCDesktopAlertDialog::CMFCDesktopAlertDialog(p);   // 0x3b5d0
//     return p;
// DEVIATION: the block is zeroed before the constructor runs.  Retail's
// constructor overwrites +0x00 with its vftable; OpenMFC's does not
// (deviation (1)), so without this the vfptr slot would be heap garbage
// rather than a deterministic NULL.
// Symbol: ?CreateObject@CMFCDesktopAlertDialog@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCDesktopAlertDialog__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_AlertDlg));
    if (p != nullptr) {
        std::memset(p, 0, sizeof(S_AlertDlg));
        impl___0CMFCDesktopAlertDialog__QEAA_XZ(p);
    }
    return p;
}

// ===========================================================================
// Operations
// ===========================================================================

// Retail RVA 0x3c630, fully transcribed:
//     return m_bDefault ? m_sizeDlg : CSize(0, 0);
// CSize is returned through the hidden pointer in RDX (`mov %rax,(%rdx);
// mov %rdx,%rax`); `this` stays in RCX.
// Symbol: ?GetDlgSize@CMFCDesktopAlertDialog@@QEAA?AVCSize@@XZ
extern "C" SIZE* MS_ABI impl__GetDlgSize_CMFCDesktopAlertDialog__QEAA_AVCSize__XZ(void* pThis, SIZE* pRet) {
    SIZE size = { 0, 0 };
    if (pThis != nullptr && D(pThis)->m_bDefault != 0) size = D(pThis)->m_sizeDlg;
    *pRet = size;
    return pRet;
}

// Retail RVA 0x3bec0, fully transcribed.  `str` is a by-value CString: the
// caller passes a pointer to its temporary in R8 and THIS function releases it
// on every path (the `lock xadd` at 0x3bf14 and 0x3c110 decrements the
// refcount of the CStringData header whose data pointer is loaded from (%r8)).
//     if (str.IsEmpty()) return CSize(0, 0);
//     CRect rectScreen(0,0,0,0), rectWindow(0,0,0,0);
//     ::GetWindowRect(m_hWnd, &rectWindow);                    // IAT 0x1802c6c08
//     MONITORINFO mi; mi.cbSize = 0x28;
//     if (::GetMonitorInfo(::MonitorFromPoint(rectWindow.TopLeft(),
//                                             MONITOR_DEFAULTTONEAREST), &mi))  // 0x1802c6c40 / 0x1802c6c48
//         ::CopyRect(&rectScreen, &mi.rcWork);                 // 0x1802c7218
//     else
//         ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);           // 0x1802c6c50
//     CClientDC dc(this);                                      // 0x2a3b20
//     CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);           // 0x2a2730
//     int nStepY = afxGlobalData.GetTextHeight();  int nStepX = 3 * nStepY;
//     CRect rectText(0, 0, nStepX, nStepY);
//     int nHeight = dc.DrawText(str, str.GetLength(), rectText,
//                               DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX /*0xc10*/);  // CDC vslot 28
//     if (nHeight > nStepY) {
//         <loop>: while the REQUESTED rect (not DrawText's result) still fits
//         the work area -- width <= rectScreen.Width() and height <=
//         rectScreen.Height() -- grow the request by (nStepX, nStepY), re-run
//         the same DrawText, and stop once nHeight <= requested height.
//     }
//     rectText.right = rectText.left + <width of the last DrawText rect> + 5;
//     rectText.bottom = rectText.top + nHeight + 5;
//     dc.SelectObject(pOldFont);
//     return rectText.Size();                                  // ~CClientDC 0x2a3be0
// DEVIATION: DrawText goes to the DrawTextW helper, deviation (4).
// Symbol: ?GetOptimalTextSize@CMFCDesktopAlertDialog@@IEAA?AVCSize@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" SIZE* MS_ABI impl__GetOptimalTextSize_CMFCDesktopAlertDialog__IEAA_AVCSize__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, SIZE* pRet, CString* pStr) {
    if (pStr->IsEmpty() || pThis == nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
        pStr->~CString();
        return pRet;
    }

    RECT rectScreen = { 0, 0, 0, 0 };
    RECT rectWindow = { 0, 0, 0, 0 };
    ::GetWindowRect(HwndOf(pThis), &rectWindow);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    POINT ptTopLeft = { rectWindow.left, rectWindow.top };
    if (::GetMonitorInfoW(::MonitorFromPoint(ptTopLeft, MONITOR_DEFAULTTONEAREST), &mi)) {
        ::CopyRect(&rectScreen, &mi.rcWork);
    } else {
        ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectScreen, 0);
    }

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));

    GdEnsureInitialized();
    CFont* pOldFont = impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(
        pDC, reinterpret_cast<CFont*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdFontRegular));
    GdEnsureInitialized();
    const int nStepY = GdInt(kGdTextHeightHorz);
    const int nStepX = 3 * nStepY;

    const wchar_t* psz = pStr->GetString();
    const int nLen = pStr->GetLength();

    int nReqRight = nStepX;         // retail r13 / r12: the requested extent,
    int nReqBottom = nStepY;        // tested against the work area in the loop
    RECT rectText = { 0, 0, nReqRight, nReqBottom };
    int nHeight = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
        pDC, psz, nLen, &rectText, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
    int nWidth = rectText.right - rectText.left;
    rectText.left = 0;
    rectText.top = 0;

    if (nHeight > nStepY) {
        for (;;) {
            if (nReqRight - rectText.left > rectScreen.right - rectScreen.left) break;
            if (nReqBottom - rectText.top > rectScreen.bottom - rectScreen.top) break;
            nReqRight += nStepX;
            rectText.right = nReqRight;
            nReqBottom += nStepY;
            rectText.bottom = nReqBottom;
            nHeight = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
                pDC, psz, nLen, &rectText, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
            nWidth = rectText.right - rectText.left;
            rectText.left = 0;
            rectText.top = 0;
            if (nHeight <= nReqBottom - rectText.top) break;
        }
    }

    rectText.bottom = rectText.top + nHeight + 5;
    rectText.right = rectText.left + nWidth + 5;

    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    pRet->cx = rectText.right - rectText.left;
    pRet->cy = rectText.bottom - rectText.top;
    impl___1CClientDC__UEAA_XZ(pDC);
    pStr->~CString();
    return pRet;
}

// Retail RVA 0x3b9f0, fully transcribed:
//     if (this == NULL || m_hWnd == NULL) return FALSE;
//     if (m_bMenuIsActive) return TRUE;
//     CWnd* pMainWnd = AfxGetMainWnd();    // inline: AfxGetModuleThreadState()
//                                          // (0x133a20)->m_pCurrentWinThread (+8),
//                                          // then its vftable slot 31 (+0xf8)
//     if (pMainWnd == NULL || pMainWnd->m_hWnd == NULL) return FALSE;
//     if (::IsIconic(pMainWnd->m_hWnd)) return FALSE;                 // IAT 0x1802c6c28
//     if (!::IsWindowVisible(pMainWnd->m_hWnd)) return FALSE;         // 0x1802c7328
//     if (CWnd::FromHandle(::GetForegroundWindow()) != pMainWnd)      // 0x1802c6c30
//         return FALSE;
//     CWnd* pFocus = CWnd::FromHandle(::GetFocus());                  // 0x1802c71b0
//     if (pFocus == NULL || pFocus->m_hWnd == NULL) return FALSE;
//     if (::IsChild(m_hWnd, pFocus->m_hWnd)) return TRUE;             // 0x1802c71b8
//     return pFocus->m_hWnd == m_hWnd;
// DEVIATION: the thread lookup and GetMainWnd are made through the exported
// AfxGetThread / CWinThread::GetMainWnd thunks (devirtualised, deviation (1)),
// as featurepack/customize/CMFCKeyMapDialog.cpp does.
// Symbol: ?HasFocus@CMFCDesktopAlertDialog@@QEBAHXZ
extern "C" int MS_ABI impl__HasFocus_CMFCDesktopAlertDialog__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr || HwndOf(pThis) == nullptr) return FALSE;
    if (D(pThis)->m_bMenuIsActive != 0) return TRUE;

    CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
    if (pThread == nullptr) return FALSE;
    CWnd* pMainWnd = impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread);
    if (pMainWnd == nullptr || HwndOf(pMainWnd) == nullptr) return FALSE;
    if (::IsIconic(HwndOf(pMainWnd))) return FALSE;
    if (!::IsWindowVisible(HwndOf(pMainWnd))) return FALSE;
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetForegroundWindow()) != pMainWnd) return FALSE;

    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    if (pFocus == nullptr || HwndOf(pFocus) == nullptr) return FALSE;
    if (::IsChild(HwndOf(pThis), HwndOf(pFocus))) return TRUE;
    return HwndOf(pFocus) == HwndOf(pThis) ? TRUE : FALSE;
}

// ===========================================================================
// Overrides and message handlers
// ===========================================================================

// Retail RVA 0x3bad0, fully transcribed:
//     if (m_pParentPopup->ProcessCommand((HWND)lParam))   // 0x3d8a0, no NULL check
//         return TRUE;
//     if (m_btnURL.m_hWnd == (HWND)lParam &&
//         m_btnURL.GetDlgCtrlID() == LOWORD(wParam) &&    // 0x2a99a0; movzwl %r15w
//         m_pParentPopup->OnClickLinkButton(m_btnURL.GetDlgCtrlID()))   // alert-wnd vslot 97
//         return TRUE;
//     return CDialogEx::OnCommand(wParam, lParam);        // 0x3f400
// DEVIATIONS: OnClickLinkButton is evaluated as its base body, deviation (2),
// so the second `return TRUE` is never taken.  A NULL m_pParentPopup, which
// retail would dereference, reaches the sibling's ProcessCommand thunk, which
// returns FALSE for a NULL `this`.
// Symbol: ?OnCommand@CMFCDesktopAlertDialog@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCDesktopAlertDialog__MEAAH_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return FALSE;
    S_AlertDlg* d = D(pThis);
    const HWND hwndCtl = reinterpret_cast<HWND>(static_cast<LPARAM>(lParam));
    if (impl__ProcessCommand_CMFCDesktopAlertWnd__QEAAHPEAUHWND_____Z(d->m_pParentPopup, hwndCtl) != 0) {
        return TRUE;
    }
    const CWnd* pBtn = reinterpret_cast<const CWnd*>(d->m_btnURL);
    if (HwndOf(pBtn) == hwndCtl &&
        impl__GetDlgCtrlID_CWnd__QEBAHXZ(pBtn) == static_cast<int>(LOWORD(wParam)) &&
        BaseOnClickLinkButton(static_cast<unsigned int>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(pBtn)))) {
        return TRUE;
    }
    return impl__OnCommand_CDialogEx__MEAAH_K_J_Z(reinterpret_cast<CDialogEx*>(pThis),
                                                  static_cast<WPARAM>(wParam),
                                                  static_cast<LPARAM>(lParam));
}

// Retail RVA 0x3bb70, fully transcribed:
//     if (CWnd::OnCreate(lpCreateStruct) == -1)       // inline: CWnd::Default() 0x28ac80
//         return -1;
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));      // IAT 0x1802c72d8
//     m_pParentPopup = (pParent != NULL &&
//         pParent->IsKindOf(RUNTIME_CLASS(CMFCDesktopAlertWnd)))  // 0x234cf0; 0x1802e4128
//         ? (CMFCDesktopAlertWnd*)pParent : NULL;
//     return 0;
// Symbol: ?OnCreate@CMFCDesktopAlertDialog@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCDesktopAlertDialog__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (pThis == nullptr) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis))) == -1) return -1;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HwndOf(pThis)));
    if (pParent != nullptr &&
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParent, impl__GetThisClass_CMFCDesktopAlertWnd__SAPEAUCRuntimeClass__XZ())) {
        pParent = nullptr;
    }
    D(pThis)->m_pParentPopup = pParent;
    return 0;
}

// Retail RVA 0x3b840, fully transcribed:
//     if (nCtlColor != CTLCOLOR_STATIC /*6*/)
//         return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);   // 0x3f230
//     pDC->SetBkMode(TRANSPARENT);                             // 0x2a2860 (`lea -0x5(%r9)` == 1)
//     if (afxGlobalData.IsHighContrastMode())      // +0x264 white, then +0x260 black
//         pDC->SetTextColor(afxGlobalData.clrWindowText);      // CDC vslot 14 (+0x70); +0x7c
//     return (HBRUSH)::GetStockObject(NULL_BRUSH /*5*/);       // IAT 0x1802c6250
// The afxGlobalData init gate is inlined twice -- once before the two
// high-contrast reads and again before the clrWindowText read -- as here.
// DEVIATION: SetTextColor through the exported CDC thunk, deviation (4).
// Symbol: ?OnCtlColor@CMFCDesktopAlertDialog@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCDesktopAlertDialog__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    void* pThis, CDC* pDC, CWnd* pWnd, unsigned int nCtlColor) {
    if (pThis == nullptr) return nullptr;
    if (nCtlColor != CTLCOLOR_STATIC) {
        return impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
            reinterpret_cast<CDialogEx*>(pThis), pDC, pWnd, nCtlColor);
    }
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    GdEnsureInitialized();
    if (GdInt(kGdIsWhiteHighContrast) != 0 || GdInt(kGdIsBlackHighContrast) != 0) {
        GdEnsureInitialized();
        impl__SetTextColor_CDC__UEAAKK_Z(pDC, static_cast<unsigned long>(GdInt(kGdClrWindowText)));
    }
    return static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
}

// Retail RVA 0x3bd20 (vftable slot 101), fully transcribed:
//     CRect rectClient(0,0,0,0);
//     ::GetClientRect(m_hWnd, &rectClient);                    // IAT 0x1802c7330
//     CMFCVisualManager::GetInstance()                         // 0x9774, deviation (3)
//         ->OnFillPopupWindowBackground(pDC, rectClient);      // VM vslot 121 (+0x3c8), CRect by value
//     for (CWnd* pWndChild = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));   // IAT 0x1802c6c10
//          pWndChild != NULL;
//          pWndChild = CWnd::FromHandle(::GetWindow(pWndChild->m_hWnd, GW_HWNDNEXT))) {
//         if (<class is "Static"> && (pWndChild->GetStyle() & 0x1f) == SS_ICON) {
//             CRect rectIcon(0,0,0,0);
//             ::GetWindowRect(pWndChild->m_hWnd, &rectIcon);  // 0x1802c6c08
//             ScreenToClient(&rectIcon);                       // 0x2a32b0
//             HICON hIcon = (HICON)::SendMessage(pWndChild->m_hWnd, STM_GETICON /*0x171*/, 0, 0);  // 0x1802c7120
//             ::DrawIcon(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon);    // 0x1802c6c38
//         }
//     }
// Symbol: ?OnDraw@CMFCDesktopAlertDialog@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCDesktopAlertDialog__MEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    if (pThis == nullptr) return;
    CDC* pCDC = static_cast<CDC*>(pDC);
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(HwndOf(pThis), &rectClient);
    void* pVisManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pVisManager != nullptr) {   // deviation (3): retail creates the default manager here
        impl__OnFillPopupWindowBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
            pVisManager, pCDC, CRect(rectClient));
    }
    for (CWnd* pWndChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(HwndOf(pThis), GW_CHILD));
         pWndChild != nullptr;
         pWndChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(HwndOf(pWndChild), GW_HWNDNEXT))) {
        if (!IsIconStatic(pWndChild)) continue;
        RECT rectIcon = { 0, 0, 0, 0 };
        ::GetWindowRect(HwndOf(pWndChild), &rectIcon);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rectIcon);
        HICON hIcon = reinterpret_cast<HICON>(::SendMessage(HwndOf(pWndChild), STM_GETICON, 0, 0));
        ::DrawIcon(pCDC != nullptr ? pCDC->m_hDC : nullptr, rectIcon.left, rectIcon.top, hIcon);
    }
}

// Retail: the WM_ERASEBKGND entry of the message map 0x1802e3ca8 points at
// RVA 0x3a60, whose whole body is `mov $0x1,%eax; ret` -- the handler returns
// TRUE and erases nothing (OnDraw paints the background).  0x3a60 carries the
// name ?accDoDefaultAction@CMFCBaseAccessibleObject@@... in the symbol map: the
// linker folded the identical bodies, which is why this export has no RVA of
// its own in the map.
// Symbol: ?OnEraseBkgnd@CMFCDesktopAlertDialog@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCDesktopAlertDialog__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Retail RVA 0x3bbd0 (vftable slot 96), fully transcribed:
//     CDialog::OnInitDialog();                     // 0x208d10, result discarded
//     for (CWnd* pWndChild = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));
//          pWndChild != NULL;
//          pWndChild = CWnd::FromHandle(::GetWindow(pWndChild->m_hWnd, GW_HWNDNEXT))) {
//         if (pWndChild->IsKindOf(RUNTIME_CLASS(CMFCButton)))  // 0x234cf0; 0x1802df498
//             ((CMFCButton*)pWndChild)->m_bDrawFocus = FALSE;   // movl $0,0xfc(%rbx)
//         else if (<class is "Static"> && (pWndChild->GetStyle() & 0x1f) == SS_ICON)
//             pWndChild->ShowWindow(SW_HIDE);                    // 0x2a9ad0, edx = 0
//     }
//     return TRUE;
// +0xfc is CMFCButton::m_bDrawFocus by the member order of afxbutton.h:56-62
// (m_nFlatStyle at +0xe8, right after the 0xe8-byte CButton, then
// m_nAlignStyle, m_bRightImage, m_bTopImage, m_bTransparent, m_bDrawFocus);
// detail/CMFCButtonSupport.h:83 places m_bDrawFocus at +0xfc as well.
// The icon statics are hidden because OnDraw paints their icons itself.
// DEVIATION: CDialog::OnInitDialog is called through its exported thunk
// rather than the vftable, deviation (1).
// Symbol: ?OnInitDialog@CMFCDesktopAlertDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCDesktopAlertDialog__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    impl__OnInitDialog_CDialog__UEAAHXZ(reinterpret_cast<CDialog*>(pThis));
    const CRuntimeClass* pButtonClass = impl__GetThisClass_CMFCButton__SAPEAUCRuntimeClass__XZ();
    for (CWnd* pWndChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(HwndOf(pThis), GW_CHILD));
         pWndChild != nullptr;
         pWndChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(HwndOf(pWndChild), GW_HWNDNEXT))) {
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWndChild, pButtonClass)) {
            *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(pWndChild) + 0xfc) = FALSE;
        } else if (IsIconStatic(pWndChild)) {
            impl__ShowWindow_CWnd__QEAAHH_Z(pWndChild, SW_HIDE);
        }
    }
    return TRUE;
}

// Retail RVA 0x3b990, fully transcribed:
//     CWnd::Default();                                        // 0x28ac80 (the base OnLButtonDown)
//     CWnd::FromHandle(::GetParent(m_hWnd))->SendMessage(     // IAT 0x1802c72d8 / 0x1802c7120
//         WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));   // wParam is 0, NOT nFlags
//     SetFocus();                                             // tail jump 0x2a9b60
// DEVIATION: retail dereferences the FromHandle result unchecked; HwndOf() is
// NULL-safe, so a missing parent turns the SendMessage into a no-op.
// Symbol: ?OnLButtonDown@CMFCDesktopAlertDialog@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCDesktopAlertDialog__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    const POINT pt = PointFromArg(point);
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HwndOf(pThis)));
    ::SendMessage(HwndOf(pParent), WM_LBUTTONDOWN, 0, MAKELPARAM(pt.x, pt.y));
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
}

// Retail RVA 0x3b8f0, fully transcribed:
//     CPaintDC dc(this);                          // 0x2a3d20
//     CMemDC memDC(dc, this);                     // ??0CMemDC@@QEAA@AEAVCDC@@PEAVCWnd@@@Z 0x69f50
//     OnDraw(&memDC.GetDC());                     // inline GetDC: m_bMemDC ? m_dcMem : m_dc
//                                                 // (`cmpl $0,0x30(%rsp); cmove`); vftable slot 101
//     memDC.~CMemDC();  dc.~CPaintDC();           // 0x6a380 / 0x2a3dd0
// The CMemDC is the S_Cmemdc view driven through its thunks, as
// featurepack/docking/CPaneDivider.cpp::OnPaint does.
// DEVIATION: OnDraw is reached non-virtually, deviation (1).
// Symbol: ?OnPaint@CMFCDesktopAlertDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCDesktopAlertDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, static_cast<CWnd*>(pThis));
    alignas(void*) unsigned char memStorage[sizeof(openmfc::detail::cmemdc::S_Cmemdc)] = {};
    openmfc::detail::cmemdc::S_Cmemdc* pMemDC =
        reinterpret_cast<openmfc::detail::cmemdc::S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, pThis);
    void* pDC = pMemDC->m_bMemDC ? static_cast<void*>(&pMemDC->m_dcMem) : pMemDC->m_dc;
    impl__OnDraw_CMFCDesktopAlertDialog__MEAAXPEAVCDC___Z(pThis, pDC);
    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}

// Retail RVA 0x3bcf0, fully transcribed:
//     if (nFlags & PRF_CLIENT /*4*/) OnDraw(pDC);   // vftable slot 101 (+0x328)
//     return 0;
// DEVIATION: OnDraw non-virtual, deviation (1).
// Symbol: ?OnPrintClient@CMFCDesktopAlertDialog@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CMFCDesktopAlertDialog__IEAA_JPEAVCDC__I_Z(
    void* pThis, void* pDC, unsigned int nFlags) {
    if (pThis != nullptr && (nFlags & PRF_CLIENT) != 0) {
        impl__OnDraw_CMFCDesktopAlertDialog__MEAAXPEAVCDC___Z(pThis, pDC);
    }
    return 0;
}

// Retail RVA 0x3c650, fully transcribed:
//     if (m_bDontSetFocus && pOldWnd != NULL && pOldWnd->m_hWnd != NULL)
//         pOldWnd->SetFocus();                      // tail jump 0x2a9b60
//     else
//         CWnd::OnSetFocus(pOldWnd);                // tail jump 0x28f2a0
// Symbol: ?OnSetFocus@CMFCDesktopAlertDialog@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCDesktopAlertDialog__IEAAXPEAVCWnd___Z(void* pThis, void* pOldWnd) {
    if (pThis == nullptr) return;
    if (D(pThis)->m_bDontSetFocus != 0 && pOldWnd != nullptr && HwndOf(pOldWnd) != nullptr) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pOldWnd));
        return;
    }
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(static_cast<CWnd*>(pThis), pOldWnd);
}

// Retail RVA 0x3c680 (vftable slot 69), fully transcribed:
//     if (pMsg->message == WM_LBUTTONDOWN &&
//         m_pParentPopup != NULL && m_pParentPopup->m_hWnd != NULL) {
//         CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);          // 0x28ad70
//         if (pWnd != NULL && (<class is "Static"> ||          // no SS_ICON test here
//                              pWnd->m_hWnd == m_hWnd))
//             m_pParentPopup->StartWindowMove();              // 0x3e880
//     }
//     return CDialogEx::PreTranslateMessage(pMsg);            // 0x3f3b0; always reached
// So a click on the dialog's own background or on any static drags the popup.
// Symbol: ?PreTranslateMessage@CMFCDesktopAlertDialog@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCDesktopAlertDialog__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    S_AlertDlg* d = D(pThis);
    if (pMsg->message == WM_LBUTTONDOWN &&
        d->m_pParentPopup != nullptr && HwndOf(d->m_pParentPopup) != nullptr) {
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pMsg->hwnd);
        if (pWnd != nullptr && (IsStaticClass(pWnd) || HwndOf(pWnd) == HwndOf(pThis))) {
            impl__StartWindowMove_CMFCDesktopAlertWnd__IEAAXXZ(d->m_pParentPopup);
        }
    }
    return impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(reinterpret_cast<CDialogEx*>(pThis), pMsg);
}
