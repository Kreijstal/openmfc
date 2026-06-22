// OpenMFC: CLongBinary RTTI — GetThisClass / GetRuntimeClass.
//
// CLongBinary : public CObject (afxdao.h) is a simple holder for a block of
// binary data (HGLOBAL m_hData + DWORD m_dwDataLength) used by the database
// classes for long-binary columns. It is IMPLEMENT_DYNAMIC (no DYNCREATE
// factory), so only the two RTTI getters are exported; the descriptor itself is
// internal (not an exported symbol). The descriptor chains to CObject.
//
// Layout (verified at runtime): sizeof(CLongBinary) == 24 (vptr + HGLOBAL +
// DWORD, padded). The behavioral test static_asserts this against the real
// afxdao.h type so the literal below cannot silently drift.

#include "openmfc/afx.h"   // CObject, CRuntimeClass, CObject::classCObject

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
CRuntimeClass classCLongBinary = {
    "CLongBinary",
    24,                       // m_nObjectSize (sizeof(CLongBinary), runtime-verified)
    0xFFFF,                   // m_wSchema (DYNAMIC, not serializable)
    nullptr,                  // m_pfnCreateObject (no DYNCREATE factory)
    nullptr,                  // m_pfnGetBaseClass
    &CObject::classCObject,   // m_pBaseClass
    nullptr                   // m_pNextClass
};
} // namespace

// ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ() {
    return &classCLongBinary;
}

// ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCLongBinary;
}
