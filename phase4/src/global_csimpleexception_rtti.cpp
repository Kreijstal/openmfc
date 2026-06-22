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
static_assert(sizeof(wchar_t) == 2, "Unicode build expects 2-byte wchar_t");

// ---------------------------------------------------------------------------
// Runtime class descriptor. We extern the base descriptor; never redefine it.
// CException::classCException is exported by mfc_exceptions.cpp; referencing it
// via the C++ name resolves to that single definition at link time.
// ---------------------------------------------------------------------------
extern "C" {

CRuntimeClass classCSimpleException = {
    "CSimpleException",                 // m_lpszClassName
    (int)sizeof(CSimpleException),      // m_nObjectSize
    0xFFFF,                             // m_wSchema
    nullptr,                            // m_pfnCreateObject (abstract base, no DYNCREATE)
    nullptr,                            // m_pfnGetBaseClass
    &CException::classCException,        // m_pBaseClass -> CException
    nullptr                            // m_pNextClass
};

} // extern "C"

#ifdef __GNUC__
// Export the descriptor under its MSVC-mangled name as well, for callers that
// reference the static data member directly.
asm(".globl \"?classCSimpleException@CSimpleException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCSimpleException@CSimpleException@@2UCRuntimeClass@@A\", classCSimpleException\n");
#endif

// ---------------------------------------------------------------------------
// ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
//   virtual int GetErrorMessage(wchar_t* lpszError, UINT nMaxError,
//                               UINT* pnHelpContext) const;
// Copies the cached message into lpszError, truncating to nMaxError-1 wide
// chars and NUL-terminating. Sets *pnHelpContext = m_nHelpContext. Returns 1
// if an initialized, non-empty message was produced, else 0.
// ---------------------------------------------------------------------------
// Symbol: ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(
    const CSimpleException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (pnHelpContext) {
        *pnHelpContext = pThis ? pThis->m_nHelpContext : 0;
    }
    if (!lpszError || nMaxError == 0) {
        return 0;
    }
    lpszError[0] = L'\0';
    if (!pThis || !pThis->m_bInitialized || pThis->m_szMessage[0] == L'\0') {
        return 0;
    }
    // Copy at most nMaxError-1 chars, always NUL-terminate.
    UINT i = 0;
    const UINT limit = nMaxError - 1;
    while (i < limit && pThis->m_szMessage[i] != L'\0') {
        lpszError[i] = pThis->m_szMessage[i];
        ++i;
    }
    lpszError[i] = L'\0';
    return 1;
}

// ---------------------------------------------------------------------------
// ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
//   virtual CRuntimeClass* GetRuntimeClass() const;
// ---------------------------------------------------------------------------
// Symbol: ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(
    const CSimpleException* /*pThis*/
) {
    return &classCSimpleException;
}

// ---------------------------------------------------------------------------
// ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
//   static CRuntimeClass* GetThisClass();
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ() {
    return &classCSimpleException;
}

// ---------------------------------------------------------------------------
// ?InitString@CSimpleException@@QEAAXXZ
//   void InitString();
// Marks the cached message as initialized (the message text itself is loaded
// from a resource by derived classes; once that has happened InitString flips
// m_bInitialized so GetErrorMessage will surface the cached text).
// ---------------------------------------------------------------------------
// Symbol: ?InitString@CSimpleException@@QEAAXXZ
extern "C" void MS_ABI impl__InitString_CSimpleException__QEAAXXZ(CSimpleException* pThis) {
    if (!pThis) return;
    pThis->m_bInitialized = TRUE;
}
