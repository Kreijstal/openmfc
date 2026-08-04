// CWindowDC — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// CWindowDC constructor
// Symbol: ??0CWindowDC@@QEAA@PEAVCWnd@@@Z
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    pThis->m_hDC = ::GetWindowDC(hWnd);
    pThis->m_hAttribDC = pThis->m_hDC;

    return pThis;
}
// CWindowDC destructor
// Symbol: ??1CWindowDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis) {
    if (pThis && pThis->m_hDC) {
        HWND hWnd = pThis->m_pWnd ? pThis->m_pWnd->GetSafeHwnd() : nullptr;
        ::ReleaseDC(hWnd, pThis->m_hDC);
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
    if (pThis) {
        pThis->m_pWnd = nullptr;
    }
}
