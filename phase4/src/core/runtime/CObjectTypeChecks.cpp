// Native runtime-class validation and ancestry traversal.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
void AFXAPI AfxThrowInvalidArgException() {impl__AfxThrowInvalidArgException__YAXXZ();}
// Symbol: ?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z
extern "C" BOOL MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(const CRuntimeClass* self,const CRuntimeClass* base) {
    if(!self || !base)impl__AfxThrowInvalidArgException__YAXXZ();
    const CRuntimeClass* current=self;
    while(current!=base) {
        auto next=current->m_pfnGetBaseClass;
        if(!next)return FALSE;
        current=next();
    }
    return TRUE;
}
// Symbol: ?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* self,const CRuntimeClass* base) {
    if(!self)impl__AfxThrowInvalidArgException__YAXXZ();
    const CRuntimeClass* type=self->GetRuntimeClass();
    if(!type)impl__AfxThrowInvalidArgException__YAXXZ();
    return impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(type,base);
}
