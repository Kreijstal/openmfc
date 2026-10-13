// Native CException owns distinct non-const/const message slots, then ReportError.
#define OPENMFC_APPCORE_IMPL
#include "detail/CsimpleexceptionRttiSupport.h"
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException*,UINT,UINT);
int CException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) {
    return static_cast<const CException*>(this)->GetErrorMessage(out,capacity,help);
}
int CException::ReportError(UINT type,UINT message) {
    return impl__ReportError_CException__UEAAHII_Z(this,type,message);
}
