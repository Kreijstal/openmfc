#pragma once
// Shared internals of the former global_cappdestinations.cpp translation unit.
// Definitions live in detail/CAppDestinationsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cappdestinations {} } }
using namespace openmfc::detail::cappdestinations;
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
#include "detail/AtlStrTCore.h"   // brings <windows.h>, <oleauto.h>, openmfc/afxstr.h + AtlStrT<CH>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc_str::AtlStrT;


// ---- Minimal IApplicationDestinations (shobjidl) -----------------------------------
// C-style vtable so the interface dispatch does not depend on the C++ compiler's virtual
// layout; the three real methods live after IUnknown's QI/AddRef/Release.
namespace openmfc { namespace detail { namespace cappdestinations {
struct IApplicationDestinations;
} } }
namespace openmfc { namespace detail { namespace cappdestinations {
struct IApplicationDestinationsVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IApplicationDestinations*, const IID&, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IApplicationDestinations*);
    ULONG   (STDMETHODCALLTYPE *Release)(IApplicationDestinations*);
    HRESULT (STDMETHODCALLTYPE *SetAppID)(IApplicationDestinations*, LPCWSTR pszAppID);
    HRESULT (STDMETHODCALLTYPE *RemoveDestination)(IApplicationDestinations*, IUnknown* punk);
    HRESULT (STDMETHODCALLTYPE *RemoveAllDestinations)(IApplicationDestinations*);
};
} } }
namespace openmfc { namespace detail { namespace cappdestinations {
struct IApplicationDestinations { const IApplicationDestinationsVtbl* lpVtbl; };
} } }

// CLSID_ApplicationDestinations {86C14003-4D6B-4EF3-A7B4-0506663B2E68}
namespace openmfc { namespace detail { namespace cappdestinations {
extern const GUID kCLSID_ApplicationDestinations;
} } }
// IID_IApplicationDestinations {12337D35-94C6-48A0-BCE7-6A9C69D4D600}
namespace openmfc { namespace detail { namespace cappdestinations {
extern const GUID kIID_IApplicationDestinations;
} } }

// ---- layout-faithful view ----------------------------------------------------------
namespace openmfc { namespace detail { namespace cappdestinations {
struct S_Cappdestinations {
    IApplicationDestinations* m_appDests;   // offset 0  (CComPtr<...> == raw pointer)
    wchar_t*                  m_strAppID;    // offset 8  (CStringW  == m_pszData)
};
} } }

// The CStringW member is a single m_pszData pointer -- identical to AtlStrT<wchar_t>'s
// layout -- so we operate on it through the ATL-binary-compatible string core.
namespace openmfc { namespace detail { namespace cappdestinations {
AtlStrT<wchar_t>* Str(S_Cappdestinations* s);
} } }

// CAppDestinations::Init(): lazily create the COM helper and push the app id into it.
namespace openmfc { namespace detail { namespace cappdestinations {
BOOL DoInit(S_Cappdestinations* s);
} } }


// ------------------------------------------------------------------------------------
// Exported thunks
// ------------------------------------------------------------------------------------






