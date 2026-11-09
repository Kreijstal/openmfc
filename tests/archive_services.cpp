// Application resource and failed-allocation callbacks, outside archive storage.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/ModuleThreadStateSupport.h"
#include <cstdlib>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT){return GetModuleHandleW(nullptr);}
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT){std::abort();}
extern "C" int impl__AfxMessageBox__YAHIII_Z(UINT,UINT,UINT){std::abort();}
extern "C" AFX_MODULE_THREAD_STATE* impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ(){std::abort();}
template class ATL::CSimpleStringT<char,true>;
template class ATL::CSimpleStringT<wchar_t,true>;
