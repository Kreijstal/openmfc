#pragma once
#include "common_macros.h"

// Stage 1: Basic Layout
// Goal: Determine vptr offset and member alignment on x64.
class ABI_STRESS_API CStage1_Simple {
public:
    CStage1_Simple();
    virtual ~CStage1_Simple();

    // Virtual method ensures a vtable exists
    virtual int GetValue() const;
    virtual void SetValue(int v);

private:
    // We expect the vptr to be at offset 0 (8 bytes on x64).
    // Where does this int land? Offset 8?
    // What is the total size? 16 bytes (padding)?
    int m_value;
};
