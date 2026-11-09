// Native CException owns distinct non-const/const message slots, then ReportError.
#define OPENMFC_APPCORE_IMPL
#include "detail/CsimpleexceptionRttiSupport.h"
int CException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) {
    return static_cast<const CException*>(this)->GetErrorMessage(out,capacity,help);
}
