#pragma once
#include "common_macros.h"

// POD type: standard C-struct behavior
struct Pod8 { int a, b; };
struct Pod16 { long long a, b; };
struct Pod24 { long long a, b, c; };

// Non-POD type: has a destructor, might force hidden pointer return
struct NonPod8 {
    int a, b;
    ~NonPod8() {}
};

class ABI_STRESS_API CStage5_RVO {
public:
    // We check disassembly to see if these return in RAX, XMM0, or via hidden ptr (RCX/RDX)
    static Pod8 RetPod8();
    static Pod16 RetPod16();
    static Pod24 RetPod24();
    static NonPod8 RetNonPod8();
};
