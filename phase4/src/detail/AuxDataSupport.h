#pragma once
#include <windows.h>
#include <cstddef>

namespace openmfc { namespace detail {
// Release x64 AUX_DATA. Reserved members retain their native storage; only the
// cached metrics, colors, shared handles and owned menu bitmap are used here.
struct AuxData {
    int cxVScroll, cyHScroll, cxIcon, cyIcon;
    int cxBorder2, cyBorder2, cxPixelsPerInch, cyPixelsPerInch;
    HBRUSH hbrWindowFrame, hbrBtnFace;
    COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite, clrBtnText, clrWindowFrame;
    DWORD reserved68;
    HCURSOR hcurWait, hcurArrow;
    void* reserved88;
    UINT64 reserved96, reserved104;
    HBITMAP hbmMenuDot;
};
static_assert(sizeof(AuxData)==120 && offsetof(AuxData,hbmMenuDot)==112,"native AUX_DATA layout");
AuxData& GetAuxData();
void UpdateAuxDataMetrics();
void UpdateAuxDataColors();
void InitializeAuxData();
void CleanupAuxData() noexcept;
} }
