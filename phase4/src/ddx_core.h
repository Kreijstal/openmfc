// OpenMFC: Dialog Data Exchange / Validation core.  The exported DDX_/DDP_/DDV_ helpers
// were weak stubs, so a real MSVC client's DoDataExchange moved no data.  These faithful
// implementations drive the same Win32 control messages MFC's afxdd_.h does, keyed off
// CDataExchange::m_bSaveAndValidate (save = control->member, load = member->control).
//
// Only the generated thunk file (global_ddx_exchange.cpp) includes this header; it never
// affects any client ABI.
#pragma once
#include "openmfc/afxwin.h"
#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include <cwchar>
#include <cstdlib>

namespace openmfc_ddx {

// CDataExchange, ABI-faithful to MFC (afxdd_.h): no vtable.
struct CDX {
    int   m_bSaveAndValidate;   // @0  BOOL
    // 4 bytes tail pad before the 8-byte pointer
    CWnd* m_pDlgWnd;            // @8
    HWND  m_hWndLastControl;    // @16
    int   m_bEditLastControl;   // @24 BOOL
    // @28 UINT m_idLastControl (unused here)
};

inline bool Saving(void* pDX) { return pDX && static_cast<CDX*>(pDX)->m_bSaveAndValidate; }

inline HWND Ctrl(void* pDX, int nIDC, bool edit = false) {
    CDX* dx = static_cast<CDX*>(pDX);
    HWND hDlg = (dx && dx->m_pDlgWnd) ? dx->m_pDlgWnd->GetSafeHwnd() : nullptr;
    HWND h = hDlg ? ::GetDlgItem(hDlg, nIDC) : nullptr;
    if (dx) { dx->m_hWndLastControl = h; dx->m_bEditLastControl = edit; }
    return h;
}

// Numeric type tags for the templated text exchange.
enum TCode { T_BYTE, T_SHORT, T_INT, T_UINT, T_LONG, T_DWORD, T_FLOAT, T_DOUBLE, T_I64, T_U64 };

inline void ReadNum(const wchar_t* s, void* pv, int code) {
    switch (code) {
    case T_BYTE:  *(unsigned char*)pv = (unsigned char)wcstoul(s, nullptr, 10); break;
    case T_SHORT: *(short*)pv = (short)wcstol(s, nullptr, 10); break;
    case T_INT:   *(int*)pv = (int)wcstol(s, nullptr, 10); break;
    case T_UINT:  *(unsigned*)pv = (unsigned)wcstoul(s, nullptr, 10); break;
    case T_LONG:  *(long*)pv = wcstol(s, nullptr, 10); break;
    case T_DWORD: *(unsigned long*)pv = wcstoul(s, nullptr, 10); break;
    case T_FLOAT: *(float*)pv = (float)wcstod(s, nullptr); break;
    case T_DOUBLE:*(double*)pv = wcstod(s, nullptr); break;
    case T_I64:   *(long long*)pv = wcstoll(s, nullptr, 10); break;
    case T_U64:   *(unsigned long long*)pv = wcstoull(s, nullptr, 10); break;
    }
}

inline void WriteNum(wchar_t* buf, int cap, const void* pv, int code) {
    switch (code) {
    case T_BYTE:  std::swprintf(buf, cap, L"%u", (unsigned)*(const unsigned char*)pv); break;
    case T_SHORT: std::swprintf(buf, cap, L"%d", (int)*(const short*)pv); break;
    case T_INT:   std::swprintf(buf, cap, L"%d", *(const int*)pv); break;
    case T_UINT:  std::swprintf(buf, cap, L"%u", *(const unsigned*)pv); break;
    case T_LONG:  std::swprintf(buf, cap, L"%ld", *(const long*)pv); break;
    case T_DWORD: std::swprintf(buf, cap, L"%lu", *(const unsigned long*)pv); break;
    case T_FLOAT: std::swprintf(buf, cap, L"%.7g", (double)*(const float*)pv); break;
    case T_DOUBLE:std::swprintf(buf, cap, L"%.15g", *(const double*)pv); break;
    case T_I64:   std::swprintf(buf, cap, L"%lld", *(const long long*)pv); break;
    case T_U64:   std::swprintf(buf, cap, L"%llu", *(const unsigned long long*)pv); break;
    }
}

// DDX_Text(pDX, nIDC, numeric&)
inline void TextNum(void* pDX, int nIDC, void* pv, int code) {
    HWND h = Ctrl(pDX, nIDC, true);
    if (!h) return;
    if (Saving(pDX)) {
        wchar_t buf[64] = {0};
        ::GetWindowTextW(h, buf, 64);
        ReadNum(buf, pv, code);
    } else {
        wchar_t buf[64];
        WriteNum(buf, 64, pv, code);
        ::SetWindowTextW(h, buf);
    }
}

// DDX_Text(pDX, nIDC, LPTSTR buffer, int nMaxChars)
inline void TextBuf(void* pDX, int nIDC, wchar_t* buf, int nMax) {
    HWND h = Ctrl(pDX, nIDC, true);
    if (!h || !buf) return;
    if (Saving(pDX)) ::GetWindowTextW(h, buf, nMax);
    else ::SetWindowTextW(h, buf);
}

// DDX_Text(pDX, nIDC, CString&)  -- CString is ABI-compatible with the client's CStringW.
inline void TextStr(void* pDX, int nIDC, CString* pv) {
    HWND h = Ctrl(pDX, nIDC, true);
    if (!h) return;
    if (Saving(pDX)) {
        int n = ::GetWindowTextLengthW(h);
        wchar_t* buf = pv->GetBuffer(n + 1);
        ::GetWindowTextW(h, buf, n + 1);
        pv->ReleaseBuffer(-1);
    } else {
        ::SetWindowTextW(h, pv->GetString());
    }
}

// DDX_Check / DDX_Radio / DDX_Scroll / index-style helpers.
inline void Check(void* pDX, int nIDC, int* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) *pv = (int)::SendMessageW(h, BM_GETCHECK, 0, 0);
    else ::SendMessageW(h, BM_SETCHECK, (WPARAM)(*pv), 0);
}

