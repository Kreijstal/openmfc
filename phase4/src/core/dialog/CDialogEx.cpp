// CDialogEx — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
    CDialog* pThis, MSG* pMsg);

// Symbol: ?CommonConstruct@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__CommonConstruct_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (!pThis) return;
    CDialogExAccess* access = DialogExAccess(pThis);
    access->m_clrBackground = GetSysColor(COLOR_BTNFACE);
    access->m_hBackgroundImage = nullptr;
    g_dialogExBackgroundState[pThis] = CDialogExBackgroundState();
}
// Symbol: ?GetMessageMap@CDialogEx@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialogEx__MEBAPEBUAFX_MSGMAP__XZ(
    const CDialogEx* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CDialogEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialogEx__UEBAPEAUCRuntimeClass__XZ(
    const CDialogEx* pThis) {
    (void)pThis;
    return CDialogEx::GetThisClass();
}
// Symbol: ?GetThisClass@CDialogEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialogEx__SAPEAUCRuntimeClass__XZ() {
    return CDialogEx::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CDialogEx@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?OnActivate@CDialogEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CDialogEx__IEAAXIPEAVCWnd__H_Z(
    CDialogEx* pThis, UINT nState, CWnd* pWndOther, BOOL bMinimized) {
    (void)pThis;
    (void)nState;
    (void)pWndOther;
    (void)bMinimized;
}
// Symbol: ?OnCommand@CDialogEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CDialogEx__MEAAH_K_J_Z(
    CDialogEx* pThis, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    if (!pThis) return FALSE;
    switch (LOWORD(wParam)) {
    case IDOK:
        impl__OnOK_CDialog__MEAAXXZ(pThis);
        return TRUE;
    case IDCANCEL:
        impl__OnCancel_CDialog__MEAAXXZ(pThis);
        return TRUE;
    default:
        return FALSE;
    }
}
// Symbol: ?OnCtlColor@CDialogEx@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CDialogEx* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    (void)pWnd;
    (void)nCtlColor;
    if (!pThis) return GetSysColorBrush(COLOR_BTNFACE);
    if (pDC && pDC->GetSafeHdc()) {
        ::SetBkColor(pDC->GetSafeHdc(), DialogExAccess(pThis)->m_clrBackground);
    }
    auto& state = g_dialogExBackgroundState[pThis];
    if (!state.brush) {
        state.brush = ::CreateSolidBrush(DialogExAccess(pThis)->m_clrBackground);
    }
    return state.brush ? state.brush : GetSysColorBrush(COLOR_BTNFACE);
}
// Symbol: ?OnDestroy@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_dlgMap.erase(pThis->m_hWnd);
    }
    auto it = g_dialogExBackgroundState.find(pThis);
    if (it != g_dialogExBackgroundState.end()) {
        if (it->second.brush) ::DeleteObject(it->second.brush);
        if (it->second.ownsBitmap && DialogExAccess(pThis)->m_hBackgroundImage) {
            ::DeleteObject(DialogExAccess(pThis)->m_hBackgroundImage);
            DialogExAccess(pThis)->m_hBackgroundImage = nullptr;
        }
        g_dialogExBackgroundState.erase(it);
    }
}
// Symbol: ?OnEraseBkgnd@CDialogEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CDialogEx__IEAAHPEAVCDC___Z(CDialogEx* pThis, CDC* pDC) {
    if (!pThis || !pDC || !pDC->GetSafeHdc() || !pThis->GetSafeHwnd()) return FALSE;

    RECT client;
    if (!::GetClientRect(pThis->GetSafeHwnd(), &client)) return FALSE;
    HDC hdc = pDC->GetSafeHdc();
    auto& state = g_dialogExBackgroundState[pThis];
    if (!state.brush) state.brush = ::CreateSolidBrush(DialogExAccess(pThis)->m_clrBackground);
    if (state.brush) ::FillRect(hdc, &client, state.brush);

    HBITMAP bitmap = static_cast<HBITMAP>(DialogExAccess(pThis)->m_hBackgroundImage);
    if (!bitmap) return state.brush != nullptr;

    BITMAP bm;
    if (::GetObjectW(bitmap, sizeof(bm), &bm) != sizeof(bm) || bm.bmWidth <= 0 || bm.bmHeight <= 0) return FALSE;
    HDC source = ::CreateCompatibleDC(hdc);
    if (!source) return FALSE;
    HGDIOBJ old = ::SelectObject(source, bitmap);
    int x = 0;
    int y = 0;
    if (state.location == 2) x = client.right - bm.bmWidth;
    else if (state.location == 3) y = client.bottom - bm.bmHeight;
    else if (state.location == 4) { x = client.right - bm.bmWidth; y = client.bottom - bm.bmHeight; }

    if (state.location == 0) {
        for (int tileY = 0; tileY < client.bottom; tileY += bm.bmHeight) {
            for (int tileX = 0; tileX < client.right; tileX += bm.bmWidth) {
                ::BitBlt(hdc, tileX, tileY, bm.bmWidth, bm.bmHeight, source, 0, 0, SRCCOPY);
            }
        }
    } else {
        ::BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, source, 0, 0, SRCCOPY);
    }
    ::SelectObject(source, old);
    ::DeleteDC(source);
    return TRUE;
}
// Symbol: ?OnNcActivate@CDialogEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CDialogEx__IEAAHH_Z(CDialogEx* pThis, BOOL bActive) {
    (void)pThis;
    return bActive;
}
// Symbol: ?OnSettingChange@CDialogEx@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CDialogEx__IEAAXIPEB_W_Z(
    CDialogEx* pThis, UINT uFlags, const wchar_t* lpszSection) {
    (void)pThis;
    (void)uFlags;
    (void)lpszSection;
}
// Symbol: ?OnSysColorChange@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (pThis) {
        DialogExAccess(pThis)->m_clrBackground = GetSysColor(COLOR_BTNFACE);
    }
}
// Symbol: ?PreTranslateMessage@CDialogEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(
    CDialogEx* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Symbol: ?SetActiveMenu@CDialogEx@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__SetActiveMenu_CDialogEx__IEAAXPEAVCMFCPopupMenu___Z(
    CDialogEx* pThis, void* pMenu) {
    (void)pThis;
    (void)pMenu;
}
// Symbol: ?SetBackgroundColor@CDialogEx@@QEAAXKH@Z
extern "C" void MS_ABI impl__SetBackgroundColor_CDialogEx__QEAAXKH_Z(
    CDialogEx* pThis, COLORREF color, BOOL bRepaint) {
    if (!pThis) return;
    DialogExAccess(pThis)->m_clrBackground = color;
    auto& state = g_dialogExBackgroundState[pThis];
    if (state.brush) ::DeleteObject(state.brush);
    state.brush = ::CreateSolidBrush(color);
    if (bRepaint && pThis->m_hWnd) {
        InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}
// Symbol: ?SetBackgroundImage@CDialogEx@@QEAAHIW4BackgroundLocation@1@H@Z
extern "C" void MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAXPEAUHBITMAP____W4BackgroundLocation_1_HH_Z(
    CDialogEx* pThis, HBITMAP hBitmap, int backgroundLocation, BOOL bAutoDestroy, BOOL bRepaint);
// Symbol: ?SetBackgroundImage@CDialogEx@@QEAAXPEAUHBITMAP__@@W4BackgroundLocation@1@HH@Z
extern "C" void MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAXPEAUHBITMAP____W4BackgroundLocation_1_HH_Z(
    CDialogEx* pThis, HBITMAP hBitmap, int backgroundLocation, BOOL bAutoDestroy, BOOL bRepaint) {
    if (!pThis) return;
    auto& state = g_dialogExBackgroundState[pThis];
    HBITMAP oldBitmap = static_cast<HBITMAP>(DialogExAccess(pThis)->m_hBackgroundImage);
    if (oldBitmap && oldBitmap != hBitmap && state.ownsBitmap) ::DeleteObject(oldBitmap);
    DialogExAccess(pThis)->m_hBackgroundImage = hBitmap;
    state.location = backgroundLocation;
    state.ownsBitmap = bAutoDestroy != FALSE;
    if (bRepaint && pThis->m_hWnd) {
        InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}
