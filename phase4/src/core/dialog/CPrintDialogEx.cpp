// CPrintDialogEx — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

#include <cstddef>

// ===========================================================================
// Retail layout of CPrintDialogEx, and why this file carries two of them.
//
// Retail (afxdlgs.h:774 in the 14.51 SDK on this host) declares
//     class CPrintDialogEx : public CCommonDialog,
//                            public IPrintDialogCallback, public IObjectWithSite
// with data members m_pdex (PRINTDLGEX) and m_pUnkSite.  The retail
// constructor ??0CPrintDialogEx@@QEAA@KPEAVCWnd@@@Z, RVA 0x2195c0 (mfc140u),
// pins every offset used below: it stores the IPrintDialogCallback vfptr at
// +0x130 and the IObjectWithSite vfptr at +0x138, zeroes m_pUnkSite at +0x1c8,
// memsets 0x88 bytes at +0x140 (m_pdex) and writes lStructSize = 0x88 there,
// Flags at +0x168 (m_pdex + 0x28) and nStartPage = 0xffffffff at +0x1c0
// (m_pdex + 0x80).
//
// OpenMFC's public header (include/openmfc/afxwin.h) instead declares
// CPrintDialogEx : CDialog with m_pdex directly at +0x130 followed by 24
// padding bytes.  The total size agrees (0x1d0) but m_pdex sits 0x10 bytes
// lower and there are no interface vfptrs.  The C++ member functions defined
// in this file (DoModal, GetCopies, CreatePrinterDC, ...) use that OpenMFC
// layout; the impl__ bodies at the end of the file are transcribed from retail and use the retail layout
// through S_PrintDlgExView, because that is the layout of an object built by a
// client compiled against the real afxdlgs.h.  The two sets therefore disagree
// about where m_pdex lives until the header is brought to the retail shape
// (reported as a header request).
//
// The CDialog members read here (m_pParentWnd +0x110, m_hWndTop +0x118) are
// retail CDialog members.  Their offsets follow from the retail declaration
// order in afxwin.h (m_nIDHelp, m_lpszTemplateName, m_hDialogTemplate,
// m_lpDialogTemplate, m_lpDialogInit, m_pParentWnd, m_hWndTop,
// m_bClosedByEndDialog, m_pOccDialogInfo after a 0xe8-byte CWnd) and agree
// with the retail bodies below (GetDefaults / PreModal read +0x110 and pass
// &+0x118).  OpenMFC's CDialog does not keep them at these offsets (it tracks
// the parent in a side table, detail/DlgcoreSupport.cpp g_dlgParents); on an
// OpenMFC CDialog both land inside CDialog::_dialog_padding, asserted below.
// ===========================================================================
namespace {

struct S_PrintDlgExView {
    unsigned char m_cmdTarget[0x40];              // +0x000 CCmdTarget part of CWnd
    HWND          m_hWnd;                         // +0x040 CWnd::m_hWnd
    unsigned char m_wndRest[0x110 - 0x48];        // +0x048 rest of CWnd, CDialog members up to m_lpDialogInit
    CWnd*         m_pParentWnd;                   // +0x110 CDialog::m_pParentWnd
    HWND          m_hWndTop;                      // +0x118 CDialog::m_hWndTop
    unsigned char m_dlgRest[0x10];                // +0x120 m_bClosedByEndDialog, m_pOccDialogInfo (not used here)
    void*         m_vfptrPrintDialogCallback;     // +0x130 IPrintDialogCallback subobject
    void*         m_vfptrObjectWithSite;          // +0x138 IObjectWithSite subobject
    PRINTDLGEXW   m_pdex;                         // +0x140
    IUnknown*     m_pUnkSite;                     // +0x1c8
};
static_assert(offsetof(S_PrintDlgExView, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");
static_assert(offsetof(S_PrintDlgExView, m_pParentWnd) == 0x110, "GetDefaults/PreModal: mov 0x110(%rbx),%rax");
static_assert(offsetof(S_PrintDlgExView, m_hWndTop) == 0x118, "GetDefaults/PreModal: lea 0x118(%rbx),%rdx");
static_assert(offsetof(S_PrintDlgExView, m_vfptrPrintDialogCallback) == 0x130, "ctor: mov %rax,0x130(%rsi)");
static_assert(offsetof(S_PrintDlgExView, m_vfptrObjectWithSite) == 0x138, "ctor: mov %rax,0x138(%rsi)");
static_assert(offsetof(S_PrintDlgExView, m_pdex) == 0x140, "ctor: lea 0x140(%rsi),%rbx; movl $0x88,(%rbx)");
static_assert(offsetof(S_PrintDlgExView, m_pUnkSite) == 0x1c8, "ctor: movq $0x0,0x1c8(%rsi)");
static_assert(sizeof(S_PrintDlgExView) == 0x1d0, "m_pUnkSite is the last retail member");
static_assert(sizeof(PRINTDLGEXW) == 0x88, "ctor: memset(&m_pdex, 0, 0x88)");
static_assert(offsetof(PRINTDLGEXW, hwndOwner) == 0x8, "GetDefaults: mov %rax,0x148(%rbx)");
static_assert(offsetof(PRINTDLGEXW, Flags) == 0x28, "GetDefaults: mov 0x28(%rdi),%eax / 0x168(%rcx)");
static_assert(sizeof(CPrintDialogEx) == 0x1d0, "OpenMFC CPrintDialogEx is the retail size");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "OpenMFC CWnd::m_hWnd agrees with retail");
static_assert(sizeof(CDialog) == 0x130 && offsetof(CDialog, m_nIDHelp) + sizeof(unsigned int) <= 0x110,
              "retail m_pParentWnd/m_hWndTop fall in OpenMFC CDialog::_dialog_padding");
static_assert(DWLP_MSGRESULT == 0, "HandleMessage: xor %edx,%edx before SetWindowLongPtrW");

// The IPrintDialogCallback subobject sits at +0x130 and IObjectWithSite at
// +0x138 (constructor above); interface methods recover the outer object with
// the same adjustment retail uses (lea -0x130(%rcx) in HandleMessage).
constexpr std::ptrdiff_t kPrintDialogCallbackOffset = 0x130;
constexpr std::ptrdiff_t kObjectWithSiteOffset = 0x138;

inline S_PrintDlgExView* V(void* pThis) { return static_cast<S_PrintDlgExView*>(pThis); }
inline S_PrintDlgExView* FromPrintDialogCallback(void* pItf) {
    return reinterpret_cast<S_PrintDlgExView*>(static_cast<unsigned char*>(pItf) - kPrintDialogCallbackOffset);
}
inline S_PrintDlgExView* FromObjectWithSite(void* pItf) {
    return reinterpret_cast<S_PrintDlgExView*>(static_cast<unsigned char*>(pItf) - kObjectWithSiteOffset);
}

// IID_IPrintDialogCallback {5852A2C3-6530-11D1-B6A3-0000F8757BF9} and
// IID_IObjectWithSite {FC4801A3-2BA9-11CF-A229-00AA003D7352}, as the 16 bytes
// retail QueryInterface compares against (mfc140u .rdata 0x18034c988 and
// 0x1802d9e18, read from mfc140u.dll itself -- the same VAs in mfc140.dll hold
// unrelated bytes).  Retail tests the first dword of IID_IPrintDialogCallback
// as the immediate 0x5852a2c3 and loads the other three from 0x18034c98c..
// 0x18034c994.  Spelled out here so the file does not depend on uuid.lib
// providing either symbol.
const GUID kIID_IPrintDialogCallback =
    {0x5852a2c3, 0x6530, 0x11d1, {0xb6, 0xa3, 0x00, 0x00, 0xf8, 0x75, 0x7b, 0xf9}};
const GUID kIID_IObjectWithSite =
    {0xfc4801a3, 0x2ba9, 0x11cf, {0xa2, 0x29, 0x00, 0xaa, 0x00, 0x3d, 0x73, 0x52}};
// IID_IUnknown {00000000-0000-0000-C000-000000000046}: retail tests the four
// dwords 0, 0, 0xc0, 0x46000000 inline.
const GUID kIID_IUnknown =
    {0x00000000, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

inline bool SameGuid(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }

} // namespace

// ---- sibling impl__ exports called by the bodies below ----------------------
// Each matches a definition that exists today; the defining file is named.
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND hParent, HWND* pWndTop); // core/window/CWnd.cpp
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(int bEnable);                                     // core/app/CWinApp.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                      // core/app/Globals.cpp
extern "C" HWND__* MS_ABI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd* pThis);                                  // core/window/Thunks.cpp
extern "C" int MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd* pThis, HWND__* hWnd);                        // core/window/Thunks.cpp
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(CWnd* pThis, UINT message, WPARAM wParam,
                                                            LPARAM lParam, LRESULT* pResult);                   // core/window/CWnd.cpp

