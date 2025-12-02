#pragma once
#include "common_macros.h"

class ABI_STRESS_API IPure {
public:
    virtual void PureMethod() = 0;
    virtual ~IPure() = default;
};

// Intentional purecall trigger; not instantiated, but vftable should point to _purecall.
class ABI_STRESS_API PureCaller {
public:
    static void CallPure(IPure* p);
};
