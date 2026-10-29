// CDialogEx — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"
#include "detail/ModuleThreadStateSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
    CDialog* pThis, MSG* pMsg);
extern "C" void* MS_ABI impl___0CDialogImpl__IEAA_AEAVCWnd___Z(void* pThis, void* pWnd);
extern "C" void MS_ABI impl__OnActivate_CDialogImpl__IEAAXIPEAVCWnd___Z(
    void* pThis, unsigned int nState, CWnd* pWndOther);
extern "C" void MS_ABI impl__OnDestroy_CDialogImpl__IEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnCommand_CDialogImpl__IEAAH_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" void MS_ABI impl__OnNcActivate_CDialogImpl__IEAAXAEAH_Z(void* pThis, int* pbActive);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnDisplayChange_CWnd__IEAAXIHH_Z(
    CWnd* pThis, UINT type, int cx, int cy);
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis);
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* pThis);
extern "C" intptr_t MS_ABI impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam);
extern "C" intptr_t MS_ABI impl__OnHelpHitTest_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam);
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(
    CDialog* pThis, CFont* pFont, BOOL bRedraw);
extern "C" void MS_ABI impl__OnPaint_CDialog__IEAAXXZ(CDialog* pThis);
extern "C" int MS_ABI impl__OnQueryEndSession_CDialog__IEAAHXZ(CDialog* pThis);
extern "C" void MS_ABI impl__OnEndSession_CDialog__IEAAXH_Z(CDialog* pThis, BOOL bEnding);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__OnActivate_CDialogEx__IEAAXIPEAVCWnd__H_Z(
    CDialogEx* pThis, UINT nState, CWnd* pWndOther, BOOL bMinimized);
extern "C" int MS_ABI impl__OnNcActivate_CDialogEx__IEAAHH_Z(CDialogEx* pThis, BOOL bActive);
extern "C" int MS_ABI impl__OnEraseBkgnd_CDialogEx__IEAAHPEAVCDC___Z(CDialogEx* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnDestroy_CDialogEx__IEAAXXZ(CDialogEx* pThis);
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CDialogEx* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor);
extern "C" void MS_ABI impl__OnSysColorChange_CDialogEx__IEAAXXZ(CDialogEx* pThis);
extern "C" void MS_ABI impl__OnSettingChange_CDialogEx__IEAAXIPEB_W_Z(
    CDialogEx* pThis, UINT uFlags, const wchar_t* lpszSection);

namespace {
struct DialogBaseMessageMapTarget : CDialog {
    void OnOkCommand() { impl__OnOK_CDialog__MEAAXXZ(this); }
    void OnCancelCommand() { impl__OnCancel_CDialog__MEAAXXZ(this); }
    LRESULT OnCommandHelpMessage(WPARAM wParam, LPARAM lParam) {
        return impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(this, wParam, lParam);
    }
    LRESULT OnHelpHitTestMessage(WPARAM wParam, LPARAM lParam) {
        return impl__OnHelpHitTest_CDialog__IEAA_J_K_J_Z(this, wParam, lParam);
    }
    LRESULT OnInitDialogMessage(WPARAM wParam, LPARAM lParam) {
        return impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(this, wParam, lParam);
    }
    void OnSetFontMessage(CFont* font, BOOL redraw) {
        impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(this, font, redraw);
    }
    void OnPaintMessage() { impl__OnPaint_CDialog__IEAAXXZ(this); }
    BOOL OnQueryEndSessionMessage() { return impl__OnQueryEndSession_CDialog__IEAAHXZ(this); }
    void OnEndSessionMessage(UINT ending) {
        impl__OnEndSession_CDialog__IEAAXH_Z(this, static_cast<BOOL>(ending));
    }
};

const AFX_MSGMAP* DialogBaseMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = {
        {WM_COMMAND, CN_COMMAND, IDOK, IDOK, AfxSigCmd_v, (AFX_PMSG)&DialogBaseMessageMapTarget::OnOkCommand},
        {WM_COMMAND, CN_COMMAND, IDCANCEL, IDCANCEL, AfxSigCmd_v, (AFX_PMSG)&DialogBaseMessageMapTarget::OnCancelCommand},
        {0x365, 0, 0, 0, AfxSig_l_w_l, (AFX_PMSG)&DialogBaseMessageMapTarget::OnCommandHelpMessage},
        {0x366, 0, 0, 0, AfxSig_l_w_l, (AFX_PMSG)&DialogBaseMessageMapTarget::OnHelpHitTestMessage},
        {WM_INITDIALOG, 0, 0, 0, AfxSig_l_w_l, (AFX_PMSG)&DialogBaseMessageMapTarget::OnInitDialogMessage},
        {WM_SETFONT, 0, 0, 0, AfxSig_v_F_b, (AFX_PMSG)&DialogBaseMessageMapTarget::OnSetFontMessage},
        {WM_PAINT, 0, 0, 0, AfxSig_v_v_v, (AFX_PMSG)&DialogBaseMessageMapTarget::OnPaintMessage},
        {WM_QUERYENDSESSION, 0, 0, 0, AfxSig_b_v_v, (AFX_PMSG)&DialogBaseMessageMapTarget::OnQueryEndSessionMessage},
        {WM_ENDSESSION, 0, 0, 0, AfxSig_v_u_v, (AFX_PMSG)&DialogBaseMessageMapTarget::OnEndSessionMessage},
        {0, 0, 0, 0, AfxSig_end, nullptr}
    };
    static const AFX_MSGMAP map = {&CWnd::GetThisMessageMap, entries};
    return &map;
}

