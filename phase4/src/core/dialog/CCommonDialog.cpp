// CCommonDialog — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"

// Symbol: ?OnOK@CCommonDialog@@MEAAXXZ
// Ordinal: 10703
extern "C" void MS_ABI impl__OnOK_CCommonDialog__MEAAXXZ(CDialog* pThis) {
    (void)pThis;
    // Retail MFC: CCommonDialog has no OK button — must NOT close the dialog
    // (deliberately different from CDialog::OnOK, which calls EndDialog(IDOK)).
}
// Symbol: ?OnCancel@CCommonDialog@@MEAAXXZ
// Ordinal: 8730
extern "C" void MS_ABI impl__OnCancel_CCommonDialog__MEAAXXZ(CDialog* pThis) {
    (void)pThis;
    // Retail MFC: CCommonDialog has no Cancel button — must NOT close the dialog
    // (deliberately different from CDialog::OnCancel, which calls EndDialog(IDCANCEL)).
}
// Symbol: ?OnPaint@CCommonDialog@@IEAAXXZ
// Ordinal: 10724
extern "C" void MS_ABI impl__OnPaint_CCommonDialog__IEAAXXZ(CDialog* pThis) {
    // Mirror CDialog::OnPaint: BeginPaint/EndPaint guard on pThis->m_hWnd
    if (!pThis || !pThis->m_hWnd) return;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(pThis->m_hWnd, &ps);
    if (hdc) {
        EndPaint(pThis->m_hWnd, &ps);
    }
}
// Symbol: ?OnHelpInfo@CCommonDialog@@IEAAHPEAUtagHELPINFO@@@Z
// Ordinal: 10082
extern "C" int MS_ABI impl__OnHelpInfo_CCommonDialog__IEAAHPEAUtagHELPINFO___Z(
    CDialog* pThis, HELPINFO* pHelpInfo) {
    (void)pThis;
    (void)pHelpInfo;
    // Mirror CWnd::OnHelpInfo — at minimum return TRUE (handled).
    return TRUE;
}
