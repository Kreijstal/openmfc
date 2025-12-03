#include "11_rtti_harvest.h"
#include <typeinfo>

CRttiBase::~CRttiBase() {}
CRttiDerived::~CRttiDerived() {}

// Force compiler to emit RTTI structures by using typeid/dynamic_cast.
static void ForceRttiEmission() {
    CRttiDerived d;
    CRttiBase* b = &d;
    (void)typeid(*b).name();
    (void)dynamic_cast<CRttiDerived*>(b);
}

// Anchor to keep the function and RTTI from being discarded.
extern "C" __declspec(dllexport) void* g_rtti_anchor = (void*)&ForceRttiEmission;