struct DialogExMessageMapTarget : CDialogEx {
    void OnActivateMessage(UINT state, CWnd* other, int minimized) {
        impl__OnActivate_CDialogEx__IEAAXIPEAVCWnd__H_Z(this, state, other, minimized);
    }
    int OnNcActivateMessage(int active) {
        return impl__OnNcActivate_CDialogEx__IEAAHH_Z(this, active);
    }
    int OnEraseBkgndMessage(CDC* dc) {
        return impl__OnEraseBkgnd_CDialogEx__IEAAHPEAVCDC___Z(this, dc);
    }
    void OnDestroyMessage() {
        impl__OnDestroy_CDialogEx__IEAAXXZ(this);
    }
    HBRUSH OnCtlColorMessage(CDC* dc, CWnd* wnd, UINT kind) {
        return impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(this, dc, wnd, kind);
    }
    void OnSysColorChangeMessage() {
        impl__OnSysColorChange_CDialogEx__IEAAXXZ(this);
    }
    void OnSettingChangeMessage(UINT flags, const wchar_t* section) {
        impl__OnSettingChange_CDialogEx__IEAAXIPEB_W_Z(this, flags, section);
    }
};

bool IsCurrentModuleMainWnd(CDialogEx* pThis) {
    AFX_MODULE_THREAD_STATE* state =
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    CWinThread* thread = state ? state->m_pCurrentWinThread : nullptr;
    return thread && thread->GetMainWnd() == static_cast<CWnd*>(pThis);
}

bool g_dialogExGlobalDataInitialized = false;
void EnsureDialogExGlobalDataInitialized() {
    if (!g_dialogExGlobalDataInitialized) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        g_dialogExGlobalDataInitialized = true;
    }
}

const AFX_MSGMAP* DialogExMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = {
        {WM_ACTIVATE, 0, 0, 0, AfxSig_ACTIVATE, (AFX_PMSG)&DialogExMessageMapTarget::OnActivateMessage},
        {WM_NCACTIVATE, 0, 0, 0, AfxSig_b_b_v, (AFX_PMSG)&DialogExMessageMapTarget::OnNcActivateMessage},
        {WM_ERASEBKGND, 0, 0, 0, AfxSig_b_D_v, (AFX_PMSG)&DialogExMessageMapTarget::OnEraseBkgndMessage},
        {WM_DESTROY, 0, 0, 0, AfxSig_v_v_v, (AFX_PMSG)&DialogExMessageMapTarget::OnDestroyMessage},
        {0x19, 0, 0, 0, AfxSig_CTLCOLOR, (AFX_PMSG)&DialogExMessageMapTarget::OnCtlColorMessage},
        {WM_SYSCOLORCHANGE, 0, 0, 0, AfxSig_v_v_v, (AFX_PMSG)&DialogExMessageMapTarget::OnSysColorChangeMessage},
        {WM_SETTINGCHANGE, 0, 0, 0, AfxSig_v_u_cs, (AFX_PMSG)&DialogExMessageMapTarget::OnSettingChangeMessage},
        {0, 0, 0, 0, AfxSig_end, nullptr}
    };
    static const AFX_MSGMAP map = {&DialogBaseMessageMap, entries};
    return &map;
}
} // namespace

