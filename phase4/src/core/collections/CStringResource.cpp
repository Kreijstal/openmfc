// Native Unicode CString resource loading preserves the destination on misses.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/StringResourceSupport.h"
#include <cerrno>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
namespace {
HINSTANCE FindModule(UINT id) noexcept {
    return static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(id));
}
int LoadImage(CString& text,const WORD* image) {
    if(!image)return FALSE;
    int length=*image;
    wchar_t* buffer=text.GetBuffer(length);
    int result=wmemcpy_s(buffer,length,reinterpret_cast<const wchar_t*>(image+1),*image);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
    text.ReleaseBufferSetLength(length);
    return TRUE;
}
}
int CString::LoadString(UINT id) {
    auto module=FindModule(id);
    return module?LoadString(module,id):FALSE;
}
int CString::LoadString(HINSTANCE module,UINT id) {
    return LoadImage(*this,openmfc::detail::StringResourceImage(module,id));
}
int CString::LoadString(HINSTANCE module,UINT id,unsigned short language) {
    return LoadImage(*this,openmfc::detail::StringResourceImage(module,id,language));
}
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHI@Z
extern "C" int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* self,UINT id) {return self->LoadString(id);}
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@I@Z
extern "C" int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* self,HINSTANCE module,UINT id) {return self->LoadString(module,id);}
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@IG@Z
extern "C" int impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(CString* self,HINSTANCE module,UINT id,unsigned short language) {return self->LoadString(module,id,language);}
