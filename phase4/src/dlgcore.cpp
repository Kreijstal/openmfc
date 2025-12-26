// Dialog Core Implementation
//
// Implements CDialog and dialog-related functionality.
// This enables dialog-based MFC applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CDialog Runtime Class
// =============================================================================

IMPLEMENT_DYNAMIC(CDialog, CWnd)
IMPLEMENT_DYNAMIC(CDialogEx, CDialog)

// =============================================================================
// Dialog Procedure
// =============================================================================

// Forward declaration
static INT_PTR CALLBACK AfxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Map HWND to CDialog* for dialog message routing
#include <map>
static std::map<HWND, CDialog*> g_dlgMap;

// =============================================================================
// CDialog Constructors
// =============================================================================

// Default constructor
// Symbol: ??0CDialog@@QAA@XZ
// Ordinal: 448
extern "C" void MS_ABI impl___0CDialog__QEAA_XZ(CDialog* pThis) {
    // Zero initialize the object
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = nullptr;
    pThis->m_nIDHelp = 0;
}

// Constructor with resource ID
// Symbol: ??0CDialog@@QAA@IPAVCWnd@@@Z
// Ordinal: 446
extern "C" void MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    (void)pParentWnd;
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    pThis->m_nIDHelp = nIDTemplate;
}

// Constructor with template name
// Symbol: ??0CDialog@@QAA@PB_WPAVCWnd@@@Z
// Ordinal: 447
extern "C" void MS_ABI impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    (void)pParentWnd;
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
}

// Destructor
// Symbol: ??1CDialog@@UAA@XZ
// Ordinal: 1089
extern "C" void MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_dlgMap.erase(pThis->m_hWnd);
        // Don't destroy - dialog should already be destroyed
        pThis->m_hWnd = nullptr;
    }
}

// =============================================================================
// CDialog::DoModal - Modal Dialog
// =============================================================================

// Symbol: ?DoModal@CDialog@@UEAA_JXZ (x64: returns intptr_t/_J)
// Ordinal: 3961
extern "C" intptr_t MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog* pThis) {
    if (!pThis || !pThis->m_lpszTemplateName) {
        return -1;  // IDABORT
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Get parent window
    CWnd* pParent = AfxGetMainWnd();
    HWND hWndParent = pParent ? pParent->m_hWnd : nullptr;

    // Store the CDialog pointer so the dialog proc can find it
    // Use a thread-local for the pending dialog
    static thread_local CDialog* s_pPendingDialog = nullptr;
    s_pPendingDialog = pThis;

    // Create the modal dialog
    INT_PTR nResult = DialogBoxParamW(
        hInst,
        pThis->m_lpszTemplateName,
        hWndParent,
        AfxDlgProc,
        reinterpret_cast<LPARAM>(pThis)
    );

    s_pPendingDialog = nullptr;
    return nResult;
}

// =============================================================================
// CDialog::Create - Modeless Dialog
// =============================================================================

// Symbol: ?Create@CDialog@@UAAHPB_WPAVCWnd@@@Z
// Ordinal: 3081
extern "C" int MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    if (!pThis) return FALSE;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    HWND hWndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;

    // Store template name
    pThis->m_lpszTemplateName = lpszTemplateName;

    // Create the modeless dialog
    HWND hDlg = CreateDialogParamW(
        hInst,
        lpszTemplateName,
        hWndParent,
        AfxDlgProc,
        reinterpret_cast<LPARAM>(pThis)
    );

    if (!hDlg) {
        return FALSE;
    }

    pThis->m_hWnd = hDlg;
    g_dlgMap[hDlg] = pThis;

    return TRUE;
}

// Create with resource ID
extern "C" int MS_ABI impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
        pThis, MAKEINTRESOURCEW(nIDTemplate), pParentWnd);
}

// =============================================================================
// CDialog::EndDialog
// =============================================================================

// Symbol: ?EndDialog@CDialog@@QAAXH@Z
// Ordinal: 4353
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult) {
    if (pThis && pThis->m_hWnd) {
        ::EndDialog(pThis->m_hWnd, nResult);
    }
}

// =============================================================================
// CDialog::OnInitDialog
// =============================================================================

