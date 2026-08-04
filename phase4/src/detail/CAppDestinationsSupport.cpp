#include "CAppDestinationsSupport.h"

static_assert(sizeof(S_Cappdestinations) == 16, "CAppDestinations size(16)");
static_assert(offsetof(S_Cappdestinations, m_appDests) == 0, "m_appDests @0");
static_assert(offsetof(S_Cappdestinations, m_strAppID) == 8, "m_strAppID @8");
namespace openmfc { namespace detail { namespace cappdestinations {
const GUID kCLSID_ApplicationDestinations =
    { 0x86c14003, 0x4d6b, 0x4ef3, { 0xa7, 0xb4, 0x05, 0x06, 0x66, 0x3b, 0x2e, 0x68 } };
const GUID kIID_IApplicationDestinations =
    { 0x12337d35, 0x94c6, 0x48a0, { 0xbc, 0xe7, 0x6a, 0x9c, 0x69, 0xd4, 0xd6, 0x00 } };
AtlStrT<wchar_t>* Str(S_Cappdestinations* s) {
    return reinterpret_cast<AtlStrT<wchar_t>*>(&s->m_strAppID);
}
BOOL DoInit(S_Cappdestinations* s) {
    if (s->m_appDests != nullptr)
        return TRUE;
    IApplicationDestinations* p = nullptr;
    HRESULT hr = ::CoCreateInstance(kCLSID_ApplicationDestinations, nullptr,
                                    CLSCTX_INPROC_SERVER, kIID_IApplicationDestinations,
                                    reinterpret_cast<void**>(&p));
    if (FAILED(hr) || p == nullptr)
        return FALSE;
    s->m_appDests = p;
    if (Str(s)->GetLength() != 0) {
        if (FAILED(p->lpVtbl->SetAppID(p, Str(s)->GetString())))
            return FALSE;
    }
    return TRUE;
}
} } }  // namespace openmfc::detail::cappdestinations