namespace {
// Retail tests AfxGetModuleState()->m_pCurrentWinApp (+0x8 of the retail
// AFX_MODULE_STATE), i.e. afxCurrentWinApp / AfxGetApp().  OpenMFC's
// AFX_MODULE_STATE does not have that shape (detail/RegcoreSupport.h keeps
// m_pCurrentWinApp at +0x10), and OpenMFC's own AfxGetApp() (afxwin.h) is
// static_cast<CWinApp*>(AfxGetThread()); this mirrors that definition through
// the exported thunk.
inline bool HaveCurrentWinApp() { return impl__AfxGetThread__YAPEAVCWinThread__XZ() != nullptr; }
} // namespace

// Symbol: ??0CPrintDialogEx@@QEAA@KPEAVCWnd@@@Z
// CPrintDialogEx::CPrintDialogEx(DWORD dwFlags, CWnd* pParentWnd)
extern "C" void* MS_ABI impl___0CPrintDialogEx__QEAA_KPEAVCWnd___Z(unsigned long dwFlags, CWnd* pParentWnd) {
    return new CPrintDialogEx(dwFlags, pParentWnd);
}
// Symbol: ?GetDeviceName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDeviceName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetDeviceName() : CString());
}
// Symbol: ?GetDriverName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDriverName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetDriverName() : CString());
}
// Symbol: ?GetPortName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetPortName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetPortName() : CString());
}
CPrintDialogEx::CPrintDialogEx(DWORD dwFlags, CWnd* pParentWnd)
    : CDialog() {
    memset(&m_pdex, 0, sizeof(m_pdex));
    m_pdex.lStructSize = sizeof(PRINTDLGEXW);
    m_pdex.hwndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_pdex.Flags = dwFlags;
    m_pdex.nStartPage = START_PAGE_GENERAL;
    memset(_printdialogex_padding, 0, sizeof(_printdialogex_padding));
}
CPrintDialogEx::~CPrintDialogEx() {
    if (m_pdex.hDevMode) ::GlobalFree(m_pdex.hDevMode);
    if (m_pdex.hDevNames) ::GlobalFree(m_pdex.hDevNames);
}
intptr_t CPrintDialogEx::DoModal() {
    if (m_pdex.lStructSize == 0) {
        m_pdex.lStructSize = sizeof(PRINTDLGEXW);
    }

    if (!m_pdex.hwndOwner) {
        CWnd* pMain = AfxGetMainWnd();
        m_pdex.hwndOwner = pMain ? pMain->GetSafeHwnd() : nullptr;
    }

    HRESULT hr = ::PrintDlgExW(&m_pdex);
    if (FAILED(hr)) {
        return -1;
    }

    switch (m_pdex.dwResultAction) {
    case PD_RESULT_PRINT:
        return IDOK;
    case PD_RESULT_CANCEL:
        return IDCANCEL;
    default:
        return -1;
    }
}
int CPrintDialogEx::GetCopies() const { return (int)m_pdex.nCopies; }
int CPrintDialogEx::GetFromPage() const {
    return (m_pdex.nPageRanges > 0 && m_pdex.lpPageRanges) ? (int)m_pdex.lpPageRanges[0].nFromPage : 0;
}
int CPrintDialogEx::GetToPage() const {
    return (m_pdex.nPageRanges > 0 && m_pdex.lpPageRanges) ? (int)m_pdex.lpPageRanges[0].nToPage : 0;
}
CString CPrintDialogEx::GetDeviceName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wDeviceOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}
CString CPrintDialogEx::GetDriverName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wDriverOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}
CString CPrintDialogEx::GetPortName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wOutputOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}
HDC CPrintDialogEx::GetPrinterDC() const { return m_pdex.hDC; }
HDC CPrintDialogEx::CreatePrinterDC() {
    if (m_pdex.hDC) {
        return m_pdex.hDC;
    }
    if (!m_pdex.hDevMode || !m_pdex.hDevNames) {
        return nullptr;
    }

    DEVMODEW* pDevMode = static_cast<DEVMODEW*>(::GlobalLock(m_pdex.hDevMode));
    DEVNAMES* pDevNames = static_cast<DEVNAMES*>(::GlobalLock(m_pdex.hDevNames));
    if (!pDevMode || !pDevNames) {
        if (pDevMode) ::GlobalUnlock(m_pdex.hDevMode);
        if (pDevNames) ::GlobalUnlock(m_pdex.hDevNames);
        return nullptr;
    }

    const wchar_t* pDriver = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDriverOffset;
    const wchar_t* pDevice = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDeviceOffset;
    const wchar_t* pOutput = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wOutputOffset;
    HDC hdc = ::CreateDCW(pDriver, pDevice, pOutput, pDevMode);

    ::GlobalUnlock(m_pdex.hDevNames);
    ::GlobalUnlock(m_pdex.hDevMode);

    m_pdex.hDC = hdc;
    return hdc;
}
int CPrintDialogEx::GetPortrait() const {
    if (m_pdex.hDevMode) {
        DEVMODEW* pDevMode = (DEVMODEW*)::GlobalLock(m_pdex.hDevMode);
        if (pDevMode) {
            int portrait = (pDevMode->dmOrientation == DMORIENT_PORTRAIT);
            ::GlobalUnlock(m_pdex.hDevMode);
            return portrait;
        }
    }
    return 1;
}
LPDEVMODEW CPrintDialogEx::GetDevMode() const {
    if (m_pdex.hDevMode) return (LPDEVMODEW)::GlobalLock(m_pdex.hDevMode);
    return nullptr;
}
void CPrintDialogEx::SetPageRange(int nMinPage, int nMaxPage, BOOL bPageRange) {
    m_pdex.nMinPage = nMinPage;
    m_pdex.nMaxPage = nMaxPage;
    if (bPageRange) m_pdex.Flags |= PD_PAGENUMS;
}
// Symbol: ?AddRef@CPrintDialogEx@@UEAAKXZ
// The export resolves (ordinal 1959 through mfc140u's export address table)
// to RVA 0x3a60 (mfc140u), a body identical-COMDAT-folded with
// InitDone/Release/SelectionChange and other one-liners: `mov $0x1,%eax; ret`.
// CPrintDialogEx is not reference counted; AddRef returns 1.  `this` is the
// IPrintDialogCallback subobject (+0x130) and is not read.
extern "C" unsigned long MS_ABI impl__AddRef_CPrintDialogEx__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// Symbol: ?DefWindowProcW@CPrintDialogEx@@UEAA_JI_K_J@Z
// Resolves (ordinal 3717) to RVA 0x71e0 (mfc140u), a folded `xor %eax,%eax;
// ret`: retail CPrintDialogEx::DefWindowProc returns 0 and calls nothing.
extern "C" __int64 MS_ABI impl__DefWindowProcW_CPrintDialogEx__UEAA_JI_K_J_Z(
    CPrintDialogEx* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)message;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?GetDefaults@CPrintDialogEx@@QEAAHXZ
