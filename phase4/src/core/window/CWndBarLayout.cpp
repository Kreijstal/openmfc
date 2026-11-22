#define OPENMFC_APPCORE_IMPL
#include "detail/ControlBarLayoutSupport.h"
#include "openmfc/afxole.h"

void openmfc::detail::dock::RepositionControlBars(
    CWnd* window, UINT first, UINT last, UINT leftOver, UINT flags, RECT* result,
    const RECT* client, BOOL stretch, const CFrameWnd* inPlaceFrame) {
    const UINT mode = flags & ~CWnd::reposNoPosLeftOver;
    const HWND parent = window->m_hWnd;
    if (!parent || (mode == CWnd::reposQuery && !result)) return;
    SizeParentParams layout{};
    layout.bStretch = stretch;
    if (client) layout.rect = *client;
    else ::GetClientRect(parent, &layout.rect);
    if (mode != CWnd::reposQuery) layout.hDWP = ::BeginDeferWindowPos(8);

    HWND remaining = nullptr;
    for (HWND child = ::GetTopWindow(parent); child; child = ::GetWindow(child, GW_HWNDNEXT)) {
        const UINT id = static_cast<UINT>(::GetDlgCtrlID(child));
        if (id == leftOver) {
            remaining = child;
        } else if (id >= first && id <= last) {
            auto* childWindow = static_cast<CWnd*>(CWnd::FromHandlePermanent(child));
            if (!childWindow) continue;
            const auto* bar = childWindow->IsKindOf(RUNTIME_CLASS(CControlBar))
                ? static_cast<CControlBar*>(childWindow) : nullptr;
            if (inPlaceFrame && (!bar || bar->m_pDockSite != inPlaceFrame)) continue;
            if (!inPlaceFrame && bar && bar->m_pInPlaceOwner &&
                bar->m_pDockSite && bar->m_pDockSite->m_hWnd == parent) continue;
            ::SendMessageW(child, sizeParentMessage, 0, reinterpret_cast<LPARAM>(&layout));
        }
    }

    if (mode == CWnd::reposQuery) {
        *result = stretch ? layout.rect : RECT{0, 0, layout.sizeTotal.cx, layout.sizeTotal.cy};
        return;
    }
    if (leftOver && remaining && !(flags & CWnd::reposNoPosLeftOver)) {
        if (mode == CWnd::reposExtra && result) {
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

void CWnd::RepositionBars(UINT first, UINT last, UINT leftOver, UINT flags,
                          RECT* result, const RECT* client, BOOL stretch) {
    openmfc::detail::dock::RepositionControlBars(this, first, last, leftOver, flags, result, client, stretch, nullptr);
}

// Symbol: ?RepositionBars@CWnd@@QEAAXIIIIPEAUtagRECT@@PEBU2@H@Z
extern "C" void impl__RepositionBars_CWnd__QEAAXIIIIPEAUtagRECT__PEBU2_H_Z(
    CWnd* window, UINT first, UINT last, UINT leftOver, UINT flags,
    RECT* result, const RECT* client, BOOL stretch) {
    if (window) window->RepositionBars(first, last, leftOver, flags, result, client, stretch);
}
