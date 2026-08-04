// CParkingWnd — OpenMFC implementation.
// Sources: global_cparkingwnd.cpp

#include "detail/CParkingWndSupport.h"

// Symbol: ?WindowProc@CParkingWnd@@UEAA_JI_K_J@Z
extern "C" std::intptr_t MS_ABI impl__WindowProc_CParkingWnd__UEAA_JI_K_J_Z(
    CWnd* pThis,
    unsigned int message,
    std::uintptr_t wParam,
    std::intptr_t lParam
) {
    return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
