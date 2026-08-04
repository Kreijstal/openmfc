// CTime — OpenMFC implementation.
// Sources: global_ctime_serialization.cpp

#include "detail/CTimeSupport.h"

// Symbol: ?Serialize64@CTime@ATL@@QEAAAEAVCArchive@@AEAV3@@Z
extern "C" CArchive* MS_ABI impl__Serialize64_CTime_ATL__QEAAAEAVCArchive__AEAV3__Z(
    void* pThis,
    CArchive* archive
) {
    return SerializeTime64(pThis, archive);
}
