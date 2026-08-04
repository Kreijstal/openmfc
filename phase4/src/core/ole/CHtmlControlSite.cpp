// CHtmlControlSite — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CHtmlControlSite@@QEAA@PEAVCOleControlContainer@@@Z
extern "C" void* MS_ABI impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(void* pThis, void* pContainer) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) COleControlSite(static_cast<COleControlContainer*>(pContainer));
}
// Symbol: ??1CHtmlControlSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlControlSite__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<COleControlSite*>(pThis)->~COleControlSite();
}
