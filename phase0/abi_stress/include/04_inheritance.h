#pragma once
#include "common_macros.h"

class ABI_STRESS_API CStage4_A {
public:
    virtual void FuncA();
    int m_a;
};

class ABI_STRESS_API CStage4_B {
public:
    virtual void FuncB(); // Calling this on CStage4_Multi requires 'this' adjustment
    int m_b;
};

class ABI_STRESS_API CStage4_Multi : public CStage4_A, public CStage4_B {
public:
    virtual void FuncA() override;
    virtual void FuncB() override;
    int m_multi;
};
