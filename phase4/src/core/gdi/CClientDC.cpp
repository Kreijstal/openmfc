// CClientDC — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CClientDC constructor
// Symbol: ??0CClientDC@@QEAA@PEAVCWnd@@@Z
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    pThis->m_hDC = ::GetDC(hWnd);
    pThis->m_hAttribDC = pThis->m_hDC;

    return pThis;
}
// CClientDC destructor
// Symbol: ??1CClientDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis) {
    if (pThis && pThis->m_hDC) {
        HWND hWnd = pThis->m_pWnd ? pThis->m_pWnd->GetSafeHwnd() : nullptr;
        ::ReleaseDC(hWnd, pThis->m_hDC);
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
}
// Symbol: ?GetRuntimeClass@CClientDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@CClientDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
