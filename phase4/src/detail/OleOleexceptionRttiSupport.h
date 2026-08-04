#pragma once
// Shared internals of the former ole_oleexception_rtti.cpp translation unit.
// Definitions live in detail/OleOleexceptionRttiSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace oleoleexceptionrtti {} } }
using namespace openmfc::detail::oleoleexceptionrtti;
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

// Order: m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
//        m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
// DYNAMIC (not DYNCREATE): no factory; schema 0xFFFF (not serializable).
extern CRuntimeClass classCOleException;
extern CRuntimeClass classCOleDispatchException;




