#include "FrameFontExportsSupport.h"

namespace openmfc { namespace detail { namespace framefontexports {
LOGFONTW* FontLog(CFontDialog* pThis) {
    return pThis ? reinterpret_cast<LOGFONTW*>(static_cast<CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}
const LOGFONTW* FontLog(const CFontDialog* pThis) {
    return pThis ? reinterpret_cast<const LOGFONTW*>(static_cast<const CFontDialogAccess*>(pThis)->m_lf) : nullptr;
}
const AFX_MSGMAP* EmptyMessageMap_FrameFontExports() {
    return CWnd::GetThisMessageMap();
}
} } }  // namespace openmfc::detail::framefontexports
