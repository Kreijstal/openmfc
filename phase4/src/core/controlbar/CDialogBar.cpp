// CDialogBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ?CalcFixedLayout@CDialogBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CDialogBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CDialogBar* pThis, int bStretch, int bHorz) {
    CSize size(0, 0);
    if (pThis && pThis->GetSafeHwnd()) {
        RECT rc = {};
        if (::GetWindowRect(pThis->GetSafeHwnd(), &rc)) {
            size.cx = rc.right - rc.left;
            size.cy = rc.bottom - rc.top;
        }
        if (bStretch) {
            HWND hParent = ::GetParent(pThis->GetSafeHwnd());
            if (hParent && ::GetClientRect(hParent, &rc)) {
                if (bHorz) size.cx = rc.right - rc.left;
                else size.cy = rc.bottom - rc.top;
            }
        }
    }
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?HandleInitDialog@CDialogBar@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CDialogBar__IEAA_J_K_J_Z(CDialogBar* pThis, WPARAM, LPARAM) {
    if (!pThis) return TRUE;
    return TRUE;
}
// Symbol: ?OnUpdateCmdUI@CDialogBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDialogBar__UEAAXPEAVCFrameWnd__H_Z(
    CDialogBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    (void)pTarget;
    (void)bDisableIfNoHndler;
    if (pThis && pThis->GetSafeHwnd()) {
        // Real MFC updates command UI from toolbar state; this approximation keeps
        // layout and command dispatch side effects safe for compatibility callers.
    }
}
// Symbol: ?SetOccDialogInfo@CDialogBar@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CDialogBar__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CDialogBar* pThis, _AFX_OCC_DIALOG_INFO* pInfo) {
    if (!pThis) return FALSE;
    pThis->SetOccDialogInfo(static_cast<void*>(pInfo));
    return TRUE;
}
CDialogBar::CDialogBar()
    : m_bAutoDelete(FALSE), m_pOccDialogInfo(nullptr) {
    memset(_dialogbar_padding, 0, sizeof(_dialogbar_padding));
}
CDialogBar::~CDialogBar() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}
BOOL CDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDTemplate),
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}
BOOL CDialogBar::Create(CWnd* pParentWnd, const wchar_t* lpszTemplateName,
                         UINT nStyle, UINT nID) {
    if (!pParentWnd || !lpszTemplateName) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   lpszTemplateName,
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}
void CDialogBar::UpdateData(BOOL bSaveAndValidate) {
    (void)bSaveAndValidate;
}
BOOL CDialogBar::IsVisible() const {
    return m_hWnd && ::IsWindowVisible(m_hWnd);
}
void CDialogBar::SetOccDialogInfo(void* pDialogInfo) {
    m_pOccDialogInfo = pDialogInfo;
}
