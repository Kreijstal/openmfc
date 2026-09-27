// CToolTipCtrl — OpenMFC implementation.
// Sources: cbarcore.cpp, ctrl_tooltip.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CToolTipCtrlSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Sibling thunks the retail-transcribed bodies below call.  Each signature is
// derived from its mangled name; the definition site is noted.
// ---------------------------------------------------------------------------
extern "C" int    MS_ABI impl__Lookup_CMapStringToPtr__QEBAHPEB_WAEAPEAX_Z(const CMapStringToPtr* pThis, const wchar_t* key, void*& value);          // core/collections/CMapStringToPtr.cpp
extern "C" void** MS_ABI impl___ACMapStringToPtr__QEAAAEAPEAXPEB_W_Z(CMapStringToPtr* pThis, const wchar_t* key);                                    // core/collections/CMapStringToPtr.cpp
extern "C" int    MS_ABI impl__LookupKey_CMapStringToPtr__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToPtr* pThis, const wchar_t* key, const wchar_t*& actualKey);   // core/collections/CMapStringToPtr.cpp
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);                          // core/window/CWnd.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszName, const wchar_t* lpszType);                // core/runtime/Globals.cpp
extern "C" void*  MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);                                                  // featurepack/CMFC_misc_stubs.cpp
extern "C" int    MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void   MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                                                                   // detail/MfcExceptionsSupport.cpp
extern "C" void   MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                                                                       // detail/MfcExceptionsSupport.cpp
extern "C" void   MS_ABI impl__UpdateTipText_CToolTipCtrl__QEAAXPEB_WPEAVCWnd___K_Z(CToolTipCtrl* pThis, const wchar_t* lpszText, CWnd* pWnd, unsigned __int64 nIDTool);   // core/controls/Thunks.cpp

