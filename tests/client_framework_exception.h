#pragma once
#include "openmfc/afxwin.h"
class TestFrameworkException : public CException {
public:
    using CException::CException;
    ~TestFrameworkException() override = default;
};
