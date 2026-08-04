#include "CMFCCustomColorsPropertyPageSupport.h"

static_assert(offsetof(CColorsPage, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorsPage, m_pOwner) == 0x158, "owner @0x158");
static_assert(offsetof(CColorsPage, m_dL) == 0x3a8, "L @0x3a8");
static_assert(offsetof(CColorsPage, m_dS) == 0x3b0, "S @0x3b0");
static_assert(offsetof(CColorsPage, m_dH) == 0x3b8, "H @0x3b8");
static_assert(offsetof(CColorsPage, m_nR) == 0x410, "R @0x410");
static_assert(offsetof(CColorsPage, m_nL) == 0x41c, "L int @0x41c");
static_assert(offsetof(CColorsPage, m_nH) == 0x420, "H int @0x420");
static_assert(offsetof(CColorsPage, m_bUpdating) == 0x42c, "guard @0x42c");
namespace openmfc { namespace detail { namespace cmfccustomcolorspropertypage {
int ChannelToInt(double d) { return static_cast<int>(d * 255.0 + 0.5); }
COLORREF HlsToRgb_Cmfccustomcolorspropertypage(double H, double L, double S)
{
    if (S == 0.0) {
        const int v = static_cast<int>(L * 255.0);
        return RGB(v, v, v);
    }
    double M2;
    if (L <= 0.5) {
        M2 = L * (1.0 + S);
    } else if (L == 1.0) {
        M2 = L;
    } else {
        M2 = L + S - L * S;
    }
    const double M1 = 2.0 * L - M2;
    const int r = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H + 1.0 / 3.0) * 255.0);
    const int g = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H) * 255.0);
    const int b = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H - 1.0 / 3.0) * 255.0);
    return RGB(r, g, b);
}
} } }  // namespace openmfc::detail::cmfccustomcolorspropertypage
