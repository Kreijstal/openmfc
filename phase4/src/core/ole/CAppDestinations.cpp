// CAppDestinations — OpenMFC implementation.
// Sources: global_cappdestinations.cpp

#include "detail/CAppDestinationsSupport.h"

// Symbol: ??0CAppDestinations@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAppDestinations__QEAA_XZ(void* pThis) {
    S_Cappdestinations* s = static_cast<S_Cappdestinations*>(pThis);
    s->m_appDests = nullptr;      // CComPtr default -> NULL
    Str(s)->Construct();          // CStringW default -> empty (nil buffer)
    return pThis;
}
// Symbol: ??0CAppDestinations@@QEAA@PEB_W@Z
extern "C" void* MS_ABI impl___0CAppDestinations__QEAA_PEB_W_Z(void* pThis,
                                                               const wchar_t* lpszAppID) {
    S_Cappdestinations* s = static_cast<S_Cappdestinations*>(pThis);
    s->m_appDests = nullptr;
    Str(s)->ConstructPsz(lpszAppID);   // CStringW m_strAppID(lpszAppID)
    return pThis;
}
// Symbol: ?Init@CAppDestinations@@IEAAHXZ
extern "C" int MS_ABI impl__Init_CAppDestinations__IEAAHXZ(void* pThis) {
    return DoInit(static_cast<S_Cappdestinations*>(pThis)) ? TRUE : FALSE;
}
// Symbol: ?RemoveAllDestinations@CAppDestinations@@QEAAHXZ
extern "C" int MS_ABI impl__RemoveAllDestinations_CAppDestinations__QEAAHXZ(void* pThis) {
    S_Cappdestinations* s = static_cast<S_Cappdestinations*>(pThis);
    if (!DoInit(s))
        return FALSE;
    HRESULT hr = s->m_appDests->lpVtbl->RemoveAllDestinations(s->m_appDests);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?RemoveDestination@CAppDestinations@@QEAAHPEAUIUnknown@@@Z
extern "C" int MS_ABI impl__RemoveDestination_CAppDestinations__QEAAHPEAUIUnknown___Z(
        void* pThis, IUnknown* pUnk) {
    S_Cappdestinations* s = static_cast<S_Cappdestinations*>(pThis);
    if (!DoInit(s))
        return FALSE;
    HRESULT hr = s->m_appDests->lpVtbl->RemoveDestination(s->m_appDests, pUnk);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?SetAppID@CAppDestinations@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAppID_CAppDestinations__QEAAXPEB_W_Z(void* pThis,
                                                                     const wchar_t* lpszAppID) {
    S_Cappdestinations* s = static_cast<S_Cappdestinations*>(pThis);
    Str(s)->AssignPsz(lpszAppID);            // m_strAppID = lpszAppID
    if (s->m_appDests != nullptr)            // already created -> forward immediately
        s->m_appDests->lpVtbl->SetAppID(s->m_appDests, s->m_strAppID);
}
