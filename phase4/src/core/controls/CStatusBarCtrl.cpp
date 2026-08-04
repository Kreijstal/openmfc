// CStatusBarCtrl — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?GetText@CStatusBarCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEAH@Z
extern "C" void MS_ABI impl__GetText_CStatusBarCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEAH_Z(
    CString* pRet, const CWnd* pThis, int nPane, int* pType) {
    CString text;
    int type = 0;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (hwnd) {
        LRESULT lenAndType = ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0);
        int len = LOWORD(lenAndType);
        type = HIWORD(lenAndType);
        if (len > 0) {
            std::vector<wchar_t> buffer(static_cast<size_t>(len) + 1);
            ::SendMessageW(hwnd, SB_GETTEXTW, nPane, reinterpret_cast<LPARAM>(buffer.data()));
            text = buffer.data();
        }
    }
    if (pType) *pType = type;
    new (pRet) CString(text);
}
// Symbol: ?GetTipText@CStatusBarCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetTipText_CStatusBarCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CWnd* pThis, int nPane) {
    CString text;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (hwnd) {
        constexpr int kMaxTipText = 1024;
        std::vector<wchar_t> buffer(kMaxTipText);
        ::SendMessageW(hwnd, SB_GETTIPTEXTW, MAKEWPARAM(nPane, kMaxTipText), reinterpret_cast<LPARAM>(buffer.data()));
        buffer.back() = L'\0';
        text = buffer.data();
    }
    new (pRet) CString(text);
}
// Symbol: ?Create@CStatusBarCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CStatusBarCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(0, STATUSCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}
// Symbol: ?CreateEx@CStatusBarCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CStatusBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(dwExStyle, STATUSCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}
// Symbol: ?GetBorders@CStatusBarCtrl@@QEBAHAEAH00@Z
extern "C" int MS_ABI impl__GetBorders_CStatusBarCtrl__QEBAHAEAH00_Z(
    const CWnd* pThis, int* pHorz, int* pVert, int* pSpacing) {
    int borders[3] = {};
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    BOOL ok = hwnd ? static_cast<BOOL>(::SendMessageW(hwnd, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders))) : FALSE;
    if (pHorz) *pHorz = borders[0];
    if (pVert) *pVert = borders[1];
    if (pSpacing) *pSpacing = borders[2];
    return ok;
}
// Symbol: ?GetText@CStatusBarCtrl@@QEBAHPEA_WHPEAH@Z
extern "C" int MS_ABI impl__GetText_CStatusBarCtrl__QEBAHPEA_WHPEAH_Z(
    const CWnd* pThis, wchar_t* pBuffer, int nPane, int* pType) {
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (!hwnd) {
        if (pType) *pType = 0;
        if (pBuffer) *pBuffer = L'\0';
        return 0;
    }
    LRESULT lenAndType = ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0);
    if (pType) *pType = HIWORD(lenAndType);
    if (!pBuffer) return LOWORD(lenAndType);
    LRESULT copied = ::SendMessageW(hwnd, SB_GETTEXTW, nPane, reinterpret_cast<LPARAM>(pBuffer));
    return LOWORD(copied);
}
// Symbol: ?GetTextLength@CStatusBarCtrl@@QEBAHHPEAH@Z
extern "C" int MS_ABI impl__GetTextLength_CStatusBarCtrl__QEBAHHPEAH_Z(
    const CWnd* pThis, int nPane, int* pType) {
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    LRESULT lenAndType = hwnd ? ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0) : 0;
    if (pType) *pType = HIWORD(lenAndType);
    return LOWORD(lenAndType);
}
// Symbol: ??1CStatusBarCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CStatusBarCtrl__UEAA_XZ(void* pThis) {
    if (pThis) {
        reinterpret_cast<CStatusBarCtrl*>(pThis)->~CStatusBarCtrl();
    }
}
// Symbol: ?DrawItem@CStatusBarCtrl@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CStatusBarCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CStatusBarCtrl* pThis, DRAWITEMSTRUCT* lpDrawItemStruct) {
    if (pThis) {
        pThis->DrawItem(lpDrawItemStruct);
    }
}
// Symbol: ?OnChildNotify@CStatusBarCtrl@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CStatusBarCtrl__MEAAHI_K_JPEA_J_Z(
    CStatusBarCtrl* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam, __int64* pResult) {
    if (pThis) {
        return (int)pThis->OnChildNotify(message, wParam, lParam, pResult);
    }
    return 0;
}
CStatusBarCtrl::~CStatusBarCtrl() {
}
void CStatusBarCtrl::DrawItem(void* /*lpDrawItemStruct*/) {
    // Default: unhandled (owner-draw hook for derived classes).
}
BOOL CStatusBarCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    return CWnd::OnChildNotify(message, wParam, lParam, pResult);
}
