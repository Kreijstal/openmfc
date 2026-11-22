// CDialogBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

CSize CDialogBar::CalcFixedLayout(BOOL stretch, BOOL horizontal) {
    return CSize(stretch && horizontal ? 32767 : m_sizeDefault.cx,
                 stretch && !horizontal ? 32767 : m_sizeDefault.cy);
}

// Symbol: ?CalcFixedLayout@CDialogBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CDialogBar__UEAA_AVCSize__HH_Z(
    CDialogBar* pThis, CSize* result, BOOL stretch, BOOL horizontal) {
    return new (result) CSize(pThis->CDialogBar::CalcFixedLayout(stretch, horizontal));
}

// Symbol: ?HandleInitDialog@CDialogBar@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CDialogBar__IEAA_J_K_J_Z(CDialogBar* pThis, WPARAM, LPARAM) {
    if (!pThis) return TRUE;
    return TRUE;
}
void CDialogBar::OnUpdateCmdUI(CFrameWnd* target, BOOL disableIfNoHandler) {
    UpdateDialogControls(target, disableIfNoHandler);
}

// Symbol: ?OnUpdateCmdUI@CDialogBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDialogBar__UEAAXPEAVCFrameWnd__H_Z(
    CDialogBar* pThis, CFrameWnd* target, BOOL disableIfNoHandler) {
    pThis->CDialogBar::OnUpdateCmdUI(target, disableIfNoHandler);
}

// Symbol: ?SetOccDialogInfo@CDialogBar@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" BOOL MS_ABI impl__SetOccDialogInfo_CDialogBar__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    CDialogBar* pThis, _AFX_OCC_DIALOG_INFO* info) {
    return pThis->CDialogBar::SetOccDialogInfo(info);
}

CDialogBar::CDialogBar()
    : m_pOccDialogInfo(nullptr), m_lpszTemplateName(nullptr) {
}

static_assert(sizeof(CDialogBar) == 352);

CDialogBar::~CDialogBar() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}
BOOL CDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) {
    return Create(pParentWnd, MAKEINTRESOURCEW(nIDTemplate), nStyle, nID);
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

    m_lpszTemplateName = lpszTemplateName;
    RECT rect = {};
    ::GetWindowRect(m_hWnd, &rect);
    m_sizeDefault = CSize(rect.right - rect.left, rect.bottom - rect.top);

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}
void CDialogBar::UpdateData(BOOL bSaveAndValidate) {
    (void)bSaveAndValidate;
}
BOOL CDialogBar::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* info) {
    m_pOccDialogInfo = info;
    return TRUE;
}
