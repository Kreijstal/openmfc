// CPaintDC — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CRuntimeClassSupport.h"

// CPaintDC constructor
// Symbol: ??0CPaintDC@@QEAA@PEAVCWnd@@@Z
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;

    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    pThis->m_pWnd = pWnd;
    std::memset(&pThis->m_ps, 0, sizeof(pThis->m_ps));

    HWND hWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    if (hWnd) {
        pThis->m_hDC = ::BeginPaint(hWnd, &pThis->m_ps);
        pThis->m_hAttribDC = pThis->m_hDC;
    }

    return pThis;
}
// CPaintDC destructor
// Symbol: ??1CPaintDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis) {
    if (pThis && pThis->m_pWnd) {
        HWND hWnd = pThis->m_pWnd->GetSafeHwnd();
        if (hWnd) {
            ::EndPaint(hWnd, &pThis->m_ps);
        }
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;  // Also reset attrib DC
    }
}
// Symbol: ?GetRuntimeClass@CPaintDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPaintDC__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    InitializeClasses();
    return &CPaintDC::classCPaintDC;
}
// Symbol: ?GetThisClass@CPaintDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPaintDC__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &CPaintDC::classCPaintDC;
}
