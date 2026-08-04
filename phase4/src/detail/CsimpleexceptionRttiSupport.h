#pragma once
// Shared internals of the former global_csimpleexception_rtti.cpp translation unit.
// Definitions live in detail/CsimpleexceptionRttiSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace csimpleexceptionrtti {} } }
using namespace openmfc::detail::csimpleexceptionrtti;
// OpenMFC: CSimpleException RTTI descriptor + four exported methods.
//
// CSimpleException is NOT defined in the OpenMFC headers (only referenced in a
// comment in mfc_exceptions.cpp). The real MFC layout (atlmfc/include/afx.h) is:
//
//   class CSimpleException : public CException {
//       BOOL    m_bInitialized;
//       UINT    m_nResourceID;
//       UINT    m_nHelpContext;
//       TCHAR   m_szMessage[256];   // wchar_t in the Unicode (u) build
//   };
//
// We faithfully reproduce that layout here so the descriptor's m_nObjectSize is
// a real value and the member offsets used by GetErrorMessage/InitString match
// the genuine class.
//
// The four methods implemented (all confirmed present as alias lines in
// build-phase4/openmfc.def):
//   ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
//   ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
//   ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
//   ?InitString@CSimpleException@@QEAAXXZ
//
// The descriptor chains CSimpleException -> CException -> CObject using the
// externally-provided base descriptors (defined in mfc_exceptions.cpp and
// cobject_impl.cpp). We must NOT redefine those.

#include "openmfc/afxwin.h"   // CException, CObject, CRuntimeClass, BOOL/UINT via windows
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Faithful CSimpleException layout.
// ---------------------------------------------------------------------------
class CSimpleException : public CException {
public:
    BOOL    m_bInitialized;
    UINT    m_nResourceID;
    UINT    m_nHelpContext;
    wchar_t m_szMessage[256];
};

// Base CException is: vptr(8) + int m_bAutoDelete(4) -> padded to 16.
// CSimpleException adds: BOOL(4) + UINT(4) + UINT(4) + wchar_t[256](512).
// On the MinGW host wchar_t is 2 bytes, matching the MSVC Unicode build.

// ---------------------------------------------------------------------------
// Runtime class descriptor. We extern the base descriptor; never redefine it.
// CException::classCException is exported by mfc_exceptions.cpp; referencing it
// via the C++ name resolves to that single definition at link time.
// ---------------------------------------------------------------------------
extern "C" {


extern CRuntimeClass classCSimpleException;

}

#ifdef __GNUC__
// Export the descriptor under its MSVC-mangled name as well, for callers that
// reference the static data member directly.
asm(".globl \"?classCSimpleException@CSimpleException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCSimpleException@CSimpleException@@2UCRuntimeClass@@A\", classCSimpleException\n");
#endif