// Transcribed from RVA 0x219950 (mfc140u), retail layout (S_PrintDlgExView):
//   m_pdex.Flags |= PD_RETURNDEFAULT;                 // bts $0xa, store to +0x168
//   if (m_hWnd == NULL)                               // cmp %rcx(=0),0x40(%rbx)
//       m_pdex.hwndOwner = CWnd::GetSafeOwner_(       // call 0x1ced10, store to +0x148
//           m_pParentWnd->GetSafeHwnd(), &m_hWndTop); // +0x110 ->+0x40, lea 0x118
//   return SUCCEEDED(<PrintDlgEx>(&m_pdex));          // call 0x219dcc; not/shr $0x1f
// Deviation: 0x219dcc (mfc140u, not exported) is a wrapper that resolves
// comdlg32!PrintDlgExW with GetProcAddress (it passes the ANSI name
// "PrintDlgExW" at 0x18034c978) under the module's activation context and
// returns HRESULT_FROM_WIN32(GetLastError()) if that fails.  OpenMFC links
// PrintDlgExW directly, as CPrintDialogEx::DoModal above already does.
extern "C" int MS_ABI impl__GetDefaults_CPrintDialogEx__QEAAHXZ(CPrintDialogEx* pThis) {
    static_assert(PD_RETURNDEFAULT == 0x400, "bts $0xa");
    S_PrintDlgExView* d = V(pThis);
    d->m_pdex.Flags |= PD_RETURNDEFAULT;
    if (d->m_hWnd == nullptr) {
        CWnd* pParent = d->m_pParentWnd;
        d->m_pdex.hwndOwner = impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(
            pParent ? pParent->m_hWnd : nullptr, &d->m_hWndTop);
    }
    return SUCCEEDED(::PrintDlgExW(&d->m_pdex)) ? TRUE : FALSE;
}