// Symbol: ?OnInitDialog@CDialog@@UAAHXZ
// Ordinal: 10170
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis) {
    (void)pThis;
    // Default implementation - return TRUE to set focus to first control
    return TRUE;
}

// =============================================================================
// CDialog::OnOK / OnCancel
// =============================================================================

// Symbol: ?OnOK@CDialog@@MAAXXZ
// Ordinal: 10711
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis) {
    // Call stub directly to avoid needing member function implementation
    impl__EndDialog_CDialog__QEAAXH_Z(pThis, IDOK);
}

// Symbol: ?OnCancel@CDialog@@MAAXXZ
// Ordinal: 8738
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* pThis) {
    // Call stub directly to avoid needing member function implementation
    impl__EndDialog_CDialog__QEAAXH_Z(pThis, IDCANCEL);
}

// =============================================================================
// CDialog::CheckAutoCenter
// =============================================================================

// Symbol: ?CheckAutoCenter@CDialog@@UAAHXZ
// Ordinal: 2701
extern "C" int MS_ABI impl__CheckAutoCenter_CDialog__UEAAHXZ(CDialog* pThis) {
    (void)pThis;
    return TRUE;  // Default: auto-center is enabled
}

// =============================================================================
// CDialog::PreTranslateMessage
// =============================================================================

// Symbol: ?PreTranslateMessage@CDialog@@UAAHPAUtagMSG@@@Z
// Ordinal: 11861
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
    CDialog* pThis, MSG* pMsg)
{
    if (pThis && pThis->m_hWnd && pMsg) {
        // Handle dialog navigation keys
        if (IsDialogMessageW(pThis->m_hWnd, pMsg)) {
            return TRUE;
        }
    }
    return FALSE;
}

// =============================================================================
// CDialog::Initialize
// =============================================================================

// Symbol: ?Initialize@CDialog@@QAAXXZ
// Ordinal: 7705
extern "C" void MS_ABI impl__Initialize_CDialog__QEAAXXZ(CDialog* pThis) {
    (void)pThis;
    // Default initialization - nothing to do
}

// =============================================================================
// Dialog Procedure
// =============================================================================

static INT_PTR CALLBACK AfxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CDialog* pDlg = nullptr;

    if (message == WM_INITDIALOG) {
        // lParam contains the CDialog pointer
        pDlg = reinterpret_cast<CDialog*>(lParam);
        if (pDlg) {
            pDlg->m_hWnd = hDlg;
            g_dlgMap[hDlg] = pDlg;
        }

        // Call OnInitDialog
        if (pDlg) {
            return pDlg->OnInitDialog() ? TRUE : FALSE;
        }
        return TRUE;
    }

    // Look up CDialog from HWND
    auto it = g_dlgMap.find(hDlg);
    if (it != g_dlgMap.end()) {
        pDlg = it->second;
    }

    if (!pDlg) {
        return FALSE;  // Not handled
    }

    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            pDlg->OnOK();
            return TRUE;
        case IDCANCEL:
            pDlg->OnCancel();
            return TRUE;
        }
        break;

    case WM_CLOSE:
        pDlg->OnCancel();
        return TRUE;

    case WM_DESTROY:
        g_dlgMap.erase(hDlg);
        pDlg->m_hWnd = nullptr;
        break;
    }

    return FALSE;  // Not handled - use default dialog processing
}

// =============================================================================
// CDialogEx Implementation
// =============================================================================

// CDialogEx default constructor
extern "C" void MS_ABI impl___0CDialogEx__QEAA_XZ(CDialogEx* pThis) {
    impl___0CDialog__QEAA_XZ(pThis);
    // Additional CDialogEx initialization
}

// CDialogEx constructor with ID
extern "C" void MS_ABI impl___0CDialogEx__QEAA_IPEAVCWnd___Z(
    CDialogEx* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    impl___0CDialog__QEAA_IPEAVCWnd___Z(pThis, nIDTemplate, pParentWnd);
}

// CDialogEx constructor with template name
extern "C" void MS_ABI impl___0CDialogEx__QEAA_PEB_WPEAVCWnd___Z(
    CDialogEx* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(pThis, lpszTemplateName, pParentWnd);
}

