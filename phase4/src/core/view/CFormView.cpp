// CFormView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


extern "C" void MS_ABI impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(CView*,int,CView*,CView*);

// Symbol: ??0CFormView@@IEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    new(pThis) CFormView(lpszTemplateName);
}
// Symbol: ??0CFormView@@IEAA@I@Z
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    new(pThis) CFormView(nIDTemplate);
}
// Symbol: ?GetRuntimeClass@CFormView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFormView__UEBAPEAUCRuntimeClass__XZ(const CFormView*) { return &CFormView::classCFormView; }
// Symbol: ?GetThisClass@CFormView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ() { return &CFormView::classCFormView; }
// Symbol: ?OnActivateView@CFormView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z(CFormView* pThis, int active, CView* av, CView* dv) { impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(pThis, active, av, dv); }
// Symbol: ?PreTranslateMessage@CFormView@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z(CFormView* pThis, MSG* msg) { return pThis && pThis->m_hWnd && msg ? ::IsDialogMessageW(pThis->m_hWnd, msg) : FALSE; }
CFormView::~CFormView() { CleanupFormView(this); }
void CFormView::OnDraw(void* pDC) { impl__OnDraw_CFormView__UEAAXPEAX_Z(this, pDC); }
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