// Symbol: ?GetSite@CPrintDialogEx@@UEAAJAEBU_GUID@@PEAPEAX@Z
// Transcribed from RVA 0x219bf0 (mfc140u).  `this` is the IObjectWithSite
// subobject (+0x138); retail reads m_pUnkSite at this+0x90 (= +0x1c8):
//   if (ppvSite == NULL) return E_POINTER;
//   if (m_pUnkSite != NULL) return m_pUnkSite->QueryInterface(riid, ppvSite);  // slot 0, tail jump
//   *ppvSite = NULL; return E_FAIL;
extern "C" long MS_ABI impl__GetSite_CPrintDialogEx__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvSite) {
    if (ppvSite == nullptr) return E_POINTER;
    S_PrintDlgExView* d = FromObjectWithSite(pThis);
    if (d->m_pUnkSite != nullptr) {
        return d->m_pUnkSite->QueryInterface(*riid, ppvSite);
    }
    *ppvSite = nullptr;
    return E_FAIL;
}

// Symbol: ?HandleInitDialog@CPrintDialogEx@@QEAA_J_K_J@Z
// STUB.  Retail RVA 0x219cd0 (mfc140u) was disassembled: it calls the
// virtual at vftable +0x320 on `this` (slot 100, PreInitDialog), then, when
// AfxGetModuleState()->+0xa0 (the COccManager) and m_pOccDialogInfo (+0x128)
// are both non-NULL, COccManager vslot 8 (with m_lpDialogInit +0x108, when
// that is non-NULL) or vslot 9 (with m_lpszTemplateName +0xf0) -- by argument
// shape the two CreateDlgControls overloads of afxocc.h:522/524 -- followed on
// a FALSE return by CDialog::EndDialog(-1) (call 0x208b10) and a 0 return; then the virtual at +0x300 (slot 96,
// OnInitDialog), and if that returned nonzero and m_nFlags (+0xa8) has
// WF_OLECTLCONTAINER (0x100): p = CWnd::FromHandle(::GetNextDlgTabItem(m_hWnd,
// NULL, FALSE)) (call 0x28ad70), and only if p != NULL, p->SetFocus() (call
// 0x2a9b60) and a 0 return; otherwise the OnInitDialog result, sign-extended.
// In the retail CPrintDialogEx vftable (mfc140u 0x1803287c0, stored by the
// constructor) slot 96 is CDialog::OnInitDialog (0x208d10) and slot 100 is
// CDialog::PreInitDialog, folded to a bare `ret` at 0x27d0.  Not implemented:
// slots 96/100 are there to reach a client's OnInitDialog/PreInitDialog
// overrides through its MSVC vftable, and this repo does not dispatch through
// an object's vptr because an object built by OpenMFC's own C++ constructor
// carries a GCC vtable instead (devirtualising to CDialog::OnInitDialog would
// silently skip the override that is the point of this handler); and
// OpenMFC's AFX_MODULE_STATE has no COccManager slot at +0xa0.
extern "C" __int64 MS_ABI impl__HandleInitDialog_CPrintDialogEx__QEAA_J_K_J_Z(
    CPrintDialogEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?HandleMessage@CPrintDialogEx@@UEAAJPEAUHWND__@@I_K_JPEA_J@Z
// Transcribed from RVA 0x219a70 (mfc140u).  `this` is the
// IPrintDialogCallback subobject; retail recovers the dialog with
// lea -0x130(%rcx),%rdi.  Retail order:
//   if (m_hWnd == NULL) CWnd::Attach(hWnd);                     // call 0x28adf0
//   pState = AfxGetThreadState() (inlined: CThreadLocalObject::GetData,
//            call 0x14cf40, on the slot at mfc140u 0x1803c4110;
//            AfxThrowInvalidArgException, call 0x227720, if NULL);
//   pState->m_lastSentMsg = { hWnd, message, wParam, lParam,
//            ::GetMessageTime(), (short)LOWORD/HIWORD(::GetMessagePos()) };
//   BOOL bHandled = OnWndMsg(message, wParam, lParam, plResult); // vftable +0x240 (slot 72)
//   HRESULT hr = bHandled ? S_OK : S_FALSE;
//   if (bHandled && message == WM_NOTIFY)
//       ::SetWindowLongPtr(::GetParent(hWnd), DWLP_MSGRESULT, *plResult);
//   else if (message == WM_INITDIALOG && bHandled && (int)*plResult != 0)
//       hr = S_FALSE;                                            // cmpl $0x0,(%r15)
//   return hr;
// Deviations: (1) the m_lastSentMsg record is omitted -- OpenMFC's
// _AFX_THREAD_STATE (detail/CWinAppSupport.h) is a five-field struct with no
// such member, so the GetMessageTime/GetMessagePos calls that only feed it are
// dropped as well, and so is retail's AfxThrowInvalidArgException on a NULL
// thread state.  (2) The slot-72 virtual OnWndMsg (slot 72 of the retail
// CPrintDialogEx vftable is CWnd::OnWndMsg, RVA 0x28d1b0 mfc140u) is
// devirtualised to the exported CWnd::OnWndMsg thunk, following this repo's
// convention (see impl__OnWndMsg_CWnd in core/window/CWnd.cpp): the object's
// vptr cannot be assumed to be an MSVC-layout vftable -- an object built by
// OpenMFC's own C++ constructor carries a GCC vtable whose slot 72 is not
// OnWndMsg.  In the retail headers OnWndMsg is declared only by CWnd
// (afxwin.h:3010), not by CDialog, CCommonDialog or CPrintDialogEx, so this
// loses only an override in a client-derived class.
extern "C" long MS_ABI impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(
    void* pThis, HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam,
    __int64* plResult) {
    S_PrintDlgExView* d = FromPrintDialogCallback(pThis);
    CWnd* pWnd = reinterpret_cast<CWnd*>(d);
    if (d->m_hWnd == nullptr) {
        impl__Attach_CWnd__QEAAHPEAUHWND_____Z(pWnd, hWnd);
    }
    const int bHandled = impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(pWnd, message, wParam, lParam, plResult);
    long hr = bHandled ? S_OK : S_FALSE;
    if (bHandled && message == WM_NOTIFY) {
        ::SetWindowLongPtrW(::GetParent(hWnd), DWLP_MSGRESULT, static_cast<LONG_PTR>(*plResult));
    } else if (message == WM_INITDIALOG && bHandled && static_cast<int>(*plResult) != 0) {
        hr = S_FALSE;
    }
    return hr;
}

// Symbol: ?InitDone@CPrintDialogEx@@UEAAJXZ
// Resolves (ordinal 7677) to the folded RVA 0x3a60 (mfc140u), `mov $0x1,%eax;
// ret`: returns S_FALSE, letting PrintDlgEx do its default processing.
extern "C" long MS_ABI impl__InitDone_CPrintDialogEx__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_FALSE;
}

