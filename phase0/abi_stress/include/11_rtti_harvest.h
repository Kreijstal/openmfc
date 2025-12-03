#pragma once
#include "common_macros.h"

// Simple hierarchy to force full RTTI emission (type descriptors, COL, hierarchy descriptors).
class ABI_STRESS_API CRttiBase {
public:
    virtual ~CRttiBase();
    int m_base;
};

class ABI_STRESS_API CRttiDerived : public CRttiBase {
public:
    virtual ~CRttiDerived();
    int m_derived;
};
