#define OPENMFC_APPCORE_IMPL
#include "detail/ClassicDockingSupport.h"
#include "detail/DockContextSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include <memory>
#include <vector>

extern "C" void impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(
    void*, CControlBar*, const RECT*);

using namespace openmfc::detail::dock;
using openmfc::detail::frame::ControlBars;
using openmfc::detail::frame::RegisterControlBar;
using openmfc::detail::frame::UnregisterControlBar;

namespace {
bool IsDockBar(const CWnd* window) {
    return window->IsKindOf(impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ());
}

void PositionFloatingFrame(CFrameWnd* frame, CPoint position) {
    ::SetWindowPos(frame->m_hWnd, nullptr, position.x, position.y, 0, 0,
                   SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
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
        if (dockBar->IsKindOf(impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ())) {
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
    constexpr struct { DWORD alignment; DWORD border; UINT id; } sides[] = {
        {alignTop, borderBottom, topBarId}, {alignBottom, borderTop, bottomBarId},
        {alignLeft, borderRight, leftBarId}, {alignRight, borderLeft, rightBarId}
    };
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

void CFrameWnd::ReDockControlBar(CControlBar* bar, CDockBar* target, const RECT* rect) {
    if (!bar) AfxThrowInvalidArgException();
    if (!target && bar->m_pDockContext) {
        const auto* context = static_cast<const DockContext*>(bar->m_pDockContext);
        auto* candidate = GetControlBar(context->m_uMRUDockID);
        if (candidate && IsDockBar(candidate)) target = reinterpret_cast<CDockBar*>(candidate);
    }
    if (target) {
        RegisterControlBar(this, bar);
        impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(target, bar, rect);
    } else {
        DockControlBar(bar, 0u, rect);
    }
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
