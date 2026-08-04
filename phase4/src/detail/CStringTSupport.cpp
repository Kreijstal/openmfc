#include "CStringTSupport.h"

namespace openmfc { namespace detail { namespace cstringt {
CString* TrimLeftWithSet(CString* pThis, const wchar_t* trimSet) {
    if (!pThis || !trimSet || *trimSet == L'\0') return pThis;
    const wchar_t* text = pThis->GetString();
    int len = pThis->GetLength();
    int first = 0;
    while (first < len && IsTrimChar(text[first], trimSet)) {
        ++first;
    }
    if (first > 0) {
        *pThis = pThis->Mid(first);
    }
    return pThis;
}
CString* TrimRightWithSet(CString* pThis, const wchar_t* trimSet) {
    if (!pThis || !trimSet || *trimSet == L'\0') return pThis;
    const wchar_t* text = pThis->GetString();
    int len = pThis->GetLength();
    int last = len;
    while (last > 0 && IsTrimChar(text[last - 1], trimSet)) {
        --last;
    }
    if (last < len) {
        wchar_t* buf = pThis->GetBuffer(last);
        buf[last] = L'\0';
        pThis->ReleaseBuffer(last);
    }
    return pThis;
}
} } }  // namespace openmfc::detail::cstringt