inline void Radio(void* pDX, int nIDC, int* pv) {
    // nIDC is the first radio in the group; walk the group by WS_GROUP.
    CDX* dx = static_cast<CDX*>(pDX);
    HWND hDlg = (dx && dx->m_pDlgWnd) ? dx->m_pDlgWnd->GetSafeHwnd() : nullptr;
    if (!hDlg) return;
    HWND h = ::GetDlgItem(hDlg, nIDC);
    if (dx) { dx->m_hWndLastControl = h; dx->m_bEditLastControl = false; }
    if (Saving(pDX)) *pv = -1;
    int idx = 0;
    for (HWND w = h; w; w = ::GetWindow(w, GW_HWNDNEXT), ++idx) {
        if (idx > 0 && (::GetWindowLongW(w, GWL_STYLE) & WS_GROUP)) break;
        if (Saving(pDX)) {
            if (::SendMessageW(w, BM_GETCHECK, 0, 0) != 0) { *pv = idx; break; }
        } else {
            ::SendMessageW(w, BM_SETCHECK, (WPARAM)(idx == *pv ? BST_CHECKED : BST_UNCHECKED), 0);
        }
    }
}

inline void ScrollPos(void* pDX, int nIDC, int* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) *pv = ::GetScrollPos(h, SB_CTL);
    else ::SetScrollPos(h, SB_CTL, *pv, TRUE);
}

inline void SliderPos(void* pDX, int nIDC, int* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) *pv = (int)::SendMessageW(h, TBM_GETPOS, 0, 0);
    else ::SendMessageW(h, TBM_SETPOS, TRUE, (LPARAM)*pv);
}

