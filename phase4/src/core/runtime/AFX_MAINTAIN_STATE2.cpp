// Native release-x64 module-state scope retaining its thread pointer (0x133170).
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
// Symbol: ??0AFX_MAINTAIN_STATE2@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* AFXAPI
impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* self, AFX_MODULE_STATE* module) {
    auto* thread = AfxGetThreadState();
    if (!thread)
        impl__AfxThrowInvalidArgException__YAXXZ();
    auto** fields = static_cast<void**>(self);
    fields[1] = thread;
    fields[0] = thread->m_pModuleState;
    thread->m_pModuleState = module;
    return self;
}
// Symbol: ??1AFX_MAINTAIN_STATE2@@QEAA@XZ
extern "C" void AFXAPI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* self) {
    auto** fields = static_cast<void**>(self);
    auto* thread = static_cast<_AFX_THREAD_STATE*>(fields[1]);
    if (thread)
        thread->m_pModuleState = static_cast<AFX_MODULE_STATE*>(fields[0]);
}