namespace {

// Layout.  CToolTipCtrl is declared in include/openmfc/afxole.h:139 as CWnd
// (0xe8 bytes, pinned by afxwin.h's static_assert) followed by
// char _tooltip_padding[56].  Retail's ??0CToolTipCtrl@@QEAA@XZ (RVA 0x273810,
// mfc140) constructs `CMapStringToPtr m_mapString` (afxcmn.h, last member of
// the class) at +0xe8: it stores the map vftable there and writes
// +0xf0/+0x100/+0x108/+0x110 = 0, +0xf8 = 17 and the 8-byte +0x118 = 10.
// OnAddTool (below) addresses the map at `lea 0xe8(%rdi)`.
constexpr std::size_t kOffHWnd      = 0x40;   // CWnd::m_hWnd (retail reads 0x40(%rcx) in OnDisableModal)
constexpr std::size_t kOffMapString = 0xe8;   // CToolTipCtrl::m_mapString
static_assert(offsetof(CWnd, m_hWnd) == kOffHWnd, "CWnd::m_hWnd must sit at +0x40");
static_assert(sizeof(CWnd) == kOffMapString, "CToolTipCtrl::m_mapString follows CWnd at +0xe8");
static_assert(offsetof(CToolTipCtrl, _tooltip_padding) == kOffMapString,
              "OpenMFC models m_mapString as _tooltip_padding at +0xe8");

// TOOLINFOW field offsets retail's bodies address (x64): cbSize +0, uFlags +4,
// hwnd +8, uId +0x10, rect +0x18, hinst +0x28, lpszText +0x30, lParam +0x38,
// lpReserved +0x40; sizeof 0x48.  0x40 is TTTOOLINFOW_V2_SIZE.
static_assert(offsetof(TOOLINFOW, rect) == 0x18, "TOOLINFOW::rect");
static_assert(offsetof(TOOLINFOW, hinst) == 0x28, "TOOLINFOW::hinst");
static_assert(offsetof(TOOLINFOW, lpszText) == 0x30, "TOOLINFOW::lpszText");
static_assert(sizeof(TOOLINFOW) == 0x48, "TOOLINFOW size");

inline HWND HWndOf(const void* pWnd) {
    HWND h;
    std::memcpy(&h, static_cast<const char*>(pWnd) + kOffHWnd, sizeof h);
    return h;
}
inline CMapStringToPtr* MapStringOf(void* pThis) {
    return reinterpret_cast<CMapStringToPtr*>(static_cast<char*>(pThis) + kOffMapString);
}

// Transcribed from retail ?FillInToolInfo@CToolTipCtrl@@QEBAXAEAUtagTOOLINFOW@@PEAVCWnd@@_K@Z
// (RVA 0x275670, mfc140u), which AddTool / GetText below call directly:
//     memset(&ti, 0, 0x40);  ti.cbSize = 0x40;          // TTTOOLINFOW_V2_SIZE, NOT sizeof
//     HWND hWnd = pWnd ? pWnd->m_hWnd : NULL;
//     if (nIDTool == 0) { ti.hwnd = ::GetParent(hWnd); ti.uFlags = TTF_IDISHWND; ti.uId = (UINT_PTR)hWnd; }
//     else              { ti.hwnd = hWnd;              ti.uFlags = 0;            ti.uId = nIDTool; }
// This file's exported FillInToolInfo thunk (further below) and the shared
// detail::FillToolInfo helper do NOT behave like this (they use cbSize 0x48,
// TTF_SUBCLASS and never GetParent); the new bodies therefore use this local
// transcription rather than either of them.
void RetailFillInToolInfo(TOOLINFOW& ti, CWnd* pWnd, UINT_PTR nIDTool) {
    std::memset(&ti, 0, 0x40);
    ti.cbSize = 0x40;
    HWND hWnd = pWnd != nullptr ? HWndOf(pWnd) : nullptr;
    if (nIDTool == 0) {
        ti.hwnd = ::GetParent(hWnd);
        ti.uFlags = TTF_IDISHWND;
        ti.uId = reinterpret_cast<UINT_PTR>(hWnd);
    } else {
        ti.hwnd = hWnd;
        ti.uFlags = 0;
        ti.uId = nIDTool;
    }
}

// Transcribed from the unexported retail helper at RVA 0x2ac258 (mfc140) /
// 0x2ae35c (mfc140u), MFC's _AfxIsComboBoxControl by behaviour:
//     if (hWnd == NULL) return FALSE;
//     if ((::GetWindowLong(hWnd, GWL_STYLE) & 0x0F) != nStyle) return FALSE;
//     TCHAR sz[10]; ::GetClassName(hWnd, sz, 10);
//     return ::CompareString(LOCALE_INVARIANT /*0x7f*/, NORM_IGNORECASE /*1*/,
//                            sz, -1, _T("combobox"), -1) == CSTR_EQUAL;
BOOL AfxIsComboBoxControlImpl(HWND hWnd, UINT nStyle) {
    if (hWnd == nullptr) return FALSE;
    if ((static_cast<UINT>(::GetWindowLongW(hWnd, GWL_STYLE)) & 0x0F) != nStyle) return FALSE;
    wchar_t sz[10];
    ::GetClassNameW(hWnd, sz, 10);
    return ::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, sz, -1, L"combobox", -1) == CSTR_EQUAL;
}

// Transcribed from the unexported retail helper at RVA 0x2ac2f0 (mfc140) /
// 0x2ae3f4 (mfc140u), MFC's _AfxChildWindowFromPoint by behaviour.  The same
// transcription exists in core/controlbar/CReBar.cpp (AfxChildWindowFromPointImpl);
// it is repeated here because that one is file-local to CReBar.cpp.
//     ::ClientToScreen(hWnd, &pt);
//     for (HWND h = ::GetWindow(hWnd, GW_CHILD); h; h = ::GetWindow(h, GW_HWNDNEXT))
//         if ((UINT)::GetDlgCtrlID(h) != 0xffff && (::GetWindowLong(h, GWL_STYLE) & WS_VISIBLE)) {
//             RECT r = {0}; ::GetWindowRect(h, &r);
//             if (::PtInRect(&r, pt)) return h;
//         }
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

} // namespace

