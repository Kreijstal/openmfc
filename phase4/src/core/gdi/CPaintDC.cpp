// CPaintDC — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();
CPaintDC::CPaintDC(CWnd* window):m_hWnd(window->m_hWnd) {
    if(!Attach(::BeginPaint(m_hWnd,&m_ps)))impl__AfxThrowResourceException__YAXXZ();
}
CPaintDC::~CPaintDC() {
    ::EndPaint(m_hWnd,&m_ps);
    Detach();
}
// Symbol: ??0CPaintDC@@QEAA@PEAVCWnd@@@Z
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* self,CWnd* window) { return new(self) CPaintDC(window); }
// Symbol: ??1CPaintDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* self) { self->CPaintDC::~CPaintDC(); }
// Symbol: ?GetRuntimeClass@CPaintDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPaintDC__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return &CPaintDC::classCPaintDC;
}
// Symbol: ?GetThisClass@CPaintDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPaintDC__SAPEAUCRuntimeClass__XZ() {
    return &CPaintDC::classCPaintDC;
}
