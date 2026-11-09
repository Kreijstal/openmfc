// Native length-prefixed RT_STRING loading used by resource-critical exceptions.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cstring>
#include <algorithm>
#include "detail/StringResourceSupport.h"
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
// Symbol: ?AfxLoadString@@YAHIPEA_WI@Z
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT id,wchar_t* output,UINT capacity) {
    if (!output || !capacity) AfxThrowInvalidArgException();
    auto module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(id));
    if (!module) {*output=0;return 0;}
    const WORD* image=openmfc::detail::StringResourceImage(module,id);
    UINT size=(std::min)(capacity-1,UINT(*image));
    if (size && memcpy_s(output,size_t(capacity-1)*2,image+1,size_t(size)*2)) AfxThrowInvalidArgException();
    output[size]=0;
    return size;
}
