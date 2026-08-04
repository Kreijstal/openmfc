// CVSListBoxBase — OpenMFC implementation.
// Sources: dlgcommon.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?AddButton@CVSListBoxBase@@QEAAHIPEB_WGEI@Z
extern "C" int MS_ABI impl__AddButton_CVSListBoxBase__QEAAHIPEB_WGEI_Z(
    CVSListBoxBase* pThis, unsigned int nID, const wchar_t* lpszName, unsigned short, unsigned char, unsigned int) {
    (void)pThis;
    (void)lpszName;
    return (int)nID;
}
// Symbol: ?AdjustLayout@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CVSListBoxBase__IEAAXXZ(CVSListBoxBase* pThis) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Symbol: ?CreateNewItem@CVSListBoxBase@@MEAAXXZ
extern "C" void MS_ABI impl__CreateNewItem_CVSListBoxBase__MEAAXXZ(CVSListBoxBase* pThis) {
    (void)pThis;
}
// Symbol: ?EnableBrowseButton@CVSListBoxBase@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableBrowseButton_CVSListBoxBase__QEAAXH_Z(CVSListBoxBase* pThis, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Symbol: ?EnableButton@CVSListBoxBase@@QEAAHHH@Z
extern "C" void MS_ABI impl__EnableButton_CVSListBoxBase__QEAAHHH_Z(CVSListBoxBase* pThis, int, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Symbol: ?GetButtonID@CVSListBoxBase@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetButtonID_CVSListBoxBase__QEBAIH_Z(const CVSListBoxBase* pThis, int nIndex) {
    (void)pThis;
    return (unsigned int)nIndex;
}
// Symbol: ?GetButtonNum@CVSListBoxBase@@QEBAHI@Z
extern "C" int MS_ABI impl__GetButtonNum_CVSListBoxBase__QEBAHI_Z(const CVSListBoxBase* pThis, unsigned int nID) {
    (void)pThis;
    return (int)nID;
}
// Symbol: ?GetMessageMap@CVSListBoxBase@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CVSListBoxBase__MEBAPEBUAFX_MSGMAP__XZ(const CVSListBoxBase* pThis) {
    (void)pThis;
    return CStatic::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CVSListBoxBase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxBase__UEBAPEAUCRuntimeClass__XZ(const CVSListBoxBase* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxBase::GetThisClass();
}
// Symbol: ?GetStdButtonNum@CVSListBoxBase@@IEBAHI@Z
extern "C" int MS_ABI impl__GetStdButtonNum_CVSListBoxBase__IEBAHI_Z(const CVSListBoxBase* pThis, unsigned int nID) {
    (void)pThis;
    return (int)nID;
}
// Symbol: ?GetThisClass@CVSListBoxBase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxBase__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxBase::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CVSListBoxBase@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ() {
    return CStatic::GetThisMessageMap();
}
// Symbol: ?Init@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__Init_CVSListBoxBase__IEAAXXZ(CVSListBoxBase* pThis) {
    (void)pThis;
}
// Symbol: ?OnClickButton@CVSListBoxBase@@UEAAXH@Z
extern "C" void MS_ABI impl__OnClickButton_CVSListBoxBase__UEAAXH_Z(CVSListBoxBase* pThis, int) {
    (void)pThis;
}
// Symbol: ?OnCommand@CVSListBoxBase@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CVSListBoxBase__MEAAH_K_J_Z(CVSListBoxBase* pThis, unsigned __int64, __int64) {
    (void)pThis;
    return FALSE;
}
// Symbol: ?OnCreate@CVSListBoxBase@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CVSListBoxBase__IEAAHPEAUtagCREATESTRUCTW___Z(CVSListBoxBase* pThis, CREATESTRUCTW*) {
    return pThis ? 0 : -1;
}
// Symbol: ?OnEnable@CVSListBoxBase@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CVSListBoxBase__IEAAXH_Z(CVSListBoxBase* pThis, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Symbol: ?OnEndEditLabel@CVSListBoxBase@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnEndEditLabel_CVSListBoxBase__UEAAXPEB_W_Z(CVSListBoxBase* pThis, const wchar_t*) {
    (void)pThis;
}
// Symbol: ?OnEraseBkgnd@CVSListBoxBase@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CVSListBoxBase__IEAAHPEAVCDC___Z(CVSListBoxBase* pThis, CDC*) {
    (void)pThis;
    return TRUE;
}
// Symbol: ?OnGetDlgCode@CVSListBoxBase@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CVSListBoxBase__IEAAIXZ(CVSListBoxBase*) {
    return DLGC_WANTARROWS | DLGC_WANTCHARS;
}
// Symbol: ?OnKey@CVSListBoxBase@@UEAAXGE@Z
extern "C" void MS_ABI impl__OnKey_CVSListBoxBase__UEAAXGE_Z(CVSListBoxBase* pThis, unsigned short, unsigned char) {
    (void)pThis;
}
// Symbol: ?OnSetFocus@CVSListBoxBase@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CVSListBoxBase__IEAAXPEAVCWnd___Z(CVSListBoxBase* pThis, CWnd*) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, FALSE);
}
// Symbol: ?OnSetText@CVSListBoxBase@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CVSListBoxBase__IEAAHPEB_W_Z(CVSListBoxBase* pThis, const wchar_t* lpszText) {
    return pThis && pThis->GetSafeHwnd() ? ::SetWindowTextW(pThis->GetSafeHwnd(), lpszText) : FALSE;
}
// Symbol: ?OnSize@CVSListBoxBase@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CVSListBoxBase__IEAAXIHH_Z(CVSListBoxBase* pThis, unsigned int, int, int) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Symbol: ?PreSubclassWindow@CVSListBoxBase@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CVSListBoxBase__MEAAXXZ(CVSListBoxBase* pThis) {
    (void)pThis;
}
// Symbol: ?SetGrayDisabledButtons@CVSListBoxBase@@QEAAXH@Z
extern "C" void MS_ABI impl__SetGrayDisabledButtons_CVSListBoxBase__QEAAXH_Z(CVSListBoxBase* pThis, int) {
    (void)pThis;
}
// Symbol: ?SetStandardButtons@CVSListBoxBase@@QEAAHI@Z
extern "C" int MS_ABI impl__SetStandardButtons_CVSListBoxBase__QEAAHI_Z(CVSListBoxBase* pThis, unsigned int nButtons) {
    (void)pThis;
    return (int)nButtons;
}
// Symbol: ?OnGetFont@CVSListBoxBase@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CVSListBoxBase__IEAAPEAUHFONT____XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd->GetSafeHwnd() == nullptr) {
        return nullptr;
    }
    auto* pFontHandle = reinterpret_cast<void*>(
        static_cast<uintptr_t>(pWnd->SendMessageW(WM_GETFONT, 0, 0)));
    return pFontHandle;
}
// Symbol: ?OnPaint@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CVSListBoxBase__IEAAXXZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?OnSetFont@CVSListBoxBase@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CVSListBoxBase__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int redraw) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr) {
        return;
    }
    if (pWnd->GetSafeHwnd() == nullptr) {
        return;
    }
    HFONT hFont = nullptr;
    if (pFont != nullptr) {
        hFont = reinterpret_cast<HFONT>(
            static_cast<CFont*>(pFont)->GetSafeHandle());
    }
    pWnd->SendMessageW(WM_SETFONT, reinterpret_cast<uintptr_t>(hFont), redraw);
}
