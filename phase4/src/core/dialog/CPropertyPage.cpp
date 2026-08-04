// CPropertyPage — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
    CDialog* pThis, MSG* pMsg);

// Symbol: ?CommonConstruct@CPropertyPage@@IEAAXPEB_WI@Z
extern "C" void MS_ABI impl__CommonConstruct_CPropertyPage__IEAAXPEB_WI_Z(
    CPropertyPage* pThis, const wchar_t* lpszTemplateName, UINT nIDCaption) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
    pThis->m_nIDCaption = nIDCaption;
    pThis->m_bModified = FALSE;
    std::memset(PropertyPageAccess(pThis)->_propertypage_padding, 0, sizeof(PropertyPageAccess(pThis)->_propertypage_padding));
}
// Symbol: ?CommonConstruct@CPropertyPage@@IEAAXPEB_WIII@Z
extern "C" void MS_ABI impl__CommonConstruct_CPropertyPage__IEAAXPEB_WIII_Z(
    CPropertyPage* pThis, const wchar_t* lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle) {
    (void)nIDHeaderTitle;
    (void)nIDHeaderSubTitle;
    impl__CommonConstruct_CPropertyPage__IEAAXPEB_WI_Z(pThis, lpszTemplateName, nIDCaption);
}
// Symbol: ?Construct@CPropertyPage@@QEAAXII@Z
extern "C" void MS_ABI impl__Construct_CPropertyPage__QEAAXII_Z(
    CPropertyPage* pThis, UINT nIDTemplate, UINT nIDCaption) {
    impl__CommonConstruct_CPropertyPage__IEAAXPEB_WI_Z(pThis, MAKEINTRESOURCEW(nIDTemplate), nIDCaption);
    if (pThis) pThis->m_nIDHelp = nIDTemplate;
}
// Symbol: ?Construct@CPropertyPage@@QEAAXIIII@Z
extern "C" void MS_ABI impl__Construct_CPropertyPage__QEAAXIIII_Z(
    CPropertyPage* pThis, UINT nIDTemplate, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle) {
    (void)nIDHeaderTitle;
    (void)nIDHeaderSubTitle;
    impl__Construct_CPropertyPage__QEAAXII_Z(pThis, nIDTemplate, nIDCaption);
}
// Symbol: ?Construct@CPropertyPage@@QEAAXPEB_WI@Z
extern "C" void MS_ABI impl__Construct_CPropertyPage__QEAAXPEB_WI_Z(
    CPropertyPage* pThis, const wchar_t* lpszTemplateName, UINT nIDCaption) {
    impl__CommonConstruct_CPropertyPage__IEAAXPEB_WI_Z(pThis, lpszTemplateName, nIDCaption);
}
// Symbol: ?Construct@CPropertyPage@@QEAAXPEB_WIII@Z
extern "C" void MS_ABI impl__Construct_CPropertyPage__QEAAXPEB_WIII_Z(
    CPropertyPage* pThis, const wchar_t* lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle) {
    (void)nIDHeaderTitle;
    (void)nIDHeaderSubTitle;
    impl__Construct_CPropertyPage__QEAAXPEB_WI_Z(pThis, lpszTemplateName, nIDCaption);
}
// Symbol: ?Cleanup@CPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__Cleanup_CPropertyPage__IEAAXXZ(CPropertyPage* pThis) {
    if (!pThis) return;
    pThis->m_bModified = FALSE;
    g_propertyPagePspMap.erase(pThis);
}
// Symbol: ?GetMessageMap@CPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(
    const CPropertyPage* pThis) {
    (void)pThis;
    return EmptyMessageMap_Dlgcore();
}
// Symbol: ?GetRuntimeClass@CPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPropertyPage__UEBAPEAUCRuntimeClass__XZ(
    const CPropertyPage* pThis) {
    return CPropertyPage::GetThisClass();
}
// Symbol: ?GetThisClass@CPropertyPage@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPropertyPage__SAPEAUCRuntimeClass__XZ() {
    return CPropertyPage::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_Dlgcore();
}
// Symbol: ?AllocPSP@CPropertyPage@@IEAAXK@Z
extern "C" void MS_ABI impl__AllocPSP_CPropertyPage__IEAAXK_Z(CPropertyPage* pThis, DWORD dwSize) {
    (void)pThis;
    (void)dwSize;
}
// Symbol: ?EndDialog@CPropertyPage@@QEAAXH@Z
extern "C" void MS_ABI impl__EndDialog_CPropertyPage__QEAAXH_Z(CPropertyPage* pThis, int nEndID) {
    CPropertySheet* pSheet = pThis ? pThis->GetParentSheet() : nullptr;
    if (pSheet) {
        pSheet->EndDialog(nEndID);
    } else if (pThis && pThis->m_hWnd) {
        ::EndDialog(pThis->m_hWnd, nEndID);
    }
}
// Symbol: ?InitDialogInfo@CPropertyPage@@IEAAPEBUDLGTEMPLATE@@PEBU2@@Z
extern "C" const DLGTEMPLATE* MS_ABI impl__InitDialogInfo_CPropertyPage__IEAAPEBUDLGTEMPLATE__PEBU2__Z(
    CPropertyPage* pThis, const DLGTEMPLATE* pTemplate) {
    (void)pThis;
    return pTemplate;
}
// Symbol: ?IsButtonEnabled@CPropertyPage@@IEAAHH@Z
extern "C" int MS_ABI impl__IsButtonEnabled_CPropertyPage__IEAAHH_Z(CPropertyPage* pThis, int nButton) {
    CPropertySheet* pSheet = pThis ? pThis->GetParentSheet() : nullptr;
    HWND hButton = pSheet && pSheet->m_hWnd ? ::GetDlgItem(pSheet->m_hWnd, nButton) : nullptr;
    return hButton ? ::IsWindowEnabled(hButton) : FALSE;
}
// Symbol: ?MapWizardResult@CPropertyPage@@IEAA_J_J@Z
extern "C" LRESULT MS_ABI impl__MapWizardResult_CPropertyPage__IEAA_J_J_Z(CPropertyPage* pThis, LRESULT result) {
    (void)pThis;
    return result;
}
// Symbol: ?OnCtlColor@CPropertyPage@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CPropertyPage__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CPropertyPage* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    (void)pThis;
    (void)pDC;
    (void)pWnd;
    (void)nCtlColor;
    return nullptr;
}
// Symbol: ?OnNotify@CPropertyPage@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CPropertyPage__MEAAH_K_JPEA_J_Z(
    CPropertyPage* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (!pThis || !lParam) return FALSE;
    NMHDR* header = reinterpret_cast<NMHDR*>(lParam);
    LRESULT result = 0;
    switch (header->code) {
    case PSN_SETACTIVE: result = pThis->OnSetActive() ? 0 : -1; break;
    case PSN_KILLACTIVE: result = pThis->OnKillActive() ? FALSE : TRUE; break;
    case PSN_APPLY: result = pThis->OnApply() ? PSNRET_NOERROR : PSNRET_INVALID; break;
    case PSN_RESET: pThis->OnReset(); break;
    case PSN_QUERYCANCEL: result = pThis->OnQueryCancel() ? FALSE : TRUE; break;
    case PSN_WIZBACK: result = pThis->OnWizardBack(); break;
    case PSN_WIZNEXT: result = pThis->OnWizardNext(); break;
    case PSN_WIZFINISH: result = pThis->OnWizardFinish() ? FALSE : TRUE; break;
    default: return FALSE;
    }
    if (pResult) *pResult = result;
    if (pThis->GetSafeHwnd()) ::SetWindowLongPtrW(pThis->GetSafeHwnd(), DWLP_MSGRESULT, result);
    return TRUE;
}
// Symbol: ?OnWizardFinishEx@CPropertyPage@@UEAAPEAUHWND__@@XZ
extern "C" HWND MS_ABI impl__OnWizardFinishEx_CPropertyPage__UEAAPEAUHWND____XZ(CPropertyPage* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetPSP@CPropertyPage@@QEAAAEAU_PROPSHEETPAGEW@@XZ
extern "C" PROPSHEETPAGEW& MS_ABI impl__GetPSP_CPropertyPage__QEAAAEAU_PROPSHEETPAGEW__XZ(CPropertyPage* pThis) {
    PROPSHEETPAGEW& psp = g_propertyPagePspMap[pThis];
    if (psp.dwSize == 0) {
        psp.dwSize = sizeof(psp);
        psp.hInstance = AfxGetInstanceHandle();
        psp.pszTemplate = pThis ? pThis->m_lpszTemplateName : nullptr;
        psp.pfnDlgProc = PropPageDlgProc;
        psp.lParam = reinterpret_cast<LPARAM>(pThis);
    }
    return psp;
}
// Symbol: ?GetPSP@CPropertyPage@@QEBAAEBU_PROPSHEETPAGEW@@XZ
extern "C" const PROPSHEETPAGEW& MS_ABI impl__GetPSP_CPropertyPage__QEBAAEBU_PROPSHEETPAGEW__XZ(const CPropertyPage* pThis) {
    return impl__GetPSP_CPropertyPage__QEAAAEAU_PROPSHEETPAGEW__XZ(const_cast<CPropertyPage*>(pThis));
}
// Symbol: ?PreProcessPageTemplate@CPropertyPage@@IEAAXAEAU_PROPSHEETPAGEW@@H@Z
extern "C" void MS_ABI impl__PreProcessPageTemplate_CPropertyPage__IEAAXAEAU_PROPSHEETPAGEW__H_Z(
    CPropertyPage* pThis, PROPSHEETPAGEW& psp, int bWizard) {
    (void)bWizard;
    std::memset(&psp, 0, sizeof(psp));
    psp.dwSize = sizeof(psp);
    psp.dwFlags = 0;
    psp.hInstance = AfxGetInstanceHandle();
    psp.pszTemplate = pThis ? pThis->m_lpszTemplateName : nullptr;
    psp.pfnDlgProc = PropPageDlgProc;
    psp.lParam = reinterpret_cast<LPARAM>(pThis);
    if (pThis) {
        g_propertyPagePspMap[pThis] = psp;
    }
}
// Symbol: ?PreTranslateMessage@CPropertyPage@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertyPage__MEAAHPEAUtagMSG___Z(
    CPropertyPage* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
CPropertyPage::CPropertyPage()
    : CDialog(), m_nIDCaption(0), m_bModified(FALSE)
{
    memset(_propertypage_padding, 0, sizeof(_propertypage_padding));
}
CPropertyPage::CPropertyPage(unsigned int nIDTemplate, unsigned int nIDCaption)
    : CDialog(nIDTemplate, nullptr), m_nIDCaption(nIDCaption), m_bModified(FALSE)
{
    memset(_propertypage_padding, 0, sizeof(_propertypage_padding));
}
CPropertyPage::CPropertyPage(const wchar_t* lpszTemplateName, unsigned int nIDCaption)
    : CDialog(lpszTemplateName, nullptr), m_nIDCaption(nIDCaption), m_bModified(FALSE)
{
    memset(_propertypage_padding, 0, sizeof(_propertypage_padding));
}
int CPropertyPage::OnSetActive() {
    return TRUE;  // Allow page to become active
}
int CPropertyPage::OnKillActive() {
    return TRUE;  // Allow leaving the page
}
void CPropertyPage::OnOK() {
    // Default: do nothing special, just allow dialog to close
}
void CPropertyPage::OnCancel() {
    // Default: do nothing special
}
int CPropertyPage::OnApply() {
    return TRUE;  // Allow Apply
}
void CPropertyPage::OnReset() {
    // Default: do nothing
}
int CPropertyPage::OnQueryCancel() {
    return TRUE;  // Allow cancel
}
int CPropertyPage::OnWizardBack() {
    return 0;  // Use default behavior (go to previous page)
}
int CPropertyPage::OnWizardNext() {
    return 0;  // Use default behavior (go to next page)
}
int CPropertyPage::OnWizardFinish() {
    return TRUE;  // Allow finish
}
void CPropertyPage::SetModified(int bChanged) {
    m_bModified = bChanged;
    // Notify parent property sheet
    CPropertySheet* pSheet = GetParentSheet();
    if (pSheet && pSheet->m_hWnd) {
        ::SendMessageW(pSheet->m_hWnd, PSM_CHANGED, (WPARAM)m_hWnd, 0);
    }
}
int CPropertyPage::QuerySiblings(uintptr_t wParam, intptr_t lParam) {
    CPropertySheet* pSheet = GetParentSheet();
    if (pSheet && pSheet->m_hWnd) {
        return (int)::SendMessageW(pSheet->m_hWnd, PSM_QUERYSIBLINGS, wParam, lParam);
    }
    return 0;
}
void CPropertyPage::CancelToClose() {
    CPropertySheet* pSheet = GetParentSheet();
    if (pSheet && pSheet->m_hWnd) {
        ::PostMessageW(pSheet->m_hWnd, PSM_CANCELTOCLOSE, 0, 0);
    }
}
CPropertySheet* CPropertyPage::GetParentSheet() {
    if (!m_hWnd) return nullptr;
    HWND hParent = ::GetParent(m_hWnd);
    if (!hParent) return nullptr;
    auto it = g_propertySheetMap.find(hParent);
    return it != g_propertySheetMap.end() ? it->second : nullptr;
}
