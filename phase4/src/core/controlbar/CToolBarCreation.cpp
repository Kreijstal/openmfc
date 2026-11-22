#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

BOOL CToolBar::Create(CWnd* parent, DWORD style, UINT id) {
    return CreateEx(parent, 0, style,
                    CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), id);
}

BOOL CToolBar::CreateEx(CWnd* parent, DWORD controlStyle, DWORD style, CRect borders, UINT id) {
    if (!parent || !parent->m_hWnd || m_hWnd) return FALSE;
    INITCOMMONCONTROLSEX controls{sizeof(controls), ICC_BAR_CLASSES};
    if (!::InitCommonControlsEx(&controls)) return FALSE;

    m_cxLeftBorder = borders.left;
    m_cyTopBorder = borders.top;
    m_cxRightBorder = borders.right;
    m_cyBottomBorder = borders.bottom;
    m_dwStyle = style & CBRS_ALL;
    if (id == AFX_IDW_TOOLBAR) m_dwStyle |= CBRS_HIDE_INPLACE;

    const DWORD windowStyle = (style & ~CBRS_ALL) | controlStyle |
        CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER;
    const RECT bounds{};
    if (!CWnd::Create(TOOLBARCLASSNAMEW, nullptr, windowStyle, bounds, parent, id, nullptr)) return FALSE;

    SetSizes(m_sizeButton, m_sizeImage);
    return TRUE;
}

// Symbol: ?Create@CToolBar@@UEAAHPEAVCWnd@@KI@Z
extern "C" BOOL impl__Create_CToolBar__UEAAHPEAVCWnd__KI_Z(CToolBar* bar, CWnd* parent, DWORD style, UINT id) {
    return bar->CToolBar::Create(parent, style, id);
}

// Symbol: ?CreateEx@CToolBar@@UEAAHPEAVCWnd@@KKVCRect@@I@Z
extern "C" BOOL impl__CreateEx_CToolBar__UEAAHPEAVCWnd__KKVCRect__I_Z(
    CToolBar* bar, CWnd* parent, DWORD controlStyle, DWORD style, CRect borders, UINT id) {
    return bar->CToolBar::CreateEx(parent, controlStyle, style, borders, id);
}
