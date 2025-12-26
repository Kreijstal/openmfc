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
