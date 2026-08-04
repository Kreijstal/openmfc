#include "SimpleExceptionsRttiSupport.h"

namespace openmfc { namespace detail { namespace simpleexceptionsrtti {
CRuntimeClass class_CInvalidArgException = {
    "CInvalidArgException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CInvalidArgException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};
CRuntimeClass class_CNotSupportedException = {
    "CNotSupportedException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CNotSupportedException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};
CRuntimeClass class_CResourceException = {
    "CResourceException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CResourceException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};
CRuntimeClass class_CUserException = {
    "CUserException_SimpleExceptionsRtti",
    static_cast<int>(sizeof(CUserException_SimpleExceptionsRtti)),
    0xFFFF,
    nullptr,
    nullptr,
    &CException::classCException,
    nullptr
};
} } }  // namespace openmfc::detail::simpleexceptionsrtti
