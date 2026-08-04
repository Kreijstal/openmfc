// CPrintInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CPrintInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPrintInfo__QEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        TrackCPrintInfo(pThis);
    }
    return pThis;
}
// Symbol: ??1CPrintInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1CPrintInfo__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    UntrackCPrintInfo(pThis);
}
