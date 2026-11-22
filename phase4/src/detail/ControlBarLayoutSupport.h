#pragma once

#include "openmfc/afxwin.h"

namespace openmfc::detail::dock {
constexpr UINT sizeParentMessage = 0x0361;

struct SizeParentParams {
    HDWP hDWP;
    RECT rect;
    SIZE sizeTotal;
    BOOL bStretch;
};
static_assert(offsetof(SizeParentParams, rect) == 0x08);
static_assert(offsetof(SizeParentParams, sizeTotal) == 0x18);
static_assert(offsetof(SizeParentParams, bStretch) == 0x20);
}

extern "C" void impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void*, HWND, const RECT*);
