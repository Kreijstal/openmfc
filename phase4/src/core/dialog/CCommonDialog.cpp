// CCommonDialog — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"

extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int save);
extern "C" LONGLONG MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);

// CCommonDialog is absent from include/openmfc; retail afxdlgs.h:105-117
// declares it as a fieldless CDialog-derived class. These handlers touch no
// CCommonDialog fields; pin the CWnd/CDialog base sizes used by the thunks.
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CDialog) == 0x130,
              "retail CCommonDialog CWnd/CDialog base layout");

// Symbol: ?OnOK@CCommonDialog@@MEAAXXZ
// Ordinal: 10703
extern "C" void MS_ABI impl__OnOK_CCommonDialog__MEAAXXZ(void* pThis) {
    // Retail mfc140u.dll (Unicode), RVA 0x207b80: call UpdateData(TRUE),
    // then call Default only when the update succeeds.
    CWnd* pWnd = reinterpret_cast<CWnd*>(pThis);
    if (impl__UpdateData_CWnd__QEAAHH_Z(pWnd, TRUE)) {
        (void)impl__Default_CWnd__IEAA_JXZ(pWnd);
    }
}
// Symbol: ?OnCancel@CCommonDialog@@MEAAXXZ
// Ordinal: 8730
extern "C" void MS_ABI impl__OnCancel_CCommonDialog__MEAAXXZ(void* pThis) {
    // Retail mfc140u.dll (Unicode), ordinal 8730 at RVA 0xda30: this shared
    // entry tail-jumps to CWnd::Default.
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// Symbol: ?OnPaint@CCommonDialog@@IEAAXXZ
// Ordinal: 10724
extern "C" void MS_ABI impl__OnPaint_CCommonDialog__IEAAXXZ(CDialog* pThis) {
    // Retail mfc140u.dll (Unicode), ordinal 10724 shares RVA 0xda30 with
    // OnCancel and tail-jumps to CWnd::Default.
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// Symbol: ?OnHelpInfo@CCommonDialog@@IEAAHPEAUtagHELPINFO@@@Z
// Ordinal: 10082
extern "C" int MS_ABI impl__OnHelpInfo_CCommonDialog__IEAAHPEAUtagHELPINFO___Z(
    void* pThis, HELPINFO* pHelpInfo) {
    (void)pHelpInfo;
    // Retail mfc140u.dll (Unicode), RVA 0x207bb0: return whether CWnd::Default
    // returns a nonzero result.
    return impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis)) != 0 ? TRUE : FALSE;
}
