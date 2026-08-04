#include "CControlBarInfoSupport.h"

static_assert(sizeof(S_Ccontrolbarinfo) == 120, "CControlBarInfo size mismatch");
static_assert(offsetof(S_Ccontrolbarinfo, m_nBarID) == 0, "m_nBarID offset");
static_assert(offsetof(S_Ccontrolbarinfo, m_bVisible) == 4, "m_bVisible offset");
static_assert(offsetof(S_Ccontrolbarinfo, m_bFloating) == 8, "m_bFloating offset");
static_assert(offsetof(S_Ccontrolbarinfo, m_rectMRUDockPos_left) == 40, "m_rectMRUDockPos offset");
static_assert(offsetof(S_Ccontrolbarinfo, m_arrBarID_vfptr) == 72, "m_arrBarID offset");
static_assert(offsetof(S_Ccontrolbarinfo, m_pBar) == 112, "m_pBar offset");
namespace openmfc { namespace detail { namespace ccontrolbarinfo {
int MakeBarSection(const wchar_t* lpszProfileName, int nIndex,
                          wchar_t* buf, size_t cch)
{
    if (buf == nullptr || cch == 0)
        return 0;
    const wchar_t* name = lpszProfileName ? lpszProfileName : L"";
    // Equivalent to wsprintf(buf, L"%s-Bar%d", name, nIndex).
    int n = _snwprintf(buf, cch, L"%s-Bar%d", name, nIndex);
    if (n < 0 || static_cast<size_t>(n) >= cch) {
        buf[cch - 1] = L'\0';
        n = static_cast<int>(cch - 1);
    }
    return n;
}
} } }  // namespace openmfc::detail::ccontrolbarinfo
