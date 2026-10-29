// CMFCColorPropertySheet — OpenMFC implementation.
// Sources: global_cmfccolorpropertysheet.cpp

#include "detail/CMFCColorPropertySheetSupport.h"
#include <cstddef>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"

namespace {
struct S_CMFCColorPropertySheetLoadAcceleratorLayout {
    BYTE m_reserved[0x190];
    HACCEL m_hAccelTable;
};
static_assert(offsetof(S_CMFCColorPropertySheetLoadAcceleratorLayout, m_hAccelTable) == 0x190,
              "CMFCColorPropertySheet accelerator handle @0x190 (retail RVA 0x2eb30, mfc140u)");
static_assert(sizeof(S_CMFCColorPropertySheetLoadAcceleratorLayout) == 408,
              "CMFCColorPropertySheet size 408 bytes");

struct S_AfxModuleStateInstanceHandleLayout {
    void* m_vftable;
    void* m_pCurrentWinApp;
    HINSTANCE m_hCurrentInstanceHandle;
};
static_assert(offsetof(S_AfxModuleStateInstanceHandleLayout, m_hCurrentInstanceHandle) == 0x10,
              "AFX_MODULE_STATE current instance handle @0x10");
} // namespace

// Symbol: ??0CMFCColorPropertySheet@@QEAA@IPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCColorPropertySheet__QEAA_IPEAVCWnd__I_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    (void)nIDCaption;
    InitBase_Cmfccolorpropertysheet(pThis, pParentWnd, iSelectPage);
    return pThis;
}
// Symbol: ??0CMFCColorPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCColorPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    InitBase_Cmfccolorpropertysheet(pThis, pParentWnd, iSelectPage);
    static_cast<CPropertySheet*>(pThis)->m_pszCaption = pszCaption;
    return pThis;
}
// Symbol: ??1CMFCColorPropertySheet@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorPropertySheet__UEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CPropertySheet*>(pThis)->~CPropertySheet();
    }
}
// Symbol: ?OnInitDialog@CMFCColorPropertySheet@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCColorPropertySheet__UEAAHXZ(void* pThis) {
    return impl__OnInitDialog_CPropertySheet__UEAAHXZ(static_cast<CPropertySheet*>(pThis));
}
// Symbol: ?PreTranslateMessage@CMFCColorPropertySheet@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCColorPropertySheet__MEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(static_cast<CPropertySheet*>(pThis), pMsg);
}
// Symbol: ?LoadAcceleratorTable@CMFCColorPropertySheet@@QEAAXI@Z
// Retail mfc140u RVA 0x2eb30 returns immediately for ID 0. Otherwise it passes
// the low WORD as an integer-resource name to USER32!LoadAcceleratorsW, using
// AfxGetModuleState()->+0x10 as HINSTANCE, then stores the HACCEL at this+0x190.
extern "C" void MS_ABI impl__LoadAcceleratorTable_CMFCColorPropertySheet__QEAAXI_Z(
    void* pThis, unsigned int nID) {
    if (nID == 0) {
        return;
    }

    const WORD resourceId = static_cast<WORD>(nID);
    auto* moduleState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    const auto* stateLayout = reinterpret_cast<const S_AfxModuleStateInstanceHandleLayout*>(moduleState);
    auto* sheet = static_cast<S_CMFCColorPropertySheetLoadAcceleratorLayout*>(pThis);
    sheet->m_hAccelTable = ::LoadAcceleratorsW(
        stateLayout->m_hCurrentInstanceHandle, MAKEINTRESOURCEW(resourceId));
}
// Symbol: ?OnNotify@CMFCColorPropertySheet@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCColorPropertySheet__MEAAH_K_JPEA_J_Z(
    void* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    auto* pSheet = static_cast<CPropertySheet*>(pThis);
    if (!pSheet) {
        if (pResult) {
            *pResult = 0;
        }
        return FALSE;
    }
    return pSheet->OnNotify(wParam, lParam, pResult);
}
// Symbol: ?OnSize@CMFCColorPropertySheet@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCColorPropertySheet__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->Invalidate();
        (void)nType;
        (void)cx;
        (void)cy;
    }
}
// Symbol: ?PostNcDestroy@CMFCColorPropertySheet@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CMFCColorPropertySheet__MEAAXXZ(void* pThis) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->PostNcDestroy();
    }
}
// Symbol: ?ResizeControl@CMFCColorPropertySheet@@IEAAXXZ
extern "C" void MS_ABI impl__ResizeControl_CMFCColorPropertySheet__IEAAXXZ(void* pThis) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->Invalidate();
        pWnd->UpdateWindow();
    }
}
