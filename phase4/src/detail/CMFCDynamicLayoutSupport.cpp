#include "CMFCDynamicLayoutSupport.h"

namespace openmfc { namespace detail { namespace cmfcdynamiclayout {
double DM_HuetoRGB(double m1, double m2, double h)
{
    if (h < 0.0) h += 1.0;
    if (h > 1.0) h -= 1.0;
    if (6.0 * h < 1.0) return m1 + (m2 - m1) * h * 6.0;
    if (2.0 * h < 1.0) return m2;
    if (3.0 * h < 2.0) return m1 + (m2 - m1) * (2.0 / 3.0 - h) * 6.0;
    return m1;
}
} } }  // namespace openmfc::detail::cmfcdynamiclayout
