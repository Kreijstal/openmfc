// CFindReplaceDialog — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/DlgcommonSupport.h"

// Symbol: ?GetRuntimeClass@CFindReplaceDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFindReplaceDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFindReplaceDialog* pThis) {
    return CFindReplaceDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CFindReplaceDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFindReplaceDialog__SAPEAUCRuntimeClass__XZ() {
    return CFindReplaceDialog::GetThisClass();
}
// Symbol: ?PostNcDestroy@CFindReplaceDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CFindReplaceDialog__MEAAXXZ(CFindReplaceDialog* pThis) {
    if (pThis) pThis->m_hWnd = nullptr;
}
// Symbol: ?Create@CFindReplaceDialog@@UEAAHHPEB_W0KPEAVCWnd@@@Z
int CFindReplaceDialog::Create(int bFindDialogOnly,
                               const wchar_t* lpszFindWhat,
                               const wchar_t* lpszReplaceWith,
                               unsigned long dwFlags,
                               CWnd* pParentWnd) {
    // Register the find/replace message if not already done
    if (s_nFindReplaceMsg == 0) {
        s_nFindReplaceMsg = RegisterWindowMessageW(FINDMSGSTRINGW);
    }

    // Copy initial strings
    if (lpszFindWhat != nullptr) {
        wcsncpy(m_szFindWhat, lpszFindWhat, 255);
        m_szFindWhat[255] = L'\0';
    }
    if (lpszReplaceWith != nullptr) {
        wcsncpy(m_szReplaceWith, lpszReplaceWith, 255);
        m_szReplaceWith[255] = L'\0';
    }

    // Setup the FINDREPLACE structure
    FINDREPLACEW* pfr = reinterpret_cast<FINDREPLACEW*>(m_fr);
    pfr->lStructSize = sizeof(FINDREPLACEW);
    pfr->hwndOwner = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    pfr->hInstance = nullptr;
    pfr->Flags = dwFlags;
    pfr->lpstrFindWhat = m_szFindWhat;
    pfr->lpstrReplaceWith = m_szReplaceWith;
    pfr->wFindWhatLen = sizeof(m_szFindWhat) / sizeof(wchar_t);
    pfr->wReplaceWithLen = sizeof(m_szReplaceWith) / sizeof(wchar_t);
    pfr->lCustData = reinterpret_cast<LPARAM>(this);  // Store 'this' for GetNotifier

    HWND hDlg;
    if (bFindDialogOnly) {
        hDlg = FindTextW(pfr);
    } else {
        hDlg = ReplaceTextW(pfr);
    }

    if (hDlg != nullptr) {
        m_hWnd = hDlg;
        return TRUE;
    }

    return FALSE;
}
CFindReplaceDialog::CFindReplaceDialog()
    : CDialog() {
    memset(m_fr, 0, sizeof(m_fr));
    memset(m_szFindWhat, 0, sizeof(m_szFindWhat));
    memset(m_szReplaceWith, 0, sizeof(m_szReplaceWith));
}
CFindReplaceDialog::~CFindReplaceDialog() {
    // Modeless dialog - destroyed via DestroyWindow when user closes
    if (m_hWnd != nullptr) {
        ::DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}
CString CFindReplaceDialog::GetFindString() const {
    return CString(m_szFindWhat);
}
CString CFindReplaceDialog::GetReplaceString() const {
    return CString(m_szReplaceWith);
}
int CFindReplaceDialog::SearchDown() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_DOWN) != 0;
}
int CFindReplaceDialog::FindNext() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_FINDNEXT) != 0;
}
int CFindReplaceDialog::MatchCase() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_MATCHCASE) != 0;
}
int CFindReplaceDialog::MatchWholeWord() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_WHOLEWORD) != 0;
}
int CFindReplaceDialog::ReplaceCurrent() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_REPLACE) != 0;
}
int CFindReplaceDialog::ReplaceAll() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_REPLACEALL) != 0;
}
int CFindReplaceDialog::IsTerminating() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_DIALOGTERM) != 0;
}
CFindReplaceDialog* CFindReplaceDialog::GetNotifier(LPARAM lParam) {
    FINDREPLACEW* pfr = reinterpret_cast<FINDREPLACEW*>(lParam);
    if (pfr != nullptr && pfr->lCustData != 0) {
        return reinterpret_cast<CFindReplaceDialog*>(pfr->lCustData);
    }
    return nullptr;
}
unsigned int CFindReplaceDialog::GetFindReplaceMessage() {
    if (s_nFindReplaceMsg == 0) {
        s_nFindReplaceMsg = RegisterWindowMessageW(FINDMSGSTRINGW);
    }
    return s_nFindReplaceMsg;
}
