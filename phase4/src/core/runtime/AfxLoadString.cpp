// Native length-prefixed RT_STRING loading used by resource-critical exceptions.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cstring>
#include <algorithm>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
namespace {
const WORD* StringImage(HINSTANCE module,UINT id) {
    HRSRC resource=FindResourceW(module,MAKEINTRESOURCEW(WORD((id>>4)+1)),RT_STRING);
    if (!resource) return nullptr;
    HGLOBAL loaded=LoadResource(module,resource);
    if (!loaded) return nullptr;
    auto* cursor=static_cast<const WORD*>(LockResource(loaded));
    if (!cursor) return nullptr;
    const BYTE* end=reinterpret_cast<const BYTE*>(cursor)+SizeofResource(module,resource);
    for (UINT index=id&15;index;--index) {
        if (reinterpret_cast<const BYTE*>(cursor)>=end) return nullptr;
        cursor+=1+*cursor;
    }
    if (reinterpret_cast<const BYTE*>(cursor)>=end || !*cursor) return nullptr;
    return cursor;
}
}
// Symbol: ?AfxLoadString@@YAHIPEA_WI@Z
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT id,wchar_t* output,UINT capacity) {
    if (!output || !capacity) AfxThrowInvalidArgException();
    auto module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(id));
    if (!module) {*output=0;return 0;}
    const WORD* image=StringImage(module,id);
    UINT size=(std::min)(capacity-1,UINT(*image));
    if (size && memcpy_s(output,size_t(capacity-1)*2,image+1,size_t(size)*2)) AfxThrowInvalidArgException();
    output[size]=0;
    return size;
}
