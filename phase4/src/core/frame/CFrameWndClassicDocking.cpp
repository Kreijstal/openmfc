#define OPENMFC_APPCORE_IMPL
#include "detail/ClassicDockingSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include "openmfc/afxole.h"
#include <cstring>
#include <memory>
#include <vector>

extern "C" void impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(
    void*, CControlBar*, const RECT*);
extern "C" int impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(void*, CControlBar*, int);

using namespace openmfc::detail::dock;
using openmfc::detail::frame::ControlBars;
using openmfc::detail::frame::RegisterControlBar;
using openmfc::detail::frame::UnregisterControlBar;

namespace {
constexpr struct DockSide {
    DWORD alignment;
    DWORD border;
    UINT id;
} sides[] = {
    {alignTop, borderBottom, topBarId}, {alignBottom, borderTop, bottomBarId},
    {alignLeft, borderRight, leftBarId}, {alignRight, borderLeft, rightBarId}
};

bool IsDockBar(const CWnd* window) {
    return window->IsKindOf(impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ()) &&
           static_cast<const CControlBar*>(window)->IsDockBar();
}

void PositionFloatingFrame(CFrameWnd* frame, CPoint position) {
    ::SetWindowPos(frame->m_hWnd, nullptr, position.x, position.y, 0, 0,
                   SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
}

void CFrameWnd::RecalcLayout(BOOL notify) {
    if (!m_hWnd || m_retailTail.m_bInRecalcLayout) return;
    m_retailTail.m_bInRecalcLayout = TRUE;
    struct ResetLayoutFlag {
        BOOL& flag;
        ~ResetLayoutFlag() { flag = FALSE; }
    } reset{m_retailTail.m_bInRecalcLayout};

    const DWORD flags = m_retailTail.m_dwBorderFlags;
    m_retailTail.m_dwBorderFlags &= ~(idleNotify | idleLayout);
    if (notify || (flags & idleNotify)) {
        COleFrameHook* hook = nullptr;
        std::memcpy(&hook, m_retailMiddle.m_pNotifyHook, sizeof(hook));
        if (hook) hook->OnRecalcLayout();
    }

    constexpr UINT paneId = 0xe900;
    constexpr DWORD snapToBars = 0x2000;
    auto& border = m_retailMiddle.m_rectBorder;
    if (::GetWindowLongW(m_hWnd, GWL_STYLE) & snapToBars) {
        RECT rect{0, 0, 32767, 32767};
        RepositionBars(0, 0xffff, paneId, reposQuery, &rect, &rect, FALSE);
        RepositionBars(0, 0xffff, paneId, reposExtra, &border, &rect);
        CalcWindowRect(&rect, 0);
        ::SetWindowPos(m_hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    } else {
        RepositionBars(0, 0xffff, paneId, reposExtra, &border);
    }
}

DWORD CFrameWnd::CanDock(CRect rect, DWORD style, CDockBar** target) {
    if (target) *target = nullptr;
    for (auto* node = ControlBars(this).m_pNodeHead; node; node = node->pNext) {
        auto* bar = static_cast<CControlBar*>(node->data);
        if (!bar || !IsDockBar(bar) || !::IsWindowVisible(bar->m_hWnd)) continue;
        const DWORD allowed = style & bar->m_dwStyle;
        if (!(allowed & alignAny) || (IsFloatingBar(bar) && !(allowed & floatMulti))) continue;

        RECT bounds{};
        if (!::GetWindowRect(bar->m_hWnd, &bounds)) continue;
        // An empty dock bar still offers a one-pixel drop target.
        if (bounds.left == bounds.right) ++bounds.right;
        if (bounds.top == bounds.bottom) ++bounds.bottom;
        if (!::IntersectRect(&bounds, &bounds, static_cast<const RECT*>(rect))) continue;

        if (target) *target = reinterpret_cast<CDockBar*>(bar);
        return allowed & (alignAny | floatMulti);
    }
    return 0;
}

void CFrameWnd::DestroyDockBars() {
    std::vector<HWND> dockBars;
    std::vector<HWND> controlBars;
    for (auto* node = ControlBars(this).m_pNodeHead; node; node = node->pNext) {
        auto* bar = static_cast<CControlBar*>(node->data);
        if (!bar || !bar->m_hWnd) continue;
        auto& handles = IsDockBar(bar) ? dockBars : controlBars;
        handles.push_back(bar->m_hWnd);
    }

    // Window destruction can also remove other entries from the frame's list.
    for (HWND handle : dockBars) {
        auto* bar = static_cast<CWnd*>(CWnd::FromHandlePermanent(handle));
        if (!bar || !IsDockBar(bar)) continue;
        if (IsFloatingBar(bar)) {
            auto* frame = bar->GetParentFrame();
            if (!frame) AfxThrowInvalidArgException();
            frame->DestroyWindow();
        } else {
            bar->DestroyWindow();
        }
    }
    for (HWND handle : controlBars) {
        auto* bar = static_cast<CWnd*>(CWnd::FromHandlePermanent(handle));
        if (bar && bar->IsKindOf(impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ())) bar->DestroyWindow();
    }
}

void CFrameWnd::DockControlBar(CControlBar* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    if (!pBar) {
        return;
    }

    if (!nDockBarID) {
        const DWORD bars = pBar->m_dwStyle & alignAny ? pBar->m_dwStyle : pBar->m_dwDockStyle;
        if ((bars & alignTop) && GetControlBar(topBarId)) nDockBarID = topBarId;
        else if ((bars & alignBottom) && GetControlBar(bottomBarId)) nDockBarID = bottomBarId;
        else if ((bars & alignLeft) && GetControlBar(leftBarId)) nDockBarID = leftBarId;
        else if ((bars & alignRight) && GetControlBar(rightBarId)) nDockBarID = rightBarId;
    }
    if (auto* dockBar = GetControlBar(nDockBarID)) {
        if (dockBar->IsDockBar()) {
            DockControlBar(pBar, reinterpret_cast<CDockBar*>(dockBar), lpRect);
            return;
        }
    }

    RegisterControlBar(this, pBar);

    if (pBar->m_hWnd && lpRect) {
        ::MoveWindow(pBar->m_hWnd,
                     lpRect->left, lpRect->top,
                     lpRect->right - lpRect->left,
                     lpRect->bottom - lpRect->top,
                     TRUE);
    }
    if (pBar->m_hWnd) {
        ::ShowWindow(pBar->m_hWnd, SW_SHOW);
    }
}

void CFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, const RECT* lpRect) {
    if (!pBar) return;
    if (!pDockBar) {
        DockControlBar(pBar, 0u, lpRect);
        return;
    }
    RegisterControlBar(this, pBar);
    impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(pDockBar, pBar, lpRect);
}

void CFrameWnd::EnableDocking(DWORD dwDockStyle) {
    m_retailTail.m_pFloatingFrameClass = impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ();
    for (const auto& side : sides) {
        if (!(dwDockStyle & side.alignment) || GetControlBar(side.id)) continue;
        std::unique_ptr<CControlBar> dock(NewDockBar());
        if (!dock) AfxThrowMemoryException();
        // A failed CWnd::Create can call PostNcDestroy before returning.
        dock->m_bAutoDelete = FALSE;
        const DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                            side.alignment | side.border;
        if (!impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(dock.get(), this, style, side.id))
            AfxThrowResourceException();
        dock.release()->m_bAutoDelete = TRUE;
    }
}

void CFrameWnd::RemoveControlBar(CControlBar* bar) {
    if (bar) UnregisterControlBar(this, bar);
}

void CFrameWnd::ShowControlBar(CControlBar* bar, BOOL show, BOOL delay) {
    if (!bar) AfxThrowInvalidArgException();
    if (!bar->m_hWnd) return;
    auto* dockSite = static_cast<CFrameWnd*>(bar->m_pDockSite);
    auto* frame = bar->m_pInPlaceOwner && dockSite && dockSite->m_hWnd == ::GetParent(bar->m_hWnd)
        ? dockSite : bar->GetParentFrame();
    if (!frame) frame = bar->m_pDockSite ? static_cast<CFrameWnd*>(bar->m_pDockSite) : this;

    const bool floating = impl__IsFloating_CControlBar__QEBAHXZ(bar) != FALSE;
    if (delay) {
        bar->DelayShow(show);
        ScheduleLayout(frame);
    } else {
        const UINT flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE;
        ::SetWindowPos(bar->m_hWnd, nullptr, 0, 0, 0, 0,
                       flags | (show ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
        bar->DelayShow(show);
        if (show || !floating) frame->RecalcLayout(FALSE);
    }

    if (!floating) return;
    const int visibleCount = bar->m_pDockBar
        ? impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(bar->m_pDockBar) : (show != FALSE);
    if (!visibleCount || (show && visibleCount == 1)) {
        const int command = visibleCount ? SW_SHOWNA : SW_HIDE;
        frame->m_retailMiddle.m_nShowDelay = delay ? command : -1;
        if (!delay) ::ShowWindow(frame->m_hWnd, command);
        else if (show) frame->RecalcLayout(FALSE);
    } else if (!delay) {
        frame->RecalcLayout(FALSE);
    }
}

void CFrameWnd::ReDockControlBar(CControlBar* bar, CDockBar* target, const RECT* rect) {
    if (!bar) AfxThrowInvalidArgException();
    if (!target) {
        const auto id = static_cast<UINT_PTR>(static_cast<UINT>(::GetDlgCtrlID(bar->m_hWnd)));
        auto* placeholder = reinterpret_cast<CControlBar*>(id);
        for (const auto& side : sides) {
            auto* dock = GetControlBar(side.id);
            if (!dock || !IsDockBar(dock)) continue;
            if (impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(dock, placeholder, -1) > 0) {
                target = reinterpret_cast<CDockBar*>(dock);
                break;
            }
            if ((bar->m_dwStyle & alignAny) == side.alignment)
                target = reinterpret_cast<CDockBar*>(dock);
        }
    }
    if (!target) AfxThrowInvalidArgException();
    RegisterControlBar(this, bar);
    impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(target, bar, rect);
}

void CFrameWnd::FloatControlBar(CControlBar* bar, CPoint position, DWORD style) {
    if (!bar) AfxThrowInvalidArgException();
    auto* previousDock = static_cast<CControlBar*>(bar->m_pDockBar);
    if (bar->m_pDockSite && IsFloatingBar(previousDock) &&
        impl__GetDockedCount_CDockBar__QEBAHXZ(previousDock) == 1 &&
        (previousDock->m_dwStyle & style & alignAny)) {
        auto* frame = previousDock->GetParentFrame();
        if (!frame) AfxThrowInvalidArgException();
        PositionFloatingFrame(frame, position);
        frame->RecalcLayout(TRUE);
        ::UpdateWindow(frame->m_hWnd);
        return;
    }

    if (bar->m_dwStyle & sizeDynamic) {
        style |= sizeDynamic;
        if (style & (alignLeft | alignRight)) style = (style & ~alignAny) | alignTop;
    }

    auto* floating = CreateFloatingFrame(style);
    if (!floating) AfxThrowInvalidArgException();
    auto* frame = FrameWindow(floating);
    PositionFloatingFrame(frame, position);
    if (!frame->m_hWndOwner) frame->m_hWndOwner = bar->m_hWnd;
    auto* destination = FloatingDockBar(floating);
    if (!destination || !destination->m_hWnd) AfxThrowInvalidArgException();
    DockControlBar(bar, reinterpret_cast<CDockBar*>(destination), nullptr);
    frame->RecalcLayout(TRUE);
    if (::GetWindowLongW(bar->m_hWnd, GWL_STYLE) & WS_VISIBLE) {
        ::ShowWindow(frame->m_hWnd, SW_SHOWNA);
        ::UpdateWindow(frame->m_hWnd);
    }
}
