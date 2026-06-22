// OpenMFC: MFC RTTI descriptors + GetThisClass/GetRuntimeClass exports for the
// four parameterless derived exceptions:
//   CInvalidArgException, CNotSupportedException, CResourceException, CUserException
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
// m_pBaseClass to CException::classCException (defined in mfc_exceptions.cpp);
// CException::classCException.m_pBaseClass already points at CObject::classCObject.

#include "openmfc/afxwin.h"

// MS ABI calling convention (matches mfc_exceptions.cpp; not in a shared header).
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Minimal, ABI-matching local class defs. These mirror the real MFC layout:
// CException adds a single int m_bAutoDelete after the vptr; these derived
// classes add no data members, so sizeof == sizeof(CException) == 16.
class CInvalidArgException : public CException {
public:
    CInvalidArgException() : CException(1) {}
};

class CNotSupportedException : public CException {
public:
    CNotSupportedException() : CException(1) {}
};

class CResourceException : public CException {
public:
    CResourceException() : CException(1) {}
};

class CUserException : public CException {
public:
    CUserException() : CException(1) {}
};

// Real CRuntimeClass descriptors.
// Order: m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
//        m_pfnGetBaseClass, m_pBaseClass, m_pNextClass
// These exceptions are DYNAMIC, not DYNCREATE, so there is no factory function.
CRuntimeClass class_CInvalidArgException = {
    "CInvalidArgException",
    static_cast<int>(sizeof(CInvalidArgException)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};

CRuntimeClass class_CNotSupportedException = {
    "CNotSupportedException",
    static_cast<int>(sizeof(CNotSupportedException)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};

CRuntimeClass class_CResourceException = {
    "CResourceException",
    static_cast<int>(sizeof(CResourceException)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};

CRuntimeClass class_CUserException = {
    "CUserException",
    static_cast<int>(sizeof(CUserException)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};

} // namespace

// ---- CInvalidArgException ----------------------------------------------------
// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ() {
    return &class_CInvalidArgException;
}
// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CInvalidArgException;
}

// ---- CNotSupportedException --------------------------------------------------
// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ() {
    return &class_CNotSupportedException;
}
// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CNotSupportedException;
}

// ---- CResourceException ------------------------------------------------------
// Symbol: ?GetThisClass@CResourceException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ() {
    return &class_CResourceException;
}
// Symbol: ?GetRuntimeClass@CResourceException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CResourceException;
}

// ---- CUserException ----------------------------------------------------------
// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ() {
    return &class_CUserException;
}
// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(const void* /*pThis*/) {
    return &class_CUserException;
}
