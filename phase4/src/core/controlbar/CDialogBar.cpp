// CDialogBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/RegcoreSupport.h"
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"
#include <new>

extern "C" BOOL MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd*, DWORD, DWORD, UINT);

CSize CDialogBar::CalcFixedLayout(BOOL stretch, BOOL horizontal) {
    return CSize(stretch && horizontal ? 32767 : m_sizeDefault.cx,
                 stretch && !horizontal ? 32767 : m_sizeDefault.cy);
}

// Symbol: ?CalcFixedLayout@CDialogBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CDialogBar__UEAA_AVCSize__HH_Z(
    CDialogBar* pThis, CSize* result, BOOL stretch, BOOL horizontal) {
    return new (result) CSize(pThis->CDialogBar::CalcFixedLayout(stretch, horizontal));
}

LRESULT CDialogBar::HandleInitDialog(WPARAM, LPARAM) {
    Default();
    COccManager* manager = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_pOccManager;
    if (manager && m_pOccDialogInfo) {
        using Create = BOOL (MS_ABI*)(COccManager*, CWnd*, const wchar_t*, _AFX_OCC_DIALOG_INFO*);
        if (!openmfc::detail::WindowSlot<Create>(manager, 9)(manager, this, m_lpszTemplateName, m_pOccDialogInfo))
            return FALSE;
    }
    LoadDynamicLayoutResource(m_lpszTemplateName);
    return FALSE;
}

// Symbol: ?HandleInitDialog@CDialogBar@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__HandleInitDialog_CDialogBar__IEAA_J_K_J_Z(
    CDialogBar* bar, WPARAM wParam, LPARAM lParam) {
    return bar ? bar->HandleInitDialog(wParam, lParam) : FALSE;
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
    m_bAutoDelete = FALSE;
    CControlBar::DestroyWindow();
}

BOOL CDialogBar::Create(CWnd* parent, UINT templateId, UINT style, UINT id) {
    return Create(parent, MAKEINTRESOURCEW(templateId), style, id);
}

BOOL CDialogBar::Create(CWnd* parent, const wchar_t* name, UINT style, UINT id) {
    if (m_hWnd || !name) return FALSE;
    m_dwStyle = style & CBRS_ALL;

    CREATESTRUCTW create{};
    create.hInstance = AfxGetInstanceHandle();
    create.hwndParent = parent ? parent->m_hWnd : nullptr;
    create.hMenu = reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id));
    create.style = style | WS_CHILD;
    create.lpszClass = L"AfxControlBar140u";
    if (!PreCreateWindow(create)) return FALSE;

    m_lpszTemplateName = name;
    BOOL created;
    try {
        created = CreateDlg(name, parent);
    } catch (...) {
        m_lpszTemplateName = nullptr;
        throw;
    }
    m_lpszTemplateName = nullptr;
    if (!created) return FALSE;

    SetDlgCtrlID(id);
    RECT rect{};
    ::GetWindowRect(m_hWnd, &rect);
    m_sizeDefault = CSize(rect.right - rect.left, rect.bottom - rect.top);
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(this, 0, WS_CLIPSIBLINGS, 0);
    if (!ExecuteDlgInit(name)) {
        CControlBar::DestroyWindow();
        return FALSE;
    }
    ::SetWindowPos(m_hWnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    return TRUE;
}

BOOL CDialogBar::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* info) {
    m_pOccDialogInfo = info;
    return TRUE;
}

BEGIN_MESSAGE_MAP(CDialogBar, CControlBar)
    ON_MESSAGE(WM_INITDIALOG, &ThisClass::HandleInitDialog)
END_MESSAGE_MAP()

// Symbol: ?GetMessageMap@CDialogBar@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialogBar__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return CDialogBar::GetThisMessageMap();
}

// Symbol: ?GetThisMessageMap@CDialogBar@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogBar__KAPEBUAFX_MSGMAP__XZ() {
    return CDialogBar::GetThisMessageMap();
}