// =============================================================================
// Helper: GetDlgItem wrapper
// =============================================================================

extern CWnd* OpenMfcAttachCWnd(HWND hWnd);

// CWnd::GetDlgItem
// Symbol: ?GetDlgItem@CWnd@@QEBAPEAV1@H@Z
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }

    HWND hCtrl = ::GetDlgItem(pThis->m_hWnd, nID);
    if (!hCtrl) {
        return nullptr;
    }

    return OpenMfcAttachCWnd(hCtrl);
}

// CWnd::GetDlgItem (HWND** overload)
// Symbol: ?GetDlgItem@CWnd@@QEBAXHPEAPEAUHWND__@@@Z
extern "C" void MS_ABI impl__GetDlgItem_CWnd__QEBAXHPEAPEAUHWND_____Z(
    const CWnd* pThis, int nID, HWND* pWnd) {
    if (!pWnd) {
        return;
    }
    *pWnd = nullptr;
    if (!pThis || !pThis->m_hWnd) {
        return;
    }
    *pWnd = ::GetDlgItem(pThis->m_hWnd, nID);
}

// =============================================================================
// Dialog Data Exchange helpers
// =============================================================================

// Get text from dialog control
extern "C" int MS_ABI impl__GetDlgItemText_CWnd__QEAAHHAEAV_CStringT___Z(
    CWnd* pThis, int nID, CString* rString)
{
    if (!pThis || !pThis->m_hWnd || !rString) {
        return 0;
    }

    HWND hCtrl = ::GetDlgItem(pThis->m_hWnd, nID);
    if (!hCtrl) {
        rString->Empty();
        return 0;
    }

    int nLen = ::GetWindowTextLengthW(hCtrl);
    if (nLen == 0) {
        rString->Empty();
        return 0;
    }

    // GetBuffer(nLen + 1) to accommodate null terminator for GetWindowTextW
    wchar_t* pBuf = rString->GetBuffer(nLen + 1);
    nLen = ::GetWindowTextW(hCtrl, pBuf, nLen + 1);
    rString->ReleaseBuffer(nLen);
    return nLen;
}

// Set text on dialog control
extern "C" void MS_ABI impl__SetDlgItemText_CWnd__QEAAXHPEB_W_Z(
    CWnd* pThis, int nID, const wchar_t* lpszString)
{
    if (!pThis || !pThis->m_hWnd) {
        return;
    }

    HWND hCtrl = ::GetDlgItem(pThis->m_hWnd, nID);
    if (hCtrl) {
        ::SetWindowTextW(hCtrl, lpszString ? lpszString : L"");
    }
}

// =============================================================================
// CDialog member functions (for vtable usage)
// These are needed by derived classes (CColorDialog, CFileDialog, etc.)
// =============================================================================

CDialog::CDialog() {
    m_hWnd = nullptr;
    m_lpszTemplateName = nullptr;
    m_nIDHelp = 0;
}

CDialog::CDialog(UINT nIDTemplate, CWnd* pParentWnd) {
    (void)pParentWnd;
    m_hWnd = nullptr;
    m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    m_nIDHelp = nIDTemplate;
}

CDialog::CDialog(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    (void)pParentWnd;
    m_hWnd = nullptr;
    m_lpszTemplateName = lpszTemplateName;
    m_nIDHelp = 0;
}

CDialog::~CDialog() {
    if (m_hWnd) {
        g_dlgMap.erase(m_hWnd);
        m_hWnd = nullptr;
    }
}

intptr_t CDialog::DoModal() {
    return impl__DoModal_CDialog__UEAA_JXZ(this);
}

int CDialog::Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(this, lpszTemplateName, pParentWnd);
}

int CDialog::Create(UINT nIDTemplate, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(this, nIDTemplate, pParentWnd);
}

int CDialog::OnInitDialog() {
    return TRUE;
}

void CDialog::OnOK() {
    EndDialog(IDOK);
}

void CDialog::OnCancel() {
    EndDialog(IDCANCEL);
}

void CDialog::OnSetFont(CWnd* /* pFont */) {
    // Default: do nothing
}