// Retail mfc140u RVA 0x3ed70 initializes the image handle and size to zero,
// the image location to -1, and auto-destroy to FALSE. The local class uses
// padding for these fields, so initialize the verified retail offsets directly;
// the CBrush handle at +0x148 is also zeroed because no local CBrush ctor runs.
// The embedded CDialogImpl constructor is also materialized here because the
// local CDialogEx declaration has no C++ member for it.
// Symbol: ?CommonConstruct@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__CommonConstruct_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (!pThis) return;
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    *reinterpret_cast<HBITMAP*>(bytes + 0x130) = nullptr;
    *reinterpret_cast<SIZE*>(bytes + 0x138) = SIZE{0, 0};
    *reinterpret_cast<HBRUSH*>(bytes + 0x148) = nullptr;
    *reinterpret_cast<int*>(bytes + 0x150) = -1;
    *reinterpret_cast<BOOL*>(bytes + 0x168) = FALSE;
    impl___0CDialogImpl__IEAA_AEAVCWnd___Z(bytes + 0x158, static_cast<CWnd*>(pThis));
    CDialogExBackgroundState state;
    state.location = -1;
    g_dialogExBackgroundState[pThis] = state;
}
// Retail mfc140u RVA 0x3ef30 returns the CDialogEx map (descriptor RVA
// 0x2e49a0), whose base is CDialog. The seven CDialogEx handlers and the nine
// CDialog base entries are represented by the local maps above.
// Symbol: ?GetMessageMap@CDialogEx@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialogEx__MEBAPEBUAFX_MSGMAP__XZ(
    const CDialogEx* pThis) {
    (void)pThis;
    return DialogExMessageMap();
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
    return DialogExMessageMap();
}
// Retail mfc140u RVA 0x3ef40 calls CWnd::Default first, then tail-jumps to
// CDialogImpl::OnActivate(nState, pWndOther); bMinimized is not forwarded.
// The retail member access places m_Impl at +0x158; the local declaration
// represents its storage as padding, so this call uses the verified offset.
// Symbol: ?OnActivate@CDialogEx@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CDialogEx__IEAAXIPEAVCWnd__H_Z(
    CDialogEx* pThis, UINT nState, CWnd* pWndOther, BOOL bMinimized) {
    (void)bMinimized;
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    impl__OnActivate_CDialogImpl__IEAAXIPEAVCWnd___Z(
        reinterpret_cast<unsigned char*>(pThis) + 0x158, nState, pWndOther);
}
// Retail mfc140u RVA 0x3f400 calls m_Impl.OnCommand first and, if it does
// not handle the command, forwards both arguments to CWnd::OnCommand.
// Symbol: ?OnCommand@CDialogEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CDialogEx__MEAAH_K_J_Z(
    CDialogEx* pThis, WPARAM wParam, LPARAM lParam) {
    void* pImpl = reinterpret_cast<unsigned char*>(pThis) + 0x158;
    if (impl__OnCommand_CDialogImpl__IEAAH_K_J_Z(pImpl, wParam, lParam)) {
        return TRUE;
    }
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(static_cast<CWnd*>(pThis), wParam, lParam);
}
// Retail mfc140u RVA 0x3f230 has a theme/brush path and bitmap-dependent
// behavior beyond the base handler. The local layout does not model those
// objects closely enough for a faithful transcription, so keep this stubbed.
// Symbol: ?OnCtlColor@CDialogEx@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CDialogEx__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CDialogEx* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    (void)pThis;
    (void)pDC;
    (void)pWnd;
    (void)nCtlColor;
    return nullptr;
}
// Retail mfc140u RVA 0x3f1e0 destroys the bitmap only when the auto-destroy
// flag at +0x168 is set, then calls CDialogImpl::OnDestroy and CWnd::OnDestroy.
// OpenMFC additionally removes its HWND/state bookkeeping and releases the
// locally-created background brush because the shadow class has no CBrush dtor.
// Symbol: ?OnDestroy@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (!pThis) return;
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    HBITMAP* pBitmap = reinterpret_cast<HBITMAP*>(bytes + 0x130);
    HBRUSH* pBrush = reinterpret_cast<HBRUSH*>(bytes + 0x148);
    if (*reinterpret_cast<BOOL*>(bytes + 0x168) && *pBitmap) {
        ::DeleteObject(*pBitmap);
        *pBitmap = nullptr;
    }
    if (pThis->m_hWnd) g_dlgMap.erase(pThis->m_hWnd);
    if (*pBrush) {
        ::DeleteObject(*pBrush);
        *pBrush = nullptr;
    }
    g_dialogExBackgroundState.erase(pThis);
    impl__OnDestroy_CDialogImpl__IEAAXXZ(bytes + 0x158);
    impl__OnDestroy_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
}
// Retail's message-map callback at RVA 0x3efd0 (mfc140u) uses CWnd::Default
// when neither bitmap nor brush exists, and as the fill fallback when a
// non-tiled bitmap has no brush.
// Symbol: ?OnEraseBkgnd@CDialogEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CDialogEx__IEAAHPEAVCDC___Z(CDialogEx* pThis, CDC* pDC) {
    if (!pThis) return FALSE;
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    HBITMAP bitmap = *reinterpret_cast<HBITMAP*>(bytes + 0x130);
    HBRUSH brush = *reinterpret_cast<HBRUSH*>(bytes + 0x148);
    if (!bitmap && !brush) {
        return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis)));
    }
    if (!pDC || !pDC->GetSafeHdc() || !pThis->GetSafeHwnd()) return FALSE;

    RECT client{};
    if (!::GetClientRect(pThis->GetSafeHwnd(), &client)) return FALSE;
    HDC hdc = pDC->GetSafeHdc();
    const int location = *reinterpret_cast<int*>(bytes + 0x150);
    if (location != 0 || !bitmap) {
        if (brush) ::FillRect(hdc, &client, brush);
        else impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
    if (!bitmap) return TRUE;

    SIZE imageSize = *reinterpret_cast<SIZE*>(bytes + 0x138);
    BITMAP bm{};
    if (::GetObjectW(bitmap, sizeof(bm), &bm) != sizeof(bm) || bm.bmWidth <= 0 || bm.bmHeight <= 0) return FALSE;
    if (imageSize.cx > 0) bm.bmWidth = imageSize.cx;
    if (imageSize.cy > 0) bm.bmHeight = imageSize.cy;
    HDC source = ::CreateCompatibleDC(hdc);
    if (!source) return FALSE;
    HGDIOBJ old = ::SelectObject(source, bitmap);
    int x = 0;
    int y = 0;
    if (location == 2) x = client.right - bm.bmWidth;
    else if (location == 3) y = client.bottom - bm.bmHeight;
    else if (location == 4) { x = client.right - bm.bmWidth; y = client.bottom - bm.bmHeight; }

    if (location == 0) {
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
// Retail's message-map callback at RVA 0x3ef80 (mfc140u) asks m_Impl to
// adjust bActive, then virtually calls DefWindowProc(WM_NCACTIVATE, bActive, 0).
// OpenMFC devirtualizes this to the CWnd thunk because its vtable is not retail's;
// a derived DefWindowProc override is therefore not reached here.
// Symbol: ?OnNcActivate@CDialogEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CDialogEx__IEAAHH_Z(CDialogEx* pThis, BOOL bActive) {
    int active = bActive;
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    impl__OnNcActivate_CDialogImpl__IEAAXAEAH_Z(bytes + 0x158, &active);
    return static_cast<int>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
        static_cast<CWnd*>(pThis), WM_NCACTIVATE,
        static_cast<WPARAM>(static_cast<LONG_PTR>(active)), 0));
}
// The exact export name has no resolved RVA, but the message-map callback at
// RVA 0x3f4c0 (mfc140u) is present. Retail runs the internal CWnd setting-change
// path and updates AFX_GLOBAL_DATA only when the module thread's GetMainWnd is
// this dialog. The CWnd control-container branch at +0xc8 and its main-window
// metric refresh (call at 0x1d5e40 inside OnDisplayChange entry 0x28f4f0,
// mfc140u) are not modeled here.
// Symbol: ?OnSettingChange@CDialogEx@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CDialogEx__IEAAXIPEB_W_Z(
    CDialogEx* pThis, UINT uFlags, const wchar_t* lpszSection) {
    (void)uFlags;
    (void)lpszSection;
    if (!pThis) return;
    // Retail's internal CWnd handler tail-jumps to OnDisplayChange(this, 0, 0, 0),
    // whose body ends in CWnd::Default.
    impl__OnDisplayChange_CWnd__IEAAXIHH_Z(static_cast<CWnd*>(pThis), 0, 0, 0);
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (!IsCurrentModuleMainWnd(pThis)) return;
    EnsureDialogExGlobalDataInitialized();
    impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
}
// Retail's message-map callback at RVA 0x3f450 (mfc140u) invokes the CWnd
// system-color handler, then initializes and refreshes AFX_GLOBAL_DATA only for
// the module's main window. The existing OpenMFC CWnd thunk only invalidates;
// its separate metric-refresh call is not modeled here.
// Symbol: ?OnSysColorChange@CDialogEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CDialogEx__IEAAXXZ(CDialogEx* pThis) {
    if (!pThis) return;
    impl__OnSysColorChange_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    if (!IsCurrentModuleMainWnd(pThis)) return;
    EnsureDialogExGlobalDataInitialized();
    impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
}
// Symbol: ?PreTranslateMessage@CDialogEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(
    CDialogEx* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// No CDialogEx SetActiveMenu entry is present in the retail message map, and
