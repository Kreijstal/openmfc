// CDialog — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"

// Symbol: ?CheckAutoCenter@CDialog@@UEAAHXZ
// Ordinal: 2701
extern "C" int MS_ABI impl__CheckAutoCenter_CDialog__UEAAHXZ(CDialog* pThis) {
    (void)pThis;
    return TRUE;  // Default: auto-center is enabled
}
// Symbol: ?PreTranslateMessage@CDialog@@UEAAHPEAUtagMSG@@@Z
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
// Symbol: ?Initialize@CDialog@@QEAAXXZ
// Ordinal: 7705
extern "C" void MS_ABI impl__Initialize_CDialog__QEAAXXZ(CDialog* pThis) {
    (void)pThis;
    // Default initialization - nothing to do
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEAXPEAVCWnd@@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    if (!pThis || !lpDialogTemplate) return FALSE;
    if (!hInst) hInst = AfxGetInstanceHandle();
    if (!hInst) hInst = GetModuleHandleW(nullptr);
    HWND hWndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    HWND hDlg = CreateDialogIndirectParamW(
        hInst,
        static_cast<LPCDLGTEMPLATEW>(lpDialogTemplate),
        hWndParent,
        AfxDlgProc,
        reinterpret_cast<LPARAM>(pThis));
    if (!hDlg) return FALSE;
    pThis->m_hWnd = hDlg;
    g_dlgIndirectTemplates.erase(pThis);
    g_dlgMap[hDlg] = pThis;
    return TRUE;
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAXPEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__IEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAXPEAUHINSTANCE_____Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit,
    HINSTANCE hInst) {
    (void)lpDialogInit;
    return impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
        pThis, const_cast<DLGTEMPLATE*>(lpDialogTemplate), pParentWnd, hInst);
}
// Symbol: ?CreateIndirect@CDialog@@UEAAHPEAXPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd) {
    return impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
        pThis, lpDialogTemplate, pParentWnd, nullptr);
}
// Symbol: ?CreateIndirect@CDialog@@UEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAX@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__UEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit) {
    return impl__CreateIndirect_CDialog__IEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAXPEAUHINSTANCE_____Z(
        pThis, lpDialogTemplate, pParentWnd, lpDialogInit, nullptr);
}
// Symbol: ?GetMessageMap@CDialog@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialog__MEBAPEBUAFX_MSGMAP__XZ(
    const CDialog* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetOccDialogInfo@CDialog@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" _AFX_OCC_DIALOG_INFO* MS_ABI impl__GetOccDialogInfo_CDialog__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(
    CDialog* pThis) {
    return pThis ? pThis->GetOccDialogInfo() : nullptr;
}
// Symbol: ?GetRuntimeClass@CDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialog__UEBAPEAUCRuntimeClass__XZ(
    const CDialog* pThis) {
    return CDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ() {
    return CDialog::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CDialog@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEAXPEAVCWnd@@@Z
extern "C" int MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEAXPEAVCWnd___Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd) {
    if (!pThis || !lpDialogTemplate) return FALSE;
    pThis->m_lpszTemplateName = nullptr;
    g_dlgIndirectTemplates[pThis] = static_cast<const DLGTEMPLATE*>(lpDialogTemplate);
    g_dlgParents[pThis] = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    return TRUE;
}
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAX@Z
extern "C" int MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit) {
    (void)lpDialogInit;
    return impl__InitModalIndirect_CDialog__QEAAHPEAXPEAVCWnd___Z(
        pThis, const_cast<DLGTEMPLATE*>(lpDialogTemplate), pParentWnd);
}
// Symbol: ?HandleInitDialog@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;
    return pThis ? impl__OnInitDialog_CDialog__UEAAHXZ(pThis) : TRUE;
}
// Symbol: ?OnCmdMsg@CDialog@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CDialog__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDialog* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    (void)pThis;
    (void)nID;
    (void)nCode;
    (void)pExtra;
    (void)pHandlerInfo;
    return FALSE;
}
// Symbol: ?OnCommandHelp@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnEndSession@CDialog@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEndSession_CDialog__IEAAXH_Z(CDialog* pThis, BOOL bEnding) {
    (void)pThis;
    (void)bEnding;
}
// Symbol: ?OnHelpHitTest@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnHelpHitTest_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnPaint@CDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDialog__IEAAXXZ(CDialog* pThis) {
    if (!pThis || !pThis->m_hWnd) return;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(pThis->m_hWnd, &ps);
    if (hdc) {
        EndPaint(pThis->m_hWnd, &ps);
    }
}
// Symbol: ?OnQueryEndSession@CDialog@@IEAAHXZ
extern "C" int MS_ABI impl__OnQueryEndSession_CDialog__IEAAHXZ(CDialog* pThis) {
    (void)pThis;
    return TRUE;
}
// Symbol: ?PostModal@CDialog@@IEAAXXZ
extern "C" void MS_ABI impl__PostModal_CDialog__IEAAXXZ(CDialog* pThis) {
    (void)pThis;
}
// Symbol: ?PreInitDialog@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PreInitDialog_CDialog__MEAAXXZ(CDialog* pThis) {
    (void)pThis;
}
// Symbol: ?PreModal@CDialog@@IEAAPEAUHWND__@@XZ
extern "C" HWND MS_ABI impl__PreModal_CDialog__IEAAPEAUHWND____XZ(CDialog* pThis) {
    CWnd* pParent = AfxGetMainWnd();
    HWND hWndParent = pParent ? pParent->m_hWnd : nullptr;
    return (pThis && pThis->m_hWnd) ? pThis->m_hWnd : hWndParent;
}
// Symbol: ?SetOccDialogInfo@CDialog@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CDialog__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    CDialog* pThis, _AFX_OCC_DIALOG_INFO* pDialogInfo) {
    (void)pThis;
    (void)pDialogInfo;
    return TRUE;
}
// Symbol: ?OnSetFont@CDialog@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(
    CDialog* pThis, CFont* pFont, int bRedraw) {
    if (!pThis || !pThis->m_hWnd || !pFont) return;
    ::SendMessageW(pThis->m_hWnd, WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), (LPARAM)bRedraw);
}
// Symbol: ?OnSetFont@CDialog@@UEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__UEAAXPEAVCFont___Z(
    CDialog* pThis, CFont* pFont) {
    impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(pThis, pFont, TRUE);
}
// Symbol: ?DoModal@CDialog@@UEAA_JXZ
intptr_t CDialog::DoModal() {
    return impl__DoModal_CDialog__UEAA_JXZ(this);
}
// Symbol: ?Create@CDialog@@UEAAHPEB_WPEAVCWnd@@@Z
int CDialog::Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(this, lpszTemplateName, pParentWnd);
}
// Symbol: ?OnInitDialog@CDialog@@UEAAHXZ
int CDialog::OnInitDialog() {
    return TRUE;
}
// Symbol: ?OnOK@CDialog@@MEAAXXZ
void CDialog::OnOK() {
    EndDialog(IDOK);
}
// Symbol: ?OnCancel@CDialog@@MEAAXXZ
void CDialog::OnCancel() {
    EndDialog(IDCANCEL);
}
// Symbol: ?EndDialog@CDialog@@QEAAXH@Z
void CDialog::EndDialog(int nResult) {
    if (m_hWnd) {
        ::EndDialog(m_hWnd, nResult);
    }
}
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
    g_dlgIndirectTemplates.erase(this);
}
int CDialog::Create(UINT nIDTemplate, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(this, nIDTemplate, pParentWnd);
}
void CDialog::OnSetFont(CWnd* /* pFont */) {
    // Default: do nothing
}
