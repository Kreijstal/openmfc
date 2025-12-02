#pragma once
#include "common_macros.h"

// Pure virtual interface
class ABI_STRESS_API IStage7_Abstract {
public:
    virtual void PureMethod() = 0; // Should point to _purecall or similar
    virtual ~IStage7_Abstract() = default;
};

// novtable optimization test
class __declspec(novtable) ABI_STRESS_API CStage7_NoVtable {
public:
    CStage7_NoVtable();
    virtual void Method();
};
