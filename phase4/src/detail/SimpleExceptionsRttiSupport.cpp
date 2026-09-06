#include "SimpleExceptionsRttiSupport.h"

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
static CRuntimeClass* AFXAPI _openmfc_gb_class_CInvalidArgException() { return &CException::classCException; }
CRuntimeClass class_CInvalidArgException = {
    "CInvalidArgException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CInvalidArgException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_class_CInvalidArgException,
    nullptr,
    nullptr
};
static CRuntimeClass* AFXAPI _openmfc_gb_class_CNotSupportedException() { return &CException::classCException; }
CRuntimeClass class_CNotSupportedException = {
    "CNotSupportedException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CNotSupportedException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_class_CNotSupportedException,
    nullptr,
    nullptr
};
static CRuntimeClass* AFXAPI _openmfc_gb_class_CResourceException() { return &CException::classCException; }
CRuntimeClass class_CResourceException = {
    "CResourceException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CResourceException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_class_CResourceException,
    nullptr,
    nullptr
};
static CRuntimeClass* AFXAPI _openmfc_gb_class_CUserException() { return &CException::classCException; }
CRuntimeClass class_CUserException = {
    "CUserException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CUserException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    &_openmfc_gb_class_CUserException,
    nullptr,
    nullptr
};
} } }  // namespace openmfc::detail::simpleexceptionsrtti
