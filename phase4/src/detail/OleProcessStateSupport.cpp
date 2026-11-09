#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include "OleProcessStateSupport.h"
#include "CProcessLocalObjectSupport.h"
extern "C" void* impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(void*,CreateFn);
extern "C" void impl___1CProcessLocalObject__QEAA_XZ(void*);
namespace {
S_Cprocesslocalobject processState{};
void* CreateOleState() { return new _AFX_OLE_STATE; }
struct ProcessStateCleanup {
    ~ProcessStateCleanup() { impl___1CProcessLocalObject__QEAA_XZ(&processState); }
} cleanup;
}
namespace openmfc { namespace detail {
_AFX_OLE_STATE* GetOleProcessState() {
    auto* state=static_cast<_AFX_OLE_STATE*>(impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(&processState,CreateOleState));
    if (!state) AfxThrowInvalidArgException();
    return state;
}
} }