inline void CBIndex(void* pDX, int nIDC, int* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) *pv = (int)::SendMessageW(h, CB_GETCURSEL, 0, 0);
    else ::SendMessageW(h, CB_SETCURSEL, (WPARAM)*pv, 0);
}
inline void LBIndex(void* pDX, int nIDC, int* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) *pv = (int)::SendMessageW(h, LB_GETCURSEL, 0, 0);
    else ::SendMessageW(h, LB_SETCURSEL, (WPARAM)*pv, 0);
}

inline void CBString(void* pDX, int nIDC, CString* pv, bool exact) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) {
        int i = (int)::SendMessageW(h, CB_GETCURSEL, 0, 0);
        if (i < 0) { *pv = L""; return; }
        int len = (int)::SendMessageW(h, CB_GETLBTEXTLEN, i, 0);
        wchar_t* b = pv->GetBuffer(len + 1);
        ::SendMessageW(h, CB_GETLBTEXT, i, (LPARAM)b);
        pv->ReleaseBuffer(-1);
    } else {
        int i = (int)::SendMessageW(h, exact ? CB_FINDSTRINGEXACT : CB_FINDSTRING, (WPARAM)-1, (LPARAM)pv->GetString());
        ::SendMessageW(h, CB_SETCURSEL, (WPARAM)i, 0);
    }
}
inline void LBString(void* pDX, int nIDC, CString* pv, bool exact) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) {
        int i = (int)::SendMessageW(h, LB_GETCURSEL, 0, 0);
        if (i < 0) { *pv = L""; return; }
        int len = (int)::SendMessageW(h, LB_GETTEXTLEN, i, 0);
        wchar_t* b = pv->GetBuffer(len + 1);
        ::SendMessageW(h, LB_GETTEXT, i, (LPARAM)b);
        pv->ReleaseBuffer(-1);
    } else {
        int i = (int)::SendMessageW(h, exact ? LB_FINDSTRINGEXACT : LB_FINDSTRING, (WPARAM)-1, (LPARAM)pv->GetString());
        ::SendMessageW(h, LB_SETCURSEL, (WPARAM)i, 0);
    }
}

inline void IPAddress(void* pDX, int nIDC, unsigned long* pv) {
    HWND h = Ctrl(pDX, nIDC);
    if (!h) return;
    if (Saving(pDX)) ::SendMessageW(h, IPM_GETADDRESS, 0, (LPARAM)pv);
    else ::SendMessageW(h, IPM_SETADDRESS, 0, (LPARAM)*pv);
}

// DDV validation.  MFC aborts DoDataExchange by throwing; that path cannot be replicated
// here because a mingw-thrown C++ exception cannot unwind through the MSVC client's
// DoDataExchange frame (mismatched exception ABIs would crash).  We instead flag the
// failure non-destructively -- beep and move focus to the offending control -- so an
// invalid entry is surfaced to the user without corrupting the process.
inline void FailRange(void* pDX) {
    ::MessageBeep(MB_ICONEXCLAMATION);
    CDX* dx = static_cast<CDX*>(pDX);
    if (dx && dx->m_hWndLastControl) ::SetFocus(dx->m_hWndLastControl);
}

template <class T>
inline void MinMax(void* pDX, T val, T lo, T hi) {
    if (!Saving(pDX)) return;
    if (val < lo || val > hi) FailRange(pDX);
}

inline void MaxChars(void* pDX, const CString* pv, int nChars) {
    if (!Saving(pDX)) return;
    if (pv->GetLength() > nChars) FailRange(pDX);
}

// DDP_ (property-page persistent DDX): behaves like DDX then records the (nIDC, member,
// pszPropName) for OnUpdateValue.  Data-movement is identical to DDX; the property-name
// bookkeeping lives in COlePropertyPage, so faithfully we perform the same control I/O.
inline void PostProcessing(void*) { /* commits recorded properties; no-op without a live
    property set, matching an empty page */ }

} // namespace openmfc_ddx
