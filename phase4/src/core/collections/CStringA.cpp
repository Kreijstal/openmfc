// Native ANSI CString constructors, conversion and resource loading.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/StringResourceSupport.h"
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
namespace {
HINSTANCE FindModule(UINT id) noexcept {return static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(id));}
int LoadImage(CStringA& text,const WORD* image) {
    if(!image)return FALSE;
    int length=::WideCharToMultiByte(CP_THREAD_ACP,0,reinterpret_cast<const wchar_t*>(image+1),*image,nullptr,0,nullptr,nullptr);
    char* output=text.GetBuffer(length);
    ::WideCharToMultiByte(CP_THREAD_ACP,0,reinterpret_cast<const wchar_t*>(image+1),*image,output,length,nullptr,nullptr);
    text.ReleaseBufferSetLength(length);return TRUE;
}
}
CStringA::CStringA(const char* source):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CStringA::CStringA(const char* source,IAtlStringMgr* manager):CSimpleStringA(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CStringA::CStringA(const wchar_t* source):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CStringA::CStringA(const wchar_t* source,IAtlStringMgr* manager):CSimpleStringA(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CStringA::CStringA(const wchar_t* source,int length):CSimpleStringA(OpenMFC_GetStringMgr()) {Convert(source,length);}
CStringA::CStringA(const wchar_t* source,int length,IAtlStringMgr* manager):CSimpleStringA(manager) {Convert(source,length);}
void CStringA::Convert(const wchar_t* source,int length) {
    if(length<=0)return;
    if(!source)InvalidLength();
    int bytes=::WideCharToMultiByte(CP_THREAD_ACP,0,source,length,nullptr,0,nullptr,nullptr);
    char* out=GetBuffer(bytes);
    ::WideCharToMultiByte(CP_THREAD_ACP,0,source,length,out,bytes,nullptr,nullptr);
    SetLength(bytes);
}
CStringA& CStringA::operator=(const wchar_t* source) {
    int bytes=source?int32_t(uint32_t(::WideCharToMultiByte(CP_THREAD_ACP,0,source,-1,nullptr,0,nullptr,nullptr))-1):0;
    if(bytes>0) {
        char* out=GetBuffer(bytes);
        ::WideCharToMultiByte(CP_THREAD_ACP,0,source,-1,out,bytes,nullptr,nullptr);
        SetLength(bytes);
    } else Empty();
    return *this;
}
CStringA& CStringA::operator=(wchar_t value) {wchar_t text[]={value,0};return operator=(text);}
CStringA::CStringA(char value,int count):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(count>0){memset(GetBuffer(count),value,size_t(count));SetLength(count);}
}
CStringA::CStringA(wchar_t value,int count):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(count<=0)return;
    wchar_t text[]={value,0};
    int width=value?int32_t(uint32_t(::WideCharToMultiByte(CP_THREAD_ACP,0,text,-1,nullptr,0,nullptr,nullptr))-1):1;
    int capacity=int32_t(uint32_t(width)+1);char local[10]={};char* converted=local;
    size_t allocation=size_t(int64_t(capacity));
    if(allocation>sizeof(local)){converted=static_cast<char*>(malloc(allocation));if(!converted)AfxThrowMemoryException();}
    ::WideCharToMultiByte(CP_THREAD_ACP,0,text,1,converted,capacity,nullptr,nullptr);
    int length=int32_t(uint32_t(count)*uint32_t(width));
    try {
        char* out=GetBuffer(length);
        if(width==1)memset(out,converted[0],size_t(count));
        else for(int index=0;index<count;++index)for(int byte=0;byte<width;++byte)*out++=converted[byte];
        SetLength(length);
    } catch(...){if(converted!=local)free(converted);throw;}
    if(converted!=local)free(converted);
}
int CStringA::LoadString(UINT id) {auto module=FindModule(id);return module?LoadString(module,id):FALSE;}
int CStringA::LoadString(HINSTANCE module,UINT id) {return LoadImage(*this,openmfc::detail::StringResourceImage(module,id));}
int CStringA::LoadString(HINSTANCE module,UINT id,unsigned short language) {return LoadImage(*this,openmfc::detail::StringResourceImage(module,id,language));}
