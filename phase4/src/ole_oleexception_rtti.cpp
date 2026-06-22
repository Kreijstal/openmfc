// OpenMFC: COleException / COleDispatchException RTTI —
// GetThisClass / GetRuntimeClass exports (4 total).
//
// Both are MFC DECLARE_DYNAMIC exception classes, so each has:
//   - a file-internal CRuntimeClass descriptor (not an exported symbol)
//   - static  CRuntimeClass* GetThisClass()           -> &descriptor
//   - virtual CRuntimeClass* GetRuntimeClass() const  -> &descriptor
//
// mfc_exceptions.cpp owns the *throwing* side (AfxThrowOle*, _CxxThrowException
// RTTI) and COleException/COleDispatchException::GetErrorMessage, but defines NO
// CRuntimeClass descriptor and NO GetThisClass/GetRuntimeClass for these two —
// those four exports were weak stubs until now.
//
// RTTI identity graph (matches retail mfc140 RUNTIME_CLASS chains, which is what
// IsKindOf walks):
//   COleDispatchException -> COleException -> CException -> CObject
// CException::classCException is defined in mfc_exceptions.cpp; its m_pBaseClass
// already points at CObject::classCObject. CDaoException RTTI is intentionally
// NOT here — daocore.cpp already owns it (classCDaoException + getters).
//
// m_nObjectSize is sizeof() of the repo's own class (afxdisp.h), so it stays
// consistent with the objects this DLL actually instantiates; the behavioral
// test static_asserts those literals against the headers so they cannot drift.

#include "openmfc/afxwin.h"    // CObject, CException, CRuntimeClass
#include "openmfc/afxdisp.h"   // COleException, COleDispatchException

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// Order: m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
//        m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
// DYNAMIC (not DYNCREATE): no factory; schema 0xFFFF (not serializable).
CRuntimeClass classCOleException = {
    "COleException",
    static_cast<int>(sizeof(COleException)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};
CRuntimeClass classCOleDispatchException = {
    "COleDispatchException",
    static_cast<int>(sizeof(COleDispatchException)),
    0xFFFF,
    nullptr,
    nullptr,
    &classCOleException,       // chains through COleException, like retail
    nullptr
};
} // namespace

// Symbol: ?GetThisClass@COleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleException;
}

// Symbol: ?GetRuntimeClass@COleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleException;
}

// Symbol: ?GetThisClass@COleDispatchException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ() {
    return &classCOleDispatchException;
}

// Symbol: ?GetRuntimeClass@COleDispatchException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(
    const void* /*pThis*/) {
    return &classCOleDispatchException;
}
