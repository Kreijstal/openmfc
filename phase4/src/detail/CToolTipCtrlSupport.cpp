#include "CToolTipCtrlSupport.h"

namespace openmfc { namespace detail { namespace ctooltipctrl {
HWND ToolTipHwnd(const CToolTipCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}
HWND ToolWindow(CWnd* pWnd) {
    return pWnd ? pWnd->GetSafeHwnd() : nullptr;
}
UINT ToolFlags(UINT_PTR nIDTool) {
    return nIDTool == 0 ? (TTF_SUBCLASS | TTF_IDISHWND) : TTF_SUBCLASS;
}
UINT_PTR ToolId(CWnd* pWnd, UINT_PTR nIDTool) {
    return nIDTool == 0 ? (UINT_PTR)ToolWindow(pWnd) : nIDTool;
}
void FillToolInfo(TOOLINFOW* pInfo, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!pInfo) return;
    pInfo->cbSize = sizeof(TOOLINFOW);
    pInfo->hwnd = ToolWindow(pWnd);
    pInfo->uId = ToolId(pWnd, nIDTool);
    pInfo->uFlags = ToolFlags(nIDTool);
}
CRuntimeClass g_classCToolTipCtrl = {
    "CToolTipCtrl",
    sizeof(CToolTipCtrl),
    0xFFFF,
    nullptr,
    CWnd::GetThisClass,
    CWnd::GetThisClass(),
    nullptr
};
} } }  // namespace openmfc::detail::ctooltipctrl
