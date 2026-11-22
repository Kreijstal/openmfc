#include "CControlBarInfoSupport.h"
#include <cerrno>
#include <cwchar>

namespace openmfc::detail::ccontrolbarinfo {
void MakeBarSection(const wchar_t* profile, int index, wchar_t (&section)[256]) {
    const int savedError = errno;
    errno = 0;
    _snwprintf_s(section, _countof(section), _TRUNCATE, L"%ls-Bar%d", profile, index);
    if (!errno) errno = savedError;
}
}
