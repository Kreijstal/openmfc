#pragma once
#include "common_macros.h"

// Covariant return probe
class ABI_STRESS_API CCovariantBase {
public:
    virtual CCovariantBase* Clone();
    virtual ~CCovariantBase() = default;
};

class ABI_STRESS_API CCovariantDerived : public CCovariantBase {
public:
    virtual CCovariantDerived* Clone() override;
    virtual ~CCovariantDerived() = default;
};
