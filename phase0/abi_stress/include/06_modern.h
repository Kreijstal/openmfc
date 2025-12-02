#pragma once
#include "common_macros.h"
#include <utility>

class ABI_STRESS_API CStage6_Modern {
public:
    CStage6_Modern();
    CStage6_Modern(CStage6_Modern&& other) noexcept; // Move ctor
    CStage6_Modern& operator=(CStage6_Modern&& other) noexcept; // Move assign

    virtual void NoExceptFunc() noexcept;
    virtual void ConstFunc() const;
};