void CDialog::EndDialog(int nResult) {
    if (m_hWnd) {
        ::EndDialog(m_hWnd, nResult);
    }
}

// =============================================================================
// CPropertyPage Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CPropertyPage, CDialog)

// Default constructor
CPropertyPage::CPropertyPage()
    : CDialog(), m_nIDCaption(0), m_bModified(FALSE)
{
    memset(_propertypage_padding, 0, sizeof(_propertypage_padding));
}

// Constructor with resource ID
CPropertyPage::CPropertyPage(unsigned int nIDTemplate, unsigned int nIDCaption)
    : CDialog(nIDTemplate, nullptr), m_nIDCaption(nIDCaption), m_bModified(FALSE)
{
    memset(_propertypage_padding, 0, sizeof(_propertypage_padding));
}

// Constructor with template name
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
    // Try to find CPropertySheet from the window map
    auto it = g_dlgMap.find(hParent);
    if (it != g_dlgMap.end()) {
        return dynamic_cast<CPropertySheet*>(reinterpret_cast<CWnd*>(it->second));
    }
    return nullptr;
}

// =============================================================================
// CPropertySheet Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CPropertySheet, CWnd)

// Property sheet callback for modeless sheets
static int CALLBACK PropSheetCallback(HWND hDlg, UINT message, LPARAM lParam) {
    (void)lParam;
    if (message == PSCB_INITIALIZED) {
        // Sheet is initialized
        (void)hDlg;
    }
    return 0;
}

// Property page dialog proc
static INT_PTR CALLBACK PropPageDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CPropertyPage* pPage = nullptr;

    if (message == WM_INITDIALOG) {
        // lParam is PROPSHEETPAGE*
        PROPSHEETPAGEW* psp = reinterpret_cast<PROPSHEETPAGEW*>(lParam);
        if (psp) {
            pPage = reinterpret_cast<CPropertyPage*>(psp->lParam);
            if (pPage) {
                pPage->m_hWnd = hDlg;
                SetWindowLongPtrW(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pPage));
            }
        }
        if (pPage) {
            return pPage->OnInitDialog() ? TRUE : FALSE;
        }
        return TRUE;
    }

    pPage = reinterpret_cast<CPropertyPage*>(GetWindowLongPtrW(hDlg, DWLP_USER));
    if (!pPage) {
        return FALSE;
    }

    switch (message) {
    case WM_NOTIFY: {
        NMHDR* pnmh = reinterpret_cast<NMHDR*>(lParam);
        switch (pnmh->code) {
        case PSN_SETACTIVE:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnSetActive() ? 0 : -1);
            return TRUE;
        case PSN_KILLACTIVE:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnKillActive() ? FALSE : TRUE);
            return TRUE;
        case PSN_APPLY:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnApply() ? PSNRET_NOERROR : PSNRET_INVALID);
            return TRUE;
        case PSN_RESET:
            pPage->OnReset();
            return TRUE;
        case PSN_QUERYCANCEL:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnQueryCancel() ? FALSE : TRUE);
            return TRUE;
        case PSN_WIZBACK:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnWizardBack());
            return TRUE;
        case PSN_WIZNEXT:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnWizardNext());
            return TRUE;
        case PSN_WIZFINISH:
            SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, pPage->OnWizardFinish() ? FALSE : TRUE);
            return TRUE;
        }
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            pPage->OnOK();
            return TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            pPage->OnCancel();
            return TRUE;
        }
        break;
    }

    return FALSE;
}

// Default constructor
CPropertySheet::CPropertySheet()
    : CWnd(), m_pszCaption(nullptr), m_pParentWnd(nullptr),
      m_nActivePage(0), m_bWizardMode(FALSE), m_nPageCount(0)
{
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}

// Constructor with caption ID
CPropertySheet::CPropertySheet(unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage)
    : CWnd(), m_pszCaption(nullptr), m_pParentWnd(pParentWnd),
      m_nActivePage(iSelectPage), m_bWizardMode(FALSE), m_nPageCount(0)
{
    (void)nIDCaption;  // Would load from resources
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}