// the exact symbol has no resolved RVA. Do not assume the CMFCPropertyPage
// forwarding body applies to CDialogEx.
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
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    HBRUSH* pBrush = reinterpret_cast<HBRUSH*>(bytes + 0x148);
    if (*pBrush) ::DeleteObject(*pBrush);
    *pBrush = ::CreateSolidBrush(color);
    if (bRepaint && pThis->m_hWnd) {
        InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}
// Symbol: ?SetBackgroundImage@CDialogEx@@QEAAXPEAUHBITMAP__@@W4BackgroundLocation@1@HH@Z
extern "C" void MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAXPEAUHBITMAP____W4BackgroundLocation_1_HH_Z(
    CDialogEx* pThis, HBITMAP hBitmap, int backgroundLocation, BOOL bAutoDestroy, BOOL bRepaint) {
    if (!pThis) return;
    auto* bytes = reinterpret_cast<unsigned char*>(pThis);
    HBITMAP* pOldBitmap = reinterpret_cast<HBITMAP*>(bytes + 0x130);
    BOOL* pOldAutoDestroy = reinterpret_cast<BOOL*>(bytes + 0x168);
    if (*pOldBitmap && *pOldBitmap != hBitmap && *pOldAutoDestroy) {
        ::DeleteObject(*pOldBitmap);
    }
    *pOldBitmap = hBitmap;
    *reinterpret_cast<SIZE*>(bytes + 0x138) = SIZE{0, 0};
    if (hBitmap) {
        BITMAP bitmap{};
        if (::GetObjectW(hBitmap, sizeof(bitmap), &bitmap) == sizeof(bitmap)) {
            *reinterpret_cast<SIZE*>(bytes + 0x138) = SIZE{bitmap.bmWidth, bitmap.bmHeight};
        }
    }
    *reinterpret_cast<int*>(bytes + 0x150) = backgroundLocation;
    *pOldAutoDestroy = bAutoDestroy != FALSE;
    auto& state = g_dialogExBackgroundState[pThis];
    state.location = backgroundLocation;
    state.ownsBitmap = bAutoDestroy != FALSE;
    if (bRepaint && pThis->m_hWnd) {
        InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}
