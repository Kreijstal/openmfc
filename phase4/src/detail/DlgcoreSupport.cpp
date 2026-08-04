#include "DlgcoreSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAXPEAUHBITMAP____W4BackgroundLocation_1_HH_Z(
    CDialogEx* pThis, HBITMAP hBitmap, int backgroundLocation, BOOL bAutoDestroy, BOOL bRepaint);

IMPLEMENT_DYNAMIC(CDialog, CWnd)
IMPLEMENT_DYNAMIC(CDialogEx, CDialog)
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult) {
    if (pThis && pThis->m_hWnd) {
        ::EndDialog(pThis->m_hWnd, nResult);
    }
}
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis) {
    (void)pThis;
    // Default implementation - return TRUE to set focus to first control
    return TRUE;
}
INT_PTR CALLBACK AfxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
IMPLEMENT_DYNAMIC(CPropertyPage, CDialog)
IMPLEMENT_DYNAMIC(CPropertySheet, CWnd)
INT_PTR CALLBACK PropPageDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
namespace openmfc { namespace detail { namespace dlgcore {
std::map<HWND, CDialog*> g_dlgMap;
std::map<CDialog*, const DLGTEMPLATE*> g_dlgIndirectTemplates;
std::map<CDialog*, HWND> g_dlgParents;
std::map<CPropertyPage*, PROPSHEETPAGEW> g_propertyPagePspMap;
std::map<HWND, CPropertySheet*> g_propertySheetMap;
std::map<CPropertySheet*, CString> g_propertySheetCaptions;
thread_local CPropertySheet* g_pendingPropertySheet = nullptr;
const AFX_MSGMAP* EmptyMessageMap_Dlgcore() {
    static const AFX_MSGMAP_ENTRY entries[] = {{0, 0, 0, 0, 0, nullptr}};
    static const AFX_MSGMAP msgMap = {nullptr, entries};
    return &msgMap;
}
std::map<CDialogEx*, CDialogExBackgroundState> g_dialogExBackgroundState;
CDialogExAccess* DialogExAccess(CDialogEx* pThis) {
    return static_cast<CDialogExAccess*>(pThis);
}
CPropertyPageAccess* PropertyPageAccess(CPropertyPage* pThis) {
    return static_cast<CPropertyPageAccess*>(pThis);
}
CPropertySheetAccess* PropertySheetAccess(CPropertySheet* pThis) {
    return static_cast<CPropertySheetAccess*>(pThis);
}
extern "C" void* MS_ABI impl___0CDialog__QEAA_XZ(CDialog* pThis) {
    // Zero initialize the object
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = nullptr;
    pThis->m_nIDHelp = 0;
    g_dlgParents.erase(pThis);
    return pThis;
}
extern "C" void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    pThis->m_nIDHelp = nIDTemplate;
    g_dlgParents[pThis] = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    return pThis;
}
extern "C" void* MS_ABI impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    pThis->m_hWnd = nullptr;
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
    g_dlgParents[pThis] = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    return pThis;
}
extern "C" void MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_dlgMap.erase(pThis->m_hWnd);
        // Don't destroy - dialog should already be destroyed
        pThis->m_hWnd = nullptr;
    }
    if (pThis) {
        g_dlgIndirectTemplates.erase(pThis);
        g_dlgParents.erase(pThis);
    }
}
extern "C" intptr_t MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog* pThis) {
    auto indirectIt = g_dlgIndirectTemplates.find(pThis);
    if (!pThis || (!pThis->m_lpszTemplateName && indirectIt == g_dlgIndirectTemplates.end())) {
        return -1;  // IDABORT
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Get parent window
    auto parentIt = g_dlgParents.find(pThis);
    CWnd* pParent = AfxGetMainWnd();
    HWND hWndParent = parentIt != g_dlgParents.end() ? parentIt->second : (pParent ? pParent->m_hWnd : nullptr);

    // Store the CDialog pointer so the dialog proc can find it
    // Use a thread-local for the pending dialog
    thread_local CDialog* s_pPendingDialog = nullptr;
    s_pPendingDialog = pThis;

    INT_PTR nResult = 0;
    if (indirectIt != g_dlgIndirectTemplates.end()) {
        nResult = DialogBoxIndirectParamW(
            hInst,
            indirectIt->second,
            hWndParent,
            AfxDlgProc,
            reinterpret_cast<LPARAM>(pThis));
    } else {
        nResult = DialogBoxParamW(
            hInst,
            pThis->m_lpszTemplateName,
            hWndParent,
            AfxDlgProc,
            reinterpret_cast<LPARAM>(pThis));
    }

    s_pPendingDialog = nullptr;
    return nResult;
}
extern "C" int MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    if (!pThis) return FALSE;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    HWND hWndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    g_dlgParents[pThis] = hWndParent;

    // Store template name
    pThis->m_lpszTemplateName = lpszTemplateName;
    g_dlgIndirectTemplates.erase(pThis);

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
extern "C" int MS_ABI impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
        pThis, MAKEINTRESOURCEW(nIDTemplate), pParentWnd);
}
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis) {
    // Call stub directly to avoid needing member function implementation
    impl__EndDialog_CDialog__QEAAXH_Z(pThis, IDOK);
}
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* pThis) {
    // Call stub directly to avoid needing member function implementation
    impl__EndDialog_CDialog__QEAAXH_Z(pThis, IDCANCEL);
}
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_XZ(CDialogEx* pThis) {
    impl___0CDialog__QEAA_XZ(pThis);
    impl__CommonConstruct_CDialogEx__IEAAXXZ(pThis);
    return pThis;
}
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_IPEAVCWnd___Z(
    CDialogEx* pThis, UINT nIDTemplate, CWnd* pParentWnd)
{
    impl___0CDialog__QEAA_IPEAVCWnd___Z(pThis, nIDTemplate, pParentWnd);
    impl__CommonConstruct_CDialogEx__IEAAXXZ(pThis);
    return pThis;
}
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_PEB_WPEAVCWnd___Z(
    CDialogEx* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd)
{
    impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(pThis, lpszTemplateName, pParentWnd);
    impl__CommonConstruct_CDialogEx__IEAAXXZ(pThis);
    return pThis;
}
extern "C" int MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAHIW4BackgroundLocation_1_H_Z(
    CDialogEx* pThis, UINT uiBmpResId, int backgroundLocation, BOOL bRepaint) {
    if (!pThis || uiBmpResId == 0) return FALSE;
    HINSTANCE instance = AfxGetInstanceHandle();
    if (!instance) instance = ::GetModuleHandleW(nullptr);
    HBITMAP bitmap = static_cast<HBITMAP>(::LoadImageW(
        instance, MAKEINTRESOURCEW(uiBmpResId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
    if (!bitmap) return FALSE;
    impl__SetBackgroundImage_CDialogEx__QEAAXPEAUHBITMAP____W4BackgroundLocation_1_HH_Z(
        pThis, bitmap, backgroundLocation, TRUE, bRepaint);
    return TRUE;
}
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
int CALLBACK PropSheetCallback(HWND hDlg, UINT message, LPARAM lParam) {
    (void)lParam;
    if (message == PSCB_INITIALIZED) {
        if (g_pendingPropertySheet) {
            g_pendingPropertySheet->m_hWnd = hDlg;
            g_propertySheetMap[hDlg] = g_pendingPropertySheet;
        }
    }
    return 0;
}
} } }  // namespace openmfc::detail::dlgcore