BOOL CToolTipCtrl::Create(CWnd* pParentWnd, DWORD dwStyle) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(0, TOOLTIPS_CLASSW, nullptr,
                                dwStyle | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                pParentWnd->GetSafeHwnd(), nullptr,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    return m_hWnd != nullptr;
}
BOOL CToolTipCtrl::AddTool(CWnd* pWnd, const wchar_t* pszText, LPCRECT lpRectTool, UINT_PTR nIDTool) {
    if (!m_hWnd || !pWnd) return FALSE;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd->GetSafeHwnd();
    ti.uId = nIDTool ? nIDTool : (UINT_PTR)pWnd->GetSafeHwnd();
    ti.lpszText = (LPWSTR)pszText;
    ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
    if (lpRectTool) ti.rect = *lpRectTool;
    return ::SendMessageW(m_hWnd, TTM_ADDTOOLW, 0, (LPARAM)&ti) ? TRUE : FALSE;
}
void CToolTipCtrl::UpdateTipText(const wchar_t* pszText, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!m_hWnd) return;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = (LPWSTR)pszText;
    ::SendMessageW(m_hWnd, TTM_UPDATETIPTEXTW, 0, (LPARAM)&ti);
}
void CToolTipCtrl::Activate(BOOL bActivate) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_ACTIVATE, (WPARAM)bActivate, 0);
}
void CToolTipCtrl::SetMaxTipWidth(int iWidth) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)iWidth);
}
int CToolTipCtrl::GetText(CWnd* pWnd, UINT_PTR nIDTool, wchar_t* pszText, int cchMax) const {
    if (!m_hWnd) return 0;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = pszText;
    return (int)::SendMessageW(m_hWnd, TTM_GETTEXTW, 0, (LPARAM)&ti);
}
// Symbol: ?CreateEx@CToolTipCtrl@@UEAAHPEAVCWnd@@KK@Z
extern "C" int MS_ABI impl__CreateEx_CToolTipCtrl__UEAAHPEAVCWnd__KK_Z(
    CToolTipCtrl* pThis, CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle) {
    if (!pThis || !pParentWnd) return FALSE;
    pThis->m_hWnd = ::CreateWindowExW(dwExStyle, TOOLTIPS_CLASSW, nullptr,
        dwStyle | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        pParentWnd->GetSafeHwnd(), nullptr, AfxGetInstanceHandle(), nullptr);
    if (pThis->m_hWnd) {
        ::SetWindowPos(pThis->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    return pThis->m_hWnd != nullptr;
}
// Symbol: ?DelTool@CToolTipCtrl@@QEAAXPEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__DelTool_CToolTipCtrl__QEAAXPEAVCWnd___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, UINT_PTR nIDTool) {
    TOOLINFOW info = {};
    FillToolInfo(&info, pWnd, nIDTool);
    ::SendMessageW(ToolTipHwnd(pThis), TTM_DELTOOLW, 0, (LPARAM)&info);
}
// Symbol: ?DestroyToolTipCtrl@CToolTipCtrl@@QEAAHXZ
extern "C" int MS_ABI impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(CToolTipCtrl* pThis) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    HWND hWnd = pThis->m_hWnd;
    pThis->m_hWnd = nullptr;
    return ::DestroyWindow(hWnd);
}
// Symbol: ?FillInToolInfo@CToolTipCtrl@@QEBAXAEAUtagTOOLINFOW@@PEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__FillInToolInfo_CToolTipCtrl__QEBAXAEAUtagTOOLINFOW__PEAVCWnd___K_Z(
    const CToolTipCtrl* pThis, TOOLINFOW* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    (void)pThis;
    if (!pInfo) return;
    *pInfo = {};
    FillToolInfo(pInfo, pWnd, nIDTool);
}
// Symbol: ?GetMessageMap@CToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CToolTipCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolTipCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CToolTipCtrl* pThis) {
    (void)pThis;
    return &g_classCToolTipCtrl;
}
// Symbol: ?GetThisClass@CToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolTipCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_classCToolTipCtrl;
}
// Symbol: ?GetThisMessageMap@CToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolTipCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetToolInfo@CToolTipCtrl@@QEBAHAEAVCToolInfo@@PEAVCWnd@@_K@Z
extern "C" int MS_ABI impl__GetToolInfo_CToolTipCtrl__QEBAHAEAVCToolInfo__PEAVCWnd___K_Z(
    const CToolTipCtrl* pThis, CToolInfo* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!pInfo) return FALSE;
    TOOLINFOW* pToolInfo = reinterpret_cast<TOOLINFOW*>(pInfo);
    *pToolInfo = {};
    FillToolInfo(pToolInfo, pWnd, nIDTool);
    return (int)::SendMessageW(ToolTipHwnd(pThis), TTM_GETTOOLINFOW, 0, (LPARAM)pToolInfo);
}
// Symbol: ?HitTest@CToolTipCtrl@@QEBAHPEAVCWnd@@VCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__HitTest_CToolTipCtrl__QEBAHPEAVCWnd__VCPoint__PEAUtagTOOLINFOW___Z(
    const CToolTipCtrl* pThis, CWnd* pWnd, CPoint pt, TOOLINFOW* pInfo) {
    TTHITTESTINFOW hit = {};
    hit.hwnd = ToolWindow(pWnd);
    hit.pt.x = pt.x;
    hit.pt.y = pt.y;
    if (pInfo) hit.ti = *pInfo;
    hit.ti.cbSize = sizeof(TOOLINFOW);
    int result = (int)::SendMessageW(ToolTipHwnd(pThis), TTM_HITTESTW, 0, (LPARAM)&hit);
    if (pInfo) *pInfo = hit.ti;
    return result;
}
// Transcribed from retail ?OnAddTool@CToolTipCtrl@@IEAA_J_K_J@Z, RVA 0x274e80
// (mfc140u; 0x273a20 in mfc140).  Retail body:
//     TOOLINFO ti;                                   // not initialised
//     if (((TOOLINFO*)lParam)->cbSize != 0)          // inlined memcpy_s(&ti, 0x48, lParam, cbSize):
//         if (cbSize <= 0x48) memcpy(&ti, lParam, cbSize);
//         else { memset(&ti, 0, 0x48); errno = ERANGE; _invalid_parameter_noinfo(); }
//     if (ti.hinst == NULL && ti.lpszText != NULL && ti.lpszText != LPSTR_TEXTCALLBACK) {
//         void* pv;                                  // (`lpszText - 1 > (size_t)-3` rejects 0 and -1)
//         if (!m_mapString.Lookup(ti.lpszText, pv))  // call at 0x274f0b -> 0x2335a0 (mfc140u)
//             m_mapString[ti.lpszText] = NULL;       // call at 0x274f1b -> 0x233600, then `movq $0,(%rax)`
//         LPCTSTR pKey = NULL;                       // zeroed before the call
//         m_mapString.LookupKey(ti.lpszText, pKey);  // call at 0x274f3a -> 0x2335d0; result ignored
//         ti.lpszText = (LPTSTR)pKey;                // stored unconditionally
//     }
//     return this->DefWindowProc(TTM_ADDTOOLW /*0x432*/, wParam, (LPARAM)&ti);   // vtable +0x248, slot 73
// The three map callees are the wide-key exports themselves (mfc140u export
// table): 0x2335a0 = ?Lookup@CMapStringToPtr@@QEBAHPEB_WAEAPEAX@Z,
// 0x233600 = ??ACMapStringToPtr@@QEAAAEAPEAXPEB_W@Z, 0x2335d0 =
// ?LookupKey@CMapStringToPtr@@QEBAHPEB_WAEAPEB_W@Z -- the thunks called below.
// Slot 73 (+0x248) of the CToolTipCtrl vftable 0x180331a30 (mfc140u, stored
// by ??0CToolTipCtrl at 0x274c70) is ?DefWindowProcW@CWnd@@ (0x28bb80).
// Deviations:
//  (1) ti is zero-initialised first, so cbSize == 0 copies nothing into a
//      zeroed struct instead of retail's uninitialised stack.  The copy itself
//      is the CRT's memcpy_s, which is the function retail inlines (same
//      zero-fill / ERANGE / invalid-parameter path when cbSize > 0x48).
//  (2) m_mapString at +0xe8 is not a constructed CMapStringToPtr in OpenMFC
//      (afxole.h models it as _tooltip_padding and ??0CToolTipCtrl does not
//      construct it).  OpenMFC's CMapStringToPtr thunks keep their storage in
//      a side table keyed by the object address and create it lazily, so they
//      work on this address; but nothing ever destroys that entry.  Also,
//      OpenMFC's LookupKey copies the stored key into a per-map scratch
//      CString (CMapStringToPtr.cpp, lookupKeyScratch) and returns that
//      CString's buffer.  The copy is a reference-counted share of the map's
//      own key data (afxstr.h copy/assignment AddRef the same CStringData),
//      so the pointer stays valid while the key remains in the map, like
//      retail's pointer to the map's permanent key.  See the headerRequest
//      filed for CToolTipCtrl::m_mapString.
//  (3) DefWindowProc is a virtual call in retail (slot 73).  It is
//      devirtualised here, as elsewhere in this tree (CReBar.cpp,
//      CToolBar.cpp, CFrameWndEx.cpp): an override of DefWindowProc in a
//      client class derived from CToolTipCtrl is therefore NOT reached.  The
//      body is reproduced inline from OpenMFC's C++ CWnd::DefWindowProcW
//      (core/window/CWnd.cpp:1986 -- CallWindowProc(m_pfnSuper, ...) when a
//      super proc is recorded, else the exported CWnd::DefWindowProc thunk).
//      That member is defined, but it is non-virtual and calling it would
//      add a C++-method reference that the per-file link audit rejects, so
//      its three lines are repeated instead.  The exported thunk alone would
//      send TTM_ADDTOOLW to ::DefWindowProcW and never reach the comctl32
//      tooltip procedure of a subclassed control.  m_pfnSuper is OpenMFC's
//      field at +0x48 (retail's DefWindowProc reads +0xb0).  Retail's further
//      fallback through GetSuperWndProcAddr() (vtable +0x208) is not
//      reproduced.
// Symbol: ?OnAddTool@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnAddTool_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    const TOOLINFOW* pSrc = reinterpret_cast<const TOOLINFOW*>(lParam);
    TOOLINFOW ti = {};                                            // deviation (1)
    if (pSrc->cbSize != 0) {
        memcpy_s(&ti, sizeof(ti), pSrc, pSrc->cbSize);
    }
    if (ti.hinst == nullptr && ti.lpszText != nullptr && ti.lpszText != LPSTR_TEXTCALLBACKW) {
        CMapStringToPtr* pMap = MapStringOf(pThis);               // deviation (2)
        void* pv = nullptr;
        if (!impl__Lookup_CMapStringToPtr__QEBAHPEB_WAEAPEAX_Z(pMap, ti.lpszText, pv)) {
            *impl___ACMapStringToPtr__QEAAAEAPEAXPEB_W_Z(pMap, ti.lpszText) = nullptr;
        }
        const wchar_t* pKey = nullptr;
        impl__LookupKey_CMapStringToPtr__QEBAHPEB_WAEAPEB_W_Z(pMap, ti.lpszText, pKey);
        ti.lpszText = const_cast<LPWSTR>(pKey);
    }
    CWnd* pWnd = pThis;                                            // deviation (3)
    if (pWnd->m_hWnd != nullptr && pWnd->m_pfnSuper != nullptr) {
        return ::CallWindowProcW(pWnd->m_pfnSuper, pWnd->m_hWnd, TTM_ADDTOOLW, wParam,
                                 reinterpret_cast<LPARAM>(&ti));
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pWnd, TTM_ADDTOOLW, wParam,
                                                    reinterpret_cast<LPARAM>(&ti));
}
// Transcribed from retail ?OnDisableModal@CToolTipCtrl@@IEAA_J_K_J@Z, RVA
// 0x274f90 (mfc140u; 0x273b30 in mfc140), complete:
//     ::SendMessage(m_hWnd /*0x40*/, TTM_ACTIVATE /*0x401*/, FALSE, 0);
//     return 0;
// (the IAT slot resolves to SendMessageW in mfc140u.)  No null check on
// m_hWnd, as in retail.
// Symbol: ?OnDisableModal@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnDisableModal_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;
    ::SendMessageW(HWndOf(pThis), TTM_ACTIVATE, FALSE, 0);
    return 0;
}
// Symbol: ?OnEnable@CToolTipCtrl@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CToolTipCtrl__IEAAXH_Z(CToolTipCtrl* pThis, int bEnable) {
    if (pThis) pThis->Activate(bEnable ? TRUE : FALSE);
}
// Transcribed from retail ?OnWindowFromPoint@CToolTipCtrl@@IEAA_J_K_J@Z, RVA
// 0x274fd0 (mfc140u; ordinal 11595, read from the DLL's export table -- the
// symbol is merely absent from mfc140u_rva_symbols.json) / 0x273b70 (mfc140).
// IAT slots resolved in mfc140u: WindowFromPoint, GetParent, ScreenToClient,
// IsWindowEnabled.  Helpers called: 0x2ae35c and 0x2ae3f4 (mfc140u).
// Complete retail body:
//     POINT pt = *(POINT*)lParam;
//     HWND hWnd = ::WindowFromPoint(pt);
//     if (hWnd == NULL) return 0;
//     HWND hWndTemp = ::GetParent(hWnd);
//     if (hWndTemp != NULL && _AfxIsComboBoxControl(hWndTemp, CBS_DROPDOWN /*2*/))
//         return (LRESULT)hWndTemp;
//     ::ScreenToClient(hWnd, &pt);
//     hWndTemp = _AfxChildWindowFromPoint(hWnd, pt);
//     if (hWndTemp != NULL && !::IsWindowEnabled(hWndTemp))   // cmove: only when disabled
//         hWnd = hWndTemp;
//     return (LRESULT)hWnd;
// Symbol: ?OnWindowFromPoint@CToolTipCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnWindowFromPoint_CToolTipCtrl__IEAA_J_K_J_Z(
    CToolTipCtrl* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    POINT pt = *reinterpret_cast<const POINT*>(lParam);
    HWND hWnd = ::WindowFromPoint(pt);
    if (hWnd == nullptr) return 0;
    HWND hWndTemp = ::GetParent(hWnd);
    if (hWndTemp != nullptr && AfxIsComboBoxControlImpl(hWndTemp, CBS_DROPDOWN)) {
        return reinterpret_cast<LRESULT>(hWndTemp);
    }
    ::ScreenToClient(hWnd, &pt);
    hWndTemp = AfxChildWindowFromPointImpl(hWnd, pt);
    if (hWndTemp != nullptr && !::IsWindowEnabled(hWndTemp)) {
        hWnd = hWndTemp;
    }
    return reinterpret_cast<LRESULT>(hWnd);
}
// Symbol: ?SetToolRect@CToolTipCtrl@@QEAAXPEAVCWnd@@_KPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__SetToolRect_CToolTipCtrl__QEAAXPEAVCWnd___KPEBUtagRECT___Z(
    CToolTipCtrl* pThis, CWnd* pWnd, UINT_PTR nIDTool, const RECT* pRect) {
    TOOLINFOW info = {};
    FillToolInfo(&info, pWnd, nIDTool);
    if (pRect) info.rect = *pRect;
    ::SendMessageW(ToolTipHwnd(pThis), TTM_NEWTOOLRECTW, 0, (LPARAM)&info);
}

