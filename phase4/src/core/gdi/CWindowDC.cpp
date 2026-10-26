// Whole-window borrowed DC ownership and release-x64 layout.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();
CWindowDC::CWindowDC(CWnd* window):CWindowDC(window,AcquisitionState{}) {}
CWindowDC::CWindowDC(CWnd* window,AcquisitionState state) try :m_hWnd(window?window->m_hWnd:nullptr) {
    state.vtable=*reinterpret_cast<void**>(this);
    HDC handle;
    try {handle=::GetWindowDC(m_hWnd);}
    catch(...) {state.output=m_hDC;m_hDC=nullptr;throw;}
    state.vtable=nullptr;
    if(!Attach(handle))impl__AfxThrowResourceException__YAXXZ();
} catch(...) {
    // Native unwind metadata excludes the acquisition import from base
    // cleanup. Retain its derived vptr and any callback-written output handle.
    if(state.vtable) {
        std::memcpy(static_cast<void*>(this),&state.vtable,sizeof(state.vtable));
        std::memcpy(reinterpret_cast<BYTE*>(this)+8,&state.output,sizeof(state.output));
    }
    throw;
}
CWindowDC::~CWindowDC() {
    HDC handle=Detach();
    ::ReleaseDC(m_hWnd,handle);
}
// Symbol: ??0CWindowDC@@QEAA@PEAVCWnd@@@Z
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* self,CWnd* window) {return new(self) CWindowDC(window);}
// Symbol: ??1CWindowDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* self) {self->CWindowDC::~CWindowDC();}
// Symbol: ?GetRuntimeClass@CWindowDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWindowDC__UEBAPEAUCRuntimeClass__XZ(const CWindowDC*) {return &CWindowDC::classCWindowDC;}
// Symbol: ?GetThisClass@CWindowDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWindowDC__SAPEAUCRuntimeClass__XZ() {return &CWindowDC::classCWindowDC;}
