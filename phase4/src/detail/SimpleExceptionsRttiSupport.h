#pragma once
// Shared internals of the former global_simple_exceptions_rtti.cpp translation unit.
// Definitions live in detail/SimpleExceptionsRttiSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace simpleexceptionsrtti {} } }
using namespace openmfc::detail::simpleexceptionsrtti;
// OpenMFC: MFC RTTI descriptors + GetThisClass/GetRuntimeClass exports for the
// four parameterless derived exceptions:
//   CInvalidArgException_SimpleExceptionsRtti, CNotSupportedException_SimpleExceptionsRtti, CResourceException_SimpleExceptionsRtti, CUserException_SimpleExceptionsRtti
//
// Each of these is an MFC exception declared via DECLARE_DYNAMIC in real MFC, so
// each has:
//   - a static CRuntimeClass descriptor (classXXX)
//   - static CRuntimeClass* GetThisClass()           -> &classXXX
//   - virtual CRuntimeClass* GetRuntimeClass() const -> &classXXX
//
// mfc_exceptions.cpp owns the *throwing* side (AfxThrow*, _CxxThrowException RTTI)
// and declares minimal file-local versions of these classes, but defines NO
// CRuntimeClass descriptors and NO GetThisClass/GetRuntimeClass exports for them.
// This translation unit owns those 8 RTTI exports.
//
// The base chain is: CXxxException -> CException -> CObject. We chain
// m_pNextClass to CException::classCException (defined in mfc_exceptions.cpp);
// CException::classCException already links to CObject::classCObject through its
// m_pfnGetBaseClass thunk.

#include "openmfc/afxwin.h"

// MS ABI calling convention (matches mfc_exceptions.cpp; not in a shared header).
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Minimal, ABI-matching local class defs. These mirror the real MFC layout:
// CException adds a single int m_bAutoDelete after the vptr; these derived
// classes add no data members, so sizeof == sizeof(CException) == 16.
namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
class CInvalidArgException_SimpleExceptionsRtti : public CException {
public:
    CInvalidArgException_SimpleExceptionsRtti() : CException(1) {}
};
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
class CNotSupportedException_SimpleExceptionsRtti : public CException {
public:
    CNotSupportedException_SimpleExceptionsRtti() : CException(1) {}
};
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
class CResourceException_SimpleExceptionsRtti : public CException {
public:
    CResourceException_SimpleExceptionsRtti() : CException(1) {}
};
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
class CUserException_SimpleExceptionsRtti : public CException {
public:
    CUserException_SimpleExceptionsRtti() : CException(1) {}
};
} } }

// Real CRuntimeClass descriptors.
// Order: m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
//        m_pfnGetBaseClass, m_pNextClass, m_pClassInit
// These exceptions are DYNAMIC, not DYNCREATE, so there is no factory function.
namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
extern CRuntimeClass class_CInvalidArgException;
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
extern CRuntimeClass class_CNotSupportedException;
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
extern CRuntimeClass class_CResourceException;
} } }

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
extern CRuntimeClass class_CUserException;
} } }





