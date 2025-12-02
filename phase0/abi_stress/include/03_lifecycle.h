#pragma once
#include "common_macros.h"

class ABI_STRESS_API CStage3_Base {
public:
    CStage3_Base();
    virtual ~CStage3_Base(); // Should generate scalar deleting destructor
    int m_base;
};