// === Moved from ManualThunks.cpp ===
// CToolTipCtrl::AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool, UINT_PTR nIDTool).
// Transcribed from retail RVA 0x2750f0 (mfc140u), complete:
//     TOOLINFO ti;  FillInToolInfo(ti, pWnd, nIDTool);          // direct call to 0x275670
//     if (lpRectTool != NULL) memcpy(&ti.rect, lpRectTool, sizeof(RECT));
//     ti.hinst = AfxFindResourceHandle(MAKEINTRESOURCE((WORD)((nIDText >> 4) + 1)), RT_STRING);   // 0x2aeb50
//     ti.lpszText = (LPTSTR)MAKEINTRESOURCE((WORD)nIDText);
//     return (BOOL)::SendMessage(m_hWnd /*0x40*/, TTM_ADDTOOLW /*0x432*/, 0, (LPARAM)&ti);
// Retail checks neither m_hWnd nor pWnd here (FillInToolInfo tolerates NULL pWnd).
// The earlier auto-generated parameter list (void* p0..p3) did not match the
// mangled name; it is replaced with the real one.
// Symbol: ?AddTool@CToolTipCtrl@@QEAAHPEAVCWnd@@IPEBUtagRECT@@_K@Z
extern "C" int MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__IPEBUtagRECT___K_Z(
    CToolTipCtrl* pThis, CWnd* pWnd, UINT nIDText, const RECT* lpRectTool, UINT_PTR nIDTool) {
    TOOLINFOW ti = {};
    RetailFillInToolInfo(ti, pWnd, nIDTool);
    if (lpRectTool != nullptr) {
        std::memcpy(&ti.rect, lpRectTool, sizeof(RECT));
    }
    ti.hinst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
        MAKEINTRESOURCEW(static_cast<WORD>((nIDText >> 4) + 1)), RT_STRING);
    ti.lpszText = MAKEINTRESOURCEW(static_cast<WORD>(nIDText));
    return static_cast<int>(::SendMessageW(HWndOf(pThis), TTM_ADDTOOLW, 0,
                                           reinterpret_cast<LPARAM>(&ti)));
}


