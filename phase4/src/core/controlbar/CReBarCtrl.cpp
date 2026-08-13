// CReBarCtrl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 rebar-control wrappers.

#define OPENMFC_APPCORE_IMPL

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
HWND ReBarHwnd(const CWnd* pThis) {
    return pThis ? pThis->GetSafeHwnd() : nullptr;
}
}

// Symbol: ?Create@CReBarCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CReBarCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(REBARCLASSNAMEW, L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?CreateEx@CReBarCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CReBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, REBARCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}

// Symbol: ?GetColorScheme@CReBarCtrl@@QEBAHPEAUtagCOLORSCHEME@@@Z
extern "C" int MS_ABI impl__GetColorScheme_CReBarCtrl__QEBAHPEAUtagCOLORSCHEME___Z(
    const CWnd* pThis, COLORSCHEME* pColorScheme) {
    if (!pColorScheme) return FALSE;
    pColorScheme->dwSize = sizeof(COLORSCHEME);
    return (int)::SendMessageW(ReBarHwnd(pThis), RB_GETCOLORSCHEME, 0, (LPARAM)pColorScheme);
}

// Symbol: ?GetImageList@CReBarCtrl@@QEBAPEAVCImageList@@XZ
extern "C" CImageList* MS_ABI impl__GetImageList_CReBarCtrl__QEBAPEAVCImageList__XZ(
    const CWnd* pThis) {
    HWND hWnd = ReBarHwnd(pThis);
    if (!hWnd) return nullptr;
    REBARINFO rbi = {};
    rbi.cbSize = sizeof(REBARINFO);
    rbi.fMask = RBIM_IMAGELIST;
    if (!::SendMessageW(hWnd, RB_GETBARINFO, 0, (LPARAM)&rbi) || !rbi.himl) return nullptr;
    CImageList* pImageList = new CImageList();
    if (!pImageList) return nullptr;
    pImageList->Attach(rbi.himl);
    return pImageList;
}

// Symbol: ?SetColorScheme@CReBarCtrl@@QEAAXPEBUtagCOLORSCHEME@@@Z
extern "C" void MS_ABI impl__SetColorScheme_CReBarCtrl__QEAAXPEBUtagCOLORSCHEME___Z(
    CWnd* pThis, const COLORSCHEME* pColorScheme) {
    HWND hWnd = ReBarHwnd(pThis);
    if (!hWnd || !pColorScheme) return;
    COLORSCHEME cs = *pColorScheme;
    cs.dwSize = sizeof(COLORSCHEME);
    ::SendMessageW(hWnd, RB_SETCOLORSCHEME, 0, (LPARAM)&cs);
}

// Symbol: ?SetImageList@CReBarCtrl@@QEAAHPEAVCImageList@@@Z
extern "C" int MS_ABI impl__SetImageList_CReBarCtrl__QEAAHPEAVCImageList___Z(
    CWnd* pThis, CImageList* pImageList) {
    HWND hWnd = ReBarHwnd(pThis);
    if (!hWnd) return FALSE;
    REBARINFO rbi = {};
    rbi.cbSize = sizeof(REBARINFO);
    rbi.fMask = RBIM_IMAGELIST;
    rbi.himl = pImageList ? (HIMAGELIST)*pImageList : nullptr;
    return (int)::SendMessageW(hWnd, RB_SETBARINFO, 0, (LPARAM)&rbi);
}

// Symbol: ??1CReBarCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CReBarCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
}
