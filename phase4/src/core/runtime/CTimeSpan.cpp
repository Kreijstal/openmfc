// CTimeSpan — OpenMFC implementation.
// Sources: global_ctime_serialization.cpp

#include "detail/CTimeSupport.h"

// Symbol: ?Serialize64@CTimeSpan@ATL@@QEAAAEAVCArchive@@AEAV3@@Z
extern "C" CArchive* MS_ABI impl__Serialize64_CTimeSpan_ATL__QEAAAEAVCArchive__AEAV3__Z(
    void* pThis,
    CArchive* archive
) {
    return SerializeTime64(pThis, archive);
}
