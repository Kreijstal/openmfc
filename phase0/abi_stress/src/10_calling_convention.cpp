#include "10_calling_convention.h"

extern "C" int CCStdCall(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

extern "C" int CCThiscall(void* /*self*/, int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

int CCallProbe::SumFive(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}
