// Native ANSI CString constructors, conversion and resource loading.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CStringOperations.h"
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
CStringA::CStringT(const char* source):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CStringA::CStringT(const char* source,IAtlStringMgr* manager):CSimpleStringA(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CStringA::CStringT(const wchar_t* source):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CStringA::CStringT(const wchar_t* source,IAtlStringMgr* manager):CSimpleStringA(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CStringA::CStringT(const wchar_t* source,int length):CSimpleStringA(OpenMFC_GetStringMgr()) {Convert(source,length);}
CStringA::CStringT(const wchar_t* source,int length,IAtlStringMgr* manager):CSimpleStringA(manager) {Convert(source,length);}
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
CStringA::CStringT(char value,int count):CSimpleStringA(OpenMFC_GetStringMgr()) {
    if(count>0){memset(GetBuffer(count),value,size_t(count));SetLength(count);}
}
CStringA::CStringT(wchar_t value,int count):CSimpleStringA(OpenMFC_GetStringMgr()) {
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
int CStringA::LoadStringW(UINT id) {auto module=FindModule(id);return module?LoadString(module,id):FALSE;}
int CStringA::LoadStringW(HINSTANCE module,UINT id) {return LoadImage(*this,openmfc::detail::StringResourceImage(module,id));}
int CStringA::LoadStringW(HINSTANCE module,UINT id,unsigned short language) {return LoadImage(*this,openmfc::detail::StringResourceImage(module,id,language));}

CStringA CStringA::Left(int count) const {return openmfc_string_ops::Left(*this,count);}
CStringA CStringA::Right(int count) const {return openmfc_string_ops::Right(*this,count);}
CStringA CStringA::Mid(int first) const {return openmfc_string_ops::Mid(*this,first);}
CStringA CStringA::Mid(int first,int count) const {return openmfc_string_ops::Mid(*this,first,count);}
CStringA CStringA::SpanIncluding(const char* set) const {return openmfc_string_ops::Span(*this,set,true);}
CStringA CStringA::SpanExcluding(const char* set) const {return openmfc_string_ops::Span(*this,set,false);}
CStringA CStringA::Tokenize(const char* delimiters,int& start) const {return openmfc_string_ops::Tokenize(*this,delimiters,start);}

// Native member exports place this before the explicit result pointer.
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@D$00@1@@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBV__CSimpleStringT_D_00_1__Z(CStringA* self,const CSimpleStringA* source) {*self+=*source;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_D_Z(CStringA* self,char value) {*self+=value;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@E@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_E_Z(CStringA* self,unsigned char value) {*self+=value;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(CStringA* self,const char* source) {*self+=source;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01__W_Z(CStringA* self,wchar_t value) {*self+=value;return self;}
// Symbol: ?GetManager@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* impl__GetManager___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CStringA* self) {return self->GetManager();}
// Symbol: ?Left@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CStringA* impl__Left___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(const CStringA* self,CStringA* result,int count) {return ::new(result) CStringA(self->Left(count));}
// Symbol: ?Mid@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CStringA* impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(const CStringA* self,CStringA* result,int first) {return ::new(result) CStringA(self->Mid(first));}
// Symbol: ?Mid@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@HH@Z
extern "C" CStringA* impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_HH_Z(const CStringA* self,CStringA* result,int first,int count) {return ::new(result) CStringA(self->Mid(first,count));}
// Symbol: ?Right@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CStringA* impl__Right___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_H_Z(const CStringA* self,CStringA* result,int count) {return ::new(result) CStringA(self->Right(count));}
// Symbol: ?SpanExcluding@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBD@Z
extern "C" CStringA* impl__SpanExcluding___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBD_Z(const CStringA* self,CStringA* result,const char* set) {return ::new(result) CStringA(self->SpanExcluding(set));}
// Symbol: ?SpanIncluding@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBD@Z
extern "C" CStringA* impl__SpanIncluding___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBD_Z(const CStringA* self,CStringA* result,const char* set) {return ::new(result) CStringA(self->SpanIncluding(set));}
// Symbol: ?Tokenize@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEBA?AV12@PEBDAEAH@Z
extern "C" CStringA* impl__Tokenize___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_PEBDAEAH_Z(const CStringA* self,CStringA* result,const char* delimiters,int* start) {return ::new(result) CStringA(self->Tokenize(delimiters,*start));}
