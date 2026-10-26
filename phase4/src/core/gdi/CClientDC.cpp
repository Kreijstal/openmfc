// Client-area borrowed DC ownership and release-x64 layout.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"
#include <cstring>
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();
CClientDC::CClientDC(CWnd* window):CClientDC(window,AcquisitionState{}) {}
CClientDC::CClientDC(CWnd* window,AcquisitionState state) try :m_hWnd(window?window->m_hWnd:nullptr) {
    state.vtable=*reinterpret_cast<void**>(this);
    HDC handle;
    try {handle=::GetDC(m_hWnd);}
    catch(...) {state.output=m_hDC;m_hDC=nullptr;throw;}
    state.vtable=nullptr;
    if(!Attach(handle))impl__AfxThrowResourceException__YAXXZ();
} catch(...) {
    // Match the native constructor's acquisition-region unwind state.
    if(state.vtable) {
        std::memcpy(static_cast<void*>(this),&state.vtable,sizeof(state.vtable));
        std::memcpy(reinterpret_cast<BYTE*>(this)+8,&state.output,sizeof(state.output));
    }
    throw;
}
CClientDC::~CClientDC() {
    HDC handle=Detach();
    ::ReleaseDC(m_hWnd,handle);
}
// Symbol: ??0CClientDC@@QEAA@PEAVCWnd@@@Z
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* self,CWnd* window) {return new(self) CClientDC(window);}
// Symbol: ??1CClientDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* self) {self->CClientDC::~CClientDC();}
// Symbol: ?GetRuntimeClass@CClientDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(const CClientDC*) {return &CClientDC::classCClientDC;}
// Symbol: ?GetThisClass@CClientDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ() {return &CClientDC::classCClientDC;}