// Constructor with caption string
CPropertySheet::CPropertySheet(const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage)
    : CWnd(), m_pszCaption(pszCaption), m_pParentWnd(pParentWnd),
      m_nActivePage(iSelectPage), m_bWizardMode(FALSE), m_nPageCount(0)
{
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}

int CPropertySheet::GetPageCount() const {
    return m_nPageCount;
}

CPropertyPage* CPropertySheet::GetActivePage() const {
    if (m_hWnd) {
        int nActive = (int)PropSheet_HwndToIndex(m_hWnd, (HWND)PropSheet_GetCurrentPageHwnd(m_hWnd));
        if (nActive >= 0 && nActive < m_nPageCount) {
            return m_pages[nActive];
        }
    }
    if ((int)m_nActivePage < m_nPageCount) {
        return m_pages[m_nActivePage];
    }
    return nullptr;
}

int CPropertySheet::GetActiveIndex() const {
    if (m_hWnd) {
        return (int)PropSheet_HwndToIndex(m_hWnd, (HWND)PropSheet_GetCurrentPageHwnd(m_hWnd));
    }
    return (int)m_nActivePage;
}

CPropertyPage* CPropertySheet::GetPage(int nPage) const {
    if (nPage >= 0 && nPage < m_nPageCount) {
        return m_pages[nPage];
    }
    return nullptr;
}

int CPropertySheet::GetPageIndex(CPropertyPage* pPage) const {
    for (int i = 0; i < m_nPageCount; i++) {
        if (m_pages[i] == pPage) {
            return i;
        }
    }
    return -1;
}

int CPropertySheet::SetActivePage(int nPage) {
    if (m_hWnd) {
        return PropSheet_SetCurSel(m_hWnd, nullptr, nPage);
    }
    if (nPage >= 0 && nPage < m_nPageCount) {
        m_nActivePage = nPage;
        return TRUE;
    }
    return FALSE;
}

int CPropertySheet::SetActivePage(CPropertyPage* pPage) {
    int nIndex = GetPageIndex(pPage);
    if (nIndex >= 0) {
        return SetActivePage(nIndex);
    }
    return FALSE;
}

void CPropertySheet::SetTitle(const wchar_t* lpszText, unsigned int nStyle) {
    m_pszCaption = lpszText;
    if (m_hWnd) {
        PropSheet_SetTitle(m_hWnd, nStyle, lpszText);
    }
}

void CPropertySheet::SetWizardMode() {
    m_bWizardMode = TRUE;
}

void CPropertySheet::SetWizardButtons(unsigned long dwFlags) {
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, PSM_SETWIZBUTTONS, 0, (LPARAM)dwFlags);
    }
}

void CPropertySheet::SetFinishText(const wchar_t* lpszText) {
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, PSM_SETFINISHTEXT, 0, (LPARAM)lpszText);
    }
}

void CPropertySheet::AddPage(CPropertyPage* pPage) {
    if (m_nPageCount < 16 && pPage) {
        m_pages[m_nPageCount++] = pPage;
        // If sheet is already created, add page dynamically
        if (m_hWnd && pPage->m_lpszTemplateName) {
            PROPSHEETPAGEW psp = {};
            psp.dwSize = sizeof(psp);
            psp.dwFlags = PSP_DLGINDIRECT;
            psp.hInstance = AfxGetInstanceHandle();
            psp.pszTemplate = pPage->m_lpszTemplateName;
            psp.pfnDlgProc = PropPageDlgProc;
            psp.lParam = reinterpret_cast<LPARAM>(pPage);
            HPROPSHEETPAGE hPage = CreatePropertySheetPageW(&psp);
            if (hPage) {
                PropSheet_AddPage(m_hWnd, hPage);
            }
        }
    }
}

void CPropertySheet::RemovePage(CPropertyPage* pPage) {
    int nIndex = GetPageIndex(pPage);
    if (nIndex >= 0) {
        RemovePage(nIndex);
    }
}

void CPropertySheet::RemovePage(int nPage) {
    if (nPage >= 0 && nPage < m_nPageCount) {
        if (m_hWnd) {
            PropSheet_RemovePage(m_hWnd, nPage, nullptr);
        }
        // Shift pages down
        for (int i = nPage; i < m_nPageCount - 1; i++) {
            m_pages[i] = m_pages[i + 1];
        }
        m_pages[--m_nPageCount] = nullptr;
    }
}

