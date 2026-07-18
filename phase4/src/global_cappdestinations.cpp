// OpenMFC: real implementation of CAppDestinations (Windows 7 Jump List / taskbar
// "application destinations" helper). The class wraps a COM IApplicationDestinations
// object plus a stored Application User Model ID string.
//
// Harvested layout  (cl.exe /d1reportSingleClassLayout), size(16):
//    0 | ::ATL::CComPtr<IApplicationDestinations> m_appDests   (single raw pointer)
//    8 | CStringW                                 m_strAppID   (single m_pszData pointer)
//
// Not polymorphic (no vfptr). No exported destructor: a client destroys the object with
// its own inlined CComPtr::~CComPtr / CStringW::~CStringW, so the string we allocate MUST
// be byte-compatible with the exported CStringW manager -- hence AtlStrT<wchar_t>.
#include "atl_strt_core.h"   // brings <windows.h>, <oleauto.h>, openmfc/afxstr.h + AtlStrT<CH>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc_str::AtlStrT;

namespace {

// ---- Minimal IApplicationDestinations (shobjidl) -----------------------------------
// C-style vtable so the interface dispatch does not depend on the C++ compiler's virtual
// layout; the three real methods live after IUnknown's QI/AddRef/Release.
struct IApplicationDestinations;
struct IApplicationDestinationsVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IApplicationDestinations*, const IID&, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IApplicationDestinations*);
    ULONG   (STDMETHODCALLTYPE *Release)(IApplicationDestinations*);
    HRESULT (STDMETHODCALLTYPE *SetAppID)(IApplicationDestinations*, LPCWSTR pszAppID);
    HRESULT (STDMETHODCALLTYPE *RemoveDestination)(IApplicationDestinations*, IUnknown* punk);
    HRESULT (STDMETHODCALLTYPE *RemoveAllDestinations)(IApplicationDestinations*);
};
struct IApplicationDestinations { const IApplicationDestinationsVtbl* lpVtbl; };

// CLSID_ApplicationDestinations {86C14003-4D6B-4EF3-A7B4-0506663B2E68}
static const GUID kCLSID_ApplicationDestinations =
    { 0x86c14003, 0x4d6b, 0x4ef3, { 0xa7, 0xb4, 0x05, 0x06, 0x66, 0x3b, 0x2e, 0x68 } };
// IID_IApplicationDestinations {12337D35-94C6-48A0-BCE7-6A9C69D4D600}
static const GUID kIID_IApplicationDestinations =
    { 0x12337d35, 0x94c6, 0x48a0, { 0xbc, 0xe7, 0x6a, 0x9c, 0x69, 0xd4, 0xd6, 0x00 } };

// ---- layout-faithful view ----------------------------------------------------------
struct S {
    IApplicationDestinations* m_appDests;   // offset 0  (CComPtr<...> == raw pointer)
    wchar_t*                  m_strAppID;    // offset 8  (CStringW  == m_pszData)
};
static_assert(sizeof(S) == 16, "CAppDestinations size(16)");
static_assert(offsetof(S, m_appDests) == 0, "m_appDests @0");
static_assert(offsetof(S, m_strAppID) == 8, "m_strAppID @8");

// The CStringW member is a single m_pszData pointer -- identical to AtlStrT<wchar_t>'s
// layout -- so we operate on it through the ATL-binary-compatible string core.
static inline AtlStrT<wchar_t>* Str(S* s) {
    return reinterpret_cast<AtlStrT<wchar_t>*>(&s->m_strAppID);
}

// CAppDestinations::Init(): lazily create the COM helper and push the app id into it.
static BOOL DoInit(S* s) {
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

} // namespace

// ------------------------------------------------------------------------------------
// Exported thunks
// ------------------------------------------------------------------------------------

// Symbol: ??0CAppDestinations@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAppDestinations__QEAA_XZ(void* pThis) {
    S* s = static_cast<S*>(pThis);
    s->m_appDests = nullptr;      // CComPtr default -> NULL
    Str(s)->Construct();          // CStringW default -> empty (nil buffer)
    return pThis;
}

// Symbol: ??0CAppDestinations@@QEAA@PEB_W@Z
extern "C" void* MS_ABI impl___0CAppDestinations__QEAA_PEB_W_Z(void* pThis,
                                                               const wchar_t* lpszAppID) {
    S* s = static_cast<S*>(pThis);
    s->m_appDests = nullptr;
    Str(s)->ConstructPsz(lpszAppID);   // CStringW m_strAppID(lpszAppID)
    return pThis;
}

// Symbol: ?Init@CAppDestinations@@IEAAHXZ
extern "C" int MS_ABI impl__Init_CAppDestinations__IEAAHXZ(void* pThis) {
    return DoInit(static_cast<S*>(pThis)) ? TRUE : FALSE;
}

// Symbol: ?RemoveAllDestinations@CAppDestinations@@QEAAHXZ
extern "C" int MS_ABI impl__RemoveAllDestinations_CAppDestinations__QEAAHXZ(void* pThis) {
    S* s = static_cast<S*>(pThis);
    if (!DoInit(s))
        return FALSE;
    HRESULT hr = s->m_appDests->lpVtbl->RemoveAllDestinations(s->m_appDests);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// Symbol: ?RemoveDestination@CAppDestinations@@QEAAHPEAUIUnknown@@@Z
extern "C" int MS_ABI impl__RemoveDestination_CAppDestinations__QEAAHPEAUIUnknown___Z(
        void* pThis, IUnknown* pUnk) {
    S* s = static_cast<S*>(pThis);
    if (!DoInit(s))
        return FALSE;
    HRESULT hr = s->m_appDests->lpVtbl->RemoveDestination(s->m_appDests, pUnk);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

// Symbol: ?SetAppID@CAppDestinations@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAppID_CAppDestinations__QEAAXPEB_W_Z(void* pThis,
                                                                     const wchar_t* lpszAppID) {
    S* s = static_cast<S*>(pThis);
    Str(s)->AssignPsz(lpszAppID);            // m_strAppID = lpszAppID
    if (s->m_appDests != nullptr)            // already created -> forward immediately
        s->m_appDests->lpVtbl->SetAppID(s->m_appDests, s->m_strAppID);
}