// CToolTipCtrl::GetText(CString& str, CWnd* pWnd, UINT_PTR nIDTool) const.
// Transcribed from retail RVA 0x275200 (mfc140u):
//     TOOLINFO ti;  FillInToolInfo(ti, pWnd, nIDTool);          // direct call to 0x275670
//     const TCHAR guard[5] = { 'M', 0, 'F', 'C', 0 };           // 10 bytes read from 0x34ec70 (mfc140u)
//     ti.lpszText = str.GetBuffer(0x405);                       // 1024 + 5 guard chars
//     memset(ti.lpszText, 0, 0x400 * sizeof(TCHAR));
//     memcpy(ti.lpszText + 0x400, guard, sizeof(guard));        // inlined memcpy_s
//     ::SendMessage(m_hWnd /*0x40*/, TTM_GETTEXTW /*0x438*/, 0, (LPARAM)&ti);   // wParam 0, as retail
//     if (memcmp(ti.lpszText + 0x400, guard, sizeof(guard)) != 0)
//         AfxThrowMemoryException();                            // 0x2276c0, noreturn
//     str.ReleaseBuffer();                                      // wcsnlen(p, nAllocLength)
// Deviations: GetBuffer/ReleaseBuffer are OpenMFC's inline CString members
// (afxstr.h); ReleaseBuffer measures with wcslen where retail uses
// wcsnlen(p, nAllocLength) and throws E_INVALIDARG past the allocation -- the
// guard block guarantees a terminator inside the buffer, so the length agrees.
// OpenMFC's GetBuffer also locks the data (nRefs = -1) and its ReleaseBuffer
// sets nRefs = 1, where retail's GetBuffer/ReleaseBuffer leave nRefs alone, so
// a string the caller had already LockBuffer()ed comes back unlocked; and a
// reallocation goes through OpenMFC's string manager rather than the
// string's own pStringMgr.
// Retail's null-destination branch of the inlined memcpy_s (only reachable if
// GetBuffer returned a pointer p with p + 0x400 == NULL) is not reproduced.
// The earlier auto-generated parameter list (void* p0..p4) did not match the
// mangled name; it is replaced with the real one.
// Symbol: ?GetText@CToolTipCtrl@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__GetText_CToolTipCtrl__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCWnd___K_Z(
    const CToolTipCtrl* pThis, CString& str, CWnd* pWnd, UINT_PTR nIDTool) {
    static const wchar_t kGuard[5] = { L'M', L'\0', L'F', L'C', L'\0' };
    static_assert(sizeof(kGuard) == 10, "retail compares 10 guard bytes");
    TOOLINFOW ti = {};
    RetailFillInToolInfo(ti, pWnd, nIDTool);
    wchar_t* pBuf = str.GetBuffer(0x405);
    ti.lpszText = pBuf;
    std::memset(pBuf, 0, 0x400 * sizeof(wchar_t));
    std::memcpy(pBuf + 0x400, kGuard, sizeof(kGuard));
    ::SendMessageW(HWndOf(pThis), TTM_GETTEXTW, 0, reinterpret_cast<LPARAM>(&ti));
    if (std::memcmp(pBuf + 0x400, kGuard, sizeof(kGuard)) != 0) {
        impl__AfxThrowMemoryException__YAXXZ();
        return;
    }
    str.ReleaseBuffer();
}


