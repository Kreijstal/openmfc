// External application services for the separately linked string DLL fixture.
// String objects, managers, allocations, exceptions and RTTI use production owners.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/ModuleThreadStateSupport.h"
#include <cstdlib>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT) {
    return GetModuleHandleW(L"openmfc_string_types.dll");
}
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT) {std::abort();}
extern "C" int impl__AfxMessageBox__YAHIII_Z(UINT,UINT,UINT) {std::abort();}
// A failed process-heap allocation's application new-handler is outside this fixture.
extern "C" AFX_MODULE_THREAD_STATE* impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {std::abort();}
// Materialize the real template members for independently compiled SDK consumers.
template class ATL::CSimpleStringT<char,true>;
template class ATL::CSimpleStringT<wchar_t,true>;
