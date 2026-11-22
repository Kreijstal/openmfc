#define OPENMFC_APPCORE_IMPL
#include "detail/ControlBarLayoutSupport.h"

void CWnd::RepositionBars(UINT first, UINT last, UINT leftOver, UINT flags,
                          RECT* result, const RECT* client, BOOL stretch) {
    using namespace openmfc::detail::dock;
    const UINT mode = flags & ~reposNoPosLeftOver;
    if (!m_hWnd || (mode == reposQuery && !result)) return;
    SizeParentParams layout{};
    layout.bStretch = stretch;
    if (client) layout.rect = *client;
    else ::GetClientRect(m_hWnd, &layout.rect);
    if (mode != reposQuery) layout.hDWP = ::BeginDeferWindowPos(8);

    HWND remaining = nullptr;
    for (HWND child = ::GetTopWindow(m_hWnd); child; child = ::GetWindow(child, GW_HWNDNEXT)) {
        const UINT id = static_cast<UINT>(::GetDlgCtrlID(child));
        if (id == leftOver) {
            remaining = child;
        } else if (id >= first && id <= last && CWnd::FromHandlePermanent(child)) {
            ::SendMessageW(child, sizeParentMessage, 0, reinterpret_cast<LPARAM>(&layout));
        }
    }

    if (mode == reposQuery) {
        *result = stretch ? layout.rect : RECT{0, 0, layout.sizeTotal.cx, layout.sizeTotal.cy};
        return;
    }
    if (leftOver && remaining && !(flags & reposNoPosLeftOver)) {
        if (mode == reposExtra && result) {
            layout.rect.left += result->left;
            layout.rect.top += result->top;
            layout.rect.right -= result->right;
            layout.rect.bottom -= result->bottom;
        }
        if (auto* pane = CWnd::FromHandle(remaining)) pane->CalcWindowRect(&layout.rect, 0);
        impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
            &layout, remaining, &layout.rect);
    }
    if (layout.hDWP) ::EndDeferWindowPos(layout.hDWP);
}

// Symbol: ?RepositionBars@CWnd@@QEAAXIIIIPEAUtagRECT@@PEBU2@H@Z
extern "C" void impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
    CWnd* window, UINT first, UINT last, UINT leftOver, UINT flags,
    RECT* result, const RECT* client, BOOL stretch) {
    if (window) window->RepositionBars(first, last, leftOver, flags, result, client, stretch);
}
