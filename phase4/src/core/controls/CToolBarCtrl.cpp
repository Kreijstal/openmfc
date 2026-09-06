// CToolBarCtrl — OpenMFC implementation.
// Sources: cbarcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?AddString@CToolBarCtrl@@QEAAHI@Z
extern "C" int MS_ABI impl__AddString_CToolBarCtrl__QEAAHI_Z(CWnd* pThis, unsigned int nStringID) {
    if (!pThis || !pThis->GetSafeHwnd()) return -1;
    wchar_t text[256] = {};
    ::LoadStringW(AfxGetInstanceHandle(), nStringID, text, 256);
    return (int)::SendMessageW(pThis->GetSafeHwnd(), TB_ADDSTRINGW, 0, (LPARAM)text);
}
// Symbol: ?Create@CToolBarCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CToolBarCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, unsigned long dwStyle, const RECT* rect, CWnd* pParentWnd, unsigned int nID) {
    RECT rc = rect ? *rect : RECT{0, 0, 0, 0};
    return impl__CreateEx_CToolBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(pThis, 0, dwStyle, &rc, pParentWnd, nID);
}
// Symbol: ?CreateEx@CToolBarCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CToolBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, unsigned long dwExStyle, unsigned long dwStyle, const RECT* rect, CWnd* pParentWnd, unsigned int nID) {
    if (!pThis || !pParentWnd) return FALSE;
    RECT rc = rect ? *rect : RECT{0, 0, 0, 0};
    HWND hwnd = ::CreateWindowExW(dwExStyle, TOOLBARCLASSNAMEW, nullptr, dwStyle | WS_CHILD,
                                  rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                                  pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                  AfxGetInstanceHandle(), nullptr);
    pThis->m_hWnd = hwnd;
    if (hwnd) ::SendMessageW(hwnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    return hwnd != nullptr;
}
// Symbol: ?GetDropTarget@CToolBarCtrl@@QEBAJPEAPEAUIDropTarget@@@Z
extern "C" long MS_ABI impl__GetDropTarget_CToolBarCtrl__QEBAJPEAPEAUIDropTarget___Z(const CWnd* pThis, IDropTarget** ppDropTarget) {
    (void)pThis;
    if (ppDropTarget) *ppDropTarget = nullptr;
    return E_NOTIMPL;
}
// GetMessageMap/GetThisMessageMap for CToolBarCtrl live in core/controls/MessageMaps.cpp
// (classCToolBarCtrl_msgmap, base map CWnd). Retail agrees: ?GetThisMessageMap@
// CToolBarCtrl@@KAPEBUAFX_MSGMAP@@XZ in mfc140.dll returns the AFX_MSGMAP at 0x180336c20,
// whose pfnGetBaseMap is ?GetThisMessageMap@CWnd@@KAPEBUAFX_MSGMAP@@XZ. The copies that
// used to be here returned CWnd's own map, collapsing CToolBarCtrl out of the chain.
// Symbol: ?GetRuntimeClass@CToolBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBarCtrl__UEBAPEAUCRuntimeClass__XZ(const CWnd* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CWnd::GetThisClass();
}
// Symbol: ?GetThisClass@CToolBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return CWnd::GetThisClass();
}
// Symbol: ?OnCreate@CToolBarCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CToolBarCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(CWnd* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpCreateStruct) return -1;
    return ::SendMessageW(pThis->GetSafeHwnd(), TB_BUTTONSTRUCTSIZE,
                          sizeof(TBBUTTON), 0) ? 0 : -1;
}
// Symbol: ?RestoreState@CToolBarCtrl@@QEAAXPEAUHKEY__@@PEB_W1@Z
extern "C" void MS_ABI impl__RestoreState_CToolBarCtrl__QEAAXPEAUHKEY____PEB_W1_Z(CWnd* pThis, HKEY hKeyRoot, const wchar_t* lpszSubKey, const wchar_t* lpszValueName) {
    if (!pThis || !pThis->GetSafeHwnd() || !hKeyRoot || !lpszSubKey || !lpszValueName) return;
    TBSAVEPARAMSW params = { hKeyRoot, lpszSubKey, lpszValueName };
    ::SendMessageW(pThis->GetSafeHwnd(), TB_SAVERESTOREW, FALSE,
                   reinterpret_cast<LPARAM>(&params));
}
// Symbol: ?SaveState@CToolBarCtrl@@QEAAXPEAUHKEY__@@PEB_W1@Z
extern "C" void MS_ABI impl__SaveState_CToolBarCtrl__QEAAXPEAUHKEY____PEB_W1_Z(CWnd* pThis, HKEY hKeyRoot, const wchar_t* lpszSubKey, const wchar_t* lpszValueName) {
    if (!pThis || !pThis->GetSafeHwnd() || !hKeyRoot || !lpszSubKey || !lpszValueName) return;
    TBSAVEPARAMSW params = { hKeyRoot, lpszSubKey, lpszValueName };
    ::SendMessageW(pThis->GetSafeHwnd(), TB_SAVERESTOREW, TRUE,
                   reinterpret_cast<LPARAM>(&params));
}
// Symbol: ?GetString@CToolBarCtrl@@QEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetString_CToolBarCtrl__QEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, int nString, CString* pString) {
    if (!pString) return 0;
    *pString = L"";
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (!hwnd) return 0;

    constexpr int kMaxToolBarString = 1024;
    std::vector<wchar_t> buffer(kMaxToolBarString);
    LRESULT copied = ::SendMessageW(
        hwnd, TB_GETSTRINGW, MAKEWPARAM(kMaxToolBarString, nString), reinterpret_cast<LPARAM>(buffer.data()));
    if (copied < 0) return static_cast<int>(copied);
    buffer.back() = L'\0';
    *pString = buffer.data();
    return static_cast<int>(copied);
}
// Symbol: ??1CToolBarCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CToolBarCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?AddBitmap@CToolBarCtrl@@QEAAHHI@Z
extern "C" int MS_ABI impl__AddBitmap_CToolBarCtrl__QEAAHHI_Z(
    void* pThis, int nNumBits, unsigned int nBitmapID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || nBitmapID == 0 || nNumBits <= 0) {
        return -1;
    }

    TBADDBITMAP info{};
    info.hInst = AfxGetInstanceHandle();
    info.nID = nBitmapID;
    return static_cast<int>(::SendMessageW(pWnd->GetSafeHwnd(), TB_ADDBITMAP, static_cast<WPARAM>(nNumBits), reinterpret_cast<LPARAM>(&info)));
}
// Symbol: ?AddBitmap@CToolBarCtrl@@QEAAHHPEAVCBitmap@@@Z
extern "C" int MS_ABI impl__AddBitmap_CToolBarCtrl__QEAAHHPEAVCBitmap___Z(
    void* pThis, int nNumBits, void* pBitmap) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || pBitmap == nullptr || nNumBits <= 0) {
        return -1;
    }

    HBITMAP hBitmap = static_cast<HBITMAP>(static_cast<CGdiObject*>(pBitmap)->GetSafeHandle());
    return AddBitmapViaHandle(pWnd, nNumBits, hBitmap);
}
