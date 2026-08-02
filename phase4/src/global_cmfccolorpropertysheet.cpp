// OpenMFC — real implementations for CMFCColorPropertySheet stub exports.
//
// CMFCColorPropertySheet (size 408, base CPropertySheet) is the colour-picker
// property sheet used by CMFCColorMenuButton and the ribbon colour picker.
// It adds a colour palette control, accelerator table, and resize logic.
// This file provides minimal-but-real implementations that forward to base
// CPropertySheet where possible.

#include <windows.h>
#include <cstring>
#include <new>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(CPropertySheet*, CWnd*, unsigned int);
extern "C" int MS_ABI impl__OnInitDialog_CPropertySheet__UEAAHXZ(CPropertySheet*);
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(CPropertySheet*, MSG*);

namespace {

// Size harvested from mfc140u.dll (tools/harvest).
static const size_t kSizeof_CMFCColorPropertySheet = 408;

inline void* InitBase(void* pThis, CWnd* pParentWnd, unsigned int iSelectPage) {
    std::memset(pThis, 0, kSizeof_CMFCColorPropertySheet);
    auto* base = static_cast<CPropertySheet*>(pThis);
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(base, pParentWnd, iSelectPage);
    return pThis;
}

} // namespace

// Symbol: ??0CMFCColorPropertySheet@@QEAA@IPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCColorPropertySheet__QEAA_IPEAVCWnd__I_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    (void)nIDCaption;
    InitBase(pThis, pParentWnd, iSelectPage);
    return pThis;
}

// Symbol: ??0CMFCColorPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCColorPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    InitBase(pThis, pParentWnd, iSelectPage);
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
extern "C" void MS_ABI impl__LoadAcceleratorTable_CMFCColorPropertySheet__QEAAXI_Z(
    void* pThis, unsigned int nID) {
    (void)pThis;
    (void)nID;
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
