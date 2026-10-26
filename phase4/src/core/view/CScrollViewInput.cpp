// Release x64 scroll commands, scrollbar reflection and mouse-wheel handling.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include <climits>

namespace openmfc { namespace detail {
// Native MFC caches these once per DLL, including failed settings queries.
BOOL scrollViewWheelSettingsInitialized = FALSE;
UINT scrollViewWheelLines = 0;
} }
namespace {
int Add(int value, int amount) {
    return static_cast<int>(static_cast<UINT>(value) + static_cast<UINT>(amount));
}
int Subtract(int value, int amount) {
    return static_cast<int>(static_cast<UINT>(value) - static_cast<UINT>(amount));
}
int Target(BYTE code, int current, UINT position, int line, int page) {
    switch (code) {
    case SB_LINEUP: return Subtract(current, line);
    case SB_LINEDOWN: return Add(current, line);
    case SB_PAGEUP: return Subtract(current, page);
    case SB_PAGEDOWN: return Add(current, page);
    case SB_THUMBTRACK: return static_cast<int>(position);
    case SB_TOP: return 0;
    case SB_BOTTOM: return INT_MAX;
    default: return current;
    }
}
int EnabledDelta(const CScrollView* self, int axis, DWORD style, int delta) {
    CScrollBar* control = self->GetScrollBarCtrl(axis);
    // A disabled control suppresses movement even if the window style has
    // the axis bit. CheckScrollBars uses a different, additive predicate.
    if (control) return control->IsWindowEnabled() ? delta : 0;
    return style & (axis == SB_HORZ ? WS_HSCROLL : WS_VSCROLL) ? delta : 0;
}
int Clamp(int position, int delta, int limit) {
    int result = Add(position, delta);
    if (result < 0) return 0;
    return result > limit ? limit : result;
}
void ScrollBarMessage(CScrollView* self, int axis, UINT code, UINT position, CScrollBar* control) {
    if (control && control->SendChildNotifyLastMsg(nullptr)) return;
    if (self->GetScrollBarCtrl(axis) != control) return;
    UINT combined = axis == SB_HORZ ? (0xff00u | BYTE(code)) : ((UINT(BYTE(code)) << 8) | 0xffu);
    self->OnScroll(combined, position, TRUE);
}
}

