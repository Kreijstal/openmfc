#include "OleOleexceptionRttiSupport.h"

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
