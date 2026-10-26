// Native scrollbar-control routing and scrolling of window/OLE children.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);

namespace {
HWND ScrollHandle(const CWnd* window, int& bar) {
    CScrollBar* control = window->GetScrollBarCtrl(bar);
    if (control) {
        bar = SB_CTL;
        return control->m_hWnd;
    }
    // A virtual lookup can replace the window handle.
    return window->m_hWnd;
}
int Add(int value, int delta) {
    return static_cast<int>(static_cast<unsigned>(value) + static_cast<unsigned>(delta));
}
}

CWnd* CWnd::GetDlgItem(int id) const {
    if (m_pCtrlCont) {
        using Item = CWnd* (MS_ABI*)(void*, int);
        return openmfc::detail::WindowSlot<Item>(m_pCtrlCont, 31)(m_pCtrlCont, id);
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDlgItem(m_hWnd, id));
}

int CWnd::GetScrollPos(int bar) const {
    HWND handle = ScrollHandle(this, bar);
    return ::GetScrollPos(handle, bar);
}
int CWnd::SetScrollPos(int bar, int position, int redraw) {
    HWND handle = ScrollHandle(this, bar);
    return ::SetScrollPos(handle, bar, position, redraw);
}
void CWnd::SetScrollRange(int bar, int minimum, int maximum, int redraw) {
    HWND handle = ScrollHandle(this, bar);
    ::SetScrollRange(handle, bar, minimum, maximum, redraw);
}
int CWnd::SetScrollInfo(int bar, SCROLLINFO* info, int redraw) {
    HWND handle = m_hWnd;
    if (bar != SB_CTL) {
        CScrollBar* control = GetScrollBarCtrl(bar);
        if (control) {
            handle = control->m_hWnd;
            bar = SB_CTL;
        }
    }
    info->cbSize = sizeof(*info);
    ::SetScrollInfo(handle, bar, info, redraw);
    // The Win32 result is the previous position, not a success predicate.
    return TRUE;
}
void CWnd::EnableScrollBarCtrl(int bar, int enable) {
    if (bar == SB_BOTH) {
        EnableScrollBarCtrl(SB_HORZ, enable);
        bar = SB_VERT;
    }
    CScrollBar* control = GetScrollBarCtrl(bar);
    if (control) control->EnableWindow(enable);
    else ::ShowScrollBar(m_hWnd, bar, enable);
}
void CWnd::GetScrollRange(int bar, int* minimum, int* maximum) const {
    HWND handle = ScrollHandle(this, bar);
    ::GetScrollRange(handle, bar, minimum, maximum);
}
BOOL CWnd::GetScrollInfo(int bar, SCROLLINFO* info, UINT mask) {
    // Unlike the position/range helpers, native GetScrollInfo retains this
    // handle across the lookup and never queries a control for SB_CTL.
    HWND handle = m_hWnd;
    if (bar != SB_CTL) {
        CScrollBar* control = GetScrollBarCtrl(bar);
        if (control) {
            handle = control->m_hWnd;
            bar = SB_CTL;
        }
    }
    info->cbSize = sizeof(*info);
    info->fMask = mask;
    return ::GetScrollInfo(handle, bar, info);
}
int CWnd::GetScrollLimit(int bar) {
    int minimum, maximum;
    GetScrollRange(bar, &minimum, &maximum);
    SCROLLINFO info;
    if (GetScrollInfo(bar, &info, SIF_PAGE) && info.nPage > 1)
        return Add(maximum, static_cast<int>(1u - info.nPage));
    return maximum;
}
void CWnd::ScrollWindow(int dx, int dy, const RECT* scroll, const RECT* clip) {
    if (::IsWindowVisible(m_hWnd) || scroll || clip) {
        ::ScrollWindow(m_hWnd, dx, dy, scroll, clip);
    } else {
        for (HWND child = ::GetWindow(m_hWnd, GW_CHILD); child;
             child = ::GetWindow(child, GW_HWNDNEXT)) {
            RECT rectangle{};
            ::GetWindowRect(child, &rectangle);
            ScreenToClient(&rectangle);
            ::SetWindowPos(child, nullptr, Add(rectangle.left, dx), Add(rectangle.top, dy),
                           0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
    if (m_pCtrlCont && !scroll) {
        using Scroll = void (MS_ABI*)(void*, int, int);
        openmfc::detail::WindowSlot<Scroll>(m_pCtrlCont, 25)(m_pCtrlCont, dx, dy);
    }
}

// Symbol: ?GetScrollBarCtrl@CWnd@@UEBAPEAVCScrollBar@@H@Z
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd*, int) { return nullptr; }
// Symbol: ?GetDlgItem@CWnd@@QEBAPEAV1@H@Z
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* self, int id) { return self->GetDlgItem(id); }
// Symbol: ?GetScrollPos@CWnd@@QEBAHH@Z
extern "C" int MS_ABI impl__GetScrollPos_CWnd__QEBAHH_Z(const CWnd* self, int bar) { return self->GetScrollPos(bar); }
// Symbol: ?SetScrollPos@CWnd@@QEAAHHHH@Z
extern "C" int MS_ABI impl__SetScrollPos_CWnd__QEAAHHHH_Z(CWnd* self, int bar, int position, int redraw) { return self->SetScrollPos(bar, position, redraw); }
// Symbol: ?SetScrollInfo@CWnd@@QEAAHHPEAUtagSCROLLINFO@@H@Z
extern "C" int MS_ABI impl__SetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__H_Z(CWnd* self, int bar, SCROLLINFO* info, int redraw) { return self->SetScrollInfo(bar, info, redraw); }
// Symbol: ?SetScrollRange@CWnd@@QEAAXHHHH@Z
extern "C" void MS_ABI impl__SetScrollRange_CWnd__QEAAXHHHH_Z(CWnd* self, int bar, int minimum, int maximum, int redraw) { self->SetScrollRange(bar, minimum, maximum, redraw); }
// Symbol: ?EnableScrollBarCtrl@CWnd@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z(CWnd* self, int bar, int enable) { self->EnableScrollBarCtrl(bar, enable); }
// Symbol: ?GetScrollRange@CWnd@@QEBAXHPEAH0@Z
extern "C" void MS_ABI impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z(const CWnd* self, int bar, int* minimum, int* maximum) { self->GetScrollRange(bar, minimum, maximum); }
// Symbol: ?GetScrollInfo@CWnd@@QEAAHHPEAUtagSCROLLINFO@@I@Z
extern "C" BOOL MS_ABI impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z(CWnd* self, int bar, SCROLLINFO* info, UINT mask) { return self->GetScrollInfo(bar, info, mask); }
// Symbol: ?GetScrollLimit@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__GetScrollLimit_CWnd__QEAAHH_Z(CWnd* self, int bar) { return self->GetScrollLimit(bar); }
// Symbol: ?ScrollWindow@CWnd@@QEAAXHHPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__ScrollWindow_CWnd__QEAAXHHPEBUtagRECT__0_Z(CWnd* self, int dx, int dy, const RECT* scroll, const RECT* clip) { self->ScrollWindow(dx, dy, scroll, clip); }