// CToolTipCtrl::UpdateTipText(UINT nIDText, CWnd* pWnd, UINT_PTR nIDTool).
// Transcribed from retail RVA 0x2755d0 (mfc140u; ordinal 14167, read from the
// DLL's export table -- absent from mfc140u_rva_symbols.json) / 0x274160
// (mfc140), complete:
//     CString str;                                   // from the string manager's nil string
//     HINSTANCE hInst = AfxFindStringResourceHandle(nIDText);   // call 0x2aee00 (mfc140u)
//     if (hInst == NULL || !str.LoadString(hInst, nIDText))     // call 0xdb70, ?LoadStringW@?$CStringT...(HINSTANCE,UINT) (mfc140u)
//         AfxThrowInvalidArgException();             // 0x227720 (mfc140u), i.e. ENSURE(str.LoadString(nIDText))
//     UpdateTipText(str, pWnd, nIDTool);             // direct call: ?UpdateTipText@...PEB_W... 0x275520 (mfc140u)
// The last call goes to the exported LPCWSTR overload's thunk, whose OpenMFC
// body lives in core/controls/Thunks.cpp (it is not a retail transcription).
// The earlier auto-generated parameter list (void* p0..p2) did not match the
// mangled name; it is replaced with the real one.
// Symbol: ?UpdateTipText@CToolTipCtrl@@QEAAXIPEAVCWnd@@_K@Z
extern "C" void MS_ABI impl__UpdateTipText_CToolTipCtrl__QEAAXIPEAVCWnd___K_Z(
    CToolTipCtrl* pThis, UINT nIDText, CWnd* pWnd, UINT_PTR nIDTool) {
    CString str;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nIDText));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nIDText)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__UpdateTipText_CToolTipCtrl__QEAAXPEB_WPEAVCWnd___K_Z(pThis, str.GetString(), pWnd, nIDTool);
}
