// CEnumOleVerb — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?OnNext@CEnumOleVerb@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumOleVerb__MEAAHPEAX_Z(CEnumOleVerb* pThis, void* pv) {
    return pThis ? pThis->OnNext(pv) : FALSE;
}
CEnumOleVerb::CEnumOleVerb() : m_verbs(nullptr), m_count(0), m_position(0) {
}
CEnumOleVerb::~CEnumOleVerb() {
}
void CEnumOleVerb::SetVerbs(OLEVERB* pVerbs, unsigned long nCount) {
    m_verbs = pVerbs;
    m_count = nCount;
    m_position = 0;
}
int CEnumOleVerb::OnNext(void* pv) {
    if (pv == nullptr) return FALSE;
    if (m_verbs == nullptr || m_position >= m_count) return FALSE;

    // Build the result from the verb at the cursor WITHOUT consuming it, so a
    // failed allocation leaves the enumerator exactly where it was. Advancing
    // first would silently swallow a verb on OOM: the caller gets FALSE, and
    // a retry resumes at the following element.
    const OLEVERB& src = m_verbs[m_position];
    OLEVERB result = src;

    if (src.lpszVerbName != nullptr) {
        const size_t cb = (wcslen(src.lpszVerbName) + 1) * sizeof(wchar_t);
        LPOLESTR copy = static_cast<LPOLESTR>(::CoTaskMemAlloc(cb));
        if (copy == nullptr) {
            return FALSE;   // cursor untouched; caller may retry this verb
        }
        memcpy(copy, src.lpszVerbName, cb);
        result.lpszVerbName = copy;
    }

    *static_cast<OLEVERB*>(pv) = result;
    ++m_position;
    return TRUE;
}
