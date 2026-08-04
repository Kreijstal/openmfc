// COleResizeBar — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleResizeBar::COleResizeBar()
    : m_pClientItem(nullptr), m_nHandleSize(4) {
    memset(_oleresizebar_padding, 0, sizeof(_oleresizebar_padding));
}
COleResizeBar::~COleResizeBar() {
    if (m_hWnd) {
        DestroyWindow();
    }
    m_pClientItem = nullptr;
}
BOOL COleResizeBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    (void)dwStyle;
    if (!pParentWnd) return FALSE;
    m_hWnd = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
                           0, 0, 0, 0, pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                           AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}
