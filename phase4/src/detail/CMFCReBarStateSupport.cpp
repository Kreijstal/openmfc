#include "CMFCReBarStateSupport.h"

static_assert(sizeof(S_Cmfcrebarstate) == 1, "CMFCReBarState size mismatch");
namespace openmfc { namespace detail { namespace cmfcrebarstate {
void BuildKeyPath(const wchar_t* pszKey, wchar_t* out, size_t cch)
{
    const wchar_t* prefix = L"Software\\OpenMFC\\ReBarState\\";
    out[0] = L'\0';
    lstrcpynW(out, prefix, (int)cch);
    size_t used = wcslen(out);
    if (pszKey && *pszKey && used + 1 < cch)
        lstrcpynW(out + used, pszKey, (int)(cch - used));
}
bool IsRebar(HWND hwnd)
{
    wchar_t cls[64];
    int n = GetClassNameW(hwnd, cls, 64);
    if (n <= 0)
        return false;
    return lstrcmpiW(cls, L"ReBarWindow32") == 0;
}
} } }  // namespace openmfc::detail::cmfcrebarstate
