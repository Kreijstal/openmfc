// Native Unicode CString constructors and ANSI-to-Unicode conversion.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CStringOperations.h"
#include "detail/CStringFormatSupport.h"
#include <new>
#include <cerrno>

CString::CStringT(const wchar_t* source):CStringT(source,OpenMFC_GetStringMgr()) {}
CString::CStringT(const wchar_t* source,IAtlStringMgr* manager):CSimpleStringW(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CString::CStringT(const char* source):CStringT(source,OpenMFC_GetStringMgr()) {}
CString::CStringT(const char* source,IAtlStringMgr* manager):CSimpleStringW(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CString::CStringT(const unsigned char* source):CStringT(source,OpenMFC_GetStringMgr()) {}
CString::CStringT(const unsigned char* source,IAtlStringMgr* manager):CSimpleStringW(manager) {operator=(reinterpret_cast<const char*>(source));}
CString::CStringT(const char* source,int length):CStringT(source,length,OpenMFC_GetStringMgr()) {}
CString::CStringT(const char* source,int length,IAtlStringMgr* manager):CSimpleStringW(manager) {Convert(source,length);}
void CString::Convert(const char* source,int length) {
    if(length<=0)return;
    if(!source)InvalidLength();
    int characters=::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,nullptr,0);
    wchar_t* out=GetBuffer(characters);
    ::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,out,characters);
    SetLength(characters);
}
CString& CString::operator=(const char* source) {
    int characters=source?int32_t(uint32_t(::MultiByteToWideChar(CP_THREAD_ACP,0,source,-1,nullptr,0))-1):0;
    if(characters>0) {
        wchar_t* out=GetBuffer(characters);
        ::MultiByteToWideChar(CP_THREAD_ACP,0,source,-1,out,characters);
        SetLength(characters);
    } else Empty();
    return *this;
}
CString::CStringT(wchar_t value,int count):CSimpleStringW(OpenMFC_GetStringMgr()) {
    if(count<=0)return;
    wchar_t text[]={value,0};
    size_t width=value?wcslen(text):1;
    size_t capacity=size_t(int64_t(int32_t(uint32_t(width)+1)));
    if(capacity>SIZE_MAX/sizeof(wchar_t))AfxThrowOleException(static_cast<long>(0x80070216u));
    wchar_t local[5]={};wchar_t* converted=local;
    if(capacity*sizeof(wchar_t)>sizeof(local)) {
        converted=static_cast<wchar_t*>(malloc(capacity*sizeof(wchar_t)));
        if(!converted)AfxThrowMemoryException();
    }
    try {
        int result=wmemcpy_s(converted,capacity,text,1);
        if(result && result!=STRUNCATE) {
            if(result==ENOMEM)AfxThrowMemoryException();else AfxThrowInvalidArgException();
        }
        int length=int32_t(uint32_t(count)*uint32_t(width));
        wchar_t* out=GetBuffer(length);
        if(width==1)wmemset(out,converted[0],size_t(count));
        else for(int index=0;index<count;++index)for(int at=0;at<int(width);++at)*out++=converted[at];
        SetLength(length);
    } catch(...){if(converted!=local)free(converted);throw;}
    if(converted!=local)free(converted);
}
CString::CStringT(char value,int count):CSimpleStringW(OpenMFC_GetStringMgr()) {
    if(count>0){wmemset(GetBuffer(count),wchar_t(static_cast<signed char>(value)),size_t(count));SetLength(count);}
}

// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(CString* self,const char* text) {*self=text;return self;}
// Symbol: ??1?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" void impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self) {self->~CString();}
// Symbol: ??1?$CSimpleStringT@_W$00@ATL@@QEAA@XZ
extern "C" void impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ(CSimpleStringW* self) {self->~CSimpleStringW();}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@_WH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA__WH_Z(CString* self,wchar_t value,int count) {return ::new(self) CString(value,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self) {return ::new(self) CString();}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,int count,IAtlStringMgr* manager) {return ::new(self) CString(text,count,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WH_Z(CString* self,const wchar_t* text,int count) {return ::new(self) CString(text,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_W@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_W_Z(CString* self,const wchar_t* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBEPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(CString* self,const unsigned char* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBE@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBE_Z(CString* self,const unsigned char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CString* self,const char* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDHPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CString* self,const char* text,int count,IAtlStringMgr* manager) {return ::new(self) CString(text,count,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDH_Z(CString* self,const char* text,int count) {return ::new(self) CString(text,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBD@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBD_Z(CString* self,const char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEA_W@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEA_W_Z(CString* self,wchar_t* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(CString* self,IAtlStringMgr* manager) {return ::new(self) CString(manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAE@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z(CString* self,unsigned char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAD@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAD_Z(CString* self,char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@DH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_DH_Z(CString* self,char value,int count) {return ::new(self) CString(value,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@AEBV01@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBV01__Z(CString* self,const CString* source) {return ::new(self) CString(*source);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(text,manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,int count,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(text,count,manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(CSimpleStringW* self,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const ATL::CSimpleStringT<wchar_t,false>* source) {return ::new(self) CSimpleStringW(*source);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV01@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source) {return ::new(self) CSimpleStringW(*source);}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(CString* self,wchar_t source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CString* self,const wchar_t* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBE@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBE_Z(CString* self,const unsigned char* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(CString* self,char source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(CString* self,const CString* source) {*self=*source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(CSimpleStringW* self,const wchar_t* source) {*self=source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const ATL::CSimpleStringT<wchar_t,false>* source) {*self=*source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source) {*self=*source;return self;}

CString CString::Left(int count) const {return openmfc_string_ops::Left(*this,count);}
CString CString::Right(int count) const {return openmfc_string_ops::Right(*this,count);}
CString CString::Mid(int first) const {return openmfc_string_ops::Mid(*this,first);}
CString CString::Mid(int first,int count) const {return openmfc_string_ops::Mid(*this,first,count);}
CString CString::SpanIncluding(const wchar_t* set) const {return openmfc_string_ops::Span(*this,set,true);}
CString CString::SpanExcluding(const wchar_t* set) const {return openmfc_string_ops::Span(*this,set,false);}
CString CString::Tokenize(const wchar_t* delimiters,int& start) const {return openmfc_string_ops::Tokenize(*this,delimiters,start);}
CString& CString::operator+=(const char* source) {CString converted(source,GetManager());Append(converted);return *this;}

// Native member exports place this before the explicit result pointer.
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@_W$00@1@@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_00_1__Z(CString* self,const CSimpleStringW* source) {*self+=*source;return self;}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(CString* self,char value) {*self+=value;return self;}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@E@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_E_Z(CString* self,unsigned char value) {*self+=value;return self;}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(CString* self,const char* source) {*self+=source;return self;}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CString* self,const wchar_t* source) {*self+=source;return self;}
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(CString* self,wchar_t value) {*self+=value;return self;}
// Symbol: ?GetManager@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* impl__GetManager___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CString* self) {return self->GetManager();}
// Symbol: ?Left@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CString* impl__Left___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(const CString* self,CString* result,int count) {return ::new(result) CString(self->Left(count));}
// Symbol: ?Mid@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CString* impl__Mid___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(const CString* self,CString* result,int first) {return ::new(result) CString(self->Mid(first));}
// Symbol: ?Mid@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@HH@Z
extern "C" CString* impl__Mid___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_HH_Z(const CString* self,CString* result,int first,int count) {return ::new(result) CString(self->Mid(first,count));}
// Symbol: ?Right@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@H@Z
extern "C" CString* impl__Right___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_H_Z(const CString* self,CString* result,int count) {return ::new(result) CString(self->Right(count));}
// Symbol: ?SpanExcluding@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_W@Z
extern "C" CString* impl__SpanExcluding___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_W_Z(const CString* self,CString* result,const wchar_t* set) {return ::new(result) CString(self->SpanExcluding(set));}
// Symbol: ?SpanIncluding@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_W@Z
extern "C" CString* impl__SpanIncluding___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_W_Z(const CString* self,CString* result,const wchar_t* set) {return ::new(result) CString(self->SpanIncluding(set));}
// Symbol: ?Tokenize@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBA?AV12@PEB_WAEAH@Z
extern "C" CString* impl__Tokenize___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBA_AV12_PEB_WAEAH_Z(const CString* self,CString* result,const wchar_t* delimiters,int* start) {return ::new(result) CString(self->Tokenize(delimiters,*start));}

int CString::Compare(const wchar_t* text) const {return openmfc_string_ops::Compare(*this,text,0);}
int CString::CompareNoCase(const wchar_t* text) const {return openmfc_string_ops::Compare(*this,text,1);}
int CString::Collate(const wchar_t* text) const {return openmfc_string_ops::Compare(*this,text,2);}
int CString::CollateNoCase(const wchar_t* text) const {return openmfc_string_ops::Compare(*this,text,3);}
int CString::Find(wchar_t value,int start) const {return openmfc_string_ops::Find(*this,value,start);}
int CString::Find(const wchar_t* text,int start) const {return openmfc_string_ops::Find(*this,text,start);}
int CString::FindOneOf(const wchar_t* set) const {return openmfc_string_ops::FindOneOf(*this,set);}
int CString::ReverseFind(wchar_t value) const {return openmfc_string_ops::ReverseFind(*this,value);}
CString& CString::MakeLower() {return openmfc_string_ops::ChangeCase(*this,0);}
CString& CString::MakeUpper() {return openmfc_string_ops::ChangeCase(*this,1);}
CString& CString::MakeReverse() {return openmfc_string_ops::ChangeCase(*this,2);}

// Symbol: ?Find@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAH_WH@Z
extern "C" int impl__Find___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAH_WH_Z(const CString* self,wchar_t value,int start) {return self->Find(value,start);}
// Symbol: ?Find@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_WH@Z
extern "C" int impl__Find___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_WH_Z(const CString* self,const wchar_t* text,int start) {return self->Find(text,start);}
// Symbol: ?FindOneOf@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int impl__FindOneOf___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* self,const wchar_t* text) {return self->FindOneOf(text);}
// Symbol: ?MakeLower@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* impl__MakeLower___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* self) {return &self->MakeLower();}
// Symbol: ?MakeReverse@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* impl__MakeReverse___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* self) {return &self->MakeReverse();}
// Symbol: ?MakeUpper@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* self) {return &self->MakeUpper();}
// Symbol: ?Compare@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int impl__Compare___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* self,const wchar_t* text) {return self->Compare(text);}
// Symbol: ?CompareNoCase@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int impl__CompareNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* self,const wchar_t* text) {return self->CompareNoCase(text);}
// Symbol: ?Collate@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int impl__Collate___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* self,const wchar_t* text) {return self->Collate(text);}
// Symbol: ?CollateNoCase@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int impl__CollateNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* self,const wchar_t* text) {return self->CollateNoCase(text);}
// Symbol: ?ReverseFind@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAH_W@Z
extern "C" int impl__ReverseFind___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAH_W_Z(const CString* self,wchar_t value) {return self->ReverseFind(value);}

#pragma push_macro("FormatMessage")
#undef FormatMessage
void CString::FormatV(const wchar_t* format,va_list args) {openmfc_string_format::Printf(*this,format,args,false);}
void CString::AppendFormatV(const wchar_t* format,va_list args) {openmfc_string_format::Printf(*this,format,args,true);}
void CString::FormatMessageV(const wchar_t* format,va_list* args) {openmfc_string_format::Message(*this,format,args);}
void CString::Format(const wchar_t* format,...) {va_list args;va_start(args,format);FormatV(format,args);va_end(args);}
void CString::Format(UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::Resource(*this,id,args,false);va_end(args);}
void CString::AppendFormat(const wchar_t* format,...) {va_list args;va_start(args,format);AppendFormatV(format,args);va_end(args);}
void CString::AppendFormat(UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::Resource(*this,id,args,true);va_end(args);}
void CString::FormatMessage(const wchar_t* format,...) {if(!format)AfxThrowOleException(static_cast<long>(0x80070057u));va_list args;va_start(args,format);FormatMessageV(format,&args);va_end(args);}
void CString::FormatMessage(UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::MessageResource(*this,id,&args);va_end(args);}
void CString::FormatMessageW(const wchar_t* format,...) {if(!format)AfxThrowOleException(static_cast<long>(0x80070057u));va_list args;va_start(args,format);FormatMessageV(format,&args);va_end(args);}
void CString::FormatMessageW(UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::MessageResource(*this,id,&args);va_end(args);}
#pragma pop_macro("FormatMessage")

// Symbol: ?FormatV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAD@Z
extern "C" void impl__FormatV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAD_Z(CString* self,const wchar_t* format,char* args) {self->FormatV(format,args);}
// Symbol: ?Format@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* self,const wchar_t* format,...) {va_list args;va_start(args,format);self->FormatV(format,args);va_end(args);}
// Symbol: ?AppendFormat@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void impl__AppendFormat___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(CString* self,UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::Resource(*self,id,args,true);va_end(args);}
// Symbol: ?AppendFormat@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void impl__AppendFormat___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* self,const wchar_t* format,...) {va_list args;va_start(args,format);self->AppendFormatV(format,args);va_end(args);}
// Symbol: ?AppendFormatV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAD@Z
extern "C" void impl__AppendFormatV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAD_Z(CString* self,const wchar_t* format,char* args) {self->AppendFormatV(format,args);}
// Symbol: ?Format@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(CString* self,UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::Resource(*self,id,args,false);va_end(args);}
// Symbol: ?FormatMessage@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void impl__FormatMessage___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(CString* self,UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::MessageResource(*self,id,&args);va_end(args);}
// Symbol: ?FormatMessage@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void impl__FormatMessage___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* self,const wchar_t* format,...) {if(!format)AfxThrowOleException(static_cast<long>(0x80070057u));va_list args;va_start(args,format);self->FormatMessageV(format,&args);va_end(args);}
// Symbol: ?FormatMessageV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAPEAD@Z
extern "C" void impl__FormatMessageV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAPEAD_Z(CString* self,const wchar_t* format,char** args) {self->FormatMessageV(format,args);}
// Symbol: ?FormatMessageW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXIZZ
extern "C" void impl__FormatMessageW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXIZZ(CString* self,UINT id,...) {va_list args;va_start(args,id);openmfc_string_format::MessageResource(*self,id,&args);va_end(args);}
// Symbol: ?FormatMessageW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void impl__FormatMessageW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* self,const wchar_t* format,...) {if(!format)AfxThrowOleException(static_cast<long>(0x80070057u));va_list args;va_start(args,format);self->FormatMessageV(format,&args);va_end(args);}
