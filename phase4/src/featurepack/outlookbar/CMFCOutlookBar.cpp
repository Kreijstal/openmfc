// CMFCOutlookBar — OpenMFC implementation.
// Sources: global_cmfcoutlookbar.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCOutlookBarSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail (0x180097f70):
//     if (m_hWnd == NULL) m_bMode2003 = bMode;
// (The retail NULL-this path falls through to the store and would fault; we
//  guard it instead.)
// Symbol: ?SetMode2003@CMFCOutlookBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMode2003_CMFCOutlookBar__QEAAXH_Z(void* pThis, int bMode2003)
{
    if (pThis == nullptr)
        return;

    OutlookBar* pBar = static_cast<OutlookBar*>(pThis);
    if (pBar->m_hWnd == nullptr) {
        pBar->m_bMode2003 = bMode2003;
    }
}
// Symbol: ??0CMFCOutlookBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBar__QEAA_XZ(void* pThis) {
    return pThis;
}
