#pragma once
#include "common_macros.h"

class ABI_STRESS_API CStage2_Signatures {
public:
    virtual void Func(int a);
    virtual void Func(int* a);
    virtual void Func(const int* a);
    virtual void Func(int& a);
    virtual void Func(const int& a);
    virtual void Func(volatile int* a);
};