void CScrollView::CheckScrollBars(BOOL& horizontal, BOOL& vertical) const {
    DWORD style = GetStyle();
    CScrollBar* control = GetScrollBarCtrl(SB_VERT);
    vertical = (control && control->IsWindowEnabled()) || (style & WS_VSCROLL);
    control = GetScrollBarCtrl(SB_HORZ);
    horizontal = (control && control->IsWindowEnabled()) || (style & WS_HSCROLL);
}
CPoint CScrollView::GetDeviceScrollPosition() const {
    int vertical = GetScrollPos(SB_VERT);
    int horizontal = GetScrollPos(SB_HORZ);
    return CPoint(horizontal, vertical);
}
CSize CScrollView::GetWheelScrollDistance(CSize size, BOOL horizontal, BOOL vertical) {
    return CSize(horizontal ? size.cx / 10 : 0, vertical ? size.cy / 10 : 0);
}
BOOL CScrollView::OnScroll(UINT code, UINT position, BOOL scroll) {
    int oldX = GetScrollPos(SB_HORZ);
    int x = Target(BYTE(code), oldX, position, m_lineDev.cx, m_pageDev.cx);
    int oldY = GetScrollPos(SB_VERT);
    int y = Target(BYTE(code >> 8), oldY, position, m_lineDev.cy, m_pageDev.cy);
    BOOL result = OnScrollBy(CSize(Subtract(x, oldX), Subtract(y, oldY)), scroll);
    if (result && scroll) ::UpdateWindow(m_hWnd);
    return result;
}
BOOL CScrollView::OnScrollBy(CSize size, BOOL scroll) {
    DWORD style = GetStyle();
    int dy = EnabledDelta(this, SB_VERT, style, size.cy);
    int dx = EnabledDelta(this, SB_HORZ, style, size.cx);
    int oldX = GetScrollPos(SB_HORZ);
    int limitX = GetScrollLimit(SB_HORZ);
    int x = Clamp(oldX, dx, limitX);
    int oldY = GetScrollPos(SB_VERT);
    int limitY = GetScrollLimit(SB_VERT);
    int y = Clamp(oldY, dy, limitY);
    if (x == oldX && y == oldY) return FALSE;
    if (scroll) {
        ScrollWindow(Subtract(oldX, x), Subtract(oldY, y), nullptr, nullptr);
        if (x != oldX) SetScrollPos(SB_HORZ, x, TRUE);
        if (y != oldY) SetScrollPos(SB_VERT, y, TRUE);
    }
    return TRUE;
}
void CScrollView::OnHScroll(UINT code, UINT position, CScrollBar* control) { ScrollBarMessage(this, SB_HORZ, code, position, control); }
void CScrollView::OnVScroll(UINT code, UINT position, CScrollBar* control) { ScrollBarMessage(this, SB_VERT, code, position, control); }
BOOL CScrollView::DoMouseWheel(UINT, short delta, CPoint) {
    BOOL horizontal, vertical;
    CheckScrollBars(horizontal, vertical);
    if (!horizontal && !vertical) return FALSE;
    using namespace openmfc::detail;
    if (!scrollViewWheelSettingsInitialized) {
        scrollViewWheelSettingsInitialized = TRUE;
        scrollViewWheelLines = 3;
        ::SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &scrollViewWheelLines, 0);
    }
    UINT lines = scrollViewWheelLines;
    int notches = ::MulDiv(-static_cast<int>(delta), static_cast<int>(lines), WHEEL_DELTA);
    int page = vertical ? m_pageDev.cy : m_pageDev.cx;
    int distance;
    if (lines == WHEEL_PAGESCROLL) {
        distance = delta > 0 ? Subtract(0, page) : page;
    } else {
        int line = vertical ? m_lineDev.cy : m_lineDev.cx;
        distance = static_cast<int>(static_cast<UINT>(notches) * static_cast<UINT>(line));
        if (distance > page) distance = page;
    }
    BOOL result = OnScrollBy(vertical ? CSize(0, distance) : CSize(distance, 0), TRUE);
    if (result) ::UpdateWindow(m_hWnd);
    return result;
}
BOOL CScrollView::OnMouseWheel(UINT flags, short delta, CPoint point) {
    if (flags & (MK_SHIFT | MK_CONTROL)) return FALSE;
    if (CView::GetParentSplitter(this, TRUE)) return FALSE;
    return DoMouseWheel(flags, delta, point);
}

// Member aggregate returns use RCX=this, RDX=output and return output in RAX.
// Symbol: ?GetDeviceScrollPosition@CScrollView@@QEBA?AVCPoint@@XZ
extern "C" CPoint* MS_ABI impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ(const CScrollView* self, CPoint* out) { return new (out) CPoint(self->GetDeviceScrollPosition()); }
// Symbol: ?GetWheelScrollDistance@CScrollView@@UEAA?AVCSize@@V2@HH@Z
extern "C" CSize* MS_ABI impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z(CScrollView* self, CSize* out, CSize size, BOOL horizontal, BOOL vertical) { return new (out) CSize(self->CScrollView::GetWheelScrollDistance(size, horizontal, vertical)); }
// Symbol: ?CheckScrollBars@CScrollView@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z(const CScrollView* self, BOOL* horizontal, BOOL* vertical) { self->CheckScrollBars(*horizontal, *vertical); }
// Symbol: ?OnScroll@CScrollView@@UEAAHIIH@Z
extern "C" BOOL MS_ABI impl__OnScroll_CScrollView__UEAAHIIH_Z(CScrollView* self, UINT code, UINT position, BOOL scroll) { return self->CScrollView::OnScroll(code, position, scroll); }
// Symbol: ?OnScrollBy@CScrollView@@UEAAHVCSize@@H@Z
extern "C" BOOL MS_ABI impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(CScrollView* self, CSize size, BOOL scroll) { return self->CScrollView::OnScrollBy(size, scroll); }
// Symbol: ?OnHScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* self, UINT code, UINT position, CScrollBar* control) { self->OnHScroll(code, position, control); }
// Symbol: ?OnVScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* self, UINT code, UINT position, CScrollBar* control) { self->OnVScroll(code, position, control); }
// Symbol: ?DoMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" BOOL MS_ABI impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* self, UINT flags, short delta, CPoint point) { return self->DoMouseWheel(flags, delta, point); }
// Symbol: ?OnMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" BOOL MS_ABI impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* self, UINT flags, short delta, CPoint point) { return self->OnMouseWheel(flags, delta, point); }