void CPropertySheet::PressButton(int nButton) {
    if (m_hWnd) {
        ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)nButton, 0);
    }
}

void CPropertySheet::EndDialog(int nEndID) {
    if (m_hWnd) {
        // For modal property sheets, post a message to close
        if (nEndID == IDOK) {
            ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)PSBTN_OK, 0);
        } else {
            ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)PSBTN_CANCEL, 0);
        }
    }
}

intptr_t CPropertySheet::DoModal() {
    if (m_nPageCount == 0) {
        return -1;
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Build array of PROPSHEETPAGE structures
    PROPSHEETPAGEW* pPages = new PROPSHEETPAGEW[m_nPageCount];
    memset(pPages, 0, sizeof(PROPSHEETPAGEW) * m_nPageCount);

    for (int i = 0; i < m_nPageCount; i++) {
        pPages[i].dwSize = sizeof(PROPSHEETPAGEW);
        pPages[i].dwFlags = 0;
        pPages[i].hInstance = hInst;
        pPages[i].pszTemplate = m_pages[i]->m_lpszTemplateName;
        pPages[i].pfnDlgProc = PropPageDlgProc;
        pPages[i].lParam = reinterpret_cast<LPARAM>(m_pages[i]);
    }

    // Build property sheet header
    PROPSHEETHEADERW psh = {};
    psh.dwSize = sizeof(psh);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USECALLBACK;
    if (m_bWizardMode) {
        psh.dwFlags |= PSH_WIZARD;
    }
    psh.hwndParent = m_pParentWnd ? m_pParentWnd->m_hWnd : nullptr;
    psh.hInstance = hInst;
    psh.pszCaption = m_pszCaption;
    psh.nPages = m_nPageCount;
    psh.nStartPage = m_nActivePage;
    psh.ppsp = pPages;
    psh.pfnCallback = PropSheetCallback;

    // Show the property sheet
    INT_PTR nResult = PropertySheetW(&psh);

    delete[] pPages;

    // Convert result: positive = IDOK, 0 = IDCANCEL, negative = error
    if (nResult > 0) {
        return IDOK;
    } else if (nResult == 0) {
        return IDCANCEL;
    }
    return -1;
}

int CPropertySheet::Create(CWnd* pParentWnd, unsigned long dwStyle, unsigned long dwExStyle) {
    if (m_nPageCount == 0) {
        return FALSE;
    }

    (void)dwStyle;
    (void)dwExStyle;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Build array of PROPSHEETPAGE structures
    PROPSHEETPAGEW* pPages = new PROPSHEETPAGEW[m_nPageCount];
    memset(pPages, 0, sizeof(PROPSHEETPAGEW) * m_nPageCount);

    for (int i = 0; i < m_nPageCount; i++) {
        pPages[i].dwSize = sizeof(PROPSHEETPAGEW);
        pPages[i].dwFlags = 0;
        pPages[i].hInstance = hInst;
        pPages[i].pszTemplate = m_pages[i]->m_lpszTemplateName;
        pPages[i].pfnDlgProc = PropPageDlgProc;
        pPages[i].lParam = reinterpret_cast<LPARAM>(m_pages[i]);
    }

    // Build property sheet header for modeless
    PROPSHEETHEADERW psh = {};
    psh.dwSize = sizeof(psh);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_MODELESS | PSH_USECALLBACK;
    if (m_bWizardMode) {
        psh.dwFlags |= PSH_WIZARD;
    }
    psh.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    psh.hInstance = hInst;
    psh.pszCaption = m_pszCaption;
    psh.nPages = m_nPageCount;
    psh.nStartPage = m_nActivePage;
    psh.ppsp = pPages;
    psh.pfnCallback = PropSheetCallback;

    // Create the modeless property sheet
    m_hWnd = (HWND)PropertySheetW(&psh);
    m_pParentWnd = pParentWnd;

    delete[] pPages;

    return m_hWnd != nullptr;
}

int CPropertySheet::OnInitDialog() {
    return TRUE;
}

void CPropertySheet::OnPageChanged() {
    // Default: do nothing
}
