#pragma once
#include "common_macros.h"

extern "C" ABI_STRESS_API int CCStdCall(int a, int b, int c, int d, int e);
extern "C" ABI_STRESS_API int CCThiscall(void* self, int a, int b, int c, int d, int e);

class ABI_STRESS_API CCallProbe {
public:
    int SumFive(int a, int b, int c, int d, int e);
};