// Symbol: ?PostModal@CPrintDialogEx@@QEAAXXZ
// Transcribed from RVA 0x219c70 (mfc140u; the RVA has no entry in the symbol
// map and was found through ordinal 11758 in the export address table),
// retail layout:
//   Detach();                                          // call 0x28ae50 (CWnd::Detach)
//   if (::IsWindow(m_hWndTop)) ::EnableWindow(m_hWndTop, TRUE);   // +0x118
//   m_hWndTop = NULL;
//   if (AfxGetModuleState()->m_pCurrentWinApp != NULL)
//       CWinApp::DoEnableModeless(TRUE);               // call 0x1ce9f0
// The app test goes through HaveCurrentWinApp() (see its comment).
extern "C" void MS_ABI impl__PostModal_CPrintDialogEx__QEAAXXZ(CPrintDialogEx* pThis) {
    S_PrintDlgExView* d = V(pThis);
    impl__Detach_CWnd__QEAAPEAUHWND____XZ(reinterpret_cast<CWnd*>(pThis));
    if (::IsWindow(d->m_hWndTop)) {
        ::EnableWindow(d->m_hWndTop, TRUE);
    }
    d->m_hWndTop = nullptr;
    if (HaveCurrentWinApp()) {
        impl__DoEnableModeless_CWinApp__SAXH_Z(TRUE);
    }
}

