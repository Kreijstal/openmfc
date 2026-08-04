// CFormView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(CView* pThis, unsigned int, CFrameWnd*);
extern "C" void MS_ABI impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(CView* pThis, CREATESTRUCTW* pCreateStruct);
extern "C" intptr_t MS_ABI impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(CScrollView* pThis, CDC* dc, unsigned int);

// Symbol: ??0CFormView@@IEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    impl___0CFormView__QEAA_PEB_W_Z(pThis, lpszTemplateName);
}
// Symbol: ??0CFormView@@IEAA@I@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    impl___0CFormView__QEAA_I_Z(pThis, nIDTemplate);
}
// Destructor
// Symbol: ??1CFormView@@UEAA@XZ
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* pThis) {
    impl___1CScrollView__UEAA_XZ(pThis);
}
// OnInitialUpdate (CFormView)
// Symbol: ?OnInitialUpdate@CFormView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis) {
    impl__OnInitialUpdate_CScrollView__UEAAXXZ(pThis);
}
// Symbol: ?GetRuntimeClass@CFormView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFormView__UEBAPEAUCRuntimeClass__XZ(const CFormView*) { return &CFormView::classCFormView; }
// Symbol: ?GetThisClass@CFormView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ() { return &CFormView::classCFormView; }
// Symbol: ?GetMessageMap@CFormView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFormView__MEBAPEBUAFX_MSGMAP__XZ(const CFormView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CFormView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// CFormView residuals.
// Symbol: ?GetOccDialogInfo@CFormView@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" void* MS_ABI impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(CFormView* pThis) {
    return pThis ? (_AFX_OCC_DIALOG_INFO*)pThis->GetOccDialogInfo() : nullptr;
}
// Symbol: ?SetOccDialogInfo@CFormView@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CFormView* pThis, void* pInfo) {
    if (!pThis) return FALSE;
    pThis->SetOccDialogInfo(static_cast<_AFX_OCC_DIALOG_INFO*>(pInfo));
    return TRUE;
}
// Symbol: ?HandleInitDialog@CFormView@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z(CFormView* pThis, uintptr_t, intptr_t) { if (pThis) pThis->OnInitialUpdate(); return TRUE; }
// Symbol: ?OnActivateFrame@CFormView@@MEAAXIPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z(CFormView* pThis, unsigned int state, CFrameWnd* frame) { impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(pThis, state, frame); }
// Symbol: ?OnActivateView@CFormView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z(CFormView* pThis, int active, CView* av, CView* dv) { impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(pThis, active, av, dv); }
// Symbol: ?OnCreate@CFormView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CFormView__IEAAHPEAUtagCREATESTRUCTW___Z(CFormView* pThis, CREATESTRUCTW* pCreateStruct) {
    if (!pThis) {
        return -1;
    }

    return impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, pCreateStruct);
}
// Symbol: ?OnPrintClient@CFormView@@IEAA_JPEAVCDC@@I@Z
extern "C" intptr_t MS_ABI impl__OnPrintClient_CFormView__IEAA_JPEAVCDC__I_Z(CFormView* pThis, CDC* dc, unsigned int flags) { return impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(pThis, dc, flags); }
// Symbol: ?OnSetFocus@CFormView@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z(CFormView* pThis, CWnd*) { if (pThis && pThis->m_hWnd) ::SetFocus(pThis->m_hWnd); }
// Symbol: ?PreTranslateMessage@CFormView@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z(CFormView* pThis, MSG* msg) { return pThis && pThis->m_hWnd && msg ? ::IsDialogMessageW(pThis->m_hWnd, msg) : FALSE; }
// Symbol: ?SaveFocusControl@CFormView@@IEAAHXZ
extern "C" int MS_ABI impl__SaveFocusControl_CFormView__IEAAHXZ(CFormView* pThis) { return pThis && pThis->m_hWnd && ::GetFocus() != nullptr; }
CFormView::~CFormView() { impl___1CFormView__UEAA_XZ(this); }
void CFormView::OnDraw(void* pDC) { impl__OnDraw_CFormView__UEAAXPEAX_Z(this, pDC); }
void CFormView::OnInitialUpdate() { impl__OnInitialUpdate_CFormView__UEAAXXZ(this); }
void CFormView::DoDataExchange(void* pDX) { impl__DoDataExchange_CFormView__UEAAXPEAX_Z(this, pDX); }
int CFormView::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                      DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd,
                      unsigned int nID, CCreateContext* pContext) {
    (void)lpszClassName;
    (void)lpszWindowName;
    (void)dwStyle;
    (void)pContext;

    if (!pParentWnd || !pParentWnd->m_hWnd) return FALSE;

    // Get resource handle
    HINSTANCE hInst = ::GetModuleHandleW(nullptr);

    // Create the form view as a dialog-based child window
    m_hWnd = ::CreateDialogParamW(
        hInst,
        m_lpszTemplateName,
        pParentWnd->m_hWnd,
        nullptr,  // Dialog proc not needed - we're using it as a child window
        0
    );

    if (!m_hWnd) return FALSE;

    // Set the control ID
    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    // Position the window
    ::SetWindowPos(m_hWnd, nullptr, rect.left, rect.top,
                   rect.right - rect.left, rect.bottom - rect.top,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    // Show the window
    ::ShowWindow(m_hWnd, SW_SHOW);

    return TRUE;
}
