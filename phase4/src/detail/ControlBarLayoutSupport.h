#pragma once

#include "openmfc/afxwin.h"

struct AFX_SIZEPARENTPARAMS {
    HDWP hDWP;
    RECT rect;
    SIZE sizeTotal;
    BOOL bStretch;
};
namespace openmfc::detail::dock {
constexpr UINT sizeParentMessage = 0x0361;
using SizeParentParams = AFX_SIZEPARENTPARAMS;
static_assert(offsetof(SizeParentParams, rect) == 0x08);
static_assert(offsetof(SizeParentParams, sizeTotal) == 0x18);
static_assert(offsetof(SizeParentParams, bStretch) == 0x20);

void RepositionControlBars(CWnd* window, UINT first, UINT last, UINT leftOver, UINT flags,
                           RECT* result, const RECT* client, BOOL stretch, const CFrameWnd* inPlaceFrame);
}

extern "C" void impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void*, HWND, const RECT*);
