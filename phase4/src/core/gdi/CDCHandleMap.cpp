// Native output/attribute handle ownership and the module's permanent DC map.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
BOOL CDC::Attach(HDC handle) {
    if(!handle)return FALSE;
    m_hDC=handle;
    try {GetDCHandleMaps(true)->permanent[handle]=this;}
    catch(const std::bad_alloc&) {impl__AfxThrowMemoryException__YAXXZ();}
    SetAttribDC(m_hDC);
    return TRUE;
}
HDC CDC::Detach() {
    HDC previous=m_hDC;
    if(previous)if(auto* maps=GetDCHandleMaps(false))maps->permanent.erase(m_hDC);
    ReleaseAttribDC();
    m_hDC=nullptr;
    return previous;
}
// Symbol: ?Attach@CDC@@QEAAHPEAUHDC__@@@Z
extern "C" BOOL MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC* self,HDC handle) {return self->Attach(handle);}
// Symbol: ?Detach@CDC@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__Detach_CDC__QEAAPEAUHDC____XZ(CDC* self) {return self->Detach();}