// Symbol: ?PreModal@CPrintDialogEx@@QEAAPEAUHWND__@@XZ
// Transcribed from RVA 0x219c20 (mfc140u; ordinal 11818 through the export
// address table), retail layout:
//   if (AfxGetModuleState()->m_pCurrentWinApp != NULL)
//       CWinApp::DoEnableModeless(FALSE);                           // call 0x1ce9f0
//   return CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), &m_hWndTop);  // tail jump 0x1ced10
extern "C" HWND__* MS_ABI impl__PreModal_CPrintDialogEx__QEAAPEAUHWND____XZ(CPrintDialogEx* pThis) {
    S_PrintDlgExView* d = V(pThis);
    if (HaveCurrentWinApp()) {
        impl__DoEnableModeless_CWinApp__SAXH_Z(FALSE);
    }
    CWnd* pParent = d->m_pParentWnd;
    return impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(
        pParent ? pParent->m_hWnd : nullptr, &d->m_hWndTop);
}

// Symbol: ?QueryInterface@CPrintDialogEx@@UEAAJAEBU_GUID@@PEAPEAX@Z
// Transcribed from RVA 0x2199c0 (mfc140u).  `this` is the
// IPrintDialogCallback subobject (+0x130):
//   if (ppvObject == NULL) return E_POINTER;
//   if (riid == IID_IUnknown || riid == IID_IPrintDialogCallback)
//       *ppvObject = static_cast<IPrintDialogCallback*>(pOuter);   // this, or NULL if pOuter is NULL
//   else if (riid == IID_IObjectWithSite)
//       *ppvObject = static_cast<IObjectWithSite*>(pOuter);        // this + 8, or NULL
//   else return E_NOINTERFACE;                                      // *ppvObject left untouched
//   return S_OK;
// The NULL tests are the static_cast null checks retail emits
// (lea -0x130(%r9); neg; sbb; and).  No AddRef is made.
extern "C" long MS_ABI impl__QueryInterface_CPrintDialogEx__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvObject) {
    if (ppvObject == nullptr) return E_POINTER;
    S_PrintDlgExView* pOuter = FromPrintDialogCallback(pThis);
    if (SameGuid(*riid, kIID_IUnknown) || SameGuid(*riid, kIID_IPrintDialogCallback)) {
        *ppvObject = pOuter ? static_cast<void*>(&pOuter->m_vfptrPrintDialogCallback) : nullptr;
    } else if (SameGuid(*riid, kIID_IObjectWithSite)) {
        *ppvObject = pOuter ? static_cast<void*>(&pOuter->m_vfptrObjectWithSite) : nullptr;
    } else {
        return E_NOINTERFACE;
    }
    return S_OK;
}

