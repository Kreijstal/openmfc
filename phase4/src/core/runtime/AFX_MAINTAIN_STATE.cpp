// Native release-x64 one-word module-state scope (0x1330f0/0x133110).
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
extern "C" AFX_MODULE_STATE* AFXAPI
impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(AFX_MODULE_STATE*);
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
// Symbol: ??0AFX_MAINTAIN_STATE@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* AFXAPI
impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z(void* self, AFX_MODULE_STATE* module) {
    *static_cast<AFX_MODULE_STATE**>(self) =
        impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(module);
    return self;
}
// Symbol: ??1AFX_MAINTAIN_STATE@@QEAA@XZ
extern "C" void AFXAPI impl___1AFX_MAINTAIN_STATE__QEAA_XZ(void* self) {
    auto* thread = AfxGetThreadState();
    if (!thread)
        impl__AfxThrowInvalidArgException__YAXXZ();
    thread->m_pModuleState = *static_cast<AFX_MODULE_STATE**>(self);
}
