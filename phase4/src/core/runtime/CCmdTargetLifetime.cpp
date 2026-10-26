// Native NOVTABLE base construction and embedded-dispatch shutdown.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
CCmdTarget::~CCmdTarget() {
    if (m_xDispatch) {
        using Disconnect = void (AFXAPI*)(void*);
        auto** table = reinterpret_cast<void**>(m_xDispatch);
        reinterpret_cast<Disconnect>(table[7])(&m_xDispatch);
    }
    m_pModuleState = nullptr;
}
// Symbol: ??0CCmdTarget@@QEAA@XZ
extern "C" CCmdTarget* AFXAPI impl___0CCmdTarget__QEAA_XZ(CCmdTarget* self) {
    return ::new(self) CCmdTarget;
}
// Symbol: ??1CCmdTarget@@UEAA@XZ
extern "C" void AFXAPI impl___1CCmdTarget__UEAA_XZ(CCmdTarget* self) {
    self->CCmdTarget::~CCmdTarget();
}