// Symbol: ?Release@CPrintDialogEx@@UEAAKXZ
// Resolves (ordinal 12238) to the folded RVA 0x3a60 (mfc140u), `mov $0x1,%eax;
// ret`: not reference counted, returns 1 and never deletes.
extern "C" unsigned long MS_ABI impl__Release_CPrintDialogEx__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// Symbol: ?SelectionChange@CPrintDialogEx@@UEAAJXZ
// Resolves (ordinal 12747) to the folded RVA 0x3a60 (mfc140u), `mov $0x1,%eax;
// ret`: returns S_FALSE (default processing).
extern "C" long MS_ABI impl__SelectionChange_CPrintDialogEx__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_FALSE;
}

// Symbol: ?SetSite@CPrintDialogEx@@UEAAJPEAUIUnknown@@@Z
// Transcribed from RVA 0x219b90 (mfc140u; ordinal 13590 through the export
// address table).  `this` is the IObjectWithSite subobject (+0x138);
// m_pUnkSite is this+0x90 (= +0x1c8):
//   if (m_pUnkSite != NULL) m_pUnkSite->Release();     // vftable +0x10
//   m_pUnkSite = pUnkSite;
//   if (pUnkSite != NULL) pUnkSite->AddRef();          // vftable +0x8
//   return S_OK;
extern "C" long MS_ABI impl__SetSite_CPrintDialogEx__UEAAJPEAUIUnknown___Z(void* pThis, IUnknown* pUnkSite) {
    S_PrintDlgExView* d = FromObjectWithSite(pThis);
    if (d->m_pUnkSite != nullptr) {
        d->m_pUnkSite->Release();
    }
    d->m_pUnkSite = pUnkSite;
    if (pUnkSite != nullptr) {
        pUnkSite->AddRef();
    }
    return S_OK;
}
