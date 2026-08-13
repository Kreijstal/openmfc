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
// ?OnEraseBkgnd@COleResizeBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_COleResizeBar__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// ?OnLButtonDown@COleResizeBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_COleResizeBar__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// ?OnPaint@COleResizeBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_COleResizeBar__IEAAXXZ() {}

// ?OnSetCursor@COleResizeBar@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_COleResizeBar__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// ?OnSize@COleResizeBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleResizeBar__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// ?OnSizeParent@COleResizeBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSizeParent_COleResizeBar__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// ?OnUpdateCmdUI@COleResizeBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_COleResizeBar__UEAAXPEAVCFrameWnd__H_Z(void* /*class*/* p0, int p1) {}

